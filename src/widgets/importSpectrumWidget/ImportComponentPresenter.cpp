/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ImportComponentPresenter.h"
#include "../../util/HistProcessor.h"
#include "../../model/Model.h"

ImportComponentPresenter::ImportComponentPresenter(AbstractImportSpectrumView* view) : AbstractImportSpectrumPresenter(view){
//	model = instantinateModel();
}
        
void ImportComponentPresenter::setModelFileName(TString* fileName){
    model->setFileName(fileName);
}

void ImportComponentPresenter::setModelHist(TH1F* hist){
	// Save original histogram to Model
	model->setComponentHist(hist);
};
    
