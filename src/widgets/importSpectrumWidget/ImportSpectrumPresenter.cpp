/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ImportSpectrumPresenter.h"
#include "../../util/HistProcessor.h"
#include "../../util/UiHelper.h"
#include "../../model/Model.h"

ClassImp(ImportSpectrumPresenter)

ImportSpectrumPresenter::ImportSpectrumPresenter(AbstractImportSpectrumView* view) : AbstractImportSpectrumPresenter(view){
//	model = instantinateModel();
    // model->Connect("histogramImported(TH1F*)", this->ClassName(), this, "onModelHistogramImported(TH1F*)");
}
        
void ImportSpectrumPresenter::setModelFileName(TString* fileName){
    model->setFileName(fileName);
}

void ImportSpectrumPresenter::setModelHist(TH1F* hist){
    // Save original histogram to Model
	model->setHist(hist);
};


// Slots for Model Signals

//void ImportSpectrumPresenter::onModelHistogramImported(TH1F* hist){
//    // hist->Print("base");
//    view->drawHistogram(hist);
//}
