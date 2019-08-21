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
    static TString* suffix(const char* name, int index, const char* nameSuffix = "");
    static TString* ordinal(int index, const char*);
    static Bool_t isSubstring(const char* parent, const char* child);
};

#endif /* STRINGUTILS_H */

