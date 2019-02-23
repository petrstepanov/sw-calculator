/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   HasEnergyPdf.h
 * Author: petrstepanov
 *
 * Created on August 15, 2017, 10:03 PM
 */

#ifndef HASENERGYPDF_H
#define HASENERGYPDF_H

#include <TROOT.h>
#include <RooRealVar.h>
#include <utility>

class IndirectParamPdf {
public:
    IndirectParamPdf();
    IndirectParamPdf(const IndirectParamPdf& orig);
    virtual ~IndirectParamPdf();

    virtual RooArgList* getParameters(Bool_t isTwoDetector = kTRUE) = 0;
};

#endif /* HASENERGYPDF_H */
