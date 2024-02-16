/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RootHelper.h
 * Author: petrstepanov
 *
 * Created on August 24, 2017, 8:16 PM
 */

#ifndef ROOTHELPER_H
#define ROOTHELPER_H

#include <TROOT.h>
#include <TStopwatch.h>
#include <TGFrame.h>
#include <TGNumberEntry.h>
#include <TRandom.h>
#include <RooRealVar.h>
#include <RooAbsPdf.h>

class RootHelper {
public:
    static void deleteObject(const char* name);
    static void deleteObject(TObject* obj);
    // static Int_t getNumCpu();
    static void startTimer(void);
    static void stopAndPrintTimer();
    static TGNumberFormat::EStyle getNumberFormatStyle(Double_t value);
    static void setRooRealVarValueLimits(RooRealVar* var, Double_t value, Double_t min, Double_t max);
    static RooRealVar* getParameterNameContains(RooAbsPdf* pdf, const char* nameSubstring);
    static RooAbsArg* findArgNameSubstring(RooAbsCollection* list, const char* nameSubstring);
    static std::pair<TMatrixD,TList*> rooPlotToMatrix(RooRealVar* axis, RooPlot* plot);
    static RooArgList* getLinearIntensities(RooArgList* recursiveIntensities);
    static Int_t getSigDigits(RooAbsReal* rooAbsReal);
    static void setSigDigits(RooAbsReal* rooAbsReal, Int_t sigDigits);
    static Int_t getRandomInt();
    static TString* getUUID();

private:
    static TStopwatch* watch;
    static TRandom* random;
};

#endif /* ROOTHELPER_H */

