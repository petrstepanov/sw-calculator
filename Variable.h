/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Variable.h
 * Author: petrstepanov
 *
 * Created on August 19, 2017, 11:13 PM
 */

#ifndef VARIABLE_H
#define VARIABLE_H

#include <TROOT.h>

class Variable {
public:
    Variable();
    Variable(const Variable& orig);
    Variable(Double_t value, Double_t error=0, const char* description = "", const char* unit="");
    virtual ~Variable();
    
    Double_t getValue(void);
    Double_t getError(void);
    const char* getDescription(void);
    const char* getUnit(void);
    void print();
    
private:
    Double_t value;
    Double_t error;
    TString description;
    TString unit;
};

#endif /* VARIABLE_H */

