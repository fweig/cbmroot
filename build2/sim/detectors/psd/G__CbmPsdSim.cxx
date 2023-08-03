// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmPsdSim
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
#include "CbmPsdMC.h"
#include "CbmPsdIdealDigitizer.h"
#include "CbmPsdSimpleDigitizer.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmPsdMC(void *p = nullptr);
   static void *newArray_CbmPsdMC(Long_t size, void *p);
   static void delete_CbmPsdMC(void *p);
   static void deleteArray_CbmPsdMC(void *p);
   static void destruct_CbmPsdMC(void *p);
   static void streamer_CbmPsdMC(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdMC*)
   {
      ::CbmPsdMC *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdMC >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdMC", ::CbmPsdMC::Class_Version(), "CbmPsdMC.h", 34,
                  typeid(::CbmPsdMC), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdMC::Dictionary, isa_proxy, 16,
                  sizeof(::CbmPsdMC) );
      instance.SetNew(&new_CbmPsdMC);
      instance.SetNewArray(&newArray_CbmPsdMC);
      instance.SetDelete(&delete_CbmPsdMC);
      instance.SetDeleteArray(&deleteArray_CbmPsdMC);
      instance.SetDestructor(&destruct_CbmPsdMC);
      instance.SetStreamerFunc(&streamer_CbmPsdMC);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdMC*)
   {
      return GenerateInitInstanceLocal((::CbmPsdMC*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdMC*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPsdIdealDigitizer(void *p = nullptr);
   static void *newArray_CbmPsdIdealDigitizer(Long_t size, void *p);
   static void delete_CbmPsdIdealDigitizer(void *p);
   static void deleteArray_CbmPsdIdealDigitizer(void *p);
   static void destruct_CbmPsdIdealDigitizer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdIdealDigitizer*)
   {
      ::CbmPsdIdealDigitizer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdIdealDigitizer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdIdealDigitizer", ::CbmPsdIdealDigitizer::Class_Version(), "CbmPsdIdealDigitizer.h", 30,
                  typeid(::CbmPsdIdealDigitizer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdIdealDigitizer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPsdIdealDigitizer) );
      instance.SetNew(&new_CbmPsdIdealDigitizer);
      instance.SetNewArray(&newArray_CbmPsdIdealDigitizer);
      instance.SetDelete(&delete_CbmPsdIdealDigitizer);
      instance.SetDeleteArray(&deleteArray_CbmPsdIdealDigitizer);
      instance.SetDestructor(&destruct_CbmPsdIdealDigitizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdIdealDigitizer*)
   {
      return GenerateInitInstanceLocal((::CbmPsdIdealDigitizer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdIdealDigitizer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigitizelECbmPsdDigigR_Dictionary();
   static void CbmDigitizelECbmPsdDigigR_TClassManip(TClass*);
   static void delete_CbmDigitizelECbmPsdDigigR(void *p);
   static void deleteArray_CbmDigitizelECbmPsdDigigR(void *p);
   static void destruct_CbmDigitizelECbmPsdDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigitize<CbmPsdDigi>*)
   {
      ::CbmDigitize<CbmPsdDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigitize<CbmPsdDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigitize<CbmPsdDigi>", ::CbmDigitize<CbmPsdDigi>::Class_Version(), "CbmDigitize.h", 44,
                  typeid(::CbmDigitize<CbmPsdDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigitizelECbmPsdDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigitize<CbmPsdDigi>) );
      instance.SetDelete(&delete_CbmDigitizelECbmPsdDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigitizelECbmPsdDigigR);
      instance.SetDestructor(&destruct_CbmDigitizelECbmPsdDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigitize<CbmPsdDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigitize<CbmPsdDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigitize<CbmPsdDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigitizelECbmPsdDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmPsdDigi>*)nullptr)->GetClass();
      CbmDigitizelECbmPsdDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigitizelECbmPsdDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPsdSimpleDigitizer(void *p = nullptr);
   static void *newArray_CbmPsdSimpleDigitizer(Long_t size, void *p);
   static void delete_CbmPsdSimpleDigitizer(void *p);
   static void deleteArray_CbmPsdSimpleDigitizer(void *p);
   static void destruct_CbmPsdSimpleDigitizer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdSimpleDigitizer*)
   {
      ::CbmPsdSimpleDigitizer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdSimpleDigitizer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdSimpleDigitizer", ::CbmPsdSimpleDigitizer::Class_Version(), "CbmPsdSimpleDigitizer.h", 28,
                  typeid(::CbmPsdSimpleDigitizer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdSimpleDigitizer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPsdSimpleDigitizer) );
      instance.SetNew(&new_CbmPsdSimpleDigitizer);
      instance.SetNewArray(&newArray_CbmPsdSimpleDigitizer);
      instance.SetDelete(&delete_CbmPsdSimpleDigitizer);
      instance.SetDeleteArray(&deleteArray_CbmPsdSimpleDigitizer);
      instance.SetDestructor(&destruct_CbmPsdSimpleDigitizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdSimpleDigitizer*)
   {
      return GenerateInitInstanceLocal((::CbmPsdSimpleDigitizer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdSimpleDigitizer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdMC::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdMC::Class_Name()
{
   return "CbmPsdMC";
}

//______________________________________________________________________________
const char *CbmPsdMC::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMC*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdMC::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMC*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdMC::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMC*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdMC::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMC*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdIdealDigitizer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdIdealDigitizer::Class_Name()
{
   return "CbmPsdIdealDigitizer";
}

//______________________________________________________________________________
const char *CbmPsdIdealDigitizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdIdealDigitizer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdIdealDigitizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdIdealDigitizer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdIdealDigitizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdIdealDigitizer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdIdealDigitizer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdIdealDigitizer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigitize<CbmPsdDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmPsdDigi>::Class_Name()
{
   return "CbmDigitize<CbmPsdDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmPsdDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmPsdDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigitize<CbmPsdDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmPsdDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmPsdDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmPsdDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmPsdDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmPsdDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdSimpleDigitizer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdSimpleDigitizer::Class_Name()
{
   return "CbmPsdSimpleDigitizer";
}

//______________________________________________________________________________
const char *CbmPsdSimpleDigitizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdSimpleDigitizer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdSimpleDigitizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdSimpleDigitizer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdSimpleDigitizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdSimpleDigitizer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdSimpleDigitizer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdSimpleDigitizer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmPsdMC::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdMC.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairDetector::Streamer(R__b);
      R__b >> fPosX;
      R__b >> fPosZ;
      R__b >> fRotY;
      R__b >> fUserPlacement;
      R__b.CheckByteCount(R__s, R__c, CbmPsdMC::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmPsdMC::IsA(), kTRUE);
      FairDetector::Streamer(R__b);
      R__b << fPosX;
      R__b << fPosZ;
      R__b << fRotY;
      R__b << fUserPlacement;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdMC(void *p) {
      return  p ? new(p) ::CbmPsdMC : new ::CbmPsdMC;
   }
   static void *newArray_CbmPsdMC(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdMC[nElements] : new ::CbmPsdMC[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdMC(void *p) {
      delete ((::CbmPsdMC*)p);
   }
   static void deleteArray_CbmPsdMC(void *p) {
      delete [] ((::CbmPsdMC*)p);
   }
   static void destruct_CbmPsdMC(void *p) {
      typedef ::CbmPsdMC current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmPsdMC(TBuffer &buf, void *obj) {
      ((::CbmPsdMC*)obj)->::CbmPsdMC::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmPsdMC

//______________________________________________________________________________
void CbmPsdIdealDigitizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdIdealDigitizer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPsdIdealDigitizer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPsdIdealDigitizer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdIdealDigitizer(void *p) {
      return  p ? new(p) ::CbmPsdIdealDigitizer : new ::CbmPsdIdealDigitizer;
   }
   static void *newArray_CbmPsdIdealDigitizer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdIdealDigitizer[nElements] : new ::CbmPsdIdealDigitizer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdIdealDigitizer(void *p) {
      delete ((::CbmPsdIdealDigitizer*)p);
   }
   static void deleteArray_CbmPsdIdealDigitizer(void *p) {
      delete [] ((::CbmPsdIdealDigitizer*)p);
   }
   static void destruct_CbmPsdIdealDigitizer(void *p) {
      typedef ::CbmPsdIdealDigitizer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPsdIdealDigitizer

//______________________________________________________________________________
template <> void CbmDigitize<CbmPsdDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigitize<CbmPsdDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigitize<CbmPsdDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigitize<CbmPsdDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmDigitizelECbmPsdDigigR(void *p) {
      delete ((::CbmDigitize<CbmPsdDigi>*)p);
   }
   static void deleteArray_CbmDigitizelECbmPsdDigigR(void *p) {
      delete [] ((::CbmDigitize<CbmPsdDigi>*)p);
   }
   static void destruct_CbmDigitizelECbmPsdDigigR(void *p) {
      typedef ::CbmDigitize<CbmPsdDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigitize<CbmPsdDigi>

//______________________________________________________________________________
void CbmPsdSimpleDigitizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdSimpleDigitizer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPsdSimpleDigitizer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPsdSimpleDigitizer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdSimpleDigitizer(void *p) {
      return  p ? new(p) ::CbmPsdSimpleDigitizer : new ::CbmPsdSimpleDigitizer;
   }
   static void *newArray_CbmPsdSimpleDigitizer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdSimpleDigitizer[nElements] : new ::CbmPsdSimpleDigitizer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdSimpleDigitizer(void *p) {
      delete ((::CbmPsdSimpleDigitizer*)p);
   }
   static void deleteArray_CbmPsdSimpleDigitizer(void *p) {
      delete [] ((::CbmPsdSimpleDigitizer*)p);
   }
   static void destruct_CbmPsdSimpleDigitizer(void *p) {
      typedef ::CbmPsdSimpleDigitizer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPsdSimpleDigitizer

namespace {
  void TriggerDictionaryInitialization_libCbmPsdSim_Impl() {
    static const char* headers[] = {
"CbmPsdMC.h",
"CbmPsdIdealDigitizer.h",
"CbmPsdSimpleDigitizer.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/psd",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/psd",
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
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/psd/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmPsdSim dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmPsdMC.h")))  CbmPsdMC;
class __attribute__((annotate("$clingAutoload$CbmPsdIdealDigitizer.h")))  CbmPsdIdealDigitizer;
class __attribute__((annotate("$clingAutoload$CbmPsdDigi.h")))  __attribute__((annotate("$clingAutoload$CbmPsdSimpleDigitizer.h")))  CbmPsdDigi;
template <class Digi> class __attribute__((annotate("$clingAutoload$CbmDigitize.h")))  __attribute__((annotate("$clingAutoload$CbmPsdSimpleDigitizer.h")))  CbmDigitize;

class __attribute__((annotate("$clingAutoload$CbmPsdSimpleDigitizer.h")))  CbmPsdSimpleDigitizer;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmPsdSim dictionary payload"

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
#include "CbmPsdMC.h"
#include "CbmPsdIdealDigitizer.h"
#include "CbmPsdSimpleDigitizer.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmDigitize<CbmPsdDigi>", payloadCode, "@",
"CbmPsdIdealDigitizer", payloadCode, "@",
"CbmPsdMC", payloadCode, "@",
"CbmPsdSimpleDigitizer", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmPsdSim",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmPsdSim_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmPsdSim_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmPsdSim() {
  TriggerDictionaryInitialization_libCbmPsdSim_Impl();
}
