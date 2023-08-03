// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmPsdAna
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
#include "CbmPsdMCbmQaReal.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmPsdMCbmQaReal(void *p = nullptr);
   static void *newArray_CbmPsdMCbmQaReal(Long_t size, void *p);
   static void delete_CbmPsdMCbmQaReal(void *p);
   static void deleteArray_CbmPsdMCbmQaReal(void *p);
   static void destruct_CbmPsdMCbmQaReal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdMCbmQaReal*)
   {
      ::CbmPsdMCbmQaReal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdMCbmQaReal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdMCbmQaReal", ::CbmPsdMCbmQaReal::Class_Version(), "CbmPsdMCbmQaReal.h", 25,
                  typeid(::CbmPsdMCbmQaReal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdMCbmQaReal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPsdMCbmQaReal) );
      instance.SetNew(&new_CbmPsdMCbmQaReal);
      instance.SetNewArray(&newArray_CbmPsdMCbmQaReal);
      instance.SetDelete(&delete_CbmPsdMCbmQaReal);
      instance.SetDeleteArray(&deleteArray_CbmPsdMCbmQaReal);
      instance.SetDestructor(&destruct_CbmPsdMCbmQaReal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdMCbmQaReal*)
   {
      return GenerateInitInstanceLocal((::CbmPsdMCbmQaReal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdMCbmQaReal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdMCbmQaReal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdMCbmQaReal::Class_Name()
{
   return "CbmPsdMCbmQaReal";
}

//______________________________________________________________________________
const char *CbmPsdMCbmQaReal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMCbmQaReal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdMCbmQaReal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMCbmQaReal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdMCbmQaReal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMCbmQaReal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdMCbmQaReal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMCbmQaReal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmPsdMCbmQaReal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdMCbmQaReal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPsdMCbmQaReal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPsdMCbmQaReal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdMCbmQaReal(void *p) {
      return  p ? new(p) ::CbmPsdMCbmQaReal : new ::CbmPsdMCbmQaReal;
   }
   static void *newArray_CbmPsdMCbmQaReal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdMCbmQaReal[nElements] : new ::CbmPsdMCbmQaReal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdMCbmQaReal(void *p) {
      delete ((::CbmPsdMCbmQaReal*)p);
   }
   static void deleteArray_CbmPsdMCbmQaReal(void *p) {
      delete [] ((::CbmPsdMCbmQaReal*)p);
   }
   static void destruct_CbmPsdMCbmQaReal(void *p) {
      typedef ::CbmPsdMCbmQaReal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPsdMCbmQaReal

namespace {
  void TriggerDictionaryInitialization_libCbmPsdAna_Impl() {
    static const char* headers[] = {
"CbmPsdMCbmQaReal.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/detectors/psd",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/detectors/psd",
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
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/detectors/psd/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmPsdAna dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmPsdMCbmQaReal.h")))  CbmPsdMCbmQaReal;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmPsdAna dictionary payload"

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
#include "CbmPsdMCbmQaReal.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmPsdMCbmQaReal", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmPsdAna",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmPsdAna_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmPsdAna_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmPsdAna() {
  TriggerDictionaryInitialization_libCbmPsdAna_Impl();
}
