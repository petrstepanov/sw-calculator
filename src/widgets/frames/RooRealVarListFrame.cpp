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
#include "../../util/UiHelper.h"
#include "../../model/Constants.h"
#include <TGLabel.h>
#include <TObject.h>
#include <TG3DLine.h>
#include <TGResourcePool.h>
#include <TGFont.h>

ClassImp(RooRealVarListFrame);

RooRealVarListFrame::RooRealVarListFrame(const TGWindow *p, RooArgSet* vars) : TGVerticalFrame(p) {
	parameters = new RooArgList(*vars);
	initUI();
}

RooRealVarListFrame::~RooRealVarListFrame() {}

void RooRealVarListFrame::initUI(){
	// Init bold font
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
	headerFrame->AddFrame(valueLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, EPadding::dx, EPadding::dx));

	TGLabel* limitsLabel = new TGLabel(headerFrame, "Limits");
	limitsLabel->ChangeOptions(nameLabel->GetOptions() | kFixedWidth);
	limitsLabel->SetWidth(189);
	limitsLabel->SetTextJustify(kTextLeft);
	limitsLabel->SetTextFont(menuHiliteFont);
	headerFrame->AddFrame(limitsLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, EPadding::dx, EPadding::dx));

	TGLabel* fixedLabel = new TGLabel(headerFrame, "Fixed");
	fixedLabel->SetTextFont(menuHiliteFont);
	headerFrame->AddFrame(fixedLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, EPadding::dx));

	this->AddFrame(headerFrame, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 0, 0, EPadding::dy, EPadding::dy));
	this->AddFrame(new TGHorizontal3DLine(this), new TGLayoutHints(kLHintsExpandX, 0, 0, EPadding::dy, EPadding::dy));

	// List of RooRealVars
	TIterator* it = parameters->createIterator();
	TObject* temp;
	while((temp = it->Next())){
		RooRealVar* var = dynamic_cast<RooRealVar*>(temp);

		// Show all parameters excluding those who have ATTR_HIDE_PARAMETER_FROM_UI
		// Useful for recursive coefficients that are not too user friendly
		// TODISS: put in dissertation (mess top peak with higher statistics)
		if (var && !var->getAttribute(Constants::ATTR_HIDE_PARAMETER_FROM_UI)){
			RooRealVarFrame* frame = new RooRealVarFrame(this, var);
			this->AddFrame(frame, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 0, 0, EPadding::dy, EPadding::dy));
		}
	}
}
