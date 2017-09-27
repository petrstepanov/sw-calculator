/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HistProcessor.cpp
 * Author: petrstepanov
 * 
 * Created on August 2, 2017, 5:41 AM
 */

#include "HistProcessor.h"
#include "RootHelper.h"
#include <iostream>
#include <TF1.h>
#include <RooAbsPdf.h>

HistProcessor::HistProcessor(){};

HistProcessor::HistProcessor(HistProcessor const& fileUtil){};

HistProcessor* HistProcessor::instance = NULL;

HistProcessor* HistProcessor::getInstance(){
	if (!instance){
		instance = new HistProcessor;
	}
	return instance;
}

TH1* HistProcessor::cutHist(TH1* fullHist, Int_t xMin, Int_t xMax){
	Int_t minBin = fullHist->FindBin(xMin);
	Int_t maxBin = fullHist->FindBin(xMax);
	Int_t nBins = maxBin - minBin + 1;

	//std::cout << "reduceHist: miBin - " << minBin << ", maxBin - " << maxBin << std::endl;

	// Construct new histogram
	Double_t lowEdge = fullHist->GetXaxis()->GetBinLowEdge(minBin);
	Double_t upEdge = fullHist->GetXaxis()->GetBinUpEdge(maxBin);

	//std::cout << "reduceHist: lowEdge - " << lowEdge << ", upEdge - " << upEdge << std::endl;
        RootHelper::deleteObject("subHist");
	TH1* subHist = new TH1F("subHist", "Counts Histogram", nBins, lowEdge, upEdge);
	for (int j = minBin; j <= maxBin; j++){
		subHist->SetBinContent(j - minBin + 1, fullHist->GetBinContent(j));
		subHist->SetBinError(j - minBin + 1, fullHist->GetBinError(j));
	}

	return subHist;
}

RooCurve* HistProcessor::subtractCurves(RooCurve* curveFit, RooCurve* curveBg){
	RooCurve* c = (RooCurve*)curveFit->Clone("FitNoBg");
	// std::cout << "Curve No Bg:" << std::endl;
	for (Int_t i = 0; i < curveFit->GetN(); i++){
		Double_t energy;
		Double_t fit;
		Double_t bg;
		curveFit->GetPoint(i, energy, fit);

		if (curveBg != NULL){
			bg = curveBg->Eval(energy);
		}
		else {
			bg = 0;
		}
		c->SetPoint(i, energy, fit - bg);
		//std::cout << energy << " " << fit << " " << bg << " " <<  fit - bg << std::endl;
	}
	return c;
}

TH1* HistProcessor::subtractCurve(TH1* hist, RooCurve* curve){
	Double_t xMin = hist->GetXaxis()->GetXmin();
	Double_t xMax = hist->GetXaxis()->GetXmax();
	Double_t nBins = hist->GetXaxis()->GetNbins();

        RootHelper::deleteObject("histMinusCurve");
	TH1* histMinusCurve = new TH1F("histMinusCurve", "Histogram Minus Curve", nBins, xMin, xMax);
	for (int i = 1; i <= nBins; i++){
		Double_t bg = (curve == NULL) ? 0.0 : curve->Eval(hist->GetXaxis()->GetBinCenter(i));
		if (hist->GetBinContent(i) - bg >= 0) {
			histMinusCurve->SetBinContent(i, hist->GetBinContent(i) - bg);
			histMinusCurve->SetBinError(i, hist->GetBinError(i));
		}
		else {
			histMinusCurve->SetBinContent(i, 0);
			histMinusCurve->SetBinError(i, 0);
		}
	}
	return histMinusCurve;
}

TH1* HistProcessor::getChi2Hist(TH1* hist, RooCurve* curve){
	Double_t xMin = hist->GetXaxis()->GetXmin();
	Double_t xMax = hist->GetXaxis()->GetXmax();
	Double_t nBins = hist->GetXaxis()->GetNbins();

        RootHelper::deleteObject("chiHist");
	TH1* chiHist = new TH1F("chiHist", "Chi2 Histogram", nBins, xMin, xMax);
	for (int i = 1; i <= nBins; i++){
		Double_t count = hist->GetBinContent(i);
		Double_t error = sqrt(count);//hist -> GetBinError(i);
		Double_t theor = curve->Eval(hist->GetXaxis()->GetBinCenter(i));
		if (theor != 0){
                        // We want the chi^2 to be positive and negative
			Double_t chi2 = (count - theor) * std::abs(count - theor) / pow(error, 2);
                        // std::cout << hist->GetBinCenter(i) << " " << count << " " << theor << " " << chi2 << std::endl;
			chiHist->SetBinContent(i, chi2);
		}
	}
	return chiHist;
}

Double_t HistProcessor::getTotalCounts(TH1* hist){
    Double_t counts = 0;
    Double_t nBins = hist->GetXaxis()->GetNbins();
    for (int i = 1; i <= nBins; i++){
        counts += hist->GetBinContent(i);
    }
    return counts;
}

Bool_t HistProcessor::hasAtan(TH1* hist){
	const Int_t wingBins = 10;
	Double_t leftWingMeanValue, rightWingMeanValue;
	Int_t nBins = hist->GetXaxis()->GetNbins();
	leftWingMeanValue = (hist->Integral(1, 1 + (wingBins - 1))) / (Double_t)wingBins;
	rightWingMeanValue = (hist->Integral(nBins - (wingBins - 1), nBins)) / (Double_t)wingBins;
	if (leftWingMeanValue > 2 && rightWingMeanValue > 2 && leftWingMeanValue > rightWingMeanValue * 2){
		return true;
	}
	return false;
}

Double_t HistProcessor::calcBackgroundFraction(TH1* hist){
	Int_t nBins = hist->GetXaxis()->GetNbins();
	const Int_t wingBins = 10;
	Double_t fullInt = hist->Integral(1, nBins);
	Double_t rightWingAverage = (hist->Integral(nBins - (wingBins - 1), nBins)) / (Double_t)(wingBins);
	Double_t bgInt = nBins * rightWingAverage;
	Double_t bg = bgInt / fullInt;
	return bg > 0 ? bg : 0.1;
}

std::pair<Double_t, Int_t> HistProcessor::getChi2(TH1* hist, RooCurve* curve, Int_t numberOfFreeParameters){
	Double_t nBins = hist->GetXaxis()->GetNbins();

	Int_t degreesOfFreedom = 0;

	Double_t sum = 0;
	for (int i = 1; i <= nBins; i++){
		Double_t value = hist->GetBinContent(i);
		Double_t fit = curve->Eval(hist->GetXaxis()->GetBinCenter(i));
		Double_t error = hist->GetBinError(i);
		if (value != 0 && error != 0){
			// std::cout << "value: " << value << " fit: " << fit << "  error: " << error << std::endl;
			sum += pow(value - fit, 2) / value;
			degreesOfFreedom++;
		}
	}
	degreesOfFreedom -= (numberOfFreeParameters + 1);
	std::cout << "sum: " << sum << std::endl;
	std::cout << "number of free prameters: " << numberOfFreeParameters << std::endl;
	std::cout << "degrees of freedom: " << degreesOfFreedom << std::endl;
	// Double_t chi2Int = sum / (Double_t)(degreesOfFreedom);
	// Double_t chi2IntErr = sqrt((double)2*degreesOfFreedom);
	return std::make_pair(sum, degreesOfFreedom);
}

std::pair<Double_t, Double_t> HistProcessor::calcIntegral(TH1* hist, Double_t min, Double_t max){
	int minBin = hist->FindBin(min);
	int maxBin = hist->FindBin(max);
	Double_t sum = 0;
	Double_t error;
	//cout <<  minBin << " " << maxBin << endl;
	sum += ((hist->GetXaxis()->GetBinUpEdge(minBin)) - min) * (hist->GetBinContent(minBin));
	if (minBin + 1 <= maxBin - 1){
		sum += hist->IntegralAndError(minBin + 1, maxBin - 1, error, "width");
	}
	sum += (max - (hist->GetXaxis()->GetBinLowEdge(maxBin))) * (hist->GetBinContent(maxBin));
	return std::make_pair(sum, error);
}

Bool_t HistProcessor::isTwoDetetor(TH1* hist){
	TAxis* x = hist->GetXaxis();
	return !((x->GetXmin() < 511) && (x->GetXmax() > 511));
}

Double_t HistProcessor::getPdfMaximumX(RooAbsPdf* pdf, const RooArgList& args){
    TF1* tf1 = pdf->asTF(args);
    return tf1->GetMaximumX();
}