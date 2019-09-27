/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AddPdf.cpp
 * Author: petrstepanov
 * 
 * Created on December 11, 2018, 4:52 PM
 */

#include "AddPdf.h"

#include <string>

#include "RooAbsArg.h"
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooAddPdf.h"
#include "../util/StringUtils.h"
#include "../model/Constants.h"

AddPdf::AddPdf() {
}

AddPdf::~AddPdf() {
}

RooAbsPdf* AddPdf::addReversed(RooArgList* pdfList, RooArgList* intensities, RooRealVar* observable, const char* pdfName) {
	// Construct inverse list;
	RooArgList* pdfInverseList = new RooArgList();
	TIterator* it = pdfList->createIterator(kIterBackward);
	TObject* temp;
	while ((temp = it->Next())) {
		RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(temp);
		if (pdf) {
			pdfInverseList->add(*pdf);
		}
	}

	// Add PDFs
	return add(pdfInverseList, intensities, observable, pdfName);
}

RooAbsPdf* AddPdf::add(RooArgList* pdfList, RooArgList* intensities, RooRealVar* observable, const char* pdfName, Bool_t recursive) {
	// If one component given just return it
	unsigned numberOfComponents = pdfList->getSize();
	if (numberOfComponents == 0) {
		return nullptr;
	}
	if (numberOfComponents == 1) {
		RooAbsArg* arg = pdfList->at(0);
		RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(arg);
		return pdf;
	}

	// Initialize intensities list if it was not done before
	if (!intensities) intensities = new RooArgList();

	for (unsigned i = 0; i < numberOfComponents - 1; i++) {
		// Construct list of original model coefficients
		// I_i = [I4, I3, I2]
		const char* componentName = pdfList->at(i)->GetName();
		const char* componentTitle = pdfList->at(i)->GetTitle();

		if (recursive){
			RooRealVar* intensity = new RooRealVar(TString::Format("int%s", componentName).Data(), TString::Format("Intensity of %s", componentTitle).Data(), 0.75, 0, 1);

			// Intensities are always different depending on number of components.
			// To avoid total intensity more than 1 we don't save them in Pool
			intensity->setAttribute(Constants::ATTR_NO_SAVE_TO_POOL, kTRUE);
			intensity->setAttribute(Constants::ATTR_HIDE_PARAMETER_FROM_UI, kTRUE);
			intensities->add(*intensity);
		}
		else {
			Double_t defaultIntensity = 100. / (Double_t) pdfList->getSize();
			RooRealVar* intensity = new RooRealVar(TString::Format("int%s", componentName).Data(), TString::Format("Intensity of %s", componentTitle).Data(), defaultIntensity, 0, 100, "%");
			intensity->setAttribute(Constants::ATTR_NO_SAVE_TO_POOL, kTRUE);
			RooFormulaVar* intensityNorm = new RooFormulaVar(TString::Format("int%sNorm", componentName).Data(), TString::Format("Intensity of %s, normalized", componentTitle).Data(), "@0/100", *intensity);
			intensities->add(*intensityNorm);
		}
	}

	RooAddPdf* pdf = new RooAddPdf(TString::Format("addPdf%s", pdfName), TString::Format("Additive model %s", pdfName), *pdfList, *intensities, recursive);

	// https://sft.its.cern.ch/jira/browse/ROOT-9653
	pdf->fixAddCoefNormalization(RooArgSet(*observable));
	return pdf;
}
