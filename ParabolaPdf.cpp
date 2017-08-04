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

#include "ParabolaPdf.h"

ParabolaPdf::ParabolaPdf(const char *name, const char *title,
	RooAbsReal& _x,
	RooAbsReal& _mean,
	RooAbsReal& _r) :
	RooAbsPdf(name, title),
	x("x", "x", this, _x),
	mean("mean", "Parabola mean", this, _mean),
	r("r", "Parabola root", this, _r)
{
        
}

ParabolaPdf::ParabolaPdf(const ParabolaPdf& other, const char* name) :
RooAbsPdf(other, name),
x("x", this, other.x),
mean("mean", this, other.mean),
r("r", this, other.r){
}

Double_t ParabolaPdf::evaluate() const {
	Double_t arg = x - mean;
	if (arg < -r) return 0.;
	if (arg > r) return 0.;
	return (-arg*arg + r*r);
}

Int_t ParabolaPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars, analVars, x)) return 1;
	return 0;
}

Double_t ParabolaPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
    switch (code) {
        case 1: {
//          Matlab or Dwight 856.21.
//          syms x
//          syms r
//          int(r*r-x*x, 'x')
//          int(r*r-x*x, 'x', -r, r)            
            if (x.max(rangeName) < mean - r || x.min(rangeName) > mean + r) return 0.;
            Double_t x1 = TMath::Max(-r, x.min(rangeName) - mean);
            Double_t x2 = TMath::Min(r, x.max(rangeName) - mean);
            // ﻿r^2*x - x^3/3
            Double_t integral = (pow(r,2)*x2 - pow(x2,3)/3) - (pow(r,2)*x1 - pow(x1,3)/3);
//          integral *= 4*pow(r,3)/3;﻿r^2*x - x^3/3
            return integral;
        }
    }
    assert(0);
    return 0;
}

