// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmGeoSetup
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
#include "CbmGeoSetup.h"
#include "CbmGeoSetupField.h"
#include "CbmGeoSetupModule.h"
#include "CbmGeoSetupMedia.h"
#include "CbmGeoSetupProvider.h"
#include "CbmGeoSetupRepoProvider.h"
#include "CbmGeoSetupDbProvider.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmGeoSetupField(void *p = nullptr);
   static void *newArray_CbmGeoSetupField(Long_t size, void *p);
   static void delete_CbmGeoSetupField(void *p);
   static void deleteArray_CbmGeoSetupField(void *p);
   static void destruct_CbmGeoSetupField(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGeoSetupField*)
   {
      ::CbmGeoSetupField *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGeoSetupField >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGeoSetupField", ::CbmGeoSetupField::Class_Version(), "CbmGeoSetupField.h", 25,
                  typeid(::CbmGeoSetupField), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGeoSetupField::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGeoSetupField) );
      instance.SetNew(&new_CbmGeoSetupField);
      instance.SetNewArray(&newArray_CbmGeoSetupField);
      instance.SetDelete(&delete_CbmGeoSetupField);
      instance.SetDeleteArray(&deleteArray_CbmGeoSetupField);
      instance.SetDestructor(&destruct_CbmGeoSetupField);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGeoSetupField*)
   {
      return GenerateInitInstanceLocal((::CbmGeoSetupField*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGeoSetupField*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmGeoSetupMedia(void *p = nullptr);
   static void *newArray_CbmGeoSetupMedia(Long_t size, void *p);
   static void delete_CbmGeoSetupMedia(void *p);
   static void deleteArray_CbmGeoSetupMedia(void *p);
   static void destruct_CbmGeoSetupMedia(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGeoSetupMedia*)
   {
      ::CbmGeoSetupMedia *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGeoSetupMedia >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGeoSetupMedia", ::CbmGeoSetupMedia::Class_Version(), "CbmGeoSetupMedia.h", 23,
                  typeid(::CbmGeoSetupMedia), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGeoSetupMedia::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGeoSetupMedia) );
      instance.SetNew(&new_CbmGeoSetupMedia);
      instance.SetNewArray(&newArray_CbmGeoSetupMedia);
      instance.SetDelete(&delete_CbmGeoSetupMedia);
      instance.SetDeleteArray(&deleteArray_CbmGeoSetupMedia);
      instance.SetDestructor(&destruct_CbmGeoSetupMedia);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGeoSetupMedia*)
   {
      return GenerateInitInstanceLocal((::CbmGeoSetupMedia*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGeoSetupMedia*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmGeoSetupModule(void *p = nullptr);
   static void *newArray_CbmGeoSetupModule(Long_t size, void *p);
   static void delete_CbmGeoSetupModule(void *p);
   static void deleteArray_CbmGeoSetupModule(void *p);
   static void destruct_CbmGeoSetupModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGeoSetupModule*)
   {
      ::CbmGeoSetupModule *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGeoSetupModule >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGeoSetupModule", ::CbmGeoSetupModule::Class_Version(), "CbmGeoSetupModule.h", 26,
                  typeid(::CbmGeoSetupModule), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGeoSetupModule::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGeoSetupModule) );
      instance.SetNew(&new_CbmGeoSetupModule);
      instance.SetNewArray(&newArray_CbmGeoSetupModule);
      instance.SetDelete(&delete_CbmGeoSetupModule);
      instance.SetDeleteArray(&deleteArray_CbmGeoSetupModule);
      instance.SetDestructor(&destruct_CbmGeoSetupModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGeoSetupModule*)
   {
      return GenerateInitInstanceLocal((::CbmGeoSetupModule*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGeoSetupModule*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmGeoSetup(void *p = nullptr);
   static void *newArray_CbmGeoSetup(Long_t size, void *p);
   static void delete_CbmGeoSetup(void *p);
   static void deleteArray_CbmGeoSetup(void *p);
   static void destruct_CbmGeoSetup(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGeoSetup*)
   {
      ::CbmGeoSetup *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGeoSetup >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGeoSetup", ::CbmGeoSetup::Class_Version(), "CbmGeoSetup.h", 34,
                  typeid(::CbmGeoSetup), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGeoSetup::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGeoSetup) );
      instance.SetNew(&new_CbmGeoSetup);
      instance.SetNewArray(&newArray_CbmGeoSetup);
      instance.SetDelete(&delete_CbmGeoSetup);
      instance.SetDeleteArray(&deleteArray_CbmGeoSetup);
      instance.SetDestructor(&destruct_CbmGeoSetup);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGeoSetup*)
   {
      return GenerateInitInstanceLocal((::CbmGeoSetup*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGeoSetup*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmGeoSetupProvider(void *p);
   static void deleteArray_CbmGeoSetupProvider(void *p);
   static void destruct_CbmGeoSetupProvider(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGeoSetupProvider*)
   {
      ::CbmGeoSetupProvider *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGeoSetupProvider >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGeoSetupProvider", ::CbmGeoSetupProvider::Class_Version(), "CbmGeoSetupProvider.h", 32,
                  typeid(::CbmGeoSetupProvider), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGeoSetupProvider::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGeoSetupProvider) );
      instance.SetDelete(&delete_CbmGeoSetupProvider);
      instance.SetDeleteArray(&deleteArray_CbmGeoSetupProvider);
      instance.SetDestructor(&destruct_CbmGeoSetupProvider);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGeoSetupProvider*)
   {
      return GenerateInitInstanceLocal((::CbmGeoSetupProvider*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGeoSetupProvider*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmGeoSetupRepoProvider(void *p = nullptr);
   static void *newArray_CbmGeoSetupRepoProvider(Long_t size, void *p);
   static void delete_CbmGeoSetupRepoProvider(void *p);
   static void deleteArray_CbmGeoSetupRepoProvider(void *p);
   static void destruct_CbmGeoSetupRepoProvider(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGeoSetupRepoProvider*)
   {
      ::CbmGeoSetupRepoProvider *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGeoSetupRepoProvider >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGeoSetupRepoProvider", ::CbmGeoSetupRepoProvider::Class_Version(), "CbmGeoSetupRepoProvider.h", 25,
                  typeid(::CbmGeoSetupRepoProvider), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGeoSetupRepoProvider::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGeoSetupRepoProvider) );
      instance.SetNew(&new_CbmGeoSetupRepoProvider);
      instance.SetNewArray(&newArray_CbmGeoSetupRepoProvider);
      instance.SetDelete(&delete_CbmGeoSetupRepoProvider);
      instance.SetDeleteArray(&deleteArray_CbmGeoSetupRepoProvider);
      instance.SetDestructor(&destruct_CbmGeoSetupRepoProvider);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGeoSetupRepoProvider*)
   {
      return GenerateInitInstanceLocal((::CbmGeoSetupRepoProvider*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGeoSetupRepoProvider*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmGeoSetupDbProvider(void *p = nullptr);
   static void *newArray_CbmGeoSetupDbProvider(Long_t size, void *p);
   static void delete_CbmGeoSetupDbProvider(void *p);
   static void deleteArray_CbmGeoSetupDbProvider(void *p);
   static void destruct_CbmGeoSetupDbProvider(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGeoSetupDbProvider*)
   {
      ::CbmGeoSetupDbProvider *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGeoSetupDbProvider >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGeoSetupDbProvider", ::CbmGeoSetupDbProvider::Class_Version(), "CbmGeoSetupDbProvider.h", 25,
                  typeid(::CbmGeoSetupDbProvider), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGeoSetupDbProvider::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGeoSetupDbProvider) );
      instance.SetNew(&new_CbmGeoSetupDbProvider);
      instance.SetNewArray(&newArray_CbmGeoSetupDbProvider);
      instance.SetDelete(&delete_CbmGeoSetupDbProvider);
      instance.SetDeleteArray(&deleteArray_CbmGeoSetupDbProvider);
      instance.SetDestructor(&destruct_CbmGeoSetupDbProvider);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGeoSetupDbProvider*)
   {
      return GenerateInitInstanceLocal((::CbmGeoSetupDbProvider*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGeoSetupDbProvider*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmGeoSetupField::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGeoSetupField::Class_Name()
{
   return "CbmGeoSetupField";
}

//______________________________________________________________________________
const char *CbmGeoSetupField::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupField*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGeoSetupField::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupField*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGeoSetupField::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupField*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGeoSetupField::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupField*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGeoSetupMedia::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGeoSetupMedia::Class_Name()
{
   return "CbmGeoSetupMedia";
}

//______________________________________________________________________________
const char *CbmGeoSetupMedia::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupMedia*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGeoSetupMedia::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupMedia*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGeoSetupMedia::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupMedia*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGeoSetupMedia::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupMedia*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGeoSetupModule::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGeoSetupModule::Class_Name()
{
   return "CbmGeoSetupModule";
}

//______________________________________________________________________________
const char *CbmGeoSetupModule::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupModule*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGeoSetupModule::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupModule*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGeoSetupModule::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupModule*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGeoSetupModule::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupModule*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGeoSetup::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGeoSetup::Class_Name()
{
   return "CbmGeoSetup";
}

//______________________________________________________________________________
const char *CbmGeoSetup::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetup*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGeoSetup::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetup*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGeoSetup::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetup*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGeoSetup::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetup*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGeoSetupProvider::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGeoSetupProvider::Class_Name()
{
   return "CbmGeoSetupProvider";
}

//______________________________________________________________________________
const char *CbmGeoSetupProvider::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupProvider*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGeoSetupProvider::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupProvider*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGeoSetupProvider::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupProvider*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGeoSetupProvider::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupProvider*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGeoSetupRepoProvider::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGeoSetupRepoProvider::Class_Name()
{
   return "CbmGeoSetupRepoProvider";
}

//______________________________________________________________________________
const char *CbmGeoSetupRepoProvider::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupRepoProvider*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGeoSetupRepoProvider::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupRepoProvider*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGeoSetupRepoProvider::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupRepoProvider*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGeoSetupRepoProvider::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupRepoProvider*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGeoSetupDbProvider::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGeoSetupDbProvider::Class_Name()
{
   return "CbmGeoSetupDbProvider";
}

//______________________________________________________________________________
const char *CbmGeoSetupDbProvider::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupDbProvider*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGeoSetupDbProvider::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupDbProvider*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGeoSetupDbProvider::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupDbProvider*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGeoSetupDbProvider::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoSetupDbProvider*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmGeoSetupField::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGeoSetupField.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGeoSetupField::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGeoSetupField::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGeoSetupField(void *p) {
      return  p ? new(p) ::CbmGeoSetupField : new ::CbmGeoSetupField;
   }
   static void *newArray_CbmGeoSetupField(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGeoSetupField[nElements] : new ::CbmGeoSetupField[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGeoSetupField(void *p) {
      delete ((::CbmGeoSetupField*)p);
   }
   static void deleteArray_CbmGeoSetupField(void *p) {
      delete [] ((::CbmGeoSetupField*)p);
   }
   static void destruct_CbmGeoSetupField(void *p) {
      typedef ::CbmGeoSetupField current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGeoSetupField

//______________________________________________________________________________
void CbmGeoSetupMedia::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGeoSetupMedia.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGeoSetupMedia::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGeoSetupMedia::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGeoSetupMedia(void *p) {
      return  p ? new(p) ::CbmGeoSetupMedia : new ::CbmGeoSetupMedia;
   }
   static void *newArray_CbmGeoSetupMedia(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGeoSetupMedia[nElements] : new ::CbmGeoSetupMedia[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGeoSetupMedia(void *p) {
      delete ((::CbmGeoSetupMedia*)p);
   }
   static void deleteArray_CbmGeoSetupMedia(void *p) {
      delete [] ((::CbmGeoSetupMedia*)p);
   }
   static void destruct_CbmGeoSetupMedia(void *p) {
      typedef ::CbmGeoSetupMedia current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGeoSetupMedia

//______________________________________________________________________________
void CbmGeoSetupModule::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGeoSetupModule.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGeoSetupModule::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGeoSetupModule::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGeoSetupModule(void *p) {
      return  p ? new(p) ::CbmGeoSetupModule : new ::CbmGeoSetupModule;
   }
   static void *newArray_CbmGeoSetupModule(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGeoSetupModule[nElements] : new ::CbmGeoSetupModule[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGeoSetupModule(void *p) {
      delete ((::CbmGeoSetupModule*)p);
   }
   static void deleteArray_CbmGeoSetupModule(void *p) {
      delete [] ((::CbmGeoSetupModule*)p);
   }
   static void destruct_CbmGeoSetupModule(void *p) {
      typedef ::CbmGeoSetupModule current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGeoSetupModule

//______________________________________________________________________________
void CbmGeoSetup::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGeoSetup.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGeoSetup::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGeoSetup::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGeoSetup(void *p) {
      return  p ? new(p) ::CbmGeoSetup : new ::CbmGeoSetup;
   }
   static void *newArray_CbmGeoSetup(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGeoSetup[nElements] : new ::CbmGeoSetup[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGeoSetup(void *p) {
      delete ((::CbmGeoSetup*)p);
   }
   static void deleteArray_CbmGeoSetup(void *p) {
      delete [] ((::CbmGeoSetup*)p);
   }
   static void destruct_CbmGeoSetup(void *p) {
      typedef ::CbmGeoSetup current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGeoSetup

//______________________________________________________________________________
void CbmGeoSetupProvider::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGeoSetupProvider.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGeoSetupProvider::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGeoSetupProvider::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmGeoSetupProvider(void *p) {
      delete ((::CbmGeoSetupProvider*)p);
   }
   static void deleteArray_CbmGeoSetupProvider(void *p) {
      delete [] ((::CbmGeoSetupProvider*)p);
   }
   static void destruct_CbmGeoSetupProvider(void *p) {
      typedef ::CbmGeoSetupProvider current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGeoSetupProvider

//______________________________________________________________________________
void CbmGeoSetupRepoProvider::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGeoSetupRepoProvider.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGeoSetupRepoProvider::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGeoSetupRepoProvider::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGeoSetupRepoProvider(void *p) {
      return  p ? new(p) ::CbmGeoSetupRepoProvider : new ::CbmGeoSetupRepoProvider;
   }
   static void *newArray_CbmGeoSetupRepoProvider(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGeoSetupRepoProvider[nElements] : new ::CbmGeoSetupRepoProvider[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGeoSetupRepoProvider(void *p) {
      delete ((::CbmGeoSetupRepoProvider*)p);
   }
   static void deleteArray_CbmGeoSetupRepoProvider(void *p) {
      delete [] ((::CbmGeoSetupRepoProvider*)p);
   }
   static void destruct_CbmGeoSetupRepoProvider(void *p) {
      typedef ::CbmGeoSetupRepoProvider current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGeoSetupRepoProvider

//______________________________________________________________________________
void CbmGeoSetupDbProvider::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGeoSetupDbProvider.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGeoSetupDbProvider::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGeoSetupDbProvider::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGeoSetupDbProvider(void *p) {
      return  p ? new(p) ::CbmGeoSetupDbProvider : new ::CbmGeoSetupDbProvider;
   }
   static void *newArray_CbmGeoSetupDbProvider(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGeoSetupDbProvider[nElements] : new ::CbmGeoSetupDbProvider[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGeoSetupDbProvider(void *p) {
      delete ((::CbmGeoSetupDbProvider*)p);
   }
   static void deleteArray_CbmGeoSetupDbProvider(void *p) {
      delete [] ((::CbmGeoSetupDbProvider*)p);
   }
   static void destruct_CbmGeoSetupDbProvider(void *p) {
      typedef ::CbmGeoSetupDbProvider current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGeoSetupDbProvider

namespace ROOT {
   static TClass *maplEECbmModuleIdcOCbmGeoSetupModulegR_Dictionary();
   static void maplEECbmModuleIdcOCbmGeoSetupModulegR_TClassManip(TClass*);
   static void *new_maplEECbmModuleIdcOCbmGeoSetupModulegR(void *p = nullptr);
   static void *newArray_maplEECbmModuleIdcOCbmGeoSetupModulegR(Long_t size, void *p);
   static void delete_maplEECbmModuleIdcOCbmGeoSetupModulegR(void *p);
   static void deleteArray_maplEECbmModuleIdcOCbmGeoSetupModulegR(void *p);
   static void destruct_maplEECbmModuleIdcOCbmGeoSetupModulegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<ECbmModuleId,CbmGeoSetupModule>*)
   {
      map<ECbmModuleId,CbmGeoSetupModule> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<ECbmModuleId,CbmGeoSetupModule>));
      static ::ROOT::TGenericClassInfo 
         instance("map<ECbmModuleId,CbmGeoSetupModule>", -2, "map", 100,
                  typeid(map<ECbmModuleId,CbmGeoSetupModule>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEECbmModuleIdcOCbmGeoSetupModulegR_Dictionary, isa_proxy, 0,
                  sizeof(map<ECbmModuleId,CbmGeoSetupModule>) );
      instance.SetNew(&new_maplEECbmModuleIdcOCbmGeoSetupModulegR);
      instance.SetNewArray(&newArray_maplEECbmModuleIdcOCbmGeoSetupModulegR);
      instance.SetDelete(&delete_maplEECbmModuleIdcOCbmGeoSetupModulegR);
      instance.SetDeleteArray(&deleteArray_maplEECbmModuleIdcOCbmGeoSetupModulegR);
      instance.SetDestructor(&destruct_maplEECbmModuleIdcOCbmGeoSetupModulegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<ECbmModuleId,CbmGeoSetupModule> >()));

      ::ROOT::AddClassAlternate("map<ECbmModuleId,CbmGeoSetupModule>","std::map<ECbmModuleId, CbmGeoSetupModule, std::less<ECbmModuleId>, std::allocator<std::pair<ECbmModuleId const, CbmGeoSetupModule> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<ECbmModuleId,CbmGeoSetupModule>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEECbmModuleIdcOCbmGeoSetupModulegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<ECbmModuleId,CbmGeoSetupModule>*)nullptr)->GetClass();
      maplEECbmModuleIdcOCbmGeoSetupModulegR_TClassManip(theClass);
   return theClass;
   }

   static void maplEECbmModuleIdcOCbmGeoSetupModulegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEECbmModuleIdcOCbmGeoSetupModulegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,CbmGeoSetupModule> : new map<ECbmModuleId,CbmGeoSetupModule>;
   }
   static void *newArray_maplEECbmModuleIdcOCbmGeoSetupModulegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,CbmGeoSetupModule>[nElements] : new map<ECbmModuleId,CbmGeoSetupModule>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEECbmModuleIdcOCbmGeoSetupModulegR(void *p) {
      delete ((map<ECbmModuleId,CbmGeoSetupModule>*)p);
   }
   static void deleteArray_maplEECbmModuleIdcOCbmGeoSetupModulegR(void *p) {
      delete [] ((map<ECbmModuleId,CbmGeoSetupModule>*)p);
   }
   static void destruct_maplEECbmModuleIdcOCbmGeoSetupModulegR(void *p) {
      typedef map<ECbmModuleId,CbmGeoSetupModule> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<ECbmModuleId,CbmGeoSetupModule>

namespace {
  void TriggerDictionaryInitialization_libCbmGeoSetup_Impl() {
    static const char* headers[] = {
"CbmGeoSetup.h",
"CbmGeoSetupField.h",
"CbmGeoSetupModule.h",
"CbmGeoSetupMedia.h",
"CbmGeoSetupProvider.h",
"CbmGeoSetupRepoProvider.h",
"CbmGeoSetupDbProvider.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/geosetup",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/geosetup",
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
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/much/qa",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
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
"/home/heinemann/gpuunpacker/cbmroot/sim/passive",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/psd",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/rich/geo",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich/utils",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/sts/qa",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd/qa",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd",
"/usr/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/sim/transport/geosetup/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmGeoSetup dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmGeoSetupField.h")))  __attribute__((annotate("$clingAutoload$CbmGeoSetup.h")))  CbmGeoSetupField;
class __attribute__((annotate("$clingAutoload$CbmGeoSetupMedia.h")))  __attribute__((annotate("$clingAutoload$CbmGeoSetup.h")))  CbmGeoSetupMedia;
class __attribute__((annotate("$clingAutoload$CbmGeoSetupModule.h")))  __attribute__((annotate("$clingAutoload$CbmGeoSetup.h")))  CbmGeoSetupModule;
class __attribute__((annotate("$clingAutoload$CbmGeoSetup.h")))  CbmGeoSetup;
class __attribute__((annotate("$clingAutoload$CbmGeoSetupProvider.h")))  CbmGeoSetupProvider;
class __attribute__((annotate("$clingAutoload$CbmGeoSetupRepoProvider.h")))  CbmGeoSetupRepoProvider;
class __attribute__((annotate("$clingAutoload$CbmGeoSetupDbProvider.h")))  CbmGeoSetupDbProvider;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmGeoSetup dictionary payload"

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
#include "CbmGeoSetup.h"
#include "CbmGeoSetupField.h"
#include "CbmGeoSetupModule.h"
#include "CbmGeoSetupMedia.h"
#include "CbmGeoSetupProvider.h"
#include "CbmGeoSetupRepoProvider.h"
#include "CbmGeoSetupDbProvider.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmGeoSetup", payloadCode, "@",
"CbmGeoSetupDbProvider", payloadCode, "@",
"CbmGeoSetupField", payloadCode, "@",
"CbmGeoSetupMedia", payloadCode, "@",
"CbmGeoSetupModule", payloadCode, "@",
"CbmGeoSetupProvider", payloadCode, "@",
"CbmGeoSetupRepoProvider", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmGeoSetup",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmGeoSetup_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmGeoSetup_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmGeoSetup() {
  TriggerDictionaryInitialization_libCbmGeoSetup_Impl();
}
