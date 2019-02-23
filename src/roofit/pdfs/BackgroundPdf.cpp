/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BackgroundPdf.cpp
 * Author: petrstepanov
 * 
 * Created on October 19, 2017, 2:02 AM
 */

#include "BackgroundPdf.h"
#include "../../model/Constants.h"
#include <RooMath.h>

ClassImp(BackgroundPdf);

BackgroundPdf::BackgroundPdf(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _threeGContrib) :
		RooAbsPdf(name, title), x("x", "x", this, _x), threeGContrib("threeGContrib", "Three gamma Fraction", this, _threeGContrib) {
}

BackgroundPdf::BackgroundPdf(const BackgroundPdf& other, const char* name) :
		RooAbsPdf(other, name), x("x", this, other.x), threeGContrib("threeGContrib", this, other.threeGContrib) {
}

Double_t BackgroundPdf::evaluate() const {
	Double_t ridgeProfile = 1;
	if (x <= 511) {
		Double_t e = x;
		Double_t mc2 = Constants::mc2 / 1000; // keV
		Double_t orePowell = e * (mc2 - e) / pow(2 * mc2 - e, 2) + (2 * mc2 - e) / e + (2 * mc2 * (mc2 - e) / (e * e) - 2 * mc2 * pow(mc2 - e, 2) / pow(2 * mc2 - e, 3)) * log(1 - e / mc2);
		ridgeProfile += threeGContrib * orePowell;
	}
	return ridgeProfile;
}
