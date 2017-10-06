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

#include <RooPlot.h>
#include <TAxis.h>
#include "GraphicsHelper.h"

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

void GraphicsHelper::setupPlotAxis(RooPlot* frame, const char* xTitle, Double_t xTitleOffset, Double_t xLabelOffset, const char* yTitle, Double_t yTitleOffset, Double_t yLabelOffset){
	frame->SetTitle("");
	setupAxis(frame->GetXaxis(), xTitle, xTitleOffset, xLabelOffset);
	setupAxis(frame->GetYaxis(), yTitle, yTitleOffset, yLabelOffset);
}

