/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AddPdf.h
 * Author: petrstepanov
 *
 * Created on December 11, 2018, 4:52 PM
 */

#ifndef AddPdf_H
#define AddPdf_H

#include "RooAbsPdf.h"

class AddPdf {
public:
    AddPdf();
    AddPdf(const AddPdf& orig);
    virtual ~AddPdf();
    
//    static addPdf(RooArgList* pdfList, char* intensityNamePrefix = "");
//    static RooAbsPdf* addPdfRecursive(RooArgList* pdfList, const char* prefix = "");
    static RooAbsPdf* add(RooArgList* pdfList, RooRealVar* observable, const char* prefix = "");
    static RooAbsPdf* addReversed(RooArgList* pdfList, RooRealVar* observable, const char* prefix = "");
//    static RooAbsPdf* addRecursive(RooArgList* pdfList, const char* prefix = "");
private:

};

#endif /* AddPdf_H */

