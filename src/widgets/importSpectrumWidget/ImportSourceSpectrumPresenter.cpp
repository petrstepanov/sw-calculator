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
}
        
void ImportSourceSpectrumPresenter::setModelFileName(TString* fileName){
    Model* model = getModel();
    model->setSourceFileName(fileName);
}

void ImportSourceSpectrumPresenter::setModelHist(TH1F* hist){
    Model* model = getModel();
    model->setSourceHist(hist);
};
    