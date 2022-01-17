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
#include <cmath>
#include <TMath.h>
#include <TDatime.h>
#include "Debug.h"
#include <cmath>

HistProcessor::HistProcessor(){};

HistProcessor::HistProcessor(HistProcessor const& fileUtil){};

HistProcessor* HistProcessor::instance = NULL;

HistProcessor* HistProcessor::getInstance(){
	if (!instance){
		instance = new HistProcessor();
	}
	return instance;
}

Int_t HistProcessor::getLeftNonZeroBin(TH1F* hist){
    // Going from center of the histogram X axis towards the left side
    for (Int_t bin = hist->GetNbinsX()/2; bin >= 1; bin--){
        if (hist->GetBinContent(bin) <= 0) return bin+1;
    }
    return 1;
}

Int_t HistProcessor::getRightNonZeroBin(TH1F* hist){
    // Going from center of the histogram X axis towards the left side
    for (Int_t bin = hist->GetNbinsX()/2; bin <= hist->GetNbinsX(); bin++){
        if (hist->GetBinContent(bin) <= 0) return bin-1;
    }
    return hist->GetNbinsX();
}

TH1F* HistProcessor::cutHistZeros(TH1F* hist){
    // Check if histogram needs trimming
    Int_t newMinBin = HistProcessor::getLeftNonZeroBin(hist);
    Int_t newMaxBin = HistProcessor::getRightNonZeroBin(hist);

    if (newMinBin == 1 && newMaxBin == hist->GetNbinsX()) return hist;

    // Histogram needs trimming
    TString newName = hist->GetName();
    newName += "-trim";
    return cutHist(newName, hist, newMinBin, newMaxBin);
}

TH1F* HistProcessor::cutHistBasement(const char *newname, TH1F* hist, Int_t xMin, Int_t xMax){
	Int_t minBin = hist->FindBin(xMin);
	Int_t maxBin = hist->FindBin(xMax);
	if (minBin <= 1) minBin = 1;
	if (maxBin >= hist->GetXaxis()->GetNbins()) maxBin = hist->GetXaxis()->GetNbins();
	Int_t nBins = maxBin - minBin + 1;

	Double_t leftVal = hist->GetBinContent(minBin);
	Double_t rightVal = hist->GetBinContent(maxBin);
	Double_t baseVal = (leftVal + rightVal)/2;

	//std::cout << "reduceHist: miBin - " << minBin << ", maxBin - " << maxBin << std::endl;

	// Construct new histogram
	Double_t lowEdge = hist->GetXaxis()->GetBinLowEdge(minBin);
	Double_t upEdge = hist->GetXaxis()->GetBinUpEdge(maxBin);

	//std::cout << "reduceHist: lowEdge - " << lowEdge << ", upEdge - " << upEdge << std::endl;
	RootHelper::deleteObject(newname);
	TH1F* subHist = new TH1F(newname, "Histogram with cutted basement", nBins, lowEdge, upEdge);
	for (int j = minBin; j <= maxBin; j++){
		Double_t val = hist->GetBinContent(j) - baseVal;
		subHist->SetBinContent(j - minBin + 1, val < 0 ? 0 : val);
		subHist->SetBinError(j - minBin + 1, sqrt(val));
	}
	return subHist;
}

Double_t HistProcessor::getHistMaximumInRange(TH1* hist, Double_t xMin, Double_t xMax){
    Int_t minBin = hist->FindBin(xMin);
    Int_t maxBin = hist->FindBin(xMax);
    return getHistMaximumInRange(hist, minBin, maxBin);
}

Double_t HistProcessor::getHistMaximumInRange(TH1* hist, Int_t minBin, Int_t maxBin){
    Double_t max = std::numeric_limits<double>::min();
    for (Int_t bin = minBin; bin <= maxBin; bin++){
        max = TMath::Max(max, hist->GetBinContent(bin));
    }
    return max;
}

//Double_t HistProcessor::liftHist(TH1F* hist, Double_t lift){
//	for (UInt_t i = 1; i <= hist->GetNbinsX(); i++){
//		Double_t value = hist->GetBinContent(i);
//		hist->SetBinContent(i, value + lift);
//	}
//	return lift;
//}

TH1F* HistProcessor::removeHistNegatives(const char *newname, TH1F* hist){
	Double_t xMin = hist->GetXaxis()->GetXmin();
	Double_t xMax = hist->GetXaxis()->GetXmax();
	Double_t nBins = hist->GetXaxis()->GetNbins();

	RootHelper::deleteObject(newname);
	TH1F* newHist = new TH1F(newname, "Histogram with subtracted negatives", nBins, xMin, xMax);
	for (int i = 1; i <= nBins; i++){
		Double_t value = hist->GetBinContent(i);
		newHist->SetBinContent(i, value < 0 ? 0 : value);
		Double_t error = hist->GetBinError(i);
		newHist->SetBinError(i, error);
	}
	return newHist;
}

TH1F* HistProcessor::cutHist(const char *newname, TH1F* hist, Int_t minBin, Int_t maxBin, Bool_t setBinErrors){
	#ifdef USEDEBUG
		std::cout << "HistProcessor::cutHist" << std::endl;
		std::cout << "original histogram" << std::endl;
		hist->Print("base");
//		hist->Print("range");
	#endif

//	Int_t minBin = hist->GetXaxis()->GetFirst();
//	Int_t maxBin = hist->GetXaxis()->GetLast();

	// Construct new histogram
	Double_t lowEdge = hist->GetXaxis()->GetBinLowEdge(minBin);
	Double_t upEdge = hist->GetXaxis()->GetBinUpEdge(maxBin);

	// Int_t id = (new TDatime())->Get();

	// RootHelper::deleteObject(newname);
	TH1F* trimmedHist = new TH1F(newname, hist->GetTitle(), maxBin-minBin+1, lowEdge, upEdge);
	for (Int_t i = 1; i <= trimmedHist->GetXaxis()->GetNbins(); i++){
		for (int v=0; v < hist->GetBinContent(i+minBin-1); v++){
			Double_t binCenter = hist->GetBinCenter(i+minBin-1);
			trimmedHist->Fill(binCenter);
		}
		if (setBinErrors) trimmedHist->SetBinError(i, hist->GetBinError(i+minBin-1));
	}

	#ifdef USEDEBUG
	    trimmedHist->Print("base");
//	    trimmedHist->Print("range");
    #endif

	return trimmedHist;
}

RooCurve* HistProcessor::subtractCurves(const char *newname, RooCurve* curveFit, RooCurve* curveBg){
//	RootHelper::deleteObject(newname);
	RooCurve* c = (RooCurve*)curveFit->Clone(newname);
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

TH1F* HistProcessor::subtractCurve(const char *newname, TH1F* hist, RooCurve* curve){
	Double_t xMin = hist->GetXaxis()->GetXmin();
	Double_t xMax = hist->GetXaxis()->GetXmax();
	Double_t nBins = hist->GetXaxis()->GetNbins();

//	RootHelper::deleteObject("histMinusCurve");
	TH1F* histMinusCurve = new TH1F(newname, "Histogram minus curve", nBins, xMin, xMax);
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

TH1F* HistProcessor::getResidualHist(const char *newname, TH1F* hist, RooCurve* curve) {
	Double_t xMin = hist->GetXaxis()->GetXmin();
	Double_t xMax = hist->GetXaxis()->GetXmax();
	Double_t nBins = hist->GetXaxis()->GetNbins();

	RootHelper::deleteObject(newname);
	TH1F* resHist = new TH1F(newname, "Residual histogram", nBins, xMin, xMax);
	for (int i = 1; i <= nBins; i++){
		Double_t count = hist->GetBinContent(i);
		Double_t error = sqrt(count);//hist -> GetBinError(i);
		Double_t theor = curve->Eval(hist->GetXaxis()->GetBinCenter(i));
		// We want the chi^2 to be positive and negative
		Double_t res = (count - theor) / error;
		if(std::isfinite(res)) {
			resHist->SetBinContent(i, res);
		}
		else {
			resHist->SetBinContent(i, 0);
			std::cout << "Histogram at E=" << hist->GetBinCenter(i) << " (bin " << i << ") has count " << count << std::endl;
		}
	}
	return resHist;
}

TH1F* HistProcessor::getChi2Hist(const char *newname, TH1F* hist, RooCurve* curve){
	Double_t xMin = hist->GetXaxis()->GetXmin();
	Double_t xMax = hist->GetXaxis()->GetXmax();
	Double_t nBins = hist->GetXaxis()->GetNbins();

	// RootHelper::deleteObject(newname);
	TH1F* chiHist = new TH1F(newname, "Chi2 histogram", nBins, xMin, xMax);
	for (int i = 1; i <= nBins; i++){
		Double_t count = hist->GetBinContent(i);
		Double_t error = sqrt(count);//hist -> GetBinError(i);
		Double_t theor = curve->Eval(hist->GetXaxis()->GetBinCenter(i));
		// We want the chi^2 to be positive and negative
		Double_t chi2 = (count - theor) * std::abs(count - theor) / pow(error, 2);
		if(std::isfinite(chi2)) {
			chiHist->SetBinContent(i, chi2);
		}
		else {
			chiHist->SetBinContent(i, 0);
			std::cout << "Histogram at E=" << hist->GetBinCenter(i) << " (bin " << i << ") has count " << count << std::endl;
		}
	}
	return chiHist;
}

Double_t HistProcessor::getTotalCounts(TH1F* hist){
	Double_t counts = 0;
	Double_t nBins = hist->GetXaxis()->GetNbins();
	for (int i = 1; i <= nBins; i++){
	counts += hist->GetBinContent(i);
	}
	return counts;
}

Bool_t HistProcessor::hasAtan(TH1F* hist){
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

Double_t HistProcessor::calcLeftWing(TH1F* hist){
    Double_t leftWingIntegral = hist->Integral(1, wingBins, "width");
    Double_t leftWingWidth = hist->GetXaxis()->GetBinUpEdge(wingBins) - hist->GetXaxis()->GetBinLowEdge(1);
    return leftWingIntegral/leftWingWidth;
}

Double_t HistProcessor::calcRightWing(TH1F* hist){
    Int_t nBins = hist->GetXaxis()->GetNbins();
    Double_t rightWingIntegral = hist->Integral(nBins-wingBins+1, nBins, "width");
    Double_t rightWingWidth = (hist->GetXaxis()->GetBinUpEdge(nBins) - hist->GetXaxis()->GetBinLowEdge(nBins-wingBins+1));
    return rightWingIntegral/rightWingWidth;
}

Double_t HistProcessor::calcRectBackgroundFraction(TH1F* hist){
	Double_t leftWingAverage = calcLeftWing(hist);
	Double_t rightWingAverage = calcRightWing(hist);

	Double_t histBackgroundRectArea = TMath::Min(leftWingAverage, rightWingAverage) * (hist->GetXaxis()->GetXmax() - hist->GetXaxis()->GetXmin());
	Double_t histIntegral = hist->Integral("width");
	return histBackgroundRectArea / histIntegral;
}

Double_t HistProcessor::calcAsymBackgroundFraction(TH1F* hist){
    Int_t nBins = hist->GetXaxis()->GetNbins();
    // Int_t maxBin = hist->GetMinimumBin();
    const Int_t wingBins = 10;
    Double_t leftWingAverage = (hist->Integral(1, wingBins, "width")) / (hist->GetXaxis()->GetBinUpEdge(wingBins) - hist->GetXaxis()->GetBinLowEdge(1));
    Double_t rightWingAverage = (hist->Integral(nBins-wingBins+1, nBins, "width")) / (hist->GetXaxis()->GetBinUpEdge(nBins) - hist->GetXaxis()->GetBinLowEdge(nBins-wingBins+1));

    Double_t histAsymBackgroundRectArea = TMath::Abs(leftWingAverage-rightWingAverage) * (hist->GetXaxis()->GetXmax() - hist->GetXaxis()->GetXmin()) / 2;
    Double_t histIntegral = hist->Integral("width");
    return histAsymBackgroundRectArea / histIntegral;
}


Chi2Struct HistProcessor::getChi2(TH1F* hist, RooCurve* curve, RooAbsPdf* model){
	// https://en.wikipedia.org/wiki/Goodness_of_fit
	Int_t degreesOfFreedom = 0;
	Double_t chiSum = 0;
	for (int i = 1; i <= hist->GetXaxis()->GetNbins(); i++){
	Double_t value = hist->GetBinContent(i);
	Double_t fit = curve->Eval(hist->GetXaxis()->GetBinCenter(i));
	Double_t error = hist->GetBinError(i);
	if (value != 0 && error != 0){
		// std::cout << "value: " << value << " fit: " << fit << "  error: " << error << std::endl;
		chiSum += pow(value - fit, 2) / value;
		degreesOfFreedom++;
	}
	}
	// Subtract number of free parameters + 1
	degreesOfFreedom -= (model->getVariables()->getSize() + 1);

	Chi2Struct chi2Struct = {chiSum, degreesOfFreedom, chiSum / (Double_t)(degreesOfFreedom)};

	//	Double_t chi2Err = TMath::Sqrt((Double_t)2 * freeParameters) / degreesFreedom;
	return chi2Struct;
}

std::pair<Double_t, Double_t> HistProcessor::calcIntegral(TH1F* hist, Double_t min, Double_t max){
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

RooRealVar* HistProcessor::getSParameter(TH1F* hist, Double_t sWidth, Double_t mean) {
	// Calculate full histogram integral
	if (isTwoDetector(hist)) sWidth*=2;
	std::cout << "Calculating S parameter. Mean " << mean << ", sWidth " <<  sWidth << std::endl;
	std::pair<Double_t, Double_t> fullInt;
	fullInt.first = hist->IntegralAndError(1, hist->GetXaxis()->GetNbins(), fullInt.second, "width");
	std::cout << "Full Integral: " << fullInt.first << " ± " << fullInt.second << std::endl;
	std::pair<Double_t, Double_t> sInt = calcIntegral(hist, mean - sWidth/2, mean + sWidth/2);
	std::cout << "S Integral: " << sInt.first << " ± " << sInt.second << std::endl;
	Double_t s = sInt.first / fullInt.first;
	Double_t sError = sqrt(pow((1 / fullInt.first)*(sInt.second), 2) + pow((sInt.first / fullInt.first / fullInt.first*fullInt.second), 2));
	RooRealVar* sRealVar = new RooRealVar("sParameter", "S Parameter", s);
	sRealVar->setError(sError);
	return sRealVar;
}

RooRealVar* HistProcessor::getWParameter(TH1F* hist, Double_t wWidth, Double_t wShift, Double_t mean) {
	// Calculate full histogram integral
	if (isTwoDetector(hist)){
	    wWidth*=2;
	    wShift*=2;
	}
	std::cout << "Calculating W parameter. Mean " << mean << ", wWidth " <<  wWidth << ", wShift " << wShift << std::endl;
	std::pair<Double_t, Double_t> fullInt;
	fullInt.first = hist->IntegralAndError(1, hist->GetXaxis()->GetNbins(), fullInt.second, "width");
	std::cout << "Full Integral: " << fullInt.first << " ± " << fullInt.second << std::endl;
	std::pair<Double_t, Double_t> w1Int = calcIntegral(hist, mean - wShift - wWidth, mean - wShift);
	std::cout << "W1 Integral: " << w1Int.first << " ± " << w1Int.second << std::endl;
	std::pair<Double_t, Double_t> w2Int = calcIntegral(hist, mean + wShift, mean + wShift + wWidth);
	std::cout << "W2 Integral: " << w2Int.first << " ± " << w2Int.second << std::endl;
	Double_t w = (w1Int.first + w2Int.first) / fullInt.first;
	Double_t wError = sqrt(pow((1 / fullInt.first)*(w1Int.second), 2) + pow((1 / fullInt.first)*(w2Int.second), 2) + pow(((w1Int.first + w2Int.first) / fullInt.first / fullInt.first*fullInt.second), 2));
	RooRealVar* wRealVar = new RooRealVar("wParameter", "W Parameter", w);
	wRealVar->setError(wError);
	return wRealVar;
}

Bool_t HistProcessor::isTwoDetector(TH1* hist){
	if (hist->GetXaxis()->GetXmin() < 511 && hist->GetXaxis()->GetXmax() > 511){
		return kFALSE;
	}
	return kTRUE;
}

Double_t HistProcessor::getPdfMaximumX(RooAbsPdf* pdf, const RooArgList& args){
	TF1* tf1 = pdf->asTF(args);
	return tf1->GetMaximumX();
}

std::pair<Double_t, Double_t> HistProcessor::getHistogramSafeFitRange(TH1F* hist){
	Int_t maxBin = hist->GetMaximumBin();
	Int_t firstSafeBin = 1; // First bin with zero value (if )
	for (Int_t i = 1; i < maxBin; i++){
		if (hist->GetBinContent(i) < 1){
			firstSafeBin = i+1;
		}
	}
	Int_t nbins = hist->GetXaxis()->GetNbins();
	Int_t lastSafeBin = nbins;
	for (Int_t i = nbins; i > maxBin; i--){
		if (hist->GetBinContent(i) < 1){
			lastSafeBin = i-1;
		}
	}
	return std::make_pair(hist->GetXaxis()->GetBinLowEdge(firstSafeBin), hist->GetXaxis()->GetBinUpEdge(lastSafeBin));
}
