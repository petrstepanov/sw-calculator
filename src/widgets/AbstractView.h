/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IView.h
 * Author: petrstepanov
 *
 * Created on October 10, 2017, 7:10 PM
 */

#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <TGFrame.h>
#include <iostream>

enum Padding {
   dx = 10,
   dy = 5,
   d3x = 3*dx
};
//
//enum ViewLayout {
//    horizontal,
//    vertical
//};

template <class P>
class AbstractView : public TGCompositeFrame {
  public:
    AbstractView(TGWindow *w = 0) : 
        TGCompositeFrame(w){
            std::cout << "AbstractView::AbstractView()" << std::endl;            
//            parentCompositeFrame = w;
    }
        
    ~AbstractView(){
        delete presenter;
    }
    
    P* getPresenter(){
        if (presenter == NULL){
            presenter = instantinatePresenter();
        }
        return presenter;
    }
    
//    TGCompositeFrame* GetParentComposite(){
//        return parentCompositeFrame;
//    }

  private:
    P* presenter;
//    TGCompositeFrame* parentCompositeFrame;    
    
  protected:
    virtual P* instantinatePresenter()=0;
    virtual void initUI()=0;
};

#endif /* ABSTRACTVIEW_H */

