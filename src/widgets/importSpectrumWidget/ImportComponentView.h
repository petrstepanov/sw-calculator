/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImportSpectrumView.h
 * Author: petrstepanov
 *
 * Created on October 12, 2017, 4:50 PM
 */

#ifndef IMPORTCOMPONENTVIEW_H
#define IMPORTCOMPONENTVIEW_H

#include "AbstractImportSpectrumView.h"
#include "ImportComponentPresenter.h"
#include <TGFrame.h>

class ImportComponentView : public AbstractImportSpectrumView {
public:
    ImportComponentView(const TGWindow *p = 0);

    AbstractImportSpectrumPresenter* instantinatePresenter() override;
    
    Int_t getHistogramColor() override;

ClassDefOverride(ImportComponentView, 0)
};

#endif /* IMPORTSPECTRUMVIEW_H */

