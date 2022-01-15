/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImportSourceSpectrumPresenter.cpp
 * Author: petrstepanov
 * 
 * Created on October 12, 2017, 8:55 PM
 */

#include "ImportSourceSpectrumPresenter.h"

ClassImp(ImportSourceSpectrumPresenter)

ImportSourceSpectrumPresenter::ImportSourceSpectrumPresenter(AbstractImportSpectrumView* view) : AbstractImportSpectrumPresenter(view){
//	model = instantinateModel();
    model->Connect("sourceHistogramImported(TH1F*)", this->ClassName(), this, "onModelSourceHistogramImported(TH1F*)");
}
        
void ImportSourceSpectrumPresenter::setModelFileName(TString* fileName){
    model->setSourceFileName(fileName);
}

void ImportSourceSpectrumPresenter::setModelHist(TH1F* hist){
    model->setSourceHist(hist);
};


// Slots for Model Signals

void ImportSourceSpectrumPresenter::onModelSourceHistogramImported(TH1F* hist){
    hist->Print("base");
    view->drawHistogram(hist);
}
