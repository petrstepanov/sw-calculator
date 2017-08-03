/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimpleParabolaPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 2, 2017, 5:34 AM
 */

#include "SimpleParabolaPdf.h"

SimpleParabolaPdf::SimpleParabolaPdf(const char *name, const char *title,
	RooAbsReal& _x,
	RooAbsReal& _mean,
	RooAbsReal& _r) :
	RooAbsPdf(name, title),
	x("x", "x", this, _x),
	mean("mean", "Parabola mean", this, _mean),
	r("r", "Parabola root", this, _r)
{
}

SimpleParabolaPdf::SimpleParabolaPdf(const SimpleParabolaPdf& other, const char* name) :
RooAbsPdf(other, name),
x("x", this, other.x),
mean("mean", this, other.mean),
r("r", this, other.r){
}

Double_t SimpleParabolaPdf::evaluate() const {
	Double_t arg = x - mean;
	if (arg < -r) return 0.;
	if (arg > r) return 0.;
	return (-arg*arg + r*r) * 0.75 / (r*r*r);
}

Int_t SimpleParabolaPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars, analVars, x)) return 1;
	return 0;
}

Double_t SimpleParabolaPdf::analyticalIntegral(Int_t code, const char* rangeName) const
{
	switch (code) {
	case 1: {
				Double_t ret(0);
				if (x.max(rangeName) < mean - r || x.min(rangeName) > mean + r) return 0.;

				Double_t x1 = TMath::Max(-r, x.min(rangeName) - mean);
				Double_t x2 = TMath::Min(r, x.max(rangeName) - mean);

				ret = 1 / (4 * r*r*r)*(x1*x1*x1 - x2*x2*x2) + 3 * (x2 - x1) / 4; // (x1*x1*x1 - x2*x2*x2) / 3. + r*r*(x2 - x1);
				return ret;
	}
	}
	assert(0);
	return 0;
}

