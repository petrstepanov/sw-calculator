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

#ifndef IMPORTSOURECESPECTRUMVIEW_H
#define IMPORTSOURCESPECTRUMVIEW_H

#include "AbstractImportSpectrumView.h"
#include "ImportSpectrumPresenter.h"
#include <TGFrame.h>

class ImportSourceSpectrumView : public AbstractImportSpectrumView<ImportSpectrumPresenter> {
public:
    ImportSourceSpectrumView(TGCompositeFrame *p = 0);

    ImportSourceSpectrumPresenter* instantinatePresenter();
};

#endif /* IMPORTSPECTRUMVIEW_H */

