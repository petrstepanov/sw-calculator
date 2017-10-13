/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImportSpectrumView.h
 * Author: petrstepanov
 *
 * Created on October 11, 2017, 6:36 PM
 */

#include "ImportSourceSpectrumView.h"

ImportSourceSpectrumView::ImportSourceSpectrumView(TGCompositeFrame *p) : AbstractImportSpectrumView<ImportSpectrumPresenter>(p){
}

// Override base class virtual functions
ImportSourceSpectrumPresenter* ImportSourceSpectrumView::instantinatePresenter() {
    return new ImportSourceSpectrumPresenter(this);
};