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
#include "../../roofit/CompositeModelProvider.h"

using namespace RooFit;

// ClassImp(SWCalculatorView)

//SWCalculatorView::SWCalculatorView(const TGWindow* w) : TGMainFrame(w, Constants::windowWidth, Constants::windowHeight){
SWCalculatorView::SWCalculatorView(const TGWindow* w) : AbstractView<SWCalculatorPresenter>(w){
    initUI();
	presenter = instantinatePresenter();
//	connectSlots();
}

SWCalculatorPresenter* SWCalculatorView::instantinatePresenter(){
    return new SWCalculatorPresenter(this);
}

void SWCalculatorView::initUI(){
    SetLayoutManager(new TGHorizontalLayout(this));

    // Tabs widget
    tabsWidget = new TGTab(this, Constants::leftPanelWidth);

    // Import spectrum tab
    TGCompositeFrame *tabImport = tabsWidget->AddTab("Experimental Spectrum");
    tabImport->SetLayoutManager(new TGVerticalLayout(tabImport));
    tabImport->AddFrame(new ImportSpectrumView(tabImport), new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy*2, dy*2));

    // Import Kapton spectrum tab
    TGCompositeFrame *tabImportKapton = tabsWidget->AddTab("Source Spectrum");
    tabImportKapton->SetLayoutManager(new TGVerticalLayout(tabImportKapton));
    tabImportKapton->AddFrame(new ImportSourceSpectrumView(tabImportKapton), new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy*2, dy*2));

    // Fit Data tab
    tabFit = tabsWidget->AddTab("Fit Data");
    tabFit->SetLayoutManager(new TGVerticalLayout(tabFit));

    // Fit Range Row
    TGHorizontalFrame *frameFitRange = new TGHorizontalFrame(tabFit);
    numFitMin = new TGNumberEntry(frameFitRange, -60, 4, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEAAnyNumber,
            TGNumberFormat::kNELLimitMinMax,
            -9999, 9999);
    numFitMin->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorView", this, "onNumFitMinChanged()");

    numFitMax = new TGNumberEntry(frameFitRange, 60, 4, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEAAnyNumber,
            TGNumberFormat::kNELLimitMinMax,
            -9999, 9999);
    numFitMax->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorView", this, "onNumFitMaxChanged()");
    frameFitRange->AddFrame(new TGLabel(frameFitRange, "Fit Range, keV"), new TGLayoutHints(kLHintsNormal, 0, 0, dy, 0));
    frameFitRange->AddFrame(numFitMax, new TGLayoutHints(kLHintsRight));
    frameFitRange->AddFrame(new TGLabel(frameFitRange, "-"), new TGLayoutHints(kLHintsRight, dx, dx, 3*dy/5, 0));
    frameFitRange->AddFrame(numFitMin, new TGLayoutHints(kLHintsRight));

    tabFit->AddFrame(frameFitRange, new TGLayoutHints(kLHintsExpandX, dx, dx, dy*2, dy));

    // Separator
    tabFit->AddFrame(new TGHorizontal3DLine(tabFit), new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Integration Parameters
    TGHorizontalFrame *frameSWidth = new TGHorizontalFrame(tabFit);
    lblRescale1 = new TGLabel(frameSWidth, "# x ");
    lblRescale1->SetTextJustify(kTextLeft);
    numSWidth = new TGNumberEntry(frameSWidth, 1.60, 4, -1, TGNumberFormat::kNESRealTwo,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMin,
            0.01, 9.99);
    frameSWidth->AddFrame(new TGLabel(frameSWidth, "S Region Width"), new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, dy, 0));
    frameSWidth->AddFrame(numSWidth, new TGLayoutHints(kLHintsRight | kLHintsTop));
    frameSWidth->AddFrame(lblRescale1, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, dx, 3*dy/5, 0));

    tabFit->AddFrame(frameSWidth, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    TGHorizontalFrame *frameWWidth = new TGHorizontalFrame(tabFit);
    lblRescale2 = new TGLabel(frameWWidth, "# x ");
    numWWidth = new TGNumberEntry(frameWWidth, 3.00, 4, -1, TGNumberFormat::kNESRealTwo,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMin,
            0.01, 9.99);
    frameWWidth->AddFrame(new TGLabel(frameWWidth, "W Regions Width"), new TGLayoutHints(kLHintsNormal, 0, 0, dy, 0));
    frameWWidth->AddFrame(numWWidth, new TGLayoutHints(kLHintsRight));
    frameWWidth->AddFrame(lblRescale2, new TGLayoutHints(kLHintsRight, 0, dx, 3*dy/5, 0));

    tabFit->AddFrame(frameWWidth, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    TGHorizontalFrame *frameWShift = new TGHorizontalFrame(tabFit);
    lblRescale3 = new TGLabel(frameWShift, "# x ");
    numWShift = new TGNumberEntry(frameWShift, 2.76, 4, -1, TGNumberFormat::kNESRealTwo,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMin,
            0.01, 9.99);
    frameWShift->AddFrame(new TGLabel(frameWShift, "W Regions Shift"), new TGLayoutHints(kLHintsNormal, 0, 0, dy, 0));
    frameWShift->AddFrame(numWShift, new TGLayoutHints(kLHintsRight));
    frameWShift->AddFrame(lblRescale3, new TGLayoutHints(kLHintsRight, 0, dx, 3*dy/5, 0));

    tabFit->AddFrame(frameWShift, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Separator
    tabFit->AddFrame(new TGHorizontal3DLine(tabFit), new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Convolution type and resolution function
    TGHorizontalFrame* convolutionParamsFrame = new TGHorizontalFrame(tabFit);

    comboConvolutionType = new TGComboBox(convolutionParamsFrame, 0);
    // Initialize a map with std::initialzer_list
    const std::map<Int_t, TString> convolutionTypesMap = {
    		{CompositeModelProvider::kNoConvolution, "None"},
    		{CompositeModelProvider::kFftConvolution, "FFT3"}
    };
    for(auto it=convolutionTypesMap.begin(); it!=convolutionTypesMap.end(); ++it){
        comboConvolutionType->AddEntry((it->second).Data(), it->first);
    }
    comboConvolutionType->Connect("Selected(Int_t)", "SWCalculatorView", this, "onConvolutionSelected(Int_t)");
    comboConvolutionType->Select(CompositeModelProvider::kNoConvolution, kFALSE);
    comboConvolutionType->Resize(60, 20);

    convolutionParamsFrame->AddFrame(new TGLabel(convolutionParamsFrame, "Convolution"),
                                     new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 4*dy/5, 0));
    convolutionParamsFrame->AddFrame(comboConvolutionType, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, dy/5, 0));

    resolutionFwhmFrame = new TGHorizontalFrame(convolutionParamsFrame);
    checkboxResFixed = new TGCheckButton(resolutionFwhmFrame, "fixed");
    checkboxResFixed->SetOn();
    numResolutionFWHM = new TGNumberEntry(resolutionFwhmFrame, 2.0, 4, -1, TGNumberFormat::kNESRealTwo,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            0.5, 4.0);

    resolutionFwhmFrame->AddFrame(checkboxResFixed, new TGLayoutHints(kLHintsRight | kLHintsTop, dx, 0, 3*dy/5, 0));
    resolutionFwhmFrame->AddFrame(numResolutionFWHM, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, dy/5, 0));
    resolutionFwhmFrame->AddFrame(new TGLabel(resolutionFwhmFrame, "Sum of detectors' FWHMs, keV"),
                                     new TGLayoutHints(kLHintsRight | kLHintsTop, 0, dx, 4*dy/5, 0));
    convolutionParamsFrame->AddFrame(resolutionFwhmFrame, new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 0));

    tabFit->AddFrame(convolutionParamsFrame, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Separator
    tabFit->AddFrame(new TGHorizontal3DLine(tabFit), new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Add source contribution frame
    sourceContributionFrame = new TGVerticalFrame(tabFit);
    sourceContributionView = new RooRealVarView(sourceContributionFrame);
    sourceContributionFrame->AddFrame(sourceContributionView, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));
    sourceContributionFrame->AddFrame(new TGHorizontal3DLine(sourceContributionFrame), new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));
    tabFit->AddFrame(sourceContributionFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX));

    // Model parameters
    TGHorizontalFrame* modelParamsFrame = new TGHorizontalFrame(tabFit);
    //	modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Model details:"), new TGLayoutHints(kLHintsNormal, 0, 30, 5, 0));
    checkboxHasParabola = new TGCheckButton(modelParamsFrame, "Parabola", -1);
    modelParamsFrame->AddFrame(checkboxHasParabola, new TGLayoutHints(kLHintsNormal, 0, 4*dx, 4*dy/5, 0));

    numGauss = new TGNumberEntry(modelParamsFrame, 3, 1, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            0, 4);
    modelParamsFrame->AddFrame(numGauss, new TGLayoutHints(kLHintsNormal, 0, dx, 2*dy/5, 0));
    modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Gauss"), new TGLayoutHints(kLHintsNormal, 0, 3*dx, dy, 0));

    numExponent = new TGNumberEntry(modelParamsFrame, 0, 1, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            0, 4);
    modelParamsFrame->AddFrame(numExponent, new TGLayoutHints(kLHintsNormal, 0, dx, 2*dy/5, 0));
    modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Exp"), new TGLayoutHints(kLHintsNormal, 0, 3*dx, dy, 0));

    numDampExponent = new TGNumberEntry(modelParamsFrame, 0, 1, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            0, 4);
    modelParamsFrame->AddFrame(numDampExponent, new TGLayoutHints(kLHintsNormal, 0, dx, 2*dy/5, 0));
    modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Damping Exp"), new TGLayoutHints(kLHintsNormal, 0, 3*dx, dy, 0));

    //	hasOrtho = new TGCheckButton(modelParamsFrame, "Ortho Exps", -1);
    //	modelParamsFrme->AddFrame(hasOrtho, new TGLayoutHints(kLHintsNormal, 0, 20, 4, 0));

    tabFit->AddFrame(modelParamsFrame, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Fit Button
    TGHorizontalFrame* frameFitSpectrum = new TGHorizontalFrame(tabFit);
    btnFitSpectrum = new TGTextButton(frameFitSpectrum, "Fit and plot");
    btnFitSpectrum->Connect("Clicked()", "SWCalculatorView", this, "onFitSpectrumClicked()");
    frameFitSpectrum->AddFrame(btnFitSpectrum, new TGLayoutHints(kLHintsExpandX));
    tabFit->AddFrame(frameFitSpectrum, new TGLayoutHints(kLHintsExpandX, dx, dx, dy + 3, dy));

    // Fit Result TextBox
    txtFitResult = new TGTextEdit(tabFit);
    txtFitResult->SetBackgroundColor(GraphicsHelper::colorAppWindow->GetPixel());
    tabFit->AddFrame(txtFitResult, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy, dy));

    TGHorizontalFrame* splitFrame = new TGHorizontalFrame(tabFit);

    btnClearResult = new TGTextButton(splitFrame, "Clear output");
    btnClearResult->Connect("Clicked()", "SWCalculatorView", this, "onClearResultsClicked()");
    splitFrame->AddFrame(btnClearResult, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 0, dx/2));
    btnSaveResult = new TGTextButton(splitFrame, "Save to file");
    btnSaveResult->Connect("Clicked()", "SWCalculatorView", this, "onSaveResultsClicked()");
    splitFrame->AddFrame(btnSaveResult, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, dx/2));
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

    displayMin = new TGNumberEntry(frameExportButtons, 0, 5, -1, TGNumberFormat::kNESRealOne,
        TGNumberFormat::kNEAAnyNumber,
        TGNumberFormat::kNELLimitMinMax,
        -1000, 1000);
    displayMin->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorView", this, "onDisplayMinChange(char*)");
    frameExportButtons->AddFrame(displayMin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));

    zoomSlider = new TGDoubleHSlider(frameExportButtons, 190, kDoubleScaleBoth, -1, kHorizontalFrame, GetDefaultFrameBackground(), kFALSE, kFALSE);
    zoomSlider->Connect("PositionChanged()", "SWCalculatorView",
                       this, "onSliderChange()");
    zoomSlider->SetRange(-60,60);
    zoomSlider->SetPosition(-60,60);
    frameExportButtons->AddFrame(zoomSlider, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, dx, dx, 0, 0));

    displayMax = new TGNumberEntry(frameExportButtons, 0, 5, -1, TGNumberFormat::kNESRealOne,
        TGNumberFormat::kNEAAnyNumber,
        TGNumberFormat::kNELLimitMinMax,
        -1000, 1000);
    displayMax->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorView", this, "onDisplayMaxChange(char*)");
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

    // Save data file button
//    btnSaveData = new TGTextButton(frameExportButtons, "Export Fit Data");
//    btnSaveData->Connect("Clicked()", "SWCalculatorView", this, "onSaveDataClicked()");
//    btnSaveData->SetEnabled(false);
//    frameExportButtons->AddFrame(btnSaveData, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 0, 0));  // left, right, top, bottom

    // Save image button
    btnSaveImage = new TGTextButton(frameExportButtons, "Save Image");
    btnSaveImage->Connect("Clicked()", "SWCalculatorView", this, "onSaveImageClicked()");
    btnSaveImage->SetEnabled(false);
    frameExportButtons->AddFrame(btnSaveImage, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, dx, 0, 0));

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

// Calls from Presenter
//RooRealVarView* SWCalculatorView::getSourceContributionView() {
//    return sourceContributionView;
//}

void SWCalculatorView::onUiReady(){
	RootHelper::hideFrame(resolutionFwhmFrame);
	RootHelper::hideFrame(checkboxResFixed);
	RootHelper::hideFrame(sourceContributionFrame);
}


void SWCalculatorView::setTabEnabled(Int_t tabNumber, Bool_t isEnabled){
    tabsWidget->SetEnabled(tabNumber, isEnabled);
}

Int_t SWCalculatorView::getFitMinValue(){
    return numFitMin->GetNumber();
}

Int_t SWCalculatorView::getFitMaxValue(){
    return numFitMax->GetNumber();
}

void SWCalculatorView::setFitMinMaxRange(Int_t min, Int_t max){
    numFitMin->SetLimitValues(min,max);
    numFitMax->SetLimitValues(min,max);
}

void SWCalculatorView::setFitMinMaxValues(Bool_t isTwoDetector){
    if (isTwoDetector){
        // Set fit values
        Int_t leftLimitMin = TMath::Abs(numFitMin->GetNumMin());
        Int_t rightLimitMax = TMath::Abs(numFitMax->GetNumMax());

        Int_t limit = TMath::Min(leftLimitMin, rightLimitMax);
        if (numFitMin->GetNumber() < -limit) numFitMin->SetNumber(-limit);
        if (numFitMax->GetNumber() > limit) numFitMax->SetNumber(limit);
    }
    else {
        numFitMin->SetNumber(496);
        numFitMax->SetNumber(526);
    }
}

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

void SWCalculatorView::onDisplayMinChange(char* c) {
    Double_t min = displayMin->GetNumber();
    Double_t max = zoomSlider->GetMaxPosition();
    zoomSlider->SetPosition(min, max);
    updateCanvasLimits(min, max);
}

void SWCalculatorView::onDisplayMaxChange(char* c) {
    Double_t min = zoomSlider->GetMinPosition();
    Double_t max = displayMax->GetNumber();
    zoomSlider->SetPosition(min, max);
    updateCanvasLimits(min, max);
}

void SWCalculatorView::onSliderChange() {
//   char buf[32];

//   sprintf(buf, "%.1f", zoomSlider->GetMinPosition());
//   tbMin->Clear();
//   tbMin->AddText(0, buf);
//   displayMin->SetCursorPosition(displayMin->GetCursorPosition());
//   displayMin->Deselect();
//   gClient->NeedRedraw(displayMin);

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

Double_t SWCalculatorView::getSWidth(){
    return numSWidth->GetNumber();
}

Double_t SWCalculatorView::getWWidth(){
    return numWWidth->GetNumber();
}

Double_t SWCalculatorView::getWShift(){
    return numWShift->GetNumber();
}

Int_t SWCalculatorView::getConvolutionType(){
    return comboConvolutionType->GetSelected();
}

Bool_t SWCalculatorView::hasParabola(){
    return checkboxHasParabola->IsOn();
}

Int_t SWCalculatorView::getNumGauss(){
    return numGauss->GetNumber();
}

Int_t SWCalculatorView::getNumExp(){
    return numExponent->GetNumber();
}

Int_t SWCalculatorView::getNumDampExp(){
    return numDampExponent->GetNumber();
}

Double_t SWCalculatorView::getResolutionFWHM(){
    return numResolutionFWHM->GetNumber();
}

Bool_t SWCalculatorView::isResolutionFixed() {
    return checkboxResFixed->IsOn();
}

TPad* SWCalculatorView::getPadData() {
    return padData;
}

TPad* SWCalculatorView::getPadChi2() {
    return padChi2;
}

void SWCalculatorView::setToolbarEnabled(Bool_t isEnabled){
//    btnApplyZoom->SetEnabled(isEnabled);
//    btnResetZoom->SetEnabled(isEnabled);
    btnSaveImage->SetEnabled(isEnabled);
//    btnSaveData->SetEnabled(isEnabled);
//    numDisplayMin->SetState(isEnabled);
//    numDisplayMax->SetState(isEnabled);
    displayMin->SetState(isEnabled);
    displayMax->SetState(isEnabled);
}

void SWCalculatorView::setSourceContributionFrameVisible(Bool_t isVisible) {
    if (isVisible) RootHelper::showFrame(sourceContributionFrame);
    else RootHelper::hideFrame(sourceContributionFrame);
}

TCanvas* SWCalculatorView::getCanvas() {
    return canvasPlot;
}

void SWCalculatorView::onConvolutionSelected(Int_t id){
	if (id == CompositeModelProvider::kNoConvolution){
		RootHelper::hideFrame(resolutionFwhmFrame);
	}
	else {
		RootHelper::showFrame(resolutionFwhmFrame);
	}
}

void SWCalculatorView::updateRegionLabels(Bool_t isTwoDetector){
    lblRescale1->SetText(isTwoDetector ? "2 x " : " ");
    lblRescale2->SetText(isTwoDetector ? "2 x " : " ");
    lblRescale3->SetText(isTwoDetector ? "2 x " : " ");
}

void SWCalculatorView::displayFilename(TString* fileName) {
    txtFitResult->AddLineFast("");
    TString* onlyFileName = StringUtils::stripFileName(fileName);
    txtFitResult->AddLineFast(Form("    %s", onlyFileName->Data()));
}

void SWCalculatorView::displayFitParameters(RooFitResult* fitResult) {
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

void SWCalculatorView::displaySW(std::pair<Double_t, Double_t> sValueError, std::pair<Double_t, Double_t> wValueError) {
    txtFitResult->AddLineFast(dashedLine);
    txtFitResult->AddLineFast(Form("%*s    %1.4e +/-  %1.2e", 22, "S Parameter", sValueError.first, sValueError.second));
    txtFitResult->AddLineFast(Form("%*s    %1.4e +/-  %1.2e", 22, "W Parameter", wValueError.first, wValueError.second));
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
void SWCalculatorView::onNumFitMinChanged(){
	Debug("SWCalculatorView::onNumFitMinChanged");
}

void SWCalculatorView::onNumFitMaxChanged(){
	Debug("SWCalculatorView::onNumFitMaxChanged");
}

void SWCalculatorView::onFitSpectrumClicked(){
    presenter->onFitSpectrumClicked();
}

void SWCalculatorView::onApplyZoomClicked(){
	Debug("SWCalculatorView::onApplyZoomClicked");
}

void SWCalculatorView::onResetZoomClicked(){
	Debug("SWCalculatorView::onResetZoomClicked");
}

void SWCalculatorView::onSaveResultsClicked() {
    presenter->onSaveResultsClicked();
}

void SWCalculatorView::onClearResultsClicked() {
    presenter->onClearResultsClicked();
}

void SWCalculatorView::saveFitResults(TString* fileName) {
    Bool_t ok = txtFitResult->SaveFile(fileName->Data());
    UiHelper* uiHelper = UiHelper::getInstance();
    uiHelper->showOkDialog(ok ? "Results saved successfully" : "Error saving results file");
}

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

//void SWCalculatorView::setSourceContribution(RooRealVar* sourceContribution){
//	sourceContributionView->setRooRealVar(sourceContribution);
//}


//void SWCalculatorView::onSaveDataClicked(){
//    std::cout << "SWCalculatorView::onSaveDataClicked()" << std::endl;
//}

void SWCalculatorView::onSaveImageClicked(){
    presenter->onSaveImageClicked();
}

SWCalculatorView::~SWCalculatorView() {
    Cleanup();
    delete numPeakPosition;
    delete numFitMin;
    delete numFitMax;
    delete lblRescale1;
    delete lblRescale2;
    delete lblRescale3;
    delete numSWidth;
    delete numWWidth;
    delete numWShift;
    delete comboConvolutionType;
    delete numResolutionFWHM;
    delete checkboxResFixed;
    delete checkboxHasParabola;
    delete numGauss;
    delete numExponent;
    delete numDampExponent;
    delete btnFitSpectrum;
    delete txtFitResult;
//    delete btnSaveData;
    delete btnSaveImage;
    delete fitFrame;
    delete chiFrame;
    delete padData;
    delete padChi2;
    delete canvasPlot;
}
