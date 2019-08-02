/*
 * MyTGTextEntry.h
 *
 *  Created on: Aug 2, 2019
 *      Author: petrstepanov
 */

#ifndef SRC_WIDGETS_OVERRIDES_MYTGNUMBERENTRY_H_
#define SRC_WIDGETS_OVERRIDES_MYTGNUMBERENTRY_H_

#include <TGNumberEntry.h>

class MyTGNumberEntry: public TGNumberEntry {
public:
	MyTGNumberEntry(const TGWindow *parent = 0, Double_t val = 0,
	                 Int_t digitwidth = 5, Int_t id = -1,
	                 EStyle style = kNESReal,
	                 EAttribute attr = kNEAAnyNumber,
	                 ELimit limits = kNELNoLimits,
	                 Double_t min = 0, Double_t max = 1);
	virtual ~MyTGNumberEntry();

	virtual Bool_t HandleFocusChange(Event_t *event);

	ClassDef(MyTGNumberEntry,0)
};

#endif /* SRC_WIDGETS_OVERRIDES_MYTGNUMBERENTRY_H_ */
