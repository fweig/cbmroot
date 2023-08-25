// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__LxTrack
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
#include "LxTBTask.h"
#include "LxTBTieTracks.h"
#include "LxTBTrdTask.h"
#include "GenNoiseElectrons.h"
#include "CalcStats.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_LxTBFinder(void *p = nullptr);
   static void *newArray_LxTBFinder(Long_t size, void *p);
   static void delete_LxTBFinder(void *p);
   static void deleteArray_LxTBFinder(void *p);
   static void destruct_LxTBFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LxTBFinder*)
   {
      ::LxTBFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LxTBFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("LxTBFinder", ::LxTBFinder::Class_Version(), "LxTBTask.h", 36,
                  typeid(::LxTBFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::LxTBFinder::Dictionary, isa_proxy, 4,
                  sizeof(::LxTBFinder) );
      instance.SetNew(&new_LxTBFinder);
      instance.SetNewArray(&newArray_LxTBFinder);
      instance.SetDelete(&delete_LxTBFinder);
      instance.SetDeleteArray(&deleteArray_LxTBFinder);
      instance.SetDestructor(&destruct_LxTBFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LxTBFinder*)
   {
      return GenerateInitInstanceLocal((::LxTBFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LxTBFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_LxTBTrdFinder(void *p = nullptr);
   static void *newArray_LxTBTrdFinder(Long_t size, void *p);
   static void delete_LxTBTrdFinder(void *p);
   static void deleteArray_LxTBTrdFinder(void *p);
   static void destruct_LxTBTrdFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LxTBTrdFinder*)
   {
      ::LxTBTrdFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LxTBTrdFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("LxTBTrdFinder", ::LxTBTrdFinder::Class_Version(), "LxTBTrdTask.h", 18,
                  typeid(::LxTBTrdFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::LxTBTrdFinder::Dictionary, isa_proxy, 4,
                  sizeof(::LxTBTrdFinder) );
      instance.SetNew(&new_LxTBTrdFinder);
      instance.SetNewArray(&newArray_LxTBTrdFinder);
      instance.SetDelete(&delete_LxTBTrdFinder);
      instance.SetDeleteArray(&deleteArray_LxTBTrdFinder);
      instance.SetDestructor(&destruct_LxTBTrdFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LxTBTrdFinder*)
   {
      return GenerateInitInstanceLocal((::LxTBTrdFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LxTBTrdFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_LxGenNoiseElectrons(void *p = nullptr);
   static void *newArray_LxGenNoiseElectrons(Long_t size, void *p);
   static void delete_LxGenNoiseElectrons(void *p);
   static void deleteArray_LxGenNoiseElectrons(void *p);
   static void destruct_LxGenNoiseElectrons(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LxGenNoiseElectrons*)
   {
      ::LxGenNoiseElectrons *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LxGenNoiseElectrons >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("LxGenNoiseElectrons", ::LxGenNoiseElectrons::Class_Version(), "GenNoiseElectrons.h", 12,
                  typeid(::LxGenNoiseElectrons), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::LxGenNoiseElectrons::Dictionary, isa_proxy, 4,
                  sizeof(::LxGenNoiseElectrons) );
      instance.SetNew(&new_LxGenNoiseElectrons);
      instance.SetNewArray(&newArray_LxGenNoiseElectrons);
      instance.SetDelete(&delete_LxGenNoiseElectrons);
      instance.SetDeleteArray(&deleteArray_LxGenNoiseElectrons);
      instance.SetDestructor(&destruct_LxGenNoiseElectrons);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LxGenNoiseElectrons*)
   {
      return GenerateInitInstanceLocal((::LxGenNoiseElectrons*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LxGenNoiseElectrons*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_LxCalcStats(void *p = nullptr);
   static void *newArray_LxCalcStats(Long_t size, void *p);
   static void delete_LxCalcStats(void *p);
   static void deleteArray_LxCalcStats(void *p);
   static void destruct_LxCalcStats(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LxCalcStats*)
   {
      ::LxCalcStats *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LxCalcStats >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("LxCalcStats", ::LxCalcStats::Class_Version(), "CalcStats.h", 14,
                  typeid(::LxCalcStats), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::LxCalcStats::Dictionary, isa_proxy, 4,
                  sizeof(::LxCalcStats) );
      instance.SetNew(&new_LxCalcStats);
      instance.SetNewArray(&newArray_LxCalcStats);
      instance.SetDelete(&delete_LxCalcStats);
      instance.SetDeleteArray(&deleteArray_LxCalcStats);
      instance.SetDestructor(&destruct_LxCalcStats);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LxCalcStats*)
   {
      return GenerateInitInstanceLocal((::LxCalcStats*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LxCalcStats*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr LxTBFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *LxTBFinder::Class_Name()
{
   return "LxTBFinder";
}

//______________________________________________________________________________
const char *LxTBFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LxTBFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int LxTBFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LxTBFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LxTBFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LxTBFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LxTBFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LxTBFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr LxTBTrdFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *LxTBTrdFinder::Class_Name()
{
   return "LxTBTrdFinder";
}

//______________________________________________________________________________
const char *LxTBTrdFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LxTBTrdFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int LxTBTrdFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LxTBTrdFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LxTBTrdFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LxTBTrdFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LxTBTrdFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LxTBTrdFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr LxGenNoiseElectrons::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *LxGenNoiseElectrons::Class_Name()
{
   return "LxGenNoiseElectrons";
}

//______________________________________________________________________________
const char *LxGenNoiseElectrons::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LxGenNoiseElectrons*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int LxGenNoiseElectrons::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LxGenNoiseElectrons*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LxGenNoiseElectrons::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LxGenNoiseElectrons*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LxGenNoiseElectrons::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LxGenNoiseElectrons*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr LxCalcStats::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *LxCalcStats::Class_Name()
{
   return "LxCalcStats";
}

//______________________________________________________________________________
const char *LxCalcStats::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LxCalcStats*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int LxCalcStats::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LxCalcStats*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LxCalcStats::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LxCalcStats*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LxCalcStats::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LxCalcStats*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void LxTBFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class LxTBFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(LxTBFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(LxTBFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_LxTBFinder(void *p) {
      return  p ? new(p) ::LxTBFinder : new ::LxTBFinder;
   }
   static void *newArray_LxTBFinder(Long_t nElements, void *p) {
      return p ? new(p) ::LxTBFinder[nElements] : new ::LxTBFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_LxTBFinder(void *p) {
      delete ((::LxTBFinder*)p);
   }
   static void deleteArray_LxTBFinder(void *p) {
      delete [] ((::LxTBFinder*)p);
   }
   static void destruct_LxTBFinder(void *p) {
      typedef ::LxTBFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LxTBFinder

//______________________________________________________________________________
void LxTBTrdFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class LxTBTrdFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(LxTBTrdFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(LxTBTrdFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_LxTBTrdFinder(void *p) {
      return  p ? new(p) ::LxTBTrdFinder : new ::LxTBTrdFinder;
   }
   static void *newArray_LxTBTrdFinder(Long_t nElements, void *p) {
      return p ? new(p) ::LxTBTrdFinder[nElements] : new ::LxTBTrdFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_LxTBTrdFinder(void *p) {
      delete ((::LxTBTrdFinder*)p);
   }
   static void deleteArray_LxTBTrdFinder(void *p) {
      delete [] ((::LxTBTrdFinder*)p);
   }
   static void destruct_LxTBTrdFinder(void *p) {
      typedef ::LxTBTrdFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LxTBTrdFinder

//______________________________________________________________________________
void LxGenNoiseElectrons::Streamer(TBuffer &R__b)
{
   // Stream an object of class LxGenNoiseElectrons.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(LxGenNoiseElectrons::Class(),this);
   } else {
      R__b.WriteClassBuffer(LxGenNoiseElectrons::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_LxGenNoiseElectrons(void *p) {
      return  p ? new(p) ::LxGenNoiseElectrons : new ::LxGenNoiseElectrons;
   }
   static void *newArray_LxGenNoiseElectrons(Long_t nElements, void *p) {
      return p ? new(p) ::LxGenNoiseElectrons[nElements] : new ::LxGenNoiseElectrons[nElements];
   }
   // Wrapper around operator delete
   static void delete_LxGenNoiseElectrons(void *p) {
      delete ((::LxGenNoiseElectrons*)p);
   }
   static void deleteArray_LxGenNoiseElectrons(void *p) {
      delete [] ((::LxGenNoiseElectrons*)p);
   }
   static void destruct_LxGenNoiseElectrons(void *p) {
      typedef ::LxGenNoiseElectrons current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LxGenNoiseElectrons

//______________________________________________________________________________
void LxCalcStats::Streamer(TBuffer &R__b)
{
   // Stream an object of class LxCalcStats.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(LxCalcStats::Class(),this);
   } else {
      R__b.WriteClassBuffer(LxCalcStats::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_LxCalcStats(void *p) {
      return  p ? new(p) ::LxCalcStats : new ::LxCalcStats;
   }
   static void *newArray_LxCalcStats(Long_t nElements, void *p) {
      return p ? new(p) ::LxCalcStats[nElements] : new ::LxCalcStats[nElements];
   }
   // Wrapper around operator delete
   static void delete_LxCalcStats(void *p) {
      delete ((::LxCalcStats*)p);
   }
   static void deleteArray_LxCalcStats(void *p) {
      delete [] ((::LxCalcStats*)p);
   }
   static void destruct_LxCalcStats(void *p) {
      typedef ::LxCalcStats current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LxCalcStats

namespace ROOT {
   static TClass *vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR_Dictionary();
   static void vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR(void *p);
   static void deleteArray_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR(void *p);
   static void destruct_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<LxTBTrdFinder::TrackDataHolder> >*)
   {
      vector<vector<LxTBTrdFinder::TrackDataHolder> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<LxTBTrdFinder::TrackDataHolder> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<LxTBTrdFinder::TrackDataHolder> >", -2, "vector", 389,
                  typeid(vector<vector<LxTBTrdFinder::TrackDataHolder> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<LxTBTrdFinder::TrackDataHolder> >) );
      instance.SetNew(&new_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<LxTBTrdFinder::TrackDataHolder> > >()));

      ::ROOT::AddClassAlternate("vector<vector<LxTBTrdFinder::TrackDataHolder> >","std::vector<std::vector<LxTBTrdFinder::TrackDataHolder, std::allocator<LxTBTrdFinder::TrackDataHolder> >, std::allocator<std::vector<LxTBTrdFinder::TrackDataHolder, std::allocator<LxTBTrdFinder::TrackDataHolder> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<LxTBTrdFinder::TrackDataHolder> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<LxTBTrdFinder::TrackDataHolder> >*)nullptr)->GetClass();
      vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<LxTBTrdFinder::TrackDataHolder> > : new vector<vector<LxTBTrdFinder::TrackDataHolder> >;
   }
   static void *newArray_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<LxTBTrdFinder::TrackDataHolder> >[nElements] : new vector<vector<LxTBTrdFinder::TrackDataHolder> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR(void *p) {
      delete ((vector<vector<LxTBTrdFinder::TrackDataHolder> >*)p);
   }
   static void deleteArray_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR(void *p) {
      delete [] ((vector<vector<LxTBTrdFinder::TrackDataHolder> >*)p);
   }
   static void destruct_vectorlEvectorlELxTBTrdFindercLcLTrackDataHoldergRsPgR(void *p) {
      typedef vector<vector<LxTBTrdFinder::TrackDataHolder> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<LxTBTrdFinder::TrackDataHolder> >

namespace ROOT {
   static TClass *vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR_Dictionary();
   static void vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR(void *p);
   static void deleteArray_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR(void *p);
   static void destruct_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<LxTBTrdFinder::PointDataHolder> >*)
   {
      vector<vector<LxTBTrdFinder::PointDataHolder> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<LxTBTrdFinder::PointDataHolder> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<LxTBTrdFinder::PointDataHolder> >", -2, "vector", 389,
                  typeid(vector<vector<LxTBTrdFinder::PointDataHolder> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<LxTBTrdFinder::PointDataHolder> >) );
      instance.SetNew(&new_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<LxTBTrdFinder::PointDataHolder> > >()));

      ::ROOT::AddClassAlternate("vector<vector<LxTBTrdFinder::PointDataHolder> >","std::vector<std::vector<LxTBTrdFinder::PointDataHolder, std::allocator<LxTBTrdFinder::PointDataHolder> >, std::allocator<std::vector<LxTBTrdFinder::PointDataHolder, std::allocator<LxTBTrdFinder::PointDataHolder> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<LxTBTrdFinder::PointDataHolder> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<LxTBTrdFinder::PointDataHolder> >*)nullptr)->GetClass();
      vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<LxTBTrdFinder::PointDataHolder> > : new vector<vector<LxTBTrdFinder::PointDataHolder> >;
   }
   static void *newArray_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<LxTBTrdFinder::PointDataHolder> >[nElements] : new vector<vector<LxTBTrdFinder::PointDataHolder> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR(void *p) {
      delete ((vector<vector<LxTBTrdFinder::PointDataHolder> >*)p);
   }
   static void deleteArray_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR(void *p) {
      delete [] ((vector<vector<LxTBTrdFinder::PointDataHolder> >*)p);
   }
   static void destruct_vectorlEvectorlELxTBTrdFindercLcLPointDataHoldergRsPgR(void *p) {
      typedef vector<vector<LxTBTrdFinder::PointDataHolder> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<LxTBTrdFinder::PointDataHolder> >

namespace ROOT {
   static TClass *vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR_Dictionary();
   static void vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR(void *p);
   static void deleteArray_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR(void *p);
   static void destruct_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<LxTBFinder::TrackDataHolder> >*)
   {
      vector<vector<LxTBFinder::TrackDataHolder> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<LxTBFinder::TrackDataHolder> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<LxTBFinder::TrackDataHolder> >", -2, "vector", 389,
                  typeid(vector<vector<LxTBFinder::TrackDataHolder> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<LxTBFinder::TrackDataHolder> >) );
      instance.SetNew(&new_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<LxTBFinder::TrackDataHolder> > >()));

      ::ROOT::AddClassAlternate("vector<vector<LxTBFinder::TrackDataHolder> >","std::vector<std::vector<LxTBFinder::TrackDataHolder, std::allocator<LxTBFinder::TrackDataHolder> >, std::allocator<std::vector<LxTBFinder::TrackDataHolder, std::allocator<LxTBFinder::TrackDataHolder> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<LxTBFinder::TrackDataHolder> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<LxTBFinder::TrackDataHolder> >*)nullptr)->GetClass();
      vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<LxTBFinder::TrackDataHolder> > : new vector<vector<LxTBFinder::TrackDataHolder> >;
   }
   static void *newArray_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<LxTBFinder::TrackDataHolder> >[nElements] : new vector<vector<LxTBFinder::TrackDataHolder> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR(void *p) {
      delete ((vector<vector<LxTBFinder::TrackDataHolder> >*)p);
   }
   static void deleteArray_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR(void *p) {
      delete [] ((vector<vector<LxTBFinder::TrackDataHolder> >*)p);
   }
   static void destruct_vectorlEvectorlELxTBFindercLcLTrackDataHoldergRsPgR(void *p) {
      typedef vector<vector<LxTBFinder::TrackDataHolder> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<LxTBFinder::TrackDataHolder> >

namespace ROOT {
   static TClass *vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR_Dictionary();
   static void vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR(void *p);
   static void deleteArray_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR(void *p);
   static void destruct_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<LxTBFinder::PointDataHolder> >*)
   {
      vector<vector<LxTBFinder::PointDataHolder> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<LxTBFinder::PointDataHolder> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<LxTBFinder::PointDataHolder> >", -2, "vector", 389,
                  typeid(vector<vector<LxTBFinder::PointDataHolder> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<LxTBFinder::PointDataHolder> >) );
      instance.SetNew(&new_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<LxTBFinder::PointDataHolder> > >()));

      ::ROOT::AddClassAlternate("vector<vector<LxTBFinder::PointDataHolder> >","std::vector<std::vector<LxTBFinder::PointDataHolder, std::allocator<LxTBFinder::PointDataHolder> >, std::allocator<std::vector<LxTBFinder::PointDataHolder, std::allocator<LxTBFinder::PointDataHolder> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<LxTBFinder::PointDataHolder> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<LxTBFinder::PointDataHolder> >*)nullptr)->GetClass();
      vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<LxTBFinder::PointDataHolder> > : new vector<vector<LxTBFinder::PointDataHolder> >;
   }
   static void *newArray_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<LxTBFinder::PointDataHolder> >[nElements] : new vector<vector<LxTBFinder::PointDataHolder> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR(void *p) {
      delete ((vector<vector<LxTBFinder::PointDataHolder> >*)p);
   }
   static void deleteArray_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR(void *p) {
      delete [] ((vector<vector<LxTBFinder::PointDataHolder> >*)p);
   }
   static void destruct_vectorlEvectorlELxTBFindercLcLPointDataHoldergRsPgR(void *p) {
      typedef vector<vector<LxTBFinder::PointDataHolder> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<LxTBFinder::PointDataHolder> >

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = nullptr);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 389,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));

      ::ROOT::AddClassAlternate("vector<double>","std::vector<double, std::allocator<double> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)nullptr)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlELxTBTrdFindercLcLTrackDataHoldergR_Dictionary();
   static void vectorlELxTBTrdFindercLcLTrackDataHoldergR_TClassManip(TClass*);
   static void *new_vectorlELxTBTrdFindercLcLTrackDataHoldergR(void *p = nullptr);
   static void *newArray_vectorlELxTBTrdFindercLcLTrackDataHoldergR(Long_t size, void *p);
   static void delete_vectorlELxTBTrdFindercLcLTrackDataHoldergR(void *p);
   static void deleteArray_vectorlELxTBTrdFindercLcLTrackDataHoldergR(void *p);
   static void destruct_vectorlELxTBTrdFindercLcLTrackDataHoldergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<LxTBTrdFinder::TrackDataHolder>*)
   {
      vector<LxTBTrdFinder::TrackDataHolder> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<LxTBTrdFinder::TrackDataHolder>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<LxTBTrdFinder::TrackDataHolder>", -2, "vector", 389,
                  typeid(vector<LxTBTrdFinder::TrackDataHolder>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlELxTBTrdFindercLcLTrackDataHoldergR_Dictionary, isa_proxy, 0,
                  sizeof(vector<LxTBTrdFinder::TrackDataHolder>) );
      instance.SetNew(&new_vectorlELxTBTrdFindercLcLTrackDataHoldergR);
      instance.SetNewArray(&newArray_vectorlELxTBTrdFindercLcLTrackDataHoldergR);
      instance.SetDelete(&delete_vectorlELxTBTrdFindercLcLTrackDataHoldergR);
      instance.SetDeleteArray(&deleteArray_vectorlELxTBTrdFindercLcLTrackDataHoldergR);
      instance.SetDestructor(&destruct_vectorlELxTBTrdFindercLcLTrackDataHoldergR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<LxTBTrdFinder::TrackDataHolder> >()));

      ::ROOT::AddClassAlternate("vector<LxTBTrdFinder::TrackDataHolder>","std::vector<LxTBTrdFinder::TrackDataHolder, std::allocator<LxTBTrdFinder::TrackDataHolder> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<LxTBTrdFinder::TrackDataHolder>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlELxTBTrdFindercLcLTrackDataHoldergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<LxTBTrdFinder::TrackDataHolder>*)nullptr)->GetClass();
      vectorlELxTBTrdFindercLcLTrackDataHoldergR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlELxTBTrdFindercLcLTrackDataHoldergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlELxTBTrdFindercLcLTrackDataHoldergR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<LxTBTrdFinder::TrackDataHolder> : new vector<LxTBTrdFinder::TrackDataHolder>;
   }
   static void *newArray_vectorlELxTBTrdFindercLcLTrackDataHoldergR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<LxTBTrdFinder::TrackDataHolder>[nElements] : new vector<LxTBTrdFinder::TrackDataHolder>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlELxTBTrdFindercLcLTrackDataHoldergR(void *p) {
      delete ((vector<LxTBTrdFinder::TrackDataHolder>*)p);
   }
   static void deleteArray_vectorlELxTBTrdFindercLcLTrackDataHoldergR(void *p) {
      delete [] ((vector<LxTBTrdFinder::TrackDataHolder>*)p);
   }
   static void destruct_vectorlELxTBTrdFindercLcLTrackDataHoldergR(void *p) {
      typedef vector<LxTBTrdFinder::TrackDataHolder> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<LxTBTrdFinder::TrackDataHolder>

namespace ROOT {
   static TClass *vectorlELxTBTrdFindercLcLPointDataHoldergR_Dictionary();
   static void vectorlELxTBTrdFindercLcLPointDataHoldergR_TClassManip(TClass*);
   static void *new_vectorlELxTBTrdFindercLcLPointDataHoldergR(void *p = nullptr);
   static void *newArray_vectorlELxTBTrdFindercLcLPointDataHoldergR(Long_t size, void *p);
   static void delete_vectorlELxTBTrdFindercLcLPointDataHoldergR(void *p);
   static void deleteArray_vectorlELxTBTrdFindercLcLPointDataHoldergR(void *p);
   static void destruct_vectorlELxTBTrdFindercLcLPointDataHoldergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<LxTBTrdFinder::PointDataHolder>*)
   {
      vector<LxTBTrdFinder::PointDataHolder> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<LxTBTrdFinder::PointDataHolder>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<LxTBTrdFinder::PointDataHolder>", -2, "vector", 389,
                  typeid(vector<LxTBTrdFinder::PointDataHolder>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlELxTBTrdFindercLcLPointDataHoldergR_Dictionary, isa_proxy, 0,
                  sizeof(vector<LxTBTrdFinder::PointDataHolder>) );
      instance.SetNew(&new_vectorlELxTBTrdFindercLcLPointDataHoldergR);
      instance.SetNewArray(&newArray_vectorlELxTBTrdFindercLcLPointDataHoldergR);
      instance.SetDelete(&delete_vectorlELxTBTrdFindercLcLPointDataHoldergR);
      instance.SetDeleteArray(&deleteArray_vectorlELxTBTrdFindercLcLPointDataHoldergR);
      instance.SetDestructor(&destruct_vectorlELxTBTrdFindercLcLPointDataHoldergR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<LxTBTrdFinder::PointDataHolder> >()));

      ::ROOT::AddClassAlternate("vector<LxTBTrdFinder::PointDataHolder>","std::vector<LxTBTrdFinder::PointDataHolder, std::allocator<LxTBTrdFinder::PointDataHolder> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<LxTBTrdFinder::PointDataHolder>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlELxTBTrdFindercLcLPointDataHoldergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<LxTBTrdFinder::PointDataHolder>*)nullptr)->GetClass();
      vectorlELxTBTrdFindercLcLPointDataHoldergR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlELxTBTrdFindercLcLPointDataHoldergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlELxTBTrdFindercLcLPointDataHoldergR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<LxTBTrdFinder::PointDataHolder> : new vector<LxTBTrdFinder::PointDataHolder>;
   }
   static void *newArray_vectorlELxTBTrdFindercLcLPointDataHoldergR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<LxTBTrdFinder::PointDataHolder>[nElements] : new vector<LxTBTrdFinder::PointDataHolder>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlELxTBTrdFindercLcLPointDataHoldergR(void *p) {
      delete ((vector<LxTBTrdFinder::PointDataHolder>*)p);
   }
   static void deleteArray_vectorlELxTBTrdFindercLcLPointDataHoldergR(void *p) {
      delete [] ((vector<LxTBTrdFinder::PointDataHolder>*)p);
   }
   static void destruct_vectorlELxTBTrdFindercLcLPointDataHoldergR(void *p) {
      typedef vector<LxTBTrdFinder::PointDataHolder> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<LxTBTrdFinder::PointDataHolder>

namespace ROOT {
   static TClass *vectorlELxTBFindercLcLTrackDataHoldergR_Dictionary();
   static void vectorlELxTBFindercLcLTrackDataHoldergR_TClassManip(TClass*);
   static void *new_vectorlELxTBFindercLcLTrackDataHoldergR(void *p = nullptr);
   static void *newArray_vectorlELxTBFindercLcLTrackDataHoldergR(Long_t size, void *p);
   static void delete_vectorlELxTBFindercLcLTrackDataHoldergR(void *p);
   static void deleteArray_vectorlELxTBFindercLcLTrackDataHoldergR(void *p);
   static void destruct_vectorlELxTBFindercLcLTrackDataHoldergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<LxTBFinder::TrackDataHolder>*)
   {
      vector<LxTBFinder::TrackDataHolder> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<LxTBFinder::TrackDataHolder>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<LxTBFinder::TrackDataHolder>", -2, "vector", 389,
                  typeid(vector<LxTBFinder::TrackDataHolder>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlELxTBFindercLcLTrackDataHoldergR_Dictionary, isa_proxy, 0,
                  sizeof(vector<LxTBFinder::TrackDataHolder>) );
      instance.SetNew(&new_vectorlELxTBFindercLcLTrackDataHoldergR);
      instance.SetNewArray(&newArray_vectorlELxTBFindercLcLTrackDataHoldergR);
      instance.SetDelete(&delete_vectorlELxTBFindercLcLTrackDataHoldergR);
      instance.SetDeleteArray(&deleteArray_vectorlELxTBFindercLcLTrackDataHoldergR);
      instance.SetDestructor(&destruct_vectorlELxTBFindercLcLTrackDataHoldergR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<LxTBFinder::TrackDataHolder> >()));

      ::ROOT::AddClassAlternate("vector<LxTBFinder::TrackDataHolder>","std::vector<LxTBFinder::TrackDataHolder, std::allocator<LxTBFinder::TrackDataHolder> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<LxTBFinder::TrackDataHolder>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlELxTBFindercLcLTrackDataHoldergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<LxTBFinder::TrackDataHolder>*)nullptr)->GetClass();
      vectorlELxTBFindercLcLTrackDataHoldergR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlELxTBFindercLcLTrackDataHoldergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlELxTBFindercLcLTrackDataHoldergR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<LxTBFinder::TrackDataHolder> : new vector<LxTBFinder::TrackDataHolder>;
   }
   static void *newArray_vectorlELxTBFindercLcLTrackDataHoldergR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<LxTBFinder::TrackDataHolder>[nElements] : new vector<LxTBFinder::TrackDataHolder>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlELxTBFindercLcLTrackDataHoldergR(void *p) {
      delete ((vector<LxTBFinder::TrackDataHolder>*)p);
   }
   static void deleteArray_vectorlELxTBFindercLcLTrackDataHoldergR(void *p) {
      delete [] ((vector<LxTBFinder::TrackDataHolder>*)p);
   }
   static void destruct_vectorlELxTBFindercLcLTrackDataHoldergR(void *p) {
      typedef vector<LxTBFinder::TrackDataHolder> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<LxTBFinder::TrackDataHolder>

namespace ROOT {
   static TClass *vectorlELxTBFindercLcLPointDataHoldergR_Dictionary();
   static void vectorlELxTBFindercLcLPointDataHoldergR_TClassManip(TClass*);
   static void *new_vectorlELxTBFindercLcLPointDataHoldergR(void *p = nullptr);
   static void *newArray_vectorlELxTBFindercLcLPointDataHoldergR(Long_t size, void *p);
   static void delete_vectorlELxTBFindercLcLPointDataHoldergR(void *p);
   static void deleteArray_vectorlELxTBFindercLcLPointDataHoldergR(void *p);
   static void destruct_vectorlELxTBFindercLcLPointDataHoldergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<LxTBFinder::PointDataHolder>*)
   {
      vector<LxTBFinder::PointDataHolder> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<LxTBFinder::PointDataHolder>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<LxTBFinder::PointDataHolder>", -2, "vector", 389,
                  typeid(vector<LxTBFinder::PointDataHolder>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlELxTBFindercLcLPointDataHoldergR_Dictionary, isa_proxy, 0,
                  sizeof(vector<LxTBFinder::PointDataHolder>) );
      instance.SetNew(&new_vectorlELxTBFindercLcLPointDataHoldergR);
      instance.SetNewArray(&newArray_vectorlELxTBFindercLcLPointDataHoldergR);
      instance.SetDelete(&delete_vectorlELxTBFindercLcLPointDataHoldergR);
      instance.SetDeleteArray(&deleteArray_vectorlELxTBFindercLcLPointDataHoldergR);
      instance.SetDestructor(&destruct_vectorlELxTBFindercLcLPointDataHoldergR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<LxTBFinder::PointDataHolder> >()));

      ::ROOT::AddClassAlternate("vector<LxTBFinder::PointDataHolder>","std::vector<LxTBFinder::PointDataHolder, std::allocator<LxTBFinder::PointDataHolder> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<LxTBFinder::PointDataHolder>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlELxTBFindercLcLPointDataHoldergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<LxTBFinder::PointDataHolder>*)nullptr)->GetClass();
      vectorlELxTBFindercLcLPointDataHoldergR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlELxTBFindercLcLPointDataHoldergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlELxTBFindercLcLPointDataHoldergR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<LxTBFinder::PointDataHolder> : new vector<LxTBFinder::PointDataHolder>;
   }
   static void *newArray_vectorlELxTBFindercLcLPointDataHoldergR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<LxTBFinder::PointDataHolder>[nElements] : new vector<LxTBFinder::PointDataHolder>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlELxTBFindercLcLPointDataHoldergR(void *p) {
      delete ((vector<LxTBFinder::PointDataHolder>*)p);
   }
   static void deleteArray_vectorlELxTBFindercLcLPointDataHoldergR(void *p) {
      delete [] ((vector<LxTBFinder::PointDataHolder>*)p);
   }
   static void destruct_vectorlELxTBFindercLcLPointDataHoldergR(void *p) {
      typedef vector<LxTBFinder::PointDataHolder> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<LxTBFinder::PointDataHolder>

namespace ROOT {
   static TClass *listlELxTbBinnedFindercLcLChainmUgR_Dictionary();
   static void listlELxTbBinnedFindercLcLChainmUgR_TClassManip(TClass*);
   static void *new_listlELxTbBinnedFindercLcLChainmUgR(void *p = nullptr);
   static void *newArray_listlELxTbBinnedFindercLcLChainmUgR(Long_t size, void *p);
   static void delete_listlELxTbBinnedFindercLcLChainmUgR(void *p);
   static void deleteArray_listlELxTbBinnedFindercLcLChainmUgR(void *p);
   static void destruct_listlELxTbBinnedFindercLcLChainmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const list<LxTbBinnedFinder::Chain*>*)
   {
      list<LxTbBinnedFinder::Chain*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(list<LxTbBinnedFinder::Chain*>));
      static ::ROOT::TGenericClassInfo 
         instance("list<LxTbBinnedFinder::Chain*>", -2, "list", 557,
                  typeid(list<LxTbBinnedFinder::Chain*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &listlELxTbBinnedFindercLcLChainmUgR_Dictionary, isa_proxy, 0,
                  sizeof(list<LxTbBinnedFinder::Chain*>) );
      instance.SetNew(&new_listlELxTbBinnedFindercLcLChainmUgR);
      instance.SetNewArray(&newArray_listlELxTbBinnedFindercLcLChainmUgR);
      instance.SetDelete(&delete_listlELxTbBinnedFindercLcLChainmUgR);
      instance.SetDeleteArray(&deleteArray_listlELxTbBinnedFindercLcLChainmUgR);
      instance.SetDestructor(&destruct_listlELxTbBinnedFindercLcLChainmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< list<LxTbBinnedFinder::Chain*> >()));

      ::ROOT::AddClassAlternate("list<LxTbBinnedFinder::Chain*>","std::__cxx11::list<LxTbBinnedFinder::Chain*, std::allocator<LxTbBinnedFinder::Chain*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const list<LxTbBinnedFinder::Chain*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *listlELxTbBinnedFindercLcLChainmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const list<LxTbBinnedFinder::Chain*>*)nullptr)->GetClass();
      listlELxTbBinnedFindercLcLChainmUgR_TClassManip(theClass);
   return theClass;
   }

   static void listlELxTbBinnedFindercLcLChainmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_listlELxTbBinnedFindercLcLChainmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<LxTbBinnedFinder::Chain*> : new list<LxTbBinnedFinder::Chain*>;
   }
   static void *newArray_listlELxTbBinnedFindercLcLChainmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<LxTbBinnedFinder::Chain*>[nElements] : new list<LxTbBinnedFinder::Chain*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_listlELxTbBinnedFindercLcLChainmUgR(void *p) {
      delete ((list<LxTbBinnedFinder::Chain*>*)p);
   }
   static void deleteArray_listlELxTbBinnedFindercLcLChainmUgR(void *p) {
      delete [] ((list<LxTbBinnedFinder::Chain*>*)p);
   }
   static void destruct_listlELxTbBinnedFindercLcLChainmUgR(void *p) {
      typedef list<LxTbBinnedFinder::Chain*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class list<LxTbBinnedFinder::Chain*>

namespace {
  void TriggerDictionaryInitialization_libLxTrack_Impl() {
    static const char* headers[] = {
"LxTBTask.h",
"LxTBTieTracks.h",
"LxTBTrdTask.h",
"GenNoiseElectrons.h",
"CalcStats.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/tracking/lx",
"/home/heinemann/gpuunpacker/cbmroot/reco/tracking/lx/TBBinned",
"/home/heinemann/gpuunpacker/cbmroot/reco/tracking/lx/GenNoiseElectrons",
"/home/heinemann/gpuunpacker/cbmroot/reco/tracking/lx/CalcStats",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/tracking/lx",
"/home/heinemann/gpuunpacker/cbmroot/reco/tracking/lx/TBBinned",
"/home/heinemann/gpuunpacker/cbmroot/reco/tracking/lx/GenNoiseElectrons",
"/home/heinemann/gpuunpacker/cbmroot/reco/tracking/lx/CalcStats",
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
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/much/qa",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/finder",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/fitter",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/propagation",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/selection",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/data",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/base",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/utils",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/base",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/utils",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/elid",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/reco",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/generators",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/mc",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/base",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/material",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/tracking",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/clustering",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/fit",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/field",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/fieldapr",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/radlength",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/tof",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca/simd",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/lx/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libLxTrack dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$LxTBTask.h")))  LxTBFinder;
class __attribute__((annotate("$clingAutoload$LxTBTrdTask.h")))  LxTBTrdFinder;
class __attribute__((annotate("$clingAutoload$GenNoiseElectrons.h")))  LxGenNoiseElectrons;
class __attribute__((annotate("$clingAutoload$CalcStats.h")))  LxCalcStats;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libLxTrack dictionary payload"

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
#include "LxTBTask.h"
#include "LxTBTieTracks.h"
#include "LxTBTrdTask.h"
#include "GenNoiseElectrons.h"
#include "CalcStats.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"LxCalcStats", payloadCode, "@",
"LxGenNoiseElectrons", payloadCode, "@",
"LxTBFinder", payloadCode, "@",
"LxTBTrdFinder", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libLxTrack",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libLxTrack_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libLxTrack_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libLxTrack() {
  TriggerDictionaryInitialization_libLxTrack_Impl();
}
