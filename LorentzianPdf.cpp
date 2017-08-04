/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LorentzianPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 2, 2017, 11:24 PM
 */

#include "LorentzianPdf.h"
#include "TMath.h"

LorentzianPdf::LorentzianPdf(const char *name, const char *title,
	RooAbsReal& _x,
	RooAbsReal& _mean,
	RooAbsReal& _epsilon) :
	RooAbsPdf(name, title),
	x("x", "x", this, _x),
	mean("mean", "Lorentzian mean", this, _mean),
	epsilon("s", "Horizontal stretch", this, _epsilon){
}

LorentzianPdf::LorentzianPdf(const LorentzianPdf& other, const char* name) :
RooAbsPdf(other, name),
x("x", this, other.x),
mean("mean", this, other.mean),
epsilon("epsilon", this, other.epsilon){
}

Double_t LorentzianPdf::evaluate() const {
	Double_t arg = x - mean;
	return 1/pow(1+pow(arg/epsilon,2),3);
}

Int_t LorentzianPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars, analVars, x)) return 1;
	return 0;
}

Double_t LorentzianPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
    switch (code) {
        case 1: {
//          Matlab or Dwight 856.21.
//          syms x
//          syms epsilon
//          int(1/(1+(x/epsilon)^2)^3, 'x')
//          int(1/(1+(x/epsilon)^2)^3, 'x', -inf, inf)
            Double_t x1 = x.min(rangeName) - mean;
            Double_t x2 = x.max(rangeName) - mean;
            Double_t integral = 0;
            // ﻿((5*epsilon^4*x)/8 + (3*epsilon^2*x^3)/8)/(epsilon^4 + 2*epsilon^2*x^2 + x^4) + (3*epsilon*atan(x/epsilon))/8
            integral += ((5*pow(epsilon,4)*x2)/8 + (3*pow(epsilon,2)*pow(x2,3))/8)/(pow(epsilon,4) + 2*pow(epsilon*x2,2) + pow(x2,4)) + 3*epsilon*atan(x2/epsilon)/8;
            integral -= ((5*pow(epsilon,4)*x1)/8 + (3*pow(epsilon,2)*pow(x1,3))/8)/(pow(epsilon,4) + 2*pow(epsilon*x1,2) + pow(x1,4)) + 3*epsilon*atan(x1/epsilon)/8;
//            integral *= 3 * epsilon * TMath::Pi() / 8;
            return integral;
        }
    }
    assert(0);
    return 0;    
}
        