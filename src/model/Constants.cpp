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

const Double_t Constants::padMargin[4] = {0.10, 0.03, 0.15, 0.05}; // left, right, bottom, top
const Int_t Constants::colorSet[7]  = {kViolet+6, kSpring-5, kAzure+8, kPink+1, kRed-7, kViolet-4, kGray+1}; 

TColor* Constants::colorPrimary   = new TColor(0.232, 0.757, 0.832);
TColor* Constants::colorWarning   = new TColor(0.832, 0.769, 0.232);
TColor* Constants::colorGray      = new TColor(0.5, 0.5, 0.5);
TColor* Constants::colorAppWindow = new TColor(0.906, 0.906, 0.906);