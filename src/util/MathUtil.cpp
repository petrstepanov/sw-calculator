/*
 * MathUtil.cpp
 *
 *  Created on: Feb 4, 2019
 *      Author: student
 */

#include <TMath.h>
#include "MathUtil.h"

Int_t MathUtil::orderOfMagnitude(Double_t number) {
	Int_t mag = 0;

	while (number > 1) {
		mag++;
		number = number / 10;
	};

	return --mag;
}

// https://stackoverflow.com/questions/1083304/c-c-counting-the-number-of-decimals
Int_t MathUtil::numberOfDecimals(LongDouble_t num){
	Int_t count = 0;
	num = abs(num);
	num = num - round(num);
	while(abs(num) >= 0.0000001){
	    num = num * 10;
	    count++;
	    num = num - round(num);
	}
	return TMath::Min(count,6);
}
