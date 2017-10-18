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
#include <TGMsgBox.h>
#include <TGFileDialog.h>
#include "../model/Constants.h"

UiHelper::UiHelper() {
}

UiHelper::UiHelper(const UiHelper& orig) {
}

UiHelper::~UiHelper() {
}

TGFileInfo* UiHelper::getFileFromDialog(const TGWindow* main){    
    // show file dialog
    TGFileInfo* fileInfo = new TGFileInfo();
//    TGFileInfo* fileInfo = new TGFileInfo();
    const char* filetypes[] = { "All files",  "*",
                                "Data files", "*.[dD][aA][tT]",
                                "CSV files",  "*.[cC][sS][vV]",
                                "Text files", "*.[tT][xX][tT]",
                                0, 0 };
    static TString dir(".");
    fileInfo->fFileTypes = filetypes;
    fileInfo->fIniDir = StrDup(dir);
    // TODO: check Dialog Centering
    new TGFileDialog(gClient->GetRoot(), main, kFDOpen, fileInfo);
    printf("Open file: %s (dir: %s)\n", fileInfo->fFilename, fileInfo->fIniDir);
    return fileInfo;
}

void UiHelper::showOkDialog(const char* message){
    new TGMsgBox(gClient->GetRoot(), gClient->GetRoot(), Constants::applicationName, message, kMBIconAsterisk, kMBOk);
}

void UiHelper::setLabelColor(TGLabel* label, const char* color){
    ULong_t lblFileNameTextColor;
    gClient->GetColorByName(color, lblFileNameTextColor);    
    label->SetTextColor(15);
}