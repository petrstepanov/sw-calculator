/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DampLorentzPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 19, 2017, 10:15 PM
 */

#include "DampLorentzPdf.h"
#include "Constants.h"
#include <RooRealVar.h>
#include <TMath.h>

DampLorentzPdf::DampLorentzPdf(const char *name, const char *title,
	RooAbsReal& _x,
	RooAbsReal& _mean,
	RooAbsReal& _epsilon) :
	RooAbsPdf(name, title), IndirectParamPdf(_epsilon),
	x("x", "x", this, _x),
	mean("mean", "Lorentzian mean", this, _mean),
	epsilon("s", "Horizontal stretch", this, _epsilon){
}

DampLorentzPdf::DampLorentzPdf(const DampLorentzPdf& other, const char* name) :
RooAbsPdf(other, name),
x("x", this, other.x),
mean("mean", this, other.mean),
epsilon("epsilon", this, other.epsilon){
}

Double_t DampLorentzPdf::evaluate() const {
	Double_t arg = x - mean;
        Double_t a = 23/(30*pow(1+pow(arg/epsilon,2),5));
        Double_t b = -2*pow(arg/epsilon,2)/(3*pow(1+pow(arg/epsilon,2),5));
        Double_t c = pow(arg/epsilon,4)/(6*pow(1+pow(arg/epsilon,2),5));
        return a+b+c;
	// return 1/pow(1+pow(arg/epsilon,2),5);
}

Int_t DampLorentzPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars, analVars, x)) return 1;
	return 0;
}

Double_t DampLorentzPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
    switch (code) {
        case 1: {
            Double_t x1 = x.min(rangeName) - mean;
            Double_t x2 = x.max(rangeName) - mean;
            Double_t e = epsilon;
            Double_t integral = 0;
            integral += e*(45*pow(x2,7)*e + 165*pow(x2,5)*pow(e,3) + 211*pow(x2,3)*pow(e,5) + 139*x2*pow(e,7)+45*pow(x2*x2+e*e,4)*atan(x2/e))/(240*pow(x2*x2+e*e, 4));
            integral -= e*(45*pow(x1,7)*e + 165*pow(x1,5)*pow(e,3) + 211*pow(x1,3)*pow(e,5) + 139*x1*pow(e,7)+45*pow(x1*x1+e*e,4)*atan(x1/e))/(240*pow(x2*x2+e*e, 4));
            return integral;
        }
    }
    assert(0);
    return 0;    
}

std::list<Variable*> DampLorentzPdf::getParameters(Bool_t isTwoDetector){
    Double_t sigma = getParameter()->getVal() * 1e3; // eV
    Double_t Ry = Constants::Ry; // eV
    Double_t a = Constants::chbar / sigma; // A
    Double_t a_B = Constants::a_B; // A
    Double_t e = isTwoDetector ? Ry*pow(a_B/a, 2) : Ry*pow(a_B/a, 2);
    Double_t de = isTwoDetector ? 0 : 0;
    Variable* v1 = new Variable(e, de, "Binding E (exp*r wf)", "eV");
    Variable* v2 = new Variable(a, 0, "a (exp*r wf)", "A");
    std::list<Variable*> list;
    list.push_back(v1);
    list.push_back(v2);
    return list;    
}
