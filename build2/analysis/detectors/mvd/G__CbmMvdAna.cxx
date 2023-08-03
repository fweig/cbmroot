// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmMvdAna
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
#include "CbmMvdReadoutSimple.h"
#include "CbmMvdReadoutCluster.h"
#include "CbmMvdReadout.h"
#include "CbmMvdSensorReadoutTask.h"
#include "CbmMvdQa.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmMvdReadoutSimple(void *p = nullptr);
   static void *newArray_CbmMvdReadoutSimple(Long_t size, void *p);
   static void delete_CbmMvdReadoutSimple(void *p);
   static void deleteArray_CbmMvdReadoutSimple(void *p);
   static void destruct_CbmMvdReadoutSimple(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdReadoutSimple*)
   {
      ::CbmMvdReadoutSimple *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdReadoutSimple >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdReadoutSimple", ::CbmMvdReadoutSimple::Class_Version(), "CbmMvdReadoutSimple.h", 36,
                  typeid(::CbmMvdReadoutSimple), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdReadoutSimple::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdReadoutSimple) );
      instance.SetNew(&new_CbmMvdReadoutSimple);
      instance.SetNewArray(&newArray_CbmMvdReadoutSimple);
      instance.SetDelete(&delete_CbmMvdReadoutSimple);
      instance.SetDeleteArray(&deleteArray_CbmMvdReadoutSimple);
      instance.SetDestructor(&destruct_CbmMvdReadoutSimple);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdReadoutSimple*)
   {
      return GenerateInitInstanceLocal((::CbmMvdReadoutSimple*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdReadoutSimple*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdReadoutCluster(void *p = nullptr);
   static void *newArray_CbmMvdReadoutCluster(Long_t size, void *p);
   static void delete_CbmMvdReadoutCluster(void *p);
   static void deleteArray_CbmMvdReadoutCluster(void *p);
   static void destruct_CbmMvdReadoutCluster(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdReadoutCluster*)
   {
      ::CbmMvdReadoutCluster *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdReadoutCluster >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdReadoutCluster", ::CbmMvdReadoutCluster::Class_Version(), "CbmMvdReadoutCluster.h", 33,
                  typeid(::CbmMvdReadoutCluster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdReadoutCluster::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdReadoutCluster) );
      instance.SetNew(&new_CbmMvdReadoutCluster);
      instance.SetNewArray(&newArray_CbmMvdReadoutCluster);
      instance.SetDelete(&delete_CbmMvdReadoutCluster);
      instance.SetDeleteArray(&deleteArray_CbmMvdReadoutCluster);
      instance.SetDestructor(&destruct_CbmMvdReadoutCluster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdReadoutCluster*)
   {
      return GenerateInitInstanceLocal((::CbmMvdReadoutCluster*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdReadoutCluster*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdReadout(void *p = nullptr);
   static void *newArray_CbmMvdReadout(Long_t size, void *p);
   static void delete_CbmMvdReadout(void *p);
   static void deleteArray_CbmMvdReadout(void *p);
   static void destruct_CbmMvdReadout(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdReadout*)
   {
      ::CbmMvdReadout *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdReadout >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdReadout", ::CbmMvdReadout::Class_Version(), "CbmMvdReadout.h", 25,
                  typeid(::CbmMvdReadout), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdReadout::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdReadout) );
      instance.SetNew(&new_CbmMvdReadout);
      instance.SetNewArray(&newArray_CbmMvdReadout);
      instance.SetDelete(&delete_CbmMvdReadout);
      instance.SetDeleteArray(&deleteArray_CbmMvdReadout);
      instance.SetDestructor(&destruct_CbmMvdReadout);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdReadout*)
   {
      return GenerateInitInstanceLocal((::CbmMvdReadout*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdReadout*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdSensorReadoutTask(void *p = nullptr);
   static void *newArray_CbmMvdSensorReadoutTask(Long_t size, void *p);
   static void delete_CbmMvdSensorReadoutTask(void *p);
   static void deleteArray_CbmMvdSensorReadoutTask(void *p);
   static void destruct_CbmMvdSensorReadoutTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdSensorReadoutTask*)
   {
      ::CbmMvdSensorReadoutTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdSensorReadoutTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdSensorReadoutTask", ::CbmMvdSensorReadoutTask::Class_Version(), "CbmMvdSensorReadoutTask.h", 32,
                  typeid(::CbmMvdSensorReadoutTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdSensorReadoutTask::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdSensorReadoutTask) );
      instance.SetNew(&new_CbmMvdSensorReadoutTask);
      instance.SetNewArray(&newArray_CbmMvdSensorReadoutTask);
      instance.SetDelete(&delete_CbmMvdSensorReadoutTask);
      instance.SetDeleteArray(&deleteArray_CbmMvdSensorReadoutTask);
      instance.SetDestructor(&destruct_CbmMvdSensorReadoutTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdSensorReadoutTask*)
   {
      return GenerateInitInstanceLocal((::CbmMvdSensorReadoutTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdSensorReadoutTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdQa(void *p = nullptr);
   static void *newArray_CbmMvdQa(Long_t size, void *p);
   static void delete_CbmMvdQa(void *p);
   static void deleteArray_CbmMvdQa(void *p);
   static void destruct_CbmMvdQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdQa*)
   {
      ::CbmMvdQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdQa", ::CbmMvdQa::Class_Version(), "CbmMvdQa.h", 38,
                  typeid(::CbmMvdQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdQa) );
      instance.SetNew(&new_CbmMvdQa);
      instance.SetNewArray(&newArray_CbmMvdQa);
      instance.SetDelete(&delete_CbmMvdQa);
      instance.SetDeleteArray(&deleteArray_CbmMvdQa);
      instance.SetDestructor(&destruct_CbmMvdQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdQa*)
   {
      return GenerateInitInstanceLocal((::CbmMvdQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdReadoutSimple::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdReadoutSimple::Class_Name()
{
   return "CbmMvdReadoutSimple";
}

//______________________________________________________________________________
const char *CbmMvdReadoutSimple::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdReadoutSimple*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdReadoutSimple::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdReadoutSimple*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdReadoutSimple::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdReadoutSimple*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdReadoutSimple::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdReadoutSimple*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdReadoutCluster::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdReadoutCluster::Class_Name()
{
   return "CbmMvdReadoutCluster";
}

//______________________________________________________________________________
const char *CbmMvdReadoutCluster::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdReadoutCluster*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdReadoutCluster::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdReadoutCluster*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdReadoutCluster::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdReadoutCluster*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdReadoutCluster::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdReadoutCluster*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdReadout::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdReadout::Class_Name()
{
   return "CbmMvdReadout";
}

//______________________________________________________________________________
const char *CbmMvdReadout::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdReadout*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdReadout::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdReadout*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdReadout::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdReadout*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdReadout::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdReadout*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdSensorReadoutTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdSensorReadoutTask::Class_Name()
{
   return "CbmMvdSensorReadoutTask";
}

//______________________________________________________________________________
const char *CbmMvdSensorReadoutTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorReadoutTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdSensorReadoutTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorReadoutTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdSensorReadoutTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorReadoutTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdSensorReadoutTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorReadoutTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdQa::Class_Name()
{
   return "CbmMvdQa";
}

//______________________________________________________________________________
const char *CbmMvdQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmMvdReadoutSimple::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdReadoutSimple.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdReadoutSimple::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdReadoutSimple::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdReadoutSimple(void *p) {
      return  p ? new(p) ::CbmMvdReadoutSimple : new ::CbmMvdReadoutSimple;
   }
   static void *newArray_CbmMvdReadoutSimple(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdReadoutSimple[nElements] : new ::CbmMvdReadoutSimple[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdReadoutSimple(void *p) {
      delete ((::CbmMvdReadoutSimple*)p);
   }
   static void deleteArray_CbmMvdReadoutSimple(void *p) {
      delete [] ((::CbmMvdReadoutSimple*)p);
   }
   static void destruct_CbmMvdReadoutSimple(void *p) {
      typedef ::CbmMvdReadoutSimple current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdReadoutSimple

//______________________________________________________________________________
void CbmMvdReadoutCluster::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdReadoutCluster.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdReadoutCluster::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdReadoutCluster::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdReadoutCluster(void *p) {
      return  p ? new(p) ::CbmMvdReadoutCluster : new ::CbmMvdReadoutCluster;
   }
   static void *newArray_CbmMvdReadoutCluster(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdReadoutCluster[nElements] : new ::CbmMvdReadoutCluster[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdReadoutCluster(void *p) {
      delete ((::CbmMvdReadoutCluster*)p);
   }
   static void deleteArray_CbmMvdReadoutCluster(void *p) {
      delete [] ((::CbmMvdReadoutCluster*)p);
   }
   static void destruct_CbmMvdReadoutCluster(void *p) {
      typedef ::CbmMvdReadoutCluster current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdReadoutCluster

//______________________________________________________________________________
void CbmMvdReadout::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdReadout.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdReadout::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdReadout::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdReadout(void *p) {
      return  p ? new(p) ::CbmMvdReadout : new ::CbmMvdReadout;
   }
   static void *newArray_CbmMvdReadout(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdReadout[nElements] : new ::CbmMvdReadout[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdReadout(void *p) {
      delete ((::CbmMvdReadout*)p);
   }
   static void deleteArray_CbmMvdReadout(void *p) {
      delete [] ((::CbmMvdReadout*)p);
   }
   static void destruct_CbmMvdReadout(void *p) {
      typedef ::CbmMvdReadout current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdReadout

//______________________________________________________________________________
void CbmMvdSensorReadoutTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdSensorReadoutTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdSensorReadoutTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdSensorReadoutTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdSensorReadoutTask(void *p) {
      return  p ? new(p) ::CbmMvdSensorReadoutTask : new ::CbmMvdSensorReadoutTask;
   }
   static void *newArray_CbmMvdSensorReadoutTask(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdSensorReadoutTask[nElements] : new ::CbmMvdSensorReadoutTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdSensorReadoutTask(void *p) {
      delete ((::CbmMvdSensorReadoutTask*)p);
   }
   static void deleteArray_CbmMvdSensorReadoutTask(void *p) {
      delete [] ((::CbmMvdSensorReadoutTask*)p);
   }
   static void destruct_CbmMvdSensorReadoutTask(void *p) {
      typedef ::CbmMvdSensorReadoutTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdSensorReadoutTask

//______________________________________________________________________________
void CbmMvdQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdQa(void *p) {
      return  p ? new(p) ::CbmMvdQa : new ::CbmMvdQa;
   }
   static void *newArray_CbmMvdQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdQa[nElements] : new ::CbmMvdQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdQa(void *p) {
      delete ((::CbmMvdQa*)p);
   }
   static void deleteArray_CbmMvdQa(void *p) {
      delete [] ((::CbmMvdQa*)p);
   }
   static void destruct_CbmMvdQa(void *p) {
      typedef ::CbmMvdQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdQa

namespace {
  void TriggerDictionaryInitialization_libCbmMvdAna_Impl() {
    static const char* headers[] = {
"CbmMvdReadoutSimple.h",
"CbmMvdReadoutCluster.h",
"CbmMvdReadout.h",
"CbmMvdSensorReadoutTask.h",
"CbmMvdQa.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/analysis/detectors/mvd/qa",
"/home/heinemann/gpuunpacker/cbmroot/analysis/detectors/mvd/plugins/tasks",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/analysis/detectors/mvd/qa",
"/home/heinemann/gpuunpacker/cbmroot/analysis/detectors/mvd/plugins/tasks",
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
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/detectors/mvd/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmMvdAna dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmMvdReadoutSimple.h")))  CbmMvdReadoutSimple;
class __attribute__((annotate("$clingAutoload$CbmMvdReadoutCluster.h")))  CbmMvdReadoutCluster;
class __attribute__((annotate("$clingAutoload$CbmMvdReadout.h")))  CbmMvdReadout;
class __attribute__((annotate("$clingAutoload$CbmMvdSensorReadoutTask.h")))  CbmMvdSensorReadoutTask;
class __attribute__((annotate("$clingAutoload$CbmMvdQa.h")))  CbmMvdQa;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmMvdAna dictionary payload"

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
#include "CbmMvdReadoutSimple.h"
#include "CbmMvdReadoutCluster.h"
#include "CbmMvdReadout.h"
#include "CbmMvdSensorReadoutTask.h"
#include "CbmMvdQa.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmMvdQa", payloadCode, "@",
"CbmMvdReadout", payloadCode, "@",
"CbmMvdReadoutCluster", payloadCode, "@",
"CbmMvdReadoutSimple", payloadCode, "@",
"CbmMvdSensorReadoutTask", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmMvdAna",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmMvdAna_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmMvdAna_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmMvdAna() {
  TriggerDictionaryInitialization_libCbmMvdAna_Impl();
}
