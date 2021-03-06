/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Constants.cpp
 * Author: petrstepanov
 * 
 * Created on August 19, 2017, 11:30 PM
 */

// Disable warnings for -Wwrite-strings
// https://stackoverflow.com/questions/925179/selectively-remove-warning-message-gcc
#pragma GCC diagnostic ignored "-Wwrite-strings"
#include "Constants.h"
#include <TMath.h>

// Application name
const char* Constants::applicationName = "SW Calculator";

// Convolution buffer fraction
Double_t Constants::bufferFraction = 2;

// Convert sigma to FWHM: https://en.wikipedia.org/wiki/Full_width_at_half_maximum
const Double_t Constants::sigmaToFwhm = 2 * TMath::Sqrt(2 * TMath::Log(2));
const Double_t Constants::fwhmToSigma = 1 / sigmaToFwhm;

RooConstVar* Constants::rooFwhmToSigma = new RooConstVar("rooFwhmToSigma", "Coefficient to convert FWHM to dispersion",
		Constants::fwhmToSigma);
RooConstVar* Constants::pi = new RooConstVar("pi", "pi", TMath::Pi());

char* Constants::applicationIcon[] = { "48 48 17 1", " 	c None", ".	c #93E8FE", "+	c #10BCE8", "@	c #E9FAFE",
		"#	c #F8FDFF", "$	c #D1F4FD", "%	c #26C2EA", "&	c #3DD8FF", "*	c #54DDFF", "=	c #A8EAFB", "-	c #83DCF3",
		";	c #4ACCEE", ">	c #B7EFFC", ",	c #34D6FF", "'	c #00B7E6", ")	c #FFFFFF", "!	c #000000",
		"        ))))))))))))))))))))))))))))))))        ", "      ))))))))))))))))))))))))))))))))))))      ",
		"    ))))))))))))))))))))))))))))))))))))))))    ", "   ))))))))))))))))))))))))))))))))))))))))))   ",
		"  ))))))))))))))))))))))))))))))))))))))))))))  ", "  ))))))))))))))))))))))))))))))))))))))))))))  ",
		" )))))))))))))))))))))))))))))))))))))))))))))) ", " )))))))))))))))))))))))))))))))))))))))))))))) ",
		"))))))))))))))))))))))))))))))))))))))))))))))))", "))))))))))))))))))))))))))))))))))))))))))))))))",
		"))))))))))))))))))))#-%'+;$)))))))))))))))))))))", ")))))))))))))))))))$%'''''+>))))))))))))))))))))",
		"))))))))))))))))))$+'''''''+$)))))))))))))))))))", ")))))))))))))))))@%'''''''''%)))))))))))))))))))",
		"))))))))))))))))#;'''''''''''-))))))))))))))))))", "))))))))))))))))-''''''''''''+@)))))))))))))))))",
		")))))))))))))))>''''''''''''''-)))))))))))))))))", "))))))))))))))$+''''''''''''''+@))))))))))))))))",
		")))))))))))))@%''''''''''''''''-))))))))))))))))", "))))))))))))@*'''''''''''''''''+#)))))))))))))))",
		")))))))))))$&,''''''''''''''''''.)))))))))))))))", ")))))))))#.,,,''''''''''''''''''%#))))))))))))))",
		")))))))@.&,,,,'''''''''''''''''''=))))))))))))))", ")#@$>.*,,,,,,,'''''''''''''''''''%#)))))))))))))",
		",,,,,,,,,,,,,,''''''''''''''''''''>)))))))))))))", ",,,,,,,,,,,,,,''''''''''''''''''''*#))))))))))))",
		",,,,,,,,,,,,,,'''''''''''''''''''',.))))))))))))", ",,,,,,,,,,,,,,'''''''''''''''''''',&@)))))))))))",
		",,,,,,,,,,,,,,'''''''''''''''''''',,*)))))))))))", ",,,,,,,,,,,,,,'''''''''''''''''''',,,.))))))))))",
		",,,,,,,,,,,,,,'''''''''''''''''''',,,,=)))))))))", ",,,,,,,,,,,,,,'''''''''''''''''''',,,,,>))))))))",
		",,,,,,,,,,,,,,'''''''''''''''''''',,,,,,.#))))))", ",,,,,,,,,,,,,,'''''''''''''''''''',,,,,,,*$)))))",
		",,,,,,,,,,,,,,'''''''''''''''''''',,,,,,,,,*.$@#", ",,,,,,,,,,,,,,'''''''''''''''''''',,,,,,,,,,,,,,",
		",,,,,,,,,,,,,,'''''''''''''''''''',,,,,,,,,,,,,,", ",,,,,,,,,,,,,,'''''''''''''''''''',,,,,,,,,,,,,,",
		",,,,,,,,,,,,,,'''''''''''''''''''',,,,,,,,,,,,,,", ",,,,,,,,,,,,,,'''''''''''''''''''',,,,,,,,,,,,,,",
		" ,,,,,,,,,,,,,'''''''''''''''''''',,,,,,,,,,,,, ", " ,,,,,,,,,,,,,'''''''''''''''''''',,,,,,,,,,,,, ",
		"  ,,,,,,,,,,,,'''''''''''''''''''',,,,,,,,,,,,  ", "  ,,,,,,,,,,,,'''''''''''''''''''',,,,,,,,,,,,  ",
		"   ,,,,,,,,,,,'''''''''''''''''''',,,,,,,,,,,   ", "    ,,,,,,,,,,'''''''''''''''''''',,,,,,,,,,    ",
		"      ,,,,,,,,'''''''''''''''''''',,,,,,,,      ", "        ,,,,,,'''''''''''''''''''',,,,,,        " };

const char* Constants::ATTR_NO_SAVE_TO_POOL = "noSaveInPool";
const char* Constants::ATTR_NO_DRAW_ON_PLOT = "noDrawOnPlot";

const char* Constants::ATTR_HIDE_PARAMETER_FROM_UI = "hideParameterFromUi";

//const char* Constants::ATTR_TPAVE_SEPARATOR = "attrTPaveSeparator";


const char* Constants::ATTR_FORMAT_SIG_DIGITS_0 = "attrFormatSigDigits0";
const char* Constants::ATTR_FORMAT_SIG_DIGITS_1 = "attrFormatSigDigits1";
const char* Constants::ATTR_FORMAT_SIG_DIGITS_2 = "attrFormatSigDigits2";
const char* Constants::ATTR_FORMAT_SIG_DIGITS_3 = "attrFormatSigDigits3";
const char* Constants::ATTR_FORMAT_SIG_DIGITS_4 = "attrFormatSigDigits4";
const char* Constants::ATTR_FORMAT_SIG_DIGITS_5 = "attrFormatSigDigits5";

const char* Constants::LABEL_NO_FILE_LOADED = "no file loaded";
