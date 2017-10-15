/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ImportSpectrumPresenter.h"

ImportSpectrumPresenter::ImportSpectrumPresenter(AbstractImportSpectrumView* view) : AbstractImportSpectrumPresenter(view){
    std::cout << "ImportSpectrumPresenter::ImportSpectrumPresenter()" << std::endl;    
}
        
void ImportSpectrumPresenter::setModelFileName(const char* fileName){
    std::cout << "ImportSpectrumPresenter::setModelFilename()" << std::endl;

    Model* model = getModel();
    TString* fN = new TString(fileName);
    model->setFileName(fN);
}

void ImportSpectrumPresenter::setModelHist(TH1F* hist, Bool_t isTwoDetector){
    std::cout << "ImportSpectrumPresenter::setModelHist()" << std::endl;
    
    Model* model = getModel();
    model->setHist(hist);
    model->setTwoDetector(isTwoDetector);
};
    