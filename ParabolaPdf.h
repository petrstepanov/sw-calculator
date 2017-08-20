/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimpleParabolaPdf.h
 * Author: petrstepanov
 *
 * Created on August 2, 2017, 5:34 AM
 */

#ifndef PARABOLAPDF_H
#define PARABOLAPDF_H

#include <RooAbsPdf.h>
#include <RooAbsReal.h>
#include <RooRealProxy.h>
#include "IndirectParamPdf.h"

class ParabolaPdf : public RooAbsPdf, public IndirectParamPdf {
public:
	ParabolaPdf() {};
	ParabolaPdf(const char *name, const char *title,
		RooAbsReal& _x,
		RooAbsReal& _mean,
		RooAbsReal& _r);
	ParabolaPdf(const ParabolaPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const { return new ParabolaPdf(*this, newname); }
	inline virtual ~ParabolaPdf() { }
	Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName = 0) const;
	Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

        std::pair<Double_t, Double_t> getParameterValue(Bool_t isTwoDetector);
        TString getParameterName();
        
protected:
	RooRealProxy x;
	RooRealProxy mean;
	RooRealProxy r;      
        
	Double_t evaluate() const;

private:
	//ClassDef(SimpleParabolaPdf, 1) // Your description goes here...
};

#endif /* PARABOLAPDF_H */

