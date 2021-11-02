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
#include "../../util/GraphicsHelper.h"

ClassImp(ImportSpectrumView);

ImportSpectrumView::ImportSpectrumView(const TGWindow *w) : AbstractImportSpectrumView(w){
    presenter = instantinatePresenter();
}

// Override base class virtual functions
AbstractImportSpectrumPresenter* ImportSpectrumView::instantinatePresenter() {
    return new ImportSpectrumPresenter(this);
};

Int_t ImportSpectrumView::getHistogramColor(){
    return GraphicsHelper::colorPrimary->GetNumber();
}
