// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmMuchAnalysis
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
#include "CbmAnaMuonCandidate.h"
#include "CbmAnaDimuonAnalysis.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmAnaMuonCandidate(void *p = nullptr);
   static void *newArray_CbmAnaMuonCandidate(Long_t size, void *p);
   static void delete_CbmAnaMuonCandidate(void *p);
   static void deleteArray_CbmAnaMuonCandidate(void *p);
   static void destruct_CbmAnaMuonCandidate(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaMuonCandidate*)
   {
      ::CbmAnaMuonCandidate *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaMuonCandidate >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaMuonCandidate", ::CbmAnaMuonCandidate::Class_Version(), "CbmAnaMuonCandidate.h", 19,
                  typeid(::CbmAnaMuonCandidate), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaMuonCandidate::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaMuonCandidate) );
      instance.SetNew(&new_CbmAnaMuonCandidate);
      instance.SetNewArray(&newArray_CbmAnaMuonCandidate);
      instance.SetDelete(&delete_CbmAnaMuonCandidate);
      instance.SetDeleteArray(&deleteArray_CbmAnaMuonCandidate);
      instance.SetDestructor(&destruct_CbmAnaMuonCandidate);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaMuonCandidate*)
   {
      return GenerateInitInstanceLocal((::CbmAnaMuonCandidate*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaMuonCandidate*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmAnaDimuonAnalysis(void *p);
   static void deleteArray_CbmAnaDimuonAnalysis(void *p);
   static void destruct_CbmAnaDimuonAnalysis(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaDimuonAnalysis*)
   {
      ::CbmAnaDimuonAnalysis *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaDimuonAnalysis >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaDimuonAnalysis", ::CbmAnaDimuonAnalysis::Class_Version(), "CbmAnaDimuonAnalysis.h", 43,
                  typeid(::CbmAnaDimuonAnalysis), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaDimuonAnalysis::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaDimuonAnalysis) );
      instance.SetDelete(&delete_CbmAnaDimuonAnalysis);
      instance.SetDeleteArray(&deleteArray_CbmAnaDimuonAnalysis);
      instance.SetDestructor(&destruct_CbmAnaDimuonAnalysis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaDimuonAnalysis*)
   {
      return GenerateInitInstanceLocal((::CbmAnaDimuonAnalysis*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaDimuonAnalysis*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaMuonCandidate::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaMuonCandidate::Class_Name()
{
   return "CbmAnaMuonCandidate";
}

//______________________________________________________________________________
const char *CbmAnaMuonCandidate::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaMuonCandidate*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaMuonCandidate::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaMuonCandidate*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaMuonCandidate::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaMuonCandidate*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaMuonCandidate::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaMuonCandidate*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaDimuonAnalysis::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaDimuonAnalysis::Class_Name()
{
   return "CbmAnaDimuonAnalysis";
}

//______________________________________________________________________________
const char *CbmAnaDimuonAnalysis::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaDimuonAnalysis*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaDimuonAnalysis::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaDimuonAnalysis*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaDimuonAnalysis::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaDimuonAnalysis*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaDimuonAnalysis::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaDimuonAnalysis*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmAnaMuonCandidate::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaMuonCandidate.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaMuonCandidate::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaMuonCandidate::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaMuonCandidate(void *p) {
      return  p ? new(p) ::CbmAnaMuonCandidate : new ::CbmAnaMuonCandidate;
   }
   static void *newArray_CbmAnaMuonCandidate(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaMuonCandidate[nElements] : new ::CbmAnaMuonCandidate[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaMuonCandidate(void *p) {
      delete ((::CbmAnaMuonCandidate*)p);
   }
   static void deleteArray_CbmAnaMuonCandidate(void *p) {
      delete [] ((::CbmAnaMuonCandidate*)p);
   }
   static void destruct_CbmAnaMuonCandidate(void *p) {
      typedef ::CbmAnaMuonCandidate current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaMuonCandidate

//______________________________________________________________________________
void CbmAnaDimuonAnalysis::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaDimuonAnalysis.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaDimuonAnalysis::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaDimuonAnalysis::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmAnaDimuonAnalysis(void *p) {
      delete ((::CbmAnaDimuonAnalysis*)p);
   }
   static void deleteArray_CbmAnaDimuonAnalysis(void *p) {
      delete [] ((::CbmAnaDimuonAnalysis*)p);
   }
   static void destruct_CbmAnaDimuonAnalysis(void *p) {
      typedef ::CbmAnaDimuonAnalysis current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaDimuonAnalysis

namespace {
  void TriggerDictionaryInitialization_libCbmMuchAnalysis_Impl() {
    static const char* headers[] = {
"CbmAnaMuonCandidate.h",
"CbmAnaDimuonAnalysis.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dimuon",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dimuon",
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
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators/unigen",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators/pluto",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/PWGDIL/dimuon/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmMuchAnalysis dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmAnaMuonCandidate.h")))  CbmAnaMuonCandidate;
class __attribute__((annotate("$clingAutoload$CbmAnaDimuonAnalysis.h")))  CbmAnaDimuonAnalysis;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmMuchAnalysis dictionary payload"

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
#include "CbmAnaMuonCandidate.h"
#include "CbmAnaDimuonAnalysis.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmAnaDimuonAnalysis", payloadCode, "@",
"CbmAnaMuonCandidate", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmMuchAnalysis",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmMuchAnalysis_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmMuchAnalysis_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmMuchAnalysis() {
  TriggerDictionaryInitialization_libCbmMuchAnalysis_Impl();
}
