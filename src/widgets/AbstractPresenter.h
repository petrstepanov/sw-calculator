/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AbstractPresenter.h
 * Author: petrstepanov
 *
 * Created on October 10, 2017, 7:37 PM
 */

#ifndef ABSTRACTPRESENTER_H
#define ABSTRACTPRESENTER_H

template <class M, class V>

class AbstractPresenter {
  public:
    AbstractPresenter(V* v){
        model = nullptr;
        view = v;
        addEventListeners();
    }
    
    ~AbstractPresenter(){
        delete model;
//        delete view;
    }

    virtual V* getView() {
        return view;
    }

    M* getModel(){
        if (!model){
            model = instantinateModel();
        }          
        return model;
    }

  private:
    M* model;
    V* view;
    
  protected:
    virtual M* instantinateModel()=0;
    virtual void addEventListeners(){}
};

#endif /* ABSTRACTPRESENTER_H */

