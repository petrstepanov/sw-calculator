/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HasEnergyPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 15, 2017, 10:03 PM
 */

#include "IndirectParamPdf.h"
#include <iostream>

IndirectParamPdf::IndirectParamPdf() {
}

IndirectParamPdf::IndirectParamPdf(RooRealVar* param) {
//    this->param = param;
}

IndirectParamPdf::IndirectParamPdf(RooAbsReal& param) {
//    std::cout << "IndirectParamPdf: saving RooAbsReal parameter" << std::endl;
    this->param = &param;
//    param.Print();
    //    RooRealVar* paramRooRealVar = dynamic_cast<RooRealVar*>(&param);
//    if (paramRooRealVar != NULL){
//        this->param->Print();
//    }
//    RooRealVar* paramRooRealVar = dynamic_cast<RooRealVar*>(&param);
//    if (paramRooRealVar != NULL){
//        this->param->Print();
//    }    
}

IndirectParamPdf::IndirectParamPdf(const IndirectParamPdf& orig) {
}
//
//void IndirectParamPdf::setParameter(RooRealVar* param){
//    this->param = param;
//}

RooAbsReal* IndirectParamPdf::getParameter(){
    return param;
}

IndirectParamPdf::~IndirectParamPdf() {
}

