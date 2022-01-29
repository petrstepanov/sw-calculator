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

	fitProperties.convolutionType = ConvolutionType::kNoConvolution;
    fitProperties.singleBgType = BackgroundType::kErf;
	fitProperties.hasParabola = kFALSE;
    fitProperties.hasDelta = kFALSE;
	fitProperties.numberOfGaussians = 1;
	fitProperties.numberOfExponents = 0;
	fitProperties.numberOfDampingExponents = 0;
	fitProperties.hist = nullptr;
	fitProperties.sourceHist = nullptr;
	fitProperties.componentHist = nullptr;

	parametersPool = nullptr;
	// needsRebuild = kTRUE;
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

void Model::setLift(Int_t l){
    fitProperties.lift = l;
}

void Model::setHist(TH1F* hist){
    if (hist && fitProperties.hist != hist){
        fitProperties.hist = hist;
        fitProperties.minFitBin = 1;
        fitProperties.maxFitBin = hist->GetNbinsX();

        // Make new parameters pool
        parametersPool = new ParametersPool();

        // Emit signals to presenters
        // fitPropertiesChanged();

        histogramImported(hist);
    }
}

void Model::setSourceHist(TH1F* sourceHist){
//    if (fitProperties.sourceHist != sourceHist){
        fitProperties.sourceHist = sourceHist;

        // Emit signals to presenters
        // fitPropertiesChanged();
        sourceHistogramImported(sourceHist);
//    }
}

void Model::setComponentHist(TH1F* componentHist){
    if (fitProperties.componentHist != componentHist){
        fitProperties.componentHist = componentHist;

        // Emit signals to presenters
        // fitPropertiesChanged();
        componentHistogramImported(componentHist);
    }
}

void Model::setFitRange(Int_t minBin, Int_t maxBin){
    // Check histogram exists
    if (!fitProperties.hist) return;


    Int_t minAllowedFitBinsRange = 50;

    // Make sure fit range is within the histogram limits
    if (minBin < 1) minBin = 1;
    if (maxBin > fitProperties.hist->GetXaxis()->GetNbins()) maxBin = fitProperties.hist->GetXaxis()->GetNbins();

    // If changed min bin and it is over the max bin
    if (minBin > fitProperties.minFitBin && minBin >= fitProperties.maxFitBin - minAllowedFitBinsRange){
        minBin = fitProperties.maxFitBin - minAllowedFitBinsRange + 1;
    }

    // If changed max bin and it is below the min bin
    if (maxBin < fitProperties.maxFitBin && maxBin <= fitProperties.minFitBin + minAllowedFitBinsRange){
        maxBin = fitProperties.minFitBin + minAllowedFitBinsRange + 1;
    }

    if (fitProperties.hist->GetXaxis()->GetFirst() != minBin || fitProperties.hist->GetXaxis()->GetLast() != maxBin){
        // Do not mess with hist ranges here because it complicates things when displaying hists in the view
        // fitProperties.hist->GetXaxis()->SetRange(minBin, maxBin);

        // Instead store min and max bins as a parameters in the FitProperties struct
        fitProperties.minFitBin = minBin;
        fitProperties.maxFitBin = maxBin;

        // Reset Parameters Pool because we can be fitting another peak
        // Actually just remove mean from pool for now
        parametersPool = new ParametersPool();

        // Notfy the view about the fit range change
        // Int_t first = fitProperties.hist->GetXaxis()->GetFirst();
        // Int_t last = fitProperties.hist->GetXaxis()->GetLast();
        DoublePair* range = new DoublePair(minBin, maxBin);

        // Emit signals to presenters
        fitRangeSet(range);
    }
}

std::pair<Double_t, Double_t> Model::getFitRange(){
	// return std::make_pair(fitProperties.hist->GetXaxis()->GetFirst(), fitProperties.hist->GetXaxis()->GetLast());
    return std::make_pair(fitProperties.minFitBin, fitProperties.maxFitBin);
}

// Fit Properties setters getters

void Model::setConvolutionType(ConvolutionType t){
    if (fitProperties.convolutionType != t){
        fitProperties.convolutionType = t;

        // Emit signals to presenters
        // fitPropertiesChanged();
        convolutionTypeSet(t);
    }
}

ConvolutionType Model::getConvolutionType(){
	return fitProperties.convolutionType;
}

void Model::setSingleBackgroundType(BackgroundType t){
    if (fitProperties.singleBgType != t){
        fitProperties.singleBgType = t;

        // Emit signals to presenters
        // fitPropertiesChanged();
        backgroundTypeSet(t);
    }
}

BackgroundType Model::getSingleBackgroundType(){
    return fitProperties.singleBgType;
}


void Model::setHasParabola(Bool_t b){
    if (fitProperties.hasParabola != b){
        fitProperties.hasParabola = b;

        // Emit signals to presenters
        // fitPropertiesChanged();
        hasParabolaSet(b);
    }
}

Bool_t Model::getHasParabola(){
    return fitProperties.hasParabola;
}

void Model::setHasDelta(Bool_t b){
    if (fitProperties.hasDelta != b){
        fitProperties.hasDelta = b;

        // Emit signals to presenters
        // fitPropertiesChanged();
        hasDeltaSet(b);
    }
}

Bool_t Model::getHasDelta(){
    return fitProperties.hasDelta;
}

void Model::setNumberOfGaussians(Int_t num){
    if (fitProperties.numberOfGaussians != num){
        fitProperties.numberOfGaussians = num;

        // Emit signals to presenters
        // fitPropertiesChanged();
        numberOfGaussiansSet(num);
    }
}

Int_t Model::getNumberOfGaussians(){
	return fitProperties.numberOfGaussians;
}

void Model::setNumberOfExponents(Int_t num){
    if (fitProperties.numberOfExponents != num){
        fitProperties.numberOfExponents = num;

        // Emit signals to presenters
        // fitPropertiesChanged();
        numberOfExponentsSet(num);
    }
}

Int_t Model::getNumberOfExponents(){
	return fitProperties.numberOfExponents;
}

void Model::setNumberOfDampingExponents(Int_t num){
    if (fitProperties.numberOfDampingExponents != num){
        fitProperties.numberOfDampingExponents = num;

        // Emit signals to presenters
        // fitPropertiesChanged();
        numberOfDampingExponentsSet(num);
    }
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

// SIGNALS TO PRESENTERS

void Model::fitRangeSet(DoublePair* pair){
	Emit("fitRangeSet(DoublePair*)", pair);
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

//void Model::twoDetectorSet(Bool_t isTwoDetector){
//	Emit("twoDetectorSet(Bool_t)", isTwoDetector);
//}

void Model::convolutionTypeSet(Int_t t){
	Emit("convolutionTypeSet(Int_t)", t);
}

void Model::backgroundTypeSet(Int_t t){
    Emit("backgroundTypeSet(Int_t)", t);
}

void Model::hasParabolaSet(Bool_t b){
	Emit("hasParabolaSet(Bool_t)", b);
}

void Model::hasDeltaSet(Bool_t b){
    Emit("hasDeltaSet(Bool_t)", b);
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

//void Model::fitPropertiesChanged(){
//    Emit("fitPropertiesChanged()");
//}
