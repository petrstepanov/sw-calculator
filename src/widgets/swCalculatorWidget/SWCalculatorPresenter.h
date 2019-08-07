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
#include "../../roofit/CompositeModelProvider.h"

class SWCalculatorView;

class SWCalculatorPresenter : public AbstractPresenter<Model, SWCalculatorView> {
public:
    SWCalculatorPresenter(SWCalculatorView* view);

    // Override base class virtual methods
    Model* instantinateModel();

    void onInitModel();
    
    void onEditParametersClicked();
    void onFitSpectrumClicked();
    void onSaveImageClicked();
    void onSaveResultsClicked();
    void onClearResultsClicked();

    // Slots for View Signals
    void onViewFitSliderRangeSet();
    void onViewFitRangeSet();
    void onViewConvolutionSelected(Int_t convolutionType);
    void onViewHasParabolaSet(Bool_t b);
    void onViewNumGaussSet();
    void onViewNumExponentSet();
    void onViewNumDampExponentSet();

    // Slots for Model Signals
    void onModelHistogramImported(TH1F* hist);
    void onModelSourceHistogramImported(TH1F* hist);
	void onModelTwoDetectorSet(Bool_t isTwoDetector);
	void onModelFitRangeLimitsSet(DoublePair* fitRangeLimits);
	void onModelFitRangeSet(DoublePair* fitFange);

	void onModelConvolutionTypeSet(Int_t);
	void onModelHasParabolaSet(Bool_t b);
	void onModelNumberOfGaussiansSet(Int_t);
	void onModelNumberOfExponentsSet(Int_t);
	void onModelNumberOfDampingExponentsSet(Int_t);

private:
	CompositeModelProvider* pdfProvider;
	void buildFittingModel();
};

#endif /* SWCALCULATORPRESENTER_H */

