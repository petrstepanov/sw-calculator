/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ImportSpectrumPresenter.h"
#include "../../util/HistProcessor.h"
#include "../../model/Model.h"

ImportSpectrumPresenter::ImportSpectrumPresenter(AbstractImportSpectrumView* view) : AbstractImportSpectrumPresenter(view){
//	model = instantinateModel();
}
        
void ImportSpectrumPresenter::setModelFileName(TString* fileName){
    model->setFileName(fileName);
}

void ImportSpectrumPresenter::setModelHist(TH1F* hist){
    model->setHist(hist);

    HistProcessor* histProcessor = HistProcessor::getInstance();
    Bool_t isTwoDetector = histProcessor->isTwoDetetor(hist);

    if (isTwoDetector){
        std::pair<Double_t, Double_t> safeFitRange = histProcessor->getHistogramSafeFitRange(hist);
        model->setSafeFitRange(safeFitRange.first, safeFitRange.second);
    } else {
        std::pair<Double_t, Double_t> safeFitRange = std::make_pair(491, 531);        
        model->setSafeFitRange(safeFitRange.first, safeFitRange.second);
    }
    
    model->setTwoDetector(isTwoDetector);
};
    
