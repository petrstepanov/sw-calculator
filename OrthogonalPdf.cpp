/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OrthogonalPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 20, 2017, 3:55 PM
 */

#include "OrthogonalPdf.h"


#include "OrthogonalPdf.h"
#include "Constants.h"
#include <RooRealVar.h>
#include <TMath.h>

OrthogonalPdf::OrthogonalPdf(const char *name, const char *title,
	RooAbsReal& _x,
	RooAbsReal& _mean,
	RooAbsReal& _a1,
	RooAbsReal& _a2,        
	RooAbsReal& _epsilon) :
	RooAbsPdf(name, title), IndirectParamPdf(_epsilon),
	x("x", "x", this, _x),
	mean("mean", "Lorentzian mean", this, _mean),
	a1("a1", "a1", this, _a1),
	a2("a2", "a2", this, _a2),
	epsilon("s", "Horizontal stretch", this, _epsilon){
}

OrthogonalPdf::OrthogonalPdf(const OrthogonalPdf& other, const char* name) :
RooAbsPdf(other, name),
x("x", this, other.x),
mean("mean", this, other.mean),
a1("mean", this, other.a1),
a2("mean", this, other.a2),
epsilon("epsilon", this, other.epsilon){
}

Double_t OrthogonalPdf::evaluate() const {
	Double_t arg = x - mean;
        Double_t a = 3*a1*a1/(a1+a2)+a2*(a2+3*a1*a1/(a1+a2))*arg*arg;
        Double_t b = 3*a1*a1/(a1+a2)+a2*a2*(a2+3*a1*a1/(a1+a2))*arg*arg;
        return (23+5*a*b/(a2*a2))/pow(1+pow(a2*arg,2),5);
}

Int_t OrthogonalPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const {
	if (matchArgs(allVars, analVars, x)) return 1;
	return 0;
}

Double_t OrthogonalPdf::analyticalIntegral(Int_t code, const char* rangeName) const {
    switch (code) {
        case 1: {
            Double_t x1 = x.min(rangeName) - mean;
            Double_t x2 = x.max(rangeName) - mean;
            Double_t e = epsilon;
            Double_t b = 3*a1*a2/(a1+a2);

            Double_t m1 = 48*a2*a2*(23*a2*a2-5*a2*(-1+b)+5*b)*x2*pow(e,7)/pow(a2*a2*x2*x2+e*e,4);
            Double_t m2 = 8*a2*(161*pow(a2,3) + 5*a2*a2*(-9+b)-40*b*b+5*a2*b*(-17+8*b))*x2*pow(e,5)/pow(a2*a2*x2*x2+e*e,3);
            Double_t m3 = 10*a2*(161*pow(a2,3)+8*b*b+a2*a2*(3+5*b)+a2*b*(11+40*b))*x2*pow(e,3)/pow(a2*a2*x2*x2+e*e,2);
            Double_t m4 = 15*a2*(161*pow(a2,3)+8*b*b+a2*a2*(3+5*b)+a2*b*(11+40*b))*x2*e/(a2*a2*x2*x2+e*e);
            Double_t m5 = 15*(161*pow(a2,3)+8*b*b+a2*a2*(3+5*b)+a2*b*(11+40*b))*atan(a2*x2/2);
            Double_t ix2 = e*(m1+m2+m3+m4+m5)/(384*pow(a2,4));
            
            Double_t n1 = 48*a2*a2*(23*a2*a2-5*a2*(-1+b)+5*b)*x1*pow(e,7)/pow(a2*a2*x1*x1+e*e,4);
            Double_t n2 = 8*a2*(161*pow(a2,3) + 5*a2*a2*(-9+b)-40*b*b+5*a2*b*(-17+8*b))*x1*pow(e,5)/pow(a2*a2*x1*x1+e*e,3);
            Double_t n3 = 10*a2*(161*pow(a2,3)+8*b*b+a2*a2*(3+5*b)+a2*b*(11+40*b))*x1*pow(e,3)/pow(a2*a2*x1*x1+e*e,2);
            Double_t n4 = 15*a2*(161*pow(a2,3)+8*b*b+a2*a2*(3+5*b)+a2*b*(11+40*b))*x1*e/(a2*a2*x1*x1+e*e);
            Double_t n5 = 15*(161*pow(a2,3)+8*b*b+a2*a2*(3+5*b)+a2*b*(11+40*b))*atan(a2*x1/2);
            Double_t ix1 = e*(n1+n2+n3+n4+n5)/(384*pow(a2,4));
            
            return ix2-ix1;
        }
    }
    assert(0);
    return 0;    
}

std::list<Variable*> OrthogonalPdf::getParameters(Bool_t isTwoDetector){
    Double_t sigma = getParameter()->getVal() * 1e3; // eV
    Double_t Ry = Constants::Ry; // eV
    Double_t a = Constants::chbar / sigma; // A
    Double_t a_B = Constants::a_B; // A
    Double_t e = isTwoDetector ? Ry*pow(a_B/a, 2) : Ry*pow(a_B/a, 2);
    Double_t de = isTwoDetector ? 0 : 0;
    Variable* v1 = new Variable(e, de, "Binding E (exp wf)", "eV");
    Variable* v2 = new Variable(a, 0, "a (exp wf)", "A");
    std::list<Variable*> list;
//    list.push_back(v1);
//    list.push_back(v2);
    return list;    
}
