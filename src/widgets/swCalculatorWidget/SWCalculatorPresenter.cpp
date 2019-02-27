/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SWCalculatorPresenter.cpp
 * Author: petrstepanov
 * 
 * Created on October 6, 2017, 6:31 PM
 */

#include "SWCalculatorPresenter.h"
#include "SWCalculatorView.h"
#include "../../model/Model.h"
#include "../../event/events/HistogramImportedEvent.h"
#include "../../event/EventBus.h"
#include "../../event/HandlerRegistration.h"
#include "../../util/RootHelper.h"
#include "../../util/HistProcessor.h"
#include "../../util/GraphicsHelper.h"
#include "../../util/StringUtils.h"
#include "../../util/UiHelper.h"
#include "../../util/Debug.h"
#include "../../roofit/CompositeModelProvider.h"
#include "../../model/Constants.h"
#include <RooFit.h>
#include <TPaveText.h>
#include <RooDataHist.h>
#include <RooChi2Var.h>
#include <RooMinimizer.h>
#include <RooFitResult.h>
#include <RooRealVar.h>
#include <RooArgSet.h>
#include <TMath.h>
#include <TLegend.h>
#include <iostream>
#include "../rooRealVarWidget/RooRealVarView.h"

SWCalculatorPresenter::SWCalculatorPresenter(SWCalculatorView* view) :
		AbstractPresenter<Model, SWCalculatorView>(view) {
	// Need to instantinate RooRealVarView model for source Contribution
//    Model* model = getModel();
}

Model* SWCalculatorPresenter::instantinateModel() {
	return Model::getInstance();
}

void SWCalculatorPresenter::onInitModel() {
	std::cout << "SWCalculatorPresenter::onInitModel" << std::endl;
}

void SWCalculatorPresenter::addEventListeners() {
	SWCalculatorView* view = getView();
	Model* model = getModel();

	class HistogramImportedEventListener: public EventHandler<HistogramImportedEvent> {
	public:
		HistogramImportedEventListener(SWCalculatorView* v) :
				view(v) {
		}

		virtual void onEvent(HistogramImportedEvent & e) override {
			std::cout << "HistogramImportedEventListener::onEvent()" << std::endl;
			view->setFitMinMaxRange(e.getHistMin() + 0.5, e.getHistMax());
			view->setTabEnabled(1, true);
			view->setTabEnabled(2, true);
		}
		SWCalculatorView* view = nullptr;
	};
	EventBus::AddHandler<HistogramImportedEvent>(*(new HistogramImportedEventListener(view)));

	class SourceHistogramImportedEventListener: public EventHandler<SourceHistogramImportedEvent> {
	public:
		SourceHistogramImportedEventListener(SWCalculatorView* v, Model* m) :
				view(v), model(m) {
		}

		virtual void onEvent(SourceHistogramImportedEvent & e) override {
			std::cout << "SourceHistogramImportedEventListener::onEvent()" << std::endl;
			RooRealVarView* sourceContributionView = view->getSourceContributionView();
			sourceContributionView->getPresenter()->setModel(model->getSourceContribution());
			view->setSourceContributionFrameVisible(kTRUE);
		}
		SWCalculatorView* view = nullptr;
		Model* model = nullptr;
	};
	EventBus::AddHandler<SourceHistogramImportedEvent>(*(new SourceHistogramImportedEventListener(view, model)));

	class IsTwoDetectorEventListener: public EventHandler<IsTwoDetectorEvent> {
	public:
		IsTwoDetectorEventListener(SWCalculatorView* v) :
				view(v) {
		}

		virtual void onEvent(IsTwoDetectorEvent & e) override {
			std::cout << "IsTwoDetectorEvent::onEvent()" << std::endl;
			view->updateRegionLabels(e.isTwoDetector());
			view->setFitMinMaxValues(e.isTwoDetector());
		}
		SWCalculatorView* view = nullptr;
	};
	EventBus::AddHandler<IsTwoDetectorEvent>(*(new IsTwoDetectorEventListener(view)));
}

void SWCalculatorPresenter::onFitSpectrumClicked() {
	// Start Tracking Time
	RootHelper::startTimer();

	// Get UI values
	SWCalculatorView* view = getView();
	Int_t fitMin = view->getFitMinValue();
	Int_t fitMax = view->getFitMaxValue();

	// TODO: Set Display Range same as Fit Range
	// TODO: Enable bottom Toolbar (add sliders)

	// Construct histogram with limits [fitMin, fitMax]
	// Important for 1D Doppler spectra where spectrum range can be [0, 1] MeV
	Model* model = getModel();
	TH1F* originalHist = model->getHist();
	HistProcessor* histProcessor = HistProcessor::getInstance();
	TH1F* fitHist = (TH1F*) histProcessor->cutHist("fitHist", originalHist, fitMin, fitMax);

	// Define energy axis
	RooRealVar* e = new RooRealVar("e", "Energy axis", fitHist->GetXaxis()->GetXmin(), fitHist->GetXaxis()->GetXmax(), "keV");
	// If fitting with ranges instead
	// x->setRange("fitRange", fitMin, fitMax);

	// Set binning for different types of convolution
	e->setBins(fitHist->GetNbinsX());
	e->setBins(fitHist->GetNbinsX(), "cache");

	// Define median for fitting functions as  coordinate of maximum histogram value
	RooRealVar* e0;
	{
		Double_t e0Val = fitHist->GetBinCenter(fitHist->GetMaximumBin());
		e0 = new RooRealVar("e0", "Spectrum Mean", e0Val, e0Val - 1, e0Val + 1, "keV");
	}

	// Convert Histogram to RooDataHist for Fitting
	RooDataHist* data = new RooDataHist("data", "Dataset with e", RooArgSet(*e), RooFit::Import(*fitHist));

	// Construct model provider (now using CompositeModelProvider)
	CompositeModelProvider* modelProvider;
	{
		Bool_t hasParabola = view->hasParabola();
		Int_t numGauss = view->getNumGauss();
		Int_t numExp = view->getNumExp();
		Int_t numDampExp = view->getNumDampExp();
		Double_t fwhm = view->getResolutionFWHM();

		if (hasParabola + numGauss + numExp + numDampExp == 0)
			return;

		RooRealVar* fwhmRealVar = nullptr;
		if (view->getConvolutionType() != 0) {
			RootHelper::deleteObject("resFunctFWHM");
			fwhmRealVar = new RooRealVar("resFunctFWHM", "Resolution function FWHM", fwhm, fwhm / 4, fwhm * 4, "keV");
			fwhmRealVar->setConstant(view->isResolutionFixed());
		}

		modelProvider = new CompositeModelProvider(e, e0);

		TH1F* kaptonHist = model->getSourceHist();
		if (kaptonHist) {
			RooRealVar* sourceContribution = model->getSourceContribution();
			modelProvider->initSourcePdf(kaptonHist, sourceContribution);
		}

		if (model->isTwoDetector()) {
			modelProvider->initTwoDetector(hasParabola, numGauss, numExp, numDampExp, fwhmRealVar);
		} else {
			Double_t bgFraction = histProcessor->calcBackgroundFraction(fitHist);
			modelProvider->initSingleDetector(hasParabola, numGauss, numExp, numDampExp, fwhmRealVar, bgFraction);
		}
	}

	// Obtain fitting model and convoluted model
	RooAbsPdf* fittingNonConvolutedModel = modelProvider->getModel();
	if (fittingNonConvolutedModel == nullptr)
		return;
	RooAbsPdf* fittingConvolutedModel = modelProvider->getConvolutedModel();
	RooAbsPdf* fittingModel = (fittingConvolutedModel) ? fittingConvolutedModel : fittingNonConvolutedModel;

	// Testing purposes - let model generate data
	// data = static_cast<RooAddPdf*>(model)->generateBinned(*x,1000000) ;

	// Fitting
	// Default chi2FitTo used when we use non-standard errors.
	// RooFitResult* fitResult = convolutedModel->chi2FitTo(*data, Save(kTRUE), Range("fitRange"), NumCPU(RootHelper::getNumCpu()));

	// The RooFit chi2 fit does not work when the bins have zero entries. (Two-detector setup)
	// You should either use a binned likelihood fit or use the standard chi2 fit provided by ROOT. In this case bins with zero entries are excluded from the fit

	// Chi2 fit
	Int_t numCpu = RootHelper::getNumCpu();
	RooChi2Var* chi2 = new RooChi2Var("chi2", "chi2", *fittingModel, *data, RooFit::NumCPU(numCpu));
	RooMinimizer* m = new RooMinimizer(*chi2);
	Int_t resultMigrad = m->migrad();
	Int_t resultHesse = m->hesse();
	Debug("SWCalculatorPresenter::onFitSpectrumClicked", "RooMinimizer: migrad=" << resultMigrad << ", hesse=" << resultHesse);

	RooFitResult* fitResult = m->save();

	// Create RooPlot from energy axis frame
	RooPlot* spectrumPlot = e->frame();
	spectrumPlot->SetTitle("");                             // Set Empty Graph Title
	spectrumPlot->GetXaxis()->SetRangeUser(fitMin, fitMax);      // Do we need this?

	// Configure axis labels and look
	GraphicsHelper* graphicsHelper = GraphicsHelper::getInstance();
	graphicsHelper->setupAxis(spectrumPlot->GetXaxis(), "E_{1} #minus E_{2}, keV", 2.2, 0.02); // Title, Title offset, Label offset
	graphicsHelper->setupAxis(spectrumPlot->GetYaxis(), "Counts", 1.6, 0.012);

	// Evaluate Counts axis limits
	Double_t yAxisMin, yAxisMax;
	if (model->isTwoDetector()) {
//        yAxisMin=0.1;
//        while (yAxisMin * 10 < fitHist->GetBinContent(fitHist->GetMinimumBin()) + 1) yAxisMin *= 10;        
//        Double_t logYAxisMax = 1.15 * TMath::Log10(fitHist->GetBinContent(fitHist->GetMaximumBin())); // Max limit is 15% larger hist maximum
//        yAxisMax = pow(10, logYAxisMax);  
		Double_t histMin = fitHist->GetBinContent(fitHist->GetMinimumBin());
		yAxisMin = histMin < 0 ? 0 : histMin;
		Double_t logYAxisMax = 1.15 * TMath::Log10(fitHist->GetBinContent(fitHist->GetMaximumBin())); // Max limit is 15% larger hist maximum
		yAxisMax = pow(10, logYAxisMax);

	} else {
		Double_t logMin = TMath::Log10(fitHist->GetBinContent(fitHist->GetMinimumBin()));
		yAxisMin = TMath::Power(10, logMin / 2);
		Double_t logYAxisMax = 1.05 * TMath::Log10(fitHist->GetBinContent(fitHist->GetMaximumBin())); // Max limit is 15% larger hist maximum
		yAxisMax = pow(10, logYAxisMax);
	}
	std::cout << "Counts axis limits: " << yAxisMin << ", " << yAxisMax << std::endl;

//    Double_t modelMean = histProcessor->getPdfMaximumX(fittingModel, RooArgList(*e));
	Double_t modelMean = modelProvider->getMean()->getValV();
	{
		// Draw S, W regions
		Double_t sWidth = view->getSWidth();
		Double_t wWidth = view->getWWidth();
		Double_t wShift = view->getWShift();
		// Energy value of the maximum of the convoluted model
		Bool_t isTwoDetector = model->isTwoDetector();

		graphicsHelper->drawSWRegions(spectrumPlot, sWidth, wWidth, wShift, modelMean, yAxisMin, yAxisMax, isTwoDetector);
	}

	// Plot data points first (in transparent color). Essential for normalization of PDFs
	// data->plotOn(spectrumPlot, RooFit::Invisible());
	data->plotOn(spectrumPlot, RooFit::LineColor(kGray + 3), RooFit::XErrorSize(0), RooFit::MarkerSize(0.5), RooFit::MarkerColor(kGray + 3), RooFit::Name("data"));

	// Initialize legend
	TLegend *legend = new TLegend(GraphicsHelper::LEGEND_X1, 0.5, 1 - 1.5 * GraphicsHelper::padMargins.right, 1 - 2 * GraphicsHelper::padMargins.top);
	legend->AddEntry(spectrumPlot->findObject("data"), "Data points", "pe");

	// Plot convoluted and unconvoluted models

	if (fittingConvolutedModel != NULL) {
		fittingConvolutedModel->plotOn(spectrumPlot, RooFit::LineColor(kOrange + 6), RooFit::LineWidth(2), RooFit::Name("fit"));
		legend->AddEntry(spectrumPlot->findObject("fit"), "Convoluted model", "l");
		fittingNonConvolutedModel->plotOn(spectrumPlot, RooFit::LineColor(kOrange + 6), RooFit::LineWidth(1), RooFit::LineStyle(kDashed), RooFit::Name("model"));
		legend->AddEntry(spectrumPlot->findObject("model"), "Unconvoluted model", "l");
	} else {
		fittingNonConvolutedModel->plotOn(spectrumPlot, RooFit::LineColor(kOrange + 6), RooFit::LineWidth(2), RooFit::Name("fit"));
		legend->AddEntry(spectrumPlot->findObject("fit"), "Unconvoluted model", "l");
	}
	RooCurve* curveFit = spectrumPlot->getCurve("fit");

	// Plot unconvoluted model components
	{
		RooArgList* components = modelProvider->getComponents();
		TIterator* it = components->createIterator();
		Int_t i = 0;
		while (TObject* tempObject = it->Next()) {
			RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(tempObject);
			if (pdf) {
				RooArgSet* argSet = new RooArgSet();
				argSet->add(*pdf);
				fittingNonConvolutedModel->plotOn(spectrumPlot, RooFit::Components(*argSet), RooFit::LineStyle(kDashed), RooFit::LineColor(GraphicsHelper::colorSet[i++]), RooFit::LineWidth(1),
						RooFit::Name(pdf->GetName()));
				legend->AddEntry(spectrumPlot->findObject(pdf->GetName()), pdf->GetTitle(), "l");
			}
		}
	}

	// Plot Resolution Function
	RooAbsPdf* resolutionFunction = modelProvider->getResolutionFuncton();
	if (resolutionFunction != NULL) {
		resolutionFunction->plotOn(spectrumPlot, RooFit::LineStyle(kDashed), RooFit::LineColor(kGray), RooFit::LineWidth(1), RooFit::Name("rf")); //, RooFit::Normalization(totalFitCounts, RooAbsReal::NumEvent));
		legend->AddEntry(spectrumPlot->findObject("rf"), "Resolution function", "l");
	}

	// This histogram is used for calculating S and W parameters
	TH1F* fitHistNoBg = fitHist;

	// Single-dimentional experiment case - plot background
	if (!model->isTwoDetector()) {
		// Plot Convoluted Model background (its added after convolution graph because otherwise it changes backgroud)
		fittingNonConvolutedModel->plotOn(spectrumPlot, RooFit::Components(*(modelProvider->getBgComponents())), RooFit::LineStyle(kDashed), RooFit::LineColor(kPink - 4), RooFit::LineWidth(1),
				RooFit::Name("bg"));
		legend->AddEntry(spectrumPlot->findObject("bg"), "Atan background", "l");
		// Plot fit without background
		RooCurve* curveBg = spectrumPlot->getCurve("bg");
		// RooCurve* curveFitNoBg = histProcessor->subtractCurves(curveFit, curveBg);
		// curveFitNoBg->SetLineColor(kGray);
		// curveFitNoBg->SetLineWidth(2);
		fitHistNoBg = (TH1F*) histProcessor->subtractCurve("fitHistNoBg", fitHist, curveBg);
		RooDataHist* dataNoBg = new RooDataHist("dataNoBg", "Dataset with e (no background)", RooArgSet(*e), RooFit::Import(*fitHistNoBg));
		dataNoBg->plotOn(spectrumPlot, RooFit::XErrorSize(0), RooFit::MarkerSize(0.5), RooFit::DataError(RooAbsData::None), RooFit::MarkerColor(kGray), RooFit::Name("dataNoBg"));
		spectrumPlot->drawBefore("data", "dataNoBg");
		legend->AddEntry(spectrumPlot->findObject("dataNoBg"), "Subtracted background", "p");
	}

	// Set legend's bottom coordinate depending on the number of entries
	legend->SetY1(legend->GetY2() - legend->GetNRows() * GraphicsHelper::LEGEND_LINE_HEIGHT);
	legend->SetFillStyle(0);  // transparent
	legend->SetBorderSize(0); // no border
	spectrumPlot->addObject(legend);

	// Plot data points
	// data->plotOn(spectrumPlot, RooFit::LineColor(kGray + 3), RooFit::XErrorSize(0), RooFit::MarkerSize(0.5),
	//     RooFit::MarkerColor(kGray + 3), RooFit::DataError(RooAbsData::SumW2)); // LineStyle(kSolid), LineWidth(2)

	// Print Plot names
	spectrumPlot->Print("v");

	// Set Y Axis range after all (otherwise throws error?)
	spectrumPlot->GetYaxis()->SetRangeUser(yAxisMin, yAxisMax);

	// Plot Bottom Frame with Fit Goodness
	TH1F* resHist = (TH1F*) histProcessor->getResidualHist("resHist", fitHist, curveFit);
	RooDataHist* chi2DataHist = new RooDataHist("chi2DataHist", "Chi2", RooArgSet(*e), resHist);

	// Create RooPlot for chi^2
	RooPlot* residualsPlot = e->frame();
	Double_t scaleFactor = (1 - GraphicsHelper::RESIDUALS_PAD_RELATIVE_HEIGHT) / (GraphicsHelper::RESIDUALS_PAD_RELATIVE_HEIGHT);
	residualsPlot->SetTitle("");                             // Set Empty Graph Title
	residualsPlot->GetXaxis()->SetRangeUser(fitMin, fitMax);      // Do we need this?
	graphicsHelper->setupAxis(residualsPlot->GetXaxis(), "", 2.5, 0.05); // Title, Title offset, Label offset
	graphicsHelper->setupAxis(residualsPlot->GetYaxis(), "Residuals", 1.6, 0.012); // "#chi^{2}"

	// Plot residuals
	chi2DataHist->plotOn(residualsPlot, RooFit::LineColor(kGray + 3), RooFit::XErrorSize(0), RooFit::DataError(RooAbsData::None), RooFit::MarkerSize(0.5), RooFit::MarkerColor(kGray + 3));

	// Plot chi2
	Chi2Struct chi2Struct = histProcessor->getChi2(fitHist, curveFit, fittingNonConvolutedModel);
	TPaveText* chiText = new TPaveText(GraphicsHelper::LEGEND_X1, 1 - 2 * GraphicsHelper::padMargins.right - 0.18, 1 - 1.2 * GraphicsHelper::padMargins.right, 1 - 2 * GraphicsHelper::padMargins.top, "NDC");
	chiText->AddText(Form("#chi^{2} = %.1f #divide %d = %.3f", chi2Struct.chiSum, chi2Struct.degreesOfFreedom, chi2Struct.chi2));
	chiText->SetTextSize(gStyle->GetLegendTextSize()); // make chi2 text same size like the legend
	chiText->SetTextFont(gStyle->GetLegendFont());
	chiText->SetFillColor(kWhite);
	chiText->SetBorderSize(0);
	chiText->SetTextAlign(kHAlignRight + kVAlignCenter);
	residualsPlot->addObject(chiText);

	// Draw data plot on canvas
	TPad* padData = view->getPadData();
	padData->cd();
	spectrumPlot->Draw();

	// Draw data plot on canvas
	TPad* padChi2 = view->getPadChi2();
	padChi2->cd();
	residualsPlot->Draw();

	// Update canvas
	view->updateCanvas();

	// Output Parameters to the View
	{
		SWCalculatorView* view = getView();
		Model* model = getModel();

		// Output filename
		view->displayFilename(model->getFileName());
		// Output model parameters (RooRealVars')
		view->displayFitParameters(fitResult);
		// Output indirect model parameters
		view->displayVariables(modelProvider->getIndirectParameters());
		// Output components intensities
		view->displayVariables(modelProvider->getIntensities());
		view->displayVariable(modelProvider->getSourceContribution());
		// Output Integral Chi^2
		view->displayChi2(chi2Struct);

		// Output S and W values
		Double_t sWidth = view->getSWidth();
		Double_t wWidth = view->getWWidth();
		Double_t wShift = view->getWShift();
		Bool_t isTwoDetector = model->isTwoDetector();
		std::pair<Double_t, Double_t> sValueError = histProcessor->getSParameter(fitHistNoBg, sWidth, modelMean, isTwoDetector);
		std::pair<Double_t, Double_t> wValueError = histProcessor->getWParameter(fitHistNoBg, wWidth, wShift, modelMean, isTwoDetector);
		view->displaySW(sValueError, wValueError);
		view->scrollOutputDown();
	}

	view->initRooPlots(spectrumPlot, residualsPlot);
	view->setToolbarEnabled(kTRUE);
	view->setDisplayLimits(fitMin, fitMax);
	RootHelper::stopAndPrintTimer();
}

void SWCalculatorPresenter::onSaveImageClicked() {
	Model* model = getModel();
	SWCalculatorView* view = getView();

	// Create image file names
	TString* filePath = model->getFileName();
	TString* filePathNoExtension = StringUtils::stripFileExtension(filePath);
	if (model->getSourceHist()) {
		*filePathNoExtension += "-kapton";
	}
	TString pngFilePath = *filePathNoExtension + ".png";
	TString pdfFilePath = *filePathNoExtension + ".pdf";

	// Save raster and vector images
	TCanvas* canvas = view->getCanvas();
	TImage *image = TImage::Create();
	image->FromPad(canvas);
	image->WriteImage(pngFilePath.Data()); // raster
	canvas->Print(pdfFilePath.Data());     // vector

	// Show ok dialog
	UiHelper* uiHelper = UiHelper::getInstance();
	uiHelper->showOkDialog("PNG and PDF images saved.");
}

void SWCalculatorPresenter::onSaveResultsClicked() {
	Model* model = getModel();
	SWCalculatorView* view = getView();

	// Create image file names
	TString* filePath = model->getFileName();
	TString* filePathNoExtension = StringUtils::stripFileExtension(filePath);
	if (model->getSourceHist()) {
		*filePathNoExtension += "-kapton";
	}
	TString resultsFilename = *filePathNoExtension + "-results.txt";

	view->saveFitResults(&resultsFilename);
}

void SWCalculatorPresenter::onClearResultsClicked() {
	SWCalculatorView* view = getView();
	view->clearFitResults();
}
