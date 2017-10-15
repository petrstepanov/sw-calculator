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

AbstractImportSpectrumPresenter::AbstractImportSpectrumPresenter(AbstractImportSpectrumView* view) : 
    AbstractPresenter<Model, AbstractImportSpectrumView>(view) {
}

void AbstractImportSpectrumPresenter::onOpenFileClicked(){
    std::cout << "AbstractImportSpectrumPresenter::onOpenFileClicked()" << std::endl;
    AbstractImportSpectrumView* view = getView();
    
    // Get FileInfo from open file dialog
    TGFileInfo* fileInfo = UiHelper::getFileFromDialog(view->GetParent());
    if (!fileInfo->fFilename){
        std::cout << "AbstractImportSpectrumPresenter::onOpenFileClicked() filename is NULL" << std::endl;
        return;
    }
    // Update Model with new file name (virtual function)
    setModelFileName(fileInfo->fFilename);
    // Update View to reflect 
    view->loadFile(fileInfo->fFilename);
}

void AbstractImportSpectrumPresenter::onImportSpectrumClicked(){
    std::cout << "AbstractImportSpectrumPresenter::onImportSpectrumClicked()" << std::endl;

    // Import Histogram
    AbstractImportSpectrumView* view = getView();
    Int_t energyColumn = view->getEnergyColumnNumber();
    Int_t countsColumn = view->getCountsColumnNumber();
    TString* fileName = view->getFileName();
    FileUtils* fileUtils = FileUtils::getInstance();
    TH1F* hist = fileUtils->importTH1(fileName->Data(), energyColumn, countsColumn);
    if (!hist){
        return;
    }
    
    // Update Model
    Bool_t isTwoDetector = (hist->GetXaxis()->GetXmin() < 0) && (hist->GetXaxis()->GetXmax() > 0);
    setModelHist(hist, isTwoDetector);
    
    // Update View
    view->drawHistogram(hist);
}