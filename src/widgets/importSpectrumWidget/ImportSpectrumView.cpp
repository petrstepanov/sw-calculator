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

#include "ImportSpectrumView.h"

ImportSpectrumView::ImportSpectrumView(const TGWindow *w) : AbstractImportSpectrumView<ImportSpectrumPresenter>(w){
}

// Override base class virtual functions
ImportSpectrumPresenter* ImportSpectrumView::instantinatePresenter() {
    return new ImportSpectrumPresenter(this);
};