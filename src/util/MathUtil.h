/*
 * MathUtil.h
 *
 *  Created on: Feb 4, 2019
 *      Author: student
 */

#ifndef SRC_UTIL_MATHUTIL_H_
#define SRC_UTIL_MATHUTIL_H_

#include <RtypesCore.h>

class MathUtil {
public:
//	MathUtil(){};
//	virtual ~MathUtil(){};

	static Int_t orderOfMagnitude(Double_t value);
	static Int_t numberOfDecimals(LongDouble_t value);
};

#endif /* SRC_UTIL_MATHUTIL_H_ */
