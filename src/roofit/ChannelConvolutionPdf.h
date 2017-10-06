/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ChannelConvolutionPdf.h
 * Author: petrstepanov
 *
 * Created on September 25, 2017, 9:44 PM
 */

#ifndef CHANNELCONVOLUTIONPDF_H
#define CHANNELCONVOLUTIONPDF_H

#include <RooRealProxy.h>
#include <RooAbsPdf.h>
#include <RooAbsArg.h>
#include <RooListProxy.h>

class ChannelConvolutionPdf : public RooAbsPdf{
public:
    ChannelConvolutionPdf();
    ChannelConvolutionPdf(const char *name, const char *title, RooRealVar& convVar, RooAbsPdf& pdf, RooAbsPdf& resmodel) ;
    ChannelConvolutionPdf(const ChannelConvolutionPdf& other, const char* name=0) ;

    virtual TObject* clone(const char* newname) const { return new ChannelConvolutionPdf(*this,newname) ; }
    virtual ~ChannelConvolutionPdf() ;

    virtual Double_t evaluate() const ;
    
   // Access components
    RooRealVar&  var() const { return (RooRealVar&)(const_cast<RooAbsReal&>(_origVar.arg())) ; }
    RooAbsReal&  pdf() const { return const_cast<RooAbsReal&>(_origPdf.arg()) ; }
    RooAbsReal&  model() const { return const_cast<RooAbsReal&>(_origModel.arg()) ; }   

    void clearConvolutionWindow() ;
    void setConvolutionWindow(RooAbsReal& centerParam, RooAbsReal& widthParam, Double_t widthScaleFactor=1) ;    
//    void setConvolutionBins(Int_t) ;
    
protected:

    mutable Bool_t _init ; //! do not persist
    void initialize() const ;    
    
    RooRealProxy _origVar ;         // Original convolution variable
    RooRealProxy _origPdf ;         // Original input PDF
    RooRealProxy _origModel ;       // Original resolution model
    
    mutable RooArgSet    _ownedClonedPdfSet ;   // Owning set of cloned PDF components
    mutable RooArgSet    _ownedClonedModelSet ; // Owning set of cloned model components

    mutable RooRealVar*  _cloneVar ;        // Pointer to cloned convolution variable
    mutable RooAbsPdf*  _clonePdf ;        // Pointer to cloned PDF 
    mutable RooAbsPdf*  _cloneModel ;      // Pointer to cloned model

    Bool_t       _useWindow   ;     // Switch to activate window convolution
    Double_t     _windowScale ;     // Scale factor for window parameter
    RooListProxy _windowParam ;     // Holder for optional convolution integration window scaling parameter
    
    Int_t _bins ;
    
    RooRealVar& cloneVar()   const { if (!_init) initialize() ; return (RooRealVar&) *_cloneVar ; }
    RooAbsPdf&  clonePdf()   const { if (!_init) initialize() ; return (RooAbsPdf&) *_clonePdf ; }
    RooAbsPdf&  cloneModel() const { if (!_init) initialize() ; return (RooAbsPdf&) *_cloneModel ; }    
};

#endif /* CHANNELCONVOLUTIONPDF_H */

