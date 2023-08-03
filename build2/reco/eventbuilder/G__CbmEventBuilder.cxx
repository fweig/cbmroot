// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmEventBuilder
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
#include "CbmAlgoBuildRawEvents.h"
#include "CbmTaskBuildRawEvents.h"
#include "CbmBuildEventsIdeal.h"
#include "CbmBuildEventsIdealNew.h"
#include "CbmBuildEventsQa.h"
#include "CbmBuildEventsSimple.h"
#include "CbmSeedFinderSlidingWindow.h"
#include "CbmSeedFinderQa.h"
#include "CbmBuildEventsFromTracksIdeal.h"
#include "CbmBuildEventsFromTracksReal.h"
#include "CbmEventBuilderQa.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmAlgoBuildRawEvents(void *p = nullptr);
   static void *newArray_CbmAlgoBuildRawEvents(Long_t size, void *p);
   static void delete_CbmAlgoBuildRawEvents(void *p);
   static void deleteArray_CbmAlgoBuildRawEvents(void *p);
   static void destruct_CbmAlgoBuildRawEvents(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAlgoBuildRawEvents*)
   {
      ::CbmAlgoBuildRawEvents *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAlgoBuildRawEvents >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAlgoBuildRawEvents", ::CbmAlgoBuildRawEvents::Class_Version(), "CbmAlgoBuildRawEvents.h", 120,
                  typeid(::CbmAlgoBuildRawEvents), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAlgoBuildRawEvents::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAlgoBuildRawEvents) );
      instance.SetNew(&new_CbmAlgoBuildRawEvents);
      instance.SetNewArray(&newArray_CbmAlgoBuildRawEvents);
      instance.SetDelete(&delete_CbmAlgoBuildRawEvents);
      instance.SetDeleteArray(&deleteArray_CbmAlgoBuildRawEvents);
      instance.SetDestructor(&destruct_CbmAlgoBuildRawEvents);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAlgoBuildRawEvents*)
   {
      return GenerateInitInstanceLocal((::CbmAlgoBuildRawEvents*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAlgoBuildRawEvents*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTaskBuildRawEvents(void *p = nullptr);
   static void *newArray_CbmTaskBuildRawEvents(Long_t size, void *p);
   static void delete_CbmTaskBuildRawEvents(void *p);
   static void deleteArray_CbmTaskBuildRawEvents(void *p);
   static void destruct_CbmTaskBuildRawEvents(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTaskBuildRawEvents*)
   {
      ::CbmTaskBuildRawEvents *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTaskBuildRawEvents >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTaskBuildRawEvents", ::CbmTaskBuildRawEvents::Class_Version(), "CbmTaskBuildRawEvents.h", 42,
                  typeid(::CbmTaskBuildRawEvents), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTaskBuildRawEvents::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTaskBuildRawEvents) );
      instance.SetNew(&new_CbmTaskBuildRawEvents);
      instance.SetNewArray(&newArray_CbmTaskBuildRawEvents);
      instance.SetDelete(&delete_CbmTaskBuildRawEvents);
      instance.SetDeleteArray(&deleteArray_CbmTaskBuildRawEvents);
      instance.SetDestructor(&destruct_CbmTaskBuildRawEvents);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTaskBuildRawEvents*)
   {
      return GenerateInitInstanceLocal((::CbmTaskBuildRawEvents*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTaskBuildRawEvents*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmBuildEventsIdeal(void *p = nullptr);
   static void *newArray_CbmBuildEventsIdeal(Long_t size, void *p);
   static void delete_CbmBuildEventsIdeal(void *p);
   static void deleteArray_CbmBuildEventsIdeal(void *p);
   static void destruct_CbmBuildEventsIdeal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBuildEventsIdeal*)
   {
      ::CbmBuildEventsIdeal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBuildEventsIdeal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBuildEventsIdeal", ::CbmBuildEventsIdeal::Class_Version(), "CbmBuildEventsIdeal.h", 47,
                  typeid(::CbmBuildEventsIdeal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBuildEventsIdeal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBuildEventsIdeal) );
      instance.SetNew(&new_CbmBuildEventsIdeal);
      instance.SetNewArray(&newArray_CbmBuildEventsIdeal);
      instance.SetDelete(&delete_CbmBuildEventsIdeal);
      instance.SetDeleteArray(&deleteArray_CbmBuildEventsIdeal);
      instance.SetDestructor(&destruct_CbmBuildEventsIdeal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBuildEventsIdeal*)
   {
      return GenerateInitInstanceLocal((::CbmBuildEventsIdeal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBuildEventsIdeal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmBuildEventsIdealNew(void *p = nullptr);
   static void *newArray_CbmBuildEventsIdealNew(Long_t size, void *p);
   static void delete_CbmBuildEventsIdealNew(void *p);
   static void deleteArray_CbmBuildEventsIdealNew(void *p);
   static void destruct_CbmBuildEventsIdealNew(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBuildEventsIdealNew*)
   {
      ::CbmBuildEventsIdealNew *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBuildEventsIdealNew >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBuildEventsIdealNew", ::CbmBuildEventsIdealNew::Class_Version(), "CbmBuildEventsIdealNew.h", 37,
                  typeid(::CbmBuildEventsIdealNew), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBuildEventsIdealNew::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBuildEventsIdealNew) );
      instance.SetNew(&new_CbmBuildEventsIdealNew);
      instance.SetNewArray(&newArray_CbmBuildEventsIdealNew);
      instance.SetDelete(&delete_CbmBuildEventsIdealNew);
      instance.SetDeleteArray(&deleteArray_CbmBuildEventsIdealNew);
      instance.SetDestructor(&destruct_CbmBuildEventsIdealNew);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBuildEventsIdealNew*)
   {
      return GenerateInitInstanceLocal((::CbmBuildEventsIdealNew*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBuildEventsIdealNew*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmBuildEventsQa(void *p = nullptr);
   static void *newArray_CbmBuildEventsQa(Long_t size, void *p);
   static void delete_CbmBuildEventsQa(void *p);
   static void deleteArray_CbmBuildEventsQa(void *p);
   static void destruct_CbmBuildEventsQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBuildEventsQa*)
   {
      ::CbmBuildEventsQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBuildEventsQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBuildEventsQa", ::CbmBuildEventsQa::Class_Version(), "CbmBuildEventsQa.h", 35,
                  typeid(::CbmBuildEventsQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBuildEventsQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBuildEventsQa) );
      instance.SetNew(&new_CbmBuildEventsQa);
      instance.SetNewArray(&newArray_CbmBuildEventsQa);
      instance.SetDelete(&delete_CbmBuildEventsQa);
      instance.SetDeleteArray(&deleteArray_CbmBuildEventsQa);
      instance.SetDestructor(&destruct_CbmBuildEventsQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBuildEventsQa*)
   {
      return GenerateInitInstanceLocal((::CbmBuildEventsQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBuildEventsQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmBuildEventsSimple(void *p = nullptr);
   static void *newArray_CbmBuildEventsSimple(Long_t size, void *p);
   static void delete_CbmBuildEventsSimple(void *p);
   static void deleteArray_CbmBuildEventsSimple(void *p);
   static void destruct_CbmBuildEventsSimple(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBuildEventsSimple*)
   {
      ::CbmBuildEventsSimple *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBuildEventsSimple >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBuildEventsSimple", ::CbmBuildEventsSimple::Class_Version(), "CbmBuildEventsSimple.h", 27,
                  typeid(::CbmBuildEventsSimple), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBuildEventsSimple::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBuildEventsSimple) );
      instance.SetNew(&new_CbmBuildEventsSimple);
      instance.SetNewArray(&newArray_CbmBuildEventsSimple);
      instance.SetDelete(&delete_CbmBuildEventsSimple);
      instance.SetDeleteArray(&deleteArray_CbmBuildEventsSimple);
      instance.SetDestructor(&destruct_CbmBuildEventsSimple);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBuildEventsSimple*)
   {
      return GenerateInitInstanceLocal((::CbmBuildEventsSimple*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBuildEventsSimple*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmSeedFinderSlidingWindow_Dictionary();
   static void CbmSeedFinderSlidingWindow_TClassManip(TClass*);
   static void delete_CbmSeedFinderSlidingWindow(void *p);
   static void deleteArray_CbmSeedFinderSlidingWindow(void *p);
   static void destruct_CbmSeedFinderSlidingWindow(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmSeedFinderSlidingWindow*)
   {
      ::CbmSeedFinderSlidingWindow *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmSeedFinderSlidingWindow));
      static ::ROOT::TGenericClassInfo 
         instance("CbmSeedFinderSlidingWindow", "CbmSeedFinderSlidingWindow.h", 32,
                  typeid(::CbmSeedFinderSlidingWindow), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmSeedFinderSlidingWindow_Dictionary, isa_proxy, 4,
                  sizeof(::CbmSeedFinderSlidingWindow) );
      instance.SetDelete(&delete_CbmSeedFinderSlidingWindow);
      instance.SetDeleteArray(&deleteArray_CbmSeedFinderSlidingWindow);
      instance.SetDestructor(&destruct_CbmSeedFinderSlidingWindow);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmSeedFinderSlidingWindow*)
   {
      return GenerateInitInstanceLocal((::CbmSeedFinderSlidingWindow*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmSeedFinderSlidingWindow*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmSeedFinderSlidingWindow_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmSeedFinderSlidingWindow*)nullptr)->GetClass();
      CbmSeedFinderSlidingWindow_TClassManip(theClass);
   return theClass;
   }

   static void CbmSeedFinderSlidingWindow_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmBuildEventsFromTracksIdeal(void *p = nullptr);
   static void *newArray_CbmBuildEventsFromTracksIdeal(Long_t size, void *p);
   static void delete_CbmBuildEventsFromTracksIdeal(void *p);
   static void deleteArray_CbmBuildEventsFromTracksIdeal(void *p);
   static void destruct_CbmBuildEventsFromTracksIdeal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBuildEventsFromTracksIdeal*)
   {
      ::CbmBuildEventsFromTracksIdeal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBuildEventsFromTracksIdeal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBuildEventsFromTracksIdeal", ::CbmBuildEventsFromTracksIdeal::Class_Version(), "CbmBuildEventsFromTracksIdeal.h", 29,
                  typeid(::CbmBuildEventsFromTracksIdeal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBuildEventsFromTracksIdeal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBuildEventsFromTracksIdeal) );
      instance.SetNew(&new_CbmBuildEventsFromTracksIdeal);
      instance.SetNewArray(&newArray_CbmBuildEventsFromTracksIdeal);
      instance.SetDelete(&delete_CbmBuildEventsFromTracksIdeal);
      instance.SetDeleteArray(&deleteArray_CbmBuildEventsFromTracksIdeal);
      instance.SetDestructor(&destruct_CbmBuildEventsFromTracksIdeal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBuildEventsFromTracksIdeal*)
   {
      return GenerateInitInstanceLocal((::CbmBuildEventsFromTracksIdeal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBuildEventsFromTracksIdeal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmBuildEventsFromTracksReal(void *p = nullptr);
   static void *newArray_CbmBuildEventsFromTracksReal(Long_t size, void *p);
   static void delete_CbmBuildEventsFromTracksReal(void *p);
   static void deleteArray_CbmBuildEventsFromTracksReal(void *p);
   static void destruct_CbmBuildEventsFromTracksReal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBuildEventsFromTracksReal*)
   {
      ::CbmBuildEventsFromTracksReal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBuildEventsFromTracksReal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBuildEventsFromTracksReal", ::CbmBuildEventsFromTracksReal::Class_Version(), "CbmBuildEventsFromTracksReal.h", 19,
                  typeid(::CbmBuildEventsFromTracksReal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBuildEventsFromTracksReal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBuildEventsFromTracksReal) );
      instance.SetNew(&new_CbmBuildEventsFromTracksReal);
      instance.SetNewArray(&newArray_CbmBuildEventsFromTracksReal);
      instance.SetDelete(&delete_CbmBuildEventsFromTracksReal);
      instance.SetDeleteArray(&deleteArray_CbmBuildEventsFromTracksReal);
      instance.SetDestructor(&destruct_CbmBuildEventsFromTracksReal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBuildEventsFromTracksReal*)
   {
      return GenerateInitInstanceLocal((::CbmBuildEventsFromTracksReal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBuildEventsFromTracksReal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmEventBuilderQa(void *p = nullptr);
   static void *newArray_CbmEventBuilderQa(Long_t size, void *p);
   static void delete_CbmEventBuilderQa(void *p);
   static void deleteArray_CbmEventBuilderQa(void *p);
   static void destruct_CbmEventBuilderQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmEventBuilderQa*)
   {
      ::CbmEventBuilderQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmEventBuilderQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmEventBuilderQa", ::CbmEventBuilderQa::Class_Version(), "CbmEventBuilderQa.h", 34,
                  typeid(::CbmEventBuilderQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmEventBuilderQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmEventBuilderQa) );
      instance.SetNew(&new_CbmEventBuilderQa);
      instance.SetNewArray(&newArray_CbmEventBuilderQa);
      instance.SetDelete(&delete_CbmEventBuilderQa);
      instance.SetDeleteArray(&deleteArray_CbmEventBuilderQa);
      instance.SetDestructor(&destruct_CbmEventBuilderQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmEventBuilderQa*)
   {
      return GenerateInitInstanceLocal((::CbmEventBuilderQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmEventBuilderQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmAlgoBuildRawEvents::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAlgoBuildRawEvents::Class_Name()
{
   return "CbmAlgoBuildRawEvents";
}

//______________________________________________________________________________
const char *CbmAlgoBuildRawEvents::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAlgoBuildRawEvents*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAlgoBuildRawEvents::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAlgoBuildRawEvents*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAlgoBuildRawEvents::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAlgoBuildRawEvents*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAlgoBuildRawEvents::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAlgoBuildRawEvents*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTaskBuildRawEvents::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTaskBuildRawEvents::Class_Name()
{
   return "CbmTaskBuildRawEvents";
}

//______________________________________________________________________________
const char *CbmTaskBuildRawEvents::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskBuildRawEvents*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTaskBuildRawEvents::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskBuildRawEvents*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTaskBuildRawEvents::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskBuildRawEvents*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTaskBuildRawEvents::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskBuildRawEvents*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmBuildEventsIdeal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBuildEventsIdeal::Class_Name()
{
   return "CbmBuildEventsIdeal";
}

//______________________________________________________________________________
const char *CbmBuildEventsIdeal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsIdeal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBuildEventsIdeal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsIdeal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBuildEventsIdeal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsIdeal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBuildEventsIdeal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsIdeal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmBuildEventsIdealNew::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBuildEventsIdealNew::Class_Name()
{
   return "CbmBuildEventsIdealNew";
}

//______________________________________________________________________________
const char *CbmBuildEventsIdealNew::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsIdealNew*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBuildEventsIdealNew::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsIdealNew*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBuildEventsIdealNew::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsIdealNew*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBuildEventsIdealNew::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsIdealNew*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmBuildEventsQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBuildEventsQa::Class_Name()
{
   return "CbmBuildEventsQa";
}

//______________________________________________________________________________
const char *CbmBuildEventsQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBuildEventsQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBuildEventsQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBuildEventsQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmBuildEventsSimple::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBuildEventsSimple::Class_Name()
{
   return "CbmBuildEventsSimple";
}

//______________________________________________________________________________
const char *CbmBuildEventsSimple::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsSimple*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBuildEventsSimple::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsSimple*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBuildEventsSimple::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsSimple*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBuildEventsSimple::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsSimple*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmBuildEventsFromTracksIdeal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBuildEventsFromTracksIdeal::Class_Name()
{
   return "CbmBuildEventsFromTracksIdeal";
}

//______________________________________________________________________________
const char *CbmBuildEventsFromTracksIdeal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsFromTracksIdeal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBuildEventsFromTracksIdeal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsFromTracksIdeal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBuildEventsFromTracksIdeal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsFromTracksIdeal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBuildEventsFromTracksIdeal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsFromTracksIdeal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmBuildEventsFromTracksReal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBuildEventsFromTracksReal::Class_Name()
{
   return "CbmBuildEventsFromTracksReal";
}

//______________________________________________________________________________
const char *CbmBuildEventsFromTracksReal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsFromTracksReal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBuildEventsFromTracksReal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsFromTracksReal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBuildEventsFromTracksReal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsFromTracksReal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBuildEventsFromTracksReal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBuildEventsFromTracksReal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmEventBuilderQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmEventBuilderQa::Class_Name()
{
   return "CbmEventBuilderQa";
}

//______________________________________________________________________________
const char *CbmEventBuilderQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmEventBuilderQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmEventBuilderQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmEventBuilderQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmEventBuilderQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmEventBuilderQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmEventBuilderQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmEventBuilderQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmAlgoBuildRawEvents::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAlgoBuildRawEvents.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAlgoBuildRawEvents::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAlgoBuildRawEvents::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAlgoBuildRawEvents(void *p) {
      return  p ? new(p) ::CbmAlgoBuildRawEvents : new ::CbmAlgoBuildRawEvents;
   }
   static void *newArray_CbmAlgoBuildRawEvents(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAlgoBuildRawEvents[nElements] : new ::CbmAlgoBuildRawEvents[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAlgoBuildRawEvents(void *p) {
      delete ((::CbmAlgoBuildRawEvents*)p);
   }
   static void deleteArray_CbmAlgoBuildRawEvents(void *p) {
      delete [] ((::CbmAlgoBuildRawEvents*)p);
   }
   static void destruct_CbmAlgoBuildRawEvents(void *p) {
      typedef ::CbmAlgoBuildRawEvents current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAlgoBuildRawEvents

//______________________________________________________________________________
void CbmTaskBuildRawEvents::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTaskBuildRawEvents.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTaskBuildRawEvents::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTaskBuildRawEvents::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTaskBuildRawEvents(void *p) {
      return  p ? new(p) ::CbmTaskBuildRawEvents : new ::CbmTaskBuildRawEvents;
   }
   static void *newArray_CbmTaskBuildRawEvents(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTaskBuildRawEvents[nElements] : new ::CbmTaskBuildRawEvents[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTaskBuildRawEvents(void *p) {
      delete ((::CbmTaskBuildRawEvents*)p);
   }
   static void deleteArray_CbmTaskBuildRawEvents(void *p) {
      delete [] ((::CbmTaskBuildRawEvents*)p);
   }
   static void destruct_CbmTaskBuildRawEvents(void *p) {
      typedef ::CbmTaskBuildRawEvents current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTaskBuildRawEvents

//______________________________________________________________________________
void CbmBuildEventsIdeal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBuildEventsIdeal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBuildEventsIdeal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBuildEventsIdeal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBuildEventsIdeal(void *p) {
      return  p ? new(p) ::CbmBuildEventsIdeal : new ::CbmBuildEventsIdeal;
   }
   static void *newArray_CbmBuildEventsIdeal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBuildEventsIdeal[nElements] : new ::CbmBuildEventsIdeal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBuildEventsIdeal(void *p) {
      delete ((::CbmBuildEventsIdeal*)p);
   }
   static void deleteArray_CbmBuildEventsIdeal(void *p) {
      delete [] ((::CbmBuildEventsIdeal*)p);
   }
   static void destruct_CbmBuildEventsIdeal(void *p) {
      typedef ::CbmBuildEventsIdeal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBuildEventsIdeal

//______________________________________________________________________________
void CbmBuildEventsIdealNew::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBuildEventsIdealNew.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBuildEventsIdealNew::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBuildEventsIdealNew::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBuildEventsIdealNew(void *p) {
      return  p ? new(p) ::CbmBuildEventsIdealNew : new ::CbmBuildEventsIdealNew;
   }
   static void *newArray_CbmBuildEventsIdealNew(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBuildEventsIdealNew[nElements] : new ::CbmBuildEventsIdealNew[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBuildEventsIdealNew(void *p) {
      delete ((::CbmBuildEventsIdealNew*)p);
   }
   static void deleteArray_CbmBuildEventsIdealNew(void *p) {
      delete [] ((::CbmBuildEventsIdealNew*)p);
   }
   static void destruct_CbmBuildEventsIdealNew(void *p) {
      typedef ::CbmBuildEventsIdealNew current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBuildEventsIdealNew

//______________________________________________________________________________
void CbmBuildEventsQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBuildEventsQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBuildEventsQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBuildEventsQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBuildEventsQa(void *p) {
      return  p ? new(p) ::CbmBuildEventsQa : new ::CbmBuildEventsQa;
   }
   static void *newArray_CbmBuildEventsQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBuildEventsQa[nElements] : new ::CbmBuildEventsQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBuildEventsQa(void *p) {
      delete ((::CbmBuildEventsQa*)p);
   }
   static void deleteArray_CbmBuildEventsQa(void *p) {
      delete [] ((::CbmBuildEventsQa*)p);
   }
   static void destruct_CbmBuildEventsQa(void *p) {
      typedef ::CbmBuildEventsQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBuildEventsQa

//______________________________________________________________________________
void CbmBuildEventsSimple::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBuildEventsSimple.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBuildEventsSimple::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBuildEventsSimple::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBuildEventsSimple(void *p) {
      return  p ? new(p) ::CbmBuildEventsSimple : new ::CbmBuildEventsSimple;
   }
   static void *newArray_CbmBuildEventsSimple(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBuildEventsSimple[nElements] : new ::CbmBuildEventsSimple[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBuildEventsSimple(void *p) {
      delete ((::CbmBuildEventsSimple*)p);
   }
   static void deleteArray_CbmBuildEventsSimple(void *p) {
      delete [] ((::CbmBuildEventsSimple*)p);
   }
   static void destruct_CbmBuildEventsSimple(void *p) {
      typedef ::CbmBuildEventsSimple current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBuildEventsSimple

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmSeedFinderSlidingWindow(void *p) {
      delete ((::CbmSeedFinderSlidingWindow*)p);
   }
   static void deleteArray_CbmSeedFinderSlidingWindow(void *p) {
      delete [] ((::CbmSeedFinderSlidingWindow*)p);
   }
   static void destruct_CbmSeedFinderSlidingWindow(void *p) {
      typedef ::CbmSeedFinderSlidingWindow current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmSeedFinderSlidingWindow

//______________________________________________________________________________
void CbmBuildEventsFromTracksIdeal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBuildEventsFromTracksIdeal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBuildEventsFromTracksIdeal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBuildEventsFromTracksIdeal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBuildEventsFromTracksIdeal(void *p) {
      return  p ? new(p) ::CbmBuildEventsFromTracksIdeal : new ::CbmBuildEventsFromTracksIdeal;
   }
   static void *newArray_CbmBuildEventsFromTracksIdeal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBuildEventsFromTracksIdeal[nElements] : new ::CbmBuildEventsFromTracksIdeal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBuildEventsFromTracksIdeal(void *p) {
      delete ((::CbmBuildEventsFromTracksIdeal*)p);
   }
   static void deleteArray_CbmBuildEventsFromTracksIdeal(void *p) {
      delete [] ((::CbmBuildEventsFromTracksIdeal*)p);
   }
   static void destruct_CbmBuildEventsFromTracksIdeal(void *p) {
      typedef ::CbmBuildEventsFromTracksIdeal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBuildEventsFromTracksIdeal

//______________________________________________________________________________
void CbmBuildEventsFromTracksReal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBuildEventsFromTracksReal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBuildEventsFromTracksReal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBuildEventsFromTracksReal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBuildEventsFromTracksReal(void *p) {
      return  p ? new(p) ::CbmBuildEventsFromTracksReal : new ::CbmBuildEventsFromTracksReal;
   }
   static void *newArray_CbmBuildEventsFromTracksReal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBuildEventsFromTracksReal[nElements] : new ::CbmBuildEventsFromTracksReal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBuildEventsFromTracksReal(void *p) {
      delete ((::CbmBuildEventsFromTracksReal*)p);
   }
   static void deleteArray_CbmBuildEventsFromTracksReal(void *p) {
      delete [] ((::CbmBuildEventsFromTracksReal*)p);
   }
   static void destruct_CbmBuildEventsFromTracksReal(void *p) {
      typedef ::CbmBuildEventsFromTracksReal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBuildEventsFromTracksReal

//______________________________________________________________________________
void CbmEventBuilderQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmEventBuilderQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmEventBuilderQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmEventBuilderQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmEventBuilderQa(void *p) {
      return  p ? new(p) ::CbmEventBuilderQa : new ::CbmEventBuilderQa;
   }
   static void *newArray_CbmEventBuilderQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmEventBuilderQa[nElements] : new ::CbmEventBuilderQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmEventBuilderQa(void *p) {
      delete ((::CbmEventBuilderQa*)p);
   }
   static void deleteArray_CbmEventBuilderQa(void *p) {
      delete [] ((::CbmEventBuilderQa*)p);
   }
   static void destruct_CbmEventBuilderQa(void *p) {
      typedef ::CbmEventBuilderQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmEventBuilderQa

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
   static TClass *vectorlERawEventBuilderDetectorgR_Dictionary();
   static void vectorlERawEventBuilderDetectorgR_TClassManip(TClass*);
   static void *new_vectorlERawEventBuilderDetectorgR(void *p = nullptr);
   static void *newArray_vectorlERawEventBuilderDetectorgR(Long_t size, void *p);
   static void delete_vectorlERawEventBuilderDetectorgR(void *p);
   static void deleteArray_vectorlERawEventBuilderDetectorgR(void *p);
   static void destruct_vectorlERawEventBuilderDetectorgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<RawEventBuilderDetector>*)
   {
      vector<RawEventBuilderDetector> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<RawEventBuilderDetector>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<RawEventBuilderDetector>", -2, "vector", 389,
                  typeid(vector<RawEventBuilderDetector>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlERawEventBuilderDetectorgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<RawEventBuilderDetector>) );
      instance.SetNew(&new_vectorlERawEventBuilderDetectorgR);
      instance.SetNewArray(&newArray_vectorlERawEventBuilderDetectorgR);
      instance.SetDelete(&delete_vectorlERawEventBuilderDetectorgR);
      instance.SetDeleteArray(&deleteArray_vectorlERawEventBuilderDetectorgR);
      instance.SetDestructor(&destruct_vectorlERawEventBuilderDetectorgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<RawEventBuilderDetector> >()));

      ::ROOT::AddClassAlternate("vector<RawEventBuilderDetector>","std::vector<RawEventBuilderDetector, std::allocator<RawEventBuilderDetector> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<RawEventBuilderDetector>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlERawEventBuilderDetectorgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<RawEventBuilderDetector>*)nullptr)->GetClass();
      vectorlERawEventBuilderDetectorgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlERawEventBuilderDetectorgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlERawEventBuilderDetectorgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<RawEventBuilderDetector> : new vector<RawEventBuilderDetector>;
   }
   static void *newArray_vectorlERawEventBuilderDetectorgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<RawEventBuilderDetector>[nElements] : new vector<RawEventBuilderDetector>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlERawEventBuilderDetectorgR(void *p) {
      delete ((vector<RawEventBuilderDetector>*)p);
   }
   static void deleteArray_vectorlERawEventBuilderDetectorgR(void *p) {
      delete [] ((vector<RawEventBuilderDetector>*)p);
   }
   static void destruct_vectorlERawEventBuilderDetectorgR(void *p) {
      typedef vector<RawEventBuilderDetector> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<RawEventBuilderDetector>

namespace ROOT {
   static TClass *vectorlEECbmModuleIdgR_Dictionary();
   static void vectorlEECbmModuleIdgR_TClassManip(TClass*);
   static void *new_vectorlEECbmModuleIdgR(void *p = nullptr);
   static void *newArray_vectorlEECbmModuleIdgR(Long_t size, void *p);
   static void delete_vectorlEECbmModuleIdgR(void *p);
   static void deleteArray_vectorlEECbmModuleIdgR(void *p);
   static void destruct_vectorlEECbmModuleIdgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ECbmModuleId>*)
   {
      vector<ECbmModuleId> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ECbmModuleId>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ECbmModuleId>", -2, "vector", 389,
                  typeid(vector<ECbmModuleId>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEECbmModuleIdgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ECbmModuleId>) );
      instance.SetNew(&new_vectorlEECbmModuleIdgR);
      instance.SetNewArray(&newArray_vectorlEECbmModuleIdgR);
      instance.SetDelete(&delete_vectorlEECbmModuleIdgR);
      instance.SetDeleteArray(&deleteArray_vectorlEECbmModuleIdgR);
      instance.SetDestructor(&destruct_vectorlEECbmModuleIdgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ECbmModuleId> >()));

      ::ROOT::AddClassAlternate("vector<ECbmModuleId>","std::vector<ECbmModuleId, std::allocator<ECbmModuleId> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<ECbmModuleId>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEECbmModuleIdgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<ECbmModuleId>*)nullptr)->GetClass();
      vectorlEECbmModuleIdgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEECbmModuleIdgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEECbmModuleIdgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ECbmModuleId> : new vector<ECbmModuleId>;
   }
   static void *newArray_vectorlEECbmModuleIdgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ECbmModuleId>[nElements] : new vector<ECbmModuleId>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEECbmModuleIdgR(void *p) {
      delete ((vector<ECbmModuleId>*)p);
   }
   static void deleteArray_vectorlEECbmModuleIdgR(void *p) {
      delete [] ((vector<ECbmModuleId>*)p);
   }
   static void destruct_vectorlEECbmModuleIdgR(void *p) {
      typedef vector<ECbmModuleId> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<ECbmModuleId>

namespace ROOT {
   static TClass *vectorlECbmTrdDigigR_Dictionary();
   static void vectorlECbmTrdDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmTrdDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmTrdDigigR(Long_t size, void *p);
   static void delete_vectorlECbmTrdDigigR(void *p);
   static void deleteArray_vectorlECbmTrdDigigR(void *p);
   static void destruct_vectorlECbmTrdDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmTrdDigi>*)
   {
      vector<CbmTrdDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmTrdDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmTrdDigi>", -2, "vector", 389,
                  typeid(vector<CbmTrdDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmTrdDigigR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmTrdDigi>) );
      instance.SetNew(&new_vectorlECbmTrdDigigR);
      instance.SetNewArray(&newArray_vectorlECbmTrdDigigR);
      instance.SetDelete(&delete_vectorlECbmTrdDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmTrdDigigR);
      instance.SetDestructor(&destruct_vectorlECbmTrdDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmTrdDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmTrdDigi>","std::vector<CbmTrdDigi, std::allocator<CbmTrdDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmTrdDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmTrdDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmTrdDigi>*)nullptr)->GetClass();
      vectorlECbmTrdDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmTrdDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmTrdDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTrdDigi> : new vector<CbmTrdDigi>;
   }
   static void *newArray_vectorlECbmTrdDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTrdDigi>[nElements] : new vector<CbmTrdDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmTrdDigigR(void *p) {
      delete ((vector<CbmTrdDigi>*)p);
   }
   static void deleteArray_vectorlECbmTrdDigigR(void *p) {
      delete [] ((vector<CbmTrdDigi>*)p);
   }
   static void destruct_vectorlECbmTrdDigigR(void *p) {
      typedef vector<CbmTrdDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmTrdDigi>

namespace ROOT {
   static TClass *vectorlECbmTofDigigR_Dictionary();
   static void vectorlECbmTofDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmTofDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmTofDigigR(Long_t size, void *p);
   static void delete_vectorlECbmTofDigigR(void *p);
   static void deleteArray_vectorlECbmTofDigigR(void *p);
   static void destruct_vectorlECbmTofDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmTofDigi>*)
   {
      vector<CbmTofDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmTofDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmTofDigi>", -2, "vector", 389,
                  typeid(vector<CbmTofDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmTofDigigR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmTofDigi>) );
      instance.SetNew(&new_vectorlECbmTofDigigR);
      instance.SetNewArray(&newArray_vectorlECbmTofDigigR);
      instance.SetDelete(&delete_vectorlECbmTofDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmTofDigigR);
      instance.SetDestructor(&destruct_vectorlECbmTofDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmTofDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmTofDigi>","std::vector<CbmTofDigi, std::allocator<CbmTofDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmTofDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmTofDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmTofDigi>*)nullptr)->GetClass();
      vectorlECbmTofDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmTofDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmTofDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTofDigi> : new vector<CbmTofDigi>;
   }
   static void *newArray_vectorlECbmTofDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTofDigi>[nElements] : new vector<CbmTofDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmTofDigigR(void *p) {
      delete ((vector<CbmTofDigi>*)p);
   }
   static void deleteArray_vectorlECbmTofDigigR(void *p) {
      delete [] ((vector<CbmTofDigi>*)p);
   }
   static void destruct_vectorlECbmTofDigigR(void *p) {
      typedef vector<CbmTofDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmTofDigi>

namespace ROOT {
   static TClass *vectorlECbmStsDigigR_Dictionary();
   static void vectorlECbmStsDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmStsDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmStsDigigR(Long_t size, void *p);
   static void delete_vectorlECbmStsDigigR(void *p);
   static void deleteArray_vectorlECbmStsDigigR(void *p);
   static void destruct_vectorlECbmStsDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsDigi>*)
   {
      vector<CbmStsDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsDigi>", -2, "vector", 389,
                  typeid(vector<CbmStsDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsDigigR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmStsDigi>) );
      instance.SetNew(&new_vectorlECbmStsDigigR);
      instance.SetNewArray(&newArray_vectorlECbmStsDigigR);
      instance.SetDelete(&delete_vectorlECbmStsDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsDigigR);
      instance.SetDestructor(&destruct_vectorlECbmStsDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsDigi>","std::vector<CbmStsDigi, std::allocator<CbmStsDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsDigi>*)nullptr)->GetClass();
      vectorlECbmStsDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsDigi> : new vector<CbmStsDigi>;
   }
   static void *newArray_vectorlECbmStsDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsDigi>[nElements] : new vector<CbmStsDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsDigigR(void *p) {
      delete ((vector<CbmStsDigi>*)p);
   }
   static void deleteArray_vectorlECbmStsDigigR(void *p) {
      delete [] ((vector<CbmStsDigi>*)p);
   }
   static void destruct_vectorlECbmStsDigigR(void *p) {
      typedef vector<CbmStsDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsDigi>

namespace ROOT {
   static TClass *vectorlECbmRichDigigR_Dictionary();
   static void vectorlECbmRichDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmRichDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmRichDigigR(Long_t size, void *p);
   static void delete_vectorlECbmRichDigigR(void *p);
   static void deleteArray_vectorlECbmRichDigigR(void *p);
   static void destruct_vectorlECbmRichDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmRichDigi>*)
   {
      vector<CbmRichDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmRichDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmRichDigi>", -2, "vector", 389,
                  typeid(vector<CbmRichDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmRichDigigR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmRichDigi>) );
      instance.SetNew(&new_vectorlECbmRichDigigR);
      instance.SetNewArray(&newArray_vectorlECbmRichDigigR);
      instance.SetDelete(&delete_vectorlECbmRichDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmRichDigigR);
      instance.SetDestructor(&destruct_vectorlECbmRichDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmRichDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmRichDigi>","std::vector<CbmRichDigi, std::allocator<CbmRichDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmRichDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmRichDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmRichDigi>*)nullptr)->GetClass();
      vectorlECbmRichDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmRichDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmRichDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmRichDigi> : new vector<CbmRichDigi>;
   }
   static void *newArray_vectorlECbmRichDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmRichDigi>[nElements] : new vector<CbmRichDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmRichDigigR(void *p) {
      delete ((vector<CbmRichDigi>*)p);
   }
   static void deleteArray_vectorlECbmRichDigigR(void *p) {
      delete [] ((vector<CbmRichDigi>*)p);
   }
   static void destruct_vectorlECbmRichDigigR(void *p) {
      typedef vector<CbmRichDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmRichDigi>

namespace ROOT {
   static TClass *vectorlECbmPsdDigigR_Dictionary();
   static void vectorlECbmPsdDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmPsdDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmPsdDigigR(Long_t size, void *p);
   static void delete_vectorlECbmPsdDigigR(void *p);
   static void deleteArray_vectorlECbmPsdDigigR(void *p);
   static void destruct_vectorlECbmPsdDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmPsdDigi>*)
   {
      vector<CbmPsdDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmPsdDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmPsdDigi>", -2, "vector", 389,
                  typeid(vector<CbmPsdDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmPsdDigigR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmPsdDigi>) );
      instance.SetNew(&new_vectorlECbmPsdDigigR);
      instance.SetNewArray(&newArray_vectorlECbmPsdDigigR);
      instance.SetDelete(&delete_vectorlECbmPsdDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmPsdDigigR);
      instance.SetDestructor(&destruct_vectorlECbmPsdDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmPsdDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmPsdDigi>","std::vector<CbmPsdDigi, std::allocator<CbmPsdDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmPsdDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmPsdDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmPsdDigi>*)nullptr)->GetClass();
      vectorlECbmPsdDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmPsdDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmPsdDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmPsdDigi> : new vector<CbmPsdDigi>;
   }
   static void *newArray_vectorlECbmPsdDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmPsdDigi>[nElements] : new vector<CbmPsdDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmPsdDigigR(void *p) {
      delete ((vector<CbmPsdDigi>*)p);
   }
   static void deleteArray_vectorlECbmPsdDigigR(void *p) {
      delete [] ((vector<CbmPsdDigi>*)p);
   }
   static void destruct_vectorlECbmPsdDigigR(void *p) {
      typedef vector<CbmPsdDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmPsdDigi>

namespace ROOT {
   static TClass *vectorlECbmMuchDigigR_Dictionary();
   static void vectorlECbmMuchDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmMuchDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmMuchDigigR(Long_t size, void *p);
   static void delete_vectorlECbmMuchDigigR(void *p);
   static void deleteArray_vectorlECbmMuchDigigR(void *p);
   static void destruct_vectorlECbmMuchDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMuchDigi>*)
   {
      vector<CbmMuchDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMuchDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMuchDigi>", -2, "vector", 389,
                  typeid(vector<CbmMuchDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMuchDigigR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmMuchDigi>) );
      instance.SetNew(&new_vectorlECbmMuchDigigR);
      instance.SetNewArray(&newArray_vectorlECbmMuchDigigR);
      instance.SetDelete(&delete_vectorlECbmMuchDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMuchDigigR);
      instance.SetDestructor(&destruct_vectorlECbmMuchDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMuchDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmMuchDigi>","std::vector<CbmMuchDigi, std::allocator<CbmMuchDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMuchDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMuchDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMuchDigi>*)nullptr)->GetClass();
      vectorlECbmMuchDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMuchDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMuchDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMuchDigi> : new vector<CbmMuchDigi>;
   }
   static void *newArray_vectorlECbmMuchDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMuchDigi>[nElements] : new vector<CbmMuchDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMuchDigigR(void *p) {
      delete ((vector<CbmMuchDigi>*)p);
   }
   static void deleteArray_vectorlECbmMuchDigigR(void *p) {
      delete [] ((vector<CbmMuchDigi>*)p);
   }
   static void destruct_vectorlECbmMuchDigigR(void *p) {
      typedef vector<CbmMuchDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMuchDigi>

namespace ROOT {
   static TClass *vectorlECbmMuchBeamTimeDigigR_Dictionary();
   static void vectorlECbmMuchBeamTimeDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmMuchBeamTimeDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmMuchBeamTimeDigigR(Long_t size, void *p);
   static void delete_vectorlECbmMuchBeamTimeDigigR(void *p);
   static void deleteArray_vectorlECbmMuchBeamTimeDigigR(void *p);
   static void destruct_vectorlECbmMuchBeamTimeDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMuchBeamTimeDigi>*)
   {
      vector<CbmMuchBeamTimeDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMuchBeamTimeDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMuchBeamTimeDigi>", -2, "vector", 389,
                  typeid(vector<CbmMuchBeamTimeDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMuchBeamTimeDigigR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmMuchBeamTimeDigi>) );
      instance.SetNew(&new_vectorlECbmMuchBeamTimeDigigR);
      instance.SetNewArray(&newArray_vectorlECbmMuchBeamTimeDigigR);
      instance.SetDelete(&delete_vectorlECbmMuchBeamTimeDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMuchBeamTimeDigigR);
      instance.SetDestructor(&destruct_vectorlECbmMuchBeamTimeDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMuchBeamTimeDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmMuchBeamTimeDigi>","std::vector<CbmMuchBeamTimeDigi, std::allocator<CbmMuchBeamTimeDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMuchBeamTimeDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMuchBeamTimeDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMuchBeamTimeDigi>*)nullptr)->GetClass();
      vectorlECbmMuchBeamTimeDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMuchBeamTimeDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMuchBeamTimeDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMuchBeamTimeDigi> : new vector<CbmMuchBeamTimeDigi>;
   }
   static void *newArray_vectorlECbmMuchBeamTimeDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMuchBeamTimeDigi>[nElements] : new vector<CbmMuchBeamTimeDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMuchBeamTimeDigigR(void *p) {
      delete ((vector<CbmMuchBeamTimeDigi>*)p);
   }
   static void deleteArray_vectorlECbmMuchBeamTimeDigigR(void *p) {
      delete [] ((vector<CbmMuchBeamTimeDigi>*)p);
   }
   static void destruct_vectorlECbmMuchBeamTimeDigigR(void *p) {
      typedef vector<CbmMuchBeamTimeDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMuchBeamTimeDigi>

namespace ROOT {
   static TClass *vectorlECbmMatchgR_Dictionary();
   static void vectorlECbmMatchgR_TClassManip(TClass*);
   static void *new_vectorlECbmMatchgR(void *p = nullptr);
   static void *newArray_vectorlECbmMatchgR(Long_t size, void *p);
   static void delete_vectorlECbmMatchgR(void *p);
   static void deleteArray_vectorlECbmMatchgR(void *p);
   static void destruct_vectorlECbmMatchgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMatch>*)
   {
      vector<CbmMatch> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMatch>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMatch>", -2, "vector", 389,
                  typeid(vector<CbmMatch>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMatchgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmMatch>) );
      instance.SetNew(&new_vectorlECbmMatchgR);
      instance.SetNewArray(&newArray_vectorlECbmMatchgR);
      instance.SetDelete(&delete_vectorlECbmMatchgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMatchgR);
      instance.SetDestructor(&destruct_vectorlECbmMatchgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMatch> >()));

      ::ROOT::AddClassAlternate("vector<CbmMatch>","std::vector<CbmMatch, std::allocator<CbmMatch> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMatch>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMatchgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMatch>*)nullptr)->GetClass();
      vectorlECbmMatchgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMatchgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMatchgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMatch> : new vector<CbmMatch>;
   }
   static void *newArray_vectorlECbmMatchgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMatch>[nElements] : new vector<CbmMatch>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMatchgR(void *p) {
      delete ((vector<CbmMatch>*)p);
   }
   static void deleteArray_vectorlECbmMatchgR(void *p) {
      delete [] ((vector<CbmMatch>*)p);
   }
   static void destruct_vectorlECbmMatchgR(void *p) {
      typedef vector<CbmMatch> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMatch>

namespace ROOT {
   static TClass *vectorlECbmDigiEventgR_Dictionary();
   static void vectorlECbmDigiEventgR_TClassManip(TClass*);
   static void *new_vectorlECbmDigiEventgR(void *p = nullptr);
   static void *newArray_vectorlECbmDigiEventgR(Long_t size, void *p);
   static void delete_vectorlECbmDigiEventgR(void *p);
   static void deleteArray_vectorlECbmDigiEventgR(void *p);
   static void destruct_vectorlECbmDigiEventgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmDigiEvent>*)
   {
      vector<CbmDigiEvent> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmDigiEvent>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmDigiEvent>", -2, "vector", 389,
                  typeid(vector<CbmDigiEvent>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmDigiEventgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmDigiEvent>) );
      instance.SetNew(&new_vectorlECbmDigiEventgR);
      instance.SetNewArray(&newArray_vectorlECbmDigiEventgR);
      instance.SetDelete(&delete_vectorlECbmDigiEventgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmDigiEventgR);
      instance.SetDestructor(&destruct_vectorlECbmDigiEventgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmDigiEvent> >()));

      ::ROOT::AddClassAlternate("vector<CbmDigiEvent>","std::vector<CbmDigiEvent, std::allocator<CbmDigiEvent> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmDigiEvent>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmDigiEventgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmDigiEvent>*)nullptr)->GetClass();
      vectorlECbmDigiEventgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmDigiEventgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmDigiEventgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmDigiEvent> : new vector<CbmDigiEvent>;
   }
   static void *newArray_vectorlECbmDigiEventgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmDigiEvent>[nElements] : new vector<CbmDigiEvent>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmDigiEventgR(void *p) {
      delete ((vector<CbmDigiEvent>*)p);
   }
   static void deleteArray_vectorlECbmDigiEventgR(void *p) {
      delete [] ((vector<CbmDigiEvent>*)p);
   }
   static void destruct_vectorlECbmDigiEventgR(void *p) {
      typedef vector<CbmDigiEvent> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmDigiEvent>

namespace ROOT {
   static TClass *vectorlECbmBmonDigigR_Dictionary();
   static void vectorlECbmBmonDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmBmonDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmBmonDigigR(Long_t size, void *p);
   static void delete_vectorlECbmBmonDigigR(void *p);
   static void deleteArray_vectorlECbmBmonDigigR(void *p);
   static void destruct_vectorlECbmBmonDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmBmonDigi>*)
   {
      vector<CbmBmonDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmBmonDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmBmonDigi>", -2, "vector", 389,
                  typeid(vector<CbmBmonDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmBmonDigigR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmBmonDigi>) );
      instance.SetNew(&new_vectorlECbmBmonDigigR);
      instance.SetNewArray(&newArray_vectorlECbmBmonDigigR);
      instance.SetDelete(&delete_vectorlECbmBmonDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmBmonDigigR);
      instance.SetDestructor(&destruct_vectorlECbmBmonDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmBmonDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmBmonDigi>","std::vector<CbmBmonDigi, std::allocator<CbmBmonDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmBmonDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmBmonDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmBmonDigi>*)nullptr)->GetClass();
      vectorlECbmBmonDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmBmonDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmBmonDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmBmonDigi> : new vector<CbmBmonDigi>;
   }
   static void *newArray_vectorlECbmBmonDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmBmonDigi>[nElements] : new vector<CbmBmonDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmBmonDigigR(void *p) {
      delete ((vector<CbmBmonDigi>*)p);
   }
   static void deleteArray_vectorlECbmBmonDigigR(void *p) {
      delete [] ((vector<CbmBmonDigi>*)p);
   }
   static void destruct_vectorlECbmBmonDigigR(void *p) {
      typedef vector<CbmBmonDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmBmonDigi>

namespace ROOT {
   static TClass *maplEECbmModuleIdcOTH2ImUgR_Dictionary();
   static void maplEECbmModuleIdcOTH2ImUgR_TClassManip(TClass*);
   static void *new_maplEECbmModuleIdcOTH2ImUgR(void *p = nullptr);
   static void *newArray_maplEECbmModuleIdcOTH2ImUgR(Long_t size, void *p);
   static void delete_maplEECbmModuleIdcOTH2ImUgR(void *p);
   static void deleteArray_maplEECbmModuleIdcOTH2ImUgR(void *p);
   static void destruct_maplEECbmModuleIdcOTH2ImUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<ECbmModuleId,TH2I*>*)
   {
      map<ECbmModuleId,TH2I*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<ECbmModuleId,TH2I*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<ECbmModuleId,TH2I*>", -2, "map", 100,
                  typeid(map<ECbmModuleId,TH2I*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEECbmModuleIdcOTH2ImUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<ECbmModuleId,TH2I*>) );
      instance.SetNew(&new_maplEECbmModuleIdcOTH2ImUgR);
      instance.SetNewArray(&newArray_maplEECbmModuleIdcOTH2ImUgR);
      instance.SetDelete(&delete_maplEECbmModuleIdcOTH2ImUgR);
      instance.SetDeleteArray(&deleteArray_maplEECbmModuleIdcOTH2ImUgR);
      instance.SetDestructor(&destruct_maplEECbmModuleIdcOTH2ImUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<ECbmModuleId,TH2I*> >()));

      ::ROOT::AddClassAlternate("map<ECbmModuleId,TH2I*>","std::map<ECbmModuleId, TH2I*, std::less<ECbmModuleId>, std::allocator<std::pair<ECbmModuleId const, TH2I*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<ECbmModuleId,TH2I*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEECbmModuleIdcOTH2ImUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<ECbmModuleId,TH2I*>*)nullptr)->GetClass();
      maplEECbmModuleIdcOTH2ImUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEECbmModuleIdcOTH2ImUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEECbmModuleIdcOTH2ImUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,TH2I*> : new map<ECbmModuleId,TH2I*>;
   }
   static void *newArray_maplEECbmModuleIdcOTH2ImUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,TH2I*>[nElements] : new map<ECbmModuleId,TH2I*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEECbmModuleIdcOTH2ImUgR(void *p) {
      delete ((map<ECbmModuleId,TH2I*>*)p);
   }
   static void deleteArray_maplEECbmModuleIdcOTH2ImUgR(void *p) {
      delete [] ((map<ECbmModuleId,TH2I*>*)p);
   }
   static void destruct_maplEECbmModuleIdcOTH2ImUgR(void *p) {
      typedef map<ECbmModuleId,TH2I*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<ECbmModuleId,TH2I*>

namespace ROOT {
   static TClass *maplEECbmModuleIdcOTH1FmUgR_Dictionary();
   static void maplEECbmModuleIdcOTH1FmUgR_TClassManip(TClass*);
   static void *new_maplEECbmModuleIdcOTH1FmUgR(void *p = nullptr);
   static void *newArray_maplEECbmModuleIdcOTH1FmUgR(Long_t size, void *p);
   static void delete_maplEECbmModuleIdcOTH1FmUgR(void *p);
   static void deleteArray_maplEECbmModuleIdcOTH1FmUgR(void *p);
   static void destruct_maplEECbmModuleIdcOTH1FmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<ECbmModuleId,TH1F*>*)
   {
      map<ECbmModuleId,TH1F*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<ECbmModuleId,TH1F*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<ECbmModuleId,TH1F*>", -2, "map", 100,
                  typeid(map<ECbmModuleId,TH1F*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEECbmModuleIdcOTH1FmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<ECbmModuleId,TH1F*>) );
      instance.SetNew(&new_maplEECbmModuleIdcOTH1FmUgR);
      instance.SetNewArray(&newArray_maplEECbmModuleIdcOTH1FmUgR);
      instance.SetDelete(&delete_maplEECbmModuleIdcOTH1FmUgR);
      instance.SetDeleteArray(&deleteArray_maplEECbmModuleIdcOTH1FmUgR);
      instance.SetDestructor(&destruct_maplEECbmModuleIdcOTH1FmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<ECbmModuleId,TH1F*> >()));

      ::ROOT::AddClassAlternate("map<ECbmModuleId,TH1F*>","std::map<ECbmModuleId, TH1F*, std::less<ECbmModuleId>, std::allocator<std::pair<ECbmModuleId const, TH1F*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<ECbmModuleId,TH1F*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEECbmModuleIdcOTH1FmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<ECbmModuleId,TH1F*>*)nullptr)->GetClass();
      maplEECbmModuleIdcOTH1FmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEECbmModuleIdcOTH1FmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEECbmModuleIdcOTH1FmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,TH1F*> : new map<ECbmModuleId,TH1F*>;
   }
   static void *newArray_maplEECbmModuleIdcOTH1FmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,TH1F*>[nElements] : new map<ECbmModuleId,TH1F*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEECbmModuleIdcOTH1FmUgR(void *p) {
      delete ((map<ECbmModuleId,TH1F*>*)p);
   }
   static void deleteArray_maplEECbmModuleIdcOTH1FmUgR(void *p) {
      delete [] ((map<ECbmModuleId,TH1F*>*)p);
   }
   static void destruct_maplEECbmModuleIdcOTH1FmUgR(void *p) {
      typedef map<ECbmModuleId,TH1F*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<ECbmModuleId,TH1F*>

namespace ROOT {
   static TClass *maplEECbmModuleIdcOCbmQaCanvasmUgR_Dictionary();
   static void maplEECbmModuleIdcOCbmQaCanvasmUgR_TClassManip(TClass*);
   static void *new_maplEECbmModuleIdcOCbmQaCanvasmUgR(void *p = nullptr);
   static void *newArray_maplEECbmModuleIdcOCbmQaCanvasmUgR(Long_t size, void *p);
   static void delete_maplEECbmModuleIdcOCbmQaCanvasmUgR(void *p);
   static void deleteArray_maplEECbmModuleIdcOCbmQaCanvasmUgR(void *p);
   static void destruct_maplEECbmModuleIdcOCbmQaCanvasmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<ECbmModuleId,CbmQaCanvas*>*)
   {
      map<ECbmModuleId,CbmQaCanvas*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<ECbmModuleId,CbmQaCanvas*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<ECbmModuleId,CbmQaCanvas*>", -2, "map", 100,
                  typeid(map<ECbmModuleId,CbmQaCanvas*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEECbmModuleIdcOCbmQaCanvasmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<ECbmModuleId,CbmQaCanvas*>) );
      instance.SetNew(&new_maplEECbmModuleIdcOCbmQaCanvasmUgR);
      instance.SetNewArray(&newArray_maplEECbmModuleIdcOCbmQaCanvasmUgR);
      instance.SetDelete(&delete_maplEECbmModuleIdcOCbmQaCanvasmUgR);
      instance.SetDeleteArray(&deleteArray_maplEECbmModuleIdcOCbmQaCanvasmUgR);
      instance.SetDestructor(&destruct_maplEECbmModuleIdcOCbmQaCanvasmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<ECbmModuleId,CbmQaCanvas*> >()));

      ::ROOT::AddClassAlternate("map<ECbmModuleId,CbmQaCanvas*>","std::map<ECbmModuleId, CbmQaCanvas*, std::less<ECbmModuleId>, std::allocator<std::pair<ECbmModuleId const, CbmQaCanvas*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<ECbmModuleId,CbmQaCanvas*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEECbmModuleIdcOCbmQaCanvasmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<ECbmModuleId,CbmQaCanvas*>*)nullptr)->GetClass();
      maplEECbmModuleIdcOCbmQaCanvasmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEECbmModuleIdcOCbmQaCanvasmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEECbmModuleIdcOCbmQaCanvasmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,CbmQaCanvas*> : new map<ECbmModuleId,CbmQaCanvas*>;
   }
   static void *newArray_maplEECbmModuleIdcOCbmQaCanvasmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,CbmQaCanvas*>[nElements] : new map<ECbmModuleId,CbmQaCanvas*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEECbmModuleIdcOCbmQaCanvasmUgR(void *p) {
      delete ((map<ECbmModuleId,CbmQaCanvas*>*)p);
   }
   static void deleteArray_maplEECbmModuleIdcOCbmQaCanvasmUgR(void *p) {
      delete [] ((map<ECbmModuleId,CbmQaCanvas*>*)p);
   }
   static void destruct_maplEECbmModuleIdcOCbmQaCanvasmUgR(void *p) {
      typedef map<ECbmModuleId,CbmQaCanvas*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<ECbmModuleId,CbmQaCanvas*>

namespace {
  void TriggerDictionaryInitialization_libCbmEventBuilder_Impl() {
    static const char* headers[] = {
"CbmAlgoBuildRawEvents.h",
"CbmTaskBuildRawEvents.h",
"CbmBuildEventsIdeal.h",
"CbmBuildEventsIdealNew.h",
"CbmBuildEventsQa.h",
"CbmBuildEventsSimple.h",
"CbmSeedFinderSlidingWindow.h",
"CbmSeedFinderQa.h",
"CbmBuildEventsFromTracksIdeal.h",
"CbmBuildEventsFromTracksReal.h",
"CbmEventBuilderQa.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/eventbuilder",
"/home/heinemann/gpuunpacker/cbmroot/reco/eventbuilder/digis",
"/home/heinemann/gpuunpacker/cbmroot/reco/eventbuilder/tracks",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/eventbuilder",
"/home/heinemann/gpuunpacker/cbmroot/reco/eventbuilder/digis",
"/home/heinemann/gpuunpacker/cbmroot/reco/eventbuilder/tracks",
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
"/home/heinemann/gpuunpacker/cbmroot/core/qa/checker",
"/home/heinemann/gpuunpacker/cbmroot/core/qa",
"/home/heinemann/gpuunpacker/cbmroot/external/yaml-cpp/include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca/simd",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/OffLineInterface",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/ParticleFinder",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/utils",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/eventbuilder/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmEventBuilder dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmAlgoBuildRawEvents.h")))  CbmAlgoBuildRawEvents;
class __attribute__((annotate("$clingAutoload$CbmTaskBuildRawEvents.h")))  CbmTaskBuildRawEvents;
class __attribute__((annotate("$clingAutoload$CbmBuildEventsIdeal.h")))  CbmBuildEventsIdeal;
class __attribute__((annotate("$clingAutoload$CbmBuildEventsIdealNew.h")))  CbmBuildEventsIdealNew;
class __attribute__((annotate("$clingAutoload$CbmBuildEventsQa.h")))  CbmBuildEventsQa;
class __attribute__((annotate("$clingAutoload$CbmBuildEventsSimple.h")))  CbmBuildEventsSimple;
class __attribute__((annotate("$clingAutoload$CbmSeedFinderSlidingWindow.h")))  CbmSeedFinderSlidingWindow;
class __attribute__((annotate("$clingAutoload$CbmBuildEventsFromTracksIdeal.h")))  CbmBuildEventsFromTracksIdeal;
class __attribute__((annotate("$clingAutoload$CbmBuildEventsFromTracksReal.h")))  CbmBuildEventsFromTracksReal;
class __attribute__((annotate("$clingAutoload$CbmEventBuilderQa.h")))  CbmEventBuilderQa;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmEventBuilder dictionary payload"

#ifndef HAVE_SSE
  #define HAVE_SSE 1
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
#include "CbmAlgoBuildRawEvents.h"
#include "CbmTaskBuildRawEvents.h"
#include "CbmBuildEventsIdeal.h"
#include "CbmBuildEventsIdealNew.h"
#include "CbmBuildEventsQa.h"
#include "CbmBuildEventsSimple.h"
#include "CbmSeedFinderSlidingWindow.h"
#include "CbmSeedFinderQa.h"
#include "CbmBuildEventsFromTracksIdeal.h"
#include "CbmBuildEventsFromTracksReal.h"
#include "CbmEventBuilderQa.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmAlgoBuildRawEvents", payloadCode, "@",
"CbmBuildEventsFromTracksIdeal", payloadCode, "@",
"CbmBuildEventsFromTracksReal", payloadCode, "@",
"CbmBuildEventsIdeal", payloadCode, "@",
"CbmBuildEventsIdealNew", payloadCode, "@",
"CbmBuildEventsQa", payloadCode, "@",
"CbmBuildEventsSimple", payloadCode, "@",
"CbmEventBuilderQa", payloadCode, "@",
"CbmSeedFinderSlidingWindow", payloadCode, "@",
"CbmTaskBuildRawEvents", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmEventBuilder",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmEventBuilder_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmEventBuilder_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmEventBuilder() {
  TriggerDictionaryInitialization_libCbmEventBuilder_Impl();
}
