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

class Model : public TQObject {
public:
    static Model* getInstance();
    
    void setFileName(TString* fileName);
    TString* getFileName();

    void setSourceFileName(TString* fileName);
    TString* getSourceFileName();
    
    void setHist(TH1F* hist);
    TH1F* getHist();
    
    void setSourceHist(TH1F* hist);
    TH1F* getSourceHist();

    void setTwoDetector(Bool_t b);
    Bool_t isTwoDetector();

    void setSafeFitRange(Double_t eMin, Double_t eMax);
    
    RooRealVar* getSourceContribution();
    
    // SIGNALS
    void sourceHistogramImported(TH1F* hist); // *SIGNAL*
    void twoDetectorSet(Bool_t isTwoDetector); // *SIGNAL*
    void safeFitRangeSet(Double_t eMin, Double_t eMax); // *SIGNAL*

private:
    Model();                              // Private so that it can not be called
    static Model* instance;

    TString* strFileName = nullptr;
    TString* strSourceContribFileName = nullptr;
    TH1F* fullHist = nullptr;
    TH1F* sourceHist = nullptr;
    RooCurve* curveBg = nullptr;
    RooCurve* curveFit = nullptr;
    TH1F* peakHistNoBg = nullptr;
    TH1F* chiHist = nullptr;
    RooRealVar* sourceContribution;
    Bool_t twoDetector;
    std::pair<Double_t, Double_t> safeFitRange;

	ClassDef(Model, 0)
};

#endif /* MODEL_H */

