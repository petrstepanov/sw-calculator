/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OrthogonalPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 20, 2017, 3:55 PM
 */

#include "OrthogonalPdf.h"
#include "../../model/Constants.h"
#include <RooRealVar.h>
#include <TMath.h>

ClassImp(OrthogonalPdf);

OrthogonalPdf::OrthogonalPdf(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _mean, RooAbsReal& _a1, RooAbsReal& _a2) :
		RooAbsPdf(name, title), x("x", "x", this, _x), mean("mean", "Orthogonal mean", this, _mean), a1("a1", "a1", this, _a1), a2("a2", "a2", this, _a2) {
}

OrthogonalPdf::OrthogonalPdf(const OrthogonalPdf& other, const char* name) :
		RooAbsPdf(other, name), x("x", this, other.x), mean("mean", this, other.mean), a1("a1", this, other.a1), a2("a2", this, other.a2) {
}

Double_t OrthogonalPdf::evaluate() const {
	Double_t chbar = Constants::chbar / 1E3;
	Double_t _x = (x - mean) / chbar; // We need keV*A cause x is in keVs, length is A. chbar is eV*A
	Double_t b = 3 * a1 * a2 / (a1 + a2);
	// Everything works better if we reduce fractions
	return chbar * (48 * a2 * a2 - 30 * a2 * (a2 + b) * (1 + pow(a2 * _x, 2)) + 5 * pow(a2 + b, 2) * pow(1 + pow(a2 * _x, 2), 2)) / pow(1 + pow(a2 * _x, 2), 5); // where a2 in znamenatel?
	// Do not reduсe fractions
	//return pow(a2,4)*(48*a2*a2 - 30*a2*(a2+b)*(1+pow(a2*_x,2)) + 5*pow(a2+b,2)*pow(1+pow(a2*_x,2),2))/(30*pow(1+pow(a2*_x,2),5)); // where a2 in znamenatel?
}

Int_t OrthogonalPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars, analVars, x))
		return 1;
	return 0;
}

Double_t OrthogonalPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
	switch (code) {
	case 1: {
		Double_t chbar = Constants::chbar / 1E3;
		Double_t x1 = (x.min(rangeName) - mean) / chbar;
		Double_t x2 = (x.max(rangeName) - mean) / chbar;
		return indefiniteIntegral(x2) - indefiniteIntegral(x1);
	}
	}
	assert(0);
	return 0;
}

Double_t OrthogonalPdf::indefiniteIntegral(Double_t _x) const {
	Double_t b = 3 * a1 * a2 / (a1 + a2);

	Double_t s1 = 6 * pow(a2, 2) * _x / pow(1 + pow(a2 * _x, 2), 4);
	Double_t s2 = a2 * (2 * a2 - 5 * b) * _x / pow(1 + pow(a2 * _x, 2), 3);
	Double_t s3 = 5 * (3 * a2 * a2 - 3 * a2 * b + b * b) * _x / (4 * pow(1 + pow(a2 * _x, 2), 2));
	Double_t s4 = 15 * (3 * a2 * a2 - 3 * a2 * b + b * b) * _x / (8 + 8 * pow(a2 * _x, 2));
	Double_t s5 = 15 * (3 * a2 * a2 - 3 * a2 * b + b * b) * atan(a2 * _x) / (8 * a2);
	return s1 + s2 + s3 + s4 + s5;
//    return pow(a2,4)*(s1+s2+s3+s4+s5)/30;
}

RooArgList* OrthogonalPdf::getParameters(Bool_t isTwoDetector) {
	Double_t Ry = Constants::Ry; // eV
	Double_t a_B = Constants::a_B; // A
	Double_t e1 = isTwoDetector ? Ry * pow(a_B / a1, 2) : Ry * pow(a_B / a1, 2);
	Double_t de1 = isTwoDetector ? 0 : 0;
	RooRealVar()
	RooRealVar* v1 = new RooRealVar("bindEnergyOrth", "Binding E (ort wf)", e1, de1, "Binding E (ort wf)", "eV");
	RooArgList* list = new RooArgList();
	list->add(*v1);
	return list;
}
