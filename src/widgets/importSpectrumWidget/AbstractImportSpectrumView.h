/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   IImportSpectrumView.h
 * Author: petrstepanov
 *
 * Created on October 6, 2017, 12:40 AM
 */

#ifndef ABSTRACTIMPORTSPECTRUMVIEW_H
#define ABSTRACTIMPORTSPECTRUMVIEW_H

#include "../AbstractView.h"
#include "AbstractImportSpectrumPresenter.h"
#include <TGWindow.h>
#include <TGButton.h>
#include <TGLabel.h>
#include <TGTextView.h>
#include <TRootEmbeddedCanvas.h>
#include <TGNumberEntry.h>
#include <TCanvas.h>
#include <TRootEmbeddedCanvas.h>
#include <iostream>
#include <TGDoubleSlider.h>
#include <TGFrame.h>
#include <TPaveStats.h>
//#include "AbstractImportSpectrumPresenter.h"

// Omit circular dependency
class AbstractImportSpectrumPresenter;

class AbstractImportSpectrumView : public AbstractView<AbstractImportSpectrumPresenter> {
  private:

    // Ui components
    TGTextButton* btnOpenFile;
    TGLabel* lblFileName;
    TGTextView* txtFileBrowser;
    TGTextButton* btnImportSpectrum;
    TRootEmbeddedCanvas* embedCanvas;
    // TGDoubleHSlider* rangeSlider;
    // TGNumberEntry* numEnergyColumn;
    // TGNumberEntry* numCountsColumn;


    // Mini histogram preview
    TH1F* miniHist;
    TPaveStats* miniStats;

  public:
    AbstractImportSpectrumView(const TGWindow *w);

    virtual ~AbstractImportSpectrumView();

    // Override base class virtual functions
    void initUI();

    AbstractImportSpectrumPresenter* instantinatePresenter() override;

    // Calls from Presenter
    void loadFile(TString* fileNamePath);
    // Int_t getEnergyColumnNumber();
    // Int_t getCountsColumnNumber();
    void drawHistogram(TH1F* hist);
    // void initRangeSlider(Int_t min, Int_t max);
    // void onRangeSliderChange();
    // void onSetRangeClicked();

    // Calls to Presenter
    void onOpenFileClicked();
//    void onImportSpectrumClicked();
    virtual Int_t getHistogramColor();

//    void OnProcessedConfigure(Event_t* event);
//    void OnProcessed(Event_t* event);

    void handleUiReady();

    // https://root-forum.cern.ch/t/classdef-variants/44736/2
    // ClassDefOverride: Use this one if your class definition use the c++ `override` keyword for virtual functions
    // If you use the regular ClassDef, the compiler might warn that the function declared in the ClassDef are missing the override keyword
    ClassDefOverride(AbstractImportSpectrumView, 0)
};

#endif /* ABSTRACTIMPORTSPECTRUMVIEW_H */
