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

GaussianPdf::GaussianPdf(const char *name,
		const char *title,
		RooAbsReal& _x,
		RooAbsReal& _mean, RooAbsReal& _a, Bool_t _isTwoDetector) :
		RooAbsPdf(name, title),
		x("x", "x", this, _x),
		mean("mean", "mean", this, _mean),
		a("a", "a", this,_a),
		isTwoDetector(_isTwoDetector){
}

GaussianPdf::GaussianPdf(const GaussianPdf& other, const char* name) :
		RooAbsPdf(other, name),
		x("x", this, other.x),
		mean("mean", this, other.mean),
		a("a", this, other.a),
		isTwoDetector(other.isTwoDetector){
}

Double_t GaussianPdf::evaluate() const {
	Double_t k = a / (Constants::chbar / 1E3); // convert chbar to [keV]

	// Shift exists only in 1D case
	// 1D spectroscopy: shift = E_b/2
	// 2D spectroscopy: shift = 0
	Double_t shift = isTwoDetector ? 0 : getBindingEnergy()/2;
	Double_t _x = x - (mean - shift);
	// Without normalization
	return exp(-0.5 * _x * _x * k * k);

	// With normalization
	//    return pow(a,4)*exp(-0.5*_x*_x*a*a) ;
}

Int_t GaussianPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars, analVars, x)) return 1;
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

Double_t GaussianPdf::getBindingEnergy() const {
	// Energy value
	Double_t Ry = Constants::Ry; // eV
	Double_t a_B = Constants::a_B; // A
	// return isTwoDetector ? 3 * Ry * pow(a_B / a, 2) : 3 * Ry * pow(a_B / a / 2, 2);
	return 3 * Ry * pow(a_B / a, 2);
}

Double_t GaussianPdf::getBindingEnergyError(Double_t da) const {
	// Energy value
	Double_t Ry = Constants::Ry; // eV
	Double_t a_B = Constants::a_B; // A
	// return isTwoDetector ? 2 * 3 * Ry * a_B * a_B / a / a / a * da : 3 / 2 * Ry * a_B * a_B / a / a / a * da;
	return 2 * 3 * Ry * a_B * a_B / a / a / a * da;
}


RooArgList* GaussianPdf::getParameters(Bool_t isTwoDetector) {
	// A value and error
	RooRealVar* aReal = dynamic_cast<RooRealVar*>(a.absArg());
	Double_t da = aReal ? aReal->getError() : 0;

	// Binding Energy value and error
	Double_t Ry = Constants::Ry; // eV
	Double_t a_B = Constants::a_B; // A
	Double_t e = getBindingEnergy();
	Double_t de = getBindingEnergyError(da);

	// Build list and return vars
	const char* name = Form("bindE%s", this->GetName());
	const char* title = Form("Binding E %s", this->GetTitle());
	RooRealVar* v = new RooRealVar(name, title, e, "eV");
	v->setError(de);

	// Add FWHM's
	Double_t sigma = 1 / (a / (Constants::chbar / 1E3)); // this is 1/k - see evaluate() above
	Double_t fwhm = sigma*Constants::sigmaToFwhm;
	 // uncertanty propagation: https://en.wikipedia.org/wiki/Propagation_of_uncertainty
	Double_t fwhmErr = da  / a / a * (Constants::chbar / 1E3) * Constants::sigmaToFwhm;
	const char* nameFWHM = Form("fwhm%s", this->GetName());
	const char* titleFWHM = Form("FWHM of %s", this->GetTitle());
	RooRealVar* f = new RooRealVar(nameFWHM, titleFWHM, fwhm, "keV");
	f->setError(fwhmErr);

	RooArgList* list = new RooArgList();
	list->add(*v);
	list->add(*f);
	return list;
}
