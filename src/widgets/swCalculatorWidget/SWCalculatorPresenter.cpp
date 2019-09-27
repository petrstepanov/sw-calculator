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
#include "../../util/RootHelper.h"
#include "../../util/HistProcessor.h"
#include "../../util/GraphicsHelper.h"
#include "../../util/StringUtils.h"
#include "../../util/UiHelper.h"
#include "../../util/Debug.h"
#include "../../util/FileUtils.h"
#include "../../model/Constants.h"
#include "../../model/ParametersPool.h"
#include "../frames/ModalDialogFrame.h"
#include "../frames/RooRealVarListFrame.h"
#include "../importSpectrumWidget/ImportComponentView.h"
#include <RooFit.h>
#include <RooAbsArg.h>
#include <RooStringVar.h>
#include <RooDataHist.h>
#include <RooDataHist.h>
#include <TPaveText.h>
#include <RooChi2Var.h>
#include <RooMinimizer.h>
#include <RooFitResult.h>
#include <RooRealVar.h>
#include <RooArgSet.h>
#include <RooNumIntConfig.h>
#include <RooCategory.h>
#include <TMath.h>
#include <TLegend.h>
#include <iostream>

SWCalculatorPresenter::SWCalculatorPresenter(SWCalculatorView* view) : AbstractPresenter<Model, SWCalculatorView>(view) {
	model = instantinateModel();
	onInitModel();
}

Model* SWCalculatorPresenter::instantinateModel() {
	return Model::getInstance();
}

void SWCalculatorPresenter::onInitModel() {
	model->Connect("histogramImported(TH1F*)", "SWCalculatorPresenter", this, "onModelHistogramImported(TH1F*)");
	model->Connect("sourceHistogramImported(TH1F*)", "SWCalculatorPresenter", this, "onModelSourceHistogramImported(TH1F*)");
	model->Connect("componentHistogramImported(TH1F*)", "SWCalculatorPresenter", this, "onModelComponentHistogramImported(TH1F*)");

	model->Connect("fitRangeLimitsSet(DoublePair*)", "SWCalculatorPresenter", this, "onModelFitRangeLimitsSet(DoublePair*)");
	model->Connect("fitRangeSet(DoublePair*)", "SWCalculatorPresenter", this, "onModelFitRangeSet(DoublePair*)");

	model->Connect("twoDetectorSet(Bool_t)", "SWCalculatorPresenter", this, "onModelTwoDetectorSet(Bool_t)");
	model->Connect("convolutionTypeSet(Int_t)", "SWCalculatorPresenter", this, "onModelConvolutionTypeSet(Int_t)");
	model->Connect("hasParabolaSet(Bool_t)", "SWCalculatorPresenter", this, "onModelHasParabolaSet(Bool_t)");
	model->Connect("numberOfGaussiansSet(Int_t)", "SWCalculatorPresenter", this, "onModelNumberOfGaussiansSet(Int_t)");
	model->Connect("numberOfExponentsSet(Int_t)", "SWCalculatorPresenter", this, "onModelNumberOfExponentsSet(Int_t)");
	model->Connect("numberOfDampingExponentsSet(Int_t)", "SWCalculatorPresenter", this, "onModelNumberOfDampingExponentsSet(Int_t)");

	// Restore view from model
 	std::pair<Double_t, Double_t> fitRangeLimits = model->getFitRangeLimits();
	view->setFitRangeLimits(fitRangeLimits.first, fitRangeLimits.second);

	std::pair<Double_t, Double_t> fitRange = model->getFitRange();
	view->setFitRange(fitRange.first, fitRange.second);

	view->setConvolutionType(model->getConvolutionType());

	view->hasParabola->SetOn(model->getHasParabola());
	view->numGauss->SetNumber(model->getNumberOfGaussians());
	view->numExponent->SetNumber(model->getNumberOfExponents());
	view->numDampExponent->SetNumber(model->getNumberOfDampingExponents());
}

void SWCalculatorPresenter::buildFittingModel(){
	// Construct model
	pdfProvider = new PdfProvider(model->getFitProperties());
	RooAbsPdf* pdf = pdfProvider->getPdf();
	if (!pdf) return;

	RooRealVar* observable = pdfProvider->getObservable();
//	RooArgSet* pdfParameters = pdfProvider->getPdfNonConvoluted()->getParameters(RooArgSet(*observable));
	RooArgSet* pdfParameters = pdfProvider->getPdf()->getParameters(RooArgSet(*observable));

	model->getParametersPool()->synchronizePdfParameters(pdfParameters);
}

// Slots for View Signals
void SWCalculatorPresenter::onViewEditParametersClicked() {
	ModalDialogFrame* dialog = UiHelper::getInstance()->getDialog("Model Parameters");
	RooRealVar* observable = pdfProvider->getObservable();
	RooAbsPdf* pdf = pdfProvider->getPdf();
	RooArgSet* parameters = pdf->getParameters(RooArgSet(*observable));
	RooRealVarListFrame* listFrame = new RooRealVarListFrame(dialog, parameters);

	dialog->AddFrame(listFrame, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, dx, dx, dy, dy));
	dialog->show();
}

void SWCalculatorPresenter::onViewFitSpectrumClicked() {
	// Testing purposes - let model generate data
	// data = static_cast<RooAddPdf*>(model)->generateBinned(*x,1000000) ;

	// Fitting
	// Default chi2FitTo used when we use non-standard errors.
	// RooFitResult* fitResult = convolutedModel->chi2FitTo(*data, Save(kTRUE), Range("fitRange"), NumCPU(RootHelper::getNumCpu()));

	// The RooFit chi2 fit does not work when the bins have zero entries. (Two-detector setup)
	// You should either use a binned likelihood fit or use the standard chi2 fit provided by ROOT. In this case bins with zero entries are excluded from the fit

	// Convert Histogram to RooDataHist for Fitting
//	RooDataHist* data = new RooDataHist("data", "Dataset with e", RooArgSet(*observable), RooFit::Import(*fitHist));

	// Obtain fitting model and convoluted model

	RooRealVar* observable = pdfProvider->getObservable();
	TH1F* fitHist = pdfProvider->getFitHistogram();

	RooAbsPdf* pdfInMaterial = pdfProvider->getPdfInMaterial();
	RooAbsPdf* pdf = pdfProvider->getPdf();

	if (!pdf) return;

	RooDataHist* data = new RooDataHist("data", "Dataset", RooArgList(*observable), fitHist);

	RooAbsReal::defaultIntegratorConfig()->Print("v"); // RooAdaptiveIntegratorND
	// RooAbsReal::defaultIntegratorConfig()->getConfigSection("RooIntegrator1D").setRealValue("maxSteps", 30); // default 20

	 RooAbsReal::defaultIntegratorConfig()->method1D().setLabel("RooSegmentedIntegrator1D");
	 RooAbsReal::defaultIntegratorConfig()->method2D().setLabel("RooSegmentedIntegrator2D");

	// Chi2 fit
	RootHelper::startTimer();  // Start tracking Time
	RooChi2Var* chi2 = new RooChi2Var("chi2", "chi2", *(pdfProvider->getPdf()), *data, RooFit::NumCPU(RootHelper::getNumCpu()));
	RooMinimizer* m = new RooMinimizer(*chi2);
	m->setMinimizerType("Minuit2");

	// Print Chi2 fit results
	Int_t resultMigrad = m->migrad();
	Int_t resultHesse = m->hesse();
	Debug("SWCalculatorPresenter::onFitSpectrumClicked", "RooMinimizer: migrad=" << resultMigrad << ", hesse=" << resultHesse);

	RooFitResult* fitResult = m->save();

	// Create RooPlot from energy axis frame
	RooPlot* spectrumPlot = observable->frame();
	spectrumPlot->SetTitle("");  // Set Empty Graph Title
	// spectrumPlot->GetXaxis()->SetRangeUser(fitRangeMin, fitRangeMax);      // Do we need this?

	// Configure axis labels and look
	GraphicsHelper* graphicsHelper = GraphicsHelper::getInstance();
	graphicsHelper->styleAxis(spectrumPlot->GetXaxis(), "E_{1} #minus E_{2}, keV", 1.5, 0.02, kTRUE); // Title, Title offset, Label offset
	graphicsHelper->styleAxis(spectrumPlot->GetYaxis(), "Counts", 1.1, 0.012, kTRUE);

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

	Double_t modelMean = pdfProvider->getMean()->getValV();
	{
		// Draw S, W regions
		Double_t sWidth = view->numSWidth->GetNumber();
		Double_t wWidth = view->numWWidth->GetNumber();
		Double_t wShift = view->numWShift->GetNumber();

		// Energy value of the maximum of the convoluted model
		Bool_t isTwoDetector = model->isTwoDetector();

		graphicsHelper->drawSWRegions(spectrumPlot, sWidth, wWidth, wShift, modelMean, yAxisMin, yAxisMax, isTwoDetector);
	}

	// Plot data points first (in transparent color). Essential for normalization of PDFs
	// data->plotOn(setNumberOfGaussiansspectrumPlot, RooFit::Invisible());
	data->plotOn(spectrumPlot, RooFit::LineColor(kGray + 3), /*RooFit::ShiftToZero(),*/ RooFit::XErrorSize(0), RooFit::MarkerSize(0.5), RooFit::MarkerColor(kGray + 3), RooFit::Name("data"));

	// Initialize legend
	TLegend *legend = new TLegend(GraphicsHelper::padMargins.left + 0.04, 0.5, GraphicsHelper::padMargins.left + 0.4, 1 - GraphicsHelper::padMargins.top - 0.05);
	legend->AddEntry(spectrumPlot->findObject("data"), "Data points", "pe");

	// Plot fitting pdf
	if (pdf){
		pdf->plotOn(spectrumPlot, RooFit::LineColor(kOrange + 6), RooFit::LineWidth(1), RooFit::Name("pdf"));
		legend->AddEntry(spectrumPlot->findObject("pdf"), "Fitting model", "l");
	}
	RooCurve* curveFit = spectrumPlot->getCurve("pdf");

	// Plot unconvoluted model components
	// Problem: convoluted PDF loses normalization of components
	// TODO: normalize components on (1-source contribution) relative!
	{
		RooArgSet* components = pdfInMaterial->getComponents();
		TIterator* it = components->createIterator();
		Int_t i = 0;
//		Double_t integral = pdfProvider->getFitHistogram()->Integral();
//		Double_t range = pdfProvider->getObservable()->getMax() - pdfProvider->getObservable()->getMin();
//		Double_t totalFitCounts = integral*range;
		// Here we have to normalize non-convoluted material components with respect to the source contribution!
		Double_t relativeNormalization = pdfProvider->getSourceContribution() ? 1-pdfProvider->getSourceContribution()->getVal()/100. : 1;
		while (TObject* tempObject = it->Next()) {
			RooAbsPdf* component = dynamic_cast<RooAbsPdf*>(tempObject);
			if (component && component->getAttribute(Constants::ATTR_NO_DRAW_ON_PLOT)==kFALSE) {
				Double_t precision = strcmp(component->GetName(), "Parabola") == 0 ? 1E-8 : 1E-3;
				RooCmdArg precisionCmdArg =  RooFit::Precision(precision);
				pdfInMaterial->plotOn(spectrumPlot, RooFit::Components(*component), RooFit::LineStyle(kDashed), RooFit::LineColor(GraphicsHelper::colorSet[i++]), RooFit::LineWidth(1), RooFit::Name(component->GetName()), RooFit::Normalization(relativeNormalization, RooAbsReal::Relative), precisionCmdArg);
				legend->AddEntry(spectrumPlot->findObject(component->GetName()), component->GetTitle(), "l");
			}
		}
	}

	// Plot Resolution Function
	RooAbsPdf* resolutionFunction = pdfProvider->getResolutionFunction();
	if (resolutionFunction) {
		resolutionFunction->plotOn(spectrumPlot, RooFit::LineStyle(kDashed), RooFit::LineColor(kGray), RooFit::LineWidth(1), RooFit::Name(resolutionFunction->GetName())); //, RooFit::Normalization(totalFitCounts, RooAbsReal::NumEvent)); Normalization(1.0,RooAbsReal::RelativeExpected)
		legend->AddEntry(spectrumPlot->findObject(resolutionFunction->GetName()), resolutionFunction->GetTitle(), "l");
	}

	// Plot Source contribution
	RooAbsPdf* sourcePdf = pdfProvider->getSourcePdf();
	if (sourcePdf) {
		pdf->plotOn(spectrumPlot, RooFit::Components(*sourcePdf), RooFit::LineStyle(kDashed), RooFit::LineColor(kBlack+3), RooFit::LineWidth(1), RooFit::Name(sourcePdf->GetName()));
		legend->AddEntry(spectrumPlot->findObject(sourcePdf->GetName()), sourcePdf->GetTitle(), "l");
	}

	// This histogram is used for calculating S and W parameters
	HistProcessor* histProcessor = HistProcessor::getInstance();
	TH1F* fitHistNoBg = fitHist;

//	// Single-dimentional experiment case - plot background
//	if (!model->isTwoDetector()) {
//		// Plot Convoluted Model background (its added after convolution graph because otherwise it changes backgroud)
//		pdfNonConvoluted->plotOn(spectrumPlot, RooFit::Components(*(pdfProvider->getBackgroundComponents())), RooFit::LineStyle(kDashed), RooFit::LineColor(kPink - 4), RooFit::LineWidth(1), RooFit::Name("bg"));
//		legend->AddEntry(spectrumPlot->findObject("bg"), "Atan background", "l");
//		// Plot fit without background
//		RooCurve* curveBg = spectrumPlot->getCurve("bg");
//		// RooCurve* curveFitNoBg = histProcessor->subtractCurves(curveFit, curveBg);
//		// curveFitNoBg->SetLineColor(kGray);
//		// curveFitNoBg->SetLineWidth(2);
//		fitHistNoBg = (TH1F*) histProcessor->subtractCurve("fitHistNoBg", fitHist, curveBg);
//		RooDataHist* dataNoBg = new RooDataHist("dataNoBg", "Dataset with e (no background)", RooArgSet(*observable), RooFit::Import(*fitHistNoBg));
//		dataNoBg->plotOn(spectrumPlot, RooFit::XErrorSize(0), RooFit::MarkerSize(0.5), RooFit::DataError(RooAbsData::None), RooFit::MarkerColor(kGray), RooFit::Name("dataNoBg"));
//		spectrumPlot->drawBefore("data", "dataNoBg");
//		legend->AddEntry(spectrumPlot->findObject("dataNoBg"), "Subtracted background", "p");
//	}

	// Set legend's bottom coordinate depending on the number of entries
	legend->SetY1(legend->GetY2() - legend->GetNRows() * GraphicsHelper::LINE_HEIGHT_NORMAL);
	legend->SetFillStyle(0);  // transparent
	legend->SetBorderSize(0); // no border
	legend->SetMargin(0.15);
	legend->SetTextSize(GraphicsHelper::TEXT_SIZE_SMALL*GraphicsHelper::getFontSizeScale(kTRUE));
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
	RooDataHist* chi2DataHist = new RooDataHist("chi2DataHist", "Chi2", RooArgList(*observable), resHist);

	// Create RooPlot for chi^2
	RooPlot* residualsPlot = observable->frame();
	residualsPlot->SetTitle(""); // Set Empty Graph Title
	// residualsPlot->GetXaxis()->SetRangeUser(fitRangeMin, fitRangeMax);      // Do we need this?
	graphicsHelper->styleAxis(residualsPlot->GetXaxis(), "", 0, 0.05, kFALSE); // Title, Title offset, Label offset
	graphicsHelper->styleAxis(residualsPlot->GetYaxis(), "Residuals", 0.44, 0.015, kFALSE); // "#chi^{2}"
	Int_t nDivisions = residualsPlot->GetYaxis()->GetNdivisions();
	nDivisions = (nDivisions%100)/2;
	residualsPlot->GetYaxis()->SetNdivisions(nDivisions);

	// Plot residuals
	chi2DataHist->plotOn(residualsPlot, RooFit::LineColor(kGray + 3), RooFit::XErrorSize(0), RooFit::DataError(RooAbsData::None), RooFit::MarkerSize(0.5), RooFit::MarkerColor(kGray + 3));

	// Draw horizontal line
	TLine* hr = new TLine(observable->getMin(), 0, observable->getMax(), 0);
	hr->SetLineStyle(kDashed); // https://root.cern.ch/doc/master/classTAttLine.html#L3
	hr->SetLineColor(kGray+2);
	residualsPlot->addObject(hr);

	// Plot chi2 legend
	Chi2Struct chi2Struct = histProcessor->getChi2(fitHist, curveFit, pdf);
	TPaveText* chiPaveText = new TPaveText(GraphicsHelper::LEGEND_X1, 1 - 2 * GraphicsHelper::padMargins.right - 0.18, 1 - 1.2 * GraphicsHelper::padMargins.right, 1 - 2 * GraphicsHelper::padMargins.top, "NDC");
	TText* text = chiPaveText->AddText(Form("#chi^{2} = %.1f #divide %d = %.3f", chi2Struct.chiSum, chi2Struct.degreesOfFreedom, chi2Struct.chi2));
	// text->SetTextSize(GraphicsHelper::TEXT_SIZE_SMALL);
	chiPaveText->SetTextSize(GraphicsHelper::TEXT_SIZE_SMALLER*GraphicsHelper::getFontSizeScale(kFALSE));
	chiPaveText->SetTextFont(GraphicsHelper::getFontCode(GraphicsHelper::FONT_REGULAR));
	chiPaveText->SetFillColor(kWhite);
	chiPaveText->SetBorderSize(0);
	chiPaveText->SetTextAlign(kHAlignRight + kVAlignCenter);
	residualsPlot->addObject(chiPaveText);

	// Calculate S and W parameters
	Double_t sWidth = view->numSWidth->GetNumber();
	Double_t wWidth = view->numWWidth->GetNumber();
	Double_t wShift = view->numWShift->GetNumber();
	Bool_t isTwoDetector = model->isTwoDetector();
	RooRealVar* s = histProcessor->getSParameter(fitHistNoBg, sWidth, modelMean, isTwoDetector);
	RooRealVar* w = histProcessor->getWParameter(fitHistNoBg, wWidth, wShift, modelMean, isTwoDetector);

	// Calculate linear intensitiesv
	RooArgList* recursiveIntensities = pdfProvider->getIntensitiesInMaterial();
	RooArgList* linearIntensities = RootHelper::getLinearIntensities(recursiveIntensities);

	// Output Parameters to the View
	{
		// Output filename
		view->displayFilename(model->getFileName());
		view->displayFitParameters(fitResult);

		// Output indirect model parameters
		view->displayVariables(pdfProvider->getIndirectParameters());

		// Output components intensities
		if (linearIntensities->getSize()){
			view->displayVariables(linearIntensities);
		}

		// Output source contribution
		if (RooRealVar* sourceContribution = pdfProvider->getSourceContribution()){
			view->displayVariable(sourceContribution);
		}

		// Output Integral Chi^2
		view->displayChi2(chi2Struct);

		// Output S and W values
		view->displaySW(s, w);

		view->scrollOutputDown();
	}

	// Output parameters onto the plot
	{
		RooArgList* plotParametersList = new RooArgList();

		RooArgSet* modelParameters = pdfProvider->getParameters();
		plotParametersList->add(*modelParameters);

		// Separator
		RooStringVar* separator = new RooStringVar(RootHelper::getUUID()->Data(), "", " ");
		plotParametersList->add(*separator);

		RooArgList* indirectParameters = pdfProvider->getIndirectParameters();
		plotParametersList->add(*indirectParameters);

		if (linearIntensities->getSize()){
			// Separator
			separator = new RooStringVar(RootHelper::getUUID()->Data(), "", " ");
			plotParametersList->add(*separator);

			// Output components intensities
			plotParametersList->add(*linearIntensities);
		}

		// Output source contribution
		if (RooRealVar* sourceContribution = pdfProvider->getSourceContribution()){
			separator = new RooStringVar(RootHelper::getUUID()->Data(), "", " ");
			plotParametersList->add(*separator);
			plotParametersList->add(*sourceContribution);
		}

		// Separator
		separator = new RooStringVar(RootHelper::getUUID()->Data(), "", " ");
		plotParametersList->add(*separator);

		// Output S and W values
		plotParametersList->add(RooArgList(*s, *w));

		TPaveText* paramsPaveText = GraphicsHelper::makeParametersPaveText(*plotParametersList, GraphicsHelper::LEGEND_X1, 1-GraphicsHelper::padMargins.right, 1 - GraphicsHelper::padMargins.top);
		paramsPaveText->SetTextSize(GraphicsHelper::TEXT_SIZE_SMALLER*GraphicsHelper::getFontSizeScale(kTRUE));
		paramsPaveText->SetTextFont(GraphicsHelper::getFontCode(GraphicsHelper::FONT_REGULAR));
		spectrumPlot->addObject(paramsPaveText);
	}

	// Draw data plot on canvas
	view->padData->cd();
	spectrumPlot->Draw();

	// Draw data plot on canvas
	view->padChi2->cd();
	residualsPlot->Draw();

	// Update canvas
	view->updateCanvas();

	view->initRooPlots(spectrumPlot, residualsPlot);
	view->setToolbarEnabled(kTRUE);
	view->setDisplayLimits(fitHist->GetXaxis()->GetXmin(), fitHist->GetXaxis()->GetXmax());
	RootHelper::stopAndPrintTimer();
}

void SWCalculatorPresenter::onViewSaveDataClicked() {
	TString fileName = TString::Format("%s-data-columns.txt", model->getFileName()->Data());
	FileUtils::savePlotsToFile(view->fitFrame, view->chiFrame, fileName.Data(), pdfProvider->getObservable());

	UiHelper* uiHelper = UiHelper::getInstance();
    uiHelper->showOkDialog("Canvas exported to ASCII data columns successfully");
}


void SWCalculatorPresenter::onViewSaveImageClicked() {
	// Create image file names
	TString* filePath = model->getFileName();
	TString* filePathNoExtension = StringUtils::stripFileExtension(filePath);
	if (pdfProvider->getSourceContribution()) {
		*filePathNoExtension += "-kapton";
	}

	TString pngFilePath = *filePathNoExtension + ".png";
	TString pdfFilePath = *filePathNoExtension + ".pdf";
	TString rooFilePath = *filePathNoExtension + ".root";

		// Save raster and vector images
	TCanvas* canvas = view->getCanvas();
//	TImage *image = TImage::Create();
//	image->FromPad(canvas);
//	image->WriteImage(pngFilePath.Data()); // raster
//	canvas->Print(pdfFilePath.Data());     // vector

	canvas->SaveAs(pngFilePath);
	canvas->SaveAs(pdfFilePath);
	canvas->SaveAs(rooFilePath);

	// Show ok dialog
	UiHelper* uiHelper = UiHelper::getInstance();
	uiHelper->showOkDialog("PNG and PDF images saved. Canvas exported as ROOT file.");
}

void SWCalculatorPresenter::onViewSaveResultsClicked() {
	// Create image file names
	TString* filePath = model->getFileName();
	TString* filePathNoExtension = StringUtils::stripFileExtension(filePath);
	if (model->getFitProperties().sourceHist) {
		*filePathNoExtension += "-kapton";
	}
	TString resultsFilename = *filePathNoExtension + "-results.txt";

    Bool_t ok = view->txtFitResult->SaveFile(resultsFilename.Data());
    UiHelper* uiHelper = UiHelper::getInstance();
    uiHelper->showOkDialog(ok ? "Results saved successfully" : "Error saving results file");
}

void SWCalculatorPresenter::onViewClearResultsClicked() {
	view->clearFitResults();
}

void SWCalculatorPresenter::onViewFitSliderRangeSet(){
	Float_t min, max;
	view->numFitSlider->GetPosition(min, max);
	model->setFitRange(min, max);
}

void SWCalculatorPresenter::onViewFitRangeSet(){
	Double_t min = view->numFitMin->GetNumberEntry()->GetNumber();
	Double_t max = view->numFitMax->GetNumberEntry()->GetNumber();
	model->setFitRange(min, max);
}

void SWCalculatorPresenter::onViewConvolutionSelected(Int_t i){
	ConvolutionType convolutionType = static_cast<ConvolutionType>(i);
	// Fix: TGButtonGroup signal fires twice?
	if (model->getConvolutionType() != convolutionType){
		model->setConvolutionType(convolutionType);
	}
}

void SWCalculatorPresenter::onViewHasParabolaSet(Bool_t b){
	model->setHasParabola(b);
}

void SWCalculatorPresenter::onViewNumGaussSet(){
	Int_t value = view->numGauss->GetNumberEntry()->GetIntNumber();
	model->setNumberOfGaussians(value);
}

void SWCalculatorPresenter::onViewNumExponentSet(){
	Int_t value = view->numExponent->GetNumberEntry()->GetIntNumber();
	model->setNumberOfExponents(value);
}

void SWCalculatorPresenter::onViewNumDampExponentSet(){
	Int_t value = view->numDampExponent->GetNumberEntry()->GetIntNumber();
	model->setNumberOfDampingExponents(value);
}

void SWCalculatorPresenter::onViewAddHistComponentClicked(){
	ModalDialogFrame* dialog = UiHelper::getInstance()->getDialog("Add model component");
	ImportComponentView* importComponentView = new ImportComponentView(dialog);

	dialog->AddFrame(importComponentView, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, dx, dx, dy*2, dy*2));

	dialog->show(480, 480);
}

void SWCalculatorPresenter::onViewRemoveHistComponentClicked(){
	model->setComponentHist(nullptr);
}

// Slots for Model Signals

void SWCalculatorPresenter::onModelHistogramImported(TH1F* hist){
	view->numFitSlider->SetScale(hist->GetNbinsX());
	view->setTabEnabled(1, true);
	view->setTabEnabled(2, true);
}

void SWCalculatorPresenter::onModelSourceHistogramImported(TH1F* hist){
	buildFittingModel();
}

void SWCalculatorPresenter::onModelComponentHistogramImported(TH1F* hist){
	view->setComponentHistogram(hist);
	buildFittingModel();
}

void SWCalculatorPresenter::onModelFitRangeLimitsSet(DoublePair* fitRangeLimits){
	view->setFitRangeLimits(fitRangeLimits->first, fitRangeLimits->second);
}

void SWCalculatorPresenter::onModelConvolutionTypeSet(Int_t convolutionType){
	view->setConvolutionType((ConvolutionType)convolutionType);
	buildFittingModel();
}

void SWCalculatorPresenter::onModelFitRangeSet(DoublePair* fitRange){
	view->setFitRange(fitRange->first, fitRange->second);
	buildFittingModel();
}

void SWCalculatorPresenter::onModelTwoDetectorSet(Bool_t isTwoDetector){
	view->reflectTwoDetector(isTwoDetector);
}

void SWCalculatorPresenter::onModelHasParabolaSet(Bool_t b){
	view->hasParabola->SetOn(b);
	buildFittingModel();
}

void SWCalculatorPresenter::onModelNumberOfGaussiansSet(Int_t num){
	view->numGauss->SetNumber(num);
	buildFittingModel();
}

void SWCalculatorPresenter::onModelNumberOfExponentsSet(Int_t num){
	view->numExponent->SetNumber(num);
	buildFittingModel();
}

void SWCalculatorPresenter::onModelNumberOfDampingExponentsSet(Int_t num){
	view->numDampExponent->SetNumber(num);
	buildFittingModel();
}
