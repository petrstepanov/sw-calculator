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
#include <iostream>

UiHelper::UiHelper() {}

UiHelper* UiHelper::instance = NULL;

TGFileInfo* UiHelper::getFileFromDialog(){    
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
    new TGFileDialog(gClient->GetRoot(), mainFrame ? mainFrame : gClient->GetRoot(), kFDOpen, fileInfo);
    // printf("Open file: %s (dir: %s)\n", fileInfo->fFilename, fileInfo->fIniDir);
    return fileInfo;
}

int UiHelper::showOkDialog(const char* message){
    int retval;
    new TGMsgBox(gClient->GetRoot(), mainFrame ? mainFrame : gClient->GetRoot(), Constants::applicationName, message, kMBIconAsterisk, kMBOk, &retval);
    return retval;
}

void UiHelper::setLabelColor(TGLabel* label, const char* color){
    ULong_t lblFileNameTextColor;
    gClient->GetColorByName(color, lblFileNameTextColor);    
    label->SetTextColor(15);
}

void UiHelper::setMainFrame(TGWindow* w) {
    mainFrame = w;
}

UiHelper* UiHelper::getInstance() {
    if (!instance){
        instance = new UiHelper();
    }
    return instance;
}
