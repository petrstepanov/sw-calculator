/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StringUtils.cpp
 * Author: petrstepanov
 * 
 * Created on August 2, 2017, 5:44 AM
 */

#include "StringUtils.h"

TString* StringUtils::stripFileName(TString* path){
	std::string p(path->Data());
	std::size_t found = p.find_last_of("/\\");
	return new TString(p.substr(found + 1).c_str());
}

TString* StringUtils::stripFileExtension(TString* path){
	std::string p(path->Data());
	std::size_t found = p.find_last_of(".");
	return new TString(p.substr(0, found).c_str());
}

TString* StringUtils::suffix(const char* name, int index, const char* nameSuffix) {
    TString* s = new TString(name);
    *s += nameSuffix;
    if (index > 1) *s += index;
    return s;
}

char* StringUtils::ordinal(int index, const char* name) {
	if (index == 1){
		TString* s = new TString(name);
		return s;
    }
    if (index == 2){
    	TString* s = new TString(Form("2nd %s", name));
    	return s;
    }
    if (index == 3){
    	TString* s = new TString(Form("3rd %s", name));
    	return s;
    }
    TString* s = new TString(Form("%dth %s", index, name));
	return s;
}

Bool_t StringUtils::isSubstring(const char* parent, const char* child) {
	TString parentString(parent);
	TString childString(child);
	return parentString.Contains(childString);
}
