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
#include <TGWindow.h>
#include <TGButton.h>
#include <TGLabel.h>
#include <TGTextEdit.h>
#include <TGNumberEntry.h>
#include <TCanvas.h>

// Omit circular dependency
//class AbstractImportSpectrumPresenter;

template <class P>
class AbstractImportSpectrumView : public AbstractView<P> {
  public:
    AbstractImportSpectrumView(TGCompositeFrame *p = 0);
    virtual ~AbstractImportSpectrumView();
    
    // Override base class virtual functions
    P* instantinatePresenter() = 0;
    void initUI();

    // Calls from Presenter
    void loadFile(const char* fileNamePath);
    TString* getFileName();
    Int_t getEnergyColumnNumber();
    Int_t getCountsColumnNumber();
    void drawHistogram(TH1F* hist);
    
  private:
    // Ui components
    TGTextButton* btnOpenFile;
    TGLabel* lblFileName;
    TGTextEdit* txtFileBrowser;
    TGNumberEntry* numEnergyColumn;    
    TGNumberEntry* numCountsColumn;
    TGTextButton* btnImportSpectrum;    
    TCanvas* canvasHist;

    // Calls to Presenter
    void onOpenFileClicked();
    void onImportSpectrumClicked();    
};

#endif /* ABSTRACTIMPORTSPECTRUMVIEW_H */