// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmBmonReco
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
#include "CbmBmonUnpackAlgo.h"
#include "CbmBmonUnpackConfig.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmBmonUnpackAlgo(void *p = nullptr);
   static void *newArray_CbmBmonUnpackAlgo(Long_t size, void *p);
   static void delete_CbmBmonUnpackAlgo(void *p);
   static void deleteArray_CbmBmonUnpackAlgo(void *p);
   static void destruct_CbmBmonUnpackAlgo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBmonUnpackAlgo*)
   {
      ::CbmBmonUnpackAlgo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBmonUnpackAlgo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBmonUnpackAlgo", ::CbmBmonUnpackAlgo::Class_Version(), "CbmBmonUnpackAlgo.h", 28,
                  typeid(::CbmBmonUnpackAlgo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBmonUnpackAlgo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBmonUnpackAlgo) );
      instance.SetNew(&new_CbmBmonUnpackAlgo);
      instance.SetNewArray(&newArray_CbmBmonUnpackAlgo);
      instance.SetDelete(&delete_CbmBmonUnpackAlgo);
      instance.SetDeleteArray(&deleteArray_CbmBmonUnpackAlgo);
      instance.SetDestructor(&destruct_CbmBmonUnpackAlgo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBmonUnpackAlgo*)
   {
      return GenerateInitInstanceLocal((::CbmBmonUnpackAlgo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBmonUnpackAlgo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmBmonUnpackConfig(void *p);
   static void deleteArray_CbmBmonUnpackConfig(void *p);
   static void destruct_CbmBmonUnpackConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBmonUnpackConfig*)
   {
      ::CbmBmonUnpackConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBmonUnpackConfig >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBmonUnpackConfig", ::CbmBmonUnpackConfig::Class_Version(), "CbmBmonUnpackConfig.h", 22,
                  typeid(::CbmBmonUnpackConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBmonUnpackConfig::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBmonUnpackConfig) );
      instance.SetDelete(&delete_CbmBmonUnpackConfig);
      instance.SetDeleteArray(&deleteArray_CbmBmonUnpackConfig);
      instance.SetDestructor(&destruct_CbmBmonUnpackConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBmonUnpackConfig*)
   {
      return GenerateInitInstanceLocal((::CbmBmonUnpackConfig*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBmonUnpackConfig*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmBmonUnpackAlgo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBmonUnpackAlgo::Class_Name()
{
   return "CbmBmonUnpackAlgo";
}

//______________________________________________________________________________
const char *CbmBmonUnpackAlgo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonUnpackAlgo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBmonUnpackAlgo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonUnpackAlgo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBmonUnpackAlgo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonUnpackAlgo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBmonUnpackAlgo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonUnpackAlgo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmBmonUnpackConfig::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBmonUnpackConfig::Class_Name()
{
   return "CbmBmonUnpackConfig";
}

//______________________________________________________________________________
const char *CbmBmonUnpackConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonUnpackConfig*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBmonUnpackConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonUnpackConfig*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBmonUnpackConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonUnpackConfig*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBmonUnpackConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonUnpackConfig*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmBmonUnpackAlgo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBmonUnpackAlgo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBmonUnpackAlgo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBmonUnpackAlgo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBmonUnpackAlgo(void *p) {
      return  p ? new(p) ::CbmBmonUnpackAlgo : new ::CbmBmonUnpackAlgo;
   }
   static void *newArray_CbmBmonUnpackAlgo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBmonUnpackAlgo[nElements] : new ::CbmBmonUnpackAlgo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBmonUnpackAlgo(void *p) {
      delete ((::CbmBmonUnpackAlgo*)p);
   }
   static void deleteArray_CbmBmonUnpackAlgo(void *p) {
      delete [] ((::CbmBmonUnpackAlgo*)p);
   }
   static void destruct_CbmBmonUnpackAlgo(void *p) {
      typedef ::CbmBmonUnpackAlgo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBmonUnpackAlgo

//______________________________________________________________________________
void CbmBmonUnpackConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBmonUnpackConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBmonUnpackConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBmonUnpackConfig::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmBmonUnpackConfig(void *p) {
      delete ((::CbmBmonUnpackConfig*)p);
   }
   static void deleteArray_CbmBmonUnpackConfig(void *p) {
      delete [] ((::CbmBmonUnpackConfig*)p);
   }
   static void destruct_CbmBmonUnpackConfig(void *p) {
      typedef ::CbmBmonUnpackConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBmonUnpackConfig

namespace {
  void TriggerDictionaryInitialization_libCbmBmonReco_Impl() {
    static const char* headers[] = {
"CbmBmonUnpackAlgo.h",
"CbmBmonUnpackConfig.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/bmon",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/bmon/unpack",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/bmon",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/bmon/unpack",
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
"/home/heinemann/gpuunpacker/cbmroot/fles/flestools",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/tof/unpack",
"/home/heinemann/gpuunpacker/cbmroot/core/eventdisplay",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils/flestools",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/detectors/bmon/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmBmonReco dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmBmonUnpackAlgo.h")))  CbmBmonUnpackAlgo;
class __attribute__((annotate("$clingAutoload$CbmBmonUnpackConfig.h")))  CbmBmonUnpackConfig;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmBmonReco dictionary payload"

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
#include "CbmBmonUnpackAlgo.h"
#include "CbmBmonUnpackConfig.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmBmonUnpackAlgo", payloadCode, "@",
"CbmBmonUnpackConfig", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmBmonReco",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmBmonReco_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmBmonReco_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmBmonReco() {
  TriggerDictionaryInitialization_libCbmBmonReco_Impl();
}
