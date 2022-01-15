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

#ifndef IMPORTSPECTRUMVIEW_H
#define IMPORTSPECTRUMVIEW_H

#include "AbstractImportSpectrumView.h"
#include "ImportSpectrumPresenter.h"
#include <TGFrame.h>

class ImportSpectrumView : public AbstractImportSpectrumView {
public:
    ImportSpectrumView(const TGWindow *p = 0);

    AbstractImportSpectrumPresenter* instantinatePresenter() override;
    
    Int_t getHistogramColor() override;

    // https://root-forum.cern.ch/t/classdef-variants/44736/2
    ClassDefOverride(ImportSpectrumView, 0)
};

#endif /* IMPORTSPECTRUMVIEW_H */

