/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AbstractModelProvider.cpp
 * Author: petrstepanov
 * 
 * Created on August 2, 2017, 5:17 AM
 */

#include "AbstractModelProvider.h"
#include <TMath.h>

AbstractModelProvider::AbstractModelProvider(RooRealVar* mean){
	this->mean = mean;
	this->model = nullptr;
	this->convolutedModel = nullptr;
	this->components = new RooArgList();
	this->bgComponents = new RooArgList();
//    this->convolutionPoints = 10000;
    this->resolutionFunction = nullptr;
}

RooAbsPdf* AbstractModelProvider::getModel(){
	return model;
}

RooAbsPdf* AbstractModelProvider::getConvolutedModel(){
	return convolutedModel;
}

RooAbsPdf* AbstractModelProvider::getResolutionFuncton(){
	return resolutionFunction;
}

RooRealVar* AbstractModelProvider::getMean(){
	return mean;
}

RooArgList* AbstractModelProvider::getComponents(){
	return components;
}

RooArgList* AbstractModelProvider::getBgComponents(){
	return bgComponents;
}
