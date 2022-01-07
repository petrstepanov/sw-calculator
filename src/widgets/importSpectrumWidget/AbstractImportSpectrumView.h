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
#include <TGNumberEntry.h>
#include <TCanvas.h>
#include <TRootEmbeddedCanvas.h>
#include <iostream>
#include <TGDoubleSlider.h>
//#include "AbstractImportSpectrumPresenter.h"

// Omit circular dependency
class AbstractImportSpectrumPresenter;

class AbstractImportSpectrumView : public AbstractView<AbstractImportSpectrumPresenter> {
  private:

    // Ui components
    TGTextButton* btnOpenFile;
    TGLabel* lblFileName;
    TGTextView* txtFileBrowser;
//    TGNumberEntry* numEnergyColumn;
//    TGNumberEntry* numCountsColumn;
    TGTextButton* btnImportSpectrum;
    TCanvas* canvasHist;
    // TGDoubleHSlider* rangeSlider;

    // Hack temporary
    TGTextButton* btnSetRange;

  public:
    AbstractImportSpectrumView(const TGWindow *w);

    virtual ~AbstractImportSpectrumView();

    // Override base class virtual functions
    void initUI();

    AbstractImportSpectrumPresenter* instantinatePresenter() override;

    // Calls from Presenter
    void loadFile(TString* fileNamePath);
    TString* getFileName();
    // Int_t getEnergyColumnNumber();
    // Int_t getCountsColumnNumber();
    void drawHistogram(TH1F* hist);
    // void initRangeSlider(Int_t min, Int_t max);
    // void onRangeSliderChange();
    // void onSetRangeClicked();

    // Calls to Presenter
    void onOpenFileClicked();
    void onImportSpectrumClicked();
    virtual Int_t getHistogramColor();

ClassDefOverride(AbstractImportSpectrumView, 0)
};

#endif /* ABSTRACTIMPORTSPECTRUMVIEW_H */
