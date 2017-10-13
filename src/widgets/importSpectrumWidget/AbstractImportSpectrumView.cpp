/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "AbstractImportSpectrumView.h"
#include "AbstractImportSpectrumPresenter.h"
#include <TRootEmbeddedCanvas.h>
#include "../../util/StringUtils.h"
#include "../../util/GraphicsHelper.h"

// Calls to Presenter
template <class P>
void AbstractImportSpectrumView<P>::onOpenFileClicked(){
    P* p = this->template getPresenter();
    AbstractImportSpectrumPresenter<AbstractImportSpectrumView>* presenter = static_cast<AbstractImportSpectrumPresenter<AbstractImportSpectrumView>*>(p);
    presenter->onOpenFileClicked();
}

template <class P>
void AbstractImportSpectrumView<P>::onImportSpectrumClicked(){
    P* p = this->template getPresenter();
    AbstractImportSpectrumPresenter<AbstractImportSpectrumView>* presenter = static_cast<AbstractImportSpectrumPresenter<AbstractImportSpectrumView>*>(p);
    presenter->onImportSpectrumClicked();
}

// Calls from Presenter 
template <class P>
void AbstractImportSpectrumView<P>::loadFile(const char* fileNamePath){
    // Load file contents to FileBrowser
    txtFileBrowser->LoadFile(fileNamePath);
    // Update FileName label
    TString* fileName = StringUtils::stripFileName(new TString(fileNamePath));
    lblFileName->SetText(fileName->Data());
}

template <class P>
Int_t AbstractImportSpectrumView<P>::getEnergyColumnNumber(){
    return numEnergyColumn->GetIntNumber();
}

template <class P>
Int_t AbstractImportSpectrumView<P>::getCountsColumnNumber(){
    return numCountsColumn->GetIntNumber();
}

template <class P>
TString* AbstractImportSpectrumView<P>::getFileName(){
    const char* s = (lblFileName->GetText())->GetString();
    return new TString(s);
}

template <class P>
void AbstractImportSpectrumView<P>::drawHistogram(TH1F* hist){
    GraphicsHelper* graphicsHelper = GraphicsHelper::getInstance();
    canvasHist->cd();
    hist->Draw();
    graphicsHelper->setDefaultAxisFonts(hist->GetXaxis());
    graphicsHelper->setDefaultAxisFonts(hist->GetYaxis());
    hist->SetStats(0);
    canvasHist->Update();
}