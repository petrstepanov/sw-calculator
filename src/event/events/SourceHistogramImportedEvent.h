/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SourceSourceHistogramImportedEvent.h
 * Author: petrstepanov
 *
 * Created on October 23, 2017, 4:49 AM
 */

#ifndef SOURCEHISTOGRAMIMPORTEDEVENT_H
#define SOURCEHISTOGRAMIMPORTEDEVENT_H

#include "../Object.h"
#include "../Event.h"

class SourceHistogramImportedEvent : public Event {
  public:
    SourceHistogramImportedEvent(Object & sender) : Event(sender) {
    }

    virtual ~SourceHistogramImportedEvent() { }
};

#endif /* SOURCEHISTOGRAMIMPORTEDEVENT_H */

