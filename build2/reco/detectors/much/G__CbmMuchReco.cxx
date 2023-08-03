// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmMuchReco
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
#include "CbmMuchFindHitsGem.h"
#include "CbmMuchHitProducerIdeal.h"
#include "CbmMuchFindTracks.h"
#include "CbmMuchMatchTracks.h"
#include "CbmMuchTrackFinderIdeal.h"
#include "CbmMuchHitFinderQa.h"
#include "CbmMuchUnpackAlgo.h"
#include "CbmMuchUnpackMonitor.h"
#include "CbmMuchUnpackConfig.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_CbmMuchFindHitsGem(void *p);
   static void deleteArray_CbmMuchFindHitsGem(void *p);
   static void destruct_CbmMuchFindHitsGem(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchFindHitsGem*)
   {
      ::CbmMuchFindHitsGem *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchFindHitsGem >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchFindHitsGem", ::CbmMuchFindHitsGem::Class_Version(), "CbmMuchFindHitsGem.h", 53,
                  typeid(::CbmMuchFindHitsGem), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchFindHitsGem::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchFindHitsGem) );
      instance.SetDelete(&delete_CbmMuchFindHitsGem);
      instance.SetDeleteArray(&deleteArray_CbmMuchFindHitsGem);
      instance.SetDestructor(&destruct_CbmMuchFindHitsGem);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchFindHitsGem*)
   {
      return GenerateInitInstanceLocal((::CbmMuchFindHitsGem*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchFindHitsGem*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmMuchHitProducerIdeal(void *p);
   static void deleteArray_CbmMuchHitProducerIdeal(void *p);
   static void destruct_CbmMuchHitProducerIdeal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchHitProducerIdeal*)
   {
      ::CbmMuchHitProducerIdeal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchHitProducerIdeal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchHitProducerIdeal", ::CbmMuchHitProducerIdeal::Class_Version(), "CbmMuchHitProducerIdeal.h", 22,
                  typeid(::CbmMuchHitProducerIdeal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchHitProducerIdeal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchHitProducerIdeal) );
      instance.SetDelete(&delete_CbmMuchHitProducerIdeal);
      instance.SetDeleteArray(&deleteArray_CbmMuchHitProducerIdeal);
      instance.SetDestructor(&destruct_CbmMuchHitProducerIdeal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchHitProducerIdeal*)
   {
      return GenerateInitInstanceLocal((::CbmMuchHitProducerIdeal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchHitProducerIdeal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchFindTracks(void *p = nullptr);
   static void *newArray_CbmMuchFindTracks(Long_t size, void *p);
   static void delete_CbmMuchFindTracks(void *p);
   static void deleteArray_CbmMuchFindTracks(void *p);
   static void destruct_CbmMuchFindTracks(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchFindTracks*)
   {
      ::CbmMuchFindTracks *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchFindTracks >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchFindTracks", ::CbmMuchFindTracks::Class_Version(), "CbmMuchFindTracks.h", 24,
                  typeid(::CbmMuchFindTracks), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchFindTracks::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchFindTracks) );
      instance.SetNew(&new_CbmMuchFindTracks);
      instance.SetNewArray(&newArray_CbmMuchFindTracks);
      instance.SetDelete(&delete_CbmMuchFindTracks);
      instance.SetDeleteArray(&deleteArray_CbmMuchFindTracks);
      instance.SetDestructor(&destruct_CbmMuchFindTracks);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchFindTracks*)
   {
      return GenerateInitInstanceLocal((::CbmMuchFindTracks*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchFindTracks*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchMatchTracks(void *p = nullptr);
   static void *newArray_CbmMuchMatchTracks(Long_t size, void *p);
   static void delete_CbmMuchMatchTracks(void *p);
   static void deleteArray_CbmMuchMatchTracks(void *p);
   static void destruct_CbmMuchMatchTracks(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchMatchTracks*)
   {
      ::CbmMuchMatchTracks *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchMatchTracks >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchMatchTracks", ::CbmMuchMatchTracks::Class_Version(), "CbmMuchMatchTracks.h", 25,
                  typeid(::CbmMuchMatchTracks), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchMatchTracks::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchMatchTracks) );
      instance.SetNew(&new_CbmMuchMatchTracks);
      instance.SetNewArray(&newArray_CbmMuchMatchTracks);
      instance.SetDelete(&delete_CbmMuchMatchTracks);
      instance.SetDeleteArray(&deleteArray_CbmMuchMatchTracks);
      instance.SetDestructor(&destruct_CbmMuchMatchTracks);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchMatchTracks*)
   {
      return GenerateInitInstanceLocal((::CbmMuchMatchTracks*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchMatchTracks*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchTrackFinderIdeal(void *p = nullptr);
   static void *newArray_CbmMuchTrackFinderIdeal(Long_t size, void *p);
   static void delete_CbmMuchTrackFinderIdeal(void *p);
   static void deleteArray_CbmMuchTrackFinderIdeal(void *p);
   static void destruct_CbmMuchTrackFinderIdeal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchTrackFinderIdeal*)
   {
      ::CbmMuchTrackFinderIdeal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchTrackFinderIdeal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchTrackFinderIdeal", ::CbmMuchTrackFinderIdeal::Class_Version(), "CbmMuchTrackFinderIdeal.h", 26,
                  typeid(::CbmMuchTrackFinderIdeal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchTrackFinderIdeal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchTrackFinderIdeal) );
      instance.SetNew(&new_CbmMuchTrackFinderIdeal);
      instance.SetNewArray(&newArray_CbmMuchTrackFinderIdeal);
      instance.SetDelete(&delete_CbmMuchTrackFinderIdeal);
      instance.SetDeleteArray(&deleteArray_CbmMuchTrackFinderIdeal);
      instance.SetDestructor(&destruct_CbmMuchTrackFinderIdeal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchTrackFinderIdeal*)
   {
      return GenerateInitInstanceLocal((::CbmMuchTrackFinderIdeal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchTrackFinderIdeal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchHitFinderQa(void *p = nullptr);
   static void *newArray_CbmMuchHitFinderQa(Long_t size, void *p);
   static void delete_CbmMuchHitFinderQa(void *p);
   static void deleteArray_CbmMuchHitFinderQa(void *p);
   static void destruct_CbmMuchHitFinderQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchHitFinderQa*)
   {
      ::CbmMuchHitFinderQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchHitFinderQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchHitFinderQa", ::CbmMuchHitFinderQa::Class_Version(), "CbmMuchHitFinderQa.h", 38,
                  typeid(::CbmMuchHitFinderQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchHitFinderQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchHitFinderQa) );
      instance.SetNew(&new_CbmMuchHitFinderQa);
      instance.SetNewArray(&newArray_CbmMuchHitFinderQa);
      instance.SetDelete(&delete_CbmMuchHitFinderQa);
      instance.SetDeleteArray(&deleteArray_CbmMuchHitFinderQa);
      instance.SetDestructor(&destruct_CbmMuchHitFinderQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchHitFinderQa*)
   {
      return GenerateInitInstanceLocal((::CbmMuchHitFinderQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchHitFinderQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchUnpackMonitor(void *p = nullptr);
   static void *newArray_CbmMuchUnpackMonitor(Long_t size, void *p);
   static void delete_CbmMuchUnpackMonitor(void *p);
   static void deleteArray_CbmMuchUnpackMonitor(void *p);
   static void destruct_CbmMuchUnpackMonitor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchUnpackMonitor*)
   {
      ::CbmMuchUnpackMonitor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchUnpackMonitor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchUnpackMonitor", ::CbmMuchUnpackMonitor::Class_Version(), "CbmMuchUnpackMonitor.h", 22,
                  typeid(::CbmMuchUnpackMonitor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchUnpackMonitor::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchUnpackMonitor) );
      instance.SetNew(&new_CbmMuchUnpackMonitor);
      instance.SetNewArray(&newArray_CbmMuchUnpackMonitor);
      instance.SetDelete(&delete_CbmMuchUnpackMonitor);
      instance.SetDeleteArray(&deleteArray_CbmMuchUnpackMonitor);
      instance.SetDestructor(&destruct_CbmMuchUnpackMonitor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchUnpackMonitor*)
   {
      return GenerateInitInstanceLocal((::CbmMuchUnpackMonitor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchUnpackMonitor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchUnpackAlgo(void *p = nullptr);
   static void *newArray_CbmMuchUnpackAlgo(Long_t size, void *p);
   static void delete_CbmMuchUnpackAlgo(void *p);
   static void deleteArray_CbmMuchUnpackAlgo(void *p);
   static void destruct_CbmMuchUnpackAlgo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchUnpackAlgo*)
   {
      ::CbmMuchUnpackAlgo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchUnpackAlgo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchUnpackAlgo", ::CbmMuchUnpackAlgo::Class_Version(), "CbmMuchUnpackAlgo.h", 40,
                  typeid(::CbmMuchUnpackAlgo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchUnpackAlgo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchUnpackAlgo) );
      instance.SetNew(&new_CbmMuchUnpackAlgo);
      instance.SetNewArray(&newArray_CbmMuchUnpackAlgo);
      instance.SetDelete(&delete_CbmMuchUnpackAlgo);
      instance.SetDeleteArray(&deleteArray_CbmMuchUnpackAlgo);
      instance.SetDestructor(&destruct_CbmMuchUnpackAlgo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchUnpackAlgo*)
   {
      return GenerateInitInstanceLocal((::CbmMuchUnpackAlgo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchUnpackAlgo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmMuchUnpackConfig(void *p);
   static void deleteArray_CbmMuchUnpackConfig(void *p);
   static void destruct_CbmMuchUnpackConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchUnpackConfig*)
   {
      ::CbmMuchUnpackConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchUnpackConfig >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchUnpackConfig", ::CbmMuchUnpackConfig::Class_Version(), "CbmMuchUnpackConfig.h", 32,
                  typeid(::CbmMuchUnpackConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchUnpackConfig::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchUnpackConfig) );
      instance.SetDelete(&delete_CbmMuchUnpackConfig);
      instance.SetDeleteArray(&deleteArray_CbmMuchUnpackConfig);
      instance.SetDestructor(&destruct_CbmMuchUnpackConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchUnpackConfig*)
   {
      return GenerateInitInstanceLocal((::CbmMuchUnpackConfig*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchUnpackConfig*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchFindHitsGem::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchFindHitsGem::Class_Name()
{
   return "CbmMuchFindHitsGem";
}

//______________________________________________________________________________
const char *CbmMuchFindHitsGem::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchFindHitsGem*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchFindHitsGem::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchFindHitsGem*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchFindHitsGem::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchFindHitsGem*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchFindHitsGem::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchFindHitsGem*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchHitProducerIdeal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchHitProducerIdeal::Class_Name()
{
   return "CbmMuchHitProducerIdeal";
}

//______________________________________________________________________________
const char *CbmMuchHitProducerIdeal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchHitProducerIdeal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchHitProducerIdeal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchHitProducerIdeal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchHitProducerIdeal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchHitProducerIdeal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchHitProducerIdeal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchHitProducerIdeal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchFindTracks::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchFindTracks::Class_Name()
{
   return "CbmMuchFindTracks";
}

//______________________________________________________________________________
const char *CbmMuchFindTracks::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchFindTracks*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchFindTracks::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchFindTracks*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchFindTracks::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchFindTracks*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchFindTracks::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchFindTracks*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchMatchTracks::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchMatchTracks::Class_Name()
{
   return "CbmMuchMatchTracks";
}

//______________________________________________________________________________
const char *CbmMuchMatchTracks::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchMatchTracks*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchMatchTracks::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchMatchTracks*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchMatchTracks::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchMatchTracks*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchMatchTracks::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchMatchTracks*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchTrackFinderIdeal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchTrackFinderIdeal::Class_Name()
{
   return "CbmMuchTrackFinderIdeal";
}

//______________________________________________________________________________
const char *CbmMuchTrackFinderIdeal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrackFinderIdeal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchTrackFinderIdeal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrackFinderIdeal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchTrackFinderIdeal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrackFinderIdeal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchTrackFinderIdeal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrackFinderIdeal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchHitFinderQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchHitFinderQa::Class_Name()
{
   return "CbmMuchHitFinderQa";
}

//______________________________________________________________________________
const char *CbmMuchHitFinderQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchHitFinderQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchHitFinderQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchHitFinderQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchHitFinderQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchHitFinderQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchHitFinderQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchHitFinderQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchUnpackMonitor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchUnpackMonitor::Class_Name()
{
   return "CbmMuchUnpackMonitor";
}

//______________________________________________________________________________
const char *CbmMuchUnpackMonitor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackMonitor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchUnpackMonitor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackMonitor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchUnpackMonitor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackMonitor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchUnpackMonitor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackMonitor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchUnpackAlgo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchUnpackAlgo::Class_Name()
{
   return "CbmMuchUnpackAlgo";
}

//______________________________________________________________________________
const char *CbmMuchUnpackAlgo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackAlgo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchUnpackAlgo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackAlgo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchUnpackAlgo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackAlgo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchUnpackAlgo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackAlgo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchUnpackConfig::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchUnpackConfig::Class_Name()
{
   return "CbmMuchUnpackConfig";
}

//______________________________________________________________________________
const char *CbmMuchUnpackConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackConfig*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchUnpackConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackConfig*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchUnpackConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackConfig*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchUnpackConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackConfig*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmMuchFindHitsGem::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchFindHitsGem.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchFindHitsGem::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchFindHitsGem::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmMuchFindHitsGem(void *p) {
      delete ((::CbmMuchFindHitsGem*)p);
   }
   static void deleteArray_CbmMuchFindHitsGem(void *p) {
      delete [] ((::CbmMuchFindHitsGem*)p);
   }
   static void destruct_CbmMuchFindHitsGem(void *p) {
      typedef ::CbmMuchFindHitsGem current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchFindHitsGem

//______________________________________________________________________________
void CbmMuchHitProducerIdeal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchHitProducerIdeal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchHitProducerIdeal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchHitProducerIdeal::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmMuchHitProducerIdeal(void *p) {
      delete ((::CbmMuchHitProducerIdeal*)p);
   }
   static void deleteArray_CbmMuchHitProducerIdeal(void *p) {
      delete [] ((::CbmMuchHitProducerIdeal*)p);
   }
   static void destruct_CbmMuchHitProducerIdeal(void *p) {
      typedef ::CbmMuchHitProducerIdeal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchHitProducerIdeal

//______________________________________________________________________________
void CbmMuchFindTracks::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchFindTracks.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchFindTracks::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchFindTracks::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchFindTracks(void *p) {
      return  p ? new(p) ::CbmMuchFindTracks : new ::CbmMuchFindTracks;
   }
   static void *newArray_CbmMuchFindTracks(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchFindTracks[nElements] : new ::CbmMuchFindTracks[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchFindTracks(void *p) {
      delete ((::CbmMuchFindTracks*)p);
   }
   static void deleteArray_CbmMuchFindTracks(void *p) {
      delete [] ((::CbmMuchFindTracks*)p);
   }
   static void destruct_CbmMuchFindTracks(void *p) {
      typedef ::CbmMuchFindTracks current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchFindTracks

//______________________________________________________________________________
void CbmMuchMatchTracks::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchMatchTracks.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchMatchTracks::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchMatchTracks::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchMatchTracks(void *p) {
      return  p ? new(p) ::CbmMuchMatchTracks : new ::CbmMuchMatchTracks;
   }
   static void *newArray_CbmMuchMatchTracks(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchMatchTracks[nElements] : new ::CbmMuchMatchTracks[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchMatchTracks(void *p) {
      delete ((::CbmMuchMatchTracks*)p);
   }
   static void deleteArray_CbmMuchMatchTracks(void *p) {
      delete [] ((::CbmMuchMatchTracks*)p);
   }
   static void destruct_CbmMuchMatchTracks(void *p) {
      typedef ::CbmMuchMatchTracks current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchMatchTracks

//______________________________________________________________________________
void CbmMuchTrackFinderIdeal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchTrackFinderIdeal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchTrackFinderIdeal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchTrackFinderIdeal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchTrackFinderIdeal(void *p) {
      return  p ? new(p) ::CbmMuchTrackFinderIdeal : new ::CbmMuchTrackFinderIdeal;
   }
   static void *newArray_CbmMuchTrackFinderIdeal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchTrackFinderIdeal[nElements] : new ::CbmMuchTrackFinderIdeal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchTrackFinderIdeal(void *p) {
      delete ((::CbmMuchTrackFinderIdeal*)p);
   }
   static void deleteArray_CbmMuchTrackFinderIdeal(void *p) {
      delete [] ((::CbmMuchTrackFinderIdeal*)p);
   }
   static void destruct_CbmMuchTrackFinderIdeal(void *p) {
      typedef ::CbmMuchTrackFinderIdeal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchTrackFinderIdeal

//______________________________________________________________________________
void CbmMuchHitFinderQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchHitFinderQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchHitFinderQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchHitFinderQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchHitFinderQa(void *p) {
      return  p ? new(p) ::CbmMuchHitFinderQa : new ::CbmMuchHitFinderQa;
   }
   static void *newArray_CbmMuchHitFinderQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchHitFinderQa[nElements] : new ::CbmMuchHitFinderQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchHitFinderQa(void *p) {
      delete ((::CbmMuchHitFinderQa*)p);
   }
   static void deleteArray_CbmMuchHitFinderQa(void *p) {
      delete [] ((::CbmMuchHitFinderQa*)p);
   }
   static void destruct_CbmMuchHitFinderQa(void *p) {
      typedef ::CbmMuchHitFinderQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchHitFinderQa

//______________________________________________________________________________
void CbmMuchUnpackMonitor::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchUnpackMonitor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchUnpackMonitor::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchUnpackMonitor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchUnpackMonitor(void *p) {
      return  p ? new(p) ::CbmMuchUnpackMonitor : new ::CbmMuchUnpackMonitor;
   }
   static void *newArray_CbmMuchUnpackMonitor(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchUnpackMonitor[nElements] : new ::CbmMuchUnpackMonitor[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchUnpackMonitor(void *p) {
      delete ((::CbmMuchUnpackMonitor*)p);
   }
   static void deleteArray_CbmMuchUnpackMonitor(void *p) {
      delete [] ((::CbmMuchUnpackMonitor*)p);
   }
   static void destruct_CbmMuchUnpackMonitor(void *p) {
      typedef ::CbmMuchUnpackMonitor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchUnpackMonitor

//______________________________________________________________________________
void CbmMuchUnpackAlgo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchUnpackAlgo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchUnpackAlgo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchUnpackAlgo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchUnpackAlgo(void *p) {
      return  p ? new(p) ::CbmMuchUnpackAlgo : new ::CbmMuchUnpackAlgo;
   }
   static void *newArray_CbmMuchUnpackAlgo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchUnpackAlgo[nElements] : new ::CbmMuchUnpackAlgo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchUnpackAlgo(void *p) {
      delete ((::CbmMuchUnpackAlgo*)p);
   }
   static void deleteArray_CbmMuchUnpackAlgo(void *p) {
      delete [] ((::CbmMuchUnpackAlgo*)p);
   }
   static void destruct_CbmMuchUnpackAlgo(void *p) {
      typedef ::CbmMuchUnpackAlgo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchUnpackAlgo

//______________________________________________________________________________
void CbmMuchUnpackConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchUnpackConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchUnpackConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchUnpackConfig::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmMuchUnpackConfig(void *p) {
      delete ((::CbmMuchUnpackConfig*)p);
   }
   static void deleteArray_CbmMuchUnpackConfig(void *p) {
      delete [] ((::CbmMuchUnpackConfig*)p);
   }
   static void destruct_CbmMuchUnpackConfig(void *p) {
      typedef ::CbmMuchUnpackConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchUnpackConfig

namespace ROOT {
   static TClass *vectorlEvectorlEunsignedsPintgRsPgR_Dictionary();
   static void vectorlEvectorlEunsignedsPintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEunsignedsPintgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEunsignedsPintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEunsignedsPintgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEunsignedsPintgRsPgR(void *p);
   static void destruct_vectorlEvectorlEunsignedsPintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<unsigned int> >*)
   {
      vector<vector<unsigned int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<unsigned int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<unsigned int> >", -2, "vector", 389,
                  typeid(vector<vector<unsigned int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEunsignedsPintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<unsigned int> >) );
      instance.SetNew(&new_vectorlEvectorlEunsignedsPintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEunsignedsPintgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEunsignedsPintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEunsignedsPintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEunsignedsPintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<unsigned int> > >()));

      ::ROOT::AddClassAlternate("vector<vector<unsigned int> >","std::vector<std::vector<unsigned int, std::allocator<unsigned int> >, std::allocator<std::vector<unsigned int, std::allocator<unsigned int> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<unsigned int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEunsignedsPintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<unsigned int> >*)nullptr)->GetClass();
      vectorlEvectorlEunsignedsPintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEunsignedsPintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEunsignedsPintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<unsigned int> > : new vector<vector<unsigned int> >;
   }
   static void *newArray_vectorlEvectorlEunsignedsPintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<unsigned int> >[nElements] : new vector<vector<unsigned int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEunsignedsPintgRsPgR(void *p) {
      delete ((vector<vector<unsigned int> >*)p);
   }
   static void deleteArray_vectorlEvectorlEunsignedsPintgRsPgR(void *p) {
      delete [] ((vector<vector<unsigned int> >*)p);
   }
   static void destruct_vectorlEvectorlEunsignedsPintgRsPgR(void *p) {
      typedef vector<vector<unsigned int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<unsigned int> >

namespace ROOT {
   static TClass *vectorlEunsignedsPlonggR_Dictionary();
   static void vectorlEunsignedsPlonggR_TClassManip(TClass*);
   static void *new_vectorlEunsignedsPlonggR(void *p = nullptr);
   static void *newArray_vectorlEunsignedsPlonggR(Long_t size, void *p);
   static void delete_vectorlEunsignedsPlonggR(void *p);
   static void deleteArray_vectorlEunsignedsPlonggR(void *p);
   static void destruct_vectorlEunsignedsPlonggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned long>*)
   {
      vector<unsigned long> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned long>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<unsigned long>", -2, "vector", 389,
                  typeid(vector<unsigned long>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEunsignedsPlonggR_Dictionary, isa_proxy, 0,
                  sizeof(vector<unsigned long>) );
      instance.SetNew(&new_vectorlEunsignedsPlonggR);
      instance.SetNewArray(&newArray_vectorlEunsignedsPlonggR);
      instance.SetDelete(&delete_vectorlEunsignedsPlonggR);
      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPlonggR);
      instance.SetDestructor(&destruct_vectorlEunsignedsPlonggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned long> >()));

      ::ROOT::AddClassAlternate("vector<unsigned long>","std::vector<unsigned long, std::allocator<unsigned long> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<unsigned long>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEunsignedsPlonggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<unsigned long>*)nullptr)->GetClass();
      vectorlEunsignedsPlonggR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEunsignedsPlonggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEunsignedsPlonggR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned long> : new vector<unsigned long>;
   }
   static void *newArray_vectorlEunsignedsPlonggR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned long>[nElements] : new vector<unsigned long>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEunsignedsPlonggR(void *p) {
      delete ((vector<unsigned long>*)p);
   }
   static void deleteArray_vectorlEunsignedsPlonggR(void *p) {
      delete [] ((vector<unsigned long>*)p);
   }
   static void destruct_vectorlEunsignedsPlonggR(void *p) {
      typedef vector<unsigned long> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<unsigned long>

namespace ROOT {
   static TClass *vectorlEunsignedsPintgR_Dictionary();
   static void vectorlEunsignedsPintgR_TClassManip(TClass*);
   static void *new_vectorlEunsignedsPintgR(void *p = nullptr);
   static void *newArray_vectorlEunsignedsPintgR(Long_t size, void *p);
   static void delete_vectorlEunsignedsPintgR(void *p);
   static void deleteArray_vectorlEunsignedsPintgR(void *p);
   static void destruct_vectorlEunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned int>*)
   {
      vector<unsigned int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<unsigned int>", -2, "vector", 389,
                  typeid(vector<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<unsigned int>) );
      instance.SetNew(&new_vectorlEunsignedsPintgR);
      instance.SetNewArray(&newArray_vectorlEunsignedsPintgR);
      instance.SetDelete(&delete_vectorlEunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPintgR);
      instance.SetDestructor(&destruct_vectorlEunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned int> >()));

      ::ROOT::AddClassAlternate("vector<unsigned int>","std::vector<unsigned int, std::allocator<unsigned int> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<unsigned int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<unsigned int>*)nullptr)->GetClass();
      vectorlEunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned int> : new vector<unsigned int>;
   }
   static void *newArray_vectorlEunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned int>[nElements] : new vector<unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEunsignedsPintgR(void *p) {
      delete ((vector<unsigned int>*)p);
   }
   static void deleteArray_vectorlEunsignedsPintgR(void *p) {
      delete [] ((vector<unsigned int>*)p);
   }
   static void destruct_vectorlEunsignedsPintgR(void *p) {
      typedef vector<unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<unsigned int>

namespace ROOT {
   static TClass *vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR_Dictionary();
   static void vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p);
   static void deleteArray_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p);
   static void destruct_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<unsigned int,unsigned int> >*)
   {
      vector<pair<unsigned int,unsigned int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<unsigned int,unsigned int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<unsigned int,unsigned int> >", -2, "vector", 389,
                  typeid(vector<pair<unsigned int,unsigned int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<unsigned int,unsigned int> >) );
      instance.SetNew(&new_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<unsigned int,unsigned int> > >()));

      ::ROOT::AddClassAlternate("vector<pair<unsigned int,unsigned int> >","std::vector<std::pair<unsigned int, unsigned int>, std::allocator<std::pair<unsigned int, unsigned int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pair<unsigned int,unsigned int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<unsigned int,unsigned int> >*)nullptr)->GetClass();
      vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<unsigned int,unsigned int> > : new vector<pair<unsigned int,unsigned int> >;
   }
   static void *newArray_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<unsigned int,unsigned int> >[nElements] : new vector<pair<unsigned int,unsigned int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p) {
      delete ((vector<pair<unsigned int,unsigned int> >*)p);
   }
   static void deleteArray_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p) {
      delete [] ((vector<pair<unsigned int,unsigned int> >*)p);
   }
   static void destruct_vectorlEpairlEunsignedsPintcOunsignedsPintgRsPgR(void *p) {
      typedef vector<pair<unsigned int,unsigned int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<unsigned int,unsigned int> >

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
   static TClass *vectorlEboolgR_Dictionary();
   static void vectorlEboolgR_TClassManip(TClass*);
   static void *new_vectorlEboolgR(void *p = nullptr);
   static void *newArray_vectorlEboolgR(Long_t size, void *p);
   static void delete_vectorlEboolgR(void *p);
   static void deleteArray_vectorlEboolgR(void *p);
   static void destruct_vectorlEboolgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<bool>*)
   {
      vector<bool> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<bool>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<bool>", -2, "vector", 596,
                  typeid(vector<bool>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEboolgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<bool>) );
      instance.SetNew(&new_vectorlEboolgR);
      instance.SetNewArray(&newArray_vectorlEboolgR);
      instance.SetDelete(&delete_vectorlEboolgR);
      instance.SetDeleteArray(&deleteArray_vectorlEboolgR);
      instance.SetDestructor(&destruct_vectorlEboolgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<bool> >()));

      ::ROOT::AddClassAlternate("vector<bool>","std::vector<bool, std::allocator<bool> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<bool>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEboolgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<bool>*)nullptr)->GetClass();
      vectorlEboolgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEboolgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEboolgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bool> : new vector<bool>;
   }
   static void *newArray_vectorlEboolgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bool>[nElements] : new vector<bool>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEboolgR(void *p) {
      delete ((vector<bool>*)p);
   }
   static void deleteArray_vectorlEboolgR(void *p) {
      delete [] ((vector<bool>*)p);
   }
   static void destruct_vectorlEboolgR(void *p) {
      typedef vector<bool> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<bool>

namespace ROOT {
   static TClass *vectorlETProfile2DmUgR_Dictionary();
   static void vectorlETProfile2DmUgR_TClassManip(TClass*);
   static void *new_vectorlETProfile2DmUgR(void *p = nullptr);
   static void *newArray_vectorlETProfile2DmUgR(Long_t size, void *p);
   static void delete_vectorlETProfile2DmUgR(void *p);
   static void deleteArray_vectorlETProfile2DmUgR(void *p);
   static void destruct_vectorlETProfile2DmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TProfile2D*>*)
   {
      vector<TProfile2D*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TProfile2D*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TProfile2D*>", -2, "vector", 389,
                  typeid(vector<TProfile2D*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETProfile2DmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TProfile2D*>) );
      instance.SetNew(&new_vectorlETProfile2DmUgR);
      instance.SetNewArray(&newArray_vectorlETProfile2DmUgR);
      instance.SetDelete(&delete_vectorlETProfile2DmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETProfile2DmUgR);
      instance.SetDestructor(&destruct_vectorlETProfile2DmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TProfile2D*> >()));

      ::ROOT::AddClassAlternate("vector<TProfile2D*>","std::vector<TProfile2D*, std::allocator<TProfile2D*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TProfile2D*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETProfile2DmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TProfile2D*>*)nullptr)->GetClass();
      vectorlETProfile2DmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETProfile2DmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETProfile2DmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TProfile2D*> : new vector<TProfile2D*>;
   }
   static void *newArray_vectorlETProfile2DmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TProfile2D*>[nElements] : new vector<TProfile2D*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETProfile2DmUgR(void *p) {
      delete ((vector<TProfile2D*>*)p);
   }
   static void deleteArray_vectorlETProfile2DmUgR(void *p) {
      delete [] ((vector<TProfile2D*>*)p);
   }
   static void destruct_vectorlETProfile2DmUgR(void *p) {
      typedef vector<TProfile2D*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TProfile2D*>

namespace ROOT {
   static TClass *vectorlETProfilemUgR_Dictionary();
   static void vectorlETProfilemUgR_TClassManip(TClass*);
   static void *new_vectorlETProfilemUgR(void *p = nullptr);
   static void *newArray_vectorlETProfilemUgR(Long_t size, void *p);
   static void delete_vectorlETProfilemUgR(void *p);
   static void deleteArray_vectorlETProfilemUgR(void *p);
   static void destruct_vectorlETProfilemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TProfile*>*)
   {
      vector<TProfile*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TProfile*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TProfile*>", -2, "vector", 389,
                  typeid(vector<TProfile*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETProfilemUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TProfile*>) );
      instance.SetNew(&new_vectorlETProfilemUgR);
      instance.SetNewArray(&newArray_vectorlETProfilemUgR);
      instance.SetDelete(&delete_vectorlETProfilemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETProfilemUgR);
      instance.SetDestructor(&destruct_vectorlETProfilemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TProfile*> >()));

      ::ROOT::AddClassAlternate("vector<TProfile*>","std::vector<TProfile*, std::allocator<TProfile*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TProfile*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETProfilemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TProfile*>*)nullptr)->GetClass();
      vectorlETProfilemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETProfilemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETProfilemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TProfile*> : new vector<TProfile*>;
   }
   static void *newArray_vectorlETProfilemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TProfile*>[nElements] : new vector<TProfile*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETProfilemUgR(void *p) {
      delete ((vector<TProfile*>*)p);
   }
   static void deleteArray_vectorlETProfilemUgR(void *p) {
      delete [] ((vector<TProfile*>*)p);
   }
   static void destruct_vectorlETProfilemUgR(void *p) {
      typedef vector<TProfile*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TProfile*>

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
   static TClass *vectorlETCanvasmUgR_Dictionary();
   static void vectorlETCanvasmUgR_TClassManip(TClass*);
   static void *new_vectorlETCanvasmUgR(void *p = nullptr);
   static void *newArray_vectorlETCanvasmUgR(Long_t size, void *p);
   static void delete_vectorlETCanvasmUgR(void *p);
   static void deleteArray_vectorlETCanvasmUgR(void *p);
   static void destruct_vectorlETCanvasmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TCanvas*>*)
   {
      vector<TCanvas*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TCanvas*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TCanvas*>", -2, "vector", 389,
                  typeid(vector<TCanvas*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETCanvasmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TCanvas*>) );
      instance.SetNew(&new_vectorlETCanvasmUgR);
      instance.SetNewArray(&newArray_vectorlETCanvasmUgR);
      instance.SetDelete(&delete_vectorlETCanvasmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETCanvasmUgR);
      instance.SetDestructor(&destruct_vectorlETCanvasmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TCanvas*> >()));

      ::ROOT::AddClassAlternate("vector<TCanvas*>","std::vector<TCanvas*, std::allocator<TCanvas*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TCanvas*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETCanvasmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TCanvas*>*)nullptr)->GetClass();
      vectorlETCanvasmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETCanvasmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETCanvasmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TCanvas*> : new vector<TCanvas*>;
   }
   static void *newArray_vectorlETCanvasmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TCanvas*>[nElements] : new vector<TCanvas*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETCanvasmUgR(void *p) {
      delete ((vector<TCanvas*>*)p);
   }
   static void deleteArray_vectorlETCanvasmUgR(void *p) {
      delete [] ((vector<TCanvas*>*)p);
   }
   static void destruct_vectorlETCanvasmUgR(void *p) {
      typedef vector<TCanvas*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TCanvas*>

namespace ROOT {
   static TClass *vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR_Dictionary();
   static void vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR_TClassManip(TClass*);
   static void *new_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR(void *p = nullptr);
   static void *newArray_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR(Long_t size, void *p);
   static void delete_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR(void *p);
   static void deleteArray_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR(void *p);
   static void destruct_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMuchUnpackConfig::FebChanMaskReco>*)
   {
      vector<CbmMuchUnpackConfig::FebChanMaskReco> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMuchUnpackConfig::FebChanMaskReco>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMuchUnpackConfig::FebChanMaskReco>", -2, "vector", 389,
                  typeid(vector<CbmMuchUnpackConfig::FebChanMaskReco>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmMuchUnpackConfig::FebChanMaskReco>) );
      instance.SetNew(&new_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR);
      instance.SetNewArray(&newArray_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR);
      instance.SetDelete(&delete_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR);
      instance.SetDestructor(&destruct_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMuchUnpackConfig::FebChanMaskReco> >()));

      ::ROOT::AddClassAlternate("vector<CbmMuchUnpackConfig::FebChanMaskReco>","std::vector<CbmMuchUnpackConfig::FebChanMaskReco, std::allocator<CbmMuchUnpackConfig::FebChanMaskReco> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMuchUnpackConfig::FebChanMaskReco>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMuchUnpackConfig::FebChanMaskReco>*)nullptr)->GetClass();
      vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMuchUnpackConfig::FebChanMaskReco> : new vector<CbmMuchUnpackConfig::FebChanMaskReco>;
   }
   static void *newArray_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMuchUnpackConfig::FebChanMaskReco>[nElements] : new vector<CbmMuchUnpackConfig::FebChanMaskReco>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR(void *p) {
      delete ((vector<CbmMuchUnpackConfig::FebChanMaskReco>*)p);
   }
   static void deleteArray_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR(void *p) {
      delete [] ((vector<CbmMuchUnpackConfig::FebChanMaskReco>*)p);
   }
   static void destruct_vectorlECbmMuchUnpackConfigcLcLFebChanMaskRecogR(void *p) {
      typedef vector<CbmMuchUnpackConfig::FebChanMaskReco> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMuchUnpackConfig::FebChanMaskReco>

namespace ROOT {
   static TClass *setlEunsignedsPintgR_Dictionary();
   static void setlEunsignedsPintgR_TClassManip(TClass*);
   static void *new_setlEunsignedsPintgR(void *p = nullptr);
   static void *newArray_setlEunsignedsPintgR(Long_t size, void *p);
   static void delete_setlEunsignedsPintgR(void *p);
   static void deleteArray_setlEunsignedsPintgR(void *p);
   static void destruct_setlEunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const set<unsigned int>*)
   {
      set<unsigned int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(set<unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("set<unsigned int>", -2, "set", 94,
                  typeid(set<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &setlEunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(set<unsigned int>) );
      instance.SetNew(&new_setlEunsignedsPintgR);
      instance.SetNewArray(&newArray_setlEunsignedsPintgR);
      instance.SetDelete(&delete_setlEunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_setlEunsignedsPintgR);
      instance.SetDestructor(&destruct_setlEunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Insert< set<unsigned int> >()));

      ::ROOT::AddClassAlternate("set<unsigned int>","std::set<unsigned int, std::less<unsigned int>, std::allocator<unsigned int> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const set<unsigned int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *setlEunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const set<unsigned int>*)nullptr)->GetClass();
      setlEunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void setlEunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_setlEunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<unsigned int> : new set<unsigned int>;
   }
   static void *newArray_setlEunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<unsigned int>[nElements] : new set<unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_setlEunsignedsPintgR(void *p) {
      delete ((set<unsigned int>*)p);
   }
   static void deleteArray_setlEunsignedsPintgR(void *p) {
      delete [] ((set<unsigned int>*)p);
   }
   static void destruct_setlEunsignedsPintgR(void *p) {
      typedef set<unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class set<unsigned int>

namespace ROOT {
   static TClass *maplEunsignedsPintcOunsignedsPintgR_Dictionary();
   static void maplEunsignedsPintcOunsignedsPintgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOunsignedsPintgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOunsignedsPintgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOunsignedsPintgR(void *p);
   static void deleteArray_maplEunsignedsPintcOunsignedsPintgR(void *p);
   static void destruct_maplEunsignedsPintcOunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,unsigned int>*)
   {
      map<unsigned int,unsigned int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,unsigned int>", -2, "map", 100,
                  typeid(map<unsigned int,unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,unsigned int>) );
      instance.SetNew(&new_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,unsigned int> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,unsigned int>","std::map<unsigned int, unsigned int, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, unsigned int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,unsigned int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,unsigned int>*)nullptr)->GetClass();
      maplEunsignedsPintcOunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,unsigned int> : new map<unsigned int,unsigned int>;
   }
   static void *newArray_maplEunsignedsPintcOunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,unsigned int>[nElements] : new map<unsigned int,unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      delete ((map<unsigned int,unsigned int>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      delete [] ((map<unsigned int,unsigned int>*)p);
   }
   static void destruct_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      typedef map<unsigned int,unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,unsigned int>

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

namespace {
  void TriggerDictionaryInitialization_libCbmMuchReco_Impl() {
    static const char* headers[] = {
"CbmMuchFindHitsGem.h",
"CbmMuchHitProducerIdeal.h",
"CbmMuchFindTracks.h",
"CbmMuchMatchTracks.h",
"CbmMuchTrackFinderIdeal.h",
"CbmMuchHitFinderQa.h",
"CbmMuchUnpackAlgo.h",
"CbmMuchUnpackMonitor.h",
"CbmMuchUnpackConfig.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/much/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/much/unpack",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/much/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/much/unpack",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/core/qa/checker",
"/home/heinemann/gpuunpacker/cbmroot/core/qa",
"/home/heinemann/gpuunpacker/cbmroot/external/yaml-cpp/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/detectors/much/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmMuchReco dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmMuchFindHitsGem.h")))  CbmMuchFindHitsGem;
class __attribute__((annotate("$clingAutoload$CbmMuchHitProducerIdeal.h")))  CbmMuchHitProducerIdeal;
class __attribute__((annotate("$clingAutoload$CbmMuchFindTracks.h")))  CbmMuchFindTracks;
class __attribute__((annotate("$clingAutoload$CbmMuchMatchTracks.h")))  CbmMuchMatchTracks;
class __attribute__((annotate("$clingAutoload$CbmMuchTrackFinderIdeal.h")))  CbmMuchTrackFinderIdeal;
class __attribute__((annotate("$clingAutoload$CbmMuchHitFinderQa.h")))  CbmMuchHitFinderQa;
class __attribute__((annotate("$clingAutoload$CbmMuchUnpackMonitor.h")))  __attribute__((annotate("$clingAutoload$CbmMuchUnpackAlgo.h")))  CbmMuchUnpackMonitor;
class __attribute__((annotate("$clingAutoload$CbmMuchUnpackAlgo.h")))  CbmMuchUnpackAlgo;
class __attribute__((annotate("$clingAutoload$CbmMuchUnpackConfig.h")))  CbmMuchUnpackConfig;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmMuchReco dictionary payload"

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
#include "CbmMuchFindHitsGem.h"
#include "CbmMuchHitProducerIdeal.h"
#include "CbmMuchFindTracks.h"
#include "CbmMuchMatchTracks.h"
#include "CbmMuchTrackFinderIdeal.h"
#include "CbmMuchHitFinderQa.h"
#include "CbmMuchUnpackAlgo.h"
#include "CbmMuchUnpackMonitor.h"
#include "CbmMuchUnpackConfig.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmMuchFindHitsGem", payloadCode, "@",
"CbmMuchFindTracks", payloadCode, "@",
"CbmMuchHitFinderQa", payloadCode, "@",
"CbmMuchHitProducerIdeal", payloadCode, "@",
"CbmMuchMatchTracks", payloadCode, "@",
"CbmMuchTrackFinderIdeal", payloadCode, "@",
"CbmMuchUnpackAlgo", payloadCode, "@",
"CbmMuchUnpackConfig", payloadCode, "@",
"CbmMuchUnpackMonitor", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmMuchReco",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmMuchReco_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmMuchReco_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmMuchReco() {
  TriggerDictionaryInitialization_libCbmMuchReco_Impl();
}
