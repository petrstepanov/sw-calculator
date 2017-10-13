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

template <class P>
AbstractImportSpectrumView<P>::AbstractImportSpectrumView(TGCompositeFrame *p) : AbstractView<P>(p){    
}

template <class P>
AbstractImportSpectrumView<P>::~AbstractImportSpectrumView(){
    // default destructor doesn't follow pointers
    delete btnOpenFile;
    delete lblFileName;
    delete txtFileBrowser;
    delete numEnergyColumn;
    delete numCountsColumn;
    delete btnImportSpectrum;
}

// error: allocating an object of abstract class type 'AbstractImportSpectrumPresenter'
// AbstractImportSpectrumPresenter* AbstractImportSpectrumView::instantinatePresenter(){
//     return new AbstractImportSpectrumPresenter(this);
// }

template <class P>
void AbstractImportSpectrumView<P>::initUI(){
    // Open File Frame
    TGHorizontalFrame* frameOpenFile = new TGHorizontalFrame(AbstractView<P>::GetParent());
    btnOpenFile = new TGTextButton(frameOpenFile, "Open File");
    btnOpenFile->Connect("Clicked()", "AbstractImportSpectrumView", this, "onOpenFileClicked()");
    frameOpenFile->AddFrame(btnOpenFile, new TGLayoutHints(kLHintsLeft | kLHintsTop));
    lblFileName = new TGLabel(frameOpenFile, "");
    frameOpenFile->AddFrame(lblFileName, new TGLayoutHints(kLHintsLeft | kLHintsTop, dx, 0, dx)); // left right top bottom
    this->template GetParentComposite()->AddFrame(frameOpenFile, new TGLayoutHints(kLHintsExpandX, dx, dx, dx, d2x));
    
    // File browser
    txtFileBrowser = new TGTextEdit(AbstractView<P>::GetParent());
    this->template GetParentComposite()->AddFrame(txtFileBrowser, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dx, d2x));
    
    // Energy column
    TGHorizontalFrame* frameEnergyColumn = new TGHorizontalFrame(AbstractView<P>::GetParent());
    TGLabel* lblEnergyColumn = new TGLabel(frameEnergyColumn, "Energy Column #");
    numEnergyColumn = new TGNumberEntry(frameEnergyColumn, 1, 2, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            1, 99);
    frameEnergyColumn->AddFrame(lblEnergyColumn, new TGLayoutHints(kLHintsNormal, 0, 0, dx, 0));
    frameEnergyColumn->AddFrame(numEnergyColumn, new TGLayoutHints(kLHintsRight));
    this->template GetParentComposite()->AddFrame(frameEnergyColumn, new TGLayoutHints(kLHintsExpandX, dx, dx, d2x, d2x));
    
    // Counts column
    TGHorizontalFrame* frameCountsColumn = new TGHorizontalFrame(AbstractView<P>::GetParent());
    TGLabel* lblCountsColumn = new TGLabel(frameCountsColumn, "Counts Column #");
    numCountsColumn = new TGNumberEntry(frameCountsColumn, 2, 2, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            1, 99);
    frameCountsColumn->AddFrame(lblCountsColumn, new TGLayoutHints(kLHintsNormal, 0, 0, dx, 0));
    frameCountsColumn->AddFrame(numCountsColumn, new TGLayoutHints(kLHintsRight));
    this->template GetParentComposite()->AddFrame(frameCountsColumn, new TGLayoutHints(kLHintsExpandX, dx, dx, d2x, d2x));
    
    // Import Spectrum button
    TGHorizontalFrame* frameImportSpectrum = new TGHorizontalFrame(AbstractView<P>::GetParent());
    btnImportSpectrum = new TGTextButton(frameImportSpectrum, "Import Spectrum");
    btnImportSpectrum->Connect("Clicked()", "AbstractImportSpectrumView", this, "onImportSpectrumClicked()");
    btnImportSpectrum->SetEnabled(false);
    frameImportSpectrum->AddFrame(btnImportSpectrum, new TGLayoutHints(kLHintsExpandX));
    this->template GetParentComposite()->AddFrame(frameImportSpectrum, new TGLayoutHints(kLHintsExpandX, dx, dx, d2x, d2x));

    // Histogram canvas
    TRootEmbeddedCanvas* embedHist = new TRootEmbeddedCanvas("embedHist", AbstractView<P>::GetParent());
    canvasHist = embedHist->GetCanvas(); //new TCanvas("canvasHist", 10, 10, idHist);
    canvasHist->SetLogy();
    this->template GetParentComposite()->AddFrame(embedHist, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, d2x, dx));    
}

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