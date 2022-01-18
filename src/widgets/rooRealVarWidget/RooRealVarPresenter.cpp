/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RooRealVarPresenter.cpp
 * Author: petrstepanov
 * 
 * Created on October 23, 2017, 1:23 AM
 */

#include "RooRealVarPresenter.h"
#include "RooRealVarView.h"
#include <TString.h>
#include <iostream>

ClassImp(RooRealVarPresenter)

RooRealVarPresenter::RooRealVarPresenter(RooRealVarView* view) : 
    AbstractPresenter<RooRealVar, RooRealVarView>(view) {
}

void RooRealVarPresenter::onInitModel() {
    std::cout << "RooRealVarPresenter::onInitModel" << std::endl;
    TString* description = new TString(model->getTitle());
    if(strlen(model->getUnit()) > 0) {
        description->Append(", ");
        description->Append(model->getUnit());        
    }
    view->setDescription(description);
    view->setValue(model->getVal());
    view->setMinimum(model->getMin());
    view->setMaximum(model->getMax());
    view->setConstant(model->isConstant());
}


void RooRealVarPresenter::onMaximumChange(Double_t max) {
    model->setMax(max);
}

void RooRealVarPresenter::onMinimumChange(Double_t min) {
    model->setMin(min);
}

void RooRealVarPresenter::onSetConstant(Bool_t isConstant) {
    model->setConstant(isConstant);
}

void RooRealVarPresenter::onValueChange(Double_t value) {
    model->setVal(value);
}
