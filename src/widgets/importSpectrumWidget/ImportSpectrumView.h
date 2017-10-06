/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImportSpectrumView.h
 * Author: petrstepanov
 *
 * Created on October 5, 2017, 8:29 PM
 */

#ifndef IMPORTSPECTRUMVIEW_H
#define IMPORTSPECTRUMVIEW_H

#include <TGFrame.h>
#include "IImportSpectrumView.h"

class ImportSpectrumView : public TGFrame, public IImportSpectrumView {
public:
    ImportSpectrumView();
    ImportSpectrumView(const ImportSpectrumView& orig);
    virtual ~ImportSpectrumView();

    void onOpenFileClicked();
    void setFilenameLabel(const char* str);
    void setFilebrowserText();
    void getEnergyColumnNumber();
    void getNumberColumnNumber();
    void onImportSpectrumClicked();
    void drawHistogram();    
    
private:
    
};

#endif /* IMPORTSPECTRUMVIEW_H */

