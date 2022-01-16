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
#include "../widgets/swCalculatorWidget/SWCalculatorView.h"

ClassImp(MyMainFrame)

MyMainFrame::MyMainFrame() : TGMainFrame(gClient->GetRoot(), Constants::windowWidth, Constants::windowHeight) {
	SetIconPixmap(Constants::applicationIcon);
	SetWindowName(Constants::applicationName);
	SetCleanup(kDeepCleanup);

	// Exit this application via the Exit button or Window Manager.
	Connect("CloseWindow()", "MyMainFrame", this, "exit()");
    DontCallClose();

    // Save reverence to MyMainFrame in UiHelper (for centering of modal dialogs)
    // UiHelper::getInstance()->setMainFrame(this);

    SWCalculatorView* swCalculatorView = new SWCalculatorView(this);
    AddFrame(swCalculatorView, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

    // Set minimum and maximum window size for window manager
   	// SetWMSizeHints(Constants::windowWidth, Constants::windowHeight, 1E4, 1E4, 1, 1);
	MapSubwindows();
	Layout();
	// Issue - GetDefaultSize() returns zeros on Ubuntu 18.04.
    // Resize(Constants::windowWidth, Constants::windowHeight);
    // Maps the window "id" and all of its subwindows that have had map requests on the screen
    // and put this window on the top of of the stack of all windows
    MapWindow();

    // You can hide child frames only after the frames have been laid out and the sub windows of the composite frames have been mapped
    uiReady();
}

MyMainFrame::~MyMainFrame(){
    Cleanup();
}

void MyMainFrame::exit(){
//	DeleteWindow();              // to stay in the ROOT session
	gApplication->Terminate();   // to exit and close the ROOT session
}

void MyMainFrame::uiReady(){
	Emit("uiReady()");
}
