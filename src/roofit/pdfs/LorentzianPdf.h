/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LorentzianPdf.h
 * Author: petrstepanov
 *
 * Created on August 2, 2017, 11:24 PM
 */

#ifndef LORENTZIANPDF_H
#define LORENTZIANPDF_H

#include <RooAbsPdf.h>
#include <RooAbsReal.h>
#include <RooRealProxy.h>
#include <TString.h>
#include "../IndirectParamPdf.h"

class LorentzianPdf: public RooAbsPdf, public IndirectParamPdf {
public:
	LorentzianPdf(Bool_t _isTwoDetector) : isTwoDetector(_isTwoDetector){};
	LorentzianPdf(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _mean, RooAbsReal& _a, Bool_t isTwoDetector);
	LorentzianPdf(const LorentzianPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const {
		return new LorentzianPdf(*this, newname);
	}
	inline virtual ~LorentzianPdf(){}

	Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName = 0) const;
	Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

	RooArgList* getParameters(Bool_t isTwoDetector);

//protected:
	RooRealProxy x;
	RooRealProxy mean;
	RooRealProxy a;

	Bool_t isTwoDetector;

protected:
	Double_t indefiniteIntegral(Double_t _x) const;
	Double_t evaluate() const;

	Double_t getBindingEnergy() const;
	Double_t getBindingEnergyError(Double_t da) const;

private:
ClassDef(LorentzianPdf, 1)
};

#endif /* LORENTZIANPDF_H */
