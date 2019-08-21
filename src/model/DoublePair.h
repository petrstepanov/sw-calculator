/*
 * DoublePair.h
 *
 *  Created on: Jul 31, 2019
 *      Author: petrstepanov
 */

#ifndef SRC_MODEL_DOUBLEPAIR_H_
#define SRC_MODEL_DOUBLEPAIR_H_

#include <Rtypes.h>

class DoublePair {
public:
	DoublePair();
	DoublePair(Double_t first, Double_t second);
	virtual ~DoublePair();

	Double_t first;
	Double_t second;
};

#endif /* SRC_MODEL_DOUBLEPAIR_H_ */
