/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UiHelper.h
 * Author: petrstepanov
 *
 * Created on October 11, 2017, 1:31 AM
 */

#ifndef UIHELPER_H
#define UIHELPER_H

#include <TGFrame.h>
#include <TGMsgBox.h>
#include <TGFileDialog.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGWindow.h>
#include "../widgets/frames/ModalDialogFrame.h"

enum EPadding {
    dx = 10,
    dy = 5
};

class UiHelper {
public:   
    static UiHelper* getInstance();
    static void setLabelColor(TGLabel* label, const char* color);
    static Int_t getUId();
    static int showOkDialog(const TGWindow* mainFrame = NULL, const char* message = "Hello World!", EMsgBoxIcon icon = EMsgBoxIcon::kMBIconAsterisk);
    TGFileInfo* getFileFromDialog(const TGWindow* mainFrame = NULL);
    static TGCompositeFrame* getParentFrame(TGFrame* frame);
    static void showFrame(TGFrame* frame);
    static void hideFrame(TGFrame* frame);
        
private:
    UiHelper();
    static Int_t uid;
    static UiHelper* instance;
//    TGWindow* mainFrame = nullptr;
};

#endif /* UIHELPER_H */

