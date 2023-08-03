// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmPsdBase
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
#include "PronyFitter.h"
#include "CbmPsdContFact.h"
#include "CbmMcbm2018PsdPar.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *PsdSignalFittingcLcLPronyFitter_Dictionary();
   static void PsdSignalFittingcLcLPronyFitter_TClassManip(TClass*);
   static void *new_PsdSignalFittingcLcLPronyFitter(void *p = nullptr);
   static void *newArray_PsdSignalFittingcLcLPronyFitter(Long_t size, void *p);
   static void delete_PsdSignalFittingcLcLPronyFitter(void *p);
   static void deleteArray_PsdSignalFittingcLcLPronyFitter(void *p);
   static void destruct_PsdSignalFittingcLcLPronyFitter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PsdSignalFitting::PronyFitter*)
   {
      ::PsdSignalFitting::PronyFitter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PsdSignalFitting::PronyFitter));
      static ::ROOT::TGenericClassInfo 
         instance("PsdSignalFitting::PronyFitter", "PronyFitter.h", 25,
                  typeid(::PsdSignalFitting::PronyFitter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PsdSignalFittingcLcLPronyFitter_Dictionary, isa_proxy, 0,
                  sizeof(::PsdSignalFitting::PronyFitter) );
      instance.SetNew(&new_PsdSignalFittingcLcLPronyFitter);
      instance.SetNewArray(&newArray_PsdSignalFittingcLcLPronyFitter);
      instance.SetDelete(&delete_PsdSignalFittingcLcLPronyFitter);
      instance.SetDeleteArray(&deleteArray_PsdSignalFittingcLcLPronyFitter);
      instance.SetDestructor(&destruct_PsdSignalFittingcLcLPronyFitter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PsdSignalFitting::PronyFitter*)
   {
      return GenerateInitInstanceLocal((::PsdSignalFitting::PronyFitter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PsdSignalFitting::PronyFitter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PsdSignalFittingcLcLPronyFitter_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PsdSignalFitting::PronyFitter*)nullptr)->GetClass();
      PsdSignalFittingcLcLPronyFitter_TClassManip(theClass);
   return theClass;
   }

   static void PsdSignalFittingcLcLPronyFitter_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPsdContFact(void *p = nullptr);
   static void *newArray_CbmPsdContFact(Long_t size, void *p);
   static void delete_CbmPsdContFact(void *p);
   static void deleteArray_CbmPsdContFact(void *p);
   static void destruct_CbmPsdContFact(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdContFact*)
   {
      ::CbmPsdContFact *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdContFact >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdContFact", ::CbmPsdContFact::Class_Version(), "CbmPsdContFact.h", 26,
                  typeid(::CbmPsdContFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdContFact::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPsdContFact) );
      instance.SetNew(&new_CbmPsdContFact);
      instance.SetNewArray(&newArray_CbmPsdContFact);
      instance.SetDelete(&delete_CbmPsdContFact);
      instance.SetDeleteArray(&deleteArray_CbmPsdContFact);
      instance.SetDestructor(&destruct_CbmPsdContFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdContFact*)
   {
      return GenerateInitInstanceLocal((::CbmPsdContFact*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdContFact*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018PsdPar(void *p = nullptr);
   static void *newArray_CbmMcbm2018PsdPar(Long_t size, void *p);
   static void delete_CbmMcbm2018PsdPar(void *p);
   static void deleteArray_CbmMcbm2018PsdPar(void *p);
   static void destruct_CbmMcbm2018PsdPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018PsdPar*)
   {
      ::CbmMcbm2018PsdPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018PsdPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018PsdPar", ::CbmMcbm2018PsdPar::Class_Version(), "CbmMcbm2018PsdPar.h", 23,
                  typeid(::CbmMcbm2018PsdPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018PsdPar::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2018PsdPar) );
      instance.SetNew(&new_CbmMcbm2018PsdPar);
      instance.SetNewArray(&newArray_CbmMcbm2018PsdPar);
      instance.SetDelete(&delete_CbmMcbm2018PsdPar);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018PsdPar);
      instance.SetDestructor(&destruct_CbmMcbm2018PsdPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018PsdPar*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018PsdPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018PsdPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdContFact::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdContFact::Class_Name()
{
   return "CbmPsdContFact";
}

//______________________________________________________________________________
const char *CbmPsdContFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdContFact*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdContFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdContFact*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdContFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdContFact*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdContFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdContFact*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018PsdPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018PsdPar::Class_Name()
{
   return "CbmMcbm2018PsdPar";
}

//______________________________________________________________________________
const char *CbmMcbm2018PsdPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018PsdPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018PsdPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018PsdPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018PsdPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018PsdPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018PsdPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018PsdPar*)nullptr)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PsdSignalFittingcLcLPronyFitter(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PsdSignalFitting::PronyFitter : new ::PsdSignalFitting::PronyFitter;
   }
   static void *newArray_PsdSignalFittingcLcLPronyFitter(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PsdSignalFitting::PronyFitter[nElements] : new ::PsdSignalFitting::PronyFitter[nElements];
   }
   // Wrapper around operator delete
   static void delete_PsdSignalFittingcLcLPronyFitter(void *p) {
      delete ((::PsdSignalFitting::PronyFitter*)p);
   }
   static void deleteArray_PsdSignalFittingcLcLPronyFitter(void *p) {
      delete [] ((::PsdSignalFitting::PronyFitter*)p);
   }
   static void destruct_PsdSignalFittingcLcLPronyFitter(void *p) {
      typedef ::PsdSignalFitting::PronyFitter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PsdSignalFitting::PronyFitter

//______________________________________________________________________________
void CbmPsdContFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdContFact.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPsdContFact::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPsdContFact::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdContFact(void *p) {
      return  p ? new(p) ::CbmPsdContFact : new ::CbmPsdContFact;
   }
   static void *newArray_CbmPsdContFact(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdContFact[nElements] : new ::CbmPsdContFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdContFact(void *p) {
      delete ((::CbmPsdContFact*)p);
   }
   static void deleteArray_CbmPsdContFact(void *p) {
      delete [] ((::CbmPsdContFact*)p);
   }
   static void destruct_CbmPsdContFact(void *p) {
      typedef ::CbmPsdContFact current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPsdContFact

//______________________________________________________________________________
void CbmMcbm2018PsdPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018PsdPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2018PsdPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2018PsdPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018PsdPar(void *p) {
      return  p ? new(p) ::CbmMcbm2018PsdPar : new ::CbmMcbm2018PsdPar;
   }
   static void *newArray_CbmMcbm2018PsdPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018PsdPar[nElements] : new ::CbmMcbm2018PsdPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018PsdPar(void *p) {
      delete ((::CbmMcbm2018PsdPar*)p);
   }
   static void deleteArray_CbmMcbm2018PsdPar(void *p) {
      delete [] ((::CbmMcbm2018PsdPar*)p);
   }
   static void destruct_CbmMcbm2018PsdPar(void *p) {
      typedef ::CbmMcbm2018PsdPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2018PsdPar

namespace {
  void TriggerDictionaryInitialization_libCbmPsdBase_Impl() {
    static const char* headers[] = {
"PronyFitter.h",
"CbmPsdContFact.h",
"CbmMcbm2018PsdPar.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/psd",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/psd",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/core/detectors/psd/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmPsdBase dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace PsdSignalFitting{class __attribute__((annotate("$clingAutoload$PronyFitter.h")))  PronyFitter;}
class __attribute__((annotate(R"ATTRDUMP(Factory for all Psd parameter containers)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmPsdContFact.h")))  CbmPsdContFact;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018PsdPar.h")))  CbmMcbm2018PsdPar;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmPsdBase dictionary payload"

#ifndef FMT_LOCALE
  #define FMT_LOCALE 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "PronyFitter.h"
#include "CbmPsdContFact.h"
#include "CbmMcbm2018PsdPar.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmMcbm2018PsdPar", payloadCode, "@",
"CbmPsdContFact", payloadCode, "@",
"PsdSignalFitting::PronyFitter", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmPsdBase",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmPsdBase_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmPsdBase_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmPsdBase() {
  TriggerDictionaryInitialization_libCbmPsdBase_Impl();
}
