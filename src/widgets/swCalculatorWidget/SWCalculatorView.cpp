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
#include <TG3DLine.h>
#include <TLine.h>
#include <TFrame.h>
#include "../../roofit/PdfProvider.h"

using namespace RooFit;

// https://root.cern/manual/signal_slot/
ClassImp(SWCalculatorView);

//SWCalculatorView::SWCalculatorView(const TGWindow* w) : TGMainFrame(w, Constants::windowWidth, Constants::windowHeight){
SWCalculatorView::SWCalculatorView(const TGWindow* w) : AbstractView<SWCalculatorPresenter>(w){
    initUI();
	presenter = instantinatePresenter();
	connectSignals();

	// Instantiate THStack for displaying imported histograms
	tHStack = new THStack("hs", "");

	// Tweak display styles
	// Question: Is this same for RooPlot and regular canvas?
	gStyle->SetTitleOffset(1.3, "X");
    gStyle->SetTitleOffset(1.5, "Y");
	gStyle->SetLabelOffset(0.015, "XY");
    gStyle->SetLabelSize(GraphicsHelper::TEXT_SIZE_SMALL, "XY");
    gStyle->SetTitleSize(GraphicsHelper::TEXT_SIZE_NORMAL, "XY");
	// currentCanvasMode = CanvasMode::onePad;
}

SWCalculatorPresenter* SWCalculatorView::instantinatePresenter(){
    return new SWCalculatorPresenter(this);
}

void SWCalculatorView::initUI(){
    SetLayoutManager(new TGHorizontalLayout(this));

    // Tabs widget
    tabsWidget = new TGTab(this, Constants::leftPanelWidth);

    // Import spectrum tab
    TGCompositeFrame *tabImport = tabsWidget->AddTab("Studied Spectrum");
    tabImport->SetLayoutManager(new TGVerticalLayout(tabImport));
    tabImport->AddFrame(new ImportSpectrumView(tabImport), new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy*2, dy*2));

    // Import Kapton spectrum tab
    TGCompositeFrame *tabImportKapton = tabsWidget->AddTab("Source Contribution");
    tabImportKapton->SetLayoutManager(new TGVerticalLayout(tabImportKapton));
    tabImportKapton->AddFrame(new ImportSourceSpectrumView(tabImportKapton), new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy*2, dy*2));

    // Fit Data tab
    tabFit = tabsWidget->AddTab("Analysis");
    tabFit->SetLayoutManager(new TGVerticalLayout(tabFit));

    // Fit Range Row (now in bins!)
    // TODO: display real values as labels
    tabFit->AddFrame(new TGLabel(tabFit, "Spectrum fitting range, bins"), new TGLayoutHints(kLHintsNormal, dx, dx, 2*dy, 0));
    TGHorizontalFrame *frameFitRange = new TGHorizontalFrame(tabFit);
    numFitMin = new MyTGNumberEntry(frameFitRange, 1, 5, UiHelper::getUId(), TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEAPositive,
            TGNumberFormat::kNELLimitMinMax,
            1, 9999);

    numFitMax = new MyTGNumberEntry(frameFitRange, 100, 5, UiHelper::getUId(), TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEAPositive,
            TGNumberFormat::kNELLimitMinMax,
            1, 9999);

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
    numSWidth = new MyTGNumberEntry(frameSWidth, 1.60, 4, UiHelper::getUId(), TGNumberFormat::kNESRealTwo,
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
    numWWidth = new MyTGNumberEntry(frameWWidth, 3.00, 4, UiHelper::getUId(), TGNumberFormat::kNESRealTwo,
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
    numWShift = new MyTGNumberEntry(frameWShift, 2.76, 4, UiHelper::getUId(), TGNumberFormat::kNESRealTwo,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMin,
            0.01, 9.99);
    frameWShift->AddFrame(new TGLabel(frameWShift, "W regions shift, keV"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
    frameWShift->AddFrame(numWShift, new TGLayoutHints(kLHintsRight | kLHintsCenterY));
    frameWShift->AddFrame(lblRescale3, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsCenterY, 0, dx));

    tabFit->AddFrame(frameWShift, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Separator
    tabFit->AddFrame(new TGHorizontal3DLine(tabFit), new TGLayoutHints(kLHintsExpandX, dx, dx, dy, 0));

    // Background type (for single detector experiment)
    singleDetectorBackgroundFrame = new TGHorizontalFrame(tabFit);
    singleDetectorBackgroundFrame->AddFrame(new TGLabel(singleDetectorBackgroundFrame, "Background:"), new TGLayoutHints(kLHintsLeft | kLHintsBottom, dx+1, 3*dx));

    bgTypeButtonGroup = new TGButtonGroup(singleDetectorBackgroundFrame, 1, 3, 0, kLHintsLeft | kLHintsCenterY); // rows, columns, spacing
    {
        TGRadioButton* radioButton = new TGRadioButton(bgTypeButtonGroup, "Erf", BackgroundType::kErf);
        radioButton->SetWidth(75);
        radioButton->ChangeOptions(EFrameType::kFixedWidth);
    }
    {
        TGRadioButton* radioButton = new TGRadioButton(bgTypeButtonGroup, "Atan", BackgroundType::kAtan);
        radioButton->SetWidth(75);
        radioButton->ChangeOptions(EFrameType::kFixedWidth);
    }
    {
        TGRadioButton* radioButton = new TGRadioButton(bgTypeButtonGroup, "Step", BackgroundType::kStep);
        radioButton->SetWidth(75);
        radioButton->ChangeOptions(EFrameType::kFixedWidth);
    }
    singleDetectorBackgroundFrame->AddFrame(bgTypeButtonGroup, new TGLayoutHints(kLHintsNormal));

    tabFit->AddFrame(singleDetectorBackgroundFrame, new TGLayoutHints(kLHintsNormal, dx, dx, 0, 0));

//    resolutionFwhmFrame = new TGHorizontalFrame(convolutionParamsFrame);
//    checkboxResFixed = new TGCheckButton(resolutionFwhmFrame, "fixed");
//    checkboxResFixed->SetOn();
//    numResolutionFWHM = new MyTGNumberEntry(resolutionFwhmFrame, 2.0, 4, -1, TGNumberFormat::kNESRealTwo,
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

    // Model parameters group frame
    TGCompositeFrame* modelComponentsGroupFrame = new TGGroupFrame(tabFit, "Model Components");
    TGCompositeFrame* modelParamsFrame = new TGVerticalFrame(modelComponentsGroupFrame);

    // Parabola and Delta function
    {
        TGHorizontalFrame* frame = new TGHorizontalFrame(modelParamsFrame);

        hasParabola = new TGCheckButton(frame, "Parabola");
        frame->AddFrame(hasParabola, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 3*dx - 3));

        hasDelta = new TGCheckButton(frame, "Delta function");
        frame->AddFrame(hasDelta, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));

        modelParamsFrame->AddFrame(frame, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 0, 0, dy, dx));
    }

    // Gauss, Exponent, other pdfs
    {
        TGHorizontalFrame* frame = new TGHorizontalFrame(modelParamsFrame);

        numGauss = new MyTGNumberEntry(frame, 0, 1, UiHelper::getUId(), TGNumberFormat::kNESInteger,
                TGNumberFormat::kNEANonNegative,
                TGNumberFormat::kNELLimitMinMax,
                0, 9);
        frame->AddFrame(numGauss, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, dx));
        frame->AddFrame(new TGLabel(frame, "Gauss"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 3*dx));

        numExponent = new MyTGNumberEntry(frame, 0, 1, UiHelper::getUId(), TGNumberFormat::kNESInteger,
                TGNumberFormat::kNEANonNegative,
                TGNumberFormat::kNELLimitMinMax,
                0, 9);
        frame->AddFrame(numExponent, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, dx));
        frame->AddFrame(new TGLabel(frame, "Lorentzian"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 3*dx));

        numDampExponent = new MyTGNumberEntry(frame, 0, 1, UiHelper::getUId(), TGNumberFormat::kNESInteger,
                TGNumberFormat::kNEANonNegative,
                TGNumberFormat::kNELLimitMinMax,
                0, 9);
        frame->AddFrame(numDampExponent, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, dx));
        frame->AddFrame(new TGLabel(frame, "Damped Lorentzian"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY));

        modelParamsFrame->AddFrame(frame, new TGLayoutHints(kLHintsNormal, 0, 0, dy, dx));
    }
    modelComponentsGroupFrame->AddFrame(modelParamsFrame, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, 0, 0, dy, dy));

    // Add custom histogram pdf
    TGHorizontalFrame* modelHistogramFrame = new TGHorizontalFrame(modelComponentsGroupFrame);
    addHistogramButton = new TGTextButton(modelHistogramFrame, "Add from file...");
    modelHistogramFrame->AddFrame(addHistogramButton, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, dx));
    histComponentLabel = new TGLabel(modelHistogramFrame, Constants::LABEL_NO_FILE_LOADED);
    histComponentLabel->SetTextJustify(kTextLeft);
    histComponentLabel->Disable(kTRUE);
    histComponentLabel->ChangeOptions(histComponentLabel->GetOptions() | kFixedSize);
    modelHistogramFrame->AddFrame(histComponentLabel, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsCenterY, 0, dx));

    removeHistogramButton = new TGTextButton(modelHistogramFrame, "Remove");
    modelHistogramFrame->AddFrame(removeHistogramButton, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0));
    removeHistogramButton->SetEnabled(false);
    //	hasOrtho = new TGCheckButton(modelParamsFrame, "Ortho Exps", UiHelper::getUId());
    //	modelParamsFrme->AddFrame(hasOrtho, new TGLayoutHints(kLHintsNormal, 0, 20, 4, 0));
    modelComponentsGroupFrame->AddFrame(modelHistogramFrame, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, 0, 0, dy, dy));
    tabFit->AddFrame(modelComponentsGroupFrame, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, dx, dx, dy*3, 0));

    // Convolution type and resolution function
    TGHorizontalFrame* convolutionParamsFrame = new TGHorizontalFrame(tabFit);
    convolutionParamsFrame->AddFrame(new TGLabel(convolutionParamsFrame, "Convolution:"), new TGLayoutHints(kLHintsLeft | kLHintsBottom, dx+1, 3*dx));

    // Initialize a map with std::initialzer_list
//    comboConvolutionType = new TGComboBox(convolutionParamsFrame, 0);
//    const std::map<Int_t, TString> convolutionTypesMap = {
//          {ConvolutionType::kNoConvolution, "None"},
//          {ConvolutionType::kFFTConvolution, "Numerical FFT"}
//    };
//    for (auto it = convolutionTypesMap.begin(); it != convolutionTypesMap.end(); ++it) {
//        comboConvolutionType->AddEntry((it->second).Data(), it->first);
//    }
//    comboConvolutionType->Select(ConvolutionType::kNoConvolution, kFALSE);
//    comboConvolutionType->Resize(120, 20);
//    convolutionParamsFrame->AddFrame(comboConvolutionType, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, dx, 0, 0, 2));

    // Create button Group with Matrix layout. ButtonGroup spacings are a mess.
    convTypeButtonGroup = new TGButtonGroup(convolutionParamsFrame, 1, 2, 0, kLHintsLeft | kLHintsCenterY); // rows, columns, spacing
    {
        TGRadioButton* radioButton = new TGRadioButton(convTypeButtonGroup, "None", ConvolutionType::kNoConvolution);
        radioButton->SetWidth(75);
        radioButton->ChangeOptions(EFrameType::kFixedWidth);
    }
    {
        TGRadioButton* radioButton = new TGRadioButton(convTypeButtonGroup, "Fourier", ConvolutionType::kFFTConvolution);
        radioButton->SetWidth(75);
        radioButton->ChangeOptions(EFrameType::kFixedWidth);
    }
    // {
    //     TGRadioButton* radioButton = new TGRadioButton(convTypeButtonGroup, "Numeric", ConvolutionType::kNumericConvolution);
    //     radioButton->SetWidth(75);
    //     radioButton->ChangeOptions(EFrameType::kFixedWidth);
    // }
    convolutionParamsFrame->AddFrame(convTypeButtonGroup, new TGLayoutHints(kLHintsNormal));
    tabFit->AddFrame(convolutionParamsFrame, new TGLayoutHints(kLHintsNormal, dx, dx, 0, dy*2));

    // "Edit Fit Parameters" and "Fit and Plot" buttons
    TGHorizontalFrame* frameFitSpectrum = new TGHorizontalFrame(tabFit);
    btnEditParameters = new TGTextButton(frameFitSpectrum, "Edit Fit Parameters");
    frameFitSpectrum->AddFrame(btnEditParameters, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, 0, dx/2));
    btnFitSpectrum = new TGTextButton(frameFitSpectrum, "Fit and Plot");
    frameFitSpectrum->AddFrame(btnFitSpectrum, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, dx/2));
    tabFit->AddFrame(frameFitSpectrum, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Fit Result TextBox
    txtFitResult = new TGTextEdit(tabFit);
//    txtFitResult->ChangeOptions(txtFitResult->GetOptions() | EFrameType::kFixedWidth);
//    txtFitResult->SetMinWidth(700);
    // txtFitResult->SetWidth(400);
    txtFitResult->SetBackgroundColor(GraphicsHelper::colorAppWindow->GetPixel());
    tabFit->AddFrame(txtFitResult, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy, dy));

    TGHorizontalFrame* splitFrame = new TGHorizontalFrame(tabFit);

    btnClearResult = new TGTextButton(splitFrame, "Clear Output");
    splitFrame->AddFrame(btnClearResult, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, 0, dx/2));
    btnSaveResult = new TGTextButton(splitFrame, "Save Output");
    splitFrame->AddFrame(btnSaveResult, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, dx/2));
    tabFit->AddFrame(splitFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, dx, dx, dy, 2*dy));

    // Attach Tabs Widget
    tabsWidget->SetTab(0);
    setTabEnabled(1, false);
    setTabEnabled(2, false);
    // Without Layout() tabs widget did not get resized - not sure why. Looked up hint in TGFrame::Resize(Int_t, Int_t)
    tabsWidget->SetWidth(Constants::leftPanelWidth);
    tabsWidget->Layout();

    AddFrame(tabsWidget, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY, dx, dx, 2*dy, 2*dy));

    // Right panel
    TGVerticalFrame* frameRightVertical = new TGVerticalFrame(this);

    // Attach Right Canvas (Plot)
    embedCanvas = new TRootEmbeddedCanvas("embedCanvas", frameRightVertical);
    embedCanvas->GetCanvas()->SetGridy();
    frameRightVertical->AddFrame(embedCanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY, 0, dx, dx, 0));

    // Attach bottom toolbar
    TGHorizontalFrame* bottomToolbarFrame = new TGHorizontalFrame(frameRightVertical);
    logScaleCheckButton = new TGCheckButton(bottomToolbarFrame, "Logarithmic scale");
    logScaleCheckButton->SetOn(kTRUE);
    bottomToolbarFrame->AddFrame(logScaleCheckButton, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 5*dx));

    toolbarFrame = new TGHorizontalFrame(bottomToolbarFrame);

//    numDisplayMin = new MyTGNumberEntry(toolbarFrame, 0, 6, -1, TGNumberFormat::kNESRealOne,
//            TGNumberFormat::kNEAAnyNumber,
//            TGNumberFormat::kNELLimitMinMax,
//            -9999, 9999);
//    numDisplayMax = new MyTGNumberEntry(toolbarFrame, 0, 6, -1, TGNumberFormat::kNESRealOne,
//            TGNumberFormat::kNEAAnyNumber,
//            TGNumberFormat::kNELLimitMinMax,
//            -9999, 9999);
//    btnApplyZoom = new TGTextButton(toolbarFrame, "Apply Display Range");
//    btnApplyZoom->Connect("Clicked()", "SWCalculatorView", this, "onApplyZoomClicked()");
//    btnResetZoom = new TGTextButton(toolbarFrame, "Reset");
//    btnResetZoom->Connect("Clicked()", "SWCalculatorView", this, "onResetZoomClicked()");

//    toolbarFrame->AddFrame(numDisplayMin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom
//    toolbarFrame->AddFrame(new TGLabel(toolbarFrame, "-"), new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, dy, 0));  // left, right, top, bottom
//    toolbarFrame->AddFrame(numDisplayMax, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom
//    toolbarFrame->AddFrame(btnApplyZoom, new TGLayoutHints(kLHintsLeft | kLHintsTop, dx, dx, 0, 0));  // left, right, top, bottom
//    toolbarFrame->AddFrame(btnResetZoom, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom

    displayMin = new MyTGNumberEntry(toolbarFrame, 0, 6, UiHelper::getUId(), TGNumberFormat::kNESRealOne,
        TGNumberFormat::kNEAAnyNumber,
        TGNumberFormat::kNELLimitMinMax,
        -1000, 1000);
    toolbarFrame->AddFrame(displayMin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));

    zoomSlider = new TGDoubleHSlider(toolbarFrame, 190, kDoubleScaleBoth, UiHelper::getUId(), kHorizontalFrame, GetDefaultFrameBackground(), kFALSE, kFALSE);
    zoomSlider->SetRange(-60,60);
    zoomSlider->SetPosition(-60,60);
    toolbarFrame->AddFrame(zoomSlider, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, dx, dx, 0, 0));

    displayMax = new MyTGNumberEntry(toolbarFrame, 0, 6, UiHelper::getUId(), TGNumberFormat::kNESRealOne,
        TGNumberFormat::kNEAAnyNumber,
        TGNumberFormat::kNELLimitMinMax,
        -1000, 1000);
    toolbarFrame->AddFrame(displayMax, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 5*dx, 0, 0));
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
    btnSaveImage = new TGTextButton(toolbarFrame, "Save image");
    toolbarFrame->AddFrame(btnSaveImage, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, dx, 0, 0));

    // Save data file button
    btnSaveData = new TGTextButton(toolbarFrame, "Export as ASCII");
    toolbarFrame->AddFrame(btnSaveData, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom

    bottomToolbarFrame->AddFrame(toolbarFrame, new TGLayoutHints(kLHintsLeft | kLHintsCenterY | kLHintsExpandX, 0, 0, 0, 0));

    // Attach toolbar
    frameRightVertical->AddFrame(bottomToolbarFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 0, 0, dy*2));

    AddFrame(frameRightVertical, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, dx));
    // You should call, for example HideFrame(TGFrame *f), only after the frames have been laid out and the sub windows
    // of the composite frame have been mapped via method MapSubwindows()
    // RootHelper::hideFrame(resolutionFwhmFrame);

    // Disable toolbar on uiReady event
    Connect(this->GetMainFrame()->ClassName(), "uiReady()", this->ClassName(), this, "onUiReady()");
}

void SWCalculatorView::onUiReady(){
    setToolbarEnabled(kFALSE);
    UiHelper::hideFrame(logScaleCheckButton);
}

void SWCalculatorView::connectSignals(){
	numFitMin->Connect("ValueSet(Long_t)", "SWCalculatorPresenter", presenter, "onViewFitRangeSet()");
	numFitMax->Connect("ValueSet(Long_t)", "SWCalculatorPresenter", presenter, "onViewFitRangeSet()");
	numFitSlider->Connect("PositionChanged()", "SWCalculatorPresenter", presenter, "onViewFitSliderRangeSet()");
	// comboConvolutionType->Connect("Selected(Int_t)", "SWCalculatorPresenter", presenter, "onViewConvolutionSelected(Int_t)");
	convTypeButtonGroup->Connect("Pressed(Int_t)", "SWCalculatorPresenter", presenter, "onViewConvolutionSelected(Int_t)");
    bgTypeButtonGroup->Connect("Pressed(Int_t)", "SWCalculatorPresenter", presenter, "onViewSingleBgTypeSelected(Int_t)");

	hasParabola->Connect("Toggled(Bool_t)", "SWCalculatorPresenter", presenter, "onViewHasParabolaSet(Bool_t)");
    hasDelta->Connect("Toggled(Bool_t)", "SWCalculatorPresenter", presenter, "onViewHasDeltaSet(Bool_t)");
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

    // tabsWidget->Connect("Selected(Int_t)", presenter->Class_Name(), presenter, onViewFitTabActive);

	// Local signals within view
	// numFitMin->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorView", this, "onNumFitMinChanged()");
    // numFitMax->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorView", this, "onNumFitMaxChanged()");

    displayMin->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorView", this, "onDisplayMinChange(char*)");
    displayMax->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorView", this, "onDisplayMaxChange(char*)");
    zoomSlider->Connect("PositionChanged()", this->ClassName(), this, "onSliderChange()");

    logScaleCheckButton->Connect("Toggled(Bool_t)", this->ClassName(), this, "onLogScaleSet(Bool_t)");

    // Notifications to draw text on canvas (like status)
    Connect("AbstractImportSpectrumPresenter", "notifyUser(const char*)", this->ClassName(), this, "drawText(const char*)");
}

void SWCalculatorView::onLogScaleSet(Bool_t isLogScale){
    // Goal: always have 10% visual margin  above histogram
    if (isLogScale){
        gStyle->SetHistTopMargin(2);
    }
    else {
        gStyle->SetHistTopMargin(0.05); // 0.05 is default value
    }

    if (currentCanvasMode == CanvasMode::onePad){
        embedCanvas->GetCanvas()->SetLogy(isLogScale);
        updateStackVerticalLimits();
        embedCanvas->GetCanvas()->Modified();
        embedCanvas->GetCanvas()->Update();
    }
    else {
        TH1F* fitHist = presenter->getPdfProvider()->getFitHistogram();
        std::pair<Double_t, Double_t> yRange = presenter->estimateYAxisLimits(fitHist);
        embedCanvas->GetCanvas()->cd(1)->SetLogy(isLogScale);
        spectrumPlot->GetYaxis()->SetRangeUser(yRange.first, yRange.second);
        embedCanvas->GetCanvas()->cd(1)->Modified();
        embedCanvas->GetCanvas()->cd(1)->Update();
    }
}

void SWCalculatorView::setTabEnabled(Int_t tabNumber, Bool_t isEnabled){
    tabsWidget->SetEnabled(tabNumber, isEnabled);
}

void SWCalculatorView::drawFitResult(RooPlot* spectrumPlot, RooPlot* residualsPlot){
    this->spectrumPlot = spectrumPlot;
    this->residualsPlot = residualsPlot;

    // Set two pads mode
    setCanvasMode(CanvasMode::twoPads);

    // Draw data plot on canvas
    embedCanvas->GetCanvas()->cd(1);
    spectrumPlot->Draw();

    // Draw data plot on canvas
    embedCanvas->GetCanvas()->cd(2);
    residualsPlot->Draw();

    // Update canvas
    embedCanvas->GetCanvas()->cd(1)->Modified();
    embedCanvas->GetCanvas()->cd(1)->Update();
    embedCanvas->GetCanvas()->cd(2)->Modified();
    embedCanvas->GetCanvas()->cd(2)->Update();

    // view->initRooPlots(spectrumPlot, residualsPlot);
    setToolbarEnabled(kTRUE);
}

void SWCalculatorView::setFitRangeLimits(Int_t minBin, Int_t maxBin){
	// Set peak selection limits
	numFitMin->SetLimitValues((Double_t)minBin, (Double_t)maxBin);
	numFitMax->SetLimitValues((Double_t)minBin, (Double_t)maxBin);
	numFitSlider->SetRange((Double_t)minBin, (Double_t)maxBin);
    // numFitSlider->SetScale(scale)Range(min, max);
}

void SWCalculatorView::updateFitRange(Int_t minBin, Int_t maxBin){
    // Set peak selection initial values
	numFitMin->SetNumber(minBin);
	numFitMax->SetNumber(maxBin);
	numFitSlider->SetPosition(minBin, maxBin);

	// Update Canvas to reflect histogram range
    // Delete all primitives but keep subpads

//	TList* primitives = canvas->GetListOfPrimitives();
//    if (primitives){
//      TListIter next(primitives);
//      TObject *object;
//      while ((object=next())) {
//        if (object->InheritsFrom(THStack::Class())){
//          // Not deleting primitives but removing them from the list - safer.
//          // https://root.cern/root/roottalk/roottalk00/2082.html
//			THStack* hStack = (THStack*)object;
//			hStack->GetXaxis()->SetLimits(minEnergy, maxEnergy);
//        }
//      }
//    }

	// Update X axis range
	TH1* hist = presenter->getModelFitProperties().hist;
	Double_t minBinLoEdge = hist->GetXaxis()->GetBinLowEdge(minBin);
    Double_t maxBinHiEdge = hist->GetXaxis()->GetBinUpEdge(maxBin);
	tHStack->GetXaxis()->SetLimits(minBinLoEdge, maxBinHiEdge);

	// Upddate Y axis range
	updateStackVerticalLimits();

    embedCanvas->GetCanvas()->Modified();
    embedCanvas->GetCanvas()->Update();
}

void SWCalculatorView::updateStackVerticalLimits(){
    // Update Y axis range dynamically as we change the x range
    Double_t yMax = std::numeric_limits<double>::min();
    TList* histsList = tHStack->GetHists();
    TListIter next(histsList);
    TObject *object;
    while ((object=next())) {
        if (object->InheritsFrom(TH1::Class())){
            TH1* hist = (TH1*) object;
            FitProperties fitProperties = presenter->getModelFitProperties();
            Double_t histMaxInRange = HistProcessor::getHistMaximumInRange(hist, fitProperties.minFitBin, fitProperties.maxFitBin);
            yMax = TMath::Max(histMaxInRange, yMax);
        }
    }

//    FitProperties fitProperties = presenter->getModelFitProperties();
//    Double_t yMax = HistProcessor::getHistMaximumInRange(fitProperties.hist, fitProperties.minFitBin, fitProperties.maxFitBin);
//    if (fitProperties.sourceHist) yMax = TMath::Max(yMax, HistProcessor::getHistMaximumInRange(fitProperties.sourceHist, fitProperties.minFitBin, fitProperties.maxFitBin));

    // Set stack y range
    // Problem: was getting errors when adjusting Y axis of the THStack in log scale.
    // Error in <THistPainter::PaintInit>: Cannot set Y axis to log scale
    // Solution: https://root-forum.cern.ch/t/cannot-set-y-axis-to-log-scale/21502/7
    //           set histogram minimum to 1
    // tHStack->SetMinimum(1);

    // Similar to TStyle::SetHistogramTopMargin()
    Double_t topMargin = Constants::histogramTopMargin;
    tHStack->SetMaximum(logScaleCheckButton->IsOn() ? TMath::Power(10, TMath::Log10(yMax)*(1+topMargin)) : yMax*(1+topMargin));
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

//void SWCalculatorView::initRooPlots(RooPlot* fitFrame, RooPlot* chiFrame) {
//    this->fitFrame = fitFrame;
//    this->chiFrame = chiFrame;
//}

void SWCalculatorView::setToolbarEnabled(Bool_t isEnabled){
    isEnabled ? UiHelper::showFrame(toolbarFrame) : UiHelper::hideFrame(toolbarFrame);
//    btnSaveData->SetEnabled(isEnabled);
//    btnSaveImage->SetEnabled(isEnabled);
//    displayMin->SetState(isEnabled);
//    displayMax->SetState(isEnabled);
//    isEnabled ? UiHelper::showFrame(zoomSlider) : UiHelper::hideFrame(zoomSlider);
}

TCanvas* SWCalculatorView::getCanvas() {
    return embedCanvas->GetCanvas();
}

void SWCalculatorView::reflectTwoDetector(Bool_t isTwoDetector){
    // Update S and W label ranges
    const char* text = isTwoDetector ? "2 x" : "";
    lblRescale1->SetText(text);
    lblRescale2->SetText(text);
    lblRescale3->SetText(text);

    // Show or hide single detector background panel
    isTwoDetector ? UiHelper::hideFrame(singleDetectorBackgroundFrame) : UiHelper::showFrame(singleDetectorBackgroundFrame);
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

void SWCalculatorView::drawText(const char* heading, const char* text){
    // Ensure single pad mode
    setCanvasMode(CanvasMode::onePad);

    // Clear the canvas
    TCanvas* canvas = embedCanvas->GetCanvas();
    canvas->Clear();

    // Draw heading
	TText* t = new TText(0.5, text==0 ? 0.5 : 0.55, heading);
    t->SetTextAlign(ETextAlign::kHAlignCenter + ETextAlign::kVAlignCenter);
    t->SetNDC();
	t->SetTextFont(GraphicsHelper::getFont(EFontFace::Helvetica));
    t->SetTextSize(GraphicsHelper::TEXT_SIZE_HUGE);
    t->SetTextColor(EColor::kGray);\
	t->Draw();

	// Draw caption
	if (text){
	    TText* t2 = new TText(0.5, 0.45, text);
	    t2->SetTextAlign(ETextAlign::kHAlignCenter + ETextAlign::kVAlignCenter);
	    t2->SetNDC();
	    t2->SetTextFont(GraphicsHelper::getFont(EFontFace::Helvetica));
	    t2->SetTextSize(GraphicsHelper::TEXT_SIZE_NORMAL);
	    t2->SetTextColor(EColor::kBlack);
	    t2->Draw();
	}

	canvas->Modified();
	canvas->Update();
}

void SWCalculatorView::setCanvasMode(CanvasMode mode){
	if (currentCanvasMode == mode) return;
	TCanvas* canvas = embedCanvas->GetCanvas();

	// Clear current canvas
	canvas->Clear();

	if (mode == CanvasMode::onePad){
		// Initialize one pad on canvas (for setting limits)
//	    canvas->SetMargin((GraphicsHelper::padMargins).left,
//	    		(GraphicsHelper::padMargins).right, (GraphicsHelper::padMargins).bottom, (GraphicsHelper::padMargins).top);
		canvas->SetRightMargin((GraphicsHelper::padMargins).right);
        canvas->SetTopMargin((GraphicsHelper::padMargins).top*3./4.); // weird that margins for rooplot and hist are different

        // TODO: create UI checkbox
        canvas->SetLogy(logScaleCheckButton->IsOn());
	}
	else {
	    // Initialize two pads on canvas (for fitting)
		canvas->Divide(1, 2);

	    canvas->cd(1)->SetPad("padData", "Pad for data", 0.0, 1/(1+GraphicsHelper::TOP_TO_BOTTOM_PAD_HEIGHT_RATIO), 1.0, 1.0, kWhite);
	    canvas->cd(1)->SetMargin((GraphicsHelper::padMargins).left, (GraphicsHelper::padMargins).right, (GraphicsHelper::padMargins).bottom, (GraphicsHelper::padMargins).top);
        canvas->cd(1)->SetLogy(logScaleCheckButton->IsOn());

	    canvas->cd(2)->SetPad("padChi2", "Pad for chi^2", 0.0, 0.0, 1.0, 1/(1+GraphicsHelper::TOP_TO_BOTTOM_PAD_HEIGHT_RATIO), kWhite);
	    canvas->cd(2)->SetMargin((GraphicsHelper::padMargins).left, (GraphicsHelper::padMargins).right, (GraphicsHelper::padMargins).bottom, (GraphicsHelper::padMargins).top);
	}
	currentCanvasMode = mode;

//    canvas->Modified();
//    canvas->Update();
}

void SWCalculatorView::setConvolutionType(ConvolutionType convolutionType){
	convTypeButtonGroup->SetButton(convolutionType);
}

void SWCalculatorView::setBackgroundType(BackgroundType backgroundType){
    bgTypeButtonGroup->SetButton(backgroundType);
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
    spectrumPlot->GetXaxis()->SetRangeUser(min, max);
    residualsPlot->GetXaxis()->SetRangeUser(min, max);

    TCanvas* c = embedCanvas->GetCanvas();
    c->cd(1)->Modified();
    c->cd(1)->Update();
    c->cd(2)->Modified();
    c->cd(2)->Update();
}

void SWCalculatorView::drawHistograms(TH1* hist, TH1* sourceHist){
    // IMPORTANT:
    // Histograms should not have any error values set because otherwise log canvas will try drawing bottom error bar below zero...
    // Initialize single-pad mode
    setCanvasMode(CanvasMode::onePad);

    TCanvas* canvas = embedCanvas->GetCanvas();
	canvas->cd();

	// If both histograms are empty - display welcome message
	if (hist == nullptr && sourceHist == nullptr){
	    drawText("No Spectra Loaded", "Click 'Open File...' on the left to import a spectrum");
	    return;
	}

	// Make stack of regular and source histograms (if set)
    // THStack* hStack = new THStack("hs", "");
    // hist->Print("base");

	// Clear histogram stack
	if (tHStack->GetHists()){
	    tHStack->GetHists()->RemoveAll();
	}

    // Add acquisitioned histogram to the stack
    TH1* histCopy = new TH1F();
    hist->Copy(*histCopy);
    histCopy->SetLineColor(kBlack);
    tHStack->Add(histCopy);

    // Disable title on the THStack. We draw our own title
    tHStack->SetTitle(" ");

    // Add source contribution histogram to the stack (if exists)
    if (sourceHist){
        sourceHist->Print("base");
        TH1* sourceHistCopy = (TH1*)sourceHist->Clone();
        sourceHistCopy->SetLineColor(EColor::kBlack);
        sourceHistCopy->SetLineStyle(ELineStyle::kDashed);
        // sourceHistCopy->SetMinimum(1);
        tHStack->Add(sourceHistCopy);
//        tHStack->SetTitle("");
    }

    // Draw THStack
    tHStack->Draw("nostack");

    // Set THStack axis titles - after Draw() only
    tHStack->GetXaxis()->SetTitle("Energy, keV");
    tHStack->GetXaxis()->CenterTitle();
    tHStack->GetYaxis()->SetTitle("Counts");
    tHStack->GetYaxis()->CenterTitle();

    updateStackVerticalLimits();

	canvas->BuildLegend();
	GraphicsHelper::alignPave(canvas, Alignment::TOP_RIGHT, Decoration::DEFAULT, 0.07, 0.35);

    // Draw "crosshair"
    Double_t x1 = canvas->GetLeftMargin();
    Double_t x2 = 1-canvas->GetRightMargin();
    Double_t y1 = canvas->GetBottomMargin();
    Double_t y2 = 1-canvas->GetTopMargin();

    TLine* line = new TLine();
	line->SetLineColor(kGray+1);
	line->SetLineStyle(ELineStyle::kDotted);
	line->DrawLineNDC((x1+x2)/2, y1, (x1+x2)/2, y2)->Draw();

    // Add title
//    TText* t = new TText(0.5, 0.95, sourceHist ? "Studied Material and Source Contribution Spectra" : "Studied Material Spectrum");
//    t->SetNDC();
//    t->SetTextFont(42);
//    t->SetTextSize(0.035);
//    t->SetTextAlign(ETextAlign::kHAlignCenter + ETextAlign::kVAlignCenter);
//    t->Draw();

    canvas->Modified();
    canvas->Update();

    setToolbarEnabled(kFALSE);
}

SWCalculatorView::~SWCalculatorView() {
    Cleanup();
}
