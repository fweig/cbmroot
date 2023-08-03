// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmPassive
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
#include "CbmCave.h"
#include "CbmGeoCave.h"
#include "CbmGeoPassivePar.h"
#include "CbmGeoPlatform.h"
#include "CbmMagnet.h"
#include "CbmPassiveContFact.h"
#include "CbmPipe.h"
#include "CbmPlatform.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmCave(void *p = nullptr);
   static void *newArray_CbmCave(Long_t size, void *p);
   static void delete_CbmCave(void *p);
   static void deleteArray_CbmCave(void *p);
   static void destruct_CbmCave(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCave*)
   {
      ::CbmCave *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCave >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCave", ::CbmCave::Class_Version(), "CbmCave.h", 11,
                  typeid(::CbmCave), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCave::Dictionary, isa_proxy, 4,
                  sizeof(::CbmCave) );
      instance.SetNew(&new_CbmCave);
      instance.SetNewArray(&newArray_CbmCave);
      instance.SetDelete(&delete_CbmCave);
      instance.SetDeleteArray(&deleteArray_CbmCave);
      instance.SetDestructor(&destruct_CbmCave);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCave*)
   {
      return GenerateInitInstanceLocal((::CbmCave*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCave*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmGeoCave(void *p = nullptr);
   static void *newArray_CbmGeoCave(Long_t size, void *p);
   static void delete_CbmGeoCave(void *p);
   static void deleteArray_CbmGeoCave(void *p);
   static void destruct_CbmGeoCave(void *p);
   static void streamer_CbmGeoCave(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGeoCave*)
   {
      ::CbmGeoCave *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGeoCave >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGeoCave", ::CbmGeoCave::Class_Version(), "CbmGeoCave.h", 16,
                  typeid(::CbmGeoCave), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGeoCave::Dictionary, isa_proxy, 16,
                  sizeof(::CbmGeoCave) );
      instance.SetNew(&new_CbmGeoCave);
      instance.SetNewArray(&newArray_CbmGeoCave);
      instance.SetDelete(&delete_CbmGeoCave);
      instance.SetDeleteArray(&deleteArray_CbmGeoCave);
      instance.SetDestructor(&destruct_CbmGeoCave);
      instance.SetStreamerFunc(&streamer_CbmGeoCave);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGeoCave*)
   {
      return GenerateInitInstanceLocal((::CbmGeoCave*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGeoCave*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmGeoPassivePar(void *p = nullptr);
   static void *newArray_CbmGeoPassivePar(Long_t size, void *p);
   static void delete_CbmGeoPassivePar(void *p);
   static void deleteArray_CbmGeoPassivePar(void *p);
   static void destruct_CbmGeoPassivePar(void *p);
   static void streamer_CbmGeoPassivePar(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGeoPassivePar*)
   {
      ::CbmGeoPassivePar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGeoPassivePar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGeoPassivePar", ::CbmGeoPassivePar::Class_Version(), "CbmGeoPassivePar.h", 10,
                  typeid(::CbmGeoPassivePar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGeoPassivePar::Dictionary, isa_proxy, 16,
                  sizeof(::CbmGeoPassivePar) );
      instance.SetNew(&new_CbmGeoPassivePar);
      instance.SetNewArray(&newArray_CbmGeoPassivePar);
      instance.SetDelete(&delete_CbmGeoPassivePar);
      instance.SetDeleteArray(&deleteArray_CbmGeoPassivePar);
      instance.SetDestructor(&destruct_CbmGeoPassivePar);
      instance.SetStreamerFunc(&streamer_CbmGeoPassivePar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGeoPassivePar*)
   {
      return GenerateInitInstanceLocal((::CbmGeoPassivePar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGeoPassivePar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmGeoPlatform(void *p = nullptr);
   static void *newArray_CbmGeoPlatform(Long_t size, void *p);
   static void delete_CbmGeoPlatform(void *p);
   static void deleteArray_CbmGeoPlatform(void *p);
   static void destruct_CbmGeoPlatform(void *p);
   static void streamer_CbmGeoPlatform(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGeoPlatform*)
   {
      ::CbmGeoPlatform *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGeoPlatform >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGeoPlatform", ::CbmGeoPlatform::Class_Version(), "CbmGeoPlatform.h", 10,
                  typeid(::CbmGeoPlatform), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGeoPlatform::Dictionary, isa_proxy, 16,
                  sizeof(::CbmGeoPlatform) );
      instance.SetNew(&new_CbmGeoPlatform);
      instance.SetNewArray(&newArray_CbmGeoPlatform);
      instance.SetDelete(&delete_CbmGeoPlatform);
      instance.SetDeleteArray(&deleteArray_CbmGeoPlatform);
      instance.SetDestructor(&destruct_CbmGeoPlatform);
      instance.SetStreamerFunc(&streamer_CbmGeoPlatform);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGeoPlatform*)
   {
      return GenerateInitInstanceLocal((::CbmGeoPlatform*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGeoPlatform*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMagnet(void *p = nullptr);
   static void *newArray_CbmMagnet(Long_t size, void *p);
   static void delete_CbmMagnet(void *p);
   static void deleteArray_CbmMagnet(void *p);
   static void destruct_CbmMagnet(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMagnet*)
   {
      ::CbmMagnet *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMagnet >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMagnet", ::CbmMagnet::Class_Version(), "CbmMagnet.h", 13,
                  typeid(::CbmMagnet), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMagnet::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMagnet) );
      instance.SetNew(&new_CbmMagnet);
      instance.SetNewArray(&newArray_CbmMagnet);
      instance.SetDelete(&delete_CbmMagnet);
      instance.SetDeleteArray(&deleteArray_CbmMagnet);
      instance.SetDestructor(&destruct_CbmMagnet);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMagnet*)
   {
      return GenerateInitInstanceLocal((::CbmMagnet*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMagnet*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPassiveContFact(void *p = nullptr);
   static void *newArray_CbmPassiveContFact(Long_t size, void *p);
   static void delete_CbmPassiveContFact(void *p);
   static void deleteArray_CbmPassiveContFact(void *p);
   static void destruct_CbmPassiveContFact(void *p);
   static void streamer_CbmPassiveContFact(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPassiveContFact*)
   {
      ::CbmPassiveContFact *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPassiveContFact >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPassiveContFact", ::CbmPassiveContFact::Class_Version(), "CbmPassiveContFact.h", 12,
                  typeid(::CbmPassiveContFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPassiveContFact::Dictionary, isa_proxy, 16,
                  sizeof(::CbmPassiveContFact) );
      instance.SetNew(&new_CbmPassiveContFact);
      instance.SetNewArray(&newArray_CbmPassiveContFact);
      instance.SetDelete(&delete_CbmPassiveContFact);
      instance.SetDeleteArray(&deleteArray_CbmPassiveContFact);
      instance.SetDestructor(&destruct_CbmPassiveContFact);
      instance.SetStreamerFunc(&streamer_CbmPassiveContFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPassiveContFact*)
   {
      return GenerateInitInstanceLocal((::CbmPassiveContFact*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPassiveContFact*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPipe(void *p = nullptr);
   static void *newArray_CbmPipe(Long_t size, void *p);
   static void delete_CbmPipe(void *p);
   static void deleteArray_CbmPipe(void *p);
   static void destruct_CbmPipe(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPipe*)
   {
      ::CbmPipe *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPipe >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPipe", ::CbmPipe::Class_Version(), "CbmPipe.h", 14,
                  typeid(::CbmPipe), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPipe::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPipe) );
      instance.SetNew(&new_CbmPipe);
      instance.SetNewArray(&newArray_CbmPipe);
      instance.SetDelete(&delete_CbmPipe);
      instance.SetDeleteArray(&deleteArray_CbmPipe);
      instance.SetDestructor(&destruct_CbmPipe);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPipe*)
   {
      return GenerateInitInstanceLocal((::CbmPipe*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPipe*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPlatform(void *p = nullptr);
   static void *newArray_CbmPlatform(Long_t size, void *p);
   static void delete_CbmPlatform(void *p);
   static void deleteArray_CbmPlatform(void *p);
   static void destruct_CbmPlatform(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPlatform*)
   {
      ::CbmPlatform *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPlatform >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPlatform", ::CbmPlatform::Class_Version(), "CbmPlatform.h", 14,
                  typeid(::CbmPlatform), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPlatform::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPlatform) );
      instance.SetNew(&new_CbmPlatform);
      instance.SetNewArray(&newArray_CbmPlatform);
      instance.SetDelete(&delete_CbmPlatform);
      instance.SetDeleteArray(&deleteArray_CbmPlatform);
      instance.SetDestructor(&destruct_CbmPlatform);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPlatform*)
   {
      return GenerateInitInstanceLocal((::CbmPlatform*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPlatform*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmCave::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCave::Class_Name()
{
   return "CbmCave";
}

//______________________________________________________________________________
const char *CbmCave::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCave*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCave::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCave*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCave::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCave*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCave::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCave*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGeoCave::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGeoCave::Class_Name()
{
   return "CbmGeoCave";
}

//______________________________________________________________________________
const char *CbmGeoCave::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoCave*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGeoCave::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoCave*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGeoCave::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoCave*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGeoCave::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoCave*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGeoPassivePar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGeoPassivePar::Class_Name()
{
   return "CbmGeoPassivePar";
}

//______________________________________________________________________________
const char *CbmGeoPassivePar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoPassivePar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGeoPassivePar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoPassivePar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGeoPassivePar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoPassivePar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGeoPassivePar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoPassivePar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGeoPlatform::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGeoPlatform::Class_Name()
{
   return "CbmGeoPlatform";
}

//______________________________________________________________________________
const char *CbmGeoPlatform::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoPlatform*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGeoPlatform::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoPlatform*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGeoPlatform::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoPlatform*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGeoPlatform::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoPlatform*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMagnet::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMagnet::Class_Name()
{
   return "CbmMagnet";
}

//______________________________________________________________________________
const char *CbmMagnet::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMagnet*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMagnet::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMagnet*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMagnet::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMagnet*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMagnet::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMagnet*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPassiveContFact::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPassiveContFact::Class_Name()
{
   return "CbmPassiveContFact";
}

//______________________________________________________________________________
const char *CbmPassiveContFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPassiveContFact*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPassiveContFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPassiveContFact*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPassiveContFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPassiveContFact*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPassiveContFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPassiveContFact*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPipe::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPipe::Class_Name()
{
   return "CbmPipe";
}

//______________________________________________________________________________
const char *CbmPipe::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPipe*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPipe::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPipe*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPipe::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPipe*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPipe::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPipe*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPlatform::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPlatform::Class_Name()
{
   return "CbmPlatform";
}

//______________________________________________________________________________
const char *CbmPlatform::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPlatform*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPlatform::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPlatform*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPlatform::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPlatform*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPlatform::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPlatform*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmCave::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCave.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmCave::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmCave::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmCave(void *p) {
      return  p ? new(p) ::CbmCave : new ::CbmCave;
   }
   static void *newArray_CbmCave(Long_t nElements, void *p) {
      return p ? new(p) ::CbmCave[nElements] : new ::CbmCave[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmCave(void *p) {
      delete ((::CbmCave*)p);
   }
   static void deleteArray_CbmCave(void *p) {
      delete [] ((::CbmCave*)p);
   }
   static void destruct_CbmCave(void *p) {
      typedef ::CbmCave current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmCave

//______________________________________________________________________________
void CbmGeoCave::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGeoCave.

   FairGeoSet::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGeoCave(void *p) {
      return  p ? new(p) ::CbmGeoCave : new ::CbmGeoCave;
   }
   static void *newArray_CbmGeoCave(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGeoCave[nElements] : new ::CbmGeoCave[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGeoCave(void *p) {
      delete ((::CbmGeoCave*)p);
   }
   static void deleteArray_CbmGeoCave(void *p) {
      delete [] ((::CbmGeoCave*)p);
   }
   static void destruct_CbmGeoCave(void *p) {
      typedef ::CbmGeoCave current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmGeoCave(TBuffer &buf, void *obj) {
      ((::CbmGeoCave*)obj)->::CbmGeoCave::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmGeoCave

//______________________________________________________________________________
void CbmGeoPassivePar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGeoPassivePar.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairParGenericSet::Streamer(R__b);
      R__b >> fGeoSensNodes;
      R__b >> fGeoPassNodes;
      R__b.CheckByteCount(R__s, R__c, CbmGeoPassivePar::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmGeoPassivePar::IsA(), kTRUE);
      FairParGenericSet::Streamer(R__b);
      R__b << fGeoSensNodes;
      R__b << fGeoPassNodes;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGeoPassivePar(void *p) {
      return  p ? new(p) ::CbmGeoPassivePar : new ::CbmGeoPassivePar;
   }
   static void *newArray_CbmGeoPassivePar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGeoPassivePar[nElements] : new ::CbmGeoPassivePar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGeoPassivePar(void *p) {
      delete ((::CbmGeoPassivePar*)p);
   }
   static void deleteArray_CbmGeoPassivePar(void *p) {
      delete [] ((::CbmGeoPassivePar*)p);
   }
   static void destruct_CbmGeoPassivePar(void *p) {
      typedef ::CbmGeoPassivePar current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmGeoPassivePar(TBuffer &buf, void *obj) {
      ((::CbmGeoPassivePar*)obj)->::CbmGeoPassivePar::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmGeoPassivePar

//______________________________________________________________________________
void CbmGeoPlatform::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGeoPlatform.

   FairGeoSet::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGeoPlatform(void *p) {
      return  p ? new(p) ::CbmGeoPlatform : new ::CbmGeoPlatform;
   }
   static void *newArray_CbmGeoPlatform(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGeoPlatform[nElements] : new ::CbmGeoPlatform[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGeoPlatform(void *p) {
      delete ((::CbmGeoPlatform*)p);
   }
   static void deleteArray_CbmGeoPlatform(void *p) {
      delete [] ((::CbmGeoPlatform*)p);
   }
   static void destruct_CbmGeoPlatform(void *p) {
      typedef ::CbmGeoPlatform current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmGeoPlatform(TBuffer &buf, void *obj) {
      ((::CbmGeoPlatform*)obj)->::CbmGeoPlatform::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmGeoPlatform

//______________________________________________________________________________
void CbmMagnet::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMagnet.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMagnet::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMagnet::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMagnet(void *p) {
      return  p ? new(p) ::CbmMagnet : new ::CbmMagnet;
   }
   static void *newArray_CbmMagnet(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMagnet[nElements] : new ::CbmMagnet[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMagnet(void *p) {
      delete ((::CbmMagnet*)p);
   }
   static void deleteArray_CbmMagnet(void *p) {
      delete [] ((::CbmMagnet*)p);
   }
   static void destruct_CbmMagnet(void *p) {
      typedef ::CbmMagnet current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMagnet

//______________________________________________________________________________
void CbmPassiveContFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPassiveContFact.

   FairContFact::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPassiveContFact(void *p) {
      return  p ? new(p) ::CbmPassiveContFact : new ::CbmPassiveContFact;
   }
   static void *newArray_CbmPassiveContFact(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPassiveContFact[nElements] : new ::CbmPassiveContFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPassiveContFact(void *p) {
      delete ((::CbmPassiveContFact*)p);
   }
   static void deleteArray_CbmPassiveContFact(void *p) {
      delete [] ((::CbmPassiveContFact*)p);
   }
   static void destruct_CbmPassiveContFact(void *p) {
      typedef ::CbmPassiveContFact current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmPassiveContFact(TBuffer &buf, void *obj) {
      ((::CbmPassiveContFact*)obj)->::CbmPassiveContFact::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmPassiveContFact

//______________________________________________________________________________
void CbmPipe::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPipe.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPipe::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPipe::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPipe(void *p) {
      return  p ? new(p) ::CbmPipe : new ::CbmPipe;
   }
   static void *newArray_CbmPipe(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPipe[nElements] : new ::CbmPipe[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPipe(void *p) {
      delete ((::CbmPipe*)p);
   }
   static void deleteArray_CbmPipe(void *p) {
      delete [] ((::CbmPipe*)p);
   }
   static void destruct_CbmPipe(void *p) {
      typedef ::CbmPipe current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPipe

//______________________________________________________________________________
void CbmPlatform::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPlatform.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPlatform::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPlatform::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPlatform(void *p) {
      return  p ? new(p) ::CbmPlatform : new ::CbmPlatform;
   }
   static void *newArray_CbmPlatform(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPlatform[nElements] : new ::CbmPlatform[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPlatform(void *p) {
      delete ((::CbmPlatform*)p);
   }
   static void deleteArray_CbmPlatform(void *p) {
      delete [] ((::CbmPlatform*)p);
   }
   static void destruct_CbmPlatform(void *p) {
      typedef ::CbmPlatform current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPlatform

namespace {
  void TriggerDictionaryInitialization_libCbmPassive_Impl() {
    static const char* headers[] = {
"CbmCave.h",
"CbmGeoCave.h",
"CbmGeoPassivePar.h",
"CbmGeoPlatform.h",
"CbmMagnet.h",
"CbmPassiveContFact.h",
"CbmPipe.h",
"CbmPlatform.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/passive",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/passive",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
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
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/sim/passive/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmPassive dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate(R"ATTRDUMP(CBMCaveSD)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmCave.h")))  CbmCave;
class __attribute__((annotate(R"ATTRDUMP(Class for the geometry of CAVE)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmGeoCave.h")))  CbmGeoCave;
class __attribute__((annotate("$clingAutoload$CbmGeoPassivePar.h")))  CbmGeoPassivePar;
class __attribute__((annotate(R"ATTRDUMP(Class for the geometry of Platform)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmGeoPlatform.h")))  CbmGeoPlatform;
class __attribute__((annotate(R"ATTRDUMP(CBMMagnet)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmMagnet.h")))  CbmMagnet;
class __attribute__((annotate(R"ATTRDUMP(Factory for all Passive parameter containers)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmPassiveContFact.h")))  CbmPassiveContFact;
class __attribute__((annotate(R"ATTRDUMP(CBMPIPE)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmPipe.h")))  CbmPipe;
class __attribute__((annotate(R"ATTRDUMP(CBMPLATFORM)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmPlatform.h")))  CbmPlatform;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmPassive dictionary payload"

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
#include "CbmCave.h"
#include "CbmGeoCave.h"
#include "CbmGeoPassivePar.h"
#include "CbmGeoPlatform.h"
#include "CbmMagnet.h"
#include "CbmPassiveContFact.h"
#include "CbmPipe.h"
#include "CbmPlatform.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmCave", payloadCode, "@",
"CbmGeoCave", payloadCode, "@",
"CbmGeoPassivePar", payloadCode, "@",
"CbmGeoPlatform", payloadCode, "@",
"CbmMagnet", payloadCode, "@",
"CbmPassiveContFact", payloadCode, "@",
"CbmPipe", payloadCode, "@",
"CbmPlatform", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmPassive",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmPassive_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmPassive_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmPassive() {
  TriggerDictionaryInitialization_libCbmPassive_Impl();
}
