/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OrthogonalPdf.h
 * Author: petrstepanov
 *
 * Created on August 20, 2017, 3:55 PM
 */

#ifndef ORTHOGONALPDF_H
#define ORTHOGONALPDF_H

#include <RooAbsPdf.h>
#include <RooAbsReal.h>
#include <RooRealProxy.h>
#include <TString.h>
#include "IndirectParamPdf.h"
#include "Variable.h"

class OrthogonalPdf : public RooAbsPdf, public IndirectParamPdf {
public:
	OrthogonalPdf() {};
	OrthogonalPdf(const char *name, const char *title,
		RooAbsReal& _x,
		RooAbsReal& _mean,
                RooAbsReal& _a1,
                RooAbsReal& _a2,
		RooAbsReal& _epsilon);
	OrthogonalPdf(const OrthogonalPdf& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const { return new OrthogonalPdf(*this, newname); }
	inline virtual ~OrthogonalPdf() { }
	Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName = 0) const;
	Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

        std::list<Variable*> getParameters(Bool_t isTwoDetector);
        
protected:
	RooRealProxy x;
	RooRealProxy mean;
	RooRealProxy a1;
	RooRealProxy a2;
	RooRealProxy epsilon;

	Double_t evaluate() const;

private:
	//ClassDef(OrthogonalPdf, 1) // Your description goes here...
};

#endif /* ORTHOGONALPDF_H */

