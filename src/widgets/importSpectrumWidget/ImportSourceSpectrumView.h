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

#ifndef IMPORTSOURCESPECTRUMVIEW_H
#define IMPORTSOURCESPECTRUMVIEW_H

#include "AbstractImportSpectrumView.h"
#include "ImportSourceSpectrumPresenter.h"
#include <TGWindow.h>

class ImportSourceSpectrumView : public AbstractImportSpectrumView {
public:
    ImportSourceSpectrumView(const TGWindow *w = 0);

    AbstractImportSpectrumPresenter* instantinatePresenter() override;
    
    Int_t getHistogramColor() override;

    // https://root-forum.cern.ch/t/classdef-variants/44736/2
    ClassDefOverride(ImportSourceSpectrumView, 0)
};

#endif /* IMPORTSOURCESPECTRUMVIEW_H */

