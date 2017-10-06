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

class GraphicsHelper {
	public:
		static const int DEFAULT_FONT_SIZE = 14;  // px
		static const int DEFAULT_FONT_NUMBER = 6; // Sans Serif
		static constexpr double DEFAULT_X_TITLE_OFFSET = 2.5;
		static constexpr double DEFAULT_X_LABEL_OFFSET = 0.01;
		static constexpr double DEFAULT_Y_TITLE_OFFSET = 2.1;
		static constexpr double DEFAULT_Y_LABEL_OFFSET = 0.02;

		static GraphicsHelper* getInstance();

		Style_t getFontCode(Int_t fontSize);
		void setDefaultAxisFonts(TAxis* axis);
		void setupAxis(TAxis* axis, const char* title = "",
		               Double_t titleOffset = DEFAULT_X_TITLE_OFFSET,
		               Double_t labelOffset = DEFAULT_X_LABEL_OFFSET);
		void setupPlotAxis(RooPlot* plot, const char* xTitle = "",
		                   Double_t xTitleOffset = DEFAULT_X_TITLE_OFFSET,
		                   Double_t xLabelOffset = DEFAULT_X_LABEL_OFFSET,
		                   const char* yTitle = "",
		                   Double_t yTitleOffset = DEFAULT_Y_TITLE_OFFSET,
		                   Double_t yLabelOffset = DEFAULT_Y_LABEL_OFFSET);

	private:
		GraphicsHelper();
		static GraphicsHelper* instance;
};

#endif /* GRAPHICSHELPER_H */

