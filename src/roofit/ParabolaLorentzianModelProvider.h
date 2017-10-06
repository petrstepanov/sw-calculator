/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParabolaLorentzianModelProvider.h
 * Author: petrstepanov
 *
 * Created on August 2, 2017, 11:48 PM
 */

#ifndef PARABOLALORENTZIANMODELPROVIDER_H
#define PARABOLALORENTZIANMODELPROVIDER_H

#include "ParabolaGaussModelProvider.h"
#include "AbstractModelProvider.h"
#include <RooRealVar.h>

class ParabolaLorentzianModelProvider : public AbstractModelProvider {
protected:
	RooRealVar* parabolaRoot;
        RooRealVar** epsilon;

public:
	ParabolaLorentzianModelProvider(RooRealVar*, RooRealVar*, Bool_t, Double_t, const Int_t, Bool_t);
	RooRealVar* getParabolaRoot();
        RooRealVar** getLorentzianCoefficients();
	const Int_t maxLorentzNumber = 4;
};

#endif /* PARABOLALORENTZIANMODELPROVIDER_H */

