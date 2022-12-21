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
#include "../IndirectParamPdf.h"

class ParabolaPdf: public RooAbsPdf/*, public IndirectParamPdf*/ {
  public:
    ParabolaPdf(Bool_t _isTwoDetector);
    ParabolaPdf(const char *name, const char *title, RooAbsReal &_x, RooAbsReal &_mean, RooAbsReal &_Ef,
      Bool_t isTwoDetector);
    ParabolaPdf(const ParabolaPdf &other, const char *name = 0);
    virtual TObject* clone(const char *newname) const {
      return new ParabolaPdf(*this, newname);
    }
    inline virtual ~ParabolaPdf() {
    }

    Int_t getAnalyticalIntegral(RooArgSet &allVars, RooArgSet &analVars, const char *rangeName = 0) const;
    Double_t analyticalIntegral(Int_t code, const char *rangeName = 0) const;

//    RooArgList* getParameters(Bool_t isTwoDetector);

  protected:
    RooRealProxy x;
    RooRealProxy mean;
    RooRealProxy Ef;

    Bool_t isTwoDetector;

    Double_t evaluate() const;

    Double_t getMean() const;

    Double_t indefiniteIntegral(Double_t _x) const;

    std::pair<double, double> getFermiEnergy() const;
    std::pair<double,double> getBindingEnergy() const;

  private:ClassDef(ParabolaPdf, 1)
};

#endif /* PARABOLAPDF_H */

