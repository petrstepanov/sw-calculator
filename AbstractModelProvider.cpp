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

AbstractModelProvider::AbstractModelProvider(RooRealVar* E_0){
	this->E_0 = E_0; //new RooRealVar("E_0", "Peak Center", 511, 507, 515, "KeV");
	this->fwhm2sigma = new RooConstVar("fwhm2sigma", "Coefficient to convert FWHM to dispersion", 0.42466);
	this->pi = new RooConstVar("pi", "pi", TMath::Pi());
	this->model = NULL;
	this->convolutedModel = NULL;
	this->components = new RooArgSet();
	this->bgComponents = new RooArgSet();
        this->convolutionPoints = 3000;
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

RooRealVar* AbstractModelProvider::getPeakCenter(){
	return E_0;
}

RooArgSet* AbstractModelProvider::getComponents(){
	return components;
}

RooArgSet* AbstractModelProvider::getBgComponents(){
	return bgComponents;
}