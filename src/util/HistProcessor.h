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

class HistProcessor {
public:
	static HistProcessor* getInstance();
        
	TH1* cutHist(TH1*, Int_t, Int_t);
	RooCurve* subtractCurves(RooCurve*, RooCurve*);
	TH1* subtractCurve(TH1*, RooCurve*);
	TH1* getChi2Hist(TH1*, RooCurve*);
        Double_t getTotalCounts(TH1*);
	Bool_t hasBackground(TH1*);
	Bool_t hasAtan(TH1*);
	Double_t calcBackgroundFraction(TH1*);
	std::pair<Double_t, Int_t> getChi2(TH1*, RooCurve*, Int_t);
	std::pair<Double_t, Double_t> calcIntegral(TH1*, Double_t, Double_t);
	Bool_t isTwoDetetor(TH1*);
        Double_t getPdfMaximumX (RooAbsPdf*, const RooArgList&);

private:
	HistProcessor();                                      // Private so that it can  not be called
	HistProcessor(HistProcessor const&);                  // Copy constructor is private
	HistProcessor& operator=(HistProcessor const&);       // Assignment operator is private
	static HistProcessor* instance;
};

#endif /* HISTPROCESSOR_H */

