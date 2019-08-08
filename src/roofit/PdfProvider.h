/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PdfProvider.h
 * Author: petrstepanov
 *
 * Created on August 15, 2017, 9:59 PM
 */

#ifndef PdfProvider_H
#define PdfProvider_H

#include <RooRealVar.h>
#include <RooHistPdf.h>
#include "../model/Model.h"

class PdfProvider {
public:

    PdfProvider(FitProperties fitProperties);
    virtual ~PdfProvider();

    RooArgList* getIndirectParameters();
//    RooArgList* getIntensities();
    RooRealVar* getSourceContribution();
    TH1F* getFitHistogram(); // return trimmed histogram to be fitted

    // Return RooRealVar variables
    RooRealVar* getObservable();
    RooRealVar* getMean();

    // Return PDFs
    RooAbsPdf* getResolutionFunction();
    RooAbsPdf* getPdfConvoluted();
    RooAbsPdf* getPdfNonConvoluted();
    RooAbsPdf* getPdf();

//    RooArgList* getBackgroundComponents();
    RooArgList* getIntensities();
    //    RooArgSet* getParameters();
    
    void initSourcePdf(TH1F* sourceHist, RooRealVar* sourceContrib);
    
private:
    PdfProvider();
    PdfProvider(const PdfProvider& orig);

    // General variables
    RooRealVar* observable;
    RooRealVar* mean;

    TH1F* fitHistogram;
    TH1F* sourceHistogram;

    // Components in material
//	RooArgList* components;
//	RooArgList* intensities;

	// Source contribution
	RooRealVar* intSource;

	// Background
//	RooArgList* backgroundComponents;
//	RooArgList* coeffsInMaterial;

	// Convolution
	RooAbsPdf* resolutionFunction;
	RooAbsPdf* modelNonConvoluted;
	RooAbsPdf* modelConvoluted;

    FitProperties fitProperties;

//    RooArgList* pdfsInMaterial;            // list of pdfs to be convoluted
//    RooArgList* coeffsInMaterial;          // list of coefficients of pdfs in material
    

    void initObservableAndMean();
    void initModel(Bool_t hasParabola, const Int_t, const Int_t numLorentz, const Int_t numLorentzSum);
    void initSourceContribution(TH1F* sourceHist);
    void initConvolutedModel(ConvolutionType convolutionType);
    void initSingleDetectorBackground();
    void initTwoDetectorBackground();

    Double_t getDefaultAValue(Double_t aMin, Double_t aMax, Int_t currentIndex, Int_t maxIndex);

//    Double_t* getDefaultGaussAs(Int_t numGauss);
//    Double_t* getDefaultLorentzAs(Int_t numGauss);
//    Double_t* getDefaultDampLorentzAs(Int_t numGauss);

    void deleteObject();
};

#endif /* PdfProvider_H */

