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
#include "RootHelper.h"
#include "../model/Constants.h"

#include <RooPlot.h>
#include <TAxis.h>
#include <TBox.h>
#include <TLatex.h>
#include <TMath.h>
#include <TLegend.h>
#include <TH1.h>
#include <TLine.h>
#include <TBox.h>
#include <TString.h>
#include <RooRealVar.h>
#include <TPaveStats.h>
#include <TCanvas.h>
#include <RooStringVar.h>
#include <RooFormulaVar.h>
// #include <RooFormula.h>
#include <iostream>

// for fonts just read this https://root.cern.ch/doc/master/classTAttText.html
// tip: don't use font sizes in pixels; they are not relative to the canvas size

//ClassImp(GraphicsHelper);

const Double_t GraphicsHelper::TEXT_SIZE_NORMAL = 0.03;
const Double_t GraphicsHelper::TEXT_SIZE_SMALL = TEXT_SIZE_NORMAL*3./4.;
const Double_t GraphicsHelper::TEXT_SIZE_SMALLER = TEXT_SIZE_SMALL*3./4.;
const Double_t GraphicsHelper::TEXT_SIZE_LARGE = TEXT_SIZE_NORMAL*4./3.;
const Double_t GraphicsHelper::TEXT_SIZE_LARGER = TEXT_SIZE_LARGE*4./3.;
const Double_t GraphicsHelper::TEXT_SIZE_HUGE = TEXT_SIZE_LARGER*4./3.;

const Double_t GraphicsHelper::LINE_HEIGHT_NORMAL = 0.045;
const Double_t GraphicsHelper::LINE_HEIGHT_SMALL = LINE_HEIGHT_NORMAL*0.75;

const Double_t GraphicsHelper::DEFAULT_X_TITLE_OFFSET = 2.5;
const Double_t GraphicsHelper::DEFAULT_X_LABEL_OFFSET = 0.01;
const Double_t GraphicsHelper::DEFAULT_Y_TITLE_OFFSET = 2.1;
const Double_t GraphicsHelper::DEFAULT_Y_LABEL_OFFSET = 0.02;
const Double_t GraphicsHelper::LEGEND_X1 = 0.65;

const Double_t GraphicsHelper::TOP_TO_BOTTOM_PAD_HEIGHT_RATIO = 2.5;

const Margin GraphicsHelper::padMargins = { 0.10, 0.02, 0.20, 0.035 }; // left, right, bottom, top

const Int_t GraphicsHelper::colorSet[7] = { kViolet + 6, kSpring - 5, kAzure + 8,
		kPink + 1, kGray + 1, kViolet - 4, kRed - 7 };

Color_t GraphicsHelper::histColor = kAzure+6;
Color_t GraphicsHelper::sourceHistColor = kRed-7;

TColor* GraphicsHelper::colorPrimary = new TColor(0.232, 0.757, 0.832);
TColor* GraphicsHelper::colorWarning = new TColor(0.832, 0.769, 0.232);
TColor* GraphicsHelper::colorGray = new TColor(0.5, 0.5, 0.5);
TColor* GraphicsHelper::colorAppWindow = new TColor(0.906, 0.906, 0.906);

// Private Singleton Members
GraphicsHelper::GraphicsHelper(){};

GraphicsHelper::~GraphicsHelper() {
  delete instance;
}

GraphicsHelper* GraphicsHelper::instance = NULL;

GraphicsHelper* GraphicsHelper::getInstance(){
    if (!instance){
        instance = new GraphicsHelper;
    }
    return instance;
}

Style_t GraphicsHelper::getFont(EFontFace fontFace){
    // https://root.cern.ch/doc/master/classTAttText.html#autotoc_md31
	const Int_t precision = 2;  // scalable and rotatable fonts (if "1" then problems rendering, "3" - non scalable fonts look bad on big screens)
	return (Int_t)fontFace * 10 + precision;
}

Double_t GraphicsHelper::getFontSizeScale(Bool_t isTopPad){
	if (isTopPad){
		Double_t scale = 1/(TOP_TO_BOTTOM_PAD_HEIGHT_RATIO/(TOP_TO_BOTTOM_PAD_HEIGHT_RATIO+1));
		return scale;
	}
	Double_t scale = 1/(1/(TOP_TO_BOTTOM_PAD_HEIGHT_RATIO+1));
	return scale;
}

void GraphicsHelper::styleAxis(TAxis* axis, const char* title, Double_t titleOffset, Double_t labelOffset, Bool_t isTopPad){
    Style_t fontCode = getFont();

    axis->SetLabelFont(fontCode);
    axis->SetLabelOffset(labelOffset);
    axis->SetLabelSize(GraphicsHelper::TEXT_SIZE_SMALL*getFontSizeScale(isTopPad));

    axis->SetTitleFont(fontCode);
    axis->SetTitleOffset(titleOffset);
    axis->SetTitleSize(GraphicsHelper::TEXT_SIZE_NORMAL*getFontSizeScale(isTopPad));

    axis->CenterTitle(kTRUE);
    axis->SetTitle(title);
}

void GraphicsHelper::drawRegion(RooPlot* plot, Double_t x1, Double_t y1, Double_t x2, Double_t y2, const char* text, Double_t textY){
    TBox* box = new TBox(x1, y1, x2, y2);
    box->SetLineWidth(0);
    box->SetFillStyle(EFillStyle::kFDotted3);
    box->SetFillColor(EColor::kGray);
    plot->addObject(box);

    TLatex* latex = new TLatex((x1+x2)/2, textY, text);
    latex->SetTextAlign(ETextAlign::kHAlignCenter);
    latex->SetTextColor(kGray+1);
    latex->SetTextSize(TEXT_SIZE_NORMAL);
    plot->addObject(latex);
}

void GraphicsHelper::drawSWRegions(RooPlot* frame, Double_t sWidth, Double_t wWidth, Double_t wShift, Double_t mean, Double_t yMin, Double_t yMax, Bool_t isTwoDetector) {
    sWidth = isTwoDetector ? sWidth * 2 : sWidth;
    wWidth = isTwoDetector ? wWidth * 2 : wWidth;
    wShift = isTwoDetector ? wShift * 2 : wShift;

    // yMax = 0.95*yMax;
//    yMin = frame->GetYaxis()->GetXmin();
//    yMax = frame->GetYaxis()->GetXmax();

    Double_t logYMin = TMath::Log10(yMin);
    Double_t logYMax = TMath::Log10(yMax);  
    Double_t heightCoeff = isTwoDetector ? 0.25 : 0.65;
    Double_t letterBottomShift = logYMin + (logYMax - logYMin)*heightCoeff;
    Double_t letterY = TMath::Power(10, letterBottomShift);
    
    // Draw S box
    std::cout << "S integration range (" << mean - sWidth / 2 << ", " << mean + sWidth / 2. << ")" << std::endl;
    drawRegion(frame, mean - sWidth/2, yMin, mean + sWidth/2, yMax, "#it{S}", letterY);

    // Draw W1 box
    std::cout << "W1 integration region (" << mean - wShift - wWidth << ", " << mean - wShift << ")" << std::endl;
    drawRegion(frame, mean - wShift - wWidth, yMin, mean - wShift, yMax, "#it{W#scale[0.75]{1}}", letterY);

    // Draw W2 box
    std::cout << "W2 integration region (" << mean + wShift << ", " << mean + wShift + wWidth << ")" << std::endl;
    drawRegion(frame, mean + wShift, yMin, mean + wShift + wWidth, yMax, "#it{W#scale[0.75]{2}}", letterY);
}

void GraphicsHelper::printVariable(const char* options, Int_t& currentLine, RooAbsArg* rooAbsArg, TPaveText* box) {
	// Don't print hidden variables
	if (rooAbsArg->getAttribute(Constants::ATTR_HIDE_PARAMETER_FROM_UI)) return;

	if (RooRealVar* var = dynamic_cast<RooRealVar*>(rooAbsArg)){
		Int_t sigDigits = RootHelper::getSigDigits(var);
		TString* formatted = var->format(sigDigits, options);
		const char* formattedString = formatted->Data();

		Bool_t isConstant = var->isConstant();
		Bool_t atMaximum = var->getVal() + var->getError() > var->getMax();
		Bool_t atMinimum = var->getVal() - var->getError() < var->getMin();

		#ifdef USEDEBUG
			std::cout << "GraphicsHelper::printVariable" << std::endl;
			std::cout << var->GetName() << "; constant " << isConstant << "; atMinimum " << atMinimum << "; atMaximum " << atMaximum << std::endl;
		#endif

		if (!isConstant && (atMaximum || atMinimum)){
			TText* t = box->AddText(TString::Format("%s (at limit)", formattedString).Data());
			t->SetTextColor(kOrange+8);
//			t->SetTextSize(GraphicsHelper::TEXT_SIZE_SMALL);
		} else {
			TText* t = box->AddText(formattedString);
//			t->SetTextSize(GraphicsHelper::TEXT_SIZE_SMALL);
		}
		// Mark variable as printed
		currentLine++;
	}
	else if (RooFormulaVar* var = dynamic_cast<RooFormulaVar*>(rooAbsArg)){
		#ifdef USEDEBUG
			std::cout << "GraphicsHelper::printVariable" << std::endl;
			std::cout << var->GetName() << std::endl;
		#endif
		TString fmt("%s = %.#f (indirect)");
		Int_t sigDigits = RootHelper::getSigDigits(var);
		fmt.ReplaceAll("#", TString::Format("%d", sigDigits).Data());
		TString formatted = TString::Format(fmt.Data(), var->GetName(), var->evaluate());
		const char* formattedString = formatted.Data();
		TText* t = box->AddText(formattedString);
		t->SetTextColor(kGray+1);
		currentLine++;
	}
}

TPaveText* GraphicsHelper::makeParametersPaveText(const RooArgList& params, Double_t xmin, Double_t xmax, Double_t ymax){
	params.Print("V");

	Bool_t showConstants = kTRUE;
	const char* options = "NEULP";
	TIterator* pIter = params.createIterator();

	// Calculate bottom Legend coordinate (ymin)
	Double_t ymin = ymax;
	while (RooAbsArg* var = (RooAbsArg*) pIter->Next()) {
		if (showConstants || !var->isConstant()) ymin -= LINE_HEIGHT_SMALL;
	}
	ymin -= 2*LINE_HEIGHT_SMALL;
	ymin = TMath::Max(ymin, padMargins.bottom);

	// Create the box and set its options
	TPaveText *box = new TPaveText(xmin, ymax, xmax, ymin, "BRNDC"); //
	box->SetName("myParamBox");
	box->SetFillColor(EColor::kBlack);
	box->SetBorderSize(1);
//	box->SetTextFont(getFontCode(FONT_REGULAR));
	box->SetTextAlign(ETextAlign::kHAlignLeft + ETextAlign::kVAlignCenter);
	box->SetFillStyle(EFillStyle::kFDotted1);
	box->SetFillColor(EColor::kWhite);

	// Line counter (to draw horizontal lines later)
	Int_t linesNumber = 0;
	std::vector<int> hrLineNumbers;

	// Empty first line (for better padding)
	 box->AddText("");
	 linesNumber++;

	// Add horizontal rule
	// hrLineNumbers.push_back(linesNumber);
	// box->AddText("");
	// linesNumber++;

	// Print other variables (model)
	pIter = params.createIterator();
	while (TObject* obj = pIter->Next()) {
		if (RooAbsArg* var = dynamic_cast<RooAbsArg*>(obj)){
			if (RooStringVar* stringVar = dynamic_cast<RooStringVar*>(var)){
				hrLineNumbers.push_back(linesNumber);
				box->AddText(stringVar->getVal());
				linesNumber++;
			} else {
				printVariable(options, linesNumber, var, box);
			}
		}
	}

	// Empty last line (for better padding)
	 box->AddText("");
	 linesNumber++;

	// Draw horizontal rules
	for (std::vector<int>::iterator it = hrLineNumbers.begin(); it != hrLineNumbers.end(); ++it){
		Double_t y = ((Double_t)*it + 0.5)/(Double_t)linesNumber;
		box->AddLine(0, 1-y, 1, 1-y);
	}

	delete pIter;
	return box;
}

Int_t GraphicsHelper::getNumberOfLines(TPave* pave){
	if(TPaveText* paveText = dynamic_cast<TPaveText*>(pave)) {
		return paveText->GetListOfLines()->GetSize();
	}
	else if (TLegend* legend = dynamic_cast<TLegend*>(pave)){
		return legend->GetNRows();
	}
	return 1;
}

TLegend* GraphicsHelper::getPadLegend(TVirtualPad* pad){
  // ROOT creates legend always with name "TPave" - thats how we find it on the pad
  TLegend* legend = (TLegend*)pad->FindObject("TPave");
  return legend;
}

TObject* GraphicsHelper::findObjectOnPad(TClass *c, TVirtualPad *pad) {
  for (TObject *object : *(pad->GetListOfPrimitives())) {
    if (object->InheritsFrom(c))
      return object;
  }
  return NULL;
}

TPaveStats* GraphicsHelper::getPadStats(TVirtualPad* pad){
	  // Update Pad - in case the histogram was just drawn - need to update otherwise no primitives
	  if (pad->IsModified()) {
	    pad->Update();
	  }

	  // SCENARIO 1: Statistics box is produced by histogram. See line THistPainter.cxx:8628 "stats->SetParent(fH);"
	  // https://root.cern.ch/doc/master/classTPaveStats.html#autotoc_md223
	  // TObject* o = pad->GetPrimitive("stats");
	  // TPaveStats* ps = (TPaveStats*)o;
	  // TObject* par = ps->GetParent();
	  if (pad->GetPrimitive("stats") && ((TPaveStats*)(pad->GetPrimitive("stats")))->GetParent() && ((TPaveStats*)(pad->GetPrimitive("stats")))->GetParent()->InheritsFrom("TH1")) {
	    TClass * th1_class = TClass::GetClass("TH1");
	    TH1 *hist = (TH1*) findObjectOnPad(th1_class, pad);
	    TPaveStats *pave = (TPaveStats*) (pad->GetPrimitive("stats"));
	    pave->SetName("mystats");                     // rename to "mystats"
	    hist->GetListOfFunctions()->Remove(pave);    // disconnect from the graph
	    hist->SetStats(kFALSE); // disconnect from the histogram.
	    pad->GetListOfPrimitives()->Add(pave);        // attach to pad primitives
	    pave->SetParent(pad);
	    return pave;
	  }

	  // SCENARIO 3: Return stats box previously detached
	  if (pad->GetPrimitive("mystats") != NULL) {
	    TPaveStats *pave = (TPaveStats*) pad->GetPrimitive("mystats");
	    return pave;
	  }

	  // Return random stats found
	  TPaveStats *pave = (TPaveStats*) findObjectOnPad(TClass::GetClass("TPaveStats"), pad);
	  if (pave != NULL)
	    return pave;

	  return NULL;
	}
void GraphicsHelper::alignPave(TVirtualPad* pad, Alignment alignment, Decoration decoration, Double_t statsLineHeight, Double_t statsWidth){
	// Try finding Legend first
	TPave* pave = getPadLegend(pad);

	// Next try finding pad
	if (!pave){
		pave = getPadStats(pad);
	}

	if (!pave){
		std::cout << "alignPave: could not find neither legend nor pave on canvas" << std::endl;
		return;
	}
	alignPave(pave, pad, alignment, decoration, statsLineHeight, statsWidth);
}


void GraphicsHelper::alignPave(TPave* pave, TVirtualPad* pad, Alignment alignment, Decoration decoration, Double_t statsLineHeight, Double_t statsWidth){
  // Update
  pad->Modified();
  pad->Update();

  // Legend height needs recalculation
  // it depends on the number of hte lines in the legend
  // std::cout << "Number of lines: " << getNumberOfLines(pave) << std::endl;
  Double_t legendHeight = statsLineHeight*getNumberOfLines(pave); // stats->GetY2NDC() - stats->GetY1NDC();
  Double_t legendWidth = statsWidth == 0 ? (pave->GetX2NDC() - pave->GetX1NDC()): statsWidth;
  // std::cout << legendWidth << "x" << legendHeight << std::endl;

  Double_t dx = (decoration == Decoration::TRANSPAREN) ? 0.04 : 0;
  Double_t dy = (decoration == Decoration::TRANSPAREN) ? 0.04 : 0;
  if (alignment == Alignment::TOP_LEFT){
    pave->SetX1NDC(pad->GetLeftMargin() + dx);
    pave->SetY1NDC(1 - pad->GetTopMargin() - legendHeight - dy);
    pave->SetX2NDC(pad->GetLeftMargin() + legendWidth + dx);
    pave->SetY2NDC(1 - pad->GetTopMargin() - dy);
  }
  else if (alignment == Alignment::BOTTOM_RIGHT){
    pave->SetX1NDC(1 - pad->GetRightMargin() - legendWidth);
    pave->SetY1NDC(pad->GetBottomMargin() + 2*dy);
    pave->SetX2NDC(1 - pad->GetRightMargin());
    pave->SetY2NDC(pad->GetBottomMargin() + 2*dy + legendHeight);
  }
  else if (alignment == Alignment::TOP_RIGHT){
    pave->SetX1NDC(1 - pad->GetRightMargin() - legendWidth - dx);
    pave->SetY1NDC(1 - pad->GetTopMargin() - legendHeight - dy);
    pave->SetX2NDC(1 - pad->GetRightMargin() - dx);
    pave->SetY2NDC(1 - pad->GetTopMargin() - dy);
  }

  if (decoration == Decoration::TRANSPAREN){
    pave->SetLineWidth(0);
    pave->SetFillStyle(0);
  }
  pad->Modified();
  pad->Update();
}

void GraphicsHelper::alignStats(TVirtualPad* pad, Alignment alignment, Decoration decoration, Double_t statsLineHeight, Double_t statsWidth){
  // Find stats box on canvas
  TPave* stats = getPadStats(pad);
  alignPave(stats, pad, alignment, decoration, statsLineHeight, statsWidth);
}

void GraphicsHelper::drawXCanvas(TRootEmbeddedCanvas* embedCanvas){
	TCanvas* canvas = embedCanvas->GetCanvas();
	canvas->cd();

	// Create main ine object for drawing
	TLine* line = new TLine();
	line->SetLineColor(kGray+1);

	Double_t ww = embedCanvas->GetWidth();
	Double_t wh = embedCanvas->GetHeight();

	Double_t hPx = 1.0/ww; // NDC one pixel in horizontal length
	Double_t vPx = 1.0/wh; // NDC one pixel in vertical length

	line->DrawLineNDC(0, vPx, 1-hPx, vPx)->Draw();
	line->DrawLineNDC(1-hPx, vPx, 1-hPx, 1-vPx)->Draw();
	line->DrawLineNDC(1-hPx, 1-vPx, 0, 1-vPx)->Draw();
	line->DrawLineNDC(0, 1-vPx, 0, vPx)->Draw();

	line->DrawLineNDC(0, vPx, 1-hPx, 1-vPx)->Draw();
	line->DrawLineNDC(0, 1-vPx, 1-hPx, vPx)->Draw();
}
