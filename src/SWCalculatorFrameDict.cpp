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
#include "util/Variable.h"
#include "widgets/importSpectrumWidget/AbstractImportSpectrumView.h"
#include "widgets/importSpectrumWidget/AbstractImportSpectrumPresenter.h"
#include "widgets/importSpectrumWidget/ImportSpectrumView.h"
#include "widgets/importSpectrumWidget/ImportSpectrumPresenter.h"
#include "widgets/importSpectrumWidget/ImportSourceSpectrumView.h"
#include "widgets/importSpectrumWidget/ImportSourceSpectrumPresenter.h"
#include "widgets/swCalculatorWidget/SWCalculatorView.h"
#include "widgets/swCalculatorWidget/SWCalculatorPresenter.h"
#include "widgets/AbstractPresenter.h"
#include "widgets/AbstractView.h"
#include "main.h"

// Header files passed via #pragma extra_include

namespace {
  void TriggerDictionaryInitialization_SWCalculatorFrameDict_Impl() {
    static const char* headers[] = {
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
"util/Variable.h",
"widgets/importSpectrumWidget/AbstractImportSpectrumView.h",
"widgets/importSpectrumWidget/AbstractImportSpectrumPresenter.h",
"widgets/importSpectrumWidget/ImportSpectrumView.h",
"widgets/importSpectrumWidget/ImportSpectrumPresenter.h",
"widgets/importSpectrumWidget/ImportSourceSpectrumView.h",
"widgets/importSpectrumWidget/ImportSourceSpectrumPresenter.h",
"widgets/swCalculatorWidget/SWCalculatorView.h",
"widgets/swCalculatorWidget/SWCalculatorPresenter.h",
"widgets/AbstractPresenter.h",
"widgets/AbstractView.h",
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
#line 1 "SWCalculatorFrameDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SWCalculatorFrameDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
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
#include "util/Variable.h"
#include "widgets/importSpectrumWidget/AbstractImportSpectrumView.h"
#include "widgets/importSpectrumWidget/AbstractImportSpectrumPresenter.h"
#include "widgets/importSpectrumWidget/ImportSpectrumView.h"
#include "widgets/importSpectrumWidget/ImportSpectrumPresenter.h"
#include "widgets/importSpectrumWidget/ImportSourceSpectrumView.h"
#include "widgets/importSpectrumWidget/ImportSourceSpectrumPresenter.h"
#include "widgets/swCalculatorWidget/SWCalculatorView.h"
#include "widgets/swCalculatorWidget/SWCalculatorPresenter.h"
#include "widgets/AbstractPresenter.h"
#include "widgets/AbstractView.h"
#include "main.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
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
