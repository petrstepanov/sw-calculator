/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImportSourceSpectrumPresenter.h
 * Author: petrstepanov
 *
 * Created on October 12, 2017, 8:55 PM
 */

#ifndef IMPORTSOURCESPECTRUMPRESENTER_H
#define IMPORTSOURCESPECTRUMPRESENTER_H

#include "AbstractImportSpectrumPresenter.h"
#include "ImportSpectrumView.h"
#include <TString.h>

//class ImportSourceSpectrumView;

class ImportSourceSpectrumPresenter : public AbstractImportSpectrumPresenter {
    public:
        ImportSourceSpectrumPresenter(AbstractImportSpectrumView* view);
        
        void setModelFileName(const char* fileName);        
        void setModelHist(TH1F* hist, Bool_t isTwoDetector);
};

#endif /* IMPORTSOURCESPECTRUMPRESENTER_H */

