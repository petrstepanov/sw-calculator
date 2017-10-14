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
#include "../../SWCalculatorFrameLinkDef.h"
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
////#include <RQ_OBJECT.h>
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

SWCalculatorView::SWCalculatorView(TGWindow* w) : AbstractView<SWCalculatorPresenter>(w){
    initUI();    
}

SWCalculatorPresenter* SWCalculatorView::instantinatePresenter(){
    return new SWCalculatorPresenter(this);
}

void SWCalculatorView::initUI(){
    std::cout << "SWCalculatorView::initUI" << std::endl;

//    GetParent()->Print();

    // Tabs widget
    tabsWidget = new TGTab(this, Constants::leftPanelWidth);

    // Import spectrum tab
    TGCompositeFrame *tabImport = tabsWidget->AddTab("Material Spectrum");
    tabImport->SetLayoutManager(new TGVerticalLayout(tabImport));
    tabImport->AddFrame(new ImportSpectrumView(tabImport), new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy, dy));

    // Import Kapton spectrum tab
    TGCompositeFrame *tabImportKapton = tabsWidget->AddTab("Kapton Spectrum");
    tabImportKapton->SetLayoutManager(new TGVerticalLayout(tabImportKapton));
    tabImportKapton->AddFrame(new ImportSourceSpectrumView(tabImportKapton), new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy, dy));
    
    // Fit Data tab
    TGCompositeFrame *tabFit = tabsWidget->AddTab("Fit Data");
    tabFit->SetLayoutManager(new TGVerticalLayout(tabFit));

    // Fit Range Row
    TGHorizontalFrame *frameFitRange = new TGHorizontalFrame(tabFit);
    numFitMin = new TGNumberEntry(frameFitRange, 485, 4, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEAAnyNumber,
            TGNumberFormat::kNELLimitMinMax,
            -9999, 9999);
    numFitMin->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorFrame",
            this, "onNumFitMinChanged()");

    numFitMax = new TGNumberEntry(frameFitRange, 537, 4, -1, TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEAAnyNumber,
            TGNumberFormat::kNELLimitMinMax,
            -9999, 9999);
    numFitMax->GetNumberEntry()->Connect("TextChanged(char*)", "SWCalculatorFrame",
            this, "onNumFitMaxChanged()");
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
                                     new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 4*dx/5, 0));
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

//    tabFit->AddFrame(modelParamsFrame, new TGLayoutHints(kLHintsExpandX, dx, dx, dy, dy));

    // Fit Button
//    TGHorizontalFrame* frameFitSpectrum = new TGHorizontalFrame(tabFit);
//    btnFitSpectrum = new TGTextButton(frameFitSpectrum, "Fit and plot");
//    btnFitSpectrum->Connect("Clicked()", "SWCalculatorFrame", this, "onFitClicked()");
//    frameFitSpectrum->AddFrame(btnFitSpectrum, new TGLayoutHints(kLHintsExpandX));
//    tabFit->AddFrame(frameFitSpectrum, new TGLayoutHints(kLHintsExpandX, dx, dx, dy + 3, dy));

    // Fit Result TextBox
//    txtFitResult = new TGTextEdit(tabFit);
//    tabFit->AddFrame(txtFitResult, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy, dy));

    // Attach Tabs Widget
//    tabsWidget->SetTab(0);
//    tabsWidget->SetEnabled(1, false);
    // tabsWidget->SetWidth(540);//Resize(tabsWidget->GetDefaultSize());
    this->AddFrame(tabsWidget, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, 2*dy, 2*dy));

    // Right panel
//    TGVerticalFrame* frameRightVertical = new TGVerticalFrame(GetParent());
//    TGHorizontalFrame* frameExportButtons = new TGHorizontalFrame(frameRightVertical);
//
//    numDisplayMin = new TGNumberEntry(frameExportButtons, 0, 6, -1, TGNumberFormat::kNESRealOne,
//            TGNumberFormat::kNEAAnyNumber,
//            TGNumberFormat::kNELLimitMinMax,
//            -9999, 9999);
//    TGLabel *lblDisplayDash = new TGLabel(frameExportButtons, "-");
//    numDisplayMax = new TGNumberEntry(frameExportButtons, 0, 6, -1, TGNumberFormat::kNESRealOne,
//            TGNumberFormat::kNEAAnyNumber,
//            TGNumberFormat::kNELLimitMinMax,
//            -9999, 9999);
//    btnApplyZoom = new TGTextButton(frameExportButtons, "Apply Display Range");
//    btnApplyZoom->Connect("Clicked()", "SWCalculatorFrame", this, "applyZoom()");
//    btnResetZoom = new TGTextButton(frameExportButtons, "Reset");
//    btnResetZoom->Connect("Clicked()", "SWCalculatorFrame", this, "resetZoom()");
//
//    frameExportButtons->AddFrame(numDisplayMin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom
//    frameExportButtons->AddFrame(lblDisplayDash, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, dy, 0));  // left, right, top, bottom
//    frameExportButtons->AddFrame(numDisplayMax, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom
//    frameExportButtons->AddFrame(btnApplyZoom, new TGLayoutHints(kLHintsLeft | kLHintsTop, dx, dx, 0, 0));  // left, right, top, bottom
//    frameExportButtons->AddFrame(btnResetZoom, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, dx, 0, 0));  // left, right, top, bottom
//
//    btnSaveData = new TGTextButton(frameExportButtons, "Export Fit Data");
//    btnSaveData->Connect("Clicked()", "SWCalculatorFrame", this, "saveData()");
//    btnSaveData->SetEnabled(false);
//    frameExportButtons->AddFrame(btnSaveData, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 0, 0));  // left, right, top, bottom
//
//    btnSaveImage = new TGTextButton(frameExportButtons, "Save Image");
//    btnSaveImage->Connect("Clicked()", "SWCalculatorFrame", this, "saveImage()");
//    btnSaveImage->SetEnabled(false);
//    frameExportButtons->AddFrame(btnSaveImage, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, dx, 0, 0));
//
//    setToolbarEnabled(kFALSE);

    // Attach Right Canvas (Plot)
//    TRootEmbeddedCanvas *embedPlot = new TRootEmbeddedCanvas("embedPlot", frameRightVertical, 600, 600);
//    frameRightVertical->AddFrame(embedPlot, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY, dx, dx, dx, dx));
//    canvasPlot = embedPlot->GetCanvas();
//
//    frameRightVertical->AddFrame(frameExportButtons, new TGLayoutHints(kLHintsExpandX, 0, dx, 0, 0));
//
//    this->GetParentComposite()->AddFrame(frameRightVertical, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, dx));

    // Plot Canvas Settings
//    padData = new TPad("padData", "Pad for data", 0.0, 0.3, 1.0, 1.0, kWhite); // x_low, y_low, x_hi, y_hi
//    padData->SetMargin(Constants::padMargin[0], Constants::padMargin[1], Constants::padMargin[2], Constants::padMargin[3]);
//    padData->Draw();
//
//    padChi2 = new TPad("padChi2", "Pad for chi^2", 0.0, 0.0, 1.0, 0.3, kWhite);
//    padChi2->SetMargin(Constants::padMargin[0], Constants::padMargin[1], Constants::padMargin[2], Constants::padMargin[3]);
//    padChi2->Draw();
}

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

SWCalculatorView::~SWCalculatorView() {
//	mainFrame->Cleanup();
    delete tabsWidget;
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
    delete btnSaveData;
    delete btnSaveImage;
    delete canvasPlot;
    delete numDisplayMin;
    delete numDisplayMax;
//  delete hasOrtho;
    delete btnApplyZoom;
    delete btnResetZoom;              
    delete fitFrame;
    delete chiFrame;
    delete padData;
    delete padChi2;
}