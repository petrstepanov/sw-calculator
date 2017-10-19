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
#include "../../util/Variable.h"
#include "../AbstractView.h"
#include "SWCalculatorPresenter.h"

class SWCalculatorPresenter;

class SWCalculatorView : public AbstractView<SWCalculatorPresenter> {
//class SWCalculatorView : public TGMainFrame {
  public:
    SWCalculatorView(const TGWindow *w = 0);
    virtual ~SWCalculatorView();
    
    // Override base class virtual functions
    SWCalculatorPresenter* instantinatePresenter();

    // Calls from Presenter
    void setTabEnabled(Int_t, Bool_t);
    Int_t getFitMinValue();
    void setFitMinMaxValues(Int_t, Int_t);
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
    void setTwoDetector(Bool_t isTwoDetector);
    void displayFilename(TString* fileName);
    void displayFitParameters(RooFitResult* fitResult);
    void displayIndirectParameters(std::list<Variable*> parameters);
    void displayIntensities(std::list<std::pair<const char*, Double_t>> intensities);
    void displayChi2(Double_t sumChi2, Int_t freeParameters, Int_t degreesFreedom);
    void displaySW(std::pair<Double_t, Double_t> sValueError, std::pair<Double_t, Double_t> wValueError);
    void updateCanvas();
    
    // Calls to Presenter
    void onNumFitMinChanged();
    void onNumFitMaxChanged();
    void onFitSpectrumClicked();
    void onApplyZoomClicked();
    void onResetZoomClicked();
    void onSaveDataClicked();
    void onSaveImageClicked();   
//    void CloseWindow();
    
  protected:
    void initUI();
    
//      #if defined(__ROOTCLING__)
//	  ClassDef(SWCalculatorFrame, 1);
//      #endif

  private:
    TGTextButton* btn;
    TGTab* tabsWidget;
    TGNumberEntry* numPeakPosition;

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

    // Fitting function
    TGCheckButton* checkboxHasParabola;
    TGNumberEntry* numGauss;
    TGNumberEntry* numExponent;
    TGNumberEntry* numDampExponent;
    
    // Fit
    TGTextButton* btnFitSpectrum;
    TGTextEdit* txtFitResult;  

    // Plot and display params
    TGTextButton* btnSaveData;
    TGTextButton* btnSaveImage;
    TCanvas* canvasPlot;
    TGNumberEntry* numDisplayMin;
    TGNumberEntry* numDisplayMax;
//    TGCheckButton* hasOrtho;
    TGTextButton*  btnApplyZoom;
    TGTextButton*  btnResetZoom;              
    RooPlot* fitFrame;
    RooPlot* chiFrame;
    TPad* padData;
    TPad* padChi2;
};

#endif /* SWCALCULATORVIEW_H */

