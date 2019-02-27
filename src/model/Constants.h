/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Constants.h
 * Author: petrstepanov
 *
 * Created on August 19, 2017, 11:30 PM
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <TROOT.h>
#include <TColor.h>
#include <RooConstVar.h>

class Constants {
  public:
    static constexpr Double_t chbar = 0.19732697 * 1E4; // c*hbar, eV*Å
    static constexpr Double_t mc2 = 0.511E6;            // electron mc^2, eV
    static constexpr Double_t Ry = 13.605;              // Rydberg constant, eV
    static constexpr Double_t a_B = 0.529177;           // Bohr radius, Å
    
    static const UInt_t windowWidth = 1200;
    static const UInt_t windowHeight = 600;
    static const UInt_t leftPanelWidth = 420;
    
    static Double_t bufferFraction;

    static const char* applicationName;
    static char* applicationIcon[];

    static RooConstVar* fwhm2sigma;
    static RooConstVar* pi;
};

#endif /* CONSTANTS_H */

