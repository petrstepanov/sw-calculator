/*
 * MyTGTextEntry.cpp
 *
 *  Created on: Aug 2, 2019
 *      Author: petrstepanov
 */

#include "MyTGNumberEntry.h"

ClassImp(MyTGNumberEntry);

MyTGNumberEntry::MyTGNumberEntry(const TGWindow *parent, Double_t val, Int_t digitwidth, Int_t id, EStyle style, EAttribute attr, ELimit limits, Double_t min, Double_t max) : TGNumberEntry(parent, val, digitwidth, id, style, attr, limits, min, max){
}

MyTGNumberEntry::~MyTGNumberEntry() {
}

Bool_t MyTGNumberEntry::HandleFocusChange(Event_t * event){
	if (IsEnabled() &&
	   (event->fCode == kNotifyNormal) &&
	   (event->fState != kNotifyPointer) && (event->fType == kFocusOut)) {
		fNumericEntry->ReturnPressed();
	}
	return TGNumberEntry::HandleFocusChange(event);
}
