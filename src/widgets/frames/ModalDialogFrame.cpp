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

	SetLayoutManager(new TGVerticalLayout(this));

	// Automatic deep cleanup in this composite frame and all child composite frames (hierarchical)
	SetCleanup(kDeepCleanup);
	Connect("CloseWindow()", "ModalDialogFrame", this, "closeWindow()");
	DontCallClose();
}

ModalDialogFrame::~ModalDialogFrame() {
}

void ModalDialogFrame::show(Int_t minWidth, Int_t minHeight){

	// Map all sub windows that are part of the composite frame
	MapSubwindows();

	// Resize to either default size or provided width and height
	// Tip: Resize() calls Layout() itself!
	Int_t width = minWidth ? minWidth : GetDefaultSize().fWidth;
	Int_t height = minHeight ? minHeight : GetDefaultSize().fHeight;
	Resize(width, height);

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
