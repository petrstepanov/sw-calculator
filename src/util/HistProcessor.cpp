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

HistProcessor::HistProcessor(){};

HistProcessor::HistProcessor(HistProcessor const& fileUtil){};

HistProcessor* HistProcessor::instance = NULL;

HistProcessor* HistProcessor::getInstance(){
	if (!instance){
		instance = new HistProcessor();
	}
	return instance;
}

TH1* HistProcessor::cutHistBasement(const char *newname, TH1* hist, Int_t xMin, Int_t xMax){
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
//        RootHelper::deleteObject(newname);
	TH1* subHist = new TH1F(newname, "Histogram with cutted basement", nBins, lowEdge, upEdge);
	for (int j = minBin; j <= maxBin; j++){
            Double_t val = hist->GetBinContent(j) - baseVal;
            subHist->SetBinContent(j - minBin + 1, val < 0 ? 0 : val);
            subHist->SetBinError(j - minBin + 1, sqrt(val));
	}
	return subHist;
}

TH1* HistProcessor::cutHist(const char *newname, TH1* hist, Int_t xMin, Int_t xMax){
	Int_t minBin = hist->FindBin(xMin);
	Int_t maxBin = hist->FindBin(xMax);
        if (minBin <= 1) minBin = 1;
        if (maxBin >= hist->GetXaxis()->GetNbins()) maxBin = hist->GetXaxis()->GetNbins();
	Int_t nBins = maxBin - minBin + 1;
        
	//std::cout << "reduceHist: miBin - " << minBin << ", maxBin - " << maxBin << std::endl;

	// Construct new histogram
	Double_t lowEdge = hist->GetXaxis()->GetBinLowEdge(minBin);
	Double_t upEdge = hist->GetXaxis()->GetBinUpEdge(maxBin);

	//std::cout << "reduceHist: lowEdge - " << lowEdge << ", upEdge - " << upEdge << std::endl;
//        RootHelper::deleteObject(newname);
	TH1* subHist = new TH1F(newname, "Cutted histogram", nBins, lowEdge, upEdge);
	for (int j = minBin; j <= maxBin; j++){
		subHist->SetBinContent(j - minBin + 1, hist->GetBinContent(j));
		subHist->SetBinError(j - minBin + 1, hist->GetBinError(j));
	}
	return subHist;
}

RooCurve* HistProcessor::subtractCurves(const char *newname, RooCurve* curveFit, RooCurve* curveBg){
//        RootHelper::deleteObject(newname);    
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

TH1* HistProcessor::subtractCurve(const char *newname, TH1* hist, RooCurve* curve){
	Double_t xMin = hist->GetXaxis()->GetXmin();
	Double_t xMax = hist->GetXaxis()->GetXmax();
	Double_t nBins = hist->GetXaxis()->GetNbins();

//        RootHelper::deleteObject("histMinusCurve");
	TH1* histMinusCurve = new TH1F(newname, "Histogram minus curve", nBins, xMin, xMax);
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

TH1* HistProcessor::getResidualHist(const char *newname, TH1* hist, RooCurve* curve) {
	Double_t xMin = hist->GetXaxis()->GetXmin();
	Double_t xMax = hist->GetXaxis()->GetXmax();
	Double_t nBins = hist->GetXaxis()->GetNbins();

        RootHelper::deleteObject(newname);
	TH1* resHist = new TH1F(newname, "Residual histogram", nBins, xMin, xMax);
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

TH1* HistProcessor::getChi2Hist(const char *newname, TH1* hist, RooCurve* curve){
	Double_t xMin = hist->GetXaxis()->GetXmin();
	Double_t xMax = hist->GetXaxis()->GetXmax();
	Double_t nBins = hist->GetXaxis()->GetNbins();

//        RootHelper::deleteObject(newname);
	TH1* chiHist = new TH1F(newname, "Chi2 histogram", nBins, xMin, xMax);
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
        Int_t maxBin = hist->GetMinimumBin();
	const Int_t wingBins = 10;
	Double_t leftWingAverage = (hist->Integral(1, wingBins)) / (Double_t)(wingBins);
	Double_t rightWingAverage = (hist->Integral(nBins - (wingBins - 1), nBins)) / (Double_t)(wingBins);
        Double_t backgroundCounts = leftWingAverage*(maxBin) + rightWingAverage*(nBins-maxBin);
	Double_t fullInt = hist->Integral(1, nBins);
	Double_t bgFraction = backgroundCounts / fullInt;
	return bgFraction;
}

std::pair<Double_t, Int_t> HistProcessor::getChi2(TH1* hist, RooCurve* curve, RooAbsPdf* model){
    RooArgSet* params = model->getVariables();
    params->Print("v");
    Int_t numberOfFreeParameters = params->getSize();   
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

std::pair<Double_t, Double_t> HistProcessor::getSParameter(TH1* hist, Double_t sWidth, Double_t mean, Bool_t isTwoDetector) {
    // Calculate full histogram integral
    if (isTwoDetector) sWidth*=2;
    std::cout << "Calculating S parameter. Mean " << mean << ", sWidth " <<  sWidth << std::endl;
    std::pair<Double_t, Double_t> fullInt;    
    fullInt.first = hist->IntegralAndError(1, hist->GetXaxis()->GetNbins(), fullInt.second, "width");
    std::cout << "Full Integral: " << fullInt.first << " ± " << fullInt.second << std::endl;
    std::pair<Double_t, Double_t> sInt = calcIntegral(hist, mean - sWidth/2, mean + sWidth/2);
    std::cout << "S Integral: " << sInt.first << " ± " << sInt.second << std::endl;
    Double_t S = sInt.first / fullInt.first;
    Double_t dS = sqrt(pow((1 / fullInt.first)*(sInt.second), 2) + pow((sInt.first / fullInt.first / fullInt.first*fullInt.second), 2));
    return std::make_pair(S, dS);
}

std::pair<Double_t, Double_t> HistProcessor::getWParameter(TH1* hist, Double_t wWidth, Double_t wShift, Double_t mean, Bool_t isTwoDetector) {
    // Calculate full histogram integral
    if (isTwoDetector){ wWidth*=2; wShift*=2; }
    std::cout << "Calculating W parameter. Mean " << mean << ", wWidth " <<  wWidth << ", wShift " << wShift << std::endl;    
    std::pair<Double_t, Double_t> fullInt;
    fullInt.first = hist->IntegralAndError(1, hist->GetXaxis()->GetNbins(), fullInt.second, "width");
    std::cout << "Full Integral: " << fullInt.first << " ± " << fullInt.second << std::endl;
    std::pair<Double_t, Double_t> w1Int = calcIntegral(hist, mean - wShift - wWidth, mean - wShift);
    std::cout << "W1 Integral: " << w1Int.first << " ± " << w1Int.second << std::endl;
    std::pair<Double_t, Double_t> w2Int = calcIntegral(hist, mean + wShift, mean + wShift + wWidth);
    std::cout << "W2 Integral: " << w2Int.first << " ± " << w2Int.second << std::endl;
    Double_t W = (w1Int.first + w2Int.first) / fullInt.first;
    Double_t dW = sqrt(pow((1 / fullInt.first)*(w1Int.second), 2) + pow((1 / fullInt.first)*(w2Int.second), 2) + pow(((w1Int.first + w2Int.first) / fullInt.first / fullInt.first*fullInt.second), 2));
    return std::make_pair(W, dW);
}

Bool_t HistProcessor::isTwoDetetor(TH1* hist){
	TAxis* x = hist->GetXaxis();
	return !((x->GetXmin() < 511) && (x->GetXmax() > 511));
}

Double_t HistProcessor::getPdfMaximumX(RooAbsPdf* pdf, const RooArgList& args){
    TF1* tf1 = pdf->asTF(args);
    return tf1->GetMaximumX();
}

std::pair<Double_t, Double_t> HistProcessor::getHistogramSafeFitRange(TH1* hist){
    Int_t maxBin = hist->GetMaximumBin();
    Int_t firstBin;
    for (Int_t i = 1; i < maxBin; i++){
        if (hist->GetBinContent(i) < 1){
            firstBin = i;
        }
    }
    firstBin++;
    Int_t lastBin;
    Int_t nbins = hist->GetXaxis()->GetNbins();
    for (Int_t i = nbins; i > maxBin; i--){
        if (hist->GetBinContent(i) < 1){
            lastBin = i;
        }
    }
    lastBin--;
    return std::make_pair(hist->GetXaxis()->GetBinLowEdge(firstBin), hist->GetXaxis()->GetBinUpEdge(lastBin));
}