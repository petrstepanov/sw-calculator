/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LorentzianPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 2, 2017, 11:24 PM
 */

#include "LorentzianPdf.h"
#include "../../model/Constants.h"
#include <RooRealVar.h>
#include <TMath.h>

LorentzianPdf::LorentzianPdf(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _mean, RooAbsReal& _a, Bool_t _isTwoDetector) :
	RooAbsPdf(name, title), x("x", "x", this, _x), mean("mean", "mean", this, _mean), a("a", "a", this, _a), isTwoDetector(_isTwoDetector) {
}

LorentzianPdf::LorentzianPdf(const LorentzianPdf& other, const char* name) :
	RooAbsPdf(other, name), x("x", this, other.x), mean("mean", this, other.mean), a("a", this, other.a), isTwoDetector(other.isTwoDetector) {
}

Double_t LorentzianPdf::evaluate() const {
	Double_t k = a / (Constants::chbar / 1E3); // because x is in keVs

	// Shift exists only in 1D case
	// 1D spectroscopy: shift = E_b/2
	// 2D spectroscopy: shift = 0
	Double_t shift = isTwoDetector ? 0 : getBindingEnergy()/2;
	Double_t _x = x - (mean - shift);

	return 1 / pow(1 + pow(k * _x, 2), 3);
//    return 8*k/(3*TMath::Pi())/pow(1+pow(k*_x,2),3);
//    return 1;
}

Int_t LorentzianPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars, analVars, x))
		return 1;
	return 0;
}

Double_t LorentzianPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
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

Double_t LorentzianPdf::indefiniteIntegral(Double_t _x) const {
	Double_t k = a / (Constants::chbar / 1E3);
	Double_t s1 = _x * (5 + 3 * pow(k * _x, 2)) / pow(1 + k * k * _x * _x, 2);
	Double_t s2 = 3 * atan(k * _x) / k;
	return (s1 + s2) / 8.;
//    return _x;
}

Double_t LorentzianPdf::getBindingEnergy() const {
	// Energy value
	Double_t Ry = Constants::Ry; // eV
	Double_t a_B = Constants::a_B; // A
	// return isTwoDetector ? Ry * pow(a_B / a, 2) : Ry * pow(a_B / a, 2);
	return Ry * pow(a_B / a, 2);
}

Double_t LorentzianPdf::getBindingEnergyError(Double_t da) const {
	// Energy value
	Double_t Ry = Constants::Ry; // eV
	Double_t a_B = Constants::a_B; // A
	return 2 * Ry * a_B * a_B / a / a / a * da;
}

RooArgList* LorentzianPdf::getParameters(Bool_t isTwoDetector) {
	// A value and error
	RooAbsArg* aAbsArg = a.absArg();
	RooRealVar* aReal = dynamic_cast<RooRealVar*>(aAbsArg);
	Double_t da = aReal ? aReal->getError() : 0;

	// Energy value and error
	Double_t Ry = Constants::Ry; // eV
	Double_t a_B = Constants::a_B; // A
	Double_t e = getBindingEnergy();
	Double_t de = getBindingEnergyError(da);

	// Build list and return vars
	const char* name = Form("bindE%s", this->GetName());
	const char* title = Form("Binding E %s", this->GetTitle());
	RooRealVar* v = new RooRealVar(name, title, e, "eV");
	v->setError(de);
	RooArgList* list = new RooArgList();
	list->add(*v);
	return list;
}
