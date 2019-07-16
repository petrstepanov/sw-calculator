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
#include <iostream>

template<class M, class V> class AbstractPresenter: public TQObject {
protected:
	M* model = nullptr;
	V* view = nullptr;

	virtual M* instantinateModel()=0;

public:
	AbstractPresenter(V* v){
		#ifdef USEDEBUG
			std::cout << "AbtractPresenter::AbstractPresenter" << std::endl;
		#endif
		view = v;
	}

	~AbstractPresenter(){}
};

#endif /* ABSTRACTPRESENTER_H */


///*
// * To change this license header, choose License Headers in Project Properties.
// * To change this template file, choose Tools | Templates
// * and open the template in the editor.
// */
//
///*
// * File:   AbstractPresenter.h
// * Author: petrstepanov
// *
// * Created on October 10, 2017, 7:37 PM
// */
//
//#ifndef ABSTRACTPRESENTER_H
//#define ABSTRACTPRESENTER_H
//
//#include <iostream>
//
//template <class M, class V> class AbstractPresenter {
//  public:
//    AbstractPresenter(V* v){
//        model = nullptr;
//        view = v;
//    }
//
//    ~AbstractPresenter(){
//        delete model;
////        delete view;
//    }
//
//    virtual V* getView() {
//        return view;
//    }
//
//    M* getModel(){
//        if (!model){
//            model = instantinateModel();
//            onInitModel();
//        }
//        return model;
//    }
//
//    void setModel(M* m){
//        std::cout << "AbstractPresenter::setModel" << std::endl;
//        model = m;
//        onInitModel();
//    }
//
//  private:
//    M* model;
//    V* view;
//
//  protected:
//    virtual M* instantinateModel()=0;
//    virtual void addEventListeners(){}
//    virtual void onInitModel(){}
//};
//
//#endif /* ABSTRACTPRESENTER_H */
