// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmBmonSim
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
#include "CbmBmonDigitize.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *CbmDigitizelECbmBmonDigigR_Dictionary();
   static void CbmDigitizelECbmBmonDigigR_TClassManip(TClass*);
   static void delete_CbmDigitizelECbmBmonDigigR(void *p);
   static void deleteArray_CbmDigitizelECbmBmonDigigR(void *p);
   static void destruct_CbmDigitizelECbmBmonDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigitize<CbmBmonDigi>*)
   {
      ::CbmDigitize<CbmBmonDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigitize<CbmBmonDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigitize<CbmBmonDigi>", ::CbmDigitize<CbmBmonDigi>::Class_Version(), "CbmDigitize.h", 44,
                  typeid(::CbmDigitize<CbmBmonDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigitizelECbmBmonDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigitize<CbmBmonDigi>) );
      instance.SetDelete(&delete_CbmDigitizelECbmBmonDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigitizelECbmBmonDigigR);
      instance.SetDestructor(&destruct_CbmDigitizelECbmBmonDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigitize<CbmBmonDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigitize<CbmBmonDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigitize<CbmBmonDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigitizelECbmBmonDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmBmonDigi>*)nullptr)->GetClass();
      CbmDigitizelECbmBmonDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigitizelECbmBmonDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmBmonDigitize(void *p = nullptr);
   static void *newArray_CbmBmonDigitize(Long_t size, void *p);
   static void delete_CbmBmonDigitize(void *p);
   static void deleteArray_CbmBmonDigitize(void *p);
   static void destruct_CbmBmonDigitize(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBmonDigitize*)
   {
      ::CbmBmonDigitize *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBmonDigitize >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBmonDigitize", ::CbmBmonDigitize::Class_Version(), "CbmBmonDigitize.h", 29,
                  typeid(::CbmBmonDigitize), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBmonDigitize::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBmonDigitize) );
      instance.SetNew(&new_CbmBmonDigitize);
      instance.SetNewArray(&newArray_CbmBmonDigitize);
      instance.SetDelete(&delete_CbmBmonDigitize);
      instance.SetDeleteArray(&deleteArray_CbmBmonDigitize);
      instance.SetDestructor(&destruct_CbmBmonDigitize);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBmonDigitize*)
   {
      return GenerateInitInstanceLocal((::CbmBmonDigitize*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBmonDigitize*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigitize<CbmBmonDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmBmonDigi>::Class_Name()
{
   return "CbmDigitize<CbmBmonDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmBmonDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmBmonDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigitize<CbmBmonDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmBmonDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmBmonDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmBmonDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmBmonDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmBmonDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmBmonDigitize::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBmonDigitize::Class_Name()
{
   return "CbmBmonDigitize";
}

//______________________________________________________________________________
const char *CbmBmonDigitize::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonDigitize*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBmonDigitize::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonDigitize*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBmonDigitize::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonDigitize*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBmonDigitize::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonDigitize*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> void CbmDigitize<CbmBmonDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigitize<CbmBmonDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigitize<CbmBmonDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigitize<CbmBmonDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmDigitizelECbmBmonDigigR(void *p) {
      delete ((::CbmDigitize<CbmBmonDigi>*)p);
   }
   static void deleteArray_CbmDigitizelECbmBmonDigigR(void *p) {
      delete [] ((::CbmDigitize<CbmBmonDigi>*)p);
   }
   static void destruct_CbmDigitizelECbmBmonDigigR(void *p) {
      typedef ::CbmDigitize<CbmBmonDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigitize<CbmBmonDigi>

//______________________________________________________________________________
void CbmBmonDigitize::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBmonDigitize.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBmonDigitize::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBmonDigitize::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBmonDigitize(void *p) {
      return  p ? new(p) ::CbmBmonDigitize : new ::CbmBmonDigitize;
   }
   static void *newArray_CbmBmonDigitize(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBmonDigitize[nElements] : new ::CbmBmonDigitize[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBmonDigitize(void *p) {
      delete ((::CbmBmonDigitize*)p);
   }
   static void deleteArray_CbmBmonDigitize(void *p) {
      delete [] ((::CbmBmonDigitize*)p);
   }
   static void destruct_CbmBmonDigitize(void *p) {
      typedef ::CbmBmonDigitize current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBmonDigitize

namespace {
  void TriggerDictionaryInitialization_libCbmBmonSim_Impl() {
    static const char* headers[] = {
"CbmBmonDigitize.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/bmon",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/bmon",
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
"/home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/bmon/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmBmonSim dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmBmonDigi.h")))  __attribute__((annotate("$clingAutoload$CbmBmonDigitize.h")))  CbmBmonDigi;
template <class Digi> class __attribute__((annotate("$clingAutoload$CbmDigitize.h")))  __attribute__((annotate("$clingAutoload$CbmBmonDigitize.h")))  CbmDigitize;

class __attribute__((annotate("$clingAutoload$CbmBmonDigitize.h")))  CbmBmonDigitize;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmBmonSim dictionary payload"

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
#include "CbmBmonDigitize.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmBmonDigitize", payloadCode, "@",
"CbmDigitize<CbmBmonDigi>", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmBmonSim",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmBmonSim_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmBmonSim_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmBmonSim() {
  TriggerDictionaryInitialization_libCbmBmonSim_Impl();
}
