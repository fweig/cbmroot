// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmGlobal
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
#include "CbmFindGlobalTracks.h"
#include "CbmFindPrimaryVertex.h"
#include "CbmFindPrimaryVertexEvents.h"
#include "CbmFitGlobalTracks.h"
#include "CbmFitGlobalTracksQa.h"
#include "CbmGlobalTrackFitterIdeal.h"
#include "CbmPVFinderIdeal.h"
#include "CbmTrackMergerIdeal.h"
#include "CbmRecoTzero.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmFindGlobalTracks(void *p = nullptr);
   static void *newArray_CbmFindGlobalTracks(Long_t size, void *p);
   static void delete_CbmFindGlobalTracks(void *p);
   static void deleteArray_CbmFindGlobalTracks(void *p);
   static void destruct_CbmFindGlobalTracks(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFindGlobalTracks*)
   {
      ::CbmFindGlobalTracks *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFindGlobalTracks >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFindGlobalTracks", ::CbmFindGlobalTracks::Class_Version(), "CbmFindGlobalTracks.h", 33,
                  typeid(::CbmFindGlobalTracks), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFindGlobalTracks::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFindGlobalTracks) );
      instance.SetNew(&new_CbmFindGlobalTracks);
      instance.SetNewArray(&newArray_CbmFindGlobalTracks);
      instance.SetDelete(&delete_CbmFindGlobalTracks);
      instance.SetDeleteArray(&deleteArray_CbmFindGlobalTracks);
      instance.SetDestructor(&destruct_CbmFindGlobalTracks);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFindGlobalTracks*)
   {
      return GenerateInitInstanceLocal((::CbmFindGlobalTracks*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFindGlobalTracks*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmFindPrimaryVertex(void *p = nullptr);
   static void *newArray_CbmFindPrimaryVertex(Long_t size, void *p);
   static void delete_CbmFindPrimaryVertex(void *p);
   static void deleteArray_CbmFindPrimaryVertex(void *p);
   static void destruct_CbmFindPrimaryVertex(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFindPrimaryVertex*)
   {
      ::CbmFindPrimaryVertex *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFindPrimaryVertex >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFindPrimaryVertex", ::CbmFindPrimaryVertex::Class_Version(), "CbmFindPrimaryVertex.h", 37,
                  typeid(::CbmFindPrimaryVertex), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFindPrimaryVertex::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFindPrimaryVertex) );
      instance.SetNew(&new_CbmFindPrimaryVertex);
      instance.SetNewArray(&newArray_CbmFindPrimaryVertex);
      instance.SetDelete(&delete_CbmFindPrimaryVertex);
      instance.SetDeleteArray(&deleteArray_CbmFindPrimaryVertex);
      instance.SetDestructor(&destruct_CbmFindPrimaryVertex);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFindPrimaryVertex*)
   {
      return GenerateInitInstanceLocal((::CbmFindPrimaryVertex*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFindPrimaryVertex*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmFindPrimaryVertexEvents(void *p = nullptr);
   static void *newArray_CbmFindPrimaryVertexEvents(Long_t size, void *p);
   static void delete_CbmFindPrimaryVertexEvents(void *p);
   static void deleteArray_CbmFindPrimaryVertexEvents(void *p);
   static void destruct_CbmFindPrimaryVertexEvents(void *p);
   static void streamer_CbmFindPrimaryVertexEvents(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFindPrimaryVertexEvents*)
   {
      ::CbmFindPrimaryVertexEvents *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFindPrimaryVertexEvents >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFindPrimaryVertexEvents", ::CbmFindPrimaryVertexEvents::Class_Version(), "CbmFindPrimaryVertexEvents.h", 34,
                  typeid(::CbmFindPrimaryVertexEvents), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFindPrimaryVertexEvents::Dictionary, isa_proxy, 16,
                  sizeof(::CbmFindPrimaryVertexEvents) );
      instance.SetNew(&new_CbmFindPrimaryVertexEvents);
      instance.SetNewArray(&newArray_CbmFindPrimaryVertexEvents);
      instance.SetDelete(&delete_CbmFindPrimaryVertexEvents);
      instance.SetDeleteArray(&deleteArray_CbmFindPrimaryVertexEvents);
      instance.SetDestructor(&destruct_CbmFindPrimaryVertexEvents);
      instance.SetStreamerFunc(&streamer_CbmFindPrimaryVertexEvents);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFindPrimaryVertexEvents*)
   {
      return GenerateInitInstanceLocal((::CbmFindPrimaryVertexEvents*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFindPrimaryVertexEvents*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmFitGlobalTracks(void *p = nullptr);
   static void *newArray_CbmFitGlobalTracks(Long_t size, void *p);
   static void delete_CbmFitGlobalTracks(void *p);
   static void deleteArray_CbmFitGlobalTracks(void *p);
   static void destruct_CbmFitGlobalTracks(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFitGlobalTracks*)
   {
      ::CbmFitGlobalTracks *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFitGlobalTracks >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFitGlobalTracks", ::CbmFitGlobalTracks::Class_Version(), "CbmFitGlobalTracks.h", 18,
                  typeid(::CbmFitGlobalTracks), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFitGlobalTracks::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFitGlobalTracks) );
      instance.SetNew(&new_CbmFitGlobalTracks);
      instance.SetNewArray(&newArray_CbmFitGlobalTracks);
      instance.SetDelete(&delete_CbmFitGlobalTracks);
      instance.SetDeleteArray(&deleteArray_CbmFitGlobalTracks);
      instance.SetDestructor(&destruct_CbmFitGlobalTracks);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFitGlobalTracks*)
   {
      return GenerateInitInstanceLocal((::CbmFitGlobalTracks*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFitGlobalTracks*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmFitGlobalTracksQa(void *p = nullptr);
   static void *newArray_CbmFitGlobalTracksQa(Long_t size, void *p);
   static void delete_CbmFitGlobalTracksQa(void *p);
   static void deleteArray_CbmFitGlobalTracksQa(void *p);
   static void destruct_CbmFitGlobalTracksQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFitGlobalTracksQa*)
   {
      ::CbmFitGlobalTracksQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFitGlobalTracksQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFitGlobalTracksQa", ::CbmFitGlobalTracksQa::Class_Version(), "CbmFitGlobalTracksQa.h", 18,
                  typeid(::CbmFitGlobalTracksQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFitGlobalTracksQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFitGlobalTracksQa) );
      instance.SetNew(&new_CbmFitGlobalTracksQa);
      instance.SetNewArray(&newArray_CbmFitGlobalTracksQa);
      instance.SetDelete(&delete_CbmFitGlobalTracksQa);
      instance.SetDeleteArray(&deleteArray_CbmFitGlobalTracksQa);
      instance.SetDestructor(&destruct_CbmFitGlobalTracksQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFitGlobalTracksQa*)
   {
      return GenerateInitInstanceLocal((::CbmFitGlobalTracksQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFitGlobalTracksQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmGlobalTrackFitterIdeal(void *p = nullptr);
   static void *newArray_CbmGlobalTrackFitterIdeal(Long_t size, void *p);
   static void delete_CbmGlobalTrackFitterIdeal(void *p);
   static void deleteArray_CbmGlobalTrackFitterIdeal(void *p);
   static void destruct_CbmGlobalTrackFitterIdeal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGlobalTrackFitterIdeal*)
   {
      ::CbmGlobalTrackFitterIdeal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGlobalTrackFitterIdeal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGlobalTrackFitterIdeal", ::CbmGlobalTrackFitterIdeal::Class_Version(), "CbmGlobalTrackFitterIdeal.h", 17,
                  typeid(::CbmGlobalTrackFitterIdeal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGlobalTrackFitterIdeal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGlobalTrackFitterIdeal) );
      instance.SetNew(&new_CbmGlobalTrackFitterIdeal);
      instance.SetNewArray(&newArray_CbmGlobalTrackFitterIdeal);
      instance.SetDelete(&delete_CbmGlobalTrackFitterIdeal);
      instance.SetDeleteArray(&deleteArray_CbmGlobalTrackFitterIdeal);
      instance.SetDestructor(&destruct_CbmGlobalTrackFitterIdeal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGlobalTrackFitterIdeal*)
   {
      return GenerateInitInstanceLocal((::CbmGlobalTrackFitterIdeal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGlobalTrackFitterIdeal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPVFinderIdeal(void *p = nullptr);
   static void *newArray_CbmPVFinderIdeal(Long_t size, void *p);
   static void delete_CbmPVFinderIdeal(void *p);
   static void deleteArray_CbmPVFinderIdeal(void *p);
   static void destruct_CbmPVFinderIdeal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPVFinderIdeal*)
   {
      ::CbmPVFinderIdeal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPVFinderIdeal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPVFinderIdeal", ::CbmPVFinderIdeal::Class_Version(), "CbmPVFinderIdeal.h", 30,
                  typeid(::CbmPVFinderIdeal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPVFinderIdeal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPVFinderIdeal) );
      instance.SetNew(&new_CbmPVFinderIdeal);
      instance.SetNewArray(&newArray_CbmPVFinderIdeal);
      instance.SetDelete(&delete_CbmPVFinderIdeal);
      instance.SetDeleteArray(&deleteArray_CbmPVFinderIdeal);
      instance.SetDestructor(&destruct_CbmPVFinderIdeal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPVFinderIdeal*)
   {
      return GenerateInitInstanceLocal((::CbmPVFinderIdeal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPVFinderIdeal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrackMergerIdeal(void *p = nullptr);
   static void *newArray_CbmTrackMergerIdeal(Long_t size, void *p);
   static void delete_CbmTrackMergerIdeal(void *p);
   static void deleteArray_CbmTrackMergerIdeal(void *p);
   static void destruct_CbmTrackMergerIdeal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrackMergerIdeal*)
   {
      ::CbmTrackMergerIdeal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrackMergerIdeal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrackMergerIdeal", ::CbmTrackMergerIdeal::Class_Version(), "CbmTrackMergerIdeal.h", 29,
                  typeid(::CbmTrackMergerIdeal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrackMergerIdeal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrackMergerIdeal) );
      instance.SetNew(&new_CbmTrackMergerIdeal);
      instance.SetNewArray(&newArray_CbmTrackMergerIdeal);
      instance.SetDelete(&delete_CbmTrackMergerIdeal);
      instance.SetDeleteArray(&deleteArray_CbmTrackMergerIdeal);
      instance.SetDestructor(&destruct_CbmTrackMergerIdeal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrackMergerIdeal*)
   {
      return GenerateInitInstanceLocal((::CbmTrackMergerIdeal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrackMergerIdeal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRecoTzero(void *p = nullptr);
   static void *newArray_CbmRecoTzero(Long_t size, void *p);
   static void delete_CbmRecoTzero(void *p);
   static void deleteArray_CbmRecoTzero(void *p);
   static void destruct_CbmRecoTzero(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRecoTzero*)
   {
      ::CbmRecoTzero *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRecoTzero >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRecoTzero", ::CbmRecoTzero::Class_Version(), "CbmRecoTzero.h", 51,
                  typeid(::CbmRecoTzero), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRecoTzero::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRecoTzero) );
      instance.SetNew(&new_CbmRecoTzero);
      instance.SetNewArray(&newArray_CbmRecoTzero);
      instance.SetDelete(&delete_CbmRecoTzero);
      instance.SetDeleteArray(&deleteArray_CbmRecoTzero);
      instance.SetDestructor(&destruct_CbmRecoTzero);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRecoTzero*)
   {
      return GenerateInitInstanceLocal((::CbmRecoTzero*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRecoTzero*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmFindGlobalTracks::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFindGlobalTracks::Class_Name()
{
   return "CbmFindGlobalTracks";
}

//______________________________________________________________________________
const char *CbmFindGlobalTracks::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFindGlobalTracks*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFindGlobalTracks::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFindGlobalTracks*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFindGlobalTracks::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFindGlobalTracks*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFindGlobalTracks::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFindGlobalTracks*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmFindPrimaryVertex::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFindPrimaryVertex::Class_Name()
{
   return "CbmFindPrimaryVertex";
}

//______________________________________________________________________________
const char *CbmFindPrimaryVertex::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFindPrimaryVertex*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFindPrimaryVertex::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFindPrimaryVertex*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFindPrimaryVertex::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFindPrimaryVertex*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFindPrimaryVertex::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFindPrimaryVertex*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmFindPrimaryVertexEvents::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFindPrimaryVertexEvents::Class_Name()
{
   return "CbmFindPrimaryVertexEvents";
}

//______________________________________________________________________________
const char *CbmFindPrimaryVertexEvents::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFindPrimaryVertexEvents*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFindPrimaryVertexEvents::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFindPrimaryVertexEvents*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFindPrimaryVertexEvents::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFindPrimaryVertexEvents*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFindPrimaryVertexEvents::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFindPrimaryVertexEvents*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmFitGlobalTracks::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFitGlobalTracks::Class_Name()
{
   return "CbmFitGlobalTracks";
}

//______________________________________________________________________________
const char *CbmFitGlobalTracks::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFitGlobalTracks*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFitGlobalTracks::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFitGlobalTracks*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFitGlobalTracks::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFitGlobalTracks*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFitGlobalTracks::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFitGlobalTracks*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmFitGlobalTracksQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFitGlobalTracksQa::Class_Name()
{
   return "CbmFitGlobalTracksQa";
}

//______________________________________________________________________________
const char *CbmFitGlobalTracksQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFitGlobalTracksQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFitGlobalTracksQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFitGlobalTracksQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFitGlobalTracksQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFitGlobalTracksQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFitGlobalTracksQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFitGlobalTracksQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGlobalTrackFitterIdeal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGlobalTrackFitterIdeal::Class_Name()
{
   return "CbmGlobalTrackFitterIdeal";
}

//______________________________________________________________________________
const char *CbmGlobalTrackFitterIdeal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackFitterIdeal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGlobalTrackFitterIdeal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackFitterIdeal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGlobalTrackFitterIdeal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackFitterIdeal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGlobalTrackFitterIdeal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackFitterIdeal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPVFinderIdeal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPVFinderIdeal::Class_Name()
{
   return "CbmPVFinderIdeal";
}

//______________________________________________________________________________
const char *CbmPVFinderIdeal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPVFinderIdeal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPVFinderIdeal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPVFinderIdeal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPVFinderIdeal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPVFinderIdeal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPVFinderIdeal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPVFinderIdeal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrackMergerIdeal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrackMergerIdeal::Class_Name()
{
   return "CbmTrackMergerIdeal";
}

//______________________________________________________________________________
const char *CbmTrackMergerIdeal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMergerIdeal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrackMergerIdeal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMergerIdeal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrackMergerIdeal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMergerIdeal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrackMergerIdeal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMergerIdeal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRecoTzero::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRecoTzero::Class_Name()
{
   return "CbmRecoTzero";
}

//______________________________________________________________________________
const char *CbmRecoTzero::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoTzero*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRecoTzero::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoTzero*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRecoTzero::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoTzero*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRecoTzero::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoTzero*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmFindGlobalTracks::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFindGlobalTracks.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFindGlobalTracks::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFindGlobalTracks::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFindGlobalTracks(void *p) {
      return  p ? new(p) ::CbmFindGlobalTracks : new ::CbmFindGlobalTracks;
   }
   static void *newArray_CbmFindGlobalTracks(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFindGlobalTracks[nElements] : new ::CbmFindGlobalTracks[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFindGlobalTracks(void *p) {
      delete ((::CbmFindGlobalTracks*)p);
   }
   static void deleteArray_CbmFindGlobalTracks(void *p) {
      delete [] ((::CbmFindGlobalTracks*)p);
   }
   static void destruct_CbmFindGlobalTracks(void *p) {
      typedef ::CbmFindGlobalTracks current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFindGlobalTracks

//______________________________________________________________________________
void CbmFindPrimaryVertex::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFindPrimaryVertex.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFindPrimaryVertex::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFindPrimaryVertex::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFindPrimaryVertex(void *p) {
      return  p ? new(p) ::CbmFindPrimaryVertex : new ::CbmFindPrimaryVertex;
   }
   static void *newArray_CbmFindPrimaryVertex(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFindPrimaryVertex[nElements] : new ::CbmFindPrimaryVertex[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFindPrimaryVertex(void *p) {
      delete ((::CbmFindPrimaryVertex*)p);
   }
   static void deleteArray_CbmFindPrimaryVertex(void *p) {
      delete [] ((::CbmFindPrimaryVertex*)p);
   }
   static void destruct_CbmFindPrimaryVertex(void *p) {
      typedef ::CbmFindPrimaryVertex current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFindPrimaryVertex

//______________________________________________________________________________
void CbmFindPrimaryVertexEvents::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFindPrimaryVertexEvents.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      fTimer.Streamer(R__b);
      R__b >> fFinder;
      R__b >> fPrimVert;
      R__b >> fNofEvents;
      R__b >> fTimeTot;
      R__b.CheckByteCount(R__s, R__c, CbmFindPrimaryVertexEvents::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmFindPrimaryVertexEvents::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      fTimer.Streamer(R__b);
      R__b << fFinder;
      R__b << fPrimVert;
      R__b << fNofEvents;
      R__b << fTimeTot;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFindPrimaryVertexEvents(void *p) {
      return  p ? new(p) ::CbmFindPrimaryVertexEvents : new ::CbmFindPrimaryVertexEvents;
   }
   static void *newArray_CbmFindPrimaryVertexEvents(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFindPrimaryVertexEvents[nElements] : new ::CbmFindPrimaryVertexEvents[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFindPrimaryVertexEvents(void *p) {
      delete ((::CbmFindPrimaryVertexEvents*)p);
   }
   static void deleteArray_CbmFindPrimaryVertexEvents(void *p) {
      delete [] ((::CbmFindPrimaryVertexEvents*)p);
   }
   static void destruct_CbmFindPrimaryVertexEvents(void *p) {
      typedef ::CbmFindPrimaryVertexEvents current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmFindPrimaryVertexEvents(TBuffer &buf, void *obj) {
      ((::CbmFindPrimaryVertexEvents*)obj)->::CbmFindPrimaryVertexEvents::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmFindPrimaryVertexEvents

//______________________________________________________________________________
void CbmFitGlobalTracks::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFitGlobalTracks.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFitGlobalTracks::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFitGlobalTracks::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFitGlobalTracks(void *p) {
      return  p ? new(p) ::CbmFitGlobalTracks : new ::CbmFitGlobalTracks;
   }
   static void *newArray_CbmFitGlobalTracks(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFitGlobalTracks[nElements] : new ::CbmFitGlobalTracks[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFitGlobalTracks(void *p) {
      delete ((::CbmFitGlobalTracks*)p);
   }
   static void deleteArray_CbmFitGlobalTracks(void *p) {
      delete [] ((::CbmFitGlobalTracks*)p);
   }
   static void destruct_CbmFitGlobalTracks(void *p) {
      typedef ::CbmFitGlobalTracks current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFitGlobalTracks

//______________________________________________________________________________
void CbmFitGlobalTracksQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFitGlobalTracksQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFitGlobalTracksQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFitGlobalTracksQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFitGlobalTracksQa(void *p) {
      return  p ? new(p) ::CbmFitGlobalTracksQa : new ::CbmFitGlobalTracksQa;
   }
   static void *newArray_CbmFitGlobalTracksQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFitGlobalTracksQa[nElements] : new ::CbmFitGlobalTracksQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFitGlobalTracksQa(void *p) {
      delete ((::CbmFitGlobalTracksQa*)p);
   }
   static void deleteArray_CbmFitGlobalTracksQa(void *p) {
      delete [] ((::CbmFitGlobalTracksQa*)p);
   }
   static void destruct_CbmFitGlobalTracksQa(void *p) {
      typedef ::CbmFitGlobalTracksQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFitGlobalTracksQa

//______________________________________________________________________________
void CbmGlobalTrackFitterIdeal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGlobalTrackFitterIdeal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGlobalTrackFitterIdeal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGlobalTrackFitterIdeal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGlobalTrackFitterIdeal(void *p) {
      return  p ? new(p) ::CbmGlobalTrackFitterIdeal : new ::CbmGlobalTrackFitterIdeal;
   }
   static void *newArray_CbmGlobalTrackFitterIdeal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGlobalTrackFitterIdeal[nElements] : new ::CbmGlobalTrackFitterIdeal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGlobalTrackFitterIdeal(void *p) {
      delete ((::CbmGlobalTrackFitterIdeal*)p);
   }
   static void deleteArray_CbmGlobalTrackFitterIdeal(void *p) {
      delete [] ((::CbmGlobalTrackFitterIdeal*)p);
   }
   static void destruct_CbmGlobalTrackFitterIdeal(void *p) {
      typedef ::CbmGlobalTrackFitterIdeal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGlobalTrackFitterIdeal

//______________________________________________________________________________
void CbmPVFinderIdeal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPVFinderIdeal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPVFinderIdeal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPVFinderIdeal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPVFinderIdeal(void *p) {
      return  p ? new(p) ::CbmPVFinderIdeal : new ::CbmPVFinderIdeal;
   }
   static void *newArray_CbmPVFinderIdeal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPVFinderIdeal[nElements] : new ::CbmPVFinderIdeal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPVFinderIdeal(void *p) {
      delete ((::CbmPVFinderIdeal*)p);
   }
   static void deleteArray_CbmPVFinderIdeal(void *p) {
      delete [] ((::CbmPVFinderIdeal*)p);
   }
   static void destruct_CbmPVFinderIdeal(void *p) {
      typedef ::CbmPVFinderIdeal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPVFinderIdeal

//______________________________________________________________________________
void CbmTrackMergerIdeal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrackMergerIdeal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrackMergerIdeal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrackMergerIdeal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrackMergerIdeal(void *p) {
      return  p ? new(p) ::CbmTrackMergerIdeal : new ::CbmTrackMergerIdeal;
   }
   static void *newArray_CbmTrackMergerIdeal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrackMergerIdeal[nElements] : new ::CbmTrackMergerIdeal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrackMergerIdeal(void *p) {
      delete ((::CbmTrackMergerIdeal*)p);
   }
   static void deleteArray_CbmTrackMergerIdeal(void *p) {
      delete [] ((::CbmTrackMergerIdeal*)p);
   }
   static void destruct_CbmTrackMergerIdeal(void *p) {
      typedef ::CbmTrackMergerIdeal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrackMergerIdeal

//______________________________________________________________________________
void CbmRecoTzero::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRecoTzero.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRecoTzero::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRecoTzero::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRecoTzero(void *p) {
      return  p ? new(p) ::CbmRecoTzero : new ::CbmRecoTzero;
   }
   static void *newArray_CbmRecoTzero(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRecoTzero[nElements] : new ::CbmRecoTzero[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRecoTzero(void *p) {
      delete ((::CbmRecoTzero*)p);
   }
   static void deleteArray_CbmRecoTzero(void *p) {
      delete [] ((::CbmRecoTzero*)p);
   }
   static void destruct_CbmRecoTzero(void *p) {
      typedef ::CbmRecoTzero current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRecoTzero

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

namespace {
  void TriggerDictionaryInitialization_libCbmGlobal_Impl() {
    static const char* headers[] = {
"CbmFindGlobalTracks.h",
"CbmFindPrimaryVertex.h",
"CbmFindPrimaryVertexEvents.h",
"CbmFitGlobalTracks.h",
"CbmFitGlobalTracksQa.h",
"CbmGlobalTrackFitterIdeal.h",
"CbmPVFinderIdeal.h",
"CbmTrackMergerIdeal.h",
"CbmRecoTzero.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/global",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/global",
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
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/global/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmGlobal dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmFindGlobalTracks.h")))  CbmFindGlobalTracks;
class __attribute__((annotate("$clingAutoload$CbmFindPrimaryVertex.h")))  CbmFindPrimaryVertex;
class __attribute__((annotate("$clingAutoload$CbmFindPrimaryVertexEvents.h")))  CbmFindPrimaryVertexEvents;
class __attribute__((annotate("$clingAutoload$CbmFitGlobalTracks.h")))  CbmFitGlobalTracks;
class __attribute__((annotate("$clingAutoload$CbmFitGlobalTracksQa.h")))  CbmFitGlobalTracksQa;
class __attribute__((annotate("$clingAutoload$CbmGlobalTrackFitterIdeal.h")))  CbmGlobalTrackFitterIdeal;
class __attribute__((annotate("$clingAutoload$CbmPVFinderIdeal.h")))  CbmPVFinderIdeal;
class __attribute__((annotate("$clingAutoload$CbmTrackMergerIdeal.h")))  CbmTrackMergerIdeal;
class __attribute__((annotate("$clingAutoload$CbmRecoTzero.h")))  CbmRecoTzero;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmGlobal dictionary payload"

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
#include "CbmFindGlobalTracks.h"
#include "CbmFindPrimaryVertex.h"
#include "CbmFindPrimaryVertexEvents.h"
#include "CbmFitGlobalTracks.h"
#include "CbmFitGlobalTracksQa.h"
#include "CbmGlobalTrackFitterIdeal.h"
#include "CbmPVFinderIdeal.h"
#include "CbmTrackMergerIdeal.h"
#include "CbmRecoTzero.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmFindGlobalTracks", payloadCode, "@",
"CbmFindPrimaryVertex", payloadCode, "@",
"CbmFindPrimaryVertexEvents", payloadCode, "@",
"CbmFitGlobalTracks", payloadCode, "@",
"CbmFitGlobalTracksQa", payloadCode, "@",
"CbmGlobalTrackFitterIdeal", payloadCode, "@",
"CbmPVFinderIdeal", payloadCode, "@",
"CbmRecoTzero", payloadCode, "@",
"CbmTrackMergerIdeal", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmGlobal",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmGlobal_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmGlobal_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmGlobal() {
  TriggerDictionaryInitialization_libCbmGlobal_Impl();
}
