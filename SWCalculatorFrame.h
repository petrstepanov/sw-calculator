 /*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SWCalculatorFrame.h
 * Author: petrstepanov
 *
 * Created on August 1, 2017, 5:02 PM
 */

#ifndef SWCALCULATORFRAME_H
#define SWCALCULATORFRAME_H

#include <TGFrame.h>
#include <TString.h>
#include <TGLabel.h>
#include <TGTab.h>
#include <TGTextEdit.h>
#include <TGNumberEntry.h>
#include <TGComboBox.h>
#include <TGButton.h>
#include <TCanvas.h>
#include <TImage.h>
#include <TH1I.h>
#include <TH1F.h>
#include <RooCurve.h>
#include <RooPlot.h>

class SWCalculatorFrame : public TGMainFrame {
private:
	TString* strFileName;
	TGLabel* lblFileName;
	TGLabel* lblRescale1;
	TGLabel* lblRescale2;
	TGLabel* lblRescale3;
	TGMainFrame* mainFrame;
	TGTab* tabsWidget;
	TGTextEdit* textEdit;
	TGNumberEntry* numCountsColumn;
	TGNumberEntry* numEnergyColumn;
	TGTextButton* btnImportSpectrum;
	TGTextButton* btnSaveData;
	TGTextButton* btnSaveImage;
	TCanvas* canvasHist;
	TCanvas* canvasPlot;
	TGNumberEntry* numPeakPosition;
	TGNumberEntry* numFitMin;
	TGNumberEntry* numFitMax;
	TGNumberEntry* numDisplayMin;
	TGNumberEntry* numDisplayMax;
	TGNumberEntry* numGauss;
	TGCheckButton* hasParabola;
	TGTextButton*  btnApplyZoom;
	TGTextButton*  btnResetZoom;
        TGComboBox* fitFunctionType;
	TGNumberEntry* numSWidth;
	TGNumberEntry* numWWidth;
	TGNumberEntry* numWShift;
	Bool_t isSingleDetector;
	TGTextEdit* txtFitResult;

	TH1I* fullHist;
	TH1I* peakHist;
	RooCurve* curveBg;
	RooCurve* curveFit;
	TH1F* peakHistNoBg;
	TH1F* chiHist;

	RooPlot* fitFrame;
	RooPlot* chiFrame;
	TPad* padData;
	TPad* padChi2;

	// Menu Commands
	enum ETestCommandIdentifiers {
		M_FILE_EXIT,
	};

public:
	SWCalculatorFrame(const TGWindow*, UInt_t, UInt_t);
	void ShowOkDialog(TString*, TString*);
	void openFile();
	void saveData();
	void saveImage();
	void applyZoom();
	void resetZoom();
	void importSpectrum();
	void fitSpectrum();
	void HandleMenu(Int_t id);
	void CloseWindow();
	void numPeakPositionChanged();
	void numFitMinChanged();
	void numFitMaxChanged();
	void setToolbarEnabled(Bool_t);
	// void prepareCanvas();
	void drawMainCanvas();
        void printParabolaInfo(RooRealVar*, Bool_t);
        void printLorentzInfo(RooRealVar**, Int_t, Bool_t);        
	virtual ~SWCalculatorFrame();
        
//      #if defined(__ROOTCLING__)
//	  ClassDef(SWCalculatorFrame, 1);
//      #endif
};

#endif /* SWCALCULATORFRAME_H */

