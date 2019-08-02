/*
 * RooRealVarListFrame.h
 *
 *  Created on: Jul 14, 2019
 *      Author: petrstepanov
 */

#ifndef SRC_FRAMES_RooRealVarListFrame_H_
#define SRC_FRAMES_RooRealVarListFrame_H_

#include <TGFrame.h>
#include <RooRealVar.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TGButton.h>
#include <RooArgList.h>

class RooRealVarListFrame : public TGVerticalFrame {
public:
	RooRealVarListFrame(const TGWindow *p = 0, RooArgSet* vars = 0);
	virtual ~RooRealVarListFrame();

	void setParameters(RooArgSet* parameters);

private:
	void initUI();
	RooArgList* parameters;

	ClassDef(RooRealVarListFrame,0)
};

#endif /* SRC_FRAMES_RooRealVarListFrame_H_ */
