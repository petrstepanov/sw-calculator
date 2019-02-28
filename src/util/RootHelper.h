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

class RootHelper {
public:
    static void deleteObject(const char* name);
    static void deleteObject(TObject* obj);
    static Int_t getNumCpu();
    static void startTimer(void);
    static void stopAndPrintTimer();
    static TGCompositeFrame* getParentFrame(TGFrame* frame);
    static void showFrame(TGFrame* frame);
    static void hideFrame(TGFrame* frame);

private:
    static TStopwatch* watch;
};

#endif /* ROOTHELPER_H */

