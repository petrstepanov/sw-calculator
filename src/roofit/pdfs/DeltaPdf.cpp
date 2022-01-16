/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimpleDeltaPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 2, 2017, 5:34 AM
 */

#include "DeltaPdf.h"
#include <iostream>

ClassImp(DeltaPdf);

DeltaPdf::DeltaPdf(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _mean) :
		RooAbsPdf(name, title), x("x", "x", this, _x), mean("mean", "Step mean", this, _mean) {

}

DeltaPdf::DeltaPdf(const DeltaPdf& other, const char* name) :
		RooAbsPdf(other, name), x("x", this, other.x), mean("mean", this, other.mean){
}

Double_t DeltaPdf::evaluate() const {
    Double_t arg = x-mean;
	if (arg < epsilon) return 0;
	if (arg > epsilon) return 0;
	return 1./epsilon;
}

Int_t DeltaPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars, analVars, x))
		return 1;
	return 0;
}

Double_t DeltaPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
	switch (code) {
        case 1: {
            Double_t xMin = x.min(rangeName) - mean;
            Double_t xMax = x.max(rangeName) - mean;

            if (xMax < epsilon) return 0;
            if (xMin > epsilon) return 0;

            // TODO: update...
            return 1;
        }
	}
	assert(0);
	return 0;
}
