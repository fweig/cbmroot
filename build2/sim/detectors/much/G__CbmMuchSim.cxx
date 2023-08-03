// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmMuchSim
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
#include "CbmGeoMuch.h"
#include "CbmMuch.h"
#include "CbmMuchDigitizeGem.h"
#include "CbmMuchReadoutBuffer.h"
#include "CbmMuchSignal.h"
#include "CbmMuchTransportQa.h"
#include "CbmMuchDigitizerQa.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmGeoMuch(void *p = nullptr);
   static void *newArray_CbmGeoMuch(Long_t size, void *p);
   static void delete_CbmGeoMuch(void *p);
   static void deleteArray_CbmGeoMuch(void *p);
   static void destruct_CbmGeoMuch(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGeoMuch*)
   {
      ::CbmGeoMuch *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGeoMuch >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGeoMuch", ::CbmGeoMuch::Class_Version(), "CbmGeoMuch.h", 20,
                  typeid(::CbmGeoMuch), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGeoMuch::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGeoMuch) );
      instance.SetNew(&new_CbmGeoMuch);
      instance.SetNewArray(&newArray_CbmGeoMuch);
      instance.SetDelete(&delete_CbmGeoMuch);
      instance.SetDeleteArray(&deleteArray_CbmGeoMuch);
      instance.SetDestructor(&destruct_CbmGeoMuch);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGeoMuch*)
   {
      return GenerateInitInstanceLocal((::CbmGeoMuch*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGeoMuch*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuch(void *p = nullptr);
   static void *newArray_CbmMuch(Long_t size, void *p);
   static void delete_CbmMuch(void *p);
   static void deleteArray_CbmMuch(void *p);
   static void destruct_CbmMuch(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuch*)
   {
      ::CbmMuch *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuch >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuch", ::CbmMuch::Class_Version(), "CbmMuch.h", 33,
                  typeid(::CbmMuch), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuch::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuch) );
      instance.SetNew(&new_CbmMuch);
      instance.SetNewArray(&newArray_CbmMuch);
      instance.SetDelete(&delete_CbmMuch);
      instance.SetDeleteArray(&deleteArray_CbmMuch);
      instance.SetDestructor(&destruct_CbmMuch);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuch*)
   {
      return GenerateInitInstanceLocal((::CbmMuch*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuch*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigitizelECbmMuchDigigR_Dictionary();
   static void CbmDigitizelECbmMuchDigigR_TClassManip(TClass*);
   static void delete_CbmDigitizelECbmMuchDigigR(void *p);
   static void deleteArray_CbmDigitizelECbmMuchDigigR(void *p);
   static void destruct_CbmDigitizelECbmMuchDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigitize<CbmMuchDigi>*)
   {
      ::CbmDigitize<CbmMuchDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigitize<CbmMuchDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigitize<CbmMuchDigi>", ::CbmDigitize<CbmMuchDigi>::Class_Version(), "CbmDigitize.h", 44,
                  typeid(::CbmDigitize<CbmMuchDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigitizelECbmMuchDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigitize<CbmMuchDigi>) );
      instance.SetDelete(&delete_CbmDigitizelECbmMuchDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigitizelECbmMuchDigigR);
      instance.SetDestructor(&destruct_CbmDigitizelECbmMuchDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigitize<CbmMuchDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigitize<CbmMuchDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigitize<CbmMuchDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigitizelECbmMuchDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmMuchDigi>*)nullptr)->GetClass();
      CbmDigitizelECbmMuchDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigitizelECbmMuchDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchSignal(void *p = nullptr);
   static void *newArray_CbmMuchSignal(Long_t size, void *p);
   static void delete_CbmMuchSignal(void *p);
   static void deleteArray_CbmMuchSignal(void *p);
   static void destruct_CbmMuchSignal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchSignal*)
   {
      ::CbmMuchSignal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchSignal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchSignal", ::CbmMuchSignal::Class_Version(), "CbmMuchSignal.h", 36,
                  typeid(::CbmMuchSignal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchSignal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchSignal) );
      instance.SetNew(&new_CbmMuchSignal);
      instance.SetNewArray(&newArray_CbmMuchSignal);
      instance.SetDelete(&delete_CbmMuchSignal);
      instance.SetDeleteArray(&deleteArray_CbmMuchSignal);
      instance.SetDestructor(&destruct_CbmMuchSignal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchSignal*)
   {
      return GenerateInitInstanceLocal((::CbmMuchSignal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchSignal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchDigitizeGem(void *p = nullptr);
   static void *newArray_CbmMuchDigitizeGem(Long_t size, void *p);
   static void delete_CbmMuchDigitizeGem(void *p);
   static void deleteArray_CbmMuchDigitizeGem(void *p);
   static void destruct_CbmMuchDigitizeGem(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchDigitizeGem*)
   {
      ::CbmMuchDigitizeGem *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchDigitizeGem >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchDigitizeGem", ::CbmMuchDigitizeGem::Class_Version(), "CbmMuchDigitizeGem.h", 66,
                  typeid(::CbmMuchDigitizeGem), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchDigitizeGem::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchDigitizeGem) );
      instance.SetNew(&new_CbmMuchDigitizeGem);
      instance.SetNewArray(&newArray_CbmMuchDigitizeGem);
      instance.SetDelete(&delete_CbmMuchDigitizeGem);
      instance.SetDeleteArray(&deleteArray_CbmMuchDigitizeGem);
      instance.SetDestructor(&destruct_CbmMuchDigitizeGem);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchDigitizeGem*)
   {
      return GenerateInitInstanceLocal((::CbmMuchDigitizeGem*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchDigitizeGem*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchReadoutBuffer(void *p = nullptr);
   static void *newArray_CbmMuchReadoutBuffer(Long_t size, void *p);
   static void delete_CbmMuchReadoutBuffer(void *p);
   static void deleteArray_CbmMuchReadoutBuffer(void *p);
   static void destruct_CbmMuchReadoutBuffer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchReadoutBuffer*)
   {
      ::CbmMuchReadoutBuffer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchReadoutBuffer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchReadoutBuffer", ::CbmMuchReadoutBuffer::Class_Version(), "CbmMuchReadoutBuffer.h", 27,
                  typeid(::CbmMuchReadoutBuffer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchReadoutBuffer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchReadoutBuffer) );
      instance.SetNew(&new_CbmMuchReadoutBuffer);
      instance.SetNewArray(&newArray_CbmMuchReadoutBuffer);
      instance.SetDelete(&delete_CbmMuchReadoutBuffer);
      instance.SetDeleteArray(&deleteArray_CbmMuchReadoutBuffer);
      instance.SetDestructor(&destruct_CbmMuchReadoutBuffer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchReadoutBuffer*)
   {
      return GenerateInitInstanceLocal((::CbmMuchReadoutBuffer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchReadoutBuffer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchTransportQa(void *p = nullptr);
   static void *newArray_CbmMuchTransportQa(Long_t size, void *p);
   static void delete_CbmMuchTransportQa(void *p);
   static void deleteArray_CbmMuchTransportQa(void *p);
   static void destruct_CbmMuchTransportQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchTransportQa*)
   {
      ::CbmMuchTransportQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchTransportQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchTransportQa", ::CbmMuchTransportQa::Class_Version(), "CbmMuchTransportQa.h", 39,
                  typeid(::CbmMuchTransportQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchTransportQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchTransportQa) );
      instance.SetNew(&new_CbmMuchTransportQa);
      instance.SetNewArray(&newArray_CbmMuchTransportQa);
      instance.SetDelete(&delete_CbmMuchTransportQa);
      instance.SetDeleteArray(&deleteArray_CbmMuchTransportQa);
      instance.SetDestructor(&destruct_CbmMuchTransportQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchTransportQa*)
   {
      return GenerateInitInstanceLocal((::CbmMuchTransportQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchTransportQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchDigitizerQa(void *p = nullptr);
   static void *newArray_CbmMuchDigitizerQa(Long_t size, void *p);
   static void delete_CbmMuchDigitizerQa(void *p);
   static void deleteArray_CbmMuchDigitizerQa(void *p);
   static void destruct_CbmMuchDigitizerQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchDigitizerQa*)
   {
      ::CbmMuchDigitizerQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchDigitizerQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchDigitizerQa", ::CbmMuchDigitizerQa::Class_Version(), "CbmMuchDigitizerQa.h", 44,
                  typeid(::CbmMuchDigitizerQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchDigitizerQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchDigitizerQa) );
      instance.SetNew(&new_CbmMuchDigitizerQa);
      instance.SetNewArray(&newArray_CbmMuchDigitizerQa);
      instance.SetDelete(&delete_CbmMuchDigitizerQa);
      instance.SetDeleteArray(&deleteArray_CbmMuchDigitizerQa);
      instance.SetDestructor(&destruct_CbmMuchDigitizerQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchDigitizerQa*)
   {
      return GenerateInitInstanceLocal((::CbmMuchDigitizerQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchDigitizerQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmGeoMuch::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGeoMuch::Class_Name()
{
   return "CbmGeoMuch";
}

//______________________________________________________________________________
const char *CbmGeoMuch::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoMuch*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGeoMuch::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoMuch*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGeoMuch::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoMuch*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGeoMuch::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoMuch*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuch::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuch::Class_Name()
{
   return "CbmMuch";
}

//______________________________________________________________________________
const char *CbmMuch::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuch*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuch::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuch*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuch::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuch*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuch::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuch*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigitize<CbmMuchDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmMuchDigi>::Class_Name()
{
   return "CbmDigitize<CbmMuchDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmMuchDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmMuchDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigitize<CbmMuchDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmMuchDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmMuchDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmMuchDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmMuchDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmMuchDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchSignal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchSignal::Class_Name()
{
   return "CbmMuchSignal";
}

//______________________________________________________________________________
const char *CbmMuchSignal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSignal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchSignal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSignal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchSignal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSignal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchSignal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSignal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchDigitizeGem::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchDigitizeGem::Class_Name()
{
   return "CbmMuchDigitizeGem";
}

//______________________________________________________________________________
const char *CbmMuchDigitizeGem::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigitizeGem*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchDigitizeGem::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigitizeGem*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchDigitizeGem::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigitizeGem*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchDigitizeGem::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigitizeGem*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchReadoutBuffer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchReadoutBuffer::Class_Name()
{
   return "CbmMuchReadoutBuffer";
}

//______________________________________________________________________________
const char *CbmMuchReadoutBuffer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchReadoutBuffer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchReadoutBuffer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchReadoutBuffer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchReadoutBuffer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchReadoutBuffer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchReadoutBuffer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchReadoutBuffer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchTransportQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchTransportQa::Class_Name()
{
   return "CbmMuchTransportQa";
}

//______________________________________________________________________________
const char *CbmMuchTransportQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTransportQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchTransportQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTransportQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchTransportQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTransportQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchTransportQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTransportQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchDigitizerQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchDigitizerQa::Class_Name()
{
   return "CbmMuchDigitizerQa";
}

//______________________________________________________________________________
const char *CbmMuchDigitizerQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigitizerQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchDigitizerQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigitizerQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchDigitizerQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigitizerQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchDigitizerQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigitizerQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmGeoMuch::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGeoMuch.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGeoMuch::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGeoMuch::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGeoMuch(void *p) {
      return  p ? new(p) ::CbmGeoMuch : new ::CbmGeoMuch;
   }
   static void *newArray_CbmGeoMuch(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGeoMuch[nElements] : new ::CbmGeoMuch[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGeoMuch(void *p) {
      delete ((::CbmGeoMuch*)p);
   }
   static void deleteArray_CbmGeoMuch(void *p) {
      delete [] ((::CbmGeoMuch*)p);
   }
   static void destruct_CbmGeoMuch(void *p) {
      typedef ::CbmGeoMuch current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGeoMuch

//______________________________________________________________________________
void CbmMuch::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuch.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuch::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuch::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuch(void *p) {
      return  p ? new(p) ::CbmMuch : new ::CbmMuch;
   }
   static void *newArray_CbmMuch(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuch[nElements] : new ::CbmMuch[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuch(void *p) {
      delete ((::CbmMuch*)p);
   }
   static void deleteArray_CbmMuch(void *p) {
      delete [] ((::CbmMuch*)p);
   }
   static void destruct_CbmMuch(void *p) {
      typedef ::CbmMuch current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuch

//______________________________________________________________________________
template <> void CbmDigitize<CbmMuchDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigitize<CbmMuchDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigitize<CbmMuchDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigitize<CbmMuchDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmDigitizelECbmMuchDigigR(void *p) {
      delete ((::CbmDigitize<CbmMuchDigi>*)p);
   }
   static void deleteArray_CbmDigitizelECbmMuchDigigR(void *p) {
      delete [] ((::CbmDigitize<CbmMuchDigi>*)p);
   }
   static void destruct_CbmDigitizelECbmMuchDigigR(void *p) {
      typedef ::CbmDigitize<CbmMuchDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigitize<CbmMuchDigi>

//______________________________________________________________________________
void CbmMuchSignal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchSignal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchSignal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchSignal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchSignal(void *p) {
      return  p ? new(p) ::CbmMuchSignal : new ::CbmMuchSignal;
   }
   static void *newArray_CbmMuchSignal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchSignal[nElements] : new ::CbmMuchSignal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchSignal(void *p) {
      delete ((::CbmMuchSignal*)p);
   }
   static void deleteArray_CbmMuchSignal(void *p) {
      delete [] ((::CbmMuchSignal*)p);
   }
   static void destruct_CbmMuchSignal(void *p) {
      typedef ::CbmMuchSignal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchSignal

//______________________________________________________________________________
void CbmMuchDigitizeGem::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchDigitizeGem.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchDigitizeGem::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchDigitizeGem::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchDigitizeGem(void *p) {
      return  p ? new(p) ::CbmMuchDigitizeGem : new ::CbmMuchDigitizeGem;
   }
   static void *newArray_CbmMuchDigitizeGem(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchDigitizeGem[nElements] : new ::CbmMuchDigitizeGem[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchDigitizeGem(void *p) {
      delete ((::CbmMuchDigitizeGem*)p);
   }
   static void deleteArray_CbmMuchDigitizeGem(void *p) {
      delete [] ((::CbmMuchDigitizeGem*)p);
   }
   static void destruct_CbmMuchDigitizeGem(void *p) {
      typedef ::CbmMuchDigitizeGem current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchDigitizeGem

//______________________________________________________________________________
void CbmMuchReadoutBuffer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchReadoutBuffer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchReadoutBuffer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchReadoutBuffer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchReadoutBuffer(void *p) {
      return  p ? new(p) ::CbmMuchReadoutBuffer : new ::CbmMuchReadoutBuffer;
   }
   static void *newArray_CbmMuchReadoutBuffer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchReadoutBuffer[nElements] : new ::CbmMuchReadoutBuffer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchReadoutBuffer(void *p) {
      delete ((::CbmMuchReadoutBuffer*)p);
   }
   static void deleteArray_CbmMuchReadoutBuffer(void *p) {
      delete [] ((::CbmMuchReadoutBuffer*)p);
   }
   static void destruct_CbmMuchReadoutBuffer(void *p) {
      typedef ::CbmMuchReadoutBuffer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchReadoutBuffer

//______________________________________________________________________________
void CbmMuchTransportQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchTransportQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchTransportQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchTransportQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchTransportQa(void *p) {
      return  p ? new(p) ::CbmMuchTransportQa : new ::CbmMuchTransportQa;
   }
   static void *newArray_CbmMuchTransportQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchTransportQa[nElements] : new ::CbmMuchTransportQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchTransportQa(void *p) {
      delete ((::CbmMuchTransportQa*)p);
   }
   static void deleteArray_CbmMuchTransportQa(void *p) {
      delete [] ((::CbmMuchTransportQa*)p);
   }
   static void destruct_CbmMuchTransportQa(void *p) {
      typedef ::CbmMuchTransportQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchTransportQa

//______________________________________________________________________________
void CbmMuchDigitizerQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchDigitizerQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchDigitizerQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchDigitizerQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchDigitizerQa(void *p) {
      return  p ? new(p) ::CbmMuchDigitizerQa : new ::CbmMuchDigitizerQa;
   }
   static void *newArray_CbmMuchDigitizerQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchDigitizerQa[nElements] : new ::CbmMuchDigitizerQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchDigitizerQa(void *p) {
      delete ((::CbmMuchDigitizerQa*)p);
   }
   static void deleteArray_CbmMuchDigitizerQa(void *p) {
      delete [] ((::CbmMuchDigitizerQa*)p);
   }
   static void destruct_CbmMuchDigitizerQa(void *p) {
      typedef ::CbmMuchDigitizerQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchDigitizerQa

namespace ROOT {
   static TClass *maplEunsignedsPintcOunsignedsPintgR_Dictionary();
   static void maplEunsignedsPintcOunsignedsPintgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOunsignedsPintgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOunsignedsPintgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOunsignedsPintgR(void *p);
   static void deleteArray_maplEunsignedsPintcOunsignedsPintgR(void *p);
   static void destruct_maplEunsignedsPintcOunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,unsigned int>*)
   {
      map<unsigned int,unsigned int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,unsigned int>", -2, "map", 100,
                  typeid(map<unsigned int,unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,unsigned int>) );
      instance.SetNew(&new_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,unsigned int> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,unsigned int>","std::map<unsigned int, unsigned int, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, unsigned int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,unsigned int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,unsigned int>*)nullptr)->GetClass();
      maplEunsignedsPintcOunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,unsigned int> : new map<unsigned int,unsigned int>;
   }
   static void *newArray_maplEunsignedsPintcOunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,unsigned int>[nElements] : new map<unsigned int,unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      delete ((map<unsigned int,unsigned int>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      delete [] ((map<unsigned int,unsigned int>*)p);
   }
   static void destruct_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      typedef map<unsigned int,unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,unsigned int>

namespace {
  void TriggerDictionaryInitialization_libCbmMuchSim_Impl() {
    static const char* headers[] = {
"CbmGeoMuch.h",
"CbmMuch.h",
"CbmMuchDigitizeGem.h",
"CbmMuchReadoutBuffer.h",
"CbmMuchSignal.h",
"CbmMuchTransportQa.h",
"CbmMuchDigitizerQa.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/much/qa",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/much/qa",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/core/qa/checker",
"/home/heinemann/gpuunpacker/cbmroot/core/qa",
"/home/heinemann/gpuunpacker/cbmroot/external/yaml-cpp/include",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/much/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmMuchSim dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate(R"ATTRDUMP(Class for MUID)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmGeoMuch.h")))  CbmGeoMuch;
class __attribute__((annotate("$clingAutoload$CbmMuch.h")))  CbmMuch;
class __attribute__((annotate("$clingAutoload$CbmMuchDigi.h")))  __attribute__((annotate("$clingAutoload$CbmMuchDigitizeGem.h")))  CbmMuchDigi;
template <class Digi> class __attribute__((annotate("$clingAutoload$CbmDigitize.h")))  __attribute__((annotate("$clingAutoload$CbmMuchDigitizeGem.h")))  CbmDigitize;

class __attribute__((annotate("$clingAutoload$CbmMuchSignal.h")))  __attribute__((annotate("$clingAutoload$CbmMuchDigitizeGem.h")))  CbmMuchSignal;
class __attribute__((annotate("$clingAutoload$CbmMuchDigitizeGem.h")))  CbmMuchDigitizeGem;
class __attribute__((annotate("$clingAutoload$CbmMuchReadoutBuffer.h")))  CbmMuchReadoutBuffer;
class __attribute__((annotate("$clingAutoload$CbmMuchTransportQa.h")))  CbmMuchTransportQa;
class __attribute__((annotate("$clingAutoload$CbmMuchDigitizerQa.h")))  CbmMuchDigitizerQa;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmMuchSim dictionary payload"

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
#include "CbmGeoMuch.h"
#include "CbmMuch.h"
#include "CbmMuchDigitizeGem.h"
#include "CbmMuchReadoutBuffer.h"
#include "CbmMuchSignal.h"
#include "CbmMuchTransportQa.h"
#include "CbmMuchDigitizerQa.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmDigitize<CbmMuchDigi>", payloadCode, "@",
"CbmGeoMuch", payloadCode, "@",
"CbmMuch", payloadCode, "@",
"CbmMuchDigitizeGem", payloadCode, "@",
"CbmMuchDigitizerQa", payloadCode, "@",
"CbmMuchReadoutBuffer", payloadCode, "@",
"CbmMuchSignal", payloadCode, "@",
"CbmMuchTransportQa", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmMuchSim",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmMuchSim_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmMuchSim_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmMuchSim() {
  TriggerDictionaryInitialization_libCbmMuchSim_Impl();
}
