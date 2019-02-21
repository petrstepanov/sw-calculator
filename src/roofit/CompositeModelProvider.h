/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CompositeModelProvider.h
 * Author: petrstepanov
 *
 * Created on August 15, 2017, 9:59 PM
 */

#ifndef COMPOSITEMODELPROVIDER_H
#define COMPOSITEMODELPROVIDER_H

#include "AbstractModelProvider.h"
#include <RooRealVar.h>
#include <RooHistPdf.h>

class CompositeModelProvider : public AbstractModelProvider {
public:
    CompositeModelProvider();
    CompositeModelProvider(const CompositeModelProvider& orig);
    
    CompositeModelProvider(RooRealVar* x, RooRealVar* x0);
//    virtual ~CompositeModelProvider();

    RooArgList* getIndirectParameters();
    RooArgList* getIntensities();
    
    static std::map<Int_t, TString> getConvolutionTypes(void);
    
    void initTwoDetector(Bool_t hasParabola = kTRUE, const Int_t numGauss = 1, const Int_t numLorentz = 1, const Int_t numLorentzSum = 1, /*Bool_t hasOrthogonal = kFALSE,*/ RooRealVar* fwhm = nullptr);
    void initSingleDetector(Bool_t hasParabola = kTRUE, const Int_t numGauss = 1, const Int_t numLorentz = 1, const Int_t numLorentzSum = 1, /*Bool_t hasOrthogonal = kFALSE,*/ RooRealVar* fwhm = nullptr, Double_t bgFraction = 0.1);
    
    void initSourcePdf(TH1F* sourceHist, RooAbsReal* sourceContrib);
    
private:
    RooArgList* pdfsInMaterial;            // list of pdfs to be convoluted
    RooArgList* coeffsInMaterial;          // list of coefficients of pdfs in material
    RooRealVar* observable;
    RooHistPdf* sourcePdf = nullptr;
    RooAbsReal* Int_sourcePdf = nullptr;
    Bool_t isTwoDetector;
    
    static std::map<Int_t, TString> createConvolutionType(){
        std::map<Int_t, TString> m = {{1, TString("None")}, {2, TString("FFT3")}};
        return m;
    }; 

    static std::map<Int_t, TString> convolutionType;
    
    void initModel(Bool_t hasParabola, const Int_t, const Int_t numLorentz, const Int_t numLorentzSum);
    void initConvolutedModel(RooRealVar* fwhm);
    void initBackground(Double_t backgroundFraction);
    
    Double_t* getDefaultGaussAs(Int_t numGauss);
    Double_t* getDefaultLorentzAs(Int_t numGauss);
    Double_t* getDefaultDampLorentzAs(Int_t numGauss);

    void deleteObject();

    RooAbsPdf* getConvolutedPdf(RooAbsPdf* pdf, RooRealVar* fwhm);
};

#endif /* COMPOSITEMODELPROVIDER_H */

