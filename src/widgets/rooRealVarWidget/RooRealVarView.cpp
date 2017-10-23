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
#include <RooRealVar.h>

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

    // Description label
    description = new TGLabel(this, ""); // Variable name
    description->SetTextJustify(kTextLeft);
    AddFrame(description, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 0, dx, 3*dy/5));

    // Value textbox
    value = new TGNumberEntry(this, 0, 5, -1, TGNumberFormat::kNESRealTwo,
                TGNumberFormat::kNEANonNegative,
                TGNumberFormat::kNELLimitMinMax,
                -100, 100);
    value->GetNumberEntry()->Connect("TextChanged(char*)", "RooRealVarView", this, "onValueChange(char*)");    
    AddFrame(value, new TGLayoutHints(kLHintsLeft | kLHintsTop, dx, dx));
      
    // Minimum value
    minValue = new TGNumberEntry(this, 0, 5, -1, TGNumberFormat::kNESRealTwo,
                TGNumberFormat::kNEANonNegative,
                TGNumberFormat::kNELLimitMax,
                -100, 100);
    minValue->GetNumberEntry()->Connect("TextChanged(char*)", "RooRealVarView", this, "onMinimumChange(char*)");    
    AddFrame(minValue, new TGLayoutHints(kLHintsLeft | kLHintsTop, dx, dx));    
    
    // Dash label
    AddFrame(new TGLabel(this, "-"), new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 3*dy/5));
    
    // Minimum value
    maxValue = new TGNumberEntry(this, 0, 5, -1, TGNumberFormat::kNESRealTwo,
                TGNumberFormat::kNEANonNegative,
                TGNumberFormat::kNELLimitMin,
                -100, 100);
    maxValue->GetNumberEntry()->Connect("TextChanged(char*)", "RooRealVarView", this, "onMaximumChange(char*)");    
    AddFrame(maxValue, new TGLayoutHints(kLHintsLeft | kLHintsTop, dx, dx));    

    // Fixed checkbox
    fixed = new TGCheckButton(this, "fixed");
    AddFrame(fixed, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 2*dy/5, 0));
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
    RooRealVarPresenter* presenter = getPresenter();
    presenter->onMinimumChange(min);
}

void RooRealVarView::onMaximumChange(char* c) {
    Double_t max = maxValue->GetNumber();
    RooRealVarPresenter* presenter = getPresenter();
    presenter->onMaximumChange(max);
}

void RooRealVarView::onSetConstant(Bool_t isConstant) {
    RooRealVarPresenter* presenter = getPresenter();
    isConstant = fixed->IsOn();
    presenter->onSetConstant(isConstant);
    minValue->SetState(isConstant ? kFALSE : kTRUE);
    maxValue->SetState(isConstant ? kFALSE : kTRUE);    
}

void RooRealVarView::onValueChange(char* c) {
    Double_t val = value->GetNumber();
    RooRealVarPresenter* presenter = getPresenter();
    presenter->onValueChange(val);
}
