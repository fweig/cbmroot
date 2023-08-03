// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmSimSteer
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
#include "CbmSetup.h"
#include "CbmTransport.h"
#include "CbmVMCSettings.h"
#include "CbmGeant3Settings.h"
#include "CbmGeant4Settings.h"
#include "CbmTransportConfig.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_CbmSetup(void *p);
   static void deleteArray_CbmSetup(void *p);
   static void destruct_CbmSetup(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmSetup*)
   {
      ::CbmSetup *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmSetup >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmSetup", ::CbmSetup::Class_Version(), "CbmSetup.h", 43,
                  typeid(::CbmSetup), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmSetup::Dictionary, isa_proxy, 4,
                  sizeof(::CbmSetup) );
      instance.SetDelete(&delete_CbmSetup);
      instance.SetDeleteArray(&deleteArray_CbmSetup);
      instance.SetDestructor(&destruct_CbmSetup);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmSetup*)
   {
      return GenerateInitInstanceLocal((::CbmSetup*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmSetup*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmSetupStorable(void *p = nullptr);
   static void *newArray_CbmSetupStorable(Long_t size, void *p);
   static void delete_CbmSetupStorable(void *p);
   static void deleteArray_CbmSetupStorable(void *p);
   static void destruct_CbmSetupStorable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmSetupStorable*)
   {
      ::CbmSetupStorable *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmSetupStorable >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmSetupStorable", ::CbmSetupStorable::Class_Version(), "CbmSetup.h", 219,
                  typeid(::CbmSetupStorable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmSetupStorable::Dictionary, isa_proxy, 4,
                  sizeof(::CbmSetupStorable) );
      instance.SetNew(&new_CbmSetupStorable);
      instance.SetNewArray(&newArray_CbmSetupStorable);
      instance.SetDelete(&delete_CbmSetupStorable);
      instance.SetDeleteArray(&deleteArray_CbmSetupStorable);
      instance.SetDestructor(&destruct_CbmSetupStorable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmSetupStorable*)
   {
      return GenerateInitInstanceLocal((::CbmSetupStorable*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmSetupStorable*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTransport(void *p = nullptr);
   static void *newArray_CbmTransport(Long_t size, void *p);
   static void delete_CbmTransport(void *p);
   static void deleteArray_CbmTransport(void *p);
   static void destruct_CbmTransport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTransport*)
   {
      ::CbmTransport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTransport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTransport", ::CbmTransport::Class_Version(), "CbmTransport.h", 55,
                  typeid(::CbmTransport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTransport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTransport) );
      instance.SetNew(&new_CbmTransport);
      instance.SetNewArray(&newArray_CbmTransport);
      instance.SetDelete(&delete_CbmTransport);
      instance.SetDeleteArray(&deleteArray_CbmTransport);
      instance.SetDestructor(&destruct_CbmTransport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTransport*)
   {
      return GenerateInitInstanceLocal((::CbmTransport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTransport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmVMCSettings(void *p = nullptr);
   static void *newArray_CbmVMCSettings(Long_t size, void *p);
   static void delete_CbmVMCSettings(void *p);
   static void deleteArray_CbmVMCSettings(void *p);
   static void destruct_CbmVMCSettings(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmVMCSettings*)
   {
      ::CbmVMCSettings *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmVMCSettings >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmVMCSettings", ::CbmVMCSettings::Class_Version(), "CbmVMCSettings.h", 35,
                  typeid(::CbmVMCSettings), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmVMCSettings::Dictionary, isa_proxy, 4,
                  sizeof(::CbmVMCSettings) );
      instance.SetNew(&new_CbmVMCSettings);
      instance.SetNewArray(&newArray_CbmVMCSettings);
      instance.SetDelete(&delete_CbmVMCSettings);
      instance.SetDeleteArray(&deleteArray_CbmVMCSettings);
      instance.SetDestructor(&destruct_CbmVMCSettings);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmVMCSettings*)
   {
      return GenerateInitInstanceLocal((::CbmVMCSettings*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmVMCSettings*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmGeant3Settings(void *p = nullptr);
   static void *newArray_CbmGeant3Settings(Long_t size, void *p);
   static void delete_CbmGeant3Settings(void *p);
   static void deleteArray_CbmGeant3Settings(void *p);
   static void destruct_CbmGeant3Settings(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGeant3Settings*)
   {
      ::CbmGeant3Settings *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGeant3Settings >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGeant3Settings", ::CbmGeant3Settings::Class_Version(), "CbmGeant3Settings.h", 23,
                  typeid(::CbmGeant3Settings), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGeant3Settings::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGeant3Settings) );
      instance.SetNew(&new_CbmGeant3Settings);
      instance.SetNewArray(&newArray_CbmGeant3Settings);
      instance.SetDelete(&delete_CbmGeant3Settings);
      instance.SetDeleteArray(&deleteArray_CbmGeant3Settings);
      instance.SetDestructor(&destruct_CbmGeant3Settings);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGeant3Settings*)
   {
      return GenerateInitInstanceLocal((::CbmGeant3Settings*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGeant3Settings*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmGeant4Settings(void *p = nullptr);
   static void *newArray_CbmGeant4Settings(Long_t size, void *p);
   static void delete_CbmGeant4Settings(void *p);
   static void deleteArray_CbmGeant4Settings(void *p);
   static void destruct_CbmGeant4Settings(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGeant4Settings*)
   {
      ::CbmGeant4Settings *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGeant4Settings >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGeant4Settings", ::CbmGeant4Settings::Class_Version(), "CbmGeant4Settings.h", 27,
                  typeid(::CbmGeant4Settings), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGeant4Settings::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGeant4Settings) );
      instance.SetNew(&new_CbmGeant4Settings);
      instance.SetNewArray(&newArray_CbmGeant4Settings);
      instance.SetDelete(&delete_CbmGeant4Settings);
      instance.SetDeleteArray(&deleteArray_CbmGeant4Settings);
      instance.SetDestructor(&destruct_CbmGeant4Settings);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGeant4Settings*)
   {
      return GenerateInitInstanceLocal((::CbmGeant4Settings*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGeant4Settings*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTransportConfig(void *p = nullptr);
   static void *newArray_CbmTransportConfig(Long_t size, void *p);
   static void delete_CbmTransportConfig(void *p);
   static void deleteArray_CbmTransportConfig(void *p);
   static void destruct_CbmTransportConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTransportConfig*)
   {
      ::CbmTransportConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTransportConfig >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTransportConfig", ::CbmTransportConfig::Class_Version(), "CbmTransportConfig.h", 12,
                  typeid(::CbmTransportConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTransportConfig::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTransportConfig) );
      instance.SetNew(&new_CbmTransportConfig);
      instance.SetNewArray(&newArray_CbmTransportConfig);
      instance.SetDelete(&delete_CbmTransportConfig);
      instance.SetDeleteArray(&deleteArray_CbmTransportConfig);
      instance.SetDestructor(&destruct_CbmTransportConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTransportConfig*)
   {
      return GenerateInitInstanceLocal((::CbmTransportConfig*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTransportConfig*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmSetup::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmSetup::Class_Name()
{
   return "CbmSetup";
}

//______________________________________________________________________________
const char *CbmSetup::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmSetup*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmSetup::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmSetup*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmSetup::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmSetup*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmSetup::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmSetup*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmSetupStorable::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmSetupStorable::Class_Name()
{
   return "CbmSetupStorable";
}

//______________________________________________________________________________
const char *CbmSetupStorable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmSetupStorable*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmSetupStorable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmSetupStorable*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmSetupStorable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmSetupStorable*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmSetupStorable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmSetupStorable*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTransport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTransport::Class_Name()
{
   return "CbmTransport";
}

//______________________________________________________________________________
const char *CbmTransport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTransport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTransport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTransport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTransport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTransport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTransport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTransport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmVMCSettings::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmVMCSettings::Class_Name()
{
   return "CbmVMCSettings";
}

//______________________________________________________________________________
const char *CbmVMCSettings::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmVMCSettings*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmVMCSettings::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmVMCSettings*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmVMCSettings::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmVMCSettings*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmVMCSettings::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmVMCSettings*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGeant3Settings::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGeant3Settings::Class_Name()
{
   return "CbmGeant3Settings";
}

//______________________________________________________________________________
const char *CbmGeant3Settings::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeant3Settings*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGeant3Settings::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeant3Settings*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGeant3Settings::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeant3Settings*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGeant3Settings::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeant3Settings*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGeant4Settings::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGeant4Settings::Class_Name()
{
   return "CbmGeant4Settings";
}

//______________________________________________________________________________
const char *CbmGeant4Settings::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeant4Settings*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGeant4Settings::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeant4Settings*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGeant4Settings::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeant4Settings*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGeant4Settings::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeant4Settings*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTransportConfig::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTransportConfig::Class_Name()
{
   return "CbmTransportConfig";
}

//______________________________________________________________________________
const char *CbmTransportConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTransportConfig*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTransportConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTransportConfig*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTransportConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTransportConfig*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTransportConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTransportConfig*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmSetup::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmSetup.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmSetup::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmSetup::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmSetup(void *p) {
      delete ((::CbmSetup*)p);
   }
   static void deleteArray_CbmSetup(void *p) {
      delete [] ((::CbmSetup*)p);
   }
   static void destruct_CbmSetup(void *p) {
      typedef ::CbmSetup current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmSetup

//______________________________________________________________________________
void CbmSetupStorable::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmSetupStorable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmSetupStorable::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmSetupStorable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmSetupStorable(void *p) {
      return  p ? new(p) ::CbmSetupStorable : new ::CbmSetupStorable;
   }
   static void *newArray_CbmSetupStorable(Long_t nElements, void *p) {
      return p ? new(p) ::CbmSetupStorable[nElements] : new ::CbmSetupStorable[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmSetupStorable(void *p) {
      delete ((::CbmSetupStorable*)p);
   }
   static void deleteArray_CbmSetupStorable(void *p) {
      delete [] ((::CbmSetupStorable*)p);
   }
   static void destruct_CbmSetupStorable(void *p) {
      typedef ::CbmSetupStorable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmSetupStorable

//______________________________________________________________________________
void CbmTransport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTransport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTransport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTransport::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTransport(void *p) {
      return  p ? new(p) ::CbmTransport : new ::CbmTransport;
   }
   static void *newArray_CbmTransport(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTransport[nElements] : new ::CbmTransport[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTransport(void *p) {
      delete ((::CbmTransport*)p);
   }
   static void deleteArray_CbmTransport(void *p) {
      delete [] ((::CbmTransport*)p);
   }
   static void destruct_CbmTransport(void *p) {
      typedef ::CbmTransport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTransport

//______________________________________________________________________________
void CbmVMCSettings::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmVMCSettings.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmVMCSettings::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmVMCSettings::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmVMCSettings(void *p) {
      return  p ? new(p) ::CbmVMCSettings : new ::CbmVMCSettings;
   }
   static void *newArray_CbmVMCSettings(Long_t nElements, void *p) {
      return p ? new(p) ::CbmVMCSettings[nElements] : new ::CbmVMCSettings[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmVMCSettings(void *p) {
      delete ((::CbmVMCSettings*)p);
   }
   static void deleteArray_CbmVMCSettings(void *p) {
      delete [] ((::CbmVMCSettings*)p);
   }
   static void destruct_CbmVMCSettings(void *p) {
      typedef ::CbmVMCSettings current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmVMCSettings

//______________________________________________________________________________
void CbmGeant3Settings::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGeant3Settings.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGeant3Settings::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGeant3Settings::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGeant3Settings(void *p) {
      return  p ? new(p) ::CbmGeant3Settings : new ::CbmGeant3Settings;
   }
   static void *newArray_CbmGeant3Settings(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGeant3Settings[nElements] : new ::CbmGeant3Settings[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGeant3Settings(void *p) {
      delete ((::CbmGeant3Settings*)p);
   }
   static void deleteArray_CbmGeant3Settings(void *p) {
      delete [] ((::CbmGeant3Settings*)p);
   }
   static void destruct_CbmGeant3Settings(void *p) {
      typedef ::CbmGeant3Settings current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGeant3Settings

//______________________________________________________________________________
void CbmGeant4Settings::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGeant4Settings.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGeant4Settings::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGeant4Settings::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGeant4Settings(void *p) {
      return  p ? new(p) ::CbmGeant4Settings : new ::CbmGeant4Settings;
   }
   static void *newArray_CbmGeant4Settings(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGeant4Settings[nElements] : new ::CbmGeant4Settings[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGeant4Settings(void *p) {
      delete ((::CbmGeant4Settings*)p);
   }
   static void deleteArray_CbmGeant4Settings(void *p) {
      delete [] ((::CbmGeant4Settings*)p);
   }
   static void destruct_CbmGeant4Settings(void *p) {
      typedef ::CbmGeant4Settings current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGeant4Settings

//______________________________________________________________________________
void CbmTransportConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTransportConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTransportConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTransportConfig::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTransportConfig(void *p) {
      return  p ? new(p) ::CbmTransportConfig : new ::CbmTransportConfig;
   }
   static void *newArray_CbmTransportConfig(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTransportConfig[nElements] : new ::CbmTransportConfig[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTransportConfig(void *p) {
      delete ((::CbmTransportConfig*)p);
   }
   static void deleteArray_CbmTransportConfig(void *p) {
      delete [] ((::CbmTransportConfig*)p);
   }
   static void destruct_CbmTransportConfig(void *p) {
      typedef ::CbmTransportConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTransportConfig

namespace ROOT {
   static TClass *vectorlEstringgR_Dictionary();
   static void vectorlEstringgR_TClassManip(TClass*);
   static void *new_vectorlEstringgR(void *p = nullptr);
   static void *newArray_vectorlEstringgR(Long_t size, void *p);
   static void delete_vectorlEstringgR(void *p);
   static void deleteArray_vectorlEstringgR(void *p);
   static void destruct_vectorlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<string>*)
   {
      vector<string> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<string>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<string>", -2, "vector", 389,
                  typeid(vector<string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<string>) );
      instance.SetNew(&new_vectorlEstringgR);
      instance.SetNewArray(&newArray_vectorlEstringgR);
      instance.SetDelete(&delete_vectorlEstringgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstringgR);
      instance.SetDestructor(&destruct_vectorlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<string> >()));

      ::ROOT::AddClassAlternate("vector<string>","std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<string>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<string>*)nullptr)->GetClass();
      vectorlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string> : new vector<string>;
   }
   static void *newArray_vectorlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string>[nElements] : new vector<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstringgR(void *p) {
      delete ((vector<string>*)p);
   }
   static void deleteArray_vectorlEstringgR(void *p) {
      delete [] ((vector<string>*)p);
   }
   static void destruct_vectorlEstringgR(void *p) {
      typedef vector<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<string>

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = nullptr);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 389,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));

      ::ROOT::AddClassAlternate("vector<int>","std::vector<int, std::allocator<int> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)nullptr)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEFairGeneratormUgR_Dictionary();
   static void vectorlEFairGeneratormUgR_TClassManip(TClass*);
   static void *new_vectorlEFairGeneratormUgR(void *p = nullptr);
   static void *newArray_vectorlEFairGeneratormUgR(Long_t size, void *p);
   static void delete_vectorlEFairGeneratormUgR(void *p);
   static void deleteArray_vectorlEFairGeneratormUgR(void *p);
   static void destruct_vectorlEFairGeneratormUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<FairGenerator*>*)
   {
      vector<FairGenerator*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<FairGenerator*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<FairGenerator*>", -2, "vector", 389,
                  typeid(vector<FairGenerator*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEFairGeneratormUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<FairGenerator*>) );
      instance.SetNew(&new_vectorlEFairGeneratormUgR);
      instance.SetNewArray(&newArray_vectorlEFairGeneratormUgR);
      instance.SetDelete(&delete_vectorlEFairGeneratormUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEFairGeneratormUgR);
      instance.SetDestructor(&destruct_vectorlEFairGeneratormUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<FairGenerator*> >()));

      ::ROOT::AddClassAlternate("vector<FairGenerator*>","std::vector<FairGenerator*, std::allocator<FairGenerator*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<FairGenerator*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEFairGeneratormUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<FairGenerator*>*)nullptr)->GetClass();
      vectorlEFairGeneratormUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEFairGeneratormUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEFairGeneratormUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<FairGenerator*> : new vector<FairGenerator*>;
   }
   static void *newArray_vectorlEFairGeneratormUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<FairGenerator*>[nElements] : new vector<FairGenerator*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEFairGeneratormUgR(void *p) {
      delete ((vector<FairGenerator*>*)p);
   }
   static void deleteArray_vectorlEFairGeneratormUgR(void *p) {
      delete [] ((vector<FairGenerator*>*)p);
   }
   static void destruct_vectorlEFairGeneratormUgR(void *p) {
      typedef vector<FairGenerator*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<FairGenerator*>

namespace ROOT {
   static TClass *maplEintcOvectorlEintgRsPgR_Dictionary();
   static void maplEintcOvectorlEintgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOvectorlEintgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOvectorlEintgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOvectorlEintgRsPgR(void *p);
   static void deleteArray_maplEintcOvectorlEintgRsPgR(void *p);
   static void destruct_maplEintcOvectorlEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,vector<int> >*)
   {
      map<int,vector<int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,vector<int> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,vector<int> >", -2, "map", 100,
                  typeid(map<int,vector<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOvectorlEintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,vector<int> >) );
      instance.SetNew(&new_maplEintcOvectorlEintgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOvectorlEintgRsPgR);
      instance.SetDelete(&delete_maplEintcOvectorlEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOvectorlEintgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOvectorlEintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,vector<int> > >()));

      ::ROOT::AddClassAlternate("map<int,vector<int> >","std::map<int, std::vector<int, std::allocator<int> >, std::less<int>, std::allocator<std::pair<int const, std::vector<int, std::allocator<int> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,vector<int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOvectorlEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,vector<int> >*)nullptr)->GetClass();
      maplEintcOvectorlEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOvectorlEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOvectorlEintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<int> > : new map<int,vector<int> >;
   }
   static void *newArray_maplEintcOvectorlEintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<int> >[nElements] : new map<int,vector<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOvectorlEintgRsPgR(void *p) {
      delete ((map<int,vector<int> >*)p);
   }
   static void deleteArray_maplEintcOvectorlEintgRsPgR(void *p) {
      delete [] ((map<int,vector<int> >*)p);
   }
   static void destruct_maplEintcOvectorlEintgRsPgR(void *p) {
      typedef map<int,vector<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,vector<int> >

namespace {
  void TriggerDictionaryInitialization_libCbmSimSteer_Impl() {
    static const char* headers[] = {
"CbmSetup.h",
"CbmTransport.h",
"CbmVMCSettings.h",
"CbmGeant3Settings.h",
"CbmGeant4Settings.h",
"CbmTransportConfig.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/steer",
"/home/heinemann/gpuunpacker/cbmroot/core/config",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/steer",
"/home/heinemann/gpuunpacker/cbmroot/core/config",
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
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/geosetup",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/core/field",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators/unigen",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators/pluto",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/TGeant3",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/geant4vmc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/g4root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/Geant4",
"/usr/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/BaseVGM",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/XmlVGM",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/ClhepVGM",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/Geant4GM",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/RootGM",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/sim/transport/steer/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmSimSteer dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmSetup.h")))  CbmSetup;
class __attribute__((annotate("$clingAutoload$CbmSetup.h")))  CbmSetupStorable;
class __attribute__((annotate("$clingAutoload$CbmTransport.h")))  CbmTransport;
class __attribute__((annotate("$clingAutoload$CbmVMCSettings.h")))  CbmVMCSettings;
class __attribute__((annotate("$clingAutoload$CbmGeant3Settings.h")))  CbmGeant3Settings;
class __attribute__((annotate("$clingAutoload$CbmGeant4Settings.h")))  CbmGeant4Settings;
class __attribute__((annotate("$clingAutoload$CbmTransportConfig.h")))  CbmTransportConfig;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmSimSteer dictionary payload"

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
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif
#ifndef G4LIB_BUILD_DLL
  #define G4LIB_BUILD_DLL 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "CbmSetup.h"
#include "CbmTransport.h"
#include "CbmVMCSettings.h"
#include "CbmGeant3Settings.h"
#include "CbmGeant4Settings.h"
#include "CbmTransportConfig.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmGeant3Settings", payloadCode, "@",
"CbmGeant4Settings", payloadCode, "@",
"CbmSetup", payloadCode, "@",
"CbmSetupStorable", payloadCode, "@",
"CbmTransport", payloadCode, "@",
"CbmTransportConfig", payloadCode, "@",
"CbmVMCSettings", payloadCode, "@",
"ECbmEngine", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmSimSteer",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmSimSteer_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmSimSteer_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmSimSteer() {
  TriggerDictionaryInitialization_libCbmSimSteer_Impl();
}
