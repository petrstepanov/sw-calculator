/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RooRealVarPresenter.h
 * Author: petrstepanov
 *
 * Created on October 23, 2017, 1:23 AM
 */

#ifndef ROOREALVARPRESENTER_H
#define ROOREALVARPRESENTER_H

#include "../AbstractPresenter.h"
//#include "RooRealVarView.h"
#include <RooRealVar.h>

class RooRealVarView;

class RooRealVarPresenter : public AbstractPresenter<RooRealVar, RooRealVarView> {
public:
    RooRealVarPresenter(RooRealVarView* view);

    virtual ~RooRealVarPresenter(){
    }

    // Override base class virtual methods (obsolete here)
    RooRealVar* instantinateModel(){
        return new RooRealVar();
    }
    
    void onInitModel();
    
    // View functions
    void onValueChange(Double_t value);
    void onMinimumChange(Double_t min);
    void onMaximumChange(Double_t max);
    void onSetConstant(Bool_t isConstant);

    ClassDef(RooRealVarPresenter,0)
};

#endif /* ROOREALVARPRESENTER_H */

