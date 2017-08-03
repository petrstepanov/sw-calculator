/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AbstractModelProvider.h
 * Author: petrstepanov
 *
 * Created on August 2, 2017, 5:17 AM
 */

#ifndef ABSTRACTMODELPROVIDER_H
#define ABSTRACTMODELPROVIDER_H

#include <RooRealVar.h>
#include <RooConstVar.h>
#include <RooAbsPdf.h>
#include <RooArgSet.h>

class AbstractModelProvider {
protected:
	RooRealVar* E_0;
	RooAbsPdf* model;
	RooAbsPdf* convolutedModel;
	RooAbsPdf* resolutionFunction;
	RooConstVar* fwhm2sigma;
	RooConstVar* pi;
	RooArgSet* components;
	RooArgSet* bgComponents;
public:
	AbstractModelProvider(RooRealVar*);
	virtual ~AbstractModelProvider(){};
	//virtual RooArgSet getBackgroundSet() = 0;
	RooAbsPdf* getModel();
	RooAbsPdf* getConvolutedModel();
	RooAbsPdf* getResolutionFuncton();
	RooRealVar* getPeakCenter();
	RooArgSet* getComponents();
	RooArgSet* getBgComponents();
};

#endif /* ABSTRACTMODELPROVIDER_H */

