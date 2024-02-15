/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GaussianPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 16, 2017, 2:10 AM
 */

#include "GaussianPdf.h"
#include "../../model/Constants.h"
#include <RooMath.h>

ClassImp(GaussianPdf);

GaussianPdf::GaussianPdf(Bool_t _isTwoDetector) : isTwoDetector(_isTwoDetector) {
}

GaussianPdf::GaussianPdf(const char *name, const char *title, RooAbsReal &_x, RooAbsReal &_mean, RooAbsReal &_a,
  Bool_t _isTwoDetector) : RooAbsPdf(name, title), x("x", "x", this, _x), mean("mean", "mean", this, _mean), a("a", "a",
                                                                                                               this,
                                                                                                               _a), isTwoDetector(
    _isTwoDetector) {
}

GaussianPdf::GaussianPdf(const GaussianPdf &other, const char *name) : RooAbsPdf(other, name), x("x", this, other.x), mean(
    "mean", this, other.mean), a("a", this, other.a), isTwoDetector(other.isTwoDetector) {
}

Double_t GaussianPdf::evaluate() const {

  // Binding energy shift exists only in 1D case
  Double_t ch = Constants::chbarKeV; // c*ħ (in KeV)

  // One dimensional: "wolfram/gaus-1d.nb" and "_PAS_RU.pdf", pp.29
  // 1D spectroscopy: shift = mean(511) - E_b/2
  if (!isTwoDetector) {
    Double_t B = getBindingEnergy().first / 1000; // should be in keV
    // Non normalized
    return exp(-2 * a * a * TMath::Power(B / 2 - mean + x, 2) / ch / ch);

    // Normalized (not needed here, integral calculated from non-norm function)
    // You can however use normalized function here but RooFit can normalize across different variaibles so...
    // return exp(-2 * a*a*TMath::Power(B / 2 - mean+x, 2)/ch/ch)/ch*TMath::Sqrt(TMath::PiOver2())*a;
  }

  // Two dimensional: "wolfram/gaus-2d.nb" and "_PAS_RU.pdf", pp.29
  // 2D spectroscopy: shift = 0
  return exp(-a * a * x * x / 2 / ch / ch);
}

Int_t GaussianPdf::getAnalyticalIntegral(RooArgSet &allVars, RooArgSet &analVars, const char* /*rangeName*/) const {
  if (matchArgs(allVars, analVars, x))
    return 1;
  return 0;
}

Double_t GaussianPdf::analyticalIntegral(Int_t code, const char *rangeName) const {
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

Double_t GaussianPdf::indefiniteIntegral(Double_t _x) const {
  // Important: argument here should be _x, not x!

  Double_t ch = Constants::chbarKeV;
  // One-dimensional case: "wolfram/gaus-1d.mb"
  if (!isTwoDetector) {
    Double_t B = getBindingEnergy().first / 1000; // need KeV here because x axis is in keV
    // For Non-Normalized function (works ok)
    return ch * TMath::Sqrt(TMath::PiOver2()) * TMath::Erf(a * (B - 2 * mean + 2 * _x) / TMath::Sqrt2() / ch) / 2 / a;
    // For Normalized function
    // return 0.5 * TMath::Erf(a * (B - 2 * mean + 2 * _x) / TMath::Sqrt2() / ch);
  }

  // Two-dimensional case: "wolfram/gaus-2d.mb"
  return ch * TMath::Sqrt(TMath::PiOver2()) * TMath::Erf(a * _x / TMath::Sqrt2() / ch) / a;
}

// Important - this is in eV, not keV!
//             this is same for 1D and 2D cases because wave function is the same
std::pair<double, double> GaussianPdf::getBindingEnergy() const {
  // Value
  Double_t Ry = Constants::Ry; // eV
  Double_t a_B = Constants::a_B; // A
  Double_t eB = 3 * Ry * pow(a_B / a, 2);

  // Error
  RooRealVar *aRealVar = (RooRealVar*) a.absArg();
  Double_t aVal = aRealVar ? aRealVar->getVal() : 0;
  Double_t aErr = aRealVar ? aRealVar->getError() : 0;

  // See error propagation in "gaus-1d.nb"
  Double_t eBErr = 6 * Ry * a_B * a_B * aErr / aVal / aVal / aVal;

  return std::make_pair(eB, eBErr);
}

// Important - this is in eV, not keV!
std::pair<double, double> GaussianPdf::getSTD(Bool_t isTwoDetector) const {
  Double_t ch = Constants::chbar;
  if (!isTwoDetector) {
    // 1D case see "gaus-1d.nb"
    // Value
    Double_t stdValue = ch / 2 / a;

    // Error
    RooRealVar *aRealVar = (RooRealVar*) a.absArg();
    // Double_t aVal = aRealVar ? aRealVar->getVal() : 0;
    Double_t aErr = aRealVar ? aRealVar->getError() : 0;
    // Uncertanty propagation: https://en.wikipedia.org/wiki/Propagation_of_uncertainty
    Double_t stdError = ch * aErr / 2 / a / a;

    return std::make_pair(stdValue, stdError);
  }

  // 2D case see "gaus-1d.nb"
  Double_t stdValue = ch / a;
  RooRealVar *aRealVar = (RooRealVar*) a.absArg();
  // Double_t aVal = aRealVar ? aRealVar->getVal() : 0;
  Double_t aErr = aRealVar ? aRealVar->getError() : 0;
  Double_t stdError = ch * aErr / a / a;
  return std::make_pair(stdValue, stdError);
}

RooArgList* GaussianPdf::getParameters(Bool_t isTwoDetector) {
  RooArgList *list = new RooArgList();

  // Binding Energy value and error can only be calculated in 1D case
  // if (!isTwoDetector) {
  std::pair<double, double> Eb = getBindingEnergy();
  // Compose RooRealVar
  const char *name = Form("bindE%s", this->GetName());
  const char *title = Form("Binding E %s", this->GetTitle());
  RooRealVar *EbRealVar = new RooRealVar(name, title, Eb.first, "eV");
  EbRealVar->setError(Eb.second);
  list->add(*EbRealVar);
  // }

  // Add FWHM
  std::pair<double, double> std = getSTD(isTwoDetector);
  Double_t fwhm = std.first * Constants::sigmaToFwhm;
  Double_t fwhmErr = std.second * Constants::sigmaToFwhm;
  const char *nameFWHM = Form("fwhm%s", this->GetName());
  const char *titleFWHM = Form("FWHM of %s", this->GetTitle());
  RooRealVar *f = new RooRealVar(nameFWHM, titleFWHM, fwhm, "keV");
  f->setError(fwhmErr);

  list->add(*f);
  return list;
}
