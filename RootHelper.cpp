/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RootHelper.cpp
 * Author: petrstepanov
 * 
 * Created on August 24, 2017, 8:16 PM
 */

#include "RootHelper.h"

RootHelper::RootHelper() {
}

RootHelper::RootHelper(const RootHelper& orig) {
}

RootHelper::~RootHelper() {
}

void RootHelper::deleteObject(const char* name){
    TObject* obj = gROOT->FindObject(name);
    if (obj) {
        obj->Delete();
    }
}

