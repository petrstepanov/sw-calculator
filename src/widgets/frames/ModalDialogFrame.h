/*
 * ModalDialogFrame.h
 *
 *  Created on: Jul 27, 2019
 *      Author: petrstepanov
 */

#ifndef SRC_WIDGETS_FRAMES_MODALDIALOGFRAME_H_
#define SRC_WIDGETS_FRAMES_MODALDIALOGFRAME_H_

#include <TGFrame.h>

class ModalDialogFrame: public TGTransientFrame {
public:
	ModalDialogFrame(const TGWindow *p, const TGWindow *main, const char* windowName = "");
	virtual ~ModalDialogFrame();

	virtual void closeWindow();
	void show(Int_t minWidth = 0, Int_t minHeight = 0);

	void uiReady(); // **SIGNAL**

	ClassDef(ModalDialogFrame, 0)
};

#endif /* SRC_WIDGETS_FRAMES_MODALDIALOGFRAME_H_ */
