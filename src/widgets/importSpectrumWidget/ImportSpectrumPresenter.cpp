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
	// Save original histogram to Model
	model->setHist(hist);

    // Determine if spectrum is two detector or not
    HistProcessor* histProcessor = HistProcessor::getInstance();
    Bool_t isTwoDetector = histProcessor->isTwoDetetor(hist);
	model->setTwoDetector(isTwoDetector);

	// Set fit range limits
	Double_t fitMinLo = hist->GetXaxis()->GetXmin();
	Double_t fitMinUp = hist->GetXaxis()->GetBinLowEdge(hist->GetMaximumBin());
	Double_t fitMaxLo = hist->GetXaxis()->GetBinUpEdge(hist->GetMaximumBin());
	Double_t fitMaxUp = hist->GetXaxis()->GetXmax();
	model->setFitRangeLimits(fitMinLo, fitMinUp, fitMaxLo, fitMaxUp);
};
    
