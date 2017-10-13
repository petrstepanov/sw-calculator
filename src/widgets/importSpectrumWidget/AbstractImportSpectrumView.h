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
#include <TRootEmbeddedCanvas.h>
// Omit circular dependency
//class AbstractImportSpectrumPresenter;

template <class P>
class AbstractImportSpectrumView : public AbstractView<P> {
  public:
    AbstractImportSpectrumView(TGWindow *w) : AbstractView<P>(w){    
        initUI();
    }

    ~AbstractImportSpectrumView(){
        // default destructor doesn't follow pointers
        delete btnOpenFile;
        delete lblFileName;
        delete txtFileBrowser;
        delete numEnergyColumn;
        delete numCountsColumn;
        delete btnImportSpectrum;
    }
    
    // Override base class virtual functions
    void initUI(){
        // Open File Frame
        TGHorizontalFrame* frameOpenFile = new TGHorizontalFrame(this->GetParent());
        btnOpenFile = new TGTextButton(frameOpenFile, "Open File");
        btnOpenFile->Connect("Clicked()", "AbstractImportSpectrumView", this, "onOpenFileClicked()");
        frameOpenFile->AddFrame(btnOpenFile, new TGLayoutHints(kLHintsLeft | kLHintsTop));
        lblFileName = new TGLabel(frameOpenFile, "");
        frameOpenFile->AddFrame(lblFileName, new TGLayoutHints(kLHintsLeft | kLHintsTop, dx, 0, dx)); // left right top bottom
        this->AddFrame(frameOpenFile, new TGLayoutHints(kLHintsExpandX, dx, dx, dy/2, dy));

        // File browser
        txtFileBrowser = new TGTextEdit(this->GetParent());
        this->AddFrame(txtFileBrowser, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy/2, dy));

        // Energy column
        TGHorizontalFrame* frameEnergyColumn = new TGHorizontalFrame(this->GetParent());
        TGLabel* lblEnergyColumn = new TGLabel(frameEnergyColumn, "Energy Column #");
        numEnergyColumn = new TGNumberEntry(frameEnergyColumn, 1, 2, -1, TGNumberFormat::kNESInteger,
                TGNumberFormat::kNEANonNegative,
                TGNumberFormat::kNELLimitMinMax,
                1, 99);
        frameEnergyColumn->AddFrame(lblEnergyColumn, new TGLayoutHints(kLHintsNormal, 0, 0, dx, 0));
        frameEnergyColumn->AddFrame(numEnergyColumn, new TGLayoutHints(kLHintsRight));
        this->AddFrame(frameEnergyColumn, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

        // Counts column
        TGHorizontalFrame* frameCountsColumn = new TGHorizontalFrame(this->GetParent());
        TGLabel* lblCountsColumn = new TGLabel(frameCountsColumn, "Counts Column #");
        numCountsColumn = new TGNumberEntry(frameCountsColumn, 2, 2, -1, TGNumberFormat::kNESInteger,
                TGNumberFormat::kNEANonNegative,
                TGNumberFormat::kNELLimitMinMax,
                1, 99);
        frameCountsColumn->AddFrame(lblCountsColumn, new TGLayoutHints(kLHintsNormal, 0, 0, dx, 0));
        frameCountsColumn->AddFrame(numCountsColumn, new TGLayoutHints(kLHintsRight));
        this->AddFrame(frameCountsColumn, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

        // Import Spectrum button
        TGHorizontalFrame* frameImportSpectrum = new TGHorizontalFrame(this->GetParent());
        btnImportSpectrum = new TGTextButton(frameImportSpectrum, "Import Spectrum");
        btnImportSpectrum->Connect("Clicked()", "AbstractImportSpectrumView", this, "onImportSpectrumClicked()");
        btnImportSpectrum->SetEnabled(false);
        frameImportSpectrum->AddFrame(btnImportSpectrum, new TGLayoutHints(kLHintsExpandX));
        this->AddFrame(frameImportSpectrum, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

        // Histogram canvas
        TRootEmbeddedCanvas* embedHist = new TRootEmbeddedCanvas("embedHist", this->GetParent());
        canvasHist = embedHist->GetCanvas(); //new TCanvas("canvasHist", 10, 10, idHist);
        canvasHist->SetLogy();
        this->AddFrame(embedHist, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy, dx));    
    }    

    P* instantinatePresenter() = 0;

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