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

AbstractImportSpectrumView::AbstractImportSpectrumView(const TGWindow* w) : AbstractView<AbstractImportSpectrumPresenter>(w){    
    initUI();
}

AbstractImportSpectrumView::~AbstractImportSpectrumView() {
    // default destructor doesn't follow pointers
    delete btnOpenFile;
    delete lblFileName;
    delete txtFileBrowser;
    delete numEnergyColumn;
    delete numCountsColumn;
    delete btnImportSpectrum;
}

void AbstractImportSpectrumView::initUI(){
    // Open File Frame
    TGHorizontalFrame* frameOpenFile = new TGHorizontalFrame(this);
    btnOpenFile = new TGTextButton(frameOpenFile, "Open File");
    btnOpenFile->Connect("Clicked()", "AbstractImportSpectrumView", this, "onOpenFileClicked()");
    frameOpenFile->AddFrame(btnOpenFile, new TGLayoutHints(kLHintsLeft | kLHintsTop));
    lblFileName = new TGLabel(frameOpenFile, "no file loaded");
    frameOpenFile->AddFrame(lblFileName, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, dx, 0, dx)); // left right top bottom
    this->AddFrame(frameOpenFile, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy/2));

    // File browser
    txtFileBrowser = new TGTextEdit(this);
    this->AddFrame(txtFileBrowser, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy, dy/2));

    // Energy column
    TGHorizontalFrame* frameEnergyColumn = new TGHorizontalFrame(this);
    TGLabel* lblEnergyColumn = new TGLabel(frameEnergyColumn, "Energy Column #");
    numEnergyColumn = new TGNumberEntry(frameEnergyColumn, 1, 2, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            1, 99);
    frameEnergyColumn->AddFrame(lblEnergyColumn, new TGLayoutHints(kLHintsNormal, 0, 0, dx, 0));
    frameEnergyColumn->AddFrame(numEnergyColumn, new TGLayoutHints(kLHintsRight));
    this->AddFrame(frameEnergyColumn, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Counts column
    TGHorizontalFrame* frameCountsColumn = new TGHorizontalFrame(this);
    TGLabel* lblCountsColumn = new TGLabel(frameCountsColumn, "Counts Column #");
    numCountsColumn = new TGNumberEntry(frameCountsColumn, 2, 2, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            1, 99);
    frameCountsColumn->AddFrame(lblCountsColumn, new TGLayoutHints(kLHintsNormal, 0, 0, dx, 0));
    frameCountsColumn->AddFrame(numCountsColumn, new TGLayoutHints(kLHintsRight));
    this->AddFrame(frameCountsColumn, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Import Spectrum button
    TGHorizontalFrame* frameImportSpectrum = new TGHorizontalFrame(this);
    btnImportSpectrum = new TGTextButton(frameImportSpectrum, "Import Spectrum");
    btnImportSpectrum->Connect("Clicked()", "AbstractImportSpectrumView", this, "onImportSpectrumClicked()");
//    btnImportSpectrum->SetEnabled(false);
    frameImportSpectrum->AddFrame(btnImportSpectrum, new TGLayoutHints(kLHintsExpandX));
    this->AddFrame(frameImportSpectrum, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Histogram canvas
    TRootEmbeddedCanvas* embedHist = new TRootEmbeddedCanvas("embedHist", this);
    canvasHist = embedHist->GetCanvas(); //new TCanvas("canvasHist", 10, 10, idHist);
    canvasHist->SetLogy();
    this->AddFrame(embedHist, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy, dx));   
}

AbstractImportSpectrumPresenter* AbstractImportSpectrumView::instantinatePresenter() {
    std::cout << "AbstractImportSpectrumView::instantinatePresenter()" << std::endl;
    return new AbstractImportSpectrumPresenter(this);
}

// Calls to Presenter

void AbstractImportSpectrumView::onOpenFileClicked(){
    std::cout << "AbstractImportSpectrumView::onOpenFileClicked()" << std::endl;
    AbstractImportSpectrumPresenter* presenter = this->getPresenter();
    presenter->onOpenFileClicked();
}
    
void AbstractImportSpectrumView::onImportSpectrumClicked(){
    std::cout << "AbstractImportSpectrumView::onImportSpectrumClicked()" << std::endl;
    AbstractImportSpectrumPresenter* presenter = this->getPresenter();
    presenter->onImportSpectrumClicked();
}

// Calls from Presenter 
void AbstractImportSpectrumView::loadFile(TString* fileNamePath){
    std::cout << "AbstractImportSpectrumView::loadFile() " << *fileNamePath << std::endl;
    // Load file contents to FileBrowser
    txtFileBrowser->LoadFile(fileNamePath->Data());
    // Update FileName label
    TString* fileName = StringUtils::stripFileName(fileNamePath);
    lblFileName->SetText(fileName->Data());
}

Int_t AbstractImportSpectrumView::getEnergyColumnNumber(){
    return numEnergyColumn->GetIntNumber();
}

Int_t AbstractImportSpectrumView::getCountsColumnNumber(){
    return numCountsColumn->GetIntNumber();
}

TString* AbstractImportSpectrumView::getFileName(){
    const char* s = (lblFileName->GetText())->GetString();
    return new TString(s);
}

void AbstractImportSpectrumView::drawHistogram(TH1F* hist){
    GraphicsHelper* graphicsHelper = GraphicsHelper::getInstance();
    canvasHist->cd();
    hist->Draw();
    graphicsHelper->setDefaultAxisFonts(hist->GetXaxis());
    graphicsHelper->setDefaultAxisFonts(hist->GetYaxis());
    hist->SetStats(0);
    canvasHist->Update();
}