/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParabolaLorentzianModelProvider.cpp
 * Author: petrstepanov
 * 
 * Created on August 2, 2017, 11:48 PM
 */

#include "ParabolaLorentzianModelProvider.h"
#include "ParabolaPdf.h"
#include "LorentzianPdf.h"
#include <RooGaussian.h>
#include <RooFormulaVar.h>
#include <RooAddPdf.h>
#include <RooFFTConvPdf.h>
#include <RooGenericPdf.h>

ParabolaLorentzianModelProvider::ParabolaLorentzianModelProvider(RooRealVar* x, RooRealVar* x0, Bool_t hasAtan = kTRUE, Double_t constBgFraction = 0, const Int_t numLorentz = 1, Bool_t hasParabola = kFALSE) : AbstractModelProvider(E_0){
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
	// RooRealVar** E_0 = new RooRealVar*[numLorentz];
	epsilon = new RooRealVar*[numLorentz];
	LorentzianPdf** lorentz = new LorentzianPdf*[numLorentz];
	RooRealVar** I_lorentz = new RooRealVar*[numLorentz];

	Double_t epsilonMin = 0.2; // [KeV]
	Double_t epsilonMax = 20; // [KeV]
	Double_t epsilonStep = (epsilonMax - epsilonMin) / (numLorentz + 1);
	for (int i = 0; i < numLorentz; i++){
		epsilon[i] = new RooRealVar(TString::Format("l%dStretch", i + 1), TString::Format("Lorentz%d stretch", i + 1), epsilonMin + (i + 1)*epsilonStep, epsilonMin, epsilonMax, "1/keV"); // 5 0.5 10
		lorentz[i] = new LorentzianPdf(TString::Format("lorentz%d", i + 1), TString::Format("Lorentz%d PDF", i + 1), *x, *x0, *epsilon[i]);
		// For non-recursive sum
		//            I_lorentz[i] = new RooRealVar(TString::Format("lorentz%d_coeff", i + 1), TString::Format("Gauss%d intensity", i + 1), 0.1/numLorentz, 0.0, 1.0);
		// For recursive sum
		I_lorentz[i] = new RooRealVar(TString::Format("l1%dInt", i + 1), TString::Format("Lorentz%d intensity", i + 1), 0.5, 0.0, 1.0);
		pdfList->add(*lorentz[i]);
		components->add(*lorentz[i]);
	}
	for (int i = 0; i < numLorentz - 1; i++){
		coeffList->add(*I_lorentz[i]);
	}

	// If we sum values recursively than bg~1E-5, parabola~0.9, lorentz coefficients~0.9 (needs kTRUE)
	RooAddPdf* sumModel = new RooAddPdf("sumModel", "Sum of components", *pdfList, *coeffList, kTRUE);
	this->model = sumModel;

	// Resolution Function
	RooRealVar* zero = new RooRealVar("zero", "zero", 0);
//	RooRealVar* resFunct_FWHM = new RooRealVar("resFunctFWHM", "Resolution function FWHM", 1.5, 1E-3, 3, "keV");
	RooRealVar* resFunct_FWHM = new RooRealVar("resFunctFWHM", "Resolution function FWHM", 3, 3, 3, "keV");
	RooFormulaVar* resFunct_dispersion = new RooFormulaVar("resFunctDispersion", "@0*@1", RooArgList(*resFunct_FWHM, *fwhm2sigma));
	resolutionFunction = new RooGaussian("resFunct", "Resolution Function", *x, *zero, *resFunct_dispersion);

	// Convolution
	x->setBins(convolutionPoints, "cache");
	RooFFTConvPdf* sumModelConvoluted = new RooFFTConvPdf("sumModelConvoluted", "Convoluted Model", *x, *sumModel, *resolutionFunction);
	sumModelConvoluted->setBufferFraction(1);

	// Constant Background
	//	RooPolynomial* constBg = new RooPolynomial("constBg", "y=1", *x, RooArgSet());
	RooGenericPdf* constBg = new RooGenericPdf("constBg", "1", *x);
	RooRealVar* I_const = new RooRealVar("IConst", "Constant background", constBgFraction, 0, constBgFraction*2);
	bgComponents->add(*constBg);

	// Atan background
	if (hasAtan){
		RooGenericPdf* atanBg;
		RooRealVar* I_atan;
		atanBg = new RooGenericPdf("atanBg", "@2/2 + (-1)*atan((@0 - @1))", RooArgList(*x, *x0, *pi));
		I_atan = new RooRealVar("IAtan", "Atan Intensity", 0.002, 0.0, 0.1);
		bgComponents->add(*atanBg);
		this->convolutedModel = new RooAddPdf("model_atan", "Model with Convolution and Atan Background", RooArgList(*constBg, *atanBg, *sumModelConvoluted), RooArgList(*I_const, *I_atan));
	}
	else {
		this->convolutedModel = new RooAddPdf("model_const", "Model with Convolution and Const Background", RooArgList(*constBg, *sumModelConvoluted), RooArgList(*I_const));
	}
}

RooRealVar* ParabolaLorentzianModelProvider::getParabolaRoot(){
	return parabolaRoot;
}

RooRealVar** ParabolaLorentzianModelProvider::getLorentzianCoefficients(){
    return epsilon;
}