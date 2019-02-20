/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReverseAddPdf.h
 * Author: petrstepanov
 *
 * Created on December 11, 2018, 4:52 PM
 */

#ifndef REVERSEADDPDF_H
#define REVERSEADDPDF_H

#include "RooAbsPdf.h"

class ReverseAddPdf {
public:
    ReverseAddPdf();
    ReverseAddPdf(const ReverseAddPdf& orig);
    virtual ~ReverseAddPdf();
    
//    static addPdf(RooArgList* pdfList, char* intensityNamePrefix = "");
//    static RooAbsPdf* addPdfRecursive(RooArgList* pdfList, const char* prefix = "");
    static RooAbsPdf* add(RooArgList* pdfList, const char* pdfName = "");
    static RooAbsPdf* addRecursive(RooArgList* pdfList, const char* pdfName = "");
private:

};

#endif /* REVERSEADDPDF_H */

