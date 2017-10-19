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
#include "../util/Variable.h"

class CompositeModelProvider : public AbstractModelProvider {
public:
    CompositeModelProvider();
    CompositeModelProvider(const CompositeModelProvider& orig);
    
    CompositeModelProvider(RooRealVar* x, RooRealVar* x0, Bool_t hasParabola = kTRUE, const Int_t numGauss = 1, const Int_t numLorentz = 1, const Int_t numLorentzSum = 1, Bool_t hasOrthogonal = kFALSE, Int_t convType = 0, Double_t convFWHM = 1.7, Bool_t isConvFixed = kFALSE, Double_t constBgFraction = 0, Bool_t isTwoDetector = kTRUE);
//    virtual ~CompositeModelProvider();

    std::list<Variable*> getIndirectParameters();
    std::list<std::pair<const char*, Double_t>> getIntensities();
    
    static std::map<Int_t, TString> getConvolutionTypes(void);
    
private:
    RooArgList* pdfList;
    RooArgList* coeffList;
    Bool_t isTwoDetector;
    RooRealVar* observable;

    static std::map<Int_t, TString> createConvolutionType(){
        std::map<Int_t, TString> m = {{1, TString("None")}, {2, TString("FFT3")}, {3, TString("Numeric")}, {4, TString("Custom")}};
        return m;
    }; 
    static std::map<Int_t, TString> convolutionType;
    
    Double_t* getDefaultGaussAs(Int_t numGauss);
    Double_t* getDefaultLorentzAs(Int_t numGauss);
    Double_t* getDefaultDampLorentzAs(Int_t numGauss);

    void deleteObject();
    
};

#endif /* COMPOSITEMODELPROVIDER_H */

