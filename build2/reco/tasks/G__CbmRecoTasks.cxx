// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmRecoTasks
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
#include "CbmReco.h"
#include "CbmSourceTs.h"
#include "CbmTaskBuildEvents.h"
#include "CbmTaskDigiEventQa.h"
#include "CbmTaskEventsCloneInToOut.h"
#include "CbmTaskMakeRecoEvents.h"
#include "CbmTaskTriggerDigi.h"
#include "CbmTaskTofHitFinder.h"
#include "CbmTaskUnpack.h"
#include "CbmTaskUnpackXpu.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmRecoConfig(void *p = nullptr);
   static void *newArray_CbmRecoConfig(Long_t size, void *p);
   static void delete_CbmRecoConfig(void *p);
   static void deleteArray_CbmRecoConfig(void *p);
   static void destruct_CbmRecoConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRecoConfig*)
   {
      ::CbmRecoConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRecoConfig >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRecoConfig", ::CbmRecoConfig::Class_Version(), "CbmReco.h", 22,
                  typeid(::CbmRecoConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRecoConfig::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRecoConfig) );
      instance.SetNew(&new_CbmRecoConfig);
      instance.SetNewArray(&newArray_CbmRecoConfig);
      instance.SetDelete(&delete_CbmRecoConfig);
      instance.SetDeleteArray(&deleteArray_CbmRecoConfig);
      instance.SetDestructor(&destruct_CbmRecoConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRecoConfig*)
   {
      return GenerateInitInstanceLocal((::CbmRecoConfig*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRecoConfig*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmReco(void *p = nullptr);
   static void *newArray_CbmReco(Long_t size, void *p);
   static void delete_CbmReco(void *p);
   static void deleteArray_CbmReco(void *p);
   static void destruct_CbmReco(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmReco*)
   {
      ::CbmReco *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmReco >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmReco", ::CbmReco::Class_Version(), "CbmReco.h", 60,
                  typeid(::CbmReco), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmReco::Dictionary, isa_proxy, 4,
                  sizeof(::CbmReco) );
      instance.SetNew(&new_CbmReco);
      instance.SetNewArray(&newArray_CbmReco);
      instance.SetDelete(&delete_CbmReco);
      instance.SetDeleteArray(&deleteArray_CbmReco);
      instance.SetDestructor(&destruct_CbmReco);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmReco*)
   {
      return GenerateInitInstanceLocal((::CbmReco*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmReco*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmSourceTs(void *p = nullptr);
   static void *newArray_CbmSourceTs(Long_t size, void *p);
   static void delete_CbmSourceTs(void *p);
   static void deleteArray_CbmSourceTs(void *p);
   static void destruct_CbmSourceTs(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmSourceTs*)
   {
      ::CbmSourceTs *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmSourceTs >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmSourceTs", ::CbmSourceTs::Class_Version(), "CbmSourceTs.h", 26,
                  typeid(::CbmSourceTs), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmSourceTs::Dictionary, isa_proxy, 4,
                  sizeof(::CbmSourceTs) );
      instance.SetNew(&new_CbmSourceTs);
      instance.SetNewArray(&newArray_CbmSourceTs);
      instance.SetDelete(&delete_CbmSourceTs);
      instance.SetDeleteArray(&deleteArray_CbmSourceTs);
      instance.SetDestructor(&destruct_CbmSourceTs);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmSourceTs*)
   {
      return GenerateInitInstanceLocal((::CbmSourceTs*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmSourceTs*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTaskBuildEvents(void *p = nullptr);
   static void *newArray_CbmTaskBuildEvents(Long_t size, void *p);
   static void delete_CbmTaskBuildEvents(void *p);
   static void deleteArray_CbmTaskBuildEvents(void *p);
   static void destruct_CbmTaskBuildEvents(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTaskBuildEvents*)
   {
      ::CbmTaskBuildEvents *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTaskBuildEvents >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTaskBuildEvents", ::CbmTaskBuildEvents::Class_Version(), "CbmTaskBuildEvents.h", 31,
                  typeid(::CbmTaskBuildEvents), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTaskBuildEvents::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTaskBuildEvents) );
      instance.SetNew(&new_CbmTaskBuildEvents);
      instance.SetNewArray(&newArray_CbmTaskBuildEvents);
      instance.SetDelete(&delete_CbmTaskBuildEvents);
      instance.SetDeleteArray(&deleteArray_CbmTaskBuildEvents);
      instance.SetDestructor(&destruct_CbmTaskBuildEvents);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTaskBuildEvents*)
   {
      return GenerateInitInstanceLocal((::CbmTaskBuildEvents*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTaskBuildEvents*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTaskDigiEventQa(void *p = nullptr);
   static void *newArray_CbmTaskDigiEventQa(Long_t size, void *p);
   static void delete_CbmTaskDigiEventQa(void *p);
   static void deleteArray_CbmTaskDigiEventQa(void *p);
   static void destruct_CbmTaskDigiEventQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTaskDigiEventQa*)
   {
      ::CbmTaskDigiEventQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTaskDigiEventQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTaskDigiEventQa", ::CbmTaskDigiEventQa::Class_Version(), "CbmTaskDigiEventQa.h", 30,
                  typeid(::CbmTaskDigiEventQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTaskDigiEventQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTaskDigiEventQa) );
      instance.SetNew(&new_CbmTaskDigiEventQa);
      instance.SetNewArray(&newArray_CbmTaskDigiEventQa);
      instance.SetDelete(&delete_CbmTaskDigiEventQa);
      instance.SetDeleteArray(&deleteArray_CbmTaskDigiEventQa);
      instance.SetDestructor(&destruct_CbmTaskDigiEventQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTaskDigiEventQa*)
   {
      return GenerateInitInstanceLocal((::CbmTaskDigiEventQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTaskDigiEventQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTaskEventsCloneInToOut(void *p = nullptr);
   static void *newArray_CbmTaskEventsCloneInToOut(Long_t size, void *p);
   static void delete_CbmTaskEventsCloneInToOut(void *p);
   static void deleteArray_CbmTaskEventsCloneInToOut(void *p);
   static void destruct_CbmTaskEventsCloneInToOut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTaskEventsCloneInToOut*)
   {
      ::CbmTaskEventsCloneInToOut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTaskEventsCloneInToOut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTaskEventsCloneInToOut", ::CbmTaskEventsCloneInToOut::Class_Version(), "CbmTaskEventsCloneInToOut.h", 29,
                  typeid(::CbmTaskEventsCloneInToOut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTaskEventsCloneInToOut::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTaskEventsCloneInToOut) );
      instance.SetNew(&new_CbmTaskEventsCloneInToOut);
      instance.SetNewArray(&newArray_CbmTaskEventsCloneInToOut);
      instance.SetDelete(&delete_CbmTaskEventsCloneInToOut);
      instance.SetDeleteArray(&deleteArray_CbmTaskEventsCloneInToOut);
      instance.SetDestructor(&destruct_CbmTaskEventsCloneInToOut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTaskEventsCloneInToOut*)
   {
      return GenerateInitInstanceLocal((::CbmTaskEventsCloneInToOut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTaskEventsCloneInToOut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTaskMakeRecoEvents(void *p = nullptr);
   static void *newArray_CbmTaskMakeRecoEvents(Long_t size, void *p);
   static void delete_CbmTaskMakeRecoEvents(void *p);
   static void deleteArray_CbmTaskMakeRecoEvents(void *p);
   static void destruct_CbmTaskMakeRecoEvents(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTaskMakeRecoEvents*)
   {
      ::CbmTaskMakeRecoEvents *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTaskMakeRecoEvents >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTaskMakeRecoEvents", ::CbmTaskMakeRecoEvents::Class_Version(), "CbmTaskMakeRecoEvents.h", 39,
                  typeid(::CbmTaskMakeRecoEvents), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTaskMakeRecoEvents::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTaskMakeRecoEvents) );
      instance.SetNew(&new_CbmTaskMakeRecoEvents);
      instance.SetNewArray(&newArray_CbmTaskMakeRecoEvents);
      instance.SetDelete(&delete_CbmTaskMakeRecoEvents);
      instance.SetDeleteArray(&deleteArray_CbmTaskMakeRecoEvents);
      instance.SetDestructor(&destruct_CbmTaskMakeRecoEvents);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTaskMakeRecoEvents*)
   {
      return GenerateInitInstanceLocal((::CbmTaskMakeRecoEvents*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTaskMakeRecoEvents*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTaskTriggerDigi(void *p = nullptr);
   static void *newArray_CbmTaskTriggerDigi(Long_t size, void *p);
   static void delete_CbmTaskTriggerDigi(void *p);
   static void deleteArray_CbmTaskTriggerDigi(void *p);
   static void destruct_CbmTaskTriggerDigi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTaskTriggerDigi*)
   {
      ::CbmTaskTriggerDigi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTaskTriggerDigi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTaskTriggerDigi", ::CbmTaskTriggerDigi::Class_Version(), "CbmTaskTriggerDigi.h", 35,
                  typeid(::CbmTaskTriggerDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTaskTriggerDigi::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTaskTriggerDigi) );
      instance.SetNew(&new_CbmTaskTriggerDigi);
      instance.SetNewArray(&newArray_CbmTaskTriggerDigi);
      instance.SetDelete(&delete_CbmTaskTriggerDigi);
      instance.SetDeleteArray(&deleteArray_CbmTaskTriggerDigi);
      instance.SetDestructor(&destruct_CbmTaskTriggerDigi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTaskTriggerDigi*)
   {
      return GenerateInitInstanceLocal((::CbmTaskTriggerDigi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTaskTriggerDigi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTaskTofHitFinder(void *p = nullptr);
   static void *newArray_CbmTaskTofHitFinder(Long_t size, void *p);
   static void delete_CbmTaskTofHitFinder(void *p);
   static void deleteArray_CbmTaskTofHitFinder(void *p);
   static void destruct_CbmTaskTofHitFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTaskTofHitFinder*)
   {
      ::CbmTaskTofHitFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTaskTofHitFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTaskTofHitFinder", ::CbmTaskTofHitFinder::Class_Version(), "CbmTaskTofHitFinder.h", 39,
                  typeid(::CbmTaskTofHitFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTaskTofHitFinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTaskTofHitFinder) );
      instance.SetNew(&new_CbmTaskTofHitFinder);
      instance.SetNewArray(&newArray_CbmTaskTofHitFinder);
      instance.SetDelete(&delete_CbmTaskTofHitFinder);
      instance.SetDeleteArray(&deleteArray_CbmTaskTofHitFinder);
      instance.SetDestructor(&destruct_CbmTaskTofHitFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTaskTofHitFinder*)
   {
      return GenerateInitInstanceLocal((::CbmTaskTofHitFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTaskTofHitFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTaskUnpack(void *p = nullptr);
   static void *newArray_CbmTaskUnpack(Long_t size, void *p);
   static void delete_CbmTaskUnpack(void *p);
   static void deleteArray_CbmTaskUnpack(void *p);
   static void destruct_CbmTaskUnpack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTaskUnpack*)
   {
      ::CbmTaskUnpack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTaskUnpack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTaskUnpack", ::CbmTaskUnpack::Class_Version(), "CbmTaskUnpack.h", 47,
                  typeid(::CbmTaskUnpack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTaskUnpack::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTaskUnpack) );
      instance.SetNew(&new_CbmTaskUnpack);
      instance.SetNewArray(&newArray_CbmTaskUnpack);
      instance.SetDelete(&delete_CbmTaskUnpack);
      instance.SetDeleteArray(&deleteArray_CbmTaskUnpack);
      instance.SetDestructor(&destruct_CbmTaskUnpack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTaskUnpack*)
   {
      return GenerateInitInstanceLocal((::CbmTaskUnpack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTaskUnpack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTaskUnpackXpu(void *p = nullptr);
   static void *newArray_CbmTaskUnpackXpu(Long_t size, void *p);
   static void delete_CbmTaskUnpackXpu(void *p);
   static void deleteArray_CbmTaskUnpackXpu(void *p);
   static void destruct_CbmTaskUnpackXpu(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTaskUnpackXpu*)
   {
      ::CbmTaskUnpackXpu *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTaskUnpackXpu >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTaskUnpackXpu", ::CbmTaskUnpackXpu::Class_Version(), "CbmTaskUnpackXpu.h", 37,
                  typeid(::CbmTaskUnpackXpu), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTaskUnpackXpu::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTaskUnpackXpu) );
      instance.SetNew(&new_CbmTaskUnpackXpu);
      instance.SetNewArray(&newArray_CbmTaskUnpackXpu);
      instance.SetDelete(&delete_CbmTaskUnpackXpu);
      instance.SetDeleteArray(&deleteArray_CbmTaskUnpackXpu);
      instance.SetDestructor(&destruct_CbmTaskUnpackXpu);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTaskUnpackXpu*)
   {
      return GenerateInitInstanceLocal((::CbmTaskUnpackXpu*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTaskUnpackXpu*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmRecoConfig::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRecoConfig::Class_Name()
{
   return "CbmRecoConfig";
}

//______________________________________________________________________________
const char *CbmRecoConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoConfig*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRecoConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoConfig*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRecoConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoConfig*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRecoConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoConfig*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmReco::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmReco::Class_Name()
{
   return "CbmReco";
}

//______________________________________________________________________________
const char *CbmReco::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmReco*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmReco::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmReco*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmReco::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmReco*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmReco::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmReco*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmSourceTs::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmSourceTs::Class_Name()
{
   return "CbmSourceTs";
}

//______________________________________________________________________________
const char *CbmSourceTs::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmSourceTs*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmSourceTs::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmSourceTs*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmSourceTs::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmSourceTs*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmSourceTs::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmSourceTs*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTaskBuildEvents::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTaskBuildEvents::Class_Name()
{
   return "CbmTaskBuildEvents";
}

//______________________________________________________________________________
const char *CbmTaskBuildEvents::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskBuildEvents*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTaskBuildEvents::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskBuildEvents*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTaskBuildEvents::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskBuildEvents*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTaskBuildEvents::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskBuildEvents*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTaskDigiEventQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTaskDigiEventQa::Class_Name()
{
   return "CbmTaskDigiEventQa";
}

//______________________________________________________________________________
const char *CbmTaskDigiEventQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskDigiEventQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTaskDigiEventQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskDigiEventQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTaskDigiEventQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskDigiEventQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTaskDigiEventQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskDigiEventQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTaskEventsCloneInToOut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTaskEventsCloneInToOut::Class_Name()
{
   return "CbmTaskEventsCloneInToOut";
}

//______________________________________________________________________________
const char *CbmTaskEventsCloneInToOut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskEventsCloneInToOut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTaskEventsCloneInToOut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskEventsCloneInToOut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTaskEventsCloneInToOut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskEventsCloneInToOut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTaskEventsCloneInToOut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskEventsCloneInToOut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTaskMakeRecoEvents::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTaskMakeRecoEvents::Class_Name()
{
   return "CbmTaskMakeRecoEvents";
}

//______________________________________________________________________________
const char *CbmTaskMakeRecoEvents::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskMakeRecoEvents*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTaskMakeRecoEvents::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskMakeRecoEvents*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTaskMakeRecoEvents::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskMakeRecoEvents*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTaskMakeRecoEvents::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskMakeRecoEvents*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTaskTriggerDigi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTaskTriggerDigi::Class_Name()
{
   return "CbmTaskTriggerDigi";
}

//______________________________________________________________________________
const char *CbmTaskTriggerDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskTriggerDigi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTaskTriggerDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskTriggerDigi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTaskTriggerDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskTriggerDigi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTaskTriggerDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskTriggerDigi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTaskTofHitFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTaskTofHitFinder::Class_Name()
{
   return "CbmTaskTofHitFinder";
}

//______________________________________________________________________________
const char *CbmTaskTofHitFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskTofHitFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTaskTofHitFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskTofHitFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTaskTofHitFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskTofHitFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTaskTofHitFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskTofHitFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTaskUnpack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTaskUnpack::Class_Name()
{
   return "CbmTaskUnpack";
}

//______________________________________________________________________________
const char *CbmTaskUnpack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskUnpack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTaskUnpack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskUnpack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTaskUnpack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskUnpack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTaskUnpack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskUnpack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTaskUnpackXpu::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTaskUnpackXpu::Class_Name()
{
   return "CbmTaskUnpackXpu";
}

//______________________________________________________________________________
const char *CbmTaskUnpackXpu::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskUnpackXpu*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTaskUnpackXpu::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskUnpackXpu*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTaskUnpackXpu::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskUnpackXpu*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTaskUnpackXpu::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTaskUnpackXpu*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmRecoConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRecoConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRecoConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRecoConfig::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRecoConfig(void *p) {
      return  p ? new(p) ::CbmRecoConfig : new ::CbmRecoConfig;
   }
   static void *newArray_CbmRecoConfig(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRecoConfig[nElements] : new ::CbmRecoConfig[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRecoConfig(void *p) {
      delete ((::CbmRecoConfig*)p);
   }
   static void deleteArray_CbmRecoConfig(void *p) {
      delete [] ((::CbmRecoConfig*)p);
   }
   static void destruct_CbmRecoConfig(void *p) {
      typedef ::CbmRecoConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRecoConfig

//______________________________________________________________________________
void CbmReco::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmReco.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmReco::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmReco::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmReco(void *p) {
      return  p ? new(p) ::CbmReco : new ::CbmReco;
   }
   static void *newArray_CbmReco(Long_t nElements, void *p) {
      return p ? new(p) ::CbmReco[nElements] : new ::CbmReco[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmReco(void *p) {
      delete ((::CbmReco*)p);
   }
   static void deleteArray_CbmReco(void *p) {
      delete [] ((::CbmReco*)p);
   }
   static void destruct_CbmReco(void *p) {
      typedef ::CbmReco current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmReco

//______________________________________________________________________________
void CbmSourceTs::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmSourceTs.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmSourceTs::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmSourceTs::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmSourceTs(void *p) {
      return  p ? new(p) ::CbmSourceTs : new ::CbmSourceTs;
   }
   static void *newArray_CbmSourceTs(Long_t nElements, void *p) {
      return p ? new(p) ::CbmSourceTs[nElements] : new ::CbmSourceTs[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmSourceTs(void *p) {
      delete ((::CbmSourceTs*)p);
   }
   static void deleteArray_CbmSourceTs(void *p) {
      delete [] ((::CbmSourceTs*)p);
   }
   static void destruct_CbmSourceTs(void *p) {
      typedef ::CbmSourceTs current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmSourceTs

//______________________________________________________________________________
void CbmTaskBuildEvents::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTaskBuildEvents.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTaskBuildEvents::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTaskBuildEvents::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTaskBuildEvents(void *p) {
      return  p ? new(p) ::CbmTaskBuildEvents : new ::CbmTaskBuildEvents;
   }
   static void *newArray_CbmTaskBuildEvents(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTaskBuildEvents[nElements] : new ::CbmTaskBuildEvents[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTaskBuildEvents(void *p) {
      delete ((::CbmTaskBuildEvents*)p);
   }
   static void deleteArray_CbmTaskBuildEvents(void *p) {
      delete [] ((::CbmTaskBuildEvents*)p);
   }
   static void destruct_CbmTaskBuildEvents(void *p) {
      typedef ::CbmTaskBuildEvents current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTaskBuildEvents

//______________________________________________________________________________
void CbmTaskDigiEventQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTaskDigiEventQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTaskDigiEventQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTaskDigiEventQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTaskDigiEventQa(void *p) {
      return  p ? new(p) ::CbmTaskDigiEventQa : new ::CbmTaskDigiEventQa;
   }
   static void *newArray_CbmTaskDigiEventQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTaskDigiEventQa[nElements] : new ::CbmTaskDigiEventQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTaskDigiEventQa(void *p) {
      delete ((::CbmTaskDigiEventQa*)p);
   }
   static void deleteArray_CbmTaskDigiEventQa(void *p) {
      delete [] ((::CbmTaskDigiEventQa*)p);
   }
   static void destruct_CbmTaskDigiEventQa(void *p) {
      typedef ::CbmTaskDigiEventQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTaskDigiEventQa

//______________________________________________________________________________
void CbmTaskEventsCloneInToOut::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTaskEventsCloneInToOut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTaskEventsCloneInToOut::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTaskEventsCloneInToOut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTaskEventsCloneInToOut(void *p) {
      return  p ? new(p) ::CbmTaskEventsCloneInToOut : new ::CbmTaskEventsCloneInToOut;
   }
   static void *newArray_CbmTaskEventsCloneInToOut(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTaskEventsCloneInToOut[nElements] : new ::CbmTaskEventsCloneInToOut[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTaskEventsCloneInToOut(void *p) {
      delete ((::CbmTaskEventsCloneInToOut*)p);
   }
   static void deleteArray_CbmTaskEventsCloneInToOut(void *p) {
      delete [] ((::CbmTaskEventsCloneInToOut*)p);
   }
   static void destruct_CbmTaskEventsCloneInToOut(void *p) {
      typedef ::CbmTaskEventsCloneInToOut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTaskEventsCloneInToOut

//______________________________________________________________________________
void CbmTaskMakeRecoEvents::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTaskMakeRecoEvents.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTaskMakeRecoEvents::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTaskMakeRecoEvents::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTaskMakeRecoEvents(void *p) {
      return  p ? new(p) ::CbmTaskMakeRecoEvents : new ::CbmTaskMakeRecoEvents;
   }
   static void *newArray_CbmTaskMakeRecoEvents(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTaskMakeRecoEvents[nElements] : new ::CbmTaskMakeRecoEvents[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTaskMakeRecoEvents(void *p) {
      delete ((::CbmTaskMakeRecoEvents*)p);
   }
   static void deleteArray_CbmTaskMakeRecoEvents(void *p) {
      delete [] ((::CbmTaskMakeRecoEvents*)p);
   }
   static void destruct_CbmTaskMakeRecoEvents(void *p) {
      typedef ::CbmTaskMakeRecoEvents current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTaskMakeRecoEvents

//______________________________________________________________________________
void CbmTaskTriggerDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTaskTriggerDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTaskTriggerDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTaskTriggerDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTaskTriggerDigi(void *p) {
      return  p ? new(p) ::CbmTaskTriggerDigi : new ::CbmTaskTriggerDigi;
   }
   static void *newArray_CbmTaskTriggerDigi(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTaskTriggerDigi[nElements] : new ::CbmTaskTriggerDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTaskTriggerDigi(void *p) {
      delete ((::CbmTaskTriggerDigi*)p);
   }
   static void deleteArray_CbmTaskTriggerDigi(void *p) {
      delete [] ((::CbmTaskTriggerDigi*)p);
   }
   static void destruct_CbmTaskTriggerDigi(void *p) {
      typedef ::CbmTaskTriggerDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTaskTriggerDigi

//______________________________________________________________________________
void CbmTaskTofHitFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTaskTofHitFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTaskTofHitFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTaskTofHitFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTaskTofHitFinder(void *p) {
      return  p ? new(p) ::CbmTaskTofHitFinder : new ::CbmTaskTofHitFinder;
   }
   static void *newArray_CbmTaskTofHitFinder(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTaskTofHitFinder[nElements] : new ::CbmTaskTofHitFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTaskTofHitFinder(void *p) {
      delete ((::CbmTaskTofHitFinder*)p);
   }
   static void deleteArray_CbmTaskTofHitFinder(void *p) {
      delete [] ((::CbmTaskTofHitFinder*)p);
   }
   static void destruct_CbmTaskTofHitFinder(void *p) {
      typedef ::CbmTaskTofHitFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTaskTofHitFinder

//______________________________________________________________________________
void CbmTaskUnpack::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTaskUnpack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTaskUnpack::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTaskUnpack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTaskUnpack(void *p) {
      return  p ? new(p) ::CbmTaskUnpack : new ::CbmTaskUnpack;
   }
   static void *newArray_CbmTaskUnpack(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTaskUnpack[nElements] : new ::CbmTaskUnpack[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTaskUnpack(void *p) {
      delete ((::CbmTaskUnpack*)p);
   }
   static void deleteArray_CbmTaskUnpack(void *p) {
      delete [] ((::CbmTaskUnpack*)p);
   }
   static void destruct_CbmTaskUnpack(void *p) {
      typedef ::CbmTaskUnpack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTaskUnpack

//______________________________________________________________________________
void CbmTaskUnpackXpu::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTaskUnpackXpu.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTaskUnpackXpu::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTaskUnpackXpu::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTaskUnpackXpu(void *p) {
      return  p ? new(p) ::CbmTaskUnpackXpu : new ::CbmTaskUnpackXpu;
   }
   static void *newArray_CbmTaskUnpackXpu(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTaskUnpackXpu[nElements] : new ::CbmTaskUnpackXpu[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTaskUnpackXpu(void *p) {
      delete ((::CbmTaskUnpackXpu*)p);
   }
   static void deleteArray_CbmTaskUnpackXpu(void *p) {
      delete [] ((::CbmTaskUnpackXpu*)p);
   }
   static void destruct_CbmTaskUnpackXpu(void *p) {
      typedef ::CbmTaskUnpackXpu current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTaskUnpackXpu

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<vector<vector<double> > > > >*)
   {
      vector<vector<vector<vector<vector<double> > > > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<vector<vector<double> > > > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<vector<vector<double> > > > >", -2, "vector", 389,
                  typeid(vector<vector<vector<vector<vector<double> > > > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<vector<vector<double> > > > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<vector<vector<double> > > > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<vector<vector<double> > > > >","std::vector<std::vector<std::vector<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >, std::allocator<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > > >, std::allocator<std::vector<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >, std::allocator<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > > > > >, std::allocator<std::vector<std::vector<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >, std::allocator<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > > >, std::allocator<std::vector<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >, std::allocator<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<vector<vector<double> > > > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<vector<vector<double> > > > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<vector<double> > > > > : new vector<vector<vector<vector<vector<double> > > > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<vector<double> > > > >[nElements] : new vector<vector<vector<vector<vector<double> > > > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<vector<vector<double> > > > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<vector<vector<double> > > > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<vector<vector<double> > > > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<vector<vector<double> > > > >

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<vector<double> > > >*)
   {
      vector<vector<vector<vector<double> > > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<vector<double> > > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<vector<double> > > >", -2, "vector", 389,
                  typeid(vector<vector<vector<vector<double> > > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<vector<double> > > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<vector<double> > > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<vector<double> > > >","std::vector<std::vector<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >, std::allocator<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > > >, std::allocator<std::vector<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >, std::allocator<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<vector<double> > > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<vector<double> > > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<double> > > > : new vector<vector<vector<vector<double> > > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<double> > > >[nElements] : new vector<vector<vector<vector<double> > > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<vector<double> > > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<vector<double> > > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEvectorlEdoublegRsPgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<vector<double> > > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<vector<double> > > >

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
   static TClass *vectorlEvectorlEvectorlEdoublegRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEdoublegRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<double> > >*)
   {
      vector<vector<vector<double> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<double> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<double> > >", -2, "vector", 389,
                  typeid(vector<vector<vector<double> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEdoublegRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<double> > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<double> > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<double> > >","std::vector<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >, std::allocator<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<double> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEdoublegRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<double> > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlEdoublegRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEdoublegRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<double> > > : new vector<vector<vector<double> > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<double> > >[nElements] : new vector<vector<vector<double> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<double> > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<double> > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      typedef vector<vector<vector<double> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<double> > >

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<CbmTofDigi> > >*)
   {
      vector<vector<vector<CbmTofDigi> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<CbmTofDigi> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<CbmTofDigi> > >", -2, "vector", 389,
                  typeid(vector<vector<vector<CbmTofDigi> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<CbmTofDigi> > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<CbmTofDigi> > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<CbmTofDigi> > >","std::vector<std::vector<std::vector<CbmTofDigi, std::allocator<CbmTofDigi> >, std::allocator<std::vector<CbmTofDigi, std::allocator<CbmTofDigi> > > >, std::allocator<std::vector<std::vector<CbmTofDigi, std::allocator<CbmTofDigi> >, std::allocator<std::vector<CbmTofDigi, std::allocator<CbmTofDigi> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<CbmTofDigi> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<CbmTofDigi> > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<CbmTofDigi> > > : new vector<vector<vector<CbmTofDigi> > >;
   }
   static void *newArray_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<CbmTofDigi> > >[nElements] : new vector<vector<vector<CbmTofDigi> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<CbmTofDigi> > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<CbmTofDigi> > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlECbmTofDigigRsPgRsPgR(void *p) {
      typedef vector<vector<vector<CbmTofDigi> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<CbmTofDigi> > >

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
   static TClass *vectorlEvectorlEdoublegRsPgR_Dictionary();
   static void vectorlEvectorlEdoublegRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEdoublegRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEdoublegRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEdoublegRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEdoublegRsPgR(void *p);
   static void destruct_vectorlEvectorlEdoublegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<double> >*)
   {
      vector<vector<double> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<double> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<double> >", -2, "vector", 389,
                  typeid(vector<vector<double> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEdoublegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<double> >) );
      instance.SetNew(&new_vectorlEvectorlEdoublegRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEdoublegRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEdoublegRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEdoublegRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEdoublegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<double> > >()));

      ::ROOT::AddClassAlternate("vector<vector<double> >","std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<double> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEdoublegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<double> >*)nullptr)->GetClass();
      vectorlEvectorlEdoublegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEdoublegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEdoublegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<double> > : new vector<vector<double> >;
   }
   static void *newArray_vectorlEvectorlEdoublegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<double> >[nElements] : new vector<vector<double> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEdoublegRsPgR(void *p) {
      delete ((vector<vector<double> >*)p);
   }
   static void deleteArray_vectorlEvectorlEdoublegRsPgR(void *p) {
      delete [] ((vector<vector<double> >*)p);
   }
   static void destruct_vectorlEvectorlEdoublegRsPgR(void *p) {
      typedef vector<vector<double> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<double> >

namespace ROOT {
   static TClass *vectorlEvectorlECbmTofDigigRsPgR_Dictionary();
   static void vectorlEvectorlECbmTofDigigRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlECbmTofDigigRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlECbmTofDigigRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlECbmTofDigigRsPgR(void *p);
   static void deleteArray_vectorlEvectorlECbmTofDigigRsPgR(void *p);
   static void destruct_vectorlEvectorlECbmTofDigigRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<CbmTofDigi> >*)
   {
      vector<vector<CbmTofDigi> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<CbmTofDigi> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<CbmTofDigi> >", -2, "vector", 389,
                  typeid(vector<vector<CbmTofDigi> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlECbmTofDigigRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<CbmTofDigi> >) );
      instance.SetNew(&new_vectorlEvectorlECbmTofDigigRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlECbmTofDigigRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlECbmTofDigigRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlECbmTofDigigRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlECbmTofDigigRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<CbmTofDigi> > >()));

      ::ROOT::AddClassAlternate("vector<vector<CbmTofDigi> >","std::vector<std::vector<CbmTofDigi, std::allocator<CbmTofDigi> >, std::allocator<std::vector<CbmTofDigi, std::allocator<CbmTofDigi> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<CbmTofDigi> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlECbmTofDigigRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<CbmTofDigi> >*)nullptr)->GetClass();
      vectorlEvectorlECbmTofDigigRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlECbmTofDigigRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlECbmTofDigigRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmTofDigi> > : new vector<vector<CbmTofDigi> >;
   }
   static void *newArray_vectorlEvectorlECbmTofDigigRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmTofDigi> >[nElements] : new vector<vector<CbmTofDigi> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlECbmTofDigigRsPgR(void *p) {
      delete ((vector<vector<CbmTofDigi> >*)p);
   }
   static void deleteArray_vectorlEvectorlECbmTofDigigRsPgR(void *p) {
      delete [] ((vector<vector<CbmTofDigi> >*)p);
   }
   static void destruct_vectorlEvectorlECbmTofDigigRsPgR(void *p) {
      typedef vector<vector<CbmTofDigi> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<CbmTofDigi> >

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
   static TClass *maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR_Dictionary();
   static void maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR(Long_t size, void *p);
   static void delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR(void *p);
   static void deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR(void *p);
   static void destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned short,cbm::algo::UnpackTrd>*)
   {
      map<unsigned short,cbm::algo::UnpackTrd> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned short,cbm::algo::UnpackTrd>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned short,cbm::algo::UnpackTrd>", -2, "map", 100,
                  typeid(map<unsigned short,cbm::algo::UnpackTrd>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned short,cbm::algo::UnpackTrd>) );
      instance.SetNew(&new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR);
      instance.SetNewArray(&newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR);
      instance.SetDelete(&delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR);
      instance.SetDestructor(&destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned short,cbm::algo::UnpackTrd> >()));

      ::ROOT::AddClassAlternate("map<unsigned short,cbm::algo::UnpackTrd>","std::map<unsigned short, cbm::algo::UnpackTrd, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, cbm::algo::UnpackTrd> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned short,cbm::algo::UnpackTrd>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned short,cbm::algo::UnpackTrd>*)nullptr)->GetClass();
      maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,cbm::algo::UnpackTrd> : new map<unsigned short,cbm::algo::UnpackTrd>;
   }
   static void *newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,cbm::algo::UnpackTrd>[nElements] : new map<unsigned short,cbm::algo::UnpackTrd>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR(void *p) {
      delete ((map<unsigned short,cbm::algo::UnpackTrd>*)p);
   }
   static void deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR(void *p) {
      delete [] ((map<unsigned short,cbm::algo::UnpackTrd>*)p);
   }
   static void destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrdgR(void *p) {
      typedef map<unsigned short,cbm::algo::UnpackTrd> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned short,cbm::algo::UnpackTrd>

namespace ROOT {
   static TClass *maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR_Dictionary();
   static void maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR(Long_t size, void *p);
   static void delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR(void *p);
   static void deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR(void *p);
   static void destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned short,cbm::algo::UnpackTrd2d>*)
   {
      map<unsigned short,cbm::algo::UnpackTrd2d> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned short,cbm::algo::UnpackTrd2d>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned short,cbm::algo::UnpackTrd2d>", -2, "map", 100,
                  typeid(map<unsigned short,cbm::algo::UnpackTrd2d>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned short,cbm::algo::UnpackTrd2d>) );
      instance.SetNew(&new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR);
      instance.SetNewArray(&newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR);
      instance.SetDelete(&delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR);
      instance.SetDestructor(&destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned short,cbm::algo::UnpackTrd2d> >()));

      ::ROOT::AddClassAlternate("map<unsigned short,cbm::algo::UnpackTrd2d>","std::map<unsigned short, cbm::algo::UnpackTrd2d, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, cbm::algo::UnpackTrd2d> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned short,cbm::algo::UnpackTrd2d>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned short,cbm::algo::UnpackTrd2d>*)nullptr)->GetClass();
      maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,cbm::algo::UnpackTrd2d> : new map<unsigned short,cbm::algo::UnpackTrd2d>;
   }
   static void *newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,cbm::algo::UnpackTrd2d>[nElements] : new map<unsigned short,cbm::algo::UnpackTrd2d>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR(void *p) {
      delete ((map<unsigned short,cbm::algo::UnpackTrd2d>*)p);
   }
   static void deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR(void *p) {
      delete [] ((map<unsigned short,cbm::algo::UnpackTrd2d>*)p);
   }
   static void destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTrd2dgR(void *p) {
      typedef map<unsigned short,cbm::algo::UnpackTrd2d> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned short,cbm::algo::UnpackTrd2d>

namespace ROOT {
   static TClass *maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR_Dictionary();
   static void maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR(Long_t size, void *p);
   static void delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR(void *p);
   static void deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR(void *p);
   static void destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned short,cbm::algo::UnpackTof>*)
   {
      map<unsigned short,cbm::algo::UnpackTof> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned short,cbm::algo::UnpackTof>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned short,cbm::algo::UnpackTof>", -2, "map", 100,
                  typeid(map<unsigned short,cbm::algo::UnpackTof>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned short,cbm::algo::UnpackTof>) );
      instance.SetNew(&new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR);
      instance.SetNewArray(&newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR);
      instance.SetDelete(&delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR);
      instance.SetDestructor(&destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned short,cbm::algo::UnpackTof> >()));

      ::ROOT::AddClassAlternate("map<unsigned short,cbm::algo::UnpackTof>","std::map<unsigned short, cbm::algo::UnpackTof, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, cbm::algo::UnpackTof> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned short,cbm::algo::UnpackTof>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned short,cbm::algo::UnpackTof>*)nullptr)->GetClass();
      maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,cbm::algo::UnpackTof> : new map<unsigned short,cbm::algo::UnpackTof>;
   }
   static void *newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,cbm::algo::UnpackTof>[nElements] : new map<unsigned short,cbm::algo::UnpackTof>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR(void *p) {
      delete ((map<unsigned short,cbm::algo::UnpackTof>*)p);
   }
   static void deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR(void *p) {
      delete [] ((map<unsigned short,cbm::algo::UnpackTof>*)p);
   }
   static void destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackTofgR(void *p) {
      typedef map<unsigned short,cbm::algo::UnpackTof> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned short,cbm::algo::UnpackTof>

namespace ROOT {
   static TClass *maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR_Dictionary();
   static void maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR(Long_t size, void *p);
   static void delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR(void *p);
   static void deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR(void *p);
   static void destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned short,cbm::algo::UnpackSts>*)
   {
      map<unsigned short,cbm::algo::UnpackSts> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned short,cbm::algo::UnpackSts>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned short,cbm::algo::UnpackSts>", -2, "map", 100,
                  typeid(map<unsigned short,cbm::algo::UnpackSts>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned short,cbm::algo::UnpackSts>) );
      instance.SetNew(&new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR);
      instance.SetNewArray(&newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR);
      instance.SetDelete(&delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR);
      instance.SetDestructor(&destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned short,cbm::algo::UnpackSts> >()));

      ::ROOT::AddClassAlternate("map<unsigned short,cbm::algo::UnpackSts>","std::map<unsigned short, cbm::algo::UnpackSts, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, cbm::algo::UnpackSts> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned short,cbm::algo::UnpackSts>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned short,cbm::algo::UnpackSts>*)nullptr)->GetClass();
      maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,cbm::algo::UnpackSts> : new map<unsigned short,cbm::algo::UnpackSts>;
   }
   static void *newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,cbm::algo::UnpackSts>[nElements] : new map<unsigned short,cbm::algo::UnpackSts>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR(void *p) {
      delete ((map<unsigned short,cbm::algo::UnpackSts>*)p);
   }
   static void deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR(void *p) {
      delete [] ((map<unsigned short,cbm::algo::UnpackSts>*)p);
   }
   static void destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackStsgR(void *p) {
      typedef map<unsigned short,cbm::algo::UnpackSts> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned short,cbm::algo::UnpackSts>

namespace ROOT {
   static TClass *maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR_Dictionary();
   static void maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR(Long_t size, void *p);
   static void delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR(void *p);
   static void deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR(void *p);
   static void destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned short,cbm::algo::UnpackMuch>*)
   {
      map<unsigned short,cbm::algo::UnpackMuch> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned short,cbm::algo::UnpackMuch>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned short,cbm::algo::UnpackMuch>", -2, "map", 100,
                  typeid(map<unsigned short,cbm::algo::UnpackMuch>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned short,cbm::algo::UnpackMuch>) );
      instance.SetNew(&new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR);
      instance.SetNewArray(&newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR);
      instance.SetDelete(&delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR);
      instance.SetDestructor(&destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned short,cbm::algo::UnpackMuch> >()));

      ::ROOT::AddClassAlternate("map<unsigned short,cbm::algo::UnpackMuch>","std::map<unsigned short, cbm::algo::UnpackMuch, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, cbm::algo::UnpackMuch> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned short,cbm::algo::UnpackMuch>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned short,cbm::algo::UnpackMuch>*)nullptr)->GetClass();
      maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,cbm::algo::UnpackMuch> : new map<unsigned short,cbm::algo::UnpackMuch>;
   }
   static void *newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,cbm::algo::UnpackMuch>[nElements] : new map<unsigned short,cbm::algo::UnpackMuch>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR(void *p) {
      delete ((map<unsigned short,cbm::algo::UnpackMuch>*)p);
   }
   static void deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR(void *p) {
      delete [] ((map<unsigned short,cbm::algo::UnpackMuch>*)p);
   }
   static void destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackMuchgR(void *p) {
      typedef map<unsigned short,cbm::algo::UnpackMuch> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned short,cbm::algo::UnpackMuch>

namespace ROOT {
   static TClass *maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR_Dictionary();
   static void maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR_TClassManip(TClass*);
   static void *new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR(void *p = nullptr);
   static void *newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR(Long_t size, void *p);
   static void delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR(void *p);
   static void deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR(void *p);
   static void destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned short,cbm::algo::UnpackBmon>*)
   {
      map<unsigned short,cbm::algo::UnpackBmon> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned short,cbm::algo::UnpackBmon>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned short,cbm::algo::UnpackBmon>", -2, "map", 100,
                  typeid(map<unsigned short,cbm::algo::UnpackBmon>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned short,cbm::algo::UnpackBmon>) );
      instance.SetNew(&new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR);
      instance.SetNewArray(&newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR);
      instance.SetDelete(&delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR);
      instance.SetDestructor(&destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned short,cbm::algo::UnpackBmon> >()));

      ::ROOT::AddClassAlternate("map<unsigned short,cbm::algo::UnpackBmon>","std::map<unsigned short, cbm::algo::UnpackBmon, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, cbm::algo::UnpackBmon> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned short,cbm::algo::UnpackBmon>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned short,cbm::algo::UnpackBmon>*)nullptr)->GetClass();
      maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,cbm::algo::UnpackBmon> : new map<unsigned short,cbm::algo::UnpackBmon>;
   }
   static void *newArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,cbm::algo::UnpackBmon>[nElements] : new map<unsigned short,cbm::algo::UnpackBmon>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR(void *p) {
      delete ((map<unsigned short,cbm::algo::UnpackBmon>*)p);
   }
   static void deleteArray_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR(void *p) {
      delete [] ((map<unsigned short,cbm::algo::UnpackBmon>*)p);
   }
   static void destruct_maplEunsignedsPshortcOcbmcLcLalgocLcLUnpackBmongR(void *p) {
      typedef map<unsigned short,cbm::algo::UnpackBmon> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned short,cbm::algo::UnpackBmon>

namespace ROOT {
   static TClass *maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR_Dictionary();
   static void maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR(void *p);
   static void deleteArray_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR(void *p);
   static void destruct_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> >*)
   {
      map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> >", -2, "map", 100,
                  typeid(map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> >) );
      instance.SetNew(&new_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> > >()));

      ::ROOT::AddClassAlternate("map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> >","std::map<unsigned int, std::map<unsigned int, cbm::algo::HitFinderTof, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, cbm::algo::HitFinderTof> > >, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::map<unsigned int, cbm::algo::HitFinderTof, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, cbm::algo::HitFinderTof> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> >*)nullptr)->GetClass();
      maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> > : new map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> >;
   }
   static void *newArray_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> >[nElements] : new map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR(void *p) {
      delete ((map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> >*)p);
   }
   static void deleteArray_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR(void *p) {
      delete [] ((map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> >*)p);
   }
   static void destruct_maplEunsignedsPintcOmaplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgRsPgR(void *p) {
      typedef map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,map<unsigned int,cbm::algo::HitFinderTof> >

namespace ROOT {
   static TClass *maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR_Dictionary();
   static void maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR(void *p);
   static void deleteArray_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR(void *p);
   static void destruct_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,cbm::algo::HitFinderTof>*)
   {
      map<unsigned int,cbm::algo::HitFinderTof> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,cbm::algo::HitFinderTof>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,cbm::algo::HitFinderTof>", -2, "map", 100,
                  typeid(map<unsigned int,cbm::algo::HitFinderTof>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,cbm::algo::HitFinderTof>) );
      instance.SetNew(&new_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,cbm::algo::HitFinderTof> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,cbm::algo::HitFinderTof>","std::map<unsigned int, cbm::algo::HitFinderTof, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, cbm::algo::HitFinderTof> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,cbm::algo::HitFinderTof>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,cbm::algo::HitFinderTof>*)nullptr)->GetClass();
      maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,cbm::algo::HitFinderTof> : new map<unsigned int,cbm::algo::HitFinderTof>;
   }
   static void *newArray_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,cbm::algo::HitFinderTof>[nElements] : new map<unsigned int,cbm::algo::HitFinderTof>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR(void *p) {
      delete ((map<unsigned int,cbm::algo::HitFinderTof>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR(void *p) {
      delete [] ((map<unsigned int,cbm::algo::HitFinderTof>*)p);
   }
   static void destruct_maplEunsignedsPintcOcbmcLcLalgocLcLHitFinderTofgR(void *p) {
      typedef map<unsigned int,cbm::algo::HitFinderTof> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,cbm::algo::HitFinderTof>

namespace ROOT {
   static TClass *maplEECbmModuleIdcOunsignedsPlonggR_Dictionary();
   static void maplEECbmModuleIdcOunsignedsPlonggR_TClassManip(TClass*);
   static void *new_maplEECbmModuleIdcOunsignedsPlonggR(void *p = nullptr);
   static void *newArray_maplEECbmModuleIdcOunsignedsPlonggR(Long_t size, void *p);
   static void delete_maplEECbmModuleIdcOunsignedsPlonggR(void *p);
   static void deleteArray_maplEECbmModuleIdcOunsignedsPlonggR(void *p);
   static void destruct_maplEECbmModuleIdcOunsignedsPlonggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<ECbmModuleId,unsigned long>*)
   {
      map<ECbmModuleId,unsigned long> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<ECbmModuleId,unsigned long>));
      static ::ROOT::TGenericClassInfo 
         instance("map<ECbmModuleId,unsigned long>", -2, "map", 100,
                  typeid(map<ECbmModuleId,unsigned long>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEECbmModuleIdcOunsignedsPlonggR_Dictionary, isa_proxy, 0,
                  sizeof(map<ECbmModuleId,unsigned long>) );
      instance.SetNew(&new_maplEECbmModuleIdcOunsignedsPlonggR);
      instance.SetNewArray(&newArray_maplEECbmModuleIdcOunsignedsPlonggR);
      instance.SetDelete(&delete_maplEECbmModuleIdcOunsignedsPlonggR);
      instance.SetDeleteArray(&deleteArray_maplEECbmModuleIdcOunsignedsPlonggR);
      instance.SetDestructor(&destruct_maplEECbmModuleIdcOunsignedsPlonggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<ECbmModuleId,unsigned long> >()));

      ::ROOT::AddClassAlternate("map<ECbmModuleId,unsigned long>","std::map<ECbmModuleId, unsigned long, std::less<ECbmModuleId>, std::allocator<std::pair<ECbmModuleId const, unsigned long> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<ECbmModuleId,unsigned long>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEECbmModuleIdcOunsignedsPlonggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<ECbmModuleId,unsigned long>*)nullptr)->GetClass();
      maplEECbmModuleIdcOunsignedsPlonggR_TClassManip(theClass);
   return theClass;
   }

   static void maplEECbmModuleIdcOunsignedsPlonggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEECbmModuleIdcOunsignedsPlonggR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,unsigned long> : new map<ECbmModuleId,unsigned long>;
   }
   static void *newArray_maplEECbmModuleIdcOunsignedsPlonggR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,unsigned long>[nElements] : new map<ECbmModuleId,unsigned long>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEECbmModuleIdcOunsignedsPlonggR(void *p) {
      delete ((map<ECbmModuleId,unsigned long>*)p);
   }
   static void deleteArray_maplEECbmModuleIdcOunsignedsPlonggR(void *p) {
      delete [] ((map<ECbmModuleId,unsigned long>*)p);
   }
   static void destruct_maplEECbmModuleIdcOunsignedsPlonggR(void *p) {
      typedef map<ECbmModuleId,unsigned long> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<ECbmModuleId,unsigned long>

namespace ROOT {
   static TClass *maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR_Dictionary();
   static void maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR_TClassManip(TClass*);
   static void *new_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR(void *p = nullptr);
   static void *newArray_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR(Long_t size, void *p);
   static void delete_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR(void *p);
   static void deleteArray_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR(void *p);
   static void destruct_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<ECbmModuleId,pair<double,double> >*)
   {
      map<ECbmModuleId,pair<double,double> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<ECbmModuleId,pair<double,double> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<ECbmModuleId,pair<double,double> >", -2, "map", 100,
                  typeid(map<ECbmModuleId,pair<double,double> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<ECbmModuleId,pair<double,double> >) );
      instance.SetNew(&new_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR);
      instance.SetNewArray(&newArray_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR);
      instance.SetDelete(&delete_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR);
      instance.SetDestructor(&destruct_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<ECbmModuleId,pair<double,double> > >()));

      ::ROOT::AddClassAlternate("map<ECbmModuleId,pair<double,double> >","std::map<ECbmModuleId, std::pair<double, double>, std::less<ECbmModuleId>, std::allocator<std::pair<ECbmModuleId const, std::pair<double, double> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<ECbmModuleId,pair<double,double> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<ECbmModuleId,pair<double,double> >*)nullptr)->GetClass();
      maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,pair<double,double> > : new map<ECbmModuleId,pair<double,double> >;
   }
   static void *newArray_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,pair<double,double> >[nElements] : new map<ECbmModuleId,pair<double,double> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR(void *p) {
      delete ((map<ECbmModuleId,pair<double,double> >*)p);
   }
   static void deleteArray_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR(void *p) {
      delete [] ((map<ECbmModuleId,pair<double,double> >*)p);
   }
   static void destruct_maplEECbmModuleIdcOpairlEdoublecOdoublegRsPgR(void *p) {
      typedef map<ECbmModuleId,pair<double,double> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<ECbmModuleId,pair<double,double> >

namespace {
  void TriggerDictionaryInitialization_libCbmRecoTasks_Impl() {
    static const char* headers[] = {
"CbmReco.h",
"CbmSourceTs.h",
"CbmTaskBuildEvents.h",
"CbmTaskDigiEventQa.h",
"CbmTaskEventsCloneInToOut.h",
"CbmTaskMakeRecoEvents.h",
"CbmTaskTriggerDigi.h",
"CbmTaskTofHitFinder.h",
"CbmTaskUnpack.h",
"CbmTaskUnpackXpu.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/tasks",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/tasks",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/algo",
"/home/heinemann/gpuunpacker/cbmroot/algo/data",
"/home/heinemann/gpuunpacker/cbmroot/algo/base",
"/home/heinemann/gpuunpacker/cbmroot/algo/evbuild",
"/home/heinemann/gpuunpacker/cbmroot/algo/global",
"/home/heinemann/gpuunpacker/cbmroot/algo/trigger",
"/home/heinemann/gpuunpacker/cbmroot/algo/evselector",
"/home/heinemann/gpuunpacker/cbmroot/algo/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/algo/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/algo/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/algo/detectors/bmon",
"/home/heinemann/gpuunpacker/cbmroot/algo/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/algo/detectors/trd2d",
"/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/logging",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/external/yaml-cpp/include",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/tasks/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmRecoTasks dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmReco.h")))  CbmRecoConfig;
class __attribute__((annotate("$clingAutoload$CbmReco.h")))  CbmReco;
class __attribute__((annotate("$clingAutoload$CbmSourceTs.h")))  CbmSourceTs;
class __attribute__((annotate("$clingAutoload$CbmTaskBuildEvents.h")))  CbmTaskBuildEvents;
class __attribute__((annotate("$clingAutoload$CbmTaskDigiEventQa.h")))  CbmTaskDigiEventQa;
class __attribute__((annotate("$clingAutoload$CbmTaskEventsCloneInToOut.h")))  CbmTaskEventsCloneInToOut;
class __attribute__((annotate("$clingAutoload$CbmTaskMakeRecoEvents.h")))  CbmTaskMakeRecoEvents;
class __attribute__((annotate("$clingAutoload$CbmTaskTriggerDigi.h")))  CbmTaskTriggerDigi;
class __attribute__((annotate("$clingAutoload$CbmTaskTofHitFinder.h")))  CbmTaskTofHitFinder;
class __attribute__((annotate("$clingAutoload$CbmTaskUnpack.h")))  CbmTaskUnpack;
class __attribute__((annotate("$clingAutoload$CbmTaskUnpackXpu.h")))  CbmTaskUnpackXpu;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmRecoTasks dictionary payload"

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
#ifndef NO_ROOT
  #define NO_ROOT 1
#endif
#ifndef NO_ROOT
  #define NO_ROOT 1
#endif
#ifndef BOOST_LOG_DYN_LINK
  #define BOOST_LOG_DYN_LINK 1
#endif
#ifndef BOOST_LOG_USE_NATIVE_SYSLOG
  #define BOOST_LOG_USE_NATIVE_SYSLOG 1
#endif
#ifndef BOOST_ERROR_CODE_HEADER_ONLY
  #define BOOST_ERROR_CODE_HEADER_ONLY 1
#endif
#ifndef BOOST_PROGRAM_OPTIONS_NO_LIB
  #define BOOST_PROGRAM_OPTIONS_NO_LIB 1
#endif
#ifndef BOOST_PROGRAM_OPTIONS_DYN_LINK
  #define BOOST_PROGRAM_OPTIONS_DYN_LINK 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "CbmReco.h"
#include "CbmSourceTs.h"
#include "CbmTaskBuildEvents.h"
#include "CbmTaskDigiEventQa.h"
#include "CbmTaskEventsCloneInToOut.h"
#include "CbmTaskMakeRecoEvents.h"
#include "CbmTaskTriggerDigi.h"
#include "CbmTaskTofHitFinder.h"
#include "CbmTaskUnpack.h"
#include "CbmTaskUnpackXpu.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmReco", payloadCode, "@",
"CbmRecoConfig", payloadCode, "@",
"CbmSourceTs", payloadCode, "@",
"CbmTaskBuildEvents", payloadCode, "@",
"CbmTaskDigiEventQa", payloadCode, "@",
"CbmTaskEventsCloneInToOut", payloadCode, "@",
"CbmTaskMakeRecoEvents", payloadCode, "@",
"CbmTaskTofHitFinder", payloadCode, "@",
"CbmTaskTriggerDigi", payloadCode, "@",
"CbmTaskUnpack", payloadCode, "@",
"CbmTaskUnpackXpu", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmRecoTasks",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmRecoTasks_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmRecoTasks_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmRecoTasks() {
  TriggerDictionaryInitialization_libCbmRecoTasks_Impl();
}
