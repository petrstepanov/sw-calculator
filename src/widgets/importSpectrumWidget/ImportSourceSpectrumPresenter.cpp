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

ImportSourceSpectrumPresenter::ImportSourceSpectrumPresenter(AbstractImportSpectrumView* view) : AbstractImportSpectrumPresenter(view){
    std::cout << "ImportSourceSpectrumPresenter::ImportSourceSpectrumPresenter()" << std::endl;    
}
        
void ImportSourceSpectrumPresenter::setModelFileName(const char* fileName){
    std::cout << "ImportSourceSpectrumPresenter::setModelFilename()" << std::endl;
    
    Model* model = getModel();
    TString* fN = new TString(fileName);
    model->setSourceFileName(fN);
}

void ImportSourceSpectrumPresenter::setModelHist(TH1F* hist, Bool_t isTwoDetector){
    std::cout << "ImportSourceSpectrumPresenter::setModelFilename()" << std::endl;
    
    Model* model = getModel();
    model->setSourceHist(hist);
};
    