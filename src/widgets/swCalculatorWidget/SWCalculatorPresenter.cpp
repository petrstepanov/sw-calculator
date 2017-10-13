/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SWCalculatorPresenter.cpp
 * Author: petrstepanov
 * 
 * Created on October 6, 2017, 6:31 PM
 */

#include "SWCalculatorPresenter.h"
#include "SWCalculatorView.h"
#include "../../model/Model.h"

SWCalculatorPresenter::SWCalculatorPresenter(SWCalculatorView* view) : AbstractPresenter<Model, SWCalculatorView>(view) {
}

Model* SWCalculatorPresenter::instantinateModel(){
    return Model::getInstance();
}


