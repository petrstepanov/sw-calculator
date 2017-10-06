/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IImportSpectrumView.h
 * Author: petrstepanov
 *
 * Created on October 6, 2017, 12:40 AM
 */

#ifndef IIMPORTSPECTRUMVIEW_H
#define IIMPORTSPECTRUMVIEW_H

class IImportSpectrumView {
  public:
//    virtual ~IImportSpectrumView();
    virtual void onOpenFileClicked() = 0;
    virtual void setFilenameLabel(const char*) = 0;
    virtual void setFilebrowserText() = 0;
    virtual void getEnergyColumnNumber() = 0;
    virtual void getNumberColumnNumber() = 0;
    virtual void onImportSpectrumClicked() = 0;    
    virtual void drawHistogram() = 0;    
};

#endif /* IIMPORTSPECTRUMVIEW_H */

