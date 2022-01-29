/*
 * RooRealVarFrame.h
 *
 *  Created on: Jul 14, 2019
 *      Author: petrstepanov
 */

#ifndef SRC_FRAMES_ROOREALVARFRAME_H_
#define SRC_FRAMES_ROOREALVARFRAME_H_

#include <TGFrame.h>
#include <RooRealVar.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include "../overrides/MyTGNumberEntry.h"
#include <TGButton.h>

class RooRealVarFrame : public TGHorizontalFrame {
public:
	RooRealVarFrame(const TGWindow *p = 0, RooRealVar* var = 0);
	virtual ~RooRealVarFrame();

	void setRooRealVar(RooRealVar* var);

    // Slots for UI signals
	void onValueSet();
	void onMinValueSet();
	void onMaxValueSet();
//	void onValueSet(char *);
//	void onMinValueSet(char *);
//	void onMaxValueSet(char *);
	void onFixedSet(Bool_t isFixed);

	void updateView();

	virtual Bool_t HandleFocusChange (Event_t* event);

private:
	void initUI();
	void connectSlots();

	RooRealVar* rooRealVar;

    TGLabel* nameLabel;
    TGLabel* titleLabel;
    MyTGNumberEntry* valueNumberEntry;
    MyTGNumberEntry* minValueNumberEntry;
    MyTGNumberEntry* maxValueNumberEntry;
    TGCheckButton* fixedCheckButton;

	ClassDef(RooRealVarFrame,0)
};

#endif /* SRC_FRAMES_ROOREALVARFRAME_H_ */
