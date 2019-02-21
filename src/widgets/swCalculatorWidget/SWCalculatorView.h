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

#ifndef SWCALCULATORVIEW_H
#define SWCALCULATORVIEW_H

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
#include <RooFitResult.h>
#include <RooPlot.h>
#include <TGDoubleSlider.h>
#include <TGTextEntry.h>
#include <TGTextBuffer.h>
#include "../AbstractView.h"
#include "SWCalculatorPresenter.h"
#include "../rooRealVarWidget/RooRealVarView.h"

class SWCalculatorPresenter;

class SWCalculatorView : public AbstractView<SWCalculatorPresenter> {
  protected:
    void initUI();

//      #if defined(__ROOTCLING__)
//	  ClassDef(SWCalculatorFrame, 1);
//      #endif

  private:
    TGTextButton* btn;
    TGTab* tabsWidget;
    TGNumberEntry* numPeakPosition;
    TGCompositeFrame *tabFit;

    // Fit and W,S ranges
    TGNumberEntry* numFitMin;
    TGNumberEntry* numFitMax;
    TGLabel* lblRescale1;
    TGLabel* lblRescale2;
    TGLabel* lblRescale3;
    TGNumberEntry* numSWidth;
    TGNumberEntry* numWWidth;
    TGNumberEntry* numWShift;

    // Convolution parameters
    TGComboBox* comboConvolutionType;
    TGNumberEntry* numResolutionFWHM;
    TGCheckButton* checkboxResFixed;

    // Resolution Function view
    RooRealVarView* sourceContributionView;
    TGVerticalFrame* sourceContributionFrame;

    // Fitting function
    TGCheckButton* checkboxHasParabola;
    TGNumberEntry* numGauss;
    TGNumberEntry* numExponent;
    TGNumberEntry* numDampExponent;

    // Fit
    TGTextButton* btnFitSpectrum;
    TGTextEdit* txtFitResult;
    TGTextButton* btnClearResult;
    TGTextButton* btnSaveResult;

    // Plot and display params
//    TGTextButton* btnSaveData;
    TGTextButton* btnSaveImage;
    TCanvas* canvasPlot;
    TGDoubleHSlider* zoomSlider;
    TGNumberEntry* displayMin;
    TGNumberEntry* displayMax;
//    TGTextBuffer* tbMin;
//    TGTextBuffer* tbMax;

//    TGNumberEntry* numDisplayMin;
//    TGNumberEntry* numDisplayMax;
//    TGCheckButton* hasOrtho;
//    TGTextButton*  btnApplyZoom;
//    TGTextButton*  btnResetZoom;
    RooPlot* fitFrame;
    RooPlot* chiFrame;
    TPad* padData;
    TPad* padChi2;

  public:
    SWCalculatorView(const TGWindow *w = 0);
    virtual ~SWCalculatorView();

    // Override base class virtual functions
    SWCalculatorPresenter* instantinatePresenter();

    // Calls from Presenter
    RooRealVarView* getSourceContributionView();
    void setTabEnabled(Int_t, Bool_t);
    Int_t getFitMinValue();
    void setFitMinMaxRange(Int_t min, Int_t max);
    void setFitMinMaxValues(Bool_t isTwoDetector);
    Int_t getFitMaxValue();
    Double_t getSWidth();
    Double_t getWWidth();
    Double_t getWShift();
    Int_t getConvolutionType();
    Bool_t hasParabola();
    Int_t getNumGauss();
    Int_t getNumExp();
    Int_t getNumDampExp();
    Double_t getResolutionFWHM();
    Bool_t isResolutionFixed();
    TPad* getPadData();
    TPad* getPadChi2();
    void setToolbarEnabled(Bool_t isEnabled);
    void updateRegionLabels(Bool_t isTwoDetector);
    void displayFilename(TString* fileName);
    void displayFitParameters(RooFitResult* fitResult);
    void displayIndirectParameters(RooArgList* parameters);
    void displayIntensities(RooArgList* intensities);
    void displayChi2(Double_t sumChi2, Int_t freeParameters, Int_t degreesFreedom);
    void displaySW(std::pair<Double_t, Double_t> sValueError, std::pair<Double_t, Double_t> wValueError);
    void updateCanvas();
    void setDisplayLimits(Float_t min, Float_t max);
    TCanvas* getCanvas();
    void saveFitResults(TString* fileName);
    void clearFitResults();

    // Calls to Presenter
    void onNumFitMinChanged();
    void onNumFitMaxChanged();
    void onFitSpectrumClicked();
    void onApplyZoomClicked();
    void onResetZoomClicked();
//    void onSaveDataClicked();
    void onSaveImageClicked();
    void onSaveResultsClicked();
    void onClearResultsClicked();

    //    void CloseWindow();
    void initRooPlots(RooPlot* fitFrame, RooPlot* chiFrame);

    // Update display range functions
    void onDisplayMinChange(char* c);
    void onDisplayMaxChange(char* c);
    void onSliderChange();
    void updateCanvasLimits(Double_t min, Double_t max);
    void setSourceContributionFrameVisible(Bool_t isVisible);

    // ClassDef(SWCalculatorView,0);
};

#endif /* SWCALCULATORVIEW_H */
