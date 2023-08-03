// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmGlobalTracking
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
#include "CbmGlobalTrackingTask.h"
#include "CbmGlobalTrackingQa.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmGlobalTrackingTask(void *p = nullptr);
   static void *newArray_CbmGlobalTrackingTask(Long_t size, void *p);
   static void delete_CbmGlobalTrackingTask(void *p);
   static void deleteArray_CbmGlobalTrackingTask(void *p);
   static void destruct_CbmGlobalTrackingTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGlobalTrackingTask*)
   {
      ::CbmGlobalTrackingTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGlobalTrackingTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGlobalTrackingTask", ::CbmGlobalTrackingTask::Class_Version(), "CbmGlobalTrackingTask.h", 24,
                  typeid(::CbmGlobalTrackingTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGlobalTrackingTask::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGlobalTrackingTask) );
      instance.SetNew(&new_CbmGlobalTrackingTask);
      instance.SetNewArray(&newArray_CbmGlobalTrackingTask);
      instance.SetDelete(&delete_CbmGlobalTrackingTask);
      instance.SetDeleteArray(&deleteArray_CbmGlobalTrackingTask);
      instance.SetDestructor(&destruct_CbmGlobalTrackingTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGlobalTrackingTask*)
   {
      return GenerateInitInstanceLocal((::CbmGlobalTrackingTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGlobalTrackingTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmGlobalTrackingQa(void *p = nullptr);
   static void *newArray_CbmGlobalTrackingQa(Long_t size, void *p);
   static void delete_CbmGlobalTrackingQa(void *p);
   static void deleteArray_CbmGlobalTrackingQa(void *p);
   static void destruct_CbmGlobalTrackingQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGlobalTrackingQa*)
   {
      ::CbmGlobalTrackingQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGlobalTrackingQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGlobalTrackingQa", ::CbmGlobalTrackingQa::Class_Version(), "CbmGlobalTrackingQa.h", 31,
                  typeid(::CbmGlobalTrackingQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGlobalTrackingQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGlobalTrackingQa) );
      instance.SetNew(&new_CbmGlobalTrackingQa);
      instance.SetNewArray(&newArray_CbmGlobalTrackingQa);
      instance.SetDelete(&delete_CbmGlobalTrackingQa);
      instance.SetDeleteArray(&deleteArray_CbmGlobalTrackingQa);
      instance.SetDestructor(&destruct_CbmGlobalTrackingQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGlobalTrackingQa*)
   {
      return GenerateInitInstanceLocal((::CbmGlobalTrackingQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGlobalTrackingQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmGlobalTrackingTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGlobalTrackingTask::Class_Name()
{
   return "CbmGlobalTrackingTask";
}

//______________________________________________________________________________
const char *CbmGlobalTrackingTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackingTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGlobalTrackingTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackingTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGlobalTrackingTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackingTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGlobalTrackingTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackingTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGlobalTrackingQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGlobalTrackingQa::Class_Name()
{
   return "CbmGlobalTrackingQa";
}

//______________________________________________________________________________
const char *CbmGlobalTrackingQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackingQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGlobalTrackingQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackingQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGlobalTrackingQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackingQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGlobalTrackingQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackingQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmGlobalTrackingTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGlobalTrackingTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGlobalTrackingTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGlobalTrackingTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGlobalTrackingTask(void *p) {
      return  p ? new(p) ::CbmGlobalTrackingTask : new ::CbmGlobalTrackingTask;
   }
   static void *newArray_CbmGlobalTrackingTask(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGlobalTrackingTask[nElements] : new ::CbmGlobalTrackingTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGlobalTrackingTask(void *p) {
      delete ((::CbmGlobalTrackingTask*)p);
   }
   static void deleteArray_CbmGlobalTrackingTask(void *p) {
      delete [] ((::CbmGlobalTrackingTask*)p);
   }
   static void destruct_CbmGlobalTrackingTask(void *p) {
      typedef ::CbmGlobalTrackingTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGlobalTrackingTask

//______________________________________________________________________________
void CbmGlobalTrackingQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGlobalTrackingQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGlobalTrackingQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGlobalTrackingQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGlobalTrackingQa(void *p) {
      return  p ? new(p) ::CbmGlobalTrackingQa : new ::CbmGlobalTrackingQa;
   }
   static void *newArray_CbmGlobalTrackingQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGlobalTrackingQa[nElements] : new ::CbmGlobalTrackingQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGlobalTrackingQa(void *p) {
      delete ((::CbmGlobalTrackingQa*)p);
   }
   static void deleteArray_CbmGlobalTrackingQa(void *p) {
      delete [] ((::CbmGlobalTrackingQa*)p);
   }
   static void destruct_CbmGlobalTrackingQa(void *p) {
      typedef ::CbmGlobalTrackingQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGlobalTrackingQa

namespace ROOT {
   static TClass *vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR_Dictionary();
   static void vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR(void *p);
   static void deleteArray_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR(void *p);
   static void destruct_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<CbmGlobalTrackingQa::TrackData> >*)
   {
      vector<vector<CbmGlobalTrackingQa::TrackData> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<CbmGlobalTrackingQa::TrackData> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<CbmGlobalTrackingQa::TrackData> >", -2, "vector", 389,
                  typeid(vector<vector<CbmGlobalTrackingQa::TrackData> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<CbmGlobalTrackingQa::TrackData> >) );
      instance.SetNew(&new_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<CbmGlobalTrackingQa::TrackData> > >()));

      ::ROOT::AddClassAlternate("vector<vector<CbmGlobalTrackingQa::TrackData> >","std::vector<std::vector<CbmGlobalTrackingQa::TrackData, std::allocator<CbmGlobalTrackingQa::TrackData> >, std::allocator<std::vector<CbmGlobalTrackingQa::TrackData, std::allocator<CbmGlobalTrackingQa::TrackData> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<CbmGlobalTrackingQa::TrackData> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<CbmGlobalTrackingQa::TrackData> >*)nullptr)->GetClass();
      vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmGlobalTrackingQa::TrackData> > : new vector<vector<CbmGlobalTrackingQa::TrackData> >;
   }
   static void *newArray_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmGlobalTrackingQa::TrackData> >[nElements] : new vector<vector<CbmGlobalTrackingQa::TrackData> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR(void *p) {
      delete ((vector<vector<CbmGlobalTrackingQa::TrackData> >*)p);
   }
   static void deleteArray_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR(void *p) {
      delete [] ((vector<vector<CbmGlobalTrackingQa::TrackData> >*)p);
   }
   static void destruct_vectorlEvectorlECbmGlobalTrackingQacLcLTrackDatagRsPgR(void *p) {
      typedef vector<vector<CbmGlobalTrackingQa::TrackData> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<CbmGlobalTrackingQa::TrackData> >

namespace ROOT {
   static TClass *vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR_Dictionary();
   static void vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR(void *p);
   static void deleteArray_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR(void *p);
   static void destruct_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<CbmGlobalTrackingQa::PointData> >*)
   {
      vector<vector<CbmGlobalTrackingQa::PointData> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<CbmGlobalTrackingQa::PointData> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<CbmGlobalTrackingQa::PointData> >", -2, "vector", 389,
                  typeid(vector<vector<CbmGlobalTrackingQa::PointData> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<CbmGlobalTrackingQa::PointData> >) );
      instance.SetNew(&new_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<CbmGlobalTrackingQa::PointData> > >()));

      ::ROOT::AddClassAlternate("vector<vector<CbmGlobalTrackingQa::PointData> >","std::vector<std::vector<CbmGlobalTrackingQa::PointData, std::allocator<CbmGlobalTrackingQa::PointData> >, std::allocator<std::vector<CbmGlobalTrackingQa::PointData, std::allocator<CbmGlobalTrackingQa::PointData> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<CbmGlobalTrackingQa::PointData> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<CbmGlobalTrackingQa::PointData> >*)nullptr)->GetClass();
      vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmGlobalTrackingQa::PointData> > : new vector<vector<CbmGlobalTrackingQa::PointData> >;
   }
   static void *newArray_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmGlobalTrackingQa::PointData> >[nElements] : new vector<vector<CbmGlobalTrackingQa::PointData> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR(void *p) {
      delete ((vector<vector<CbmGlobalTrackingQa::PointData> >*)p);
   }
   static void deleteArray_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR(void *p) {
      delete [] ((vector<vector<CbmGlobalTrackingQa::PointData> >*)p);
   }
   static void destruct_vectorlEvectorlECbmGlobalTrackingQacLcLPointDatagRsPgR(void *p) {
      typedef vector<vector<CbmGlobalTrackingQa::PointData> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<CbmGlobalTrackingQa::PointData> >

namespace ROOT {
   static TClass *vectorlECbmGlobalTrackingQacLcLTrackDatagR_Dictionary();
   static void vectorlECbmGlobalTrackingQacLcLTrackDatagR_TClassManip(TClass*);
   static void *new_vectorlECbmGlobalTrackingQacLcLTrackDatagR(void *p = nullptr);
   static void *newArray_vectorlECbmGlobalTrackingQacLcLTrackDatagR(Long_t size, void *p);
   static void delete_vectorlECbmGlobalTrackingQacLcLTrackDatagR(void *p);
   static void deleteArray_vectorlECbmGlobalTrackingQacLcLTrackDatagR(void *p);
   static void destruct_vectorlECbmGlobalTrackingQacLcLTrackDatagR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmGlobalTrackingQa::TrackData>*)
   {
      vector<CbmGlobalTrackingQa::TrackData> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmGlobalTrackingQa::TrackData>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmGlobalTrackingQa::TrackData>", -2, "vector", 389,
                  typeid(vector<CbmGlobalTrackingQa::TrackData>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmGlobalTrackingQacLcLTrackDatagR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmGlobalTrackingQa::TrackData>) );
      instance.SetNew(&new_vectorlECbmGlobalTrackingQacLcLTrackDatagR);
      instance.SetNewArray(&newArray_vectorlECbmGlobalTrackingQacLcLTrackDatagR);
      instance.SetDelete(&delete_vectorlECbmGlobalTrackingQacLcLTrackDatagR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmGlobalTrackingQacLcLTrackDatagR);
      instance.SetDestructor(&destruct_vectorlECbmGlobalTrackingQacLcLTrackDatagR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmGlobalTrackingQa::TrackData> >()));

      ::ROOT::AddClassAlternate("vector<CbmGlobalTrackingQa::TrackData>","std::vector<CbmGlobalTrackingQa::TrackData, std::allocator<CbmGlobalTrackingQa::TrackData> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmGlobalTrackingQa::TrackData>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmGlobalTrackingQacLcLTrackDatagR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmGlobalTrackingQa::TrackData>*)nullptr)->GetClass();
      vectorlECbmGlobalTrackingQacLcLTrackDatagR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmGlobalTrackingQacLcLTrackDatagR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmGlobalTrackingQacLcLTrackDatagR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmGlobalTrackingQa::TrackData> : new vector<CbmGlobalTrackingQa::TrackData>;
   }
   static void *newArray_vectorlECbmGlobalTrackingQacLcLTrackDatagR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmGlobalTrackingQa::TrackData>[nElements] : new vector<CbmGlobalTrackingQa::TrackData>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmGlobalTrackingQacLcLTrackDatagR(void *p) {
      delete ((vector<CbmGlobalTrackingQa::TrackData>*)p);
   }
   static void deleteArray_vectorlECbmGlobalTrackingQacLcLTrackDatagR(void *p) {
      delete [] ((vector<CbmGlobalTrackingQa::TrackData>*)p);
   }
   static void destruct_vectorlECbmGlobalTrackingQacLcLTrackDatagR(void *p) {
      typedef vector<CbmGlobalTrackingQa::TrackData> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmGlobalTrackingQa::TrackData>

namespace ROOT {
   static TClass *vectorlECbmGlobalTrackingQacLcLPointDatagR_Dictionary();
   static void vectorlECbmGlobalTrackingQacLcLPointDatagR_TClassManip(TClass*);
   static void *new_vectorlECbmGlobalTrackingQacLcLPointDatagR(void *p = nullptr);
   static void *newArray_vectorlECbmGlobalTrackingQacLcLPointDatagR(Long_t size, void *p);
   static void delete_vectorlECbmGlobalTrackingQacLcLPointDatagR(void *p);
   static void deleteArray_vectorlECbmGlobalTrackingQacLcLPointDatagR(void *p);
   static void destruct_vectorlECbmGlobalTrackingQacLcLPointDatagR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmGlobalTrackingQa::PointData>*)
   {
      vector<CbmGlobalTrackingQa::PointData> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmGlobalTrackingQa::PointData>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmGlobalTrackingQa::PointData>", -2, "vector", 389,
                  typeid(vector<CbmGlobalTrackingQa::PointData>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmGlobalTrackingQacLcLPointDatagR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmGlobalTrackingQa::PointData>) );
      instance.SetNew(&new_vectorlECbmGlobalTrackingQacLcLPointDatagR);
      instance.SetNewArray(&newArray_vectorlECbmGlobalTrackingQacLcLPointDatagR);
      instance.SetDelete(&delete_vectorlECbmGlobalTrackingQacLcLPointDatagR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmGlobalTrackingQacLcLPointDatagR);
      instance.SetDestructor(&destruct_vectorlECbmGlobalTrackingQacLcLPointDatagR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmGlobalTrackingQa::PointData> >()));

      ::ROOT::AddClassAlternate("vector<CbmGlobalTrackingQa::PointData>","std::vector<CbmGlobalTrackingQa::PointData, std::allocator<CbmGlobalTrackingQa::PointData> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmGlobalTrackingQa::PointData>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmGlobalTrackingQacLcLPointDatagR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmGlobalTrackingQa::PointData>*)nullptr)->GetClass();
      vectorlECbmGlobalTrackingQacLcLPointDatagR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmGlobalTrackingQacLcLPointDatagR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmGlobalTrackingQacLcLPointDatagR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmGlobalTrackingQa::PointData> : new vector<CbmGlobalTrackingQa::PointData>;
   }
   static void *newArray_vectorlECbmGlobalTrackingQacLcLPointDatagR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmGlobalTrackingQa::PointData>[nElements] : new vector<CbmGlobalTrackingQa::PointData>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmGlobalTrackingQacLcLPointDatagR(void *p) {
      delete ((vector<CbmGlobalTrackingQa::PointData>*)p);
   }
   static void deleteArray_vectorlECbmGlobalTrackingQacLcLPointDatagR(void *p) {
      delete [] ((vector<CbmGlobalTrackingQa::PointData>*)p);
   }
   static void destruct_vectorlECbmGlobalTrackingQacLcLPointDatagR(void *p) {
      typedef vector<CbmGlobalTrackingQa::PointData> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmGlobalTrackingQa::PointData>

namespace {
  void TriggerDictionaryInitialization_libCbmGlobalTracking_Impl() {
    static const char* headers[] = {
"CbmGlobalTrackingTask.h",
"CbmGlobalTrackingQa.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/tracking/global",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/tracking/global",
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
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/KFQA",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmGlobalTracking dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmGlobalTrackingTask.h")))  CbmGlobalTrackingTask;
class __attribute__((annotate("$clingAutoload$CbmGlobalTrackingQa.h")))  CbmGlobalTrackingQa;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmGlobalTracking dictionary payload"

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
#include "CbmGlobalTrackingTask.h"
#include "CbmGlobalTrackingQa.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmGlobalTrackingQa", payloadCode, "@",
"CbmGlobalTrackingTask", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmGlobalTracking",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmGlobalTracking_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmGlobalTracking_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmGlobalTracking() {
  TriggerDictionaryInitialization_libCbmGlobalTracking_Impl();
}
