/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GaussianPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 16, 2017, 2:10 AM
 */

#include "GaussianPdf.h"
#include "Constants.h"

GaussianPdf::GaussianPdf(const char *name, const char *title,
        RooAbsReal& _x, RooAbsReal& _mean,
        RooAbsReal& _sigma) :
        RooGaussian(name,title, _x, _mean, _sigma), IndirectParamPdf(_sigma){
}

GaussianPdf::GaussianPdf(const GaussianPdf& other, const char* name) : 
        RooGaussian(other, name){
}

std::list<Variable*> GaussianPdf::getParameters(Bool_t isTwoDetector){
    Double_t sigma = getParameter()->getVal() * 1e3; // eV
    Double_t Ry = Constants::Ry; // eV
    Double_t a = Constants::chbar / sigma; // A
    Double_t a_B = Constants::a_B; // A
    Double_t e = isTwoDetector ? 3*Ry*pow(a_B/a, 2) : 3*Ry*pow(a_B/a, 2);
    Double_t de = isTwoDetector ? 0 : 0;
    Variable* v1 = new Variable(e, de, "Binding E (gauss wf)", "eV");
    Variable* v2 = new Variable(a, 0, "a (gauss wf)", "A");
    std::list<Variable*> list;
    list.push_back(v1);
    list.push_back(v2);
    return list;
}