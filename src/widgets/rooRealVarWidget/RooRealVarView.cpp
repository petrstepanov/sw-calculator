/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   RooRealVarView.cpp
 * Author: petrstepanov
 *
 * Created on October 23, 2017, 1:22 AM
 */

#include "RooRealVarView.h"
#include "RooRealVarPresenter.h"
#include "../../util/UiHelper.h"
#include <RooRealVar.h>

ClassImp(RooRealVarView)

RooRealVarView::RooRealVarView(const TGWindow* w) : AbstractView<RooRealVarPresenter>(w){
    initUI();
}

RooRealVarView::~RooRealVarView() {
    delete description;
    delete value;
    delete minValue;
    delete maxValue;
    delete fixed;
}

void RooRealVarView::initUI() {
    // Set horizontal layout
    SetLayoutManager(new TGHorizontalLayout(this));

    // Name label
    description = new TGLabel(this, ""); // Variable name
    description->SetTextJustify(kTextLeft);
    AddFrame(description, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 0, EPadding::dx, 3*EPadding::dy/5));

    // Description label
    description = new TGLabel(this, ""); // Variable name
    description->SetTextJustify(kTextLeft);
    AddFrame(description, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 0, EPadding::dx, 3*EPadding::dy/5));

    Double_t doubleMin = std::numeric_limits<Double_t>::min();
    Double_t doubleMax = std::numeric_limits<Double_t>::min();

    // Value textbox
    value = new TGNumberEntry(this, 0, 5, -1, TGNumberFormat::kNESRealTwo,
                TGNumberFormat::kNEANonNegative,
                TGNumberFormat::kNELLimitMinMax,
                doubleMin, doubleMax);
    value->GetNumberEntry()->Connect("TextChanged(char*)", "RooRealVarView", this, "onValueChange(char*)");
    AddFrame(value, new TGLayoutHints(kLHintsLeft | kLHintsTop, EPadding::dx, EPadding::dx));

    // Minimum value
    minValue = new TGNumberEntry(this, 0, 5, -1, TGNumberFormat::kNESRealTwo,
                TGNumberFormat::kNEANonNegative,
                TGNumberFormat::kNELLimitMax,
                doubleMin, doubleMax);
    minValue->GetNumberEntry()->Connect("TextChanged(char*)", "RooRealVarView", this, "onMinimumChange(char*)");
    AddFrame(minValue, new TGLayoutHints(kLHintsLeft | kLHintsTop, EPadding::dx, EPadding::dx));

    // Dash label
    AddFrame(new TGLabel(this, "-"), new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 3*EPadding::dy/5));

    // Minimum value
    maxValue = new TGNumberEntry(this, 0, 5, -1, TGNumberFormat::kNESRealTwo,
                TGNumberFormat::kNEANonNegative,
                TGNumberFormat::kNELLimitMin,
                doubleMin, doubleMax);
    maxValue->GetNumberEntry()->Connect("TextChanged(char*)", "RooRealVarView", this, "onMaximumChange(char*)");
    AddFrame(maxValue, new TGLayoutHints(kLHintsLeft | kLHintsTop, EPadding::dx, EPadding::dx));

    // Fixed checkbox
    fixed = new TGCheckButton(this, "fixed");
    AddFrame(fixed, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 2*EPadding::dy/5, 0));
    fixed->Connect("Toggled(Bool_t)", "RooRealVarView", this, "onSetConstant(Bool_t)");
}

RooRealVarPresenter* RooRealVarView::instantinatePresenter(){
    return new RooRealVarPresenter(this);
}

// Calls from presenter

void RooRealVarView::setDescription(TString* str) {
    description->SetText(str->Data());
}

void RooRealVarView::setValue(Double_t val) {
    value->SetNumber(val);
}

void RooRealVarView::setMinimum(Double_t min) {
    minValue->SetNumber(min);
    value->SetLimitValues(min, value->GetNumMax());
}

void RooRealVarView::setMaximum(Double_t max) {
    maxValue->SetNumber(max);
    value->SetLimitValues(value->GetNumMin(), max);
}

void RooRealVarView::setConstant(Bool_t isConstant) {
    fixed->SetOn(isConstant, kFALSE);
    minValue->SetState(isConstant ? kFALSE : kTRUE);
    maxValue->SetState(isConstant ? kFALSE : kTRUE);
}

// Calls to presenter

void RooRealVarView::onMinimumChange(char* c) {
    Double_t min = minValue->GetNumber();
    presenter->onMinimumChange(min);
}

void RooRealVarView::onMaximumChange(char* c) {
    Double_t max = maxValue->GetNumber();
    presenter->onMaximumChange(max);
}

void RooRealVarView::onSetConstant(Bool_t isConstant) {
    isConstant = fixed->IsOn();
    presenter->onSetConstant(isConstant);
    minValue->SetState(isConstant ? kFALSE : kTRUE);
    maxValue->SetState(isConstant ? kFALSE : kTRUE);
}

void RooRealVarView::onValueChange(char* c) {
    Double_t val = value->GetNumber();
    presenter->onValueChange(val);
}
