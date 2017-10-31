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
#include "event/events/SourceHistogramImportedEvent.h"
#include "model/Constants.h"
#include "model/Model.h"
#include "roofit/AbstractModelProvider.h"
#include "roofit/BackgroundPdf.h"
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
#include "widgets/rooRealVarWidget/RooRealVarPresenter.h"
#include "widgets/rooRealVarWidget/RooRealVarView.h"
#include "widgets/swCalculatorWidget/SWCalculatorPresenter.h"
#include "widgets/swCalculatorWidget/SWCalculatorView.h"
#include "widgets/AbstractPresenter.h"
#include "widgets/AbstractView.h"
#include "widgets/MainView.h"
#include "main.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_AbstractImportSpectrumView(void *p);
   static void deleteArray_AbstractImportSpectrumView(void *p);
   static void destruct_AbstractImportSpectrumView(void *p);
   static void streamer_AbstractImportSpectrumView(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AbstractImportSpectrumView*)
   {
      ::AbstractImportSpectrumView *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AbstractImportSpectrumView >(0);
      static ::ROOT::TGenericClassInfo 
         instance("AbstractImportSpectrumView", ::AbstractImportSpectrumView::Class_Version(), "widgets/importSpectrumWidget/AbstractImportSpectrumView.h", 32,
                  typeid(::AbstractImportSpectrumView), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AbstractImportSpectrumView::Dictionary, isa_proxy, 16,
                  sizeof(::AbstractImportSpectrumView) );
      instance.SetDelete(&delete_AbstractImportSpectrumView);
      instance.SetDeleteArray(&deleteArray_AbstractImportSpectrumView);
      instance.SetDestructor(&destruct_AbstractImportSpectrumView);
      instance.SetStreamerFunc(&streamer_AbstractImportSpectrumView);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AbstractImportSpectrumView*)
   {
      return GenerateInitInstanceLocal((::AbstractImportSpectrumView*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::AbstractImportSpectrumView*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_RooRealVarView(void *p);
   static void deleteArray_RooRealVarView(void *p);
   static void destruct_RooRealVarView(void *p);
   static void streamer_RooRealVarView(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooRealVarView*)
   {
      ::RooRealVarView *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooRealVarView >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooRealVarView", ::RooRealVarView::Class_Version(), "widgets/swCalculatorWidget/../rooRealVarWidget/RooRealVarView.h", 26,
                  typeid(::RooRealVarView), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooRealVarView::Dictionary, isa_proxy, 16,
                  sizeof(::RooRealVarView) );
      instance.SetDelete(&delete_RooRealVarView);
      instance.SetDeleteArray(&deleteArray_RooRealVarView);
      instance.SetDestructor(&destruct_RooRealVarView);
      instance.SetStreamerFunc(&streamer_RooRealVarView);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooRealVarView*)
   {
      return GenerateInitInstanceLocal((::RooRealVarView*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::RooRealVarView*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_SWCalculatorView(void *p = 0);
   static void *newArray_SWCalculatorView(Long_t size, void *p);
   static void delete_SWCalculatorView(void *p);
   static void deleteArray_SWCalculatorView(void *p);
   static void destruct_SWCalculatorView(void *p);
   static void streamer_SWCalculatorView(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SWCalculatorView*)
   {
      ::SWCalculatorView *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SWCalculatorView >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SWCalculatorView", ::SWCalculatorView::Class_Version(), "widgets/swCalculatorWidget/SWCalculatorView.h", 42,
                  typeid(::SWCalculatorView), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::SWCalculatorView::Dictionary, isa_proxy, 16,
                  sizeof(::SWCalculatorView) );
      instance.SetNew(&new_SWCalculatorView);
      instance.SetNewArray(&newArray_SWCalculatorView);
      instance.SetDelete(&delete_SWCalculatorView);
      instance.SetDeleteArray(&deleteArray_SWCalculatorView);
      instance.SetDestructor(&destruct_SWCalculatorView);
      instance.SetStreamerFunc(&streamer_SWCalculatorView);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SWCalculatorView*)
   {
      return GenerateInitInstanceLocal((::SWCalculatorView*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SWCalculatorView*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr AbstractImportSpectrumView::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *AbstractImportSpectrumView::Class_Name()
{
   return "AbstractImportSpectrumView";
}

//______________________________________________________________________________
const char *AbstractImportSpectrumView::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AbstractImportSpectrumView*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int AbstractImportSpectrumView::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AbstractImportSpectrumView*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AbstractImportSpectrumView::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AbstractImportSpectrumView*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AbstractImportSpectrumView::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AbstractImportSpectrumView*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RooRealVarView::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooRealVarView::Class_Name()
{
   return "RooRealVarView";
}

//______________________________________________________________________________
const char *RooRealVarView::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooRealVarView*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooRealVarView::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooRealVarView*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooRealVarView::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooRealVarView*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooRealVarView::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooRealVarView*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr SWCalculatorView::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SWCalculatorView::Class_Name()
{
   return "SWCalculatorView";
}

//______________________________________________________________________________
const char *SWCalculatorView::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SWCalculatorView*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SWCalculatorView::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SWCalculatorView*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SWCalculatorView::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SWCalculatorView*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SWCalculatorView::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SWCalculatorView*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void AbstractImportSpectrumView::Streamer(TBuffer &R__b)
{
   // Stream an object of class AbstractImportSpectrumView.

   ::Error("AbstractImportSpectrumView::Streamer", "version id <=0 in ClassDef, dummy Streamer() called"); if (R__b.IsReading()) { }
}

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
   // Wrapper around a custom streamer member function.
   static void streamer_AbstractImportSpectrumView(TBuffer &buf, void *obj) {
      ((::AbstractImportSpectrumView*)obj)->::AbstractImportSpectrumView::Streamer(buf);
   }
} // end of namespace ROOT for class ::AbstractImportSpectrumView

//______________________________________________________________________________
void RooRealVarView::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooRealVarView.

   ::Error("RooRealVarView::Streamer", "version id <=0 in ClassDef, dummy Streamer() called"); if (R__b.IsReading()) { }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_RooRealVarView(void *p) {
      delete ((::RooRealVarView*)p);
   }
   static void deleteArray_RooRealVarView(void *p) {
      delete [] ((::RooRealVarView*)p);
   }
   static void destruct_RooRealVarView(void *p) {
      typedef ::RooRealVarView current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_RooRealVarView(TBuffer &buf, void *obj) {
      ((::RooRealVarView*)obj)->::RooRealVarView::Streamer(buf);
   }
} // end of namespace ROOT for class ::RooRealVarView

//______________________________________________________________________________
void SWCalculatorView::Streamer(TBuffer &R__b)
{
   // Stream an object of class SWCalculatorView.

   ::Error("SWCalculatorView::Streamer", "version id <=0 in ClassDef, dummy Streamer() called"); if (R__b.IsReading()) { }
}

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
   // Wrapper around a custom streamer member function.
   static void streamer_SWCalculatorView(TBuffer &buf, void *obj) {
      ((::SWCalculatorView*)obj)->::SWCalculatorView::Streamer(buf);
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
"event/events/SourceHistogramImportedEvent.h",
"model/Constants.h",
"model/Model.h",
"roofit/AbstractModelProvider.h",
"roofit/BackgroundPdf.h",
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
"widgets/rooRealVarWidget/RooRealVarPresenter.h",
"widgets/rooRealVarWidget/RooRealVarView.h",
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
class __attribute__((annotate("$clingAutoload$widgets/rooRealVarWidget/RooRealVarView.h")))  RooRealVarView;
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
#include "event/events/SourceHistogramImportedEvent.h"
#include "model/Constants.h"
#include "model/Model.h"
#include "roofit/AbstractModelProvider.h"
#include "roofit/BackgroundPdf.h"
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
#include "widgets/rooRealVarWidget/RooRealVarPresenter.h"
#include "widgets/rooRealVarWidget/RooRealVarView.h"
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
"RooRealVarView", payloadCode, "@",
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
