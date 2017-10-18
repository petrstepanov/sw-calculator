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

#include "Constants.h"

const char* Constants::applicationName = "SW Calculator";

const Double_t Constants::padMargin[4] = {0.12, 0.03, 0.15, 0.05};

TColor* Constants::colorPrimary = new TColor(0.232, 0.757, 0.832);
TColor* Constants::colorWarning = new TColor(0.832, 0.769, 0.232);
TColor* Constants::colorGray    = new TColor(0.5, 0.5, 0.5);