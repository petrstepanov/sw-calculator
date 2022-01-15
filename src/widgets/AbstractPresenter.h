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

#include <TQObject.h>

// AbstractPresenter needs to inherit TQObject. So its descendants can use QT signals
// AbstractPresenter needs to inherit TObject. So derived classes can use ClassName(), etc...

template<class M, class V> class AbstractPresenter: public TQObject, public TObject {
public:
    AbstractPresenter(V* v){
        view = v;
    }

    ~AbstractPresenter(){
        delete model;
        delete view;
    }

protected:
    M* model = nullptr;
    V* view = nullptr;

    virtual M* instantinateModel()=0;

    // ClassDef(AbstractPresenter,0)
};

#endif /* ABSTRACTPRESENTER_H */
