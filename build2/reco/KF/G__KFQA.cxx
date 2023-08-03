// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__KFQA
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
#include "CbmKFPartEfficiencies.h"
#include "CbmKFTrErrMCPoints.h"
#include "CbmKFTrackFitQa.h"
#include "CbmKFTrackQa.h"
#include "KFParticleMatch.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmKFPartEfficiencies(void *p = nullptr);
   static void *newArray_CbmKFPartEfficiencies(Long_t size, void *p);
   static void delete_CbmKFPartEfficiencies(void *p);
   static void deleteArray_CbmKFPartEfficiencies(void *p);
   static void destruct_CbmKFPartEfficiencies(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFPartEfficiencies*)
   {
      ::CbmKFPartEfficiencies *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFPartEfficiencies >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFPartEfficiencies", ::CbmKFPartEfficiencies::Class_Version(), "CbmKFPartEfficiencies.h", 12,
                  typeid(::CbmKFPartEfficiencies), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFPartEfficiencies::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFPartEfficiencies) );
      instance.SetNew(&new_CbmKFPartEfficiencies);
      instance.SetNewArray(&newArray_CbmKFPartEfficiencies);
      instance.SetDelete(&delete_CbmKFPartEfficiencies);
      instance.SetDeleteArray(&deleteArray_CbmKFPartEfficiencies);
      instance.SetDestructor(&destruct_CbmKFPartEfficiencies);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFPartEfficiencies*)
   {
      return GenerateInitInstanceLocal((::CbmKFPartEfficiencies*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFPartEfficiencies*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFTrErrMCPoints(void *p = nullptr);
   static void *newArray_CbmKFTrErrMCPoints(Long_t size, void *p);
   static void delete_CbmKFTrErrMCPoints(void *p);
   static void deleteArray_CbmKFTrErrMCPoints(void *p);
   static void destruct_CbmKFTrErrMCPoints(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFTrErrMCPoints*)
   {
      ::CbmKFTrErrMCPoints *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFTrErrMCPoints >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFTrErrMCPoints", ::CbmKFTrErrMCPoints::Class_Version(), "CbmKFTrErrMCPoints.h", 26,
                  typeid(::CbmKFTrErrMCPoints), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFTrErrMCPoints::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFTrErrMCPoints) );
      instance.SetNew(&new_CbmKFTrErrMCPoints);
      instance.SetNewArray(&newArray_CbmKFTrErrMCPoints);
      instance.SetDelete(&delete_CbmKFTrErrMCPoints);
      instance.SetDeleteArray(&deleteArray_CbmKFTrErrMCPoints);
      instance.SetDestructor(&destruct_CbmKFTrErrMCPoints);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFTrErrMCPoints*)
   {
      return GenerateInitInstanceLocal((::CbmKFTrErrMCPoints*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFTrErrMCPoints*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFTrackFitQa(void *p = nullptr);
   static void *newArray_CbmKFTrackFitQa(Long_t size, void *p);
   static void delete_CbmKFTrackFitQa(void *p);
   static void deleteArray_CbmKFTrackFitQa(void *p);
   static void destruct_CbmKFTrackFitQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFTrackFitQa*)
   {
      ::CbmKFTrackFitQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFTrackFitQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFTrackFitQa", ::CbmKFTrackFitQa::Class_Version(), "CbmKFTrackFitQa.h", 37,
                  typeid(::CbmKFTrackFitQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFTrackFitQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFTrackFitQa) );
      instance.SetNew(&new_CbmKFTrackFitQa);
      instance.SetNewArray(&newArray_CbmKFTrackFitQa);
      instance.SetDelete(&delete_CbmKFTrackFitQa);
      instance.SetDeleteArray(&deleteArray_CbmKFTrackFitQa);
      instance.SetDestructor(&destruct_CbmKFTrackFitQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFTrackFitQa*)
   {
      return GenerateInitInstanceLocal((::CbmKFTrackFitQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFTrackFitQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFTrackQa(void *p = nullptr);
   static void *newArray_CbmKFTrackQa(Long_t size, void *p);
   static void delete_CbmKFTrackQa(void *p);
   static void deleteArray_CbmKFTrackQa(void *p);
   static void destruct_CbmKFTrackQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFTrackQa*)
   {
      ::CbmKFTrackQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFTrackQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFTrackQa", ::CbmKFTrackQa::Class_Version(), "CbmKFTrackQa.h", 30,
                  typeid(::CbmKFTrackQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFTrackQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFTrackQa) );
      instance.SetNew(&new_CbmKFTrackQa);
      instance.SetNewArray(&newArray_CbmKFTrackQa);
      instance.SetDelete(&delete_CbmKFTrackQa);
      instance.SetDeleteArray(&deleteArray_CbmKFTrackQa);
      instance.SetDestructor(&destruct_CbmKFTrackQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFTrackQa*)
   {
      return GenerateInitInstanceLocal((::CbmKFTrackQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFTrackQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_KFParticleMatch(void *p = nullptr);
   static void *newArray_KFParticleMatch(Long_t size, void *p);
   static void delete_KFParticleMatch(void *p);
   static void deleteArray_KFParticleMatch(void *p);
   static void destruct_KFParticleMatch(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::KFParticleMatch*)
   {
      ::KFParticleMatch *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::KFParticleMatch >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("KFParticleMatch", ::KFParticleMatch::Class_Version(), "KFParticleMatch.h", 17,
                  typeid(::KFParticleMatch), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::KFParticleMatch::Dictionary, isa_proxy, 4,
                  sizeof(::KFParticleMatch) );
      instance.SetNew(&new_KFParticleMatch);
      instance.SetNewArray(&newArray_KFParticleMatch);
      instance.SetDelete(&delete_KFParticleMatch);
      instance.SetDeleteArray(&deleteArray_KFParticleMatch);
      instance.SetDestructor(&destruct_KFParticleMatch);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::KFParticleMatch*)
   {
      return GenerateInitInstanceLocal((::KFParticleMatch*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::KFParticleMatch*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmKFPartEfficiencies::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFPartEfficiencies::Class_Name()
{
   return "CbmKFPartEfficiencies";
}

//______________________________________________________________________________
const char *CbmKFPartEfficiencies::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFPartEfficiencies*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFPartEfficiencies::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFPartEfficiencies*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFPartEfficiencies::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFPartEfficiencies*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFPartEfficiencies::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFPartEfficiencies*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFTrErrMCPoints::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFTrErrMCPoints::Class_Name()
{
   return "CbmKFTrErrMCPoints";
}

//______________________________________________________________________________
const char *CbmKFTrErrMCPoints::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrErrMCPoints*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFTrErrMCPoints::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrErrMCPoints*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFTrErrMCPoints::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrErrMCPoints*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFTrErrMCPoints::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrErrMCPoints*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFTrackFitQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFTrackFitQa::Class_Name()
{
   return "CbmKFTrackFitQa";
}

//______________________________________________________________________________
const char *CbmKFTrackFitQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrackFitQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFTrackFitQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrackFitQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFTrackFitQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrackFitQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFTrackFitQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrackFitQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFTrackQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFTrackQa::Class_Name()
{
   return "CbmKFTrackQa";
}

//______________________________________________________________________________
const char *CbmKFTrackQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrackQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFTrackQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrackQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFTrackQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrackQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFTrackQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrackQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr KFParticleMatch::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *KFParticleMatch::Class_Name()
{
   return "KFParticleMatch";
}

//______________________________________________________________________________
const char *KFParticleMatch::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::KFParticleMatch*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int KFParticleMatch::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::KFParticleMatch*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *KFParticleMatch::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::KFParticleMatch*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *KFParticleMatch::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::KFParticleMatch*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmKFPartEfficiencies::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFPartEfficiencies.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFPartEfficiencies::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFPartEfficiencies::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFPartEfficiencies(void *p) {
      return  p ? new(p) ::CbmKFPartEfficiencies : new ::CbmKFPartEfficiencies;
   }
   static void *newArray_CbmKFPartEfficiencies(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFPartEfficiencies[nElements] : new ::CbmKFPartEfficiencies[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFPartEfficiencies(void *p) {
      delete ((::CbmKFPartEfficiencies*)p);
   }
   static void deleteArray_CbmKFPartEfficiencies(void *p) {
      delete [] ((::CbmKFPartEfficiencies*)p);
   }
   static void destruct_CbmKFPartEfficiencies(void *p) {
      typedef ::CbmKFPartEfficiencies current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFPartEfficiencies

//______________________________________________________________________________
void CbmKFTrErrMCPoints::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFTrErrMCPoints.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFTrErrMCPoints::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFTrErrMCPoints::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFTrErrMCPoints(void *p) {
      return  p ? new(p) ::CbmKFTrErrMCPoints : new ::CbmKFTrErrMCPoints;
   }
   static void *newArray_CbmKFTrErrMCPoints(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFTrErrMCPoints[nElements] : new ::CbmKFTrErrMCPoints[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFTrErrMCPoints(void *p) {
      delete ((::CbmKFTrErrMCPoints*)p);
   }
   static void deleteArray_CbmKFTrErrMCPoints(void *p) {
      delete [] ((::CbmKFTrErrMCPoints*)p);
   }
   static void destruct_CbmKFTrErrMCPoints(void *p) {
      typedef ::CbmKFTrErrMCPoints current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFTrErrMCPoints

//______________________________________________________________________________
void CbmKFTrackFitQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFTrackFitQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFTrackFitQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFTrackFitQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFTrackFitQa(void *p) {
      return  p ? new(p) ::CbmKFTrackFitQa : new ::CbmKFTrackFitQa;
   }
   static void *newArray_CbmKFTrackFitQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFTrackFitQa[nElements] : new ::CbmKFTrackFitQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFTrackFitQa(void *p) {
      delete ((::CbmKFTrackFitQa*)p);
   }
   static void deleteArray_CbmKFTrackFitQa(void *p) {
      delete [] ((::CbmKFTrackFitQa*)p);
   }
   static void destruct_CbmKFTrackFitQa(void *p) {
      typedef ::CbmKFTrackFitQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFTrackFitQa

//______________________________________________________________________________
void CbmKFTrackQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFTrackQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFTrackQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFTrackQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFTrackQa(void *p) {
      return  p ? new(p) ::CbmKFTrackQa : new ::CbmKFTrackQa;
   }
   static void *newArray_CbmKFTrackQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFTrackQa[nElements] : new ::CbmKFTrackQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFTrackQa(void *p) {
      delete ((::CbmKFTrackQa*)p);
   }
   static void deleteArray_CbmKFTrackQa(void *p) {
      delete [] ((::CbmKFTrackQa*)p);
   }
   static void destruct_CbmKFTrackQa(void *p) {
      typedef ::CbmKFTrackQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFTrackQa

//______________________________________________________________________________
void KFParticleMatch::Streamer(TBuffer &R__b)
{
   // Stream an object of class KFParticleMatch.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(KFParticleMatch::Class(),this);
   } else {
      R__b.WriteClassBuffer(KFParticleMatch::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_KFParticleMatch(void *p) {
      return  p ? new(p) ::KFParticleMatch : new ::KFParticleMatch;
   }
   static void *newArray_KFParticleMatch(Long_t nElements, void *p) {
      return p ? new(p) ::KFParticleMatch[nElements] : new ::KFParticleMatch[nElements];
   }
   // Wrapper around operator delete
   static void delete_KFParticleMatch(void *p) {
      delete ((::KFParticleMatch*)p);
   }
   static void deleteArray_KFParticleMatch(void *p) {
      delete [] ((::KFParticleMatch*)p);
   }
   static void destruct_KFParticleMatch(void *p) {
      typedef ::KFParticleMatch current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::KFParticleMatch

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
   static TClass *vectorlETStringgR_Dictionary();
   static void vectorlETStringgR_TClassManip(TClass*);
   static void *new_vectorlETStringgR(void *p = nullptr);
   static void *newArray_vectorlETStringgR(Long_t size, void *p);
   static void delete_vectorlETStringgR(void *p);
   static void deleteArray_vectorlETStringgR(void *p);
   static void destruct_vectorlETStringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TString>*)
   {
      vector<TString> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TString>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TString>", -2, "vector", 389,
                  typeid(vector<TString>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETStringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TString>) );
      instance.SetNew(&new_vectorlETStringgR);
      instance.SetNewArray(&newArray_vectorlETStringgR);
      instance.SetDelete(&delete_vectorlETStringgR);
      instance.SetDeleteArray(&deleteArray_vectorlETStringgR);
      instance.SetDestructor(&destruct_vectorlETStringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TString> >()));

      ::ROOT::AddClassAlternate("vector<TString>","std::vector<TString, std::allocator<TString> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TString>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETStringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TString>*)nullptr)->GetClass();
      vectorlETStringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETStringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETStringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TString> : new vector<TString>;
   }
   static void *newArray_vectorlETStringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TString>[nElements] : new vector<TString>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETStringgR(void *p) {
      delete ((vector<TString>*)p);
   }
   static void deleteArray_vectorlETStringgR(void *p) {
      delete [] ((vector<TString>*)p);
   }
   static void destruct_vectorlETStringgR(void *p) {
      typedef vector<TString> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TString>

namespace ROOT {
   static TClass *vectorlECbmTofPointmUgR_Dictionary();
   static void vectorlECbmTofPointmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmTofPointmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmTofPointmUgR(Long_t size, void *p);
   static void delete_vectorlECbmTofPointmUgR(void *p);
   static void deleteArray_vectorlECbmTofPointmUgR(void *p);
   static void destruct_vectorlECbmTofPointmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmTofPoint*>*)
   {
      vector<CbmTofPoint*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmTofPoint*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmTofPoint*>", -2, "vector", 389,
                  typeid(vector<CbmTofPoint*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmTofPointmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmTofPoint*>) );
      instance.SetNew(&new_vectorlECbmTofPointmUgR);
      instance.SetNewArray(&newArray_vectorlECbmTofPointmUgR);
      instance.SetDelete(&delete_vectorlECbmTofPointmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmTofPointmUgR);
      instance.SetDestructor(&destruct_vectorlECbmTofPointmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmTofPoint*> >()));

      ::ROOT::AddClassAlternate("vector<CbmTofPoint*>","std::vector<CbmTofPoint*, std::allocator<CbmTofPoint*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmTofPoint*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmTofPointmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmTofPoint*>*)nullptr)->GetClass();
      vectorlECbmTofPointmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmTofPointmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmTofPointmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTofPoint*> : new vector<CbmTofPoint*>;
   }
   static void *newArray_vectorlECbmTofPointmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTofPoint*>[nElements] : new vector<CbmTofPoint*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmTofPointmUgR(void *p) {
      delete ((vector<CbmTofPoint*>*)p);
   }
   static void deleteArray_vectorlECbmTofPointmUgR(void *p) {
      delete [] ((vector<CbmTofPoint*>*)p);
   }
   static void destruct_vectorlECbmTofPointmUgR(void *p) {
      typedef vector<CbmTofPoint*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmTofPoint*>

namespace ROOT {
   static TClass *vectorlECbmStsPointmUgR_Dictionary();
   static void vectorlECbmStsPointmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmStsPointmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmStsPointmUgR(Long_t size, void *p);
   static void delete_vectorlECbmStsPointmUgR(void *p);
   static void deleteArray_vectorlECbmStsPointmUgR(void *p);
   static void destruct_vectorlECbmStsPointmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsPoint*>*)
   {
      vector<CbmStsPoint*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsPoint*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsPoint*>", -2, "vector", 389,
                  typeid(vector<CbmStsPoint*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsPointmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmStsPoint*>) );
      instance.SetNew(&new_vectorlECbmStsPointmUgR);
      instance.SetNewArray(&newArray_vectorlECbmStsPointmUgR);
      instance.SetDelete(&delete_vectorlECbmStsPointmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsPointmUgR);
      instance.SetDestructor(&destruct_vectorlECbmStsPointmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsPoint*> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsPoint*>","std::vector<CbmStsPoint*, std::allocator<CbmStsPoint*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsPoint*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsPointmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsPoint*>*)nullptr)->GetClass();
      vectorlECbmStsPointmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsPointmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsPointmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsPoint*> : new vector<CbmStsPoint*>;
   }
   static void *newArray_vectorlECbmStsPointmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsPoint*>[nElements] : new vector<CbmStsPoint*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsPointmUgR(void *p) {
      delete ((vector<CbmStsPoint*>*)p);
   }
   static void deleteArray_vectorlECbmStsPointmUgR(void *p) {
      delete [] ((vector<CbmStsPoint*>*)p);
   }
   static void destruct_vectorlECbmStsPointmUgR(void *p) {
      typedef vector<CbmStsPoint*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsPoint*>

namespace ROOT {
   static TClass *vectorlECbmStsHitmUgR_Dictionary();
   static void vectorlECbmStsHitmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmStsHitmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmStsHitmUgR(Long_t size, void *p);
   static void delete_vectorlECbmStsHitmUgR(void *p);
   static void deleteArray_vectorlECbmStsHitmUgR(void *p);
   static void destruct_vectorlECbmStsHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsHit*>*)
   {
      vector<CbmStsHit*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsHit*>", -2, "vector", 389,
                  typeid(vector<CbmStsHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsHitmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmStsHit*>) );
      instance.SetNew(&new_vectorlECbmStsHitmUgR);
      instance.SetNewArray(&newArray_vectorlECbmStsHitmUgR);
      instance.SetDelete(&delete_vectorlECbmStsHitmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsHitmUgR);
      instance.SetDestructor(&destruct_vectorlECbmStsHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsHit*> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsHit*>","std::vector<CbmStsHit*, std::allocator<CbmStsHit*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsHit*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsHit*>*)nullptr)->GetClass();
      vectorlECbmStsHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsHit*> : new vector<CbmStsHit*>;
   }
   static void *newArray_vectorlECbmStsHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsHit*>[nElements] : new vector<CbmStsHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsHitmUgR(void *p) {
      delete ((vector<CbmStsHit*>*)p);
   }
   static void deleteArray_vectorlECbmStsHitmUgR(void *p) {
      delete [] ((vector<CbmStsHit*>*)p);
   }
   static void destruct_vectorlECbmStsHitmUgR(void *p) {
      typedef vector<CbmStsHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsHit*>

namespace ROOT {
   static TClass *vectorlECbmMvdPointmUgR_Dictionary();
   static void vectorlECbmMvdPointmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmMvdPointmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmMvdPointmUgR(Long_t size, void *p);
   static void delete_vectorlECbmMvdPointmUgR(void *p);
   static void deleteArray_vectorlECbmMvdPointmUgR(void *p);
   static void destruct_vectorlECbmMvdPointmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMvdPoint*>*)
   {
      vector<CbmMvdPoint*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMvdPoint*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMvdPoint*>", -2, "vector", 389,
                  typeid(vector<CbmMvdPoint*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMvdPointmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmMvdPoint*>) );
      instance.SetNew(&new_vectorlECbmMvdPointmUgR);
      instance.SetNewArray(&newArray_vectorlECbmMvdPointmUgR);
      instance.SetDelete(&delete_vectorlECbmMvdPointmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMvdPointmUgR);
      instance.SetDestructor(&destruct_vectorlECbmMvdPointmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMvdPoint*> >()));

      ::ROOT::AddClassAlternate("vector<CbmMvdPoint*>","std::vector<CbmMvdPoint*, std::allocator<CbmMvdPoint*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMvdPoint*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMvdPointmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMvdPoint*>*)nullptr)->GetClass();
      vectorlECbmMvdPointmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMvdPointmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMvdPointmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMvdPoint*> : new vector<CbmMvdPoint*>;
   }
   static void *newArray_vectorlECbmMvdPointmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMvdPoint*>[nElements] : new vector<CbmMvdPoint*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMvdPointmUgR(void *p) {
      delete ((vector<CbmMvdPoint*>*)p);
   }
   static void deleteArray_vectorlECbmMvdPointmUgR(void *p) {
      delete [] ((vector<CbmMvdPoint*>*)p);
   }
   static void destruct_vectorlECbmMvdPointmUgR(void *p) {
      typedef vector<CbmMvdPoint*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMvdPoint*>

namespace ROOT {
   static TClass *vectorlECbmMvdHitmUgR_Dictionary();
   static void vectorlECbmMvdHitmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmMvdHitmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmMvdHitmUgR(Long_t size, void *p);
   static void delete_vectorlECbmMvdHitmUgR(void *p);
   static void deleteArray_vectorlECbmMvdHitmUgR(void *p);
   static void destruct_vectorlECbmMvdHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMvdHit*>*)
   {
      vector<CbmMvdHit*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMvdHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMvdHit*>", -2, "vector", 389,
                  typeid(vector<CbmMvdHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMvdHitmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmMvdHit*>) );
      instance.SetNew(&new_vectorlECbmMvdHitmUgR);
      instance.SetNewArray(&newArray_vectorlECbmMvdHitmUgR);
      instance.SetDelete(&delete_vectorlECbmMvdHitmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMvdHitmUgR);
      instance.SetDestructor(&destruct_vectorlECbmMvdHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMvdHit*> >()));

      ::ROOT::AddClassAlternate("vector<CbmMvdHit*>","std::vector<CbmMvdHit*, std::allocator<CbmMvdHit*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMvdHit*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMvdHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMvdHit*>*)nullptr)->GetClass();
      vectorlECbmMvdHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMvdHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMvdHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMvdHit*> : new vector<CbmMvdHit*>;
   }
   static void *newArray_vectorlECbmMvdHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMvdHit*>[nElements] : new vector<CbmMvdHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMvdHitmUgR(void *p) {
      delete ((vector<CbmMvdHit*>*)p);
   }
   static void deleteArray_vectorlECbmMvdHitmUgR(void *p) {
      delete [] ((vector<CbmMvdHit*>*)p);
   }
   static void destruct_vectorlECbmMvdHitmUgR(void *p) {
      typedef vector<CbmMvdHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMvdHit*>

namespace ROOT {
   static TClass *maplEintcOintgR_Dictionary();
   static void maplEintcOintgR_TClassManip(TClass*);
   static void *new_maplEintcOintgR(void *p = nullptr);
   static void *newArray_maplEintcOintgR(Long_t size, void *p);
   static void delete_maplEintcOintgR(void *p);
   static void deleteArray_maplEintcOintgR(void *p);
   static void destruct_maplEintcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,int>*)
   {
      map<int,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,int>", -2, "map", 100,
                  typeid(map<int,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,int>) );
      instance.SetNew(&new_maplEintcOintgR);
      instance.SetNewArray(&newArray_maplEintcOintgR);
      instance.SetDelete(&delete_maplEintcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOintgR);
      instance.SetDestructor(&destruct_maplEintcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,int> >()));

      ::ROOT::AddClassAlternate("map<int,int>","std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,int>*)nullptr)->GetClass();
      maplEintcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,int> : new map<int,int>;
   }
   static void *newArray_maplEintcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,int>[nElements] : new map<int,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOintgR(void *p) {
      delete ((map<int,int>*)p);
   }
   static void deleteArray_maplEintcOintgR(void *p) {
      delete [] ((map<int,int>*)p);
   }
   static void destruct_maplEintcOintgR(void *p) {
      typedef map<int,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,int>

namespace ROOT {
   static TClass *maplETStringcOintgR_Dictionary();
   static void maplETStringcOintgR_TClassManip(TClass*);
   static void *new_maplETStringcOintgR(void *p = nullptr);
   static void *newArray_maplETStringcOintgR(Long_t size, void *p);
   static void delete_maplETStringcOintgR(void *p);
   static void deleteArray_maplETStringcOintgR(void *p);
   static void destruct_maplETStringcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,int>*)
   {
      map<TString,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,int>", -2, "map", 100,
                  typeid(map<TString,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,int>) );
      instance.SetNew(&new_maplETStringcOintgR);
      instance.SetNewArray(&newArray_maplETStringcOintgR);
      instance.SetDelete(&delete_maplETStringcOintgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOintgR);
      instance.SetDestructor(&destruct_maplETStringcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,int> >()));

      ::ROOT::AddClassAlternate("map<TString,int>","std::map<TString, int, std::less<TString>, std::allocator<std::pair<TString const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,int>*)nullptr)->GetClass();
      maplETStringcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,int> : new map<TString,int>;
   }
   static void *newArray_maplETStringcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,int>[nElements] : new map<TString,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOintgR(void *p) {
      delete ((map<TString,int>*)p);
   }
   static void deleteArray_maplETStringcOintgR(void *p) {
      delete [] ((map<TString,int>*)p);
   }
   static void destruct_maplETStringcOintgR(void *p) {
      typedef map<TString,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,int>

namespace {
  void TriggerDictionaryInitialization_libKFQA_Impl() {
    static const char* headers[] = {
"CbmKFPartEfficiencies.h",
"CbmKFTrErrMCPoints.h",
"CbmKFTrackFitQa.h",
"CbmKFTrackQa.h",
"KFParticleMatch.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/KFQA",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/KFQA",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
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
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca/simd",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/OffLineInterface",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/ParticleFinder",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/utils",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools",
"/home/heinemann/gpuunpacker/cbmroot/core/qa/checker",
"/home/heinemann/gpuunpacker/cbmroot/core/qa",
"/home/heinemann/gpuunpacker/cbmroot/external/yaml-cpp/include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/build2/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/KF/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libKFQA dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmKFPartEfficiencies.h")))  CbmKFPartEfficiencies;
class __attribute__((annotate("$clingAutoload$CbmKFTrErrMCPoints.h")))  CbmKFTrErrMCPoints;
class __attribute__((annotate("$clingAutoload$CbmKFTrackFitQa.h")))  CbmKFTrackFitQa;
class __attribute__((annotate("$clingAutoload$CbmKFTrackQa.h")))  CbmKFTrackQa;
class __attribute__((annotate("$clingAutoload$KFParticleMatch.h")))  KFParticleMatch;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libKFQA dictionary payload"

#ifndef DO_TPCCATRACKER_EFF_PERFORMANCE
  #define DO_TPCCATRACKER_EFF_PERFORMANCE 1
#endif
#ifndef NonhomogeneousField
  #define NonhomogeneousField 1
#endif
#ifndef CBM
  #define CBM 1
#endif
#ifndef USE_TIMERS
  #define USE_TIMERS 1
#endif
#ifndef HAVE_SSE
  #define HAVE_SSE 1
#endif
#ifndef DO_TPCCATRACKER_EFF_PERFORMANCE
  #define DO_TPCCATRACKER_EFF_PERFORMANCE 1
#endif
#ifndef NonhomogeneousField
  #define NonhomogeneousField 1
#endif
#ifndef CBM
  #define CBM 1
#endif
#ifndef USE_TIMERS
  #define USE_TIMERS 1
#endif
#ifndef HAVE_SSE
  #define HAVE_SSE 1
#endif
#ifndef DO_TPCCATRACKER_EFF_PERFORMANCE
  #define DO_TPCCATRACKER_EFF_PERFORMANCE 1
#endif
#ifndef NonhomogeneousField
  #define NonhomogeneousField 1
#endif
#ifndef CBM
  #define CBM 1
#endif
#ifndef USE_TIMERS
  #define USE_TIMERS 1
#endif
#ifndef HAVE_SSE
  #define HAVE_SSE 1
#endif
#ifndef DO_TPCCATRACKER_EFF_PERFORMANCE
  #define DO_TPCCATRACKER_EFF_PERFORMANCE 1
#endif
#ifndef NonhomogeneousField
  #define NonhomogeneousField 1
#endif
#ifndef CBM
  #define CBM 1
#endif
#ifndef USE_TIMERS
  #define USE_TIMERS 1
#endif
#ifndef HAVE_SSE
  #define HAVE_SSE 1
#endif
#ifndef DO_TPCCATRACKER_EFF_PERFORMANCE
  #define DO_TPCCATRACKER_EFF_PERFORMANCE 1
#endif
#ifndef NonhomogeneousField
  #define NonhomogeneousField 1
#endif
#ifndef CBM
  #define CBM 1
#endif
#ifndef USE_TIMERS
  #define USE_TIMERS 1
#endif
#ifndef HAVE_SSE
  #define HAVE_SSE 1
#endif
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
#include "CbmKFPartEfficiencies.h"
#include "CbmKFTrErrMCPoints.h"
#include "CbmKFTrackFitQa.h"
#include "CbmKFTrackQa.h"
#include "KFParticleMatch.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmKFPartEfficiencies", payloadCode, "@",
"CbmKFTrErrMCPoints", payloadCode, "@",
"CbmKFTrackFitQa", payloadCode, "@",
"CbmKFTrackQa", payloadCode, "@",
"KFParticleMatch", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libKFQA",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libKFQA_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libKFQA_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libKFQA() {
  TriggerDictionaryInitialization_libKFQA_Impl();
}
