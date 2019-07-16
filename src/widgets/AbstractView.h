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
#include "../util/Debug.h"

enum Padding {
	dx = 10, dy = 5
};

template<class P> class AbstractView: public TGCompositeFrame {
protected:
	P* presenter;
	virtual P* instantinatePresenter()=0;

public:
	AbstractView(const TGWindow *w = 0) : TGCompositeFrame(w) {
		presenter = nullptr;
	}

	~AbstractView() {
		delete presenter;
	}
};

#endif /* ABSTRACTVIEW_H */
