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
#include "../../model/Constants.h"


ImportSourceSpectrumView::ImportSourceSpectrumView(const TGWindow *w) : AbstractImportSpectrumView(w){
}

// Override base class virtual functions
AbstractImportSpectrumPresenter* ImportSourceSpectrumView::instantinatePresenter() {
    return new ImportSourceSpectrumPresenter(this);
};

Int_t ImportSourceSpectrumView::getHistogramColor(){
    return Constants::colorWarning->GetNumber();
}