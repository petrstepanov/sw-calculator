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

class ImportSpectrumView : public AbstractImportSpectrumView<ImportSpectrumPresenter> {
public:
    ImportSpectrumView(const TGWindow *p = 0);

    ImportSpectrumPresenter* instantinatePresenter();
};

#endif /* IMPORTSPECTRUMVIEW_H */

