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
#include "../../roofit/PdfProvider.h"
#include <RooPlot.h>

class SWCalculatorView;

class SWCalculatorPresenter : public AbstractPresenter<Model, SWCalculatorView> {

private:
    PdfProvider* pdfProvider;
    void buildFittingModel();

    // Saving a copy of the old fit properties to see if the fitting PDF model should be rebuild or not
    FitProperties oldFitProperties;
    Bool_t needRebuildPDF();

    // RooPlot* spectrumPlot, residualsPlot;

public:
    SWCalculatorPresenter(SWCalculatorView* view);

    // Override base class virtual methods
    Model* instantinateModel();

    void onInitModel();
    
    // Slots for View Signals
    void onViewFitSliderRangeSet();
    void onViewFitRangeSet();
    void onViewConvolutionSelected(Int_t convolutionType);
    void onViewHasParabolaSet(Bool_t b);
    void onViewNumGaussSet();
    void onViewNumExponentSet();
    void onViewNumDampExponentSet();
	void onViewAddHistComponentClicked();
	void onViewRemoveHistComponentClicked();

    void onViewSaveData();
    void onViewSaveImageClicked();
    void onViewEditParametersClicked();
    void onViewSaveDataClicked();
    void onViewFitSpectrumClicked();
    void onViewSaveResultsClicked();
    void onViewClearResultsClicked();

    // Slots for Model Signals
    void onModelHistogramImported(TH1F* hist);
    void onModelSourceHistogramImported(TH1F* hist);
    void onModelComponentHistogramImported(TH1F* hist);
	void onModelTwoDetectorSet(Bool_t isTwoDetector);
	void onModelFitRangeLimitsSet(DoublePair* fitRangeLimits);
	void onModelFitRangeSet(DoublePair* fitFange);

	void onModelConvolutionTypeSet(Int_t);
	void onModelHasParabolaSet(Bool_t b);
	void onModelNumberOfGaussiansSet(Int_t);
	void onModelNumberOfExponentsSet(Int_t);
	void onModelNumberOfDampingExponentsSet(Int_t);
	void onModelFitPropertiesChanged();

	ClassDef(SWCalculatorPresenter,1)
};

#endif /* SWCALCULATORPRESENTER_H */

