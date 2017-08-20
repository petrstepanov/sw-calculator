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

GaussianPdf::GaussianPdf(const char *name, const char *title,
        RooAbsReal& _x, RooAbsReal& _mean,
        RooAbsReal& _sigma) :
        RooGaussian(name,title, _x, _mean, _sigma), IndirectParamPdf(_sigma){
}

GaussianPdf::GaussianPdf(const GaussianPdf& other, const char* name) : 
        RooGaussian(other, name){
}

std::pair<Double_t, Double_t> GaussianPdf::getParameterValue(Bool_t isTwoDetector){
    Double_t s = getParameter()->getVal() * 1e3; // [eV]
    Double_t c_hbar = 0.197 * 1E4; // [eV * A]
    Double_t a = c_hbar / s; // [nm]
    Double_t Ry = 13.6; // [eV]
    Double_t a_B = 0.529; // [A] 
    // TODO: check for single detector
    Double_t e = isTwoDetector ? 3*Ry*pow(a_B/a, 2) : 3*Ry*pow(a_B/a, 2);
    Double_t de = isTwoDetector ? 0 : 0;
    return std::make_pair(e, de);
}

TString GaussianPdf::getParameterName(){
    return TString("e- binding energy G");
}