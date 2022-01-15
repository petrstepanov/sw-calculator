/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   PdfProvider.cpp
 * Author: petrstepanov
 *
 * Created on August 15, 2017, 9:59 PM
 */

#include "PdfProvider.h"

#include "pdfs/ParabolaPdf.h"
#include "pdfs/GaussianPdf.h"
#include "pdfs/LorentzianPdf.h"
#include "pdfs/DampLorentzPdf.h"
#include "pdfs/OrthogonalPdf.h"
#include "pdfs/BackgroundPdf.h"
#include "../util/RootHelper.h"
#include "../roofit/AddPdf.h"
#include "../model/Constants.h"
#include "../util/HistProcessor.h"
#include "../util/StringUtils.h"
#include "../util/Debug.h"
#include <RooFormulaVar.h>
#include <RooPolynomial.h>
#include <RooGaussian.h>
#include <RooAddPdf.h>
#include <RooFFTConvPdf.h>
#include <RooNumConvPdf.h>
#include <RooGenericPdf.h>
#include <RooCachedPdf.h>
#include <RooDataHist.h>
#include <RooFit.h>
#include <TIterator.h>
#include <TMath.h>
#include <TH1F.h>

PdfProvider::PdfProvider(FitProperties fitProperties) : observable(0), mean(0), resolutionFunction(0), sourcePdf(0), fitHistogram(0), pdfInMaterial(0), pdfFinal(0), intSource(0){
	// Define variables
	this->fitProperties = fitProperties;

	// Cut original histogram with respect to the fitting limits
	HistProcessor* histProcessor = HistProcessor::getInstance();

	// Histograms can be acessed via gROOT->FindObject. So proper way to treat them is pass by reference?
	// Problem: if you pass histogram by value then you copy it but it still has the same ROOT name? Mess.
	// So we pass histograms by reference. But here cutHist() always creates new histogram! So we avoid changes in model.
	fitHistogram = histProcessor->cutHist("fitHistogram", fitProperties.hist, fitProperties.minFitBin, fitProperties.maxFitBin);
	fitHistogram->Print();

	// Initialize pdfs in material
	initObservableAndMean();
	initMaterialPdf(fitProperties.hasParabola, fitProperties.componentHist, fitProperties.numberOfGaussians, fitProperties.numberOfExponents, fitProperties.numberOfDampingExponents);

	// Add correspondent background
	if (HistProcessor::isTwoDetector(fitProperties.hist)) {
        initTwoDetectorBackground();
	} else {
        initSingleDetectorBackground();
	}

	// Convolute (or not) before adding source contribution histogram
	initConvolutedModel(fitProperties.convolutionType);

	// Add source contribution latest because it should not be convoluted
	initSourceContribution(fitProperties.sourceHist);
}

PdfProvider::~PdfProvider(){
}

void PdfProvider::initObservableAndMean(){
	// Define energy axis (observable)
    Double_t eMin = fitHistogram->GetXaxis()->GetXmin();
    Double_t eMax = fitHistogram->GetXaxis()->GetXmax();
	observable = new RooRealVar("observable", "Energy axis", eMin, eMax, "keV");

	// Set binning for different types of convolution
	observable->setBins(fitHistogram->GetNbinsX());

	// Set mean
	Double_t m = fitHistogram->GetBinCenter(fitHistogram->GetMaximumBin());
	Double_t mMin = fitHistogram->GetXaxis()->GetBinLowEdge(fitHistogram->GetMaximumBin()-2);
    Double_t mMax = fitHistogram->GetXaxis()->GetBinUpEdge(fitHistogram->GetMaximumBin()+2);
	mean = new RooRealVar("mean", "Spectrum peak position", m, mMin, mMax);
}

void PdfProvider::initMaterialPdf(Bool_t hasParabola, TH1F* componentHist, const Int_t numGauss, const Int_t numLorentz, const Int_t numLorentzSum) {
	RooArgList* pdfsInMaterial = new RooArgList();

	// Parabola PDF
	if (hasParabola) {
		RooRealVar* parabolaRoot = new RooRealVar("parabolaRoot", "Coefficient at -x^2 + r*2", 3.5, 1, 10); // 3.4579 = Al (11.7)
		ParabolaPdf* parabolaPdf = new ParabolaPdf("Parabola", "Parabola", *observable, *mean, *parabolaRoot);
		pdfsInMaterial->add(*parabolaPdf);
	}

	// Component Histogram PDF
	if (componentHist){
	    // Trim component histogram one bin wider on each end than the original spectrum
		HistProcessor* histProcessor = HistProcessor::getInstance();
		Int_t minBin = componentHist->GetXaxis()->FindBin(observable->getMin());
        Int_t maxBin = componentHist->GetXaxis()->FindBin(observable->getMax());
        // Do not use ranges anymore - complicates things on view side
		// componentHist->GetXaxis()->SetRangeUser(observable->getMin(), observable->getMax());
		TH1F* componentHistogram = histProcessor->cutHist("componentHistogram", componentHist, minBin, maxBin);
		componentHistogram = histProcessor->removeHistNegatives("componentHistogramNoNegatives", componentHistogram);

		RooDataHist* componentDataHist = new RooDataHist("componentDataHist", "Component Data Hist", RooArgList(*observable), componentHistogram);
		RooHistPdf* componentHistPdf = new RooHistPdf("componentHistPdf", "Component Hist PDF", RooArgSet(*observable), *componentDataHist, 1);
		pdfsInMaterial->add(*componentHistPdf);
	}

	Double_t aMin = 0.01;
	Double_t aMax = 50;

	// Gauss PDFs
	RooRealVar** gaussA = new RooRealVar*[numGauss];
	GaussianPdf** gauss = new GaussianPdf*[numGauss];
	for (int i = 0; i < numGauss; i++) {
		Double_t aVal = getDefaultAValue(aMin, aMax, i+1, numGauss);
		gaussA[i] = new RooRealVar(Form("gaussian%dA", i + 1), StringUtils::ordinal(i+1, "Gaussian w.f. parameter")->Data(), aVal, aMin, aMax, "Angstrom");
		gauss[i] = new GaussianPdf(Form("Gaussian%d", i + 1), StringUtils::ordinal(i+1, "Gauss")->Data(), *observable, *mean, *gaussA[i]);
		pdfsInMaterial->add(*gauss[i]);
	}

	// Lorentz PDFs
	RooRealVar** lorentzA = new RooRealVar*[numLorentz];
	LorentzianPdf** lorentz = new LorentzianPdf*[numLorentz];
	for (int i = 0; i < numLorentz; i++) {
		Double_t aVal = getDefaultAValue(aMin, aMax, i+1, numLorentz);
		lorentzA[i] = new RooRealVar(Form("lorentzian%dA", i + 1), StringUtils::ordinal(i+1, "Exponential w.f. parameter")->Data(), aVal, aMin, aMax, "Angstrom"); // 5 0.5 10
		lorentz[i] = new LorentzianPdf(Form("Lorentzian%d", i + 1), StringUtils::ordinal(i+1, "Lorentz")->Data(), *observable, *mean, *lorentzA[i]);
		pdfsInMaterial->add(*lorentz[i]);
	}

	// Damping Lorentz PDFs
	RooRealVar** dampLorentzA = new RooRealVar*[numLorentzSum];
	DampLorentzPdf** dampLorentz = new DampLorentzPdf*[numLorentzSum];
	for (int i = 0; i < numLorentzSum; i++) {
		Double_t aVal = getDefaultAValue(aMin, aMax, i+1, numLorentzSum);
		dampLorentzA[i] = new RooRealVar(Form("dampLorentz%dA", i + 1), StringUtils::ordinal(i+1, "Damping exponential w.f. parameter")->Data(), aVal, aMin, aMax, "Angstrom"); // 5 0.5 10
		dampLorentz[i] = new DampLorentzPdf(Form("DampLorentz%d", i + 1), StringUtils::ordinal(i+1, "Damp lorentz")->Data(), *observable, *mean, *dampLorentzA[i]);
		pdfsInMaterial->add(*dampLorentz[i]);
	}

	// Orthogonal PDF
	// if (hasOrthogonal){
	//     RootHelper::deleteObject("a1");
	//     RooRealVar* a1 = new RooRealVar("a1", "a1 coeff", 1, 1E-2, 1E2, "A");
	//     RootHelper::deleteObject("a2");
	//     RooRePdfProvideralVar* a2 = new RooRealVar("a2", "a2 coeff", 1, 1E-2, 1E2, "A");
	//     RootHelper::deleteObject("orthogonal");
	//     OrthogonalPdf* orthogonalPdf = new OrthogonalPdf("orthogonal", "Orthogonal Pdf", *x, *x0, *a1, *a2);
	//     components->add(*orthogonalPdf);
	//     pdfsInMaterial->add(*orthogonalPdf);
	//     RooRealVar* Int_ortho = new RooRealVar("Int_ortho", "Orthogonal intensity", 0.3, 0.0, 1.0);
	//     coeffsInMaterial->add(*Int_ortho);
	// }

	materialIntensities = new RooArgList();
	// AddPdf::add initializes intensity variables itself, we only pass a RooArgList pointer there.
	pdfInMaterial = AddPdf::add(pdfsInMaterial, materialIntensities, observable, "materialPdf", kTRUE); // build recursive! sum only

	// Mark to ignore on plot if more than one component (ignore sum but plot individual components)
	if (pdfInMaterial && pdfsInMaterial->getSize() > 1){
		pdfInMaterial->setAttribute(Constants::ATTR_NO_DRAW_ON_PLOT, kTRUE);
	}
}

void PdfProvider::initSourceContribution(TH1F* sourceHist){
	if (!sourceHist) return;

	// cutHist() always returns copy of the histogram so we ensure we don't change the model
    HistProcessor* histProcessor = HistProcessor::getInstance();
    Int_t minBin = sourceHist->GetXaxis()->FindBin(observable->getMin());
    Int_t maxBin = sourceHist->GetXaxis()->FindBin(observable->getMax());
	// sourceHist->GetXaxis()->SetRangeUser(observable->getMin(), observable->getMax());
	TH1F* sourceHistogram = histProcessor->cutHist("sourceHistogram", sourceHist, minBin, maxBin);
//	histProcessor->liftHistAboveZero(sourceHistogram);
	sourceHistogram = histProcessor->removeHistNegatives("sourceHistogramNoNegatives", sourceHistogram);

	RooDataHist* sourceDataHist = new RooDataHist("sourceDataHist", "Source Data Hist", RooArgList(*observable), sourceHistogram);
	sourcePdf = new RooHistPdf("sourcePdf", "Source Contribution PDF", RooArgSet(*observable), *sourceDataHist, 1);

	// Add PDF from annihilation in source if needed
	intSource = new RooRealVar("intSource", "Source contribution", 12, 5, 20, "%");
	intSource->setConstant(kTRUE);

	RooFormulaVar* intSourceNorm = new RooFormulaVar("intSourceNorm", "Source contribution normalized", "@0/100", RooArgList(*intSource));

	pdfFinal = new RooAddPdf("materialSourcePdf", "Pdf with source contribution", RooArgList(*sourcePdf, *pdfFinal), RooArgList(*intSourceNorm));
	pdfFinal->fixAddCoefNormalization(RooArgSet(*observable));
}

void PdfProvider::initSingleDetectorBackground() {
	// Ore-Powell background
//    RooRealVar* threeGammaInt = new RooRealVar("threeGammaInt", "Three Gamma fraction", 10, 1, 100);
//    BackgroundPdf* bgPdf = new BackgroundPdf("bgPdf", "Ore-Powell background", *observable, *threeGammaInt);
//    RooRealVar* bgPdfFraction = new RooRealVar("bgPdfFraction", "Background fraction", backgroundFraction, backgroundFraction/10, 1.);
//    bgComponents->add(*bgPdf);
//    components->add(*bgPdf);
//    pdfList->add(*bgPdf);
//    coeffList->add(*bgPdfFraction);

	// Calculate constant background contribution
	RooPolynomial* flatBackgroundPdf = new RooPolynomial("flatBackgroundPdf", "Flat background", *observable, RooArgSet());
//	backgroundComponents->add(*flatBackgroundPdf);
	Double_t rectAreaBelowHistogram = (fitHistogram->GetXaxis()->GetXmax() - fitHistogram->GetXaxis()->GetXmin()) * fitHistogram->GetMinimum();
    Double_t integral = fitHistogram->Integral("width");
    Double_t flatBgContribution = rectAreaBelowHistogram/integral*100;

	Double_t flatBackgroundContribution = flatBgContribution/integral;
	RooRealVar* intFlatBackground = new RooRealVar("intFlatBackground", "Flat background fraction", flatBgContribution, flatBgContribution/1.5, TMath::Max(100., flatBgContribution*1.5), "%");
    RooFormulaVar* intFlatBackgroundNorm = new RooFormulaVar("intFlatBackgroundNorm", "Constant background fraction normalized", "@0/100", RooArgList(*intFlatBackground));

    // Calculate "atan" background contribution
    Double_t leftWingValue = fitHistogram->GetBinContent(1);
    Double_t rightWingValue = fitHistogram->GetBinContent(fitHistogram->GetNbinsX());
    Double_t peakCenter = fitHistogram->GetBinCenter(fitHistogram->GetMaximumBin());

    // Atan can be higher on left wing or right wing
    Double_t atanArea = 0;
    RooGenericPdf* atanBackgroundPdf;
    // TODO: if no convolution then erf (but stretched somehow corresponding to the resolution function of the detector?)
    // TODO: if convolution then simply step function
    if (leftWingValue < rightWingValue){
        // Atan Contribution is right rectangle area
        atanArea = (fitHistogram->GetXaxis()->GetXmax()-peakCenter)*TMath::Abs(leftWingValue-rightWingValue);
        atanBackgroundPdf = new RooGenericPdf("erfBackgroundPdf", "Erf background pdf", "erf((@0 - @1))", RooArgList(*observable, *mean));
        // atanBackgroundPdf = new RooGenericPdf("atanBackgroundPdf", "Arctangent background pdf", "@2/2 + (-1)*atan((@0 - @1))", RooArgList(*observable, *mean, *Constants::pi));
    } else {
        atanArea = (peakCenter-fitHistogram->GetXaxis()->GetXmin())*TMath::Abs(leftWingValue-rightWingValue);
        atanBackgroundPdf = new RooGenericPdf("erfBackgroundPdf", "Erf background pdf", "1-erf((@0 - @1))", RooArgList(*observable, *mean));
//        atanBackgroundPdf = new RooGenericPdf("atanBackgroundPdf", "Arctangent background pdf", "@2/2 + (-1)*erf((@0 - @1))", RooArgList(*observable, *mean, *Constants::pi));
    }
    Double_t atanContribution = atanArea/integral*100;

//	backgroundComponents->add(*atanBackgroundPdf);
	RooRealVar* intAtanBackground = new RooRealVar("intErfBackground", "Intensity of erf background", atanContribution, atanContribution/5, atanContribution*5, "%");
    RooFormulaVar* intAtanBackgroundNorm = new RooFormulaVar("intErfBackgroundNorm", "Intensity of erf background normalized", "@0/100", RooArgList(*intAtanBackground));

    if (pdfInMaterial){
		pdfInMaterial = new RooAddPdf("withBackgroundPdf", "Material components and background", RooArgList(*atanBackgroundPdf, *flatBackgroundPdf, *pdfInMaterial), RooArgList(*intAtanBackgroundNorm, *intFlatBackgroundNorm));
		pdfInMaterial->setAttribute(Constants::ATTR_NO_DRAW_ON_PLOT, kTRUE);
		pdfInMaterial->fixAddCoefNormalization(RooArgSet(*observable));
    }
    else {
    	pdfInMaterial = new RooAddPdf("backgroundPdf", "Background", RooArgList(*atanBackgroundPdf, *flatBackgroundPdf), RooArgList(*intAtanBackgroundNorm));
    }
}

void PdfProvider::initTwoDetectorBackground() {
    // Histogram might have negative values when we subtract background in 2-detector experiment.
    // In order to perform chi^2 fit we add a constant value to the histogram to avoid negative or zero values.

	// Calculate histogram ground level and lift it and find ground contribution
	HistProcessor* histProcessor = HistProcessor::getInstance();

	// lift is how high we lifted the histogram up
	Double_t lift = fitHistogram->GetMinimum() < 0 ? TMath::Abs(fitHistogram->GetMinimum()) : 0; // lift is abs(minimum hist value)
	lift++; // cant plot zeros in log scale
	histProcessor->liftHist(fitHistogram, lift);

	RooRealVar* background = new RooRealVar("background", TString::Format("Background (histogram is lifted %.1f)", lift).Data(), lift+0.5, 0, (Int_t) (lift+0.5)*10, "counts");
//	groundLevel->setConstant(kTRUE);
//	groundLevel->setAttribute(Constants::ATTR_NO_SAVE_TO_POOL);

//	Double_t r = observable->getMax() - observable->getMin();
//	RooConstVar* range = new RooConstVar("range", "Histogram range", r); // keV
//	range->setConstant(kTRUE);
//	range->setAttribute(Constants::ATTR_NO_SAVE_TO_POOL);

//	Double_t i = fitHistogram->Integral();
//	RooRealVar* integral = new RooRealVar("integral", "Histogram integral", i); // counts
//	integral->setConstant(kTRUE);
//	integral->setAttribute(Constants::ATTR_NO_SAVE_TO_POOL);

	RooConstVar* bins = new RooConstVar("bins", "Histogram bins", fitHistogram->GetXaxis()->GetNbins()); // keV
	RooConstVar* integral = new RooConstVar("integral", "Histogram integral", fitHistogram->Integral()); // counts
	RooFormulaVar* intFlatBackgroundNorm = new RooFormulaVar("intFlatBackgroundNorm", "Flat background intensity normalized", "@0*@1/@2", RooArgList(*background, *bins, *integral));

	// Initialize flat ground PDF
	RooPolynomial* flatPdf = new RooPolynomial("flatPdf", "Histogram ground level", *observable, RooArgSet());

	if (pdfInMaterial){
		pdfInMaterial = new RooAddPdf("withFlatBackgroundPdf", "Sum of model and ground level", RooArgList(*flatPdf, *pdfInMaterial), RooArgList(*intFlatBackgroundNorm));
		pdfInMaterial->setAttribute(Constants::ATTR_NO_DRAW_ON_PLOT, kTRUE);
		pdfInMaterial->fixAddCoefNormalization(RooArgSet(*observable));
	}
	else {
		pdfInMaterial = flatPdf;
	}
}


void PdfProvider::initConvolutedModel(ConvolutionType convolutionType) {
	if (convolutionType == kNoConvolution){
		pdfFinal = pdfInMaterial;
		return;
	}

	// Convolute components in material
	RooRealVar* resolutionFWHM = new RooRealVar("resolutionFWHM", "Resolution function FWHM", 2, 0.5, 4, "keV");
	// resolutionFWHM->setConstant(kTRUE);
	RooFormulaVar* resFunctSigma = new RooFormulaVar("resFunctSigma", "@0*@1", RooArgList(*resolutionFWHM, *Constants::rooFwhmToSigma));

	resolutionFunction = new RooGaussian("resolutionPdf", "Resolution function", *observable, *mean, *resFunctSigma);
	if (convolutionType == kFFTConvolution){
	    observable->setBins(2048, "cache");
	    pdfFinal = new RooFFTConvPdf("modelConvoluted", "Convoluted with resolution function", *observable, *pdfInMaterial, *resolutionFunction);
	}
	else if (convolutionType == kNumericConvolution){
	    pdfFinal = new RooNumConvPdf("modelConvoluted", "Convoluted with resolution function", *observable, *pdfInMaterial, *resolutionFunction);
	    ((RooNumConvPdf*)pdfFinal)->setConvolutionWindow(*mean, *resFunctSigma, 5);
	}
}

// Getters
RooRealVar* PdfProvider::getObservable(){
	return observable;
}

RooRealVar* PdfProvider::getMean(){
	return mean;
}

RooArgSet* PdfProvider::getParameters() {
	return getPdf()->getParameters(RooArgSet(*getObservable()));
}

RooArgList* PdfProvider::getIndirectParameters() {
	RooArgList* parameters = new RooArgList();
	TIterator* it = pdfInMaterial->getComponents()->createIterator();
	while (TObject* tempObj = it->Next()) {
		if (IndirectParamPdf* paramPdf = dynamic_cast<IndirectParamPdf*>(tempObj)) {
		    Bool_t isTwoDetector = HistProcessor::isTwoDetector(fitProperties.hist);
			RooArgList* list = paramPdf->getParameters(isTwoDetector);
			parameters->add(*list);
		}
	}
	return parameters;
}

RooRealVar* PdfProvider::getSourceContribution() {
	return intSource;
}

RooArgList* PdfProvider::getIntensitiesInMaterial(){
	return materialIntensities;
}

TH1F* PdfProvider::getFitHistogram(){
	return fitHistogram;
}

RooAbsPdf* PdfProvider::getPdf() {
	return pdfFinal;
}

RooAbsPdf* PdfProvider::getSourcePdf() {
	return sourcePdf;
}

RooAbsPdf* PdfProvider::getResolutionFunction() {
	return resolutionFunction;
}

RooAbsPdf* PdfProvider::getPdfInMaterial() {
	return pdfInMaterial;
}

Double_t PdfProvider::getDefaultAValue(Double_t aMin, Double_t aMax, Int_t currentIndex, Int_t maxIndex) {
	// Polynomial default coefficients
	// https://www.dropbox.com/s/xykmomlt9x3k181/Photo%20Aug%2002%2C%204%2048%2058%20PM.jpg?dl=0
	Double_t x = (Double_t) currentIndex/(maxIndex+1);
	Double_t y = TMath::Power(x, 4);
	return aMin + y*(aMax-aMin);
}
