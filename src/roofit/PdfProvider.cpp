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

PdfProvider::PdfProvider(FitProperties fitProperties) : observable(0), mean(0), resolutionFunction(0), fitHistogram(0), modelNonConvoluted(0), modelConvoluted(0), intSource(0){
	if ((Int_t)(fitProperties.sourceHist!=nullptr) + (Int_t)(fitProperties.hasParabola) + fitProperties.numberOfGaussians + fitProperties.numberOfExponents + fitProperties.numberOfDampingExponents == 0){
		return;
	}
	// Define variables
	this->fitProperties = fitProperties;

	// Cut original histogram with respect to the fitting limits
	HistProcessor* histProcessor = HistProcessor::getInstance();

	// Histograms can be acessed via gROOT->FindObject. So proper way to treat them is pass by reference?
	// Problem: if you pass histogram by value then you copy it but it still has the same ROOT name? Mess.
	// So we pass histograms by reference. But here cutHist() always creates new histogram! So we avoid changes in model.
	fitHistogram = histProcessor->cutHist("fitHistogram", fitProperties.hist, fitProperties.fitMin->getVal(), fitProperties.fitMax->getVal());

	// Initialize fitting pdf
	initObservableAndMean();
	initModel(fitProperties.hasParabola, fitProperties.numberOfGaussians, fitProperties.numberOfExponents, fitProperties.numberOfDampingExponents);
	initSourceContribution(fitProperties.sourceHist);

	// In case of single detector experiment - add background
//	backgroundComponents = new RooArgList();
	if (fitProperties.isTwoDetector == kFALSE) {
		initSingleDetectorBackground();
	} else {
		initTwoDetectorBackground();
	}

	initConvolutedModel(fitProperties.convolutionType);
}

PdfProvider::~PdfProvider(){
}

void PdfProvider::initObservableAndMean(){
	// Define energy axis (observable)
	observable = new RooRealVar("observable", "Energy axis", fitHistogram->GetXaxis()->GetXmin(), fitHistogram->GetXaxis()->GetXmax(), "keV");

	// Set binning for different types of convolution
	observable->setBins(fitHistogram->GetNbinsX());
	observable->setBins(1024, "cache");

	// Set mean
	Double_t m = fitHistogram->GetBinCenter(fitHistogram->GetMaximumBin());
	mean = new RooRealVar("mean", "Spectrum peak position", m, m-1, m+1);
}

void PdfProvider::initModel(Bool_t hasParabola, const Int_t numGauss, const Int_t numLorentz, const Int_t numLorentzSum) {
	// Only if has
	if ((Int_t)(fitProperties.hasParabola) + fitProperties.numberOfGaussians + fitProperties.numberOfExponents + fitProperties.numberOfDampingExponents == 0){
		return;
	}

	RooArgList* pdfsInMaterial = new RooArgList();

	// Parabola PDF
	if (hasParabola) {
		RooRealVar* parabolaRoot = new RooRealVar("parabolaRoot", "Coefficient at -x^2 + r*2", 3.5, 2.5, 8); // 3.4579 = Al (11.7)
		ParabolaPdf* parabolaPdf = new ParabolaPdf("Parabola", "Parabola p.d.f.", *observable, *mean, *parabolaRoot);
		pdfsInMaterial->add(*parabolaPdf);
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

	modelNonConvoluted = AddPdf::add(pdfsInMaterial, observable, "materialPdf");

	Int_t numberOfComponents = hasParabola + numGauss + numLorentz + numLorentzSum;
	if (numberOfComponents > 1){
		modelNonConvoluted->setAttribute(Constants::ATTR_NO_DRAW_ON_PLOT, kTRUE);
	}
}

void PdfProvider::initSourceContribution(TH1F* sourceHist){
	if (!sourceHist) return;

	// cutHist() always returns copy of the histogram so we ensure we don't change the model
	HistProcessor* histProcessor = HistProcessor::getInstance();
	TH1F* sourceHistogram = histProcessor->cutHist("sourceHistogram", sourceHist,  observable->getMin(),  observable->getMax());
	histProcessor->liftHistAboveZero(sourceHistogram);

	RooDataHist* sourceDataHist = new RooDataHist("sourceDataHist", "Source Data Hist", RooArgList(*observable), sourceHist);
	RooHistPdf* sourcePdf = new RooHistPdf("sourcePdf", "Source Contribution PDF", RooArgSet(*observable), *sourceDataHist, 1);

	// Add PDF from annihilation in source if needed
	intSource = new RooRealVar("intSource", "Contribution from annihilation in source", 12, 5, 20, "%");
	intSource->setConstant(kTRUE);

	RooFormulaVar* intSourceNorm = new RooFormulaVar("intSourceNorm", "Source contribution normalized", "@0/100", RooArgList(*intSource));

	// If there were any components besides source contribution...
	if (modelNonConvoluted){
		modelNonConvoluted = new RooAddPdf("materialSourcePdf", "Sum of components in material with source contribution", RooArgList(*sourcePdf, *modelNonConvoluted), RooArgList(*intSourceNorm));
		modelNonConvoluted->setAttribute(Constants::ATTR_NO_DRAW_ON_PLOT, kTRUE);
		modelNonConvoluted->fixAddCoefNormalization(RooArgSet(*observable));
	}
	else {
		modelNonConvoluted = sourcePdf;
	}
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

	// Constant background
	RooPolynomial* flatBackgroundPdf = new RooPolynomial("flatBackgroundPdf", "Flat background", *observable, RooArgSet());
//	backgroundComponents->add(*flatBackgroundPdf);
	RooRealVar* intFlatBackground = new RooRealVar("intFlatBackground", "Flat background fraction", 10, 0, 20, "%");
    RooFormulaVar* intFlatBackgroundNorm = new RooFormulaVar("intFlatBackgroundNorm", "Constant background fraction normalized", "@0/100", RooArgList(*intFlatBackground));

	// Atan background
	RooGenericPdf* atanBackgroundPdf = new RooGenericPdf("atanBackgroundPdf", "Arctangent background pdf", "@2/2 + (-1)*atan((@0 - @1))", RooArgList(*observable, *mean, *Constants::pi));
//	backgroundComponents->add(*atanBackgroundPdf);
	RooRealVar* intAtanBackground = new RooRealVar("intAtanBackground", "Intensity of arctangent background", 1, 0, 10, "%");
    RooFormulaVar* intAtanBackgroundNorm = new RooFormulaVar("intAtanBackgroundNorm", "Intensity of arctangent background normalized", "@0/100", RooArgList(*intAtanBackground));

	modelNonConvoluted = new RooAddPdf("withBackgroundPdf", "Sum of model and background", RooArgList(*atanBackgroundPdf, *flatBackgroundPdf, *modelNonConvoluted), RooArgList(*intAtanBackgroundNorm, *intFlatBackgroundNorm));
	modelNonConvoluted->setAttribute(Constants::ATTR_NO_DRAW_ON_PLOT, kTRUE);
	modelNonConvoluted->fixAddCoefNormalization(RooArgSet(*observable));
}

void PdfProvider::initTwoDetectorBackground() {
    // Histogram might have negative values when we subtract background in 2-detector experiment.
    // In order to perform chi^2 fit we add a constant value to the histogram to avoid negative or zero values.

	// Calculate histogram ground level and lift it and find ground contribution
	HistProcessor* histProcessor = HistProcessor::getInstance();
	Double_t lift = histProcessor->liftHistAboveZero(fitHistogram); // lift is how high we lifted the histogram up
	histProcessor->liftHist(fitHistogram, 1); lift++; // need to lift more because histogram still

	RooRealVar* groundLevel = new RooRealVar("groundLevel", "Histogram ground level (for chi2 fit)", lift, 0, (Int_t) lift*10, "counts");
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
	RooFormulaVar* intFlatBackgroundNorm = new RooFormulaVar("intFlatBackgroundNorm", "Flat background intensity normalized", "@0*@1/@2", RooArgList(*groundLevel, *bins, *integral));

	// Initialize flat ground PDF
	RooPolynomial* flatPdf = new RooPolynomial("flatPdf", "Histogram ground level", *observable, RooArgSet());

	modelNonConvoluted = new RooAddPdf("withFlatBackgroundPdf", "Sum of model and ground level", RooArgList(*flatPdf, *modelNonConvoluted), RooArgList(*intFlatBackgroundNorm));
	modelNonConvoluted->setAttribute(Constants::ATTR_NO_DRAW_ON_PLOT, kTRUE);
	modelNonConvoluted->fixAddCoefNormalization(RooArgSet(*observable));
}


void PdfProvider::initConvolutedModel(ConvolutionType convolutionType) {
	if (convolutionType == kNoConvolution) return;

	// Make resolution Function
	RooRealVar* resolutionFWHM = new RooRealVar("resolutionFWHM", "Resolution function FWHM", 2, 0.5, 4, "keV");
	resolutionFWHM->setConstant(kTRUE);
	RooFormulaVar* resFunctSigma = new RooFormulaVar("resFunctSigma", "@0*@1", RooArgList(*resolutionFWHM, *Constants::rooFwhmToSigma));

	resolutionFunction = new RooGaussian("resolutionPdf", "Resolution function", *observable, *mean, *resFunctSigma);
	modelConvoluted = new RooFFTConvPdf("modelConvoluted", "Convoluted with resolution function", *observable, *modelNonConvoluted, *resolutionFunction);
}

// Getters
RooRealVar* PdfProvider::getObservable(){
	return observable;
}

RooRealVar* PdfProvider::getMean(){
	return mean;
}

RooArgList* PdfProvider::getIndirectParameters() {
	RooArgList* parameters = new RooArgList();
	TIterator* it = modelNonConvoluted->getComponents()->createIterator();
	while (TObject* tempObj = it->Next()) {
		if (IndirectParamPdf* paramPdf = dynamic_cast<IndirectParamPdf*>(tempObj)) {
			RooArgList* list = paramPdf->getParameters(fitProperties.isTwoDetector);
			parameters->add(*list);
		}
	}
	return parameters;
}

RooRealVar* PdfProvider::getSourceContribution() {
	return intSource;
}

TH1F* PdfProvider::getFitHistogram(){
	return fitHistogram;
}

RooAbsPdf* PdfProvider::getPdf() {
	if (modelConvoluted != nullptr){
		return modelConvoluted;
	}
	return modelNonConvoluted;
}

RooAbsPdf* PdfProvider::getResolutionFunction() {
	return resolutionFunction;
}

RooAbsPdf* PdfProvider::getPdfConvoluted() {
	return modelConvoluted;
}

RooAbsPdf* PdfProvider::getPdfNonConvoluted() {
	return modelNonConvoluted;
}

Double_t PdfProvider::getDefaultAValue(Double_t aMin, Double_t aMax, Int_t currentIndex, Int_t maxIndex) {
	// Polynomial default coefficients
	// https://www.dropbox.com/s/xykmomlt9x3k181/Photo%20Aug%2002%2C%204%2048%2058%20PM.jpg?dl=0
	Double_t x = (Double_t) currentIndex/(maxIndex+1);
	Double_t y = TMath::Power(x, 4);
	return aMin + y*(aMax-aMin);
}
