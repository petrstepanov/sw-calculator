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
#include "../../SWCalculatorLinkDef.h"
#include "../../model/Constants.h"
#include "../importSpectrumWidget/ImportSpectrumView.h"
#include "../importSpectrumWidget/ImportSourceSpectrumView.h"
#include <TGFrame.h>
#include <map>
//#include <algorithm>  //min
//#include <functional>
//#include <sstream>
#include <TRootEmbeddedCanvas.h>
//#include <TBox.h>
//#include <RooDataHist.h>
//#include <RooFitResult.h>
//#include <RooChi2Var.h>
//#include <RooMinuit.h>
//#include <RooMinimizer.h>
//#include <RooAddPdf.h>
//#include <TGFileDialog.h>
//#include <TGMsgBox.h>
#include <TG3DLine.h>
//#include <TStopwatch.h>
//#include "../../util/GraphicsHelper.h"
//#include "../../roofit/AbstractModelProvider.h"
//#include "../../roofit/ParabolaGaussModelProvider.h"
//#include "../../roofit/ParabolaLorentzianModelProvider.h"
#include "../../roofit/CompositeModelProvider.h"
//#include "../../util/HistProcessor.h"
//#include "../../util/StringUtils.h"
//#include "../../util/FileUtils.h"
//#include "../../util/Variable.h"
//#include "../../util/RootHelper.h"

using namespace RooFit;

//SWCalculatorView::SWCalculatorView(const TGWindow* w) : TGMainFrame(w, Constants::windowWidth, Constants::windowHeight){
SWCalculatorView::SWCalculatorView(const TGWindow* w) : AbstractView<SWCalculatorPresenter>(w){
    initUI();
}

SWCalculatorPresenter* SWCalculatorView::instantinatePresenter(){
    return new SWCalculatorPresenter(this);
}

void SWCalculatorView::initUI(){    
    SetLayoutManager(new TGHorizontalLayout(this));
    
    // Tabs widget
    tabsWidget = new TGTab(this, Constants::leftPanelWidth);

    // Import spectrum tab
//    TGCompositeFrame *tabImport = tabsWidget->AddTab("Material Spectrum");
//    tabImport->SetLayoutManager(new TGVerticalLayout(tabImport));
//    tabImport->AddFrame(new ImportSpectrumView(tabImport), new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, dy, dy));

    // Import Kapton spectrum tab
//    TGCompositeFrame *tabImportKapton = tabsWidget->AddTab("Kapton Spectrum");
//    tabImportKapton->SetLayoutManager(new TGVerticalLayout(tabImportKapton));
//    tabImportKapton->AddFrame(new ImportSourceSpectrumView(tabImportKapton), new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, dy, dy));
    
    // Fit Data tab
    TGCompositeFrame *tabFit = tabsWidget->AddTab("Fit Data");
    tabFit->SetLayoutManager(new TGVerticalLayout(tabFit));

    // Fit Range Row
    TGHorizontalFrame *frameFitRange = new TGHorizontalFrame(tabFit);
    numFitMin = new TGNumberEntry(frameFitRange, 485, 4, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEAAnyNumber,
            TGNumberFormat::kNELLimitMinMax,
            -9999, 9999);
    numFitMin->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorView", this, "onNumFitMinChanged()");

    numFitMax = new TGNumberEntry(frameFitRange, 537, 4, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEAAnyNumber,
            TGNumberFormat::kNELLimitMinMax,
            -9999, 9999);
    numFitMax->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorView", this, "onNumFitMaxChanged()");
    frameFitRange->AddFrame(new TGLabel(frameFitRange, "Fit Range, keV"), new TGLayoutHints(kLHintsNormal, 0, 0, dy, 0));
    frameFitRange->AddFrame(numFitMax, new TGLayoutHints(kLHintsRight));
    frameFitRange->AddFrame(new TGLabel(frameFitRange, "-"), new TGLayoutHints(kLHintsRight, dx, dx, dy, 0));
    frameFitRange->AddFrame(numFitMin, new TGLayoutHints(kLHintsRight));

    tabFit->AddFrame(frameFitRange, new TGLayoutHints(kLHintsExpandX, dx, dx, dy*2, dy));

    // Separator
    tabFit->AddFrame(new TGHorizontal3DLine(tabFit), new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Integration Parameters
    TGHorizontalFrame *frameSWidth = new TGHorizontalFrame(tabFit);
    lblRescale1 = new TGLabel(frameSWidth, "2 x ");
    numSWidth = new TGNumberEntry(frameSWidth, 1.60, 4, -1, TGNumberFormat::kNESRealTwo,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMin,
            0.01, 9.99);
    frameSWidth->AddFrame(new TGLabel(frameSWidth, "S Region Width"), new TGLayoutHints(kLHintsNormal, 0, 0, dy, 0));
    frameSWidth->AddFrame(numSWidth, new TGLayoutHints(kLHintsRight));
    frameSWidth->AddFrame(lblRescale1, new TGLayoutHints(kLHintsRight, 0, dx, 3*dy/5, 0));

    tabFit->AddFrame(frameSWidth, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    TGHorizontalFrame *frameWWidth = new TGHorizontalFrame(tabFit);
    lblRescale2 = new TGLabel(frameWWidth, "2 x ");
    numWWidth = new TGNumberEntry(frameWWidth, 3.00, 4, -1, TGNumberFormat::kNESRealTwo,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMin,
            0.01, 9.99);
    frameWWidth->AddFrame(new TGLabel(frameWWidth, "W Regions Width"), new TGLayoutHints(kLHintsNormal, 0, 0, dy, 0));
    frameWWidth->AddFrame(numWWidth, new TGLayoutHints(kLHintsRight));
    frameWWidth->AddFrame(lblRescale2, new TGLayoutHints(kLHintsRight, 0, dx, 3*dy/5, 0));

    tabFit->AddFrame(frameWWidth, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    TGHorizontalFrame *frameWShift = new TGHorizontalFrame(tabFit);
    lblRescale3 = new TGLabel(frameWShift, "2 x ");
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

    // Resolution Function Parameters
    TGHorizontalFrame* convolutionParamsFrame = new TGHorizontalFrame(tabFit);
    comboConvolutionType = new TGComboBox(convolutionParamsFrame, 0);
    std::map<Int_t, TString> convTypes = CompositeModelProvider::getConvolutionTypes();
    for (std::map<Int_t, TString>::iterator it=convTypes.begin(); it!=convTypes.end(); ++it){
        comboConvolutionType->AddEntry((it->second).Data(), it->first);
    }
    comboConvolutionType->Select(4);
    comboConvolutionType->Resize(75, 20);        
    checkboxResFixed = new TGCheckButton(convolutionParamsFrame, "fixed");
    checkboxResFixed->SetOn();              
    numResolutionFWHM = new TGNumberEntry(convolutionParamsFrame, 1.7, 3, -1, TGNumberFormat::kNESRealOne,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            0.5, 4.0);
    convolutionParamsFrame->AddFrame(new TGLabel(convolutionParamsFrame, "Convolution"), 
                                     new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 4*dy/5, 0));
    convolutionParamsFrame->AddFrame(comboConvolutionType, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, dy/5, 0));
    convolutionParamsFrame->AddFrame(checkboxResFixed, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 3*dy/5, 0));
    convolutionParamsFrame->AddFrame(numResolutionFWHM, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, dx, dy/5, 0));
    convolutionParamsFrame->AddFrame(new TGLabel(convolutionParamsFrame, "Resolution FWHM, keV"), 
                                     new TGLayoutHints(kLHintsRight | kLHintsTop, 0, dx, 4*dy/5, 0));
    tabFit->AddFrame(convolutionParamsFrame, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Separator
    tabFit->AddFrame(new TGHorizontal3DLine(tabFit), new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Model parameters
    TGHorizontalFrame* modelParamsFrame = new TGHorizontalFrame(tabFit);
    //	modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Model details:"), new TGLayoutHints(kLHintsNormal, 0, 30, 5, 0));
    checkboxHasParabola = new TGCheckButton(modelParamsFrame, "Parabola", -1);
    modelParamsFrame->AddFrame(checkboxHasParabola, new TGLayoutHints(kLHintsNormal, 0, 4*dx, 4*dy/5, 0));

    numGauss = new TGNumberEntry(modelParamsFrame, 1, 1, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            0, 5);
    modelParamsFrame->AddFrame(numGauss, new TGLayoutHints(kLHintsNormal, 0, dx, 2*dy/5, 0));        
    modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Gauss"), new TGLayoutHints(kLHintsNormal, 0, 3*dx, dy, 0));

    numExponent = new TGNumberEntry(modelParamsFrame, 1, 1, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            0, 55);
    modelParamsFrame->AddFrame(numExponent, new TGLayoutHints(kLHintsNormal, 0, dx, 2*dy/5, 0));
    modelParamsFrame->AddFrame(new TGLabel(modelParamsFrame, "Exp"), new TGLayoutHints(kLHintsNormal, 0, 3*dx, dy, 0));

    numDampExponent = new TGNumberEntry(modelParamsFrame, 1, 1, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            0, 5);
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
    tabFit->AddFrame(txtFitResult, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy, dy));

    // Attach Tabs Widget
    tabsWidget->SetTab(0);
//    tabsWidget->SetEnabled(2, false);
    tabsWidget->SetWidth(Constants::leftPanelWidth); // Resize(tabsWidget->GetDefaultSize());
    AddFrame(tabsWidget, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY, dx, dx, 2*dy, 2*dy));

    // Right panel
    TGVerticalFrame* frameRightVertical = new TGVerticalFrame(this);
    TGHorizontalFrame* frameExportButtons = new TGHorizontalFrame(frameRightVertical);

    numDisplayMin = new TGNumberEntry(frameExportButtons, 0, 6, -1, TGNumberFormat::kNESRealOne,
            TGNumberFormat::kNEAAnyNumber,
            TGNumberFormat::kNELLimitMinMax,
            -9999, 9999);
    numDisplayMax = new TGNumberEntry(frameExportButtons, 0, 6, -1, TGNumberFormat::kNESRealOne,
            TGNumberFormat::kNEAAnyNumber,
            TGNumberFormat::kNELLimitMinMax,
            -9999, 9999);
    btnApplyZoom = new TGTextButton(frameExportButtons, "Apply Display Range");
    btnApplyZoom->Connect("Clicked()", "SWCalculatorView", this, "onApplyZoomClicked()");
    btnResetZoom = new TGTextButton(frameExportButtons, "Reset");
    btnResetZoom->Connect("Clicked()", "SWCalculatorView", this, "onResetZoomClicked()");

    frameExportButtons->AddFrame(numDisplayMin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom
    frameExportButtons->AddFrame(new TGLabel(frameExportButtons, "-"), new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, dy, 0));  // left, right, top, bottom
    frameExportButtons->AddFrame(numDisplayMax, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom
    frameExportButtons->AddFrame(btnApplyZoom, new TGLayoutHints(kLHintsLeft | kLHintsTop, dx, dx, 0, 0));  // left, right, top, bottom
    frameExportButtons->AddFrame(btnResetZoom, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom

    // Save data file button
    btnSaveData = new TGTextButton(frameExportButtons, "Export Fit Data");
    btnSaveData->Connect("Clicked()", "SWCalculatorView", this, "onSaveDataClicked()");
    btnSaveData->SetEnabled(false);
    frameExportButtons->AddFrame(btnSaveData, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 0, 0));  // left, right, top, bottom

    // Save image button
    btnSaveImage = new TGTextButton(frameExportButtons, "Save Image");
    btnSaveImage->Connect("Clicked()", "SWCalculatorView", this, "onSaveImageClicked()");
    btnSaveImage->SetEnabled(false);
    frameExportButtons->AddFrame(btnSaveImage, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, dx, 0, 0));

    setToolbarEnabled(kFALSE);

    // Attach Right Canvas (Plot)
    TRootEmbeddedCanvas *embedPlot = new TRootEmbeddedCanvas("embedPlot", frameRightVertical);
    frameRightVertical->AddFrame(embedPlot, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY, 0, dx, dx, dx));
    canvasPlot = embedPlot->GetCanvas();

    frameRightVertical->AddFrame(frameExportButtons, new TGLayoutHints(kLHintsExpandX, 0, dx, 0, 0));

    AddFrame(frameRightVertical, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, dx));

    MapSubwindows();
//    Resize(GetDefaultSize());
    Resize(Constants::windowWidth, Constants::windowHeight);
    MapWindow();
    
    // Plot Canvas Settings
    padData = new TPad("padData", "Pad for data", 0.0, 0.3, 1.0, 1.0, kWhite); // x_low, y_low, x_hi, y_hi
    padData->SetMargin(Constants::padMargin[0], Constants::padMargin[1], Constants::padMargin[2], Constants::padMargin[3]);
    padData->Draw();
//
    padChi2 = new TPad("padChi2", "Pad for chi^2", 0.0, 0.0, 1.0, 0.3, kWhite);
    padChi2->SetMargin(Constants::padMargin[0], Constants::padMargin[1], Constants::padMargin[2], Constants::padMargin[3]);
    padChi2->Draw();
    
//    MapSubwindows();
//    Resize(GetDefaultSize());
//    Resize(Constants::windowWidth, Constants::windowHeight);
//    MapWindow();
}

// Calls from Presenter

Int_t SWCalculatorView::getFitMinValue(){
    return numFitMin->GetNumber();    
}

Int_t SWCalculatorView::getFitMaxValue(){
    return numFitMax->GetNumber();    
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
    return comboConvolutionType->GetSelected() - 1;
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

void SWCalculatorView::setToolbarEnabled(Bool_t isEnabled){
    btnApplyZoom->SetEnabled(isEnabled);
    btnResetZoom->SetEnabled(isEnabled);
    btnSaveImage->SetEnabled(isEnabled);
    btnSaveData->SetEnabled(isEnabled);
    numDisplayMin->SetState(isEnabled);
    numDisplayMax->SetState(isEnabled);
}

// Calls to Presenter
void SWCalculatorView::onNumFitMinChanged(){
    std::cout << "SWCalculatorView::onNumFitMinChanged()" << std::endl;
}

void SWCalculatorView::onNumFitMaxChanged(){
    std::cout << "SWCalculatorView::onNumFitMaxChanged()" << std::endl;    
}

void SWCalculatorView::onFitSpectrumClicked(){
    std::cout << "SWCalculatorView::onFitSpectrumClicked()" << std::endl;    
}

void SWCalculatorView::onApplyZoomClicked(){
    std::cout << "SWCalculatorView::onApplyZoomClicked()" << std::endl;    
}

void SWCalculatorView::onResetZoomClicked(){
    std::cout << "SWCalculatorView::onResetZoomClicked()" << std::endl;    
}

void SWCalculatorView::onSaveDataClicked(){
    std::cout << "SWCalculatorView::onSaveDataClicked()" << std::endl;        
}

void SWCalculatorView::onSaveImageClicked(){
    std::cout << "SWCalculatorView::onSaveImageClicked()" << std::endl;    
}

SWCalculatorView::~SWCalculatorView() {
//    Cleanup();
//    if(numPeakPosition){numPeakPosition->Delete(); delete numPeakPosition;}
//    if(numFitMin){numFitMin->Delete(); delete numFitMin;}
//    if(numFitMax){numFitMax->Delete(); delete numFitMax;}
//    if(lblRescale1){lblRescale1->Delete(); delete lblRescale1;}
//    if(lblRescale2){lblRescale2->Delete(); delete lblRescale2;}
//    if(lblRescale3){lblRescale3->Delete(); delete lblRescale3;}
//    if(numSWidth){numSWidth->Delete(); delete numSWidth;}
//    if(numWWidth){numWWidth->Delete(); delete numWWidth;}
//    if(numWShift){numWShift->Delete(); delete numWShift;}
//    if(comboConvolutionType){comboConvolutionType->Delete(); delete comboConvolutionType;}
//    if(numResolutionFWHM){numResolutionFWHM->Delete(); delete numResolutionFWHM;}
//    if(checkboxResFixed){checkboxResFixed->Delete(); delete checkboxResFixed;}
//    if(checkboxHasParabola){checkboxHasParabola->Delete(); delete checkboxHasParabola;}
//    if(numGauss){numGauss->Delete(); delete numGauss;}
//    if(numExponent){numExponent->Delete(); delete numExponent;}
//    if(numDampExponent){numDampExponent->Delete(); delete numDampExponent;}
//    if(btnFitSpectrum){btnFitSpectrum->Delete(); delete btnFitSpectrum;}
//    if(txtFitResult){txtFitResult->Delete(); delete txtFitResult;}
//    if(btnSaveData){btnSaveData->Delete(); delete btnSaveData;}
//    if(btnSaveImage){btnSaveImage->Delete(); delete btnSaveImage;}
//    if(canvasPlot){canvasPlot->Delete(); delete canvasPlot;}
//    if(numDisplayMin){numDisplayMin->Delete(); delete numDisplayMin;}
//    if(numDisplayMax){numDisplayMax->Delete(); delete numDisplayMax;}
//    if(btnApplyZoom){btnApplyZoom->Delete(); delete btnApplyZoom;}
//    if(btnResetZoom){btnResetZoom->Delete(); delete btnResetZoom;}
//    if(fitFrame){fitFrame->Delete(); delete fitFrame;}
//    if(chiFrame){chiFrame->Delete(); delete chiFrame;}
//    if(padData){padData->Delete(); delete padData;}
//    if(padChi2){padChi2->Delete(); delete padChi2;}
}