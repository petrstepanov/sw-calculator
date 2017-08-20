/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParabolaGaussModelProvider.cpp
 * Author: petrstepanov
 * 
 * Created on August 2, 2017, 5:21 AM
 */

#include "ParabolaGaussModelProvider.h"
#include "ParabolaPdf.h"
#include <RooGaussian.h>
#include <RooFormulaVar.h>
#include <RooAddPdf.h>
#include <RooFFTConvPdf.h>
#include <RooGenericPdf.h>

ParabolaGaussModelProvider::ParabolaGaussModelProvider(RooRealVar* x, RooRealVar* x0, Bool_t hasAtan = kTRUE, Double_t constBgFraction = 0, const Int_t numGauss = 1, Bool_t hasParabola = kFALSE) : AbstractModelProvider(x0){
	RooArgList* pdfList = new RooArgList();
	RooArgList* coeffList = new RooArgList();

	// Define parabola
	parabolaRoot=NULL;
	if (hasParabola){
		parabolaRoot = new RooRealVar("parabola_root", "Coefficient at -x^2 + r*2", 2, 0.1, 10);
		ParabolaPdf* parabola = new ParabolaPdf("parabola", "parabola", *x, *x0, *parabolaRoot);
		RooRealVar* I_parabola = new RooRealVar("parabola_coeff", "Parabola intensity", 0.9, 0.0, 1.0);
		pdfList->add(*parabola);
		coeffList->add(*I_parabola);
		components->add(*parabola);
	}

	// Gauss PDFs
	RooRealVar** FWHM = new RooRealVar*[numGauss];
	RooFormulaVar** sigma = new RooFormulaVar*[numGauss];
	RooGaussian** gauss = new RooGaussian*[numGauss];
	RooRealVar** I_gauss = new RooRealVar*[numGauss];

	Double_t fwhmMin = 1.; // [KeV]
	Double_t fwhmMax = 30.; // [KeV]
	Double_t fwhmStep = (fwhmMax - fwhmMin) / (numGauss + 1);
	for (int i = 0; i < numGauss; i++){
		FWHM[i] = new RooRealVar(TString::Format("gauss%d_FWHM", i + 1), TString::Format("Gauss%d FWHM", i + 1), fwhmMin + (i + 1)*fwhmStep, fwhmMin, fwhmMax, "keV"); // 5 0.5 10
		sigma[i] = new RooFormulaVar(TString::Format("gauss%d_sigma", i + 1), "@0*@1", RooArgList(*FWHM[i], *fwhm2sigma));
		gauss[i] = new RooGaussian(TString::Format("gauss%d", i + 1), TString::Format("Gauss%d PDF", i + 1), *x, *x0, *sigma[i]);
		// For non-recursive sum
		//            I_gauss[i] = new RooRealVar(TString::Format("gauss%d_coeff", i + 1), TString::Format("Gauss%d intensity", i + 1), 0.1/numGauss, 0.0, 1.0);
		// For recursive sum
		I_gauss[i] = new RooRealVar(TString::Format("gauss%d_coeff", i + 1), TString::Format("Gauss%d intensity", i + 1), 0.9, 0.0, 1.0);
		pdfList->add(*gauss[i]);
		components->add(*gauss[i]);
	}
	for (int i = 0; i < numGauss - 1; i++){
		coeffList->add(*I_gauss[i]);
	}

	// If we sum values recursively than bg~1E-5, parabola~0.9, gauss coefficients~0.9 (needs kTRUE)
	RooAddPdf* sumModel = new RooAddPdf("sumModel", "Sum of components", *pdfList, *coeffList, kTRUE);
	this->model = sumModel;

	// Resolution Function
	RooRealVar* zero = new RooRealVar("zero", "zero", 0);
	RooRealVar* resFunct_FWHM = new RooRealVar("resFunct_FWHM", "Resolution function FWHM", 1.5, 1E-3, 3, "keV");
	RooFormulaVar* resFunct_dispersion = new RooFormulaVar("resFunct_dispersion", "@0*@1", RooArgList(*resFunct_FWHM, *fwhm2sigma));
	resolutionFunction = new RooGaussian("resFunct", "Resolution Function", *x, *zero, *resFunct_dispersion);

	// Convolution
	x->setBins(convolutionPoints, "cache");
	RooFFTConvPdf* sumModelConvoluted = new RooFFTConvPdf("sumModelConvoluted", "Convoluted Model", *x, *sumModel, *resolutionFunction);
	sumModelConvoluted->setBufferFraction(1);

	// Constant Background
	//	RooPolynomial* constBg = new RooPolynomial("constBg", "y=1", *x, RooArgSet());
	RooGenericPdf* constBg = new RooGenericPdf("constBg", "1", *x);
	RooRealVar* I_const = new RooRealVar("I_const", "Constant background", constBgFraction, constBgFraction / 100, constBgFraction * 10);
	bgComponents->add(*constBg);

	// Atan background
	if (hasAtan){
		RooGenericPdf* atanBg;
		RooRealVar* I_atan;
		atanBg = new RooGenericPdf("atanBg", "@2/2 + (-1)*atan((@0 - @1))", RooArgList(*x, *x0, *pi));
		I_atan = new RooRealVar("I_atan", "Atan Intensity", 0.002, 0.0, 0.1);
		bgComponents->add(*atanBg);
		this->convolutedModel = new RooAddPdf("model_atan", "Model with Convolution and Atan Background", RooArgList(*constBg, *atanBg, *sumModelConvoluted), RooArgList(*I_const, *I_atan));
	}
	else {
		this->convolutedModel = new RooAddPdf("model_const", "Model with Convolution and Const Background", RooArgList(*constBg, *sumModelConvoluted), RooArgList(*I_const));
	}
}

RooRealVar* ParabolaGaussModelProvider::getParabolaRoot(){
	return parabolaRoot;
}