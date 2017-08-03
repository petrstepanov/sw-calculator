/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParabolaGaussModelProvider.h
 * Author: petrstepanov
 *
 * Created on August 2, 2017, 5:21 AM
 */

#ifndef PARABOLAGAUSSMODELPROVIDER_H
#define PARABOLAGAUSSMODELPROVIDER_H

#include "ParabolaGaussModelProvider.h"
#include "AbstractModelProvider.h"
#include <RooRealVar.h>

class ParabolaGaussModelProvider : public AbstractModelProvider {
protected:
	RooRealVar* parabolaRoot;

public:
	ParabolaGaussModelProvider(RooRealVar*, RooRealVar*, Bool_t, Double_t, const Int_t, Bool_t);
	RooRealVar* getParabolaRoot();
	const Int_t maxGaussNumber = 4;
};

#endif /* PARABOLAGAUSSMODELPROVIDER_H */

