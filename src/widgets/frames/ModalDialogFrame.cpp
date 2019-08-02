/*
 * ModalDialogFrame.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: petrstepanov
 */

#include "ModalDialogFrame.h"
#include "../AbstractView.h"

ClassImp(ModalDialogFrame);

ModalDialogFrame::ModalDialogFrame(const TGWindow *p, const TGWindow *main, const char* windowName) : TGTransientFrame(p, main) {
	SetWindowName(windowName);

	// Automatic deep cleanup in this composite frame and all child composite frames (hierarchical)
	SetCleanup(kDeepCleanup);
	Connect("CloseWindow()", "ModalDialogFrame", this, "closeWindow()");
	DontCallClose();
}

ModalDialogFrame::~ModalDialogFrame() {
}

void ModalDialogFrame::show(){
	// Map all sub windows that are part of the composite frame
	MapSubwindows();

	// Resize depending on child elements size
    Layout();
    Resize(GetDefaultSize());

	// Position relative to the parent's window
	CenterOnParent();
	MapWindow();

	// Disable main window
	gClient->WaitFor(this);
}

void ModalDialogFrame::closeWindow(){
//	delete this;
		DeleteWindow();
}
