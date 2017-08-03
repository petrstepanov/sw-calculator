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

class HistProcessor {
public:
	static HistProcessor* getInstance();
	TH1I* cutHist(TH1I*, Int_t, Int_t);
	RooCurve* subtractCurves(RooCurve*, RooCurve*);
	TH1F* subtractCurve(TH1I*, RooCurve*);
	TH1F* getChi2Hist(TH1I*, RooCurve*);
	Bool_t hasBackground(TH1I*);
	Bool_t hasAtan(TH1I*);
	Double_t calcBackgroundFraction(TH1I*);
	std::pair<Double_t, Int_t> getChi2(TH1I*, RooCurve*, Int_t);
	std::pair<Double_t, Double_t> calcIntegral(TH1F*, Double_t, Double_t);
	Bool_t isTwoDetetor(TH1I*);

private:
	HistProcessor();                                      // Private so that it can  not be called
	HistProcessor(HistProcessor const&);                  // Copy constructor is private
	HistProcessor& operator=(HistProcessor const&);       // Assignment operator is private
	static HistProcessor* instance;
};

#endif /* HISTPROCESSOR_H */

