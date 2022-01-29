 /*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "AbstractImportSpectrumPresenter.h"
#include "AbstractImportSpectrumView.h"
#include <TGFileDialog.h>
#include "../../model/Model.h"
#include "../../util/UiHelper.h"
#include "../../util/FileUtils.h"
#include "../../util/HistProcessor.h"

ClassImp(AbstractImportSpectrumPresenter)

AbstractImportSpectrumPresenter::AbstractImportSpectrumPresenter(AbstractImportSpectrumView* view) : 
    AbstractPresenter<Model, AbstractImportSpectrumView>(view) {
	model = instantinateModel();
}

void AbstractImportSpectrumPresenter::onOpenFileClicked(){
    // Get FileInfo from open file dialog
    UiHelper* uiHelper = UiHelper::getInstance();
    TGFileInfo* fileInfo = uiHelper->getFileFromDialog(view->GetMainFrame());
    if (!fileInfo->fFilename){
        return;
    }
    TString* fileNamePath = new TString(fileInfo->fFilename);
    
    // Import histogram
    notifyUser("Importing File...");
    TString* fileName = fileNamePath;
	FileUtils* fileUtils = FileUtils::getInstance();
	TH1F* hist = fileUtils->importTH1(fileName->Data());
	if (!hist){
	    notifyUser("");
		UiHelper::showOkDialog(view->GetMainFrame(), "File type is not supported.", EMsgBoxIcon::kMBIconStop);
		return;
	}
    // Update Model with new file name (virtual function)
    this->setModelFileName(fileNamePath);

    // Update View to reflect
	// Display untrimmed histogram in the view's preview
	view->drawHistogram(hist);
    view->loadFile(fileNamePath);
	// hist->Print("base");

    // Problem: THStack lags plotting histograms with 0 values in log axis
    //          Error in <THistPainter::PaintInit>: Cannot set Y axis to log scale
    //          Error in THistPainter::PaintInit: log scale requested with a negative argument (-0.136274)
    // Problem: RooFit's Chi2 fit cannot work with zero bins
	// Bad solution: lift histogram up.
    // Good solution: cut range from the center that does not contain zeros

    if (hist->GetMinimum() <= 0){
        notifyUser("Processing Histogram...");
        // Notify user
        Int_t newMinBin = HistProcessor::getLeftNonZeroBin(hist);
        Int_t newMaxBin = HistProcessor::getRightNonZeroBin(hist);

        // Trim hist
        TString newHistName = hist->GetName();
        newHistName += "trimmed";
        hist = HistProcessor::cutHist(newHistName.Data(), hist, newMinBin, newMaxBin);

        // Notify user
        TString message = TString::Format("Histogram is trimmed to exclude zero and negative bins\nBinning range adjusted to [%d, %d].", newMinBin, newMaxBin);
        UiHelper::showOkDialog(view->GetMainFrame(), message, EMsgBoxIcon::kMBIconExclamation);
    }

    notifyUser("Done!");
	// Update Model. Corresponding overridden method setModelHist() will be called.
	setModelHist(hist);
}

void AbstractImportSpectrumPresenter::notifyUser(const char* text){
    Emit("notifyUser(const char*)", text);
}

//void AbstractImportSpectrumPresenter::onImportSpectrumClicked(){
//    // Import Histogram
//    // Int_t energyColumn = view->getEnergyColumnNumber();
//    // Int_t countsColumn = view->getCountsColumnNumber();
//    TString* fileName = view->getFileName();
//    FileUtils* fileUtils = FileUtils::getInstance();
//    TH1F* hist = fileUtils->importTH1(fileName->Data());
//    if (!hist){
//        return;
//    }
//
//    // Update Model
//    setModelHist(hist);
//
//    // Update View
//    view->drawHistogram(hist);
//}
