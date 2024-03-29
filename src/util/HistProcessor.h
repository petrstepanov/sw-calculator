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

//    static Double_t liftHist(TH1F* hist, Double_t lift);
    static Int_t getLeftNonZeroBin(TH1F* hist);
    static Int_t getRightNonZeroBin(TH1F* hist);
//    static TH1F* cutHistZeros(TH1F* hist);
    static TH1F* cutHist(const char *newname, TH1F* hist, Int_t minBin, Int_t maxBin, Bool_t setBinErrors = kFALSE);
    TH1F* cutHistBasement(const char *newname, TH1F* hist, Int_t, Int_t);
    RooCurve* subtractCurves(const char *newname, RooCurve*, RooCurve*);
    TH1F* subtractCurve(const char *newname, TH1F*, RooCurve*);
    TH1F* getResidualHist(const char *newname, TH1F*, RooCurve*);
    TH1F* getChi2Hist(const char *newname, TH1F*, RooCurve*);
    TH1F* removeHistNegatives(const char *newname, TH1F* hist);
    static Double_t getHistMaximumInRange(TH1* hist, Double_t xMin, Double_t xMax);
    static Double_t getHistMaximumInRange(TH1* hist, Int_t minBin, Int_t maxBin);
    Double_t getTotalCounts(TH1F*);
    Bool_t hasBackground(TH1F*);
    Bool_t hasAtan(TH1F*);
    static Double_t calcLeftWing(TH1F*);
    static Double_t calcRightWing(TH1F*);
    static Double_t calcRectBackgroundFraction(TH1F*);
    static Double_t calcAsymBackgroundFraction(TH1F*);
    Chi2Struct getChi2(TH1F*, RooCurve*, RooAbsPdf*);
    static std::pair<Double_t, Double_t> calcIntegral(TH1F*, Double_t, Double_t);
    static Bool_t isTwoDetector(TH1* hist);
    Double_t getPdfMaximumX (RooAbsPdf*, const RooArgList&);
    std::pair<Double_t, Double_t> getHistogramSafeFitRange(TH1F*);
    static RooRealVar* getSParameter(TH1F* hist, Double_t sWidth, Double_t mean);
    static RooRealVar* getWParameter(TH1F* hist, Double_t wWidth, Double_t wShift, Double_t mean);
    
private:
    HistProcessor();                                      // Private so that it can  not be called
    HistProcessor(HistProcessor const&);                  // Copy constructor is private
    HistProcessor& operator=(HistProcessor const&);       // Assignment operator is private
    static HistProcessor* instance;

    static const Int_t wingBins = 10;
};

#endif /* HISTPROCESSOR_H */

