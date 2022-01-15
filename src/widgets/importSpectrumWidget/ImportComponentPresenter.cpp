/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ImportComponentPresenter.h"
#include "../../util/HistProcessor.h"
#include "../../model/Model.h"

ClassImp(ImportComponentPresenter)

ImportComponentPresenter::ImportComponentPresenter(AbstractImportSpectrumView* view) : AbstractImportSpectrumPresenter(view){
//	model = instantinateModel();
    model->Connect("componentHistogramImported(TH1F*)", this->ClassName(), this, "onModelComponentHistogramImported(TH1F*)");
}
        
void ImportComponentPresenter::setModelFileName(TString* fileName){
    model->setFileName(fileName);
}

void ImportComponentPresenter::setModelHist(TH1F* hist){
	// Save original histogram to Model
	model->setComponentHist(hist);
};


// Slots for Model Signals

void ImportComponentPresenter::onModelComponentHistogramImported(TH1F* hist){
    hist->Print("base");
    view->drawHistogram(hist);
}
