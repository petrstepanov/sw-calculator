/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimpleParabolaPdf.cpp
 * Author: petrstepanov
 * 
 * Created on August 2, 2017, 5:34 AM
 */

#include "ParabolaPdf.h"
#include "../../model/Constants.h"
#include <iostream>

ClassImp(ParabolaPdf);

ParabolaPdf::ParabolaPdf(Bool_t _isTwoDetector) : isTwoDetector(_isTwoDetector) {
}

ParabolaPdf::ParabolaPdf(const char *name, const char *title, RooAbsReal &_x, RooAbsReal &_mean, RooAbsReal &_Ef,
  Bool_t _isTwoDetector) : RooAbsPdf(name, title), x("x", "x", this, _x), mean("mean", "Parabola mean", this, _mean), Ef(
    "Ef", "Fermi energy", this, _Ef), isTwoDetector(_isTwoDetector) {
}

ParabolaPdf::ParabolaPdf(const ParabolaPdf &other, const char *name) : RooAbsPdf(other, name), x("x", this, other.x), mean(
    "mean", this, other.mean), Ef("r", this, other.Ef), isTwoDetector(other.isTwoDetector) {
}

Double_t ParabolaPdf::evaluate() const {
  // 1D case - parabola center is sitting at (511 keV - Eb)
  Double_t mc2 = Constants::mc2KeV;
  Double_t EfkeV = Ef / 1000;
  if (!isTwoDetector) {
    Double_t B = getBindingEnergy().first / 1000;

    // "par-1d.nb"
    Double_t parFunction = 1 - 2 * pow(B / 2 + x - mean, 2) / mc2 / EfkeV;
    return parFunction > 0 ? parFunction : 0;
  }

  // "par-2d.nb"
  Double_t parFunction = 1 - x * x / 2 / EfkeV / mc2;
  return parFunction > 0 ? parFunction : 0;
}

std::pair<double, double> ParabolaPdf::getBindingEnergy() const {
  // For now we return 0, ask Dad how to calculate binding energy for parabola

  // Value
//  Double_t Ry = Constants::Ry; // eV
//  Double_t a_B = Constants::a_B; // A
//  Double_t eB = 3 * Ry * pow(a_B / a, 2);
//
//  // Error
//  RooRealVar *aRealVar = (RooRealVar*)a.absArg();
//  Double_t aVal = aRealVar ? aRealVar->getVal() : 0;
//  Double_t aErr = aRealVar ? aRealVar->getError() : 0;
//
//  // See error propagation in "gaus-1d.nb"
//  Double_t eBErr = 6*Ry*a_B*a_B*aErr/aVal/aVal/aVal;

// return std::make_pair(eB, eBErr);
  return std::make_pair(0, 0);
}

Int_t ParabolaPdf::getAnalyticalIntegral(RooArgSet &allVars, RooArgSet &analVars, const char* /*rangeName*/) const {
  if (matchArgs(allVars, analVars, x))
    return 1;
  return 0;
}

Double_t ParabolaPdf::analyticalIntegral(Int_t code, const char *rangeName) const {
  switch (code) {
  case 1: {
    Double_t x1 = x.min(rangeName);
    Double_t x2 = x.max(rangeName);

    Double_t mc2 = Constants::mc2KeV;
    Double_t EfkeV = Ef / 1000;

    // Define roots
    std::pair<double, double> roots;
    if (!isTwoDetector) {
      Double_t root = sqrt(EfkeV * mc2 / 2);
      Double_t B = getBindingEnergy().first / 1000;
      roots = std::make_pair(mean - B / 2 - root, mean - B / 2 + root);
    } else {
      Double_t root = sqrt(2 * EfkeV * mc2);
      roots = std::make_pair(-root, root);
    }

    // Border conditions
    if (x2 < roots.first)
      return 0;
    if (x1 > roots.second) {
      return !isTwoDetector ? 2 / 3 * sqrt(2 * EfkeV * mc2) : 4 / 3 * sqrt(2 * EfkeV * mc2);
    }

    Double_t x1new = TMath::Max(roots.first, x1);
    Double_t x2new = TMath::Min(roots.second, x2);

    return indefiniteIntegral(x2new) - indefiniteIntegral(x1new);
  }
  }
  assert(0);
  return 0;
}

// Important: this is valid and can be used only in between of the parabola roots
Double_t ParabolaPdf::indefiniteIntegral(Double_t _x) const {
  // Important: argument here and below should be _x, not x!

  // Dwight 856.21 or Mathematica
  Double_t mc2 = Constants::mc2KeV;
  Double_t EfkeV = Ef / 1000;

  if (!isTwoDetector) {
    // One-dimensional case: "wolfram/par-1d.mb"
    Double_t B = getBindingEnergy().first / 1000;
    return _x - pow(B + 2 * _x - 2 * mean, 3) / 12 / EfkeV / mc2;
  }

  // Two-dimensional case: "wolfram/par-1d.mb"
  return _x - pow(_x, 3) / 6 / EfkeV / mc2;
}
//
//RooArgList* ParabolaPdf::getParameters(Bool_t isTwoDetector) {
//  RooArgList *list = new RooArgList();
//  return list;
//}
