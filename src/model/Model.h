/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Model.h
 * Author: petrstepanov
 *
 * Created on October 6, 2017, 2:35 AM
 */

#ifndef MODEL_H
#define MODEL_H

#include <TString.h>
#include <TH1F.h>
#include <RtypesCore.h>
#include <RooCurve.h>
#include <iostream>
#include <TQObject.h>
#include <RooRealVar.h>
#include "DoublePair.h"
#include "ParametersPool.h"

enum ConvolutionType {
    kNoConvolution, kFFTConvolution, kNumericConvolution
};

// Struct of properties needed to build the fitting model
struct FitProperties {
    Int_t minFitBin;
    Int_t maxFitBin;
    ConvolutionType convolutionType;
    Bool_t hasParabola;
    Int_t numberOfGaussians;
    Int_t numberOfExponents;
    Int_t numberOfDampingExponents;
    TH1F *hist;
    TH1F *sourceHist;
    TH1F *componentHist;
};

// To benefit from signals/slots mechanism classes must inherit from TQObject
class Model: public TQObject {
private:
    Model();                             // Private so that it can not be called
    static Model *instance;

    TString *strFileName;
    TString *strSourceContribFileName;

//    TH1F* peakHistNoBg;
//    TH1F* chiHist;

    FitProperties fitProperties;
    ParametersPool *parametersPool;

    // Flag that controls if Fitting Function should be rebuilt
    // Moved comparison into the presenter because this not corresponds to the Model really
    // Bool_t needsRebuild;

public:
    static Model* getInstance();

    void setFileName(TString *fileName);
    TString* getFileName();

    void setSourceFileName(TString *fileName);
    TString* getSourceFileName();

    void setTwoDetector(Bool_t isTwoDetector);
    Bool_t isTwoDetector();

    // Fit properties
    // void setFitRangeLimits(Double_t fitMinLo, Double_t fitMinHi, Double_t fitMaxLo, Double_t fitMaxHi);
//    void setFitRange(Int_t minBin, Int_t fitMaxHi);
//    std::pair<Double_t, Double_t> getFitRangeLimits();

    void setFitRange(Int_t min, Int_t max);
    std::pair<Double_t, Double_t> getFitRange();

    void setConvolutionType(ConvolutionType t);
    ConvolutionType getConvolutionType();

    void setHasParabola(Bool_t hasParabola);
    Bool_t getHasParabola();

    void setNumberOfGaussians(Int_t num);
    Int_t getNumberOfGaussians();

    void setNumberOfExponents(Int_t num);
    Int_t getNumberOfExponents();

    void setNumberOfDampingExponents(Int_t num);
    Int_t getNumberOfDampingExponents();

    void setHist(TH1F *hist);
//    TH1F* getHist();

    void setSourceHist(TH1F *hist);
    void setComponentHist(TH1F *hist);
    //    TH1F* getSourceHist();

    FitProperties getFitProperties();

    ParametersPool* getParametersPool();

    // SIGNALS
    void histogramImported(TH1F *hist);              // *SIGNAL*
    void sourceHistogramImported(TH1F *hist);        // *SIGNAL*
    void componentHistogramImported(TH1F *hist);     // *SIGNAL*
    void twoDetectorSet(Bool_t isTwoDetector);       // *SIGNAL*
    void fitRangeSet(DoublePair *pair);              // *SIGNAL*
    void convolutionTypeSet(Int_t convolutionType);  // *SIGNAL*
    void hasParabolaSet(Bool_t b);                   // *SIGNAL*
    void numberOfGaussiansSet(Int_t num);            // *SIGNAL*
    void numberOfExponentsSet(Int_t num); 			 // *SIGNAL*
    void numberOfDampingExponentsSet(Int_t num);     // *SIGNAL*
    // void fitPropertiesChanged();                     // *SIGNAL*

ClassDef(Model, 0)
};

#endif /* MODEL_H */

