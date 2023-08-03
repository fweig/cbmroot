// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmHadron
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
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

// Header files passed as explicit arguments
#include "CbmHadron.h"
#include "CbmHadronAnalysis.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmHadron(void *p = nullptr);
   static void *newArray_CbmHadron(Long_t size, void *p);
   static void delete_CbmHadron(void *p);
   static void deleteArray_CbmHadron(void *p);
   static void destruct_CbmHadron(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmHadron*)
   {
      ::CbmHadron *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmHadron >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmHadron", ::CbmHadron::Class_Version(), "CbmHadron.h", 18,
                  typeid(::CbmHadron), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmHadron::Dictionary, isa_proxy, 4,
                  sizeof(::CbmHadron) );
      instance.SetNew(&new_CbmHadron);
      instance.SetNewArray(&newArray_CbmHadron);
      instance.SetDelete(&delete_CbmHadron);
      instance.SetDeleteArray(&deleteArray_CbmHadron);
      instance.SetDestructor(&destruct_CbmHadron);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmHadron*)
   {
      return GenerateInitInstanceLocal((::CbmHadron*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmHadron*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmHadronAnalysis(void *p = nullptr);
   static void *newArray_CbmHadronAnalysis(Long_t size, void *p);
   static void delete_CbmHadronAnalysis(void *p);
   static void deleteArray_CbmHadronAnalysis(void *p);
   static void destruct_CbmHadronAnalysis(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmHadronAnalysis*)
   {
      ::CbmHadronAnalysis *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmHadronAnalysis >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmHadronAnalysis", ::CbmHadronAnalysis::Class_Version(), "CbmHadronAnalysis.h", 30,
                  typeid(::CbmHadronAnalysis), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmHadronAnalysis::Dictionary, isa_proxy, 4,
                  sizeof(::CbmHadronAnalysis) );
      instance.SetNew(&new_CbmHadronAnalysis);
      instance.SetNewArray(&newArray_CbmHadronAnalysis);
      instance.SetDelete(&delete_CbmHadronAnalysis);
      instance.SetDeleteArray(&deleteArray_CbmHadronAnalysis);
      instance.SetDestructor(&destruct_CbmHadronAnalysis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmHadronAnalysis*)
   {
      return GenerateInitInstanceLocal((::CbmHadronAnalysis*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmHadronAnalysis*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmHadron::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmHadron::Class_Name()
{
   return "CbmHadron";
}

//______________________________________________________________________________
const char *CbmHadron::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHadron*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmHadron::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHadron*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmHadron::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHadron*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmHadron::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHadron*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmHadronAnalysis::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmHadronAnalysis::Class_Name()
{
   return "CbmHadronAnalysis";
}

//______________________________________________________________________________
const char *CbmHadronAnalysis::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHadronAnalysis*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmHadronAnalysis::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHadronAnalysis*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmHadronAnalysis::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHadronAnalysis*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmHadronAnalysis::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHadronAnalysis*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmHadron::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmHadron.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmHadron::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmHadron::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmHadron(void *p) {
      return  p ? new(p) ::CbmHadron : new ::CbmHadron;
   }
   static void *newArray_CbmHadron(Long_t nElements, void *p) {
      return p ? new(p) ::CbmHadron[nElements] : new ::CbmHadron[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmHadron(void *p) {
      delete ((::CbmHadron*)p);
   }
   static void deleteArray_CbmHadron(void *p) {
      delete [] ((::CbmHadron*)p);
   }
   static void destruct_CbmHadron(void *p) {
      typedef ::CbmHadron current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmHadron

//______________________________________________________________________________
void CbmHadronAnalysis::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmHadronAnalysis.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmHadronAnalysis::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmHadronAnalysis::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmHadronAnalysis(void *p) {
      return  p ? new(p) ::CbmHadronAnalysis : new ::CbmHadronAnalysis;
   }
   static void *newArray_CbmHadronAnalysis(Long_t nElements, void *p) {
      return p ? new(p) ::CbmHadronAnalysis[nElements] : new ::CbmHadronAnalysis[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmHadronAnalysis(void *p) {
      delete ((::CbmHadronAnalysis*)p);
   }
   static void deleteArray_CbmHadronAnalysis(void *p) {
      delete [] ((::CbmHadronAnalysis*)p);
   }
   static void destruct_CbmHadronAnalysis(void *p) {
      typedef ::CbmHadronAnalysis current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmHadronAnalysis

namespace {
  void TriggerDictionaryInitialization_libCbmHadron_Impl() {
    static const char* headers[] = {
"CbmHadron.h",
"CbmHadronAnalysis.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGHAD/hadron",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGHAD/hadron",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/core/data",
"/home/heinemann/gpuunpacker/cbmroot/core/data/base",
"/home/heinemann/gpuunpacker/cbmroot/core/data/raw",
"/home/heinemann/gpuunpacker/cbmroot/core/data/mvd",
"/home/heinemann/gpuunpacker/cbmroot/core/data/bmon",
"/home/heinemann/gpuunpacker/cbmroot/core/data/sts",
"/home/heinemann/gpuunpacker/cbmroot/core/data/much",
"/home/heinemann/gpuunpacker/cbmroot/core/data/trd",
"/home/heinemann/gpuunpacker/cbmroot/core/data/tof",
"/home/heinemann/gpuunpacker/cbmroot/core/data/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/data/psd",
"/home/heinemann/gpuunpacker/cbmroot/core/data/global",
"/home/heinemann/gpuunpacker/cbmroot/algo/log/.",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc",
"/home/heinemann/gpuunpacker/cbmroot/external/xpu/src",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/tof",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/PWGHAD/hadron/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmHadron dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmHadron.h")))  CbmHadron;
class __attribute__((annotate("$clingAutoload$CbmHadronAnalysis.h")))  CbmHadronAnalysis;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmHadron dictionary payload"

#ifndef FMT_LOCALE
  #define FMT_LOCALE 1
#endif
#ifndef BOOST_SERIALIZATION_NO_LIB
  #define BOOST_SERIALIZATION_NO_LIB 1
#endif
#ifndef BOOST_SERIALIZATION_DYN_LINK
  #define BOOST_SERIALIZATION_DYN_LINK 1
#endif
#ifndef USE_WEBSOCKET
  #define USE_WEBSOCKET 1
#endif
#ifndef NO_SSL_DL
  #define NO_SSL_DL 1
#endif
#ifndef HTTP_WITHOUT_FASTCGI
  #define HTTP_WITHOUT_FASTCGI 1
#endif
#ifndef BOOST_LOG_NO_LIB
  #define BOOST_LOG_NO_LIB 1
#endif
#ifndef BOOST_LOG_DYN_LINK
  #define BOOST_LOG_DYN_LINK 1
#endif
#ifndef BOOST_ATOMIC_NO_LIB
  #define BOOST_ATOMIC_NO_LIB 1
#endif
#ifndef BOOST_ATOMIC_DYN_LINK
  #define BOOST_ATOMIC_DYN_LINK 1
#endif
#ifndef BOOST_CHRONO_NO_LIB
  #define BOOST_CHRONO_NO_LIB 1
#endif
#ifndef BOOST_CHRONO_DYN_LINK
  #define BOOST_CHRONO_DYN_LINK 1
#endif
#ifndef BOOST_FILESYSTEM_NO_LIB
  #define BOOST_FILESYSTEM_NO_LIB 1
#endif
#ifndef BOOST_FILESYSTEM_DYN_LINK
  #define BOOST_FILESYSTEM_DYN_LINK 1
#endif
#ifndef BOOST_REGEX_NO_LIB
  #define BOOST_REGEX_NO_LIB 1
#endif
#ifndef BOOST_REGEX_DYN_LINK
  #define BOOST_REGEX_DYN_LINK 1
#endif
#ifndef BOOST_THREAD_NO_LIB
  #define BOOST_THREAD_NO_LIB 1
#endif
#ifndef BOOST_THREAD_DYN_LINK
  #define BOOST_THREAD_DYN_LINK 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "CbmHadron.h"
#include "CbmHadronAnalysis.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmHadron", payloadCode, "@",
"CbmHadronAnalysis", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmHadron",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmHadron_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmHadron_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmHadron() {
  TriggerDictionaryInitialization_libCbmHadron_Impl();
}
