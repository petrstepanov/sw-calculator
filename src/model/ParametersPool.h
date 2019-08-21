/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParametersPool.h
 * Author: petrstepanov
 *
 * Created on November 9, 2016, 6:08 PM
 *
 *  * Class that Stores RooRealVars in file
 */

#ifndef MY_PARAMSTORAGE
#define MY_PARAMSTORAGE

#include "RooArgSet.h"
#include "RooRealVar.h"
#include <iostream>
#include <algorithm>

class ParametersPool {
public:
    ParametersPool();

    void synchronizePdfParameters(RooArgSet* modelParameters);

private:
    RooArgSet* parametersPool;
};

#endif /* PARAMSTORAGE_H */
