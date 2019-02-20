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
#include "../model/Constants.h"
#include "../util/HistProcessor.h"
#include "../util/StringUtils.h"
//#include "ChannelConvolutionPdf.h"
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

CompositeModelProvider::CompositeModelProvider(RooRealVar* x, RooRealVar* x0) :
		AbstractModelProvider(x0) {
	observable = x;
}

// Add source contribution pdf to the model
void CompositeModelProvider::initSourcePdf(TH1F* sourceHist, RooAbsReal* sourceContrib) {
	Double_t histMin = sourceHist->GetXaxis()->GetXmin();
	Double_t histMax = sourceHist->GetXaxis()->GetXmax();
	Double_t observableMin = observable->getMin();
	Double_t observableMax = observable->getMax();
	std::cout << "histMin: " << histMin << ". histMax:" << histMax << std::endl;
	std::cout << "observableMin: " << observableMin << ". observableMax:" << observableMax << std::endl;
	RooDataHist* sourceDataHist;
	if (histMin < observableMin || histMax > observableMax) {
		HistProcessor* histProcessor = HistProcessor::getInstance();
		TH1F* cutSourceHist = (TH1F*) histProcessor->cutHist("cutSourceHist", sourceHist, observableMin, observableMax);
		Double_t min = cutSourceHist->GetXaxis()->GetXmin();
		Double_t max = cutSourceHist->GetXaxis()->GetXmax();
		std::cout << "cutSourceHistMin: " << min << ". cutSourceHistMax:" << max << std::endl;

		sourceDataHist = new RooDataHist("sourceDataHist", "Source Data Hist", RooArgList(*observable), RooFit::Import(*cutSourceHist));
	} else {
		sourceDataHist = new RooDataHist("sourceDataHist", "Source Data Hist", RooArgList(*observable), RooFit::Import(*sourceHist));
	}
	this->sourcePdf = new RooHistPdf("sourcePdf", "Source contribution", *observable, *sourceDataHist, 1);
	this->sourceContribution = sourceContrib;
}

void CompositeModelProvider::initModel(Bool_t hasParabola, const Int_t numGauss, const Int_t numLorentz, const Int_t numLorentzSum, RooRealVar* fwhm) {
	pdfsToBeConvoluted = new RooArgList();
	// Check source contribution
	if (sourcePdf) {
		if (sourceContribution->getVal() != 0) {
			components->add(*sourcePdf);
		}
	}

	// Define parabola
	if (hasParabola) {
		RootHelper::deleteObject("parabolaRoot");
		RooRealVar* parabolaRoot = new RooRealVar("parabolaRoot", "Coefficient at -x^2 + r*2", 3.5, 2.5, 8); // 3.4579 = Al (11.7)
		RootHelper::deleteObject("parabola");
		ParabolaPdf* parabola = new ParabolaPdf("parabola", "Fermi gas", *observable, *E_0, *parabolaRoot);
		components->add(*parabola);
		pdfsToBeConvoluted->add(*parabola);
	}

	// Gauss PDFs
	RooRealVar** gaussA = new RooRealVar*[numGauss];
	GaussianPdf** gauss = new GaussianPdf*[numGauss];
	RooRealVar** I_gauss = new RooRealVar*[numGauss];
	Double_t aMin = 0.01; // [KeV]
	Double_t aMax = 50; // [KeV]
	for (int i = 0; i < numGauss; i++) {
		RootHelper::deleteObject(Form("gauss%d_A", i + 1));
		gaussA[i] = new RooRealVar(Form("gauss%d_A", i + 1), Form("Gauss%d A", i + 1), getDefaultGaussAs(numGauss)[i], aMin, aMax, "A"); // 5 0.5 10

		const char* name = StringUtils::suffix("gauss", i + 1)->Data();
		RootHelper::deleteObject(name);
		gauss[i] = new GaussianPdf(name, StringUtils::ordinal("gauss", i + 1)->Data(), *observable, *E_0, *gaussA[i]);
		components->add(*gauss[i]);
		pdfsToBeConvoluted->add(*gauss[i]);
	}

	// Composite Lorentz PDFs
	RooRealVar** a2 = new RooRealVar*[numLorentzSum];
	DampLorentzPdf** lorentz2 = new DampLorentzPdf*[numLorentzSum];
	RooRealVar** I_lorentz2 = new RooRealVar*[numLorentzSum];
	for (int i = 0; i < numLorentzSum; i++) {
		RootHelper::deleteObject(Form("sl%dA", i + 1));
		a2[i] = new RooRealVar(Form("sl%dA", i + 1), Form("Sum Lorentz%d a", i + 1), getDefaultDampLorentzAs(numLorentzSum)[i], aMin, aMax, "A"); // 5 0.5 10

		const char* name = StringUtils::suffix("dampLorentz", i + 1)->Data();
		RootHelper::deleteObject(name);
		lorentz2[i] = new DampLorentzPdf(name, StringUtils::ordinal("damping lorentzian", i + 1)->Data(), *observable, *E_0, *a2[i]);
		components->add(*lorentz2[i]);
		pdfsToBeConvoluted->add(*lorentz2[i]);
	}

	// Lorentz PDFs
	RooRealVar** a = new RooRealVar*[numLorentz];
	LorentzianPdf** lorentz = new LorentzianPdf*[numLorentz];
	RooRealVar** I_lorentz = new RooRealVar*[numLorentz];
	for (int i = 0; i < numLorentz; i++) {
		RootHelper::deleteObject(Form("l%dA", i + 1));
		a[i] = new RooRealVar(Form("l%dA", i + 1), Form("Lorentz%d a", i + 1), getDefaultLorentzAs(numLorentz)[i], aMin, aMax, "A"); // 5 0.5 10

		const char* name = StringUtils::suffix("lorentz", i + 1)->Data();
		RootHelper::deleteObject(name);
		lorentz[i] = new LorentzianPdf(name, StringUtils::ordinal("lorentzian", i + 1)->Data(), *observable, *E_0, *a[i]);
		components->add(*lorentz[i]);
		pdfsToBeConvoluted->add(*lorentz[i]);
	}

	// Orthogonal PDF
	// if (hasOrthogonal){
	//     RootHelper::deleteObject("a1");
	//     RooRealVar* a1 = new RooRealVar("a1", "a1 coeff", 1, 1E-2, 1E2, "A");
	//     RootHelper::deleteObject("a2");
	//     RooRealVar* a2 = new RooRealVar("a2", "a2 coeff", 1, 1E-2, 1E2, "A");
	//     RootHelper::deleteObject("orthogonal");
	//     OrthogonalPdf* orthogonalPdf = new OrthogonalPdf("orthogonal", "Orthogonal Pdf", *x, *x0, *a1, *a2);
	//     components->add(*orthogonalPdf);
	// }

	//
	if (fwhm != NULL){

	}
	else {

	}
	RootHelper::deleteObject("sumModel");
	RooAddPdf* sumModel = ReverseAddPdf::add(*components, *coeffList, kTRUE);

	this->model = sumModel;
}

void CompositeModelProvider::initBackground(Double_t backgroundFraction) {
	// Ore-Powell background
//    RooRealVar* threeGammaInt = new RooRealVar("threeGammaInt", "Three Gamma fraction", 10, 1, 100);
//    BackgroundPdf* bgPdf = new BackgroundPdf("bgPdf", "Ore-Powell background", *observable, *threeGammaInt);
//    RooRealVar* bgPdfFraction = new RooRealVar("bgPdfFraction", "Background fraction", backgroundFraction, backgroundFraction/10, 1.);
//    bgComponents->add(*bgPdf);
//    components->add(*bgPdf);
//    pdfList->add(*bgPdf);
//    coeffList->add(*bgPdfFraction);

	// Constant background
	// RooGenericPdf* constBg = new RooGenericPdf("constBg", "1", *observable);
	RooPolynomial* constBg = new RooPolynomial("constBg", "y=1", *observable, RooArgSet());
	RooRealVar* I_const = new RooRealVar("IConst", "Const background", backgroundFraction / 2, 0, 0.2);
	bgComponents->add(*constBg);

	// Atan background
	RooGenericPdf* atanBg = new RooGenericPdf("atanBg", "Atan background", "@2/2 + (-1)*atan((@0 - @1))", RooArgList(*observable, *E_0, *pi));
	RooRealVar* I_atan = new RooRealVar("IAtan", "Atan Intensity", 0.002, 0.0, 0.1); //backgroundFraction/2, 0, 0.2);
	bgComponents->add(*atanBg);

//    RooAddPdf* sumModelAndBg = new RooAddPdf("sumModelAndBg", "Sum of components and Background", *constBg, *model, *I_const);
	RooAddPdf* sumModelAndBg = new RooAddPdf("sumModelAndBg", "Sum of components and Background", RooArgList(*atanBg, *constBg, *model), RooArgList(*I_atan, *I_const));

	this->model = sumModelAndBg;
}

RooAbsPdf* CompositeModelProvider::getConvolutedPdf(RooAbsPdf* pdf, RooRealVar* fwhm){

}

void CompositeModelProvider::initResolutionFunction(RooRealVar* fwhm) {
	// Resolution Function
	RooRealVar* resFunctMean = E_0;
	RootHelper::deleteObject("resFunctSigma");
	RooFormulaVar* resFunctSigma = new RooFormulaVar("resFunctSigma", "@0*@1", RooArgList(*fwhm, *fwhm2sigma));
	RootHelper::deleteObject("resFunct");
	if () {
		resolutionFunction = fwhm != NULL ?
				NULL
				new RooGaussian("resFunct", "Resolution Function", *observable, *resFunctMean, *resFunctSigma);
	} else {
		resolutionFunction = nullptr;
	}

	// Convolution
	RootHelper::deleteObject("sumModelConvoluted");
	switch (convType) {
	case 0:         // No convolution
		this->convolutedModel = nullptr;
		break;
	case 1:         // FFT3
	{   // https://stackoverflow.com/questions/92396/why-cant-variables-be-declared-in-a-switch-statement
		RooArgList* convPdfList = new RooArgList();
		TIterator* pdfIter = components->createIterator();
		while (TObject* tempObj = pdfIter->Next()) {
			RooAbsPdf* absPdf = dynamic_cast<RooAbsPdf*>(tempObj);
			if (absPdf != NULL) {
				Int_t i = pdfList->index(absPdf);
				RootHelper::deleteObject(Form("convAbsPdf%d", i + 1));
				RooFFTConvPdf* convAbsPdf = new RooFFTConvPdf(Form("convAbsPdf%d", i + 1), "convoluted_pdf", *observable, *absPdf, *resolutionFunction);
				convPdfList->add(*convAbsPdf);
			}
		};
		std::cout << "convPdfList" << std::endl;
		convPdfList->Print();
		this->convolutedModel = new RooAddPdf("sumModelConvoluted", "Sum of components", *convPdfList, *coeffList, kTRUE);
		break;
	}
//	case 2:         // Numeric
//		sumModelConvoluted = new RooNumConvPdf("sumModelConvoluted", "Convoluted Model", *observable, *model, *resolutionFunction);
//		((RooNumConvPdf*) sumModelConvoluted)->setConvolutionWindow(*resFunctMean, *resFunctSigma, 4);
//		this->convolutedModel = sumModelConvoluted;
//		break;
//	case 3:         // Custom
//		sumModelConvoluted = new ChannelConvolutionPdf("sumModelConvoluted", "Convoluted Model", *observable, *model, *resolutionFunction);
//		((ChannelConvolutionPdf*) sumModelConvoluted)->setConvolutionWindow(*resFunctMean,*resFunctFWHM,2);
//		this->convolutedModel = sumModelConvoluted;
//		break;
	}
}

void CompositeModelProvider::initTwoDetector(Bool_t hasParabola, const Int_t numGauss, const Int_t numLorentz, const Int_t numLorentzSum, RooRealVar* fwhm) {
	isTwoDetector = kTRUE;
	initModel(hasParabola, numGauss, numLorentz, numLorentzSum, fwhm);
}

void CompositeModelProvider::initSingleDetector(Bool_t hasParabola, const Int_t numGauss, const Int_t numLorentz, const Int_t numLorentzSum, RooRealVar* fwhm, Double_t bgFraction) {
	isTwoDetector = kFALSE;
	initModel(hasParabola, numGauss, numLorentz, numLorentzSum, fwhm);
	initBackground(bgFraction);
}

std::list<Variable*> CompositeModelProvider::getIndirectParameters() {
	std::list<Variable*> parameters;
	TIterator* it = pdfList->createIterator();
	TObject* tempObj = 0;
//    std::cout << "getIndirectParameters: Iterating PDFs" << std::endl;
	while ((tempObj = it->Next())) {
//        tempObj->Print();
		IndirectParamPdf* paramPdf = dynamic_cast<IndirectParamPdf*>(tempObj);
		if (paramPdf != NULL) {
			std::list<Variable*> list = paramPdf->getParameters(isTwoDetector);
			std::list<Variable*>::iterator iter;
			for (iter = list.begin(); iter != list.end(); iter++) {
				Variable* v = *iter;
//                v->print();
				parameters.push_back(v);
			}
		}
	}
	return parameters;
}

std::map<Int_t, TString> CompositeModelProvider::convolutionType = CompositeModelProvider::createConvolutionType();

std::map<Int_t, TString> CompositeModelProvider::getConvolutionTypes(void) {
	return convolutionType;
}

Double_t* CompositeModelProvider::getDefaultGaussAs(const Int_t numGauss) {
	Double_t* As;
	switch (numGauss) {
	case 1:
		static Double_t arr1[1] = { 0.1 };
		As = &arr1[0];
		break;
	case 2:
		static Double_t arr2[2] = { 0.5, 0.1 };
		As = &arr2[0];
		break;
	case 3:
		static Double_t arr3[3] = { 0.6, 0.3, 0.1 };
		As = &arr3[0];
		break;
	default:
		static Double_t arr4[4] = { 1.0, 0.5, 0.2, 0.05 };
		As = &arr4[0];
		break;
	}
	return As;
}

Double_t* CompositeModelProvider::getDefaultLorentzAs(const Int_t numLorentz) {
	Double_t* As;
	switch (numLorentz) {
	case 1:
		static Double_t arr1[1] = { 0.18 };
		As = &arr1[0];
		break;
	case 2:
		static Double_t arr2[2] = { 0.16, 0.5 };
		As = &arr2[0];
//            As = (Double_t[2]){Constants::a_B*TMath::Sqrt(Constants::Ry/73), Constants::a_B*TMath::Sqrt(Constants::Ry/117)};
		break;
	case 3:
		static Double_t arr3[3] = { 0.05, 0.1, 0.5 };
		As = &arr3[0];
//            As = (Double_t[3]){Constants::a_B*TMath::Sqrt(Constants::Ry/73), Constants::a_B*TMath::Sqrt(Constants::Ry/117), Constants::a_B*TMath::Sqrt(Constants::Ry/1559)};
		break;
	default:
		static Double_t arr4[4] = { 0.05, 0.1, 0.5, 1 };
		As = &arr4[0];
		break;
	}
	return As;
}
;

Double_t* CompositeModelProvider::getDefaultDampLorentzAs(const Int_t numLorentz) {
	Double_t* As;
	switch (numLorentz) {
	case 1:
		static Double_t arr1[1] = { 0.1 };
		As = &arr1[0];
		break;
	case 2:
		static Double_t arr2[2] = { 0.1, 0.4 };
		As = &arr2[0];
		break;
	case 3:
		static Double_t arr3[3] = { 0.05, 0.1, 0.5 };
		As = &arr3[0];
		break;
	default:
		static Double_t arr4[4] = { 0.05, 0.1, 0.5, 1 };
		As = &arr4[0];
		break;
	}
	return As;
}
;
