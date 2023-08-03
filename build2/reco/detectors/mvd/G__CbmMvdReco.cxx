// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmMvdReco
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
#include "CbmMvdSensorHitfinderTask.h"
#include "CbmMvdSensorClusterfinderTask.h"
#include "CbmMvdHitfinder.h"
#include "CbmMvdClusterfinder.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmMvdSensorHitfinderTask(void *p = nullptr);
   static void *newArray_CbmMvdSensorHitfinderTask(Long_t size, void *p);
   static void delete_CbmMvdSensorHitfinderTask(void *p);
   static void deleteArray_CbmMvdSensorHitfinderTask(void *p);
   static void destruct_CbmMvdSensorHitfinderTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdSensorHitfinderTask*)
   {
      ::CbmMvdSensorHitfinderTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdSensorHitfinderTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdSensorHitfinderTask", ::CbmMvdSensorHitfinderTask::Class_Version(), "CbmMvdSensorHitfinderTask.h", 38,
                  typeid(::CbmMvdSensorHitfinderTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdSensorHitfinderTask::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdSensorHitfinderTask) );
      instance.SetNew(&new_CbmMvdSensorHitfinderTask);
      instance.SetNewArray(&newArray_CbmMvdSensorHitfinderTask);
      instance.SetDelete(&delete_CbmMvdSensorHitfinderTask);
      instance.SetDeleteArray(&deleteArray_CbmMvdSensorHitfinderTask);
      instance.SetDestructor(&destruct_CbmMvdSensorHitfinderTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdSensorHitfinderTask*)
   {
      return GenerateInitInstanceLocal((::CbmMvdSensorHitfinderTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdSensorHitfinderTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdSensorClusterfinderTask(void *p = nullptr);
   static void *newArray_CbmMvdSensorClusterfinderTask(Long_t size, void *p);
   static void delete_CbmMvdSensorClusterfinderTask(void *p);
   static void deleteArray_CbmMvdSensorClusterfinderTask(void *p);
   static void destruct_CbmMvdSensorClusterfinderTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdSensorClusterfinderTask*)
   {
      ::CbmMvdSensorClusterfinderTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdSensorClusterfinderTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdSensorClusterfinderTask", ::CbmMvdSensorClusterfinderTask::Class_Version(), "CbmMvdSensorClusterfinderTask.h", 41,
                  typeid(::CbmMvdSensorClusterfinderTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdSensorClusterfinderTask::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdSensorClusterfinderTask) );
      instance.SetNew(&new_CbmMvdSensorClusterfinderTask);
      instance.SetNewArray(&newArray_CbmMvdSensorClusterfinderTask);
      instance.SetDelete(&delete_CbmMvdSensorClusterfinderTask);
      instance.SetDeleteArray(&deleteArray_CbmMvdSensorClusterfinderTask);
      instance.SetDestructor(&destruct_CbmMvdSensorClusterfinderTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdSensorClusterfinderTask*)
   {
      return GenerateInitInstanceLocal((::CbmMvdSensorClusterfinderTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdSensorClusterfinderTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdHitfinder(void *p = nullptr);
   static void *newArray_CbmMvdHitfinder(Long_t size, void *p);
   static void delete_CbmMvdHitfinder(void *p);
   static void deleteArray_CbmMvdHitfinder(void *p);
   static void destruct_CbmMvdHitfinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdHitfinder*)
   {
      ::CbmMvdHitfinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdHitfinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdHitfinder", ::CbmMvdHitfinder::Class_Version(), "CbmMvdHitfinder.h", 29,
                  typeid(::CbmMvdHitfinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdHitfinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdHitfinder) );
      instance.SetNew(&new_CbmMvdHitfinder);
      instance.SetNewArray(&newArray_CbmMvdHitfinder);
      instance.SetDelete(&delete_CbmMvdHitfinder);
      instance.SetDeleteArray(&deleteArray_CbmMvdHitfinder);
      instance.SetDestructor(&destruct_CbmMvdHitfinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdHitfinder*)
   {
      return GenerateInitInstanceLocal((::CbmMvdHitfinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdHitfinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdClusterfinder(void *p = nullptr);
   static void *newArray_CbmMvdClusterfinder(Long_t size, void *p);
   static void delete_CbmMvdClusterfinder(void *p);
   static void deleteArray_CbmMvdClusterfinder(void *p);
   static void destruct_CbmMvdClusterfinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdClusterfinder*)
   {
      ::CbmMvdClusterfinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdClusterfinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdClusterfinder", ::CbmMvdClusterfinder::Class_Version(), "CbmMvdClusterfinder.h", 29,
                  typeid(::CbmMvdClusterfinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdClusterfinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdClusterfinder) );
      instance.SetNew(&new_CbmMvdClusterfinder);
      instance.SetNewArray(&newArray_CbmMvdClusterfinder);
      instance.SetDelete(&delete_CbmMvdClusterfinder);
      instance.SetDeleteArray(&deleteArray_CbmMvdClusterfinder);
      instance.SetDestructor(&destruct_CbmMvdClusterfinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdClusterfinder*)
   {
      return GenerateInitInstanceLocal((::CbmMvdClusterfinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdClusterfinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdSensorHitfinderTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdSensorHitfinderTask::Class_Name()
{
   return "CbmMvdSensorHitfinderTask";
}

//______________________________________________________________________________
const char *CbmMvdSensorHitfinderTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorHitfinderTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdSensorHitfinderTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorHitfinderTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdSensorHitfinderTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorHitfinderTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdSensorHitfinderTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorHitfinderTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdSensorClusterfinderTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdSensorClusterfinderTask::Class_Name()
{
   return "CbmMvdSensorClusterfinderTask";
}

//______________________________________________________________________________
const char *CbmMvdSensorClusterfinderTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorClusterfinderTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdSensorClusterfinderTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorClusterfinderTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdSensorClusterfinderTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorClusterfinderTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdSensorClusterfinderTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorClusterfinderTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdHitfinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdHitfinder::Class_Name()
{
   return "CbmMvdHitfinder";
}

//______________________________________________________________________________
const char *CbmMvdHitfinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdHitfinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdHitfinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdHitfinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdHitfinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdHitfinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdHitfinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdHitfinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdClusterfinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdClusterfinder::Class_Name()
{
   return "CbmMvdClusterfinder";
}

//______________________________________________________________________________
const char *CbmMvdClusterfinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdClusterfinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdClusterfinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdClusterfinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdClusterfinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdClusterfinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdClusterfinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdClusterfinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmMvdSensorHitfinderTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdSensorHitfinderTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdSensorHitfinderTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdSensorHitfinderTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdSensorHitfinderTask(void *p) {
      return  p ? new(p) ::CbmMvdSensorHitfinderTask : new ::CbmMvdSensorHitfinderTask;
   }
   static void *newArray_CbmMvdSensorHitfinderTask(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdSensorHitfinderTask[nElements] : new ::CbmMvdSensorHitfinderTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdSensorHitfinderTask(void *p) {
      delete ((::CbmMvdSensorHitfinderTask*)p);
   }
   static void deleteArray_CbmMvdSensorHitfinderTask(void *p) {
      delete [] ((::CbmMvdSensorHitfinderTask*)p);
   }
   static void destruct_CbmMvdSensorHitfinderTask(void *p) {
      typedef ::CbmMvdSensorHitfinderTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdSensorHitfinderTask

//______________________________________________________________________________
void CbmMvdSensorClusterfinderTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdSensorClusterfinderTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdSensorClusterfinderTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdSensorClusterfinderTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdSensorClusterfinderTask(void *p) {
      return  p ? new(p) ::CbmMvdSensorClusterfinderTask : new ::CbmMvdSensorClusterfinderTask;
   }
   static void *newArray_CbmMvdSensorClusterfinderTask(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdSensorClusterfinderTask[nElements] : new ::CbmMvdSensorClusterfinderTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdSensorClusterfinderTask(void *p) {
      delete ((::CbmMvdSensorClusterfinderTask*)p);
   }
   static void deleteArray_CbmMvdSensorClusterfinderTask(void *p) {
      delete [] ((::CbmMvdSensorClusterfinderTask*)p);
   }
   static void destruct_CbmMvdSensorClusterfinderTask(void *p) {
      typedef ::CbmMvdSensorClusterfinderTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdSensorClusterfinderTask

//______________________________________________________________________________
void CbmMvdHitfinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdHitfinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdHitfinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdHitfinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdHitfinder(void *p) {
      return  p ? new(p) ::CbmMvdHitfinder : new ::CbmMvdHitfinder;
   }
   static void *newArray_CbmMvdHitfinder(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdHitfinder[nElements] : new ::CbmMvdHitfinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdHitfinder(void *p) {
      delete ((::CbmMvdHitfinder*)p);
   }
   static void deleteArray_CbmMvdHitfinder(void *p) {
      delete [] ((::CbmMvdHitfinder*)p);
   }
   static void destruct_CbmMvdHitfinder(void *p) {
      typedef ::CbmMvdHitfinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdHitfinder

//______________________________________________________________________________
void CbmMvdClusterfinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdClusterfinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdClusterfinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdClusterfinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdClusterfinder(void *p) {
      return  p ? new(p) ::CbmMvdClusterfinder : new ::CbmMvdClusterfinder;
   }
   static void *newArray_CbmMvdClusterfinder(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdClusterfinder[nElements] : new ::CbmMvdClusterfinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdClusterfinder(void *p) {
      delete ((::CbmMvdClusterfinder*)p);
   }
   static void deleteArray_CbmMvdClusterfinder(void *p) {
      delete [] ((::CbmMvdClusterfinder*)p);
   }
   static void destruct_CbmMvdClusterfinder(void *p) {
      typedef ::CbmMvdClusterfinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdClusterfinder

namespace ROOT {
   static TClass *maplEpairlEintcOintgRcOintgR_Dictionary();
   static void maplEpairlEintcOintgRcOintgR_TClassManip(TClass*);
   static void *new_maplEpairlEintcOintgRcOintgR(void *p = nullptr);
   static void *newArray_maplEpairlEintcOintgRcOintgR(Long_t size, void *p);
   static void delete_maplEpairlEintcOintgRcOintgR(void *p);
   static void deleteArray_maplEpairlEintcOintgRcOintgR(void *p);
   static void destruct_maplEpairlEintcOintgRcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<pair<int,int>,int>*)
   {
      map<pair<int,int>,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<pair<int,int>,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<pair<int,int>,int>", -2, "map", 100,
                  typeid(map<pair<int,int>,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEpairlEintcOintgRcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<pair<int,int>,int>) );
      instance.SetNew(&new_maplEpairlEintcOintgRcOintgR);
      instance.SetNewArray(&newArray_maplEpairlEintcOintgRcOintgR);
      instance.SetDelete(&delete_maplEpairlEintcOintgRcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEpairlEintcOintgRcOintgR);
      instance.SetDestructor(&destruct_maplEpairlEintcOintgRcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<pair<int,int>,int> >()));

      ::ROOT::AddClassAlternate("map<pair<int,int>,int>","std::map<std::pair<int, int>, int, std::less<std::pair<int, int> >, std::allocator<std::pair<std::pair<int, int> const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<pair<int,int>,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEpairlEintcOintgRcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<pair<int,int>,int>*)nullptr)->GetClass();
      maplEpairlEintcOintgRcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEpairlEintcOintgRcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEpairlEintcOintgRcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,int>,int> : new map<pair<int,int>,int>;
   }
   static void *newArray_maplEpairlEintcOintgRcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,int>,int>[nElements] : new map<pair<int,int>,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEpairlEintcOintgRcOintgR(void *p) {
      delete ((map<pair<int,int>,int>*)p);
   }
   static void deleteArray_maplEpairlEintcOintgRcOintgR(void *p) {
      delete [] ((map<pair<int,int>,int>*)p);
   }
   static void destruct_maplEpairlEintcOintgRcOintgR(void *p) {
      typedef map<pair<int,int>,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<pair<int,int>,int>

namespace {
  void TriggerDictionaryInitialization_libCbmMvdReco_Impl() {
    static const char* headers[] = {
"CbmMvdSensorHitfinderTask.h",
"CbmMvdSensorClusterfinderTask.h",
"CbmMvdHitfinder.h",
"CbmMvdClusterfinder.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/mvd/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/mvd/plugins/tasks",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/mvd/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/mvd/plugins/tasks",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/tools",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/SensorDataSheets",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/tasks",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/buffers",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/detectors/mvd/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmMvdReco dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmMvdSensorHitfinderTask.h")))  CbmMvdSensorHitfinderTask;
class __attribute__((annotate("$clingAutoload$CbmMvdSensorClusterfinderTask.h")))  CbmMvdSensorClusterfinderTask;
class __attribute__((annotate("$clingAutoload$CbmMvdHitfinder.h")))  CbmMvdHitfinder;
class __attribute__((annotate("$clingAutoload$CbmMvdClusterfinder.h")))  CbmMvdClusterfinder;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmMvdReco dictionary payload"

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
#include "CbmMvdSensorHitfinderTask.h"
#include "CbmMvdSensorClusterfinderTask.h"
#include "CbmMvdHitfinder.h"
#include "CbmMvdClusterfinder.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmMvdClusterfinder", payloadCode, "@",
"CbmMvdHitfinder", payloadCode, "@",
"CbmMvdSensorClusterfinderTask", payloadCode, "@",
"CbmMvdSensorHitfinderTask", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmMvdReco",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmMvdReco_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmMvdReco_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmMvdReco() {
  TriggerDictionaryInitialization_libCbmMvdReco_Impl();
}
