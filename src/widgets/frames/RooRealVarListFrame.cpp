/*
 * RooRealVarListFrame.cpp
 *
 *  Created on: Jul 14, 2019
 *      Author: petrstepanov
 */

#include "RooRealVarListFrame.h"
#include "RooRealVarFrame.h"
#include "../AbstractView.h"
#include "../../util/RootHelper.h"
#include <TGLabel.h>
#include <TObject.h>
#include <TG3DLine.h>
#include <TGResourcePool.h>
#include <TGFont.h>
//#include <GuiTypes.h>

ClassImp(RooRealVarListFrame);

RooRealVarListFrame::RooRealVarListFrame(const TGWindow *p, RooArgSet* vars) : TGVerticalFrame(p) {
	parameters = new RooArgList(*vars);
	initUI();
}

RooRealVarListFrame::~RooRealVarListFrame() {}

void RooRealVarListFrame::initUI(){
	// Inuit bold font
	TGFont* menuHiliteFont = (TGFont*) gClient->GetResourcePool()->GetMenuHiliteFont();

	// Table header
	TGHorizontalFrame* headerFrame = new TGHorizontalFrame(this);

	TGLabel* nameLabel = new TGLabel(headerFrame, "Name");
	nameLabel->ChangeOptions(nameLabel->GetOptions() | kFixedWidth);
	nameLabel->SetTextJustify(kTextLeft);
	nameLabel->SetWidth(120);
	nameLabel->SetTextFont(menuHiliteFont);
	headerFrame->AddFrame(nameLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));

	TGLabel* titleLabel = new TGLabel(headerFrame, "Title");
	titleLabel->SetTextJustify(kTextLeft);
	titleLabel->SetTextFont(menuHiliteFont);
	headerFrame->AddFrame(titleLabel, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsCenterY));

	TGLabel* valueLabel = new TGLabel(headerFrame, "Value");
	valueLabel->ChangeOptions(nameLabel->GetOptions() | kFixedWidth);
	valueLabel->SetWidth(90);
	valueLabel->SetTextJustify(kTextLeft);
	valueLabel->SetTextFont(menuHiliteFont);
	headerFrame->AddFrame(valueLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, dx, dx));

	TGLabel* limitsLabel = new TGLabel(headerFrame, "Limits");
	limitsLabel->ChangeOptions(nameLabel->GetOptions() | kFixedWidth);
	limitsLabel->SetWidth(189);
	limitsLabel->SetTextJustify(kTextLeft);
	limitsLabel->SetTextFont(menuHiliteFont);
	headerFrame->AddFrame(limitsLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, dx, dx));

	TGLabel* fixedLabel = new TGLabel(headerFrame, "Fixed");
	fixedLabel->SetTextFont(menuHiliteFont);
	headerFrame->AddFrame(fixedLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, dx));

	this->AddFrame(headerFrame, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 0, 0, dy, dy));
	this->AddFrame(new TGHorizontal3DLine(this), new TGLayoutHints(kLHintsExpandX, 0, 0, dy, dy));

	// List of RooRealVars
	TIterator* it = parameters->createIterator();
	TObject* temp;
	while((temp = it->Next())){
		RooRealVar* var = dynamic_cast<RooRealVar*>(temp);
		if (var){
			RooRealVarFrame* frame = new RooRealVarFrame(this, var);
			this->AddFrame(frame, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 0, 0, dy, dy));
		}
	}
}
