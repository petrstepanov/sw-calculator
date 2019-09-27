/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GraphicsHelper.h
 * Author: petrstepanov
 *
 * Created on August 1, 2017, 10:17 PM
 */

#ifndef GRAPHICSHELPER_H
#define GRAPHICSHELPER_H

#include <TStyle.h>
#include <RooPlot.h>
#include <TPaveText.h>
#include <RooArgList.h>

struct Margin {
	Double_t left;
	Double_t right;
	Double_t bottom;
	Double_t top;
};

class GraphicsHelper {
public:
	static const Int_t TEXT_SIZE_SMALL;
	static const Int_t TEXT_SIZE_NORMAL;
	static const Int_t DEFAULT_FONT_NUMBER;
	static const Double_t DEFAULT_X_TITLE_OFFSET;
	static const Double_t DEFAULT_X_LABEL_OFFSET;
	static const Double_t DEFAULT_Y_TITLE_OFFSET;
	static const Double_t DEFAULT_Y_LABEL_OFFSET;
	static const Double_t LEGEND_X1;
	static const Double_t LEGEND_LINE_HEIGHT;
	static const Double_t LEGEND_LINE_HEIGHT_DENSE;

    static const Margin padMargins;
    static const Int_t colorSet[7];

    static TColor* colorPrimary;
    static TColor* colorWarning;
    static TColor* colorGray;
    static TColor* colorAppWindow;

	static const Double_t RESIDUALS_PAD_RELATIVE_HEIGHT;

	static GraphicsHelper* getInstance();

	Style_t getFontCode(Int_t fontSize);
	void setDefaultAxisFonts(TAxis* axis);
	void setupAxis(TAxis* axis, const char* title = "", Double_t titleOffset =
			DEFAULT_X_TITLE_OFFSET, Double_t labelOffset =
			DEFAULT_X_LABEL_OFFSET);
	void drawSWRegions(RooPlot* frame, Double_t sWidth, Double_t wWidth,
			Double_t wShift, Double_t mean, Double_t yMin, Double_t yMax,
			Bool_t isTwoDetector);
	static TPaveText* makeParametersPaveText(const RooArgList& params, Double_t xmin, Double_t xmax, Double_t ymax);

private:
	GraphicsHelper();
	static GraphicsHelper* instance;

	static void printVariable(const char* options, Int_t& currentLine, RooAbsArg* rooAbsArg, TPaveText* box);
};

#endif /* GRAPHICSHELPER_H */

