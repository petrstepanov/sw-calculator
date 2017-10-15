/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ImportSpectrumPresenter.h"
#include "../../model/Model.h"

ImportSpectrumPresenter::ImportSpectrumPresenter(AbstractImportSpectrumView* view) : AbstractImportSpectrumPresenter(view){
    std::cout << "ImportSpectrumPresenter::ImportSpectrumPresenter()" << std::endl;    
}
        
void ImportSpectrumPresenter::setModelFileName(TString* fileName){
    std::cout << "ImportSpectrumPresenter::setModelFilename() " << *fileName << std::endl;

    Model* model = getModel();
    model->setFileName(fileName);
}

void ImportSpectrumPresenter::setModelHist(TH1F* hist, Bool_t isTwoDetector){
    std::cout << "ImportSpectrumPresenter::setModelHist()" << std::endl;
    
    Model* model = getModel();
    model->setHist(hist);
    model->setTwoDetector(isTwoDetector);
};
    