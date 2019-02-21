/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GaussianPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 16, 2017, 2:10 AM
 */

#include "GaussianPdf.h"
#include "../../model/Constants.h"
#include <RooMath.h>

ClassImp(GaussianPdf);

GaussianPdf::GaussianPdf(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _mean, RooAbsReal& _a) :
		RooAbsPdf(name, title), x("x", "x", this, _x), mean("mean", "Gauss mean", this, _mean), a("a", "Gauss a", this, _a) {
}

GaussianPdf::GaussianPdf(const GaussianPdf& other, const char* name) :
		RooAbsPdf(other, name), x("x", this, other.x), mean("mean", this, other.mean), a("a", this, other.a) {
}

Double_t GaussianPdf::evaluate() const {
	Double_t k = a / (Constants::chbar / 1E3); // because x is in keVs
	Double_t _x = (x - mean);
	// Without normalization
	return exp(-0.5 * _x * _x * k * k);

	// With normalization
	//    return pow(a,4)*exp(-0.5*_x*_x*a*a) ;
}

Int_t GaussianPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars, analVars, x))
		return 1;
	return 0;
}

Double_t GaussianPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
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

Double_t GaussianPdf::indefiniteIntegral(Double_t _x) const {
	Double_t k = a / (Constants::chbar / 1E3);
	return TMath::Sqrt(TMath::PiOver2()) * RooMath::erf(k * _x / TMath::Sqrt2()) / k;
}

RooArgList* GaussianPdf::getParameters(Bool_t isTwoDetector) {
	// Energy value
	Double_t Ry = Constants::Ry; // eV
	Double_t a_B = Constants::a_B; // A
	Double_t e = isTwoDetector ? 3 * Ry * pow(a_B / a, 2) : 3 * Ry * pow(a_B / a / 2, 2);
	// Energy error
	Double_t de = 0;
	RooAbsArg* aAbsArg = a.absArg();
	RooRealVar* aReal = dynamic_cast<RooRealVar*>(aAbsArg);
	if (aReal) {
		Double_t da = aReal->getError();
		de = isTwoDetector ? 2 * 3 * Ry * a_B * a_B / a / a / a * da : 3 / 2 * Ry * a_B * a_B / a / a / a * da;
	}
	// Build list and return vars
	const char* name = Form("bindE%s", this->GetName());
	const char* title = Form("Binding E %s", this->GetTitle());
	RooRealVar* v = new RooRealVar(name, title, e, "eV");
	v->setError(de);
	RooArgList* list = new RooArgList();
	list->add(*v);
	return list;
}
