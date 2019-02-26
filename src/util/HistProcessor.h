/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HistProcessor.h
 * Author: petrstepanov
 *
 * Created on August 2, 2017, 5:41 AM
 */

#ifndef HISTPROCESSOR_H
#define HISTPROCESSOR_H

#include <TH1.h>
#include <RooCurve.h>
#include <RooAbsPdf.h>
#include <RooArgList.h>

struct Chi2Struct {
	const Double_t chiSum;
	const Int_t degreesOfFreedom;
	const Double_t chi2;
};

class HistProcessor {
public:
    static HistProcessor* getInstance();

    TH1* cutHist(const char *newname, TH1* hist, Int_t, Int_t);
    TH1* cutHistBasement(const char *newname, TH1* hist, Int_t, Int_t);
    RooCurve* subtractCurves(const char *newname, RooCurve*, RooCurve*);
    TH1* subtractCurve(const char *newname, TH1*, RooCurve*);
    TH1* getResidualHist(const char *newname, TH1*, RooCurve*);
    TH1* getChi2Hist(const char *newname, TH1*, RooCurve*);
    Double_t getTotalCounts(TH1*);
    Bool_t hasBackground(TH1*);
    Bool_t hasAtan(TH1*);
    Double_t calcBackgroundFraction(TH1*);
    Chi2Struct getChi2(TH1*, RooCurve*, RooAbsPdf*);
    std::pair<Double_t, Double_t> calcIntegral(TH1*, Double_t, Double_t);
    Bool_t isTwoDetetor(TH1*);
    Double_t getPdfMaximumX (RooAbsPdf*, const RooArgList&);
    std::pair<Double_t, Double_t> getHistogramSafeFitRange(TH1*);
    std::pair<Double_t, Double_t> getSParameter(TH1* hist, Double_t sWidth, Double_t mean, Bool_t isTwoDetector);
    std::pair<Double_t, Double_t> getWParameter(TH1* hist, Double_t wWidth, Double_t wShift, Double_t mean, Bool_t isTwoDetector);
    
private:
    HistProcessor();                                      // Private so that it can  not be called
    HistProcessor(HistProcessor const&);                  // Copy constructor is private
    HistProcessor& operator=(HistProcessor const&);       // Assignment operator is private
    static HistProcessor* instance;
};

#endif /* HISTPROCESSOR_H */

