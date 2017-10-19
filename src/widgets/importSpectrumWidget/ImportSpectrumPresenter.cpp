/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ImportSpectrumPresenter.h"
#include "../../util/HistProcessor.h"
#include "../../model/Model.h"

ImportSpectrumPresenter::ImportSpectrumPresenter(AbstractImportSpectrumView* view) : AbstractImportSpectrumPresenter(view){
}
        
void ImportSpectrumPresenter::setModelFileName(TString* fileName){
    Model* model = getModel();
    model->setFileName(fileName);
}

void ImportSpectrumPresenter::setModelHist(TH1F* hist){
    Model* model = getModel();
    model->setHist(hist);

    HistProcessor* histProcessor = HistProcessor::getInstance();
    std::pair<Double_t, Double_t> safeFitRange = histProcessor->getHistogramSafeFitRange(hist);
    model->setSafeFitRange(safeFitRange.first, safeFitRange.second);
    
    Bool_t isTwoDetector = (hist->GetXaxis()->GetXmin() < -10) && (hist->GetXaxis()->GetXmax() > 10);
    model->setTwoDetector(isTwoDetector);
};
    