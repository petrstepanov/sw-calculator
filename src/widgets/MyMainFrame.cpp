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

	// Exit this application via the Exit button or Window Manager.
	Connect("CloseWindow()", "MyMainFrame", this, "doExit()");
    DontCallClose();

    // Save reverence to MyMainFrame in UiHelper (for centering of modal dialogs)
    UiHelper* uiHelper = UiHelper::getInstance();
    uiHelper->setMainFrame(this);
}

MyMainFrame::~MyMainFrame(){
    Cleanup();
}

void MyMainFrame::mapAndResize(){
    SetWindowName(Constants::applicationName);
    MapSubwindows();
    Layout();
    Resize(GetDefaultSize());
//    Resize(Constants::windowWidth, Constants::windowHeight);
    MapWindow();
}

void MyMainFrame::doExit(){
//	DeleteWindow();              // to stay in the ROOT session
	gApplication->Terminate();   // to exit and close the ROOT session
}
