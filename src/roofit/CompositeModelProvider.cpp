/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   CompositeModelProvider.cpp
 * Author: petrstepanov
 *
 * Created on August 15, 2017, 9:59 PM
 */

#include "CompositeModelProvider.h"
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

CompositeModelProvider::CompositeModelProvider(FitProperties fitProperties) : observable(0), mean(0), fitHistogram(0), modelNonConvoluted(0), modelConvoluted(0){
	// Define variables
	this->fitProperties = fitProperties;

	// Cut original histogram with respect to the fitting limits
	HistProcessor* histProcessor = HistProcessor::getInstance();
	fitHistogram = histProcessor->cutHist("fitHistogram", fitProperties.hist, fitProperties.fitMin->getVal(), fitProperties.fitMax->getVal());

	// Check histogram minimum and lift it if needed (bin values need to be > 0 for chi2 fit)
	Double_t histMinimum = fitProperties.hist->GetMinimum();
	if (histMinimum <= 0){
		lift = TMath::Abs(histMinimum - 0.1);
	}
	histProcessor->liftHist(fitHistogram, lift);

	// Initialize fitting pdf
	initObservableAndMean(fitProperties.fitMin, fitProperties.fitMax);
	initModel(fitProperties.hasParabola, fitProperties.numberOfGaussians, fitProperties.numberOfExponents, fitProperties.numberOfDampingExponents);
	initSourceContribution(fitProperties.sourceHist);

	// In case of single detector experiment - add background
	backgroundComponents = new RooArgList();
	if (fitProperties.isTwoDetector == kFALSE) {
		initSingleDetectorBackground();
	} else {
		initTwoDetectorBackground();
	}

	initConvolutedModel(fitProperties.convolutionType);
}

CompositeModelProvider::~CompositeModelProvider(){
}

void CompositeModelProvider::initObservableAndMean(RooRealVar* fitMin, RooRealVar* fitMax){
	// Define energy axis (observable)
	observable = new RooRealVar("observable", "Energy axis", fitHistogram->GetXaxis()->GetXmin(), fitHistogram->GetXaxis()->GetXmax(), "keV");

	// Set binning for different types of convolution
	observable->setBins(fitHistogram->GetNbinsX());
	observable->setBins(1024, "cache");

	// Set mean
	Double_t m = fitHistogram->GetBinCenter(fitHistogram->GetMaximumBin());
	mean = new RooRealVar("mean", "Spectrum peak position", m, m-1, m+1);
}

void CompositeModelProvider::initModel(Bool_t hasParabola, const Int_t numGauss, const Int_t numLorentz, const Int_t numLorentzSum) {
	RooArgList* pdfsInMaterial = new RooArgList();
//	coeffsInMaterial = new RooArgList();

	// Parabola PDF
	if (hasParabola) {
		RooRealVar* parabolaRoot = new RooRealVar("parabolaRoot", "Coefficient at -x^2 + r*2", 3.5, 2.5, 8); // 3.4579 = Al (11.7)
		ParabolaPdf* parabolaPdf = new ParabolaPdf("Parabola", "Parabola p.d.f.", *observable, *mean, *parabolaRoot);
//		components->add(*parabola);
		pdfsInMaterial->add(*parabolaPdf);
//		RooRealVar* intParabola = new RooRealVar("intParabola", "Parabola intensity", 90, 0, 100);
//		RooFormulaVar* intParabolaNorm = new RooFormulaVar("intParabolaNorm", "Parabola intensity normalized", "@0/100", RooArgList(*intParabola));
//		coeffsInMaterial->add(*intParabolaNorm);
	}

	// Gauss PDFs
	RooRealVar** gaussA = new RooRealVar*[numGauss];
	GaussianPdf** gauss = new GaussianPdf*[numGauss];
//	RooRealVar** intGauss = new RooRealVar*[numGauss];
//	RooFormulaVar** intGaussNorm = new RooFormulaVar*[numGauss];
	Double_t aMin = 0.01;
	Double_t aMax = 50;
	for (int i = 0; i < numGauss; i++) {
		Double_t aVal = getDefaultAValue(aMin, aMax, i+1, numGauss);
		gaussA[i] = new RooRealVar(Form("gaussian%dA", i + 1), StringUtils::ordinal(i+1, "Gaussian w.f. parameter")->Data(), aVal, aMin, aMax, "Angstrom"); // 5 0.5 10
		gauss[i] = new GaussianPdf(Form("Gaussian%d", i + 1), StringUtils::ordinal(i+1, "Gaussian p.d.f.")->Data(), *observable, *mean, *gaussA[i]);
		pdfsInMaterial->add(*gauss[i]);
//		intGauss[i] = new RooRealVar(Form("intGauss%d", i + 1), StringUtils::ordinal("gaussian intensity", i + 1)->Data(), 5, 0, 100);
//		intGaussNorm[i] = new RooFormulaVar(Form("intGauss%dNorm", i + 1), StringUtils::ordinal("gaussian intensity normalized", i + 1)->Data(), "@0/100", RooArgList(*intGauss[i]));
//		coeffsInMaterial->add(*intGaussNorm[i]);
	}

	// Lorentz PDFs
	RooRealVar** lorentzA = new RooRealVar*[numLorentz];
	LorentzianPdf** lorentz = new LorentzianPdf*[numLorentz];
//	RooRealVar** intLorentz = new RooRealVar*[numLorentz];
//	RooFormulaVar** intLorentzNorm = new RooFormulaVar*[numLorentz];
	for (int i = 0; i < numLorentz; i++) {
		Double_t aVal = getDefaultAValue(aMin, aMax, i+1, numLorentz);
		lorentzA[i] = new RooRealVar(Form("lorentzian%dA", i + 1), StringUtils::ordinal(i+1, "Exponential w.f. parameter")->Data(), aVal, aMin, aMax, "Angstrom"); // 5 0.5 10
		lorentz[i] = new LorentzianPdf(Form("Lorentzian%d", i + 1), StringUtils::ordinal(i+1, "Lorentzian p.d.f.")->Data(), *observable, *mean, *lorentzA[i]);
		pdfsInMaterial->add(*lorentz[i]);
//		intLorentz[i] = new RooRealVar(Form("intLorentz%d", i + 1), StringUtils::ordinal("lorentzian intensity", i + 1)->Data(), 5, 0, 100);
//		intLorentzNorm[i] = new RooFormulaVar(Form("intLorentz%dNorm", i + 1), StringUtils::ordinal("gaussian intensity normalized", i + 1)->Data(), "@0/100", RooArgList(*intLorentz[i]));
//		coeffsInMaterial->add(*intLorentzNorm[i]);
	}

	// Damping Lorentz PDFs
	RooRealVar** dampLorentzA = new RooRealVar*[numLorentzSum];
	DampLorentzPdf** dampLorentz = new DampLorentzPdf*[numLorentzSum];
//	RooRealVar** intLorentzDamp = new RooRealVar*[numLorentzSum];
//	RooFormulaVar** intLorentzDampNorm = new RooFormulaVar*[numLorentzSum];
	for (int i = 0; i < numLorentzSum; i++) {
		Double_t aVal = getDefaultAValue(aMin, aMax, i+1, numLorentzSum);
		gaussA[i] = new RooRealVar(Form("gaussian%dA", i + 1), StringUtils::ordinal(i+1, "Gaussian w.f. parameter")->Data(), aVal, aMin, aMax, "Angstrom"); // 5 0.5 10
		gauss[i] = new GaussianPdf(Form("Gaussian%d", i + 1), StringUtils::ordinal(i+1, "Gaussian p.d.f.")->Data(), *observable, *mean, *gaussA[i]);
		pdfsInMaterial->add(*gauss[i]);

		dampLorentzA[i] = new RooRealVar(Form("dampLorentz%dA", i + 1), StringUtils::ordinal(i+1, "Damping exponential w.f. parameter")->Data(), aVal, aMin, aMax, "Angstrom"); // 5 0.5 10
		dampLorentz[i] = new DampLorentzPdf(Form("DampLorentz%d", i + 1), StringUtils::ordinal(i+1, "Damping lorentzian p.d.f.")->Data(), *observable, *mean, *dampLorentzA[i]);
		pdfsInMaterial->add(*dampLorentz[i]);
//		intLorentzDamp[i] = new RooRealVar(Form("intLorentzDamp%d", i + 1), StringUtils::ordinal("damping lorentzian intensity", i + 1)->Data(), 5, 0, 100);
//		intLorentzDampNorm[i] = new RooFormulaVar(Form("intLorentzDamp%dNorm", i + 1), StringUtils::ordinal("damping lorentzian intensity normalized", i + 1)->Data(), "@0/100", RooArgList(*intLorentzDamp[i]));
//		coeffsInMaterial->add(*intLorentzDampNorm[i]);
	}

	// Orthogonal PDF
	// if (hasOrthogonal){
	//     RootHelper::deleteObject("a1");
	//     RooRealVar* a1 = new RooRealVar("a1", "a1 coeff", 1, 1E-2, 1E2, "A");
	//     RootHelper::deleteObject("a2");
	//     RooReCompositeModelProvideralVar* a2 = new RooRealVar("a2", "a2 coeff", 1, 1E-2, 1E2, "A");
	//     RootHelper::deleteObject("orthogonal");
	//     OrthogonalPdf* orthogonalPdf = new OrthogonalPdf("orthogonal", "Orthogonal Pdf", *x, *x0, *a1, *a2);
	//     components->add(*orthogonalPdf);
	//     pdfsInMaterial->add(*orthogonalPdf);
	//     RooRealVar* Int_ortho = new RooRealVar("Int_ortho", "Orthogonal intensity", 0.3, 0.0, 1.0);
	//     coeffsInMaterial->add(*Int_ortho);
	// }

    // Remove last coefficient - refer to RooAddPdf manual
//	Int_t coeffsNumber = coeffsInMaterial->getSize();
//	RooAbsArg* lastCoeff = coeffsInMaterial->at(coeffsNumber-1);
//	coeffsInMaterial->remove(*lastCoeff);
//	coeffsInMaterial->Print();

	modelNonConvoluted = AddPdf::add(pdfsInMaterial, observable, "materialPdf");

//	modelNonConvoluted = new RooAddPdf("materialPdf", "Sum of components in material", *pdfsInMaterial, *coeffsInMaterial, kTRUE);
//	modelNonConvoluted = new RooAddPdf("materialPdf", "Sum of components in material", *pdfsInMaterial, *coeffsInMaterial);
//	modelNonConvoluted->fixAddCoefNormalization(RooArgSet(*observable));
}

void CompositeModelProvider::initSourceContribution(TH1F* sourceHist){
	if (!sourceHist) return;

	Double_t histMin = sourceHist->GetXaxis()->GetXmin();
	Double_t histMax = sourceHist->GetXaxis()->GetXmax();

	RooDataHist* sourceDataHist;
	// Cut source histogram if it exceeds the observable limits
	if (histMin < observable->getMin() || histMax > observable->getMax()) {
		HistProcessor* histProcessor = HistProcessor::getInstance();
		TH1F* cutSourceHist = histProcessor->cutHist("cutSourceHist", sourceHist, observable->getMin(), observable->getMax());
		Double_t min = cutSourceHist->GetXaxis()->GetXmin();
		Double_t max = cutSourceHist->GetXaxis()->GetXmax();
		std::cout << "cutSourceHistMin: " << min << ". cutSourceHistMax:" << max << std::endl;

		sourceDataHist = new RooDataHist("sourceDataHist", "Source Data Hist", RooArgList(*observable), RooFit::Import(*cutSourceHist));
	} else {
		sourceDataHist = new RooDataHist("sourceDataHist", "Source Data Hist", RooArgList(*observable), RooFit::Import(*sourceHist));
	}
	RooHistPdf* sourcePdf = new RooHistPdf("sourcePdf", "Source Contribution PDF", *observable, *sourceDataHist, 1);

	// Add PDF from annihilation in source if needed
//	components->add(*sourcePdf);
	intSource = new RooRealVar("intSource", "Source contribution", 10, 5, 20, "%");
    RooFormulaVar* intSourceNorm = new RooFormulaVar("intSourceNorm", "Source contribution normalized", "@0/100", RooArgList(*intSource));
	modelNonConvoluted = new RooAddPdf("materialSourcePdf", "Sum of components in material with source contribution", RooArgList(*sourcePdf, *modelNonConvoluted), RooArgList(*intSourceNorm));
	modelNonConvoluted->fixAddCoefNormalization(RooArgSet(*observable));
}

void CompositeModelProvider::initSingleDetectorBackground() {
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
	backgroundComponents->add(*flatBackgroundPdf);
	RooRealVar* intFlatBackground = new RooRealVar("intFlatBackground", "Flat background fraction", 10, 0, 20, "%");
    RooFormulaVar* intFlatBackgroundNorm = new RooFormulaVar("intFlatBackgroundNorm", "Constant background fraction normalized", "@0/100", RooArgList(*intFlatBackground));

	// Atan background
	RooGenericPdf* atanBackgroundPdf = new RooGenericPdf("atanBackgroundPdf", "Arctangent background pdf", "@2/2 + (-1)*atan((@0 - @1))", RooArgList(*observable, *mean, *Constants::pi));
	backgroundComponents->add(*atanBackgroundPdf);
	RooRealVar* intAtanBackground = new RooRealVar("intAtanBackground", "Intensity of arctangent background", 1, 0, 10, "%");
    RooFormulaVar* intAtanBackgroundNorm = new RooFormulaVar("intAtanBackgroundNorm", "Intensity of arctangent background normalized", "@0/100", RooArgList(*intAtanBackground));

	modelNonConvoluted = new RooAddPdf("withBackgroundPdf", "Sum of model and background", RooArgList(*atanBackgroundPdf, *flatBackgroundPdf, *modelNonConvoluted), RooArgList(*intAtanBackgroundNorm, *intFlatBackgroundNorm));
	modelNonConvoluted->fixAddCoefNormalization(RooArgSet(*observable));
}

void CompositeModelProvider::initTwoDetectorBackground() {
	// Constant background
	RooPolynomial* flatBackgroundPdf = new RooPolynomial("flatBackgroundPdf", "Flat background", *observable, RooArgSet());
	backgroundComponents->add(*flatBackgroundPdf);

	RooConstVar* integral = new RooConstVar("integral", "Histogram integral", fitHistogram->Integral());
	RooConstVar* bins = new RooConstVar("bins", "Histogram bins", (Double_t)fitHistogram->GetNbinsX());
	RooConstVar* zeroLevel = new RooConstVar("zeroLevel", "Histogram zero level", lift);

	RooFormulaVar* intFlatBackground = new RooFormulaVar("intFlatBackground", "Flat background fraction", "@0/(@1*@2)", RooArgList(*integral, *bins, *zeroLevel));
	RooFormulaVar* intFlatBackgroundNorm = new RooFormulaVar("intFlatBackgroundNorm", "Constant background fraction normalized", "@0/100", RooArgList(*intFlatBackground));

	modelNonConvoluted = new RooAddPdf("withBackgroundPdf", "Sum of model and background", RooArgList(*flatBackgroundPdf, *modelNonConvoluted), RooArgList(*intFlatBackgroundNorm));
	modelNonConvoluted->fixAddCoefNormalization(RooArgSet(*observable));
}


void CompositeModelProvider::initConvolutedModel(ConvolutionType convolutionType) {
	if (convolutionType == kNoConvolution) return;

	// Make resolution Function
	RooRealVar* resolutionFWHM = new RooRealVar("resolutionFWHM", "Resolution function FWHM", 2, 0.5, 4, "keV");
	RooFormulaVar* resFunctSigma = new RooFormulaVar("resFunctSigma", "@0*@1", RooArgList(*resolutionFWHM, *Constants::rooFwhmToSigma));
	resolutionFunction = new RooGaussian("resolutionPdf", "Resolution function", *observable, *mean, *resFunctSigma);

	modelConvoluted = new RooFFTConvPdf("modelConvoluted", "Convoluted with resolution function", *observable, *modelNonConvoluted, *resolutionFunction);
}

// Getters

RooRealVar* CompositeModelProvider::getObservable(){
	return observable;
}

RooRealVar* CompositeModelProvider::getMean(){
	return mean;
}

RooArgList* CompositeModelProvider::getIndirectParameters() {
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
//
//RooArgList* CompositeModelProvider::getIntensities() {
//	return coeffsInMaterial;
//}


RooRealVar* CompositeModelProvider::getSourceContribution() {
	return intSource;
}

RooArgList* CompositeModelProvider::getBackgroundComponents() {
	return backgroundComponents;
}

//RooArgSet* CompositeModelProvider::getComponents() {
//	return modelNonConvoluted->getComponents();
//}
//
//RooArgSet* CompositeModelProvider::getParameters() {
//	return modelNonConvoluted->getParameters(RooArgSet(*observable));
//}

TH1F* CompositeModelProvider::getTrimmedHistogram(){
	return fitHistogram;
}

RooAbsPdf* CompositeModelProvider::getPdf() {
	if (modelConvoluted != nullptr){
		return modelConvoluted;
	}
	return modelNonConvoluted;
}

RooAbsPdf* CompositeModelProvider::getResolutionFunction() {
	return resolutionFunction;
}

RooAbsPdf* CompositeModelProvider::getPdfConvoluted() {
	return modelConvoluted;
}

RooAbsPdf* CompositeModelProvider::getPdfNonConvoluted() {
	return modelNonConvoluted;
}

Double_t CompositeModelProvider::getDefaultAValue(Int_t aMin, Int_t aMax, Int_t currentIndex, Int_t maxIndex) {
	// Polynomial default coefficients
	// https://www.dropbox.com/s/xykmomlt9x3k181/Photo%20Aug%2002%2C%204%2048%2058%20PM.jpg?dl=0
	Double_t x = currentIndex/(maxIndex+1);
	Double_t y = TMath::Power(x, 4);
	return y*(aMax-aMin);
}

//Double_t* CompositeModelProvider::getDefaultGaussAs(const Int_t numGauss) {
//	Double_t* As;
//	switch (numGauss) {
//	case 1:
//		static Double_t arr1[1] = { 0.5 };
//		As = &arr1[0];
//		break;
//	case 2:
//		static Double_t arr2[2] = { 0.5, 0.1 };
//		As = &arr2[0];
//		break;
//	case 3:
//		static Double_t arr3[3] = { 0.6, 0.1, 0.5 };
//		As = &arr3[0];
//		break;
//	default:
//		// The key is to give first two coefficients really different values
//		static Double_t arr4[4] = { 1.5, 0.1, 1, 0.1 };
//		As = &arr4[0];
//		break;
//	}
//	return As;
//}


//
//Double_t* CompositeModelProvider::getDefaultLorentzAs(const Int_t numLorentz) {
//	Double_t* As;
//	switch (numLorentz) {
//	case 1:
//		static Double_t arr1[1] = { 0.18 };
//		As = &arr1[0];
//		break;
//	case 2:
//		static Double_t arr2[2] = { 0.16, 0.5 };
//		As = &arr2[0];
////            As = (Double_t[2]){Constants::a_B*TMath::Sqrt(Constants::Ry/73), Constants::a_B*TMath::Sqrt(Constants::Ry/117)};
//		break;
//	case 3:
//		static Double_t arr3[3] = { 0.05, 0.1, 0.5 };
//		As = &arr3[0];
////            As = (Double_t[3]){Constants::a_B*TMath::Sqrt(Constants::Ry/73), Constants::a_B*TMath::Sqrt(Constants::Ry/117), Constants::a_B*TMath::Sqrt(Constants::Ry/1559)};
//		break;
//	default:
//		static Double_t arr4[4] = { 0.05, 0.1, 0.5, 1 };
//		As = &arr4[0];
//		break;
//	}
//	return As;
//}
//
//Double_t* CompositeModelProvider::getDefaultDampLorentzAs(const Int_t numLorentz) {
//	Double_t* As;
//	switch (numLorentz) {
//	case 1:
//		static Double_t arr1[1] = { 0.1 };
//		As = &arr1[0];
//		break;
//	case 2:
//		static Double_t arr2[2] = { 0.1, 0.4 };
//		As = &arr2[0];
//		break;
//	case 3:
//		static Double_t arr3[3] = { 0.05, 0.1, 0.5 };
//		As = &arr3[0];
//		break;
//	default:
//		static Double_t arr4[4] = { 0.05, 0.1, 0.5, 1 };
//		As = &arr4[0];
//		break;
//	}
//	return As;
//}
