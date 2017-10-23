/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Model.h
 * Author: petrstepanov
 *
 * Created on October 6, 2017, 2:35 AM
 */

#ifndef MODEL_H
#define MODEL_H

#include <TString.h>
#include <TH1F.h>
#include <RtypesCore.h>
#include <RooCurve.h>
#include <iostream>
#include "../event/events/HistogramImportedEvent.h"
#include "../event/events/IsTwoDetectorEvent.h"
#include "../event/events/SourceHistogramImportedEvent.h"
#include "../event/Object.h"
#include "../event/EventBus.h"
#include <RooRealVar.h>

class Model : public Object {
public:
    static Model* getInstance();
    
    void setFileName(TString* fileName){ 
        std::cout << "Model::setFileName(), " << *fileName << std::endl;
        strFileName = fileName; 
    }
    
    TString* getFileName(){ 
        return strFileName; 
    }

    void setSourceFileName(TString* fileName){  
        std::cout << "Model::setSourceFileName(), " << fileName << std::endl;
        strSourceContribFileName = fileName; 
    }
    
    TString* getSourceFileName(){ 
        return strSourceContribFileName; 
    }
    
    void setHist(TH1F* hist){ 
        fullHist = hist;
        Double_t histMin = hist->GetXaxis()->GetXmin();
        Double_t histMax = hist->GetXaxis()->GetXmax();
    }
    
    TH1F* getHist(){ 
        return fullHist; 
    }

    void setSourceHist(TH1F* hist){ 
        sourceHist = hist; 
        EventBus::FireEvent(*(new SourceHistogramImportedEvent(*this))); // Fire the event           
    }
    
    TH1F* getSourceHist(){ 
        return sourceHist; 
    }

    void setTwoDetector(Bool_t b){ 
        twoDetector = b;
        EventBus::FireEvent(*(new IsTwoDetectorEvent(*this, twoDetector))); // Fire the event        
    }
    
    Bool_t isTwoDetector(){ 
        return twoDetector; 
    }

    void setSafeFitRange(Double_t eMin, Double_t eMax){
        safeFitRange = std::make_pair(eMin, eMax);
        EventBus::FireEvent(*(new HistogramImportedEvent(*this, eMin, eMax))); // Fire the event        
    }
    
    RooRealVar* getSourceContribution(){
        return sourceContribution;
    }
    
private:
    Model();                              // Private so that it can  not be called
    static Model* instance;

    TString* strFileName = nullptr;
    TString* strSourceContribFileName = nullptr;
    TH1F* fullHist = nullptr;
//    TH1F* peakHist;
    TH1F* sourceHist = nullptr;
    RooCurve* curveBg = nullptr;
    RooCurve* curveFit = nullptr;
    TH1F* peakHistNoBg = nullptr;
    TH1F* chiHist = nullptr;
    RooRealVar* sourceContribution = nullptr;
    Bool_t twoDetector;
    std::pair<Double_t, Double_t> safeFitRange;
};

#endif /* MODEL_H */

