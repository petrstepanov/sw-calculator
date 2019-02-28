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

#include "TImage.h"
#include "MyMainFrame.h"
#include <TApplication.h>
#include "../model/Constants.h"
#include "../util/UiHelper.h"
#include <iostream>

MyMainFrame::MyMainFrame() : TGMainFrame(gClient->GetRoot(), Constants::windowWidth, Constants::windowHeight) {
	SetIconPixmap(Constants::applicationIcon);
	SetWindowName(Constants::applicationName);
	SetCleanup(kDeepCleanup);

	// Exit this application via the Exit button or Window Manager.
	Connect("CloseWindow()", "MyMainFrame", this, "doExit()");
    DontCallClose();

    // Save reverence to MyMainFrame in UiHelper (for centering of modal dialogs)
    UiHelper::getInstance()->setMainFrame(this);
}

MyMainFrame::~MyMainFrame(){
    Cleanup();
}

void MyMainFrame::addChildFrame(TGFrame* child){
    AddFrame(child, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    mapAndResize();
}

void MyMainFrame::mapAndResize(){
	MapSubwindows();
    Layout();
    Resize(GetDefaultSize());
    MapWindow();
}

void MyMainFrame::doExit(){
//	DeleteWindow();              // to stay in the ROOT session
	gApplication->Terminate();   // to exit and close the ROOT session
}
