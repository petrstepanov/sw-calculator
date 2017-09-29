/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ChannelConvolutionPdf.cpp
 * Author: petrstepanov
 * 
 * Created on September 25, 2017, 9:44 PM
 */

#include "ChannelConvolutionPdf.h"
#include <RooRealVar.h>
#include <RooAbsReal.h>
#include <RooRealProxy.h>
#include <RooAbsPdf.h>
#include <RooCustomizer.h>
#include <iostream>

ChannelConvolutionPdf::ChannelConvolutionPdf() : 
    _init(kFALSE),
    _cloneVar(0),
    _clonePdf(0),
    _cloneModel(0),        
    _useWindow(kFALSE),
    _windowScale(1),
    _bins(1000) {
}

ChannelConvolutionPdf::ChannelConvolutionPdf(const char *name, const char *title, RooRealVar& convVar, RooAbsPdf& inPdf, RooAbsPdf& resmodel) : 
    RooAbsPdf(name,title),
    _init(kFALSE),        
    _origVar("!origVar","Original Convolution variable",this,convVar),
    _origPdf("!origPdf","Original Input PDF",this,inPdf),
    _origModel("!origModel","Original Resolution model",this,resmodel),
    _ownedClonedPdfSet("ownedClonePdfSet"),
    _ownedClonedModelSet("ownedCloneModelSet"),
    _cloneVar(0),
    _clonePdf(0),
    _cloneModel(0),
    _useWindow(kFALSE),
    _windowScale(1),
    _windowParam("windowParam","Convolution window parameter",this,kFALSE),
    _bins(1000) {
    // Constructor of convolution operator PDF
    // convVar  :  convolution variable (on which both pdf and resmodel should depend)
    // pdf      :  input 'physics' pdf
    // resmodel :  input 'resultion' pdf
    // output is pdf(x) (X) resmodel(x) = Int [ pdf(x') resmodel (x-x') ] dx'
}

ChannelConvolutionPdf::ChannelConvolutionPdf(const ChannelConvolutionPdf&  other, const char* name) :
    RooAbsPdf(other,name), 
    _init(kFALSE),        
    _origVar("!origVar",this,other._origVar),
    _origPdf("!origPdf",this,other._origPdf),
    _origModel("!origModel",this,other._origModel),
    _ownedClonedPdfSet("ownedClonePdfSet"),
    _ownedClonedModelSet("ownedCloneModelSet"),
    _cloneVar(0),
    _clonePdf(0),
    _cloneModel(0),
    _useWindow(kFALSE),
    _windowScale(1),
    _windowParam("windowParam","Convolution window parameter",this,kFALSE),
    _bins(1000) {
    // Make temporary clone of original convolution to preserve configuration information
    // This information will be propagated to a newly create convolution in a subsequent
    // call to initialize() 
}

ChannelConvolutionPdf::~ChannelConvolutionPdf() {
}

//void ChannelConvolutionPdf::setConvolutionBins(Int_t bins) {
//    _bins = bins;
//}

Double_t ChannelConvolutionPdf::evaluate() const {
    if (!_init) initialize() ;

    Double_t x = _origVar ;
//    Double_t min, max;

//    if (_useWindow) {
//        Double_t center = ((RooAbsReal*)_windowParam.at(0))->getVal() ;
//        Double_t width = _windowScale * ((RooAbsReal*)_windowParam.at(1))->getVal() ;
//        min = x-center-width;
//        max = x-center+width;
//    } else {
//        Double_t bufferFraction = 0.2;
//        min = (_cloneVar->getMin()) - bufferFraction*(_cloneVar->getMax()-_cloneVar->getMin());
//        max = (_cloneVar->getMax()) + bufferFraction*(_cloneVar->getMax()-_cloneVar->getMin());
//    }
//
//    Double_t step = (max-min)/_bins;    
//    Double_t sum = 0;
//    for (Double_t t = min; t <= max; t+=step){
//        _cloneVar->setVal(t);
//        Double_t pdfVal = _clonePdf->getVal(RooArgSet(*_cloneVar));
//        _cloneVar->setVal(x-t);
//        Double_t modelVal = _cloneModel->getVal(RooArgSet(*_cloneVar));
//        sum += pdfVal*modelVal;
//    }

    Double_t minBin, maxBin;
    const RooAbsBinning* xBinning = var().getBinningPtr("cache");
    if (_useWindow) {
        Double_t center = ((RooAbsReal*)_windowParam.at(0))->getVal() ;
        Double_t width = _windowScale * ((RooAbsReal*)_windowParam.at(1))->getVal() ;
        minBin = xBinning->binNumber(x-center-width);
        maxBin = xBinning->binNumber(x-center+width);
    } else {
        minBin = 0;
        maxBin = xBinning->numBins();
    }
   
    Double_t sum = 0;
    for (Double_t bin = minBin; bin < maxBin; bin++){
        Double_t t = xBinning->binCenter(bin);
        _cloneVar->setVal(t);
        Double_t pdfVal = _clonePdf->getVal(RooArgSet(*_cloneVar));
        _cloneVar->setVal(x-t);
        Double_t modelVal = _cloneModel->getVal(RooArgSet(*_cloneVar));
        sum += pdfVal*modelVal;
    }
    
    return sum;
}

void ChannelConvolutionPdf::clearConvolutionWindow(){
    _useWindow = kFALSE ;
    _windowParam.removeAll() ;
}

void ChannelConvolutionPdf::setConvolutionWindow(RooAbsReal& centerParam, RooAbsReal& widthParam, Double_t widthScaleFactor){
    _useWindow = kTRUE ;
    _windowParam.removeAll() ;
    _windowParam.add(centerParam) ;
    _windowParam.add(widthParam) ;
    _windowScale = widthScaleFactor ;
}

void ChannelConvolutionPdf::initialize() const {
    // Start out clean 
    _ownedClonedPdfSet.removeAll() ;
    _ownedClonedModelSet.removeAll() ;
    if (_cloneVar) delete _cloneVar ;

    // Customize a copy of origPdf that is connected to x' rather than x
    // store all cloned components in _clonePdfSet as well as x' itself
    _cloneVar = new RooRealVar(Form("%s_prime",_origVar.arg().GetName()),"Convolution Variable",0) ;
    _cloneVar->setMin(var().getMin());
    _cloneVar->setMax(var().getMax());
//    _cloneVar->setBins(var().getBins());
    
    RooCustomizer mgr1(pdf(),"NumConv_PdfClone") ;
    mgr1.setCloneBranchSet(_ownedClonedPdfSet) ;
    mgr1.replaceArg(var(),*_cloneVar) ;
    _clonePdf = (RooAbsPdf*) mgr1.build() ;
    _clonePdf->Print();

    RooCustomizer mgr2(model(),"NumConv_ModelClone") ;
    mgr2.setCloneBranchSet(_ownedClonedModelSet) ;
    mgr2.replaceArg(var(),*_cloneVar) ;
    _cloneModel = (RooAbsPdf*) mgr2.build() ;
    _cloneModel->Print();

    // Change name back to original name
    _cloneVar->SetName(var().GetName()) ;
    _cloneVar->Print();
    
    _init = kTRUE ;
}