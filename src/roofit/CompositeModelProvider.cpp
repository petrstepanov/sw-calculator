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
#include "../util/RootHelper.h"
#include "../model/Constants.h"
#include "ChannelConvolutionPdf.h"
#include "BackgroundPdf.h"
#include <RooFormulaVar.h>
#include <RooPolynomial.h>
#include <RooGaussian.h>
#include <RooAddPdf.h>
#include <RooFFTConvPdf.h>
#include <RooNumConvPdf.h>
#include <RooGenericPdf.h>
#include <RooCachedPdf.h>
#include <RooDataHist.h>
#include <RooFit.h>
#include <TIterator.h>
#include <TMath.h>
#include <TH1F.h>

CompositeModelProvider::CompositeModelProvider(RooRealVar* x, RooRealVar* x0) : AbstractModelProvider(x0){
    observable = x;
}

void CompositeModelProvider::initSourcePdf(TH1F* sourceHist, Double_t sourceContribution, Bool_t isFixed) {
    RooDataHist* sourceDataHist = new RooDataHist("sourceDataHist", "Source Data Hist", RooArgList(*observable), RooFit::Import(*sourceHist));
    this->sourcePdf = new RooHistPdf("sourcePdf", "Source contribution PDF", *observable, *sourceDataHist, 0);
    if (isFixed){
        this->sourceContribution = new RooRealVar("sourceContribution", "Source contribution", sourceContribution, sourceContribution, sourceContribution);
    } else {
        this->sourceContribution = new RooRealVar("sourceContribution", "Source contribution", sourceContribution, sourceContribution/2, sourceContribution*2);
    }
}


void CompositeModelProvider::initModel(Bool_t hasParabola, const Int_t numGauss, const Int_t numLorentz, const Int_t numLorentzSum){
    pdfList = new RooArgList();
    coeffList = new RooArgList();

    // Check source contribution
    if (sourcePdf) {
        pdfList->add(*sourcePdf);
        coeffList->add(*sourceContribution);
        components->add(*sourcePdf);
    }
    
    // Define parabola
    if (hasParabola){
            RootHelper::deleteObject("parabolaRoot");
            RooRealVar* parabolaRoot = new RooRealVar("parabolaRoot", "Coefficient at -x^2 + r*2", 3.5, 0.1, 10);
            RootHelper::deleteObject("parabola");
            ParabolaPdf* parabola = new ParabolaPdf("parabola", "Fermi gas", *observable, *E_0, *parabolaRoot);
            RootHelper::deleteObject("parabolaCoeff");
            RooRealVar* parabolaCoeff = new RooRealVar("parabolaCoeff", "Parabola intensity", 0.8, 0.0, 1.0);
            pdfList->add(*parabola);
            coeffList->add(*parabolaCoeff);
            components->add(*parabola);
    }

    // Gauss PDFs
    RooRealVar** gaussA = new RooRealVar*[numGauss];
    GaussianPdf** gauss = new GaussianPdf*[numGauss];
    RooRealVar** I_gauss = new RooRealVar*[numGauss];
    Double_t aMin = 0.01; // [KeV]
    Double_t aMax = 2; // [KeV]
    for (int i = 0; i < numGauss; i++){
        RootHelper::deleteObject(TString::Format("gauss%d_A", i + 1));
        gaussA[i] = new RooRealVar(TString::Format("gauss%d_A", i + 1), TString::Format("Gauss%d A", i + 1), getDefaultGaussAs(numGauss)[i], aMin, aMax, "A"); // 5 0.5 10
        RootHelper::deleteObject(TString::Format("gauss%d", i + 1));
        gauss[i] = new GaussianPdf(TString::Format("gauss%d", i + 1), TString::Format("Gauss%d Component", i + 1), *observable, *E_0, *gaussA[i]);
        RootHelper::deleteObject(TString::Format("gauss%d_coeff", i + 1));
        I_gauss[i] = new RooRealVar(TString::Format("gauss%d_coeff", i + 1), TString::Format("Gauss%d intensity", i + 1), 0.8, 0.0, 1.0);
        pdfList->add(*gauss[i]);
        components->add(*gauss[i]);
        coeffList->add(*I_gauss[i]);
    }  

    // Composite Lorentz PDFs
    RooRealVar** a2 = new RooRealVar*[numLorentzSum];
    DampLorentzPdf** lorentz2 = new DampLorentzPdf*[numLorentzSum];
    RooRealVar** I_lorentz2 = new RooRealVar*[numLorentzSum];
    for (int i = 0; i < numLorentzSum; i++){
            RootHelper::deleteObject(TString::Format("sl%dA", i + 1));         
            a2[i] = new RooRealVar(TString::Format("sl%dA", i + 1), TString::Format("Sum Lorentz%d a", i + 1), getDefaultDampLorentzAs(numLorentzSum)[i], aMin, aMax, "A"); // 5 0.5 10
            // a2[i] = new RooRealVar(TString::Format("sl%dA", i + 1), TString::Format("Sum Lorentz%d a", i + 1), getDefaultLorentzAs(numLorentzSum)[i], getDefaultLorentzAs(numLorentzSum)[i], getDefaultLorentzAs(numLorentzSum)[i], "A");
            RootHelper::deleteObject(TString::Format("sumlorentz%d", i + 1)); 
            lorentz2[i] = new DampLorentzPdf(TString::Format("sumlorentz%d", i + 1), TString::Format("Damping exponent%d", i + 1), *observable, *E_0, *a2[i]);
            RootHelper::deleteObject(TString::Format("sl1%dInt", i + 1)); 
            I_lorentz2[i] = new RooRealVar(TString::Format("sl1%dInt", i + 1), TString::Format("Sum Lorentz%d intensity", i + 1), 0.8, 0.0, 1.0);
            pdfList->add(*lorentz2[i]);
            components->add(*lorentz2[i]);
            coeffList->add(*I_lorentz2[i]);
    }

    // Lorentz PDFs
    RooRealVar** a = new RooRealVar*[numLorentz];
    LorentzianPdf** lorentz = new LorentzianPdf*[numLorentz];
    RooRealVar** I_lorentz = new RooRealVar*[numLorentz];
    for (int i = 0; i < numLorentz; i++){
        RootHelper::deleteObject(TString::Format("l%dA", i + 1));           
        a[i] = new RooRealVar(TString::Format("l%dA", i + 1), TString::Format("Lorentz%d a", i + 1), getDefaultLorentzAs(numLorentz)[i], aMin, aMax, "A"); // 5 0.5 10
        RootHelper::deleteObject(TString::Format("lorentz%d", i + 1));           
        lorentz[i] = new LorentzianPdf(TString::Format("lorentz%d", i + 1), TString::Format("Exponent%d Component", i + 1), *observable, *E_0, *a[i]);
        RootHelper::deleteObject(TString::Format("l1%dInt", i + 1));           
        I_lorentz[i] = new RooRealVar(TString::Format("l1%dInt", i + 1), TString::Format("Lorentz%d intensity", i + 1), 0.8, 0.0, 1.0);
        pdfList->add(*lorentz[i]);
        components->add(*lorentz[i]);
        coeffList->add(*I_lorentz[i]);
    }

    // Orthogonal PDF
    // if (hasOrthogonal){
    //     RootHelper::deleteObject("a1");            
    //     RooRealVar* a1 = new RooRealVar("a1", "a1 coeff", 1, 1E-2, 1E2, "A");
    //     RootHelper::deleteObject("a2");            
    //     RooRealVar* a2 = new RooRealVar("a2", "a2 coeff", 1, 1E-2, 1E2, "A");
    //     RootHelper::deleteObject("orthogonal");            
    //     OrthogonalPdf* orthogonalPdf = new OrthogonalPdf("orthogonal", "Orthogonal Pdf", *x, *x0, *a1, *a2);
    //     RootHelper::deleteObject("oInt"); 
    //     RooRealVar* I_ortho = new RooRealVar("oInt", "Orthogonal intensity", 0.3, 0.0, 1.0);
    //     pdfList->add(*orthogonalPdf);
    //     components->add(*orthogonalPdf);
    //     coeffList->add(*I_ortho);
    // }

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
    std::cout << "coeffList" << std::endl;
    coeffList->Print();

    // If we sum values recursively than bg~1E-5, parabola~0.9, lorentz coefficients~0.9 (needs kTRUE)
    RootHelper::deleteObject("sumModel");
    RooAddPdf* sumModel = new RooAddPdf("sumModel", "Sum of components", *pdfList, *coeffList, kTRUE);

    this->model = sumModel;
}

void CompositeModelProvider::initBackground(Double_t backgroundFraction){
    // Ore-Powell background
//    RooRealVar* threeGammaInt = new RooRealVar("threeGammaInt", "Three Gamma fraction", 10, 1, 100);
//    BackgroundPdf* bgPdf = new BackgroundPdf("bgPdf", "Ore-Powell background", *observable, *threeGammaInt);
//    RooRealVar* bgPdfFraction = new RooRealVar("bgPdfFraction", "Background fraction", backgroundFraction, backgroundFraction/10, 1.);
//    bgComponents->add(*bgPdf);
//    components->add(*bgPdf);
//    pdfList->add(*bgPdf);
//    coeffList->add(*bgPdfFraction);

    // Constant background
    // RooGenericPdf* constBg = new RooGenericPdf("constBg", "1", *observable);
    RooPolynomial* constBg = new RooPolynomial("constBg", "y=1", *observable, RooArgSet());
    RooRealVar* I_const = new RooRealVar("IConst", "Const background", backgroundFraction/2, 0, 0.2);
    bgComponents->add(*constBg);
    
    // Atan background
    RooGenericPdf* atanBg = new RooGenericPdf("atanBg", "Atan background", "@2/2 + (-1)*atan((@0 - @1))", RooArgList(*observable, *E_0, *pi));
    RooRealVar* I_atan = new RooRealVar("IAtan", "Atan Intensity", 0.002, 0.0, 0.1); //backgroundFraction/2, 0, 0.2);
    bgComponents->add(*atanBg);
    
//    RooAddPdf* sumModelAndBg = new RooAddPdf("sumModelAndBg", "Sum of components and Background", *constBg, *model, *I_const);
    RooAddPdf* sumModelAndBg = new RooAddPdf("sumModelAndBg", "Sum of components and Background", RooArgList(*atanBg, *constBg, *model), RooArgList(*I_atan, *I_const));
    
    this->model = sumModelAndBg;
}

void CompositeModelProvider::initResolutionFunction(Int_t convType, Double_t convFWHM, Bool_t isConvFixed) {
    // Resolution Function
    RootHelper::deleteObject("zero");        
    RooRealVar* zero = new RooRealVar("zero", "zero", 0);
    RootHelper::deleteObject("resFunctFWHM");       
    RooRealVar* resFunctFWHM = isConvFixed ? 
        new RooRealVar("resFunctFWHM", "Resolution function FWHM", convFWHM, "keV") :
        new RooRealVar("resFunctFWHM", "Resolution function FWHM", convFWHM, convFWHM/4, convFWHM * 4, "keV");
    RootHelper::deleteObject("resFunctSigma");
    RooFormulaVar* resFunctSigma = new RooFormulaVar("resFunctSigma", "@0*@1", RooArgList(*resFunctFWHM, *fwhm2sigma));
    RootHelper::deleteObject("resFunct");
    if (convType){
        resolutionFunction = new RooGaussian("resFunct", "Resolution Function", *observable, *zero, *resFunctSigma);
    }
    else {
        resolutionFunction = nullptr;
    }
    
    // Convolution
    RootHelper::deleteObject("sumModelConvoluted");
    RooAbsPdf* sumModelConvoluted = nullptr;
    switch(convType){
        case 0:         // No convolution
            this->convolutedModel = nullptr;
            break;
        case 1:         // FFT3
            observable->setBins(10000, "cache");
            sumModelConvoluted = new RooFFTConvPdf("sumModelConvoluted", "Convoluted Model", *observable, *model, *resolutionFunction);
            ((RooFFTConvPdf*) sumModelConvoluted)->setBufferFraction(0);
            // sumModelConvoluted->setBufferStrategy(RooFFTConvPdf::Flat);
            this->convolutedModel = sumModelConvoluted;                
            break;
        case 2:         // Numeric
            sumModelConvoluted = new RooNumConvPdf("sumModelConvoluted", "Convoluted Model", *observable, *model, *resolutionFunction);
            ((RooNumConvPdf*) sumModelConvoluted)->setConvolutionWindow(*zero,*resFunctSigma,4);
            this->convolutedModel = sumModelConvoluted;
            break;
        case 3:         // Custom
            sumModelConvoluted = new ChannelConvolutionPdf("sumModelConvoluted", "Convoluted Model", *observable, *model, *resolutionFunction);
            ((ChannelConvolutionPdf*) sumModelConvoluted)->setConvolutionWindow(*zero,*resFunctFWHM,2);
            this->convolutedModel = sumModelConvoluted;
            break;
    }    
}


void CompositeModelProvider::initTwoDetector(Bool_t hasParabola, const Int_t numGauss, const Int_t numLorentz, const Int_t numLorentzSum, Int_t convType, Double_t convFWHM, Bool_t isConvFixed) {
    isTwoDetector = kTRUE;
    initModel(hasParabola, numGauss, numLorentz, numLorentzSum);
    initResolutionFunction(convType, convFWHM, isConvFixed);
}

void CompositeModelProvider::initSingleDetector(Bool_t hasParabola, const Int_t numGauss, const Int_t numLorentz, const Int_t numLorentzSum, Int_t convType, Double_t convFWHM, Bool_t isConvFixed, Double_t bgFraction){
    isTwoDetector = kFALSE;
    initModel(hasParabola, numGauss, numLorentz, numLorentzSum);
    initBackground(bgFraction);
    initResolutionFunction(convType, convFWHM, isConvFixed);    
}

std::list<Variable*> CompositeModelProvider::getIndirectParameters(){
    std::list<Variable*> parameters;
    TIterator* it = pdfList->createIterator();
    TObject* tempObj=0;
//    std::cout << "getIndirectParameters: Iterating PDFs" << std::endl;
    while((tempObj=it->Next())){
//        tempObj->Print();
        IndirectParamPdf* paramPdf = dynamic_cast<IndirectParamPdf*>(tempObj);
        if(paramPdf != NULL){
            std::list<Variable*> list = paramPdf->getParameters(isTwoDetector);
            std::list<Variable*>::iterator iter;
            for (iter = list.begin(); iter != list.end(); iter++) {
                Variable* v = *iter;
//                v->print();
                parameters.push_back(v);
            }
        }
    }
    return parameters;
}

std::map<Int_t, TString> CompositeModelProvider::convolutionType =  CompositeModelProvider::createConvolutionType();

std::map<Int_t, TString> CompositeModelProvider::getConvolutionTypes(void){
    return convolutionType;
}

std::list<std::pair<const char*, Double_t>> CompositeModelProvider::getIntensities(){
    std::list<std::pair<const char*, Double_t>> intensities;
    TIterator* it = pdfList->createIterator();
    TObject* tempObj=0;
//    std::cout << "getIntensities: Iterating PDFs" << std::endl;
//    RooAbsReal* fullInt = getModel()->createIntegral(*observable);
    Double_t previousCoeff = 1;  
    while((tempObj=it->Next())){
        RooAbsPdf* absPdf = dynamic_cast<RooAbsPdf*>(tempObj);
        if(absPdf != NULL){
            Int_t componentIndex = pdfList->index(absPdf);
            RooAbsArg* arg = coeffList->at(componentIndex);
            RooAbsReal* absReal = dynamic_cast<RooAbsReal*>(arg);
            Double_t actualCoeff;
            if (absReal!=0){
                Double_t pdfCoeff = absReal->getVal();
                actualCoeff = previousCoeff*pdfCoeff;
                previousCoeff *= (1-pdfCoeff);
            }
            else {
                actualCoeff = previousCoeff;
            } 
            std::cout << absPdf->GetName() << " " << actualCoeff << std::endl;
            std::pair<const char*, Double_t> p = std::make_pair(absPdf->GetTitle(), actualCoeff);
            intensities.push_back(p);
        }
    }
    return intensities;
}

Double_t* CompositeModelProvider::getDefaultGaussAs(const Int_t numGauss){
    Double_t* As;
    switch ( numGauss ) {
        case 1:
            As = (Double_t[1]){0.1};
            break;
        case 2:
            As = (Double_t[2]){0.5, 0.1};
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
            As = (Double_t[1]){0.18};
            break;
        case 2:
            As = (Double_t[2]){0.16, 0.5};
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

Double_t* CompositeModelProvider::getDefaultDampLorentzAs(const Int_t numLorentz){
    Double_t* As;
    switch ( numLorentz ) {
        case 1:
            As = (Double_t[1]){0.1};
            break;
        case 2:
            As = (Double_t[2]){0.1, 0.4}; // 70, 6 eV
            break;
        case 3:
            As = (Double_t[3]){0.05, 0.1, 0.5};
            break;
        default:
            As = (Double_t[4]){0.05, 0.1, 0.5, 1};
            break;
    }
    return As;
};