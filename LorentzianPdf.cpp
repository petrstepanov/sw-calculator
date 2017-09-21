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
#include "Constants.h"
#include <RooRealVar.h>
#include <TMath.h>

LorentzianPdf::LorentzianPdf(const char *name, const char *title,
	RooAbsReal& _x,
	RooAbsReal& _mean,
	RooAbsReal& _a) :
	RooAbsPdf(name, title),
	x("x", "x", this, _x),
	mean("mean", "Lorentz mean", this, _mean),
	a("a", "Lorentz a", this, _a){
}

LorentzianPdf::LorentzianPdf(const LorentzianPdf& other, const char* name) :
    RooAbsPdf(other, name),
    x("x", this, other.x),
    mean("mean", this, other.mean),
    a("epsilon", this, other.a){
}

Double_t LorentzianPdf::evaluate() const {
    Double_t chbar = Constants::chbar/1E3; // because x is in keVs
    Double_t _x = (x - mean)/chbar;
    return 1/pow(1+pow(a*_x,2),3);
    //    return pow(a,4)/(6*pow(1+pow(a*_x,2),3));
}

Int_t LorentzianPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars, analVars, x)) return 1;
	return 0;
}

Double_t LorentzianPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
    switch (code) {
        case 1: {
            Double_t chbar = Constants::chbar/1E3;
            Double_t x1 = (x.min(rangeName) - mean)/chbar;
            Double_t x2 = (x.max(rangeName) - mean)/chbar;                       
            return indefiniteIntegral(x2) - indefiniteIntegral(x1);
        }
    }
    assert(0);
    return 0;    
}

Double_t LorentzianPdf::indefiniteIntegral(Double_t _x) const {
    Double_t s1 = _x*(5+3*a*a*_x*_x)/pow(1+a*a*_x*_x,2);
    Double_t s2 = 3*atan(a*_x)/a;
    return (s1+s2)/8;
//    return pow(a,4)*(s1+s2)/48;

}

std::list<Variable*> LorentzianPdf::getParameters(Bool_t isTwoDetector){
    // Energy value
    Double_t Ry = Constants::Ry; // eV
    Double_t a_B = Constants::a_B; // A
    Double_t e = isTwoDetector ? Ry*pow(a_B/a, 2) : Ry*pow(a_B/a, 2);
    // Energy error
    Double_t de = 0;
    RooAbsArg* aAbsArg = a.absArg();
    RooRealVar* aReal = dynamic_cast<RooRealVar*>(aAbsArg);
    if (aReal){
        Double_t da = aReal->getError();
        de = isTwoDetector ? 2*3*Ry*a_B*a_B/a/a/a*da : 3/2*Ry*a_B*a_B/a/a/a*da;
    }
    // Build list and return vars
    TString str = TString::Format("Binding E %s", this->GetName());
    Variable* v1 = new Variable(e, de, str.Data(), "eV");
    std::list<Variable*> list;
    list.push_back(v1);
    return list;    
}
