/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HistogramImportedEvent.h
 * Author: petrstepanov
 *
 * Created on October 15, 2017, 10:57 PM
 */

#ifndef HISTOGRAMIMPORTEDEVENT_H
#define HISTOGRAMIMPORTEDEVENT_H

#include "../Object.h"
#include "../Event.h"
#include <RtypesCore.h>

class HistogramImportedEvent : public Event {
  public:
    HistogramImportedEvent(Object & sender, Double_t _histMin, Double_t _histMax) : Event(sender), histMin(_histMin), histMax(_histMax) {
    }
  
    virtual ~HistogramImportedEvent() { }

    Double_t getHistMin(){
        return histMin;
    }

    Double_t getHistMax(){
        return histMax;
    }

  private:
    Double_t histMin;
    Double_t histMax;
};

#endif /* HISTOGRAMIMPORTEDEVENT_H */

