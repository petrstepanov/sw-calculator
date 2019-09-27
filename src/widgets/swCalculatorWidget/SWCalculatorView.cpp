/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SWCalculatorFrame.cpp
 * Author: petrstepanov
 *
 * Created on August 1, 2017, 5:02 PM
 */

#include "SWCalculatorView.h"
#include "SWCalculatorPresenter.h"
// #include "../../LinkDef.h"
#include "../../model/Constants.h"
#include "../importSpectrumWidget/ImportSpectrumView.h"
#include "../importSpectrumWidget/ImportSourceSpectrumView.h"
#include "../../util/StringUtils.h"
#include "../../util/RootHelper.h"
#include <TGFrame.h>
#include <TMath.h>
#include <TGTextBuffer.h>
#include <TGText.h>
#include <TGDimension.h>
#include "../../util/UiHelper.h"
#include "../../util/GraphicsHelper.h"
#include "../../util/Debug.h"
#include <map>
#include <sstream>
#include <TRootEmbeddedCanvas.h>
#include <TG3DLine.h>
#include "../../roofit/PdfProvider.h"

using namespace RooFit;

ClassImp(SWCalculatorView);

//SWCalculatorView::SWCalculatorView(const TGWindow* w) : TGMainFrame(w, Constants::windowWidth, Constants::windowHeight){
SWCalculatorView::SWCalculatorView(const TGWindow* w) : AbstractView<SWCalculatorPresenter>(w){
    initUI();
	presenter = instantinatePresenter();
	connectSignals();
}

SWCalculatorPresenter* SWCalculatorView::instantinatePresenter(){
    return new SWCalculatorPresenter(this);

}

void SWCalculatorView::initUI(){
    SetLayoutManager(new TGHorizontalLayout(this));

    // Tabs widget
    tabsWidget = new TGTab(this, Constants::leftPanelWidth);

    // Import spectrum tab
    TGCompositeFrame *tabImport = tabsWidget->AddTab("Experimental spectrum");
    tabImport->SetLayoutManager(new TGVerticalLayout(tabImport));
    tabImport->AddFrame(new ImportSpectrumView(tabImport), new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy*2, dy*2));

    // Import Kapton spectrum tab
    TGCompositeFrame *tabImportKapton = tabsWidget->AddTab("Source contribution");
    tabImportKapton->SetLayoutManager(new TGVerticalLayout(tabImportKapton));
    tabImportKapton->AddFrame(new ImportSourceSpectrumView(tabImportKapton), new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy*2, dy*2));

    // Fit Data tab
    tabFit = tabsWidget->AddTab("Fit panel");
    tabFit->SetLayoutManager(new TGVerticalLayout(tabFit));

    // Fit Range Row
    tabFit->AddFrame(new TGLabel(tabFit, "Spectrum fitting range, keV"), new TGLayoutHints(kLHintsNormal, dx, dx, 2*dy, 0));
    TGHorizontalFrame *frameFitRange = new TGHorizontalFrame(tabFit);
    numFitMin = new TGNumberEntry(frameFitRange, -9999, 6, UiHelper::getUId(), TGNumberFormat::kNESRealTwo,
            TGNumberFormat::kNEAAnyNumber,
            TGNumberFormat::kNELLimitMinMax,
            -9999, 9999);

    numFitMax = new TGNumberEntry(frameFitRange, 9999, 6, UiHelper::getUId(), TGNumberFormat::kNESRealTwo,
            TGNumberFormat::kNEAAnyNumber,
            TGNumberFormat::kNELLimitMinMax,
            -9999, 9999);

    numFitSlider = new TGDoubleHSlider(frameFitRange, 100, kDoubleScaleBoth);

    frameFitRange->AddFrame(numFitMin, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
    frameFitRange->AddFrame(numFitSlider, new TGLayoutHints(kLHintsNormal | kLHintsCenterY | kLHintsExpandX, dx, dx));
    frameFitRange->AddFrame(numFitMax, new TGLayoutHints(kLHintsRight | kLHintsCenterY));
//    frameFitRange->AddFrame(new TGLabel(frameFitRange, "-"), new TGLayoutHints(kLHintsRight, dx, dx, 3*dy/5, 0));

    tabFit->AddFrame(frameFitRange, new TGLayoutHints(kLHintsExpandX, dx, dx, dy*2, dy));

    // Separator
    tabFit->AddFrame(new TGHorizontal3DLine(tabFit), new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Integration Parameters
    TGHorizontalFrame *frameSWidth = new TGHorizontalFrame(tabFit);
    lblRescale1 = new TGLabel(frameSWidth, "");
    lblRescale1->SetTextJustify(kTextRight);
    numSWidth = new TGNumberEntry(frameSWidth, 1.60, 4, UiHelper::getUId(), TGNumberFormat::kNESRealTwo,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMin,
            0.01, 9.99);
    frameSWidth->AddFrame(new TGLabel(frameSWidth, "S region width, keV"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
    frameSWidth->AddFrame(numSWidth, new TGLayoutHints(kLHintsRight | kLHintsCenterY));
    frameSWidth->AddFrame(lblRescale1, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsCenterY, 0, dx));

    tabFit->AddFrame(frameSWidth, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    TGHorizontalFrame *frameWWidth = new TGHorizontalFrame(tabFit);
    lblRescale2 = new TGLabel(frameWWidth, "");
    lblRescale2->SetTextJustify(kTextRight);
    numWWidth = new TGNumberEntry(frameWWidth, 3.00, 4, UiHelper::getUId(), TGNumberFormat::kNESRealTwo,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMin,
            0.01, 9.99);
    frameWWidth->AddFrame(new TGLabel(frameWWidth, "W regions width, keV"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
    frameWWidth->AddFrame(numWWidth, new TGLayoutHints(kLHintsRight | kLHintsCenterY));
    frameWWidth->AddFrame(lblRescale2, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsCenterY, 0, dx));

    tabFit->AddFrame(frameWWidth, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    TGHorizontalFrame *frameWShift = new TGHorizontalFrame(tabFit);
    lblRescale3 = new TGLabel(frameWShift, "");
    lblRescale3->SetTextJustify(kTextRight);
    numWShift = new TGNumberEntry(frameWShift, 2.76, 4, UiHelper::getUId(), TGNumberFormat::kNESRealTwo,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMin,
            0.01, 9.99);
    frameWShift->AddFrame(new TGLabel(frameWShift, "W regions shift, keV"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
    frameWShift->AddFrame(numWShift, new TGLayoutHints(kLHintsRight | kLHintsCenterY));
    frameWShift->AddFrame(lblRescale3, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsCenterY, 0, dx));

    tabFit->AddFrame(frameWShift, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Separator
    tabFit->AddFrame(new TGHorizontal3DLine(tabFit), new TGLayoutHints(kLHintsExpandX, dx, dx, dy, 0));

    // Convolution type and resolution function
    TGHorizontalFrame* convolutionParamsFrame = new TGHorizontalFrame(tabFit);

    // Initialize a map with std::initialzer_list
    convolutionParamsFrame->AddFrame(new TGLabel(convolutionParamsFrame, "Convolution:"), new TGLayoutHints(kLHintsLeft | kLHintsBottom, dx+1, 3*dx));


//    comboConvolutionType = new TGComboBox(convolutionParamsFrame, 0);
//    const std::map<Int_t, TString> convolutionTypesMap = {
//    		{ConvolutionType::kNoConvolution, "None"},
//    		{ConvolutionType::kFFTConvolution, "Numerical FFT"}
//    };
//    for (auto it = convolutionTypesMap.begin(); it != convolutionTypesMap.end(); ++it) {
//        comboConvolutionType->AddEntry((it->second).Data(), it->first);
//    }
//    comboConvolutionType->Select(ConvolutionType::kNoConvolution, kFALSE);
//    comboConvolutionType->Resize(120, 20);
//    convolutionParamsFrame->AddFrame(comboConvolutionType, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, dx, 0, 0, 2));

    // Create button Group with Matrix layout. ButtonGroup spacings are a mess.
    convTypeButtonGroup = new TGButtonGroup(convolutionParamsFrame, 1, 2, 0, kLHintsLeft | kLHintsTop); // rows, columns, spacing
    new TGRadioButton(convTypeButtonGroup, "None", ConvolutionType::kNoConvolution);
    new TGRadioButton(convTypeButtonGroup, "Numeric FFT", ConvolutionType::kFFTConvolution);
    convolutionParamsFrame->AddFrame(convTypeButtonGroup, new TGLayoutHints(kLHintsNormal));

    tabFit->AddFrame(convolutionParamsFrame, new TGLayoutHints(kLHintsNormal, dx, dx, 0, dy));


//    resolutionFwhmFrame = new TGHorizontalFrame(convolutionParamsFrame);
//    checkboxResFixed = new TGCheckButton(resolutionFwhmFrame, "fixed");
//    checkboxResFixed->SetOn();
//    numResolutionFWHM = new TGNumberEntry(resolutionFwhmFrame, 2.0, 4, -1, TGNumberFormat::kNESRealTwo,
//            TGNumberFormat::kNEANonNegative,
//            TGNumberFormat::kNELLimitMinMax,
//            0.5, 4.0);

//    resolutionFwhmFrame->AddFrame(checkboxResFixed, new TGLayoutHints(kLHintsRight | kLHintsTop, dx, 0, 3*dy/5, 0));
//    resolutionFwhmFrame->AddFrame(numResolutionFWHM, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, dy/5, 0));
//    resolutionFwhmFrame->AddFrame(new TGLabel(resolutionFwhmFrame, "Sum of detectors' FWHMs, keV"),
//                                     new TGLayoutHints(kLHintsRight | kLHintsTop, 0, dx, 4*dy/5, 0));
//    convolutionParamsFrame->AddFrame(resolutionFwhmFrame, new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 0));

//    tabFit->AddFrame(convolutionParamsFrame, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Separator
    // tabFit->AddFrame(new TGHorizontal3DLine(tabFit), new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Model parameters
    TGGroupFrame* modelComponentsGroupFrame = new TGGroupFrame(tabFit, "Model Components");
    TGHorizontalFrame* modelParamsFrame = new TGHorizontalFrame(modelComponentsGroupFrame);
    hasParabola = new TGCheckButton(modelParamsFrame, "Parabola");
    modelParamsFrame->AddFrame(hasParabola, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 3*dx));

    numGauss = new TGNumberEntry(modelParamsFrame, 0, 1, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            0, 9);
    modelParamsFrame->AddFrame(numGauss, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, dx));
    modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Gauss"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 3*dx));

    numExponent = new TGNumberEntry(modelParamsFrame, 0, 1, UiHelper::getUId(), TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            0, 9);
    modelParamsFrame->AddFrame(numExponent, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, dx));
    modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Exponent"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 3*dx));

    numDampExponent = new TGNumberEntry(modelParamsFrame, 0, 1, UiHelper::getUId(), TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            0, 9);
    modelParamsFrame->AddFrame(numDampExponent, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, dx));
    modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Damping Exp"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, dx));

    // Add custom histogram
    TGHorizontalFrame* modelHistogramFrame = new TGHorizontalFrame(modelComponentsGroupFrame);
    addHistogramButton = new TGTextButton(modelHistogramFrame, "Add from file...");
    modelHistogramFrame->AddFrame(addHistogramButton, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, dx));
    histComponentLabel = new TGLabel(modelHistogramFrame, Constants::LABEL_NO_FILE_LOADED);
    histComponentLabel->SetTextJustify(kTextLeft);
    histComponentLabel->Disable(kTRUE);
    histComponentLabel->ChangeOptions(histComponentLabel->GetOptions() | kFixedSize);
    modelHistogramFrame->AddFrame(histComponentLabel, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsCenterY, 0, dx));

    removeHistogramButton = new TGTextButton(modelHistogramFrame, "Remove");
    modelHistogramFrame->AddFrame(removeHistogramButton, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, dx));
    removeHistogramButton->SetEnabled(false);
    //	hasOrtho = new TGCheckButton(modelParamsFrame, "Ortho Exps", UiHelper::getUId());
    //	modelParamsFrme->AddFrame(hasOrtho, new TGLayoutHints(kLHintsNormal, 0, 20, 4, 0));
    modelComponentsGroupFrame->AddFrame(modelParamsFrame, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, 0, 0, dy, dy));
    modelComponentsGroupFrame->AddFrame(modelHistogramFrame, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, 0, 0, dy, dy));
    tabFit->AddFrame(modelComponentsGroupFrame, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, dx, dx, dy, 0));

    TGHorizontalFrame* frameFitSpectrum = new TGHorizontalFrame(tabFit);
    btnEditParameters = new TGTextButton(frameFitSpectrum, "Edit fitting parameters");
    frameFitSpectrum->AddFrame(btnEditParameters, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, 0, dx/2));
    btnFitSpectrum = new TGTextButton(frameFitSpectrum, "Fit and plot");
    frameFitSpectrum->AddFrame(btnFitSpectrum, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, dx/2));
    tabFit->AddFrame(frameFitSpectrum, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Fit Result TextBox
    txtFitResult = new TGTextEdit(tabFit);
    txtFitResult->SetBackgroundColor(GraphicsHelper::colorAppWindow->GetPixel());
    tabFit->AddFrame(txtFitResult, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy, dy));

    TGHorizontalFrame* splitFrame = new TGHorizontalFrame(tabFit);

    btnClearResult = new TGTextButton(splitFrame, "Clear output");
    splitFrame->AddFrame(btnClearResult, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, 0, dx/2));
    btnSaveResult = new TGTextButton(splitFrame, "Save output to file");
    splitFrame->AddFrame(btnSaveResult, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, dx/2));
    tabFit->AddFrame(splitFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, dx, dx, dy, 2*dy));

    // Attach Tabs Widget
    tabsWidget->SetTab(0);
    setTabEnabled(1, false);
    setTabEnabled(2, false);
    tabsWidget->SetWidth(Constants::leftPanelWidth); // Resize(tabsWidget->GetDefaultSize());
    AddFrame(tabsWidget, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY, dx, dx, 2*dy, 2*dy));

    // Right panel
    TGVerticalFrame* frameRightVertical = new TGVerticalFrame(this);
    TGHorizontalFrame* frameExportButtons = new TGHorizontalFrame(frameRightVertical);

//    numDisplayMin = new TGNumberEntry(frameExportButtons, 0, 6, -1, TGNumberFormat::kNESRealOne,
//            TGNumberFormat::kNEAAnyNumber,
//            TGNumberFormat::kNELLimitMinMax,
//            -9999, 9999);
//    numDisplayMax = new TGNumberEntry(frameExportButtons, 0, 6, -1, TGNumberFormat::kNESRealOne,
//            TGNumberFormat::kNEAAnyNumber,
//            TGNumberFormat::kNELLimitMinMax,
//            -9999, 9999);
//    btnApplyZoom = new TGTextButton(frameExportButtons, "Apply Display Range");
//    btnApplyZoom->Connect("Clicked()", "SWCalculatorView", this, "onApplyZoomClicked()");
//    btnResetZoom = new TGTextButton(frameExportButtons, "Reset");
//    btnResetZoom->Connect("Clicked()", "SWCalculatorView", this, "onResetZoomClicked()");

//    frameExportButtons->AddFrame(numDisplayMin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom
//    frameExportButtons->AddFrame(new TGLabel(frameExportButtons, "-"), new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, dy, 0));  // left, right, top, bottom
//    frameExportButtons->AddFrame(numDisplayMax, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom
//    frameExportButtons->AddFrame(btnApplyZoom, new TGLayoutHints(kLHintsLeft | kLHintsTop, dx, dx, 0, 0));  // left, right, top, bottom
//    frameExportButtons->AddFrame(btnResetZoom, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom

    displayMin = new TGNumberEntry(frameExportButtons, 0, 5, UiHelper::getUId(), TGNumberFormat::kNESRealOne,
        TGNumberFormat::kNEAAnyNumber,
        TGNumberFormat::kNELLimitMinMax,
        -1000, 1000);
    frameExportButtons->AddFrame(displayMin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));

    zoomSlider = new TGDoubleHSlider(frameExportButtons, 190, kDoubleScaleBoth, UiHelper::getUId(), kHorizontalFrame, GetDefaultFrameBackground(), kFALSE, kFALSE);
    zoomSlider->SetRange(-60,60);
    zoomSlider->SetPosition(-60,60);
    frameExportButtons->AddFrame(zoomSlider, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, dx, dx, 0, 0));

    displayMax = new TGNumberEntry(frameExportButtons, 0, 5, UiHelper::getUId(), TGNumberFormat::kNESRealOne,
        TGNumberFormat::kNEAAnyNumber,
        TGNumberFormat::kNELLimitMinMax,
        -1000, 1000);
    frameExportButtons->AddFrame(displayMax, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));
//    tbMin->AddText(0, "0.0");
//    tbMax->AddText(0, "0.0");
//
//    char buf[32];
//    sprintf(buf, "%.1f", zoomSlider->GetMinPosition());
//    tbMin->Clear();
//    tbMin->AddText(0, buf);
//    sprintf(buf, "%.1f", zoomSlider->GetMaxPosition());
//    tbMax->Clear();
//    tbMax->AddText(0, buf);

    // Save image button
    btnSaveImage = new TGTextButton(frameExportButtons, "Save image");
    btnSaveImage->SetEnabled(false);
    frameExportButtons->AddFrame(btnSaveImage, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, dx, 0, 0));

    // Save data file button
    btnSaveData = new TGTextButton(frameExportButtons, "Save data columns");
    btnSaveData->SetEnabled(false);
    frameExportButtons->AddFrame(btnSaveData, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom

    setToolbarEnabled(kFALSE);

    // Attach Right Canvas (Plot)
    TRootEmbeddedCanvas *embedPlot = new TRootEmbeddedCanvas("embedPlot", frameRightVertical);
    frameRightVertical->AddFrame(embedPlot, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY, 0, dx, dx, dx));

    frameRightVertical->AddFrame(frameExportButtons, new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 0));

    AddFrame(frameRightVertical, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, dx));

    // Initialize pads on canvas
    canvasPlot = embedPlot->GetCanvas();
    canvasPlot->Divide(1, 2);

    canvasPlot->cd(1)->SetPad("padData", "Pad for data", 0.0, GraphicsHelper::RESIDUALS_PAD_RELATIVE_HEIGHT, 1.0, 1.0, kWhite);
    gPad->SetMargin((GraphicsHelper::padMargins).left,
    		(GraphicsHelper::padMargins).right, (GraphicsHelper::padMargins).bottom, (GraphicsHelper::padMargins).top);
	gPad->SetLogy();
    padData = (TPad*)canvasPlot->GetPad(1);

    canvasPlot->cd(2)->SetPad("padChi2", "Pad for chi^2", 0.0, 0.0, 1.0, GraphicsHelper::RESIDUALS_PAD_RELATIVE_HEIGHT, kWhite);
    gPad->SetMargin((GraphicsHelper::padMargins).left, (GraphicsHelper::padMargins).right, (GraphicsHelper::padMargins).bottom, (GraphicsHelper::padMargins).top);
    padChi2 = (TPad*)canvasPlot->GetPad(2);

    // You should call, for example HideFrame(TGFrame *f), only after the frames have been laid out and the sub windows
    // of the composite frame have been mapped via method MapSubwindows()
    // RootHelper::hideFrame(resolutionFwhmFrame);
}

void SWCalculatorView::connectSignals(){
	numFitMin->Connect("ValueSet(Long_t)", "SWCalculatorPresenter", presenter, "onViewFitRangeSet()");
	numFitMax->Connect("ValueSet(Long_t)", "SWCalculatorPresenter", presenter, "onViewFitRangeSet()");
	numFitSlider->Connect("PositionChanged()", "SWCalculatorPresenter", presenter, "onViewFitSliderRangeSet()");
	// comboConvolutionType->Connect("Selected(Int_t)", "SWCalculatorPresenter", presenter, "onViewConvolutionSelected(Int_t)");
	convTypeButtonGroup->Connect("Pressed(Int_t)", "SWCalculatorPresenter", presenter, "onViewConvolutionSelected(Int_t)");

	hasParabola->Connect("Toggled(Bool_t)", "SWCalculatorPresenter", presenter, "onViewHasParabolaSet(Bool_t)");
	numGauss->Connect("ValueSet(Long_t)", "SWCalculatorPresenter", presenter, "onViewNumGaussSet()");
	numExponent->Connect("ValueSet(Long_t)", "SWCalculatorPresenter", presenter, "onViewNumExponentSet()");
	numDampExponent->Connect("ValueSet(Long_t)", "SWCalculatorPresenter", presenter, "onViewNumDampExponentSet()");

	addHistogramButton->Connect("Clicked()", "SWCalculatorPresenter", presenter, "onViewAddHistComponentClicked()");
	removeHistogramButton->Connect("Clicked()", "SWCalculatorPresenter", presenter, "onViewRemoveHistComponentClicked()");

	btnEditParameters->Connect("Clicked()", "SWCalculatorPresenter", presenter, "onViewEditParametersClicked()");
    btnFitSpectrum->Connect("Clicked()", "SWCalculatorPresenter", presenter, "onViewFitSpectrumClicked()");
    btnClearResult->Connect("Clicked()", "SWCalculatorPresenter", presenter, "onViewClearResultsClicked()");
    btnSaveResult->Connect("Clicked()", "SWCalculatorPresenter", presenter, "onViewSaveResultsClicked()");
    btnSaveData->Connect("Clicked()", "SWCalculatorPresenter", presenter, "onViewSaveDataClicked()");
    btnSaveImage->Connect("Clicked()", "SWCalculatorPresenter", presenter, "onViewSaveImageClicked()");


	// Local signals within view
	// numFitMin->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorView", this, "onNumFitMinChanged()");
    // numFitMax->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorView", this, "onNumFitMaxChanged()");

    displayMin->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorView", this, "onDisplayMinChange(char*)");
    displayMax->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorView", this, "onDisplayMaxChange(char*)");
    zoomSlider->Connect("PositionChanged()", "SWCalculatorView", this, "onSliderChange()");
}


void SWCalculatorView::setTabEnabled(Int_t tabNumber, Bool_t isEnabled){
    tabsWidget->SetEnabled(tabNumber, isEnabled);
}

void SWCalculatorView::setFitRange(Double_t min, Double_t max){
	numFitMin->SetNumber(min);
	numFitMax->SetNumber(max);
	numFitSlider->SetPosition(min, max);
}

void SWCalculatorView::setFitRangeLimits(Double_t min, Double_t max){
	numFitMin->SetLimitValues(min, max);
	numFitMax->SetLimitValues(min, max);
	numFitSlider->SetRange(min, max);
}

//void SWCalculatorView::setFitMinMaxValues(Bool_t isTwoDetector){
//    if (isTwoDetector){
//        // Set fit values
//        Int_t leftLimitMin = TMath::Abs(numFitMin->GetNumMin());
//        Int_t rightLimitMax = TMath::Abs(numFitMax->GetNumMax());
//
//        Int_t limit = TMath::Min(leftLimitMin, rightLimitMax);
//        if (numFitMin->GetNumber() < -limit) numFitMin->SetNumber(-limit);
//        if (numFitMax->GetNumber() > limit) numFitMax->SetNumber(limit);
//    }
//    else {
//        numFitMin->SetNumber(496);
//        numFitMax->SetNumber(526);
//    }
//}

void SWCalculatorView::setDisplayLimits(Float_t min, Float_t max) {
    // Update slider position
    zoomSlider->SetRange(min,max);
    zoomSlider->SetPosition(min, max);
    // Update text boxes' values
    displayMin->SetLimitValues(min, max);
    displayMin->SetNumber(min);
    displayMax->SetLimitValues(min, max);
    displayMax->SetNumber(max);
}

// Zoom min number entry
void SWCalculatorView::onDisplayMinChange(char* c) {
    Double_t min = displayMin->GetNumber();
    Double_t max = zoomSlider->GetMaxPosition();
    zoomSlider->SetPosition(min, max);
    updateCanvasLimits(min, max);
}

// Zoom max number entry
void SWCalculatorView::onDisplayMaxChange(char* c) {
    Double_t min = zoomSlider->GetMinPosition();
    Double_t max = displayMax->GetNumber();
    zoomSlider->SetPosition(min, max);
    updateCanvasLimits(min, max);
}

// Zoom slider change
void SWCalculatorView::onSliderChange() {
    Double_t min = zoomSlider->GetMinPosition();
    Double_t max = zoomSlider->GetMaxPosition();
    displayMin->SetNumber(min);
    displayMax->SetNumber(max);
    updateCanvasLimits(zoomSlider->GetMinPosition(), zoomSlider->GetMaxPosition());
}

void SWCalculatorView::initRooPlots(RooPlot* fitFrame, RooPlot* chiFrame) {
    this->fitFrame = fitFrame;
    this->chiFrame = chiFrame;
}

void SWCalculatorView::setToolbarEnabled(Bool_t isEnabled){
    btnSaveData->SetEnabled(isEnabled);
    btnSaveImage->SetEnabled(isEnabled);
    displayMin->SetState(isEnabled);
    displayMax->SetState(isEnabled);
}

TCanvas* SWCalculatorView::getCanvas() {
    return canvasPlot;
}

void SWCalculatorView::reflectTwoDetector(Bool_t isTwoDetector){
	const char* text = isTwoDetector ? "2 x" : "";
    lblRescale1->SetText(text);
    lblRescale2->SetText(text);
    lblRescale3->SetText(text);
}

void SWCalculatorView::setComponentHistogram(TH1F* hist){
	if (hist){
		addHistogramButton->SetEnabled(kFALSE);
		histComponentLabel->SetText(hist->GetTitle());
		removeHistogramButton->SetEnabled(kTRUE);
	}
	else {
		addHistogramButton->SetEnabled(kTRUE);
		histComponentLabel->SetText(Constants::LABEL_NO_FILE_LOADED);
		removeHistogramButton->SetEnabled(kFALSE);
	}
}

void SWCalculatorView::setConvolutionType(ConvolutionType convolutionType){
	convTypeButtonGroup->SetButton(convolutionType);
}

void SWCalculatorView::displayFilename(TString* fileName) {
    txtFitResult->AddLineFast("");
    TString* onlyFileName = StringUtils::stripFileName(fileName);
    txtFitResult->AddLineFast(Form("    %s", onlyFileName->Data()));
}

void SWCalculatorView::displayFitParameters(RooFitResult* fitResult) {
	if (!fitResult) return;

	std::ostringstream os;
    fitResult->printStream(os, fitResult->defaultPrintContents(""), fitResult->defaultPrintStyle(""));
    std::istringstream iss(os.str());
    std::string str;
    for (int i = 0; i<4; i++){
            std::getline(iss, str);
    }
    while (std::getline(iss, str)){
            txtFitResult->AddLineFast(str.c_str());
    }
}

void SWCalculatorView::displayVariable(RooRealVar* variable) {
	if (variable == nullptr) return;
	displayVariables(new RooArgList(*variable));
}

const char* dashedLine = "  ------------------------------------------------";

void SWCalculatorView::displayVariables(RooArgList* variables) {
	txtFitResult->AddLineFast(dashedLine);
	TIterator* it = variables->createIterator();
	while(TObject* temp = it->Next()){
		if(RooRealVar* var = dynamic_cast<RooRealVar*>(temp)){
	        const char* s = var->getError() == 0 ?
	            Form("%*s    %1.4e %s", 22, var->GetTitle(), var->getVal(), var->getUnit()) :
	            Form("%*s    %1.4e (%1.2e) %s", 22, var->GetTitle(), var->getVal(), var->getError(), var->getUnit());
			txtFitResult->AddLineFast(s);
		}
	}
}

void SWCalculatorView::displayChi2(Chi2Struct chi2Struct) {
    txtFitResult->AddLineFast(dashedLine);
    const char* s = Form("%*s    %2.1f/%d = %1.2f", 22, "chi^2/N_free", chi2Struct.chiSum, chi2Struct.degreesOfFreedom, chi2Struct.chi2);
    txtFitResult->AddLineFast(s);
}

void SWCalculatorView::displaySW(RooRealVar* s, RooRealVar* w) {
    txtFitResult->AddLineFast(dashedLine);
    txtFitResult->AddLineFast(Form("%*s    %1.4e +/-  %1.2e", 22, s->GetTitle(), s->getVal(), s->getError()));
    txtFitResult->AddLineFast(Form("%*s    %1.4e +/-  %1.2e", 22, w->GetTitle(), w->getVal(), w->getError()));
    txtFitResult->AddLineFast(dashedLine);
}

void SWCalculatorView::updateCanvas() {
	canvasPlot->Modified();
	canvasPlot->Update();
}

// Scroll output textarea to the very bottom
void SWCalculatorView::scrollOutputDown(){
    txtFitResult->Update();
    txtFitResult->ShowBottom();
}

// Calls to Presenter

void SWCalculatorView::clearFitResults() {
    txtFitResult->SetText(new TGText(""));
    TGLongPosition* pos = new TGLongPosition(0, 0);
    txtFitResult->ScrollToPosition(*pos);
}

void SWCalculatorView::updateCanvasLimits(Double_t min, Double_t max) {
    fitFrame->GetXaxis()->SetRangeUser(min, max);
    chiFrame->GetXaxis()->SetRangeUser(min, max);
    padData->Modified();
    padData->Update();
    padChi2->Modified();
    padChi2->Update();
}

SWCalculatorView::~SWCalculatorView() {
    Cleanup();
}
