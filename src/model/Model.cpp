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

Model* Model::instance = NULL;

Model* Model::getInstance(){
    
    if (!instance){
        instance = new Model();
    }
    return instance;
}

Model::Model(){
	twoDetector=false;
    sourceContribution = new RooRealVar("sourceContribution", "Source contribution", 14, 8., 16., "%");
    sourceContribution->setConstant(kTRUE);
};

void Model::setFileName(TString* fileName){
    std::cout << "Model::setFileName(), " << *fileName << std::endl;
    strFileName = fileName;
}

TString* Model::getFileName(){
    return strFileName;
}

void Model::setSourceFileName(TString* fileName){
    std::cout << "Model::setSourceFileName(), " << fileName << std::endl;
    strSourceContribFileName = fileName;
}

TString* Model::getSourceFileName(){
    return strSourceContribFileName;
}

void Model::setHist(TH1F* hist){
    fullHist = hist;
    Double_t histMin = hist->GetXaxis()->GetXmin();
    Double_t histMax = hist->GetXaxis()->GetXmax();
}

TH1F* Model::getHist(){
    return fullHist;
}

void Model::setSourceHist(TH1F* hist){
    sourceHist = hist;
    // EventBus::FireEvent(*(new SourceHistogramImportedEvent(*this))); // Fire the event
}

TH1F* Model::getSourceHist(){
    return sourceHist;
}

void Model::setTwoDetector(Bool_t b){
    twoDetector = b;
    // EventBus::FireEvent(*(new IsTwoDetectorEvent(*this, twoDetector))); // Fire the event
}

Bool_t Model::isTwoDetector(){
    return twoDetector;
}

void Model::setSafeFitRange(Double_t eMin, Double_t eMax){
    safeFitRange = std::make_pair(eMin, eMax);
    // EventBus::FireEvent(*(new HistogramImportedEvent(*this, eMin, eMax))); // Fire the event
}

RooRealVar* Model::getSourceContribution(){
    return sourceContribution;
}

// SIGNALS

void Model::sourceHistogramImported(TH1F* hist){
	Emit("sourceHistogramImported(TH1F*)", hist);
}

void Model::twoDetectorSet(Bool_t isTwoDetector){
	Emit("isTwoDetector(Bool_t)", isTwoDetector);
}

void Model::safeFitRangeSet(Double_t eMin, Double_t eMax){
	Long_t args[2];
	args[0]=(Long_t)eMin;
	args[1]=(Long_t)eMax;
	Emit("safeFitRangeSet(Double_t, Double_t)", args);
}
