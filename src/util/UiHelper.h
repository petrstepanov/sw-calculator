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
#include <TGFileDialog.h>
#include <TGLabel.h>
#include <TGWindow.h>
#include "../widgets/frames/ModalDialogFrame.h"

class UiHelper {
public:   
    static UiHelper* getInstance();
    static void setLabelColor(TGLabel* label, const char* color);
    static TGCompositeFrame* getParentFrame(TGFrame* frame);
    static Int_t getUId();
    int showOkDialog(const char* message);
    TGFileInfo* getFileFromDialog();
    void setMainFrame(TGWindow* window);
    TGWindow* getMainFrame();
    ModalDialogFrame* getDialog(const char* windowName = "");
        
private:
    UiHelper();
    static Int_t uid;
    static UiHelper* instance;
    TGWindow* mainFrame = nullptr;
};

#endif /* UIHELPER_H */

