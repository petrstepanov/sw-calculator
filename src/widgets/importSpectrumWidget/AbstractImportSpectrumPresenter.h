/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImportSpectrumPresenter.h
 * Author: petrstepanov
 *
 * Created on October 6, 2017, 12:18 AM
 */

#ifndef ABSTRACTIMPORTSPECTRUMPRESENTER_H
#define ABSTRACTIMPORTSPECTRUMPRESENTER_H

#include "../AbstractPresenter.h"
#include "AbstractImportSpectrumView.h"
#include "../../model/Model.h"

template <class V>
class AbstractImportSpectrumPresenter : public AbstractPresenter<Model, V> {
public:
    AbstractImportSpectrumPresenter(V* view);
//    virtual ~AbstractImportSpectrumPresenter();

    // Override base class virtual methods
    Model* instantinateModel();
    
    // View functions
    void onOpenFileClicked();
    void onImportSpectrumClicked();
    
    // Different implementations for importing Kapton spectrum and regular spectrum
    virtual void setModelFileName(char*){};
    virtual void setModelHist(TH1F* hist, Bool_t isTwoDetector){};
};

#endif /* ABSTRACTIMPORTSPECTRUMPRESENTER_H */

