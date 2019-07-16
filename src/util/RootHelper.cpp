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
#include <TUnixSystem.h>
#include <iostream>
#include <TGFrame.h>
#include "MathUtil.h"

void RootHelper::deleteObject(const char* name){
    TObject* obj = gROOT->FindObject(name);
    deleteObject(obj);
}

void RootHelper::deleteObject(TObject* obj){
    if (obj){
        obj->Delete();
//        delete obj;
    }
}

Int_t RootHelper::getNumCpu(){
    SysInfo_t sysInfo;
    gSystem->GetSysInfo(&sysInfo);
    std::cout << "RootHelper::getNumCpu: sysInfo.fCpus = " << sysInfo.fCpus << std::endl;
    return (sysInfo.fCpus >= 0) ? sysInfo.fCpus : 1;
}

TStopwatch* RootHelper::watch = new TStopwatch();

void RootHelper::startTimer(){
    watch->Start();
}

void RootHelper::stopAndPrintTimer(){
    watch->Stop();
    watch->Print();
}

TGCompositeFrame* RootHelper::getParentFrame(TGFrame* frame){
	const TGWindow* parentWindow = frame->GetParent();
	TGCompositeFrame* parentFrame = dynamic_cast<TGCompositeFrame*>(const_cast<TGWindow*>(parentWindow));
	return parentFrame;
}

void RootHelper::showFrame(TGFrame* frame){
	TGCompositeFrame* parentFrame = RootHelper::getParentFrame(frame);
	parentFrame->ShowFrame(frame);
}

void RootHelper::hideFrame(TGFrame* frame){
	TGCompositeFrame* parentFrame = RootHelper::getParentFrame(frame);
	parentFrame->HideFrame(frame);
}

TGNumberFormat::EStyle RootHelper::getNumberFormatStyle(Double_t value){
	Int_t numberOfDecimals = MathUtil::numberOfDecimals(value);
	switch (numberOfDecimals){
		case 0:
			return TGNumberFormat::EStyle::kNESInteger;
		case 1:
			return TGNumberFormat::EStyle::kNESRealOne;
		case 2:
			return TGNumberFormat::EStyle::kNESRealTwo;
		case 3:
			return TGNumberFormat::EStyle::kNESRealThree;
		case 4:
			return TGNumberFormat::EStyle::kNESRealFour;
		default:
			return TGNumberFormat::EStyle::kNESReal;
	}
}
