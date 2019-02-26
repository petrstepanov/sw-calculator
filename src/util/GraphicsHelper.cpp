/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GraphicsHelper.cpp
 * Author: petrstepanov
 * 
 * Created on August 1, 2017, 10:17 PM
 */

#include "GraphicsHelper.h"
#include <RooPlot.h>
#include <TAxis.h>
#include <TBox.h>
#include <TLatex.h>
#include <TMath.h>
#include <iostream>

const Int_t GraphicsHelper::DEFAULT_FONT_SIZE = 14;  // px
const Int_t GraphicsHelper::DEFAULT_FONT_NUMBER = 6; // Sans Serif
const Double_t GraphicsHelper::DEFAULT_X_TITLE_OFFSET = 2.5;
const Double_t GraphicsHelper::DEFAULT_X_LABEL_OFFSET = 0.01;
const Double_t GraphicsHelper::DEFAULT_Y_TITLE_OFFSET = 2.1;
const Double_t GraphicsHelper::DEFAULT_Y_LABEL_OFFSET = 0.02;
const Double_t GraphicsHelper::RESIDUALS_PAD_RELATIVE_HEIGHT = 0.35;
const Double_t GraphicsHelper::LEGEND_X1 = 0.72;
const Double_t GraphicsHelper::LEGEND_LINE_HEIGHT = 0.045;

const Margin GraphicsHelper::padMargins = { 0.10, 0.03, 0.15, 0.05 }; // left, right, bottom, top

const Int_t GraphicsHelper::colorSet[7] = { kViolet + 6, kSpring - 5, kAzure + 8,
		kPink + 1, kGray + 1, kViolet - 4, kRed - 7 };

TColor* GraphicsHelper::colorPrimary = new TColor(0.232, 0.757, 0.832);
TColor* GraphicsHelper::colorWarning = new TColor(0.832, 0.769, 0.232);
TColor* GraphicsHelper::colorGray = new TColor(0.5, 0.5, 0.5);
TColor* GraphicsHelper::colorAppWindow = new TColor(0.906, 0.906, 0.906);

// Private Singleton Members
GraphicsHelper::GraphicsHelper(){};

GraphicsHelper* GraphicsHelper::instance = NULL;

GraphicsHelper* GraphicsHelper::getInstance(){
    if (!instance){
        instance = new GraphicsHelper;
    }
    return instance;
}

Style_t GraphicsHelper::getFontCode(Int_t fontNumber){
    // https://root.cern.ch/doc/master/classTAttText.html#T5
    return fontNumber * 10 + 3;
}

void GraphicsHelper::setDefaultAxisFonts(TAxis *axis){
    // Font face
    Style_t fontCode = getFontCode(DEFAULT_FONT_NUMBER);

    // Label font
    axis->SetLabelFont(fontCode);
    axis->SetLabelSize(12); // px

    // Title font
    axis->SetTitleFont(fontCode);
    axis->SetTitleSize(16); // px
}

void GraphicsHelper::setupAxis(TAxis* axis, const char* title, Double_t titleOffset, Double_t labelOffset){
    setDefaultAxisFonts(axis);
    axis->CenterTitle(kTRUE);
    axis->SetTitle(title);
    axis->SetTitleOffset(titleOffset);
    axis->SetLabelOffset(labelOffset);
}

void GraphicsHelper::drawSWRegions(RooPlot* frame, Double_t sWidth, Double_t wWidth, Double_t wShift, Double_t mean, Double_t yMin, Double_t yMax, Bool_t isTwoDetector) {
    sWidth = isTwoDetector ? sWidth * 2 : sWidth;
    wWidth = isTwoDetector ? wWidth * 2 : wWidth;
    wShift = isTwoDetector ? wShift * 2 : wShift;

    yMax = 0.95*yMax;
    Double_t textSize = 0.05;
    Double_t logYMin = TMath::Log10(yMin);
    Double_t logYMax = TMath::Log10(yMax);  
    Double_t heightCoeff = isTwoDetector ? 0.25 : 0.75;
    Double_t letterBottomShift = logYMin + (logYMax - logYMin)*heightCoeff;
    Double_t letterY = TMath::Power(10, letterBottomShift);
    Double_t letterOpacity = 0.15;
    
    std::cout << "S integration range (" << mean - sWidth / 2 << ", " << mean + sWidth / 2. << ")" << std::endl;
    TBox* sBox = new TBox(mean - sWidth / 2, yMin, mean + sWidth / 2, yMax);
    sBox->SetLineWidth(0);
    sBox->SetFillColorAlpha(19, 0.6);
    frame->addObject(sBox);
    TLatex* s = new TLatex(mean, letterY, "#it{S}");
    s->SetTextAlign(22);    
    s->SetTextColorAlpha(1, letterOpacity);
    s->SetTextSize(textSize);    
    frame->addObject(s);

    std::cout << "W1 integration region (" << mean - wShift - wWidth << ", " << mean - wShift << ")" << std::endl;
    TBox* w1Box = new TBox(mean - wShift - wWidth, yMin, mean - wShift, yMax);
    w1Box->SetLineWidth(0);
    w1Box->SetFillColorAlpha(19, 0.6);
    frame->addObject(w1Box);
    TLatex* w1 = new TLatex(mean-wShift-wWidth/2, letterY, "#it{W1}");
    w1->SetTextAlign(22);
    w1->SetTextColorAlpha(1, letterOpacity);
    w1->SetTextSize(textSize);    
    frame->addObject(w1);
    
    std::cout << "W2 integration region (" << mean + wShift << ", " << mean + wShift + wWidth << ")" << std::endl;
    TBox* w2Box = new TBox(mean + wShift, yMin, mean + wShift + wWidth, yMax);
    w2Box->SetLineWidth(0);
    w2Box->SetFillColorAlpha(19, 0.6);
    frame->addObject(w2Box);
    TLatex* w2 = new TLatex(mean+wShift+wWidth/2, letterY, "#it{W2}");
    w2->SetTextAlign(22);
    w2->SetTextSize(textSize);
    w2->SetTextColorAlpha(1, letterOpacity);
//    w2->SetBBoxCenterX(mean+wShift+wWidth/2);
//    w2->SetBBoxCenterY(letterY);
    frame->addObject(w2);

}
