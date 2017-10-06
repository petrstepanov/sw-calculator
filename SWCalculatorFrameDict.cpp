// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME SWCalculatorFrameDict

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
#include "src/model/Constants.h"
#include "src/model/Model.h"
#include "src/roofit/AbstractModelProvider.h"
#include "src/roofit/ChannelConvolutionPdf.h"
#include "src/roofit/CompositeModelProvider.h"
#include "src/roofit/DampLorentzPdf.h"
#include "src/roofit/GaussianPdf.h"
#include "src/roofit/IndirectParamPdf.h"
#include "src/roofit/LorentzianPdf.h"
#include "src/roofit/OrthogonalPdf.h"
#include "src/roofit/ParabolaGaussModelProvider.h"
#include "src/roofit/ParabolaLorentzianModelProvider.h"
#include "src/roofit/ParabolaPdf.h"
#include "src/util/FileUtils.h"
#include "src/util/GraphicsHelper.h"
#include "src/util/HistProcessor.h"
#include "src/util/RootHelper.h"
#include "src/util/StringUtils.h"
#include "src/util/Variable.h"
#include "src/widgets/importSpectrumWidget/IImportSpectrumView.h"
#include "src/widgets/importSpectrumWidget/ImportSpectrumPresenter.h"
#include "src/widgets/importSpectrumWidget/ImportSpectrumView.h"
#include "src/widgets/swCalculatorWidget/SWCalculatorFrame.h"
#include "src/main.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *SWCalculatorFrame_Dictionary();
   static void SWCalculatorFrame_TClassManip(TClass*);
   static void delete_SWCalculatorFrame(void *p);
   static void deleteArray_SWCalculatorFrame(void *p);
   static void destruct_SWCalculatorFrame(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SWCalculatorFrame*)
   {
      ::SWCalculatorFrame *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::SWCalculatorFrame));
      static ::ROOT::TGenericClassInfo 
         instance("SWCalculatorFrame", "src/widgets/swCalculatorWidget/SWCalculatorFrame.h", 32,
                  typeid(::SWCalculatorFrame), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &SWCalculatorFrame_Dictionary, isa_proxy, 0,
                  sizeof(::SWCalculatorFrame) );
      instance.SetDelete(&delete_SWCalculatorFrame);
      instance.SetDeleteArray(&deleteArray_SWCalculatorFrame);
      instance.SetDestructor(&destruct_SWCalculatorFrame);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SWCalculatorFrame*)
   {
      return GenerateInitInstanceLocal((::SWCalculatorFrame*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SWCalculatorFrame*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *SWCalculatorFrame_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::SWCalculatorFrame*)0x0)->GetClass();
      SWCalculatorFrame_TClassManip(theClass);
   return theClass;
   }

   static void SWCalculatorFrame_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SWCalculatorFrame(void *p) {
      delete ((::SWCalculatorFrame*)p);
   }
   static void deleteArray_SWCalculatorFrame(void *p) {
      delete [] ((::SWCalculatorFrame*)p);
   }
   static void destruct_SWCalculatorFrame(void *p) {
      typedef ::SWCalculatorFrame current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SWCalculatorFrame

namespace {
  void TriggerDictionaryInitialization_SWCalculatorFrameDict_Impl() {
    static const char* headers[] = {
"src/model/Constants.h",
"src/model/Model.h",
"src/roofit/AbstractModelProvider.h",
"src/roofit/ChannelConvolutionPdf.h",
"src/roofit/CompositeModelProvider.h",
"src/roofit/DampLorentzPdf.h",
"src/roofit/GaussianPdf.h",
"src/roofit/IndirectParamPdf.h",
"src/roofit/LorentzianPdf.h",
"src/roofit/OrthogonalPdf.h",
"src/roofit/ParabolaGaussModelProvider.h",
"src/roofit/ParabolaLorentzianModelProvider.h",
"src/roofit/ParabolaPdf.h",
"src/util/FileUtils.h",
"src/util/GraphicsHelper.h",
"src/util/HistProcessor.h",
"src/util/RootHelper.h",
"src/util/StringUtils.h",
"src/util/Variable.h",
"src/widgets/importSpectrumWidget/IImportSpectrumView.h",
"src/widgets/importSpectrumWidget/ImportSpectrumPresenter.h",
"src/widgets/importSpectrumWidget/ImportSpectrumView.h",
"src/widgets/swCalculatorWidget/SWCalculatorFrame.h",
"src/main.h",
0
    };
    static const char* includePaths[] = {
"/Applications/root_v6.06.02/include",
"/Applications/root_v6.06.02/include",
"/Users/petrstepanov/Development/NetBeansProjects/sw-calculator/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "SWCalculatorFrameDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$src/widgets/swCalculatorWidget/SWCalculatorFrame.h")))  SWCalculatorFrame;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SWCalculatorFrameDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "src/model/Constants.h"
#include "src/model/Model.h"
#include "src/roofit/AbstractModelProvider.h"
#include "src/roofit/ChannelConvolutionPdf.h"
#include "src/roofit/CompositeModelProvider.h"
#include "src/roofit/DampLorentzPdf.h"
#include "src/roofit/GaussianPdf.h"
#include "src/roofit/IndirectParamPdf.h"
#include "src/roofit/LorentzianPdf.h"
#include "src/roofit/OrthogonalPdf.h"
#include "src/roofit/ParabolaGaussModelProvider.h"
#include "src/roofit/ParabolaLorentzianModelProvider.h"
#include "src/roofit/ParabolaPdf.h"
#include "src/util/FileUtils.h"
#include "src/util/GraphicsHelper.h"
#include "src/util/HistProcessor.h"
#include "src/util/RootHelper.h"
#include "src/util/StringUtils.h"
#include "src/util/Variable.h"
#include "src/widgets/importSpectrumWidget/IImportSpectrumView.h"
#include "src/widgets/importSpectrumWidget/ImportSpectrumPresenter.h"
#include "src/widgets/importSpectrumWidget/ImportSpectrumView.h"
#include "src/widgets/swCalculatorWidget/SWCalculatorFrame.h"
#include "src/main.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"SWCalculatorFrame", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SWCalculatorFrameDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SWCalculatorFrameDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SWCalculatorFrameDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SWCalculatorFrameDict() {
  TriggerDictionaryInitialization_SWCalculatorFrameDict_Impl();
}
