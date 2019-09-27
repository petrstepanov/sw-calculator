/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImportComponentView.h
 * Author: petrstepanov
 *
 * Created on October 11, 2017, 6:36 PM
 */

#include "ImportComponentView.h"
#include "../../util/GraphicsHelper.h"

ImportComponentView::ImportComponentView(const TGWindow *w) : AbstractImportSpectrumView(w){
    presenter = instantinatePresenter();
}

// Override base class virtual functions
AbstractImportSpectrumPresenter* ImportComponentView::instantinatePresenter() {
    return new ImportComponentPresenter(this);
};

Int_t ImportComponentView::getHistogramColor(){
    return GraphicsHelper::colorPrimary->GetNumber();
}
