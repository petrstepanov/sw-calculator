/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DampLorentzPdf.h
 * Author: petrstepanov
 *
 * Created on August 19, 2017, 10:15 PM
 */

#ifndef DAMPLORENTZPDF_H
#define DAMPLORENTZPDF_H

#include <RooAbsPdf.h>
#include <RooAbsReal.h>
#include <RooRealProxy.h>
#include <TString.h>
#include "IndirectParamPdf.h"
#include "Variable.h"

class DampLorentzPdf : public RooAbsPdf, public IndirectParamPdf {
public:
	DampLorentzPdf() {};
	DampLorentzPdf(const char *name, const char *title,
		RooAbsReal& _x,
		RooAbsReal& _mean,
		RooAbsReal& _epsilon);
	DampLorentzPdf(const DampLorentzPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const { return new DampLorentzPdf(*this, newname); }
	inline virtual ~DampLorentzPdf() { }
	Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName = 0) const;
	Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

        std::list<Variable*> getParameters(Bool_t isTwoDetector);
        
protected:
	RooRealProxy x;
	RooRealProxy mean;
	RooRealProxy epsilon;

	Double_t evaluate() const;

private:
	//ClassDef(DampLorentzPdf, 1) // Your description goes here...
};

#endif /* DAMPLORENTZPDF_H */

