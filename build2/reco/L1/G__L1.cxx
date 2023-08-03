// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__L1
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
#include "CbmL1Constants.h"
#include "CbmL1CATrdTrackFinderSA.h"
#include "CbmL1DetectorID.h"
#include "CbmL1MCPoint.h"
#include "CbmL1Hit.h"
#include "CbmL1Track.h"
#include "CbmL1TrackPar.h"
#include "CbmL1TrdHit.h"
#include "CbmL1Vtx.h"
#include "L1Def.h"
#include "L1Vector.h"
#include "L1EArray.h"
#include "L1Undef.h"
#include "CaToolsWindowFinder.h"
#include "CaToolsLinkKey.h"
#include "CaToolsHitRecord.h"
#include "CbmCaInputQaBase.h"
#include "CbmL1TrdTracklet.h"
#include "CbmL1TrdTracklet4.h"
#include "CbmL1.h"
#include "CbmL1TrackMerger.h"
#include "CbmL1TofMerger.h"
#include "CbmL1StsTrackFinder.h"
#include "CbmL1GlobalTrackFinder.h"
#include "CbmL1GlobalFindTracksEvents.h"
#include "CaAlgoRandom.h"
#include "L1Algo.h"
#include "L1TrackPar.h"
#include "L1Grid.h"
#include "CbmCaMCModule.h"
#include "CbmCaTimeSliceReader.h"
#include "L1Station.h"
#include "L1Fit.h"
#include "L1MCEvent.h"
#include "CbmL1MCTrack.h"
#include "CbmL1Track.h"
#include "L1Material.h"
#include "L1UMeasurementInfo.h"
#include "L1XYMeasurementInfo.h"
#include "L1Field.h"
#include "L1CAIteration.h"
#include "L1BaseStationInfo.h"
#include "L1InitManager.h"
#include "L1Parameters.h"
#include "L1InputData.h"
#include "L1IODataManager.h"
#include "L1CloneMerger.h"
#include "L1ConfigRW.h"
#include "L1SearchWindow.h"
#include "L1AlgoDraw.h"
#include "L1AlgoEfficiencyPerformance.h"
#include "L1AlgoPulls.h"
#include "CaToolsMCData.h"
#include "CaToolsMCPoint.h"
#include "CaToolsMCTrack.h"
#include "CaToolsQa.h"
#include "CaToolsDebugger.h"
#include "CaToolsWindowFinder.h"
#include "CaToolsWFExpression.h"
#include "CbmL1PFFitter.h"
#include "CbmL1PFMCParticle.h"
#include "CbmTrackerInputQaTrd.h"
#include "CbmTrackerInputQaTof.h"
#include "CbmTrackingInputQaSts.h"
#include "CbmCaInputQaSts.h"
#include "CbmCaInputQaMuch.h"
#include "CbmCaInputQaTrd.h"
#include "CbmCaInputQaTof.h"
#include "CbmCaOutputQa.h"
#include "CbmCaTrackTypeQa.h"
#include "CbmCaTrackFitQa.h"
#include "CbmTofInteraction.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_cacLcLtoolscLcLWindowFinder(void *p = nullptr);
   static void *newArray_cacLcLtoolscLcLWindowFinder(Long_t size, void *p);
   static void delete_cacLcLtoolscLcLWindowFinder(void *p);
   static void deleteArray_cacLcLtoolscLcLWindowFinder(void *p);
   static void destruct_cacLcLtoolscLcLWindowFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ca::tools::WindowFinder*)
   {
      ::ca::tools::WindowFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ca::tools::WindowFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ca::tools::WindowFinder", ::ca::tools::WindowFinder::Class_Version(), "CaToolsWindowFinder.h", 43,
                  typeid(::ca::tools::WindowFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ca::tools::WindowFinder::Dictionary, isa_proxy, 4,
                  sizeof(::ca::tools::WindowFinder) );
      instance.SetNew(&new_cacLcLtoolscLcLWindowFinder);
      instance.SetNewArray(&newArray_cacLcLtoolscLcLWindowFinder);
      instance.SetDelete(&delete_cacLcLtoolscLcLWindowFinder);
      instance.SetDeleteArray(&deleteArray_cacLcLtoolscLcLWindowFinder);
      instance.SetDestructor(&destruct_cacLcLtoolscLcLWindowFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ca::tools::WindowFinder*)
   {
      return GenerateInitInstanceLocal((::ca::tools::WindowFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ca::tools::WindowFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmL1TrdTracklet(void *p = nullptr);
   static void *newArray_CbmL1TrdTracklet(Long_t size, void *p);
   static void delete_CbmL1TrdTracklet(void *p);
   static void deleteArray_CbmL1TrdTracklet(void *p);
   static void destruct_CbmL1TrdTracklet(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmL1TrdTracklet*)
   {
      ::CbmL1TrdTracklet *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmL1TrdTracklet >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmL1TrdTracklet", ::CbmL1TrdTracklet::Class_Version(), "CbmL1TrdTracklet.h", 12,
                  typeid(::CbmL1TrdTracklet), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmL1TrdTracklet::Dictionary, isa_proxy, 4,
                  sizeof(::CbmL1TrdTracklet) );
      instance.SetNew(&new_CbmL1TrdTracklet);
      instance.SetNewArray(&newArray_CbmL1TrdTracklet);
      instance.SetDelete(&delete_CbmL1TrdTracklet);
      instance.SetDeleteArray(&deleteArray_CbmL1TrdTracklet);
      instance.SetDestructor(&destruct_CbmL1TrdTracklet);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmL1TrdTracklet*)
   {
      return GenerateInitInstanceLocal((::CbmL1TrdTracklet*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmL1TrdTracklet*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmL1TrdTracklet4(void *p = nullptr);
   static void *newArray_CbmL1TrdTracklet4(Long_t size, void *p);
   static void delete_CbmL1TrdTracklet4(void *p);
   static void deleteArray_CbmL1TrdTracklet4(void *p);
   static void destruct_CbmL1TrdTracklet4(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmL1TrdTracklet4*)
   {
      ::CbmL1TrdTracklet4 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmL1TrdTracklet4 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmL1TrdTracklet4", ::CbmL1TrdTracklet4::Class_Version(), "CbmL1TrdTracklet4.h", 12,
                  typeid(::CbmL1TrdTracklet4), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmL1TrdTracklet4::Dictionary, isa_proxy, 4,
                  sizeof(::CbmL1TrdTracklet4) );
      instance.SetNew(&new_CbmL1TrdTracklet4);
      instance.SetNewArray(&newArray_CbmL1TrdTracklet4);
      instance.SetDelete(&delete_CbmL1TrdTracklet4);
      instance.SetDeleteArray(&deleteArray_CbmL1TrdTracklet4);
      instance.SetDestructor(&destruct_CbmL1TrdTracklet4);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmL1TrdTracklet4*)
   {
      return GenerateInitInstanceLocal((::CbmL1TrdTracklet4*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmL1TrdTracklet4*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmL1(void *p = nullptr);
   static void *newArray_CbmL1(Long_t size, void *p);
   static void delete_CbmL1(void *p);
   static void deleteArray_CbmL1(void *p);
   static void destruct_CbmL1(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmL1*)
   {
      ::CbmL1 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmL1 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmL1", ::CbmL1::Class_Version(), "CbmL1.h", 115,
                  typeid(::CbmL1), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmL1::Dictionary, isa_proxy, 4,
                  sizeof(::CbmL1) );
      instance.SetNew(&new_CbmL1);
      instance.SetNewArray(&newArray_CbmL1);
      instance.SetDelete(&delete_CbmL1);
      instance.SetDeleteArray(&deleteArray_CbmL1);
      instance.SetDestructor(&destruct_CbmL1);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmL1*)
   {
      return GenerateInitInstanceLocal((::CbmL1*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmL1*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmL1TrackMerger(void *p = nullptr);
   static void *newArray_CbmL1TrackMerger(Long_t size, void *p);
   static void delete_CbmL1TrackMerger(void *p);
   static void deleteArray_CbmL1TrackMerger(void *p);
   static void destruct_CbmL1TrackMerger(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmL1TrackMerger*)
   {
      ::CbmL1TrackMerger *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmL1TrackMerger >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmL1TrackMerger", ::CbmL1TrackMerger::Class_Version(), "CbmL1TrackMerger.h", 20,
                  typeid(::CbmL1TrackMerger), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmL1TrackMerger::Dictionary, isa_proxy, 4,
                  sizeof(::CbmL1TrackMerger) );
      instance.SetNew(&new_CbmL1TrackMerger);
      instance.SetNewArray(&newArray_CbmL1TrackMerger);
      instance.SetDelete(&delete_CbmL1TrackMerger);
      instance.SetDeleteArray(&deleteArray_CbmL1TrackMerger);
      instance.SetDestructor(&destruct_CbmL1TrackMerger);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmL1TrackMerger*)
   {
      return GenerateInitInstanceLocal((::CbmL1TrackMerger*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmL1TrackMerger*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmL1TofMerger(void *p = nullptr);
   static void *newArray_CbmL1TofMerger(Long_t size, void *p);
   static void delete_CbmL1TofMerger(void *p);
   static void deleteArray_CbmL1TofMerger(void *p);
   static void destruct_CbmL1TofMerger(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmL1TofMerger*)
   {
      ::CbmL1TofMerger *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmL1TofMerger >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmL1TofMerger", ::CbmL1TofMerger::Class_Version(), "CbmL1TofMerger.h", 20,
                  typeid(::CbmL1TofMerger), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmL1TofMerger::Dictionary, isa_proxy, 4,
                  sizeof(::CbmL1TofMerger) );
      instance.SetNew(&new_CbmL1TofMerger);
      instance.SetNewArray(&newArray_CbmL1TofMerger);
      instance.SetDelete(&delete_CbmL1TofMerger);
      instance.SetDeleteArray(&deleteArray_CbmL1TofMerger);
      instance.SetDestructor(&destruct_CbmL1TofMerger);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmL1TofMerger*)
   {
      return GenerateInitInstanceLocal((::CbmL1TofMerger*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmL1TofMerger*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmL1StsTrackFinder(void *p = nullptr);
   static void *newArray_CbmL1StsTrackFinder(Long_t size, void *p);
   static void delete_CbmL1StsTrackFinder(void *p);
   static void deleteArray_CbmL1StsTrackFinder(void *p);
   static void destruct_CbmL1StsTrackFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmL1StsTrackFinder*)
   {
      ::CbmL1StsTrackFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmL1StsTrackFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmL1StsTrackFinder", ::CbmL1StsTrackFinder::Class_Version(), "CbmL1StsTrackFinder.h", 32,
                  typeid(::CbmL1StsTrackFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmL1StsTrackFinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmL1StsTrackFinder) );
      instance.SetNew(&new_CbmL1StsTrackFinder);
      instance.SetNewArray(&newArray_CbmL1StsTrackFinder);
      instance.SetDelete(&delete_CbmL1StsTrackFinder);
      instance.SetDeleteArray(&deleteArray_CbmL1StsTrackFinder);
      instance.SetDestructor(&destruct_CbmL1StsTrackFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmL1StsTrackFinder*)
   {
      return GenerateInitInstanceLocal((::CbmL1StsTrackFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmL1StsTrackFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmL1GlobalTrackFinder(void *p = nullptr);
   static void *newArray_CbmL1GlobalTrackFinder(Long_t size, void *p);
   static void delete_CbmL1GlobalTrackFinder(void *p);
   static void deleteArray_CbmL1GlobalTrackFinder(void *p);
   static void destruct_CbmL1GlobalTrackFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmL1GlobalTrackFinder*)
   {
      ::CbmL1GlobalTrackFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmL1GlobalTrackFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmL1GlobalTrackFinder", ::CbmL1GlobalTrackFinder::Class_Version(), "CbmL1GlobalTrackFinder.h", 39,
                  typeid(::CbmL1GlobalTrackFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmL1GlobalTrackFinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmL1GlobalTrackFinder) );
      instance.SetNew(&new_CbmL1GlobalTrackFinder);
      instance.SetNewArray(&newArray_CbmL1GlobalTrackFinder);
      instance.SetDelete(&delete_CbmL1GlobalTrackFinder);
      instance.SetDeleteArray(&deleteArray_CbmL1GlobalTrackFinder);
      instance.SetDestructor(&destruct_CbmL1GlobalTrackFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmL1GlobalTrackFinder*)
   {
      return GenerateInitInstanceLocal((::CbmL1GlobalTrackFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmL1GlobalTrackFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmL1GlobalFindTracksEvents(void *p = nullptr);
   static void *newArray_CbmL1GlobalFindTracksEvents(Long_t size, void *p);
   static void delete_CbmL1GlobalFindTracksEvents(void *p);
   static void deleteArray_CbmL1GlobalFindTracksEvents(void *p);
   static void destruct_CbmL1GlobalFindTracksEvents(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmL1GlobalFindTracksEvents*)
   {
      ::CbmL1GlobalFindTracksEvents *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmL1GlobalFindTracksEvents >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmL1GlobalFindTracksEvents", ::CbmL1GlobalFindTracksEvents::Class_Version(), "CbmL1GlobalFindTracksEvents.h", 40,
                  typeid(::CbmL1GlobalFindTracksEvents), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmL1GlobalFindTracksEvents::Dictionary, isa_proxy, 4,
                  sizeof(::CbmL1GlobalFindTracksEvents) );
      instance.SetNew(&new_CbmL1GlobalFindTracksEvents);
      instance.SetNewArray(&newArray_CbmL1GlobalFindTracksEvents);
      instance.SetDelete(&delete_CbmL1GlobalFindTracksEvents);
      instance.SetDeleteArray(&deleteArray_CbmL1GlobalFindTracksEvents);
      instance.SetDestructor(&destruct_CbmL1GlobalFindTracksEvents);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmL1GlobalFindTracksEvents*)
   {
      return GenerateInitInstanceLocal((::CbmL1GlobalFindTracksEvents*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmL1GlobalFindTracksEvents*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrackerInputQaTrd(void *p = nullptr);
   static void *newArray_CbmTrackerInputQaTrd(Long_t size, void *p);
   static void delete_CbmTrackerInputQaTrd(void *p);
   static void deleteArray_CbmTrackerInputQaTrd(void *p);
   static void destruct_CbmTrackerInputQaTrd(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrackerInputQaTrd*)
   {
      ::CbmTrackerInputQaTrd *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrackerInputQaTrd >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrackerInputQaTrd", ::CbmTrackerInputQaTrd::Class_Version(), "CbmTrackerInputQaTrd.h", 43,
                  typeid(::CbmTrackerInputQaTrd), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrackerInputQaTrd::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrackerInputQaTrd) );
      instance.SetNew(&new_CbmTrackerInputQaTrd);
      instance.SetNewArray(&newArray_CbmTrackerInputQaTrd);
      instance.SetDelete(&delete_CbmTrackerInputQaTrd);
      instance.SetDeleteArray(&deleteArray_CbmTrackerInputQaTrd);
      instance.SetDestructor(&destruct_CbmTrackerInputQaTrd);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrackerInputQaTrd*)
   {
      return GenerateInitInstanceLocal((::CbmTrackerInputQaTrd*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrackerInputQaTrd*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrackerInputQaTof(void *p = nullptr);
   static void *newArray_CbmTrackerInputQaTof(Long_t size, void *p);
   static void delete_CbmTrackerInputQaTof(void *p);
   static void deleteArray_CbmTrackerInputQaTof(void *p);
   static void destruct_CbmTrackerInputQaTof(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrackerInputQaTof*)
   {
      ::CbmTrackerInputQaTof *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrackerInputQaTof >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrackerInputQaTof", ::CbmTrackerInputQaTof::Class_Version(), "CbmTrackerInputQaTof.h", 43,
                  typeid(::CbmTrackerInputQaTof), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrackerInputQaTof::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrackerInputQaTof) );
      instance.SetNew(&new_CbmTrackerInputQaTof);
      instance.SetNewArray(&newArray_CbmTrackerInputQaTof);
      instance.SetDelete(&delete_CbmTrackerInputQaTof);
      instance.SetDeleteArray(&deleteArray_CbmTrackerInputQaTof);
      instance.SetDestructor(&destruct_CbmTrackerInputQaTof);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrackerInputQaTof*)
   {
      return GenerateInitInstanceLocal((::CbmTrackerInputQaTof*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrackerInputQaTof*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrackingInputQaSts(void *p = nullptr);
   static void *newArray_CbmTrackingInputQaSts(Long_t size, void *p);
   static void delete_CbmTrackingInputQaSts(void *p);
   static void deleteArray_CbmTrackingInputQaSts(void *p);
   static void destruct_CbmTrackingInputQaSts(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrackingInputQaSts*)
   {
      ::CbmTrackingInputQaSts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrackingInputQaSts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrackingInputQaSts", ::CbmTrackingInputQaSts::Class_Version(), "CbmTrackingInputQaSts.h", 48,
                  typeid(::CbmTrackingInputQaSts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrackingInputQaSts::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrackingInputQaSts) );
      instance.SetNew(&new_CbmTrackingInputQaSts);
      instance.SetNewArray(&newArray_CbmTrackingInputQaSts);
      instance.SetDelete(&delete_CbmTrackingInputQaSts);
      instance.SetDeleteArray(&deleteArray_CbmTrackingInputQaSts);
      instance.SetDestructor(&destruct_CbmTrackingInputQaSts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrackingInputQaSts*)
   {
      return GenerateInitInstanceLocal((::CbmTrackingInputQaSts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrackingInputQaSts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmCaInputQaSts(void *p);
   static void deleteArray_CbmCaInputQaSts(void *p);
   static void destruct_CbmCaInputQaSts(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCaInputQaSts*)
   {
      ::CbmCaInputQaSts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCaInputQaSts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCaInputQaSts", ::CbmCaInputQaSts::Class_Version(), "CbmCaInputQaSts.h", 37,
                  typeid(::CbmCaInputQaSts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCaInputQaSts::Dictionary, isa_proxy, 4,
                  sizeof(::CbmCaInputQaSts) );
      instance.SetDelete(&delete_CbmCaInputQaSts);
      instance.SetDeleteArray(&deleteArray_CbmCaInputQaSts);
      instance.SetDestructor(&destruct_CbmCaInputQaSts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCaInputQaSts*)
   {
      return GenerateInitInstanceLocal((::CbmCaInputQaSts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCaInputQaSts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmCaInputQaMuch(void *p);
   static void deleteArray_CbmCaInputQaMuch(void *p);
   static void destruct_CbmCaInputQaMuch(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCaInputQaMuch*)
   {
      ::CbmCaInputQaMuch *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCaInputQaMuch >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCaInputQaMuch", ::CbmCaInputQaMuch::Class_Version(), "CbmCaInputQaMuch.h", 36,
                  typeid(::CbmCaInputQaMuch), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCaInputQaMuch::Dictionary, isa_proxy, 4,
                  sizeof(::CbmCaInputQaMuch) );
      instance.SetDelete(&delete_CbmCaInputQaMuch);
      instance.SetDeleteArray(&deleteArray_CbmCaInputQaMuch);
      instance.SetDestructor(&destruct_CbmCaInputQaMuch);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCaInputQaMuch*)
   {
      return GenerateInitInstanceLocal((::CbmCaInputQaMuch*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCaInputQaMuch*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmCaInputQaTrd(void *p);
   static void deleteArray_CbmCaInputQaTrd(void *p);
   static void destruct_CbmCaInputQaTrd(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCaInputQaTrd*)
   {
      ::CbmCaInputQaTrd *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCaInputQaTrd >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCaInputQaTrd", ::CbmCaInputQaTrd::Class_Version(), "CbmCaInputQaTrd.h", 37,
                  typeid(::CbmCaInputQaTrd), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCaInputQaTrd::Dictionary, isa_proxy, 4,
                  sizeof(::CbmCaInputQaTrd) );
      instance.SetDelete(&delete_CbmCaInputQaTrd);
      instance.SetDeleteArray(&deleteArray_CbmCaInputQaTrd);
      instance.SetDestructor(&destruct_CbmCaInputQaTrd);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCaInputQaTrd*)
   {
      return GenerateInitInstanceLocal((::CbmCaInputQaTrd*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCaInputQaTrd*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmCaInputQaTof(void *p);
   static void deleteArray_CbmCaInputQaTof(void *p);
   static void destruct_CbmCaInputQaTof(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCaInputQaTof*)
   {
      ::CbmCaInputQaTof *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCaInputQaTof >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCaInputQaTof", ::CbmCaInputQaTof::Class_Version(), "CbmCaInputQaTof.h", 39,
                  typeid(::CbmCaInputQaTof), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCaInputQaTof::Dictionary, isa_proxy, 4,
                  sizeof(::CbmCaInputQaTof) );
      instance.SetDelete(&delete_CbmCaInputQaTof);
      instance.SetDeleteArray(&deleteArray_CbmCaInputQaTof);
      instance.SetDestructor(&destruct_CbmCaInputQaTof);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCaInputQaTof*)
   {
      return GenerateInitInstanceLocal((::CbmCaInputQaTof*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCaInputQaTof*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_cbmcLcLcacLcLOutputQa(void *p);
   static void deleteArray_cbmcLcLcacLcLOutputQa(void *p);
   static void destruct_cbmcLcLcacLcLOutputQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cbm::ca::OutputQa*)
   {
      ::cbm::ca::OutputQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cbm::ca::OutputQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("cbm::ca::OutputQa", ::cbm::ca::OutputQa::Class_Version(), "CbmCaOutputQa.h", 92,
                  typeid(::cbm::ca::OutputQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::cbm::ca::OutputQa::Dictionary, isa_proxy, 4,
                  sizeof(::cbm::ca::OutputQa) );
      instance.SetDelete(&delete_cbmcLcLcacLcLOutputQa);
      instance.SetDeleteArray(&deleteArray_cbmcLcLcacLcLOutputQa);
      instance.SetDestructor(&destruct_cbmcLcLcacLcLOutputQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cbm::ca::OutputQa*)
   {
      return GenerateInitInstanceLocal((::cbm::ca::OutputQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cbm::ca::OutputQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ca {
   namespace tools {
//______________________________________________________________________________
atomic_TClass_ptr WindowFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *WindowFinder::Class_Name()
{
   return "ca::tools::WindowFinder";
}

//______________________________________________________________________________
const char *WindowFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ca::tools::WindowFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int WindowFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ca::tools::WindowFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *WindowFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ca::tools::WindowFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *WindowFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ca::tools::WindowFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace ca::tools
} // namespace ca::tools
//______________________________________________________________________________
atomic_TClass_ptr CbmL1TrdTracklet::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmL1TrdTracklet::Class_Name()
{
   return "CbmL1TrdTracklet";
}

//______________________________________________________________________________
const char *CbmL1TrdTracklet::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TrdTracklet*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmL1TrdTracklet::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TrdTracklet*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmL1TrdTracklet::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TrdTracklet*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmL1TrdTracklet::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TrdTracklet*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmL1TrdTracklet4::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmL1TrdTracklet4::Class_Name()
{
   return "CbmL1TrdTracklet4";
}

//______________________________________________________________________________
const char *CbmL1TrdTracklet4::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TrdTracklet4*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmL1TrdTracklet4::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TrdTracklet4*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmL1TrdTracklet4::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TrdTracklet4*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmL1TrdTracklet4::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TrdTracklet4*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmL1::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmL1::Class_Name()
{
   return "CbmL1";
}

//______________________________________________________________________________
const char *CbmL1::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmL1::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmL1::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmL1::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmL1TrackMerger::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmL1TrackMerger::Class_Name()
{
   return "CbmL1TrackMerger";
}

//______________________________________________________________________________
const char *CbmL1TrackMerger::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TrackMerger*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmL1TrackMerger::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TrackMerger*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmL1TrackMerger::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TrackMerger*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmL1TrackMerger::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TrackMerger*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmL1TofMerger::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmL1TofMerger::Class_Name()
{
   return "CbmL1TofMerger";
}

//______________________________________________________________________________
const char *CbmL1TofMerger::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TofMerger*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmL1TofMerger::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TofMerger*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmL1TofMerger::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TofMerger*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmL1TofMerger::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1TofMerger*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmL1StsTrackFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmL1StsTrackFinder::Class_Name()
{
   return "CbmL1StsTrackFinder";
}

//______________________________________________________________________________
const char *CbmL1StsTrackFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1StsTrackFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmL1StsTrackFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1StsTrackFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmL1StsTrackFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1StsTrackFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmL1StsTrackFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1StsTrackFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmL1GlobalTrackFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmL1GlobalTrackFinder::Class_Name()
{
   return "CbmL1GlobalTrackFinder";
}

//______________________________________________________________________________
const char *CbmL1GlobalTrackFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1GlobalTrackFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmL1GlobalTrackFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1GlobalTrackFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmL1GlobalTrackFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1GlobalTrackFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmL1GlobalTrackFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1GlobalTrackFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmL1GlobalFindTracksEvents::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmL1GlobalFindTracksEvents::Class_Name()
{
   return "CbmL1GlobalFindTracksEvents";
}

//______________________________________________________________________________
const char *CbmL1GlobalFindTracksEvents::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1GlobalFindTracksEvents*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmL1GlobalFindTracksEvents::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1GlobalFindTracksEvents*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmL1GlobalFindTracksEvents::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1GlobalFindTracksEvents*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmL1GlobalFindTracksEvents::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1GlobalFindTracksEvents*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrackerInputQaTrd::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrackerInputQaTrd::Class_Name()
{
   return "CbmTrackerInputQaTrd";
}

//______________________________________________________________________________
const char *CbmTrackerInputQaTrd::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackerInputQaTrd*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrackerInputQaTrd::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackerInputQaTrd*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrackerInputQaTrd::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackerInputQaTrd*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrackerInputQaTrd::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackerInputQaTrd*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrackerInputQaTof::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrackerInputQaTof::Class_Name()
{
   return "CbmTrackerInputQaTof";
}

//______________________________________________________________________________
const char *CbmTrackerInputQaTof::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackerInputQaTof*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrackerInputQaTof::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackerInputQaTof*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrackerInputQaTof::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackerInputQaTof*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrackerInputQaTof::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackerInputQaTof*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrackingInputQaSts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrackingInputQaSts::Class_Name()
{
   return "CbmTrackingInputQaSts";
}

//______________________________________________________________________________
const char *CbmTrackingInputQaSts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackingInputQaSts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrackingInputQaSts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackingInputQaSts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrackingInputQaSts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackingInputQaSts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrackingInputQaSts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackingInputQaSts*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCaInputQaSts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCaInputQaSts::Class_Name()
{
   return "CbmCaInputQaSts";
}

//______________________________________________________________________________
const char *CbmCaInputQaSts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaSts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCaInputQaSts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaSts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCaInputQaSts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaSts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCaInputQaSts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaSts*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCaInputQaMuch::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCaInputQaMuch::Class_Name()
{
   return "CbmCaInputQaMuch";
}

//______________________________________________________________________________
const char *CbmCaInputQaMuch::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaMuch*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCaInputQaMuch::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaMuch*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCaInputQaMuch::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaMuch*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCaInputQaMuch::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaMuch*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCaInputQaTrd::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCaInputQaTrd::Class_Name()
{
   return "CbmCaInputQaTrd";
}

//______________________________________________________________________________
const char *CbmCaInputQaTrd::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaTrd*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCaInputQaTrd::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaTrd*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCaInputQaTrd::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaTrd*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCaInputQaTrd::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaTrd*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCaInputQaTof::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCaInputQaTof::Class_Name()
{
   return "CbmCaInputQaTof";
}

//______________________________________________________________________________
const char *CbmCaInputQaTof::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaTof*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCaInputQaTof::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaTof*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCaInputQaTof::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaTof*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCaInputQaTof::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCaInputQaTof*)nullptr)->GetClass(); }
   return fgIsA;
}

namespace cbm {
   namespace ca {
//______________________________________________________________________________
atomic_TClass_ptr OutputQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *OutputQa::Class_Name()
{
   return "cbm::ca::OutputQa";
}

//______________________________________________________________________________
const char *OutputQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cbm::ca::OutputQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int OutputQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cbm::ca::OutputQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *OutputQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cbm::ca::OutputQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *OutputQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cbm::ca::OutputQa*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace cbm::ca
} // namespace cbm::ca
namespace ca {
   namespace tools {
//______________________________________________________________________________
void WindowFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class ca::tools::WindowFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ca::tools::WindowFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(ca::tools::WindowFinder::Class(),this);
   }
}

} // namespace ca::tools
} // namespace ca::tools
namespace ROOT {
   // Wrappers around operator new
   static void *new_cacLcLtoolscLcLWindowFinder(void *p) {
      return  p ? new(p) ::ca::tools::WindowFinder : new ::ca::tools::WindowFinder;
   }
   static void *newArray_cacLcLtoolscLcLWindowFinder(Long_t nElements, void *p) {
      return p ? new(p) ::ca::tools::WindowFinder[nElements] : new ::ca::tools::WindowFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_cacLcLtoolscLcLWindowFinder(void *p) {
      delete ((::ca::tools::WindowFinder*)p);
   }
   static void deleteArray_cacLcLtoolscLcLWindowFinder(void *p) {
      delete [] ((::ca::tools::WindowFinder*)p);
   }
   static void destruct_cacLcLtoolscLcLWindowFinder(void *p) {
      typedef ::ca::tools::WindowFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ca::tools::WindowFinder

//______________________________________________________________________________
void CbmL1TrdTracklet::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmL1TrdTracklet.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmL1TrdTracklet::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmL1TrdTracklet::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmL1TrdTracklet(void *p) {
      return  p ? new(p) ::CbmL1TrdTracklet : new ::CbmL1TrdTracklet;
   }
   static void *newArray_CbmL1TrdTracklet(Long_t nElements, void *p) {
      return p ? new(p) ::CbmL1TrdTracklet[nElements] : new ::CbmL1TrdTracklet[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmL1TrdTracklet(void *p) {
      delete ((::CbmL1TrdTracklet*)p);
   }
   static void deleteArray_CbmL1TrdTracklet(void *p) {
      delete [] ((::CbmL1TrdTracklet*)p);
   }
   static void destruct_CbmL1TrdTracklet(void *p) {
      typedef ::CbmL1TrdTracklet current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmL1TrdTracklet

//______________________________________________________________________________
void CbmL1TrdTracklet4::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmL1TrdTracklet4.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmL1TrdTracklet4::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmL1TrdTracklet4::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmL1TrdTracklet4(void *p) {
      return  p ? new(p) ::CbmL1TrdTracklet4 : new ::CbmL1TrdTracklet4;
   }
   static void *newArray_CbmL1TrdTracklet4(Long_t nElements, void *p) {
      return p ? new(p) ::CbmL1TrdTracklet4[nElements] : new ::CbmL1TrdTracklet4[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmL1TrdTracklet4(void *p) {
      delete ((::CbmL1TrdTracklet4*)p);
   }
   static void deleteArray_CbmL1TrdTracklet4(void *p) {
      delete [] ((::CbmL1TrdTracklet4*)p);
   }
   static void destruct_CbmL1TrdTracklet4(void *p) {
      typedef ::CbmL1TrdTracklet4 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmL1TrdTracklet4

//______________________________________________________________________________
void CbmL1::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmL1.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmL1::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmL1::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmL1(void *p) {
      return  p ? new(p) ::CbmL1 : new ::CbmL1;
   }
   static void *newArray_CbmL1(Long_t nElements, void *p) {
      return p ? new(p) ::CbmL1[nElements] : new ::CbmL1[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmL1(void *p) {
      delete ((::CbmL1*)p);
   }
   static void deleteArray_CbmL1(void *p) {
      delete [] ((::CbmL1*)p);
   }
   static void destruct_CbmL1(void *p) {
      typedef ::CbmL1 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmL1

//______________________________________________________________________________
void CbmL1TrackMerger::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmL1TrackMerger.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmL1TrackMerger::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmL1TrackMerger::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmL1TrackMerger(void *p) {
      return  p ? new(p) ::CbmL1TrackMerger : new ::CbmL1TrackMerger;
   }
   static void *newArray_CbmL1TrackMerger(Long_t nElements, void *p) {
      return p ? new(p) ::CbmL1TrackMerger[nElements] : new ::CbmL1TrackMerger[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmL1TrackMerger(void *p) {
      delete ((::CbmL1TrackMerger*)p);
   }
   static void deleteArray_CbmL1TrackMerger(void *p) {
      delete [] ((::CbmL1TrackMerger*)p);
   }
   static void destruct_CbmL1TrackMerger(void *p) {
      typedef ::CbmL1TrackMerger current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmL1TrackMerger

//______________________________________________________________________________
void CbmL1TofMerger::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmL1TofMerger.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmL1TofMerger::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmL1TofMerger::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmL1TofMerger(void *p) {
      return  p ? new(p) ::CbmL1TofMerger : new ::CbmL1TofMerger;
   }
   static void *newArray_CbmL1TofMerger(Long_t nElements, void *p) {
      return p ? new(p) ::CbmL1TofMerger[nElements] : new ::CbmL1TofMerger[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmL1TofMerger(void *p) {
      delete ((::CbmL1TofMerger*)p);
   }
   static void deleteArray_CbmL1TofMerger(void *p) {
      delete [] ((::CbmL1TofMerger*)p);
   }
   static void destruct_CbmL1TofMerger(void *p) {
      typedef ::CbmL1TofMerger current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmL1TofMerger

//______________________________________________________________________________
void CbmL1StsTrackFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmL1StsTrackFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmL1StsTrackFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmL1StsTrackFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmL1StsTrackFinder(void *p) {
      return  p ? new(p) ::CbmL1StsTrackFinder : new ::CbmL1StsTrackFinder;
   }
   static void *newArray_CbmL1StsTrackFinder(Long_t nElements, void *p) {
      return p ? new(p) ::CbmL1StsTrackFinder[nElements] : new ::CbmL1StsTrackFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmL1StsTrackFinder(void *p) {
      delete ((::CbmL1StsTrackFinder*)p);
   }
   static void deleteArray_CbmL1StsTrackFinder(void *p) {
      delete [] ((::CbmL1StsTrackFinder*)p);
   }
   static void destruct_CbmL1StsTrackFinder(void *p) {
      typedef ::CbmL1StsTrackFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmL1StsTrackFinder

//______________________________________________________________________________
void CbmL1GlobalTrackFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmL1GlobalTrackFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmL1GlobalTrackFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmL1GlobalTrackFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmL1GlobalTrackFinder(void *p) {
      return  p ? new(p) ::CbmL1GlobalTrackFinder : new ::CbmL1GlobalTrackFinder;
   }
   static void *newArray_CbmL1GlobalTrackFinder(Long_t nElements, void *p) {
      return p ? new(p) ::CbmL1GlobalTrackFinder[nElements] : new ::CbmL1GlobalTrackFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmL1GlobalTrackFinder(void *p) {
      delete ((::CbmL1GlobalTrackFinder*)p);
   }
   static void deleteArray_CbmL1GlobalTrackFinder(void *p) {
      delete [] ((::CbmL1GlobalTrackFinder*)p);
   }
   static void destruct_CbmL1GlobalTrackFinder(void *p) {
      typedef ::CbmL1GlobalTrackFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmL1GlobalTrackFinder

//______________________________________________________________________________
void CbmL1GlobalFindTracksEvents::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmL1GlobalFindTracksEvents.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmL1GlobalFindTracksEvents::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmL1GlobalFindTracksEvents::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmL1GlobalFindTracksEvents(void *p) {
      return  p ? new(p) ::CbmL1GlobalFindTracksEvents : new ::CbmL1GlobalFindTracksEvents;
   }
   static void *newArray_CbmL1GlobalFindTracksEvents(Long_t nElements, void *p) {
      return p ? new(p) ::CbmL1GlobalFindTracksEvents[nElements] : new ::CbmL1GlobalFindTracksEvents[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmL1GlobalFindTracksEvents(void *p) {
      delete ((::CbmL1GlobalFindTracksEvents*)p);
   }
   static void deleteArray_CbmL1GlobalFindTracksEvents(void *p) {
      delete [] ((::CbmL1GlobalFindTracksEvents*)p);
   }
   static void destruct_CbmL1GlobalFindTracksEvents(void *p) {
      typedef ::CbmL1GlobalFindTracksEvents current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmL1GlobalFindTracksEvents

//______________________________________________________________________________
void CbmTrackerInputQaTrd::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrackerInputQaTrd.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrackerInputQaTrd::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrackerInputQaTrd::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrackerInputQaTrd(void *p) {
      return  p ? new(p) ::CbmTrackerInputQaTrd : new ::CbmTrackerInputQaTrd;
   }
   static void *newArray_CbmTrackerInputQaTrd(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrackerInputQaTrd[nElements] : new ::CbmTrackerInputQaTrd[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrackerInputQaTrd(void *p) {
      delete ((::CbmTrackerInputQaTrd*)p);
   }
   static void deleteArray_CbmTrackerInputQaTrd(void *p) {
      delete [] ((::CbmTrackerInputQaTrd*)p);
   }
   static void destruct_CbmTrackerInputQaTrd(void *p) {
      typedef ::CbmTrackerInputQaTrd current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrackerInputQaTrd

//______________________________________________________________________________
void CbmTrackerInputQaTof::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrackerInputQaTof.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrackerInputQaTof::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrackerInputQaTof::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrackerInputQaTof(void *p) {
      return  p ? new(p) ::CbmTrackerInputQaTof : new ::CbmTrackerInputQaTof;
   }
   static void *newArray_CbmTrackerInputQaTof(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrackerInputQaTof[nElements] : new ::CbmTrackerInputQaTof[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrackerInputQaTof(void *p) {
      delete ((::CbmTrackerInputQaTof*)p);
   }
   static void deleteArray_CbmTrackerInputQaTof(void *p) {
      delete [] ((::CbmTrackerInputQaTof*)p);
   }
   static void destruct_CbmTrackerInputQaTof(void *p) {
      typedef ::CbmTrackerInputQaTof current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrackerInputQaTof

//______________________________________________________________________________
void CbmTrackingInputQaSts::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrackingInputQaSts.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrackingInputQaSts::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrackingInputQaSts::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrackingInputQaSts(void *p) {
      return  p ? new(p) ::CbmTrackingInputQaSts : new ::CbmTrackingInputQaSts;
   }
   static void *newArray_CbmTrackingInputQaSts(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrackingInputQaSts[nElements] : new ::CbmTrackingInputQaSts[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrackingInputQaSts(void *p) {
      delete ((::CbmTrackingInputQaSts*)p);
   }
   static void deleteArray_CbmTrackingInputQaSts(void *p) {
      delete [] ((::CbmTrackingInputQaSts*)p);
   }
   static void destruct_CbmTrackingInputQaSts(void *p) {
      typedef ::CbmTrackingInputQaSts current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrackingInputQaSts

//______________________________________________________________________________
void CbmCaInputQaSts::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCaInputQaSts.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmCaInputQaSts::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmCaInputQaSts::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmCaInputQaSts(void *p) {
      delete ((::CbmCaInputQaSts*)p);
   }
   static void deleteArray_CbmCaInputQaSts(void *p) {
      delete [] ((::CbmCaInputQaSts*)p);
   }
   static void destruct_CbmCaInputQaSts(void *p) {
      typedef ::CbmCaInputQaSts current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmCaInputQaSts

//______________________________________________________________________________
void CbmCaInputQaMuch::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCaInputQaMuch.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmCaInputQaMuch::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmCaInputQaMuch::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmCaInputQaMuch(void *p) {
      delete ((::CbmCaInputQaMuch*)p);
   }
   static void deleteArray_CbmCaInputQaMuch(void *p) {
      delete [] ((::CbmCaInputQaMuch*)p);
   }
   static void destruct_CbmCaInputQaMuch(void *p) {
      typedef ::CbmCaInputQaMuch current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmCaInputQaMuch

//______________________________________________________________________________
void CbmCaInputQaTrd::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCaInputQaTrd.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmCaInputQaTrd::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmCaInputQaTrd::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmCaInputQaTrd(void *p) {
      delete ((::CbmCaInputQaTrd*)p);
   }
   static void deleteArray_CbmCaInputQaTrd(void *p) {
      delete [] ((::CbmCaInputQaTrd*)p);
   }
   static void destruct_CbmCaInputQaTrd(void *p) {
      typedef ::CbmCaInputQaTrd current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmCaInputQaTrd

//______________________________________________________________________________
void CbmCaInputQaTof::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCaInputQaTof.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmCaInputQaTof::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmCaInputQaTof::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmCaInputQaTof(void *p) {
      delete ((::CbmCaInputQaTof*)p);
   }
   static void deleteArray_CbmCaInputQaTof(void *p) {
      delete [] ((::CbmCaInputQaTof*)p);
   }
   static void destruct_CbmCaInputQaTof(void *p) {
      typedef ::CbmCaInputQaTof current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmCaInputQaTof

namespace cbm {
   namespace ca {
//______________________________________________________________________________
void OutputQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class cbm::ca::OutputQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cbm::ca::OutputQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(cbm::ca::OutputQa::Class(),this);
   }
}

} // namespace cbm::ca
} // namespace cbm::ca
namespace ROOT {
   // Wrapper around operator delete
   static void delete_cbmcLcLcacLcLOutputQa(void *p) {
      delete ((::cbm::ca::OutputQa*)p);
   }
   static void deleteArray_cbmcLcLcacLcLOutputQa(void *p) {
      delete [] ((::cbm::ca::OutputQa*)p);
   }
   static void destruct_cbmcLcLcacLcLOutputQa(void *p) {
      typedef ::cbm::ca::OutputQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cbm::ca::OutputQa

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

namespace {
  void TriggerDictionaryInitialization_libL1_Impl() {
    static const char* headers[] = {
"CbmL1Constants.h",
"CbmL1CATrdTrackFinderSA.h",
"CbmL1DetectorID.h",
"CbmL1MCPoint.h",
"CbmL1Hit.h",
"CbmL1Track.h",
"CbmL1TrackPar.h",
"CbmL1TrdHit.h",
"CbmL1Vtx.h",
"L1Def.h",
"L1Vector.h",
"L1EArray.h",
"L1Undef.h",
"CaToolsWindowFinder.h",
"CaToolsLinkKey.h",
"CaToolsHitRecord.h",
"CbmCaInputQaBase.h",
"CbmL1TrdTracklet.h",
"CbmL1TrdTracklet4.h",
"CbmL1.h",
"CbmL1TrackMerger.h",
"CbmL1TofMerger.h",
"CbmL1StsTrackFinder.h",
"CbmL1GlobalTrackFinder.h",
"CbmL1GlobalFindTracksEvents.h",
"CaAlgoRandom.h",
"L1Algo.h",
"L1TrackPar.h",
"L1Grid.h",
"CbmCaMCModule.h",
"CbmCaTimeSliceReader.h",
"L1Station.h",
"L1Fit.h",
"L1MCEvent.h",
"CbmL1MCTrack.h",
"CbmL1Track.h",
"L1Material.h",
"L1UMeasurementInfo.h",
"L1XYMeasurementInfo.h",
"L1Field.h",
"L1CAIteration.h",
"L1BaseStationInfo.h",
"L1InitManager.h",
"L1Parameters.h",
"L1InputData.h",
"L1IODataManager.h",
"L1CloneMerger.h",
"L1ConfigRW.h",
"L1SearchWindow.h",
"L1AlgoDraw.h",
"L1AlgoEfficiencyPerformance.h",
"L1AlgoPulls.h",
"CaToolsMCData.h",
"CaToolsMCPoint.h",
"CaToolsMCTrack.h",
"CaToolsQa.h",
"CaToolsDebugger.h",
"CaToolsWindowFinder.h",
"CaToolsWFExpression.h",
"CbmL1PFFitter.h",
"CbmL1PFMCParticle.h",
"CbmTrackerInputQaTrd.h",
"CbmTrackerInputQaTof.h",
"CbmTrackingInputQaSts.h",
"CbmCaInputQaSts.h",
"CbmCaInputQaMuch.h",
"CbmCaInputQaTrd.h",
"CbmCaInputQaTof.h",
"CbmCaOutputQa.h",
"CbmCaTrackTypeQa.h",
"CbmCaTrackFitQa.h",
"CbmTofInteraction.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca/simd",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/OffLineInterface",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/ParticleFinder",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/utils",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca/simd",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/OffLineInterface",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/ParticleFinder",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/utils",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools",
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
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/tools",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/SensorDataSheets",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/tasks",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/buffers",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts/sts",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts/unpack",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts/experimental",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/steer",
"/home/heinemann/gpuunpacker/cbmroot/core/config",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/geosetup",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/build2/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/L1/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libL1 dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace ca{namespace tools{class __attribute__((annotate("$clingAutoload$CaToolsWindowFinder.h")))  WindowFinder;}}
class __attribute__((annotate("$clingAutoload$CbmL1TrdTracklet.h")))  CbmL1TrdTracklet;
class __attribute__((annotate("$clingAutoload$CbmL1TrdTracklet4.h")))  CbmL1TrdTracklet4;
class __attribute__((annotate("$clingAutoload$CbmL1.h")))  CbmL1;
class __attribute__((annotate("$clingAutoload$CbmL1TrackMerger.h")))  CbmL1TrackMerger;
class __attribute__((annotate("$clingAutoload$CbmL1TofMerger.h")))  CbmL1TofMerger;
class __attribute__((annotate("$clingAutoload$CbmL1StsTrackFinder.h")))  CbmL1StsTrackFinder;
class __attribute__((annotate("$clingAutoload$CbmL1GlobalTrackFinder.h")))  CbmL1GlobalTrackFinder;
class __attribute__((annotate("$clingAutoload$CbmL1GlobalFindTracksEvents.h")))  CbmL1GlobalFindTracksEvents;
class __attribute__((annotate("$clingAutoload$CbmTrackerInputQaTrd.h")))  CbmTrackerInputQaTrd;
class __attribute__((annotate("$clingAutoload$CbmTrackerInputQaTof.h")))  CbmTrackerInputQaTof;
class __attribute__((annotate(R"ATTRDUMP(Base class for tracking input QA)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Base class for tracking input QA)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Base class for tracking input QA)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Base class for tracking input QA)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrackingInputQaSts.h")))  CbmTrackingInputQaSts;
class __attribute__((annotate("$clingAutoload$CbmCaInputQaSts.h")))  CbmCaInputQaSts;
class __attribute__((annotate("$clingAutoload$CbmCaInputQaMuch.h")))  CbmCaInputQaMuch;
class __attribute__((annotate("$clingAutoload$CbmCaInputQaTrd.h")))  CbmCaInputQaTrd;
class __attribute__((annotate("$clingAutoload$CbmCaInputQaTof.h")))  CbmCaInputQaTof;
namespace cbm{namespace ca{class __attribute__((annotate("$clingAutoload$CbmCaOutputQa.h")))  OutputQa;}}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libL1 dictionary payload"

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
#include "CbmL1Constants.h"
#include "CbmL1CATrdTrackFinderSA.h"
#include "CbmL1DetectorID.h"
#include "CbmL1MCPoint.h"
#include "CbmL1Hit.h"
#include "CbmL1Track.h"
#include "CbmL1TrackPar.h"
#include "CbmL1TrdHit.h"
#include "CbmL1Vtx.h"
#include "L1Def.h"
#include "L1Vector.h"
#include "L1EArray.h"
#include "L1Undef.h"
#include "CaToolsWindowFinder.h"
#include "CaToolsLinkKey.h"
#include "CaToolsHitRecord.h"
#include "CbmCaInputQaBase.h"
#include "CbmL1TrdTracklet.h"
#include "CbmL1TrdTracklet4.h"
#include "CbmL1.h"
#include "CbmL1TrackMerger.h"
#include "CbmL1TofMerger.h"
#include "CbmL1StsTrackFinder.h"
#include "CbmL1GlobalTrackFinder.h"
#include "CbmL1GlobalFindTracksEvents.h"
#include "CaAlgoRandom.h"
#include "L1Algo.h"
#include "L1TrackPar.h"
#include "L1Grid.h"
#include "CbmCaMCModule.h"
#include "CbmCaTimeSliceReader.h"
#include "L1Station.h"
#include "L1Fit.h"
#include "L1MCEvent.h"
#include "CbmL1MCTrack.h"
#include "CbmL1Track.h"
#include "L1Material.h"
#include "L1UMeasurementInfo.h"
#include "L1XYMeasurementInfo.h"
#include "L1Field.h"
#include "L1CAIteration.h"
#include "L1BaseStationInfo.h"
#include "L1InitManager.h"
#include "L1Parameters.h"
#include "L1InputData.h"
#include "L1IODataManager.h"
#include "L1CloneMerger.h"
#include "L1ConfigRW.h"
#include "L1SearchWindow.h"
#include "L1AlgoDraw.h"
#include "L1AlgoEfficiencyPerformance.h"
#include "L1AlgoPulls.h"
#include "CaToolsMCData.h"
#include "CaToolsMCPoint.h"
#include "CaToolsMCTrack.h"
#include "CaToolsQa.h"
#include "CaToolsDebugger.h"
#include "CaToolsWindowFinder.h"
#include "CaToolsWFExpression.h"
#include "CbmL1PFFitter.h"
#include "CbmL1PFMCParticle.h"
#include "CbmTrackerInputQaTrd.h"
#include "CbmTrackerInputQaTof.h"
#include "CbmTrackingInputQaSts.h"
#include "CbmCaInputQaSts.h"
#include "CbmCaInputQaMuch.h"
#include "CbmCaInputQaTrd.h"
#include "CbmCaInputQaTof.h"
#include "CbmCaOutputQa.h"
#include "CbmCaTrackTypeQa.h"
#include "CbmCaTrackFitQa.h"
#include "CbmTofInteraction.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmCaInputQaMuch", payloadCode, "@",
"CbmCaInputQaSts", payloadCode, "@",
"CbmCaInputQaTof", payloadCode, "@",
"CbmCaInputQaTrd", payloadCode, "@",
"CbmL1", payloadCode, "@",
"CbmL1GlobalFindTracksEvents", payloadCode, "@",
"CbmL1GlobalTrackFinder", payloadCode, "@",
"CbmL1StsTrackFinder", payloadCode, "@",
"CbmL1TofMerger", payloadCode, "@",
"CbmL1TrackMerger", payloadCode, "@",
"CbmL1TrdTracklet", payloadCode, "@",
"CbmL1TrdTracklet4", payloadCode, "@",
"CbmTrackerInputQaTof", payloadCode, "@",
"CbmTrackerInputQaTrd", payloadCode, "@",
"CbmTrackingInputQaSts", payloadCode, "@",
"ca::tools::WindowFinder", payloadCode, "@",
"cbm::ca::ETrackType", payloadCode, "@",
"cbm::ca::OutputQa", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libL1",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libL1_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libL1_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libL1() {
  TriggerDictionaryInitialization_libL1_Impl();
}
