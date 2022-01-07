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
#include "../../model/Constants.h"
#include "../../util/UiHelper.h"
#include <TSystem.h>

ClassImp(AbstractImportSpectrumView);

AbstractImportSpectrumView::AbstractImportSpectrumView(const TGWindow* w) : AbstractView<AbstractImportSpectrumPresenter>(w){
    initUI();
}

AbstractImportSpectrumView::~AbstractImportSpectrumView() {
}

void AbstractImportSpectrumView::initUI(){
    // Open File Frame
    TGHorizontalFrame* frameOpenFile = new TGHorizontalFrame(this);
    {
        btnOpenFile = new TGTextButton(frameOpenFile, " Open File ");
        btnOpenFile->Connect("Clicked()", "AbstractImportSpectrumView", this, "onOpenFileClicked()");

        lblFileName = new TGLabel(frameOpenFile, Constants::LABEL_NO_FILE_LOADED);
        lblFileName->SetTextJustify(kTextLeft);
        lblFileName->Disable(kTRUE);
        lblFileName->ChangeOptions(lblFileName->GetOptions() | kFixedSize);

        frameOpenFile->AddFrame(btnOpenFile, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
        frameOpenFile->AddFrame(lblFileName, new TGLayoutHints(kLHintsLeft | kLHintsCenterY | kLHintsExpandX, dx)); // left right top bottom
    }
    AddFrame(frameOpenFile, new TGLayoutHints(kLHintsExpandX, 0, 0, 0, dy));

    // File browser label
    AddFrame(new TGLabel(this, "ASCII file preview:"), new TGLayoutHints(kLHintsLeft, 0, 0, dy*2, 0));

    // File browser
    txtFileBrowser = new TGTextView(this);
    txtFileBrowser->SetBackgroundColor(GraphicsHelper::colorAppWindow->GetPixel());

//    txtFileBrowser->AddLine("no file loaded");
//    txtFileBrowser->ChangeOptions(0);
    AddFrame(txtFileBrowser, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, dy, dy));

    // Energy column
//    TGHorizontalFrame* frameEnergyColumn = new TGHorizontalFrame(this);
//    {
//        TGLabel* lblEnergyColumn = new TGLabel(frameEnergyColumn, "Energy column #");
//        numEnergyColumn = new TGNumberEntry(frameEnergyColumn, 1, 2, UiHelper::getUId(), TGNumberFormat::kNESInteger,
//                TGNumberFormat::kNEANonNegative,
//                TGNumberFormat::kNELLimitMinMax,
//                1, 99);
//        frameEnergyColumn->AddFrame(lblEnergyColumn, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
//        frameEnergyColumn->AddFrame(numEnergyColumn, new TGLayoutHints(kLHintsRight | kLHintsCenterY));
//    }
//    AddFrame(frameEnergyColumn, new TGLayoutHints(kLHintsExpandX, 0, 0, dy, dy));

    // Counts column
//    TGHorizontalFrame* frameCountsColumn = new TGHorizontalFrame(this);
//    {
//        TGLabel* lblCountsColumn = new TGLabel(frameCountsColumn, "Counts column #");
//        numCountsColumn = new TGNumberEntry(frameCountsColumn, 2, 2, UiHelper::getUId(), TGNumberFormat::kNESInteger,
//                TGNumberFormat::kNEANonNegative,
//                TGNumberFormat::kNELLimitMinMax,
//                1, 99);
//        frameCountsColumn->AddFrame(lblCountsColumn, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
//        frameCountsColumn->AddFrame(numCountsColumn, new TGLayoutHints(kLHintsRight | kLHintsCenterY));
//    }
//    AddFrame(frameCountsColumn, new TGLayoutHints(kLHintsExpandX, 0, 0, dy, dy));

    // Import Spectrum button
    btnImportSpectrum = new TGTextButton(this, "Import Spectrum");
    btnImportSpectrum->Connect("Clicked()", this->ClassName(), this, "onImportSpectrumClicked()");
    btnImportSpectrum->SetEnabled(kFALSE);
    AddFrame(btnImportSpectrum, new TGLayoutHints(kLHintsExpandX, 0, 0, dy, dy));

    // Histogram preview label
    AddFrame(new TGLabel(this, "Preview:"), new TGLayoutHints(kLHintsLeft, 0, 0, dy*2, 0));

    // Histogram canvas
    TRootEmbeddedCanvas* embedHist = new TRootEmbeddedCanvas("embedHist", this);

    canvasHist = embedHist->GetCanvas();
    canvasHist->SetMargin(0.09, 0.04, 0.14, 0.1);
    canvasHist->SetLogy();
    AddFrame(embedHist, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, dy, 0));

    // Temp hack
//    btnSetRange = new TGTextButton(this, " Set Range ");
//    btnSetRange->Connect("Clicked()", this->ClassName(), this, "onSetRangeClicked()");
//    AddFrame(btnSetRange, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
}

AbstractImportSpectrumPresenter* AbstractImportSpectrumView::instantinatePresenter() {
    return new AbstractImportSpectrumPresenter(this);
}

// Calls to Presenter

void AbstractImportSpectrumView::onOpenFileClicked(){
    presenter->onOpenFileClicked();
}

void AbstractImportSpectrumView::onImportSpectrumClicked(){
    presenter->onImportSpectrumClicked();
}

// Calls from Presenter
void AbstractImportSpectrumView::loadFile(TString* fileNamePath){
    // Load file contents to FileBrowser
    txtFileBrowser->LoadFile(fileNamePath->Data());
    // Update FileName label
    TString* fileName = StringUtils::stripFileName(fileNamePath);
    lblFileName->SetText(fileName->Data());
    lblFileName->Disable(kFALSE);

    // Petr Stepanov: now importing automatically
    // Enable Import button
    // btnImportSpectrum->SetEnabled(kTRUE);
}

//Int_t AbstractImportSpectrumView::getEnergyColumnNumber(){
//    return numEnergyColumn->GetIntNumber();
//}

//Int_t AbstractImportSpectrumView::getCountsColumnNumber(){
//    return numCountsColumn->GetIntNumber();
//}

TString* AbstractImportSpectrumView::getFileName(){
    const char* s = (lblFileName->GetText())->GetString();
    return new TString(s);
}

void AbstractImportSpectrumView::drawHistogram(TH1F* h){
	// Copy histogram so we dont modify original histogram colors in the Model
	TH1* hist = new TH1F(*h);
    canvasHist->cd();
    hist->SetLineColor(getHistogramColor());
    hist->SetFillColorAlpha(getHistogramColor(), 0.2);
    hist->GetXaxis()->SetLabelOffset(0.03);
    hist->GetYaxis()->SetLabelOffset(0.015);
    hist->Draw();
    GraphicsHelper* graphicsHelper = GraphicsHelper::getInstance();

    hist->GetXaxis()->SetLabelSize(2*GraphicsHelper::TEXT_SIZE_NORMAL);
    hist->GetYaxis()->SetLabelSize(2*GraphicsHelper::TEXT_SIZE_NORMAL);

    hist->SetStats(0);
    canvasHist->Update();
}

Int_t AbstractImportSpectrumView::getHistogramColor(){
    return GraphicsHelper::colorPrimary->GetNumber();
}
