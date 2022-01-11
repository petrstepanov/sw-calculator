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
#include <TGClient.h>
#include <TGResourcePool.h>

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
    AddFrame(new TGLabel(this, "ASCII file:"), new TGLayoutHints(kLHintsLeft, 0, 0, dy*2, 0));

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
    // btnImportSpectrum = new TGTextButton(this, "Import Spectrum");
    // btnImportSpectrum->Connect("Clicked()", this->ClassName(), this, "onImportSpectrumClicked()");
    // btnImportSpectrum->SetEnabled(kFALSE);
    // AddFrame(btnImportSpectrum, new TGLayoutHints(kLHintsExpandX, 0, 0, dy, dy));

    // Histogram preview label
    AddFrame(new TGLabel(this, "Graphic preview:"), new TGLayoutHints(kLHintsLeft, 0, 0, dy*2, 0));

    // Histogram canvas
    // Following options do not draw lines in TGFrame::Draw3dRectangle():
    // EFrameType::kChildFrame, EFrameType::kTransientFrame no: EFrameType::kSunkenFrame
    TRootEmbeddedCanvas* embedHist = new TRootEmbeddedCanvas("embedHist", this, 10, 10, EFrameType::kChildFrame);
    embedHist->SetHeight(210);
    AddFrame(embedHist, new TGLayoutHints(kLHintsExpandX, 0, 0, dy, 0));

    // Bitwise calculator https://bitwisecmd.com/
    // embedHist->ChangeOptions(kFixedHeight);

    canvasHist = embedHist->GetCanvas();
//    canvasHist->SetEditable(0);
    canvasHist->SetBorderMode(0);
    canvasHist->SetBorderSize(0);
    Pixel_t bgColor = TColor::GetColor(232, 232, 232);
    canvasHist->SetFillColor(bgColor);
    canvasHist->SetTopMargin(0.05); // 0.1
    canvasHist->SetBottomMargin(0.15);
    canvasHist->SetLeftMargin(0.1);
    canvasHist->SetRightMargin(0.01); // 0.05
    canvasHist->SetLogy();
    // GraphicsHelper::drawXCanvas(canvasHist);
    // canvasHist->Update();
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
	TH1* hist = new TH1F();
	h->Copy(*hist); // Copy h into hist
	hist->SetTitle("");
	hist->GetXaxis()->SetTitle("Energy, keV");
	hist->GetYaxis()->SetTitle("Counts");

    canvasHist->cd();
    canvasHist->SetFrameFillStyle(EFillStyle::kFEmpty);

    // hist->SetLineColor(getHistogramColor());
    hist->SetLineColor(kGray+1);
    hist->GetXaxis()->SetLabelOffset(0.02);
    hist->GetYaxis()->SetLabelOffset(0.01);

    hist->GetXaxis()->SetLabelSize(0.06);
    hist->GetYaxis()->SetLabelSize(0.06);

    hist->GetXaxis()->SetTitleOffset(1.2);
    hist->GetXaxis()->SetTitleSize(0.06);
    hist->GetYaxis()->SetTitleOffset(0.85);
    hist->GetYaxis()->SetTitleSize(0.06);
    hist->Draw();

    GraphicsHelper::alignStats(canvasHist, Alignment::TOP_RIGHT, Decoration::TRANSPARENT, 0.07, 0.35);

//     canvasHist->Modified();
    canvasHist->Update();
}

Int_t AbstractImportSpectrumView::getHistogramColor(){
    return GraphicsHelper::colorPrimary->GetNumber();
}
