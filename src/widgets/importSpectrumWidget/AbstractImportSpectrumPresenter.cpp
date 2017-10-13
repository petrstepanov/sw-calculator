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

template <class V>
void AbstractImportSpectrumPresenter<V>::onOpenFileClicked(){
    // Get FileInfo from open file dialog
    TGFileInfo* fileInfo = UiHelper::getFileFromDialog();
    if ((fileInfo->fFilename == NULL) || (strlen(fileInfo->fFilename)>0)){
        return;
    }
    // Update Model with new file name (virtual function)
    setModelFileName(fileInfo->fFilename);
    // Update View to reflect 
    V* v = AbstractPresenter<Model, V>::getView();
    AbstractImportSpectrumView<AbstractImportSpectrumPresenter>* view = static_cast<AbstractImportSpectrumView<AbstractImportSpectrumPresenter>*>(v);
    view->loadFile(fileInfo->fFilename);
}

template <class V>
void AbstractImportSpectrumPresenter<V>::onImportSpectrumClicked(){
    // Import Histogram
    V* v = AbstractPresenter<Model, V>::getView();
    AbstractImportSpectrumView<AbstractImportSpectrumPresenter>* view = static_cast<AbstractImportSpectrumView<AbstractImportSpectrumPresenter>*>(v);
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