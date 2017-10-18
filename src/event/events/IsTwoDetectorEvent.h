/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IsTwoDetectorEvent.h
 * Author: petrstepanov
 *
 * Created on October 18, 2017, 9:20 AM
 */

#ifndef ISTWODETECTOREVENT_H
#define ISTWODETECTOREVENT_H

#include "../Object.h"
#include "../Event.h"
#include <RtypesCore.h>

class IsTwoDetectorEvent : public Event {
  public:
    IsTwoDetectorEvent(Object & sender, Bool_t _isTwoDetector) : Event(sender), twoDetector(_isTwoDetector) {
    }
  
    virtual ~IsTwoDetectorEvent() { }

    Double_t isTwoDetector(){
        return twoDetector;
    }

  private:
    Bool_t twoDetector;
};

#endif /* ISTWODETECTOREVENT_H */

