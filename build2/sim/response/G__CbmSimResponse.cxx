// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmSimResponse
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
#include "CbmDigitization.h"
#include "CbmDigitizationSource.h"
#include "CbmDigitizeInfo.h"
#include "CbmMCInput.h"
#include "CbmMCInputSet.h"
#include "CbmRunAna.h"
#include "CbmDigitizationConfig.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmMCInput(void *p = nullptr);
   static void *newArray_CbmMCInput(Long_t size, void *p);
   static void delete_CbmMCInput(void *p);
   static void deleteArray_CbmMCInput(void *p);
   static void destruct_CbmMCInput(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMCInput*)
   {
      ::CbmMCInput *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMCInput >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMCInput", ::CbmMCInput::Class_Version(), "CbmMCInput.h", 33,
                  typeid(::CbmMCInput), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMCInput::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMCInput) );
      instance.SetNew(&new_CbmMCInput);
      instance.SetNewArray(&newArray_CbmMCInput);
      instance.SetDelete(&delete_CbmMCInput);
      instance.SetDeleteArray(&deleteArray_CbmMCInput);
      instance.SetDestructor(&destruct_CbmMCInput);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMCInput*)
   {
      return GenerateInitInstanceLocal((::CbmMCInput*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMCInput*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMCInputSet(void *p = nullptr);
   static void *newArray_CbmMCInputSet(Long_t size, void *p);
   static void delete_CbmMCInputSet(void *p);
   static void deleteArray_CbmMCInputSet(void *p);
   static void destruct_CbmMCInputSet(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMCInputSet*)
   {
      ::CbmMCInputSet *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMCInputSet >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMCInputSet", ::CbmMCInputSet::Class_Version(), "CbmMCInputSet.h", 40,
                  typeid(::CbmMCInputSet), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMCInputSet::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMCInputSet) );
      instance.SetNew(&new_CbmMCInputSet);
      instance.SetNewArray(&newArray_CbmMCInputSet);
      instance.SetDelete(&delete_CbmMCInputSet);
      instance.SetDeleteArray(&deleteArray_CbmMCInputSet);
      instance.SetDestructor(&destruct_CbmMCInputSet);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMCInputSet*)
   {
      return GenerateInitInstanceLocal((::CbmMCInputSet*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMCInputSet*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmDigitizationSource(void *p = nullptr);
   static void *newArray_CbmDigitizationSource(Long_t size, void *p);
   static void delete_CbmDigitizationSource(void *p);
   static void deleteArray_CbmDigitizationSource(void *p);
   static void destruct_CbmDigitizationSource(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigitizationSource*)
   {
      ::CbmDigitizationSource *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigitizationSource >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigitizationSource", ::CbmDigitizationSource::Class_Version(), "CbmDigitizationSource.h", 49,
                  typeid(::CbmDigitizationSource), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmDigitizationSource::Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigitizationSource) );
      instance.SetNew(&new_CbmDigitizationSource);
      instance.SetNewArray(&newArray_CbmDigitizationSource);
      instance.SetDelete(&delete_CbmDigitizationSource);
      instance.SetDeleteArray(&deleteArray_CbmDigitizationSource);
      instance.SetDestructor(&destruct_CbmDigitizationSource);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigitizationSource*)
   {
      return GenerateInitInstanceLocal((::CbmDigitizationSource*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigitizationSource*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmDigitizeInfo(void *p);
   static void deleteArray_CbmDigitizeInfo(void *p);
   static void destruct_CbmDigitizeInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigitizeInfo*)
   {
      ::CbmDigitizeInfo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigitizeInfo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigitizeInfo", ::CbmDigitizeInfo::Class_Version(), "CbmDigitizeInfo.h", 28,
                  typeid(::CbmDigitizeInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmDigitizeInfo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigitizeInfo) );
      instance.SetDelete(&delete_CbmDigitizeInfo);
      instance.SetDeleteArray(&deleteArray_CbmDigitizeInfo);
      instance.SetDestructor(&destruct_CbmDigitizeInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigitizeInfo*)
   {
      return GenerateInitInstanceLocal((::CbmDigitizeInfo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigitizeInfo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmDigitization(void *p = nullptr);
   static void *newArray_CbmDigitization(Long_t size, void *p);
   static void delete_CbmDigitization(void *p);
   static void deleteArray_CbmDigitization(void *p);
   static void destruct_CbmDigitization(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigitization*)
   {
      ::CbmDigitization *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigitization >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigitization", ::CbmDigitization::Class_Version(), "CbmDigitization.h", 32,
                  typeid(::CbmDigitization), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmDigitization::Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigitization) );
      instance.SetNew(&new_CbmDigitization);
      instance.SetNewArray(&newArray_CbmDigitization);
      instance.SetDelete(&delete_CbmDigitization);
      instance.SetDeleteArray(&deleteArray_CbmDigitization);
      instance.SetDestructor(&destruct_CbmDigitization);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigitization*)
   {
      return GenerateInitInstanceLocal((::CbmDigitization*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigitization*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRunAna(void *p = nullptr);
   static void *newArray_CbmRunAna(Long_t size, void *p);
   static void delete_CbmRunAna(void *p);
   static void deleteArray_CbmRunAna(void *p);
   static void destruct_CbmRunAna(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRunAna*)
   {
      ::CbmRunAna *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRunAna >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRunAna", ::CbmRunAna::Class_Version(), "CbmRunAna.h", 23,
                  typeid(::CbmRunAna), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRunAna::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRunAna) );
      instance.SetNew(&new_CbmRunAna);
      instance.SetNewArray(&newArray_CbmRunAna);
      instance.SetDelete(&delete_CbmRunAna);
      instance.SetDeleteArray(&deleteArray_CbmRunAna);
      instance.SetDestructor(&destruct_CbmRunAna);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRunAna*)
   {
      return GenerateInitInstanceLocal((::CbmRunAna*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRunAna*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmDigitizationConfig(void *p = nullptr);
   static void *newArray_CbmDigitizationConfig(Long_t size, void *p);
   static void delete_CbmDigitizationConfig(void *p);
   static void deleteArray_CbmDigitizationConfig(void *p);
   static void destruct_CbmDigitizationConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigitizationConfig*)
   {
      ::CbmDigitizationConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigitizationConfig >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigitizationConfig", ::CbmDigitizationConfig::Class_Version(), "CbmDigitizationConfig.h", 11,
                  typeid(::CbmDigitizationConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmDigitizationConfig::Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigitizationConfig) );
      instance.SetNew(&new_CbmDigitizationConfig);
      instance.SetNewArray(&newArray_CbmDigitizationConfig);
      instance.SetDelete(&delete_CbmDigitizationConfig);
      instance.SetDeleteArray(&deleteArray_CbmDigitizationConfig);
      instance.SetDestructor(&destruct_CbmDigitizationConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigitizationConfig*)
   {
      return GenerateInitInstanceLocal((::CbmDigitizationConfig*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigitizationConfig*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmMCInput::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMCInput::Class_Name()
{
   return "CbmMCInput";
}

//______________________________________________________________________________
const char *CbmMCInput::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCInput*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMCInput::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCInput*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMCInput::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCInput*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMCInput::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCInput*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMCInputSet::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMCInputSet::Class_Name()
{
   return "CbmMCInputSet";
}

//______________________________________________________________________________
const char *CbmMCInputSet::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCInputSet*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMCInputSet::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCInputSet*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMCInputSet::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCInputSet*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMCInputSet::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCInputSet*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmDigitizationSource::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmDigitizationSource::Class_Name()
{
   return "CbmDigitizationSource";
}

//______________________________________________________________________________
const char *CbmDigitizationSource::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizationSource*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmDigitizationSource::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizationSource*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmDigitizationSource::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizationSource*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmDigitizationSource::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizationSource*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmDigitizeInfo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmDigitizeInfo::Class_Name()
{
   return "CbmDigitizeInfo";
}

//______________________________________________________________________________
const char *CbmDigitizeInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizeInfo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmDigitizeInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizeInfo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmDigitizeInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizeInfo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmDigitizeInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizeInfo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmDigitization::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmDigitization::Class_Name()
{
   return "CbmDigitization";
}

//______________________________________________________________________________
const char *CbmDigitization::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitization*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmDigitization::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitization*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmDigitization::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitization*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmDigitization::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitization*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRunAna::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRunAna::Class_Name()
{
   return "CbmRunAna";
}

//______________________________________________________________________________
const char *CbmRunAna::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRunAna*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRunAna::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRunAna*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRunAna::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRunAna*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRunAna::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRunAna*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmDigitizationConfig::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmDigitizationConfig::Class_Name()
{
   return "CbmDigitizationConfig";
}

//______________________________________________________________________________
const char *CbmDigitizationConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizationConfig*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmDigitizationConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizationConfig*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmDigitizationConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizationConfig*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmDigitizationConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizationConfig*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmMCInput::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMCInput.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMCInput::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMCInput::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMCInput(void *p) {
      return  p ? new(p) ::CbmMCInput : new ::CbmMCInput;
   }
   static void *newArray_CbmMCInput(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMCInput[nElements] : new ::CbmMCInput[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMCInput(void *p) {
      delete ((::CbmMCInput*)p);
   }
   static void deleteArray_CbmMCInput(void *p) {
      delete [] ((::CbmMCInput*)p);
   }
   static void destruct_CbmMCInput(void *p) {
      typedef ::CbmMCInput current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMCInput

//______________________________________________________________________________
void CbmMCInputSet::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMCInputSet.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMCInputSet::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMCInputSet::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMCInputSet(void *p) {
      return  p ? new(p) ::CbmMCInputSet : new ::CbmMCInputSet;
   }
   static void *newArray_CbmMCInputSet(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMCInputSet[nElements] : new ::CbmMCInputSet[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMCInputSet(void *p) {
      delete ((::CbmMCInputSet*)p);
   }
   static void deleteArray_CbmMCInputSet(void *p) {
      delete [] ((::CbmMCInputSet*)p);
   }
   static void destruct_CbmMCInputSet(void *p) {
      typedef ::CbmMCInputSet current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMCInputSet

//______________________________________________________________________________
void CbmDigitizationSource::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigitizationSource.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigitizationSource::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigitizationSource::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigitizationSource(void *p) {
      return  p ? new(p) ::CbmDigitizationSource : new ::CbmDigitizationSource;
   }
   static void *newArray_CbmDigitizationSource(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigitizationSource[nElements] : new ::CbmDigitizationSource[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigitizationSource(void *p) {
      delete ((::CbmDigitizationSource*)p);
   }
   static void deleteArray_CbmDigitizationSource(void *p) {
      delete [] ((::CbmDigitizationSource*)p);
   }
   static void destruct_CbmDigitizationSource(void *p) {
      typedef ::CbmDigitizationSource current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigitizationSource

//______________________________________________________________________________
void CbmDigitizeInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigitizeInfo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigitizeInfo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigitizeInfo::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmDigitizeInfo(void *p) {
      delete ((::CbmDigitizeInfo*)p);
   }
   static void deleteArray_CbmDigitizeInfo(void *p) {
      delete [] ((::CbmDigitizeInfo*)p);
   }
   static void destruct_CbmDigitizeInfo(void *p) {
      typedef ::CbmDigitizeInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigitizeInfo

//______________________________________________________________________________
void CbmDigitization::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigitization.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigitization::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigitization::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigitization(void *p) {
      return  p ? new(p) ::CbmDigitization : new ::CbmDigitization;
   }
   static void *newArray_CbmDigitization(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigitization[nElements] : new ::CbmDigitization[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigitization(void *p) {
      delete ((::CbmDigitization*)p);
   }
   static void deleteArray_CbmDigitization(void *p) {
      delete [] ((::CbmDigitization*)p);
   }
   static void destruct_CbmDigitization(void *p) {
      typedef ::CbmDigitization current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigitization

//______________________________________________________________________________
void CbmRunAna::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRunAna.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRunAna::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRunAna::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRunAna(void *p) {
      return  p ? new(p) ::CbmRunAna : new ::CbmRunAna;
   }
   static void *newArray_CbmRunAna(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRunAna[nElements] : new ::CbmRunAna[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRunAna(void *p) {
      delete ((::CbmRunAna*)p);
   }
   static void deleteArray_CbmRunAna(void *p) {
      delete [] ((::CbmRunAna*)p);
   }
   static void destruct_CbmRunAna(void *p) {
      typedef ::CbmRunAna current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRunAna

//______________________________________________________________________________
void CbmDigitizationConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigitizationConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigitizationConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigitizationConfig::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigitizationConfig(void *p) {
      return  p ? new(p) ::CbmDigitizationConfig : new ::CbmDigitizationConfig;
   }
   static void *newArray_CbmDigitizationConfig(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigitizationConfig[nElements] : new ::CbmDigitizationConfig[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigitizationConfig(void *p) {
      delete ((::CbmDigitizationConfig*)p);
   }
   static void deleteArray_CbmDigitizationConfig(void *p) {
      delete [] ((::CbmDigitizationConfig*)p);
   }
   static void destruct_CbmDigitizationConfig(void *p) {
      typedef ::CbmDigitizationConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigitizationConfig

namespace ROOT {
   static TClass *vectorlECbmMCInputSetmUgR_Dictionary();
   static void vectorlECbmMCInputSetmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmMCInputSetmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmMCInputSetmUgR(Long_t size, void *p);
   static void delete_vectorlECbmMCInputSetmUgR(void *p);
   static void deleteArray_vectorlECbmMCInputSetmUgR(void *p);
   static void destruct_vectorlECbmMCInputSetmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMCInputSet*>*)
   {
      vector<CbmMCInputSet*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMCInputSet*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMCInputSet*>", -2, "vector", 389,
                  typeid(vector<CbmMCInputSet*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMCInputSetmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmMCInputSet*>) );
      instance.SetNew(&new_vectorlECbmMCInputSetmUgR);
      instance.SetNewArray(&newArray_vectorlECbmMCInputSetmUgR);
      instance.SetDelete(&delete_vectorlECbmMCInputSetmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMCInputSetmUgR);
      instance.SetDestructor(&destruct_vectorlECbmMCInputSetmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMCInputSet*> >()));

      ::ROOT::AddClassAlternate("vector<CbmMCInputSet*>","std::vector<CbmMCInputSet*, std::allocator<CbmMCInputSet*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMCInputSet*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMCInputSetmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMCInputSet*>*)nullptr)->GetClass();
      vectorlECbmMCInputSetmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMCInputSetmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMCInputSetmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMCInputSet*> : new vector<CbmMCInputSet*>;
   }
   static void *newArray_vectorlECbmMCInputSetmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMCInputSet*>[nElements] : new vector<CbmMCInputSet*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMCInputSetmUgR(void *p) {
      delete ((vector<CbmMCInputSet*>*)p);
   }
   static void deleteArray_vectorlECbmMCInputSetmUgR(void *p) {
      delete [] ((vector<CbmMCInputSet*>*)p);
   }
   static void destruct_vectorlECbmMCInputSetmUgR(void *p) {
      typedef vector<CbmMCInputSet*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMCInputSet*>

namespace ROOT {
   static TClass *setlETStringgR_Dictionary();
   static void setlETStringgR_TClassManip(TClass*);
   static void *new_setlETStringgR(void *p = nullptr);
   static void *newArray_setlETStringgR(Long_t size, void *p);
   static void delete_setlETStringgR(void *p);
   static void deleteArray_setlETStringgR(void *p);
   static void destruct_setlETStringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const set<TString>*)
   {
      set<TString> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(set<TString>));
      static ::ROOT::TGenericClassInfo 
         instance("set<TString>", -2, "set", 94,
                  typeid(set<TString>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &setlETStringgR_Dictionary, isa_proxy, 0,
                  sizeof(set<TString>) );
      instance.SetNew(&new_setlETStringgR);
      instance.SetNewArray(&newArray_setlETStringgR);
      instance.SetDelete(&delete_setlETStringgR);
      instance.SetDeleteArray(&deleteArray_setlETStringgR);
      instance.SetDestructor(&destruct_setlETStringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Insert< set<TString> >()));

      ::ROOT::AddClassAlternate("set<TString>","std::set<TString, std::less<TString>, std::allocator<TString> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const set<TString>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *setlETStringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const set<TString>*)nullptr)->GetClass();
      setlETStringgR_TClassManip(theClass);
   return theClass;
   }

   static void setlETStringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_setlETStringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<TString> : new set<TString>;
   }
   static void *newArray_setlETStringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<TString>[nElements] : new set<TString>[nElements];
   }
   // Wrapper around operator delete
   static void delete_setlETStringgR(void *p) {
      delete ((set<TString>*)p);
   }
   static void deleteArray_setlETStringgR(void *p) {
      delete [] ((set<TString>*)p);
   }
   static void destruct_setlETStringgR(void *p) {
      typedef set<TString> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class set<TString>

namespace ROOT {
   static TClass *maplEunsignedsPintcOCbmMCInputmUgR_Dictionary();
   static void maplEunsignedsPintcOCbmMCInputmUgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOCbmMCInputmUgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOCbmMCInputmUgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOCbmMCInputmUgR(void *p);
   static void deleteArray_maplEunsignedsPintcOCbmMCInputmUgR(void *p);
   static void destruct_maplEunsignedsPintcOCbmMCInputmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,CbmMCInput*>*)
   {
      map<unsigned int,CbmMCInput*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,CbmMCInput*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,CbmMCInput*>", -2, "map", 100,
                  typeid(map<unsigned int,CbmMCInput*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOCbmMCInputmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,CbmMCInput*>) );
      instance.SetNew(&new_maplEunsignedsPintcOCbmMCInputmUgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOCbmMCInputmUgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOCbmMCInputmUgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOCbmMCInputmUgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOCbmMCInputmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,CbmMCInput*> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,CbmMCInput*>","std::map<unsigned int, CbmMCInput*, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, CbmMCInput*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,CbmMCInput*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOCbmMCInputmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,CbmMCInput*>*)nullptr)->GetClass();
      maplEunsignedsPintcOCbmMCInputmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOCbmMCInputmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOCbmMCInputmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,CbmMCInput*> : new map<unsigned int,CbmMCInput*>;
   }
   static void *newArray_maplEunsignedsPintcOCbmMCInputmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,CbmMCInput*>[nElements] : new map<unsigned int,CbmMCInput*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOCbmMCInputmUgR(void *p) {
      delete ((map<unsigned int,CbmMCInput*>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOCbmMCInputmUgR(void *p) {
      delete [] ((map<unsigned int,CbmMCInput*>*)p);
   }
   static void destruct_maplEunsignedsPintcOCbmMCInputmUgR(void *p) {
      typedef map<unsigned int,CbmMCInput*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,CbmMCInput*>

namespace {
  void TriggerDictionaryInitialization_libCbmSimResponse_Impl() {
    static const char* headers[] = {
"CbmDigitization.h",
"CbmDigitizationSource.h",
"CbmDigitizeInfo.h",
"CbmMCInput.h",
"CbmMCInputSet.h",
"CbmRunAna.h",
"CbmDigitizationConfig.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/response",
"/home/heinemann/gpuunpacker/cbmroot/sim/response/base",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/response/base",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
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
"/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc",
"/home/heinemann/gpuunpacker/cbmroot/external/xpu/src",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/mvd/plugins/tasks",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/tools",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/SensorDataSheets",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/tasks",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/buffers",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/psd",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/rich/geo",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich/utils",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/steer",
"/home/heinemann/gpuunpacker/cbmroot/core/config",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/geosetup",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/sts/qa",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd/qa",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/bmon",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/sim/response/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmSimResponse dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmMCInput.h")))  __attribute__((annotate("$clingAutoload$CbmDigitization.h")))  CbmMCInput;
class __attribute__((annotate("$clingAutoload$CbmMCInputSet.h")))  __attribute__((annotate("$clingAutoload$CbmDigitization.h")))  CbmMCInputSet;
class __attribute__((annotate("$clingAutoload$CbmDigitizationSource.h")))  __attribute__((annotate("$clingAutoload$CbmDigitization.h")))  CbmDigitizationSource;
class __attribute__((annotate("$clingAutoload$CbmDigitizeInfo.h")))  __attribute__((annotate("$clingAutoload$CbmDigitization.h")))  CbmDigitizeInfo;
class __attribute__((annotate("$clingAutoload$CbmDigitization.h")))  CbmDigitization;
class __attribute__((annotate("$clingAutoload$CbmRunAna.h")))  CbmRunAna;
class __attribute__((annotate("$clingAutoload$CbmDigitizationConfig.h")))  CbmDigitizationConfig;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmSimResponse dictionary payload"

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
#include "CbmDigitization.h"
#include "CbmDigitizationSource.h"
#include "CbmDigitizeInfo.h"
#include "CbmMCInput.h"
#include "CbmMCInputSet.h"
#include "CbmRunAna.h"
#include "CbmDigitizationConfig.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmDigitization", payloadCode, "@",
"CbmDigitizationConfig", payloadCode, "@",
"CbmDigitizationSource", payloadCode, "@",
"CbmDigitizeInfo", payloadCode, "@",
"CbmMCInput", payloadCode, "@",
"CbmMCInputSet", payloadCode, "@",
"CbmRunAna", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmSimResponse",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmSimResponse_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmSimResponse_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmSimResponse() {
  TriggerDictionaryInitialization_libCbmSimResponse_Impl();
}
