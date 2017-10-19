// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME SWCalculatorDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "event/Event.h"
#include "event/EventBus.h"
#include "event/EventHandler.h"
#include "event/HandlerRegistration.h"
#include "event/Object.h"
#include "event/events/HistogramImportedEvent.h"
#include "event/events/IsTwoDetectorEvent.h"
#include "model/Constants.h"
#include "model/Model.h"
#include "roofit/AbstractModelProvider.h"
#include "roofit/ChannelConvolutionPdf.h"
#include "roofit/CompositeModelProvider.h"
#include "roofit/DampLorentzPdf.h"
#include "roofit/GaussianPdf.h"
#include "roofit/IndirectParamPdf.h"
#include "roofit/LorentzianPdf.h"
#include "roofit/OrthogonalPdf.h"
#include "roofit/ParabolaGaussModelProvider.h"
#include "roofit/ParabolaLorentzianModelProvider.h"
#include "roofit/ParabolaPdf.h"
#include "util/FileUtils.h"
#include "util/GraphicsHelper.h"
#include "util/HistProcessor.h"
#include "util/RootHelper.h"
#include "util/StringUtils.h"
#include "util/UiHelper.h"
#include "util/Variable.h"
#include "widgets/importSpectrumWidget/AbstractImportSpectrumPresenter.h"
#include "widgets/importSpectrumWidget/AbstractImportSpectrumView.h"
#include "widgets/importSpectrumWidget/ImportSourceSpectrumPresenter.h"
#include "widgets/importSpectrumWidget/ImportSourceSpectrumView.h"
#include "widgets/importSpectrumWidget/ImportSpectrumPresenter.h"
#include "widgets/importSpectrumWidget/ImportSpectrumView.h"
#include "widgets/swCalculatorWidget/SWCalculatorPresenter.h"
#include "widgets/swCalculatorWidget/SWCalculatorView.h"
#include "widgets/AbstractPresenter.h"
#include "widgets/AbstractView.h"
#include "widgets/MainView.h"
#include "main.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *AbstractImportSpectrumView_Dictionary();
   static void AbstractImportSpectrumView_TClassManip(TClass*);
   static void delete_AbstractImportSpectrumView(void *p);
   static void deleteArray_AbstractImportSpectrumView(void *p);
   static void destruct_AbstractImportSpectrumView(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AbstractImportSpectrumView*)
   {
      ::AbstractImportSpectrumView *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::AbstractImportSpectrumView));
      static ::ROOT::TGenericClassInfo 
         instance("AbstractImportSpectrumView", "widgets/importSpectrumWidget/AbstractImportSpectrumView.h", 32,
                  typeid(::AbstractImportSpectrumView), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AbstractImportSpectrumView_Dictionary, isa_proxy, 0,
                  sizeof(::AbstractImportSpectrumView) );
      instance.SetDelete(&delete_AbstractImportSpectrumView);
      instance.SetDeleteArray(&deleteArray_AbstractImportSpectrumView);
      instance.SetDestructor(&destruct_AbstractImportSpectrumView);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AbstractImportSpectrumView*)
   {
      return GenerateInitInstanceLocal((::AbstractImportSpectrumView*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::AbstractImportSpectrumView*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AbstractImportSpectrumView_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AbstractImportSpectrumView*)0x0)->GetClass();
      AbstractImportSpectrumView_TClassManip(theClass);
   return theClass;
   }

   static void AbstractImportSpectrumView_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *SWCalculatorView_Dictionary();
   static void SWCalculatorView_TClassManip(TClass*);
   static void *new_SWCalculatorView(void *p = 0);
   static void *newArray_SWCalculatorView(Long_t size, void *p);
   static void delete_SWCalculatorView(void *p);
   static void deleteArray_SWCalculatorView(void *p);
   static void destruct_SWCalculatorView(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SWCalculatorView*)
   {
      ::SWCalculatorView *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::SWCalculatorView));
      static ::ROOT::TGenericClassInfo 
         instance("SWCalculatorView", "widgets/swCalculatorWidget/SWCalculatorView.h", 38,
                  typeid(::SWCalculatorView), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &SWCalculatorView_Dictionary, isa_proxy, 0,
                  sizeof(::SWCalculatorView) );
      instance.SetNew(&new_SWCalculatorView);
      instance.SetNewArray(&newArray_SWCalculatorView);
      instance.SetDelete(&delete_SWCalculatorView);
      instance.SetDeleteArray(&deleteArray_SWCalculatorView);
      instance.SetDestructor(&destruct_SWCalculatorView);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SWCalculatorView*)
   {
      return GenerateInitInstanceLocal((::SWCalculatorView*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SWCalculatorView*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *SWCalculatorView_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::SWCalculatorView*)0x0)->GetClass();
      SWCalculatorView_TClassManip(theClass);
   return theClass;
   }

   static void SWCalculatorView_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_AbstractImportSpectrumView(void *p) {
      delete ((::AbstractImportSpectrumView*)p);
   }
   static void deleteArray_AbstractImportSpectrumView(void *p) {
      delete [] ((::AbstractImportSpectrumView*)p);
   }
   static void destruct_AbstractImportSpectrumView(void *p) {
      typedef ::AbstractImportSpectrumView current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AbstractImportSpectrumView

namespace ROOT {
   // Wrappers around operator new
   static void *new_SWCalculatorView(void *p) {
      return  p ? new(p) ::SWCalculatorView : new ::SWCalculatorView;
   }
   static void *newArray_SWCalculatorView(Long_t nElements, void *p) {
      return p ? new(p) ::SWCalculatorView[nElements] : new ::SWCalculatorView[nElements];
   }
   // Wrapper around operator delete
   static void delete_SWCalculatorView(void *p) {
      delete ((::SWCalculatorView*)p);
   }
   static void deleteArray_SWCalculatorView(void *p) {
      delete [] ((::SWCalculatorView*)p);
   }
   static void destruct_SWCalculatorView(void *p) {
      typedef ::SWCalculatorView current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SWCalculatorView

namespace {
  void TriggerDictionaryInitialization_SWCalculatorDict_Impl() {
    static const char* headers[] = {
"event/Event.h",
"event/EventBus.h",
"event/EventHandler.h",
"event/HandlerRegistration.h",
"event/Object.h",
"event/events/HistogramImportedEvent.h",
"event/events/IsTwoDetectorEvent.h",
"model/Constants.h",
"model/Model.h",
"roofit/AbstractModelProvider.h",
"roofit/ChannelConvolutionPdf.h",
"roofit/CompositeModelProvider.h",
"roofit/DampLorentzPdf.h",
"roofit/GaussianPdf.h",
"roofit/IndirectParamPdf.h",
"roofit/LorentzianPdf.h",
"roofit/OrthogonalPdf.h",
"roofit/ParabolaGaussModelProvider.h",
"roofit/ParabolaLorentzianModelProvider.h",
"roofit/ParabolaPdf.h",
"util/FileUtils.h",
"util/GraphicsHelper.h",
"util/HistProcessor.h",
"util/RootHelper.h",
"util/StringUtils.h",
"util/UiHelper.h",
"util/Variable.h",
"widgets/importSpectrumWidget/AbstractImportSpectrumPresenter.h",
"widgets/importSpectrumWidget/AbstractImportSpectrumView.h",
"widgets/importSpectrumWidget/ImportSourceSpectrumPresenter.h",
"widgets/importSpectrumWidget/ImportSourceSpectrumView.h",
"widgets/importSpectrumWidget/ImportSpectrumPresenter.h",
"widgets/importSpectrumWidget/ImportSpectrumView.h",
"widgets/swCalculatorWidget/SWCalculatorPresenter.h",
"widgets/swCalculatorWidget/SWCalculatorView.h",
"widgets/AbstractPresenter.h",
"widgets/AbstractView.h",
"widgets/MainView.h",
"main.h",
0
    };
    static const char* includePaths[] = {
"/Applications/root_v6.06.02/include",
"/Applications/root_v6.06.02/include",
"/Users/petrstepanov/Development/NetBeansProjects/sw-calculator/src/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "SWCalculatorDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$widgets/importSpectrumWidget/AbstractImportSpectrumView.h")))  AbstractImportSpectrumView;
class __attribute__((annotate("$clingAutoload$widgets/swCalculatorWidget/SWCalculatorPresenter.h")))  SWCalculatorView;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SWCalculatorDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "event/Event.h"
#include "event/EventBus.h"
#include "event/EventHandler.h"
#include "event/HandlerRegistration.h"
#include "event/Object.h"
#include "event/events/HistogramImportedEvent.h"
#include "event/events/IsTwoDetectorEvent.h"
#include "model/Constants.h"
#include "model/Model.h"
#include "roofit/AbstractModelProvider.h"
#include "roofit/ChannelConvolutionPdf.h"
#include "roofit/CompositeModelProvider.h"
#include "roofit/DampLorentzPdf.h"
#include "roofit/GaussianPdf.h"
#include "roofit/IndirectParamPdf.h"
#include "roofit/LorentzianPdf.h"
#include "roofit/OrthogonalPdf.h"
#include "roofit/ParabolaGaussModelProvider.h"
#include "roofit/ParabolaLorentzianModelProvider.h"
#include "roofit/ParabolaPdf.h"
#include "util/FileUtils.h"
#include "util/GraphicsHelper.h"
#include "util/HistProcessor.h"
#include "util/RootHelper.h"
#include "util/StringUtils.h"
#include "util/UiHelper.h"
#include "util/Variable.h"
#include "widgets/importSpectrumWidget/AbstractImportSpectrumPresenter.h"
#include "widgets/importSpectrumWidget/AbstractImportSpectrumView.h"
#include "widgets/importSpectrumWidget/ImportSourceSpectrumPresenter.h"
#include "widgets/importSpectrumWidget/ImportSourceSpectrumView.h"
#include "widgets/importSpectrumWidget/ImportSpectrumPresenter.h"
#include "widgets/importSpectrumWidget/ImportSpectrumView.h"
#include "widgets/swCalculatorWidget/SWCalculatorPresenter.h"
#include "widgets/swCalculatorWidget/SWCalculatorView.h"
#include "widgets/AbstractPresenter.h"
#include "widgets/AbstractView.h"
#include "widgets/MainView.h"
#include "main.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"AbstractImportSpectrumView", payloadCode, "@",
"SWCalculatorView", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SWCalculatorDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SWCalculatorDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SWCalculatorDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SWCalculatorDict() {
  TriggerDictionaryInitialization_SWCalculatorDict_Impl();
}
