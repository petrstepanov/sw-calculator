/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimpleDeltaPdf.h
 * Author: petrstepanov
 *
 * Created on August 2, 2017, 5:34 AM
 */

#ifndef DeltaPdf_H
#define DeltaPdf_H

#include <RooAbsPdf.h>
#include <RooAbsReal.h>
#include <RooRealProxy.h>

class DeltaPdf: public RooAbsPdf {
public:
//	DeltaPdf() {};
	DeltaPdf(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _mean);
	DeltaPdf(const DeltaPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const {
		return new DeltaPdf(*this, newname);
	}
	inline virtual ~DeltaPdf() {}

	Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName = 0) const;
	Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

protected:
	RooRealProxy x;
	RooRealProxy mean;

	Double_t evaluate() const;

private:
	Double_t epsilon = 0.0001;

ClassDef(DeltaPdf, 1)
};

#endif /* DeltaPdf_H */

