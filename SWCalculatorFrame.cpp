/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SWCalculatorFrame.cpp
 * Author: petrstepanov
 * 
 * Created on August 1, 2017, 5:02 PM
 */

#include "SWCalculatorFrame.h"
#include "SWCalculatorFrameLinkDef.h"

#include <algorithm>  //min
#include <functional>
#include <sstream>
#include <TRootEmbeddedCanvas.h>
#include <TBox.h>
#include <RooDataHist.h>
#include <RooFitResult.h>
#include <RooChi2Var.h>
#include <RooMinuit.h>
#include <RooAddPdf.h>
#include <TGFileDialog.h>
#include <TGMsgBox.h>
#include <TG3DLine.h>
//#include <RQ_OBJECT.h>
#include "GraphicsHelper.h"
#include "AbstractModelProvider.h"
#include "ParabolaGaussModelProvider.h"
#include "ParabolaLorentzianModelProvider.h"
#include "CompositeModelProvider.h"
#include "HistProcessor.h"
#include "StringUtils.h"
#include "FileUtils.h"
#include "Variable.h"

using namespace RooFit;

//ClassImp(SWCalculatorFrame)

SWCalculatorFrame::SWCalculatorFrame(const TGWindow* p, UInt_t w, UInt_t h){
	fullHist = NULL;
	Int_t dx = 10; // horizontal padding
	Int_t dy = 5; // vertical padding
	Int_t tabWidth = 420; //px
	strFileName = NULL;

	// main frame
	mainFrame = new TGMainFrame(p, w, h, kMainFrame | kVerticalFrame);
	mainFrame->SetName("mainFrame");
	mainFrame->SetWindowName("SW Calculator");
	mainFrame->SetIconPixmap("icon.png");

	// menu
	//TGDockableFrame* fMenuDock = new TGDockableFrame(mainFrame);
	// mainFrame->AddFrame(fMenuDock, new TGLayoutHints(kLHintsExpandX, 0, 0, 1, 0));

	//TGPopupMenu* fMenuFile = new TGPopupMenu(gClient->GetRoot());
	//fMenuFile->AddEntry("&Exit", M_FILE_EXIT);
	//fMenuFile->Connect("Activated(Int_t)", "SWCalculatorFrame", this, "HandleMenu(Int_t)");

	//TGMenuBar* fMenuBar = new TGMenuBar(fMenuDock, 1, 1, kHorizontalFrame);
	//fMenuBar->AddPopup("&File", fMenuFile, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0));
	//fMenuDock->AddFrame(fMenuBar, new TGLayoutHints(kLHintsTop | kLHintsExpandX));

	TGHorizontalFrame* mainChildFrame = new TGHorizontalFrame(mainFrame);

	// tab widget
	tabsWidget = new TGTab(mainChildFrame, tabWidth, 600);

	// container of "Import Spectrum" tab
	TGCompositeFrame *tabImport;
	tabImport = tabsWidget->AddTab("Import Spectrum");
	tabImport->SetLayoutManager(new TGVerticalLayout(tabImport));

	// row "Open File"
	TGHorizontalFrame* frameOpenFile = new TGHorizontalFrame(tabImport);
	TGTextButton* btnOpenFile = new TGTextButton(frameOpenFile, "Open File");
	btnOpenFile->Connect("Clicked()", "SWCalculatorFrame", this, "openFile()");
	frameOpenFile->AddFrame(btnOpenFile, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 0, 0));
	lblFileName = new TGLabel(frameOpenFile, "");
	frameOpenFile->AddFrame(lblFileName, new TGLayoutHints(kLHintsExpandX, 0, 0, 5, 0));

	tabImport->AddFrame(frameOpenFile, new TGLayoutHints(kLHintsExpandX, dx, dx, dx, dy));

	// textarea
	textEdit = new TGTextEdit(tabImport);
	//   textEdit->LoadFile("TxtEdit1569");
	tabImport->AddFrame(textEdit, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy, dy));

	// row "Energy Column"
	TGHorizontalFrame* frameEnergyColumn = new TGHorizontalFrame(tabImport);
	TGLabel* lblEnergyColumn = new TGLabel(frameEnergyColumn, "Energy Column #");

	numEnergyColumn = new TGNumberEntry(frameEnergyColumn, 1, 2, -1, TGNumberFormat::kNESInteger,
		TGNumberFormat::kNEANonNegative,
		TGNumberFormat::kNELLimitMinMax,
		1, 99);

	frameEnergyColumn->AddFrame(lblEnergyColumn, new TGLayoutHints(kLHintsNormal, 0, 0, 5, 0));
	frameEnergyColumn->AddFrame(numEnergyColumn, new TGLayoutHints(kLHintsRight));

	tabImport->AddFrame(frameEnergyColumn, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

	// row "Counts Column"
	TGHorizontalFrame* frameCountsColumn = new TGHorizontalFrame(tabImport);
	TGLabel* lblCountsColumn = new TGLabel(frameCountsColumn, "Counts Column #");

	numCountsColumn = new TGNumberEntry(frameCountsColumn, 2, 2, -1, TGNumberFormat::kNESInteger,
		TGNumberFormat::kNEANonNegative,
		TGNumberFormat::kNELLimitMinMax,
		1, 99);

	frameCountsColumn->AddFrame(lblCountsColumn, new TGLayoutHints(kLHintsNormal, 0, 0, 5, 0));
	frameCountsColumn->AddFrame(numCountsColumn, new TGLayoutHints(kLHintsRight));

	tabImport->AddFrame(frameCountsColumn, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

	// button "Import Spectrum"
	TGHorizontalFrame* frameImportSpectrum = new TGHorizontalFrame(tabImport);
	btnImportSpectrum = new TGTextButton(frameImportSpectrum, "Import Spectrum");
	btnImportSpectrum->Connect("Clicked()", "SWCalculatorFrame", this, "importSpectrum()");
	btnImportSpectrum->SetEnabled(false);
	frameImportSpectrum->AddFrame(btnImportSpectrum, new TGLayoutHints(kLHintsExpandX));

	tabImport->AddFrame(frameImportSpectrum, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

	// canvas for histogram
	TGHorizontalFrame *frameCanvasHist = new TGHorizontalFrame(tabImport);
	TRootEmbeddedCanvas *embedHist = new TRootEmbeddedCanvas("embedHist", frameCanvasHist, tabWidth, 50);
	frameCanvasHist->AddFrame(embedHist, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
	canvasHist = embedHist->GetCanvas(); //new TCanvas("canvasHist", 10, 10, idHist);
	canvasHist->SetLogy();

	tabImport->AddFrame(frameCanvasHist, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy, dx));

	// container of "Fit Data" tab
	TGCompositeFrame *tabFit;
	tabFit = tabsWidget->AddTab("Fit Data");
	tabFit->SetLayoutManager(new TGVerticalLayout(tabFit));

	// Peak Position Row
	TGHorizontalFrame *framePeakPosition = new TGHorizontalFrame(tabFit);
	TGLabel *lblPeakPosition = new TGLabel(framePeakPosition, "Peak Center, keV");
	numPeakPosition = new TGNumberEntry(framePeakPosition, 485, 4, -1, TGNumberFormat::kNESInteger,
		TGNumberFormat::kNEAAnyNumber,
		TGNumberFormat::kNELLimitMinMax,
		-100, 2000);
	numPeakPosition->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorFrame",
		this, "numPeakPositionChanged()");
	framePeakPosition->AddFrame(lblPeakPosition, new TGLayoutHints(kLHintsNormal, 0, 0, dy, 0));
	framePeakPosition->AddFrame(numPeakPosition, new TGLayoutHints(kLHintsRight));
	tabFit->AddFrame(framePeakPosition, new TGLayoutHints(kLHintsExpandX, dx, dx, dx, dy));

	// Fit Range Row
	TGHorizontalFrame *frameFitRange = new TGHorizontalFrame(tabFit);
	TGLabel *lblFitRange = new TGLabel(frameFitRange, "Fit Range, keV");
	numFitMin = new TGNumberEntry(frameFitRange, 485, 4, -1, TGNumberFormat::kNESInteger,
		TGNumberFormat::kNEAAnyNumber,
		TGNumberFormat::kNELLimitMinMax,
		-9999, 9999);
	numFitMin->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorFrame",
		this, "numFitMinChanged()");
	TGLabel *lblFitDash = new TGLabel(frameFitRange, "-");
	numFitMax = new TGNumberEntry(frameFitRange, 537, 4, -1, TGNumberFormat::kNESInteger,
		TGNumberFormat::kNEAAnyNumber,
		TGNumberFormat::kNELLimitMinMax,
		-9999, 9999);
	numFitMax->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorFrame",
		this, "numFitMaxChanged()");
	frameFitRange->AddFrame(lblFitRange, new TGLayoutHints(kLHintsNormal, 0, 0, dy, 0));
	frameFitRange->AddFrame(numFitMax, new TGLayoutHints(kLHintsRight));
	frameFitRange->AddFrame(lblFitDash, new TGLayoutHints(kLHintsRight, dx, dx, dy, 0));
	frameFitRange->AddFrame(numFitMin, new TGLayoutHints(kLHintsRight));

	tabFit->AddFrame(frameFitRange, new TGLayoutHints(kLHintsExpandX, dx, dx, dx, dy));

	// Separator
	TGHorizontal3DLine* l1 = new TGHorizontal3DLine(tabFit);
	tabFit->AddFrame(l1, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

	// Integration Parameters
	TGHorizontalFrame *frameSWidth = new TGHorizontalFrame(tabFit);
	TGLabel *lblSWidth = new TGLabel(frameSWidth, "S Region Width");
	lblRescale1 = new TGLabel(frameSWidth, "2 x ");
	numSWidth = new TGNumberEntry(frameSWidth, 1.60, 4, -1, TGNumberFormat::kNESRealTwo,
		TGNumberFormat::kNEANonNegative,
		TGNumberFormat::kNELLimitMin,
		0.01, 9.99);
	frameSWidth->AddFrame(lblSWidth, new TGLayoutHints(kLHintsNormal, 0, 0, dy, 0));
	frameSWidth->AddFrame(numSWidth, new TGLayoutHints(kLHintsRight));
	frameSWidth->AddFrame(lblRescale1, new TGLayoutHints(kLHintsRight, 0, 5, 3, 0));

	tabFit->AddFrame(frameSWidth, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

	TGHorizontalFrame *frameWWidth = new TGHorizontalFrame(tabFit);
	TGLabel *lblWWidth = new TGLabel(frameWWidth, "W Regions Width");
	lblRescale2 = new TGLabel(frameWWidth, "2 x ");
	numWWidth = new TGNumberEntry(frameWWidth, 3.00, 4, -1, TGNumberFormat::kNESRealTwo,
		TGNumberFormat::kNEANonNegative,
		TGNumberFormat::kNELLimitMin,
		0.01, 9.99);
	frameWWidth->AddFrame(lblWWidth, new TGLayoutHints(kLHintsNormal, 0, 0, dy, 0));
	frameWWidth->AddFrame(numWWidth, new TGLayoutHints(kLHintsRight));
	frameWWidth->AddFrame(lblRescale2, new TGLayoutHints(kLHintsRight, 0, 5, 3, 0));

	tabFit->AddFrame(frameWWidth, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

	TGHorizontalFrame *frameWShift = new TGHorizontalFrame(tabFit);
	TGLabel *lblWShift = new TGLabel(frameWShift, "W Regions Shift");
	lblRescale3 = new TGLabel(frameWShift, "2 x ");
	numWShift = new TGNumberEntry(frameWShift, 2.76, 4, -1, TGNumberFormat::kNESRealTwo,
		TGNumberFormat::kNEANonNegative,
		TGNumberFormat::kNELLimitMin,
		0.01, 9.99);
	frameWShift->AddFrame(lblWShift, new TGLayoutHints(kLHintsNormal, 0, 0, dy, 0));
	frameWShift->AddFrame(numWShift, new TGLayoutHints(kLHintsRight));
	frameWShift->AddFrame(lblRescale3, new TGLayoutHints(kLHintsRight, 0, 5, 3, 0));

	tabFit->AddFrame(frameWShift, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

	// Separator
	TGHorizontal3DLine* l2 = new TGHorizontal3DLine(tabFit);
	tabFit->AddFrame(l2, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

	// Model parameters
	TGHorizontalFrame* modelParamsFrame = new TGHorizontalFrame(tabFit);
//	modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Model details:"), new TGLayoutHints(kLHintsNormal, 0, 30, 5, 0));
	hasParabola = new TGCheckButton(modelParamsFrame, "Parabola", -1);
	modelParamsFrame->AddFrame(hasParabola, new TGLayoutHints(kLHintsNormal, 0, 20, 4, 0));

	numGauss = new TGNumberEntry(modelParamsFrame, 1, 1, -1, TGNumberFormat::kNESInteger,
		TGNumberFormat::kNEANonNegative,
		TGNumberFormat::kNELLimitMinMax,
		0, 4);
	modelParamsFrame->AddFrame(numGauss, new TGLayoutHints(kLHintsNormal, 0, 5, 2, 0));        
	modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Gauss"), new TGLayoutHints(kLHintsNormal, 0, 30, 5, 0));

	numLorentz = new TGNumberEntry(modelParamsFrame, 1, 1, -1, TGNumberFormat::kNESInteger,
		TGNumberFormat::kNEANonNegative,
		TGNumberFormat::kNELLimitMinMax,
		0, 4);
	modelParamsFrame->AddFrame(numLorentz, new TGLayoutHints(kLHintsNormal, 0, 5, 2, 0));
	modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Lorentz"), new TGLayoutHints(kLHintsNormal, 0, 30, 5, 0));

	numLorentzComplex = new TGNumberEntry(modelParamsFrame, 1, 1, -1, TGNumberFormat::kNESInteger,
		TGNumberFormat::kNEANonNegative,
		TGNumberFormat::kNELLimitMinMax,
		0, 4);
	modelParamsFrame->AddFrame(numLorentzComplex, new TGLayoutHints(kLHintsNormal, 0, 5, 2, 0));
	modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Lorentz Sum"), new TGLayoutHints(kLHintsNormal, 0, 30, 5, 0));

//	hasOrtho = new TGCheckButton(modelParamsFrame, "Ortho Exps", -1);
//	modelParamsFrame->AddFrame(hasOrtho, new TGLayoutHints(kLHintsNormal, 0, 20, 4, 0));        

	// Draw Option
//	fitFunctionType = new TGComboBox(modelParamsFrame, 0);
//	fitFunctionType->AddEntry("Gauss",1);
//	fitFunctionType->AddEntry("Lorentz",2);
//	fitFunctionType->Select(2);
//	fitFunctionType->Resize(130, 20);
        
//	modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Gauss"), new TGLayoutHints(kLHintsNormal, 0, 0, 5, 0));
//	modelParamsFrame->AddFrame(fitFunctionType, new TGLayoutHints(kLHintsNormal, 0, 0, 2, 0));
	tabFit->AddFrame(modelParamsFrame, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

	// Fit Button
	TGHorizontalFrame* frameFitSpectrum = new TGHorizontalFrame(tabFit);
	TGTextButton* btnFitSpectrum = new TGTextButton(frameFitSpectrum, "Fit and Calculate S and W");
	btnFitSpectrum->Connect("Clicked()", "SWCalculatorFrame", this, "fitSpectrum()");
	frameFitSpectrum->AddFrame(btnFitSpectrum, new TGLayoutHints(kLHintsExpandX));

	tabFit->AddFrame(frameFitSpectrum, new TGLayoutHints(kLHintsExpandX, dx, dx, dy + 3, dy));

	// Fit Result TextBox
	txtFitResult = new TGTextEdit(tabFit);
	tabFit->AddFrame(txtFitResult, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy, dy));

	// Attach Tabs Widget
	tabsWidget->SetTab(0);
	tabsWidget->SetEnabled(1, false);

	// tabsWidget->SetWidth(540);//Resize(tabsWidget->GetDefaultSize());
	mainChildFrame->AddFrame(tabsWidget, new TGLayoutHints(kLHintsLeft | kLHintsBottom | kLHintsExpandY, dx, dx, dx, dx));

	// Right
	TGVerticalFrame* frameRightVertical = new TGVerticalFrame(mainChildFrame);
	TGHorizontalFrame* frameExportButtons = new TGHorizontalFrame(frameRightVertical);

	numDisplayMin = new TGNumberEntry(frameExportButtons, 0, 6, -1, TGNumberFormat::kNESRealOne,
		TGNumberFormat::kNEAAnyNumber,
		TGNumberFormat::kNELLimitMinMax,
		-9999, 9999);
	//numDisplayMin->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorFrame",
	//	this, "numDisplayMinChanged()");
	TGLabel *lblDisplayDash = new TGLabel(frameExportButtons, "-");
	numDisplayMax = new TGNumberEntry(frameExportButtons, 0, 6, -1, TGNumberFormat::kNESRealOne,
		TGNumberFormat::kNEAAnyNumber,
		TGNumberFormat::kNELLimitMinMax,
		-9999, 9999);
	//numDisplayMax->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorFrame",
	//	this, "numDisplayMaxChanged()");
	btnApplyZoom = new TGTextButton(frameExportButtons, "Apply Display Range");
	btnApplyZoom->Connect("Clicked()", "SWCalculatorFrame", this, "applyZoom()");
	btnResetZoom = new TGTextButton(frameExportButtons, "Reset");
	btnResetZoom->Connect("Clicked()", "SWCalculatorFrame", this, "resetZoom()");

	frameExportButtons->AddFrame(numDisplayMin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom
	frameExportButtons->AddFrame(lblDisplayDash, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, dy, 0));  // left, right, top, bottom
	frameExportButtons->AddFrame(numDisplayMax, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom
	frameExportButtons->AddFrame(btnApplyZoom, new TGLayoutHints(kLHintsLeft | kLHintsTop, dx, dx, 0, 0));  // left, right, top, bottom
	frameExportButtons->AddFrame(btnResetZoom, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom

	btnSaveData = new TGTextButton(frameExportButtons, "Export Fit Data");
	btnSaveData->Connect("Clicked()", "SWCalculatorFrame", this, "saveData()");
	btnSaveData->SetEnabled(false);
	frameExportButtons->AddFrame(btnSaveData, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 0, 0));  // left, right, top, bottom

	btnSaveImage = new TGTextButton(frameExportButtons, "Save Image");
	btnSaveImage->Connect("Clicked()", "SWCalculatorFrame", this, "saveImage()");
	btnSaveImage->SetEnabled(false);
	frameExportButtons->AddFrame(btnSaveImage, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, dx, 0, 0));

	setToolbarEnabled(kFALSE);

	// Attach Right Canvas (Plot)
	TRootEmbeddedCanvas *embedPlot = new TRootEmbeddedCanvas("embedPlot", frameRightVertical, 600, 600);
	frameRightVertical->AddFrame(embedPlot, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY, dx, dx, dx, dx));
	canvasPlot = embedPlot->GetCanvas();

	frameRightVertical->AddFrame(frameExportButtons, new TGLayoutHints(kLHintsExpandX, 0, dx, 0, 0));

	mainChildFrame->AddFrame(frameRightVertical, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, dx));

	mainFrame->AddFrame(mainChildFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));


	mainFrame->SetMWMHints(kMWMDecorAll, kMWMFuncAll, kMWMInputModeless);
	mainFrame->MapSubwindows();

	mainFrame->Resize(mainFrame->GetDefaultSize());
	mainFrame->MapWindow();

	// Preview Canvas Settings
	// canvasHist->SetMargin(0.12, 0.07, 0.12, 0.07);

	// Plot Canvas Settings
	// canvasPlot->cd();
	padData = new TPad("padData", "Pad with data", 0.0, 0.3, 1.0, 1.0, kWhite);
	padChi2 = new TPad("padChi2", "Pad with chi2", 0.0, 0.0, 1.0, 0.3, kWhite);
	padData->Draw();
	padData->SetMargin(0.12, 0.03, 0.15, 0.05); // left, right, bottom, top
	padChi2->Draw();
	padChi2->SetMargin(0.12, 0.03, 0.15, 0.05); // left, right, bottom, top
}

void SWCalculatorFrame::applyZoom(){
	fitFrame->GetXaxis()->SetRangeUser(numDisplayMin->GetNumber(), numDisplayMax->GetNumber());
	chiFrame->GetXaxis()->SetRangeUser(numDisplayMin->GetNumber(), numDisplayMax->GetNumber());
	drawMainCanvas();
}

void SWCalculatorFrame::resetZoom(){
	if (fitFrame != NULL && chiFrame != NULL){
		fitFrame->GetXaxis()->SetRangeUser(numFitMin->GetIntNumber(), numFitMax->GetIntNumber());
		chiFrame->GetXaxis()->SetRangeUser(numFitMin->GetIntNumber(), numFitMax->GetIntNumber());
	}
	numDisplayMax->SetNumber(numFitMax->GetIntNumber());
	numDisplayMin->SetNumber(numFitMin->GetIntNumber());
	padData->cd();
	drawMainCanvas();
}

void SWCalculatorFrame::drawMainCanvas(){
	padData->cd();
	fitFrame->Draw();
	padData->SetLogy();
	padChi2->cd();
	chiFrame->Draw();
	padData->Update();
	padChi2->Update();
	canvasPlot->Update();
}

void SWCalculatorFrame::numFitMinChanged(){
	Int_t peakCenter = numPeakPosition->GetIntNumber();
	Int_t fitMin = numFitMin->GetIntNumber();
	numFitMax->SetIntNumber(peakCenter + (peakCenter - fitMin));
}
void SWCalculatorFrame::numFitMaxChanged(){
	Int_t peakCenter = numPeakPosition->GetIntNumber();
	Int_t fitMax = numFitMax->GetIntNumber();
	numFitMin->SetIntNumber(peakCenter - (fitMax - peakCenter));
}
void SWCalculatorFrame::numPeakPositionChanged(){
	Int_t peakCenter = numPeakPosition->GetIntNumber();
	Int_t eMin = (Int_t)(fullHist->GetXaxis()->GetXmin());
	Int_t eMax = (Int_t)(fullHist->GetXaxis()->GetXmax() + 0.5);
	Int_t limitWidth = std::min(peakCenter - eMin, eMax - peakCenter);
	Int_t currentHalfWidth = (numFitMax->GetNumber() - numFitMin->GetNumber()) / 2;
	numFitMin->SetLimitValues(peakCenter - limitWidth, peakCenter - 1);
	numFitMax->SetLimitValues(peakCenter + 1, peakCenter + limitWidth);
	numFitMin->SetIntNumber(peakCenter - currentHalfWidth);
}

void SWCalculatorFrame::HandleMenu(Int_t id){
	switch (id) {
	case M_FILE_EXIT:
		CloseWindow();   // terminate theApp no need to use SendCloseMessage()
		break;

	default:
		std::cout << "Menu item " << id << " selected" << std::endl;
		break;
	}
}

void SWCalculatorFrame::CloseWindow(){
	delete this;
}

void SWCalculatorFrame::ShowOkDialog(TString* heading, TString* message){
	int retval;
	// EMsgBoxIcon icontype = kMBIconStop;
	// mainFrame->Disconnect("CloseWindow()");
	// mainFrame->Connect("CloseWindow()", "TestMsgBox", this, "TryToClose()");
	new TGMsgBox(gClient->GetRoot(), mainFrame,
		heading->Data(), message->Data(),
		kMBIconAsterisk, kMBOk, &retval);
	// fMain->Disconnect("CloseWindow()");
	// fMain->Connect("CloseWindow()", "TestMsgBox", this, "CloseWindow()");
}

void SWCalculatorFrame::openFile(void){
	// show file dialog
	static TString dir(".");
	TGFileInfo fi;
	const char *filetypes[] = { "All files", "*",
		"Data files", "*.dat",
		"CSV files", "*.csv",
		"Text files", "*.[tT][xX][tT]",
		0, 0 };

	fi.fFileTypes = filetypes;
	fi.fIniDir = StrDup(dir);
	new TGFileDialog(gClient->GetRoot(), this->mainFrame, kFDOpen, &fi);

	// when obtained the filename - process the file
	if ((fi.fFilename != NULL) && (strlen(fi.fFilename)>0)){
		std::cout << "openFile: filename '" << fi.fFilename << "'" << std::endl;
		std::cout << "openFile: directory '" << fi.fIniDir << "'" << std::endl;
		textEdit->LoadFile(fi.fFilename);
		strFileName = new TString(fi.fFilename);
		TString* strOnlyFileName = StringUtils::stripFileName(strFileName);
		lblFileName->SetText(strOnlyFileName->Data());
		btnImportSpectrum->SetEnabled(true);
	}
}

void SWCalculatorFrame::saveData(void){
	TString* onlyFileName = StringUtils::stripFileExtension(strFileName);
	TString* dataFileName = &(onlyFileName->Append("-fit.csv"));
	FileUtils* fileUtils = FileUtils::getInstance();
	fileUtils->saveData(dataFileName, peakHist, curveBg, curveFit, chiHist, peakHistNoBg);
	TString* heading = new TString("SW Calculator");
	TString* message = &((new TString("File saved: "))->Append(*dataFileName));
	ShowOkDialog(heading, message);
}

void SWCalculatorFrame::saveImage(void){
	TString* onlyFileName = StringUtils::stripFileExtension(strFileName);
	TString* imageFileName = &(onlyFileName->Append("-2D-img.png"));
	TImage *imgCanvas = TImage::Create();
	imgCanvas->FromPad(canvasPlot);
	imgCanvas->WriteImage(imageFileName->Data());
	delete imgCanvas;
	TString* vectorImageFileName = &(onlyFileName->Append("-2D-img.pdf"));
	canvasPlot->Print(vectorImageFileName->Data());
	TString* heading = new TString("SW Calculator");
	TString* message = &((new TString("File saved: "))->Append(*imageFileName));
	ShowOkDialog(heading, message);
}

void SWCalculatorFrame::importSpectrum(void){
	const char* fileName = strFileName->Data();
	FileUtils* fileUtils = FileUtils::getInstance();

	fullHist = fileUtils->importTH1(fileName,
		numEnergyColumn->GetIntNumber(),
		numCountsColumn->GetIntNumber());
	// Cut full histogram if it is single detector
	Double_t spectrumWidth = 60;

	// Cut spectrum (not necessary)
	if (fullHist->GetXaxis()->GetXmin() < 511 - spectrumWidth / 2 && fullHist->GetXaxis()->GetXmax()> 511 + spectrumWidth / 2){
		HistProcessor* histProcessor = HistProcessor::getInstance();
		fullHist = histProcessor->cutHist(fullHist, 511 - spectrumWidth / 2, 511 + spectrumWidth / 2);
	}

	if (fullHist){
		GraphicsHelper* graphicsHelper = GraphicsHelper::getInstance();
		canvasHist->cd();
		fullHist->Draw();
		graphicsHelper->setDefaultAxisFonts(fullHist->GetXaxis());
		graphicsHelper->setDefaultAxisFonts(fullHist->GetYaxis());
		fullHist->SetStats(0);
		canvasHist->Update();

		tabsWidget->SetEnabled(1, true);
		Int_t eMin = (Int_t)(fullHist->GetXaxis()->GetXmin());
		Int_t eMax = (Int_t)(fullHist->GetXaxis()->GetXmax() + 0.5);
		Int_t maxCountBin = fullHist->GetMaximumBin();
		Int_t peakCenter = (Int_t)(fullHist->GetBinCenter(maxCountBin) + 0.5);
		numPeakPosition->SetLimitValues(eMin + 1, eMax - 1);
		Int_t limitWidth = std::min(peakCenter - eMin, eMax - peakCenter);
		if (numPeakPosition->GetIntNumber() != peakCenter){
			numPeakPosition->SetIntNumber(peakCenter);
			numDisplayMin->SetIntNumber(peakCenter - std::min(limitWidth, 35));
			numDisplayMax->SetIntNumber(peakCenter + std::min(limitWidth, 35));
			numFitMin->SetIntNumber(peakCenter - std::min(limitWidth, 35));
		}
		numFitMin->SetLimitValues(peakCenter - limitWidth, peakCenter - 1);
		numFitMax->SetLimitValues(peakCenter + 1, peakCenter + limitWidth);
		numDisplayMin->SetLimitValues(peakCenter - limitWidth, peakCenter + limitWidth);
		numDisplayMax->SetLimitValues(peakCenter - limitWidth, peakCenter + limitWidth);

		HistProcessor* histProcessor = HistProcessor::getInstance();
		if (!histProcessor->isTwoDetetor(fullHist)){
			lblRescale1->SetText("");
			lblRescale2->SetText("");
			lblRescale3->SetText("");
		}
		else {
			lblRescale1->SetText("2 x ");
			lblRescale2->SetText("2 x ");
			lblRescale3->SetText("2 x ");
		}
	}
	else {
		std::cout << "importFile: doublcheck file formatting and settings." << std::endl;
		tabsWidget->SetEnabled(1, false);
	}
}

void SWCalculatorFrame::setToolbarEnabled(Bool_t isEnabled){
	btnApplyZoom->SetEnabled(isEnabled);
	btnResetZoom->SetEnabled(isEnabled);
	btnSaveImage->SetEnabled(isEnabled);
	btnSaveData->SetEnabled(isEnabled);
	numDisplayMin->SetState(isEnabled);
	numDisplayMax->SetState(isEnabled);
}

void SWCalculatorFrame::fitSpectrum(void){
	Double_t eFitMin = numFitMin->GetNumber();
	Double_t eFitMax = numFitMax->GetNumber();

	// Set Display Range same as Fit Range
	numDisplayMin->SetNumber(eFitMin);
	numDisplayMax->SetNumber(eFitMax);

	// Enable bottom Toolbar
	setToolbarEnabled(kTRUE);

	HistProcessor* histProcessor = HistProcessor::getInstance();
	peakHist = histProcessor->cutHist(fullHist, eFitMin, eFitMax);

        Int_t totalFitCounts = histProcessor->getTotalCounts(peakHist);
        
	RooRealVar* x = new RooRealVar("x", "Energy", peakHist->GetXaxis()->GetXmin(), peakHist->GetXaxis()->GetXmax(), "keV");
	Double_t dE_0 = 2;
	RooRealVar* E_0 = new RooRealVar("E_0", "Peak Center", numPeakPosition->GetNumber(), numPeakPosition->GetNumber() - dE_0, numPeakPosition->GetNumber() + dE_0, "keV");

	// Convert Histogram to RooDataHist for Fitting
	RooDataHist* data = new RooDataHist("data", "Dataset with x", RooArgSet(*x), peakHist);

	Bool_t hasAtan = !(histProcessor->isTwoDetetor(peakHist));
	std::cout << "hasAtan: " << hasAtan << std::endl;
	Double_t bgFraction = histProcessor->calcBackgroundFraction(peakHist);

	// Fit precise peak center - either 511 for single spectra or 0 for two-detector setup
	//        x->setRange("peak_center",numPeakPosition->GetNumber()-dE_0,numPeakPosition->GetNumber()+dE_0);
	//        RooRealVar* tempSigma = new RooRealVar("tempSigma", "Peak Gauss sigma", 1, 0, 4, "KeV");
	//        RooGaussian* tempGauss = new RooGaussian("tempGauss", "Gauss to fit the center position", *x, *E_0, *tempSigma);
	//        tempGauss->fitTo(*data, Range("peak_center"));
	//        std::cout << "Peak position is " << E_0->getValV() << std::endl;
	//        E_0->setConstant(kTRUE);
	//        E_0->setMin(E_0->getValV());
	//        E_0->setMax(E_0->getValV());

	// Obtain Fit Model
	//AbstractModelProvider* modelProvider = new GaussModelProvider(x, E_0, hasAtan, bgFraction);
        Bool_t isTwoDetector = histProcessor->isTwoDetetor(fullHist);
	CompositeModelProvider* modelProvider;
        modelProvider = new CompositeModelProvider(x, E_0, hasParabola->IsOn(), (Int_t)numGauss->GetNumber(), (Int_t)numLorentz->GetNumber(), (Int_t)numLorentzComplex->GetNumber(), kFALSE /*hasOrtho->IsOn()*/, hasAtan, bgFraction, isTwoDetector);
            
	RooAbsPdf* model = modelProvider->getModel();
	RooAbsPdf* convolutedModel = modelProvider->getConvolutedModel();

	// Fitting
	// Default chi2FitTo used when we use non-standart errors.
	// RooFitResult* fitResult = convolutedModel->chi2FitTo(*data, Save(), Extended());

	// The RooFit chi2 fit does not work when the bins have zero entries. (Doppler-broadened spectra)
	// You should either use a binned likelihood fit or use the standard chi2 fit provided by ROOT. In this case bins with zero entries are excluded from the fit

	// Chi2 fit
//	RooChi2Var* chi2 = new RooChi2Var("chi2", "chi2", *convolutedModel, *data);
//	RooMinuit* m = new RooMinuit(*chi2);
//	m->migrad();
//	m->hesse();
//	// m->optimizeConst(1);
//	RooFitResult* fitResult = m->save();

//        data = static_cast<RooAddPdf*>(model)->generateBinned(*x,10000) ;
        
        // Simple Fit
        RooFitResult* fitResult = convolutedModel->fitTo(*data, Save(kTRUE));
        
	GraphicsHelper* graphicsHelper = GraphicsHelper::getInstance();
        Double_t convolutedModelMaxX = histProcessor->getPdfMaximumX(convolutedModel, RooArgList(*x));
        std::cout << "convolutedModelMaxX: " << convolutedModelMaxX << std::endl;
        
	// Plotting top RooPlot
	fitFrame = x->frame(); 	// Set Empty Graph Title
	fitFrame->GetXaxis()->SetRangeUser(numFitMin->GetNumber(), numFitMax->GetNumber());
	graphicsHelper->setupPlotAxis(fitFrame, "E1-E2, keV", 2.5, 0.02, "Counts", 2.1, 0.01);

	// Calculate Y Axis Maximum and Minimum
	Double_t yAxisMax = 0.1;
	while (yAxisMax < peakHist->GetBinContent(peakHist->GetMaximumBin())){
		yAxisMax *= 10;
	}
	Double_t yAxisMin = 0.1;
	while (yAxisMin * 10 < peakHist->GetBinContent(peakHist->GetMinimumBin()) + 1){
		yAxisMin *= 10;
	}

	// Draw S and W ranges
	std::cout << "isTwoDetector: " << isTwoDetector << std::endl;

	Double_t ds = isTwoDetector ? numSWidth->GetNumber() * 2 : numSWidth->GetNumber();
	Double_t dw = isTwoDetector ? numWWidth->GetNumber() * 2 : numWWidth->GetNumber();
	Double_t wOffset = isTwoDetector ? numWShift->GetNumber()*2 : numWShift->GetNumber();
	Double_t e0 = E_0->getVal();

        std::cout << "E_0: " << e0 << std::endl;

	std::cout << "S integration region (" << convolutedModelMaxX - ds / 2. << "; " << convolutedModelMaxX + ds / 2. << ")" << std::endl;
	// TColor *color = new TColor(1756, 0.97, 0.97, 0.97);
	TBox* sBox = new TBox(convolutedModelMaxX - ds / 2., yAxisMin, convolutedModelMaxX + ds / 2., yAxisMax*0.96);
	sBox->SetLineWidth(0);
	sBox->SetFillColorAlpha(19, 0.6);
	// sBox->SetFillStyle(3002);
	fitFrame->addObject(sBox);

	std::cout << "W1 integration region (" << convolutedModelMaxX - wOffset - dw << "; " << convolutedModelMaxX - wOffset << ")" << std::endl;
	TBox* w1Box = new TBox(convolutedModelMaxX - wOffset - dw, yAxisMin, convolutedModelMaxX - wOffset, yAxisMax*0.96);
	w1Box->SetLineWidth(0);
	w1Box->SetFillColorAlpha(19, 0.6);
	// w1Box->SetFillStyle(3002);
	fitFrame->addObject(w1Box);

	std::cout << "W2 integration region (" << convolutedModelMaxX + wOffset << "; " << convolutedModelMaxX + wOffset + dw << ")" << std::endl;
	TBox* w2Box = new TBox(convolutedModelMaxX + wOffset, yAxisMin, convolutedModelMaxX + wOffset + dw, yAxisMax*0.96);
	w2Box->SetLineWidth(0);
	w1Box->SetFillColorAlpha(19, 0.6);
	// w2Box->SetFillStyle(3002);
	fitFrame->addObject(w2Box);

	// Plot Data First (in white color - invisible). Otherwise roofit cant normalize model correctly
	data->plotOn(fitFrame, Invisible()); //MarkerSize(0.5),

	// Plot Convoluted Model
        // https://root-forum.cern.ch/t/roofit-normailzations/7040/2
        // Normalization(totalFitCounts, RooAbsReal::NumEvent)
	convolutedModel->plotOn(fitFrame, LineColor(kOrange + 6), LineWidth(2), Name("fit"));

	// Plot Convoluted Model background (its added after convolution graph because otherwise it changes backgroud)
	convolutedModel->plotOn(fitFrame, Components(*(modelProvider->getBgComponents())), LineStyle(kDashed), LineColor(kViolet + 6), LineWidth(1), Name("bg"));

	// Plot Non-convoluted model
	model->plotOn(fitFrame, LineColor(kOrange + 6), LineWidth(1), LineStyle(kDashed));

	// Plot Non-convoluted model components (parabola and gauss)
	RooArgSet* components = modelProvider->getComponents();
	TIterator* it = components->createIterator();
	TObject* tempObj = 0;
	while ((tempObj = it->Next())){
		RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(tempObj);
		if (pdf){
			RooArgSet* argSet = new RooArgSet();
                        std::cout << pdf->GetName() << std::endl;
			argSet->add(*pdf);
			// pdf->plotOn(fitFrame, LineStyle(kDashed), LineColor(kGray), LineWidth(1));
			model->plotOn(fitFrame, Components(*argSet), LineStyle(kDashed), LineColor(kViolet + 6), LineWidth(1), Name("component"));
		}
	}

	// Plot Resolution Function
	RooAbsPdf* resolutionFunction = modelProvider->getResolutionFuncton();
	if (resolutionFunction != NULL){
            resolutionFunction->plotOn(fitFrame, LineStyle(kDashed), LineColor(kGray), LineWidth(1),  Normalization(totalFitCounts, RooAbsReal::NumEvent));
	}

	curveBg = fitFrame->getCurve("bg");
	curveFit = fitFrame->getCurve("fit");

	RooCurve* curveFitNoBg = NULL;
	curveFitNoBg = histProcessor->subtractCurves(curveFit, curveBg);
	curveFitNoBg->SetLineColor(kGray);
	curveFitNoBg->SetLineWidth(2);
	fitFrame->addPlotable(curveFitNoBg, "L");
	fitFrame->drawBefore("fit", "FitNoBg");
	peakHistNoBg = histProcessor->subtractCurve(peakHist, curveBg);

	//RooDataHist* dataNoBg = new RooDataHist("dataNoBg", "Dataset with x", RooArgSet(*x), peakHistNoBg);
	data->plotOn(fitFrame, LineColor(kGray + 3), XErrorSize(0), MarkerSize(0.5), MarkerColor(kGray + 3), DataError(RooAbsData::SumW2)); // LineStyle(kSolid), LineWidth(2)

	// Print Plot names
	fitFrame->Print("v");

	// Plot Bottom Frame with Fit Goodness
	chiHist = histProcessor->getChi2Hist(peakHist, curveFit);
	RooDataHist* chi2DataHist = new RooDataHist("chi2DataHist", "Chi2", RooArgSet(*x), chiHist);

	// Set Y Axis range after all (otherwise throws error?)
	fitFrame->GetYaxis()->SetRangeUser(yAxisMin, yAxisMax);

	chiFrame = x->frame();
	chiFrame->GetXaxis()->SetRangeUser(numFitMin->GetNumber(), numFitMax->GetNumber());
	graphicsHelper->setupPlotAxis(chiFrame, "", 2.5, 0.05, "#chi^{2}", 2.1, 0.01); // TaTex Title

	chi2DataHist->plotOn(chiFrame, LineColor(kGray + 3), XErrorSize(0), DataError(RooAbsData::None), MarkerSize(0.5), MarkerColor(kGray + 3));

	// Initialize and draw Canvas
	drawMainCanvas();

	// Calculate S and W parameters
	std::pair<Double_t, Double_t> fullInt;

	fullInt.first = peakHistNoBg->IntegralAndError(1, peakHistNoBg->GetXaxis()->GetNbins(), fullInt.second, "width");//calculateHistIntegral(peakHistNoBg, histEMin, histEMax - 0.001);
	//std::cout << "Full Integral: " << fullInt.first << " ± " << fullInt.second << std::endl;

	std::pair<Double_t, Double_t> sInt = histProcessor->calcIntegral(peakHistNoBg, convolutedModelMaxX - ds / 2., convolutedModelMaxX + ds / 2.);
	std::pair<Double_t, Double_t> w1Int = histProcessor->calcIntegral(peakHistNoBg, convolutedModelMaxX - wOffset - dw, convolutedModelMaxX - wOffset);
	std::pair<Double_t, Double_t> w2Int = histProcessor->calcIntegral(peakHistNoBg, convolutedModelMaxX + wOffset, convolutedModelMaxX + wOffset + dw);
	std::cout << "S Integral: " << sInt.first << " ± " << sInt.second << std::endl;
	std::cout << "W1 Integral: " << w1Int.first << " ± " << w1Int.second << std::endl;
	std::cout << "W2 Integral: " << w2Int.first << " ± " << w2Int.second << std::endl;

	Double_t sValue = sInt.first / fullInt.first;
	Double_t dsValue = sqrt(pow((1 / fullInt.first)*(sInt.second), 2) + pow((sInt.first / fullInt.first / fullInt.first*fullInt.second), 2));

	Double_t wValue = (w1Int.first + w2Int.first) / fullInt.first;
	Double_t dwValue = sqrt(pow((1 / fullInt.first)*(w1Int.second), 2) + pow((1 / fullInt.first)*(w2Int.second), 2) + pow(((w1Int.first + w2Int.first) / fullInt.first / fullInt.first*fullInt.second), 2));

	// Output Filename
	txtFitResult->AddLineFast("");
	TString* onlyFileName = StringUtils::stripFileName(strFileName);
	txtFitResult->AddLineFast(Form("    %s", onlyFileName->Data()));

	// Output Fit Parameters
	std::ostringstream os;
	fitResult->printStream(os, fitResult->defaultPrintContents(""), fitResult->defaultPrintStyle(""));//Print();//printArgs(os);
	std::istringstream iss(os.str());
	std::string str;
	for (int i = 0; i<4; i++){
		std::getline(iss, str);
	}
	while (std::getline(iss, str)){
		txtFitResult->AddLineFast(str.c_str());
	}

	// Output Fermi Energy if we use parabola model
        std::list<Variable*> indirectParameters = modelProvider->getIndirectParameters();
        std::list<Variable*>::iterator iter;
        txtFitResult->AddLineFast("  ------------------------------------------------");
        for (iter = indirectParameters.begin(); iter != indirectParameters.end(); ++iter) {
            Variable* v = (*iter);
            TString str = (v->getError() == 0) ? 
                Form("%*s    %1.4e %s", 22, v->getDescription(), v->getValue(), v->getUnit()) : 
                Form("%*s    %1.4e (%1.2e) %s", 22, v->getDescription(), v->getValue(), v->getError(), v->getUnit());
            txtFitResult->AddLineFast(str);
        }

        // Output intensities
        std::list<std::pair<const char*, Double_t>> intensities = modelProvider->getIntensities();
        std::list<std::pair<const char*, Double_t>>::iterator iiter;
        txtFitResult->AddLineFast("  ------------------------------------------------");
        for (iiter = intensities.begin(); iiter != intensities.end(); ++iiter) {
            std::pair<const char*, Double_t> p = (*iiter);
            TString str = Form("%*s    %f %c", 22, p.first, p.second*100, '%');
            txtFitResult->AddLineFast(str);
        }        
        
        //	ParabolaGaussModelProvider* parabolaGaussModel = dynamic_cast<ParabolaGaussModelProvider*>(modelProvider);
//	if (parabolaGaussModel){
//            printParabolaInfo(parabolaGaussModel->getParabolaRoot(), isTwoDetector);
//	}
//
//	ParabolaLorentzianModelProvider* parabolaLorentzModel = dynamic_cast<ParabolaLorentzianModelProvider*>(modelProvider);
//	if (parabolaLorentzModel){
//		printParabolaInfo(parabolaLorentzModel->getParabolaRoot(), isTwoDetector);
//		printLorentzInfo(parabolaLorentzModel->getLorentzianCoefficients(), (Int_t)numGauss->GetNumber(), isTwoDetector);
//	}
        
	// Output Integral Chi^2
	txtFitResult->AddLineFast("  ------------------------------------------------");
	RooArgSet* params = model->getVariables();
	params->Print("v");
	Int_t numberOfFreeParameters = params->getSize();
	std::pair<Double_t, Int_t> sumChi2AndFreePars = histProcessor->getChi2(peakHist, curveFit, numberOfFreeParameters);
	Double_t sumChi2 = sumChi2AndFreePars.first;
	Int_t freeParameters = sumChi2AndFreePars.second;
	Double_t chi2ByFreePars = sumChi2 / (Double_t)(freeParameters);
	Double_t chi2Err = sqrt((double)2 * freeParameters) / freeParameters;
	TString strChiInt = Form("%*s   %2.1f/%d = %1.2f +/- %1.2f", 22, "chi^2/N", sumChi2, freeParameters, chi2ByFreePars, chi2Err); //13
	txtFitResult->AddLineFast(strChiInt);

	// Output S and W parameters
	txtFitResult->AddLineFast("  ------------------------------------------------");
	TString strS = Form("%*s    %1.4e +/-  %1.2e", 22, "S Parameter", sValue, dsValue);
	txtFitResult->AddLineFast(strS);
	TString strW = Form("%*s    %1.4e +/-  %1.2e", 22, "W Parameter", wValue, dwValue);
	txtFitResult->AddLineFast(strW);
	txtFitResult->AddLineFast("  ------------------------------------------------");

	// Update output
	txtFitResult->Update();
	txtFitResult->ScrollUp(1000);

	btnSaveData->SetEnabled(true);
	btnSaveImage->SetEnabled(true);
}

void SWCalculatorFrame::printParabolaInfo(RooRealVar* parabolaRoot, Bool_t isTwoDetector){
    if (parabolaRoot != NULL){
            // _PAS.pdf, (63)
            std::cout << "parRoot" << std::endl;
            Double_t Delta = parabolaRoot->getVal();
            Double_t DDelta = parabolaRoot->getError();
            Double_t mc2 = 0.5109989461E6;
            Double_t Ef = isTwoDetector ? Delta*Delta*1E6 / (2*mc2) : 2 * Delta*Delta*1E6 / mc2;
            Double_t dEf = isTwoDetector ? 2*Delta / (2*mc2) * DDelta : 2 * Delta*1E6 / mc2 * DDelta;
            txtFitResult->AddLineFast("  ------------------------------------------------");
            TString fermiString = Form("%*s = %1.4e +/- %1.2e", 22, "Fermi Energy", Ef, dEf);
            txtFitResult->AddLineFast(fermiString);
    }
}

void SWCalculatorFrame::printLorentzInfo(RooRealVar** eps, Int_t coeffNumber, Bool_t isTwoDetector) {
    txtFitResult->AddLineFast("  ------------------------------------------------");
    for (int i=0; i< coeffNumber; i++){
        Double_t epsilon = eps[i]->getVal() * 1e3; // [eV]
        Double_t DEpsilon = eps[i]->getError() * 1e3; // [eV]       
//        Double_t Ry = 13.6; // [eV]
        Double_t mc2 = 511*1e3; // [keV]
        // TODO: check for single detector
        Double_t E_b = isTwoDetector ? pow(epsilon,2) / (2*mc2) : pow(epsilon,2) / (2*mc2);
        Double_t DE_b = isTwoDetector ? 2 * epsilon / (2*mc2) * DEpsilon : 2 * epsilon / (2*mc2) * DEpsilon;

        TString coeffString = Form("%*s %d = %1.4e +/- %1.2e", 20, "Energy", i, E_b, DE_b);
        txtFitResult->AddLineFast(coeffString);                    
    }
}

SWCalculatorFrame::~SWCalculatorFrame() {
	mainFrame->Cleanup();
	delete mainFrame;
	delete tabsWidget;
	delete textEdit;
	delete numCountsColumn;
	delete numEnergyColumn;
	delete btnImportSpectrum;
	delete canvasHist;
}