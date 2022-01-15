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

// AbstractView inherits from TGCompositeFrame -> TQObject. Therefore its descendants can utilize QT signals

template<class P> class AbstractView: public TGCompositeFrame {
public:
    AbstractView(const TGWindow *w = 0) : TGCompositeFrame(w) {
        presenter = nullptr;
    }

    ~AbstractView() {
        delete presenter;
    }

protected:
    P* presenter;

    virtual P* instantinatePresenter()=0;

    // ClassDef(AbstractView,0)
};

// ClassImp(AbstractView);
#endif /* ABSTRACTVIEW_H */
