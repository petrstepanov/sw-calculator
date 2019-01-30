/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   MyMainFrame.cpp
 * Author: petrstepanov
 *
 * Created on October 15, 2017, 1:47 AM
 */

#include "MyMainFrame.h"
#include <TApplication.h>
#include "../model/Constants.h"
#include "../util/UiHelper.h"
#include <iostream>

MyMainFrame::MyMainFrame(const TGWindow* w) : TGMainFrame(w, Constants::windowWidth, Constants::windowHeight) {
    SetCleanup(kDeepCleanup);
    SetWindowName(Constants::applicationName);

    Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    DontCallClose();

    // Pass TGMyMainFrame to UiHelper for centering the dialogs
    UiHelper* uiHelper = UiHelper::getInstance();
    uiHelper->setMainFrame(this);
}

MyMainFrame::~MyMainFrame(){
    Cleanup();
}

void MyMainFrame::mapAndResize(){
    MapSubwindows();
    Resize(GetDefaultSize());
    Resize(Constants::windowWidth, Constants::windowHeight);
    MapWindow();
}

//void MyMainFrame::CloseWindow(){
//    delete this;
//    gApplication->Terminate(0);
//}
