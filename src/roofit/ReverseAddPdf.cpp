/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReverseAddPdf.cpp
 * Author: petrstepanov
 * 
 * Created on December 11, 2018, 4:52 PM
 */

#include <string>

#include "ReverseAddPdf.h"
#include "RooAbsArg.h"
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooAddPdf.h"
#include "../util/StringUtils.h"

ReverseAddPdf::ReverseAddPdf() {
}

ReverseAddPdf::~ReverseAddPdf() {
}

RooAbsPdf* ReverseAddPdf::add(const char* name, RooArgList* pdfList) {
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
		// 1st "Source" component intensity
		const char* pdfName = pdfInverseList->at(i)->GetName();
		RooRealVar* I = new RooRealVar(Form("Int_%s", pdfName), Form("Intensity of %s", pdfName), 5.0, 0.0, 100.0, "%");
		RooFormulaVar* INorm = new RooFormulaVar(Form("IntNorm_%s", pdfName), Form("Normalized intensity of %s", pdfName), "@0/100", *I);
		I_i->add(*INorm);
	}

	return new RooAddPdf(name, Form("%s (reverse add PDF)", name), *pdfInverseList, *I_i);
}
