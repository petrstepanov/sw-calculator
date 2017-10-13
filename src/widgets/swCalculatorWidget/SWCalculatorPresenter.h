/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SWCalculatorPresenter.h
 * Author: petrstepanov
 *
 * Created on October 6, 2017, 6:31 PM
 */

#ifndef SWCALCULATORPRESENTER_H
#define SWCALCULATORPRESENTER_H

#include "../AbstractPresenter.h"
#include "SWCalculatorView.h"
#include "../../model/Model.h"

class SWCalculatorView;

class SWCalculatorPresenter : public AbstractPresenter<Model, SWCalculatorView> {
public:
    SWCalculatorPresenter(SWCalculatorView* view);

    // Override base class virtual methods
    Model* instantinateModel();
    
    // View functions
};

#endif /* SWCALCULATORPRESENTER_H */

