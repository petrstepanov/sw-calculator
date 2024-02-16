/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UiHelper.cpp
 * Author: petrstepanov
 * 
 * Created on October 11, 2017, 1:31 AM
 */

#include "UiHelper.h"
#include <TGFileDialog.h>
#include "../model/Constants.h"
#include <iostream>

UiHelper::UiHelper() {}

UiHelper* UiHelper::instance = NULL;

TGFileInfo* UiHelper::getFileFromDialog(const TGWindow* mainFrame){
    // show file dialog
    TGFileInfo* fileInfo = new TGFileInfo();
    const char* filetypes[] = { "All files",  "*",
                                "Data files", "*.[dD][aA][tT]",
                                "CSV files",  "*.[cC][sS][vV]",
                                "Text files", "*.[tT][xX][tT]",
                                0, 0 };
    static TString dir(".");
    fileInfo->fFileTypes = filetypes;
    fileInfo->fIniDir = StrDup(dir);
    // Show the dialog
    new TGFileDialog(gClient->GetDefaultRoot(), mainFrame ? mainFrame : gClient->GetRoot(), kFDOpen, fileInfo);
    // printf("Open file: %s (dir: %s)\n", fileInfo->fFilename, fileInfo->fIniDir);
    return fileInfo;
}

int UiHelper::showOkDialog(const TGWindow* mainFrame, const char* message, EMsgBoxIcon icon){
    int retval;
    new TGMsgBox(gClient->GetRoot(), mainFrame ? mainFrame : gClient->GetRoot(), Constants::applicationTitle, message, icon, kMBOk, &retval);
    return retval;
}

void UiHelper::setLabelColor(TGLabel* label, const char* color){
    ULong_t lblFileNameTextColor;
    gClient->GetColorByName(color, lblFileNameTextColor);    
    label->SetTextColor(15);
}

// PS: main frame pointer is always stored in every widget class
//
//void UiHelper::setMainFrame(TGWindow* w) {
//    mainFrame = w;
//}
//
//TGWindow* UiHelper::getMainFrame() {
//    return mainFrame;
//}


UiHelper* UiHelper::getInstance() {
    if (!instance){
        instance = new UiHelper();
    }
    return instance;
}
//
//ModalDialogFrame* UiHelper::getDialog(TGMainFrame* mainFrame, const char* windowName){
//	ModalDialogFrame* dialog = new ModalDialogFrame(gClient->GetDefaultRoot(), mainFrame, windowName);
//	return dialog;
//}

Int_t UiHelper::getUId() {
    static Int_t uid = 0;
    return uid++;
}


TGCompositeFrame* UiHelper::getParentFrame(TGFrame* frame){
    const TGWindow* parentWindow = frame->GetParent();
    TGCompositeFrame* parentFrame = dynamic_cast<TGCompositeFrame*>(const_cast<TGWindow*>(parentWindow));
    return parentFrame;
}

void UiHelper::showFrame(TGFrame* frame){
    TGCompositeFrame* parentFrame = getParentFrame(frame);
    parentFrame->ShowFrame(frame);
}

void UiHelper::hideFrame(TGFrame* frame){
    TGCompositeFrame* parentFrame = getParentFrame(frame);
    parentFrame->HideFrame(frame);
}

