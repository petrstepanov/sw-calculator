/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TTripleSliderDemo.h
 * Author: petrstepanov
 *
 * Created on October 14, 2017, 9:32 PM
 */

#ifndef TTRIPLESLIDERDEMO_H
#define TTRIPLESLIDERDEMO_H

#include <TGButton.h>
#include <TRootEmbeddedCanvas.h>
#include <TGLayout.h>
#include <TF1.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TGTextEntry.h>
#include <TGTripleSlider.h>

enum ETestCommandIdentifiers {
   HId1
};

class TTripleSliderDemo : public TGMainFrame {

private:
    TGTextButton* btn;

public:
   TTripleSliderDemo(const TGWindow* w);
   virtual ~TTripleSliderDemo();
   void CloseWindow();
   void HandleButtons();

//   ClassDef(TTripleSliderDemo, 0)
};

#endif /* TTRIPLESLIDERDEMO_H */

