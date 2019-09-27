/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Model.cpp
 * Author: petrstepanov
 * 
 * Created on October 6, 2017, 2:35 AM
 */

#include "Model.h"
#include "../util/HistProcessor.h"

Model* Model::instance = NULL;

Model* Model::getInstance(){
    
    if (!instance){
        instance = new Model();
    }
    return instance;
}

Model::Model(){
	// Initial variable values
    strFileName = new TString();
    strSourceContribFileName = new TString();

    // TODO: Save integration parameter values here
    // sWidth = 1;
    // wWidth = 2;
    // wShift = 1;

    // Histograms
//	peakHistNoBg = nullptr;
//	chiHist = nullptr;

    // Fitting model properties
	fitProperties.fitMin = new RooRealVar("fitMin", "Fit energy minimum", -400, -500, 0, "keV");
	fitProperties.fitMax = new RooRealVar("fitMax", "Fit energy maximum", 400, 0, 500, "keV");
	fitProperties.isTwoDetector = kFALSE;
	fitProperties.convolutionType = kNoConvolution;
	fitProperties.hasParabola = kFALSE;
	fitProperties.numberOfGaussians = 1;
	fitProperties.numberOfExponents = 0;
	fitProperties.numberOfDampingExponents = 0;
	fitProperties.hist = nullptr;
	fitProperties.sourceHist = nullptr;
	fitProperties.componentHist = nullptr;

	parametersPool = nullptr;
};

void Model::setFileName(TString* fileName){
    strFileName = fileName;
}

TString* Model::getFileName(){
    return strFileName;
}

void Model::setSourceFileName(TString* fileName){
    strSourceContribFileName = fileName;
}

TString* Model::getSourceFileName(){
    return strSourceContribFileName;
}

void Model::setHist(TH1F* hist){
	// Save original histogram to Model
    fitProperties.hist = hist;
    parametersPool = new ParametersPool();
    histogramImported(hist);
}

void Model::setSourceHist(TH1F* sourceHist){
    fitProperties.sourceHist = sourceHist;
    sourceHistogramImported(sourceHist);
}

void Model::setComponentHist(TH1F* componentHist){
    fitProperties.componentHist = componentHist;
    componentHistogramImported(componentHist);
}

void Model::setTwoDetector(Bool_t isTwoDetector){
    fitProperties.isTwoDetector = isTwoDetector;
}

Bool_t Model::isTwoDetector(){
    return fitProperties.isTwoDetector;
}

void Model::setFitRangeLimits(Double_t fitMinLo, Double_t fitMinHi, Double_t fitMaxLo, Double_t fitMaxHi){
	// Set inner fit range limits
    fitProperties.fitMin->setMax(fitMinHi);
    fitProperties.fitMax->setMin(fitMaxLo);

	// Set outer fit range limits
    setFitRangeLimits(fitMinLo, fitMaxHi);
}

void Model::setFitRangeLimits(Double_t fitMinLo, Double_t fitMaxHi){
	// Set fit range limits
    fitProperties.fitMin->setMin(fitMinLo);
    fitProperties.fitMax->setMax(fitMaxHi);

    // Emit new fit range limits
    DoublePair* limits = new DoublePair(fitProperties.fitMin->getMin(), fitProperties.fitMax->getMax());
    fitRangeLimitsSet(limits);

    // Fit maximum value could have changed (via RooRealVar logic)
    DoublePair* fitRange = new DoublePair(fitProperties.fitMin->getVal(), fitProperties.fitMax->getVal());
    fitRangeSet(fitRange);
}

std::pair<Double_t, Double_t> Model::getFitRangeLimits(){
	return std::make_pair(fitProperties.fitMin->getMin(), fitProperties.fitMax->getMax());
}

void Model::setFitRange(Double_t fitMin, Double_t fitMax){
    fitProperties.fitMin->setVal(fitMin);
    fitProperties.fitMax->setVal(fitMax);

    // Fit maximum value could have changed (via RooRealVar logic)
    DoublePair* fitRange = new DoublePair(fitProperties.fitMin->getVal(), fitProperties.fitMax->getVal());
    fitRangeSet(fitRange);
}

std::pair<Double_t, Double_t> Model::getFitRange(){
	return std::make_pair(fitProperties.fitMin->getVal(), fitProperties.fitMax->getVal());
}


// Fit Properties setters getters

void Model::setConvolutionType(ConvolutionType t){
	fitProperties.convolutionType = t;
	convolutionTypeSet(t);
}

ConvolutionType Model::getConvolutionType(){
	return fitProperties.convolutionType;
}

void Model::setHasParabola(Bool_t b){
	fitProperties.hasParabola = b;
	hasParabolaSet(b);
}

Bool_t Model::getHasParabola(){
	return fitProperties.hasParabola;
}

void Model::setNumberOfGaussians(Int_t num){
	fitProperties.numberOfGaussians = num;
	numberOfGaussiansSet(num);
}

Int_t Model::getNumberOfGaussians(){
	return fitProperties.numberOfGaussians;
}

void Model::setNumberOfExponents(Int_t num){
	fitProperties.numberOfExponents = num;
	numberOfExponentsSet(num);
}

Int_t Model::getNumberOfExponents(){
	return fitProperties.numberOfExponents;
}

void Model::setNumberOfDampingExponents(Int_t num){
	fitProperties.numberOfDampingExponents = num;
	numberOfDampingExponentsSet(num);
}

Int_t Model::getNumberOfDampingExponents(){
	return fitProperties.numberOfDampingExponents;
}

FitProperties Model::getFitProperties(){
	return fitProperties;
}

ParametersPool* Model::getParametersPool(){
	return parametersPool;
}

// SIGNALS

void Model::fitRangeSet(DoublePair* pair){
	Emit("fitRangeSet(DoublePair*)", pair);
}

void Model::fitRangeLimitsSet(DoublePair* pair){
	Emit("fitRangeLimitsSet(DoublePair*)", pair);
}

void Model::histogramImported(TH1F* hist){
	Emit("histogramImported(TH1F*)", hist);
}

void Model::sourceHistogramImported(TH1F* hist){
	Emit("sourceHistogramImported(TH1F*)", hist);
}

void Model::componentHistogramImported(TH1F* hist){
	Emit("componentHistogramImported(TH1F*)", hist);
}

void Model::twoDetectorSet(Bool_t isTwoDetector){
	Emit("twoDetectorSet(Bool_t)", isTwoDetector);
}

void Model::convolutionTypeSet(Int_t t){
	Emit("convolutionTypeSet(Int_t)", t);
}

void Model::hasParabolaSet(Bool_t b){
	Emit("hasParabolaSet(Bool_t)", b);
}

void Model::numberOfGaussiansSet(Int_t num){
	Emit("numberOfGaussiansSet(Int_t)", num);
}

void Model::numberOfExponentsSet(Int_t num){
	Emit("numberOfExponentsSet(Int_t)", num);
}

void Model::numberOfDampingExponentsSet(Int_t num){
	Emit("numberOfDampingExponentsSet(Int_t)", num);
}
