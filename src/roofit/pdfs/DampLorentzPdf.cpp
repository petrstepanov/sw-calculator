/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DampLorentzPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 19, 2017, 10:15 PM
 */

#include "DampLorentzPdf.h"
#include "../../model/Constants.h"
#include <RooRealVar.h>
#include <TMath.h>

ClassImp(DampLorentzPdf);

DampLorentzPdf::DampLorentzPdf(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _mean, RooAbsReal& _a) :
		RooAbsPdf(name, title), x("x", "x", this, _x),
		mean("mean", "DampExp mean", this, _mean),
		a("a", "DampExp a", this, _a) {
}

DampLorentzPdf::DampLorentzPdf(const DampLorentzPdf& other, const char* name) :
		RooAbsPdf(other, name),
		x("x", this, other.x),
		mean("mean", this, other.mean),
		a("a", this, other.a) {
}

Double_t DampLorentzPdf::evaluate() const {
	Double_t c = Constants::chbar / 1E3; // because x is in keVs
	Double_t _x = x - mean;
	return (23 - 20 * pow(a * _x / c, 2) + 5 * pow(a * _x / c, 4)) / pow(1 + pow(a * _x / c, 2), 5);
}

Int_t DampLorentzPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars, analVars, x))
		return 1;
	return 0;
}

Double_t DampLorentzPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
	switch (code) {
	case 1: {
		Double_t x1 = (x.min(rangeName) - mean);
		Double_t x2 = (x.max(rangeName) - mean);
		return indefiniteIntegral(x2) - indefiniteIntegral(x1);
	}
	}
	assert(0);
	return 0;
}

Double_t DampLorentzPdf::indefiniteIntegral(Double_t _x) const {
	// With Dropped Constant parameter -- works good here
	Double_t k = a / (Constants::chbar / 1E3);
	return (k * _x * (139 + 211 * k * k * _x * _x + 165 * pow(k * _x, 4) + 45 * pow(k * _x, 6)) + 45 * pow(1 + k * k * _x * _x, 4) * atan(k * _x)) / (8 * k * pow(1 + k * k * _x * _x, 4));
}

RooArgList* DampLorentzPdf::getParameters(Bool_t isTwoDetector) {
	// Energy value
	Double_t Ry = Constants::Ry; // eV
	Double_t a_B = Constants::a_B; // A
	Double_t e = isTwoDetector ? Ry * pow(a_B / a, 2) / 3 : Ry * pow(a_B / a, 2) / 3;
	// Energy error
	Double_t de = 0;
	RooAbsArg* aAbsArg = a.absArg();
	RooRealVar* aReal = dynamic_cast<RooRealVar*>(aAbsArg);
	if (aReal) {
		aReal->Print();
		Double_t dA = aReal->getError();
		de = isTwoDetector ? Ry * pow(a_B / dA, 2) / 3 : Ry * pow(a_B / dA, 2) / 3;
	}
	// Build list and return vars
	TString str = TString::Format("Binding E %s", this->GetName());
	RooRealVar* v1 = new RooRealVar(e, de, str.Data(), "eV");
	RooArgList* list = new RooArgList();
	list->add(*v1);
	return list;
}
