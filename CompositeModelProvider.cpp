/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CompositeModelProvider.cpp
 * Author: petrstepanov
 * 
 * Created on August 15, 2017, 9:59 PM
 */

#include "CompositeModelProvider.h"
#include "ParabolaPdf.h"
#include "GaussianPdf.h"
#include "LorentzianPdf.h"
#include "DampLorentzPdf.h"
#include "OrthogonalPdf.h"
#include <RooFormulaVar.h>
#include <RooAddPdf.h>
#include <RooFFTConvPdf.h>
#include <RooGenericPdf.h>
#include <TIterator.h>

CompositeModelProvider::CompositeModelProvider(RooRealVar* x, RooRealVar* x0, Bool_t hasParabola, const Int_t numGauss, const Int_t numLorentz, const Int_t numLorentzSum, Bool_t hasOrthogonal, Bool_t hasAtan, Double_t constBgFraction, Bool_t isTwoDetector) : AbstractModelProvider(x0){
	pdfList = new RooArgList();
	coeffList = new RooArgList();
        this->isTwoDetector = isTwoDetector;
        
	// Define parabola
	if (hasParabola){
		RooRealVar* parabolaRoot = new RooRealVar("parabolaRoot", "Coefficient at -x^2 + r*2", 2, 0.1, 10);
		ParabolaPdf* parabola = new ParabolaPdf("parabola", "parabola", *x, *x0, *parabolaRoot);
		RooRealVar* I_parabola = new RooRealVar("parabolaCoeff", "Parabola intensity", 0.5, 0.0, 1.0);
		pdfList->add(*parabola);
		coeffList->add(*I_parabola);
		components->add(*parabola);
	}

	// Gauss PDFs
	RooRealVar** FWHM = new RooRealVar*[numGauss];
	RooFormulaVar** sigma = new RooFormulaVar*[numGauss];
	GaussianPdf** gauss = new GaussianPdf*[numGauss];
	RooRealVar** I_gauss = new RooRealVar*[numGauss];
	Double_t fwhmMin = 10.; // [KeV]
	Double_t fwhmMax = 50; // [KeV]
	Double_t fwhmStep = (fwhmMax - fwhmMin) / (numGauss + 1);
	for (int i = 0; i < numGauss; i++){
		FWHM[i] = new RooRealVar(TString::Format("gauss%d_FWHM", i + 1), TString::Format("Gauss%d FWHM", i + 1), fwhmMin + (i + 1)*fwhmStep, fwhmMin, fwhmMax, "keV"); // 5 0.5 10
		sigma[i] = new RooFormulaVar(TString::Format("gauss%d_sigma", i + 1), "@0*@1", RooArgList(*FWHM[i], *fwhm2sigma));
		gauss[i] = new GaussianPdf(TString::Format("gauss%d", i + 1), TString::Format("Gauss%d PDF", i + 1), *x, *x0, *sigma[i]);
//		gauss[i]->setParameter(sigma[i]);
                // For non-recursive sum
		//            I_gauss[i] = new RooRealVar(TString::Format("gauss%d_coeff", i + 1), TString::Format("Gauss%d intensity", i + 1), 0.1/numGauss, 0.0, 1.0);
		// For recursive sum
		I_gauss[i] = new RooRealVar(TString::Format("gauss%d_coeff", i + 1), TString::Format("Gauss%d intensity", i + 1), 0.3, 0.0, 1.0);
		pdfList->add(*gauss[i]);
		components->add(*gauss[i]);
                coeffList->add(*I_gauss[i]);
	}
//	for (int i = 0; i < numGauss - 1; i++){
//		coeffList->add(*I_gauss[i]);
//	}       
        
	// Lorentz PDFs
	RooRealVar** epsilon = new RooRealVar*[numLorentz];
	LorentzianPdf** lorentz = new LorentzianPdf*[numLorentz];
	RooRealVar** I_lorentz = new RooRealVar*[numLorentz];

	Double_t epsilonMin = 2; // [KeV]
	Double_t epsilonMax = 50; // [KeV]
	Double_t epsilonStep = (epsilonMax - epsilonMin) / (numLorentz + 1);
	for (int i = 0; i < numLorentz; i++){
		epsilon[i] = new RooRealVar(TString::Format("l%dStretch", i + 1), TString::Format("Lorentz%d stretch", i + 1), epsilonMin + (i + 1)*epsilonStep, epsilonMin, epsilonMax, "1/keV"); // 5 0.5 10
		lorentz[i] = new LorentzianPdf(TString::Format("lorentz%d", i + 1), TString::Format("Lorentz%d PDF", i + 1), *x, *x0, *epsilon[i]);
		// For non-recursive sum
		//            I_lorentz[i] = new RooRealVar(TString::Format("lorentz%d_coeff", i + 1), TString::Format("Gauss%d intensity", i + 1), 0.1/numLorentz, 0.0, 1.0);
		// For recursive sum
		I_lorentz[i] = new RooRealVar(TString::Format("l1%dInt", i + 1), TString::Format("Lorentz%d intensity", i + 1), 0.3, 0.0, 1.0);
		pdfList->add(*lorentz[i]);
		components->add(*lorentz[i]);
                coeffList->add(*I_lorentz[i]);
	}
        
	// Complex Lorentz PDFs
	RooRealVar** epsilon2 = new RooRealVar*[numLorentzSum];
	DampLorentzPdf** lorentz2 = new DampLorentzPdf*[numLorentzSum];
	RooRealVar** I_lorentz2 = new RooRealVar*[numLorentzSum];

	Double_t epsilon2Min = 0.2; // [KeV]
	Double_t epsilon2Max = 50; // [KeV]
	Double_t epsilon2Step = (epsilonMax - epsilonMin) / (numLorentzSum + 1);
	for (int i = 0; i < numLorentzSum; i++){
		epsilon2[i] = new RooRealVar(TString::Format("sl%dStretch", i + 1), TString::Format("Sum Lorentz%d stretch", i + 1), epsilon2Min + (i + 1)*epsilon2Step, epsilon2Min, epsilon2Max, "1/keV"); // 5 0.5 10
		lorentz2[i] = new DampLorentzPdf(TString::Format("slorentz%d", i + 1), TString::Format("Sum Lorentz%d PDF", i + 1), *x, *x0, *epsilon2[i]);
		// For non-recursive sum
		//            I_lorentz[i] = new RooRealVar(TString::Format("lorentz%d_coeff", i + 1), TString::Format("Gauss%d intensity", i + 1), 0.1/numLorentz, 0.0, 1.0);
		// For recursive sum
		I_lorentz2[i] = new RooRealVar(TString::Format("sl1%dInt", i + 1), TString::Format("Sum Lorentz%d intensity", i + 1), 0.3, 0.0, 1.0);
		pdfList->add(*lorentz2[i]);
		components->add(*lorentz2[i]);
                coeffList->add(*I_lorentz2[i]);
	}
        
        // Orthogonal PDF
        if (hasOrthogonal){
            RooRealVar* a1 = new RooRealVar("a1", "a1 coeff", 1, 1E-2, 1E2, "A");
            RooRealVar* a2 = new RooRealVar("a2", "a2 coeff", 1, 1E-2, 1E2, "A");
            RooRealVar* eps = new RooRealVar("oStretch", "Orthogonal stretch", 1, 0.2, 50, "1/keV"); // 5 0.5 10
            OrthogonalPdf* orthogonalPdf = new OrthogonalPdf("orthogonal", "Orthogonal Pdf", *x, *x0, *a1, *a2, *eps);
            RooRealVar* I_ortho = new RooRealVar("oInt", "Orthogonal intensity", 0.3, 0.0, 1.0);
            pdfList->add(*orthogonalPdf);
            components->add(*orthogonalPdf);
            coeffList->add(*I_ortho);
        }
        
        // Remove last coefficient for a recursive sum
        TIterator* coeffIter = coeffList->createIterator();
        TObject* tempObj=0;
        TObject* prevObj=0;        
        do {
            prevObj = tempObj;
        } while((tempObj = coeffIter->Next()));
        RooAbsArg* prevAbsArg = dynamic_cast<RooAbsArg*>(prevObj);
        if (prevAbsArg != NULL){
            coeffList->remove(*prevAbsArg);
        }
        
	// If we sum values recursively than bg~1E-5, parabola~0.9, lorentz coefficients~0.9 (needs kTRUE)
	RooAddPdf* sumModel = new RooAddPdf("sumModel", "Sum of components", *pdfList, *coeffList, kTRUE);
	this->model = sumModel;

	// Resolution Function
	RooRealVar* zero = new RooRealVar("zero", "zero", 0);
//	RooRealVar* resFunct_FWHM = new RooRealVar("resFunctFWHM", "Resolution function FWHM", 1.5, 1E-3, 3, "keV");
	RooRealVar* resFunct_FWHM = new RooRealVar("resFunctFWHM", "Resolution function FWHM", 3, 1, 3.5, "keV");
	RooFormulaVar* resFunct_dispersion = new RooFormulaVar("resFunctDispersion", "@0*@1", RooArgList(*resFunct_FWHM, *fwhm2sigma));
	resolutionFunction = new RooGaussian("resFunct", "Resolution Function", *x, *zero, *resFunct_dispersion);

	// Convolution
	x->setBins(convolutionPoints, "cache");
	RooFFTConvPdf* sumModelConvoluted = new RooFFTConvPdf("sumModelConvoluted", "Convoluted Model", *x, *sumModel, *resolutionFunction);
	sumModelConvoluted->setBufferFraction(1);

	// Constant Background
	//	RooPolynomial* constBg = new RooPolynomial("constBg", "y=1", *x, RooArgSet());
//	RooGenericPdf* constBg = new RooGenericPdf("constBg", "1", *x);
//	RooRealVar* I_const = new RooRealVar("IConst", "Constant background", constBgFraction, 0, constBgFraction*2);
//	bgComponents->add(*constBg);

	// Atan background
	if (hasAtan){
		RooGenericPdf* atanBg;
		RooRealVar* I_atan;
		atanBg = new RooGenericPdf("atanBg", "@2/2 + (-1)*atan((@0 - @1))", RooArgList(*x, *x0, *pi));
		I_atan = new RooRealVar("IAtan", "Atan Intensity", 0.002, 0.0, 0.1);
		bgComponents->add(*atanBg);
//		this->convolutedModel = new RooAddPdf("model_atan", "Model with Convolution and Atan Background", RooArgList(*constBg, *atanBg, *sumModelConvoluted), RooArgList(*I_const, *I_atan));
		this->convolutedModel = new RooAddPdf("model_atan", "Model with Convolution and Atan Background", RooArgList(*atanBg, *sumModelConvoluted), RooArgList(*I_atan));
	}
	else {
		this->convolutedModel = sumModelConvoluted;
//		this->convolutedModel = new RooAddPdf("model_const", "Model with Convolution and Const Background", RooArgList(*constBg, *sumModelConvoluted), RooArgList(*I_const));
	}
}

std::list<Variable*> CompositeModelProvider::getIndirectParameters(){
    std::list<Variable*> parameters;
    TIterator* it = pdfList->createIterator();
    TObject* tempObj=0;
    std::cout << "getIndirectParameters: Iterating PDFs" << std::endl;
    while((tempObj=it->Next())){
        tempObj->Print();
        IndirectParamPdf* paramPdf = dynamic_cast<IndirectParamPdf*>(tempObj);
        if(paramPdf != NULL){
            std::list<Variable*> list = paramPdf->getParameters(isTwoDetector);
            std::list<Variable*>::iterator iter;
            for (iter = list.begin(); iter != list.end(); iter++) {
                Variable* v = *iter;
                v->print();
                parameters.push_back(v);
            }
        }
    }
    return parameters;
}