// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmTofAna
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
#include "CbmTofAnaTestbeam.h"
#include "CbmTofGeometryQa.h"
#include "CbmTofHitFinderQa.h"
#include "CbmTofHitFinderTBQa.h"
#include "CbmTofTests.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmTofAnaTestbeam(void *p = nullptr);
   static void *newArray_CbmTofAnaTestbeam(Long_t size, void *p);
   static void delete_CbmTofAnaTestbeam(void *p);
   static void deleteArray_CbmTofAnaTestbeam(void *p);
   static void destruct_CbmTofAnaTestbeam(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofAnaTestbeam*)
   {
      ::CbmTofAnaTestbeam *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofAnaTestbeam >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofAnaTestbeam", ::CbmTofAnaTestbeam::Class_Version(), "CbmTofAnaTestbeam.h", 49,
                  typeid(::CbmTofAnaTestbeam), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofAnaTestbeam::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofAnaTestbeam) );
      instance.SetNew(&new_CbmTofAnaTestbeam);
      instance.SetNewArray(&newArray_CbmTofAnaTestbeam);
      instance.SetDelete(&delete_CbmTofAnaTestbeam);
      instance.SetDeleteArray(&deleteArray_CbmTofAnaTestbeam);
      instance.SetDestructor(&destruct_CbmTofAnaTestbeam);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofAnaTestbeam*)
   {
      return GenerateInitInstanceLocal((::CbmTofAnaTestbeam*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofAnaTestbeam*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofGeometryQa(void *p = nullptr);
   static void *newArray_CbmTofGeometryQa(Long_t size, void *p);
   static void delete_CbmTofGeometryQa(void *p);
   static void deleteArray_CbmTofGeometryQa(void *p);
   static void destruct_CbmTofGeometryQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofGeometryQa*)
   {
      ::CbmTofGeometryQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofGeometryQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofGeometryQa", ::CbmTofGeometryQa::Class_Version(), "CbmTofGeometryQa.h", 38,
                  typeid(::CbmTofGeometryQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofGeometryQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofGeometryQa) );
      instance.SetNew(&new_CbmTofGeometryQa);
      instance.SetNewArray(&newArray_CbmTofGeometryQa);
      instance.SetDelete(&delete_CbmTofGeometryQa);
      instance.SetDeleteArray(&deleteArray_CbmTofGeometryQa);
      instance.SetDestructor(&destruct_CbmTofGeometryQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofGeometryQa*)
   {
      return GenerateInitInstanceLocal((::CbmTofGeometryQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofGeometryQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofHitFinderQa(void *p = nullptr);
   static void *newArray_CbmTofHitFinderQa(Long_t size, void *p);
   static void delete_CbmTofHitFinderQa(void *p);
   static void deleteArray_CbmTofHitFinderQa(void *p);
   static void destruct_CbmTofHitFinderQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofHitFinderQa*)
   {
      ::CbmTofHitFinderQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofHitFinderQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofHitFinderQa", ::CbmTofHitFinderQa::Class_Version(), "CbmTofHitFinderQa.h", 38,
                  typeid(::CbmTofHitFinderQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofHitFinderQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofHitFinderQa) );
      instance.SetNew(&new_CbmTofHitFinderQa);
      instance.SetNewArray(&newArray_CbmTofHitFinderQa);
      instance.SetDelete(&delete_CbmTofHitFinderQa);
      instance.SetDeleteArray(&deleteArray_CbmTofHitFinderQa);
      instance.SetDestructor(&destruct_CbmTofHitFinderQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofHitFinderQa*)
   {
      return GenerateInitInstanceLocal((::CbmTofHitFinderQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofHitFinderQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofHitFinderTBQa(void *p = nullptr);
   static void *newArray_CbmTofHitFinderTBQa(Long_t size, void *p);
   static void delete_CbmTofHitFinderTBQa(void *p);
   static void deleteArray_CbmTofHitFinderTBQa(void *p);
   static void destruct_CbmTofHitFinderTBQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofHitFinderTBQa*)
   {
      ::CbmTofHitFinderTBQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofHitFinderTBQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofHitFinderTBQa", ::CbmTofHitFinderTBQa::Class_Version(), "CbmTofHitFinderTBQa.h", 16,
                  typeid(::CbmTofHitFinderTBQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofHitFinderTBQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofHitFinderTBQa) );
      instance.SetNew(&new_CbmTofHitFinderTBQa);
      instance.SetNewArray(&newArray_CbmTofHitFinderTBQa);
      instance.SetDelete(&delete_CbmTofHitFinderTBQa);
      instance.SetDeleteArray(&deleteArray_CbmTofHitFinderTBQa);
      instance.SetDestructor(&destruct_CbmTofHitFinderTBQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofHitFinderTBQa*)
   {
      return GenerateInitInstanceLocal((::CbmTofHitFinderTBQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofHitFinderTBQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofTests(void *p = nullptr);
   static void *newArray_CbmTofTests(Long_t size, void *p);
   static void delete_CbmTofTests(void *p);
   static void deleteArray_CbmTofTests(void *p);
   static void destruct_CbmTofTests(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofTests*)
   {
      ::CbmTofTests *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofTests >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofTests", ::CbmTofTests::Class_Version(), "CbmTofTests.h", 31,
                  typeid(::CbmTofTests), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofTests::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofTests) );
      instance.SetNew(&new_CbmTofTests);
      instance.SetNewArray(&newArray_CbmTofTests);
      instance.SetDelete(&delete_CbmTofTests);
      instance.SetDeleteArray(&deleteArray_CbmTofTests);
      instance.SetDestructor(&destruct_CbmTofTests);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofTests*)
   {
      return GenerateInitInstanceLocal((::CbmTofTests*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofTests*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmTofAnaTestbeam::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofAnaTestbeam::Class_Name()
{
   return "CbmTofAnaTestbeam";
}

//______________________________________________________________________________
const char *CbmTofAnaTestbeam::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofAnaTestbeam*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofAnaTestbeam::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofAnaTestbeam*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofAnaTestbeam::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofAnaTestbeam*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofAnaTestbeam::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofAnaTestbeam*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofGeometryQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofGeometryQa::Class_Name()
{
   return "CbmTofGeometryQa";
}

//______________________________________________________________________________
const char *CbmTofGeometryQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofGeometryQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofGeometryQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofGeometryQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofGeometryQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofGeometryQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofGeometryQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofGeometryQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofHitFinderQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofHitFinderQa::Class_Name()
{
   return "CbmTofHitFinderQa";
}

//______________________________________________________________________________
const char *CbmTofHitFinderQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitFinderQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofHitFinderQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitFinderQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofHitFinderQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitFinderQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofHitFinderQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitFinderQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofHitFinderTBQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofHitFinderTBQa::Class_Name()
{
   return "CbmTofHitFinderTBQa";
}

//______________________________________________________________________________
const char *CbmTofHitFinderTBQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitFinderTBQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofHitFinderTBQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitFinderTBQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofHitFinderTBQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitFinderTBQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofHitFinderTBQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitFinderTBQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofTests::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofTests::Class_Name()
{
   return "CbmTofTests";
}

//______________________________________________________________________________
const char *CbmTofTests::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTests*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofTests::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTests*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofTests::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTests*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofTests::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTests*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmTofAnaTestbeam::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofAnaTestbeam.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofAnaTestbeam::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofAnaTestbeam::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofAnaTestbeam(void *p) {
      return  p ? new(p) ::CbmTofAnaTestbeam : new ::CbmTofAnaTestbeam;
   }
   static void *newArray_CbmTofAnaTestbeam(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofAnaTestbeam[nElements] : new ::CbmTofAnaTestbeam[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofAnaTestbeam(void *p) {
      delete ((::CbmTofAnaTestbeam*)p);
   }
   static void deleteArray_CbmTofAnaTestbeam(void *p) {
      delete [] ((::CbmTofAnaTestbeam*)p);
   }
   static void destruct_CbmTofAnaTestbeam(void *p) {
      typedef ::CbmTofAnaTestbeam current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofAnaTestbeam

//______________________________________________________________________________
void CbmTofGeometryQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofGeometryQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofGeometryQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofGeometryQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofGeometryQa(void *p) {
      return  p ? new(p) ::CbmTofGeometryQa : new ::CbmTofGeometryQa;
   }
   static void *newArray_CbmTofGeometryQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofGeometryQa[nElements] : new ::CbmTofGeometryQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofGeometryQa(void *p) {
      delete ((::CbmTofGeometryQa*)p);
   }
   static void deleteArray_CbmTofGeometryQa(void *p) {
      delete [] ((::CbmTofGeometryQa*)p);
   }
   static void destruct_CbmTofGeometryQa(void *p) {
      typedef ::CbmTofGeometryQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofGeometryQa

//______________________________________________________________________________
void CbmTofHitFinderQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofHitFinderQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofHitFinderQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofHitFinderQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofHitFinderQa(void *p) {
      return  p ? new(p) ::CbmTofHitFinderQa : new ::CbmTofHitFinderQa;
   }
   static void *newArray_CbmTofHitFinderQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofHitFinderQa[nElements] : new ::CbmTofHitFinderQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofHitFinderQa(void *p) {
      delete ((::CbmTofHitFinderQa*)p);
   }
   static void deleteArray_CbmTofHitFinderQa(void *p) {
      delete [] ((::CbmTofHitFinderQa*)p);
   }
   static void destruct_CbmTofHitFinderQa(void *p) {
      typedef ::CbmTofHitFinderQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofHitFinderQa

//______________________________________________________________________________
void CbmTofHitFinderTBQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofHitFinderTBQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofHitFinderTBQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofHitFinderTBQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofHitFinderTBQa(void *p) {
      return  p ? new(p) ::CbmTofHitFinderTBQa : new ::CbmTofHitFinderTBQa;
   }
   static void *newArray_CbmTofHitFinderTBQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofHitFinderTBQa[nElements] : new ::CbmTofHitFinderTBQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofHitFinderTBQa(void *p) {
      delete ((::CbmTofHitFinderTBQa*)p);
   }
   static void deleteArray_CbmTofHitFinderTBQa(void *p) {
      delete [] ((::CbmTofHitFinderTBQa*)p);
   }
   static void destruct_CbmTofHitFinderTBQa(void *p) {
      typedef ::CbmTofHitFinderTBQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofHitFinderTBQa

//______________________________________________________________________________
void CbmTofTests::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofTests.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofTests::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofTests::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofTests(void *p) {
      return  p ? new(p) ::CbmTofTests : new ::CbmTofTests;
   }
   static void *newArray_CbmTofTests(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofTests[nElements] : new ::CbmTofTests[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofTests(void *p) {
      delete ((::CbmTofTests*)p);
   }
   static void deleteArray_CbmTofTests(void *p) {
      delete [] ((::CbmTofTests*)p);
   }
   static void destruct_CbmTofTests(void *p) {
      typedef ::CbmTofTests current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofTests

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlEintgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEintgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlEintgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<int> > >*)
   {
      vector<vector<vector<int> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<int> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<int> > >", -2, "vector", 389,
                  typeid(vector<vector<vector<int> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEintgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<int> > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEintgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEintgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEintgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEintgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEintgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<int> > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<int> > >","std::vector<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >, std::allocator<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<int> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEintgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<int> > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlEintgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEintgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<int> > > : new vector<vector<vector<int> > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEintgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<int> > >[nElements] : new vector<vector<vector<int> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<int> > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<int> > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<int> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<int> > >

namespace ROOT {
   static TClass *vectorlEvectorlEintgRsPgR_Dictionary();
   static void vectorlEvectorlEintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEintgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEintgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEintgRsPgR(void *p);
   static void destruct_vectorlEvectorlEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<int> >*)
   {
      vector<vector<int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<int> >", -2, "vector", 389,
                  typeid(vector<vector<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<int> >) );
      instance.SetNew(&new_vectorlEvectorlEintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEintgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<int> > >()));

      ::ROOT::AddClassAlternate("vector<vector<int> >","std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<int> >*)nullptr)->GetClass();
      vectorlEvectorlEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<int> > : new vector<vector<int> >;
   }
   static void *newArray_vectorlEvectorlEintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<int> >[nElements] : new vector<vector<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEintgRsPgR(void *p) {
      delete ((vector<vector<int> >*)p);
   }
   static void deleteArray_vectorlEvectorlEintgRsPgR(void *p) {
      delete [] ((vector<vector<int> >*)p);
   }
   static void destruct_vectorlEvectorlEintgRsPgR(void *p) {
      typedef vector<vector<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<int> >

namespace ROOT {
   static TClass *vectorlEvectorlEULong64_tgRsPgR_Dictionary();
   static void vectorlEvectorlEULong64_tgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEULong64_tgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEULong64_tgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEULong64_tgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEULong64_tgRsPgR(void *p);
   static void destruct_vectorlEvectorlEULong64_tgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<ULong64_t> >*)
   {
      vector<vector<ULong64_t> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<ULong64_t> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<ULong64_t> >", -2, "vector", 389,
                  typeid(vector<vector<ULong64_t> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEULong64_tgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<ULong64_t> >) );
      instance.SetNew(&new_vectorlEvectorlEULong64_tgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEULong64_tgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEULong64_tgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEULong64_tgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEULong64_tgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<ULong64_t> > >()));

      ::ROOT::AddClassAlternate("vector<vector<ULong64_t> >","std::vector<std::vector<unsigned long long, std::allocator<unsigned long long> >, std::allocator<std::vector<unsigned long long, std::allocator<unsigned long long> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<ULong64_t> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEULong64_tgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<ULong64_t> >*)nullptr)->GetClass();
      vectorlEvectorlEULong64_tgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEULong64_tgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEULong64_tgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<ULong64_t> > : new vector<vector<ULong64_t> >;
   }
   static void *newArray_vectorlEvectorlEULong64_tgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<ULong64_t> >[nElements] : new vector<vector<ULong64_t> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEULong64_tgRsPgR(void *p) {
      delete ((vector<vector<ULong64_t> >*)p);
   }
   static void deleteArray_vectorlEvectorlEULong64_tgRsPgR(void *p) {
      delete [] ((vector<vector<ULong64_t> >*)p);
   }
   static void destruct_vectorlEvectorlEULong64_tgRsPgR(void *p) {
      typedef vector<vector<ULong64_t> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<ULong64_t> >

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
   static TClass *vectorlEULong64_tgR_Dictionary();
   static void vectorlEULong64_tgR_TClassManip(TClass*);
   static void *new_vectorlEULong64_tgR(void *p = nullptr);
   static void *newArray_vectorlEULong64_tgR(Long_t size, void *p);
   static void delete_vectorlEULong64_tgR(void *p);
   static void deleteArray_vectorlEULong64_tgR(void *p);
   static void destruct_vectorlEULong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ULong64_t>*)
   {
      vector<ULong64_t> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ULong64_t>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ULong64_t>", -2, "vector", 389,
                  typeid(vector<ULong64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEULong64_tgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ULong64_t>) );
      instance.SetNew(&new_vectorlEULong64_tgR);
      instance.SetNewArray(&newArray_vectorlEULong64_tgR);
      instance.SetDelete(&delete_vectorlEULong64_tgR);
      instance.SetDeleteArray(&deleteArray_vectorlEULong64_tgR);
      instance.SetDestructor(&destruct_vectorlEULong64_tgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ULong64_t> >()));

      ::ROOT::AddClassAlternate("vector<ULong64_t>","std::vector<unsigned long long, std::allocator<unsigned long long> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<ULong64_t>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEULong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<ULong64_t>*)nullptr)->GetClass();
      vectorlEULong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEULong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEULong64_tgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ULong64_t> : new vector<ULong64_t>;
   }
   static void *newArray_vectorlEULong64_tgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ULong64_t>[nElements] : new vector<ULong64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEULong64_tgR(void *p) {
      delete ((vector<ULong64_t>*)p);
   }
   static void deleteArray_vectorlEULong64_tgR(void *p) {
      delete [] ((vector<ULong64_t>*)p);
   }
   static void destruct_vectorlEULong64_tgR(void *p) {
      typedef vector<ULong64_t> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<ULong64_t>

namespace ROOT {
   static TClass *vectorlETH3mUgR_Dictionary();
   static void vectorlETH3mUgR_TClassManip(TClass*);
   static void *new_vectorlETH3mUgR(void *p = nullptr);
   static void *newArray_vectorlETH3mUgR(Long_t size, void *p);
   static void delete_vectorlETH3mUgR(void *p);
   static void deleteArray_vectorlETH3mUgR(void *p);
   static void destruct_vectorlETH3mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TH3*>*)
   {
      vector<TH3*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TH3*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TH3*>", -2, "vector", 389,
                  typeid(vector<TH3*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETH3mUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TH3*>) );
      instance.SetNew(&new_vectorlETH3mUgR);
      instance.SetNewArray(&newArray_vectorlETH3mUgR);
      instance.SetDelete(&delete_vectorlETH3mUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETH3mUgR);
      instance.SetDestructor(&destruct_vectorlETH3mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TH3*> >()));

      ::ROOT::AddClassAlternate("vector<TH3*>","std::vector<TH3*, std::allocator<TH3*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TH3*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETH3mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TH3*>*)nullptr)->GetClass();
      vectorlETH3mUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETH3mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETH3mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH3*> : new vector<TH3*>;
   }
   static void *newArray_vectorlETH3mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH3*>[nElements] : new vector<TH3*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETH3mUgR(void *p) {
      delete ((vector<TH3*>*)p);
   }
   static void deleteArray_vectorlETH3mUgR(void *p) {
      delete [] ((vector<TH3*>*)p);
   }
   static void destruct_vectorlETH3mUgR(void *p) {
      typedef vector<TH3*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TH3*>

namespace ROOT {
   static TClass *vectorlETH2mUgR_Dictionary();
   static void vectorlETH2mUgR_TClassManip(TClass*);
   static void *new_vectorlETH2mUgR(void *p = nullptr);
   static void *newArray_vectorlETH2mUgR(Long_t size, void *p);
   static void delete_vectorlETH2mUgR(void *p);
   static void deleteArray_vectorlETH2mUgR(void *p);
   static void destruct_vectorlETH2mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TH2*>*)
   {
      vector<TH2*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TH2*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TH2*>", -2, "vector", 389,
                  typeid(vector<TH2*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETH2mUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TH2*>) );
      instance.SetNew(&new_vectorlETH2mUgR);
      instance.SetNewArray(&newArray_vectorlETH2mUgR);
      instance.SetDelete(&delete_vectorlETH2mUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETH2mUgR);
      instance.SetDestructor(&destruct_vectorlETH2mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TH2*> >()));

      ::ROOT::AddClassAlternate("vector<TH2*>","std::vector<TH2*, std::allocator<TH2*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TH2*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETH2mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TH2*>*)nullptr)->GetClass();
      vectorlETH2mUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETH2mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETH2mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH2*> : new vector<TH2*>;
   }
   static void *newArray_vectorlETH2mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH2*>[nElements] : new vector<TH2*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETH2mUgR(void *p) {
      delete ((vector<TH2*>*)p);
   }
   static void deleteArray_vectorlETH2mUgR(void *p) {
      delete [] ((vector<TH2*>*)p);
   }
   static void destruct_vectorlETH2mUgR(void *p) {
      typedef vector<TH2*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TH2*>

namespace ROOT {
   static TClass *vectorlETH1mUgR_Dictionary();
   static void vectorlETH1mUgR_TClassManip(TClass*);
   static void *new_vectorlETH1mUgR(void *p = nullptr);
   static void *newArray_vectorlETH1mUgR(Long_t size, void *p);
   static void delete_vectorlETH1mUgR(void *p);
   static void deleteArray_vectorlETH1mUgR(void *p);
   static void destruct_vectorlETH1mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TH1*>*)
   {
      vector<TH1*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TH1*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TH1*>", -2, "vector", 389,
                  typeid(vector<TH1*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETH1mUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TH1*>) );
      instance.SetNew(&new_vectorlETH1mUgR);
      instance.SetNewArray(&newArray_vectorlETH1mUgR);
      instance.SetDelete(&delete_vectorlETH1mUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETH1mUgR);
      instance.SetDestructor(&destruct_vectorlETH1mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TH1*> >()));

      ::ROOT::AddClassAlternate("vector<TH1*>","std::vector<TH1*, std::allocator<TH1*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TH1*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETH1mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TH1*>*)nullptr)->GetClass();
      vectorlETH1mUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETH1mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETH1mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH1*> : new vector<TH1*>;
   }
   static void *newArray_vectorlETH1mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH1*>[nElements] : new vector<TH1*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETH1mUgR(void *p) {
      delete ((vector<TH1*>*)p);
   }
   static void deleteArray_vectorlETH1mUgR(void *p) {
      delete [] ((vector<TH1*>*)p);
   }
   static void destruct_vectorlETH1mUgR(void *p) {
      typedef vector<TH1*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TH1*>

namespace ROOT {
   static TClass *maplEtuplelEintcOintcOintgRcOTH2mUgR_Dictionary();
   static void maplEtuplelEintcOintcOintgRcOTH2mUgR_TClassManip(TClass*);
   static void *new_maplEtuplelEintcOintcOintgRcOTH2mUgR(void *p = nullptr);
   static void *newArray_maplEtuplelEintcOintcOintgRcOTH2mUgR(Long_t size, void *p);
   static void delete_maplEtuplelEintcOintcOintgRcOTH2mUgR(void *p);
   static void deleteArray_maplEtuplelEintcOintcOintgRcOTH2mUgR(void *p);
   static void destruct_maplEtuplelEintcOintcOintgRcOTH2mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<tuple<int,int,int>,TH2*>*)
   {
      map<tuple<int,int,int>,TH2*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<tuple<int,int,int>,TH2*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<tuple<int,int,int>,TH2*>", -2, "map", 100,
                  typeid(map<tuple<int,int,int>,TH2*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEtuplelEintcOintcOintgRcOTH2mUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<tuple<int,int,int>,TH2*>) );
      instance.SetNew(&new_maplEtuplelEintcOintcOintgRcOTH2mUgR);
      instance.SetNewArray(&newArray_maplEtuplelEintcOintcOintgRcOTH2mUgR);
      instance.SetDelete(&delete_maplEtuplelEintcOintcOintgRcOTH2mUgR);
      instance.SetDeleteArray(&deleteArray_maplEtuplelEintcOintcOintgRcOTH2mUgR);
      instance.SetDestructor(&destruct_maplEtuplelEintcOintcOintgRcOTH2mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<tuple<int,int,int>,TH2*> >()));

      ::ROOT::AddClassAlternate("map<tuple<int,int,int>,TH2*>","std::map<std::tuple<int, int, int>, TH2*, std::less<std::tuple<int, int, int> >, std::allocator<std::pair<std::tuple<int, int, int> const, TH2*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<tuple<int,int,int>,TH2*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEtuplelEintcOintcOintgRcOTH2mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<tuple<int,int,int>,TH2*>*)nullptr)->GetClass();
      maplEtuplelEintcOintcOintgRcOTH2mUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEtuplelEintcOintcOintgRcOTH2mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEtuplelEintcOintcOintgRcOTH2mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<tuple<int,int,int>,TH2*> : new map<tuple<int,int,int>,TH2*>;
   }
   static void *newArray_maplEtuplelEintcOintcOintgRcOTH2mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<tuple<int,int,int>,TH2*>[nElements] : new map<tuple<int,int,int>,TH2*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEtuplelEintcOintcOintgRcOTH2mUgR(void *p) {
      delete ((map<tuple<int,int,int>,TH2*>*)p);
   }
   static void deleteArray_maplEtuplelEintcOintcOintgRcOTH2mUgR(void *p) {
      delete [] ((map<tuple<int,int,int>,TH2*>*)p);
   }
   static void destruct_maplEtuplelEintcOintcOintgRcOTH2mUgR(void *p) {
      typedef map<tuple<int,int,int>,TH2*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<tuple<int,int,int>,TH2*>

namespace ROOT {
   static TClass *maplEtuplelEintcOintcOintgRcOTH1mUgR_Dictionary();
   static void maplEtuplelEintcOintcOintgRcOTH1mUgR_TClassManip(TClass*);
   static void *new_maplEtuplelEintcOintcOintgRcOTH1mUgR(void *p = nullptr);
   static void *newArray_maplEtuplelEintcOintcOintgRcOTH1mUgR(Long_t size, void *p);
   static void delete_maplEtuplelEintcOintcOintgRcOTH1mUgR(void *p);
   static void deleteArray_maplEtuplelEintcOintcOintgRcOTH1mUgR(void *p);
   static void destruct_maplEtuplelEintcOintcOintgRcOTH1mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<tuple<int,int,int>,TH1*>*)
   {
      map<tuple<int,int,int>,TH1*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<tuple<int,int,int>,TH1*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<tuple<int,int,int>,TH1*>", -2, "map", 100,
                  typeid(map<tuple<int,int,int>,TH1*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEtuplelEintcOintcOintgRcOTH1mUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<tuple<int,int,int>,TH1*>) );
      instance.SetNew(&new_maplEtuplelEintcOintcOintgRcOTH1mUgR);
      instance.SetNewArray(&newArray_maplEtuplelEintcOintcOintgRcOTH1mUgR);
      instance.SetDelete(&delete_maplEtuplelEintcOintcOintgRcOTH1mUgR);
      instance.SetDeleteArray(&deleteArray_maplEtuplelEintcOintcOintgRcOTH1mUgR);
      instance.SetDestructor(&destruct_maplEtuplelEintcOintcOintgRcOTH1mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<tuple<int,int,int>,TH1*> >()));

      ::ROOT::AddClassAlternate("map<tuple<int,int,int>,TH1*>","std::map<std::tuple<int, int, int>, TH1*, std::less<std::tuple<int, int, int> >, std::allocator<std::pair<std::tuple<int, int, int> const, TH1*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<tuple<int,int,int>,TH1*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEtuplelEintcOintcOintgRcOTH1mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<tuple<int,int,int>,TH1*>*)nullptr)->GetClass();
      maplEtuplelEintcOintcOintgRcOTH1mUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEtuplelEintcOintcOintgRcOTH1mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEtuplelEintcOintcOintgRcOTH1mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<tuple<int,int,int>,TH1*> : new map<tuple<int,int,int>,TH1*>;
   }
   static void *newArray_maplEtuplelEintcOintcOintgRcOTH1mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<tuple<int,int,int>,TH1*>[nElements] : new map<tuple<int,int,int>,TH1*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEtuplelEintcOintcOintgRcOTH1mUgR(void *p) {
      delete ((map<tuple<int,int,int>,TH1*>*)p);
   }
   static void deleteArray_maplEtuplelEintcOintcOintgRcOTH1mUgR(void *p) {
      delete [] ((map<tuple<int,int,int>,TH1*>*)p);
   }
   static void destruct_maplEtuplelEintcOintcOintgRcOTH1mUgR(void *p) {
      typedef map<tuple<int,int,int>,TH1*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<tuple<int,int,int>,TH1*>

namespace ROOT {
   static TClass *maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR_Dictionary();
   static void maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR_TClassManip(TClass*);
   static void *new_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR(void *p = nullptr);
   static void *newArray_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR(Long_t size, void *p);
   static void delete_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR(void *p);
   static void deleteArray_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR(void *p);
   static void destruct_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<tuple<int,int,int>,TGeoPhysicalNode*>*)
   {
      map<tuple<int,int,int>,TGeoPhysicalNode*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<tuple<int,int,int>,TGeoPhysicalNode*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<tuple<int,int,int>,TGeoPhysicalNode*>", -2, "map", 100,
                  typeid(map<tuple<int,int,int>,TGeoPhysicalNode*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<tuple<int,int,int>,TGeoPhysicalNode*>) );
      instance.SetNew(&new_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR);
      instance.SetNewArray(&newArray_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR);
      instance.SetDelete(&delete_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR);
      instance.SetDeleteArray(&deleteArray_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR);
      instance.SetDestructor(&destruct_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<tuple<int,int,int>,TGeoPhysicalNode*> >()));

      ::ROOT::AddClassAlternate("map<tuple<int,int,int>,TGeoPhysicalNode*>","std::map<std::tuple<int, int, int>, TGeoPhysicalNode*, std::less<std::tuple<int, int, int> >, std::allocator<std::pair<std::tuple<int, int, int> const, TGeoPhysicalNode*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<tuple<int,int,int>,TGeoPhysicalNode*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<tuple<int,int,int>,TGeoPhysicalNode*>*)nullptr)->GetClass();
      maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<tuple<int,int,int>,TGeoPhysicalNode*> : new map<tuple<int,int,int>,TGeoPhysicalNode*>;
   }
   static void *newArray_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<tuple<int,int,int>,TGeoPhysicalNode*>[nElements] : new map<tuple<int,int,int>,TGeoPhysicalNode*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR(void *p) {
      delete ((map<tuple<int,int,int>,TGeoPhysicalNode*>*)p);
   }
   static void deleteArray_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR(void *p) {
      delete [] ((map<tuple<int,int,int>,TGeoPhysicalNode*>*)p);
   }
   static void destruct_maplEtuplelEintcOintcOintgRcOTGeoPhysicalNodemUgR(void *p) {
      typedef map<tuple<int,int,int>,TGeoPhysicalNode*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<tuple<int,int,int>,TGeoPhysicalNode*>

namespace {
  void TriggerDictionaryInitialization_libCbmTofAna_Impl() {
    static const char* headers[] = {
"CbmTofAnaTestbeam.h",
"CbmTofGeometryQa.h",
"CbmTofHitFinderQa.h",
"CbmTofHitFinderTBQa.h",
"CbmTofTests.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/detectors/tof",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/detectors/tof",
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
"/home/heinemann/gpuunpacker/cbmroot/fles/flestools",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/tof/unpack",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/tof",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/detectors/tof/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmTofAna dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmTofAnaTestbeam.h")))  CbmTofAnaTestbeam;
class __attribute__((annotate("$clingAutoload$CbmTofGeometryQa.h")))  CbmTofGeometryQa;
class __attribute__((annotate("$clingAutoload$CbmTofHitFinderQa.h")))  CbmTofHitFinderQa;
class __attribute__((annotate("$clingAutoload$CbmTofHitFinderTBQa.h")))  CbmTofHitFinderTBQa;
class __attribute__((annotate("$clingAutoload$CbmTofTests.h")))  CbmTofTests;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmTofAna dictionary payload"

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
#include "CbmTofAnaTestbeam.h"
#include "CbmTofGeometryQa.h"
#include "CbmTofHitFinderQa.h"
#include "CbmTofHitFinderTBQa.h"
#include "CbmTofTests.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmTofAnaTestbeam", payloadCode, "@",
"CbmTofGeometryQa", payloadCode, "@",
"CbmTofHitFinderQa", payloadCode, "@",
"CbmTofHitFinderTBQa", payloadCode, "@",
"CbmTofTests", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmTofAna",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmTofAna_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmTofAna_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmTofAna() {
  TriggerDictionaryInitialization_libCbmTofAna_Impl();
}
