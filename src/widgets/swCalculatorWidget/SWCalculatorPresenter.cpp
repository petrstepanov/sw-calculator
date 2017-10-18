/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SWCalculatorPresenter.cpp
 * Author: petrstepanov
 * 
 * Created on October 6, 2017, 6:31 PM
 */

#include "SWCalculatorPresenter.h"
#include "SWCalculatorView.h"
#include "../../model/Model.h"
#include "../../event/events/HistogramImportedEvent.h"
#include "../../event/EventBus.h"
#include "../../event/HandlerRegistration.h"
#include <iostream>

SWCalculatorPresenter::SWCalculatorPresenter(SWCalculatorView* view) : AbstractPresenter<Model, SWCalculatorView>(view) {
}

Model* SWCalculatorPresenter::instantinateModel(){
    return Model::getInstance();
}

void SWCalculatorPresenter::addEventListeners(){
    SWCalculatorView* view = getView();
    
    class HistogramImportedEventListener : public EventHandler<HistogramImportedEvent> {
      public:
        HistogramImportedEventListener(SWCalculatorView* v) : view(v) {}
        
        virtual void onEvent(HistogramImportedEvent & e) override {
            std::cout << "HistogramImportedEventListener::onEvent()" << std::endl;
            view->setFitMinMaxValues(e.getHistMin() + 0.5, e.getHistMax());
            view->setTabEnabled(2, true);
        }        
        SWCalculatorView* view = nullptr;
    };
    EventBus::AddHandler<HistogramImportedEvent>(*(new HistogramImportedEventListener(view)));
    
    class IsTwoDetectorEventListener : public EventHandler<IsTwoDetectorEvent> {
      public:
        IsTwoDetectorEventListener(SWCalculatorView* v) : view(v) {}
        
        virtual void onEvent(IsTwoDetectorEvent & e) override {
            std::cout << "IsTwoDetectorEvent::onEvent()" << std::endl;
            view->setTwoDetector(e.isTwoDetector());
        }        
        SWCalculatorView* view = nullptr;
    };
    EventBus::AddHandler<IsTwoDetectorEvent>(*(new IsTwoDetectorEventListener(view)));    
}

void SWCalculatorPresenter::onFitSpectrumClicked(){
    // Start Tracking Time
    RootHelper::startTimer();

    // Get UI values
    SWCalculatorView* view = getView();
    Int_t fitMin = view->getFitMinValue();
    Int_t fitMax = view->getFitMaxValue();

    // TODO: Set Display Range same as Fit Range
}