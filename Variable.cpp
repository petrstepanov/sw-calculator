/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Variable.cpp
 * Author: petrstepanov
 * 
 * Created on August 19, 2017, 11:13 PM
 */

#include "Variable.h"
#include "AbstractModelProvider.h"
#include <iostream>

Variable::Variable() {
}

Variable::Variable(const Variable& orig) {
}

Variable::Variable(Double_t value, Double_t error, const char* description, const char* unit){
    this->value = value;
    this->error = error;
    this->description = description;
    this->unit = unit;
}
    
Variable::~Variable() {
}

Double_t Variable::getValue(void){
    return value;
}

Double_t Variable::getError(void){
    return error;
}
const char* Variable::getDescription(void){
    return description;
}

const char* Variable::getUnit(void){
    return unit;
}

void Variable::print(){
    std::cout << description << ": " << value << " +/- " << error << ", " << unit << std::endl;
}