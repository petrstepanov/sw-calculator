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
#include <TVirtualPad.h>
#include <TPaveStats.h>
#include <TRootEmbeddedCanvas.h>

struct Margin {
	Double_t left;
	Double_t right;
	Double_t bottom;
	Double_t top;
};

enum class Alignment {
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};

enum class Decoration {
	DEFAULT,
	TRANSPAREN
};

// Info: https://root.cern.ch/doc/master/classTAttText.html
// Image: https://root.cern.ch/doc/master/pict1_TAttText_004.png

enum EFontFace {
    TimesItalic = 1,
    TimesBold = 2,
    TimesBoldItalic = 3,
    Helvetica = 4,
    HelveticaItalic = 5,
    HelveticaBold = 6,
    HelveticaBoldItalic = 7,
    Courier = 8,
    CourierItalic = 9,
    CourierBold = 10,
    CourierBoldItalic = 11,
    Symbol = 12,
    Times = 13,
    Wingdings = 14,
    SymbolItalic = 15
};

class GraphicsHelper {
public:

    static const Double_t TEXT_SIZE_HUGE;
    static const Double_t TEXT_SIZE_LARGER;
    static const Double_t TEXT_SIZE_LARGE;
    static const Double_t TEXT_SIZE_NORMAL;
	static const Double_t TEXT_SIZE_SMALL;
	static const Double_t TEXT_SIZE_SMALLER;

	static const Double_t LINE_HEIGHT_NORMAL;
	static const Double_t LINE_HEIGHT_SMALL;

	static const Double_t DEFAULT_X_TITLE_OFFSET;
	static const Double_t DEFAULT_X_LABEL_OFFSET;
	static const Double_t DEFAULT_Y_TITLE_OFFSET;
	static const Double_t DEFAULT_Y_LABEL_OFFSET;
	static const Double_t LEGEND_X1;

	static const Int_t FONT_REGULAR;
	static const Int_t FONT_BOLD;

    static const Margin padMargins;
    static const Int_t colorSet[7];

    static Color_t histColor;
    static Color_t sourceHistColor;

    static TColor* colorPrimary;
    static TColor* colorWarning;
    static TColor* colorGray;
    static TColor* colorAppWindow;

	static const Double_t TOP_TO_BOTTOM_PAD_HEIGHT_RATIO;

	static GraphicsHelper* getInstance();

	static Style_t getFont(EFontFace fontFace = EFontFace::Helvetica);
	static Double_t getFontSizeScale(Bool_t isTopPad);
	void styleAxis(TAxis* axis, const char* title, Double_t titleOffset, Double_t labelOffset, Bool_t isTopPad);
	void drawSWRegions(RooPlot* frame, Double_t sWidth, Double_t wWidth,
			Double_t wShift, Double_t mean, Double_t yMin, Double_t yMax,
			Bool_t isTwoDetector);
	static TPaveText* makeParametersPaveText(const RooArgList& params, Double_t xmin, Double_t xmax, Double_t ymax);

	static Int_t getNumberOfLines(TPave* pave);
	static TLegend* getPadLegend(TVirtualPad* pad);
	static TPaveStats* getPadStats(TVirtualPad* pad);
	static void alignPave(TPave* pave, TVirtualPad* pad, Alignment alignment = Alignment::TOP_RIGHT, Decoration decoration = Decoration::DEFAULT, Double_t statsLineHeight = 0.07, Double_t statsWidth = 0);
	static void alignPave(TVirtualPad* pad, Alignment alignment = Alignment::TOP_RIGHT, Decoration decoration = Decoration::DEFAULT, Double_t statsLineHeight = 0.07, Double_t statsWidth = 0);
	static void alignStats(TVirtualPad* pad, Alignment alignment = Alignment::TOP_RIGHT, Decoration decoration = Decoration::DEFAULT, Double_t statsLineHeight = 0.07, Double_t statsWidth = 0);
	static void drawXCanvas(TRootEmbeddedCanvas* c);

private:
	GraphicsHelper();
	static GraphicsHelper* instance;

	static void printVariable(const char* options, Int_t& currentLine, RooAbsArg* rooAbsArg, TPaveText* box);
    void drawRegion(RooPlot* plot, Double_t x1, Double_t y1, Double_t x2, Double_t y2, const char* text, Double_t textY);

};

#endif /* GRAPHICSHELPER_H */

