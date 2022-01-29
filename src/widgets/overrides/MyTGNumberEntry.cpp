/*
 * MyTGTextEntry.cpp
 *
 *  Created on: Aug 2, 2019
 *      Author: petrstepanov
 */

#include "MyTGNumberEntry.h"
#include <iostream>

ClassImp(MyTGNumberEntry);

MyTGNumberEntry::MyTGNumberEntry(const TGWindow *parent, Double_t val, Int_t digitwidth, Int_t id, EStyle style, EAttribute attr, ELimit limits, Double_t min, Double_t max) : TGNumberEntry(parent, val, digitwidth, id, style, attr, limits, min, max){
	   AddInput(kFocusChangeMask);
}

MyTGNumberEntry::~MyTGNumberEntry() {
}

Bool_t MyTGNumberEntry::HandleFocusChange(Event_t * event){
	if (event->fType == EGEventType::kFocusOut){
		GetNumberEntry()->ReturnPressed();
	}
	return kTRUE;
}
