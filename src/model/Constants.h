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
    static constexpr Double_t chbarKeV = 1.9732697;        // c*hbar, KeV*Å

    static constexpr Double_t mc2 = 511E3;              // electron mc^2, eV
    static constexpr Double_t mc2KeV = 511;             // electron mc^2, KeV
    static constexpr Double_t Ry = 13.605;              // Rydberg constant, eV
    // static constexpr Double_t RyKeV = 0.13605;             // Rydberg constant, KeV
    static constexpr Double_t a_B = 0.529177;           // Bohr radius, Å
    
    static const UInt_t windowWidth = 1200;
    static const UInt_t windowHeight = 680;
    static const UInt_t leftPanelWidth = 430;
    
    static constexpr Double_t histogramTopMargin = 0.1;

    static const Double_t sigmaToFwhm;
    static const Double_t fwhmToSigma;

    static Double_t bufferFraction;

    static const char* applicationName;
    static const char* applicationTitle;
    static char* applicationIcon[];

    static RooConstVar* rooFwhmToSigma;
    static RooConstVar* pi;

    static const char* ATTR_NO_SAVE_TO_POOL;
    static const char* ATTR_NO_DRAW_ON_PLOT;
    static const char* ATTR_HIDE_PARAMETER_FROM_UI;
//    static const char* ATTR_TPAVE_SEPARATOR;

    static const char* ATTR_FORMAT_SIG_DIGITS_0;
    static const char* ATTR_FORMAT_SIG_DIGITS_1;
    static const char* ATTR_FORMAT_SIG_DIGITS_2;
    static const char* ATTR_FORMAT_SIG_DIGITS_3;
    static const char* ATTR_FORMAT_SIG_DIGITS_4;
    static const char* ATTR_FORMAT_SIG_DIGITS_5;


    static const char* LABEL_NO_FILE_LOADED;

};


#endif /* CONSTANTS_H */

