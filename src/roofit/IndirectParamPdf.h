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
#include "../util/Variable.h"

class IndirectParamPdf {
public:
    IndirectParamPdf();
//    IndirectParamPdf(RooRealVar* param);
//    IndirectParamPdf(RooAbsReal& param);
    IndirectParamPdf(const IndirectParamPdf& orig);
    virtual ~IndirectParamPdf();

//    void setParameter(RooRealVar*);
    virtual std::list<Variable*> getParameters(Bool_t isTwoDetector = kTRUE) = 0;
//    RooAbsReal* getParameter();

//private:
//    RooAbsReal* param;
};

#endif /* HASENERGYPDF_H */
