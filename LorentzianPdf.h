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
#include "IndirectParamPdf.h"

class LorentzianPdf : public RooAbsPdf, public IndirectParamPdf {
public:
	LorentzianPdf() {};
	LorentzianPdf(const char *name, const char *title,
		RooAbsReal& _x,
		RooAbsReal& _mean,
		RooAbsReal& _epsilon);
	LorentzianPdf(const LorentzianPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const { return new LorentzianPdf(*this, newname); }
	inline virtual ~LorentzianPdf() { }
	Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName = 0) const;
	Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

        std::pair<Double_t, Double_t> getParameterValue(Bool_t isTwoDetector);
        TString getParameterName();
        
protected:
	RooRealProxy x;
	RooRealProxy mean;
	RooRealProxy epsilon;

	Double_t evaluate() const;

private:
	//ClassDef(LorentzianPdf, 1) // Your description goes here...
};

#endif /* LORENTZIANPDF_H */