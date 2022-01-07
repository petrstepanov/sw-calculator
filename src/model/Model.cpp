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
//	fitProperties.fitMin = 0;
//	fitProperties.fitMax = 0;
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
	// Delete old hist
//	if (fitProperties.hist != NULL){
//		delete fitProperties.hist;
//	}

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

// TODO: reimplement calls to this function
void Model::setTwoDetector(Bool_t isTwoDetector){
    fitProperties.isTwoDetector = isTwoDetector;
}

Bool_t Model::isTwoDetector(){
    return fitProperties.isTwoDetector;
}

void Model::setFitRange(Int_t minBin, Int_t maxBin){
	// Set histogram range in bins
	fitProperties.hist->GetXaxis()->SetRange(minBin, maxBin);

    // Notfy the view about the fit range change
	Int_t first = fitProperties.hist->GetXaxis()->GetFirst();
	Int_t last = fitProperties.hist->GetXaxis()->GetLast();
    DoublePair* fitRange = new DoublePair(first, last);
    fitRangeSet(fitRange);
}

std::pair<Double_t, Double_t> Model::getFitRange(){
	return std::make_pair(fitProperties.hist->GetXaxis()->GetFirst(), fitProperties.hist->GetXaxis()->GetLast());
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

//void Model::fitRangeLimitsSet(DoublePair* pair){
//	Emit("fitRangeLimitsSet(DoublePair*)", pair);
//}

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
