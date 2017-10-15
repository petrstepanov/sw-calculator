/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TTripleSliderDemo.cpp
 * Author: petrstepanov
 * 
 * Created on October 14, 2017, 9:32 PM
 */

#include "TTripleSliderDemo.h"

TTripleSliderDemo::TTripleSliderDemo(const TGWindow* w) : TGMainFrame(w, 100, 100)
{
   SetCleanup(kDeepCleanup);
   btn = new TGTextButton(this, "Button", HId1);
   btn->Connect("Clicked()", "TTripleSliderDemo", this, "HandleButtons()");
   AddFrame(btn, new TGLayoutHints(kLHintsTop | kLHintsCenterX, 5, 5, 5, 5));
   SetWindowName("Triple Slider Demo");
   MapSubwindows();
   Resize(GetDefaultSize());
   MapWindow();
}

TTripleSliderDemo::~TTripleSliderDemo(){
   Cleanup();
}

void TTripleSliderDemo::CloseWindow(){
   delete this;
}

void TTripleSliderDemo::HandleButtons(){
    std::cout << "Yo!" << std::endl;
}