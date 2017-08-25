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
#include "RootHelper.h"
#include "Constants.h"
#include <RooFormulaVar.h>
#include <RooGaussian.h>
#include <RooAddPdf.h>
#include <RooFFTConvPdf.h>
#include <RooGenericPdf.h>
#include <TIterator.h>
#include <TMath.h>

CompositeModelProvider::CompositeModelProvider(RooRealVar* x, RooRealVar* x0, Bool_t hasParabola, const Int_t numGauss, const Int_t numLorentz, const Int_t numLorentzSum, Bool_t hasOrthogonal, Bool_t hasAtan, Double_t constBgFraction, Bool_t isTwoDetector) : AbstractModelProvider(x0){
	pdfList = new RooArgList();
	coeffList = new RooArgList();
        this->isTwoDetector = isTwoDetector;
        
	// Define parabola
	if (hasParabola){
                RootHelper::deleteObject("parabolaRoot");
                RooRealVar* parabolaRoot = new RooRealVar("parabolaRoot", "Coefficient at -x^2 + r*2", 2, 0.1, 10);
                RootHelper::deleteObject("parabola");
		ParabolaPdf* parabola = new ParabolaPdf("parabola", "parabola", *x, *x0, *parabolaRoot);
                RootHelper::deleteObject("parabolaCoeff");
		RooRealVar* I_parabola = new RooRealVar("parabolaCoeff", "Parabola intensity", 0.5, 0.0, 1.0);
		pdfList->add(*parabola);
		coeffList->add(*I_parabola);
		components->add(*parabola);
	}

	// Gauss PDFs
//	RooRealVar** FWHM = new RooRealVar*[numGauss];
//	RooFormulaVar** sigma = new RooFormulaVar*[numGauss];
	RooRealVar** gaussA = new RooRealVar*[numGauss];
	GaussianPdf** gauss = new GaussianPdf*[numGauss];
	RooRealVar** I_gauss = new RooRealVar*[numGauss];
	Double_t aMin = 0.01; // [KeV]
	Double_t aMax = 2; // [KeV]
	for (int i = 0; i < numGauss; i++){
            RootHelper::deleteObject(TString::Format("gauss%d_A", i + 1));
            gaussA[i] = new RooRealVar(TString::Format("gauss%d_A", i + 1), TString::Format("Gauss%d A", i + 1), getDefaultGaussAs(numGauss)[i], aMin, aMax, "A"); // 5 0.5 10
            RootHelper::deleteObject(TString::Format("gauss%d", i + 1));
            gauss[i] = new GaussianPdf(TString::Format("gauss%d", i + 1), TString::Format("Gauss%d PDF", i + 1), *x, *x0, *gaussA[i]);
            RootHelper::deleteObject(TString::Format("gauss%d_coeff", i + 1));
            I_gauss[i] = new RooRealVar(TString::Format("gauss%d_coeff", i + 1), TString::Format("Gauss%d intensity", i + 1), 0.3, 0.0, 1.0);
            pdfList->add(*gauss[i]);
            components->add(*gauss[i]);
            coeffList->add(*I_gauss[i]);
	}  
        
	// Lorentz PDFs
	RooRealVar** a = new RooRealVar*[numLorentz];
	LorentzianPdf** lorentz = new LorentzianPdf*[numLorentz];
	RooRealVar** I_lorentz = new RooRealVar*[numLorentz];
	for (int i = 0; i < numLorentz; i++){
            RootHelper::deleteObject(TString::Format("l%dA", i + 1));           
            a[i] = new RooRealVar(TString::Format("l%dA", i + 1), TString::Format("Lorentz%d a", i + 1), getDefaultLorentzAs(numGauss)[i], aMin, aMax, "A"); // 5 0.5 10
            RootHelper::deleteObject(TString::Format("lorentz%d", i + 1));           
            lorentz[i] = new LorentzianPdf(TString::Format("lorentz%d", i + 1), TString::Format("Lorentz%d PDF", i + 1), *x, *x0, *a[i]);
            RootHelper::deleteObject(TString::Format("l1%dInt", i + 1));           
            I_lorentz[i] = new RooRealVar(TString::Format("l1%dInt", i + 1), TString::Format("Lorentz%d intensity", i + 1), 0.3, 0.0, 1.0);
            pdfList->add(*lorentz[i]);
            components->add(*lorentz[i]);
            coeffList->add(*I_lorentz[i]);
        }
        
	// Composite Lorentz PDFs
	RooRealVar** a2 = new RooRealVar*[numLorentzSum];
	DampLorentzPdf** lorentz2 = new DampLorentzPdf*[numLorentzSum];
	RooRealVar** I_lorentz2 = new RooRealVar*[numLorentzSum];
	for (int i = 0; i < numLorentzSum; i++){
                RootHelper::deleteObject(TString::Format("sl%dA", i + 1));         
		a2[i] = new RooRealVar(TString::Format("sl%dA", i + 1), TString::Format("Sum Lorentz%d a", i + 1), getDefaultLorentzAs(numLorentzSum)[i], aMin, aMax, "A"); // 5 0.5 10
//                a2[i] = new RooRealVar(TString::Format("sl%dA", i + 1), TString::Format("Sum Lorentz%d a", i + 1), getDefaultLorentzAs(numLorentzSum)[i], getDefaultLorentzAs(numLorentzSum)[i], getDefaultLorentzAs(numLorentzSum)[i], "A");
                RootHelper::deleteObject(TString::Format("slorentz%d", i + 1)); 
		lorentz2[i] = new DampLorentzPdf(TString::Format("slorentz%d", i + 1), TString::Format("Sum Lorentz%d PDF", i + 1), *x, *x0, *a2[i]);
                RootHelper::deleteObject(TString::Format("sl1%dInt", i + 1)); 
		I_lorentz2[i] = new RooRealVar(TString::Format("sl1%dInt", i + 1), TString::Format("Sum Lorentz%d intensity", i + 1), 0.3, 0.0, 1.0);
		pdfList->add(*lorentz2[i]);
		components->add(*lorentz2[i]);
                coeffList->add(*I_lorentz2[i]);
	}
        
        // Orthogonal PDF
        if (hasOrthogonal){
            RootHelper::deleteObject("a1");            
            RooRealVar* a1 = new RooRealVar("a1", "a1 coeff", 1, 1E-2, 1E2, "A");
            RootHelper::deleteObject("a2");            
            RooRealVar* a2 = new RooRealVar("a2", "a2 coeff", 1, 1E-2, 1E2, "A");
            RootHelper::deleteObject("orthogonal");            
            OrthogonalPdf* orthogonalPdf = new OrthogonalPdf("orthogonal", "Orthogonal Pdf", *x, *x0, *a1, *a2);
            RootHelper::deleteObject("oInt"); 
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
        RootHelper::deleteObject("sumModel");
	RooAddPdf* sumModel = new RooAddPdf("sumModel", "Sum of components", *pdfList, *coeffList, kTRUE);
	this->model = sumModel;

	// Resolution Function
        RootHelper::deleteObject("zero");        
	RooRealVar* zero = new RooRealVar("zero", "zero", 0);
        RootHelper::deleteObject("resFunctFWHM");       
	RooRealVar* resFunct_FWHM = new RooRealVar("resFunctFWHM", "Resolution function FWHM", 3, 1.5, 4, "keV");
        RootHelper::deleteObject("resFunctDispersion");
	RooFormulaVar* resFunct_dispersion = new RooFormulaVar("resFunctDispersion", "@0*@1", RooArgList(*resFunct_FWHM, *fwhm2sigma));
        RootHelper::deleteObject("resFunct");
	resolutionFunction = new RooGaussian("resFunct", "Resolution Function", *x, *zero, *resFunct_dispersion);

	// Convolution
	x->setBins(convolutionPoints, "cache");
        RootHelper::deleteObject("sumModelConvoluted");
	RooFFTConvPdf* sumModelConvoluted = new RooFFTConvPdf("sumModelConvoluted", "Convoluted Model", *x, *sumModel, *resolutionFunction);
        sumModelConvoluted->setBufferFraction(0) ;
//	sumModelConvoluted->setBufferFraction(1);

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

Double_t* CompositeModelProvider::getDefaultGaussAs(const Int_t numGauss){
    Double_t* As;
    switch ( numGauss ) {
        case 1:
            As = (Double_t[1]){1};
            break;
        case 2:
            As = (Double_t[2]){0.5, 1};
            break;
        case 3:
            As = (Double_t[3]){1.0, 0.5, 0.1};
            break;
        default:
            As = (Double_t[4]){1.0, 0.5, 0.2, 0.05};
            break;
    }
    return As;
}

Double_t* CompositeModelProvider::getDefaultLorentzAs(const Int_t numLorentz){
    Double_t* As;
    switch ( numLorentz ) {
        case 1:
            As = (Double_t[1]){0.2};
            break;
        case 2:
            As = (Double_t[2]){0.05, 0.2};
//            As = (Double_t[2]){Constants::a_B*TMath::Sqrt(Constants::Ry/73), Constants::a_B*TMath::Sqrt(Constants::Ry/117)};
            break;
        case 3:
            As = (Double_t[3]){0.05, 0.1, 0.5};
//            As = (Double_t[3]){Constants::a_B*TMath::Sqrt(Constants::Ry/73), Constants::a_B*TMath::Sqrt(Constants::Ry/117), Constants::a_B*TMath::Sqrt(Constants::Ry/1559)};
            break;
        default:
            As = (Double_t[4]){0.05, 0.1, 0.5, 1};
            break;
    }
    return As;
};