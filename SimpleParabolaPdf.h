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

#ifndef SIMPLEPARABOLAPDF_H
#define SIMPLEPARABOLAPDF_H

#include <RooAbsPdf.h>
#include <RooAbsReal.h>
#include <RooRealProxy.h>

class SimpleParabolaPdf : public RooAbsPdf {
public:
	SimpleParabolaPdf() {};
	SimpleParabolaPdf(const char *name, const char *title,
		RooAbsReal& _x,
		RooAbsReal& _mean,
		RooAbsReal& _r);
	SimpleParabolaPdf(const SimpleParabolaPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const { return new SimpleParabolaPdf(*this, newname); }
	inline virtual ~SimpleParabolaPdf() { }
	Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName = 0) const;
	Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

protected:
	RooRealProxy x;
	RooRealProxy mean;
	RooRealProxy r;

	Double_t evaluate() const;

private:
	//ClassDef(SimpleParabolaPdf, 1) // Your description goes here...
};

#endif /* SIMPLEPARABOLAPDF_H */

