// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__Littrack
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
#include "CbmLitDetectorSetup.h"
#include "CbmLitGlobalElectronId.h"
#include "CbmLitFindMvdTracks.h"
#include "CbmLitFitTracksParallel.h"
#include "CbmLitFitTracks.h"
#include "CbmLitPolarizedGenerator.h"
#include "CbmLitHitData.h"
#include "CbmLitTrackFinderBranch.h"
#include "CbmLitTrackFinderNN.h"
#include "CbmLitNearestHitTofMerger.h"
#include "CbmLitAllHitsTofMerger.h"
#include "CbmLitTrackFitterImp.h"
#include "CbmLitKalmanFilter.h"
#include "CbmLitKalmanSmoother.h"
#include "CbmLitTrackFitterIter.h"
#include "CbmLitTGeoTrackPropagator.h"
#include "CbmLitRK4TrackExtrapolator.h"
#include "CbmLitLineTrackExtrapolator.h"
#include "CbmLitTGeoNavigator.h"
#include "CbmLitCleverTrackExtrapolator.h"
#include "CbmLitQualitySort.h"
#include "CbmLitTrackSelectionCuts.h"
#include "CbmLitTrackSelectionEmpty.h"
#include "CbmLitTrackSelectionMuch.h"
#include "CbmLitTrackSelectionSharedHits.h"
#include "CbmLitTrackSelectionTrd.h"
#include "CbmLitMath.h"
#include "CbmLitMatrixMath.h"
#include "CbmLitMaterialEffectsImp.h"
#include "CbmLitFieldGridCreator.h"
#include "CbmLitFieldFitter.h"
#include "CbmLitToolFactory.h"
#include "CbmLitTrackingGeometryConstructor.h"
#include "CbmLitFindGlobalTracks.h"
#include "CbmLitFindGlobalTracksIdeal.h"
#include "CbmLitFindGlobalTracksParallel.h"
#include "CbmLitConverterParallel.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_CbmLitGlobalElectronId(void *p);
   static void deleteArray_CbmLitGlobalElectronId(void *p);
   static void destruct_CbmLitGlobalElectronId(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitGlobalElectronId*)
   {
      ::CbmLitGlobalElectronId *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitGlobalElectronId >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitGlobalElectronId", ::CbmLitGlobalElectronId::Class_Version(), "CbmLitGlobalElectronId.h", 13,
                  typeid(::CbmLitGlobalElectronId), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitGlobalElectronId::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitGlobalElectronId) );
      instance.SetDelete(&delete_CbmLitGlobalElectronId);
      instance.SetDeleteArray(&deleteArray_CbmLitGlobalElectronId);
      instance.SetDestructor(&destruct_CbmLitGlobalElectronId);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitGlobalElectronId*)
   {
      return GenerateInitInstanceLocal((::CbmLitGlobalElectronId*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitGlobalElectronId*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitFindMvdTracks(void *p = nullptr);
   static void *newArray_CbmLitFindMvdTracks(Long_t size, void *p);
   static void delete_CbmLitFindMvdTracks(void *p);
   static void deleteArray_CbmLitFindMvdTracks(void *p);
   static void destruct_CbmLitFindMvdTracks(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitFindMvdTracks*)
   {
      ::CbmLitFindMvdTracks *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitFindMvdTracks >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitFindMvdTracks", ::CbmLitFindMvdTracks::Class_Version(), "CbmLitFindMvdTracks.h", 34,
                  typeid(::CbmLitFindMvdTracks), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitFindMvdTracks::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitFindMvdTracks) );
      instance.SetNew(&new_CbmLitFindMvdTracks);
      instance.SetNewArray(&newArray_CbmLitFindMvdTracks);
      instance.SetDelete(&delete_CbmLitFindMvdTracks);
      instance.SetDeleteArray(&deleteArray_CbmLitFindMvdTracks);
      instance.SetDestructor(&destruct_CbmLitFindMvdTracks);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitFindMvdTracks*)
   {
      return GenerateInitInstanceLocal((::CbmLitFindMvdTracks*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitFindMvdTracks*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitFitTracksParallel(void *p = nullptr);
   static void *newArray_CbmLitFitTracksParallel(Long_t size, void *p);
   static void delete_CbmLitFitTracksParallel(void *p);
   static void deleteArray_CbmLitFitTracksParallel(void *p);
   static void destruct_CbmLitFitTracksParallel(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitFitTracksParallel*)
   {
      ::CbmLitFitTracksParallel *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitFitTracksParallel >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitFitTracksParallel", ::CbmLitFitTracksParallel::Class_Version(), "CbmLitFitTracksParallel.h", 21,
                  typeid(::CbmLitFitTracksParallel), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitFitTracksParallel::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitFitTracksParallel) );
      instance.SetNew(&new_CbmLitFitTracksParallel);
      instance.SetNewArray(&newArray_CbmLitFitTracksParallel);
      instance.SetDelete(&delete_CbmLitFitTracksParallel);
      instance.SetDeleteArray(&deleteArray_CbmLitFitTracksParallel);
      instance.SetDestructor(&destruct_CbmLitFitTracksParallel);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitFitTracksParallel*)
   {
      return GenerateInitInstanceLocal((::CbmLitFitTracksParallel*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitFitTracksParallel*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitFitTracks(void *p = nullptr);
   static void *newArray_CbmLitFitTracks(Long_t size, void *p);
   static void delete_CbmLitFitTracks(void *p);
   static void deleteArray_CbmLitFitTracks(void *p);
   static void destruct_CbmLitFitTracks(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitFitTracks*)
   {
      ::CbmLitFitTracks *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitFitTracks >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitFitTracks", ::CbmLitFitTracks::Class_Version(), "CbmLitFitTracks.h", 21,
                  typeid(::CbmLitFitTracks), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitFitTracks::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitFitTracks) );
      instance.SetNew(&new_CbmLitFitTracks);
      instance.SetNewArray(&newArray_CbmLitFitTracks);
      instance.SetDelete(&delete_CbmLitFitTracks);
      instance.SetDeleteArray(&deleteArray_CbmLitFitTracks);
      instance.SetDestructor(&destruct_CbmLitFitTracks);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitFitTracks*)
   {
      return GenerateInitInstanceLocal((::CbmLitFitTracks*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitFitTracks*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitPolarizedGenerator(void *p = nullptr);
   static void *newArray_CbmLitPolarizedGenerator(Long_t size, void *p);
   static void delete_CbmLitPolarizedGenerator(void *p);
   static void deleteArray_CbmLitPolarizedGenerator(void *p);
   static void destruct_CbmLitPolarizedGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitPolarizedGenerator*)
   {
      ::CbmLitPolarizedGenerator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitPolarizedGenerator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitPolarizedGenerator", ::CbmLitPolarizedGenerator::Class_Version(), "CbmLitPolarizedGenerator.h", 38,
                  typeid(::CbmLitPolarizedGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitPolarizedGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitPolarizedGenerator) );
      instance.SetNew(&new_CbmLitPolarizedGenerator);
      instance.SetNewArray(&newArray_CbmLitPolarizedGenerator);
      instance.SetDelete(&delete_CbmLitPolarizedGenerator);
      instance.SetDeleteArray(&deleteArray_CbmLitPolarizedGenerator);
      instance.SetDestructor(&destruct_CbmLitPolarizedGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitPolarizedGenerator*)
   {
      return GenerateInitInstanceLocal((::CbmLitPolarizedGenerator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitPolarizedGenerator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitFindGlobalTracks(void *p = nullptr);
   static void *newArray_CbmLitFindGlobalTracks(Long_t size, void *p);
   static void delete_CbmLitFindGlobalTracks(void *p);
   static void deleteArray_CbmLitFindGlobalTracks(void *p);
   static void destruct_CbmLitFindGlobalTracks(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitFindGlobalTracks*)
   {
      ::CbmLitFindGlobalTracks *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitFindGlobalTracks >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitFindGlobalTracks", ::CbmLitFindGlobalTracks::Class_Version(), "CbmLitFindGlobalTracks.h", 43,
                  typeid(::CbmLitFindGlobalTracks), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitFindGlobalTracks::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitFindGlobalTracks) );
      instance.SetNew(&new_CbmLitFindGlobalTracks);
      instance.SetNewArray(&newArray_CbmLitFindGlobalTracks);
      instance.SetDelete(&delete_CbmLitFindGlobalTracks);
      instance.SetDeleteArray(&deleteArray_CbmLitFindGlobalTracks);
      instance.SetDestructor(&destruct_CbmLitFindGlobalTracks);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitFindGlobalTracks*)
   {
      return GenerateInitInstanceLocal((::CbmLitFindGlobalTracks*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracks*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitFindGlobalTracksIdeal(void *p = nullptr);
   static void *newArray_CbmLitFindGlobalTracksIdeal(Long_t size, void *p);
   static void delete_CbmLitFindGlobalTracksIdeal(void *p);
   static void deleteArray_CbmLitFindGlobalTracksIdeal(void *p);
   static void destruct_CbmLitFindGlobalTracksIdeal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitFindGlobalTracksIdeal*)
   {
      ::CbmLitFindGlobalTracksIdeal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitFindGlobalTracksIdeal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitFindGlobalTracksIdeal", ::CbmLitFindGlobalTracksIdeal::Class_Version(), "CbmLitFindGlobalTracksIdeal.h", 37,
                  typeid(::CbmLitFindGlobalTracksIdeal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitFindGlobalTracksIdeal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitFindGlobalTracksIdeal) );
      instance.SetNew(&new_CbmLitFindGlobalTracksIdeal);
      instance.SetNewArray(&newArray_CbmLitFindGlobalTracksIdeal);
      instance.SetDelete(&delete_CbmLitFindGlobalTracksIdeal);
      instance.SetDeleteArray(&deleteArray_CbmLitFindGlobalTracksIdeal);
      instance.SetDestructor(&destruct_CbmLitFindGlobalTracksIdeal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitFindGlobalTracksIdeal*)
   {
      return GenerateInitInstanceLocal((::CbmLitFindGlobalTracksIdeal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracksIdeal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitFindGlobalTracksParallel(void *p = nullptr);
   static void *newArray_CbmLitFindGlobalTracksParallel(Long_t size, void *p);
   static void delete_CbmLitFindGlobalTracksParallel(void *p);
   static void deleteArray_CbmLitFindGlobalTracksParallel(void *p);
   static void destruct_CbmLitFindGlobalTracksParallel(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitFindGlobalTracksParallel*)
   {
      ::CbmLitFindGlobalTracksParallel *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitFindGlobalTracksParallel >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitFindGlobalTracksParallel", ::CbmLitFindGlobalTracksParallel::Class_Version(), "CbmLitFindGlobalTracksParallel.h", 29,
                  typeid(::CbmLitFindGlobalTracksParallel), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitFindGlobalTracksParallel::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitFindGlobalTracksParallel) );
      instance.SetNew(&new_CbmLitFindGlobalTracksParallel);
      instance.SetNewArray(&newArray_CbmLitFindGlobalTracksParallel);
      instance.SetDelete(&delete_CbmLitFindGlobalTracksParallel);
      instance.SetDeleteArray(&deleteArray_CbmLitFindGlobalTracksParallel);
      instance.SetDestructor(&destruct_CbmLitFindGlobalTracksParallel);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitFindGlobalTracksParallel*)
   {
      return GenerateInitInstanceLocal((::CbmLitFindGlobalTracksParallel*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracksParallel*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmLitGlobalElectronId::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitGlobalElectronId::Class_Name()
{
   return "CbmLitGlobalElectronId";
}

//______________________________________________________________________________
const char *CbmLitGlobalElectronId::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitGlobalElectronId*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitGlobalElectronId::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitGlobalElectronId*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitGlobalElectronId::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitGlobalElectronId*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitGlobalElectronId::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitGlobalElectronId*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitFindMvdTracks::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitFindMvdTracks::Class_Name()
{
   return "CbmLitFindMvdTracks";
}

//______________________________________________________________________________
const char *CbmLitFindMvdTracks::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindMvdTracks*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitFindMvdTracks::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindMvdTracks*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitFindMvdTracks::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindMvdTracks*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitFindMvdTracks::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindMvdTracks*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitFitTracksParallel::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitFitTracksParallel::Class_Name()
{
   return "CbmLitFitTracksParallel";
}

//______________________________________________________________________________
const char *CbmLitFitTracksParallel::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitTracksParallel*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitFitTracksParallel::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitTracksParallel*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitFitTracksParallel::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitTracksParallel*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitFitTracksParallel::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitTracksParallel*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitFitTracks::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitFitTracks::Class_Name()
{
   return "CbmLitFitTracks";
}

//______________________________________________________________________________
const char *CbmLitFitTracks::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitTracks*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitFitTracks::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitTracks*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitFitTracks::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitTracks*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitFitTracks::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitTracks*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitPolarizedGenerator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitPolarizedGenerator::Class_Name()
{
   return "CbmLitPolarizedGenerator";
}

//______________________________________________________________________________
const char *CbmLitPolarizedGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitPolarizedGenerator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitPolarizedGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitPolarizedGenerator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitPolarizedGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitPolarizedGenerator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitPolarizedGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitPolarizedGenerator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitFindGlobalTracks::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitFindGlobalTracks::Class_Name()
{
   return "CbmLitFindGlobalTracks";
}

//______________________________________________________________________________
const char *CbmLitFindGlobalTracks::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracks*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitFindGlobalTracks::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracks*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitFindGlobalTracks::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracks*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitFindGlobalTracks::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracks*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitFindGlobalTracksIdeal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitFindGlobalTracksIdeal::Class_Name()
{
   return "CbmLitFindGlobalTracksIdeal";
}

//______________________________________________________________________________
const char *CbmLitFindGlobalTracksIdeal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracksIdeal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitFindGlobalTracksIdeal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracksIdeal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitFindGlobalTracksIdeal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracksIdeal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitFindGlobalTracksIdeal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracksIdeal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitFindGlobalTracksParallel::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitFindGlobalTracksParallel::Class_Name()
{
   return "CbmLitFindGlobalTracksParallel";
}

//______________________________________________________________________________
const char *CbmLitFindGlobalTracksParallel::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracksParallel*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitFindGlobalTracksParallel::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracksParallel*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitFindGlobalTracksParallel::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracksParallel*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitFindGlobalTracksParallel::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFindGlobalTracksParallel*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmLitGlobalElectronId::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitGlobalElectronId.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitGlobalElectronId::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitGlobalElectronId::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmLitGlobalElectronId(void *p) {
      delete ((::CbmLitGlobalElectronId*)p);
   }
   static void deleteArray_CbmLitGlobalElectronId(void *p) {
      delete [] ((::CbmLitGlobalElectronId*)p);
   }
   static void destruct_CbmLitGlobalElectronId(void *p) {
      typedef ::CbmLitGlobalElectronId current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitGlobalElectronId

//______________________________________________________________________________
void CbmLitFindMvdTracks::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitFindMvdTracks.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitFindMvdTracks::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitFindMvdTracks::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitFindMvdTracks(void *p) {
      return  p ? new(p) ::CbmLitFindMvdTracks : new ::CbmLitFindMvdTracks;
   }
   static void *newArray_CbmLitFindMvdTracks(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitFindMvdTracks[nElements] : new ::CbmLitFindMvdTracks[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitFindMvdTracks(void *p) {
      delete ((::CbmLitFindMvdTracks*)p);
   }
   static void deleteArray_CbmLitFindMvdTracks(void *p) {
      delete [] ((::CbmLitFindMvdTracks*)p);
   }
   static void destruct_CbmLitFindMvdTracks(void *p) {
      typedef ::CbmLitFindMvdTracks current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitFindMvdTracks

//______________________________________________________________________________
void CbmLitFitTracksParallel::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitFitTracksParallel.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitFitTracksParallel::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitFitTracksParallel::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitFitTracksParallel(void *p) {
      return  p ? new(p) ::CbmLitFitTracksParallel : new ::CbmLitFitTracksParallel;
   }
   static void *newArray_CbmLitFitTracksParallel(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitFitTracksParallel[nElements] : new ::CbmLitFitTracksParallel[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitFitTracksParallel(void *p) {
      delete ((::CbmLitFitTracksParallel*)p);
   }
   static void deleteArray_CbmLitFitTracksParallel(void *p) {
      delete [] ((::CbmLitFitTracksParallel*)p);
   }
   static void destruct_CbmLitFitTracksParallel(void *p) {
      typedef ::CbmLitFitTracksParallel current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitFitTracksParallel

//______________________________________________________________________________
void CbmLitFitTracks::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitFitTracks.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitFitTracks::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitFitTracks::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitFitTracks(void *p) {
      return  p ? new(p) ::CbmLitFitTracks : new ::CbmLitFitTracks;
   }
   static void *newArray_CbmLitFitTracks(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitFitTracks[nElements] : new ::CbmLitFitTracks[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitFitTracks(void *p) {
      delete ((::CbmLitFitTracks*)p);
   }
   static void deleteArray_CbmLitFitTracks(void *p) {
      delete [] ((::CbmLitFitTracks*)p);
   }
   static void destruct_CbmLitFitTracks(void *p) {
      typedef ::CbmLitFitTracks current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitFitTracks

//______________________________________________________________________________
void CbmLitPolarizedGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitPolarizedGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitPolarizedGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitPolarizedGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitPolarizedGenerator(void *p) {
      return  p ? new(p) ::CbmLitPolarizedGenerator : new ::CbmLitPolarizedGenerator;
   }
   static void *newArray_CbmLitPolarizedGenerator(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitPolarizedGenerator[nElements] : new ::CbmLitPolarizedGenerator[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitPolarizedGenerator(void *p) {
      delete ((::CbmLitPolarizedGenerator*)p);
   }
   static void deleteArray_CbmLitPolarizedGenerator(void *p) {
      delete [] ((::CbmLitPolarizedGenerator*)p);
   }
   static void destruct_CbmLitPolarizedGenerator(void *p) {
      typedef ::CbmLitPolarizedGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitPolarizedGenerator

//______________________________________________________________________________
void CbmLitFindGlobalTracks::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitFindGlobalTracks.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitFindGlobalTracks::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitFindGlobalTracks::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitFindGlobalTracks(void *p) {
      return  p ? new(p) ::CbmLitFindGlobalTracks : new ::CbmLitFindGlobalTracks;
   }
   static void *newArray_CbmLitFindGlobalTracks(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitFindGlobalTracks[nElements] : new ::CbmLitFindGlobalTracks[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitFindGlobalTracks(void *p) {
      delete ((::CbmLitFindGlobalTracks*)p);
   }
   static void deleteArray_CbmLitFindGlobalTracks(void *p) {
      delete [] ((::CbmLitFindGlobalTracks*)p);
   }
   static void destruct_CbmLitFindGlobalTracks(void *p) {
      typedef ::CbmLitFindGlobalTracks current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitFindGlobalTracks

//______________________________________________________________________________
void CbmLitFindGlobalTracksIdeal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitFindGlobalTracksIdeal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitFindGlobalTracksIdeal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitFindGlobalTracksIdeal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitFindGlobalTracksIdeal(void *p) {
      return  p ? new(p) ::CbmLitFindGlobalTracksIdeal : new ::CbmLitFindGlobalTracksIdeal;
   }
   static void *newArray_CbmLitFindGlobalTracksIdeal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitFindGlobalTracksIdeal[nElements] : new ::CbmLitFindGlobalTracksIdeal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitFindGlobalTracksIdeal(void *p) {
      delete ((::CbmLitFindGlobalTracksIdeal*)p);
   }
   static void deleteArray_CbmLitFindGlobalTracksIdeal(void *p) {
      delete [] ((::CbmLitFindGlobalTracksIdeal*)p);
   }
   static void destruct_CbmLitFindGlobalTracksIdeal(void *p) {
      typedef ::CbmLitFindGlobalTracksIdeal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitFindGlobalTracksIdeal

//______________________________________________________________________________
void CbmLitFindGlobalTracksParallel::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitFindGlobalTracksParallel.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitFindGlobalTracksParallel::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitFindGlobalTracksParallel::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitFindGlobalTracksParallel(void *p) {
      return  p ? new(p) ::CbmLitFindGlobalTracksParallel : new ::CbmLitFindGlobalTracksParallel;
   }
   static void *newArray_CbmLitFindGlobalTracksParallel(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitFindGlobalTracksParallel[nElements] : new ::CbmLitFindGlobalTracksParallel[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitFindGlobalTracksParallel(void *p) {
      delete ((::CbmLitFindGlobalTracksParallel*)p);
   }
   static void deleteArray_CbmLitFindGlobalTracksParallel(void *p) {
      delete [] ((::CbmLitFindGlobalTracksParallel*)p);
   }
   static void destruct_CbmLitFindGlobalTracksParallel(void *p) {
      typedef ::CbmLitFindGlobalTracksParallel current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitFindGlobalTracksParallel

namespace ROOT {
   static TClass *vectorlECbmLitTrackmUgR_Dictionary();
   static void vectorlECbmLitTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmLitTrackmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmLitTrackmUgR(Long_t size, void *p);
   static void delete_vectorlECbmLitTrackmUgR(void *p);
   static void deleteArray_vectorlECbmLitTrackmUgR(void *p);
   static void destruct_vectorlECbmLitTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmLitTrack*>*)
   {
      vector<CbmLitTrack*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmLitTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmLitTrack*>", -2, "vector", 389,
                  typeid(vector<CbmLitTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmLitTrackmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmLitTrack*>) );
      instance.SetNew(&new_vectorlECbmLitTrackmUgR);
      instance.SetNewArray(&newArray_vectorlECbmLitTrackmUgR);
      instance.SetDelete(&delete_vectorlECbmLitTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmLitTrackmUgR);
      instance.SetDestructor(&destruct_vectorlECbmLitTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmLitTrack*> >()));

      ::ROOT::AddClassAlternate("vector<CbmLitTrack*>","std::vector<CbmLitTrack*, std::allocator<CbmLitTrack*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmLitTrack*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmLitTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmLitTrack*>*)nullptr)->GetClass();
      vectorlECbmLitTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmLitTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmLitTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmLitTrack*> : new vector<CbmLitTrack*>;
   }
   static void *newArray_vectorlECbmLitTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmLitTrack*>[nElements] : new vector<CbmLitTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmLitTrackmUgR(void *p) {
      delete ((vector<CbmLitTrack*>*)p);
   }
   static void deleteArray_vectorlECbmLitTrackmUgR(void *p) {
      delete [] ((vector<CbmLitTrack*>*)p);
   }
   static void destruct_vectorlECbmLitTrackmUgR(void *p) {
      typedef vector<CbmLitTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmLitTrack*>

namespace ROOT {
   static TClass *vectorlECbmLitTofTrackmUgR_Dictionary();
   static void vectorlECbmLitTofTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmLitTofTrackmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmLitTofTrackmUgR(Long_t size, void *p);
   static void delete_vectorlECbmLitTofTrackmUgR(void *p);
   static void deleteArray_vectorlECbmLitTofTrackmUgR(void *p);
   static void destruct_vectorlECbmLitTofTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmLitTofTrack*>*)
   {
      vector<CbmLitTofTrack*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmLitTofTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmLitTofTrack*>", -2, "vector", 389,
                  typeid(vector<CbmLitTofTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmLitTofTrackmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmLitTofTrack*>) );
      instance.SetNew(&new_vectorlECbmLitTofTrackmUgR);
      instance.SetNewArray(&newArray_vectorlECbmLitTofTrackmUgR);
      instance.SetDelete(&delete_vectorlECbmLitTofTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmLitTofTrackmUgR);
      instance.SetDestructor(&destruct_vectorlECbmLitTofTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmLitTofTrack*> >()));

      ::ROOT::AddClassAlternate("vector<CbmLitTofTrack*>","std::vector<CbmLitTofTrack*, std::allocator<CbmLitTofTrack*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmLitTofTrack*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmLitTofTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmLitTofTrack*>*)nullptr)->GetClass();
      vectorlECbmLitTofTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmLitTofTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmLitTofTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmLitTofTrack*> : new vector<CbmLitTofTrack*>;
   }
   static void *newArray_vectorlECbmLitTofTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmLitTofTrack*>[nElements] : new vector<CbmLitTofTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmLitTofTrackmUgR(void *p) {
      delete ((vector<CbmLitTofTrack*>*)p);
   }
   static void deleteArray_vectorlECbmLitTofTrackmUgR(void *p) {
      delete [] ((vector<CbmLitTofTrack*>*)p);
   }
   static void destruct_vectorlECbmLitTofTrackmUgR(void *p) {
      typedef vector<CbmLitTofTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmLitTofTrack*>

namespace ROOT {
   static TClass *vectorlECbmLitHitmUgR_Dictionary();
   static void vectorlECbmLitHitmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmLitHitmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmLitHitmUgR(Long_t size, void *p);
   static void delete_vectorlECbmLitHitmUgR(void *p);
   static void deleteArray_vectorlECbmLitHitmUgR(void *p);
   static void destruct_vectorlECbmLitHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmLitHit*>*)
   {
      vector<CbmLitHit*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmLitHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmLitHit*>", -2, "vector", 389,
                  typeid(vector<CbmLitHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmLitHitmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmLitHit*>) );
      instance.SetNew(&new_vectorlECbmLitHitmUgR);
      instance.SetNewArray(&newArray_vectorlECbmLitHitmUgR);
      instance.SetDelete(&delete_vectorlECbmLitHitmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmLitHitmUgR);
      instance.SetDestructor(&destruct_vectorlECbmLitHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmLitHit*> >()));

      ::ROOT::AddClassAlternate("vector<CbmLitHit*>","std::vector<CbmLitHit*, std::allocator<CbmLitHit*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmLitHit*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmLitHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmLitHit*>*)nullptr)->GetClass();
      vectorlECbmLitHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmLitHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmLitHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmLitHit*> : new vector<CbmLitHit*>;
   }
   static void *newArray_vectorlECbmLitHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmLitHit*>[nElements] : new vector<CbmLitHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmLitHitmUgR(void *p) {
      delete ((vector<CbmLitHit*>*)p);
   }
   static void deleteArray_vectorlECbmLitHitmUgR(void *p) {
      delete [] ((vector<CbmLitHit*>*)p);
   }
   static void destruct_vectorlECbmLitHitmUgR(void *p) {
      typedef vector<CbmLitHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmLitHit*>

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
  void TriggerDictionaryInitialization_libLittrack_Impl() {
    static const char* headers[] = {
"CbmLitDetectorSetup.h",
"CbmLitGlobalElectronId.h",
"CbmLitFindMvdTracks.h",
"CbmLitFitTracksParallel.h",
"CbmLitFitTracks.h",
"CbmLitPolarizedGenerator.h",
"CbmLitHitData.h",
"CbmLitTrackFinderBranch.h",
"CbmLitTrackFinderNN.h",
"CbmLitNearestHitTofMerger.h",
"CbmLitAllHitsTofMerger.h",
"CbmLitTrackFitterImp.h",
"CbmLitKalmanFilter.h",
"CbmLitKalmanSmoother.h",
"CbmLitTrackFitterIter.h",
"CbmLitTGeoTrackPropagator.h",
"CbmLitRK4TrackExtrapolator.h",
"CbmLitLineTrackExtrapolator.h",
"CbmLitTGeoNavigator.h",
"CbmLitCleverTrackExtrapolator.h",
"CbmLitQualitySort.h",
"CbmLitTrackSelectionCuts.h",
"CbmLitTrackSelectionEmpty.h",
"CbmLitTrackSelectionMuch.h",
"CbmLitTrackSelectionSharedHits.h",
"CbmLitTrackSelectionTrd.h",
"CbmLitMath.h",
"CbmLitMatrixMath.h",
"CbmLitMaterialEffectsImp.h",
"CbmLitFieldGridCreator.h",
"CbmLitFieldFitter.h",
"CbmLitToolFactory.h",
"CbmLitTrackingGeometryConstructor.h",
"CbmLitFindGlobalTracks.h",
"CbmLitFindGlobalTracksIdeal.h",
"CbmLitFindGlobalTracksParallel.h",
"CbmLitConverterParallel.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
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
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
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
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca/simd",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/steer",
"/home/heinemann/gpuunpacker/cbmroot/core/config",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/geosetup",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/KFQA",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich/utils",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/littrack/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libLittrack dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmLitGlobalElectronId.h")))  CbmLitGlobalElectronId;
class __attribute__((annotate("$clingAutoload$CbmLitFindMvdTracks.h")))  CbmLitFindMvdTracks;
class __attribute__((annotate("$clingAutoload$CbmLitFitTracksParallel.h")))  CbmLitFitTracksParallel;
class __attribute__((annotate("$clingAutoload$CbmLitFitTracks.h")))  CbmLitFitTracks;
class __attribute__((annotate("$clingAutoload$CbmLitPolarizedGenerator.h")))  CbmLitPolarizedGenerator;
class __attribute__((annotate("$clingAutoload$CbmLitFindGlobalTracks.h")))  CbmLitFindGlobalTracks;
class __attribute__((annotate("$clingAutoload$CbmLitFindGlobalTracksIdeal.h")))  CbmLitFindGlobalTracksIdeal;
class __attribute__((annotate("$clingAutoload$CbmLitFindGlobalTracksParallel.h")))  CbmLitFindGlobalTracksParallel;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libLittrack dictionary payload"

#ifndef HAVE_SSE
  #define HAVE_SSE 1
#endif
#ifndef HAVE_SSE
  #define HAVE_SSE 1
#endif
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
#include "CbmLitDetectorSetup.h"
#include "CbmLitGlobalElectronId.h"
#include "CbmLitFindMvdTracks.h"
#include "CbmLitFitTracksParallel.h"
#include "CbmLitFitTracks.h"
#include "CbmLitPolarizedGenerator.h"
#include "CbmLitHitData.h"
#include "CbmLitTrackFinderBranch.h"
#include "CbmLitTrackFinderNN.h"
#include "CbmLitNearestHitTofMerger.h"
#include "CbmLitAllHitsTofMerger.h"
#include "CbmLitTrackFitterImp.h"
#include "CbmLitKalmanFilter.h"
#include "CbmLitKalmanSmoother.h"
#include "CbmLitTrackFitterIter.h"
#include "CbmLitTGeoTrackPropagator.h"
#include "CbmLitRK4TrackExtrapolator.h"
#include "CbmLitLineTrackExtrapolator.h"
#include "CbmLitTGeoNavigator.h"
#include "CbmLitCleverTrackExtrapolator.h"
#include "CbmLitQualitySort.h"
#include "CbmLitTrackSelectionCuts.h"
#include "CbmLitTrackSelectionEmpty.h"
#include "CbmLitTrackSelectionMuch.h"
#include "CbmLitTrackSelectionSharedHits.h"
#include "CbmLitTrackSelectionTrd.h"
#include "CbmLitMath.h"
#include "CbmLitMatrixMath.h"
#include "CbmLitMaterialEffectsImp.h"
#include "CbmLitFieldGridCreator.h"
#include "CbmLitFieldFitter.h"
#include "CbmLitToolFactory.h"
#include "CbmLitTrackingGeometryConstructor.h"
#include "CbmLitFindGlobalTracks.h"
#include "CbmLitFindGlobalTracksIdeal.h"
#include "CbmLitFindGlobalTracksParallel.h"
#include "CbmLitConverterParallel.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmLitFindGlobalTracks", payloadCode, "@",
"CbmLitFindGlobalTracksIdeal", payloadCode, "@",
"CbmLitFindGlobalTracksParallel", payloadCode, "@",
"CbmLitFindMvdTracks", payloadCode, "@",
"CbmLitFitTracks", payloadCode, "@",
"CbmLitFitTracksParallel", payloadCode, "@",
"CbmLitGlobalElectronId", payloadCode, "@",
"CbmLitPolarizedGenerator", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libLittrack",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libLittrack_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libLittrack_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libLittrack() {
  TriggerDictionaryInitialization_libLittrack_Impl();
}
