/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimpleStepPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 2, 2017, 5:34 AM
 */

#include "StepPdf.h"
#include "../../model/Constants.h"
#include <iostream>

ClassImp(StepPdf);

StepPdf::StepPdf(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _mean, Bool_t _isInverted) :
		RooAbsPdf(name, title), x("x", "x", this, _x), mean("mean", "Step mean", this, _mean), isInverted(_isInverted) {

}

StepPdf::StepPdf(const StepPdf& other, const char* name) :
		RooAbsPdf(other, name), x("x", this, other.x), mean("mean", this, other.mean), isInverted(other.isInverted){
}

Double_t StepPdf::evaluate() const {
	Double_t arg = x - mean;
	if (arg <= 0){
	    if (!isInverted) return 0.;
	    return 1;
	}
    if (!isInverted) return 1.;
	return 0.;
}

Int_t StepPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars, analVars, x))
		return 1;
	return 0;
}

Double_t StepPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
	switch (code) {
        case 1: {
            Double_t xMin = x.min(rangeName);
            Double_t xMax = x.max(rangeName);

            // ____----
            if (!isInverted){
                if (xMax < mean) return 0;
                return xMax - TMath::Max(mean, xMin);
            }

            // ----____
            if (xMin > mean) return 0;
            return TMath::Min(mean, xMax) - xMin;

        }
	}
	assert(0);
	return 0;
}
