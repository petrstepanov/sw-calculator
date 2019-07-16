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
#include <TGButton.h>

class RooRealVarFrame: public TGHorizontalFrame {
public:
	RooRealVarFrame();
	RooRealVarFrame(RooRealVar* var);
	virtual ~RooRealVarFrame();

	void setRooRealVar(RooRealVar* var);

private:
	void initUI();
	void connectSlots();
	void updateView();

	RooRealVar* rooRealVar;

    TGLabel* nameLabel;
    TGLabel* descriptionLabel;
    TGNumberEntry* valueNumberEntry;
    TGNumberEntry* minValueNumberEntry;
    TGNumberEntry* maxValueNumberEntry;
    TGCheckButton* fixedCheckButton;

    // Slots for UI signals
    void onValueSet(Long_t value);
    void onMinValueSet(Long_t value);
    void onMaxValueSet(Long_t value);
    void onFixedSet(Bool_t isFixed);
};

#endif /* SRC_FRAMES_ROOREALVARFRAME_H_ */
