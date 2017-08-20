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

#include <RooGaussian.h>
#include <RooAbsReal.h>
#include <RooRealProxy.h>
#include <TString.h>
#include "IndirectParamPdf.h"

class GaussianPdf : public RooGaussian, public IndirectParamPdf {
public:
        GaussianPdf() {};
        GaussianPdf(const char *name, const char *title,
                RooAbsReal& _x,
                RooAbsReal& _mean, 
                RooAbsReal& _sigma);
	GaussianPdf(const GaussianPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const { return new GaussianPdf(*this, newname); }
	inline virtual ~GaussianPdf() { }        

        std::pair<Double_t, Double_t> getParameterValue(Bool_t isTwoDetector);        
        TString getParameterName(); 
        
private:

};

#endif /* GAUSSIANPDF_H */

