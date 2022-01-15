/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimpleStepPdf.h
 * Author: petrstepanov
 *
 * Created on August 2, 2017, 5:34 AM
 */

#ifndef StepPdf_H
#define StepPdf_H

#include <RooAbsPdf.h>
#include <RooAbsReal.h>
#include <RooRealProxy.h>
#include "../IndirectParamPdf.h"

class StepPdf: public RooAbsPdf {
public:
//	StepPdf() {};
	StepPdf(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _mean, Bool_t isInverted);
	StepPdf(const StepPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const {
		return new StepPdf(*this, newname);
	}
	inline virtual ~StepPdf() {}

	Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName = 0) const;
	Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

protected:
	RooRealProxy x;
	RooRealProxy mean;
	Bool_t isInverted;

	Double_t evaluate() const;

private:
ClassDef(StepPdf, 1)
};

#endif /* StepPdf_H */

