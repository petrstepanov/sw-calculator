/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ImportSpectrumPresenter.h"

ImportSpectrumPresenter::ImportSpectrumPresenter(ImportSpectrumView* view) : AbstractImportSpectrumPresenter(view){
}
        
void ImportSpectrumPresenter::setModelFileName(char* fileName){
    Model* model = AbstractImportSpectrumPresenter::getModel();
    TString* fN = new TString(fileName);
    model->setFileName(fN);
}

void ImportSpectrumPresenter::setModelHist(TH1F* hist, Bool_t isTwoDetector){
    Model* model = AbstractImportSpectrumPresenter::getModel();
    model->setHist(hist);
    model->setTwoDetector(isTwoDetector);
};
    