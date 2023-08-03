// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmRecoSts
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
#include "CbmRecoSts.h"
#include "CbmStsAlgoAnaCluster.h"
#include "CbmStsAlgoFindClusters.h"
#include "CbmStsAlgoFindHits.h"
#include "CbmStsAlgoFindHitsOrtho.h"
#include "CbmStsFindTracks.h"
#include "CbmStsFindTracksEvents.h"
#include "CbmStsRecoModule.h"
#include "CbmRecoStsPixel.h"
#include "CbmStsTrackFinderIdeal.h"
#include "CbmStsUnpackAlgo.h"
#include "CbmStsUnpackAlgoBase.h"
#include "CbmStsUnpackAlgoLegacy.h"
#include "CbmStsUnpackConfig.h"
#include "CbmStsUnpackMonitor.h"
#include "CbmStsFindTracksQa.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmRecoSts(void *p = nullptr);
   static void *newArray_CbmRecoSts(Long_t size, void *p);
   static void delete_CbmRecoSts(void *p);
   static void deleteArray_CbmRecoSts(void *p);
   static void destruct_CbmRecoSts(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRecoSts*)
   {
      ::CbmRecoSts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRecoSts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRecoSts", ::CbmRecoSts::Class_Version(), "CbmRecoSts.h", 64,
                  typeid(::CbmRecoSts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRecoSts::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRecoSts) );
      instance.SetNew(&new_CbmRecoSts);
      instance.SetNewArray(&newArray_CbmRecoSts);
      instance.SetDelete(&delete_CbmRecoSts);
      instance.SetDeleteArray(&deleteArray_CbmRecoSts);
      instance.SetDestructor(&destruct_CbmRecoSts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRecoSts*)
   {
      return GenerateInitInstanceLocal((::CbmRecoSts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRecoSts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsAlgoAnaCluster(void *p = nullptr);
   static void *newArray_CbmStsAlgoAnaCluster(Long_t size, void *p);
   static void delete_CbmStsAlgoAnaCluster(void *p);
   static void deleteArray_CbmStsAlgoAnaCluster(void *p);
   static void destruct_CbmStsAlgoAnaCluster(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsAlgoAnaCluster*)
   {
      ::CbmStsAlgoAnaCluster *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsAlgoAnaCluster >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsAlgoAnaCluster", ::CbmStsAlgoAnaCluster::Class_Version(), "CbmStsAlgoAnaCluster.h", 36,
                  typeid(::CbmStsAlgoAnaCluster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsAlgoAnaCluster::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsAlgoAnaCluster) );
      instance.SetNew(&new_CbmStsAlgoAnaCluster);
      instance.SetNewArray(&newArray_CbmStsAlgoAnaCluster);
      instance.SetDelete(&delete_CbmStsAlgoAnaCluster);
      instance.SetDeleteArray(&deleteArray_CbmStsAlgoAnaCluster);
      instance.SetDestructor(&destruct_CbmStsAlgoAnaCluster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsAlgoAnaCluster*)
   {
      return GenerateInitInstanceLocal((::CbmStsAlgoAnaCluster*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsAlgoAnaCluster*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmStsAlgoFindClusters_Dictionary();
   static void CbmStsAlgoFindClusters_TClassManip(TClass*);
   static void *new_CbmStsAlgoFindClusters(void *p = nullptr);
   static void *newArray_CbmStsAlgoFindClusters(Long_t size, void *p);
   static void delete_CbmStsAlgoFindClusters(void *p);
   static void deleteArray_CbmStsAlgoFindClusters(void *p);
   static void destruct_CbmStsAlgoFindClusters(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsAlgoFindClusters*)
   {
      ::CbmStsAlgoFindClusters *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmStsAlgoFindClusters));
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsAlgoFindClusters", "CbmStsAlgoFindClusters.h", 59,
                  typeid(::CbmStsAlgoFindClusters), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmStsAlgoFindClusters_Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsAlgoFindClusters) );
      instance.SetNew(&new_CbmStsAlgoFindClusters);
      instance.SetNewArray(&newArray_CbmStsAlgoFindClusters);
      instance.SetDelete(&delete_CbmStsAlgoFindClusters);
      instance.SetDeleteArray(&deleteArray_CbmStsAlgoFindClusters);
      instance.SetDestructor(&destruct_CbmStsAlgoFindClusters);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsAlgoFindClusters*)
   {
      return GenerateInitInstanceLocal((::CbmStsAlgoFindClusters*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsAlgoFindClusters*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmStsAlgoFindClusters_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmStsAlgoFindClusters*)nullptr)->GetClass();
      CbmStsAlgoFindClusters_TClassManip(theClass);
   return theClass;
   }

   static void CbmStsAlgoFindClusters_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsAlgoFindHits(void *p = nullptr);
   static void *newArray_CbmStsAlgoFindHits(Long_t size, void *p);
   static void delete_CbmStsAlgoFindHits(void *p);
   static void deleteArray_CbmStsAlgoFindHits(void *p);
   static void destruct_CbmStsAlgoFindHits(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsAlgoFindHits*)
   {
      ::CbmStsAlgoFindHits *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsAlgoFindHits >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsAlgoFindHits", ::CbmStsAlgoFindHits::Class_Version(), "CbmStsAlgoFindHits.h", 38,
                  typeid(::CbmStsAlgoFindHits), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsAlgoFindHits::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsAlgoFindHits) );
      instance.SetNew(&new_CbmStsAlgoFindHits);
      instance.SetNewArray(&newArray_CbmStsAlgoFindHits);
      instance.SetDelete(&delete_CbmStsAlgoFindHits);
      instance.SetDeleteArray(&deleteArray_CbmStsAlgoFindHits);
      instance.SetDestructor(&destruct_CbmStsAlgoFindHits);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsAlgoFindHits*)
   {
      return GenerateInitInstanceLocal((::CbmStsAlgoFindHits*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsAlgoFindHits*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsAlgoFindHitsOrtho(void *p = nullptr);
   static void *newArray_CbmStsAlgoFindHitsOrtho(Long_t size, void *p);
   static void delete_CbmStsAlgoFindHitsOrtho(void *p);
   static void deleteArray_CbmStsAlgoFindHitsOrtho(void *p);
   static void destruct_CbmStsAlgoFindHitsOrtho(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsAlgoFindHitsOrtho*)
   {
      ::CbmStsAlgoFindHitsOrtho *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsAlgoFindHitsOrtho >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsAlgoFindHitsOrtho", ::CbmStsAlgoFindHitsOrtho::Class_Version(), "CbmStsAlgoFindHitsOrtho.h", 40,
                  typeid(::CbmStsAlgoFindHitsOrtho), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsAlgoFindHitsOrtho::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsAlgoFindHitsOrtho) );
      instance.SetNew(&new_CbmStsAlgoFindHitsOrtho);
      instance.SetNewArray(&newArray_CbmStsAlgoFindHitsOrtho);
      instance.SetDelete(&delete_CbmStsAlgoFindHitsOrtho);
      instance.SetDeleteArray(&deleteArray_CbmStsAlgoFindHitsOrtho);
      instance.SetDestructor(&destruct_CbmStsAlgoFindHitsOrtho);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsAlgoFindHitsOrtho*)
   {
      return GenerateInitInstanceLocal((::CbmStsAlgoFindHitsOrtho*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsAlgoFindHitsOrtho*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsFindTracks(void *p = nullptr);
   static void *newArray_CbmStsFindTracks(Long_t size, void *p);
   static void delete_CbmStsFindTracks(void *p);
   static void deleteArray_CbmStsFindTracks(void *p);
   static void destruct_CbmStsFindTracks(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsFindTracks*)
   {
      ::CbmStsFindTracks *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsFindTracks >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsFindTracks", ::CbmStsFindTracks::Class_Version(), "CbmStsFindTracks.h", 38,
                  typeid(::CbmStsFindTracks), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsFindTracks::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsFindTracks) );
      instance.SetNew(&new_CbmStsFindTracks);
      instance.SetNewArray(&newArray_CbmStsFindTracks);
      instance.SetDelete(&delete_CbmStsFindTracks);
      instance.SetDeleteArray(&deleteArray_CbmStsFindTracks);
      instance.SetDestructor(&destruct_CbmStsFindTracks);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsFindTracks*)
   {
      return GenerateInitInstanceLocal((::CbmStsFindTracks*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsFindTracks*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsFindTracksEvents(void *p = nullptr);
   static void *newArray_CbmStsFindTracksEvents(Long_t size, void *p);
   static void delete_CbmStsFindTracksEvents(void *p);
   static void deleteArray_CbmStsFindTracksEvents(void *p);
   static void destruct_CbmStsFindTracksEvents(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsFindTracksEvents*)
   {
      ::CbmStsFindTracksEvents *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsFindTracksEvents >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsFindTracksEvents", ::CbmStsFindTracksEvents::Class_Version(), "CbmStsFindTracksEvents.h", 37,
                  typeid(::CbmStsFindTracksEvents), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsFindTracksEvents::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsFindTracksEvents) );
      instance.SetNew(&new_CbmStsFindTracksEvents);
      instance.SetNewArray(&newArray_CbmStsFindTracksEvents);
      instance.SetDelete(&delete_CbmStsFindTracksEvents);
      instance.SetDeleteArray(&deleteArray_CbmStsFindTracksEvents);
      instance.SetDestructor(&destruct_CbmStsFindTracksEvents);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsFindTracksEvents*)
   {
      return GenerateInitInstanceLocal((::CbmStsFindTracksEvents*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsFindTracksEvents*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsRecoModule(void *p = nullptr);
   static void *newArray_CbmStsRecoModule(Long_t size, void *p);
   static void delete_CbmStsRecoModule(void *p);
   static void deleteArray_CbmStsRecoModule(void *p);
   static void destruct_CbmStsRecoModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsRecoModule*)
   {
      ::CbmStsRecoModule *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsRecoModule >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsRecoModule", ::CbmStsRecoModule::Class_Version(), "CbmStsRecoModule.h", 51,
                  typeid(::CbmStsRecoModule), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsRecoModule::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsRecoModule) );
      instance.SetNew(&new_CbmStsRecoModule);
      instance.SetNewArray(&newArray_CbmStsRecoModule);
      instance.SetDelete(&delete_CbmStsRecoModule);
      instance.SetDeleteArray(&deleteArray_CbmStsRecoModule);
      instance.SetDestructor(&destruct_CbmStsRecoModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsRecoModule*)
   {
      return GenerateInitInstanceLocal((::CbmStsRecoModule*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsRecoModule*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRecoStsPixel(void *p = nullptr);
   static void *newArray_CbmRecoStsPixel(Long_t size, void *p);
   static void delete_CbmRecoStsPixel(void *p);
   static void deleteArray_CbmRecoStsPixel(void *p);
   static void destruct_CbmRecoStsPixel(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRecoStsPixel*)
   {
      ::CbmRecoStsPixel *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRecoStsPixel >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRecoStsPixel", ::CbmRecoStsPixel::Class_Version(), "CbmRecoStsPixel.h", 49,
                  typeid(::CbmRecoStsPixel), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRecoStsPixel::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRecoStsPixel) );
      instance.SetNew(&new_CbmRecoStsPixel);
      instance.SetNewArray(&newArray_CbmRecoStsPixel);
      instance.SetDelete(&delete_CbmRecoStsPixel);
      instance.SetDeleteArray(&deleteArray_CbmRecoStsPixel);
      instance.SetDestructor(&destruct_CbmRecoStsPixel);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRecoStsPixel*)
   {
      return GenerateInitInstanceLocal((::CbmRecoStsPixel*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRecoStsPixel*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsTrackFinderIdeal(void *p = nullptr);
   static void *newArray_CbmStsTrackFinderIdeal(Long_t size, void *p);
   static void delete_CbmStsTrackFinderIdeal(void *p);
   static void deleteArray_CbmStsTrackFinderIdeal(void *p);
   static void destruct_CbmStsTrackFinderIdeal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsTrackFinderIdeal*)
   {
      ::CbmStsTrackFinderIdeal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsTrackFinderIdeal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsTrackFinderIdeal", ::CbmStsTrackFinderIdeal::Class_Version(), "CbmStsTrackFinderIdeal.h", 42,
                  typeid(::CbmStsTrackFinderIdeal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsTrackFinderIdeal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsTrackFinderIdeal) );
      instance.SetNew(&new_CbmStsTrackFinderIdeal);
      instance.SetNewArray(&newArray_CbmStsTrackFinderIdeal);
      instance.SetDelete(&delete_CbmStsTrackFinderIdeal);
      instance.SetDeleteArray(&deleteArray_CbmStsTrackFinderIdeal);
      instance.SetDestructor(&destruct_CbmStsTrackFinderIdeal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsTrackFinderIdeal*)
   {
      return GenerateInitInstanceLocal((::CbmStsTrackFinderIdeal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsTrackFinderIdeal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsUnpackMonitor(void *p = nullptr);
   static void *newArray_CbmStsUnpackMonitor(Long_t size, void *p);
   static void delete_CbmStsUnpackMonitor(void *p);
   static void deleteArray_CbmStsUnpackMonitor(void *p);
   static void destruct_CbmStsUnpackMonitor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsUnpackMonitor*)
   {
      ::CbmStsUnpackMonitor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsUnpackMonitor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsUnpackMonitor", ::CbmStsUnpackMonitor::Class_Version(), "CbmStsUnpackMonitor.h", 22,
                  typeid(::CbmStsUnpackMonitor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsUnpackMonitor::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsUnpackMonitor) );
      instance.SetNew(&new_CbmStsUnpackMonitor);
      instance.SetNewArray(&newArray_CbmStsUnpackMonitor);
      instance.SetDelete(&delete_CbmStsUnpackMonitor);
      instance.SetDeleteArray(&deleteArray_CbmStsUnpackMonitor);
      instance.SetDestructor(&destruct_CbmStsUnpackMonitor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsUnpackMonitor*)
   {
      return GenerateInitInstanceLocal((::CbmStsUnpackMonitor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsUnpackMonitor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmStsUnpackAlgoBase(void *p);
   static void deleteArray_CbmStsUnpackAlgoBase(void *p);
   static void destruct_CbmStsUnpackAlgoBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsUnpackAlgoBase*)
   {
      ::CbmStsUnpackAlgoBase *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsUnpackAlgoBase >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsUnpackAlgoBase", ::CbmStsUnpackAlgoBase::Class_Version(), "CbmStsUnpackAlgoBase.h", 42,
                  typeid(::CbmStsUnpackAlgoBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsUnpackAlgoBase::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsUnpackAlgoBase) );
      instance.SetDelete(&delete_CbmStsUnpackAlgoBase);
      instance.SetDeleteArray(&deleteArray_CbmStsUnpackAlgoBase);
      instance.SetDestructor(&destruct_CbmStsUnpackAlgoBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsUnpackAlgoBase*)
   {
      return GenerateInitInstanceLocal((::CbmStsUnpackAlgoBase*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsUnpackAlgoBase*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsUnpackAlgo(void *p = nullptr);
   static void *newArray_CbmStsUnpackAlgo(Long_t size, void *p);
   static void delete_CbmStsUnpackAlgo(void *p);
   static void deleteArray_CbmStsUnpackAlgo(void *p);
   static void destruct_CbmStsUnpackAlgo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsUnpackAlgo*)
   {
      ::CbmStsUnpackAlgo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsUnpackAlgo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsUnpackAlgo", ::CbmStsUnpackAlgo::Class_Version(), "CbmStsUnpackAlgo.h", 28,
                  typeid(::CbmStsUnpackAlgo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsUnpackAlgo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsUnpackAlgo) );
      instance.SetNew(&new_CbmStsUnpackAlgo);
      instance.SetNewArray(&newArray_CbmStsUnpackAlgo);
      instance.SetDelete(&delete_CbmStsUnpackAlgo);
      instance.SetDeleteArray(&deleteArray_CbmStsUnpackAlgo);
      instance.SetDestructor(&destruct_CbmStsUnpackAlgo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsUnpackAlgo*)
   {
      return GenerateInitInstanceLocal((::CbmStsUnpackAlgo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsUnpackAlgo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsUnpackAlgoLegacy(void *p = nullptr);
   static void *newArray_CbmStsUnpackAlgoLegacy(Long_t size, void *p);
   static void delete_CbmStsUnpackAlgoLegacy(void *p);
   static void deleteArray_CbmStsUnpackAlgoLegacy(void *p);
   static void destruct_CbmStsUnpackAlgoLegacy(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsUnpackAlgoLegacy*)
   {
      ::CbmStsUnpackAlgoLegacy *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsUnpackAlgoLegacy >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsUnpackAlgoLegacy", ::CbmStsUnpackAlgoLegacy::Class_Version(), "CbmStsUnpackAlgoLegacy.h", 26,
                  typeid(::CbmStsUnpackAlgoLegacy), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsUnpackAlgoLegacy::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsUnpackAlgoLegacy) );
      instance.SetNew(&new_CbmStsUnpackAlgoLegacy);
      instance.SetNewArray(&newArray_CbmStsUnpackAlgoLegacy);
      instance.SetDelete(&delete_CbmStsUnpackAlgoLegacy);
      instance.SetDeleteArray(&deleteArray_CbmStsUnpackAlgoLegacy);
      instance.SetDestructor(&destruct_CbmStsUnpackAlgoLegacy);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsUnpackAlgoLegacy*)
   {
      return GenerateInitInstanceLocal((::CbmStsUnpackAlgoLegacy*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsUnpackAlgoLegacy*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmStsUnpackConfig(void *p);
   static void deleteArray_CbmStsUnpackConfig(void *p);
   static void destruct_CbmStsUnpackConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsUnpackConfig*)
   {
      ::CbmStsUnpackConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsUnpackConfig >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsUnpackConfig", ::CbmStsUnpackConfig::Class_Version(), "CbmStsUnpackConfig.h", 36,
                  typeid(::CbmStsUnpackConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsUnpackConfig::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsUnpackConfig) );
      instance.SetDelete(&delete_CbmStsUnpackConfig);
      instance.SetDeleteArray(&deleteArray_CbmStsUnpackConfig);
      instance.SetDestructor(&destruct_CbmStsUnpackConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsUnpackConfig*)
   {
      return GenerateInitInstanceLocal((::CbmStsUnpackConfig*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsUnpackConfig*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsFindTracksQa(void *p = nullptr);
   static void *newArray_CbmStsFindTracksQa(Long_t size, void *p);
   static void delete_CbmStsFindTracksQa(void *p);
   static void deleteArray_CbmStsFindTracksQa(void *p);
   static void destruct_CbmStsFindTracksQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsFindTracksQa*)
   {
      ::CbmStsFindTracksQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsFindTracksQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsFindTracksQa", ::CbmStsFindTracksQa::Class_Version(), "CbmStsFindTracksQa.h", 37,
                  typeid(::CbmStsFindTracksQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsFindTracksQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsFindTracksQa) );
      instance.SetNew(&new_CbmStsFindTracksQa);
      instance.SetNewArray(&newArray_CbmStsFindTracksQa);
      instance.SetDelete(&delete_CbmStsFindTracksQa);
      instance.SetDeleteArray(&deleteArray_CbmStsFindTracksQa);
      instance.SetDestructor(&destruct_CbmStsFindTracksQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsFindTracksQa*)
   {
      return GenerateInitInstanceLocal((::CbmStsFindTracksQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsFindTracksQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmRecoSts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRecoSts::Class_Name()
{
   return "CbmRecoSts";
}

//______________________________________________________________________________
const char *CbmRecoSts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoSts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRecoSts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoSts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRecoSts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoSts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRecoSts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoSts*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsAlgoAnaCluster::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsAlgoAnaCluster::Class_Name()
{
   return "CbmStsAlgoAnaCluster";
}

//______________________________________________________________________________
const char *CbmStsAlgoAnaCluster::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsAlgoAnaCluster*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsAlgoAnaCluster::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsAlgoAnaCluster*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsAlgoAnaCluster::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsAlgoAnaCluster*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsAlgoAnaCluster::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsAlgoAnaCluster*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsAlgoFindHits::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsAlgoFindHits::Class_Name()
{
   return "CbmStsAlgoFindHits";
}

//______________________________________________________________________________
const char *CbmStsAlgoFindHits::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsAlgoFindHits*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsAlgoFindHits::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsAlgoFindHits*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsAlgoFindHits::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsAlgoFindHits*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsAlgoFindHits::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsAlgoFindHits*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsAlgoFindHitsOrtho::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsAlgoFindHitsOrtho::Class_Name()
{
   return "CbmStsAlgoFindHitsOrtho";
}

//______________________________________________________________________________
const char *CbmStsAlgoFindHitsOrtho::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsAlgoFindHitsOrtho*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsAlgoFindHitsOrtho::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsAlgoFindHitsOrtho*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsAlgoFindHitsOrtho::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsAlgoFindHitsOrtho*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsAlgoFindHitsOrtho::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsAlgoFindHitsOrtho*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsFindTracks::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsFindTracks::Class_Name()
{
   return "CbmStsFindTracks";
}

//______________________________________________________________________________
const char *CbmStsFindTracks::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFindTracks*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsFindTracks::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFindTracks*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsFindTracks::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFindTracks*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsFindTracks::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFindTracks*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsFindTracksEvents::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsFindTracksEvents::Class_Name()
{
   return "CbmStsFindTracksEvents";
}

//______________________________________________________________________________
const char *CbmStsFindTracksEvents::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFindTracksEvents*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsFindTracksEvents::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFindTracksEvents*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsFindTracksEvents::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFindTracksEvents*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsFindTracksEvents::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFindTracksEvents*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsRecoModule::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsRecoModule::Class_Name()
{
   return "CbmStsRecoModule";
}

//______________________________________________________________________________
const char *CbmStsRecoModule::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsRecoModule*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsRecoModule::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsRecoModule*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsRecoModule::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsRecoModule*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsRecoModule::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsRecoModule*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRecoStsPixel::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRecoStsPixel::Class_Name()
{
   return "CbmRecoStsPixel";
}

//______________________________________________________________________________
const char *CbmRecoStsPixel::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoStsPixel*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRecoStsPixel::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoStsPixel*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRecoStsPixel::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoStsPixel*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRecoStsPixel::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoStsPixel*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsTrackFinderIdeal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsTrackFinderIdeal::Class_Name()
{
   return "CbmStsTrackFinderIdeal";
}

//______________________________________________________________________________
const char *CbmStsTrackFinderIdeal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackFinderIdeal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsTrackFinderIdeal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackFinderIdeal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsTrackFinderIdeal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackFinderIdeal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsTrackFinderIdeal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackFinderIdeal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsUnpackMonitor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsUnpackMonitor::Class_Name()
{
   return "CbmStsUnpackMonitor";
}

//______________________________________________________________________________
const char *CbmStsUnpackMonitor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackMonitor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsUnpackMonitor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackMonitor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsUnpackMonitor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackMonitor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsUnpackMonitor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackMonitor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsUnpackAlgoBase::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsUnpackAlgoBase::Class_Name()
{
   return "CbmStsUnpackAlgoBase";
}

//______________________________________________________________________________
const char *CbmStsUnpackAlgoBase::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackAlgoBase*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsUnpackAlgoBase::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackAlgoBase*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsUnpackAlgoBase::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackAlgoBase*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsUnpackAlgoBase::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackAlgoBase*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsUnpackAlgo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsUnpackAlgo::Class_Name()
{
   return "CbmStsUnpackAlgo";
}

//______________________________________________________________________________
const char *CbmStsUnpackAlgo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackAlgo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsUnpackAlgo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackAlgo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsUnpackAlgo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackAlgo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsUnpackAlgo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackAlgo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsUnpackAlgoLegacy::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsUnpackAlgoLegacy::Class_Name()
{
   return "CbmStsUnpackAlgoLegacy";
}

//______________________________________________________________________________
const char *CbmStsUnpackAlgoLegacy::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackAlgoLegacy*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsUnpackAlgoLegacy::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackAlgoLegacy*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsUnpackAlgoLegacy::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackAlgoLegacy*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsUnpackAlgoLegacy::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackAlgoLegacy*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsUnpackConfig::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsUnpackConfig::Class_Name()
{
   return "CbmStsUnpackConfig";
}

//______________________________________________________________________________
const char *CbmStsUnpackConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackConfig*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsUnpackConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackConfig*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsUnpackConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackConfig*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsUnpackConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsUnpackConfig*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsFindTracksQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsFindTracksQa::Class_Name()
{
   return "CbmStsFindTracksQa";
}

//______________________________________________________________________________
const char *CbmStsFindTracksQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFindTracksQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsFindTracksQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFindTracksQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsFindTracksQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFindTracksQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsFindTracksQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFindTracksQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmRecoSts::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRecoSts.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRecoSts::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRecoSts::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRecoSts(void *p) {
      return  p ? new(p) ::CbmRecoSts : new ::CbmRecoSts;
   }
   static void *newArray_CbmRecoSts(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRecoSts[nElements] : new ::CbmRecoSts[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRecoSts(void *p) {
      delete ((::CbmRecoSts*)p);
   }
   static void deleteArray_CbmRecoSts(void *p) {
      delete [] ((::CbmRecoSts*)p);
   }
   static void destruct_CbmRecoSts(void *p) {
      typedef ::CbmRecoSts current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRecoSts

//______________________________________________________________________________
void CbmStsAlgoAnaCluster::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsAlgoAnaCluster.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsAlgoAnaCluster::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsAlgoAnaCluster::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsAlgoAnaCluster(void *p) {
      return  p ? new(p) ::CbmStsAlgoAnaCluster : new ::CbmStsAlgoAnaCluster;
   }
   static void *newArray_CbmStsAlgoAnaCluster(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsAlgoAnaCluster[nElements] : new ::CbmStsAlgoAnaCluster[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsAlgoAnaCluster(void *p) {
      delete ((::CbmStsAlgoAnaCluster*)p);
   }
   static void deleteArray_CbmStsAlgoAnaCluster(void *p) {
      delete [] ((::CbmStsAlgoAnaCluster*)p);
   }
   static void destruct_CbmStsAlgoAnaCluster(void *p) {
      typedef ::CbmStsAlgoAnaCluster current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsAlgoAnaCluster

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsAlgoFindClusters(void *p) {
      return  p ? new(p) ::CbmStsAlgoFindClusters : new ::CbmStsAlgoFindClusters;
   }
   static void *newArray_CbmStsAlgoFindClusters(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsAlgoFindClusters[nElements] : new ::CbmStsAlgoFindClusters[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsAlgoFindClusters(void *p) {
      delete ((::CbmStsAlgoFindClusters*)p);
   }
   static void deleteArray_CbmStsAlgoFindClusters(void *p) {
      delete [] ((::CbmStsAlgoFindClusters*)p);
   }
   static void destruct_CbmStsAlgoFindClusters(void *p) {
      typedef ::CbmStsAlgoFindClusters current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsAlgoFindClusters

//______________________________________________________________________________
void CbmStsAlgoFindHits::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsAlgoFindHits.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsAlgoFindHits::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsAlgoFindHits::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsAlgoFindHits(void *p) {
      return  p ? new(p) ::CbmStsAlgoFindHits : new ::CbmStsAlgoFindHits;
   }
   static void *newArray_CbmStsAlgoFindHits(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsAlgoFindHits[nElements] : new ::CbmStsAlgoFindHits[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsAlgoFindHits(void *p) {
      delete ((::CbmStsAlgoFindHits*)p);
   }
   static void deleteArray_CbmStsAlgoFindHits(void *p) {
      delete [] ((::CbmStsAlgoFindHits*)p);
   }
   static void destruct_CbmStsAlgoFindHits(void *p) {
      typedef ::CbmStsAlgoFindHits current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsAlgoFindHits

//______________________________________________________________________________
void CbmStsAlgoFindHitsOrtho::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsAlgoFindHitsOrtho.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsAlgoFindHitsOrtho::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsAlgoFindHitsOrtho::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsAlgoFindHitsOrtho(void *p) {
      return  p ? new(p) ::CbmStsAlgoFindHitsOrtho : new ::CbmStsAlgoFindHitsOrtho;
   }
   static void *newArray_CbmStsAlgoFindHitsOrtho(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsAlgoFindHitsOrtho[nElements] : new ::CbmStsAlgoFindHitsOrtho[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsAlgoFindHitsOrtho(void *p) {
      delete ((::CbmStsAlgoFindHitsOrtho*)p);
   }
   static void deleteArray_CbmStsAlgoFindHitsOrtho(void *p) {
      delete [] ((::CbmStsAlgoFindHitsOrtho*)p);
   }
   static void destruct_CbmStsAlgoFindHitsOrtho(void *p) {
      typedef ::CbmStsAlgoFindHitsOrtho current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsAlgoFindHitsOrtho

//______________________________________________________________________________
void CbmStsFindTracks::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsFindTracks.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsFindTracks::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsFindTracks::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsFindTracks(void *p) {
      return  p ? new(p) ::CbmStsFindTracks : new ::CbmStsFindTracks;
   }
   static void *newArray_CbmStsFindTracks(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsFindTracks[nElements] : new ::CbmStsFindTracks[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsFindTracks(void *p) {
      delete ((::CbmStsFindTracks*)p);
   }
   static void deleteArray_CbmStsFindTracks(void *p) {
      delete [] ((::CbmStsFindTracks*)p);
   }
   static void destruct_CbmStsFindTracks(void *p) {
      typedef ::CbmStsFindTracks current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsFindTracks

//______________________________________________________________________________
void CbmStsFindTracksEvents::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsFindTracksEvents.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsFindTracksEvents::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsFindTracksEvents::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsFindTracksEvents(void *p) {
      return  p ? new(p) ::CbmStsFindTracksEvents : new ::CbmStsFindTracksEvents;
   }
   static void *newArray_CbmStsFindTracksEvents(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsFindTracksEvents[nElements] : new ::CbmStsFindTracksEvents[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsFindTracksEvents(void *p) {
      delete ((::CbmStsFindTracksEvents*)p);
   }
   static void deleteArray_CbmStsFindTracksEvents(void *p) {
      delete [] ((::CbmStsFindTracksEvents*)p);
   }
   static void destruct_CbmStsFindTracksEvents(void *p) {
      typedef ::CbmStsFindTracksEvents current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsFindTracksEvents

//______________________________________________________________________________
void CbmStsRecoModule::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsRecoModule.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsRecoModule::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsRecoModule::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsRecoModule(void *p) {
      return  p ? new(p) ::CbmStsRecoModule : new ::CbmStsRecoModule;
   }
   static void *newArray_CbmStsRecoModule(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsRecoModule[nElements] : new ::CbmStsRecoModule[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsRecoModule(void *p) {
      delete ((::CbmStsRecoModule*)p);
   }
   static void deleteArray_CbmStsRecoModule(void *p) {
      delete [] ((::CbmStsRecoModule*)p);
   }
   static void destruct_CbmStsRecoModule(void *p) {
      typedef ::CbmStsRecoModule current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsRecoModule

//______________________________________________________________________________
void CbmRecoStsPixel::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRecoStsPixel.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRecoStsPixel::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRecoStsPixel::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRecoStsPixel(void *p) {
      return  p ? new(p) ::CbmRecoStsPixel : new ::CbmRecoStsPixel;
   }
   static void *newArray_CbmRecoStsPixel(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRecoStsPixel[nElements] : new ::CbmRecoStsPixel[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRecoStsPixel(void *p) {
      delete ((::CbmRecoStsPixel*)p);
   }
   static void deleteArray_CbmRecoStsPixel(void *p) {
      delete [] ((::CbmRecoStsPixel*)p);
   }
   static void destruct_CbmRecoStsPixel(void *p) {
      typedef ::CbmRecoStsPixel current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRecoStsPixel

//______________________________________________________________________________
void CbmStsTrackFinderIdeal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsTrackFinderIdeal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsTrackFinderIdeal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsTrackFinderIdeal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsTrackFinderIdeal(void *p) {
      return  p ? new(p) ::CbmStsTrackFinderIdeal : new ::CbmStsTrackFinderIdeal;
   }
   static void *newArray_CbmStsTrackFinderIdeal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsTrackFinderIdeal[nElements] : new ::CbmStsTrackFinderIdeal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsTrackFinderIdeal(void *p) {
      delete ((::CbmStsTrackFinderIdeal*)p);
   }
   static void deleteArray_CbmStsTrackFinderIdeal(void *p) {
      delete [] ((::CbmStsTrackFinderIdeal*)p);
   }
   static void destruct_CbmStsTrackFinderIdeal(void *p) {
      typedef ::CbmStsTrackFinderIdeal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsTrackFinderIdeal

//______________________________________________________________________________
void CbmStsUnpackMonitor::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsUnpackMonitor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsUnpackMonitor::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsUnpackMonitor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsUnpackMonitor(void *p) {
      return  p ? new(p) ::CbmStsUnpackMonitor : new ::CbmStsUnpackMonitor;
   }
   static void *newArray_CbmStsUnpackMonitor(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsUnpackMonitor[nElements] : new ::CbmStsUnpackMonitor[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsUnpackMonitor(void *p) {
      delete ((::CbmStsUnpackMonitor*)p);
   }
   static void deleteArray_CbmStsUnpackMonitor(void *p) {
      delete [] ((::CbmStsUnpackMonitor*)p);
   }
   static void destruct_CbmStsUnpackMonitor(void *p) {
      typedef ::CbmStsUnpackMonitor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsUnpackMonitor

//______________________________________________________________________________
void CbmStsUnpackAlgoBase::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsUnpackAlgoBase.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsUnpackAlgoBase::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsUnpackAlgoBase::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmStsUnpackAlgoBase(void *p) {
      delete ((::CbmStsUnpackAlgoBase*)p);
   }
   static void deleteArray_CbmStsUnpackAlgoBase(void *p) {
      delete [] ((::CbmStsUnpackAlgoBase*)p);
   }
   static void destruct_CbmStsUnpackAlgoBase(void *p) {
      typedef ::CbmStsUnpackAlgoBase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsUnpackAlgoBase

//______________________________________________________________________________
void CbmStsUnpackAlgo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsUnpackAlgo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsUnpackAlgo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsUnpackAlgo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsUnpackAlgo(void *p) {
      return  p ? new(p) ::CbmStsUnpackAlgo : new ::CbmStsUnpackAlgo;
   }
   static void *newArray_CbmStsUnpackAlgo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsUnpackAlgo[nElements] : new ::CbmStsUnpackAlgo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsUnpackAlgo(void *p) {
      delete ((::CbmStsUnpackAlgo*)p);
   }
   static void deleteArray_CbmStsUnpackAlgo(void *p) {
      delete [] ((::CbmStsUnpackAlgo*)p);
   }
   static void destruct_CbmStsUnpackAlgo(void *p) {
      typedef ::CbmStsUnpackAlgo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsUnpackAlgo

//______________________________________________________________________________
void CbmStsUnpackAlgoLegacy::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsUnpackAlgoLegacy.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsUnpackAlgoLegacy::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsUnpackAlgoLegacy::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsUnpackAlgoLegacy(void *p) {
      return  p ? new(p) ::CbmStsUnpackAlgoLegacy : new ::CbmStsUnpackAlgoLegacy;
   }
   static void *newArray_CbmStsUnpackAlgoLegacy(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsUnpackAlgoLegacy[nElements] : new ::CbmStsUnpackAlgoLegacy[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsUnpackAlgoLegacy(void *p) {
      delete ((::CbmStsUnpackAlgoLegacy*)p);
   }
   static void deleteArray_CbmStsUnpackAlgoLegacy(void *p) {
      delete [] ((::CbmStsUnpackAlgoLegacy*)p);
   }
   static void destruct_CbmStsUnpackAlgoLegacy(void *p) {
      typedef ::CbmStsUnpackAlgoLegacy current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsUnpackAlgoLegacy

//______________________________________________________________________________
void CbmStsUnpackConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsUnpackConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsUnpackConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsUnpackConfig::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmStsUnpackConfig(void *p) {
      delete ((::CbmStsUnpackConfig*)p);
   }
   static void deleteArray_CbmStsUnpackConfig(void *p) {
      delete [] ((::CbmStsUnpackConfig*)p);
   }
   static void destruct_CbmStsUnpackConfig(void *p) {
      typedef ::CbmStsUnpackConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsUnpackConfig

//______________________________________________________________________________
void CbmStsFindTracksQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsFindTracksQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsFindTracksQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsFindTracksQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsFindTracksQa(void *p) {
      return  p ? new(p) ::CbmStsFindTracksQa : new ::CbmStsFindTracksQa;
   }
   static void *newArray_CbmStsFindTracksQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsFindTracksQa[nElements] : new ::CbmStsFindTracksQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsFindTracksQa(void *p) {
      delete ((::CbmStsFindTracksQa*)p);
   }
   static void deleteArray_CbmStsFindTracksQa(void *p) {
      delete [] ((::CbmStsFindTracksQa*)p);
   }
   static void destruct_CbmStsFindTracksQa(void *p) {
      typedef ::CbmStsFindTracksQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsFindTracksQa

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
   static TClass *vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR_Dictionary();
   static void vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR_TClassManip(TClass*);
   static void *new_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR(void *p = nullptr);
   static void *newArray_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR(Long_t size, void *p);
   static void delete_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR(void *p);
   static void deleteArray_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR(void *p);
   static void destruct_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsUnpackConfig::FebChanMaskReco>*)
   {
      vector<CbmStsUnpackConfig::FebChanMaskReco> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsUnpackConfig::FebChanMaskReco>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsUnpackConfig::FebChanMaskReco>", -2, "vector", 389,
                  typeid(vector<CbmStsUnpackConfig::FebChanMaskReco>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmStsUnpackConfig::FebChanMaskReco>) );
      instance.SetNew(&new_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR);
      instance.SetNewArray(&newArray_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR);
      instance.SetDelete(&delete_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR);
      instance.SetDestructor(&destruct_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsUnpackConfig::FebChanMaskReco> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsUnpackConfig::FebChanMaskReco>","std::vector<CbmStsUnpackConfig::FebChanMaskReco, std::allocator<CbmStsUnpackConfig::FebChanMaskReco> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsUnpackConfig::FebChanMaskReco>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsUnpackConfig::FebChanMaskReco>*)nullptr)->GetClass();
      vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsUnpackConfig::FebChanMaskReco> : new vector<CbmStsUnpackConfig::FebChanMaskReco>;
   }
   static void *newArray_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsUnpackConfig::FebChanMaskReco>[nElements] : new vector<CbmStsUnpackConfig::FebChanMaskReco>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR(void *p) {
      delete ((vector<CbmStsUnpackConfig::FebChanMaskReco>*)p);
   }
   static void deleteArray_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR(void *p) {
      delete [] ((vector<CbmStsUnpackConfig::FebChanMaskReco>*)p);
   }
   static void destruct_vectorlECbmStsUnpackConfigcLcLFebChanMaskRecogR(void *p) {
      typedef vector<CbmStsUnpackConfig::FebChanMaskReco> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsUnpackConfig::FebChanMaskReco>

namespace ROOT {
   static TClass *maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR_Dictionary();
   static void maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR(void *p);
   static void deleteArray_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR(void *p);
   static void destruct_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,vector<vector<double> > >*)
   {
      map<unsigned int,vector<vector<double> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,vector<vector<double> > >));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,vector<vector<double> > >", -2, "map", 100,
                  typeid(map<unsigned int,vector<vector<double> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,vector<vector<double> > >) );
      instance.SetNew(&new_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,vector<vector<double> > > >()));

      ::ROOT::AddClassAlternate("map<unsigned int,vector<vector<double> > >","std::map<unsigned int, std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,vector<vector<double> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,vector<vector<double> > >*)nullptr)->GetClass();
      maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,vector<vector<double> > > : new map<unsigned int,vector<vector<double> > >;
   }
   static void *newArray_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,vector<vector<double> > >[nElements] : new map<unsigned int,vector<vector<double> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      delete ((map<unsigned int,vector<vector<double> > >*)p);
   }
   static void deleteArray_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      delete [] ((map<unsigned int,vector<vector<double> > >*)p);
   }
   static void destruct_maplEunsignedsPintcOvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      typedef map<unsigned int,vector<vector<double> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,vector<vector<double> > >

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
   static TClass *maplEunsignedsPintcOCbmStsParModulegR_Dictionary();
   static void maplEunsignedsPintcOCbmStsParModulegR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOCbmStsParModulegR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOCbmStsParModulegR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOCbmStsParModulegR(void *p);
   static void deleteArray_maplEunsignedsPintcOCbmStsParModulegR(void *p);
   static void destruct_maplEunsignedsPintcOCbmStsParModulegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,CbmStsParModule>*)
   {
      map<unsigned int,CbmStsParModule> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,CbmStsParModule>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,CbmStsParModule>", -2, "map", 100,
                  typeid(map<unsigned int,CbmStsParModule>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOCbmStsParModulegR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,CbmStsParModule>) );
      instance.SetNew(&new_maplEunsignedsPintcOCbmStsParModulegR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOCbmStsParModulegR);
      instance.SetDelete(&delete_maplEunsignedsPintcOCbmStsParModulegR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOCbmStsParModulegR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOCbmStsParModulegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,CbmStsParModule> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,CbmStsParModule>","std::map<unsigned int, CbmStsParModule, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, CbmStsParModule> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,CbmStsParModule>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOCbmStsParModulegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,CbmStsParModule>*)nullptr)->GetClass();
      maplEunsignedsPintcOCbmStsParModulegR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOCbmStsParModulegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOCbmStsParModulegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,CbmStsParModule> : new map<unsigned int,CbmStsParModule>;
   }
   static void *newArray_maplEunsignedsPintcOCbmStsParModulegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,CbmStsParModule>[nElements] : new map<unsigned int,CbmStsParModule>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOCbmStsParModulegR(void *p) {
      delete ((map<unsigned int,CbmStsParModule>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOCbmStsParModulegR(void *p) {
      delete [] ((map<unsigned int,CbmStsParModule>*)p);
   }
   static void destruct_maplEunsignedsPintcOCbmStsParModulegR(void *p) {
      typedef map<unsigned int,CbmStsParModule> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,CbmStsParModule>

namespace {
  void TriggerDictionaryInitialization_libCbmRecoSts_Impl() {
    static const char* headers[] = {
"CbmRecoSts.h",
"CbmStsAlgoAnaCluster.h",
"CbmStsAlgoFindClusters.h",
"CbmStsAlgoFindHits.h",
"CbmStsAlgoFindHitsOrtho.h",
"CbmStsFindTracks.h",
"CbmStsFindTracksEvents.h",
"CbmStsRecoModule.h",
"CbmRecoStsPixel.h",
"CbmStsTrackFinderIdeal.h",
"CbmStsUnpackAlgo.h",
"CbmStsUnpackAlgoBase.h",
"CbmStsUnpackAlgoLegacy.h",
"CbmStsUnpackConfig.h",
"CbmStsUnpackMonitor.h",
"CbmStsFindTracksQa.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts/sts",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts/unpack",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts/experimental",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
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
"/home/heinemann/gpuunpacker/cbmroot/core/data",
"/home/heinemann/gpuunpacker/cbmroot/core/data/base",
"/home/heinemann/gpuunpacker/cbmroot/core/data/bmon",
"/home/heinemann/gpuunpacker/cbmroot/core/data/sts",
"/home/heinemann/gpuunpacker/cbmroot/core/data/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/data/much",
"/home/heinemann/gpuunpacker/cbmroot/core/data/trd",
"/home/heinemann/gpuunpacker/cbmroot/core/data/tof",
"/home/heinemann/gpuunpacker/cbmroot/core/data/psd",
"/home/heinemann/gpuunpacker/cbmroot/core/data/global",
"/home/heinemann/gpuunpacker/cbmroot/core/data/raw",
"/home/heinemann/gpuunpacker/cbmroot/algo/log/.",
"/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/heinemann/gpuunpacker/cbmroot/external/xpu/src",
"/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/logging",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/external/yaml-cpp/include",
"/home/heinemann/gpuunpacker/cbmroot/core/data/mvd",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils/flestools",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/tools",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/SensorDataSheets",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/tasks",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/buffers",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/detectors/sts/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmRecoSts dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmRecoSts.h")))  CbmRecoSts;
class __attribute__((annotate("$clingAutoload$CbmStsAlgoAnaCluster.h")))  CbmStsAlgoAnaCluster;
class __attribute__((annotate("$clingAutoload$CbmStsAlgoFindClusters.h")))  CbmStsAlgoFindClusters;
class __attribute__((annotate("$clingAutoload$CbmStsAlgoFindHits.h")))  CbmStsAlgoFindHits;
class __attribute__((annotate("$clingAutoload$CbmStsAlgoFindHitsOrtho.h")))  CbmStsAlgoFindHitsOrtho;
class __attribute__((annotate("$clingAutoload$CbmStsFindTracks.h")))  CbmStsFindTracks;
class __attribute__((annotate("$clingAutoload$CbmStsFindTracksEvents.h")))  CbmStsFindTracksEvents;
class __attribute__((annotate("$clingAutoload$CbmStsRecoModule.h")))  CbmStsRecoModule;
class __attribute__((annotate("$clingAutoload$CbmRecoStsPixel.h")))  CbmRecoStsPixel;
class __attribute__((annotate("$clingAutoload$CbmStsTrackFinderIdeal.h")))  CbmStsTrackFinderIdeal;
class __attribute__((annotate("$clingAutoload$CbmStsUnpackMonitor.h")))  __attribute__((annotate("$clingAutoload$CbmStsUnpackAlgo.h")))  CbmStsUnpackMonitor;
class __attribute__((annotate("$clingAutoload$CbmStsUnpackAlgoBase.h")))  __attribute__((annotate("$clingAutoload$CbmStsUnpackAlgo.h")))  CbmStsUnpackAlgoBase;
class __attribute__((annotate("$clingAutoload$CbmStsUnpackAlgo.h")))  CbmStsUnpackAlgo;
class __attribute__((annotate("$clingAutoload$CbmStsUnpackAlgoLegacy.h")))  CbmStsUnpackAlgoLegacy;
class __attribute__((annotate("$clingAutoload$CbmStsUnpackConfig.h")))  CbmStsUnpackConfig;
class __attribute__((annotate("$clingAutoload$CbmStsFindTracksQa.h")))  CbmStsFindTracksQa;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmRecoSts dictionary payload"

#ifndef NO_ROOT
  #define NO_ROOT 1
#endif
#ifndef NO_ROOT
  #define NO_ROOT 1
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
#ifndef BOOST_SERIALIZATION_NO_LIB
  #define BOOST_SERIALIZATION_NO_LIB 1
#endif
#ifndef BOOST_SERIALIZATION_DYN_LINK
  #define BOOST_SERIALIZATION_DYN_LINK 1
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
#ifndef FMT_LOCALE
  #define FMT_LOCALE 1
#endif
#ifndef BOOST_PROGRAM_OPTIONS_NO_LIB
  #define BOOST_PROGRAM_OPTIONS_NO_LIB 1
#endif
#ifndef BOOST_PROGRAM_OPTIONS_DYN_LINK
  #define BOOST_PROGRAM_OPTIONS_DYN_LINK 1
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

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "CbmRecoSts.h"
#include "CbmStsAlgoAnaCluster.h"
#include "CbmStsAlgoFindClusters.h"
#include "CbmStsAlgoFindHits.h"
#include "CbmStsAlgoFindHitsOrtho.h"
#include "CbmStsFindTracks.h"
#include "CbmStsFindTracksEvents.h"
#include "CbmStsRecoModule.h"
#include "CbmRecoStsPixel.h"
#include "CbmStsTrackFinderIdeal.h"
#include "CbmStsUnpackAlgo.h"
#include "CbmStsUnpackAlgoBase.h"
#include "CbmStsUnpackAlgoLegacy.h"
#include "CbmStsUnpackConfig.h"
#include "CbmStsUnpackMonitor.h"
#include "CbmStsFindTracksQa.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmRecoSts", payloadCode, "@",
"CbmRecoStsPixel", payloadCode, "@",
"CbmStsAlgoAnaCluster", payloadCode, "@",
"CbmStsAlgoFindClusters", payloadCode, "@",
"CbmStsAlgoFindHits", payloadCode, "@",
"CbmStsAlgoFindHitsOrtho", payloadCode, "@",
"CbmStsFindTracks", payloadCode, "@",
"CbmStsFindTracksEvents", payloadCode, "@",
"CbmStsFindTracksQa", payloadCode, "@",
"CbmStsRecoModule", payloadCode, "@",
"CbmStsTrackFinderIdeal", payloadCode, "@",
"CbmStsUnpackAlgo", payloadCode, "@",
"CbmStsUnpackAlgoBase", payloadCode, "@",
"CbmStsUnpackAlgoLegacy", payloadCode, "@",
"CbmStsUnpackConfig", payloadCode, "@",
"CbmStsUnpackMonitor", payloadCode, "@",
"ECbmRecoMode", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmRecoSts",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmRecoSts_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmRecoSts_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmRecoSts() {
  TriggerDictionaryInitialization_libCbmRecoSts_Impl();
}
