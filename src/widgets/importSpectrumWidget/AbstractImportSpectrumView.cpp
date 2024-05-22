/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "AbstractImportSpectrumView.h"
#include "AbstractImportSpectrumPresenter.h"

#include "../../util/StringUtils.h"
#include "../../util/GraphicsHelper.h"
#include "../../model/Constants.h"
#include "../../util/UiHelper.h"
#include <TSystem.h>
#include <TGClient.h>

#include <TGResourcePool.h>

ClassImpQ(AbstractImportSpectrumView);

AbstractImportSpectrumView::AbstractImportSpectrumView(const TGWindow* w) : AbstractView<AbstractImportSpectrumPresenter>(w){
    initUI();
    miniHist = NULL;
    miniStats = NULL;
}

AbstractImportSpectrumView::~AbstractImportSpectrumView() {
    delete btnOpenFile;
    delete lblFileName;
    delete txtFileBrowser;
    delete btnImportSpectrum;
    delete embedCanvas;
    delete miniHist;
}

void AbstractImportSpectrumView::initUI(){
    // Open File Frame
    TGHorizontalFrame* frameOpenFile = new TGHorizontalFrame(this);
    {
        btnOpenFile = new TGTextButton(frameOpenFile, " Open File...");
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
    AddFrame(new TGLabel(this, "ASCII file listing:"), new TGLayoutHints(kLHintsLeft, 0, 0, dy*2, 0));

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
    AddFrame(new TGLabel(this, "Spectrum plot:"), new TGLayoutHints(kLHintsLeft, 0, 0, dy*2, 0));

    // Histogram canvas
    embedCanvas = new TRootEmbeddedCanvas("embedHist", this, 10, 10, EFrameType::kChildFrame); // EFrameType::kChildFrame - no border
    embedCanvas->SetHeight(210);
    AddFrame(embedCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsBottom, 0, 0, dy, 0));

    TCanvas* canvas = embedCanvas->GetCanvas();

    // Make canvas same color as the UI panels
    Pixel_t bgColor = TColor::GetColor(232, 232, 232);
    canvas->SetFillColor(bgColor);

    // Set canvas margins and log y axis
    canvas->SetTopMargin(0.05); // 0.1
    canvas->SetBottomMargin(0.15);
    canvas->SetLeftMargin(0.1);
    canvas->SetRightMargin(0.05); // 0.05
    canvas->SetLogy();

    // Map windows so we can get the real pixel width of the embedded canvas
//    MapSubwindows();
//    Layout();

//    ((TGFrame *)this)->Connect("ProcessedConfigure(Event_t*)",
//                               "AbstractImportSpectrumView", this, "OnProcessedConfigure(Event_t*)");
//
//    ((TGFrame *)this)->Connect("ProcessedEvent(Event_t*)",
//                               "AbstractImportSpectrumView", this, "OnProcessedEvent(Event_t*)");
//
//    canvas->Modified();
//    canvas->Update();

    // Global slots
    // Connection from all objects of a class you should write
    // Can be "MyMainFrame" or "ModalDialogFrame"
    // std::cout << this->GetMainFrame()->ClassName() << std::endl;
    TQObject::Connect(this->GetMainFrame()->ClassName(), "uiReady()", this->ClassName(), this, "handleUiReady()");
}

AbstractImportSpectrumPresenter* AbstractImportSpectrumView::instantinatePresenter() {
    return new AbstractImportSpectrumPresenter(this);
}

// Calls to Presenter

void AbstractImportSpectrumView::onOpenFileClicked(){
    presenter->onOpenFileClicked();
}

//void AbstractImportSpectrumView::onImportSpectrumClicked(){
//    presenter->onImportSpectrumClicked();
//}

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

void AbstractImportSpectrumView::drawHistogram(TH1F* importedHist){

	// Reset histogram because of the issues with stat box
	// miniHist->Reset("ICESM"); // https://root-forum.cern.ch/t/how-to-clear-the-histograms/23697/3
	// Or simply delete the object and re-create
	if (miniHist != NULL){
		delete miniHist;
	}
	if (miniStats != NULL){
		delete miniStats;
	}

	// Use Clone(), Copy() does not create full copy!
	miniHist = (TH1F*)(importedHist->Clone());

    // Set Canvas editable just in case it affects something
	embedCanvas->GetCanvas()->SetEditable(kTRUE);

	// Copy histogram so we dont modify original histogram colors in the Model

	miniHist->Print("base");
	miniHist->SetTitle("");
	miniHist->GetXaxis()->SetTitle("Energy, keV");
	miniHist->GetYaxis()->SetTitle("Counts");

    embedCanvas->GetCanvas()->cd();

    // hist->SetLineColor(getHistogramColor());
    miniHist->SetLineColor(kGray+1);
    miniHist->GetXaxis()->SetLabelOffset(0.02);
    miniHist->GetYaxis()->SetLabelOffset(0.01);

    miniHist->GetXaxis()->SetLabelSize(0.06);
    miniHist->GetYaxis()->SetLabelSize(0.06);

    miniHist->GetXaxis()->CenterTitle(true);
    miniHist->GetYaxis()->CenterTitle(true);

    miniHist->GetXaxis()->SetTitleOffset(1.2);
    miniHist->GetXaxis()->SetTitleSize(0.06);
    miniHist->GetYaxis()->SetTitleOffset(0.85);
    miniHist->GetYaxis()->SetTitleSize(0.06);

//    gStyle->SetOptStat(10);
    miniHist->SetFillStyle(EFillStyle::kFDotted1);
    miniHist->SetFillColor(kGray);
    miniHist->SetStats(kFALSE);

    embedCanvas->GetCanvas()->Clear();
    miniHist->Draw();


    // Add custom Statistic Box (modifying default is pain)
    miniStats = new TPaveStats();
    miniStats->SetBorderSize(0);
    miniStats->SetOptStat(10); // Disable name line - see TPaveStats::Paint()
	TString labelBins = TString::Format("Bins =  %d", miniHist->GetNbinsX());
	TString labelEntries = TString::Format("Entries =  %d", (int)miniHist->Integral());
	miniStats->AddText(labelBins.Data());
	miniStats->AddText(labelEntries.Data());
	miniStats->Draw();

    // Align Statistic Box
    GraphicsHelper::alignStats(embedCanvas->GetCanvas(), Alignment::TOP_RIGHT, Decoration::TRANSPAREN, 0.08, 0.27);

    // Set Canvas not editable by user
    embedCanvas->GetCanvas()->SetEditable(kFALSE);
}

Int_t AbstractImportSpectrumView::getHistogramColor(){
    return GraphicsHelper::colorPrimary->GetNumber();
}

void AbstractImportSpectrumView::handleUiReady(){
    GraphicsHelper::drawXCanvas(embedCanvas);
}
