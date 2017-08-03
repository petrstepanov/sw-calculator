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
#include "AbstractModelProvider.h"
#include "FileUtils.h"
#include "GraphicsHelper.h"
#include "HistProcessor.h"
#include "ParabolaGaussModelProvider.h"
#include "SWCalculatorFrame.h"
#include "SimpleParabolaPdf.h"
#include "StringUtils.h"

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
         instance("SWCalculatorFrame", "SWCalculatorFrame.h", 31,
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
"AbstractModelProvider.h",
"FileUtils.h",
"GraphicsHelper.h",
"HistProcessor.h",
"ParabolaGaussModelProvider.h",
"SWCalculatorFrame.h",
"SimpleParabolaPdf.h",
"StringUtils.h",
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
class __attribute__((annotate("$clingAutoload$SWCalculatorFrame.h")))  SWCalculatorFrame;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SWCalculatorFrameDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "AbstractModelProvider.h"
#include "FileUtils.h"
#include "GraphicsHelper.h"
#include "HistProcessor.h"
#include "ParabolaGaussModelProvider.h"
#include "SWCalculatorFrame.h"
#include "SimpleParabolaPdf.h"
#include "StringUtils.h"

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
