/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GaussianPdf.h
 * Author: petrstepanov
 *
 * Created on August 16, 2017, 2:10 AM
 */

#ifndef GAUSSIANPDF_H
#define GAUSSIANPDF_H

#include <RooAbsPdf.h>
#include <RooAbsReal.h>
#include <RooRealProxy.h>
#include <TString.h>
#include "IndirectParamPdf.h"
#include "Variable.h"

class GaussianPdf : public RooAbsPdf, public IndirectParamPdf {
public:
    GaussianPdf() {};
    GaussianPdf(const char *name, const char *title,
            RooAbsReal& _x,
            RooAbsReal& _mean, 
            RooAbsReal& _a);
    GaussianPdf(const GaussianPdf& other, const char* name = 0);
    virtual TObject* clone(const char* newname) const { return new GaussianPdf(*this, newname); }
    inline virtual ~GaussianPdf() { }        
    Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName = 0) const;
    Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;
    
    std::list<Variable*> getParameters(Bool_t isTwoDetector);        

//protected:
    RooRealProxy x ;
    RooRealProxy mean ;
    RooRealProxy a ;

protected:
    Double_t indefiniteIntegral(Double_t _x) const;    
    Double_t evaluate() const ;
        
private:

};

#endif /* GAUSSIANPDF_H */

