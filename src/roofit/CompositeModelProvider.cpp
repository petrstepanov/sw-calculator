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

CompositeModelProvider::CompositeModelProvider(RooRealVar* x, RooRealVar* x0) : AbstractModelProvider(x0) {
	observable = x;
//	pdfsInMaterial = new RooArgList();
//	coeffsInMaterial = new RooArgList();
//	isTwoDetector = kTRUE;
}

// Add source contribution pdf to the model
void CompositeModelProvider::initSourcePdf(TH1F* sourceHist, RooRealVar* sourceContribution) {
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
	this->sourcePdf = new RooHistPdf("sourcePdf", "Source", *observable, *sourceDataHist, 1);
	this->intSourcePdf = sourceContribution;
}

void CompositeModelProvider::initModel(Bool_t hasParabola, const Int_t numGauss, const Int_t numLorentz, const Int_t numLorentzSum) {
	pdfsInMaterial = new RooArgList();
	coeffsInMaterial = new RooArgList();
	model = nullptr;

	// Parabola PDF
	if (hasParabola) {
		RootHelper::deleteObject("parabolaRoot");
		RooRealVar* parabolaRoot = new RooRealVar("parabolaRoot", "Coefficient at -x^2 + r*2", 3.5, 2.5, 8); // 3.4579 = Al (11.7)
		RootHelper::deleteObject("parabola");
		ParabolaPdf* parabola = new ParabolaPdf("parabola", "Fermi gas", *observable, *mean, *parabolaRoot);
		components->add(*parabola);
		pdfsInMaterial->add(*parabola);
		RooRealVar* Int_parabola = new RooRealVar("Int_parabola", "Parabola intensity", 0.9, 0.0, 1.0);
		coeffsInMaterial->add(*Int_parabola);
	}

	// Gauss PDFs
	RooRealVar** gaussA = new RooRealVar*[numGauss];
	GaussianPdf** gauss = new GaussianPdf*[numGauss];
	RooRealVar** Int_gauss = new RooRealVar*[numGauss];
	Double_t aMin = 0.01; // [KeV]
	Double_t aMax = 50; // [KeV]
	for (int i = 0; i < numGauss; i++) {
		RootHelper::deleteObject(Form("gauss%d_A", i + 1));
		gaussA[i] = new RooRealVar(Form("gauss%d_A", i + 1), Form("Gauss%d A", i + 1), getDefaultGaussAs(numGauss)[i], aMin, aMax, "A"); // 5 0.5 10

		const char* pdfName = StringUtils::suffix("gauss", i + 1)->Data();
		RootHelper::deleteObject(pdfName);
		gauss[i] = new GaussianPdf(pdfName, StringUtils::ordinal("gauss", i + 1)->Data(), *observable, *mean, *gaussA[i]);
		components->add(*gauss[i]);
		pdfsInMaterial->add(*gauss[i]);

		const char* coeffName = StringUtils::suffix("Int_gauss", i + 1)->Data();
		RootHelper::deleteObject(coeffName);
		Int_gauss[i] = new RooRealVar(coeffName, StringUtils::ordinal("gauss intensity", i + 1)->Data(), 0.5, 0.0, 1.0);
		coeffsInMaterial->add(*Int_gauss[i]);
	}

	// Lorentz PDFs
	RooRealVar** a = new RooRealVar*[numLorentz];
	LorentzianPdf** lorentz = new LorentzianPdf*[numLorentz];
	RooRealVar** Int_lorentz = new RooRealVar*[numLorentz];
	for (int i = 0; i < numLorentz; i++) {
		RootHelper::deleteObject(Form("l%dA", i + 1));
		a[i] = new RooRealVar(Form("l%dA", i + 1), Form("Lorentz%d a", i + 1), getDefaultLorentzAs(numLorentz)[i], aMin, aMax, "A"); // 5 0.5 10

		const char* pdfName = StringUtils::suffix("lorentz", i + 1)->Data();
		RootHelper::deleteObject(pdfName);
		lorentz[i] = new LorentzianPdf(pdfName, StringUtils::ordinal("lorentzian", i + 1)->Data(), *observable, *mean, *a[i]);
		components->add(*lorentz[i]);
		pdfsInMaterial->add(*lorentz[i]);

		const char* coeffName = StringUtils::suffix("Int_lorentz", i + 1)->Data();
		RootHelper::deleteObject(coeffName);
		Int_lorentz[i] = new RooRealVar(coeffName, StringUtils::ordinal("lorentzian intensity", i + 1)->Data(), 0.75, 0.0, 1.0);
		coeffsInMaterial->add(*Int_lorentz[i]);
	}

	// Damping Lorentz PDFs
	RooRealVar** a2 = new RooRealVar*[numLorentzSum];
	DampLorentzPdf** lorentz2 = new DampLorentzPdf*[numLorentzSum];
	RooRealVar** Int_lorentz2 = new RooRealVar*[numLorentzSum];
	for (int i = 0; i < numLorentzSum; i++) {
		RootHelper::deleteObject(Form("sl%dA", i + 1));
		a2[i] = new RooRealVar(Form("sl%dA", i + 1), Form("Sum Lorentz%d a", i + 1), getDefaultDampLorentzAs(numLorentzSum)[i], aMin, aMax, "A"); // 5 0.5 10

		const char* pdfName = StringUtils::suffix("dampLorentz", i + 1)->Data();
		RootHelper::deleteObject(pdfName);
		lorentz2[i] = new DampLorentzPdf(pdfName, StringUtils::ordinal("damping lorentzian", i + 1)->Data(), *observable, *mean, *a2[i]);
		components->add(*lorentz2[i]);
		pdfsInMaterial->add(*lorentz2[i]);

		const char* coeffName = StringUtils::suffix("Int_dampLorentz", i + 1)->Data();
		RootHelper::deleteObject(coeffName);
		Int_lorentz2[i] = new RooRealVar(coeffName, StringUtils::ordinal("damping lorentzian intensity", i + 1)->Data(), 0.75, 0.0, 1.0);
		coeffsInMaterial->add(*Int_lorentz2[i]);
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
	//     pdfsInMaterial->add(*orthogonalPdf);
	//     RooRealVar* Int_ortho = new RooRealVar("Int_ortho", "Orthogonal intensity", 0.3, 0.0, 1.0);
	//     coeffsInMaterial->add(*Int_ortho);
	// }

    // Remove last coefficient - refer to RooAddPdf manual
	Int_t coeffsNumber = coeffsInMaterial->getSize();
	RooAbsArg* lastCoeff = coeffsInMaterial->at(coeffsNumber-1);
	coeffsInMaterial->remove(*lastCoeff);
	coeffsInMaterial->Print();

	Debug("CompositeModelProvider::initModel", "List of pdfs in material:");
	#ifdef USEDEBUG
		pdfsInMaterial->Print("V");
	#endif
	Debug("List of intensities in material:");
	#ifdef USEDEBUG
		coeffsInMaterial->Print("V");
	#endif

	RootHelper::deleteObject("sumModel");
	model = new RooAddPdf("sumModel", "Sum of components in material", *pdfsInMaterial, *coeffsInMaterial, kTRUE);

	// Add PDF from annihilation in source if needed
	if (sourcePdf && intSourcePdf->getVal() != 0) {
		components->add(*sourcePdf);
        intSourcePdfNorm = new RooFormulaVar("intSourcePdfNorm", "sCN", "@0/100", RooArgList(*intSourcePdf));
		RootHelper::deleteObject("sumModelSource");
		model = new RooAddPdf("sumModelSource", "Sum of components in material with source contribution", RooArgList(*sourcePdf, *model), RooArgList(*intSourcePdfNorm));
	}
}

void CompositeModelProvider::initConvolutedModel(RooRealVar* fwhm) {
	Debug("CompositeModelProvider::initConvolutedModel", "1");

	if (fwhm == nullptr) {
		resolutionFunction = nullptr;
		convolutedModel = nullptr;
		return;
	}

	// Make resolution Function
	RootHelper::deleteObject("resFunctSigma");
	RooFormulaVar* resFunctSigma = new RooFormulaVar("resFunctSigma", "@0*@1", RooArgList(*fwhm, *Constants::fwhm2sigma));
	RootHelper::deleteObject("resFunct");
	resolutionFunction = new RooGaussian("resFunct", "Resolution function", *observable, *mean, *resFunctSigma);

	// Convolute and sum pdfs in material
	RooArgList* convolutedPdfsInMaterial = new RooArgList();
	TIterator* it = pdfsInMaterial->createIterator();
	while(TObject* temp = it->Next()){
		if(RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(temp)){
			const char* convPdfName = Form("%sConv", pdf->GetName());
			const char* convPdfTitle = Form("%s Convoluted", pdf->GetTitle());
			RootHelper::deleteObject(convPdfName);
			RooFFTConvPdf* convPdf = new RooFFTConvPdf(convPdfName, convPdfTitle, *observable, *pdf, *resolutionFunction);
			convolutedPdfsInMaterial->add(*convPdf);
		}
	}

	RootHelper::deleteObject("sumModelConv");
	convolutedModel = new RooAddPdf("sumModelConv", "Sum of convoluted components in material", *convolutedPdfsInMaterial, *coeffsInMaterial, kTRUE);

	// Add PDF from annihilation in source if needed
	if (sourcePdf && intSourcePdf->getValV() != 0) {
		RootHelper::deleteObject("sumModelConvSource");
		convolutedModel = new RooAddPdf("sumModelConvSource", "Sum of convoluted components in material with source contribution", RooArgList(*sourcePdf, *convolutedModel), RooArgList(*intSourcePdfNorm));
	}
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
	RootHelper::deleteObject("constBg");
	RooPolynomial* constBg = new RooPolynomial("constBg", "y=1", *observable, RooArgSet());
	RootHelper::deleteObject("Int_Const");
	RooRealVar* Int_const = new RooRealVar("Int_Const", "Constant background", backgroundFraction, backgroundFraction / 10, backgroundFraction *2);
	bgComponents->add(*constBg);

	// Atan background
	RootHelper::deleteObject("atanBg");
	RooGenericPdf* atanBg = new RooGenericPdf("atanBg", "Atan background", "@2/2 + (-1)*atan((@0 - @1))", RooArgList(*observable, *mean, *Constants::pi));
	RootHelper::deleteObject("Int_Atan");
	RooRealVar* Int_atan = new RooRealVar("Int_Atan", "Atan Intensity", 0.002, 0.0, 0.1);
	bgComponents->add(*atanBg);

	RootHelper::deleteObject("sumModelAndBg");
	this->model = new RooAddPdf("sumModelAndBg", "Sum of model and background", RooArgList(*atanBg, *constBg, *model), RooArgList(*Int_atan, *Int_const));
	RootHelper::deleteObject("sumConvModelAndBg");
	this->convolutedModel = new RooAddPdf("sumConvModelAndBg", "Sum of convoluted model and Background", RooArgList(*atanBg, *constBg, *convolutedModel), RooArgList(*Int_atan, *Int_const));
}

void CompositeModelProvider::initTwoDetector(Bool_t hasParabola, const Int_t numGauss, const Int_t numLorentz, const Int_t numLorentzSum, RooRealVar* fwhm) {
	isTwoDetector = kTRUE;
	initModel(hasParabola, numGauss, numLorentz, numLorentzSum);
	initConvolutedModel(fwhm);
	Debug("CompositeModelProvider::initTwoDetector", "finished");
}

void CompositeModelProvider::initSingleDetector(Bool_t hasParabola, const Int_t numGauss, const Int_t numLorentz, const Int_t numLorentzSum, RooRealVar* fwhm, Double_t bgFraction) {
	isTwoDetector = kFALSE;
	initModel(hasParabola, numGauss, numLorentz, numLorentzSum);
	initConvolutedModel(fwhm);
	initBackground(bgFraction);
}

RooArgList* CompositeModelProvider::getIndirectParameters() {
	RooArgList* parameters = new RooArgList();
	TIterator* it = components->createIterator();
	while (TObject* tempObj = it->Next()) {
		if (IndirectParamPdf* paramPdf = dynamic_cast<IndirectParamPdf*>(tempObj)) {
			RooArgList* list = paramPdf->getParameters(isTwoDetector);
			parameters->add(*list);
		}
	}
	return parameters;
}

RooArgList* CompositeModelProvider::getIntensities() {
	RooArgList* intensities = new RooArgList();

//	// Add material pdfs intensities (for non-recursive sum)
//	Double_t total = 0.;
//	for (unsigned i=0; i < pdfsInMaterial->getSize(); i++){
//		if (RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(pdfsInMaterial->at(i))){
//			const char* name = Form("intensity%d", i);
//			const char* title = Form("%s intensity", pdf->GetTitle());
//			RootHelper::deleteObject(name);
//			RooRealVar* v = new RooRealVar(name, title, 0, "%");
//			if (i!=pdfsInMaterial->getSize()-1){
//				// for all pdfs but not last
//				if (RooRealVar* coeff = dynamic_cast<RooRealVar*>(coeffsInMaterial->at(i))){
//					Double_t pdfIntensity = coeff->getValV();
//					total += pdfIntensity;
//					v->setVal(pdfIntensity*100);
//					v->setError(coeff->getError()*100);
//				}
//			} else {
//				// for last pdf in the list
//				v->setVal((1.-total)*100);
//			}
//			intensities->add(*v);
//		}
//	}

	// Add material pdfs intensities (for recursive sum)
	Double_t previousCoeff = 1;   // previouscoefficient
	Double_t previousCoeffError = 0;  // previous coefficient error
	for (unsigned i=0; i < pdfsInMaterial->getSize(); i++){
		if (RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(pdfsInMaterial->at(i))){
			const char* name = Form("intensity%d", i);
			const char* title = Form("%s intensity", pdf->GetTitle());
			RootHelper::deleteObject(name);
			RooRealVar* v = new RooRealVar(name, title, 0, "%");
			if (i!=pdfsInMaterial->getSize()-1){
				// for all pdfs but not last
				if (RooRealVar* coeff = dynamic_cast<RooRealVar*>(coeffsInMaterial->at(i))){
					Double_t pdfIntensity = coeff->getValV();
					Double_t pdfIntensityError = coeff->getError();
					v->setVal(previousCoeff*pdfIntensity*100);
					Double_t error = sqrt(pow(pdfIntensity*previousCoeffError,2) + pow(previousCoeff*pdfIntensityError,2));
					v->setError(error*100);
					previousCoeff*= 1.-pdfIntensity;
					previousCoeffError = error;
				}
			} else {
				// for last pdf in the list
				v->setVal(previousCoeff*100);
			}
			intensities->add(*v);
		}
	}

	Debug("CompositeModelProvider::getIntensities", "list of intensities:");
	#ifdef USEDEBUG
		intensities->Print("V");
	#endif
	return intensities;
}

RooRealVar* CompositeModelProvider::getSourceContribution() {
	if (sourcePdf==nullptr || intSourcePdf->getVal() == 0) return nullptr;
	const char* name = "intensitySource";
	const char* title = Form("%s intensity", sourcePdf->GetTitle());
	RootHelper::deleteObject(name);
	RooRealVar* v = new RooRealVar(name, title, intSourcePdf->getValV(), "%");
	v->setError(intSourcePdf->getError());
	return v;
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
		static Double_t arr4[4] = { 1.5, 1, 0.5, 0.1 };
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
