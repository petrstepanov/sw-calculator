/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MainView.h
 * Author: petrstepanov
 *
 * Created on October 15, 2017, 1:47 AM
 */

#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <TGFrame.h>

class MainView : public TGMainFrame {
public:
    MainView(const TGWindow* w);
    virtual ~MainView();

    void mapAndResize();
};

#endif /* MAINVIEW_H */

