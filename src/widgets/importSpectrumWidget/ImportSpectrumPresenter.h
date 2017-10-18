/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImportSpectrumPresenter.h
 * Author: petrstepanov
 *
 * Created on October 11, 2017, 6:30 PM
 */

#ifndef IMPORTSPECTRUMPRESENTER_H
#define IMPORTSPECTRUMPRESENTER_H

#include "AbstractImportSpectrumPresenter.h"
#include "ImportSpectrumView.h"
#include <TString.h>

//class ImpoertSpectrumView;

class ImportSpectrumPresenter : public AbstractImportSpectrumPresenter {
    public:
        ImportSpectrumPresenter(AbstractImportSpectrumView* view);
        
        void setModelFileName(TString* fileName);        
        void setModelHist(TH1F* hist);
};

#endif /* IMPORTSPECTRUMPRESENTER_H */

