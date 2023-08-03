// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIheinemanndIgpuunpackerdIcbmrootdIbuild2dIexternaldINICAFEMTOmIprefixdIsrcdINICAFEMTOmIbuilddIcutsdIG__NicaCutDict
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
#include "NicaSubCut.h"
#include "NicaCut.h"
#include "NicaCutContainer.h"
#include "NicaCutCollection.h"
#include "NicaEventCut.h"
#include "NicaTrackCut.h"
#include "NicaTwoTrackCut.h"
#include "NicaCutsAndMonitors.h"
#include "NicaEventImpactParameterCut.h"
#include "NicaEventModuloCut.h"
#include "NicaEventMultiplicityCut.h"
#include "NicaEventPhiCut.h"
#include "NicaEventPtMaxCut.h"
#include "NicaEventVirtualCut.h"
#include "NicaEventComplexCut.h"
#include "NicaEventVertexCut.h"
#include "NicaEventBinningCut.h"
#include "NicaEventPrimaryTracksNoCut.h"
#include "NicaTrackBasicToFCut.h"
#include "NicaTrackToFMass2Cut.h"
#include "NicaTrackDCACut.h"
#include "NicaTrackTpcCut.h"
#include "NicaTrackExpCut.h"
#include "NicaTrackTpcHitCut.h"
#include "NicaTrackTpcToFCut.h"
#include "NicaTrackDCAPipeCut.h"
#include "NicaTrackTpcToFThresholdlessCut.h"
#include "NicaTrackChi2Cut.h"
#include "NicaTrackBetaCut.h"
#include "NicaTrackEtaCut.h"
#include "NicaTrackMassCut.h"
#include "NicaTrackMtCut.h"
#include "NicaTrackPCut.h"
#include "NicaTrackPhiCut.h"
#include "NicaTrackPtCut.h"
#include "NicaTrackYCut.h"
#include "NicaTrackPzCut.h"
#include "NicaTrackChargeCut.h"
#include "NicaTrackBoostedMomentaCuts.h"
#include "NicaTrackBasicMCCut.h"
#include "NicaTrackFreezoutCut.h"
#include "NicaTrackMCCut.h"
#include "NicaTrackPdgCut.h"
#include "NicaTrackStartPositionCut.h"
#include "NicaTrackPdgBinCut.h"
#include "NicaTrackDeltaMomCut.h"
#include "NicaTrackDeltaMomentumCut.h"
#include "NicaTrackOnlyPrimariesCut.h"
#include "NicaTrackStatusCut.h"
#include "NicaTrackComplexCut.h"
#include "NicaTrackVirtualCut.h"
#include "NicaTrackFlagCuts.h"
#include "NicaTwoTrackEtaCut.h"
#include "NicaTwoTrackKtCut.h"
#include "NicaTwoTrackMinvCut.h"
#include "NicaTwoTrackPhiCut.h"
#include "NicaTwoTrackPtOrderCut.h"
#include "NicaTwoTrackDphiDetaCut.h"
#include "NicaTwoTrackPtSumCut.h"
#include "NicaTwoTrackRapidityCut.h"
#include "NicaTwoTrackDeltaEtaCut.h"
#include "NicaPairDeltaQinvCut.h"
#include "NicaTwoTrackLCMSCut.h"
#include "NicaTwoTrack3DCFCut.h"
#include "NicaTwoTrackComplexCut.h"
#include "NicaTwoTrackFemtoQinvCut.h"
#include "NicaTwoTrackSailorCowboyCut.h"
#include "NicaTwoTrackRadialDistanceCut.h"
#include "NicaTwoTrackDebugCut.h"
#include "NicaTwoTrackFemtoPRFCut.h"
#include "NicaPdgBinPairCut.h"
#include "NicaTwoTrackVirtualCut.h"
#include "NicaCutMonitor.h"
#include "NicaCutMonitorX.h"
#include "NicaCutMonitorXY.h"
#include "NicaCutMonitorXYZ.h"
#include "NicaPropertyMonitorX.h"
#include "NicaPropertyMonitorXY.h"
#include "NicaPropertyMonitorXYZ.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_NicaSubCut(void *p = nullptr);
   static void *newArray_NicaSubCut(Long_t size, void *p);
   static void delete_NicaSubCut(void *p);
   static void deleteArray_NicaSubCut(void *p);
   static void destruct_NicaSubCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaSubCut*)
   {
      ::NicaSubCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaSubCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaSubCut", ::NicaSubCut::Class_Version(), "NicaSubCut.h", 21,
                  typeid(::NicaSubCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaSubCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaSubCut) );
      instance.SetNew(&new_NicaSubCut);
      instance.SetNewArray(&newArray_NicaSubCut);
      instance.SetDelete(&delete_NicaSubCut);
      instance.SetDeleteArray(&deleteArray_NicaSubCut);
      instance.SetDestructor(&destruct_NicaSubCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaSubCut*)
   {
      return GenerateInitInstanceLocal((::NicaSubCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaSubCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaSubCutHisto(void *p = nullptr);
   static void *newArray_NicaSubCutHisto(Long_t size, void *p);
   static void delete_NicaSubCutHisto(void *p);
   static void deleteArray_NicaSubCutHisto(void *p);
   static void destruct_NicaSubCutHisto(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaSubCutHisto*)
   {
      ::NicaSubCutHisto *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaSubCutHisto >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaSubCutHisto", ::NicaSubCutHisto::Class_Version(), "NicaSubCut.h", 116,
                  typeid(::NicaSubCutHisto), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaSubCutHisto::Dictionary, isa_proxy, 4,
                  sizeof(::NicaSubCutHisto) );
      instance.SetNew(&new_NicaSubCutHisto);
      instance.SetNewArray(&newArray_NicaSubCutHisto);
      instance.SetDelete(&delete_NicaSubCutHisto);
      instance.SetDeleteArray(&deleteArray_NicaSubCutHisto);
      instance.SetDestructor(&destruct_NicaSubCutHisto);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaSubCutHisto*)
   {
      return GenerateInitInstanceLocal((::NicaSubCutHisto*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaSubCutHisto*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCut(void *p = nullptr);
   static void *newArray_NicaCut(Long_t size, void *p);
   static void delete_NicaCut(void *p);
   static void deleteArray_NicaCut(void *p);
   static void destruct_NicaCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCut*)
   {
      ::NicaCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCut", ::NicaCut::Class_Version(), "NicaCut.h", 36,
                  typeid(::NicaCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCut) );
      instance.SetNew(&new_NicaCut);
      instance.SetNewArray(&newArray_NicaCut);
      instance.SetDelete(&delete_NicaCut);
      instance.SetDeleteArray(&deleteArray_NicaCut);
      instance.SetDestructor(&destruct_NicaCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCut*)
   {
      return GenerateInitInstanceLocal((::NicaCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCutMonitor(void *p = nullptr);
   static void *newArray_NicaCutMonitor(Long_t size, void *p);
   static void delete_NicaCutMonitor(void *p);
   static void deleteArray_NicaCutMonitor(void *p);
   static void destruct_NicaCutMonitor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCutMonitor*)
   {
      ::NicaCutMonitor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCutMonitor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCutMonitor", ::NicaCutMonitor::Class_Version(), "NicaCutMonitor.h", 23,
                  typeid(::NicaCutMonitor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCutMonitor::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCutMonitor) );
      instance.SetNew(&new_NicaCutMonitor);
      instance.SetNewArray(&newArray_NicaCutMonitor);
      instance.SetDelete(&delete_NicaCutMonitor);
      instance.SetDeleteArray(&deleteArray_NicaCutMonitor);
      instance.SetDestructor(&destruct_NicaCutMonitor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCutMonitor*)
   {
      return GenerateInitInstanceLocal((::NicaCutMonitor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCutMonitor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCutCollection(void *p = nullptr);
   static void *newArray_NicaCutCollection(Long_t size, void *p);
   static void delete_NicaCutCollection(void *p);
   static void deleteArray_NicaCutCollection(void *p);
   static void destruct_NicaCutCollection(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCutCollection*)
   {
      ::NicaCutCollection *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCutCollection >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCutCollection", ::NicaCutCollection::Class_Version(), "NicaCutCollection.h", 29,
                  typeid(::NicaCutCollection), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCutCollection::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCutCollection) );
      instance.SetNew(&new_NicaCutCollection);
      instance.SetNewArray(&newArray_NicaCutCollection);
      instance.SetDelete(&delete_NicaCutCollection);
      instance.SetDeleteArray(&deleteArray_NicaCutCollection);
      instance.SetDestructor(&destruct_NicaCutCollection);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCutCollection*)
   {
      return GenerateInitInstanceLocal((::NicaCutCollection*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCutCollection*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCutContainer(void *p = nullptr);
   static void *newArray_NicaCutContainer(Long_t size, void *p);
   static void delete_NicaCutContainer(void *p);
   static void deleteArray_NicaCutContainer(void *p);
   static void destruct_NicaCutContainer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCutContainer*)
   {
      ::NicaCutContainer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCutContainer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCutContainer", ::NicaCutContainer::Class_Version(), "NicaCutContainer.h", 25,
                  typeid(::NicaCutContainer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCutContainer::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCutContainer) );
      instance.SetNew(&new_NicaCutContainer);
      instance.SetNewArray(&newArray_NicaCutContainer);
      instance.SetDelete(&delete_NicaCutContainer);
      instance.SetDeleteArray(&deleteArray_NicaCutContainer);
      instance.SetDestructor(&destruct_NicaCutContainer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCutContainer*)
   {
      return GenerateInitInstanceLocal((::NicaCutContainer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCutContainer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaEventCut(void *p);
   static void deleteArray_NicaEventCut(void *p);
   static void destruct_NicaEventCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventCut*)
   {
      ::NicaEventCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventCut", ::NicaEventCut::Class_Version(), "NicaEventCut.h", 16,
                  typeid(::NicaEventCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventCut) );
      instance.SetDelete(&delete_NicaEventCut);
      instance.SetDeleteArray(&deleteArray_NicaEventCut);
      instance.SetDestructor(&destruct_NicaEventCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaTrackCut(void *p);
   static void deleteArray_NicaTrackCut(void *p);
   static void destruct_NicaTrackCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackCut*)
   {
      ::NicaTrackCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackCut", ::NicaTrackCut::Class_Version(), "NicaTrackCut.h", 17,
                  typeid(::NicaTrackCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackCut) );
      instance.SetDelete(&delete_NicaTrackCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackCut);
      instance.SetDestructor(&destruct_NicaTrackCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaTwoTrackCut(void *p);
   static void deleteArray_NicaTwoTrackCut(void *p);
   static void destruct_NicaTwoTrackCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackCut*)
   {
      ::NicaTwoTrackCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackCut", ::NicaTwoTrackCut::Class_Version(), "NicaTwoTrackCut.h", 20,
                  typeid(::NicaTwoTrackCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackCut) );
      instance.SetDelete(&delete_NicaTwoTrackCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackCut);
      instance.SetDestructor(&destruct_NicaTwoTrackCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCutsAndMonitors(void *p = nullptr);
   static void *newArray_NicaCutsAndMonitors(Long_t size, void *p);
   static void delete_NicaCutsAndMonitors(void *p);
   static void deleteArray_NicaCutsAndMonitors(void *p);
   static void destruct_NicaCutsAndMonitors(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCutsAndMonitors*)
   {
      ::NicaCutsAndMonitors *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCutsAndMonitors >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCutsAndMonitors", ::NicaCutsAndMonitors::Class_Version(), "NicaCutsAndMonitors.h", 28,
                  typeid(::NicaCutsAndMonitors), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCutsAndMonitors::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCutsAndMonitors) );
      instance.SetNew(&new_NicaCutsAndMonitors);
      instance.SetNewArray(&newArray_NicaCutsAndMonitors);
      instance.SetDelete(&delete_NicaCutsAndMonitors);
      instance.SetDeleteArray(&deleteArray_NicaCutsAndMonitors);
      instance.SetDestructor(&destruct_NicaCutsAndMonitors);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCutsAndMonitors*)
   {
      return GenerateInitInstanceLocal((::NicaCutsAndMonitors*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCutsAndMonitors*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventImpactParameterCut(void *p = nullptr);
   static void *newArray_NicaEventImpactParameterCut(Long_t size, void *p);
   static void delete_NicaEventImpactParameterCut(void *p);
   static void deleteArray_NicaEventImpactParameterCut(void *p);
   static void destruct_NicaEventImpactParameterCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventImpactParameterCut*)
   {
      ::NicaEventImpactParameterCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventImpactParameterCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventImpactParameterCut", ::NicaEventImpactParameterCut::Class_Version(), "NicaEventImpactParameterCut.h", 17,
                  typeid(::NicaEventImpactParameterCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventImpactParameterCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventImpactParameterCut) );
      instance.SetNew(&new_NicaEventImpactParameterCut);
      instance.SetNewArray(&newArray_NicaEventImpactParameterCut);
      instance.SetDelete(&delete_NicaEventImpactParameterCut);
      instance.SetDeleteArray(&deleteArray_NicaEventImpactParameterCut);
      instance.SetDestructor(&destruct_NicaEventImpactParameterCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventImpactParameterCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventImpactParameterCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventImpactParameterCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventImpactCentralityCut(void *p = nullptr);
   static void *newArray_NicaEventImpactCentralityCut(Long_t size, void *p);
   static void delete_NicaEventImpactCentralityCut(void *p);
   static void deleteArray_NicaEventImpactCentralityCut(void *p);
   static void destruct_NicaEventImpactCentralityCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventImpactCentralityCut*)
   {
      ::NicaEventImpactCentralityCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventImpactCentralityCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventImpactCentralityCut", ::NicaEventImpactCentralityCut::Class_Version(), "NicaEventImpactParameterCut.h", 30,
                  typeid(::NicaEventImpactCentralityCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventImpactCentralityCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventImpactCentralityCut) );
      instance.SetNew(&new_NicaEventImpactCentralityCut);
      instance.SetNewArray(&newArray_NicaEventImpactCentralityCut);
      instance.SetDelete(&delete_NicaEventImpactCentralityCut);
      instance.SetDeleteArray(&deleteArray_NicaEventImpactCentralityCut);
      instance.SetDestructor(&destruct_NicaEventImpactCentralityCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventImpactCentralityCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventImpactCentralityCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventImpactCentralityCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventModuloCut(void *p = nullptr);
   static void *newArray_NicaEventModuloCut(Long_t size, void *p);
   static void delete_NicaEventModuloCut(void *p);
   static void deleteArray_NicaEventModuloCut(void *p);
   static void destruct_NicaEventModuloCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventModuloCut*)
   {
      ::NicaEventModuloCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventModuloCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventModuloCut", ::NicaEventModuloCut::Class_Version(), "NicaEventModuloCut.h", 18,
                  typeid(::NicaEventModuloCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventModuloCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventModuloCut) );
      instance.SetNew(&new_NicaEventModuloCut);
      instance.SetNewArray(&newArray_NicaEventModuloCut);
      instance.SetDelete(&delete_NicaEventModuloCut);
      instance.SetDeleteArray(&deleteArray_NicaEventModuloCut);
      instance.SetDestructor(&destruct_NicaEventModuloCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventModuloCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventModuloCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventModuloCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventMultiplicityCut(void *p = nullptr);
   static void *newArray_NicaEventMultiplicityCut(Long_t size, void *p);
   static void delete_NicaEventMultiplicityCut(void *p);
   static void deleteArray_NicaEventMultiplicityCut(void *p);
   static void destruct_NicaEventMultiplicityCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventMultiplicityCut*)
   {
      ::NicaEventMultiplicityCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventMultiplicityCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventMultiplicityCut", ::NicaEventMultiplicityCut::Class_Version(), "NicaEventMultiplicityCut.h", 19,
                  typeid(::NicaEventMultiplicityCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventMultiplicityCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventMultiplicityCut) );
      instance.SetNew(&new_NicaEventMultiplicityCut);
      instance.SetNewArray(&newArray_NicaEventMultiplicityCut);
      instance.SetDelete(&delete_NicaEventMultiplicityCut);
      instance.SetDeleteArray(&deleteArray_NicaEventMultiplicityCut);
      instance.SetDestructor(&destruct_NicaEventMultiplicityCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventMultiplicityCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventMultiplicityCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventMultiplicityCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventMultiChargedCut(void *p = nullptr);
   static void *newArray_NicaEventMultiChargedCut(Long_t size, void *p);
   static void delete_NicaEventMultiChargedCut(void *p);
   static void deleteArray_NicaEventMultiChargedCut(void *p);
   static void destruct_NicaEventMultiChargedCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventMultiChargedCut*)
   {
      ::NicaEventMultiChargedCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventMultiChargedCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventMultiChargedCut", ::NicaEventMultiChargedCut::Class_Version(), "NicaEventMultiplicityCut.h", 31,
                  typeid(::NicaEventMultiChargedCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventMultiChargedCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventMultiChargedCut) );
      instance.SetNew(&new_NicaEventMultiChargedCut);
      instance.SetNewArray(&newArray_NicaEventMultiChargedCut);
      instance.SetDelete(&delete_NicaEventMultiChargedCut);
      instance.SetDeleteArray(&deleteArray_NicaEventMultiChargedCut);
      instance.SetDestructor(&destruct_NicaEventMultiChargedCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventMultiChargedCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventMultiChargedCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventMultiChargedCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventPhiCut(void *p = nullptr);
   static void *newArray_NicaEventPhiCut(Long_t size, void *p);
   static void delete_NicaEventPhiCut(void *p);
   static void deleteArray_NicaEventPhiCut(void *p);
   static void destruct_NicaEventPhiCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventPhiCut*)
   {
      ::NicaEventPhiCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventPhiCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventPhiCut", ::NicaEventPhiCut::Class_Version(), "NicaEventPhiCut.h", 15,
                  typeid(::NicaEventPhiCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventPhiCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventPhiCut) );
      instance.SetNew(&new_NicaEventPhiCut);
      instance.SetNewArray(&newArray_NicaEventPhiCut);
      instance.SetDelete(&delete_NicaEventPhiCut);
      instance.SetDeleteArray(&deleteArray_NicaEventPhiCut);
      instance.SetDestructor(&destruct_NicaEventPhiCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventPhiCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventPhiCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventPhiCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventPtMaxCut(void *p = nullptr);
   static void *newArray_NicaEventPtMaxCut(Long_t size, void *p);
   static void delete_NicaEventPtMaxCut(void *p);
   static void deleteArray_NicaEventPtMaxCut(void *p);
   static void destruct_NicaEventPtMaxCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventPtMaxCut*)
   {
      ::NicaEventPtMaxCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventPtMaxCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventPtMaxCut", ::NicaEventPtMaxCut::Class_Version(), "NicaEventPtMaxCut.h", 17,
                  typeid(::NicaEventPtMaxCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventPtMaxCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventPtMaxCut) );
      instance.SetNew(&new_NicaEventPtMaxCut);
      instance.SetNewArray(&newArray_NicaEventPtMaxCut);
      instance.SetDelete(&delete_NicaEventPtMaxCut);
      instance.SetDeleteArray(&deleteArray_NicaEventPtMaxCut);
      instance.SetDestructor(&destruct_NicaEventPtMaxCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventPtMaxCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventPtMaxCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventPtMaxCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventVirtualCut(void *p = nullptr);
   static void *newArray_NicaEventVirtualCut(Long_t size, void *p);
   static void delete_NicaEventVirtualCut(void *p);
   static void deleteArray_NicaEventVirtualCut(void *p);
   static void destruct_NicaEventVirtualCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventVirtualCut*)
   {
      ::NicaEventVirtualCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventVirtualCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventVirtualCut", ::NicaEventVirtualCut::Class_Version(), "NicaEventVirtualCut.h", 17,
                  typeid(::NicaEventVirtualCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventVirtualCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventVirtualCut) );
      instance.SetNew(&new_NicaEventVirtualCut);
      instance.SetNewArray(&newArray_NicaEventVirtualCut);
      instance.SetDelete(&delete_NicaEventVirtualCut);
      instance.SetDeleteArray(&deleteArray_NicaEventVirtualCut);
      instance.SetDestructor(&destruct_NicaEventVirtualCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventVirtualCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventVirtualCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventVirtualCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventComplexCut(void *p = nullptr);
   static void *newArray_NicaEventComplexCut(Long_t size, void *p);
   static void delete_NicaEventComplexCut(void *p);
   static void deleteArray_NicaEventComplexCut(void *p);
   static void destruct_NicaEventComplexCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventComplexCut*)
   {
      ::NicaEventComplexCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventComplexCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventComplexCut", ::NicaEventComplexCut::Class_Version(), "NicaEventComplexCut.h", 14,
                  typeid(::NicaEventComplexCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventComplexCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventComplexCut) );
      instance.SetNew(&new_NicaEventComplexCut);
      instance.SetNewArray(&newArray_NicaEventComplexCut);
      instance.SetDelete(&delete_NicaEventComplexCut);
      instance.SetDeleteArray(&deleteArray_NicaEventComplexCut);
      instance.SetDestructor(&destruct_NicaEventComplexCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventComplexCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventComplexCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventComplexCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventRealCut(void *p = nullptr);
   static void *newArray_NicaEventRealCut(Long_t size, void *p);
   static void delete_NicaEventRealCut(void *p);
   static void deleteArray_NicaEventRealCut(void *p);
   static void destruct_NicaEventRealCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventRealCut*)
   {
      ::NicaEventRealCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventRealCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventRealCut", ::NicaEventRealCut::Class_Version(), "NicaEventComplexCut.h", 37,
                  typeid(::NicaEventRealCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventRealCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventRealCut) );
      instance.SetNew(&new_NicaEventRealCut);
      instance.SetNewArray(&newArray_NicaEventRealCut);
      instance.SetDelete(&delete_NicaEventRealCut);
      instance.SetDeleteArray(&deleteArray_NicaEventRealCut);
      instance.SetDestructor(&destruct_NicaEventRealCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventRealCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventRealCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventRealCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventImaginaryCut(void *p = nullptr);
   static void *newArray_NicaEventImaginaryCut(Long_t size, void *p);
   static void delete_NicaEventImaginaryCut(void *p);
   static void deleteArray_NicaEventImaginaryCut(void *p);
   static void destruct_NicaEventImaginaryCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventImaginaryCut*)
   {
      ::NicaEventImaginaryCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventImaginaryCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventImaginaryCut", ::NicaEventImaginaryCut::Class_Version(), "NicaEventComplexCut.h", 54,
                  typeid(::NicaEventImaginaryCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventImaginaryCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventImaginaryCut) );
      instance.SetNew(&new_NicaEventImaginaryCut);
      instance.SetNewArray(&newArray_NicaEventImaginaryCut);
      instance.SetDelete(&delete_NicaEventImaginaryCut);
      instance.SetDeleteArray(&deleteArray_NicaEventImaginaryCut);
      instance.SetDestructor(&destruct_NicaEventImaginaryCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventImaginaryCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventImaginaryCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventImaginaryCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventVertexCut(void *p = nullptr);
   static void *newArray_NicaEventVertexCut(Long_t size, void *p);
   static void delete_NicaEventVertexCut(void *p);
   static void deleteArray_NicaEventVertexCut(void *p);
   static void destruct_NicaEventVertexCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventVertexCut*)
   {
      ::NicaEventVertexCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventVertexCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventVertexCut", ::NicaEventVertexCut::Class_Version(), "NicaEventVertexCut.h", 13,
                  typeid(::NicaEventVertexCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventVertexCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventVertexCut) );
      instance.SetNew(&new_NicaEventVertexCut);
      instance.SetNewArray(&newArray_NicaEventVertexCut);
      instance.SetDelete(&delete_NicaEventVertexCut);
      instance.SetDeleteArray(&deleteArray_NicaEventVertexCut);
      instance.SetDestructor(&destruct_NicaEventVertexCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventVertexCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventVertexCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventVertexCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventBinningCut(void *p = nullptr);
   static void *newArray_NicaEventBinningCut(Long_t size, void *p);
   static void delete_NicaEventBinningCut(void *p);
   static void deleteArray_NicaEventBinningCut(void *p);
   static void destruct_NicaEventBinningCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventBinningCut*)
   {
      ::NicaEventBinningCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventBinningCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventBinningCut", ::NicaEventBinningCut::Class_Version(), "NicaEventBinningCut.h", 19,
                  typeid(::NicaEventBinningCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventBinningCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventBinningCut) );
      instance.SetNew(&new_NicaEventBinningCut);
      instance.SetNewArray(&newArray_NicaEventBinningCut);
      instance.SetDelete(&delete_NicaEventBinningCut);
      instance.SetDeleteArray(&deleteArray_NicaEventBinningCut);
      instance.SetDestructor(&destruct_NicaEventBinningCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventBinningCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventBinningCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventBinningCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventPrimaryTracksNoCut(void *p = nullptr);
   static void *newArray_NicaEventPrimaryTracksNoCut(Long_t size, void *p);
   static void delete_NicaEventPrimaryTracksNoCut(void *p);
   static void deleteArray_NicaEventPrimaryTracksNoCut(void *p);
   static void destruct_NicaEventPrimaryTracksNoCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventPrimaryTracksNoCut*)
   {
      ::NicaEventPrimaryTracksNoCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventPrimaryTracksNoCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventPrimaryTracksNoCut", ::NicaEventPrimaryTracksNoCut::Class_Version(), "NicaEventPrimaryTracksNoCut.h", 14,
                  typeid(::NicaEventPrimaryTracksNoCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventPrimaryTracksNoCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventPrimaryTracksNoCut) );
      instance.SetNew(&new_NicaEventPrimaryTracksNoCut);
      instance.SetNewArray(&newArray_NicaEventPrimaryTracksNoCut);
      instance.SetDelete(&delete_NicaEventPrimaryTracksNoCut);
      instance.SetDeleteArray(&deleteArray_NicaEventPrimaryTracksNoCut);
      instance.SetDestructor(&destruct_NicaEventPrimaryTracksNoCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventPrimaryTracksNoCut*)
   {
      return GenerateInitInstanceLocal((::NicaEventPrimaryTracksNoCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventPrimaryTracksNoCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaTrackExpCut(void *p);
   static void deleteArray_NicaTrackExpCut(void *p);
   static void destruct_NicaTrackExpCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackExpCut*)
   {
      ::NicaTrackExpCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackExpCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackExpCut", ::NicaTrackExpCut::Class_Version(), "NicaTrackExpCut.h", 16,
                  typeid(::NicaTrackExpCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackExpCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackExpCut) );
      instance.SetDelete(&delete_NicaTrackExpCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackExpCut);
      instance.SetDestructor(&destruct_NicaTrackExpCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackExpCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackExpCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackExpCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackBasicToFCut(void *p = nullptr);
   static void *newArray_NicaTrackBasicToFCut(Long_t size, void *p);
   static void delete_NicaTrackBasicToFCut(void *p);
   static void deleteArray_NicaTrackBasicToFCut(void *p);
   static void destruct_NicaTrackBasicToFCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackBasicToFCut*)
   {
      ::NicaTrackBasicToFCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackBasicToFCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackBasicToFCut", ::NicaTrackBasicToFCut::Class_Version(), "NicaTrackBasicToFCut.h", 13,
                  typeid(::NicaTrackBasicToFCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackBasicToFCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackBasicToFCut) );
      instance.SetNew(&new_NicaTrackBasicToFCut);
      instance.SetNewArray(&newArray_NicaTrackBasicToFCut);
      instance.SetDelete(&delete_NicaTrackBasicToFCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackBasicToFCut);
      instance.SetDestructor(&destruct_NicaTrackBasicToFCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackBasicToFCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackBasicToFCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackBasicToFCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackToFMass2Cut(void *p = nullptr);
   static void *newArray_NicaTrackToFMass2Cut(Long_t size, void *p);
   static void delete_NicaTrackToFMass2Cut(void *p);
   static void deleteArray_NicaTrackToFMass2Cut(void *p);
   static void destruct_NicaTrackToFMass2Cut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackToFMass2Cut*)
   {
      ::NicaTrackToFMass2Cut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackToFMass2Cut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackToFMass2Cut", ::NicaTrackToFMass2Cut::Class_Version(), "NicaTrackToFMass2Cut.h", 18,
                  typeid(::NicaTrackToFMass2Cut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackToFMass2Cut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackToFMass2Cut) );
      instance.SetNew(&new_NicaTrackToFMass2Cut);
      instance.SetNewArray(&newArray_NicaTrackToFMass2Cut);
      instance.SetDelete(&delete_NicaTrackToFMass2Cut);
      instance.SetDeleteArray(&deleteArray_NicaTrackToFMass2Cut);
      instance.SetDestructor(&destruct_NicaTrackToFMass2Cut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackToFMass2Cut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackToFMass2Cut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackToFMass2Cut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackTofMass2CutNoZeros(void *p = nullptr);
   static void *newArray_NicaTrackTofMass2CutNoZeros(Long_t size, void *p);
   static void delete_NicaTrackTofMass2CutNoZeros(void *p);
   static void deleteArray_NicaTrackTofMass2CutNoZeros(void *p);
   static void destruct_NicaTrackTofMass2CutNoZeros(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackTofMass2CutNoZeros*)
   {
      ::NicaTrackTofMass2CutNoZeros *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackTofMass2CutNoZeros >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackTofMass2CutNoZeros", ::NicaTrackTofMass2CutNoZeros::Class_Version(), "NicaTrackToFMass2Cut.h", 47,
                  typeid(::NicaTrackTofMass2CutNoZeros), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackTofMass2CutNoZeros::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackTofMass2CutNoZeros) );
      instance.SetNew(&new_NicaTrackTofMass2CutNoZeros);
      instance.SetNewArray(&newArray_NicaTrackTofMass2CutNoZeros);
      instance.SetDelete(&delete_NicaTrackTofMass2CutNoZeros);
      instance.SetDeleteArray(&deleteArray_NicaTrackTofMass2CutNoZeros);
      instance.SetDestructor(&destruct_NicaTrackTofMass2CutNoZeros);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackTofMass2CutNoZeros*)
   {
      return GenerateInitInstanceLocal((::NicaTrackTofMass2CutNoZeros*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackTofMass2CutNoZeros*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackDCACut(void *p = nullptr);
   static void *newArray_NicaTrackDCACut(Long_t size, void *p);
   static void delete_NicaTrackDCACut(void *p);
   static void deleteArray_NicaTrackDCACut(void *p);
   static void destruct_NicaTrackDCACut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackDCACut*)
   {
      ::NicaTrackDCACut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackDCACut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackDCACut", ::NicaTrackDCACut::Class_Version(), "NicaTrackDCACut.h", 14,
                  typeid(::NicaTrackDCACut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackDCACut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackDCACut) );
      instance.SetNew(&new_NicaTrackDCACut);
      instance.SetNewArray(&newArray_NicaTrackDCACut);
      instance.SetDelete(&delete_NicaTrackDCACut);
      instance.SetDeleteArray(&deleteArray_NicaTrackDCACut);
      instance.SetDestructor(&destruct_NicaTrackDCACut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackDCACut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackDCACut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackDCACut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackTpcCut(void *p = nullptr);
   static void *newArray_NicaTrackTpcCut(Long_t size, void *p);
   static void delete_NicaTrackTpcCut(void *p);
   static void deleteArray_NicaTrackTpcCut(void *p);
   static void destruct_NicaTrackTpcCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackTpcCut*)
   {
      ::NicaTrackTpcCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackTpcCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackTpcCut", ::NicaTrackTpcCut::Class_Version(), "NicaTrackTpcCut.h", 24,
                  typeid(::NicaTrackTpcCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackTpcCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackTpcCut) );
      instance.SetNew(&new_NicaTrackTpcCut);
      instance.SetNewArray(&newArray_NicaTrackTpcCut);
      instance.SetDelete(&delete_NicaTrackTpcCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackTpcCut);
      instance.SetDestructor(&destruct_NicaTrackTpcCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackTpcCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackTpcCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackTpcCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackTpcHitCut(void *p = nullptr);
   static void *newArray_NicaTrackTpcHitCut(Long_t size, void *p);
   static void delete_NicaTrackTpcHitCut(void *p);
   static void deleteArray_NicaTrackTpcHitCut(void *p);
   static void destruct_NicaTrackTpcHitCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackTpcHitCut*)
   {
      ::NicaTrackTpcHitCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackTpcHitCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackTpcHitCut", ::NicaTrackTpcHitCut::Class_Version(), "NicaTrackTpcHitCut.h", 14,
                  typeid(::NicaTrackTpcHitCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackTpcHitCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackTpcHitCut) );
      instance.SetNew(&new_NicaTrackTpcHitCut);
      instance.SetNewArray(&newArray_NicaTrackTpcHitCut);
      instance.SetDelete(&delete_NicaTrackTpcHitCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackTpcHitCut);
      instance.SetDestructor(&destruct_NicaTrackTpcHitCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackTpcHitCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackTpcHitCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackTpcHitCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackTpcToFCut(void *p = nullptr);
   static void *newArray_NicaTrackTpcToFCut(Long_t size, void *p);
   static void delete_NicaTrackTpcToFCut(void *p);
   static void deleteArray_NicaTrackTpcToFCut(void *p);
   static void destruct_NicaTrackTpcToFCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackTpcToFCut*)
   {
      ::NicaTrackTpcToFCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackTpcToFCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackTpcToFCut", ::NicaTrackTpcToFCut::Class_Version(), "NicaTrackTpcToFCut.h", 15,
                  typeid(::NicaTrackTpcToFCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackTpcToFCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackTpcToFCut) );
      instance.SetNew(&new_NicaTrackTpcToFCut);
      instance.SetNewArray(&newArray_NicaTrackTpcToFCut);
      instance.SetDelete(&delete_NicaTrackTpcToFCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackTpcToFCut);
      instance.SetDestructor(&destruct_NicaTrackTpcToFCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackTpcToFCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackTpcToFCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackTpcToFCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackDCAPipeCut(void *p = nullptr);
   static void *newArray_NicaTrackDCAPipeCut(Long_t size, void *p);
   static void delete_NicaTrackDCAPipeCut(void *p);
   static void deleteArray_NicaTrackDCAPipeCut(void *p);
   static void destruct_NicaTrackDCAPipeCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackDCAPipeCut*)
   {
      ::NicaTrackDCAPipeCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackDCAPipeCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackDCAPipeCut", ::NicaTrackDCAPipeCut::Class_Version(), "NicaTrackDCAPipeCut.h", 12,
                  typeid(::NicaTrackDCAPipeCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackDCAPipeCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackDCAPipeCut) );
      instance.SetNew(&new_NicaTrackDCAPipeCut);
      instance.SetNewArray(&newArray_NicaTrackDCAPipeCut);
      instance.SetDelete(&delete_NicaTrackDCAPipeCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackDCAPipeCut);
      instance.SetDestructor(&destruct_NicaTrackDCAPipeCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackDCAPipeCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackDCAPipeCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackDCAPipeCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackTpcToFThresholdlessCut(void *p = nullptr);
   static void *newArray_NicaTrackTpcToFThresholdlessCut(Long_t size, void *p);
   static void delete_NicaTrackTpcToFThresholdlessCut(void *p);
   static void deleteArray_NicaTrackTpcToFThresholdlessCut(void *p);
   static void destruct_NicaTrackTpcToFThresholdlessCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackTpcToFThresholdlessCut*)
   {
      ::NicaTrackTpcToFThresholdlessCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackTpcToFThresholdlessCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackTpcToFThresholdlessCut", ::NicaTrackTpcToFThresholdlessCut::Class_Version(), "NicaTrackTpcToFThresholdlessCut.h", 14,
                  typeid(::NicaTrackTpcToFThresholdlessCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackTpcToFThresholdlessCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackTpcToFThresholdlessCut) );
      instance.SetNew(&new_NicaTrackTpcToFThresholdlessCut);
      instance.SetNewArray(&newArray_NicaTrackTpcToFThresholdlessCut);
      instance.SetDelete(&delete_NicaTrackTpcToFThresholdlessCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackTpcToFThresholdlessCut);
      instance.SetDestructor(&destruct_NicaTrackTpcToFThresholdlessCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackTpcToFThresholdlessCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackTpcToFThresholdlessCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackTpcToFThresholdlessCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackChi2Cut(void *p = nullptr);
   static void *newArray_NicaTrackChi2Cut(Long_t size, void *p);
   static void delete_NicaTrackChi2Cut(void *p);
   static void deleteArray_NicaTrackChi2Cut(void *p);
   static void destruct_NicaTrackChi2Cut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackChi2Cut*)
   {
      ::NicaTrackChi2Cut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackChi2Cut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackChi2Cut", ::NicaTrackChi2Cut::Class_Version(), "NicaTrackChi2Cut.h", 12,
                  typeid(::NicaTrackChi2Cut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackChi2Cut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackChi2Cut) );
      instance.SetNew(&new_NicaTrackChi2Cut);
      instance.SetNewArray(&newArray_NicaTrackChi2Cut);
      instance.SetDelete(&delete_NicaTrackChi2Cut);
      instance.SetDeleteArray(&deleteArray_NicaTrackChi2Cut);
      instance.SetDestructor(&destruct_NicaTrackChi2Cut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackChi2Cut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackChi2Cut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackChi2Cut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackBetaCut(void *p = nullptr);
   static void *newArray_NicaTrackBetaCut(Long_t size, void *p);
   static void delete_NicaTrackBetaCut(void *p);
   static void deleteArray_NicaTrackBetaCut(void *p);
   static void destruct_NicaTrackBetaCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackBetaCut*)
   {
      ::NicaTrackBetaCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackBetaCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackBetaCut", ::NicaTrackBetaCut::Class_Version(), "NicaTrackBetaCut.h", 16,
                  typeid(::NicaTrackBetaCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackBetaCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackBetaCut) );
      instance.SetNew(&new_NicaTrackBetaCut);
      instance.SetNewArray(&newArray_NicaTrackBetaCut);
      instance.SetDelete(&delete_NicaTrackBetaCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackBetaCut);
      instance.SetDestructor(&destruct_NicaTrackBetaCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackBetaCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackBetaCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackBetaCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackEtaCut(void *p = nullptr);
   static void *newArray_NicaTrackEtaCut(Long_t size, void *p);
   static void delete_NicaTrackEtaCut(void *p);
   static void deleteArray_NicaTrackEtaCut(void *p);
   static void destruct_NicaTrackEtaCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackEtaCut*)
   {
      ::NicaTrackEtaCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackEtaCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackEtaCut", ::NicaTrackEtaCut::Class_Version(), "NicaTrackEtaCut.h", 17,
                  typeid(::NicaTrackEtaCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackEtaCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackEtaCut) );
      instance.SetNew(&new_NicaTrackEtaCut);
      instance.SetNewArray(&newArray_NicaTrackEtaCut);
      instance.SetDelete(&delete_NicaTrackEtaCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackEtaCut);
      instance.SetDestructor(&destruct_NicaTrackEtaCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackEtaCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackEtaCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackEtaCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackEtaAbsCut(void *p = nullptr);
   static void *newArray_NicaTrackEtaAbsCut(Long_t size, void *p);
   static void delete_NicaTrackEtaAbsCut(void *p);
   static void deleteArray_NicaTrackEtaAbsCut(void *p);
   static void destruct_NicaTrackEtaAbsCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackEtaAbsCut*)
   {
      ::NicaTrackEtaAbsCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackEtaAbsCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackEtaAbsCut", ::NicaTrackEtaAbsCut::Class_Version(), "NicaTrackEtaCut.h", 29,
                  typeid(::NicaTrackEtaAbsCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackEtaAbsCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackEtaAbsCut) );
      instance.SetNew(&new_NicaTrackEtaAbsCut);
      instance.SetNewArray(&newArray_NicaTrackEtaAbsCut);
      instance.SetDelete(&delete_NicaTrackEtaAbsCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackEtaAbsCut);
      instance.SetDestructor(&destruct_NicaTrackEtaAbsCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackEtaAbsCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackEtaAbsCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackEtaAbsCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackMassCut(void *p = nullptr);
   static void *newArray_NicaTrackMassCut(Long_t size, void *p);
   static void delete_NicaTrackMassCut(void *p);
   static void deleteArray_NicaTrackMassCut(void *p);
   static void destruct_NicaTrackMassCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackMassCut*)
   {
      ::NicaTrackMassCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackMassCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackMassCut", ::NicaTrackMassCut::Class_Version(), "NicaTrackMassCut.h", 17,
                  typeid(::NicaTrackMassCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackMassCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackMassCut) );
      instance.SetNew(&new_NicaTrackMassCut);
      instance.SetNewArray(&newArray_NicaTrackMassCut);
      instance.SetDelete(&delete_NicaTrackMassCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackMassCut);
      instance.SetDestructor(&destruct_NicaTrackMassCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackMassCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackMassCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackMassCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackMtCut(void *p = nullptr);
   static void *newArray_NicaTrackMtCut(Long_t size, void *p);
   static void delete_NicaTrackMtCut(void *p);
   static void deleteArray_NicaTrackMtCut(void *p);
   static void destruct_NicaTrackMtCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackMtCut*)
   {
      ::NicaTrackMtCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackMtCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackMtCut", ::NicaTrackMtCut::Class_Version(), "NicaTrackMtCut.h", 17,
                  typeid(::NicaTrackMtCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackMtCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackMtCut) );
      instance.SetNew(&new_NicaTrackMtCut);
      instance.SetNewArray(&newArray_NicaTrackMtCut);
      instance.SetDelete(&delete_NicaTrackMtCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackMtCut);
      instance.SetDestructor(&destruct_NicaTrackMtCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackMtCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackMtCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackMtCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackPCut(void *p = nullptr);
   static void *newArray_NicaTrackPCut(Long_t size, void *p);
   static void delete_NicaTrackPCut(void *p);
   static void deleteArray_NicaTrackPCut(void *p);
   static void destruct_NicaTrackPCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackPCut*)
   {
      ::NicaTrackPCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackPCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackPCut", ::NicaTrackPCut::Class_Version(), "NicaTrackPCut.h", 12,
                  typeid(::NicaTrackPCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackPCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackPCut) );
      instance.SetNew(&new_NicaTrackPCut);
      instance.SetNewArray(&newArray_NicaTrackPCut);
      instance.SetDelete(&delete_NicaTrackPCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackPCut);
      instance.SetDestructor(&destruct_NicaTrackPCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackPCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackPCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackPCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackPSignedCut(void *p = nullptr);
   static void *newArray_NicaTrackPSignedCut(Long_t size, void *p);
   static void delete_NicaTrackPSignedCut(void *p);
   static void deleteArray_NicaTrackPSignedCut(void *p);
   static void destruct_NicaTrackPSignedCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackPSignedCut*)
   {
      ::NicaTrackPSignedCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackPSignedCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackPSignedCut", ::NicaTrackPSignedCut::Class_Version(), "NicaTrackPCut.h", 20,
                  typeid(::NicaTrackPSignedCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackPSignedCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackPSignedCut) );
      instance.SetNew(&new_NicaTrackPSignedCut);
      instance.SetNewArray(&newArray_NicaTrackPSignedCut);
      instance.SetDelete(&delete_NicaTrackPSignedCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackPSignedCut);
      instance.SetDestructor(&destruct_NicaTrackPSignedCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackPSignedCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackPSignedCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackPSignedCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackPhiCut(void *p = nullptr);
   static void *newArray_NicaTrackPhiCut(Long_t size, void *p);
   static void delete_NicaTrackPhiCut(void *p);
   static void deleteArray_NicaTrackPhiCut(void *p);
   static void destruct_NicaTrackPhiCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackPhiCut*)
   {
      ::NicaTrackPhiCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackPhiCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackPhiCut", ::NicaTrackPhiCut::Class_Version(), "NicaTrackPhiCut.h", 15,
                  typeid(::NicaTrackPhiCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackPhiCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackPhiCut) );
      instance.SetNew(&new_NicaTrackPhiCut);
      instance.SetNewArray(&newArray_NicaTrackPhiCut);
      instance.SetDelete(&delete_NicaTrackPhiCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackPhiCut);
      instance.SetDestructor(&destruct_NicaTrackPhiCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackPhiCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackPhiCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackPhiCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackPtCut(void *p = nullptr);
   static void *newArray_NicaTrackPtCut(Long_t size, void *p);
   static void delete_NicaTrackPtCut(void *p);
   static void deleteArray_NicaTrackPtCut(void *p);
   static void destruct_NicaTrackPtCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackPtCut*)
   {
      ::NicaTrackPtCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackPtCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackPtCut", ::NicaTrackPtCut::Class_Version(), "NicaTrackPtCut.h", 17,
                  typeid(::NicaTrackPtCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackPtCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackPtCut) );
      instance.SetNew(&new_NicaTrackPtCut);
      instance.SetNewArray(&newArray_NicaTrackPtCut);
      instance.SetDelete(&delete_NicaTrackPtCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackPtCut);
      instance.SetDestructor(&destruct_NicaTrackPtCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackPtCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackPtCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackPtCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackYCut(void *p = nullptr);
   static void *newArray_NicaTrackYCut(Long_t size, void *p);
   static void delete_NicaTrackYCut(void *p);
   static void deleteArray_NicaTrackYCut(void *p);
   static void destruct_NicaTrackYCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackYCut*)
   {
      ::NicaTrackYCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackYCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackYCut", ::NicaTrackYCut::Class_Version(), "NicaTrackYCut.h", 15,
                  typeid(::NicaTrackYCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackYCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackYCut) );
      instance.SetNew(&new_NicaTrackYCut);
      instance.SetNewArray(&newArray_NicaTrackYCut);
      instance.SetDelete(&delete_NicaTrackYCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackYCut);
      instance.SetDestructor(&destruct_NicaTrackYCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackYCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackYCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackYCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackPzCut(void *p = nullptr);
   static void *newArray_NicaTrackPzCut(Long_t size, void *p);
   static void delete_NicaTrackPzCut(void *p);
   static void deleteArray_NicaTrackPzCut(void *p);
   static void destruct_NicaTrackPzCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackPzCut*)
   {
      ::NicaTrackPzCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackPzCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackPzCut", ::NicaTrackPzCut::Class_Version(), "NicaTrackPzCut.h", 12,
                  typeid(::NicaTrackPzCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackPzCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackPzCut) );
      instance.SetNew(&new_NicaTrackPzCut);
      instance.SetNewArray(&newArray_NicaTrackPzCut);
      instance.SetDelete(&delete_NicaTrackPzCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackPzCut);
      instance.SetDestructor(&destruct_NicaTrackPzCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackPzCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackPzCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackPzCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackChargeCut(void *p = nullptr);
   static void *newArray_NicaTrackChargeCut(Long_t size, void *p);
   static void delete_NicaTrackChargeCut(void *p);
   static void deleteArray_NicaTrackChargeCut(void *p);
   static void destruct_NicaTrackChargeCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackChargeCut*)
   {
      ::NicaTrackChargeCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackChargeCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackChargeCut", ::NicaTrackChargeCut::Class_Version(), "NicaTrackChargeCut.h", 12,
                  typeid(::NicaTrackChargeCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackChargeCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackChargeCut) );
      instance.SetNew(&new_NicaTrackChargeCut);
      instance.SetNewArray(&newArray_NicaTrackChargeCut);
      instance.SetDelete(&delete_NicaTrackChargeCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackChargeCut);
      instance.SetDestructor(&destruct_NicaTrackChargeCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackChargeCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackChargeCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackChargeCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackVirtualCut(void *p = nullptr);
   static void *newArray_NicaTrackVirtualCut(Long_t size, void *p);
   static void delete_NicaTrackVirtualCut(void *p);
   static void deleteArray_NicaTrackVirtualCut(void *p);
   static void destruct_NicaTrackVirtualCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackVirtualCut*)
   {
      ::NicaTrackVirtualCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackVirtualCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackVirtualCut", ::NicaTrackVirtualCut::Class_Version(), "NicaTrackVirtualCut.h", 17,
                  typeid(::NicaTrackVirtualCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackVirtualCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackVirtualCut) );
      instance.SetNew(&new_NicaTrackVirtualCut);
      instance.SetNewArray(&newArray_NicaTrackVirtualCut);
      instance.SetDelete(&delete_NicaTrackVirtualCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackVirtualCut);
      instance.SetDestructor(&destruct_NicaTrackVirtualCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackVirtualCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackVirtualCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackVirtualCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackComplexCut(void *p = nullptr);
   static void *newArray_NicaTrackComplexCut(Long_t size, void *p);
   static void delete_NicaTrackComplexCut(void *p);
   static void deleteArray_NicaTrackComplexCut(void *p);
   static void destruct_NicaTrackComplexCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackComplexCut*)
   {
      ::NicaTrackComplexCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackComplexCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackComplexCut", ::NicaTrackComplexCut::Class_Version(), "NicaTrackComplexCut.h", 15,
                  typeid(::NicaTrackComplexCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackComplexCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackComplexCut) );
      instance.SetNew(&new_NicaTrackComplexCut);
      instance.SetNewArray(&newArray_NicaTrackComplexCut);
      instance.SetDelete(&delete_NicaTrackComplexCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackComplexCut);
      instance.SetDestructor(&destruct_NicaTrackComplexCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackComplexCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackComplexCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackComplexCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackRealCut(void *p = nullptr);
   static void *newArray_NicaTrackRealCut(Long_t size, void *p);
   static void delete_NicaTrackRealCut(void *p);
   static void deleteArray_NicaTrackRealCut(void *p);
   static void destruct_NicaTrackRealCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackRealCut*)
   {
      ::NicaTrackRealCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackRealCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackRealCut", ::NicaTrackRealCut::Class_Version(), "NicaTrackComplexCut.h", 55,
                  typeid(::NicaTrackRealCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackRealCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackRealCut) );
      instance.SetNew(&new_NicaTrackRealCut);
      instance.SetNewArray(&newArray_NicaTrackRealCut);
      instance.SetDelete(&delete_NicaTrackRealCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackRealCut);
      instance.SetDestructor(&destruct_NicaTrackRealCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackRealCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackRealCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackRealCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackImaginaryCut(void *p = nullptr);
   static void *newArray_NicaTrackImaginaryCut(Long_t size, void *p);
   static void delete_NicaTrackImaginaryCut(void *p);
   static void deleteArray_NicaTrackImaginaryCut(void *p);
   static void destruct_NicaTrackImaginaryCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackImaginaryCut*)
   {
      ::NicaTrackImaginaryCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackImaginaryCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackImaginaryCut", ::NicaTrackImaginaryCut::Class_Version(), "NicaTrackComplexCut.h", 72,
                  typeid(::NicaTrackImaginaryCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackImaginaryCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackImaginaryCut) );
      instance.SetNew(&new_NicaTrackImaginaryCut);
      instance.SetNewArray(&newArray_NicaTrackImaginaryCut);
      instance.SetDelete(&delete_NicaTrackImaginaryCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackImaginaryCut);
      instance.SetDestructor(&destruct_NicaTrackImaginaryCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackImaginaryCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackImaginaryCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackImaginaryCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackBoostedPCut(void *p = nullptr);
   static void *newArray_NicaTrackBoostedPCut(Long_t size, void *p);
   static void delete_NicaTrackBoostedPCut(void *p);
   static void deleteArray_NicaTrackBoostedPCut(void *p);
   static void destruct_NicaTrackBoostedPCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackBoostedPCut*)
   {
      ::NicaTrackBoostedPCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackBoostedPCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackBoostedPCut", ::NicaTrackBoostedPCut::Class_Version(), "NicaTrackBoostedMomentaCuts.h", 14,
                  typeid(::NicaTrackBoostedPCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackBoostedPCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackBoostedPCut) );
      instance.SetNew(&new_NicaTrackBoostedPCut);
      instance.SetNewArray(&newArray_NicaTrackBoostedPCut);
      instance.SetDelete(&delete_NicaTrackBoostedPCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackBoostedPCut);
      instance.SetDestructor(&destruct_NicaTrackBoostedPCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackBoostedPCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackBoostedPCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackBoostedPCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackBoostedKinematcisCut(void *p = nullptr);
   static void *newArray_NicaTrackBoostedKinematcisCut(Long_t size, void *p);
   static void delete_NicaTrackBoostedKinematcisCut(void *p);
   static void deleteArray_NicaTrackBoostedKinematcisCut(void *p);
   static void destruct_NicaTrackBoostedKinematcisCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackBoostedKinematcisCut*)
   {
      ::NicaTrackBoostedKinematcisCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackBoostedKinematcisCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackBoostedKinematcisCut", ::NicaTrackBoostedKinematcisCut::Class_Version(), "NicaTrackBoostedMomentaCuts.h", 26,
                  typeid(::NicaTrackBoostedKinematcisCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackBoostedKinematcisCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackBoostedKinematcisCut) );
      instance.SetNew(&new_NicaTrackBoostedKinematcisCut);
      instance.SetNewArray(&newArray_NicaTrackBoostedKinematcisCut);
      instance.SetDelete(&delete_NicaTrackBoostedKinematcisCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackBoostedKinematcisCut);
      instance.SetDestructor(&destruct_NicaTrackBoostedKinematcisCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackBoostedKinematcisCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackBoostedKinematcisCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackBoostedKinematcisCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackBoostedDeltaPCut(void *p = nullptr);
   static void *newArray_NicaTrackBoostedDeltaPCut(Long_t size, void *p);
   static void delete_NicaTrackBoostedDeltaPCut(void *p);
   static void deleteArray_NicaTrackBoostedDeltaPCut(void *p);
   static void destruct_NicaTrackBoostedDeltaPCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackBoostedDeltaPCut*)
   {
      ::NicaTrackBoostedDeltaPCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackBoostedDeltaPCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackBoostedDeltaPCut", ::NicaTrackBoostedDeltaPCut::Class_Version(), "NicaTrackBoostedMomentaCuts.h", 46,
                  typeid(::NicaTrackBoostedDeltaPCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackBoostedDeltaPCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackBoostedDeltaPCut) );
      instance.SetNew(&new_NicaTrackBoostedDeltaPCut);
      instance.SetNewArray(&newArray_NicaTrackBoostedDeltaPCut);
      instance.SetDelete(&delete_NicaTrackBoostedDeltaPCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackBoostedDeltaPCut);
      instance.SetDestructor(&destruct_NicaTrackBoostedDeltaPCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackBoostedDeltaPCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackBoostedDeltaPCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackBoostedDeltaPCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaTrackMCCut(void *p);
   static void deleteArray_NicaTrackMCCut(void *p);
   static void destruct_NicaTrackMCCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackMCCut*)
   {
      ::NicaTrackMCCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackMCCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackMCCut", ::NicaTrackMCCut::Class_Version(), "NicaTrackMCCut.h", 14,
                  typeid(::NicaTrackMCCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackMCCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackMCCut) );
      instance.SetDelete(&delete_NicaTrackMCCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackMCCut);
      instance.SetDestructor(&destruct_NicaTrackMCCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackMCCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackMCCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackMCCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackBasicMCCut(void *p = nullptr);
   static void *newArray_NicaTrackBasicMCCut(Long_t size, void *p);
   static void delete_NicaTrackBasicMCCut(void *p);
   static void deleteArray_NicaTrackBasicMCCut(void *p);
   static void destruct_NicaTrackBasicMCCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackBasicMCCut*)
   {
      ::NicaTrackBasicMCCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackBasicMCCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackBasicMCCut", ::NicaTrackBasicMCCut::Class_Version(), "NicaTrackBasicMCCut.h", 16,
                  typeid(::NicaTrackBasicMCCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackBasicMCCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackBasicMCCut) );
      instance.SetNew(&new_NicaTrackBasicMCCut);
      instance.SetNewArray(&newArray_NicaTrackBasicMCCut);
      instance.SetDelete(&delete_NicaTrackBasicMCCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackBasicMCCut);
      instance.SetDestructor(&destruct_NicaTrackBasicMCCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackBasicMCCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackBasicMCCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackBasicMCCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackFreezoutCut(void *p = nullptr);
   static void *newArray_NicaTrackFreezoutCut(Long_t size, void *p);
   static void delete_NicaTrackFreezoutCut(void *p);
   static void deleteArray_NicaTrackFreezoutCut(void *p);
   static void destruct_NicaTrackFreezoutCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackFreezoutCut*)
   {
      ::NicaTrackFreezoutCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackFreezoutCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackFreezoutCut", ::NicaTrackFreezoutCut::Class_Version(), "NicaTrackFreezoutCut.h", 16,
                  typeid(::NicaTrackFreezoutCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackFreezoutCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackFreezoutCut) );
      instance.SetNew(&new_NicaTrackFreezoutCut);
      instance.SetNewArray(&newArray_NicaTrackFreezoutCut);
      instance.SetDelete(&delete_NicaTrackFreezoutCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackFreezoutCut);
      instance.SetDestructor(&destruct_NicaTrackFreezoutCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackFreezoutCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackFreezoutCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackFreezoutCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackTFreezCut(void *p = nullptr);
   static void *newArray_NicaTrackTFreezCut(Long_t size, void *p);
   static void delete_NicaTrackTFreezCut(void *p);
   static void deleteArray_NicaTrackTFreezCut(void *p);
   static void destruct_NicaTrackTFreezCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackTFreezCut*)
   {
      ::NicaTrackTFreezCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackTFreezCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackTFreezCut", ::NicaTrackTFreezCut::Class_Version(), "NicaTrackFreezoutCut.h", 27,
                  typeid(::NicaTrackTFreezCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackTFreezCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackTFreezCut) );
      instance.SetNew(&new_NicaTrackTFreezCut);
      instance.SetNewArray(&newArray_NicaTrackTFreezCut);
      instance.SetDelete(&delete_NicaTrackTFreezCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackTFreezCut);
      instance.SetDestructor(&destruct_NicaTrackTFreezCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackTFreezCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackTFreezCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackTFreezCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackTauCut(void *p = nullptr);
   static void *newArray_NicaTrackTauCut(Long_t size, void *p);
   static void delete_NicaTrackTauCut(void *p);
   static void deleteArray_NicaTrackTauCut(void *p);
   static void destruct_NicaTrackTauCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackTauCut*)
   {
      ::NicaTrackTauCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackTauCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackTauCut", ::NicaTrackTauCut::Class_Version(), "NicaTrackFreezoutCut.h", 38,
                  typeid(::NicaTrackTauCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackTauCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackTauCut) );
      instance.SetNew(&new_NicaTrackTauCut);
      instance.SetNewArray(&newArray_NicaTrackTauCut);
      instance.SetDelete(&delete_NicaTrackTauCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackTauCut);
      instance.SetDestructor(&destruct_NicaTrackTauCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackTauCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackTauCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackTauCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackPdgCut(void *p = nullptr);
   static void *newArray_NicaTrackPdgCut(Long_t size, void *p);
   static void delete_NicaTrackPdgCut(void *p);
   static void deleteArray_NicaTrackPdgCut(void *p);
   static void destruct_NicaTrackPdgCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackPdgCut*)
   {
      ::NicaTrackPdgCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackPdgCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackPdgCut", ::NicaTrackPdgCut::Class_Version(), "NicaTrackPdgCut.h", 17,
                  typeid(::NicaTrackPdgCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackPdgCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackPdgCut) );
      instance.SetNew(&new_NicaTrackPdgCut);
      instance.SetNewArray(&newArray_NicaTrackPdgCut);
      instance.SetDelete(&delete_NicaTrackPdgCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackPdgCut);
      instance.SetDestructor(&destruct_NicaTrackPdgCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackPdgCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackPdgCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackPdgCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackPdgAbsCut(void *p = nullptr);
   static void *newArray_NicaTrackPdgAbsCut(Long_t size, void *p);
   static void delete_NicaTrackPdgAbsCut(void *p);
   static void deleteArray_NicaTrackPdgAbsCut(void *p);
   static void destruct_NicaTrackPdgAbsCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackPdgAbsCut*)
   {
      ::NicaTrackPdgAbsCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackPdgAbsCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackPdgAbsCut", ::NicaTrackPdgAbsCut::Class_Version(), "NicaTrackPdgCut.h", 26,
                  typeid(::NicaTrackPdgAbsCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackPdgAbsCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackPdgAbsCut) );
      instance.SetNew(&new_NicaTrackPdgAbsCut);
      instance.SetNewArray(&newArray_NicaTrackPdgAbsCut);
      instance.SetDelete(&delete_NicaTrackPdgAbsCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackPdgAbsCut);
      instance.SetDestructor(&destruct_NicaTrackPdgAbsCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackPdgAbsCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackPdgAbsCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackPdgAbsCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackStartPositionCut(void *p = nullptr);
   static void *newArray_NicaTrackStartPositionCut(Long_t size, void *p);
   static void delete_NicaTrackStartPositionCut(void *p);
   static void deleteArray_NicaTrackStartPositionCut(void *p);
   static void destruct_NicaTrackStartPositionCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackStartPositionCut*)
   {
      ::NicaTrackStartPositionCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackStartPositionCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackStartPositionCut", ::NicaTrackStartPositionCut::Class_Version(), "NicaTrackStartPositionCut.h", 14,
                  typeid(::NicaTrackStartPositionCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackStartPositionCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackStartPositionCut) );
      instance.SetNew(&new_NicaTrackStartPositionCut);
      instance.SetNewArray(&newArray_NicaTrackStartPositionCut);
      instance.SetDelete(&delete_NicaTrackStartPositionCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackStartPositionCut);
      instance.SetDestructor(&destruct_NicaTrackStartPositionCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackStartPositionCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackStartPositionCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackStartPositionCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackPdgBinCut(void *p = nullptr);
   static void *newArray_NicaTrackPdgBinCut(Long_t size, void *p);
   static void delete_NicaTrackPdgBinCut(void *p);
   static void deleteArray_NicaTrackPdgBinCut(void *p);
   static void destruct_NicaTrackPdgBinCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackPdgBinCut*)
   {
      ::NicaTrackPdgBinCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackPdgBinCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackPdgBinCut", ::NicaTrackPdgBinCut::Class_Version(), "NicaTrackPdgBinCut.h", 14,
                  typeid(::NicaTrackPdgBinCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackPdgBinCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackPdgBinCut) );
      instance.SetNew(&new_NicaTrackPdgBinCut);
      instance.SetNewArray(&newArray_NicaTrackPdgBinCut);
      instance.SetDelete(&delete_NicaTrackPdgBinCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackPdgBinCut);
      instance.SetDestructor(&destruct_NicaTrackPdgBinCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackPdgBinCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackPdgBinCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackPdgBinCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaTrackDeltaMomentumCut(void *p);
   static void deleteArray_NicaTrackDeltaMomentumCut(void *p);
   static void destruct_NicaTrackDeltaMomentumCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackDeltaMomentumCut*)
   {
      ::NicaTrackDeltaMomentumCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackDeltaMomentumCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackDeltaMomentumCut", ::NicaTrackDeltaMomentumCut::Class_Version(), "NicaTrackDeltaMomentumCut.h", 16,
                  typeid(::NicaTrackDeltaMomentumCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackDeltaMomentumCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackDeltaMomentumCut) );
      instance.SetDelete(&delete_NicaTrackDeltaMomentumCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackDeltaMomentumCut);
      instance.SetDestructor(&destruct_NicaTrackDeltaMomentumCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackDeltaMomentumCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackDeltaMomentumCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackDeltaMomentumCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackDeltaMomCut(void *p = nullptr);
   static void *newArray_NicaTrackDeltaMomCut(Long_t size, void *p);
   static void delete_NicaTrackDeltaMomCut(void *p);
   static void deleteArray_NicaTrackDeltaMomCut(void *p);
   static void destruct_NicaTrackDeltaMomCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackDeltaMomCut*)
   {
      ::NicaTrackDeltaMomCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackDeltaMomCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackDeltaMomCut", ::NicaTrackDeltaMomCut::Class_Version(), "NicaTrackDeltaMomentumCut.h", 35,
                  typeid(::NicaTrackDeltaMomCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackDeltaMomCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackDeltaMomCut) );
      instance.SetNew(&new_NicaTrackDeltaMomCut);
      instance.SetNewArray(&newArray_NicaTrackDeltaMomCut);
      instance.SetDelete(&delete_NicaTrackDeltaMomCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackDeltaMomCut);
      instance.SetDestructor(&destruct_NicaTrackDeltaMomCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackDeltaMomCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackDeltaMomCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackDeltaMomCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackDeltaVectMomCut(void *p = nullptr);
   static void *newArray_NicaTrackDeltaVectMomCut(Long_t size, void *p);
   static void delete_NicaTrackDeltaVectMomCut(void *p);
   static void deleteArray_NicaTrackDeltaVectMomCut(void *p);
   static void destruct_NicaTrackDeltaVectMomCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackDeltaVectMomCut*)
   {
      ::NicaTrackDeltaVectMomCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackDeltaVectMomCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackDeltaVectMomCut", ::NicaTrackDeltaVectMomCut::Class_Version(), "NicaTrackDeltaMomentumCut.h", 55,
                  typeid(::NicaTrackDeltaVectMomCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackDeltaVectMomCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackDeltaVectMomCut) );
      instance.SetNew(&new_NicaTrackDeltaVectMomCut);
      instance.SetNewArray(&newArray_NicaTrackDeltaVectMomCut);
      instance.SetDelete(&delete_NicaTrackDeltaVectMomCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackDeltaVectMomCut);
      instance.SetDestructor(&destruct_NicaTrackDeltaVectMomCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackDeltaVectMomCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackDeltaVectMomCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackDeltaVectMomCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackDeltaPtCut(void *p = nullptr);
   static void *newArray_NicaTrackDeltaPtCut(Long_t size, void *p);
   static void delete_NicaTrackDeltaPtCut(void *p);
   static void deleteArray_NicaTrackDeltaPtCut(void *p);
   static void destruct_NicaTrackDeltaPtCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackDeltaPtCut*)
   {
      ::NicaTrackDeltaPtCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackDeltaPtCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackDeltaPtCut", ::NicaTrackDeltaPtCut::Class_Version(), "NicaTrackDeltaMomCut.h", 14,
                  typeid(::NicaTrackDeltaPtCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackDeltaPtCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackDeltaPtCut) );
      instance.SetNew(&new_NicaTrackDeltaPtCut);
      instance.SetNewArray(&newArray_NicaTrackDeltaPtCut);
      instance.SetDelete(&delete_NicaTrackDeltaPtCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackDeltaPtCut);
      instance.SetDestructor(&destruct_NicaTrackDeltaPtCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackDeltaPtCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackDeltaPtCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackDeltaPtCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackDeltaPCut(void *p = nullptr);
   static void *newArray_NicaTrackDeltaPCut(Long_t size, void *p);
   static void delete_NicaTrackDeltaPCut(void *p);
   static void deleteArray_NicaTrackDeltaPCut(void *p);
   static void destruct_NicaTrackDeltaPCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackDeltaPCut*)
   {
      ::NicaTrackDeltaPCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackDeltaPCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackDeltaPCut", ::NicaTrackDeltaPCut::Class_Version(), "NicaTrackDeltaMomCut.h", 22,
                  typeid(::NicaTrackDeltaPCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackDeltaPCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackDeltaPCut) );
      instance.SetNew(&new_NicaTrackDeltaPCut);
      instance.SetNewArray(&newArray_NicaTrackDeltaPCut);
      instance.SetDelete(&delete_NicaTrackDeltaPCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackDeltaPCut);
      instance.SetDestructor(&destruct_NicaTrackDeltaPCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackDeltaPCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackDeltaPCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackDeltaPCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackDeltaPzCut(void *p = nullptr);
   static void *newArray_NicaTrackDeltaPzCut(Long_t size, void *p);
   static void delete_NicaTrackDeltaPzCut(void *p);
   static void deleteArray_NicaTrackDeltaPzCut(void *p);
   static void destruct_NicaTrackDeltaPzCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackDeltaPzCut*)
   {
      ::NicaTrackDeltaPzCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackDeltaPzCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackDeltaPzCut", ::NicaTrackDeltaPzCut::Class_Version(), "NicaTrackDeltaMomCut.h", 30,
                  typeid(::NicaTrackDeltaPzCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackDeltaPzCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackDeltaPzCut) );
      instance.SetNew(&new_NicaTrackDeltaPzCut);
      instance.SetNewArray(&newArray_NicaTrackDeltaPzCut);
      instance.SetDelete(&delete_NicaTrackDeltaPzCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackDeltaPzCut);
      instance.SetDestructor(&destruct_NicaTrackDeltaPzCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackDeltaPzCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackDeltaPzCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackDeltaPzCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackDeltaPhiCut(void *p = nullptr);
   static void *newArray_NicaTrackDeltaPhiCut(Long_t size, void *p);
   static void delete_NicaTrackDeltaPhiCut(void *p);
   static void deleteArray_NicaTrackDeltaPhiCut(void *p);
   static void destruct_NicaTrackDeltaPhiCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackDeltaPhiCut*)
   {
      ::NicaTrackDeltaPhiCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackDeltaPhiCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackDeltaPhiCut", ::NicaTrackDeltaPhiCut::Class_Version(), "NicaTrackDeltaMomCut.h", 38,
                  typeid(::NicaTrackDeltaPhiCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackDeltaPhiCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackDeltaPhiCut) );
      instance.SetNew(&new_NicaTrackDeltaPhiCut);
      instance.SetNewArray(&newArray_NicaTrackDeltaPhiCut);
      instance.SetDelete(&delete_NicaTrackDeltaPhiCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackDeltaPhiCut);
      instance.SetDestructor(&destruct_NicaTrackDeltaPhiCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackDeltaPhiCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackDeltaPhiCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackDeltaPhiCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackDeltaThetaCut(void *p = nullptr);
   static void *newArray_NicaTrackDeltaThetaCut(Long_t size, void *p);
   static void delete_NicaTrackDeltaThetaCut(void *p);
   static void deleteArray_NicaTrackDeltaThetaCut(void *p);
   static void destruct_NicaTrackDeltaThetaCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackDeltaThetaCut*)
   {
      ::NicaTrackDeltaThetaCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackDeltaThetaCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackDeltaThetaCut", ::NicaTrackDeltaThetaCut::Class_Version(), "NicaTrackDeltaMomCut.h", 45,
                  typeid(::NicaTrackDeltaThetaCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackDeltaThetaCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackDeltaThetaCut) );
      instance.SetNew(&new_NicaTrackDeltaThetaCut);
      instance.SetNewArray(&newArray_NicaTrackDeltaThetaCut);
      instance.SetDelete(&delete_NicaTrackDeltaThetaCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackDeltaThetaCut);
      instance.SetDestructor(&destruct_NicaTrackDeltaThetaCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackDeltaThetaCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackDeltaThetaCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackDeltaThetaCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackDeltaVectPtCut(void *p = nullptr);
   static void *newArray_NicaTrackDeltaVectPtCut(Long_t size, void *p);
   static void delete_NicaTrackDeltaVectPtCut(void *p);
   static void deleteArray_NicaTrackDeltaVectPtCut(void *p);
   static void destruct_NicaTrackDeltaVectPtCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackDeltaVectPtCut*)
   {
      ::NicaTrackDeltaVectPtCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackDeltaVectPtCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackDeltaVectPtCut", ::NicaTrackDeltaVectPtCut::Class_Version(), "NicaTrackDeltaMomCut.h", 53,
                  typeid(::NicaTrackDeltaVectPtCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackDeltaVectPtCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackDeltaVectPtCut) );
      instance.SetNew(&new_NicaTrackDeltaVectPtCut);
      instance.SetNewArray(&newArray_NicaTrackDeltaVectPtCut);
      instance.SetDelete(&delete_NicaTrackDeltaVectPtCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackDeltaVectPtCut);
      instance.SetDestructor(&destruct_NicaTrackDeltaVectPtCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackDeltaVectPtCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackDeltaVectPtCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPtCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackDeltaVectPCut(void *p = nullptr);
   static void *newArray_NicaTrackDeltaVectPCut(Long_t size, void *p);
   static void delete_NicaTrackDeltaVectPCut(void *p);
   static void deleteArray_NicaTrackDeltaVectPCut(void *p);
   static void destruct_NicaTrackDeltaVectPCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackDeltaVectPCut*)
   {
      ::NicaTrackDeltaVectPCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackDeltaVectPCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackDeltaVectPCut", ::NicaTrackDeltaVectPCut::Class_Version(), "NicaTrackDeltaMomCut.h", 61,
                  typeid(::NicaTrackDeltaVectPCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackDeltaVectPCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackDeltaVectPCut) );
      instance.SetNew(&new_NicaTrackDeltaVectPCut);
      instance.SetNewArray(&newArray_NicaTrackDeltaVectPCut);
      instance.SetDelete(&delete_NicaTrackDeltaVectPCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackDeltaVectPCut);
      instance.SetDestructor(&destruct_NicaTrackDeltaVectPCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackDeltaVectPCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackDeltaVectPCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackDeltaVectPhiCut(void *p = nullptr);
   static void *newArray_NicaTrackDeltaVectPhiCut(Long_t size, void *p);
   static void delete_NicaTrackDeltaVectPhiCut(void *p);
   static void deleteArray_NicaTrackDeltaVectPhiCut(void *p);
   static void destruct_NicaTrackDeltaVectPhiCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackDeltaVectPhiCut*)
   {
      ::NicaTrackDeltaVectPhiCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackDeltaVectPhiCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackDeltaVectPhiCut", ::NicaTrackDeltaVectPhiCut::Class_Version(), "NicaTrackDeltaMomCut.h", 69,
                  typeid(::NicaTrackDeltaVectPhiCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackDeltaVectPhiCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackDeltaVectPhiCut) );
      instance.SetNew(&new_NicaTrackDeltaVectPhiCut);
      instance.SetNewArray(&newArray_NicaTrackDeltaVectPhiCut);
      instance.SetDelete(&delete_NicaTrackDeltaVectPhiCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackDeltaVectPhiCut);
      instance.SetDestructor(&destruct_NicaTrackDeltaVectPhiCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackDeltaVectPhiCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackDeltaVectPhiCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPhiCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackDeltaVectThetaCut(void *p = nullptr);
   static void *newArray_NicaTrackDeltaVectThetaCut(Long_t size, void *p);
   static void delete_NicaTrackDeltaVectThetaCut(void *p);
   static void deleteArray_NicaTrackDeltaVectThetaCut(void *p);
   static void destruct_NicaTrackDeltaVectThetaCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackDeltaVectThetaCut*)
   {
      ::NicaTrackDeltaVectThetaCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackDeltaVectThetaCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackDeltaVectThetaCut", ::NicaTrackDeltaVectThetaCut::Class_Version(), "NicaTrackDeltaMomCut.h", 77,
                  typeid(::NicaTrackDeltaVectThetaCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackDeltaVectThetaCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackDeltaVectThetaCut) );
      instance.SetNew(&new_NicaTrackDeltaVectThetaCut);
      instance.SetNewArray(&newArray_NicaTrackDeltaVectThetaCut);
      instance.SetDelete(&delete_NicaTrackDeltaVectThetaCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackDeltaVectThetaCut);
      instance.SetDestructor(&destruct_NicaTrackDeltaVectThetaCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackDeltaVectThetaCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackDeltaVectThetaCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackDeltaVectThetaCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackOnlyPrimariesCut(void *p = nullptr);
   static void *newArray_NicaTrackOnlyPrimariesCut(Long_t size, void *p);
   static void delete_NicaTrackOnlyPrimariesCut(void *p);
   static void deleteArray_NicaTrackOnlyPrimariesCut(void *p);
   static void destruct_NicaTrackOnlyPrimariesCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackOnlyPrimariesCut*)
   {
      ::NicaTrackOnlyPrimariesCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackOnlyPrimariesCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackOnlyPrimariesCut", ::NicaTrackOnlyPrimariesCut::Class_Version(), "NicaTrackOnlyPrimariesCut.h", 15,
                  typeid(::NicaTrackOnlyPrimariesCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackOnlyPrimariesCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackOnlyPrimariesCut) );
      instance.SetNew(&new_NicaTrackOnlyPrimariesCut);
      instance.SetNewArray(&newArray_NicaTrackOnlyPrimariesCut);
      instance.SetDelete(&delete_NicaTrackOnlyPrimariesCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackOnlyPrimariesCut);
      instance.SetDestructor(&destruct_NicaTrackOnlyPrimariesCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackOnlyPrimariesCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackOnlyPrimariesCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackOnlyPrimariesCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackStatusCut(void *p = nullptr);
   static void *newArray_NicaTrackStatusCut(Long_t size, void *p);
   static void delete_NicaTrackStatusCut(void *p);
   static void deleteArray_NicaTrackStatusCut(void *p);
   static void destruct_NicaTrackStatusCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackStatusCut*)
   {
      ::NicaTrackStatusCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackStatusCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackStatusCut", ::NicaTrackStatusCut::Class_Version(), "NicaTrackStatusCut.h", 16,
                  typeid(::NicaTrackStatusCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackStatusCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackStatusCut) );
      instance.SetNew(&new_NicaTrackStatusCut);
      instance.SetNewArray(&newArray_NicaTrackStatusCut);
      instance.SetDelete(&delete_NicaTrackStatusCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackStatusCut);
      instance.SetDestructor(&destruct_NicaTrackStatusCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackStatusCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackStatusCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackStatusCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackPrimaryFlagCut(void *p = nullptr);
   static void *newArray_NicaTrackPrimaryFlagCut(Long_t size, void *p);
   static void delete_NicaTrackPrimaryFlagCut(void *p);
   static void deleteArray_NicaTrackPrimaryFlagCut(void *p);
   static void destruct_NicaTrackPrimaryFlagCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackPrimaryFlagCut*)
   {
      ::NicaTrackPrimaryFlagCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackPrimaryFlagCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackPrimaryFlagCut", ::NicaTrackPrimaryFlagCut::Class_Version(), "NicaTrackFlagCuts.h", 16,
                  typeid(::NicaTrackPrimaryFlagCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackPrimaryFlagCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackPrimaryFlagCut) );
      instance.SetNew(&new_NicaTrackPrimaryFlagCut);
      instance.SetNewArray(&newArray_NicaTrackPrimaryFlagCut);
      instance.SetDelete(&delete_NicaTrackPrimaryFlagCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackPrimaryFlagCut);
      instance.SetDestructor(&destruct_NicaTrackPrimaryFlagCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackPrimaryFlagCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackPrimaryFlagCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackPrimaryFlagCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackV0FlagCut(void *p = nullptr);
   static void *newArray_NicaTrackV0FlagCut(Long_t size, void *p);
   static void delete_NicaTrackV0FlagCut(void *p);
   static void deleteArray_NicaTrackV0FlagCut(void *p);
   static void destruct_NicaTrackV0FlagCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackV0FlagCut*)
   {
      ::NicaTrackV0FlagCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackV0FlagCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackV0FlagCut", ::NicaTrackV0FlagCut::Class_Version(), "NicaTrackFlagCuts.h", 26,
                  typeid(::NicaTrackV0FlagCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackV0FlagCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackV0FlagCut) );
      instance.SetNew(&new_NicaTrackV0FlagCut);
      instance.SetNewArray(&newArray_NicaTrackV0FlagCut);
      instance.SetDelete(&delete_NicaTrackV0FlagCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackV0FlagCut);
      instance.SetDestructor(&destruct_NicaTrackV0FlagCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackV0FlagCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackV0FlagCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackV0FlagCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackGoodSecondaryFlagCut(void *p = nullptr);
   static void *newArray_NicaTrackGoodSecondaryFlagCut(Long_t size, void *p);
   static void delete_NicaTrackGoodSecondaryFlagCut(void *p);
   static void deleteArray_NicaTrackGoodSecondaryFlagCut(void *p);
   static void destruct_NicaTrackGoodSecondaryFlagCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackGoodSecondaryFlagCut*)
   {
      ::NicaTrackGoodSecondaryFlagCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackGoodSecondaryFlagCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackGoodSecondaryFlagCut", ::NicaTrackGoodSecondaryFlagCut::Class_Version(), "NicaTrackFlagCuts.h", 38,
                  typeid(::NicaTrackGoodSecondaryFlagCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackGoodSecondaryFlagCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackGoodSecondaryFlagCut) );
      instance.SetNew(&new_NicaTrackGoodSecondaryFlagCut);
      instance.SetNewArray(&newArray_NicaTrackGoodSecondaryFlagCut);
      instance.SetDelete(&delete_NicaTrackGoodSecondaryFlagCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackGoodSecondaryFlagCut);
      instance.SetDestructor(&destruct_NicaTrackGoodSecondaryFlagCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackGoodSecondaryFlagCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackGoodSecondaryFlagCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackGoodSecondaryFlagCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackGlobalFlagCut(void *p = nullptr);
   static void *newArray_NicaTrackGlobalFlagCut(Long_t size, void *p);
   static void delete_NicaTrackGlobalFlagCut(void *p);
   static void deleteArray_NicaTrackGlobalFlagCut(void *p);
   static void destruct_NicaTrackGlobalFlagCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackGlobalFlagCut*)
   {
      ::NicaTrackGlobalFlagCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackGlobalFlagCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackGlobalFlagCut", ::NicaTrackGlobalFlagCut::Class_Version(), "NicaTrackFlagCuts.h", 48,
                  typeid(::NicaTrackGlobalFlagCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackGlobalFlagCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackGlobalFlagCut) );
      instance.SetNew(&new_NicaTrackGlobalFlagCut);
      instance.SetNewArray(&newArray_NicaTrackGlobalFlagCut);
      instance.SetDelete(&delete_NicaTrackGlobalFlagCut);
      instance.SetDeleteArray(&deleteArray_NicaTrackGlobalFlagCut);
      instance.SetDestructor(&destruct_NicaTrackGlobalFlagCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackGlobalFlagCut*)
   {
      return GenerateInitInstanceLocal((::NicaTrackGlobalFlagCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackGlobalFlagCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackEtaCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackEtaCut(Long_t size, void *p);
   static void delete_NicaTwoTrackEtaCut(void *p);
   static void deleteArray_NicaTwoTrackEtaCut(void *p);
   static void destruct_NicaTwoTrackEtaCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackEtaCut*)
   {
      ::NicaTwoTrackEtaCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackEtaCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackEtaCut", ::NicaTwoTrackEtaCut::Class_Version(), "NicaTwoTrackEtaCut.h", 17,
                  typeid(::NicaTwoTrackEtaCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackEtaCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackEtaCut) );
      instance.SetNew(&new_NicaTwoTrackEtaCut);
      instance.SetNewArray(&newArray_NicaTwoTrackEtaCut);
      instance.SetDelete(&delete_NicaTwoTrackEtaCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackEtaCut);
      instance.SetDestructor(&destruct_NicaTwoTrackEtaCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackEtaCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackEtaCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackEtaCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackKtCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackKtCut(Long_t size, void *p);
   static void delete_NicaTwoTrackKtCut(void *p);
   static void deleteArray_NicaTwoTrackKtCut(void *p);
   static void destruct_NicaTwoTrackKtCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackKtCut*)
   {
      ::NicaTwoTrackKtCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackKtCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackKtCut", ::NicaTwoTrackKtCut::Class_Version(), "NicaTwoTrackKtCut.h", 17,
                  typeid(::NicaTwoTrackKtCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackKtCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackKtCut) );
      instance.SetNew(&new_NicaTwoTrackKtCut);
      instance.SetNewArray(&newArray_NicaTwoTrackKtCut);
      instance.SetDelete(&delete_NicaTwoTrackKtCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackKtCut);
      instance.SetDestructor(&destruct_NicaTwoTrackKtCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackKtCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackKtCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackKtCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackMinvCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackMinvCut(Long_t size, void *p);
   static void delete_NicaTwoTrackMinvCut(void *p);
   static void deleteArray_NicaTwoTrackMinvCut(void *p);
   static void destruct_NicaTwoTrackMinvCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackMinvCut*)
   {
      ::NicaTwoTrackMinvCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackMinvCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackMinvCut", ::NicaTwoTrackMinvCut::Class_Version(), "NicaTwoTrackMinvCut.h", 17,
                  typeid(::NicaTwoTrackMinvCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackMinvCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackMinvCut) );
      instance.SetNew(&new_NicaTwoTrackMinvCut);
      instance.SetNewArray(&newArray_NicaTwoTrackMinvCut);
      instance.SetDelete(&delete_NicaTwoTrackMinvCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackMinvCut);
      instance.SetDestructor(&destruct_NicaTwoTrackMinvCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackMinvCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackMinvCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackMinvCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackPhiCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackPhiCut(Long_t size, void *p);
   static void delete_NicaTwoTrackPhiCut(void *p);
   static void deleteArray_NicaTwoTrackPhiCut(void *p);
   static void destruct_NicaTwoTrackPhiCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackPhiCut*)
   {
      ::NicaTwoTrackPhiCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackPhiCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackPhiCut", ::NicaTwoTrackPhiCut::Class_Version(), "NicaTwoTrackPhiCut.h", 15,
                  typeid(::NicaTwoTrackPhiCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackPhiCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackPhiCut) );
      instance.SetNew(&new_NicaTwoTrackPhiCut);
      instance.SetNewArray(&newArray_NicaTwoTrackPhiCut);
      instance.SetDelete(&delete_NicaTwoTrackPhiCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackPhiCut);
      instance.SetDestructor(&destruct_NicaTwoTrackPhiCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackPhiCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackPhiCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackPhiCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackPtOrderCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackPtOrderCut(Long_t size, void *p);
   static void delete_NicaTwoTrackPtOrderCut(void *p);
   static void deleteArray_NicaTwoTrackPtOrderCut(void *p);
   static void destruct_NicaTwoTrackPtOrderCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackPtOrderCut*)
   {
      ::NicaTwoTrackPtOrderCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackPtOrderCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackPtOrderCut", ::NicaTwoTrackPtOrderCut::Class_Version(), "NicaTwoTrackPtOrderCut.h", 17,
                  typeid(::NicaTwoTrackPtOrderCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackPtOrderCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackPtOrderCut) );
      instance.SetNew(&new_NicaTwoTrackPtOrderCut);
      instance.SetNewArray(&newArray_NicaTwoTrackPtOrderCut);
      instance.SetDelete(&delete_NicaTwoTrackPtOrderCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackPtOrderCut);
      instance.SetDestructor(&destruct_NicaTwoTrackPtOrderCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackPtOrderCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackPtOrderCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackPtOrderCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackDphiDetaCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackDphiDetaCut(Long_t size, void *p);
   static void delete_NicaTwoTrackDphiDetaCut(void *p);
   static void deleteArray_NicaTwoTrackDphiDetaCut(void *p);
   static void destruct_NicaTwoTrackDphiDetaCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackDphiDetaCut*)
   {
      ::NicaTwoTrackDphiDetaCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackDphiDetaCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackDphiDetaCut", ::NicaTwoTrackDphiDetaCut::Class_Version(), "NicaTwoTrackDphiDetaCut.h", 18,
                  typeid(::NicaTwoTrackDphiDetaCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackDphiDetaCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackDphiDetaCut) );
      instance.SetNew(&new_NicaTwoTrackDphiDetaCut);
      instance.SetNewArray(&newArray_NicaTwoTrackDphiDetaCut);
      instance.SetDelete(&delete_NicaTwoTrackDphiDetaCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackDphiDetaCut);
      instance.SetDestructor(&destruct_NicaTwoTrackDphiDetaCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackDphiDetaCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackDphiDetaCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackDphiDetaCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackDphistarDetaCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackDphistarDetaCut(Long_t size, void *p);
   static void delete_NicaTwoTrackDphistarDetaCut(void *p);
   static void deleteArray_NicaTwoTrackDphistarDetaCut(void *p);
   static void destruct_NicaTwoTrackDphistarDetaCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackDphistarDetaCut*)
   {
      ::NicaTwoTrackDphistarDetaCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackDphistarDetaCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackDphistarDetaCut", ::NicaTwoTrackDphistarDetaCut::Class_Version(), "NicaTwoTrackDphiDetaCut.h", 28,
                  typeid(::NicaTwoTrackDphistarDetaCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackDphistarDetaCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackDphistarDetaCut) );
      instance.SetNew(&new_NicaTwoTrackDphistarDetaCut);
      instance.SetNewArray(&newArray_NicaTwoTrackDphistarDetaCut);
      instance.SetDelete(&delete_NicaTwoTrackDphistarDetaCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackDphistarDetaCut);
      instance.SetDestructor(&destruct_NicaTwoTrackDphistarDetaCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackDphistarDetaCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackDphistarDetaCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackDphistarDetaCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackPtSumCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackPtSumCut(Long_t size, void *p);
   static void delete_NicaTwoTrackPtSumCut(void *p);
   static void deleteArray_NicaTwoTrackPtSumCut(void *p);
   static void destruct_NicaTwoTrackPtSumCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackPtSumCut*)
   {
      ::NicaTwoTrackPtSumCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackPtSumCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackPtSumCut", ::NicaTwoTrackPtSumCut::Class_Version(), "NicaTwoTrackPtSumCut.h", 15,
                  typeid(::NicaTwoTrackPtSumCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackPtSumCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackPtSumCut) );
      instance.SetNew(&new_NicaTwoTrackPtSumCut);
      instance.SetNewArray(&newArray_NicaTwoTrackPtSumCut);
      instance.SetDelete(&delete_NicaTwoTrackPtSumCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackPtSumCut);
      instance.SetDestructor(&destruct_NicaTwoTrackPtSumCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackPtSumCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackPtSumCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackPtSumCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackRapidityCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackRapidityCut(Long_t size, void *p);
   static void delete_NicaTwoTrackRapidityCut(void *p);
   static void deleteArray_NicaTwoTrackRapidityCut(void *p);
   static void destruct_NicaTwoTrackRapidityCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackRapidityCut*)
   {
      ::NicaTwoTrackRapidityCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackRapidityCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackRapidityCut", ::NicaTwoTrackRapidityCut::Class_Version(), "NicaTwoTrackRapidityCut.h", 17,
                  typeid(::NicaTwoTrackRapidityCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackRapidityCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackRapidityCut) );
      instance.SetNew(&new_NicaTwoTrackRapidityCut);
      instance.SetNewArray(&newArray_NicaTwoTrackRapidityCut);
      instance.SetDelete(&delete_NicaTwoTrackRapidityCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackRapidityCut);
      instance.SetDestructor(&destruct_NicaTwoTrackRapidityCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackRapidityCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackRapidityCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackRapidityCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackDeltaEtaCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackDeltaEtaCut(Long_t size, void *p);
   static void delete_NicaTwoTrackDeltaEtaCut(void *p);
   static void deleteArray_NicaTwoTrackDeltaEtaCut(void *p);
   static void destruct_NicaTwoTrackDeltaEtaCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackDeltaEtaCut*)
   {
      ::NicaTwoTrackDeltaEtaCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackDeltaEtaCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackDeltaEtaCut", ::NicaTwoTrackDeltaEtaCut::Class_Version(), "NicaTwoTrackDeltaEtaCut.h", 15,
                  typeid(::NicaTwoTrackDeltaEtaCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackDeltaEtaCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackDeltaEtaCut) );
      instance.SetNew(&new_NicaTwoTrackDeltaEtaCut);
      instance.SetNewArray(&newArray_NicaTwoTrackDeltaEtaCut);
      instance.SetDelete(&delete_NicaTwoTrackDeltaEtaCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackDeltaEtaCut);
      instance.SetDestructor(&destruct_NicaTwoTrackDeltaEtaCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackDeltaEtaCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackDeltaEtaCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackDeltaEtaCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaPairDeltaQinvCut(void *p = nullptr);
   static void *newArray_NicaPairDeltaQinvCut(Long_t size, void *p);
   static void delete_NicaPairDeltaQinvCut(void *p);
   static void deleteArray_NicaPairDeltaQinvCut(void *p);
   static void destruct_NicaPairDeltaQinvCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaPairDeltaQinvCut*)
   {
      ::NicaPairDeltaQinvCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaPairDeltaQinvCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaPairDeltaQinvCut", ::NicaPairDeltaQinvCut::Class_Version(), "NicaPairDeltaQinvCut.h", 16,
                  typeid(::NicaPairDeltaQinvCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaPairDeltaQinvCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaPairDeltaQinvCut) );
      instance.SetNew(&new_NicaPairDeltaQinvCut);
      instance.SetNewArray(&newArray_NicaPairDeltaQinvCut);
      instance.SetDelete(&delete_NicaPairDeltaQinvCut);
      instance.SetDeleteArray(&deleteArray_NicaPairDeltaQinvCut);
      instance.SetDestructor(&destruct_NicaPairDeltaQinvCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaPairDeltaQinvCut*)
   {
      return GenerateInitInstanceLocal((::NicaPairDeltaQinvCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaPairDeltaQinvCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackLCMSCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackLCMSCut(Long_t size, void *p);
   static void delete_NicaTwoTrackLCMSCut(void *p);
   static void deleteArray_NicaTwoTrackLCMSCut(void *p);
   static void destruct_NicaTwoTrackLCMSCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackLCMSCut*)
   {
      ::NicaTwoTrackLCMSCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackLCMSCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackLCMSCut", ::NicaTwoTrackLCMSCut::Class_Version(), "NicaTwoTrackLCMSCut.h", 14,
                  typeid(::NicaTwoTrackLCMSCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackLCMSCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackLCMSCut) );
      instance.SetNew(&new_NicaTwoTrackLCMSCut);
      instance.SetNewArray(&newArray_NicaTwoTrackLCMSCut);
      instance.SetDelete(&delete_NicaTwoTrackLCMSCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackLCMSCut);
      instance.SetDestructor(&destruct_NicaTwoTrackLCMSCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackLCMSCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackLCMSCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackLCMSCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrack3DCFCut(void *p = nullptr);
   static void *newArray_NicaTwoTrack3DCFCut(Long_t size, void *p);
   static void delete_NicaTwoTrack3DCFCut(void *p);
   static void deleteArray_NicaTwoTrack3DCFCut(void *p);
   static void destruct_NicaTwoTrack3DCFCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrack3DCFCut*)
   {
      ::NicaTwoTrack3DCFCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrack3DCFCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrack3DCFCut", ::NicaTwoTrack3DCFCut::Class_Version(), "NicaTwoTrack3DCFCut.h", 15,
                  typeid(::NicaTwoTrack3DCFCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrack3DCFCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrack3DCFCut) );
      instance.SetNew(&new_NicaTwoTrack3DCFCut);
      instance.SetNewArray(&newArray_NicaTwoTrack3DCFCut);
      instance.SetDelete(&delete_NicaTwoTrack3DCFCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrack3DCFCut);
      instance.SetDestructor(&destruct_NicaTwoTrack3DCFCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrack3DCFCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrack3DCFCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrack3DCFCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackComplexCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackComplexCut(Long_t size, void *p);
   static void delete_NicaTwoTrackComplexCut(void *p);
   static void deleteArray_NicaTwoTrackComplexCut(void *p);
   static void destruct_NicaTwoTrackComplexCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackComplexCut*)
   {
      ::NicaTwoTrackComplexCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackComplexCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackComplexCut", ::NicaTwoTrackComplexCut::Class_Version(), "NicaTwoTrackComplexCut.h", 12,
                  typeid(::NicaTwoTrackComplexCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackComplexCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackComplexCut) );
      instance.SetNew(&new_NicaTwoTrackComplexCut);
      instance.SetNewArray(&newArray_NicaTwoTrackComplexCut);
      instance.SetDelete(&delete_NicaTwoTrackComplexCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackComplexCut);
      instance.SetDestructor(&destruct_NicaTwoTrackComplexCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackComplexCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackComplexCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackComplexCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackRealCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackRealCut(Long_t size, void *p);
   static void delete_NicaTwoTrackRealCut(void *p);
   static void deleteArray_NicaTwoTrackRealCut(void *p);
   static void destruct_NicaTwoTrackRealCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackRealCut*)
   {
      ::NicaTwoTrackRealCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackRealCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackRealCut", ::NicaTwoTrackRealCut::Class_Version(), "NicaTwoTrackComplexCut.h", 36,
                  typeid(::NicaTwoTrackRealCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackRealCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackRealCut) );
      instance.SetNew(&new_NicaTwoTrackRealCut);
      instance.SetNewArray(&newArray_NicaTwoTrackRealCut);
      instance.SetDelete(&delete_NicaTwoTrackRealCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackRealCut);
      instance.SetDestructor(&destruct_NicaTwoTrackRealCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackRealCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackRealCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackRealCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackImaginaryCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackImaginaryCut(Long_t size, void *p);
   static void delete_NicaTwoTrackImaginaryCut(void *p);
   static void deleteArray_NicaTwoTrackImaginaryCut(void *p);
   static void destruct_NicaTwoTrackImaginaryCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackImaginaryCut*)
   {
      ::NicaTwoTrackImaginaryCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackImaginaryCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackImaginaryCut", ::NicaTwoTrackImaginaryCut::Class_Version(), "NicaTwoTrackComplexCut.h", 54,
                  typeid(::NicaTwoTrackImaginaryCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackImaginaryCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackImaginaryCut) );
      instance.SetNew(&new_NicaTwoTrackImaginaryCut);
      instance.SetNewArray(&newArray_NicaTwoTrackImaginaryCut);
      instance.SetDelete(&delete_NicaTwoTrackImaginaryCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackImaginaryCut);
      instance.SetDestructor(&destruct_NicaTwoTrackImaginaryCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackImaginaryCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackImaginaryCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackImaginaryCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackFemtoQinvCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackFemtoQinvCut(Long_t size, void *p);
   static void delete_NicaTwoTrackFemtoQinvCut(void *p);
   static void deleteArray_NicaTwoTrackFemtoQinvCut(void *p);
   static void destruct_NicaTwoTrackFemtoQinvCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackFemtoQinvCut*)
   {
      ::NicaTwoTrackFemtoQinvCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackFemtoQinvCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackFemtoQinvCut", ::NicaTwoTrackFemtoQinvCut::Class_Version(), "NicaTwoTrackFemtoQinvCut.h", 12,
                  typeid(::NicaTwoTrackFemtoQinvCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackFemtoQinvCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackFemtoQinvCut) );
      instance.SetNew(&new_NicaTwoTrackFemtoQinvCut);
      instance.SetNewArray(&newArray_NicaTwoTrackFemtoQinvCut);
      instance.SetDelete(&delete_NicaTwoTrackFemtoQinvCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackFemtoQinvCut);
      instance.SetDestructor(&destruct_NicaTwoTrackFemtoQinvCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackFemtoQinvCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackFemtoQinvCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackFemtoQinvCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackSailorCowboyCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackSailorCowboyCut(Long_t size, void *p);
   static void delete_NicaTwoTrackSailorCowboyCut(void *p);
   static void deleteArray_NicaTwoTrackSailorCowboyCut(void *p);
   static void destruct_NicaTwoTrackSailorCowboyCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackSailorCowboyCut*)
   {
      ::NicaTwoTrackSailorCowboyCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackSailorCowboyCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackSailorCowboyCut", ::NicaTwoTrackSailorCowboyCut::Class_Version(), "NicaTwoTrackSailorCowboyCut.h", 14,
                  typeid(::NicaTwoTrackSailorCowboyCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackSailorCowboyCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackSailorCowboyCut) );
      instance.SetNew(&new_NicaTwoTrackSailorCowboyCut);
      instance.SetNewArray(&newArray_NicaTwoTrackSailorCowboyCut);
      instance.SetDelete(&delete_NicaTwoTrackSailorCowboyCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackSailorCowboyCut);
      instance.SetDestructor(&destruct_NicaTwoTrackSailorCowboyCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackSailorCowboyCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackSailorCowboyCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackSailorCowboyCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackRadialDistanceCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackRadialDistanceCut(Long_t size, void *p);
   static void delete_NicaTwoTrackRadialDistanceCut(void *p);
   static void deleteArray_NicaTwoTrackRadialDistanceCut(void *p);
   static void destruct_NicaTwoTrackRadialDistanceCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackRadialDistanceCut*)
   {
      ::NicaTwoTrackRadialDistanceCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackRadialDistanceCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackRadialDistanceCut", ::NicaTwoTrackRadialDistanceCut::Class_Version(), "NicaTwoTrackRadialDistanceCut.h", 21,
                  typeid(::NicaTwoTrackRadialDistanceCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackRadialDistanceCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackRadialDistanceCut) );
      instance.SetNew(&new_NicaTwoTrackRadialDistanceCut);
      instance.SetNewArray(&newArray_NicaTwoTrackRadialDistanceCut);
      instance.SetDelete(&delete_NicaTwoTrackRadialDistanceCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackRadialDistanceCut);
      instance.SetDestructor(&destruct_NicaTwoTrackRadialDistanceCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackRadialDistanceCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackRadialDistanceCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackRadialDistanceCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackDebugCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackDebugCut(Long_t size, void *p);
   static void delete_NicaTwoTrackDebugCut(void *p);
   static void deleteArray_NicaTwoTrackDebugCut(void *p);
   static void destruct_NicaTwoTrackDebugCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackDebugCut*)
   {
      ::NicaTwoTrackDebugCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackDebugCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackDebugCut", ::NicaTwoTrackDebugCut::Class_Version(), "NicaTwoTrackDebugCut.h", 17,
                  typeid(::NicaTwoTrackDebugCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackDebugCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackDebugCut) );
      instance.SetNew(&new_NicaTwoTrackDebugCut);
      instance.SetNewArray(&newArray_NicaTwoTrackDebugCut);
      instance.SetDelete(&delete_NicaTwoTrackDebugCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackDebugCut);
      instance.SetDestructor(&destruct_NicaTwoTrackDebugCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackDebugCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackDebugCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackDebugCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackFemtoPRFCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackFemtoPRFCut(Long_t size, void *p);
   static void delete_NicaTwoTrackFemtoPRFCut(void *p);
   static void deleteArray_NicaTwoTrackFemtoPRFCut(void *p);
   static void destruct_NicaTwoTrackFemtoPRFCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackFemtoPRFCut*)
   {
      ::NicaTwoTrackFemtoPRFCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackFemtoPRFCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackFemtoPRFCut", ::NicaTwoTrackFemtoPRFCut::Class_Version(), "NicaTwoTrackFemtoPRFCut.h", 15,
                  typeid(::NicaTwoTrackFemtoPRFCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackFemtoPRFCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackFemtoPRFCut) );
      instance.SetNew(&new_NicaTwoTrackFemtoPRFCut);
      instance.SetNewArray(&newArray_NicaTwoTrackFemtoPRFCut);
      instance.SetDelete(&delete_NicaTwoTrackFemtoPRFCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackFemtoPRFCut);
      instance.SetDestructor(&destruct_NicaTwoTrackFemtoPRFCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackFemtoPRFCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackFemtoPRFCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackFemtoPRFCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaPdgBinPairCut(void *p = nullptr);
   static void *newArray_NicaPdgBinPairCut(Long_t size, void *p);
   static void delete_NicaPdgBinPairCut(void *p);
   static void deleteArray_NicaPdgBinPairCut(void *p);
   static void destruct_NicaPdgBinPairCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaPdgBinPairCut*)
   {
      ::NicaPdgBinPairCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaPdgBinPairCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaPdgBinPairCut", ::NicaPdgBinPairCut::Class_Version(), "NicaPdgBinPairCut.h", 13,
                  typeid(::NicaPdgBinPairCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaPdgBinPairCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaPdgBinPairCut) );
      instance.SetNew(&new_NicaPdgBinPairCut);
      instance.SetNewArray(&newArray_NicaPdgBinPairCut);
      instance.SetDelete(&delete_NicaPdgBinPairCut);
      instance.SetDeleteArray(&deleteArray_NicaPdgBinPairCut);
      instance.SetDestructor(&destruct_NicaPdgBinPairCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaPdgBinPairCut*)
   {
      return GenerateInitInstanceLocal((::NicaPdgBinPairCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaPdgBinPairCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackVirtualCut(void *p = nullptr);
   static void *newArray_NicaTwoTrackVirtualCut(Long_t size, void *p);
   static void delete_NicaTwoTrackVirtualCut(void *p);
   static void deleteArray_NicaTwoTrackVirtualCut(void *p);
   static void destruct_NicaTwoTrackVirtualCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackVirtualCut*)
   {
      ::NicaTwoTrackVirtualCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackVirtualCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackVirtualCut", ::NicaTwoTrackVirtualCut::Class_Version(), "NicaTwoTrackVirtualCut.h", 17,
                  typeid(::NicaTwoTrackVirtualCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackVirtualCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackVirtualCut) );
      instance.SetNew(&new_NicaTwoTrackVirtualCut);
      instance.SetNewArray(&newArray_NicaTwoTrackVirtualCut);
      instance.SetDelete(&delete_NicaTwoTrackVirtualCut);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackVirtualCut);
      instance.SetDestructor(&destruct_NicaTwoTrackVirtualCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackVirtualCut*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackVirtualCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackVirtualCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCutMonitorX(void *p = nullptr);
   static void *newArray_NicaCutMonitorX(Long_t size, void *p);
   static void delete_NicaCutMonitorX(void *p);
   static void deleteArray_NicaCutMonitorX(void *p);
   static void destruct_NicaCutMonitorX(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCutMonitorX*)
   {
      ::NicaCutMonitorX *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCutMonitorX >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCutMonitorX", ::NicaCutMonitorX::Class_Version(), "NicaCutMonitorX.h", 15,
                  typeid(::NicaCutMonitorX), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCutMonitorX::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCutMonitorX) );
      instance.SetNew(&new_NicaCutMonitorX);
      instance.SetNewArray(&newArray_NicaCutMonitorX);
      instance.SetDelete(&delete_NicaCutMonitorX);
      instance.SetDeleteArray(&deleteArray_NicaCutMonitorX);
      instance.SetDestructor(&destruct_NicaCutMonitorX);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCutMonitorX*)
   {
      return GenerateInitInstanceLocal((::NicaCutMonitorX*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCutMonitorX*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCutMonitorXY(void *p = nullptr);
   static void *newArray_NicaCutMonitorXY(Long_t size, void *p);
   static void delete_NicaCutMonitorXY(void *p);
   static void deleteArray_NicaCutMonitorXY(void *p);
   static void destruct_NicaCutMonitorXY(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCutMonitorXY*)
   {
      ::NicaCutMonitorXY *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCutMonitorXY >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCutMonitorXY", ::NicaCutMonitorXY::Class_Version(), "NicaCutMonitorXY.h", 15,
                  typeid(::NicaCutMonitorXY), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCutMonitorXY::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCutMonitorXY) );
      instance.SetNew(&new_NicaCutMonitorXY);
      instance.SetNewArray(&newArray_NicaCutMonitorXY);
      instance.SetDelete(&delete_NicaCutMonitorXY);
      instance.SetDeleteArray(&deleteArray_NicaCutMonitorXY);
      instance.SetDestructor(&destruct_NicaCutMonitorXY);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCutMonitorXY*)
   {
      return GenerateInitInstanceLocal((::NicaCutMonitorXY*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCutMonitorXY*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCutMonitorXYZ(void *p = nullptr);
   static void *newArray_NicaCutMonitorXYZ(Long_t size, void *p);
   static void delete_NicaCutMonitorXYZ(void *p);
   static void deleteArray_NicaCutMonitorXYZ(void *p);
   static void destruct_NicaCutMonitorXYZ(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCutMonitorXYZ*)
   {
      ::NicaCutMonitorXYZ *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCutMonitorXYZ >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCutMonitorXYZ", ::NicaCutMonitorXYZ::Class_Version(), "NicaCutMonitorXYZ.h", 15,
                  typeid(::NicaCutMonitorXYZ), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCutMonitorXYZ::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCutMonitorXYZ) );
      instance.SetNew(&new_NicaCutMonitorXYZ);
      instance.SetNewArray(&newArray_NicaCutMonitorXYZ);
      instance.SetDelete(&delete_NicaCutMonitorXYZ);
      instance.SetDeleteArray(&deleteArray_NicaCutMonitorXYZ);
      instance.SetDestructor(&destruct_NicaCutMonitorXYZ);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCutMonitorXYZ*)
   {
      return GenerateInitInstanceLocal((::NicaCutMonitorXYZ*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCutMonitorXYZ*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaPropertyMonitorX(void *p = nullptr);
   static void *newArray_NicaPropertyMonitorX(Long_t size, void *p);
   static void delete_NicaPropertyMonitorX(void *p);
   static void deleteArray_NicaPropertyMonitorX(void *p);
   static void destruct_NicaPropertyMonitorX(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaPropertyMonitorX*)
   {
      ::NicaPropertyMonitorX *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaPropertyMonitorX >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaPropertyMonitorX", ::NicaPropertyMonitorX::Class_Version(), "NicaPropertyMonitorX.h", 18,
                  typeid(::NicaPropertyMonitorX), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaPropertyMonitorX::Dictionary, isa_proxy, 4,
                  sizeof(::NicaPropertyMonitorX) );
      instance.SetNew(&new_NicaPropertyMonitorX);
      instance.SetNewArray(&newArray_NicaPropertyMonitorX);
      instance.SetDelete(&delete_NicaPropertyMonitorX);
      instance.SetDeleteArray(&deleteArray_NicaPropertyMonitorX);
      instance.SetDestructor(&destruct_NicaPropertyMonitorX);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaPropertyMonitorX*)
   {
      return GenerateInitInstanceLocal((::NicaPropertyMonitorX*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaPropertyMonitorX*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaPropertyMonitorXY(void *p = nullptr);
   static void *newArray_NicaPropertyMonitorXY(Long_t size, void *p);
   static void delete_NicaPropertyMonitorXY(void *p);
   static void deleteArray_NicaPropertyMonitorXY(void *p);
   static void destruct_NicaPropertyMonitorXY(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaPropertyMonitorXY*)
   {
      ::NicaPropertyMonitorXY *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaPropertyMonitorXY >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaPropertyMonitorXY", ::NicaPropertyMonitorXY::Class_Version(), "NicaPropertyMonitorXY.h", 17,
                  typeid(::NicaPropertyMonitorXY), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaPropertyMonitorXY::Dictionary, isa_proxy, 4,
                  sizeof(::NicaPropertyMonitorXY) );
      instance.SetNew(&new_NicaPropertyMonitorXY);
      instance.SetNewArray(&newArray_NicaPropertyMonitorXY);
      instance.SetDelete(&delete_NicaPropertyMonitorXY);
      instance.SetDeleteArray(&deleteArray_NicaPropertyMonitorXY);
      instance.SetDestructor(&destruct_NicaPropertyMonitorXY);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaPropertyMonitorXY*)
   {
      return GenerateInitInstanceLocal((::NicaPropertyMonitorXY*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaPropertyMonitorXY*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaPropertyMonitorXYZ(void *p = nullptr);
   static void *newArray_NicaPropertyMonitorXYZ(Long_t size, void *p);
   static void delete_NicaPropertyMonitorXYZ(void *p);
   static void deleteArray_NicaPropertyMonitorXYZ(void *p);
   static void destruct_NicaPropertyMonitorXYZ(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaPropertyMonitorXYZ*)
   {
      ::NicaPropertyMonitorXYZ *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaPropertyMonitorXYZ >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaPropertyMonitorXYZ", ::NicaPropertyMonitorXYZ::Class_Version(), "NicaPropertyMonitorXYZ.h", 18,
                  typeid(::NicaPropertyMonitorXYZ), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaPropertyMonitorXYZ::Dictionary, isa_proxy, 4,
                  sizeof(::NicaPropertyMonitorXYZ) );
      instance.SetNew(&new_NicaPropertyMonitorXYZ);
      instance.SetNewArray(&newArray_NicaPropertyMonitorXYZ);
      instance.SetDelete(&delete_NicaPropertyMonitorXYZ);
      instance.SetDeleteArray(&deleteArray_NicaPropertyMonitorXYZ);
      instance.SetDestructor(&destruct_NicaPropertyMonitorXYZ);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaPropertyMonitorXYZ*)
   {
      return GenerateInitInstanceLocal((::NicaPropertyMonitorXYZ*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaPropertyMonitorXYZ*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NicaSubCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaSubCut::Class_Name()
{
   return "NicaSubCut";
}

//______________________________________________________________________________
const char *NicaSubCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSubCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaSubCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSubCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaSubCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSubCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaSubCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSubCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaSubCutHisto::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaSubCutHisto::Class_Name()
{
   return "NicaSubCutHisto";
}

//______________________________________________________________________________
const char *NicaSubCutHisto::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSubCutHisto*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaSubCutHisto::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSubCutHisto*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaSubCutHisto::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSubCutHisto*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaSubCutHisto::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSubCutHisto*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCut::Class_Name()
{
   return "NicaCut";
}

//______________________________________________________________________________
const char *NicaCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCutMonitor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCutMonitor::Class_Name()
{
   return "NicaCutMonitor";
}

//______________________________________________________________________________
const char *NicaCutMonitor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCutMonitor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCutMonitor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCutMonitor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCutCollection::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCutCollection::Class_Name()
{
   return "NicaCutCollection";
}

//______________________________________________________________________________
const char *NicaCutCollection::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCutCollection*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCutCollection::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCutCollection*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCutCollection::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCutCollection*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCutCollection::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCutCollection*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCutContainer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCutContainer::Class_Name()
{
   return "NicaCutContainer";
}

//______________________________________________________________________________
const char *NicaCutContainer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCutContainer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCutContainer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCutContainer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCutContainer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCutContainer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCutContainer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCutContainer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventCut::Class_Name()
{
   return "NicaEventCut";
}

//______________________________________________________________________________
const char *NicaEventCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackCut::Class_Name()
{
   return "NicaTrackCut";
}

//______________________________________________________________________________
const char *NicaTrackCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackCut::Class_Name()
{
   return "NicaTwoTrackCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCutsAndMonitors::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCutsAndMonitors::Class_Name()
{
   return "NicaCutsAndMonitors";
}

//______________________________________________________________________________
const char *NicaCutsAndMonitors::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCutsAndMonitors*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCutsAndMonitors::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCutsAndMonitors*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCutsAndMonitors::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCutsAndMonitors*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCutsAndMonitors::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCutsAndMonitors*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventImpactParameterCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventImpactParameterCut::Class_Name()
{
   return "NicaEventImpactParameterCut";
}

//______________________________________________________________________________
const char *NicaEventImpactParameterCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventImpactParameterCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventImpactParameterCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventImpactParameterCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventImpactParameterCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventImpactParameterCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventImpactParameterCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventImpactParameterCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventImpactCentralityCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventImpactCentralityCut::Class_Name()
{
   return "NicaEventImpactCentralityCut";
}

//______________________________________________________________________________
const char *NicaEventImpactCentralityCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventImpactCentralityCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventImpactCentralityCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventImpactCentralityCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventImpactCentralityCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventImpactCentralityCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventImpactCentralityCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventImpactCentralityCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventModuloCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventModuloCut::Class_Name()
{
   return "NicaEventModuloCut";
}

//______________________________________________________________________________
const char *NicaEventModuloCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventModuloCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventModuloCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventModuloCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventModuloCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventModuloCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventModuloCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventModuloCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventMultiplicityCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventMultiplicityCut::Class_Name()
{
   return "NicaEventMultiplicityCut";
}

//______________________________________________________________________________
const char *NicaEventMultiplicityCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventMultiplicityCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventMultiplicityCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventMultiplicityCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventMultiplicityCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventMultiplicityCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventMultiplicityCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventMultiplicityCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventMultiChargedCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventMultiChargedCut::Class_Name()
{
   return "NicaEventMultiChargedCut";
}

//______________________________________________________________________________
const char *NicaEventMultiChargedCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventMultiChargedCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventMultiChargedCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventMultiChargedCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventMultiChargedCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventMultiChargedCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventMultiChargedCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventMultiChargedCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventPhiCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventPhiCut::Class_Name()
{
   return "NicaEventPhiCut";
}

//______________________________________________________________________________
const char *NicaEventPhiCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventPhiCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventPhiCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventPhiCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventPhiCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventPhiCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventPhiCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventPhiCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventPtMaxCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventPtMaxCut::Class_Name()
{
   return "NicaEventPtMaxCut";
}

//______________________________________________________________________________
const char *NicaEventPtMaxCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventPtMaxCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventPtMaxCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventPtMaxCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventPtMaxCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventPtMaxCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventPtMaxCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventPtMaxCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventVirtualCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventVirtualCut::Class_Name()
{
   return "NicaEventVirtualCut";
}

//______________________________________________________________________________
const char *NicaEventVirtualCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventVirtualCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventVirtualCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventVirtualCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventVirtualCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventVirtualCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventVirtualCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventVirtualCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventComplexCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventComplexCut::Class_Name()
{
   return "NicaEventComplexCut";
}

//______________________________________________________________________________
const char *NicaEventComplexCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventComplexCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventComplexCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventComplexCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventComplexCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventComplexCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventComplexCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventComplexCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventRealCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventRealCut::Class_Name()
{
   return "NicaEventRealCut";
}

//______________________________________________________________________________
const char *NicaEventRealCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventRealCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventRealCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventRealCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventRealCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventRealCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventRealCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventRealCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventImaginaryCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventImaginaryCut::Class_Name()
{
   return "NicaEventImaginaryCut";
}

//______________________________________________________________________________
const char *NicaEventImaginaryCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventImaginaryCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventImaginaryCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventImaginaryCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventImaginaryCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventImaginaryCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventImaginaryCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventImaginaryCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventVertexCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventVertexCut::Class_Name()
{
   return "NicaEventVertexCut";
}

//______________________________________________________________________________
const char *NicaEventVertexCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventVertexCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventVertexCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventVertexCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventVertexCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventVertexCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventVertexCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventVertexCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventBinningCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventBinningCut::Class_Name()
{
   return "NicaEventBinningCut";
}

//______________________________________________________________________________
const char *NicaEventBinningCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventBinningCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventBinningCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventBinningCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventBinningCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventBinningCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventBinningCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventBinningCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventPrimaryTracksNoCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventPrimaryTracksNoCut::Class_Name()
{
   return "NicaEventPrimaryTracksNoCut";
}

//______________________________________________________________________________
const char *NicaEventPrimaryTracksNoCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventPrimaryTracksNoCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventPrimaryTracksNoCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventPrimaryTracksNoCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventPrimaryTracksNoCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventPrimaryTracksNoCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventPrimaryTracksNoCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventPrimaryTracksNoCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackExpCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackExpCut::Class_Name()
{
   return "NicaTrackExpCut";
}

//______________________________________________________________________________
const char *NicaTrackExpCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackExpCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackExpCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackExpCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackExpCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackExpCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackExpCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackExpCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackBasicToFCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackBasicToFCut::Class_Name()
{
   return "NicaTrackBasicToFCut";
}

//______________________________________________________________________________
const char *NicaTrackBasicToFCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBasicToFCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackBasicToFCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBasicToFCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackBasicToFCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBasicToFCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackBasicToFCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBasicToFCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackToFMass2Cut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackToFMass2Cut::Class_Name()
{
   return "NicaTrackToFMass2Cut";
}

//______________________________________________________________________________
const char *NicaTrackToFMass2Cut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackToFMass2Cut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackToFMass2Cut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackToFMass2Cut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackToFMass2Cut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackToFMass2Cut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackToFMass2Cut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackToFMass2Cut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackTofMass2CutNoZeros::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackTofMass2CutNoZeros::Class_Name()
{
   return "NicaTrackTofMass2CutNoZeros";
}

//______________________________________________________________________________
const char *NicaTrackTofMass2CutNoZeros::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTofMass2CutNoZeros*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackTofMass2CutNoZeros::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTofMass2CutNoZeros*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackTofMass2CutNoZeros::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTofMass2CutNoZeros*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackTofMass2CutNoZeros::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTofMass2CutNoZeros*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackDCACut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackDCACut::Class_Name()
{
   return "NicaTrackDCACut";
}

//______________________________________________________________________________
const char *NicaTrackDCACut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDCACut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackDCACut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDCACut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackDCACut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDCACut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackDCACut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDCACut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackTpcCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackTpcCut::Class_Name()
{
   return "NicaTrackTpcCut";
}

//______________________________________________________________________________
const char *NicaTrackTpcCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackTpcCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackTpcCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackTpcCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackTpcHitCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackTpcHitCut::Class_Name()
{
   return "NicaTrackTpcHitCut";
}

//______________________________________________________________________________
const char *NicaTrackTpcHitCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcHitCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackTpcHitCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcHitCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackTpcHitCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcHitCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackTpcHitCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcHitCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackTpcToFCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackTpcToFCut::Class_Name()
{
   return "NicaTrackTpcToFCut";
}

//______________________________________________________________________________
const char *NicaTrackTpcToFCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcToFCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackTpcToFCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcToFCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackTpcToFCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcToFCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackTpcToFCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcToFCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackDCAPipeCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackDCAPipeCut::Class_Name()
{
   return "NicaTrackDCAPipeCut";
}

//______________________________________________________________________________
const char *NicaTrackDCAPipeCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDCAPipeCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackDCAPipeCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDCAPipeCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackDCAPipeCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDCAPipeCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackDCAPipeCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDCAPipeCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackTpcToFThresholdlessCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackTpcToFThresholdlessCut::Class_Name()
{
   return "NicaTrackTpcToFThresholdlessCut";
}

//______________________________________________________________________________
const char *NicaTrackTpcToFThresholdlessCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcToFThresholdlessCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackTpcToFThresholdlessCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcToFThresholdlessCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackTpcToFThresholdlessCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcToFThresholdlessCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackTpcToFThresholdlessCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTpcToFThresholdlessCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackChi2Cut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackChi2Cut::Class_Name()
{
   return "NicaTrackChi2Cut";
}

//______________________________________________________________________________
const char *NicaTrackChi2Cut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackChi2Cut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackChi2Cut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackChi2Cut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackChi2Cut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackChi2Cut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackChi2Cut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackChi2Cut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackBetaCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackBetaCut::Class_Name()
{
   return "NicaTrackBetaCut";
}

//______________________________________________________________________________
const char *NicaTrackBetaCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBetaCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackBetaCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBetaCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackBetaCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBetaCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackBetaCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBetaCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackEtaCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackEtaCut::Class_Name()
{
   return "NicaTrackEtaCut";
}

//______________________________________________________________________________
const char *NicaTrackEtaCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackEtaCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackEtaCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackEtaCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackEtaCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackEtaCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackEtaCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackEtaCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackEtaAbsCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackEtaAbsCut::Class_Name()
{
   return "NicaTrackEtaAbsCut";
}

//______________________________________________________________________________
const char *NicaTrackEtaAbsCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackEtaAbsCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackEtaAbsCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackEtaAbsCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackEtaAbsCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackEtaAbsCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackEtaAbsCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackEtaAbsCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackMassCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackMassCut::Class_Name()
{
   return "NicaTrackMassCut";
}

//______________________________________________________________________________
const char *NicaTrackMassCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackMassCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackMassCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackMassCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackMassCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackMassCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackMassCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackMassCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackMtCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackMtCut::Class_Name()
{
   return "NicaTrackMtCut";
}

//______________________________________________________________________________
const char *NicaTrackMtCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackMtCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackMtCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackMtCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackMtCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackMtCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackMtCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackMtCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackPCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackPCut::Class_Name()
{
   return "NicaTrackPCut";
}

//______________________________________________________________________________
const char *NicaTrackPCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackPCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackPCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackPCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackPSignedCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackPSignedCut::Class_Name()
{
   return "NicaTrackPSignedCut";
}

//______________________________________________________________________________
const char *NicaTrackPSignedCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPSignedCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackPSignedCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPSignedCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackPSignedCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPSignedCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackPSignedCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPSignedCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackPhiCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackPhiCut::Class_Name()
{
   return "NicaTrackPhiCut";
}

//______________________________________________________________________________
const char *NicaTrackPhiCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPhiCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackPhiCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPhiCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackPhiCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPhiCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackPhiCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPhiCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackPtCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackPtCut::Class_Name()
{
   return "NicaTrackPtCut";
}

//______________________________________________________________________________
const char *NicaTrackPtCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPtCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackPtCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPtCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackPtCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPtCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackPtCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPtCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackYCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackYCut::Class_Name()
{
   return "NicaTrackYCut";
}

//______________________________________________________________________________
const char *NicaTrackYCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackYCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackYCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackYCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackYCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackYCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackYCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackYCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackPzCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackPzCut::Class_Name()
{
   return "NicaTrackPzCut";
}

//______________________________________________________________________________
const char *NicaTrackPzCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPzCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackPzCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPzCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackPzCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPzCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackPzCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPzCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackChargeCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackChargeCut::Class_Name()
{
   return "NicaTrackChargeCut";
}

//______________________________________________________________________________
const char *NicaTrackChargeCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackChargeCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackChargeCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackChargeCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackChargeCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackChargeCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackChargeCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackChargeCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackVirtualCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackVirtualCut::Class_Name()
{
   return "NicaTrackVirtualCut";
}

//______________________________________________________________________________
const char *NicaTrackVirtualCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackVirtualCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackVirtualCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackVirtualCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackVirtualCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackVirtualCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackVirtualCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackVirtualCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackComplexCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackComplexCut::Class_Name()
{
   return "NicaTrackComplexCut";
}

//______________________________________________________________________________
const char *NicaTrackComplexCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackComplexCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackComplexCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackComplexCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackComplexCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackComplexCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackComplexCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackComplexCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackRealCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackRealCut::Class_Name()
{
   return "NicaTrackRealCut";
}

//______________________________________________________________________________
const char *NicaTrackRealCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackRealCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackRealCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackRealCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackRealCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackRealCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackRealCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackRealCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackImaginaryCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackImaginaryCut::Class_Name()
{
   return "NicaTrackImaginaryCut";
}

//______________________________________________________________________________
const char *NicaTrackImaginaryCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackImaginaryCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackImaginaryCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackImaginaryCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackImaginaryCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackImaginaryCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackImaginaryCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackImaginaryCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackBoostedPCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackBoostedPCut::Class_Name()
{
   return "NicaTrackBoostedPCut";
}

//______________________________________________________________________________
const char *NicaTrackBoostedPCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBoostedPCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackBoostedPCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBoostedPCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackBoostedPCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBoostedPCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackBoostedPCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBoostedPCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackBoostedKinematcisCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackBoostedKinematcisCut::Class_Name()
{
   return "NicaTrackBoostedKinematcisCut";
}

//______________________________________________________________________________
const char *NicaTrackBoostedKinematcisCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBoostedKinematcisCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackBoostedKinematcisCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBoostedKinematcisCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackBoostedKinematcisCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBoostedKinematcisCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackBoostedKinematcisCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBoostedKinematcisCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackBoostedDeltaPCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackBoostedDeltaPCut::Class_Name()
{
   return "NicaTrackBoostedDeltaPCut";
}

//______________________________________________________________________________
const char *NicaTrackBoostedDeltaPCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBoostedDeltaPCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackBoostedDeltaPCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBoostedDeltaPCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackBoostedDeltaPCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBoostedDeltaPCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackBoostedDeltaPCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBoostedDeltaPCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackMCCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackMCCut::Class_Name()
{
   return "NicaTrackMCCut";
}

//______________________________________________________________________________
const char *NicaTrackMCCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackMCCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackMCCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackMCCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackMCCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackMCCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackMCCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackMCCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackBasicMCCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackBasicMCCut::Class_Name()
{
   return "NicaTrackBasicMCCut";
}

//______________________________________________________________________________
const char *NicaTrackBasicMCCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBasicMCCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackBasicMCCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBasicMCCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackBasicMCCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBasicMCCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackBasicMCCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackBasicMCCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackFreezoutCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackFreezoutCut::Class_Name()
{
   return "NicaTrackFreezoutCut";
}

//______________________________________________________________________________
const char *NicaTrackFreezoutCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackFreezoutCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackFreezoutCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackFreezoutCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackFreezoutCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackFreezoutCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackFreezoutCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackFreezoutCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackTFreezCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackTFreezCut::Class_Name()
{
   return "NicaTrackTFreezCut";
}

//______________________________________________________________________________
const char *NicaTrackTFreezCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTFreezCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackTFreezCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTFreezCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackTFreezCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTFreezCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackTFreezCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTFreezCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackTauCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackTauCut::Class_Name()
{
   return "NicaTrackTauCut";
}

//______________________________________________________________________________
const char *NicaTrackTauCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTauCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackTauCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTauCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackTauCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTauCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackTauCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackTauCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackPdgCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackPdgCut::Class_Name()
{
   return "NicaTrackPdgCut";
}

//______________________________________________________________________________
const char *NicaTrackPdgCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPdgCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackPdgCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPdgCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackPdgCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPdgCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackPdgCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPdgCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackPdgAbsCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackPdgAbsCut::Class_Name()
{
   return "NicaTrackPdgAbsCut";
}

//______________________________________________________________________________
const char *NicaTrackPdgAbsCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPdgAbsCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackPdgAbsCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPdgAbsCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackPdgAbsCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPdgAbsCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackPdgAbsCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPdgAbsCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackStartPositionCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackStartPositionCut::Class_Name()
{
   return "NicaTrackStartPositionCut";
}

//______________________________________________________________________________
const char *NicaTrackStartPositionCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackStartPositionCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackStartPositionCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackStartPositionCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackStartPositionCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackStartPositionCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackStartPositionCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackStartPositionCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackPdgBinCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackPdgBinCut::Class_Name()
{
   return "NicaTrackPdgBinCut";
}

//______________________________________________________________________________
const char *NicaTrackPdgBinCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPdgBinCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackPdgBinCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPdgBinCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackPdgBinCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPdgBinCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackPdgBinCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPdgBinCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackDeltaMomentumCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackDeltaMomentumCut::Class_Name()
{
   return "NicaTrackDeltaMomentumCut";
}

//______________________________________________________________________________
const char *NicaTrackDeltaMomentumCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaMomentumCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackDeltaMomentumCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaMomentumCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackDeltaMomentumCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaMomentumCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackDeltaMomentumCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaMomentumCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackDeltaMomCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackDeltaMomCut::Class_Name()
{
   return "NicaTrackDeltaMomCut";
}

//______________________________________________________________________________
const char *NicaTrackDeltaMomCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaMomCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackDeltaMomCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaMomCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackDeltaMomCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaMomCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackDeltaMomCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaMomCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackDeltaVectMomCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackDeltaVectMomCut::Class_Name()
{
   return "NicaTrackDeltaVectMomCut";
}

//______________________________________________________________________________
const char *NicaTrackDeltaVectMomCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectMomCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackDeltaVectMomCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectMomCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackDeltaVectMomCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectMomCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackDeltaVectMomCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectMomCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackDeltaPtCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackDeltaPtCut::Class_Name()
{
   return "NicaTrackDeltaPtCut";
}

//______________________________________________________________________________
const char *NicaTrackDeltaPtCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPtCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackDeltaPtCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPtCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackDeltaPtCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPtCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackDeltaPtCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPtCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackDeltaPCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackDeltaPCut::Class_Name()
{
   return "NicaTrackDeltaPCut";
}

//______________________________________________________________________________
const char *NicaTrackDeltaPCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackDeltaPCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackDeltaPCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackDeltaPCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackDeltaPzCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackDeltaPzCut::Class_Name()
{
   return "NicaTrackDeltaPzCut";
}

//______________________________________________________________________________
const char *NicaTrackDeltaPzCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPzCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackDeltaPzCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPzCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackDeltaPzCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPzCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackDeltaPzCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPzCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackDeltaPhiCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackDeltaPhiCut::Class_Name()
{
   return "NicaTrackDeltaPhiCut";
}

//______________________________________________________________________________
const char *NicaTrackDeltaPhiCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPhiCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackDeltaPhiCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPhiCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackDeltaPhiCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPhiCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackDeltaPhiCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaPhiCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackDeltaThetaCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackDeltaThetaCut::Class_Name()
{
   return "NicaTrackDeltaThetaCut";
}

//______________________________________________________________________________
const char *NicaTrackDeltaThetaCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaThetaCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackDeltaThetaCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaThetaCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackDeltaThetaCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaThetaCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackDeltaThetaCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaThetaCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackDeltaVectPtCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackDeltaVectPtCut::Class_Name()
{
   return "NicaTrackDeltaVectPtCut";
}

//______________________________________________________________________________
const char *NicaTrackDeltaVectPtCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPtCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackDeltaVectPtCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPtCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackDeltaVectPtCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPtCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackDeltaVectPtCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPtCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackDeltaVectPCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackDeltaVectPCut::Class_Name()
{
   return "NicaTrackDeltaVectPCut";
}

//______________________________________________________________________________
const char *NicaTrackDeltaVectPCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackDeltaVectPCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackDeltaVectPCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackDeltaVectPCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackDeltaVectPhiCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackDeltaVectPhiCut::Class_Name()
{
   return "NicaTrackDeltaVectPhiCut";
}

//______________________________________________________________________________
const char *NicaTrackDeltaVectPhiCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPhiCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackDeltaVectPhiCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPhiCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackDeltaVectPhiCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPhiCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackDeltaVectPhiCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectPhiCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackDeltaVectThetaCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackDeltaVectThetaCut::Class_Name()
{
   return "NicaTrackDeltaVectThetaCut";
}

//______________________________________________________________________________
const char *NicaTrackDeltaVectThetaCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectThetaCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackDeltaVectThetaCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectThetaCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackDeltaVectThetaCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectThetaCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackDeltaVectThetaCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackDeltaVectThetaCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackOnlyPrimariesCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackOnlyPrimariesCut::Class_Name()
{
   return "NicaTrackOnlyPrimariesCut";
}

//______________________________________________________________________________
const char *NicaTrackOnlyPrimariesCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackOnlyPrimariesCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackOnlyPrimariesCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackOnlyPrimariesCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackOnlyPrimariesCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackOnlyPrimariesCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackOnlyPrimariesCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackOnlyPrimariesCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackStatusCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackStatusCut::Class_Name()
{
   return "NicaTrackStatusCut";
}

//______________________________________________________________________________
const char *NicaTrackStatusCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackStatusCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackStatusCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackStatusCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackStatusCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackStatusCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackStatusCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackStatusCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackPrimaryFlagCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackPrimaryFlagCut::Class_Name()
{
   return "NicaTrackPrimaryFlagCut";
}

//______________________________________________________________________________
const char *NicaTrackPrimaryFlagCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPrimaryFlagCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackPrimaryFlagCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPrimaryFlagCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackPrimaryFlagCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPrimaryFlagCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackPrimaryFlagCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackPrimaryFlagCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackV0FlagCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackV0FlagCut::Class_Name()
{
   return "NicaTrackV0FlagCut";
}

//______________________________________________________________________________
const char *NicaTrackV0FlagCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackV0FlagCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackV0FlagCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackV0FlagCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackV0FlagCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackV0FlagCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackV0FlagCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackV0FlagCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackGoodSecondaryFlagCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackGoodSecondaryFlagCut::Class_Name()
{
   return "NicaTrackGoodSecondaryFlagCut";
}

//______________________________________________________________________________
const char *NicaTrackGoodSecondaryFlagCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackGoodSecondaryFlagCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackGoodSecondaryFlagCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackGoodSecondaryFlagCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackGoodSecondaryFlagCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackGoodSecondaryFlagCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackGoodSecondaryFlagCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackGoodSecondaryFlagCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackGlobalFlagCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackGlobalFlagCut::Class_Name()
{
   return "NicaTrackGlobalFlagCut";
}

//______________________________________________________________________________
const char *NicaTrackGlobalFlagCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackGlobalFlagCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackGlobalFlagCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackGlobalFlagCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackGlobalFlagCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackGlobalFlagCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackGlobalFlagCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackGlobalFlagCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackEtaCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackEtaCut::Class_Name()
{
   return "NicaTwoTrackEtaCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackEtaCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackEtaCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackEtaCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackEtaCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackEtaCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackEtaCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackEtaCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackEtaCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackKtCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackKtCut::Class_Name()
{
   return "NicaTwoTrackKtCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackKtCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackKtCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackKtCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackKtCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackKtCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackKtCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackKtCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackKtCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackMinvCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackMinvCut::Class_Name()
{
   return "NicaTwoTrackMinvCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackMinvCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackMinvCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackMinvCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackMinvCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackMinvCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackMinvCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackMinvCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackMinvCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackPhiCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackPhiCut::Class_Name()
{
   return "NicaTwoTrackPhiCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackPhiCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackPhiCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackPhiCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackPhiCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackPhiCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackPhiCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackPhiCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackPhiCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackPtOrderCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackPtOrderCut::Class_Name()
{
   return "NicaTwoTrackPtOrderCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackPtOrderCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackPtOrderCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackPtOrderCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackPtOrderCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackPtOrderCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackPtOrderCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackPtOrderCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackPtOrderCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackDphiDetaCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackDphiDetaCut::Class_Name()
{
   return "NicaTwoTrackDphiDetaCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackDphiDetaCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDphiDetaCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackDphiDetaCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDphiDetaCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackDphiDetaCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDphiDetaCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackDphiDetaCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDphiDetaCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackDphistarDetaCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackDphistarDetaCut::Class_Name()
{
   return "NicaTwoTrackDphistarDetaCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackDphistarDetaCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDphistarDetaCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackDphistarDetaCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDphistarDetaCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackDphistarDetaCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDphistarDetaCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackDphistarDetaCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDphistarDetaCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackPtSumCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackPtSumCut::Class_Name()
{
   return "NicaTwoTrackPtSumCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackPtSumCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackPtSumCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackPtSumCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackPtSumCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackPtSumCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackPtSumCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackPtSumCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackPtSumCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackRapidityCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackRapidityCut::Class_Name()
{
   return "NicaTwoTrackRapidityCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackRapidityCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackRapidityCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackRapidityCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackRapidityCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackRapidityCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackRapidityCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackRapidityCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackRapidityCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackDeltaEtaCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackDeltaEtaCut::Class_Name()
{
   return "NicaTwoTrackDeltaEtaCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackDeltaEtaCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDeltaEtaCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackDeltaEtaCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDeltaEtaCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackDeltaEtaCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDeltaEtaCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackDeltaEtaCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDeltaEtaCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaPairDeltaQinvCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaPairDeltaQinvCut::Class_Name()
{
   return "NicaPairDeltaQinvCut";
}

//______________________________________________________________________________
const char *NicaPairDeltaQinvCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPairDeltaQinvCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaPairDeltaQinvCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPairDeltaQinvCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaPairDeltaQinvCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPairDeltaQinvCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaPairDeltaQinvCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPairDeltaQinvCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackLCMSCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackLCMSCut::Class_Name()
{
   return "NicaTwoTrackLCMSCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackLCMSCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackLCMSCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackLCMSCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackLCMSCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackLCMSCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackLCMSCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackLCMSCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackLCMSCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrack3DCFCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrack3DCFCut::Class_Name()
{
   return "NicaTwoTrack3DCFCut";
}

//______________________________________________________________________________
const char *NicaTwoTrack3DCFCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrack3DCFCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrack3DCFCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrack3DCFCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrack3DCFCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrack3DCFCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrack3DCFCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrack3DCFCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackComplexCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackComplexCut::Class_Name()
{
   return "NicaTwoTrackComplexCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackComplexCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackComplexCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackComplexCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackComplexCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackComplexCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackComplexCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackComplexCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackComplexCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackRealCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackRealCut::Class_Name()
{
   return "NicaTwoTrackRealCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackRealCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackRealCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackRealCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackRealCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackRealCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackRealCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackRealCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackRealCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackImaginaryCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackImaginaryCut::Class_Name()
{
   return "NicaTwoTrackImaginaryCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackImaginaryCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackImaginaryCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackImaginaryCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackImaginaryCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackImaginaryCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackImaginaryCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackImaginaryCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackImaginaryCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackFemtoQinvCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackFemtoQinvCut::Class_Name()
{
   return "NicaTwoTrackFemtoQinvCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackFemtoQinvCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackFemtoQinvCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackFemtoQinvCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackFemtoQinvCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackFemtoQinvCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackFemtoQinvCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackFemtoQinvCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackFemtoQinvCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackSailorCowboyCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackSailorCowboyCut::Class_Name()
{
   return "NicaTwoTrackSailorCowboyCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackSailorCowboyCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackSailorCowboyCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackSailorCowboyCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackSailorCowboyCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackSailorCowboyCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackSailorCowboyCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackSailorCowboyCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackSailorCowboyCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackRadialDistanceCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackRadialDistanceCut::Class_Name()
{
   return "NicaTwoTrackRadialDistanceCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackRadialDistanceCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackRadialDistanceCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackRadialDistanceCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackRadialDistanceCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackRadialDistanceCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackRadialDistanceCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackRadialDistanceCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackRadialDistanceCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackDebugCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackDebugCut::Class_Name()
{
   return "NicaTwoTrackDebugCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackDebugCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDebugCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackDebugCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDebugCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackDebugCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDebugCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackDebugCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackDebugCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackFemtoPRFCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackFemtoPRFCut::Class_Name()
{
   return "NicaTwoTrackFemtoPRFCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackFemtoPRFCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackFemtoPRFCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackFemtoPRFCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackFemtoPRFCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackFemtoPRFCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackFemtoPRFCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackFemtoPRFCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackFemtoPRFCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaPdgBinPairCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaPdgBinPairCut::Class_Name()
{
   return "NicaPdgBinPairCut";
}

//______________________________________________________________________________
const char *NicaPdgBinPairCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPdgBinPairCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaPdgBinPairCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPdgBinPairCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaPdgBinPairCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPdgBinPairCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaPdgBinPairCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPdgBinPairCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackVirtualCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackVirtualCut::Class_Name()
{
   return "NicaTwoTrackVirtualCut";
}

//______________________________________________________________________________
const char *NicaTwoTrackVirtualCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackVirtualCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackVirtualCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackVirtualCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackVirtualCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackVirtualCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackVirtualCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackVirtualCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCutMonitorX::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCutMonitorX::Class_Name()
{
   return "NicaCutMonitorX";
}

//______________________________________________________________________________
const char *NicaCutMonitorX::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitorX*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCutMonitorX::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitorX*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCutMonitorX::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitorX*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCutMonitorX::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitorX*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCutMonitorXY::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCutMonitorXY::Class_Name()
{
   return "NicaCutMonitorXY";
}

//______________________________________________________________________________
const char *NicaCutMonitorXY::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitorXY*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCutMonitorXY::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitorXY*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCutMonitorXY::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitorXY*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCutMonitorXY::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitorXY*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCutMonitorXYZ::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCutMonitorXYZ::Class_Name()
{
   return "NicaCutMonitorXYZ";
}

//______________________________________________________________________________
const char *NicaCutMonitorXYZ::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitorXYZ*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCutMonitorXYZ::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitorXYZ*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCutMonitorXYZ::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitorXYZ*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCutMonitorXYZ::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCutMonitorXYZ*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaPropertyMonitorX::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaPropertyMonitorX::Class_Name()
{
   return "NicaPropertyMonitorX";
}

//______________________________________________________________________________
const char *NicaPropertyMonitorX::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPropertyMonitorX*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaPropertyMonitorX::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPropertyMonitorX*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaPropertyMonitorX::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPropertyMonitorX*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaPropertyMonitorX::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPropertyMonitorX*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaPropertyMonitorXY::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaPropertyMonitorXY::Class_Name()
{
   return "NicaPropertyMonitorXY";
}

//______________________________________________________________________________
const char *NicaPropertyMonitorXY::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPropertyMonitorXY*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaPropertyMonitorXY::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPropertyMonitorXY*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaPropertyMonitorXY::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPropertyMonitorXY*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaPropertyMonitorXY::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPropertyMonitorXY*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaPropertyMonitorXYZ::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaPropertyMonitorXYZ::Class_Name()
{
   return "NicaPropertyMonitorXYZ";
}

//______________________________________________________________________________
const char *NicaPropertyMonitorXYZ::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPropertyMonitorXYZ*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaPropertyMonitorXYZ::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPropertyMonitorXYZ*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaPropertyMonitorXYZ::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPropertyMonitorXYZ*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaPropertyMonitorXYZ::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPropertyMonitorXYZ*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void NicaSubCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaSubCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaSubCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaSubCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaSubCut(void *p) {
      return  p ? new(p) ::NicaSubCut : new ::NicaSubCut;
   }
   static void *newArray_NicaSubCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaSubCut[nElements] : new ::NicaSubCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaSubCut(void *p) {
      delete ((::NicaSubCut*)p);
   }
   static void deleteArray_NicaSubCut(void *p) {
      delete [] ((::NicaSubCut*)p);
   }
   static void destruct_NicaSubCut(void *p) {
      typedef ::NicaSubCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaSubCut

//______________________________________________________________________________
void NicaSubCutHisto::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaSubCutHisto.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaSubCutHisto::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaSubCutHisto::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaSubCutHisto(void *p) {
      return  p ? new(p) ::NicaSubCutHisto : new ::NicaSubCutHisto;
   }
   static void *newArray_NicaSubCutHisto(Long_t nElements, void *p) {
      return p ? new(p) ::NicaSubCutHisto[nElements] : new ::NicaSubCutHisto[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaSubCutHisto(void *p) {
      delete ((::NicaSubCutHisto*)p);
   }
   static void deleteArray_NicaSubCutHisto(void *p) {
      delete [] ((::NicaSubCutHisto*)p);
   }
   static void destruct_NicaSubCutHisto(void *p) {
      typedef ::NicaSubCutHisto current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaSubCutHisto

//______________________________________________________________________________
void NicaCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCut(void *p) {
      return  p ? new(p) ::NicaCut : new ::NicaCut;
   }
   static void *newArray_NicaCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCut[nElements] : new ::NicaCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCut(void *p) {
      delete ((::NicaCut*)p);
   }
   static void deleteArray_NicaCut(void *p) {
      delete [] ((::NicaCut*)p);
   }
   static void destruct_NicaCut(void *p) {
      typedef ::NicaCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCut

//______________________________________________________________________________
void NicaCutMonitor::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCutMonitor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCutMonitor::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCutMonitor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCutMonitor(void *p) {
      return  p ? new(p) ::NicaCutMonitor : new ::NicaCutMonitor;
   }
   static void *newArray_NicaCutMonitor(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCutMonitor[nElements] : new ::NicaCutMonitor[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCutMonitor(void *p) {
      delete ((::NicaCutMonitor*)p);
   }
   static void deleteArray_NicaCutMonitor(void *p) {
      delete [] ((::NicaCutMonitor*)p);
   }
   static void destruct_NicaCutMonitor(void *p) {
      typedef ::NicaCutMonitor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCutMonitor

//______________________________________________________________________________
void NicaCutCollection::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCutCollection.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCutCollection::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCutCollection::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCutCollection(void *p) {
      return  p ? new(p) ::NicaCutCollection : new ::NicaCutCollection;
   }
   static void *newArray_NicaCutCollection(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCutCollection[nElements] : new ::NicaCutCollection[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCutCollection(void *p) {
      delete ((::NicaCutCollection*)p);
   }
   static void deleteArray_NicaCutCollection(void *p) {
      delete [] ((::NicaCutCollection*)p);
   }
   static void destruct_NicaCutCollection(void *p) {
      typedef ::NicaCutCollection current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCutCollection

//______________________________________________________________________________
void NicaCutContainer::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCutContainer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCutContainer::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCutContainer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCutContainer(void *p) {
      return  p ? new(p) ::NicaCutContainer : new ::NicaCutContainer;
   }
   static void *newArray_NicaCutContainer(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCutContainer[nElements] : new ::NicaCutContainer[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCutContainer(void *p) {
      delete ((::NicaCutContainer*)p);
   }
   static void deleteArray_NicaCutContainer(void *p) {
      delete [] ((::NicaCutContainer*)p);
   }
   static void destruct_NicaCutContainer(void *p) {
      typedef ::NicaCutContainer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCutContainer

//______________________________________________________________________________
void NicaEventCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventCut::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaEventCut(void *p) {
      delete ((::NicaEventCut*)p);
   }
   static void deleteArray_NicaEventCut(void *p) {
      delete [] ((::NicaEventCut*)p);
   }
   static void destruct_NicaEventCut(void *p) {
      typedef ::NicaEventCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventCut

//______________________________________________________________________________
void NicaTrackCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackCut::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaTrackCut(void *p) {
      delete ((::NicaTrackCut*)p);
   }
   static void deleteArray_NicaTrackCut(void *p) {
      delete [] ((::NicaTrackCut*)p);
   }
   static void destruct_NicaTrackCut(void *p) {
      typedef ::NicaTrackCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackCut

//______________________________________________________________________________
void NicaTwoTrackCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackCut::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaTwoTrackCut(void *p) {
      delete ((::NicaTwoTrackCut*)p);
   }
   static void deleteArray_NicaTwoTrackCut(void *p) {
      delete [] ((::NicaTwoTrackCut*)p);
   }
   static void destruct_NicaTwoTrackCut(void *p) {
      typedef ::NicaTwoTrackCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackCut

//______________________________________________________________________________
void NicaCutsAndMonitors::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCutsAndMonitors.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCutsAndMonitors::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCutsAndMonitors::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCutsAndMonitors(void *p) {
      return  p ? new(p) ::NicaCutsAndMonitors : new ::NicaCutsAndMonitors;
   }
   static void *newArray_NicaCutsAndMonitors(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCutsAndMonitors[nElements] : new ::NicaCutsAndMonitors[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCutsAndMonitors(void *p) {
      delete ((::NicaCutsAndMonitors*)p);
   }
   static void deleteArray_NicaCutsAndMonitors(void *p) {
      delete [] ((::NicaCutsAndMonitors*)p);
   }
   static void destruct_NicaCutsAndMonitors(void *p) {
      typedef ::NicaCutsAndMonitors current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCutsAndMonitors

//______________________________________________________________________________
void NicaEventImpactParameterCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventImpactParameterCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventImpactParameterCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventImpactParameterCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventImpactParameterCut(void *p) {
      return  p ? new(p) ::NicaEventImpactParameterCut : new ::NicaEventImpactParameterCut;
   }
   static void *newArray_NicaEventImpactParameterCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventImpactParameterCut[nElements] : new ::NicaEventImpactParameterCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventImpactParameterCut(void *p) {
      delete ((::NicaEventImpactParameterCut*)p);
   }
   static void deleteArray_NicaEventImpactParameterCut(void *p) {
      delete [] ((::NicaEventImpactParameterCut*)p);
   }
   static void destruct_NicaEventImpactParameterCut(void *p) {
      typedef ::NicaEventImpactParameterCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventImpactParameterCut

//______________________________________________________________________________
void NicaEventImpactCentralityCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventImpactCentralityCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventImpactCentralityCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventImpactCentralityCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventImpactCentralityCut(void *p) {
      return  p ? new(p) ::NicaEventImpactCentralityCut : new ::NicaEventImpactCentralityCut;
   }
   static void *newArray_NicaEventImpactCentralityCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventImpactCentralityCut[nElements] : new ::NicaEventImpactCentralityCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventImpactCentralityCut(void *p) {
      delete ((::NicaEventImpactCentralityCut*)p);
   }
   static void deleteArray_NicaEventImpactCentralityCut(void *p) {
      delete [] ((::NicaEventImpactCentralityCut*)p);
   }
   static void destruct_NicaEventImpactCentralityCut(void *p) {
      typedef ::NicaEventImpactCentralityCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventImpactCentralityCut

//______________________________________________________________________________
void NicaEventModuloCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventModuloCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventModuloCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventModuloCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventModuloCut(void *p) {
      return  p ? new(p) ::NicaEventModuloCut : new ::NicaEventModuloCut;
   }
   static void *newArray_NicaEventModuloCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventModuloCut[nElements] : new ::NicaEventModuloCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventModuloCut(void *p) {
      delete ((::NicaEventModuloCut*)p);
   }
   static void deleteArray_NicaEventModuloCut(void *p) {
      delete [] ((::NicaEventModuloCut*)p);
   }
   static void destruct_NicaEventModuloCut(void *p) {
      typedef ::NicaEventModuloCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventModuloCut

//______________________________________________________________________________
void NicaEventMultiplicityCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventMultiplicityCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventMultiplicityCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventMultiplicityCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventMultiplicityCut(void *p) {
      return  p ? new(p) ::NicaEventMultiplicityCut : new ::NicaEventMultiplicityCut;
   }
   static void *newArray_NicaEventMultiplicityCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventMultiplicityCut[nElements] : new ::NicaEventMultiplicityCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventMultiplicityCut(void *p) {
      delete ((::NicaEventMultiplicityCut*)p);
   }
   static void deleteArray_NicaEventMultiplicityCut(void *p) {
      delete [] ((::NicaEventMultiplicityCut*)p);
   }
   static void destruct_NicaEventMultiplicityCut(void *p) {
      typedef ::NicaEventMultiplicityCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventMultiplicityCut

//______________________________________________________________________________
void NicaEventMultiChargedCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventMultiChargedCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventMultiChargedCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventMultiChargedCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventMultiChargedCut(void *p) {
      return  p ? new(p) ::NicaEventMultiChargedCut : new ::NicaEventMultiChargedCut;
   }
   static void *newArray_NicaEventMultiChargedCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventMultiChargedCut[nElements] : new ::NicaEventMultiChargedCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventMultiChargedCut(void *p) {
      delete ((::NicaEventMultiChargedCut*)p);
   }
   static void deleteArray_NicaEventMultiChargedCut(void *p) {
      delete [] ((::NicaEventMultiChargedCut*)p);
   }
   static void destruct_NicaEventMultiChargedCut(void *p) {
      typedef ::NicaEventMultiChargedCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventMultiChargedCut

//______________________________________________________________________________
void NicaEventPhiCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventPhiCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventPhiCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventPhiCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventPhiCut(void *p) {
      return  p ? new(p) ::NicaEventPhiCut : new ::NicaEventPhiCut;
   }
   static void *newArray_NicaEventPhiCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventPhiCut[nElements] : new ::NicaEventPhiCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventPhiCut(void *p) {
      delete ((::NicaEventPhiCut*)p);
   }
   static void deleteArray_NicaEventPhiCut(void *p) {
      delete [] ((::NicaEventPhiCut*)p);
   }
   static void destruct_NicaEventPhiCut(void *p) {
      typedef ::NicaEventPhiCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventPhiCut

//______________________________________________________________________________
void NicaEventPtMaxCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventPtMaxCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventPtMaxCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventPtMaxCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventPtMaxCut(void *p) {
      return  p ? new(p) ::NicaEventPtMaxCut : new ::NicaEventPtMaxCut;
   }
   static void *newArray_NicaEventPtMaxCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventPtMaxCut[nElements] : new ::NicaEventPtMaxCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventPtMaxCut(void *p) {
      delete ((::NicaEventPtMaxCut*)p);
   }
   static void deleteArray_NicaEventPtMaxCut(void *p) {
      delete [] ((::NicaEventPtMaxCut*)p);
   }
   static void destruct_NicaEventPtMaxCut(void *p) {
      typedef ::NicaEventPtMaxCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventPtMaxCut

//______________________________________________________________________________
void NicaEventVirtualCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventVirtualCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventVirtualCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventVirtualCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventVirtualCut(void *p) {
      return  p ? new(p) ::NicaEventVirtualCut : new ::NicaEventVirtualCut;
   }
   static void *newArray_NicaEventVirtualCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventVirtualCut[nElements] : new ::NicaEventVirtualCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventVirtualCut(void *p) {
      delete ((::NicaEventVirtualCut*)p);
   }
   static void deleteArray_NicaEventVirtualCut(void *p) {
      delete [] ((::NicaEventVirtualCut*)p);
   }
   static void destruct_NicaEventVirtualCut(void *p) {
      typedef ::NicaEventVirtualCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventVirtualCut

//______________________________________________________________________________
void NicaEventComplexCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventComplexCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventComplexCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventComplexCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventComplexCut(void *p) {
      return  p ? new(p) ::NicaEventComplexCut : new ::NicaEventComplexCut;
   }
   static void *newArray_NicaEventComplexCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventComplexCut[nElements] : new ::NicaEventComplexCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventComplexCut(void *p) {
      delete ((::NicaEventComplexCut*)p);
   }
   static void deleteArray_NicaEventComplexCut(void *p) {
      delete [] ((::NicaEventComplexCut*)p);
   }
   static void destruct_NicaEventComplexCut(void *p) {
      typedef ::NicaEventComplexCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventComplexCut

//______________________________________________________________________________
void NicaEventRealCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventRealCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventRealCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventRealCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventRealCut(void *p) {
      return  p ? new(p) ::NicaEventRealCut : new ::NicaEventRealCut;
   }
   static void *newArray_NicaEventRealCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventRealCut[nElements] : new ::NicaEventRealCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventRealCut(void *p) {
      delete ((::NicaEventRealCut*)p);
   }
   static void deleteArray_NicaEventRealCut(void *p) {
      delete [] ((::NicaEventRealCut*)p);
   }
   static void destruct_NicaEventRealCut(void *p) {
      typedef ::NicaEventRealCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventRealCut

//______________________________________________________________________________
void NicaEventImaginaryCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventImaginaryCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventImaginaryCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventImaginaryCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventImaginaryCut(void *p) {
      return  p ? new(p) ::NicaEventImaginaryCut : new ::NicaEventImaginaryCut;
   }
   static void *newArray_NicaEventImaginaryCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventImaginaryCut[nElements] : new ::NicaEventImaginaryCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventImaginaryCut(void *p) {
      delete ((::NicaEventImaginaryCut*)p);
   }
   static void deleteArray_NicaEventImaginaryCut(void *p) {
      delete [] ((::NicaEventImaginaryCut*)p);
   }
   static void destruct_NicaEventImaginaryCut(void *p) {
      typedef ::NicaEventImaginaryCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventImaginaryCut

//______________________________________________________________________________
void NicaEventVertexCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventVertexCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventVertexCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventVertexCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventVertexCut(void *p) {
      return  p ? new(p) ::NicaEventVertexCut : new ::NicaEventVertexCut;
   }
   static void *newArray_NicaEventVertexCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventVertexCut[nElements] : new ::NicaEventVertexCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventVertexCut(void *p) {
      delete ((::NicaEventVertexCut*)p);
   }
   static void deleteArray_NicaEventVertexCut(void *p) {
      delete [] ((::NicaEventVertexCut*)p);
   }
   static void destruct_NicaEventVertexCut(void *p) {
      typedef ::NicaEventVertexCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventVertexCut

//______________________________________________________________________________
void NicaEventBinningCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventBinningCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventBinningCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventBinningCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventBinningCut(void *p) {
      return  p ? new(p) ::NicaEventBinningCut : new ::NicaEventBinningCut;
   }
   static void *newArray_NicaEventBinningCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventBinningCut[nElements] : new ::NicaEventBinningCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventBinningCut(void *p) {
      delete ((::NicaEventBinningCut*)p);
   }
   static void deleteArray_NicaEventBinningCut(void *p) {
      delete [] ((::NicaEventBinningCut*)p);
   }
   static void destruct_NicaEventBinningCut(void *p) {
      typedef ::NicaEventBinningCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventBinningCut

//______________________________________________________________________________
void NicaEventPrimaryTracksNoCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventPrimaryTracksNoCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventPrimaryTracksNoCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventPrimaryTracksNoCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventPrimaryTracksNoCut(void *p) {
      return  p ? new(p) ::NicaEventPrimaryTracksNoCut : new ::NicaEventPrimaryTracksNoCut;
   }
   static void *newArray_NicaEventPrimaryTracksNoCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventPrimaryTracksNoCut[nElements] : new ::NicaEventPrimaryTracksNoCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventPrimaryTracksNoCut(void *p) {
      delete ((::NicaEventPrimaryTracksNoCut*)p);
   }
   static void deleteArray_NicaEventPrimaryTracksNoCut(void *p) {
      delete [] ((::NicaEventPrimaryTracksNoCut*)p);
   }
   static void destruct_NicaEventPrimaryTracksNoCut(void *p) {
      typedef ::NicaEventPrimaryTracksNoCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventPrimaryTracksNoCut

//______________________________________________________________________________
void NicaTrackExpCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackExpCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackExpCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackExpCut::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaTrackExpCut(void *p) {
      delete ((::NicaTrackExpCut*)p);
   }
   static void deleteArray_NicaTrackExpCut(void *p) {
      delete [] ((::NicaTrackExpCut*)p);
   }
   static void destruct_NicaTrackExpCut(void *p) {
      typedef ::NicaTrackExpCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackExpCut

//______________________________________________________________________________
void NicaTrackBasicToFCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackBasicToFCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackBasicToFCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackBasicToFCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackBasicToFCut(void *p) {
      return  p ? new(p) ::NicaTrackBasicToFCut : new ::NicaTrackBasicToFCut;
   }
   static void *newArray_NicaTrackBasicToFCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackBasicToFCut[nElements] : new ::NicaTrackBasicToFCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackBasicToFCut(void *p) {
      delete ((::NicaTrackBasicToFCut*)p);
   }
   static void deleteArray_NicaTrackBasicToFCut(void *p) {
      delete [] ((::NicaTrackBasicToFCut*)p);
   }
   static void destruct_NicaTrackBasicToFCut(void *p) {
      typedef ::NicaTrackBasicToFCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackBasicToFCut

//______________________________________________________________________________
void NicaTrackToFMass2Cut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackToFMass2Cut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackToFMass2Cut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackToFMass2Cut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackToFMass2Cut(void *p) {
      return  p ? new(p) ::NicaTrackToFMass2Cut : new ::NicaTrackToFMass2Cut;
   }
   static void *newArray_NicaTrackToFMass2Cut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackToFMass2Cut[nElements] : new ::NicaTrackToFMass2Cut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackToFMass2Cut(void *p) {
      delete ((::NicaTrackToFMass2Cut*)p);
   }
   static void deleteArray_NicaTrackToFMass2Cut(void *p) {
      delete [] ((::NicaTrackToFMass2Cut*)p);
   }
   static void destruct_NicaTrackToFMass2Cut(void *p) {
      typedef ::NicaTrackToFMass2Cut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackToFMass2Cut

//______________________________________________________________________________
void NicaTrackTofMass2CutNoZeros::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackTofMass2CutNoZeros.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackTofMass2CutNoZeros::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackTofMass2CutNoZeros::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackTofMass2CutNoZeros(void *p) {
      return  p ? new(p) ::NicaTrackTofMass2CutNoZeros : new ::NicaTrackTofMass2CutNoZeros;
   }
   static void *newArray_NicaTrackTofMass2CutNoZeros(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackTofMass2CutNoZeros[nElements] : new ::NicaTrackTofMass2CutNoZeros[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackTofMass2CutNoZeros(void *p) {
      delete ((::NicaTrackTofMass2CutNoZeros*)p);
   }
   static void deleteArray_NicaTrackTofMass2CutNoZeros(void *p) {
      delete [] ((::NicaTrackTofMass2CutNoZeros*)p);
   }
   static void destruct_NicaTrackTofMass2CutNoZeros(void *p) {
      typedef ::NicaTrackTofMass2CutNoZeros current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackTofMass2CutNoZeros

//______________________________________________________________________________
void NicaTrackDCACut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackDCACut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackDCACut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackDCACut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackDCACut(void *p) {
      return  p ? new(p) ::NicaTrackDCACut : new ::NicaTrackDCACut;
   }
   static void *newArray_NicaTrackDCACut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackDCACut[nElements] : new ::NicaTrackDCACut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackDCACut(void *p) {
      delete ((::NicaTrackDCACut*)p);
   }
   static void deleteArray_NicaTrackDCACut(void *p) {
      delete [] ((::NicaTrackDCACut*)p);
   }
   static void destruct_NicaTrackDCACut(void *p) {
      typedef ::NicaTrackDCACut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackDCACut

//______________________________________________________________________________
void NicaTrackTpcCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackTpcCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackTpcCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackTpcCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackTpcCut(void *p) {
      return  p ? new(p) ::NicaTrackTpcCut : new ::NicaTrackTpcCut;
   }
   static void *newArray_NicaTrackTpcCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackTpcCut[nElements] : new ::NicaTrackTpcCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackTpcCut(void *p) {
      delete ((::NicaTrackTpcCut*)p);
   }
   static void deleteArray_NicaTrackTpcCut(void *p) {
      delete [] ((::NicaTrackTpcCut*)p);
   }
   static void destruct_NicaTrackTpcCut(void *p) {
      typedef ::NicaTrackTpcCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackTpcCut

//______________________________________________________________________________
void NicaTrackTpcHitCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackTpcHitCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackTpcHitCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackTpcHitCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackTpcHitCut(void *p) {
      return  p ? new(p) ::NicaTrackTpcHitCut : new ::NicaTrackTpcHitCut;
   }
   static void *newArray_NicaTrackTpcHitCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackTpcHitCut[nElements] : new ::NicaTrackTpcHitCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackTpcHitCut(void *p) {
      delete ((::NicaTrackTpcHitCut*)p);
   }
   static void deleteArray_NicaTrackTpcHitCut(void *p) {
      delete [] ((::NicaTrackTpcHitCut*)p);
   }
   static void destruct_NicaTrackTpcHitCut(void *p) {
      typedef ::NicaTrackTpcHitCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackTpcHitCut

//______________________________________________________________________________
void NicaTrackTpcToFCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackTpcToFCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackTpcToFCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackTpcToFCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackTpcToFCut(void *p) {
      return  p ? new(p) ::NicaTrackTpcToFCut : new ::NicaTrackTpcToFCut;
   }
   static void *newArray_NicaTrackTpcToFCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackTpcToFCut[nElements] : new ::NicaTrackTpcToFCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackTpcToFCut(void *p) {
      delete ((::NicaTrackTpcToFCut*)p);
   }
   static void deleteArray_NicaTrackTpcToFCut(void *p) {
      delete [] ((::NicaTrackTpcToFCut*)p);
   }
   static void destruct_NicaTrackTpcToFCut(void *p) {
      typedef ::NicaTrackTpcToFCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackTpcToFCut

//______________________________________________________________________________
void NicaTrackDCAPipeCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackDCAPipeCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackDCAPipeCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackDCAPipeCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackDCAPipeCut(void *p) {
      return  p ? new(p) ::NicaTrackDCAPipeCut : new ::NicaTrackDCAPipeCut;
   }
   static void *newArray_NicaTrackDCAPipeCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackDCAPipeCut[nElements] : new ::NicaTrackDCAPipeCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackDCAPipeCut(void *p) {
      delete ((::NicaTrackDCAPipeCut*)p);
   }
   static void deleteArray_NicaTrackDCAPipeCut(void *p) {
      delete [] ((::NicaTrackDCAPipeCut*)p);
   }
   static void destruct_NicaTrackDCAPipeCut(void *p) {
      typedef ::NicaTrackDCAPipeCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackDCAPipeCut

//______________________________________________________________________________
void NicaTrackTpcToFThresholdlessCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackTpcToFThresholdlessCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackTpcToFThresholdlessCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackTpcToFThresholdlessCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackTpcToFThresholdlessCut(void *p) {
      return  p ? new(p) ::NicaTrackTpcToFThresholdlessCut : new ::NicaTrackTpcToFThresholdlessCut;
   }
   static void *newArray_NicaTrackTpcToFThresholdlessCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackTpcToFThresholdlessCut[nElements] : new ::NicaTrackTpcToFThresholdlessCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackTpcToFThresholdlessCut(void *p) {
      delete ((::NicaTrackTpcToFThresholdlessCut*)p);
   }
   static void deleteArray_NicaTrackTpcToFThresholdlessCut(void *p) {
      delete [] ((::NicaTrackTpcToFThresholdlessCut*)p);
   }
   static void destruct_NicaTrackTpcToFThresholdlessCut(void *p) {
      typedef ::NicaTrackTpcToFThresholdlessCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackTpcToFThresholdlessCut

//______________________________________________________________________________
void NicaTrackChi2Cut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackChi2Cut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackChi2Cut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackChi2Cut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackChi2Cut(void *p) {
      return  p ? new(p) ::NicaTrackChi2Cut : new ::NicaTrackChi2Cut;
   }
   static void *newArray_NicaTrackChi2Cut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackChi2Cut[nElements] : new ::NicaTrackChi2Cut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackChi2Cut(void *p) {
      delete ((::NicaTrackChi2Cut*)p);
   }
   static void deleteArray_NicaTrackChi2Cut(void *p) {
      delete [] ((::NicaTrackChi2Cut*)p);
   }
   static void destruct_NicaTrackChi2Cut(void *p) {
      typedef ::NicaTrackChi2Cut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackChi2Cut

//______________________________________________________________________________
void NicaTrackBetaCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackBetaCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackBetaCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackBetaCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackBetaCut(void *p) {
      return  p ? new(p) ::NicaTrackBetaCut : new ::NicaTrackBetaCut;
   }
   static void *newArray_NicaTrackBetaCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackBetaCut[nElements] : new ::NicaTrackBetaCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackBetaCut(void *p) {
      delete ((::NicaTrackBetaCut*)p);
   }
   static void deleteArray_NicaTrackBetaCut(void *p) {
      delete [] ((::NicaTrackBetaCut*)p);
   }
   static void destruct_NicaTrackBetaCut(void *p) {
      typedef ::NicaTrackBetaCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackBetaCut

//______________________________________________________________________________
void NicaTrackEtaCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackEtaCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackEtaCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackEtaCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackEtaCut(void *p) {
      return  p ? new(p) ::NicaTrackEtaCut : new ::NicaTrackEtaCut;
   }
   static void *newArray_NicaTrackEtaCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackEtaCut[nElements] : new ::NicaTrackEtaCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackEtaCut(void *p) {
      delete ((::NicaTrackEtaCut*)p);
   }
   static void deleteArray_NicaTrackEtaCut(void *p) {
      delete [] ((::NicaTrackEtaCut*)p);
   }
   static void destruct_NicaTrackEtaCut(void *p) {
      typedef ::NicaTrackEtaCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackEtaCut

//______________________________________________________________________________
void NicaTrackEtaAbsCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackEtaAbsCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackEtaAbsCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackEtaAbsCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackEtaAbsCut(void *p) {
      return  p ? new(p) ::NicaTrackEtaAbsCut : new ::NicaTrackEtaAbsCut;
   }
   static void *newArray_NicaTrackEtaAbsCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackEtaAbsCut[nElements] : new ::NicaTrackEtaAbsCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackEtaAbsCut(void *p) {
      delete ((::NicaTrackEtaAbsCut*)p);
   }
   static void deleteArray_NicaTrackEtaAbsCut(void *p) {
      delete [] ((::NicaTrackEtaAbsCut*)p);
   }
   static void destruct_NicaTrackEtaAbsCut(void *p) {
      typedef ::NicaTrackEtaAbsCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackEtaAbsCut

//______________________________________________________________________________
void NicaTrackMassCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackMassCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackMassCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackMassCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackMassCut(void *p) {
      return  p ? new(p) ::NicaTrackMassCut : new ::NicaTrackMassCut;
   }
   static void *newArray_NicaTrackMassCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackMassCut[nElements] : new ::NicaTrackMassCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackMassCut(void *p) {
      delete ((::NicaTrackMassCut*)p);
   }
   static void deleteArray_NicaTrackMassCut(void *p) {
      delete [] ((::NicaTrackMassCut*)p);
   }
   static void destruct_NicaTrackMassCut(void *p) {
      typedef ::NicaTrackMassCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackMassCut

//______________________________________________________________________________
void NicaTrackMtCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackMtCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackMtCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackMtCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackMtCut(void *p) {
      return  p ? new(p) ::NicaTrackMtCut : new ::NicaTrackMtCut;
   }
   static void *newArray_NicaTrackMtCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackMtCut[nElements] : new ::NicaTrackMtCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackMtCut(void *p) {
      delete ((::NicaTrackMtCut*)p);
   }
   static void deleteArray_NicaTrackMtCut(void *p) {
      delete [] ((::NicaTrackMtCut*)p);
   }
   static void destruct_NicaTrackMtCut(void *p) {
      typedef ::NicaTrackMtCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackMtCut

//______________________________________________________________________________
void NicaTrackPCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackPCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackPCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackPCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackPCut(void *p) {
      return  p ? new(p) ::NicaTrackPCut : new ::NicaTrackPCut;
   }
   static void *newArray_NicaTrackPCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackPCut[nElements] : new ::NicaTrackPCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackPCut(void *p) {
      delete ((::NicaTrackPCut*)p);
   }
   static void deleteArray_NicaTrackPCut(void *p) {
      delete [] ((::NicaTrackPCut*)p);
   }
   static void destruct_NicaTrackPCut(void *p) {
      typedef ::NicaTrackPCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackPCut

//______________________________________________________________________________
void NicaTrackPSignedCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackPSignedCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackPSignedCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackPSignedCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackPSignedCut(void *p) {
      return  p ? new(p) ::NicaTrackPSignedCut : new ::NicaTrackPSignedCut;
   }
   static void *newArray_NicaTrackPSignedCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackPSignedCut[nElements] : new ::NicaTrackPSignedCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackPSignedCut(void *p) {
      delete ((::NicaTrackPSignedCut*)p);
   }
   static void deleteArray_NicaTrackPSignedCut(void *p) {
      delete [] ((::NicaTrackPSignedCut*)p);
   }
   static void destruct_NicaTrackPSignedCut(void *p) {
      typedef ::NicaTrackPSignedCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackPSignedCut

//______________________________________________________________________________
void NicaTrackPhiCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackPhiCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackPhiCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackPhiCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackPhiCut(void *p) {
      return  p ? new(p) ::NicaTrackPhiCut : new ::NicaTrackPhiCut;
   }
   static void *newArray_NicaTrackPhiCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackPhiCut[nElements] : new ::NicaTrackPhiCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackPhiCut(void *p) {
      delete ((::NicaTrackPhiCut*)p);
   }
   static void deleteArray_NicaTrackPhiCut(void *p) {
      delete [] ((::NicaTrackPhiCut*)p);
   }
   static void destruct_NicaTrackPhiCut(void *p) {
      typedef ::NicaTrackPhiCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackPhiCut

//______________________________________________________________________________
void NicaTrackPtCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackPtCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackPtCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackPtCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackPtCut(void *p) {
      return  p ? new(p) ::NicaTrackPtCut : new ::NicaTrackPtCut;
   }
   static void *newArray_NicaTrackPtCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackPtCut[nElements] : new ::NicaTrackPtCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackPtCut(void *p) {
      delete ((::NicaTrackPtCut*)p);
   }
   static void deleteArray_NicaTrackPtCut(void *p) {
      delete [] ((::NicaTrackPtCut*)p);
   }
   static void destruct_NicaTrackPtCut(void *p) {
      typedef ::NicaTrackPtCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackPtCut

//______________________________________________________________________________
void NicaTrackYCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackYCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackYCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackYCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackYCut(void *p) {
      return  p ? new(p) ::NicaTrackYCut : new ::NicaTrackYCut;
   }
   static void *newArray_NicaTrackYCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackYCut[nElements] : new ::NicaTrackYCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackYCut(void *p) {
      delete ((::NicaTrackYCut*)p);
   }
   static void deleteArray_NicaTrackYCut(void *p) {
      delete [] ((::NicaTrackYCut*)p);
   }
   static void destruct_NicaTrackYCut(void *p) {
      typedef ::NicaTrackYCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackYCut

//______________________________________________________________________________
void NicaTrackPzCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackPzCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackPzCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackPzCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackPzCut(void *p) {
      return  p ? new(p) ::NicaTrackPzCut : new ::NicaTrackPzCut;
   }
   static void *newArray_NicaTrackPzCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackPzCut[nElements] : new ::NicaTrackPzCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackPzCut(void *p) {
      delete ((::NicaTrackPzCut*)p);
   }
   static void deleteArray_NicaTrackPzCut(void *p) {
      delete [] ((::NicaTrackPzCut*)p);
   }
   static void destruct_NicaTrackPzCut(void *p) {
      typedef ::NicaTrackPzCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackPzCut

//______________________________________________________________________________
void NicaTrackChargeCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackChargeCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackChargeCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackChargeCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackChargeCut(void *p) {
      return  p ? new(p) ::NicaTrackChargeCut : new ::NicaTrackChargeCut;
   }
   static void *newArray_NicaTrackChargeCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackChargeCut[nElements] : new ::NicaTrackChargeCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackChargeCut(void *p) {
      delete ((::NicaTrackChargeCut*)p);
   }
   static void deleteArray_NicaTrackChargeCut(void *p) {
      delete [] ((::NicaTrackChargeCut*)p);
   }
   static void destruct_NicaTrackChargeCut(void *p) {
      typedef ::NicaTrackChargeCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackChargeCut

//______________________________________________________________________________
void NicaTrackVirtualCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackVirtualCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackVirtualCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackVirtualCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackVirtualCut(void *p) {
      return  p ? new(p) ::NicaTrackVirtualCut : new ::NicaTrackVirtualCut;
   }
   static void *newArray_NicaTrackVirtualCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackVirtualCut[nElements] : new ::NicaTrackVirtualCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackVirtualCut(void *p) {
      delete ((::NicaTrackVirtualCut*)p);
   }
   static void deleteArray_NicaTrackVirtualCut(void *p) {
      delete [] ((::NicaTrackVirtualCut*)p);
   }
   static void destruct_NicaTrackVirtualCut(void *p) {
      typedef ::NicaTrackVirtualCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackVirtualCut

//______________________________________________________________________________
void NicaTrackComplexCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackComplexCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackComplexCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackComplexCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackComplexCut(void *p) {
      return  p ? new(p) ::NicaTrackComplexCut : new ::NicaTrackComplexCut;
   }
   static void *newArray_NicaTrackComplexCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackComplexCut[nElements] : new ::NicaTrackComplexCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackComplexCut(void *p) {
      delete ((::NicaTrackComplexCut*)p);
   }
   static void deleteArray_NicaTrackComplexCut(void *p) {
      delete [] ((::NicaTrackComplexCut*)p);
   }
   static void destruct_NicaTrackComplexCut(void *p) {
      typedef ::NicaTrackComplexCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackComplexCut

//______________________________________________________________________________
void NicaTrackRealCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackRealCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackRealCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackRealCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackRealCut(void *p) {
      return  p ? new(p) ::NicaTrackRealCut : new ::NicaTrackRealCut;
   }
   static void *newArray_NicaTrackRealCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackRealCut[nElements] : new ::NicaTrackRealCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackRealCut(void *p) {
      delete ((::NicaTrackRealCut*)p);
   }
   static void deleteArray_NicaTrackRealCut(void *p) {
      delete [] ((::NicaTrackRealCut*)p);
   }
   static void destruct_NicaTrackRealCut(void *p) {
      typedef ::NicaTrackRealCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackRealCut

//______________________________________________________________________________
void NicaTrackImaginaryCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackImaginaryCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackImaginaryCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackImaginaryCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackImaginaryCut(void *p) {
      return  p ? new(p) ::NicaTrackImaginaryCut : new ::NicaTrackImaginaryCut;
   }
   static void *newArray_NicaTrackImaginaryCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackImaginaryCut[nElements] : new ::NicaTrackImaginaryCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackImaginaryCut(void *p) {
      delete ((::NicaTrackImaginaryCut*)p);
   }
   static void deleteArray_NicaTrackImaginaryCut(void *p) {
      delete [] ((::NicaTrackImaginaryCut*)p);
   }
   static void destruct_NicaTrackImaginaryCut(void *p) {
      typedef ::NicaTrackImaginaryCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackImaginaryCut

//______________________________________________________________________________
void NicaTrackBoostedPCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackBoostedPCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackBoostedPCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackBoostedPCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackBoostedPCut(void *p) {
      return  p ? new(p) ::NicaTrackBoostedPCut : new ::NicaTrackBoostedPCut;
   }
   static void *newArray_NicaTrackBoostedPCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackBoostedPCut[nElements] : new ::NicaTrackBoostedPCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackBoostedPCut(void *p) {
      delete ((::NicaTrackBoostedPCut*)p);
   }
   static void deleteArray_NicaTrackBoostedPCut(void *p) {
      delete [] ((::NicaTrackBoostedPCut*)p);
   }
   static void destruct_NicaTrackBoostedPCut(void *p) {
      typedef ::NicaTrackBoostedPCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackBoostedPCut

//______________________________________________________________________________
void NicaTrackBoostedKinematcisCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackBoostedKinematcisCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackBoostedKinematcisCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackBoostedKinematcisCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackBoostedKinematcisCut(void *p) {
      return  p ? new(p) ::NicaTrackBoostedKinematcisCut : new ::NicaTrackBoostedKinematcisCut;
   }
   static void *newArray_NicaTrackBoostedKinematcisCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackBoostedKinematcisCut[nElements] : new ::NicaTrackBoostedKinematcisCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackBoostedKinematcisCut(void *p) {
      delete ((::NicaTrackBoostedKinematcisCut*)p);
   }
   static void deleteArray_NicaTrackBoostedKinematcisCut(void *p) {
      delete [] ((::NicaTrackBoostedKinematcisCut*)p);
   }
   static void destruct_NicaTrackBoostedKinematcisCut(void *p) {
      typedef ::NicaTrackBoostedKinematcisCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackBoostedKinematcisCut

//______________________________________________________________________________
void NicaTrackBoostedDeltaPCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackBoostedDeltaPCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackBoostedDeltaPCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackBoostedDeltaPCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackBoostedDeltaPCut(void *p) {
      return  p ? new(p) ::NicaTrackBoostedDeltaPCut : new ::NicaTrackBoostedDeltaPCut;
   }
   static void *newArray_NicaTrackBoostedDeltaPCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackBoostedDeltaPCut[nElements] : new ::NicaTrackBoostedDeltaPCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackBoostedDeltaPCut(void *p) {
      delete ((::NicaTrackBoostedDeltaPCut*)p);
   }
   static void deleteArray_NicaTrackBoostedDeltaPCut(void *p) {
      delete [] ((::NicaTrackBoostedDeltaPCut*)p);
   }
   static void destruct_NicaTrackBoostedDeltaPCut(void *p) {
      typedef ::NicaTrackBoostedDeltaPCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackBoostedDeltaPCut

//______________________________________________________________________________
void NicaTrackMCCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackMCCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackMCCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackMCCut::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaTrackMCCut(void *p) {
      delete ((::NicaTrackMCCut*)p);
   }
   static void deleteArray_NicaTrackMCCut(void *p) {
      delete [] ((::NicaTrackMCCut*)p);
   }
   static void destruct_NicaTrackMCCut(void *p) {
      typedef ::NicaTrackMCCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackMCCut

//______________________________________________________________________________
void NicaTrackBasicMCCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackBasicMCCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackBasicMCCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackBasicMCCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackBasicMCCut(void *p) {
      return  p ? new(p) ::NicaTrackBasicMCCut : new ::NicaTrackBasicMCCut;
   }
   static void *newArray_NicaTrackBasicMCCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackBasicMCCut[nElements] : new ::NicaTrackBasicMCCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackBasicMCCut(void *p) {
      delete ((::NicaTrackBasicMCCut*)p);
   }
   static void deleteArray_NicaTrackBasicMCCut(void *p) {
      delete [] ((::NicaTrackBasicMCCut*)p);
   }
   static void destruct_NicaTrackBasicMCCut(void *p) {
      typedef ::NicaTrackBasicMCCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackBasicMCCut

//______________________________________________________________________________
void NicaTrackFreezoutCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackFreezoutCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackFreezoutCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackFreezoutCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackFreezoutCut(void *p) {
      return  p ? new(p) ::NicaTrackFreezoutCut : new ::NicaTrackFreezoutCut;
   }
   static void *newArray_NicaTrackFreezoutCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackFreezoutCut[nElements] : new ::NicaTrackFreezoutCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackFreezoutCut(void *p) {
      delete ((::NicaTrackFreezoutCut*)p);
   }
   static void deleteArray_NicaTrackFreezoutCut(void *p) {
      delete [] ((::NicaTrackFreezoutCut*)p);
   }
   static void destruct_NicaTrackFreezoutCut(void *p) {
      typedef ::NicaTrackFreezoutCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackFreezoutCut

//______________________________________________________________________________
void NicaTrackTFreezCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackTFreezCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackTFreezCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackTFreezCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackTFreezCut(void *p) {
      return  p ? new(p) ::NicaTrackTFreezCut : new ::NicaTrackTFreezCut;
   }
   static void *newArray_NicaTrackTFreezCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackTFreezCut[nElements] : new ::NicaTrackTFreezCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackTFreezCut(void *p) {
      delete ((::NicaTrackTFreezCut*)p);
   }
   static void deleteArray_NicaTrackTFreezCut(void *p) {
      delete [] ((::NicaTrackTFreezCut*)p);
   }
   static void destruct_NicaTrackTFreezCut(void *p) {
      typedef ::NicaTrackTFreezCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackTFreezCut

//______________________________________________________________________________
void NicaTrackTauCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackTauCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackTauCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackTauCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackTauCut(void *p) {
      return  p ? new(p) ::NicaTrackTauCut : new ::NicaTrackTauCut;
   }
   static void *newArray_NicaTrackTauCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackTauCut[nElements] : new ::NicaTrackTauCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackTauCut(void *p) {
      delete ((::NicaTrackTauCut*)p);
   }
   static void deleteArray_NicaTrackTauCut(void *p) {
      delete [] ((::NicaTrackTauCut*)p);
   }
   static void destruct_NicaTrackTauCut(void *p) {
      typedef ::NicaTrackTauCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackTauCut

//______________________________________________________________________________
void NicaTrackPdgCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackPdgCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackPdgCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackPdgCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackPdgCut(void *p) {
      return  p ? new(p) ::NicaTrackPdgCut : new ::NicaTrackPdgCut;
   }
   static void *newArray_NicaTrackPdgCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackPdgCut[nElements] : new ::NicaTrackPdgCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackPdgCut(void *p) {
      delete ((::NicaTrackPdgCut*)p);
   }
   static void deleteArray_NicaTrackPdgCut(void *p) {
      delete [] ((::NicaTrackPdgCut*)p);
   }
   static void destruct_NicaTrackPdgCut(void *p) {
      typedef ::NicaTrackPdgCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackPdgCut

//______________________________________________________________________________
void NicaTrackPdgAbsCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackPdgAbsCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackPdgAbsCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackPdgAbsCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackPdgAbsCut(void *p) {
      return  p ? new(p) ::NicaTrackPdgAbsCut : new ::NicaTrackPdgAbsCut;
   }
   static void *newArray_NicaTrackPdgAbsCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackPdgAbsCut[nElements] : new ::NicaTrackPdgAbsCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackPdgAbsCut(void *p) {
      delete ((::NicaTrackPdgAbsCut*)p);
   }
   static void deleteArray_NicaTrackPdgAbsCut(void *p) {
      delete [] ((::NicaTrackPdgAbsCut*)p);
   }
   static void destruct_NicaTrackPdgAbsCut(void *p) {
      typedef ::NicaTrackPdgAbsCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackPdgAbsCut

//______________________________________________________________________________
void NicaTrackStartPositionCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackStartPositionCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackStartPositionCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackStartPositionCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackStartPositionCut(void *p) {
      return  p ? new(p) ::NicaTrackStartPositionCut : new ::NicaTrackStartPositionCut;
   }
   static void *newArray_NicaTrackStartPositionCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackStartPositionCut[nElements] : new ::NicaTrackStartPositionCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackStartPositionCut(void *p) {
      delete ((::NicaTrackStartPositionCut*)p);
   }
   static void deleteArray_NicaTrackStartPositionCut(void *p) {
      delete [] ((::NicaTrackStartPositionCut*)p);
   }
   static void destruct_NicaTrackStartPositionCut(void *p) {
      typedef ::NicaTrackStartPositionCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackStartPositionCut

//______________________________________________________________________________
void NicaTrackPdgBinCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackPdgBinCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackPdgBinCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackPdgBinCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackPdgBinCut(void *p) {
      return  p ? new(p) ::NicaTrackPdgBinCut : new ::NicaTrackPdgBinCut;
   }
   static void *newArray_NicaTrackPdgBinCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackPdgBinCut[nElements] : new ::NicaTrackPdgBinCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackPdgBinCut(void *p) {
      delete ((::NicaTrackPdgBinCut*)p);
   }
   static void deleteArray_NicaTrackPdgBinCut(void *p) {
      delete [] ((::NicaTrackPdgBinCut*)p);
   }
   static void destruct_NicaTrackPdgBinCut(void *p) {
      typedef ::NicaTrackPdgBinCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackPdgBinCut

//______________________________________________________________________________
void NicaTrackDeltaMomentumCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackDeltaMomentumCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackDeltaMomentumCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackDeltaMomentumCut::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaTrackDeltaMomentumCut(void *p) {
      delete ((::NicaTrackDeltaMomentumCut*)p);
   }
   static void deleteArray_NicaTrackDeltaMomentumCut(void *p) {
      delete [] ((::NicaTrackDeltaMomentumCut*)p);
   }
   static void destruct_NicaTrackDeltaMomentumCut(void *p) {
      typedef ::NicaTrackDeltaMomentumCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackDeltaMomentumCut

//______________________________________________________________________________
void NicaTrackDeltaMomCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackDeltaMomCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackDeltaMomCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackDeltaMomCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackDeltaMomCut(void *p) {
      return  p ? new(p) ::NicaTrackDeltaMomCut : new ::NicaTrackDeltaMomCut;
   }
   static void *newArray_NicaTrackDeltaMomCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackDeltaMomCut[nElements] : new ::NicaTrackDeltaMomCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackDeltaMomCut(void *p) {
      delete ((::NicaTrackDeltaMomCut*)p);
   }
   static void deleteArray_NicaTrackDeltaMomCut(void *p) {
      delete [] ((::NicaTrackDeltaMomCut*)p);
   }
   static void destruct_NicaTrackDeltaMomCut(void *p) {
      typedef ::NicaTrackDeltaMomCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackDeltaMomCut

//______________________________________________________________________________
void NicaTrackDeltaVectMomCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackDeltaVectMomCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackDeltaVectMomCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackDeltaVectMomCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackDeltaVectMomCut(void *p) {
      return  p ? new(p) ::NicaTrackDeltaVectMomCut : new ::NicaTrackDeltaVectMomCut;
   }
   static void *newArray_NicaTrackDeltaVectMomCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackDeltaVectMomCut[nElements] : new ::NicaTrackDeltaVectMomCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackDeltaVectMomCut(void *p) {
      delete ((::NicaTrackDeltaVectMomCut*)p);
   }
   static void deleteArray_NicaTrackDeltaVectMomCut(void *p) {
      delete [] ((::NicaTrackDeltaVectMomCut*)p);
   }
   static void destruct_NicaTrackDeltaVectMomCut(void *p) {
      typedef ::NicaTrackDeltaVectMomCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackDeltaVectMomCut

//______________________________________________________________________________
void NicaTrackDeltaPtCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackDeltaPtCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackDeltaPtCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackDeltaPtCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackDeltaPtCut(void *p) {
      return  p ? new(p) ::NicaTrackDeltaPtCut : new ::NicaTrackDeltaPtCut;
   }
   static void *newArray_NicaTrackDeltaPtCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackDeltaPtCut[nElements] : new ::NicaTrackDeltaPtCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackDeltaPtCut(void *p) {
      delete ((::NicaTrackDeltaPtCut*)p);
   }
   static void deleteArray_NicaTrackDeltaPtCut(void *p) {
      delete [] ((::NicaTrackDeltaPtCut*)p);
   }
   static void destruct_NicaTrackDeltaPtCut(void *p) {
      typedef ::NicaTrackDeltaPtCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackDeltaPtCut

//______________________________________________________________________________
void NicaTrackDeltaPCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackDeltaPCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackDeltaPCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackDeltaPCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackDeltaPCut(void *p) {
      return  p ? new(p) ::NicaTrackDeltaPCut : new ::NicaTrackDeltaPCut;
   }
   static void *newArray_NicaTrackDeltaPCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackDeltaPCut[nElements] : new ::NicaTrackDeltaPCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackDeltaPCut(void *p) {
      delete ((::NicaTrackDeltaPCut*)p);
   }
   static void deleteArray_NicaTrackDeltaPCut(void *p) {
      delete [] ((::NicaTrackDeltaPCut*)p);
   }
   static void destruct_NicaTrackDeltaPCut(void *p) {
      typedef ::NicaTrackDeltaPCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackDeltaPCut

//______________________________________________________________________________
void NicaTrackDeltaPzCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackDeltaPzCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackDeltaPzCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackDeltaPzCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackDeltaPzCut(void *p) {
      return  p ? new(p) ::NicaTrackDeltaPzCut : new ::NicaTrackDeltaPzCut;
   }
   static void *newArray_NicaTrackDeltaPzCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackDeltaPzCut[nElements] : new ::NicaTrackDeltaPzCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackDeltaPzCut(void *p) {
      delete ((::NicaTrackDeltaPzCut*)p);
   }
   static void deleteArray_NicaTrackDeltaPzCut(void *p) {
      delete [] ((::NicaTrackDeltaPzCut*)p);
   }
   static void destruct_NicaTrackDeltaPzCut(void *p) {
      typedef ::NicaTrackDeltaPzCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackDeltaPzCut

//______________________________________________________________________________
void NicaTrackDeltaPhiCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackDeltaPhiCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackDeltaPhiCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackDeltaPhiCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackDeltaPhiCut(void *p) {
      return  p ? new(p) ::NicaTrackDeltaPhiCut : new ::NicaTrackDeltaPhiCut;
   }
   static void *newArray_NicaTrackDeltaPhiCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackDeltaPhiCut[nElements] : new ::NicaTrackDeltaPhiCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackDeltaPhiCut(void *p) {
      delete ((::NicaTrackDeltaPhiCut*)p);
   }
   static void deleteArray_NicaTrackDeltaPhiCut(void *p) {
      delete [] ((::NicaTrackDeltaPhiCut*)p);
   }
   static void destruct_NicaTrackDeltaPhiCut(void *p) {
      typedef ::NicaTrackDeltaPhiCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackDeltaPhiCut

//______________________________________________________________________________
void NicaTrackDeltaThetaCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackDeltaThetaCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackDeltaThetaCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackDeltaThetaCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackDeltaThetaCut(void *p) {
      return  p ? new(p) ::NicaTrackDeltaThetaCut : new ::NicaTrackDeltaThetaCut;
   }
   static void *newArray_NicaTrackDeltaThetaCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackDeltaThetaCut[nElements] : new ::NicaTrackDeltaThetaCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackDeltaThetaCut(void *p) {
      delete ((::NicaTrackDeltaThetaCut*)p);
   }
   static void deleteArray_NicaTrackDeltaThetaCut(void *p) {
      delete [] ((::NicaTrackDeltaThetaCut*)p);
   }
   static void destruct_NicaTrackDeltaThetaCut(void *p) {
      typedef ::NicaTrackDeltaThetaCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackDeltaThetaCut

//______________________________________________________________________________
void NicaTrackDeltaVectPtCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackDeltaVectPtCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackDeltaVectPtCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackDeltaVectPtCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackDeltaVectPtCut(void *p) {
      return  p ? new(p) ::NicaTrackDeltaVectPtCut : new ::NicaTrackDeltaVectPtCut;
   }
   static void *newArray_NicaTrackDeltaVectPtCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackDeltaVectPtCut[nElements] : new ::NicaTrackDeltaVectPtCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackDeltaVectPtCut(void *p) {
      delete ((::NicaTrackDeltaVectPtCut*)p);
   }
   static void deleteArray_NicaTrackDeltaVectPtCut(void *p) {
      delete [] ((::NicaTrackDeltaVectPtCut*)p);
   }
   static void destruct_NicaTrackDeltaVectPtCut(void *p) {
      typedef ::NicaTrackDeltaVectPtCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackDeltaVectPtCut

//______________________________________________________________________________
void NicaTrackDeltaVectPCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackDeltaVectPCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackDeltaVectPCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackDeltaVectPCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackDeltaVectPCut(void *p) {
      return  p ? new(p) ::NicaTrackDeltaVectPCut : new ::NicaTrackDeltaVectPCut;
   }
   static void *newArray_NicaTrackDeltaVectPCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackDeltaVectPCut[nElements] : new ::NicaTrackDeltaVectPCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackDeltaVectPCut(void *p) {
      delete ((::NicaTrackDeltaVectPCut*)p);
   }
   static void deleteArray_NicaTrackDeltaVectPCut(void *p) {
      delete [] ((::NicaTrackDeltaVectPCut*)p);
   }
   static void destruct_NicaTrackDeltaVectPCut(void *p) {
      typedef ::NicaTrackDeltaVectPCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackDeltaVectPCut

//______________________________________________________________________________
void NicaTrackDeltaVectPhiCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackDeltaVectPhiCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackDeltaVectPhiCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackDeltaVectPhiCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackDeltaVectPhiCut(void *p) {
      return  p ? new(p) ::NicaTrackDeltaVectPhiCut : new ::NicaTrackDeltaVectPhiCut;
   }
   static void *newArray_NicaTrackDeltaVectPhiCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackDeltaVectPhiCut[nElements] : new ::NicaTrackDeltaVectPhiCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackDeltaVectPhiCut(void *p) {
      delete ((::NicaTrackDeltaVectPhiCut*)p);
   }
   static void deleteArray_NicaTrackDeltaVectPhiCut(void *p) {
      delete [] ((::NicaTrackDeltaVectPhiCut*)p);
   }
   static void destruct_NicaTrackDeltaVectPhiCut(void *p) {
      typedef ::NicaTrackDeltaVectPhiCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackDeltaVectPhiCut

//______________________________________________________________________________
void NicaTrackDeltaVectThetaCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackDeltaVectThetaCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackDeltaVectThetaCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackDeltaVectThetaCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackDeltaVectThetaCut(void *p) {
      return  p ? new(p) ::NicaTrackDeltaVectThetaCut : new ::NicaTrackDeltaVectThetaCut;
   }
   static void *newArray_NicaTrackDeltaVectThetaCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackDeltaVectThetaCut[nElements] : new ::NicaTrackDeltaVectThetaCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackDeltaVectThetaCut(void *p) {
      delete ((::NicaTrackDeltaVectThetaCut*)p);
   }
   static void deleteArray_NicaTrackDeltaVectThetaCut(void *p) {
      delete [] ((::NicaTrackDeltaVectThetaCut*)p);
   }
   static void destruct_NicaTrackDeltaVectThetaCut(void *p) {
      typedef ::NicaTrackDeltaVectThetaCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackDeltaVectThetaCut

//______________________________________________________________________________
void NicaTrackOnlyPrimariesCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackOnlyPrimariesCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackOnlyPrimariesCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackOnlyPrimariesCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackOnlyPrimariesCut(void *p) {
      return  p ? new(p) ::NicaTrackOnlyPrimariesCut : new ::NicaTrackOnlyPrimariesCut;
   }
   static void *newArray_NicaTrackOnlyPrimariesCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackOnlyPrimariesCut[nElements] : new ::NicaTrackOnlyPrimariesCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackOnlyPrimariesCut(void *p) {
      delete ((::NicaTrackOnlyPrimariesCut*)p);
   }
   static void deleteArray_NicaTrackOnlyPrimariesCut(void *p) {
      delete [] ((::NicaTrackOnlyPrimariesCut*)p);
   }
   static void destruct_NicaTrackOnlyPrimariesCut(void *p) {
      typedef ::NicaTrackOnlyPrimariesCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackOnlyPrimariesCut

//______________________________________________________________________________
void NicaTrackStatusCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackStatusCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackStatusCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackStatusCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackStatusCut(void *p) {
      return  p ? new(p) ::NicaTrackStatusCut : new ::NicaTrackStatusCut;
   }
   static void *newArray_NicaTrackStatusCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackStatusCut[nElements] : new ::NicaTrackStatusCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackStatusCut(void *p) {
      delete ((::NicaTrackStatusCut*)p);
   }
   static void deleteArray_NicaTrackStatusCut(void *p) {
      delete [] ((::NicaTrackStatusCut*)p);
   }
   static void destruct_NicaTrackStatusCut(void *p) {
      typedef ::NicaTrackStatusCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackStatusCut

//______________________________________________________________________________
void NicaTrackPrimaryFlagCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackPrimaryFlagCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackPrimaryFlagCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackPrimaryFlagCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackPrimaryFlagCut(void *p) {
      return  p ? new(p) ::NicaTrackPrimaryFlagCut : new ::NicaTrackPrimaryFlagCut;
   }
   static void *newArray_NicaTrackPrimaryFlagCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackPrimaryFlagCut[nElements] : new ::NicaTrackPrimaryFlagCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackPrimaryFlagCut(void *p) {
      delete ((::NicaTrackPrimaryFlagCut*)p);
   }
   static void deleteArray_NicaTrackPrimaryFlagCut(void *p) {
      delete [] ((::NicaTrackPrimaryFlagCut*)p);
   }
   static void destruct_NicaTrackPrimaryFlagCut(void *p) {
      typedef ::NicaTrackPrimaryFlagCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackPrimaryFlagCut

//______________________________________________________________________________
void NicaTrackV0FlagCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackV0FlagCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackV0FlagCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackV0FlagCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackV0FlagCut(void *p) {
      return  p ? new(p) ::NicaTrackV0FlagCut : new ::NicaTrackV0FlagCut;
   }
   static void *newArray_NicaTrackV0FlagCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackV0FlagCut[nElements] : new ::NicaTrackV0FlagCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackV0FlagCut(void *p) {
      delete ((::NicaTrackV0FlagCut*)p);
   }
   static void deleteArray_NicaTrackV0FlagCut(void *p) {
      delete [] ((::NicaTrackV0FlagCut*)p);
   }
   static void destruct_NicaTrackV0FlagCut(void *p) {
      typedef ::NicaTrackV0FlagCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackV0FlagCut

//______________________________________________________________________________
void NicaTrackGoodSecondaryFlagCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackGoodSecondaryFlagCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackGoodSecondaryFlagCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackGoodSecondaryFlagCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackGoodSecondaryFlagCut(void *p) {
      return  p ? new(p) ::NicaTrackGoodSecondaryFlagCut : new ::NicaTrackGoodSecondaryFlagCut;
   }
   static void *newArray_NicaTrackGoodSecondaryFlagCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackGoodSecondaryFlagCut[nElements] : new ::NicaTrackGoodSecondaryFlagCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackGoodSecondaryFlagCut(void *p) {
      delete ((::NicaTrackGoodSecondaryFlagCut*)p);
   }
   static void deleteArray_NicaTrackGoodSecondaryFlagCut(void *p) {
      delete [] ((::NicaTrackGoodSecondaryFlagCut*)p);
   }
   static void destruct_NicaTrackGoodSecondaryFlagCut(void *p) {
      typedef ::NicaTrackGoodSecondaryFlagCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackGoodSecondaryFlagCut

//______________________________________________________________________________
void NicaTrackGlobalFlagCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackGlobalFlagCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackGlobalFlagCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackGlobalFlagCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackGlobalFlagCut(void *p) {
      return  p ? new(p) ::NicaTrackGlobalFlagCut : new ::NicaTrackGlobalFlagCut;
   }
   static void *newArray_NicaTrackGlobalFlagCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackGlobalFlagCut[nElements] : new ::NicaTrackGlobalFlagCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackGlobalFlagCut(void *p) {
      delete ((::NicaTrackGlobalFlagCut*)p);
   }
   static void deleteArray_NicaTrackGlobalFlagCut(void *p) {
      delete [] ((::NicaTrackGlobalFlagCut*)p);
   }
   static void destruct_NicaTrackGlobalFlagCut(void *p) {
      typedef ::NicaTrackGlobalFlagCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackGlobalFlagCut

//______________________________________________________________________________
void NicaTwoTrackEtaCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackEtaCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackEtaCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackEtaCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackEtaCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackEtaCut : new ::NicaTwoTrackEtaCut;
   }
   static void *newArray_NicaTwoTrackEtaCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackEtaCut[nElements] : new ::NicaTwoTrackEtaCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackEtaCut(void *p) {
      delete ((::NicaTwoTrackEtaCut*)p);
   }
   static void deleteArray_NicaTwoTrackEtaCut(void *p) {
      delete [] ((::NicaTwoTrackEtaCut*)p);
   }
   static void destruct_NicaTwoTrackEtaCut(void *p) {
      typedef ::NicaTwoTrackEtaCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackEtaCut

//______________________________________________________________________________
void NicaTwoTrackKtCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackKtCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackKtCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackKtCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackKtCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackKtCut : new ::NicaTwoTrackKtCut;
   }
   static void *newArray_NicaTwoTrackKtCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackKtCut[nElements] : new ::NicaTwoTrackKtCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackKtCut(void *p) {
      delete ((::NicaTwoTrackKtCut*)p);
   }
   static void deleteArray_NicaTwoTrackKtCut(void *p) {
      delete [] ((::NicaTwoTrackKtCut*)p);
   }
   static void destruct_NicaTwoTrackKtCut(void *p) {
      typedef ::NicaTwoTrackKtCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackKtCut

//______________________________________________________________________________
void NicaTwoTrackMinvCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackMinvCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackMinvCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackMinvCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackMinvCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackMinvCut : new ::NicaTwoTrackMinvCut;
   }
   static void *newArray_NicaTwoTrackMinvCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackMinvCut[nElements] : new ::NicaTwoTrackMinvCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackMinvCut(void *p) {
      delete ((::NicaTwoTrackMinvCut*)p);
   }
   static void deleteArray_NicaTwoTrackMinvCut(void *p) {
      delete [] ((::NicaTwoTrackMinvCut*)p);
   }
   static void destruct_NicaTwoTrackMinvCut(void *p) {
      typedef ::NicaTwoTrackMinvCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackMinvCut

//______________________________________________________________________________
void NicaTwoTrackPhiCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackPhiCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackPhiCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackPhiCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackPhiCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackPhiCut : new ::NicaTwoTrackPhiCut;
   }
   static void *newArray_NicaTwoTrackPhiCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackPhiCut[nElements] : new ::NicaTwoTrackPhiCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackPhiCut(void *p) {
      delete ((::NicaTwoTrackPhiCut*)p);
   }
   static void deleteArray_NicaTwoTrackPhiCut(void *p) {
      delete [] ((::NicaTwoTrackPhiCut*)p);
   }
   static void destruct_NicaTwoTrackPhiCut(void *p) {
      typedef ::NicaTwoTrackPhiCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackPhiCut

//______________________________________________________________________________
void NicaTwoTrackPtOrderCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackPtOrderCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackPtOrderCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackPtOrderCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackPtOrderCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackPtOrderCut : new ::NicaTwoTrackPtOrderCut;
   }
   static void *newArray_NicaTwoTrackPtOrderCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackPtOrderCut[nElements] : new ::NicaTwoTrackPtOrderCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackPtOrderCut(void *p) {
      delete ((::NicaTwoTrackPtOrderCut*)p);
   }
   static void deleteArray_NicaTwoTrackPtOrderCut(void *p) {
      delete [] ((::NicaTwoTrackPtOrderCut*)p);
   }
   static void destruct_NicaTwoTrackPtOrderCut(void *p) {
      typedef ::NicaTwoTrackPtOrderCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackPtOrderCut

//______________________________________________________________________________
void NicaTwoTrackDphiDetaCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackDphiDetaCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackDphiDetaCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackDphiDetaCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackDphiDetaCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackDphiDetaCut : new ::NicaTwoTrackDphiDetaCut;
   }
   static void *newArray_NicaTwoTrackDphiDetaCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackDphiDetaCut[nElements] : new ::NicaTwoTrackDphiDetaCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackDphiDetaCut(void *p) {
      delete ((::NicaTwoTrackDphiDetaCut*)p);
   }
   static void deleteArray_NicaTwoTrackDphiDetaCut(void *p) {
      delete [] ((::NicaTwoTrackDphiDetaCut*)p);
   }
   static void destruct_NicaTwoTrackDphiDetaCut(void *p) {
      typedef ::NicaTwoTrackDphiDetaCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackDphiDetaCut

//______________________________________________________________________________
void NicaTwoTrackDphistarDetaCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackDphistarDetaCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackDphistarDetaCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackDphistarDetaCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackDphistarDetaCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackDphistarDetaCut : new ::NicaTwoTrackDphistarDetaCut;
   }
   static void *newArray_NicaTwoTrackDphistarDetaCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackDphistarDetaCut[nElements] : new ::NicaTwoTrackDphistarDetaCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackDphistarDetaCut(void *p) {
      delete ((::NicaTwoTrackDphistarDetaCut*)p);
   }
   static void deleteArray_NicaTwoTrackDphistarDetaCut(void *p) {
      delete [] ((::NicaTwoTrackDphistarDetaCut*)p);
   }
   static void destruct_NicaTwoTrackDphistarDetaCut(void *p) {
      typedef ::NicaTwoTrackDphistarDetaCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackDphistarDetaCut

//______________________________________________________________________________
void NicaTwoTrackPtSumCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackPtSumCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackPtSumCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackPtSumCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackPtSumCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackPtSumCut : new ::NicaTwoTrackPtSumCut;
   }
   static void *newArray_NicaTwoTrackPtSumCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackPtSumCut[nElements] : new ::NicaTwoTrackPtSumCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackPtSumCut(void *p) {
      delete ((::NicaTwoTrackPtSumCut*)p);
   }
   static void deleteArray_NicaTwoTrackPtSumCut(void *p) {
      delete [] ((::NicaTwoTrackPtSumCut*)p);
   }
   static void destruct_NicaTwoTrackPtSumCut(void *p) {
      typedef ::NicaTwoTrackPtSumCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackPtSumCut

//______________________________________________________________________________
void NicaTwoTrackRapidityCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackRapidityCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackRapidityCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackRapidityCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackRapidityCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackRapidityCut : new ::NicaTwoTrackRapidityCut;
   }
   static void *newArray_NicaTwoTrackRapidityCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackRapidityCut[nElements] : new ::NicaTwoTrackRapidityCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackRapidityCut(void *p) {
      delete ((::NicaTwoTrackRapidityCut*)p);
   }
   static void deleteArray_NicaTwoTrackRapidityCut(void *p) {
      delete [] ((::NicaTwoTrackRapidityCut*)p);
   }
   static void destruct_NicaTwoTrackRapidityCut(void *p) {
      typedef ::NicaTwoTrackRapidityCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackRapidityCut

//______________________________________________________________________________
void NicaTwoTrackDeltaEtaCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackDeltaEtaCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackDeltaEtaCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackDeltaEtaCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackDeltaEtaCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackDeltaEtaCut : new ::NicaTwoTrackDeltaEtaCut;
   }
   static void *newArray_NicaTwoTrackDeltaEtaCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackDeltaEtaCut[nElements] : new ::NicaTwoTrackDeltaEtaCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackDeltaEtaCut(void *p) {
      delete ((::NicaTwoTrackDeltaEtaCut*)p);
   }
   static void deleteArray_NicaTwoTrackDeltaEtaCut(void *p) {
      delete [] ((::NicaTwoTrackDeltaEtaCut*)p);
   }
   static void destruct_NicaTwoTrackDeltaEtaCut(void *p) {
      typedef ::NicaTwoTrackDeltaEtaCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackDeltaEtaCut

//______________________________________________________________________________
void NicaPairDeltaQinvCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaPairDeltaQinvCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaPairDeltaQinvCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaPairDeltaQinvCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaPairDeltaQinvCut(void *p) {
      return  p ? new(p) ::NicaPairDeltaQinvCut : new ::NicaPairDeltaQinvCut;
   }
   static void *newArray_NicaPairDeltaQinvCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaPairDeltaQinvCut[nElements] : new ::NicaPairDeltaQinvCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaPairDeltaQinvCut(void *p) {
      delete ((::NicaPairDeltaQinvCut*)p);
   }
   static void deleteArray_NicaPairDeltaQinvCut(void *p) {
      delete [] ((::NicaPairDeltaQinvCut*)p);
   }
   static void destruct_NicaPairDeltaQinvCut(void *p) {
      typedef ::NicaPairDeltaQinvCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaPairDeltaQinvCut

//______________________________________________________________________________
void NicaTwoTrackLCMSCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackLCMSCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackLCMSCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackLCMSCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackLCMSCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackLCMSCut : new ::NicaTwoTrackLCMSCut;
   }
   static void *newArray_NicaTwoTrackLCMSCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackLCMSCut[nElements] : new ::NicaTwoTrackLCMSCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackLCMSCut(void *p) {
      delete ((::NicaTwoTrackLCMSCut*)p);
   }
   static void deleteArray_NicaTwoTrackLCMSCut(void *p) {
      delete [] ((::NicaTwoTrackLCMSCut*)p);
   }
   static void destruct_NicaTwoTrackLCMSCut(void *p) {
      typedef ::NicaTwoTrackLCMSCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackLCMSCut

//______________________________________________________________________________
void NicaTwoTrack3DCFCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrack3DCFCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrack3DCFCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrack3DCFCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrack3DCFCut(void *p) {
      return  p ? new(p) ::NicaTwoTrack3DCFCut : new ::NicaTwoTrack3DCFCut;
   }
   static void *newArray_NicaTwoTrack3DCFCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrack3DCFCut[nElements] : new ::NicaTwoTrack3DCFCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrack3DCFCut(void *p) {
      delete ((::NicaTwoTrack3DCFCut*)p);
   }
   static void deleteArray_NicaTwoTrack3DCFCut(void *p) {
      delete [] ((::NicaTwoTrack3DCFCut*)p);
   }
   static void destruct_NicaTwoTrack3DCFCut(void *p) {
      typedef ::NicaTwoTrack3DCFCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrack3DCFCut

//______________________________________________________________________________
void NicaTwoTrackComplexCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackComplexCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackComplexCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackComplexCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackComplexCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackComplexCut : new ::NicaTwoTrackComplexCut;
   }
   static void *newArray_NicaTwoTrackComplexCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackComplexCut[nElements] : new ::NicaTwoTrackComplexCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackComplexCut(void *p) {
      delete ((::NicaTwoTrackComplexCut*)p);
   }
   static void deleteArray_NicaTwoTrackComplexCut(void *p) {
      delete [] ((::NicaTwoTrackComplexCut*)p);
   }
   static void destruct_NicaTwoTrackComplexCut(void *p) {
      typedef ::NicaTwoTrackComplexCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackComplexCut

//______________________________________________________________________________
void NicaTwoTrackRealCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackRealCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackRealCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackRealCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackRealCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackRealCut : new ::NicaTwoTrackRealCut;
   }
   static void *newArray_NicaTwoTrackRealCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackRealCut[nElements] : new ::NicaTwoTrackRealCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackRealCut(void *p) {
      delete ((::NicaTwoTrackRealCut*)p);
   }
   static void deleteArray_NicaTwoTrackRealCut(void *p) {
      delete [] ((::NicaTwoTrackRealCut*)p);
   }
   static void destruct_NicaTwoTrackRealCut(void *p) {
      typedef ::NicaTwoTrackRealCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackRealCut

//______________________________________________________________________________
void NicaTwoTrackImaginaryCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackImaginaryCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackImaginaryCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackImaginaryCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackImaginaryCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackImaginaryCut : new ::NicaTwoTrackImaginaryCut;
   }
   static void *newArray_NicaTwoTrackImaginaryCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackImaginaryCut[nElements] : new ::NicaTwoTrackImaginaryCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackImaginaryCut(void *p) {
      delete ((::NicaTwoTrackImaginaryCut*)p);
   }
   static void deleteArray_NicaTwoTrackImaginaryCut(void *p) {
      delete [] ((::NicaTwoTrackImaginaryCut*)p);
   }
   static void destruct_NicaTwoTrackImaginaryCut(void *p) {
      typedef ::NicaTwoTrackImaginaryCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackImaginaryCut

//______________________________________________________________________________
void NicaTwoTrackFemtoQinvCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackFemtoQinvCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackFemtoQinvCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackFemtoQinvCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackFemtoQinvCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackFemtoQinvCut : new ::NicaTwoTrackFemtoQinvCut;
   }
   static void *newArray_NicaTwoTrackFemtoQinvCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackFemtoQinvCut[nElements] : new ::NicaTwoTrackFemtoQinvCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackFemtoQinvCut(void *p) {
      delete ((::NicaTwoTrackFemtoQinvCut*)p);
   }
   static void deleteArray_NicaTwoTrackFemtoQinvCut(void *p) {
      delete [] ((::NicaTwoTrackFemtoQinvCut*)p);
   }
   static void destruct_NicaTwoTrackFemtoQinvCut(void *p) {
      typedef ::NicaTwoTrackFemtoQinvCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackFemtoQinvCut

//______________________________________________________________________________
void NicaTwoTrackSailorCowboyCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackSailorCowboyCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackSailorCowboyCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackSailorCowboyCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackSailorCowboyCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackSailorCowboyCut : new ::NicaTwoTrackSailorCowboyCut;
   }
   static void *newArray_NicaTwoTrackSailorCowboyCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackSailorCowboyCut[nElements] : new ::NicaTwoTrackSailorCowboyCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackSailorCowboyCut(void *p) {
      delete ((::NicaTwoTrackSailorCowboyCut*)p);
   }
   static void deleteArray_NicaTwoTrackSailorCowboyCut(void *p) {
      delete [] ((::NicaTwoTrackSailorCowboyCut*)p);
   }
   static void destruct_NicaTwoTrackSailorCowboyCut(void *p) {
      typedef ::NicaTwoTrackSailorCowboyCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackSailorCowboyCut

//______________________________________________________________________________
void NicaTwoTrackRadialDistanceCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackRadialDistanceCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackRadialDistanceCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackRadialDistanceCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackRadialDistanceCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackRadialDistanceCut : new ::NicaTwoTrackRadialDistanceCut;
   }
   static void *newArray_NicaTwoTrackRadialDistanceCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackRadialDistanceCut[nElements] : new ::NicaTwoTrackRadialDistanceCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackRadialDistanceCut(void *p) {
      delete ((::NicaTwoTrackRadialDistanceCut*)p);
   }
   static void deleteArray_NicaTwoTrackRadialDistanceCut(void *p) {
      delete [] ((::NicaTwoTrackRadialDistanceCut*)p);
   }
   static void destruct_NicaTwoTrackRadialDistanceCut(void *p) {
      typedef ::NicaTwoTrackRadialDistanceCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackRadialDistanceCut

//______________________________________________________________________________
void NicaTwoTrackDebugCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackDebugCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackDebugCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackDebugCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackDebugCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackDebugCut : new ::NicaTwoTrackDebugCut;
   }
   static void *newArray_NicaTwoTrackDebugCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackDebugCut[nElements] : new ::NicaTwoTrackDebugCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackDebugCut(void *p) {
      delete ((::NicaTwoTrackDebugCut*)p);
   }
   static void deleteArray_NicaTwoTrackDebugCut(void *p) {
      delete [] ((::NicaTwoTrackDebugCut*)p);
   }
   static void destruct_NicaTwoTrackDebugCut(void *p) {
      typedef ::NicaTwoTrackDebugCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackDebugCut

//______________________________________________________________________________
void NicaTwoTrackFemtoPRFCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackFemtoPRFCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackFemtoPRFCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackFemtoPRFCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackFemtoPRFCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackFemtoPRFCut : new ::NicaTwoTrackFemtoPRFCut;
   }
   static void *newArray_NicaTwoTrackFemtoPRFCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackFemtoPRFCut[nElements] : new ::NicaTwoTrackFemtoPRFCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackFemtoPRFCut(void *p) {
      delete ((::NicaTwoTrackFemtoPRFCut*)p);
   }
   static void deleteArray_NicaTwoTrackFemtoPRFCut(void *p) {
      delete [] ((::NicaTwoTrackFemtoPRFCut*)p);
   }
   static void destruct_NicaTwoTrackFemtoPRFCut(void *p) {
      typedef ::NicaTwoTrackFemtoPRFCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackFemtoPRFCut

//______________________________________________________________________________
void NicaPdgBinPairCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaPdgBinPairCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaPdgBinPairCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaPdgBinPairCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaPdgBinPairCut(void *p) {
      return  p ? new(p) ::NicaPdgBinPairCut : new ::NicaPdgBinPairCut;
   }
   static void *newArray_NicaPdgBinPairCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaPdgBinPairCut[nElements] : new ::NicaPdgBinPairCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaPdgBinPairCut(void *p) {
      delete ((::NicaPdgBinPairCut*)p);
   }
   static void deleteArray_NicaPdgBinPairCut(void *p) {
      delete [] ((::NicaPdgBinPairCut*)p);
   }
   static void destruct_NicaPdgBinPairCut(void *p) {
      typedef ::NicaPdgBinPairCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaPdgBinPairCut

//______________________________________________________________________________
void NicaTwoTrackVirtualCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackVirtualCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackVirtualCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackVirtualCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackVirtualCut(void *p) {
      return  p ? new(p) ::NicaTwoTrackVirtualCut : new ::NicaTwoTrackVirtualCut;
   }
   static void *newArray_NicaTwoTrackVirtualCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackVirtualCut[nElements] : new ::NicaTwoTrackVirtualCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackVirtualCut(void *p) {
      delete ((::NicaTwoTrackVirtualCut*)p);
   }
   static void deleteArray_NicaTwoTrackVirtualCut(void *p) {
      delete [] ((::NicaTwoTrackVirtualCut*)p);
   }
   static void destruct_NicaTwoTrackVirtualCut(void *p) {
      typedef ::NicaTwoTrackVirtualCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackVirtualCut

//______________________________________________________________________________
void NicaCutMonitorX::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCutMonitorX.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCutMonitorX::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCutMonitorX::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCutMonitorX(void *p) {
      return  p ? new(p) ::NicaCutMonitorX : new ::NicaCutMonitorX;
   }
   static void *newArray_NicaCutMonitorX(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCutMonitorX[nElements] : new ::NicaCutMonitorX[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCutMonitorX(void *p) {
      delete ((::NicaCutMonitorX*)p);
   }
   static void deleteArray_NicaCutMonitorX(void *p) {
      delete [] ((::NicaCutMonitorX*)p);
   }
   static void destruct_NicaCutMonitorX(void *p) {
      typedef ::NicaCutMonitorX current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCutMonitorX

//______________________________________________________________________________
void NicaCutMonitorXY::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCutMonitorXY.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCutMonitorXY::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCutMonitorXY::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCutMonitorXY(void *p) {
      return  p ? new(p) ::NicaCutMonitorXY : new ::NicaCutMonitorXY;
   }
   static void *newArray_NicaCutMonitorXY(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCutMonitorXY[nElements] : new ::NicaCutMonitorXY[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCutMonitorXY(void *p) {
      delete ((::NicaCutMonitorXY*)p);
   }
   static void deleteArray_NicaCutMonitorXY(void *p) {
      delete [] ((::NicaCutMonitorXY*)p);
   }
   static void destruct_NicaCutMonitorXY(void *p) {
      typedef ::NicaCutMonitorXY current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCutMonitorXY

//______________________________________________________________________________
void NicaCutMonitorXYZ::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCutMonitorXYZ.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCutMonitorXYZ::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCutMonitorXYZ::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCutMonitorXYZ(void *p) {
      return  p ? new(p) ::NicaCutMonitorXYZ : new ::NicaCutMonitorXYZ;
   }
   static void *newArray_NicaCutMonitorXYZ(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCutMonitorXYZ[nElements] : new ::NicaCutMonitorXYZ[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCutMonitorXYZ(void *p) {
      delete ((::NicaCutMonitorXYZ*)p);
   }
   static void deleteArray_NicaCutMonitorXYZ(void *p) {
      delete [] ((::NicaCutMonitorXYZ*)p);
   }
   static void destruct_NicaCutMonitorXYZ(void *p) {
      typedef ::NicaCutMonitorXYZ current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCutMonitorXYZ

//______________________________________________________________________________
void NicaPropertyMonitorX::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaPropertyMonitorX.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaPropertyMonitorX::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaPropertyMonitorX::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaPropertyMonitorX(void *p) {
      return  p ? new(p) ::NicaPropertyMonitorX : new ::NicaPropertyMonitorX;
   }
   static void *newArray_NicaPropertyMonitorX(Long_t nElements, void *p) {
      return p ? new(p) ::NicaPropertyMonitorX[nElements] : new ::NicaPropertyMonitorX[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaPropertyMonitorX(void *p) {
      delete ((::NicaPropertyMonitorX*)p);
   }
   static void deleteArray_NicaPropertyMonitorX(void *p) {
      delete [] ((::NicaPropertyMonitorX*)p);
   }
   static void destruct_NicaPropertyMonitorX(void *p) {
      typedef ::NicaPropertyMonitorX current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaPropertyMonitorX

//______________________________________________________________________________
void NicaPropertyMonitorXY::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaPropertyMonitorXY.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaPropertyMonitorXY::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaPropertyMonitorXY::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaPropertyMonitorXY(void *p) {
      return  p ? new(p) ::NicaPropertyMonitorXY : new ::NicaPropertyMonitorXY;
   }
   static void *newArray_NicaPropertyMonitorXY(Long_t nElements, void *p) {
      return p ? new(p) ::NicaPropertyMonitorXY[nElements] : new ::NicaPropertyMonitorXY[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaPropertyMonitorXY(void *p) {
      delete ((::NicaPropertyMonitorXY*)p);
   }
   static void deleteArray_NicaPropertyMonitorXY(void *p) {
      delete [] ((::NicaPropertyMonitorXY*)p);
   }
   static void destruct_NicaPropertyMonitorXY(void *p) {
      typedef ::NicaPropertyMonitorXY current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaPropertyMonitorXY

//______________________________________________________________________________
void NicaPropertyMonitorXYZ::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaPropertyMonitorXYZ.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaPropertyMonitorXYZ::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaPropertyMonitorXYZ::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaPropertyMonitorXYZ(void *p) {
      return  p ? new(p) ::NicaPropertyMonitorXYZ : new ::NicaPropertyMonitorXYZ;
   }
   static void *newArray_NicaPropertyMonitorXYZ(Long_t nElements, void *p) {
      return p ? new(p) ::NicaPropertyMonitorXYZ[nElements] : new ::NicaPropertyMonitorXYZ[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaPropertyMonitorXYZ(void *p) {
      delete ((::NicaPropertyMonitorXYZ*)p);
   }
   static void deleteArray_NicaPropertyMonitorXYZ(void *p) {
      delete [] ((::NicaPropertyMonitorXYZ*)p);
   }
   static void destruct_NicaPropertyMonitorXYZ(void *p) {
      typedef ::NicaPropertyMonitorXYZ current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaPropertyMonitorXYZ

namespace {
  void TriggerDictionaryInitialization_G__NicaCutDict_Impl() {
    static const char* headers[] = {
"NicaSubCut.h",
"NicaCut.h",
"NicaCutContainer.h",
"NicaCutCollection.h",
"NicaEventCut.h",
"NicaTrackCut.h",
"NicaTwoTrackCut.h",
"NicaCutsAndMonitors.h",
"NicaEventImpactParameterCut.h",
"NicaEventModuloCut.h",
"NicaEventMultiplicityCut.h",
"NicaEventPhiCut.h",
"NicaEventPtMaxCut.h",
"NicaEventVirtualCut.h",
"NicaEventComplexCut.h",
"NicaEventVertexCut.h",
"NicaEventBinningCut.h",
"NicaEventPrimaryTracksNoCut.h",
"NicaTrackBasicToFCut.h",
"NicaTrackToFMass2Cut.h",
"NicaTrackDCACut.h",
"NicaTrackTpcCut.h",
"NicaTrackExpCut.h",
"NicaTrackTpcHitCut.h",
"NicaTrackTpcToFCut.h",
"NicaTrackDCAPipeCut.h",
"NicaTrackTpcToFThresholdlessCut.h",
"NicaTrackChi2Cut.h",
"NicaTrackBetaCut.h",
"NicaTrackEtaCut.h",
"NicaTrackMassCut.h",
"NicaTrackMtCut.h",
"NicaTrackPCut.h",
"NicaTrackPhiCut.h",
"NicaTrackPtCut.h",
"NicaTrackYCut.h",
"NicaTrackPzCut.h",
"NicaTrackChargeCut.h",
"NicaTrackBoostedMomentaCuts.h",
"NicaTrackBasicMCCut.h",
"NicaTrackFreezoutCut.h",
"NicaTrackMCCut.h",
"NicaTrackPdgCut.h",
"NicaTrackStartPositionCut.h",
"NicaTrackPdgBinCut.h",
"NicaTrackDeltaMomCut.h",
"NicaTrackDeltaMomentumCut.h",
"NicaTrackOnlyPrimariesCut.h",
"NicaTrackStatusCut.h",
"NicaTrackComplexCut.h",
"NicaTrackVirtualCut.h",
"NicaTrackFlagCuts.h",
"NicaTwoTrackEtaCut.h",
"NicaTwoTrackKtCut.h",
"NicaTwoTrackMinvCut.h",
"NicaTwoTrackPhiCut.h",
"NicaTwoTrackPtOrderCut.h",
"NicaTwoTrackDphiDetaCut.h",
"NicaTwoTrackPtSumCut.h",
"NicaTwoTrackRapidityCut.h",
"NicaTwoTrackDeltaEtaCut.h",
"NicaPairDeltaQinvCut.h",
"NicaTwoTrackLCMSCut.h",
"NicaTwoTrack3DCFCut.h",
"NicaTwoTrackComplexCut.h",
"NicaTwoTrackFemtoQinvCut.h",
"NicaTwoTrackSailorCowboyCut.h",
"NicaTwoTrackRadialDistanceCut.h",
"NicaTwoTrackDebugCut.h",
"NicaTwoTrackFemtoPRFCut.h",
"NicaPdgBinPairCut.h",
"NicaTwoTrackVirtualCut.h",
"NicaCutMonitor.h",
"NicaCutMonitorX.h",
"NicaCutMonitorXY.h",
"NicaCutMonitorXYZ.h",
"NicaPropertyMonitorX.h",
"NicaPropertyMonitorXY.h",
"NicaPropertyMonitorXYZ.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/hist",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io/html",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/std",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/minimizer",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/eventcuts",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/detector",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/mc",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/resolution",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/detector",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/mc",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/cutmonitors",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/detector",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/compound",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/hiddeninfo",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/cuts/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__NicaCutDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$NicaSubCut.h")))  NicaSubCut;
class __attribute__((annotate("$clingAutoload$NicaSubCut.h")))  NicaSubCutHisto;
class __attribute__((annotate("$clingAutoload$NicaCut.h")))  NicaCut;
class __attribute__((annotate("$clingAutoload$NicaCutMonitor.h")))  __attribute__((annotate("$clingAutoload$NicaCutContainer.h")))  NicaCutMonitor;
class __attribute__((annotate("$clingAutoload$NicaCutCollection.h")))  __attribute__((annotate("$clingAutoload$NicaCutContainer.h")))  NicaCutCollection;
class __attribute__((annotate("$clingAutoload$NicaCutContainer.h")))  NicaCutContainer;
class __attribute__((annotate("$clingAutoload$NicaEventCut.h")))  NicaEventCut;
class __attribute__((annotate("$clingAutoload$NicaTrackCut.h")))  NicaTrackCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackCut.h")))  NicaTwoTrackCut;
class __attribute__((annotate("$clingAutoload$NicaCutsAndMonitors.h")))  NicaCutsAndMonitors;
class __attribute__((annotate("$clingAutoload$NicaEventImpactParameterCut.h")))  NicaEventImpactParameterCut;
class __attribute__((annotate("$clingAutoload$NicaEventImpactParameterCut.h")))  NicaEventImpactCentralityCut;
class __attribute__((annotate("$clingAutoload$NicaEventModuloCut.h")))  NicaEventModuloCut;
class __attribute__((annotate("$clingAutoload$NicaEventMultiplicityCut.h")))  NicaEventMultiplicityCut;
class __attribute__((annotate("$clingAutoload$NicaEventMultiplicityCut.h")))  NicaEventMultiChargedCut;
class __attribute__((annotate("$clingAutoload$NicaEventPhiCut.h")))  NicaEventPhiCut;
class __attribute__((annotate("$clingAutoload$NicaEventPtMaxCut.h")))  NicaEventPtMaxCut;
class __attribute__((annotate("$clingAutoload$NicaEventVirtualCut.h")))  NicaEventVirtualCut;
class __attribute__((annotate("$clingAutoload$NicaEventComplexCut.h")))  NicaEventComplexCut;
class __attribute__((annotate("$clingAutoload$NicaEventComplexCut.h")))  NicaEventRealCut;
class __attribute__((annotate("$clingAutoload$NicaEventComplexCut.h")))  NicaEventImaginaryCut;
class __attribute__((annotate("$clingAutoload$NicaEventVertexCut.h")))  NicaEventVertexCut;
class __attribute__((annotate("$clingAutoload$NicaEventBinningCut.h")))  NicaEventBinningCut;
class __attribute__((annotate("$clingAutoload$NicaEventPrimaryTracksNoCut.h")))  NicaEventPrimaryTracksNoCut;
class __attribute__((annotate("$clingAutoload$NicaTrackExpCut.h")))  __attribute__((annotate("$clingAutoload$NicaTrackBasicToFCut.h")))  NicaTrackExpCut;
class __attribute__((annotate("$clingAutoload$NicaTrackBasicToFCut.h")))  NicaTrackBasicToFCut;
class __attribute__((annotate("$clingAutoload$NicaTrackToFMass2Cut.h")))  NicaTrackToFMass2Cut;
class __attribute__((annotate("$clingAutoload$NicaTrackToFMass2Cut.h")))  NicaTrackTofMass2CutNoZeros;
class __attribute__((annotate("$clingAutoload$NicaTrackDCACut.h")))  NicaTrackDCACut;
class __attribute__((annotate("$clingAutoload$NicaTrackTpcCut.h")))  NicaTrackTpcCut;
class __attribute__((annotate("$clingAutoload$NicaTrackTpcHitCut.h")))  NicaTrackTpcHitCut;
class __attribute__((annotate("$clingAutoload$NicaTrackTpcToFCut.h")))  NicaTrackTpcToFCut;
class __attribute__((annotate("$clingAutoload$NicaTrackDCAPipeCut.h")))  NicaTrackDCAPipeCut;
class __attribute__((annotate("$clingAutoload$NicaTrackTpcToFThresholdlessCut.h")))  NicaTrackTpcToFThresholdlessCut;
class __attribute__((annotate("$clingAutoload$NicaTrackChi2Cut.h")))  NicaTrackChi2Cut;
class __attribute__((annotate("$clingAutoload$NicaTrackBetaCut.h")))  NicaTrackBetaCut;
class __attribute__((annotate("$clingAutoload$NicaTrackEtaCut.h")))  NicaTrackEtaCut;
class __attribute__((annotate("$clingAutoload$NicaTrackEtaCut.h")))  NicaTrackEtaAbsCut;
class __attribute__((annotate("$clingAutoload$NicaTrackMassCut.h")))  NicaTrackMassCut;
class __attribute__((annotate("$clingAutoload$NicaTrackMtCut.h")))  NicaTrackMtCut;
class __attribute__((annotate("$clingAutoload$NicaTrackPCut.h")))  NicaTrackPCut;
class __attribute__((annotate("$clingAutoload$NicaTrackPCut.h")))  NicaTrackPSignedCut;
class __attribute__((annotate("$clingAutoload$NicaTrackPhiCut.h")))  NicaTrackPhiCut;
class __attribute__((annotate("$clingAutoload$NicaTrackPtCut.h")))  NicaTrackPtCut;
class __attribute__((annotate("$clingAutoload$NicaTrackYCut.h")))  NicaTrackYCut;
class __attribute__((annotate("$clingAutoload$NicaTrackPzCut.h")))  NicaTrackPzCut;
class __attribute__((annotate("$clingAutoload$NicaTrackChargeCut.h")))  NicaTrackChargeCut;
class __attribute__((annotate("$clingAutoload$NicaTrackVirtualCut.h")))  __attribute__((annotate("$clingAutoload$NicaTrackBoostedMomentaCuts.h")))  NicaTrackVirtualCut;
class __attribute__((annotate("$clingAutoload$NicaTrackComplexCut.h")))  __attribute__((annotate("$clingAutoload$NicaTrackBoostedMomentaCuts.h")))  NicaTrackComplexCut;
class __attribute__((annotate("$clingAutoload$NicaTrackComplexCut.h")))  __attribute__((annotate("$clingAutoload$NicaTrackBoostedMomentaCuts.h")))  NicaTrackRealCut;
class __attribute__((annotate("$clingAutoload$NicaTrackComplexCut.h")))  __attribute__((annotate("$clingAutoload$NicaTrackBoostedMomentaCuts.h")))  NicaTrackImaginaryCut;
class __attribute__((annotate("$clingAutoload$NicaTrackBoostedMomentaCuts.h")))  NicaTrackBoostedPCut;
class __attribute__((annotate("$clingAutoload$NicaTrackBoostedMomentaCuts.h")))  NicaTrackBoostedKinematcisCut;
class __attribute__((annotate("$clingAutoload$NicaTrackBoostedMomentaCuts.h")))  NicaTrackBoostedDeltaPCut;
class __attribute__((annotate("$clingAutoload$NicaTrackMCCut.h")))  __attribute__((annotate("$clingAutoload$NicaTrackBasicMCCut.h")))  NicaTrackMCCut;
class __attribute__((annotate("$clingAutoload$NicaTrackBasicMCCut.h")))  NicaTrackBasicMCCut;
class __attribute__((annotate("$clingAutoload$NicaTrackFreezoutCut.h")))  NicaTrackFreezoutCut;
class __attribute__((annotate("$clingAutoload$NicaTrackFreezoutCut.h")))  NicaTrackTFreezCut;
class __attribute__((annotate("$clingAutoload$NicaTrackFreezoutCut.h")))  NicaTrackTauCut;
class __attribute__((annotate("$clingAutoload$NicaTrackPdgCut.h")))  NicaTrackPdgCut;
class __attribute__((annotate("$clingAutoload$NicaTrackPdgCut.h")))  NicaTrackPdgAbsCut;
class __attribute__((annotate("$clingAutoload$NicaTrackStartPositionCut.h")))  NicaTrackStartPositionCut;
class __attribute__((annotate("$clingAutoload$NicaTrackPdgBinCut.h")))  NicaTrackPdgBinCut;
class __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomentumCut.h")))  __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomCut.h")))  NicaTrackDeltaMomentumCut;
class __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomentumCut.h")))  __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomCut.h")))  NicaTrackDeltaMomCut;
class __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomentumCut.h")))  __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomCut.h")))  NicaTrackDeltaVectMomCut;
class __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomCut.h")))  NicaTrackDeltaPtCut;
class __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomCut.h")))  NicaTrackDeltaPCut;
class __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomCut.h")))  NicaTrackDeltaPzCut;
class __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomCut.h")))  NicaTrackDeltaPhiCut;
class __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomCut.h")))  NicaTrackDeltaThetaCut;
class __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomCut.h")))  NicaTrackDeltaVectPtCut;
class __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomCut.h")))  NicaTrackDeltaVectPCut;
class __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomCut.h")))  NicaTrackDeltaVectPhiCut;
class __attribute__((annotate("$clingAutoload$NicaTrackDeltaMomCut.h")))  NicaTrackDeltaVectThetaCut;
class __attribute__((annotate("$clingAutoload$NicaTrackOnlyPrimariesCut.h")))  NicaTrackOnlyPrimariesCut;
class __attribute__((annotate("$clingAutoload$NicaTrackStatusCut.h")))  NicaTrackStatusCut;
class __attribute__((annotate("$clingAutoload$NicaTrackFlagCuts.h")))  NicaTrackPrimaryFlagCut;
class __attribute__((annotate("$clingAutoload$NicaTrackFlagCuts.h")))  NicaTrackV0FlagCut;
class __attribute__((annotate("$clingAutoload$NicaTrackFlagCuts.h")))  NicaTrackGoodSecondaryFlagCut;
class __attribute__((annotate("$clingAutoload$NicaTrackFlagCuts.h")))  NicaTrackGlobalFlagCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackEtaCut.h")))  NicaTwoTrackEtaCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackKtCut.h")))  NicaTwoTrackKtCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackMinvCut.h")))  NicaTwoTrackMinvCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackPhiCut.h")))  NicaTwoTrackPhiCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackPtOrderCut.h")))  NicaTwoTrackPtOrderCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackDphiDetaCut.h")))  NicaTwoTrackDphiDetaCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackDphiDetaCut.h")))  NicaTwoTrackDphistarDetaCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackPtSumCut.h")))  NicaTwoTrackPtSumCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackRapidityCut.h")))  NicaTwoTrackRapidityCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackDeltaEtaCut.h")))  NicaTwoTrackDeltaEtaCut;
class __attribute__((annotate("$clingAutoload$NicaPairDeltaQinvCut.h")))  NicaPairDeltaQinvCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackLCMSCut.h")))  NicaTwoTrackLCMSCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrack3DCFCut.h")))  NicaTwoTrack3DCFCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackComplexCut.h")))  NicaTwoTrackComplexCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackComplexCut.h")))  NicaTwoTrackRealCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackComplexCut.h")))  NicaTwoTrackImaginaryCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackFemtoQinvCut.h")))  NicaTwoTrackFemtoQinvCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackSailorCowboyCut.h")))  NicaTwoTrackSailorCowboyCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackRadialDistanceCut.h")))  NicaTwoTrackRadialDistanceCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackDebugCut.h")))  NicaTwoTrackDebugCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackFemtoPRFCut.h")))  NicaTwoTrackFemtoPRFCut;
class __attribute__((annotate("$clingAutoload$NicaPdgBinPairCut.h")))  NicaPdgBinPairCut;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackVirtualCut.h")))  NicaTwoTrackVirtualCut;
class __attribute__((annotate("$clingAutoload$NicaCutMonitorX.h")))  NicaCutMonitorX;
class __attribute__((annotate("$clingAutoload$NicaCutMonitorXY.h")))  NicaCutMonitorXY;
class __attribute__((annotate("$clingAutoload$NicaCutMonitorXYZ.h")))  NicaCutMonitorXYZ;
class __attribute__((annotate("$clingAutoload$NicaPropertyMonitorX.h")))  NicaPropertyMonitorX;
class __attribute__((annotate("$clingAutoload$NicaPropertyMonitorXY.h")))  NicaPropertyMonitorXY;
class __attribute__((annotate("$clingAutoload$NicaPropertyMonitorXYZ.h")))  NicaPropertyMonitorXYZ;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__NicaCutDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "NicaSubCut.h"
#include "NicaCut.h"
#include "NicaCutContainer.h"
#include "NicaCutCollection.h"
#include "NicaEventCut.h"
#include "NicaTrackCut.h"
#include "NicaTwoTrackCut.h"
#include "NicaCutsAndMonitors.h"
#include "NicaEventImpactParameterCut.h"
#include "NicaEventModuloCut.h"
#include "NicaEventMultiplicityCut.h"
#include "NicaEventPhiCut.h"
#include "NicaEventPtMaxCut.h"
#include "NicaEventVirtualCut.h"
#include "NicaEventComplexCut.h"
#include "NicaEventVertexCut.h"
#include "NicaEventBinningCut.h"
#include "NicaEventPrimaryTracksNoCut.h"
#include "NicaTrackBasicToFCut.h"
#include "NicaTrackToFMass2Cut.h"
#include "NicaTrackDCACut.h"
#include "NicaTrackTpcCut.h"
#include "NicaTrackExpCut.h"
#include "NicaTrackTpcHitCut.h"
#include "NicaTrackTpcToFCut.h"
#include "NicaTrackDCAPipeCut.h"
#include "NicaTrackTpcToFThresholdlessCut.h"
#include "NicaTrackChi2Cut.h"
#include "NicaTrackBetaCut.h"
#include "NicaTrackEtaCut.h"
#include "NicaTrackMassCut.h"
#include "NicaTrackMtCut.h"
#include "NicaTrackPCut.h"
#include "NicaTrackPhiCut.h"
#include "NicaTrackPtCut.h"
#include "NicaTrackYCut.h"
#include "NicaTrackPzCut.h"
#include "NicaTrackChargeCut.h"
#include "NicaTrackBoostedMomentaCuts.h"
#include "NicaTrackBasicMCCut.h"
#include "NicaTrackFreezoutCut.h"
#include "NicaTrackMCCut.h"
#include "NicaTrackPdgCut.h"
#include "NicaTrackStartPositionCut.h"
#include "NicaTrackPdgBinCut.h"
#include "NicaTrackDeltaMomCut.h"
#include "NicaTrackDeltaMomentumCut.h"
#include "NicaTrackOnlyPrimariesCut.h"
#include "NicaTrackStatusCut.h"
#include "NicaTrackComplexCut.h"
#include "NicaTrackVirtualCut.h"
#include "NicaTrackFlagCuts.h"
#include "NicaTwoTrackEtaCut.h"
#include "NicaTwoTrackKtCut.h"
#include "NicaTwoTrackMinvCut.h"
#include "NicaTwoTrackPhiCut.h"
#include "NicaTwoTrackPtOrderCut.h"
#include "NicaTwoTrackDphiDetaCut.h"
#include "NicaTwoTrackPtSumCut.h"
#include "NicaTwoTrackRapidityCut.h"
#include "NicaTwoTrackDeltaEtaCut.h"
#include "NicaPairDeltaQinvCut.h"
#include "NicaTwoTrackLCMSCut.h"
#include "NicaTwoTrack3DCFCut.h"
#include "NicaTwoTrackComplexCut.h"
#include "NicaTwoTrackFemtoQinvCut.h"
#include "NicaTwoTrackSailorCowboyCut.h"
#include "NicaTwoTrackRadialDistanceCut.h"
#include "NicaTwoTrackDebugCut.h"
#include "NicaTwoTrackFemtoPRFCut.h"
#include "NicaPdgBinPairCut.h"
#include "NicaTwoTrackVirtualCut.h"
#include "NicaCutMonitor.h"
#include "NicaCutMonitorX.h"
#include "NicaCutMonitorXY.h"
#include "NicaCutMonitorXYZ.h"
#include "NicaPropertyMonitorX.h"
#include "NicaPropertyMonitorXY.h"
#include "NicaPropertyMonitorXYZ.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"NicaCut", payloadCode, "@",
"NicaCutCollection", payloadCode, "@",
"NicaCutContainer", payloadCode, "@",
"NicaCutMonitor", payloadCode, "@",
"NicaCutMonitorX", payloadCode, "@",
"NicaCutMonitorXY", payloadCode, "@",
"NicaCutMonitorXYZ", payloadCode, "@",
"NicaCutsAndMonitors", payloadCode, "@",
"NicaEventBinningCut", payloadCode, "@",
"NicaEventComplexCut", payloadCode, "@",
"NicaEventCut", payloadCode, "@",
"NicaEventImaginaryCut", payloadCode, "@",
"NicaEventImpactCentralityCut", payloadCode, "@",
"NicaEventImpactParameterCut", payloadCode, "@",
"NicaEventModuloCut", payloadCode, "@",
"NicaEventMultiChargedCut", payloadCode, "@",
"NicaEventMultiplicityCut", payloadCode, "@",
"NicaEventPhiCut", payloadCode, "@",
"NicaEventPrimaryTracksNoCut", payloadCode, "@",
"NicaEventPtMaxCut", payloadCode, "@",
"NicaEventRealCut", payloadCode, "@",
"NicaEventVertexCut", payloadCode, "@",
"NicaEventVirtualCut", payloadCode, "@",
"NicaPairDeltaQinvCut", payloadCode, "@",
"NicaPdgBinPairCut", payloadCode, "@",
"NicaPropertyMonitorX", payloadCode, "@",
"NicaPropertyMonitorXY", payloadCode, "@",
"NicaPropertyMonitorXYZ", payloadCode, "@",
"NicaSubCut", payloadCode, "@",
"NicaSubCutHisto", payloadCode, "@",
"NicaTrackBasicMCCut", payloadCode, "@",
"NicaTrackBasicToFCut", payloadCode, "@",
"NicaTrackBetaCut", payloadCode, "@",
"NicaTrackBoostedDeltaPCut", payloadCode, "@",
"NicaTrackBoostedKinematcisCut", payloadCode, "@",
"NicaTrackBoostedPCut", payloadCode, "@",
"NicaTrackChargeCut", payloadCode, "@",
"NicaTrackChi2Cut", payloadCode, "@",
"NicaTrackComplexCut", payloadCode, "@",
"NicaTrackCut", payloadCode, "@",
"NicaTrackDCACut", payloadCode, "@",
"NicaTrackDCAPipeCut", payloadCode, "@",
"NicaTrackDeltaMomCut", payloadCode, "@",
"NicaTrackDeltaMomentumCut", payloadCode, "@",
"NicaTrackDeltaPCut", payloadCode, "@",
"NicaTrackDeltaPhiCut", payloadCode, "@",
"NicaTrackDeltaPtCut", payloadCode, "@",
"NicaTrackDeltaPzCut", payloadCode, "@",
"NicaTrackDeltaThetaCut", payloadCode, "@",
"NicaTrackDeltaVectMomCut", payloadCode, "@",
"NicaTrackDeltaVectPCut", payloadCode, "@",
"NicaTrackDeltaVectPhiCut", payloadCode, "@",
"NicaTrackDeltaVectPtCut", payloadCode, "@",
"NicaTrackDeltaVectThetaCut", payloadCode, "@",
"NicaTrackEtaAbsCut", payloadCode, "@",
"NicaTrackEtaCut", payloadCode, "@",
"NicaTrackExpCut", payloadCode, "@",
"NicaTrackFreezoutCut", payloadCode, "@",
"NicaTrackGlobalFlagCut", payloadCode, "@",
"NicaTrackGoodSecondaryFlagCut", payloadCode, "@",
"NicaTrackImaginaryCut", payloadCode, "@",
"NicaTrackMCCut", payloadCode, "@",
"NicaTrackMassCut", payloadCode, "@",
"NicaTrackMtCut", payloadCode, "@",
"NicaTrackOnlyPrimariesCut", payloadCode, "@",
"NicaTrackPCut", payloadCode, "@",
"NicaTrackPSignedCut", payloadCode, "@",
"NicaTrackPdgAbsCut", payloadCode, "@",
"NicaTrackPdgBinCut", payloadCode, "@",
"NicaTrackPdgCut", payloadCode, "@",
"NicaTrackPhiCut", payloadCode, "@",
"NicaTrackPrimaryFlagCut", payloadCode, "@",
"NicaTrackPtCut", payloadCode, "@",
"NicaTrackPzCut", payloadCode, "@",
"NicaTrackRealCut", payloadCode, "@",
"NicaTrackStartPositionCut", payloadCode, "@",
"NicaTrackStatusCut", payloadCode, "@",
"NicaTrackTFreezCut", payloadCode, "@",
"NicaTrackTauCut", payloadCode, "@",
"NicaTrackToFMass2Cut", payloadCode, "@",
"NicaTrackTofMass2CutNoZeros", payloadCode, "@",
"NicaTrackTpcCut", payloadCode, "@",
"NicaTrackTpcHitCut", payloadCode, "@",
"NicaTrackTpcToFCut", payloadCode, "@",
"NicaTrackTpcToFThresholdlessCut", payloadCode, "@",
"NicaTrackV0FlagCut", payloadCode, "@",
"NicaTrackVirtualCut", payloadCode, "@",
"NicaTrackYCut", payloadCode, "@",
"NicaTwoTrack3DCFCut", payloadCode, "@",
"NicaTwoTrackComplexCut", payloadCode, "@",
"NicaTwoTrackCut", payloadCode, "@",
"NicaTwoTrackDebugCut", payloadCode, "@",
"NicaTwoTrackDeltaEtaCut", payloadCode, "@",
"NicaTwoTrackDphiDetaCut", payloadCode, "@",
"NicaTwoTrackDphistarDetaCut", payloadCode, "@",
"NicaTwoTrackEtaCut", payloadCode, "@",
"NicaTwoTrackFemtoPRFCut", payloadCode, "@",
"NicaTwoTrackFemtoQinvCut", payloadCode, "@",
"NicaTwoTrackImaginaryCut", payloadCode, "@",
"NicaTwoTrackKtCut", payloadCode, "@",
"NicaTwoTrackLCMSCut", payloadCode, "@",
"NicaTwoTrackMinvCut", payloadCode, "@",
"NicaTwoTrackPhiCut", payloadCode, "@",
"NicaTwoTrackPtOrderCut", payloadCode, "@",
"NicaTwoTrackPtSumCut", payloadCode, "@",
"NicaTwoTrackRadialDistanceCut", payloadCode, "@",
"NicaTwoTrackRapidityCut", payloadCode, "@",
"NicaTwoTrackRealCut", payloadCode, "@",
"NicaTwoTrackSailorCowboyCut", payloadCode, "@",
"NicaTwoTrackVirtualCut", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__NicaCutDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__NicaCutDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__NicaCutDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__NicaCutDict() {
  TriggerDictionaryInitialization_G__NicaCutDict_Impl();
}
