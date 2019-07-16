/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SWCalculatorPresenter.h
 * Author: petrstepanov
 *
 * Created on October 6, 2017, 6:31 PM
 */

#ifndef SWCALCULATORPRESENTER_H
#define SWCALCULATORPRESENTER_H

#include "../AbstractPresenter.h"
#include "SWCalculatorView.h"
#include "../../model/Model.h"

class SWCalculatorView;

class SWCalculatorPresenter : public AbstractPresenter<Model, SWCalculatorView> {
public:
    SWCalculatorPresenter(SWCalculatorView* view);

    // Override base class virtual methods
    Model* instantinateModel();
    
    void addEventListeners();
    // View functions
    
    void onFitSpectrumClicked();
    void onSaveImageClicked();
    void onSaveResultsClicked();
    void onClearResultsClicked();
    void onInitModel();

    // Slots for Model Signals
    void onSourceHistogramImported(TH1F* hist);
	void onTwoDetectorSet(Bool_t isTwoDetector);
	void onSafeFitRangeSet(Double_t eMin, Double_t eMax);
};

#endif /* SWCALCULATORPRESENTER_H */

