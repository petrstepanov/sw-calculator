/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   RooRealVarView.h
 * Author: petrstepanov
 *
 * Created on October 23, 2017, 1:22 AM
 */

#ifndef ROOREALVARVIEW_H
#define ROOREALVARVIEW_H

#include "RooRealVarPresenter.h"
#include "../AbstractView.h"
#include <TGWindow.h>
#include <TString.h>
#include <RtypesCore.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TGButton.h>

class RooRealVarView : public AbstractView<RooRealVarPresenter> {
private:
    TGLabel* description;
    TGNumberEntry* value;
    TGNumberEntry* minValue;
    TGNumberEntry* maxValue;
    TGCheckButton* fixed;

public:
    RooRealVarView(const TGWindow *w);
    virtual ~RooRealVarView();

    // Override base class virtual functions
    void initUI();

    RooRealVarPresenter* instantinatePresenter();

    // Calls from presenter
    void setDescription(TString* str);
    void setValue(Double_t value);
    void setMinimum(Double_t min);
    void setMaximum(Double_t max);
    void setConstant(Bool_t isConstant);

    // Calls to presenter
    void onValueChange(char* c);
    void onMinimumChange(char* c);
    void onMaximumChange(char* c);
    void onSetConstant(Bool_t isConstant);

    // ClassDef(RooRealVarView,0);    
};

#endif /* ROOREALVARVIEW_H */
