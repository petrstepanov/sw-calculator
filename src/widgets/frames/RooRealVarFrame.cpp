/*
 * RooRealVarFrame.cpp
 *
 *  Created on: Jul 14, 2019
 *      Author: petrstepanov
 */

#include "RooRealVarFrame.h"
#include "../AbstractView.h"
#include "../../util/RootHelper.h"
#include "../../util/UiHelper.h"
#include <TString.h>
#include <TVirtualX.h>

ClassImp(RooRealVarFrame);

RooRealVarFrame::RooRealVarFrame(const TGWindow *p, RooRealVar* var) : TGHorizontalFrame(p) {
	initUI();
	if (var){
		setRooRealVar(var);
	}
	connectSlots();
}

void RooRealVarFrame::setRooRealVar(RooRealVar* var){
	this->rooRealVar = var;
	updateView();
}

void RooRealVarFrame::updateView(){
	// Set name
	nameLabel->SetText(rooRealVar->GetName());

	// Append unit to title
	if (strlen(rooRealVar->getUnit()) > 0){
		titleLabel->SetText(TString::Format("%s, %s", rooRealVar->GetTitle(), rooRealVar->getUnit()).Data());
	} else {
		titleLabel->SetText(rooRealVar->GetTitle());
	}

	// Set value and limits
	// valueNumberEntry->SetNumStyle(RootHelper::getNumberFormatStyle(rooRealVar->getVal()));
	if (!valueNumberEntry->HasFocus()){
		valueNumberEntry->SetNumber(rooRealVar->getVal());
	}
	// minValueNumberEntry->SetNumStyle(RootHelper::getNumberFormatStyle(rooRealVar->getMin()));
	if (!minValueNumberEntry->HasFocus()){
		minValueNumberEntry->SetNumber(rooRealVar->getMin());
	}
	// maxValueNumberEntry->SetNumStyle(RootHelper::getNumberFormatStyle(rooRealVar->getMax()));
	if (!maxValueNumberEntry->HasFocus()){
		maxValueNumberEntry->SetNumber(rooRealVar->getMax());
	}

	// Set fixed
	fixedCheckButton->SetOn(rooRealVar->isConstant());
}

RooRealVarFrame::~RooRealVarFrame() {}

void RooRealVarFrame::initUI(){
    // Name label
    nameLabel = new TGLabel(this, "");
    nameLabel->SetTextJustify(kTextLeft);
    nameLabel->ChangeOptions(nameLabel->GetOptions() | kFixedWidth);
    nameLabel->SetWidth(120);
    AddFrame(nameLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, dy, dy));

    // Description label
    titleLabel = new TGLabel(this, "");
    titleLabel->SetTextJustify(kTextLeft);
    AddFrame(titleLabel, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsCenterY, 0, 0, dy, dy));

    // Double_t doubleMin = std::numeric_limits<Double_t>::min();
    // Double_t doubleMax = std::numeric_limits<Double_t>::max();

    // Value
    valueNumberEntry = new TGNumberEntry(this, 0, 10, UiHelper::getUId(),
    			TGNumberFormat::kNESReal,
                TGNumberFormat::kNEAAnyNumber,
                TGNumberFormat::kNELNoLimits);
    AddFrame(valueNumberEntry, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, dx, dx));

    // Minimum value
    minValueNumberEntry = new TGNumberEntry(this, 0, 10, UiHelper::getUId(),
    			TGNumberFormat::kNESReal,
                TGNumberFormat::kNEAAnyNumber,
                TGNumberFormat::kNELNoLimits);
    AddFrame(minValueNumberEntry, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, dx, dx));

    // Dash
    AddFrame(new TGLabel(this, "-"), new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, dy, dy));

    // Maximum maximum
    maxValueNumberEntry = new TGNumberEntry(this, 0, 10, UiHelper::getUId(),
    			TGNumberFormat::kNESReal,
                TGNumberFormat::kNEAAnyNumber,
                TGNumberFormat::kNELNoLimits);
    AddFrame(maxValueNumberEntry, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, dx, dx));

    // Fixed checkbox
    fixedCheckButton = new TGCheckButton(this, "", UiHelper::getUId());
    AddFrame(fixedCheckButton, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, dx, 0, dy, dy));
}

void RooRealVarFrame::connectSlots(){
	// ->Connect("ValueSet(Long_t)", ...) - fires when changing value with buttons or hit Enter. Not fires on Blur event!
	// ->GetNumberEntry()->Connect("TextChanged(char*)", ...) - fires always, but cant type "-" or "."
	valueNumberEntry->Connect("ValueSet(Long_t)", "RooRealVarFrame", this, "onValueSet()");
    minValueNumberEntry->Connect("ValueSet(Long_t)", "RooRealVarFrame", this, "onMinValueSet()");
    maxValueNumberEntry->Connect("ValueSet(Long_t)", "RooRealVarFrame", this, "onMaxValueSet()");
//	valueNumberEntry->GetNumberEntry()->Connect("TextChanged(char*)", "RooRealVarFrame", this, "onValueSet(char*)");
//	minValueNumberEntry->GetNumberEntry()->Connect("TextChanged(char*)", "RooRealVarFrame", this, "onMinValueSet(char*)");
//	maxValueNumberEntry->GetNumberEntry()->Connect("TextChanged(char*)", "RooRealVarFrame", this, "onMaxValueSet(char*)");
    fixedCheckButton->Connect("Toggled(Bool_t)", "RooRealVarFrame", this, "onFixedSet(Bool_t)");
}

// Slots
void RooRealVarFrame::onValueSet(){
	Double_t value = valueNumberEntry->GetNumberEntry()->GetNumber();
	rooRealVar->setVal(value);
	updateView();
}

void RooRealVarFrame::onMinValueSet(){
	Double_t value = minValueNumberEntry->GetNumberEntry()->GetNumber();
	rooRealVar->setMin(value);
	updateView();
}

void RooRealVarFrame::onMaxValueSet(){
	Double_t value = maxValueNumberEntry->GetNumberEntry()->GetNumber();
	rooRealVar->setMax(value);
	updateView();
}

//void RooRealVarFrame::onValueSet(char* str){
//	// Ignore updating rooRealVar value if input was not a number
//	if (TString(str).IsFloat()){
//		Double_t value = valueNumberEntry->GetNumberEntry()->GetNumber();
//		rooRealVar->setVal(value);
//		updateView();
//	}
//}
//
//void RooRealVarFrame::onMinValueSet(char* str){
//	if (TString(str).IsFloat()){
//		Double_t value = minValueNumberEntry->GetNumberEntry()->GetNumber();
//		rooRealVar->setMin(value);
//		updateView();
//	}
//}
//
//void RooRealVarFrame::onMaxValueSet(char* str){
//	if (TString(str).IsFloat()){
//		Double_t value = maxValueNumberEntry->GetNumberEntry()->GetNumber();
//		rooRealVar->setMax(value);
//		updateView();
//	}
//}

void RooRealVarFrame::onFixedSet(Bool_t isFixed){
	rooRealVar->setConstant(isFixed);
	updateView();
}
