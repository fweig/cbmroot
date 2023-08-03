// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__NicaCbmHelper
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
#include "CbmDetectorID.h"
#include "NicaCbmFirstTask.h"
#include "CbmHelix.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace NicaCbmDetectorID {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *NicaCbmDetectorID_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("NicaCbmDetectorID", 0 /*version*/, "CbmDetectorID.h", 18,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &NicaCbmDetectorID_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *NicaCbmDetectorID_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_CbmHelix(void *p = nullptr);
   static void *newArray_CbmHelix(Long_t size, void *p);
   static void delete_CbmHelix(void *p);
   static void deleteArray_CbmHelix(void *p);
   static void destruct_CbmHelix(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmHelix*)
   {
      ::CbmHelix *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmHelix >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmHelix", ::CbmHelix::Class_Version(), "CbmHelix.h", 30,
                  typeid(::CbmHelix), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmHelix::Dictionary, isa_proxy, 4,
                  sizeof(::CbmHelix) );
      instance.SetNew(&new_CbmHelix);
      instance.SetNewArray(&newArray_CbmHelix);
      instance.SetDelete(&delete_CbmHelix);
      instance.SetDeleteArray(&deleteArray_CbmHelix);
      instance.SetDestructor(&destruct_CbmHelix);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmHelix*)
   {
      return GenerateInitInstanceLocal((::CbmHelix*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmHelix*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCbmFirstTask(void *p = nullptr);
   static void *newArray_NicaCbmFirstTask(Long_t size, void *p);
   static void delete_NicaCbmFirstTask(void *p);
   static void deleteArray_NicaCbmFirstTask(void *p);
   static void destruct_NicaCbmFirstTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCbmFirstTask*)
   {
      ::NicaCbmFirstTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCbmFirstTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCbmFirstTask", ::NicaCbmFirstTask::Class_Version(), "NicaCbmFirstTask.h", 20,
                  typeid(::NicaCbmFirstTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCbmFirstTask::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCbmFirstTask) );
      instance.SetNew(&new_NicaCbmFirstTask);
      instance.SetNewArray(&newArray_NicaCbmFirstTask);
      instance.SetDelete(&delete_NicaCbmFirstTask);
      instance.SetDeleteArray(&deleteArray_NicaCbmFirstTask);
      instance.SetDestructor(&destruct_NicaCbmFirstTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCbmFirstTask*)
   {
      return GenerateInitInstanceLocal((::NicaCbmFirstTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCbmFirstTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmHelix::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmHelix::Class_Name()
{
   return "CbmHelix";
}

//______________________________________________________________________________
const char *CbmHelix::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHelix*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmHelix::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHelix*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmHelix::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHelix*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmHelix::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHelix*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCbmFirstTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCbmFirstTask::Class_Name()
{
   return "NicaCbmFirstTask";
}

//______________________________________________________________________________
const char *NicaCbmFirstTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmFirstTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCbmFirstTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmFirstTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCbmFirstTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmFirstTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCbmFirstTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmFirstTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmHelix::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmHelix.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmHelix::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmHelix::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmHelix(void *p) {
      return  p ? new(p) ::CbmHelix : new ::CbmHelix;
   }
   static void *newArray_CbmHelix(Long_t nElements, void *p) {
      return p ? new(p) ::CbmHelix[nElements] : new ::CbmHelix[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmHelix(void *p) {
      delete ((::CbmHelix*)p);
   }
   static void deleteArray_CbmHelix(void *p) {
      delete [] ((::CbmHelix*)p);
   }
   static void destruct_CbmHelix(void *p) {
      typedef ::CbmHelix current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmHelix

//______________________________________________________________________________
void NicaCbmFirstTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCbmFirstTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCbmFirstTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCbmFirstTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCbmFirstTask(void *p) {
      return  p ? new(p) ::NicaCbmFirstTask : new ::NicaCbmFirstTask;
   }
   static void *newArray_NicaCbmFirstTask(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCbmFirstTask[nElements] : new ::NicaCbmFirstTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCbmFirstTask(void *p) {
      delete ((::NicaCbmFirstTask*)p);
   }
   static void deleteArray_NicaCbmFirstTask(void *p) {
      delete [] ((::NicaCbmFirstTask*)p);
   }
   static void destruct_NicaCbmFirstTask(void *p) {
      typedef ::NicaCbmFirstTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCbmFirstTask

namespace {
  void TriggerDictionaryInitialization_libNicaCbmHelper_Impl() {
    static const char* headers[] = {
"CbmDetectorID.h",
"NicaCbmFirstTask.h",
"CbmHelix.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/helpers",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/helpers",
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
"/home/heinemann/gpuunpacker/cbmroot/build2/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/PWGC2F/femtoscopy/nicafemto/helpers/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libNicaCbmHelper dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmHelix.h")))  __attribute__((annotate("$clingAutoload$NicaCbmFirstTask.h")))  CbmHelix;
class __attribute__((annotate("$clingAutoload$NicaCbmFirstTask.h")))  NicaCbmFirstTask;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libNicaCbmHelper dictionary payload"

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
#include "CbmDetectorID.h"
#include "NicaCbmFirstTask.h"
#include "CbmHelix.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmHelix", payloadCode, "@",
"NicaCbmFirstTask", payloadCode, "@",
"NicaCbmSetup", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libNicaCbmHelper",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libNicaCbmHelper_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libNicaCbmHelper_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libNicaCbmHelper() {
  TriggerDictionaryInitialization_libNicaCbmHelper_Impl();
}
