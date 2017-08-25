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
	RooAbsReal& _a) :
	RooAbsPdf(name, title),
	x("x", "x", this, _x),
	mean("mean", "DampExp mean", this, _mean),
	a("a", "DampExp a", this, _a){
}

DampLorentzPdf::DampLorentzPdf(const DampLorentzPdf& other, const char* name) :
RooAbsPdf(other, name),
x("x", this, other.x),
mean("mean", this, other.mean),
a("a", this, other.a){
}

Double_t DampLorentzPdf::evaluate() const {
    Double_t chbar = Constants::chbar/1E3; // because x is in keVs
    Double_t _x = (x - mean)/chbar;
    return (23-20*pow(a*_x,2)+5*pow(a*_x,4))/pow(1+pow(a*_x,2),5);
//    return pow(a,6)*(23-20*pow(a*_x,2)+5*pow(a*_x,4))/(30*pow(1+pow(a*_x,2),5));
}

Int_t DampLorentzPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars, analVars, x)) return 1;
	return 0;
}

Double_t DampLorentzPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
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

Double_t DampLorentzPdf::indefiniteIntegral(Double_t _x) const {
    // With Dropped Constant parameter -- works good here
    return (a*_x*(139+211*pow(a*_x,2)+165*pow(a*_x,4)+45*pow(a*_x,6))+45*pow(1+pow(a*_x,2),4)*atan(a*_x))/(8*a*pow(1+pow(a*_x,2),4));
//    return pow(a,5)*(a*_x*(139+211*pow(a*_x,2)+165*pow(a*_x,4)+45*pow(a*_x,6))+45*pow(1+pow(a*_x,2),4)*atan(a*_x))/(240*pow(1+pow(a*_x,2),4));
}

std::list<Variable*> DampLorentzPdf::getParameters(Bool_t isTwoDetector){
    // Energy value
    Double_t Ry = Constants::Ry; // eV
    Double_t a_B = Constants::a_B; // A
    Double_t e = isTwoDetector ? Ry*pow(a_B/a,2) : Ry*pow(a_B/a,2);
    // Energy error
    Double_t de = 0;
    RooAbsArg* aAbsArg = a.absArg();
    RooRealVar* aReal = dynamic_cast<RooRealVar*>(aAbsArg);
    if (aReal){
        aReal->Print();
        Double_t dA = aReal->getError();
        de = isTwoDetector ? Ry*pow(a_B/dA,2) : Ry*pow(a_B/dA,2);
    }
    // Build list and return vars    
    TString str = TString::Format("Binding E (%s)", this->GetName());
    Variable* v1 = new Variable(e, de, str.Data(), "eV");
    std::list<Variable*> list;
    list.push_back(v1);
    return list;    
}
