/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LorentzianPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 2, 2017, 11:24 PM
 */

#include "LorentzianPdf.h"
#include "../../model/Constants.h"
#include <RooRealVar.h>
#include <TMath.h>

LorentzianPdf::LorentzianPdf(const char *name, const char *title, RooAbsReal &_x, RooAbsReal &_mean, RooAbsReal &_a,
  Bool_t _isTwoDetector) : RooAbsPdf(name, title), x("x", "x", this, _x), mean("mean", "mean", this, _mean), a("a", "a",
                                                                                                               this,
                                                                                                               _a), isTwoDetector(
    _isTwoDetector) {
}

LorentzianPdf::LorentzianPdf(const LorentzianPdf &other, const char *name) : RooAbsPdf(other, name), x("x", this,
                                                                                                       other.x), mean(
    "mean", this, other.mean), a("a", this, other.a), isTwoDetector(other.isTwoDetector) {
}

Double_t LorentzianPdf::evaluate() const {

  // Binding energy shift exists only in 1D case
  Double_t ch = Constants::chbarKeV; // c*ħ (in KeV)

  // One dimensional: "wolfram/exp-1d.nb" and "_PAS_RU.pdf", pp.28
  // 1D spectroscopy: shift = mean(511) - E_b/2
  if (!isTwoDetector) {
    Double_t B = getBindingEnergy().first / 1000; // should be in keV
    // Non normalized
    return TMath::Power(1 + (4 * TMath::Power(a, 2) * TMath::Power(B / 2. + x - mean, 2)) / (TMath::Power(ch, 2)), -3);
  }

  // Two dimensional: "wolfram/exp-2d.nb" and "_PAS_RU.pdf", pp.29
  // 2D spectroscopy: shift = 0
  return TMath::Power(1 + (TMath::Power(a, 2) * TMath::Power(x, 2)) / (TMath::Power(ch, 2)), -3);
}

Int_t LorentzianPdf::getAnalyticalIntegral(RooArgSet &allVars, RooArgSet &analVars, const char* /*rangeName*/) const {
  if (matchArgs(allVars, analVars, x))
    return 1;
  return 0;
}

Double_t LorentzianPdf::analyticalIntegral(Int_t code, const char *rangeName) const {
  switch (code) {
  case 1: {
    Double_t x1 = x.min(rangeName);
    Double_t x2 = x.max(rangeName);
    return indefiniteIntegral(x2) - indefiniteIntegral(x1);
  }
  }
  assert(0);
  return 0;
}

Double_t LorentzianPdf::indefiniteIntegral(Double_t _x) const {
  // Important: argument here should be _x, not x!

  Double_t ch = Constants::chbarKeV;
  // One-dimensional case: "wolfram/exp-1d.mb"
  if (!isTwoDetector) {
    Double_t B = getBindingEnergy().first / 1000; // need KeV here because x axis is in keV
    // For Non-Normalized function (works ok)
    Double_t center = B + 2 * _x - 2 * mean;
    return (ch
        * ((2 * TMath::Power(ch, 3) * center) / TMath::Power(
            TMath::Power(ch, 2) + TMath::Power(a, 2) * TMath::Power(center, 2), 2)
           + (3 * ch * center) / (TMath::Power(ch, 2) + TMath::Power(a, 2) * TMath::Power(center, 2))
           + (3 * TMath::ATan((a * center) / ch)) / a))
           / 16.;
  }

// Two-dimensional case: "wolfram/gaus-2d.mb"
  return (TMath::Power(ch, 4) * _x)
      / (4. * TMath::Power(TMath::Power(ch, 2) + TMath::Power(a, 2) * TMath::Power(_x, 2), 2))
         + (3 * TMath::Power(ch, 2) * _x) / (8. * (TMath::Power(ch, 2) + TMath::Power(a, 2) * TMath::Power(_x, 2)))
         + (3 * ch * TMath::ATan((a * _x) / ch)) / (8. * a);
}

// Important - this is in eV, not keV!
//             this is same for 1D and 2D cases because wave function is the same
std::pair<double, double> LorentzianPdf::getBindingEnergy() const {
// Value
  Double_t Ry = Constants::Ry; // eV
  Double_t a_B = Constants::a_B; // A
  Double_t eB = Ry * pow(a_B / a, 2);

// Error
  RooRealVar *aRealVar = (RooRealVar*) a.absArg();
  Double_t aVal = aRealVar ? aRealVar->getVal() : 0;
  Double_t aErr = aRealVar ? aRealVar->getError() : 0;

// See error propagation in "gaus-1d.nb"
  Double_t eBErr = 2 * Ry * a_B * a_B * aErr / aVal / aVal / aVal;

  return std::make_pair(eB, eBErr);
}

// Important - this is in eV, not keV!
std::pair<double, double> LorentzianPdf::getSTD(Bool_t isTwoDetector) const {
  Double_t ch = Constants::chbar;
  if (!isTwoDetector) {
    // 1D case see "gaus-1d.nb"
    // Value
    Double_t stdValue = ch / 2 / sqrt(3) / a;

    // Error
    RooRealVar *aRealVar = (RooRealVar*) a.absArg();
    // Double_t aVal = aRealVar ? aRealVar->getVal() : 0;
    Double_t aErr = aRealVar ? aRealVar->getError() : 0;
    // Uncertanty propagation: https://en.wikipedia.org/wiki/Propagation_of_uncertainty
    Double_t stdError = ch * aErr / 2 / sqrt(3) / a / a;

    return std::make_pair(stdValue, stdError);
  }

// 2D case see "gaus-1d.nb"
  Double_t stdValue = ch / a / sqrt(3);
  RooRealVar *aRealVar = (RooRealVar*) a.absArg();
// Double_t aVal = aRealVar ? aRealVar->getVal() : 0;
  Double_t aErr = aRealVar ? aRealVar->getError() : 0;
  Double_t stdError = ch * aErr / a / a / sqrt(3);
  return std::make_pair(stdValue, stdError);
}

RooArgList* LorentzianPdf::getParameters(Bool_t isTwoDetector) {
  RooArgList *list = new RooArgList();

  std::pair<double, double> Eb = getBindingEnergy();
  // Compose RooRealVar
  const char *name = Form("bindE%s", this->GetName());
  const char *title = Form("Binding E %s", this->GetTitle());
  RooRealVar *EbRealVar = new RooRealVar(name, title, Eb.first, "eV");
  EbRealVar->setError(Eb.second);
  list->add(*EbRealVar);

  return list;
}
