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

AddPdf::AddPdf() {
}

AddPdf::~AddPdf() {
}

RooAbsPdf* AddPdf::addReversed(RooArgList* pdfList, RooRealVar* observable, const char* pdfName) {
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
	return add(pdfInverseList, observable, pdfName);
}

RooAbsPdf* AddPdf::add(RooArgList* pdfList, RooRealVar* observable, const char* pdfName) {
	// If one component given just return it
	unsigned numberOfComponents = pdfList->getSize();
	if (numberOfComponents == 1) {
		RooAbsArg* arg = pdfList->at(0);
		RooAbsPdf* pdf = dynamic_cast<RooAbsPdf*>(arg);
		return pdf;
	}

	RooArgList* I_i = new RooArgList();
	for (unsigned i = 0; i < numberOfComponents - 1; i++) {
		// Construct list of original model coefficients
		// I_i = [I4, I3, I2]
		const char* componentName = pdfList->at(i)->GetName();
		const char* componentTitle = pdfList->at(i)->GetTitle();
		RooRealVar* I = new RooRealVar(TString::Format("int%s", componentName), TString::Format("Intensity of %s", componentTitle), 20, 0, 100, "%");
		RooFormulaVar* INorm = new RooFormulaVar(TString::Format("int%sNorm", componentName), TString::Format("Intensity of %s, normalized", componentTitle), "@0/100", *I);
		I_i->add(*INorm);
	}
	RooAddPdf* pdf = new RooAddPdf(TString::Format("addPdf%s", pdfName), TString::Format("Additive model %s", pdfName), *pdfList, *I_i);

	// https://sft.its.cern.ch/jira/browse/ROOT-9653
	pdf->fixAddCoefNormalization(RooArgSet(*observable));
	return pdf;
}
