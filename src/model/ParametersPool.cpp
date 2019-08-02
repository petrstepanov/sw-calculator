/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParametersPool.cpp
 * Author: petrstepanov
 * 
 * Created on November 9, 2016, 6:08 PM
 */

#include "ParametersPool.h"
#include "Constants.h"
#include "TSystem.h"
#include "../util/RootHelper.h"
#include "../util/StringUtils.h"
#include "../util/Debug.h"
#include <sstream>
#include <iostream>

ParametersPool::ParametersPool() {
	parametersPool = new RooArgSet();
}

void ParametersPool::synchronizePdfParameters(RooArgSet* modelParameters) {
	// Update Model Parameter values from
	TIterator* it = modelParameters->createIterator();
	while (TObject* temp = it->Next()) {
		if (RooRealVar* modelParameter = dynamic_cast<RooRealVar*>(temp)) {
			// If pool parameter found - update model parameter values from pool
			// Replace pool parameter pointer with model pointer
			if (RooRealVar* poolParameter = (RooRealVar*) parametersPool->find(modelParameter->GetName())) {
				if (poolParameter != modelParameter){
					RootHelper::setRooRealVarValueLimits(modelParameter, poolParameter->getVal(), poolParameter->getMin(), poolParameter->getMax());
					modelParameter->setConstant(poolParameter->isConstant());

					parametersPool->remove(*poolParameter);
					parametersPool->add(*modelParameter);
					poolParameter->Delete();
				}
			}
			// If pool parameter not found - add model parameter to pool
			else {
				parametersPool->add(*modelParameter);
			}
		}
	}
}
