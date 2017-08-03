/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StringUtils.h
 * Author: petrstepanov
 *
 * Created on August 2, 2017, 5:44 AM
 */

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <TString.h>

class StringUtils {
public:
	static TString* stripFileName(TString*);
	static TString* stripFileExtension(TString*);
};

#endif /* STRINGUTILS_H */

