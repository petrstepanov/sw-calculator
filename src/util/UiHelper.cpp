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
#include "../model/Constants.h"

UiHelper::UiHelper() {
}

UiHelper::UiHelper(const UiHelper& orig) {
}

UiHelper::~UiHelper() {
}

TGFileInfo* UiHelper::getFileFromDialog(){
    // show file dialog
    TGFileInfo* fileInfo = new TGFileInfo();
    const char* filetypes[] = { "All files",  "*",
                                "Data files", "*.[dD][aA][tT]",
                                "CSV files",  "*.[cC][sS][vV]",
                                "Text files", "*.[tT][xX][tT]",
                                0, 0 };
    char fIniDir[] = ".";
    fileInfo->fFileTypes = filetypes;
    fileInfo->fIniDir = fIniDir;
    // TODO: check Dialog Centering
    new TGFileDialog(gClient->GetRoot(), gClient->GetRoot(), kFDOpen, fileInfo);
    return fileInfo;
}

void showOkDialog(const char* message){
    new TGMsgBox(gClient->GetRoot(), gClient->GetRoot(), Constants::applicationName, message, kMBIconAsterisk, kMBOk);
}