/*
 * RooRealVarFrame.cpp
 *
 *  Created on: Jul 14, 2019
 *      Author: petrstepanov
 */

#include "RooRealVarFrame.h"
#include "../AbstractView.h"
#include "../../util/RootHelper.h"

RooRealVarFrame::RooRealVarFrame() {
	initUI();
	connectSlots();
}

RooRealVarFrame::RooRealVarFrame(RooRealVar* var) {
	initUI();
	connectSlots();
	setRooRealVar(var);
}

RooRealVarFrame::~RooRealVarFrame() {}

void RooRealVarFrame::initUI(){

    // Name label
    nameLabel = new TGLabel(this, "");
    nameLabel->SetTextJustify(kTextLeft);
    AddFrame(nameLabel, new TGLayoutHints(kLHintsNormal, 0, 0, dy, dy));

    // Description label
    descriptionLabel = new TGLabel(this, "");
    descriptionLabel->SetTextJustify(kTextLeft);
    AddFrame(descriptionLabel, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, 0, 0, dy, dy));

    Double_t doubleMin = std::numeric_limits<Double_t>::min();
    Double_t doubleMax = std::numeric_limits<Double_t>::min();

    // Value
    valueNumberEntry = new TGNumberEntry(this, 0, 8, -1, TGNumberFormat::kNESReal,
                TGNumberFormat::kNEANonNegative,
                TGNumberFormat::kNELNoLimits,
                doubleMin, doubleMax);
    AddFrame(valueNumberEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop, dx, dx));

    // Minimum value
    minValueNumberEntry = new TGNumberEntry(this, 0, 8, -1, TGNumberFormat::kNESReal,
                TGNumberFormat::kNEANonNegative,
                TGNumberFormat::kNELNoLimits,
                doubleMin, doubleMax);
    AddFrame(minValueNumberEntry, new TGLayoutHints(kLHintsNormal, dx, dx));

    // Dash
    AddFrame(new TGLabel(this, "-"), new TGLayoutHints(kLHintsNormal, 0, 0, dy, dy));

    // Maximum maximum
    maxValueNumberEntry = new TGNumberEntry(this, 0, 5, -1, TGNumberFormat::kNESReal,
                TGNumberFormat::kNEANonNegative,
                TGNumberFormat::kNELNoLimits,
                doubleMin, doubleMax);
    AddFrame(maxValueNumberEntry, new TGLayoutHints(kLHintsNormal, dx, dx));

    // Fixed checkbox
    fixedCheckButton = new TGCheckButton(this, "fixed");
    AddFrame(fixedCheckButton, new TGLayoutHints(kLHintsNormal, 0, 0, dy, dy));
}

void RooRealVarFrame::connectSlots(){
    valueNumberEntry->Connect("ValueSet(Long_t)", "RooRealVarFrame", this, "onValueSet(Long_t)");
    minValueNumberEntry->Connect("ValueSet(Long_t)", "RooRealVarFrame", this, "onMinValueSet(Long_t)");
    maxValueNumberEntry->Connect("ValueSet(Long_t)", "RooRealVarFrame", this, "onMaxValueSet(Long_t)");
    fixedCheckButton->Connect("Toggled(Bool_t)", "RooRealVarFrame", this, "onFixedSet(Bool_t)");
}


void RooRealVarFrame::setRooRealVar(RooRealVar* var){
	this->rooRealVar = var;
	updateView();
}

// Slots
void RooRealVarFrame::onValueSet(Long_t value){
	rooRealVar->setVal(value);
	updateView();
}

void RooRealVarFrame::onMinValueSet(Long_t value){
	rooRealVar->setMin(value);
	updateView();
}

void RooRealVarFrame::onMaxValueSet(Long_t value){
	rooRealVar->setMax(value);
	updateView();
}

void RooRealVarFrame::onFixedSet(Bool_t isFixed){
	rooRealVar->setConstant(isFixed);
	updateView();
}

void RooRealVarFrame::updateView(){
	valueNumberEntry->SetNumStyle(RootHelper::getNumberFormatStyle(rooRealVar->getVal()));
	valueNumberEntry->SetNumber(rooRealVar->getVal());
	minValueNumberEntry->SetNumStyle(RootHelper::getNumberFormatStyle(rooRealVar->getMin()));
	minValueNumberEntry->SetNumber(rooRealVar->getMin());
	maxValueNumberEntry->SetNumStyle(RootHelper::getNumberFormatStyle(rooRealVar->getMax()));
	maxValueNumberEntry->SetNumber(rooRealVar->getMax());
}
