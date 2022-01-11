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

	// Set low and up fit range limits

	// TODO:
	// GetFirst() and GetLast() are visible bins of the histogram.
	// Maybe use this in future instead of re-creating a new fitting histogram

	// If range set
	// Double_t fitMinLo  = hist->GetXaxis()->GetXmin();
	// Int_t lowRangeBin = hist->GetXaxis()->GetFirst();
	// if (lowRangeBin != 1){
	// 	fitMinLo = hist->GetXaxis()->GetBinLowEdge(lowRangeBin);
	// }

	// Double_t fitMinUp = hist->GetXaxis()->GetBinLowEdge(hist->GetMaximumBin());
	// Double_t fitMaxLo = hist->GetXaxis()->GetBinUpEdge(hist->GetMaximumBin());

	// Range may be set therefore check it (for ITEP spectra)
	// Double_t fitMaxUp = hist->GetXaxis()->GetXmax();
	// Int_t hiRangeBin = hist->GetXaxis()->GetLast();
	// if (hiRangeBin != hist->GetXaxis()->GetNbins()){
	// 	fitMaxUp = hist->GetXaxis()->GetBinUpEdge(hiRangeBin);
	// }

	// model->setFitRangeLimits(fitMinLo, fitMinUp, fitMaxLo, fitMaxUp);
};

//void ImportSpectrumPresenter::onRangeSliderChange(Int_t minBin, Int_t maxBin){
//	model->setHistRange(minBin, maxBin);
//}
