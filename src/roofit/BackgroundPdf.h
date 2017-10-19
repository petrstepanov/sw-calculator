/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BackgroundPdf.h
 * Author: petrstepanov
 *
 * Created on October 19, 2017, 2:02 AM
 */

#ifndef BACKGROUNDPDF_H
#define BACKGROUNDPDF_H

#include <RooAbsPdf.h>
#include <RooAbsReal.h>
#include <RooRealProxy.h>
#include <TString.h>
#include "IndirectParamPdf.h"
#include "../util/Variable.h"

class BackgroundPdf : public RooAbsPdf {
public:
    BackgroundPdf() {};
    BackgroundPdf(const char *name, const char *title,
            RooAbsReal& _x,
            RooAbsReal& _threeGContrib);
    BackgroundPdf(const BackgroundPdf& other, const char* name = 0);
    virtual TObject* clone(const char* newname) const { return new BackgroundPdf(*this, newname); }
    inline virtual ~BackgroundPdf() { }             

//protected:
    RooRealProxy x ;
    RooRealProxy threeGContrib ;

protected:  
    Double_t evaluate() const ;
        
private:

};

#endif /* BACKGROUNDPDF_H */

