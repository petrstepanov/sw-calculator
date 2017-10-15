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

class RootHelper {
public:
    RootHelper();
    RootHelper(const RootHelper& orig);
    virtual ~RootHelper();
    
    static void deleteObject(const char* name);
    static void deleteObject(TObject* obj);
    static Int_t getNumCpu();
private:

};

#endif /* ROOTHELPER_H */

