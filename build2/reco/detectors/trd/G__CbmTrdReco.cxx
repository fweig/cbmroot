// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmTrdReco
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
#include "CbmTrdTrackFinderIdeal.h"
#include "CbmTrdTrackFitterIdeal.h"
#include "CbmTrdClusterFinder.h"
#include "CbmTrdHitProducer.h"
#include "CbmTrdModuleRec.h"
#include "CbmTrdModuleRecR.h"
#include "CbmTrdModuleRec2D.h"
#include "CbmTrdDigiRec.h"
#include "CbmTrdUnpackConfig.h"
#include "CbmTrdUnpackAlgoBaseR.h"
#include "CbmTrdUnpackAlgoR.h"
#include "CbmTrdUnpackAlgoLegacy2020R.h"
#include "CbmTrdUnpackMonitor.h"
#include "CbmTrdUnpackFaspAlgo.h"
#include "CbmTrdUnpackFaspConfig.h"
#include "CbmTrdUnpackFaspMonitor.h"
#include "CbmTrdClusterizerFastQa.h"
#include "CbmTrdHitDensityQa.h"
#include "CbmTrdHitProducerClusterQa.h"
#include "CbmTrdHitProducerQa.h"
#include "CbmTrdOccupancyQa.h"
#include "CbmTrdQa.h"
#include "CbmTrdRecoQa.h"
#include "CbmTrdTracksPidQa.h"
#include "CbmTrdCalibTracker.h"
#include "CbmTrdHitMC.h"
#include "CbmTrdElectronsTrainAnn.h"
#include "CbmTrdSetTracksPidWkn.h"
#include "CbmTrdSetTracksPidModWkn.h"
#include "CbmTrdSetTracksPidLike.h"
#include "CbmTrdSetTracksPidANN.h"
#include "CbmTrdRawToDigiBaseR.h"
#include "CbmTrdRawToDigiMaxAdcR.h"
#include "CbmTrdRawToDigiFitR.h"
#include "CbmTrdRawToDigiLookUpCorrR.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmTrdTrackFinderIdeal(void *p = nullptr);
   static void *newArray_CbmTrdTrackFinderIdeal(Long_t size, void *p);
   static void delete_CbmTrdTrackFinderIdeal(void *p);
   static void deleteArray_CbmTrdTrackFinderIdeal(void *p);
   static void destruct_CbmTrdTrackFinderIdeal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdTrackFinderIdeal*)
   {
      ::CbmTrdTrackFinderIdeal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdTrackFinderIdeal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdTrackFinderIdeal", ::CbmTrdTrackFinderIdeal::Class_Version(), "CbmTrdTrackFinderIdeal.h", 31,
                  typeid(::CbmTrdTrackFinderIdeal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdTrackFinderIdeal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdTrackFinderIdeal) );
      instance.SetNew(&new_CbmTrdTrackFinderIdeal);
      instance.SetNewArray(&newArray_CbmTrdTrackFinderIdeal);
      instance.SetDelete(&delete_CbmTrdTrackFinderIdeal);
      instance.SetDeleteArray(&deleteArray_CbmTrdTrackFinderIdeal);
      instance.SetDestructor(&destruct_CbmTrdTrackFinderIdeal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdTrackFinderIdeal*)
   {
      return GenerateInitInstanceLocal((::CbmTrdTrackFinderIdeal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdTrackFinderIdeal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdTrackFitterIdeal(void *p = nullptr);
   static void *newArray_CbmTrdTrackFitterIdeal(Long_t size, void *p);
   static void delete_CbmTrdTrackFitterIdeal(void *p);
   static void deleteArray_CbmTrdTrackFitterIdeal(void *p);
   static void destruct_CbmTrdTrackFitterIdeal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdTrackFitterIdeal*)
   {
      ::CbmTrdTrackFitterIdeal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdTrackFitterIdeal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdTrackFitterIdeal", ::CbmTrdTrackFitterIdeal::Class_Version(), "CbmTrdTrackFitterIdeal.h", 20,
                  typeid(::CbmTrdTrackFitterIdeal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdTrackFitterIdeal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdTrackFitterIdeal) );
      instance.SetNew(&new_CbmTrdTrackFitterIdeal);
      instance.SetNewArray(&newArray_CbmTrdTrackFitterIdeal);
      instance.SetDelete(&delete_CbmTrdTrackFitterIdeal);
      instance.SetDeleteArray(&deleteArray_CbmTrdTrackFitterIdeal);
      instance.SetDestructor(&destruct_CbmTrdTrackFitterIdeal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdTrackFitterIdeal*)
   {
      return GenerateInitInstanceLocal((::CbmTrdTrackFitterIdeal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdTrackFitterIdeal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdClusterFinder(void *p = nullptr);
   static void *newArray_CbmTrdClusterFinder(Long_t size, void *p);
   static void delete_CbmTrdClusterFinder(void *p);
   static void deleteArray_CbmTrdClusterFinder(void *p);
   static void destruct_CbmTrdClusterFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdClusterFinder*)
   {
      ::CbmTrdClusterFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdClusterFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdClusterFinder", ::CbmTrdClusterFinder::Class_Version(), "CbmTrdClusterFinder.h", 56,
                  typeid(::CbmTrdClusterFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdClusterFinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdClusterFinder) );
      instance.SetNew(&new_CbmTrdClusterFinder);
      instance.SetNewArray(&newArray_CbmTrdClusterFinder);
      instance.SetDelete(&delete_CbmTrdClusterFinder);
      instance.SetDeleteArray(&deleteArray_CbmTrdClusterFinder);
      instance.SetDestructor(&destruct_CbmTrdClusterFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdClusterFinder*)
   {
      return GenerateInitInstanceLocal((::CbmTrdClusterFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdClusterFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdHitProducer(void *p = nullptr);
   static void *newArray_CbmTrdHitProducer(Long_t size, void *p);
   static void delete_CbmTrdHitProducer(void *p);
   static void deleteArray_CbmTrdHitProducer(void *p);
   static void destruct_CbmTrdHitProducer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdHitProducer*)
   {
      ::CbmTrdHitProducer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdHitProducer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdHitProducer", ::CbmTrdHitProducer::Class_Version(), "CbmTrdHitProducer.h", 36,
                  typeid(::CbmTrdHitProducer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdHitProducer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdHitProducer) );
      instance.SetNew(&new_CbmTrdHitProducer);
      instance.SetNewArray(&newArray_CbmTrdHitProducer);
      instance.SetDelete(&delete_CbmTrdHitProducer);
      instance.SetDeleteArray(&deleteArray_CbmTrdHitProducer);
      instance.SetDestructor(&destruct_CbmTrdHitProducer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdHitProducer*)
   {
      return GenerateInitInstanceLocal((::CbmTrdHitProducer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdHitProducer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrdModuleRec(void *p);
   static void deleteArray_CbmTrdModuleRec(void *p);
   static void destruct_CbmTrdModuleRec(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdModuleRec*)
   {
      ::CbmTrdModuleRec *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdModuleRec >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdModuleRec", ::CbmTrdModuleRec::Class_Version(), "CbmTrdModuleRec.h", 20,
                  typeid(::CbmTrdModuleRec), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdModuleRec::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdModuleRec) );
      instance.SetDelete(&delete_CbmTrdModuleRec);
      instance.SetDeleteArray(&deleteArray_CbmTrdModuleRec);
      instance.SetDestructor(&destruct_CbmTrdModuleRec);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdModuleRec*)
   {
      return GenerateInitInstanceLocal((::CbmTrdModuleRec*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdModuleRec*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdModuleRecR(void *p = nullptr);
   static void *newArray_CbmTrdModuleRecR(Long_t size, void *p);
   static void delete_CbmTrdModuleRecR(void *p);
   static void deleteArray_CbmTrdModuleRecR(void *p);
   static void destruct_CbmTrdModuleRecR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdModuleRecR*)
   {
      ::CbmTrdModuleRecR *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdModuleRecR >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdModuleRecR", ::CbmTrdModuleRecR::Class_Version(), "CbmTrdModuleRecR.h", 18,
                  typeid(::CbmTrdModuleRecR), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdModuleRecR::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdModuleRecR) );
      instance.SetNew(&new_CbmTrdModuleRecR);
      instance.SetNewArray(&newArray_CbmTrdModuleRecR);
      instance.SetDelete(&delete_CbmTrdModuleRecR);
      instance.SetDeleteArray(&deleteArray_CbmTrdModuleRecR);
      instance.SetDestructor(&destruct_CbmTrdModuleRecR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdModuleRecR*)
   {
      return GenerateInitInstanceLocal((::CbmTrdModuleRecR*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdModuleRecR*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdModuleRec2D(void *p = nullptr);
   static void *newArray_CbmTrdModuleRec2D(Long_t size, void *p);
   static void delete_CbmTrdModuleRec2D(void *p);
   static void deleteArray_CbmTrdModuleRec2D(void *p);
   static void destruct_CbmTrdModuleRec2D(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdModuleRec2D*)
   {
      ::CbmTrdModuleRec2D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdModuleRec2D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdModuleRec2D", ::CbmTrdModuleRec2D::Class_Version(), "CbmTrdModuleRec2D.h", 36,
                  typeid(::CbmTrdModuleRec2D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdModuleRec2D::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdModuleRec2D) );
      instance.SetNew(&new_CbmTrdModuleRec2D);
      instance.SetNewArray(&newArray_CbmTrdModuleRec2D);
      instance.SetDelete(&delete_CbmTrdModuleRec2D);
      instance.SetDeleteArray(&deleteArray_CbmTrdModuleRec2D);
      instance.SetDestructor(&destruct_CbmTrdModuleRec2D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdModuleRec2D*)
   {
      return GenerateInitInstanceLocal((::CbmTrdModuleRec2D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdModuleRec2D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdDigiRec(void *p = nullptr);
   static void *newArray_CbmTrdDigiRec(Long_t size, void *p);
   static void delete_CbmTrdDigiRec(void *p);
   static void deleteArray_CbmTrdDigiRec(void *p);
   static void destruct_CbmTrdDigiRec(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdDigiRec*)
   {
      ::CbmTrdDigiRec *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdDigiRec >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdDigiRec", ::CbmTrdDigiRec::Class_Version(), "CbmTrdDigiRec.h", 22,
                  typeid(::CbmTrdDigiRec), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdDigiRec::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdDigiRec) );
      instance.SetNew(&new_CbmTrdDigiRec);
      instance.SetNewArray(&newArray_CbmTrdDigiRec);
      instance.SetDelete(&delete_CbmTrdDigiRec);
      instance.SetDeleteArray(&deleteArray_CbmTrdDigiRec);
      instance.SetDestructor(&destruct_CbmTrdDigiRec);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdDigiRec*)
   {
      return GenerateInitInstanceLocal((::CbmTrdDigiRec*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdDigiRec*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrdRawToDigiBaseR(void *p);
   static void deleteArray_CbmTrdRawToDigiBaseR(void *p);
   static void destruct_CbmTrdRawToDigiBaseR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdRawToDigiBaseR*)
   {
      ::CbmTrdRawToDigiBaseR *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdRawToDigiBaseR >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdRawToDigiBaseR", ::CbmTrdRawToDigiBaseR::Class_Version(), "CbmTrdRawToDigiBaseR.h", 34,
                  typeid(::CbmTrdRawToDigiBaseR), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdRawToDigiBaseR::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdRawToDigiBaseR) );
      instance.SetDelete(&delete_CbmTrdRawToDigiBaseR);
      instance.SetDeleteArray(&deleteArray_CbmTrdRawToDigiBaseR);
      instance.SetDestructor(&destruct_CbmTrdRawToDigiBaseR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdRawToDigiBaseR*)
   {
      return GenerateInitInstanceLocal((::CbmTrdRawToDigiBaseR*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdRawToDigiBaseR*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdRawToDigiMaxAdcR(void *p = nullptr);
   static void *newArray_CbmTrdRawToDigiMaxAdcR(Long_t size, void *p);
   static void delete_CbmTrdRawToDigiMaxAdcR(void *p);
   static void deleteArray_CbmTrdRawToDigiMaxAdcR(void *p);
   static void destruct_CbmTrdRawToDigiMaxAdcR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdRawToDigiMaxAdcR*)
   {
      ::CbmTrdRawToDigiMaxAdcR *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdRawToDigiMaxAdcR >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdRawToDigiMaxAdcR", ::CbmTrdRawToDigiMaxAdcR::Class_Version(), "CbmTrdRawToDigiMaxAdcR.h", 31,
                  typeid(::CbmTrdRawToDigiMaxAdcR), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdRawToDigiMaxAdcR::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdRawToDigiMaxAdcR) );
      instance.SetNew(&new_CbmTrdRawToDigiMaxAdcR);
      instance.SetNewArray(&newArray_CbmTrdRawToDigiMaxAdcR);
      instance.SetDelete(&delete_CbmTrdRawToDigiMaxAdcR);
      instance.SetDeleteArray(&deleteArray_CbmTrdRawToDigiMaxAdcR);
      instance.SetDestructor(&destruct_CbmTrdRawToDigiMaxAdcR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdRawToDigiMaxAdcR*)
   {
      return GenerateInitInstanceLocal((::CbmTrdRawToDigiMaxAdcR*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdRawToDigiMaxAdcR*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdUnpackMonitor(void *p = nullptr);
   static void *newArray_CbmTrdUnpackMonitor(Long_t size, void *p);
   static void delete_CbmTrdUnpackMonitor(void *p);
   static void deleteArray_CbmTrdUnpackMonitor(void *p);
   static void destruct_CbmTrdUnpackMonitor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdUnpackMonitor*)
   {
      ::CbmTrdUnpackMonitor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdUnpackMonitor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdUnpackMonitor", ::CbmTrdUnpackMonitor::Class_Version(), "CbmTrdUnpackMonitor.h", 51,
                  typeid(::CbmTrdUnpackMonitor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdUnpackMonitor::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdUnpackMonitor) );
      instance.SetNew(&new_CbmTrdUnpackMonitor);
      instance.SetNewArray(&newArray_CbmTrdUnpackMonitor);
      instance.SetDelete(&delete_CbmTrdUnpackMonitor);
      instance.SetDeleteArray(&deleteArray_CbmTrdUnpackMonitor);
      instance.SetDestructor(&destruct_CbmTrdUnpackMonitor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdUnpackMonitor*)
   {
      return GenerateInitInstanceLocal((::CbmTrdUnpackMonitor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdUnpackMonitor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrdUnpackAlgoBaseR(void *p);
   static void deleteArray_CbmTrdUnpackAlgoBaseR(void *p);
   static void destruct_CbmTrdUnpackAlgoBaseR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdUnpackAlgoBaseR*)
   {
      ::CbmTrdUnpackAlgoBaseR *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdUnpackAlgoBaseR >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdUnpackAlgoBaseR", ::CbmTrdUnpackAlgoBaseR::Class_Version(), "CbmTrdUnpackAlgoBaseR.h", 45,
                  typeid(::CbmTrdUnpackAlgoBaseR), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdUnpackAlgoBaseR::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdUnpackAlgoBaseR) );
      instance.SetDelete(&delete_CbmTrdUnpackAlgoBaseR);
      instance.SetDeleteArray(&deleteArray_CbmTrdUnpackAlgoBaseR);
      instance.SetDestructor(&destruct_CbmTrdUnpackAlgoBaseR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdUnpackAlgoBaseR*)
   {
      return GenerateInitInstanceLocal((::CbmTrdUnpackAlgoBaseR*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoBaseR*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrdUnpackConfig(void *p);
   static void deleteArray_CbmTrdUnpackConfig(void *p);
   static void destruct_CbmTrdUnpackConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdUnpackConfig*)
   {
      ::CbmTrdUnpackConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdUnpackConfig >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdUnpackConfig", ::CbmTrdUnpackConfig::Class_Version(), "CbmTrdUnpackConfig.h", 36,
                  typeid(::CbmTrdUnpackConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdUnpackConfig::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdUnpackConfig) );
      instance.SetDelete(&delete_CbmTrdUnpackConfig);
      instance.SetDeleteArray(&deleteArray_CbmTrdUnpackConfig);
      instance.SetDestructor(&destruct_CbmTrdUnpackConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdUnpackConfig*)
   {
      return GenerateInitInstanceLocal((::CbmTrdUnpackConfig*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdUnpackConfig*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdUnpackAlgoR(void *p = nullptr);
   static void *newArray_CbmTrdUnpackAlgoR(Long_t size, void *p);
   static void delete_CbmTrdUnpackAlgoR(void *p);
   static void deleteArray_CbmTrdUnpackAlgoR(void *p);
   static void destruct_CbmTrdUnpackAlgoR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdUnpackAlgoR*)
   {
      ::CbmTrdUnpackAlgoR *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdUnpackAlgoR >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdUnpackAlgoR", ::CbmTrdUnpackAlgoR::Class_Version(), "CbmTrdUnpackAlgoR.h", 37,
                  typeid(::CbmTrdUnpackAlgoR), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdUnpackAlgoR::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdUnpackAlgoR) );
      instance.SetNew(&new_CbmTrdUnpackAlgoR);
      instance.SetNewArray(&newArray_CbmTrdUnpackAlgoR);
      instance.SetDelete(&delete_CbmTrdUnpackAlgoR);
      instance.SetDeleteArray(&deleteArray_CbmTrdUnpackAlgoR);
      instance.SetDestructor(&destruct_CbmTrdUnpackAlgoR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdUnpackAlgoR*)
   {
      return GenerateInitInstanceLocal((::CbmTrdUnpackAlgoR*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoR*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdUnpackAlgoLegacy2020R(void *p = nullptr);
   static void *newArray_CbmTrdUnpackAlgoLegacy2020R(Long_t size, void *p);
   static void delete_CbmTrdUnpackAlgoLegacy2020R(void *p);
   static void deleteArray_CbmTrdUnpackAlgoLegacy2020R(void *p);
   static void destruct_CbmTrdUnpackAlgoLegacy2020R(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdUnpackAlgoLegacy2020R*)
   {
      ::CbmTrdUnpackAlgoLegacy2020R *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdUnpackAlgoLegacy2020R >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdUnpackAlgoLegacy2020R", ::CbmTrdUnpackAlgoLegacy2020R::Class_Version(), "CbmTrdUnpackAlgoLegacy2020R.h", 37,
                  typeid(::CbmTrdUnpackAlgoLegacy2020R), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdUnpackAlgoLegacy2020R::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdUnpackAlgoLegacy2020R) );
      instance.SetNew(&new_CbmTrdUnpackAlgoLegacy2020R);
      instance.SetNewArray(&newArray_CbmTrdUnpackAlgoLegacy2020R);
      instance.SetDelete(&delete_CbmTrdUnpackAlgoLegacy2020R);
      instance.SetDeleteArray(&deleteArray_CbmTrdUnpackAlgoLegacy2020R);
      instance.SetDestructor(&destruct_CbmTrdUnpackAlgoLegacy2020R);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdUnpackAlgoLegacy2020R*)
   {
      return GenerateInitInstanceLocal((::CbmTrdUnpackAlgoLegacy2020R*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoLegacy2020R*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdUnpackFaspMonitor(void *p = nullptr);
   static void *newArray_CbmTrdUnpackFaspMonitor(Long_t size, void *p);
   static void delete_CbmTrdUnpackFaspMonitor(void *p);
   static void deleteArray_CbmTrdUnpackFaspMonitor(void *p);
   static void destruct_CbmTrdUnpackFaspMonitor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdUnpackFaspMonitor*)
   {
      ::CbmTrdUnpackFaspMonitor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdUnpackFaspMonitor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdUnpackFaspMonitor", ::CbmTrdUnpackFaspMonitor::Class_Version(), "CbmTrdUnpackFaspMonitor.h", 47,
                  typeid(::CbmTrdUnpackFaspMonitor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdUnpackFaspMonitor::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdUnpackFaspMonitor) );
      instance.SetNew(&new_CbmTrdUnpackFaspMonitor);
      instance.SetNewArray(&newArray_CbmTrdUnpackFaspMonitor);
      instance.SetDelete(&delete_CbmTrdUnpackFaspMonitor);
      instance.SetDeleteArray(&deleteArray_CbmTrdUnpackFaspMonitor);
      instance.SetDestructor(&destruct_CbmTrdUnpackFaspMonitor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdUnpackFaspMonitor*)
   {
      return GenerateInitInstanceLocal((::CbmTrdUnpackFaspMonitor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspMonitor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdUnpackFaspAlgo(void *p = nullptr);
   static void *newArray_CbmTrdUnpackFaspAlgo(Long_t size, void *p);
   static void delete_CbmTrdUnpackFaspAlgo(void *p);
   static void deleteArray_CbmTrdUnpackFaspAlgo(void *p);
   static void destruct_CbmTrdUnpackFaspAlgo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdUnpackFaspAlgo*)
   {
      ::CbmTrdUnpackFaspAlgo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdUnpackFaspAlgo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdUnpackFaspAlgo", ::CbmTrdUnpackFaspAlgo::Class_Version(), "CbmTrdUnpackFaspAlgo.h", 46,
                  typeid(::CbmTrdUnpackFaspAlgo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdUnpackFaspAlgo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdUnpackFaspAlgo) );
      instance.SetNew(&new_CbmTrdUnpackFaspAlgo);
      instance.SetNewArray(&newArray_CbmTrdUnpackFaspAlgo);
      instance.SetDelete(&delete_CbmTrdUnpackFaspAlgo);
      instance.SetDeleteArray(&deleteArray_CbmTrdUnpackFaspAlgo);
      instance.SetDestructor(&destruct_CbmTrdUnpackFaspAlgo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdUnpackFaspAlgo*)
   {
      return GenerateInitInstanceLocal((::CbmTrdUnpackFaspAlgo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspAlgo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrdUnpackFaspConfig(void *p);
   static void deleteArray_CbmTrdUnpackFaspConfig(void *p);
   static void destruct_CbmTrdUnpackFaspConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdUnpackFaspConfig*)
   {
      ::CbmTrdUnpackFaspConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdUnpackFaspConfig >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdUnpackFaspConfig", ::CbmTrdUnpackFaspConfig::Class_Version(), "CbmTrdUnpackFaspConfig.h", 37,
                  typeid(::CbmTrdUnpackFaspConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdUnpackFaspConfig::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdUnpackFaspConfig) );
      instance.SetDelete(&delete_CbmTrdUnpackFaspConfig);
      instance.SetDeleteArray(&deleteArray_CbmTrdUnpackFaspConfig);
      instance.SetDestructor(&destruct_CbmTrdUnpackFaspConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdUnpackFaspConfig*)
   {
      return GenerateInitInstanceLocal((::CbmTrdUnpackFaspConfig*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspConfig*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdClusterizerFastQa(void *p = nullptr);
   static void *newArray_CbmTrdClusterizerFastQa(Long_t size, void *p);
   static void delete_CbmTrdClusterizerFastQa(void *p);
   static void deleteArray_CbmTrdClusterizerFastQa(void *p);
   static void destruct_CbmTrdClusterizerFastQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdClusterizerFastQa*)
   {
      ::CbmTrdClusterizerFastQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdClusterizerFastQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdClusterizerFastQa", ::CbmTrdClusterizerFastQa::Class_Version(), "CbmTrdClusterizerFastQa.h", 14,
                  typeid(::CbmTrdClusterizerFastQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdClusterizerFastQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdClusterizerFastQa) );
      instance.SetNew(&new_CbmTrdClusterizerFastQa);
      instance.SetNewArray(&newArray_CbmTrdClusterizerFastQa);
      instance.SetDelete(&delete_CbmTrdClusterizerFastQa);
      instance.SetDeleteArray(&deleteArray_CbmTrdClusterizerFastQa);
      instance.SetDestructor(&destruct_CbmTrdClusterizerFastQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdClusterizerFastQa*)
   {
      return GenerateInitInstanceLocal((::CbmTrdClusterizerFastQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdClusterizerFastQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdHitDensityQa(void *p = nullptr);
   static void *newArray_CbmTrdHitDensityQa(Long_t size, void *p);
   static void delete_CbmTrdHitDensityQa(void *p);
   static void deleteArray_CbmTrdHitDensityQa(void *p);
   static void destruct_CbmTrdHitDensityQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdHitDensityQa*)
   {
      ::CbmTrdHitDensityQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdHitDensityQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdHitDensityQa", ::CbmTrdHitDensityQa::Class_Version(), "CbmTrdHitDensityQa.h", 34,
                  typeid(::CbmTrdHitDensityQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdHitDensityQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdHitDensityQa) );
      instance.SetNew(&new_CbmTrdHitDensityQa);
      instance.SetNewArray(&newArray_CbmTrdHitDensityQa);
      instance.SetDelete(&delete_CbmTrdHitDensityQa);
      instance.SetDeleteArray(&deleteArray_CbmTrdHitDensityQa);
      instance.SetDestructor(&destruct_CbmTrdHitDensityQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdHitDensityQa*)
   {
      return GenerateInitInstanceLocal((::CbmTrdHitDensityQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdHitDensityQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdHitProducerClusterQa(void *p = nullptr);
   static void *newArray_CbmTrdHitProducerClusterQa(Long_t size, void *p);
   static void delete_CbmTrdHitProducerClusterQa(void *p);
   static void deleteArray_CbmTrdHitProducerClusterQa(void *p);
   static void destruct_CbmTrdHitProducerClusterQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdHitProducerClusterQa*)
   {
      ::CbmTrdHitProducerClusterQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdHitProducerClusterQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdHitProducerClusterQa", ::CbmTrdHitProducerClusterQa::Class_Version(), "CbmTrdHitProducerClusterQa.h", 14,
                  typeid(::CbmTrdHitProducerClusterQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdHitProducerClusterQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdHitProducerClusterQa) );
      instance.SetNew(&new_CbmTrdHitProducerClusterQa);
      instance.SetNewArray(&newArray_CbmTrdHitProducerClusterQa);
      instance.SetDelete(&delete_CbmTrdHitProducerClusterQa);
      instance.SetDeleteArray(&deleteArray_CbmTrdHitProducerClusterQa);
      instance.SetDestructor(&destruct_CbmTrdHitProducerClusterQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdHitProducerClusterQa*)
   {
      return GenerateInitInstanceLocal((::CbmTrdHitProducerClusterQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdHitProducerClusterQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdHitProducerQa(void *p = nullptr);
   static void *newArray_CbmTrdHitProducerQa(Long_t size, void *p);
   static void delete_CbmTrdHitProducerQa(void *p);
   static void deleteArray_CbmTrdHitProducerQa(void *p);
   static void destruct_CbmTrdHitProducerQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdHitProducerQa*)
   {
      ::CbmTrdHitProducerQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdHitProducerQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdHitProducerQa", ::CbmTrdHitProducerQa::Class_Version(), "CbmTrdHitProducerQa.h", 34,
                  typeid(::CbmTrdHitProducerQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdHitProducerQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdHitProducerQa) );
      instance.SetNew(&new_CbmTrdHitProducerQa);
      instance.SetNewArray(&newArray_CbmTrdHitProducerQa);
      instance.SetDelete(&delete_CbmTrdHitProducerQa);
      instance.SetDeleteArray(&deleteArray_CbmTrdHitProducerQa);
      instance.SetDestructor(&destruct_CbmTrdHitProducerQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdHitProducerQa*)
   {
      return GenerateInitInstanceLocal((::CbmTrdHitProducerQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdHitProducerQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdOccupancyQa(void *p = nullptr);
   static void *newArray_CbmTrdOccupancyQa(Long_t size, void *p);
   static void delete_CbmTrdOccupancyQa(void *p);
   static void deleteArray_CbmTrdOccupancyQa(void *p);
   static void destruct_CbmTrdOccupancyQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdOccupancyQa*)
   {
      ::CbmTrdOccupancyQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdOccupancyQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdOccupancyQa", ::CbmTrdOccupancyQa::Class_Version(), "CbmTrdOccupancyQa.h", 82,
                  typeid(::CbmTrdOccupancyQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdOccupancyQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdOccupancyQa) );
      instance.SetNew(&new_CbmTrdOccupancyQa);
      instance.SetNewArray(&newArray_CbmTrdOccupancyQa);
      instance.SetDelete(&delete_CbmTrdOccupancyQa);
      instance.SetDeleteArray(&deleteArray_CbmTrdOccupancyQa);
      instance.SetDestructor(&destruct_CbmTrdOccupancyQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdOccupancyQa*)
   {
      return GenerateInitInstanceLocal((::CbmTrdOccupancyQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdOccupancyQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdQa(void *p = nullptr);
   static void *newArray_CbmTrdQa(Long_t size, void *p);
   static void delete_CbmTrdQa(void *p);
   static void deleteArray_CbmTrdQa(void *p);
   static void destruct_CbmTrdQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdQa*)
   {
      ::CbmTrdQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdQa", ::CbmTrdQa::Class_Version(), "CbmTrdQa.h", 29,
                  typeid(::CbmTrdQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdQa) );
      instance.SetNew(&new_CbmTrdQa);
      instance.SetNewArray(&newArray_CbmTrdQa);
      instance.SetDelete(&delete_CbmTrdQa);
      instance.SetDeleteArray(&deleteArray_CbmTrdQa);
      instance.SetDestructor(&destruct_CbmTrdQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdQa*)
   {
      return GenerateInitInstanceLocal((::CbmTrdQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdRecoQa(void *p = nullptr);
   static void *newArray_CbmTrdRecoQa(Long_t size, void *p);
   static void delete_CbmTrdRecoQa(void *p);
   static void deleteArray_CbmTrdRecoQa(void *p);
   static void destruct_CbmTrdRecoQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdRecoQa*)
   {
      ::CbmTrdRecoQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdRecoQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdRecoQa", ::CbmTrdRecoQa::Class_Version(), "CbmTrdRecoQa.h", 45,
                  typeid(::CbmTrdRecoQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdRecoQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdRecoQa) );
      instance.SetNew(&new_CbmTrdRecoQa);
      instance.SetNewArray(&newArray_CbmTrdRecoQa);
      instance.SetDelete(&delete_CbmTrdRecoQa);
      instance.SetDeleteArray(&deleteArray_CbmTrdRecoQa);
      instance.SetDestructor(&destruct_CbmTrdRecoQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdRecoQa*)
   {
      return GenerateInitInstanceLocal((::CbmTrdRecoQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdRecoQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdTracksPidQa(void *p = nullptr);
   static void *newArray_CbmTrdTracksPidQa(Long_t size, void *p);
   static void delete_CbmTrdTracksPidQa(void *p);
   static void deleteArray_CbmTrdTracksPidQa(void *p);
   static void destruct_CbmTrdTracksPidQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdTracksPidQa*)
   {
      ::CbmTrdTracksPidQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdTracksPidQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdTracksPidQa", ::CbmTrdTracksPidQa::Class_Version(), "CbmTrdTracksPidQa.h", 33,
                  typeid(::CbmTrdTracksPidQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdTracksPidQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdTracksPidQa) );
      instance.SetNew(&new_CbmTrdTracksPidQa);
      instance.SetNewArray(&newArray_CbmTrdTracksPidQa);
      instance.SetDelete(&delete_CbmTrdTracksPidQa);
      instance.SetDeleteArray(&deleteArray_CbmTrdTracksPidQa);
      instance.SetDestructor(&destruct_CbmTrdTracksPidQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdTracksPidQa*)
   {
      return GenerateInitInstanceLocal((::CbmTrdTracksPidQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdTracksPidQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdCalibTracker(void *p = nullptr);
   static void *newArray_CbmTrdCalibTracker(Long_t size, void *p);
   static void delete_CbmTrdCalibTracker(void *p);
   static void deleteArray_CbmTrdCalibTracker(void *p);
   static void destruct_CbmTrdCalibTracker(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdCalibTracker*)
   {
      ::CbmTrdCalibTracker *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdCalibTracker >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdCalibTracker", ::CbmTrdCalibTracker::Class_Version(), "CbmTrdCalibTracker.h", 43,
                  typeid(::CbmTrdCalibTracker), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdCalibTracker::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdCalibTracker) );
      instance.SetNew(&new_CbmTrdCalibTracker);
      instance.SetNewArray(&newArray_CbmTrdCalibTracker);
      instance.SetDelete(&delete_CbmTrdCalibTracker);
      instance.SetDeleteArray(&deleteArray_CbmTrdCalibTracker);
      instance.SetDestructor(&destruct_CbmTrdCalibTracker);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdCalibTracker*)
   {
      return GenerateInitInstanceLocal((::CbmTrdCalibTracker*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdCalibTracker*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdHitMC(void *p = nullptr);
   static void *newArray_CbmTrdHitMC(Long_t size, void *p);
   static void delete_CbmTrdHitMC(void *p);
   static void deleteArray_CbmTrdHitMC(void *p);
   static void destruct_CbmTrdHitMC(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdHitMC*)
   {
      ::CbmTrdHitMC *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdHitMC >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdHitMC", ::CbmTrdHitMC::Class_Version(), "CbmTrdHitMC.h", 28,
                  typeid(::CbmTrdHitMC), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdHitMC::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdHitMC) );
      instance.SetNew(&new_CbmTrdHitMC);
      instance.SetNewArray(&newArray_CbmTrdHitMC);
      instance.SetDelete(&delete_CbmTrdHitMC);
      instance.SetDeleteArray(&deleteArray_CbmTrdHitMC);
      instance.SetDestructor(&destruct_CbmTrdHitMC);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdHitMC*)
   {
      return GenerateInitInstanceLocal((::CbmTrdHitMC*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdHitMC*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrdElectronsTrainAnn(void *p);
   static void deleteArray_CbmTrdElectronsTrainAnn(void *p);
   static void destruct_CbmTrdElectronsTrainAnn(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdElectronsTrainAnn*)
   {
      ::CbmTrdElectronsTrainAnn *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdElectronsTrainAnn >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdElectronsTrainAnn", ::CbmTrdElectronsTrainAnn::Class_Version(), "CbmTrdElectronsTrainAnn.h", 57,
                  typeid(::CbmTrdElectronsTrainAnn), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdElectronsTrainAnn::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdElectronsTrainAnn) );
      instance.SetDelete(&delete_CbmTrdElectronsTrainAnn);
      instance.SetDeleteArray(&deleteArray_CbmTrdElectronsTrainAnn);
      instance.SetDestructor(&destruct_CbmTrdElectronsTrainAnn);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdElectronsTrainAnn*)
   {
      return GenerateInitInstanceLocal((::CbmTrdElectronsTrainAnn*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdElectronsTrainAnn*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdSetTracksPidWkn(void *p = nullptr);
   static void *newArray_CbmTrdSetTracksPidWkn(Long_t size, void *p);
   static void delete_CbmTrdSetTracksPidWkn(void *p);
   static void deleteArray_CbmTrdSetTracksPidWkn(void *p);
   static void destruct_CbmTrdSetTracksPidWkn(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdSetTracksPidWkn*)
   {
      ::CbmTrdSetTracksPidWkn *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdSetTracksPidWkn >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdSetTracksPidWkn", ::CbmTrdSetTracksPidWkn::Class_Version(), "CbmTrdSetTracksPidWkn.h", 40,
                  typeid(::CbmTrdSetTracksPidWkn), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdSetTracksPidWkn::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdSetTracksPidWkn) );
      instance.SetNew(&new_CbmTrdSetTracksPidWkn);
      instance.SetNewArray(&newArray_CbmTrdSetTracksPidWkn);
      instance.SetDelete(&delete_CbmTrdSetTracksPidWkn);
      instance.SetDeleteArray(&deleteArray_CbmTrdSetTracksPidWkn);
      instance.SetDestructor(&destruct_CbmTrdSetTracksPidWkn);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdSetTracksPidWkn*)
   {
      return GenerateInitInstanceLocal((::CbmTrdSetTracksPidWkn*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidWkn*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdSetTracksPidModWkn(void *p = nullptr);
   static void *newArray_CbmTrdSetTracksPidModWkn(Long_t size, void *p);
   static void delete_CbmTrdSetTracksPidModWkn(void *p);
   static void deleteArray_CbmTrdSetTracksPidModWkn(void *p);
   static void destruct_CbmTrdSetTracksPidModWkn(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdSetTracksPidModWkn*)
   {
      ::CbmTrdSetTracksPidModWkn *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdSetTracksPidModWkn >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdSetTracksPidModWkn", ::CbmTrdSetTracksPidModWkn::Class_Version(), "CbmTrdSetTracksPidModWkn.h", 43,
                  typeid(::CbmTrdSetTracksPidModWkn), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdSetTracksPidModWkn::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdSetTracksPidModWkn) );
      instance.SetNew(&new_CbmTrdSetTracksPidModWkn);
      instance.SetNewArray(&newArray_CbmTrdSetTracksPidModWkn);
      instance.SetDelete(&delete_CbmTrdSetTracksPidModWkn);
      instance.SetDeleteArray(&deleteArray_CbmTrdSetTracksPidModWkn);
      instance.SetDestructor(&destruct_CbmTrdSetTracksPidModWkn);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdSetTracksPidModWkn*)
   {
      return GenerateInitInstanceLocal((::CbmTrdSetTracksPidModWkn*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidModWkn*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdSetTracksPidLike(void *p = nullptr);
   static void *newArray_CbmTrdSetTracksPidLike(Long_t size, void *p);
   static void delete_CbmTrdSetTracksPidLike(void *p);
   static void deleteArray_CbmTrdSetTracksPidLike(void *p);
   static void destruct_CbmTrdSetTracksPidLike(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdSetTracksPidLike*)
   {
      ::CbmTrdSetTracksPidLike *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdSetTracksPidLike >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdSetTracksPidLike", ::CbmTrdSetTracksPidLike::Class_Version(), "CbmTrdSetTracksPidLike.h", 36,
                  typeid(::CbmTrdSetTracksPidLike), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdSetTracksPidLike::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdSetTracksPidLike) );
      instance.SetNew(&new_CbmTrdSetTracksPidLike);
      instance.SetNewArray(&newArray_CbmTrdSetTracksPidLike);
      instance.SetDelete(&delete_CbmTrdSetTracksPidLike);
      instance.SetDeleteArray(&deleteArray_CbmTrdSetTracksPidLike);
      instance.SetDestructor(&destruct_CbmTrdSetTracksPidLike);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdSetTracksPidLike*)
   {
      return GenerateInitInstanceLocal((::CbmTrdSetTracksPidLike*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidLike*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdSetTracksPidANN(void *p = nullptr);
   static void *newArray_CbmTrdSetTracksPidANN(Long_t size, void *p);
   static void delete_CbmTrdSetTracksPidANN(void *p);
   static void deleteArray_CbmTrdSetTracksPidANN(void *p);
   static void destruct_CbmTrdSetTracksPidANN(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdSetTracksPidANN*)
   {
      ::CbmTrdSetTracksPidANN *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdSetTracksPidANN >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdSetTracksPidANN", ::CbmTrdSetTracksPidANN::Class_Version(), "CbmTrdSetTracksPidANN.h", 36,
                  typeid(::CbmTrdSetTracksPidANN), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdSetTracksPidANN::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdSetTracksPidANN) );
      instance.SetNew(&new_CbmTrdSetTracksPidANN);
      instance.SetNewArray(&newArray_CbmTrdSetTracksPidANN);
      instance.SetDelete(&delete_CbmTrdSetTracksPidANN);
      instance.SetDeleteArray(&deleteArray_CbmTrdSetTracksPidANN);
      instance.SetDestructor(&destruct_CbmTrdSetTracksPidANN);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdSetTracksPidANN*)
   {
      return GenerateInitInstanceLocal((::CbmTrdSetTracksPidANN*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidANN*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdRawToDigiFitR(void *p = nullptr);
   static void *newArray_CbmTrdRawToDigiFitR(Long_t size, void *p);
   static void delete_CbmTrdRawToDigiFitR(void *p);
   static void deleteArray_CbmTrdRawToDigiFitR(void *p);
   static void destruct_CbmTrdRawToDigiFitR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdRawToDigiFitR*)
   {
      ::CbmTrdRawToDigiFitR *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdRawToDigiFitR >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdRawToDigiFitR", ::CbmTrdRawToDigiFitR::Class_Version(), "CbmTrdRawToDigiFitR.h", 31,
                  typeid(::CbmTrdRawToDigiFitR), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdRawToDigiFitR::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdRawToDigiFitR) );
      instance.SetNew(&new_CbmTrdRawToDigiFitR);
      instance.SetNewArray(&newArray_CbmTrdRawToDigiFitR);
      instance.SetDelete(&delete_CbmTrdRawToDigiFitR);
      instance.SetDeleteArray(&deleteArray_CbmTrdRawToDigiFitR);
      instance.SetDestructor(&destruct_CbmTrdRawToDigiFitR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdRawToDigiFitR*)
   {
      return GenerateInitInstanceLocal((::CbmTrdRawToDigiFitR*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdRawToDigiFitR*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdRawToDigiLookUpCorrR(void *p = nullptr);
   static void *newArray_CbmTrdRawToDigiLookUpCorrR(Long_t size, void *p);
   static void delete_CbmTrdRawToDigiLookUpCorrR(void *p);
   static void deleteArray_CbmTrdRawToDigiLookUpCorrR(void *p);
   static void destruct_CbmTrdRawToDigiLookUpCorrR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdRawToDigiLookUpCorrR*)
   {
      ::CbmTrdRawToDigiLookUpCorrR *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdRawToDigiLookUpCorrR >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdRawToDigiLookUpCorrR", ::CbmTrdRawToDigiLookUpCorrR::Class_Version(), "CbmTrdRawToDigiLookUpCorrR.h", 36,
                  typeid(::CbmTrdRawToDigiLookUpCorrR), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdRawToDigiLookUpCorrR::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdRawToDigiLookUpCorrR) );
      instance.SetNew(&new_CbmTrdRawToDigiLookUpCorrR);
      instance.SetNewArray(&newArray_CbmTrdRawToDigiLookUpCorrR);
      instance.SetDelete(&delete_CbmTrdRawToDigiLookUpCorrR);
      instance.SetDeleteArray(&deleteArray_CbmTrdRawToDigiLookUpCorrR);
      instance.SetDestructor(&destruct_CbmTrdRawToDigiLookUpCorrR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdRawToDigiLookUpCorrR*)
   {
      return GenerateInitInstanceLocal((::CbmTrdRawToDigiLookUpCorrR*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdRawToDigiLookUpCorrR*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdTrackFinderIdeal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdTrackFinderIdeal::Class_Name()
{
   return "CbmTrdTrackFinderIdeal";
}

//______________________________________________________________________________
const char *CbmTrdTrackFinderIdeal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFinderIdeal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdTrackFinderIdeal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFinderIdeal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdTrackFinderIdeal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFinderIdeal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdTrackFinderIdeal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFinderIdeal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdTrackFitterIdeal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdTrackFitterIdeal::Class_Name()
{
   return "CbmTrdTrackFitterIdeal";
}

//______________________________________________________________________________
const char *CbmTrdTrackFitterIdeal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFitterIdeal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdTrackFitterIdeal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFitterIdeal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdTrackFitterIdeal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFitterIdeal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdTrackFitterIdeal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFitterIdeal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdClusterFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdClusterFinder::Class_Name()
{
   return "CbmTrdClusterFinder";
}

//______________________________________________________________________________
const char *CbmTrdClusterFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdClusterFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdClusterFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdClusterFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdClusterFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdClusterFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdClusterFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdClusterFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdHitProducer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdHitProducer::Class_Name()
{
   return "CbmTrdHitProducer";
}

//______________________________________________________________________________
const char *CbmTrdHitProducer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitProducer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdHitProducer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitProducer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdHitProducer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitProducer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdHitProducer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitProducer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdModuleRec::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdModuleRec::Class_Name()
{
   return "CbmTrdModuleRec";
}

//______________________________________________________________________________
const char *CbmTrdModuleRec::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleRec*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdModuleRec::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleRec*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdModuleRec::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleRec*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdModuleRec::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleRec*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdModuleRecR::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdModuleRecR::Class_Name()
{
   return "CbmTrdModuleRecR";
}

//______________________________________________________________________________
const char *CbmTrdModuleRecR::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleRecR*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdModuleRecR::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleRecR*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdModuleRecR::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleRecR*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdModuleRecR::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleRecR*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdModuleRec2D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdModuleRec2D::Class_Name()
{
   return "CbmTrdModuleRec2D";
}

//______________________________________________________________________________
const char *CbmTrdModuleRec2D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleRec2D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdModuleRec2D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleRec2D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdModuleRec2D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleRec2D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdModuleRec2D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleRec2D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdDigiRec::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdDigiRec::Class_Name()
{
   return "CbmTrdDigiRec";
}

//______________________________________________________________________________
const char *CbmTrdDigiRec::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigiRec*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdDigiRec::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigiRec*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdDigiRec::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigiRec*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdDigiRec::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigiRec*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdRawToDigiBaseR::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdRawToDigiBaseR::Class_Name()
{
   return "CbmTrdRawToDigiBaseR";
}

//______________________________________________________________________________
const char *CbmTrdRawToDigiBaseR::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiBaseR*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdRawToDigiBaseR::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiBaseR*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdRawToDigiBaseR::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiBaseR*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdRawToDigiBaseR::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiBaseR*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdRawToDigiMaxAdcR::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdRawToDigiMaxAdcR::Class_Name()
{
   return "CbmTrdRawToDigiMaxAdcR";
}

//______________________________________________________________________________
const char *CbmTrdRawToDigiMaxAdcR::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiMaxAdcR*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdRawToDigiMaxAdcR::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiMaxAdcR*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdRawToDigiMaxAdcR::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiMaxAdcR*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdRawToDigiMaxAdcR::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiMaxAdcR*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdUnpackMonitor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdUnpackMonitor::Class_Name()
{
   return "CbmTrdUnpackMonitor";
}

//______________________________________________________________________________
const char *CbmTrdUnpackMonitor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackMonitor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdUnpackMonitor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackMonitor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdUnpackMonitor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackMonitor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdUnpackMonitor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackMonitor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdUnpackAlgoBaseR::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdUnpackAlgoBaseR::Class_Name()
{
   return "CbmTrdUnpackAlgoBaseR";
}

//______________________________________________________________________________
const char *CbmTrdUnpackAlgoBaseR::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoBaseR*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdUnpackAlgoBaseR::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoBaseR*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdUnpackAlgoBaseR::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoBaseR*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdUnpackAlgoBaseR::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoBaseR*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdUnpackConfig::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdUnpackConfig::Class_Name()
{
   return "CbmTrdUnpackConfig";
}

//______________________________________________________________________________
const char *CbmTrdUnpackConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackConfig*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdUnpackConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackConfig*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdUnpackConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackConfig*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdUnpackConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackConfig*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdUnpackAlgoR::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdUnpackAlgoR::Class_Name()
{
   return "CbmTrdUnpackAlgoR";
}

//______________________________________________________________________________
const char *CbmTrdUnpackAlgoR::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoR*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdUnpackAlgoR::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoR*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdUnpackAlgoR::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoR*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdUnpackAlgoR::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoR*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdUnpackAlgoLegacy2020R::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdUnpackAlgoLegacy2020R::Class_Name()
{
   return "CbmTrdUnpackAlgoLegacy2020R";
}

//______________________________________________________________________________
const char *CbmTrdUnpackAlgoLegacy2020R::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoLegacy2020R*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdUnpackAlgoLegacy2020R::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoLegacy2020R*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdUnpackAlgoLegacy2020R::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoLegacy2020R*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdUnpackAlgoLegacy2020R::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackAlgoLegacy2020R*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdUnpackFaspMonitor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdUnpackFaspMonitor::Class_Name()
{
   return "CbmTrdUnpackFaspMonitor";
}

//______________________________________________________________________________
const char *CbmTrdUnpackFaspMonitor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspMonitor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdUnpackFaspMonitor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspMonitor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdUnpackFaspMonitor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspMonitor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdUnpackFaspMonitor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspMonitor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdUnpackFaspAlgo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdUnpackFaspAlgo::Class_Name()
{
   return "CbmTrdUnpackFaspAlgo";
}

//______________________________________________________________________________
const char *CbmTrdUnpackFaspAlgo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspAlgo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdUnpackFaspAlgo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspAlgo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdUnpackFaspAlgo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspAlgo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdUnpackFaspAlgo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspAlgo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdUnpackFaspConfig::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdUnpackFaspConfig::Class_Name()
{
   return "CbmTrdUnpackFaspConfig";
}

//______________________________________________________________________________
const char *CbmTrdUnpackFaspConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspConfig*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdUnpackFaspConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspConfig*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdUnpackFaspConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspConfig*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdUnpackFaspConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUnpackFaspConfig*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdClusterizerFastQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdClusterizerFastQa::Class_Name()
{
   return "CbmTrdClusterizerFastQa";
}

//______________________________________________________________________________
const char *CbmTrdClusterizerFastQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdClusterizerFastQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdClusterizerFastQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdClusterizerFastQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdClusterizerFastQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdClusterizerFastQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdClusterizerFastQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdClusterizerFastQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdHitDensityQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdHitDensityQa::Class_Name()
{
   return "CbmTrdHitDensityQa";
}

//______________________________________________________________________________
const char *CbmTrdHitDensityQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitDensityQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdHitDensityQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitDensityQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdHitDensityQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitDensityQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdHitDensityQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitDensityQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdHitProducerClusterQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdHitProducerClusterQa::Class_Name()
{
   return "CbmTrdHitProducerClusterQa";
}

//______________________________________________________________________________
const char *CbmTrdHitProducerClusterQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitProducerClusterQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdHitProducerClusterQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitProducerClusterQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdHitProducerClusterQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitProducerClusterQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdHitProducerClusterQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitProducerClusterQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdHitProducerQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdHitProducerQa::Class_Name()
{
   return "CbmTrdHitProducerQa";
}

//______________________________________________________________________________
const char *CbmTrdHitProducerQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitProducerQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdHitProducerQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitProducerQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdHitProducerQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitProducerQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdHitProducerQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitProducerQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdOccupancyQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdOccupancyQa::Class_Name()
{
   return "CbmTrdOccupancyQa";
}

//______________________________________________________________________________
const char *CbmTrdOccupancyQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdOccupancyQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdOccupancyQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdOccupancyQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdOccupancyQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdOccupancyQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdOccupancyQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdOccupancyQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdQa::Class_Name()
{
   return "CbmTrdQa";
}

//______________________________________________________________________________
const char *CbmTrdQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdRecoQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdRecoQa::Class_Name()
{
   return "CbmTrdRecoQa";
}

//______________________________________________________________________________
const char *CbmTrdRecoQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRecoQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdRecoQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRecoQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdRecoQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRecoQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdRecoQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRecoQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdTracksPidQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdTracksPidQa::Class_Name()
{
   return "CbmTrdTracksPidQa";
}

//______________________________________________________________________________
const char *CbmTrdTracksPidQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTracksPidQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdTracksPidQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTracksPidQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdTracksPidQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTracksPidQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdTracksPidQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTracksPidQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdCalibTracker::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdCalibTracker::Class_Name()
{
   return "CbmTrdCalibTracker";
}

//______________________________________________________________________________
const char *CbmTrdCalibTracker::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdCalibTracker*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdCalibTracker::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdCalibTracker*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdCalibTracker::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdCalibTracker*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdCalibTracker::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdCalibTracker*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdHitMC::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdHitMC::Class_Name()
{
   return "CbmTrdHitMC";
}

//______________________________________________________________________________
const char *CbmTrdHitMC::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitMC*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdHitMC::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitMC*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdHitMC::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitMC*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdHitMC::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitMC*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdElectronsTrainAnn::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdElectronsTrainAnn::Class_Name()
{
   return "CbmTrdElectronsTrainAnn";
}

//______________________________________________________________________________
const char *CbmTrdElectronsTrainAnn::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdElectronsTrainAnn*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdElectronsTrainAnn::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdElectronsTrainAnn*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdElectronsTrainAnn::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdElectronsTrainAnn*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdElectronsTrainAnn::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdElectronsTrainAnn*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdSetTracksPidWkn::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdSetTracksPidWkn::Class_Name()
{
   return "CbmTrdSetTracksPidWkn";
}

//______________________________________________________________________________
const char *CbmTrdSetTracksPidWkn::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidWkn*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdSetTracksPidWkn::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidWkn*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdSetTracksPidWkn::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidWkn*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdSetTracksPidWkn::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidWkn*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdSetTracksPidModWkn::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdSetTracksPidModWkn::Class_Name()
{
   return "CbmTrdSetTracksPidModWkn";
}

//______________________________________________________________________________
const char *CbmTrdSetTracksPidModWkn::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidModWkn*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdSetTracksPidModWkn::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidModWkn*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdSetTracksPidModWkn::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidModWkn*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdSetTracksPidModWkn::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidModWkn*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdSetTracksPidLike::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdSetTracksPidLike::Class_Name()
{
   return "CbmTrdSetTracksPidLike";
}

//______________________________________________________________________________
const char *CbmTrdSetTracksPidLike::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidLike*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdSetTracksPidLike::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidLike*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdSetTracksPidLike::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidLike*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdSetTracksPidLike::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidLike*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdSetTracksPidANN::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdSetTracksPidANN::Class_Name()
{
   return "CbmTrdSetTracksPidANN";
}

//______________________________________________________________________________
const char *CbmTrdSetTracksPidANN::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidANN*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdSetTracksPidANN::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidANN*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdSetTracksPidANN::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidANN*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdSetTracksPidANN::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSetTracksPidANN*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdRawToDigiFitR::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdRawToDigiFitR::Class_Name()
{
   return "CbmTrdRawToDigiFitR";
}

//______________________________________________________________________________
const char *CbmTrdRawToDigiFitR::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiFitR*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdRawToDigiFitR::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiFitR*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdRawToDigiFitR::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiFitR*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdRawToDigiFitR::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiFitR*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdRawToDigiLookUpCorrR::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdRawToDigiLookUpCorrR::Class_Name()
{
   return "CbmTrdRawToDigiLookUpCorrR";
}

//______________________________________________________________________________
const char *CbmTrdRawToDigiLookUpCorrR::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiLookUpCorrR*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdRawToDigiLookUpCorrR::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiLookUpCorrR*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdRawToDigiLookUpCorrR::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiLookUpCorrR*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdRawToDigiLookUpCorrR::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiLookUpCorrR*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmTrdTrackFinderIdeal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdTrackFinderIdeal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdTrackFinderIdeal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdTrackFinderIdeal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdTrackFinderIdeal(void *p) {
      return  p ? new(p) ::CbmTrdTrackFinderIdeal : new ::CbmTrdTrackFinderIdeal;
   }
   static void *newArray_CbmTrdTrackFinderIdeal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdTrackFinderIdeal[nElements] : new ::CbmTrdTrackFinderIdeal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdTrackFinderIdeal(void *p) {
      delete ((::CbmTrdTrackFinderIdeal*)p);
   }
   static void deleteArray_CbmTrdTrackFinderIdeal(void *p) {
      delete [] ((::CbmTrdTrackFinderIdeal*)p);
   }
   static void destruct_CbmTrdTrackFinderIdeal(void *p) {
      typedef ::CbmTrdTrackFinderIdeal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdTrackFinderIdeal

//______________________________________________________________________________
void CbmTrdTrackFitterIdeal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdTrackFitterIdeal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdTrackFitterIdeal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdTrackFitterIdeal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdTrackFitterIdeal(void *p) {
      return  p ? new(p) ::CbmTrdTrackFitterIdeal : new ::CbmTrdTrackFitterIdeal;
   }
   static void *newArray_CbmTrdTrackFitterIdeal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdTrackFitterIdeal[nElements] : new ::CbmTrdTrackFitterIdeal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdTrackFitterIdeal(void *p) {
      delete ((::CbmTrdTrackFitterIdeal*)p);
   }
   static void deleteArray_CbmTrdTrackFitterIdeal(void *p) {
      delete [] ((::CbmTrdTrackFitterIdeal*)p);
   }
   static void destruct_CbmTrdTrackFitterIdeal(void *p) {
      typedef ::CbmTrdTrackFitterIdeal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdTrackFitterIdeal

//______________________________________________________________________________
void CbmTrdClusterFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdClusterFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdClusterFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdClusterFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdClusterFinder(void *p) {
      return  p ? new(p) ::CbmTrdClusterFinder : new ::CbmTrdClusterFinder;
   }
   static void *newArray_CbmTrdClusterFinder(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdClusterFinder[nElements] : new ::CbmTrdClusterFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdClusterFinder(void *p) {
      delete ((::CbmTrdClusterFinder*)p);
   }
   static void deleteArray_CbmTrdClusterFinder(void *p) {
      delete [] ((::CbmTrdClusterFinder*)p);
   }
   static void destruct_CbmTrdClusterFinder(void *p) {
      typedef ::CbmTrdClusterFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdClusterFinder

//______________________________________________________________________________
void CbmTrdHitProducer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdHitProducer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdHitProducer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdHitProducer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdHitProducer(void *p) {
      return  p ? new(p) ::CbmTrdHitProducer : new ::CbmTrdHitProducer;
   }
   static void *newArray_CbmTrdHitProducer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdHitProducer[nElements] : new ::CbmTrdHitProducer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdHitProducer(void *p) {
      delete ((::CbmTrdHitProducer*)p);
   }
   static void deleteArray_CbmTrdHitProducer(void *p) {
      delete [] ((::CbmTrdHitProducer*)p);
   }
   static void destruct_CbmTrdHitProducer(void *p) {
      typedef ::CbmTrdHitProducer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdHitProducer

//______________________________________________________________________________
void CbmTrdModuleRec::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdModuleRec.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdModuleRec::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdModuleRec::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrdModuleRec(void *p) {
      delete ((::CbmTrdModuleRec*)p);
   }
   static void deleteArray_CbmTrdModuleRec(void *p) {
      delete [] ((::CbmTrdModuleRec*)p);
   }
   static void destruct_CbmTrdModuleRec(void *p) {
      typedef ::CbmTrdModuleRec current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdModuleRec

//______________________________________________________________________________
void CbmTrdModuleRecR::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdModuleRecR.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdModuleRecR::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdModuleRecR::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdModuleRecR(void *p) {
      return  p ? new(p) ::CbmTrdModuleRecR : new ::CbmTrdModuleRecR;
   }
   static void *newArray_CbmTrdModuleRecR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdModuleRecR[nElements] : new ::CbmTrdModuleRecR[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdModuleRecR(void *p) {
      delete ((::CbmTrdModuleRecR*)p);
   }
   static void deleteArray_CbmTrdModuleRecR(void *p) {
      delete [] ((::CbmTrdModuleRecR*)p);
   }
   static void destruct_CbmTrdModuleRecR(void *p) {
      typedef ::CbmTrdModuleRecR current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdModuleRecR

//______________________________________________________________________________
void CbmTrdModuleRec2D::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdModuleRec2D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdModuleRec2D::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdModuleRec2D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdModuleRec2D(void *p) {
      return  p ? new(p) ::CbmTrdModuleRec2D : new ::CbmTrdModuleRec2D;
   }
   static void *newArray_CbmTrdModuleRec2D(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdModuleRec2D[nElements] : new ::CbmTrdModuleRec2D[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdModuleRec2D(void *p) {
      delete ((::CbmTrdModuleRec2D*)p);
   }
   static void deleteArray_CbmTrdModuleRec2D(void *p) {
      delete [] ((::CbmTrdModuleRec2D*)p);
   }
   static void destruct_CbmTrdModuleRec2D(void *p) {
      typedef ::CbmTrdModuleRec2D current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdModuleRec2D

//______________________________________________________________________________
void CbmTrdDigiRec::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdDigiRec.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdDigiRec::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdDigiRec::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdDigiRec(void *p) {
      return  p ? new(p) ::CbmTrdDigiRec : new ::CbmTrdDigiRec;
   }
   static void *newArray_CbmTrdDigiRec(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdDigiRec[nElements] : new ::CbmTrdDigiRec[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdDigiRec(void *p) {
      delete ((::CbmTrdDigiRec*)p);
   }
   static void deleteArray_CbmTrdDigiRec(void *p) {
      delete [] ((::CbmTrdDigiRec*)p);
   }
   static void destruct_CbmTrdDigiRec(void *p) {
      typedef ::CbmTrdDigiRec current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdDigiRec

//______________________________________________________________________________
void CbmTrdRawToDigiBaseR::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdRawToDigiBaseR.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdRawToDigiBaseR::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdRawToDigiBaseR::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrdRawToDigiBaseR(void *p) {
      delete ((::CbmTrdRawToDigiBaseR*)p);
   }
   static void deleteArray_CbmTrdRawToDigiBaseR(void *p) {
      delete [] ((::CbmTrdRawToDigiBaseR*)p);
   }
   static void destruct_CbmTrdRawToDigiBaseR(void *p) {
      typedef ::CbmTrdRawToDigiBaseR current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdRawToDigiBaseR

//______________________________________________________________________________
void CbmTrdRawToDigiMaxAdcR::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdRawToDigiMaxAdcR.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdRawToDigiMaxAdcR::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdRawToDigiMaxAdcR::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdRawToDigiMaxAdcR(void *p) {
      return  p ? new(p) ::CbmTrdRawToDigiMaxAdcR : new ::CbmTrdRawToDigiMaxAdcR;
   }
   static void *newArray_CbmTrdRawToDigiMaxAdcR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdRawToDigiMaxAdcR[nElements] : new ::CbmTrdRawToDigiMaxAdcR[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdRawToDigiMaxAdcR(void *p) {
      delete ((::CbmTrdRawToDigiMaxAdcR*)p);
   }
   static void deleteArray_CbmTrdRawToDigiMaxAdcR(void *p) {
      delete [] ((::CbmTrdRawToDigiMaxAdcR*)p);
   }
   static void destruct_CbmTrdRawToDigiMaxAdcR(void *p) {
      typedef ::CbmTrdRawToDigiMaxAdcR current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdRawToDigiMaxAdcR

//______________________________________________________________________________
void CbmTrdUnpackMonitor::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdUnpackMonitor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdUnpackMonitor::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdUnpackMonitor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdUnpackMonitor(void *p) {
      return  p ? new(p) ::CbmTrdUnpackMonitor : new ::CbmTrdUnpackMonitor;
   }
   static void *newArray_CbmTrdUnpackMonitor(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdUnpackMonitor[nElements] : new ::CbmTrdUnpackMonitor[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdUnpackMonitor(void *p) {
      delete ((::CbmTrdUnpackMonitor*)p);
   }
   static void deleteArray_CbmTrdUnpackMonitor(void *p) {
      delete [] ((::CbmTrdUnpackMonitor*)p);
   }
   static void destruct_CbmTrdUnpackMonitor(void *p) {
      typedef ::CbmTrdUnpackMonitor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdUnpackMonitor

//______________________________________________________________________________
void CbmTrdUnpackAlgoBaseR::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdUnpackAlgoBaseR.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdUnpackAlgoBaseR::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdUnpackAlgoBaseR::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrdUnpackAlgoBaseR(void *p) {
      delete ((::CbmTrdUnpackAlgoBaseR*)p);
   }
   static void deleteArray_CbmTrdUnpackAlgoBaseR(void *p) {
      delete [] ((::CbmTrdUnpackAlgoBaseR*)p);
   }
   static void destruct_CbmTrdUnpackAlgoBaseR(void *p) {
      typedef ::CbmTrdUnpackAlgoBaseR current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdUnpackAlgoBaseR

//______________________________________________________________________________
void CbmTrdUnpackConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdUnpackConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdUnpackConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdUnpackConfig::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrdUnpackConfig(void *p) {
      delete ((::CbmTrdUnpackConfig*)p);
   }
   static void deleteArray_CbmTrdUnpackConfig(void *p) {
      delete [] ((::CbmTrdUnpackConfig*)p);
   }
   static void destruct_CbmTrdUnpackConfig(void *p) {
      typedef ::CbmTrdUnpackConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdUnpackConfig

//______________________________________________________________________________
void CbmTrdUnpackAlgoR::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdUnpackAlgoR.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdUnpackAlgoR::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdUnpackAlgoR::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdUnpackAlgoR(void *p) {
      return  p ? new(p) ::CbmTrdUnpackAlgoR : new ::CbmTrdUnpackAlgoR;
   }
   static void *newArray_CbmTrdUnpackAlgoR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdUnpackAlgoR[nElements] : new ::CbmTrdUnpackAlgoR[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdUnpackAlgoR(void *p) {
      delete ((::CbmTrdUnpackAlgoR*)p);
   }
   static void deleteArray_CbmTrdUnpackAlgoR(void *p) {
      delete [] ((::CbmTrdUnpackAlgoR*)p);
   }
   static void destruct_CbmTrdUnpackAlgoR(void *p) {
      typedef ::CbmTrdUnpackAlgoR current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdUnpackAlgoR

//______________________________________________________________________________
void CbmTrdUnpackAlgoLegacy2020R::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdUnpackAlgoLegacy2020R.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdUnpackAlgoLegacy2020R::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdUnpackAlgoLegacy2020R::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdUnpackAlgoLegacy2020R(void *p) {
      return  p ? new(p) ::CbmTrdUnpackAlgoLegacy2020R : new ::CbmTrdUnpackAlgoLegacy2020R;
   }
   static void *newArray_CbmTrdUnpackAlgoLegacy2020R(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdUnpackAlgoLegacy2020R[nElements] : new ::CbmTrdUnpackAlgoLegacy2020R[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdUnpackAlgoLegacy2020R(void *p) {
      delete ((::CbmTrdUnpackAlgoLegacy2020R*)p);
   }
   static void deleteArray_CbmTrdUnpackAlgoLegacy2020R(void *p) {
      delete [] ((::CbmTrdUnpackAlgoLegacy2020R*)p);
   }
   static void destruct_CbmTrdUnpackAlgoLegacy2020R(void *p) {
      typedef ::CbmTrdUnpackAlgoLegacy2020R current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdUnpackAlgoLegacy2020R

//______________________________________________________________________________
void CbmTrdUnpackFaspMonitor::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdUnpackFaspMonitor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdUnpackFaspMonitor::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdUnpackFaspMonitor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdUnpackFaspMonitor(void *p) {
      return  p ? new(p) ::CbmTrdUnpackFaspMonitor : new ::CbmTrdUnpackFaspMonitor;
   }
   static void *newArray_CbmTrdUnpackFaspMonitor(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdUnpackFaspMonitor[nElements] : new ::CbmTrdUnpackFaspMonitor[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdUnpackFaspMonitor(void *p) {
      delete ((::CbmTrdUnpackFaspMonitor*)p);
   }
   static void deleteArray_CbmTrdUnpackFaspMonitor(void *p) {
      delete [] ((::CbmTrdUnpackFaspMonitor*)p);
   }
   static void destruct_CbmTrdUnpackFaspMonitor(void *p) {
      typedef ::CbmTrdUnpackFaspMonitor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdUnpackFaspMonitor

//______________________________________________________________________________
void CbmTrdUnpackFaspAlgo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdUnpackFaspAlgo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdUnpackFaspAlgo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdUnpackFaspAlgo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdUnpackFaspAlgo(void *p) {
      return  p ? new(p) ::CbmTrdUnpackFaspAlgo : new ::CbmTrdUnpackFaspAlgo;
   }
   static void *newArray_CbmTrdUnpackFaspAlgo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdUnpackFaspAlgo[nElements] : new ::CbmTrdUnpackFaspAlgo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdUnpackFaspAlgo(void *p) {
      delete ((::CbmTrdUnpackFaspAlgo*)p);
   }
   static void deleteArray_CbmTrdUnpackFaspAlgo(void *p) {
      delete [] ((::CbmTrdUnpackFaspAlgo*)p);
   }
   static void destruct_CbmTrdUnpackFaspAlgo(void *p) {
      typedef ::CbmTrdUnpackFaspAlgo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdUnpackFaspAlgo

//______________________________________________________________________________
void CbmTrdUnpackFaspConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdUnpackFaspConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdUnpackFaspConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdUnpackFaspConfig::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrdUnpackFaspConfig(void *p) {
      delete ((::CbmTrdUnpackFaspConfig*)p);
   }
   static void deleteArray_CbmTrdUnpackFaspConfig(void *p) {
      delete [] ((::CbmTrdUnpackFaspConfig*)p);
   }
   static void destruct_CbmTrdUnpackFaspConfig(void *p) {
      typedef ::CbmTrdUnpackFaspConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdUnpackFaspConfig

//______________________________________________________________________________
void CbmTrdClusterizerFastQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdClusterizerFastQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdClusterizerFastQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdClusterizerFastQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdClusterizerFastQa(void *p) {
      return  p ? new(p) ::CbmTrdClusterizerFastQa : new ::CbmTrdClusterizerFastQa;
   }
   static void *newArray_CbmTrdClusterizerFastQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdClusterizerFastQa[nElements] : new ::CbmTrdClusterizerFastQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdClusterizerFastQa(void *p) {
      delete ((::CbmTrdClusterizerFastQa*)p);
   }
   static void deleteArray_CbmTrdClusterizerFastQa(void *p) {
      delete [] ((::CbmTrdClusterizerFastQa*)p);
   }
   static void destruct_CbmTrdClusterizerFastQa(void *p) {
      typedef ::CbmTrdClusterizerFastQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdClusterizerFastQa

//______________________________________________________________________________
void CbmTrdHitDensityQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdHitDensityQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdHitDensityQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdHitDensityQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdHitDensityQa(void *p) {
      return  p ? new(p) ::CbmTrdHitDensityQa : new ::CbmTrdHitDensityQa;
   }
   static void *newArray_CbmTrdHitDensityQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdHitDensityQa[nElements] : new ::CbmTrdHitDensityQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdHitDensityQa(void *p) {
      delete ((::CbmTrdHitDensityQa*)p);
   }
   static void deleteArray_CbmTrdHitDensityQa(void *p) {
      delete [] ((::CbmTrdHitDensityQa*)p);
   }
   static void destruct_CbmTrdHitDensityQa(void *p) {
      typedef ::CbmTrdHitDensityQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdHitDensityQa

//______________________________________________________________________________
void CbmTrdHitProducerClusterQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdHitProducerClusterQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdHitProducerClusterQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdHitProducerClusterQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdHitProducerClusterQa(void *p) {
      return  p ? new(p) ::CbmTrdHitProducerClusterQa : new ::CbmTrdHitProducerClusterQa;
   }
   static void *newArray_CbmTrdHitProducerClusterQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdHitProducerClusterQa[nElements] : new ::CbmTrdHitProducerClusterQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdHitProducerClusterQa(void *p) {
      delete ((::CbmTrdHitProducerClusterQa*)p);
   }
   static void deleteArray_CbmTrdHitProducerClusterQa(void *p) {
      delete [] ((::CbmTrdHitProducerClusterQa*)p);
   }
   static void destruct_CbmTrdHitProducerClusterQa(void *p) {
      typedef ::CbmTrdHitProducerClusterQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdHitProducerClusterQa

//______________________________________________________________________________
void CbmTrdHitProducerQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdHitProducerQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdHitProducerQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdHitProducerQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdHitProducerQa(void *p) {
      return  p ? new(p) ::CbmTrdHitProducerQa : new ::CbmTrdHitProducerQa;
   }
   static void *newArray_CbmTrdHitProducerQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdHitProducerQa[nElements] : new ::CbmTrdHitProducerQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdHitProducerQa(void *p) {
      delete ((::CbmTrdHitProducerQa*)p);
   }
   static void deleteArray_CbmTrdHitProducerQa(void *p) {
      delete [] ((::CbmTrdHitProducerQa*)p);
   }
   static void destruct_CbmTrdHitProducerQa(void *p) {
      typedef ::CbmTrdHitProducerQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdHitProducerQa

//______________________________________________________________________________
void CbmTrdOccupancyQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdOccupancyQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdOccupancyQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdOccupancyQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdOccupancyQa(void *p) {
      return  p ? new(p) ::CbmTrdOccupancyQa : new ::CbmTrdOccupancyQa;
   }
   static void *newArray_CbmTrdOccupancyQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdOccupancyQa[nElements] : new ::CbmTrdOccupancyQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdOccupancyQa(void *p) {
      delete ((::CbmTrdOccupancyQa*)p);
   }
   static void deleteArray_CbmTrdOccupancyQa(void *p) {
      delete [] ((::CbmTrdOccupancyQa*)p);
   }
   static void destruct_CbmTrdOccupancyQa(void *p) {
      typedef ::CbmTrdOccupancyQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdOccupancyQa

//______________________________________________________________________________
void CbmTrdQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdQa(void *p) {
      return  p ? new(p) ::CbmTrdQa : new ::CbmTrdQa;
   }
   static void *newArray_CbmTrdQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdQa[nElements] : new ::CbmTrdQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdQa(void *p) {
      delete ((::CbmTrdQa*)p);
   }
   static void deleteArray_CbmTrdQa(void *p) {
      delete [] ((::CbmTrdQa*)p);
   }
   static void destruct_CbmTrdQa(void *p) {
      typedef ::CbmTrdQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdQa

//______________________________________________________________________________
void CbmTrdRecoQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdRecoQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdRecoQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdRecoQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdRecoQa(void *p) {
      return  p ? new(p) ::CbmTrdRecoQa : new ::CbmTrdRecoQa;
   }
   static void *newArray_CbmTrdRecoQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdRecoQa[nElements] : new ::CbmTrdRecoQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdRecoQa(void *p) {
      delete ((::CbmTrdRecoQa*)p);
   }
   static void deleteArray_CbmTrdRecoQa(void *p) {
      delete [] ((::CbmTrdRecoQa*)p);
   }
   static void destruct_CbmTrdRecoQa(void *p) {
      typedef ::CbmTrdRecoQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdRecoQa

//______________________________________________________________________________
void CbmTrdTracksPidQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdTracksPidQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdTracksPidQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdTracksPidQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdTracksPidQa(void *p) {
      return  p ? new(p) ::CbmTrdTracksPidQa : new ::CbmTrdTracksPidQa;
   }
   static void *newArray_CbmTrdTracksPidQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdTracksPidQa[nElements] : new ::CbmTrdTracksPidQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdTracksPidQa(void *p) {
      delete ((::CbmTrdTracksPidQa*)p);
   }
   static void deleteArray_CbmTrdTracksPidQa(void *p) {
      delete [] ((::CbmTrdTracksPidQa*)p);
   }
   static void destruct_CbmTrdTracksPidQa(void *p) {
      typedef ::CbmTrdTracksPidQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdTracksPidQa

//______________________________________________________________________________
void CbmTrdCalibTracker::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdCalibTracker.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdCalibTracker::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdCalibTracker::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdCalibTracker(void *p) {
      return  p ? new(p) ::CbmTrdCalibTracker : new ::CbmTrdCalibTracker;
   }
   static void *newArray_CbmTrdCalibTracker(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdCalibTracker[nElements] : new ::CbmTrdCalibTracker[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdCalibTracker(void *p) {
      delete ((::CbmTrdCalibTracker*)p);
   }
   static void deleteArray_CbmTrdCalibTracker(void *p) {
      delete [] ((::CbmTrdCalibTracker*)p);
   }
   static void destruct_CbmTrdCalibTracker(void *p) {
      typedef ::CbmTrdCalibTracker current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdCalibTracker

//______________________________________________________________________________
void CbmTrdHitMC::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdHitMC.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdHitMC::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdHitMC::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdHitMC(void *p) {
      return  p ? new(p) ::CbmTrdHitMC : new ::CbmTrdHitMC;
   }
   static void *newArray_CbmTrdHitMC(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdHitMC[nElements] : new ::CbmTrdHitMC[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdHitMC(void *p) {
      delete ((::CbmTrdHitMC*)p);
   }
   static void deleteArray_CbmTrdHitMC(void *p) {
      delete [] ((::CbmTrdHitMC*)p);
   }
   static void destruct_CbmTrdHitMC(void *p) {
      typedef ::CbmTrdHitMC current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdHitMC

//______________________________________________________________________________
void CbmTrdElectronsTrainAnn::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdElectronsTrainAnn.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdElectronsTrainAnn::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdElectronsTrainAnn::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrdElectronsTrainAnn(void *p) {
      delete ((::CbmTrdElectronsTrainAnn*)p);
   }
   static void deleteArray_CbmTrdElectronsTrainAnn(void *p) {
      delete [] ((::CbmTrdElectronsTrainAnn*)p);
   }
   static void destruct_CbmTrdElectronsTrainAnn(void *p) {
      typedef ::CbmTrdElectronsTrainAnn current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdElectronsTrainAnn

//______________________________________________________________________________
void CbmTrdSetTracksPidWkn::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdSetTracksPidWkn.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdSetTracksPidWkn::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdSetTracksPidWkn::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdSetTracksPidWkn(void *p) {
      return  p ? new(p) ::CbmTrdSetTracksPidWkn : new ::CbmTrdSetTracksPidWkn;
   }
   static void *newArray_CbmTrdSetTracksPidWkn(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdSetTracksPidWkn[nElements] : new ::CbmTrdSetTracksPidWkn[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdSetTracksPidWkn(void *p) {
      delete ((::CbmTrdSetTracksPidWkn*)p);
   }
   static void deleteArray_CbmTrdSetTracksPidWkn(void *p) {
      delete [] ((::CbmTrdSetTracksPidWkn*)p);
   }
   static void destruct_CbmTrdSetTracksPidWkn(void *p) {
      typedef ::CbmTrdSetTracksPidWkn current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdSetTracksPidWkn

//______________________________________________________________________________
void CbmTrdSetTracksPidModWkn::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdSetTracksPidModWkn.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdSetTracksPidModWkn::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdSetTracksPidModWkn::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdSetTracksPidModWkn(void *p) {
      return  p ? new(p) ::CbmTrdSetTracksPidModWkn : new ::CbmTrdSetTracksPidModWkn;
   }
   static void *newArray_CbmTrdSetTracksPidModWkn(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdSetTracksPidModWkn[nElements] : new ::CbmTrdSetTracksPidModWkn[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdSetTracksPidModWkn(void *p) {
      delete ((::CbmTrdSetTracksPidModWkn*)p);
   }
   static void deleteArray_CbmTrdSetTracksPidModWkn(void *p) {
      delete [] ((::CbmTrdSetTracksPidModWkn*)p);
   }
   static void destruct_CbmTrdSetTracksPidModWkn(void *p) {
      typedef ::CbmTrdSetTracksPidModWkn current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdSetTracksPidModWkn

//______________________________________________________________________________
void CbmTrdSetTracksPidLike::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdSetTracksPidLike.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdSetTracksPidLike::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdSetTracksPidLike::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdSetTracksPidLike(void *p) {
      return  p ? new(p) ::CbmTrdSetTracksPidLike : new ::CbmTrdSetTracksPidLike;
   }
   static void *newArray_CbmTrdSetTracksPidLike(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdSetTracksPidLike[nElements] : new ::CbmTrdSetTracksPidLike[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdSetTracksPidLike(void *p) {
      delete ((::CbmTrdSetTracksPidLike*)p);
   }
   static void deleteArray_CbmTrdSetTracksPidLike(void *p) {
      delete [] ((::CbmTrdSetTracksPidLike*)p);
   }
   static void destruct_CbmTrdSetTracksPidLike(void *p) {
      typedef ::CbmTrdSetTracksPidLike current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdSetTracksPidLike

//______________________________________________________________________________
void CbmTrdSetTracksPidANN::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdSetTracksPidANN.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdSetTracksPidANN::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdSetTracksPidANN::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdSetTracksPidANN(void *p) {
      return  p ? new(p) ::CbmTrdSetTracksPidANN : new ::CbmTrdSetTracksPidANN;
   }
   static void *newArray_CbmTrdSetTracksPidANN(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdSetTracksPidANN[nElements] : new ::CbmTrdSetTracksPidANN[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdSetTracksPidANN(void *p) {
      delete ((::CbmTrdSetTracksPidANN*)p);
   }
   static void deleteArray_CbmTrdSetTracksPidANN(void *p) {
      delete [] ((::CbmTrdSetTracksPidANN*)p);
   }
   static void destruct_CbmTrdSetTracksPidANN(void *p) {
      typedef ::CbmTrdSetTracksPidANN current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdSetTracksPidANN

//______________________________________________________________________________
void CbmTrdRawToDigiFitR::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdRawToDigiFitR.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdRawToDigiFitR::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdRawToDigiFitR::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdRawToDigiFitR(void *p) {
      return  p ? new(p) ::CbmTrdRawToDigiFitR : new ::CbmTrdRawToDigiFitR;
   }
   static void *newArray_CbmTrdRawToDigiFitR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdRawToDigiFitR[nElements] : new ::CbmTrdRawToDigiFitR[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdRawToDigiFitR(void *p) {
      delete ((::CbmTrdRawToDigiFitR*)p);
   }
   static void deleteArray_CbmTrdRawToDigiFitR(void *p) {
      delete [] ((::CbmTrdRawToDigiFitR*)p);
   }
   static void destruct_CbmTrdRawToDigiFitR(void *p) {
      typedef ::CbmTrdRawToDigiFitR current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdRawToDigiFitR

//______________________________________________________________________________
void CbmTrdRawToDigiLookUpCorrR::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdRawToDigiLookUpCorrR.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdRawToDigiLookUpCorrR::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdRawToDigiLookUpCorrR::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdRawToDigiLookUpCorrR(void *p) {
      return  p ? new(p) ::CbmTrdRawToDigiLookUpCorrR : new ::CbmTrdRawToDigiLookUpCorrR;
   }
   static void *newArray_CbmTrdRawToDigiLookUpCorrR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdRawToDigiLookUpCorrR[nElements] : new ::CbmTrdRawToDigiLookUpCorrR[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdRawToDigiLookUpCorrR(void *p) {
      delete ((::CbmTrdRawToDigiLookUpCorrR*)p);
   }
   static void deleteArray_CbmTrdRawToDigiLookUpCorrR(void *p) {
      delete [] ((::CbmTrdRawToDigiLookUpCorrR*)p);
   }
   static void destruct_CbmTrdRawToDigiLookUpCorrR(void *p) {
      typedef ::CbmTrdRawToDigiLookUpCorrR current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdRawToDigiLookUpCorrR

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlETrdElossgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlETrdElossgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<TrdEloss> > >*)
   {
      vector<vector<vector<TrdEloss> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<TrdEloss> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<TrdEloss> > >", -2, "vector", 389,
                  typeid(vector<vector<vector<TrdEloss> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlETrdElossgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<TrdEloss> > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<TrdEloss> > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<TrdEloss> > >","std::vector<std::vector<std::vector<TrdEloss, std::allocator<TrdEloss> >, std::allocator<std::vector<TrdEloss, std::allocator<TrdEloss> > > >, std::allocator<std::vector<std::vector<TrdEloss, std::allocator<TrdEloss> >, std::allocator<std::vector<TrdEloss, std::allocator<TrdEloss> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<TrdEloss> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlETrdElossgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<TrdEloss> > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlETrdElossgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlETrdElossgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<TrdEloss> > > : new vector<vector<vector<TrdEloss> > >;
   }
   static void *newArray_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<TrdEloss> > >[nElements] : new vector<vector<vector<TrdEloss> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<TrdEloss> > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<TrdEloss> > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlETrdElossgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<TrdEloss> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<TrdEloss> > >

namespace ROOT {
   static TClass *vectorlEvectorlETrdElossgRsPgR_Dictionary();
   static void vectorlEvectorlETrdElossgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlETrdElossgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlETrdElossgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlETrdElossgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlETrdElossgRsPgR(void *p);
   static void destruct_vectorlEvectorlETrdElossgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<TrdEloss> >*)
   {
      vector<vector<TrdEloss> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<TrdEloss> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<TrdEloss> >", -2, "vector", 389,
                  typeid(vector<vector<TrdEloss> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlETrdElossgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<TrdEloss> >) );
      instance.SetNew(&new_vectorlEvectorlETrdElossgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlETrdElossgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlETrdElossgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlETrdElossgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlETrdElossgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<TrdEloss> > >()));

      ::ROOT::AddClassAlternate("vector<vector<TrdEloss> >","std::vector<std::vector<TrdEloss, std::allocator<TrdEloss> >, std::allocator<std::vector<TrdEloss, std::allocator<TrdEloss> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<TrdEloss> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlETrdElossgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<TrdEloss> >*)nullptr)->GetClass();
      vectorlEvectorlETrdElossgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlETrdElossgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlETrdElossgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TrdEloss> > : new vector<vector<TrdEloss> >;
   }
   static void *newArray_vectorlEvectorlETrdElossgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TrdEloss> >[nElements] : new vector<vector<TrdEloss> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlETrdElossgRsPgR(void *p) {
      delete ((vector<vector<TrdEloss> >*)p);
   }
   static void deleteArray_vectorlEvectorlETrdElossgRsPgR(void *p) {
      delete [] ((vector<vector<TrdEloss> >*)p);
   }
   static void destruct_vectorlEvectorlETrdElossgRsPgR(void *p) {
      typedef vector<vector<TrdEloss> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<TrdEloss> >

namespace ROOT {
   static TClass *vectorlEvectorlETH1mUgRsPgR_Dictionary();
   static void vectorlEvectorlETH1mUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlETH1mUgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlETH1mUgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlETH1mUgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlETH1mUgRsPgR(void *p);
   static void destruct_vectorlEvectorlETH1mUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<TH1*> >*)
   {
      vector<vector<TH1*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<TH1*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<TH1*> >", -2, "vector", 389,
                  typeid(vector<vector<TH1*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlETH1mUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<TH1*> >) );
      instance.SetNew(&new_vectorlEvectorlETH1mUgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlETH1mUgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlETH1mUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlETH1mUgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlETH1mUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<TH1*> > >()));

      ::ROOT::AddClassAlternate("vector<vector<TH1*> >","std::vector<std::vector<TH1*, std::allocator<TH1*> >, std::allocator<std::vector<TH1*, std::allocator<TH1*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<TH1*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlETH1mUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<TH1*> >*)nullptr)->GetClass();
      vectorlEvectorlETH1mUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlETH1mUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlETH1mUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TH1*> > : new vector<vector<TH1*> >;
   }
   static void *newArray_vectorlEvectorlETH1mUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TH1*> >[nElements] : new vector<vector<TH1*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlETH1mUgRsPgR(void *p) {
      delete ((vector<vector<TH1*> >*)p);
   }
   static void deleteArray_vectorlEvectorlETH1mUgRsPgR(void *p) {
      delete [] ((vector<vector<TH1*> >*)p);
   }
   static void destruct_vectorlEvectorlETH1mUgRsPgR(void *p) {
      typedef vector<vector<TH1*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<TH1*> >

namespace ROOT {
   static TClass *vectorlEunsignedsPshortgR_Dictionary();
   static void vectorlEunsignedsPshortgR_TClassManip(TClass*);
   static void *new_vectorlEunsignedsPshortgR(void *p = nullptr);
   static void *newArray_vectorlEunsignedsPshortgR(Long_t size, void *p);
   static void delete_vectorlEunsignedsPshortgR(void *p);
   static void deleteArray_vectorlEunsignedsPshortgR(void *p);
   static void destruct_vectorlEunsignedsPshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned short>*)
   {
      vector<unsigned short> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned short>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<unsigned short>", -2, "vector", 389,
                  typeid(vector<unsigned short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEunsignedsPshortgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<unsigned short>) );
      instance.SetNew(&new_vectorlEunsignedsPshortgR);
      instance.SetNewArray(&newArray_vectorlEunsignedsPshortgR);
      instance.SetDelete(&delete_vectorlEunsignedsPshortgR);
      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPshortgR);
      instance.SetDestructor(&destruct_vectorlEunsignedsPshortgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned short> >()));

      ::ROOT::AddClassAlternate("vector<unsigned short>","std::vector<unsigned short, std::allocator<unsigned short> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<unsigned short>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEunsignedsPshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<unsigned short>*)nullptr)->GetClass();
      vectorlEunsignedsPshortgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEunsignedsPshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEunsignedsPshortgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned short> : new vector<unsigned short>;
   }
   static void *newArray_vectorlEunsignedsPshortgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned short>[nElements] : new vector<unsigned short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEunsignedsPshortgR(void *p) {
      delete ((vector<unsigned short>*)p);
   }
   static void deleteArray_vectorlEunsignedsPshortgR(void *p) {
      delete [] ((vector<unsigned short>*)p);
   }
   static void destruct_vectorlEunsignedsPshortgR(void *p) {
      typedef vector<unsigned short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<unsigned short>

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
   static TClass *vectorlEunsignedsPchargR_Dictionary();
   static void vectorlEunsignedsPchargR_TClassManip(TClass*);
   static void *new_vectorlEunsignedsPchargR(void *p = nullptr);
   static void *newArray_vectorlEunsignedsPchargR(Long_t size, void *p);
   static void delete_vectorlEunsignedsPchargR(void *p);
   static void deleteArray_vectorlEunsignedsPchargR(void *p);
   static void destruct_vectorlEunsignedsPchargR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned char>*)
   {
      vector<unsigned char> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned char>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<unsigned char>", -2, "vector", 389,
                  typeid(vector<unsigned char>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEunsignedsPchargR_Dictionary, isa_proxy, 0,
                  sizeof(vector<unsigned char>) );
      instance.SetNew(&new_vectorlEunsignedsPchargR);
      instance.SetNewArray(&newArray_vectorlEunsignedsPchargR);
      instance.SetDelete(&delete_vectorlEunsignedsPchargR);
      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPchargR);
      instance.SetDestructor(&destruct_vectorlEunsignedsPchargR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned char> >()));

      ::ROOT::AddClassAlternate("vector<unsigned char>","std::vector<unsigned char, std::allocator<unsigned char> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<unsigned char>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEunsignedsPchargR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<unsigned char>*)nullptr)->GetClass();
      vectorlEunsignedsPchargR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEunsignedsPchargR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEunsignedsPchargR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned char> : new vector<unsigned char>;
   }
   static void *newArray_vectorlEunsignedsPchargR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned char>[nElements] : new vector<unsigned char>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEunsignedsPchargR(void *p) {
      delete ((vector<unsigned char>*)p);
   }
   static void deleteArray_vectorlEunsignedsPchargR(void *p) {
      delete [] ((vector<unsigned char>*)p);
   }
   static void destruct_vectorlEunsignedsPchargR(void *p) {
      typedef vector<unsigned char> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<unsigned char>

namespace ROOT {
   static TClass *vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR_Dictionary();
   static void vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR(void *p);
   static void deleteArray_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR(void *p);
   static void destruct_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<tuple<CbmTrdPoint,double,int> >*)
   {
      vector<tuple<CbmTrdPoint,double,int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<tuple<CbmTrdPoint,double,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<tuple<CbmTrdPoint,double,int> >", -2, "vector", 389,
                  typeid(vector<tuple<CbmTrdPoint,double,int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<tuple<CbmTrdPoint,double,int> >) );
      instance.SetNew(&new_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR);
      instance.SetDelete(&delete_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<tuple<CbmTrdPoint,double,int> > >()));

      ::ROOT::AddClassAlternate("vector<tuple<CbmTrdPoint,double,int> >","std::vector<std::tuple<CbmTrdPoint, double, int>, std::allocator<std::tuple<CbmTrdPoint, double, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<tuple<CbmTrdPoint,double,int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<tuple<CbmTrdPoint,double,int> >*)nullptr)->GetClass();
      vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tuple<CbmTrdPoint,double,int> > : new vector<tuple<CbmTrdPoint,double,int> >;
   }
   static void *newArray_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tuple<CbmTrdPoint,double,int> >[nElements] : new vector<tuple<CbmTrdPoint,double,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR(void *p) {
      delete ((vector<tuple<CbmTrdPoint,double,int> >*)p);
   }
   static void deleteArray_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR(void *p) {
      delete [] ((vector<tuple<CbmTrdPoint,double,int> >*)p);
   }
   static void destruct_vectorlEtuplelECbmTrdPointcOdoublecOintgRsPgR(void *p) {
      typedef vector<tuple<CbmTrdPoint,double,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<tuple<CbmTrdPoint,double,int> >

namespace ROOT {
   static TClass *vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR_Dictionary();
   static void vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR(void *p = nullptr);
   static void *newArray_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR(void *p);
   static void deleteArray_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR(void *p);
   static void destruct_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<unsigned long,unsigned long> >*)
   {
      vector<pair<unsigned long,unsigned long> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<unsigned long,unsigned long> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<unsigned long,unsigned long> >", -2, "vector", 389,
                  typeid(vector<pair<unsigned long,unsigned long> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<pair<unsigned long,unsigned long> >) );
      instance.SetNew(&new_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR);
      instance.SetDelete(&delete_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<unsigned long,unsigned long> > >()));

      ::ROOT::AddClassAlternate("vector<pair<unsigned long,unsigned long> >","std::vector<std::pair<unsigned long, unsigned long>, std::allocator<std::pair<unsigned long, unsigned long> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pair<unsigned long,unsigned long> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<unsigned long,unsigned long> >*)nullptr)->GetClass();
      vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<unsigned long,unsigned long> > : new vector<pair<unsigned long,unsigned long> >;
   }
   static void *newArray_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<unsigned long,unsigned long> >[nElements] : new vector<pair<unsigned long,unsigned long> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR(void *p) {
      delete ((vector<pair<unsigned long,unsigned long> >*)p);
   }
   static void deleteArray_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR(void *p) {
      delete [] ((vector<pair<unsigned long,unsigned long> >*)p);
   }
   static void destruct_vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR(void *p) {
      typedef vector<pair<unsigned long,unsigned long> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<unsigned long,unsigned long> >

namespace ROOT {
   static TClass *vectorlEpairlEdoublecOintgRsPgR_Dictionary();
   static void vectorlEpairlEdoublecOintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlEdoublecOintgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEpairlEdoublecOintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlEdoublecOintgRsPgR(void *p);
   static void deleteArray_vectorlEpairlEdoublecOintgRsPgR(void *p);
   static void destruct_vectorlEpairlEdoublecOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<double,int> >*)
   {
      vector<pair<double,int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<double,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<double,int> >", -2, "vector", 389,
                  typeid(vector<pair<double,int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlEdoublecOintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<double,int> >) );
      instance.SetNew(&new_vectorlEpairlEdoublecOintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlEdoublecOintgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlEdoublecOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlEdoublecOintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlEdoublecOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<double,int> > >()));

      ::ROOT::AddClassAlternate("vector<pair<double,int> >","std::vector<std::pair<double, int>, std::allocator<std::pair<double, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pair<double,int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlEdoublecOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<double,int> >*)nullptr)->GetClass();
      vectorlEpairlEdoublecOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlEdoublecOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlEdoublecOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<double,int> > : new vector<pair<double,int> >;
   }
   static void *newArray_vectorlEpairlEdoublecOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<double,int> >[nElements] : new vector<pair<double,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlEdoublecOintgRsPgR(void *p) {
      delete ((vector<pair<double,int> >*)p);
   }
   static void deleteArray_vectorlEpairlEdoublecOintgRsPgR(void *p) {
      delete [] ((vector<pair<double,int> >*)p);
   }
   static void destruct_vectorlEpairlEdoublecOintgRsPgR(void *p) {
      typedef vector<pair<double,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<double,int> >

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
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = nullptr);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 389,
                  typeid(vector<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));

      ::ROOT::AddClassAlternate("vector<float>","std::vector<float, std::allocator<float> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<float>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<float>*)nullptr)->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete ((vector<float>*)p);
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] ((vector<float>*)p);
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<float>

namespace ROOT {
   static TClass *vectorlETrdElossgR_Dictionary();
   static void vectorlETrdElossgR_TClassManip(TClass*);
   static void *new_vectorlETrdElossgR(void *p = nullptr);
   static void *newArray_vectorlETrdElossgR(Long_t size, void *p);
   static void delete_vectorlETrdElossgR(void *p);
   static void deleteArray_vectorlETrdElossgR(void *p);
   static void destruct_vectorlETrdElossgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TrdEloss>*)
   {
      vector<TrdEloss> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TrdEloss>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TrdEloss>", -2, "vector", 389,
                  typeid(vector<TrdEloss>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETrdElossgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TrdEloss>) );
      instance.SetNew(&new_vectorlETrdElossgR);
      instance.SetNewArray(&newArray_vectorlETrdElossgR);
      instance.SetDelete(&delete_vectorlETrdElossgR);
      instance.SetDeleteArray(&deleteArray_vectorlETrdElossgR);
      instance.SetDestructor(&destruct_vectorlETrdElossgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TrdEloss> >()));

      ::ROOT::AddClassAlternate("vector<TrdEloss>","std::vector<TrdEloss, std::allocator<TrdEloss> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TrdEloss>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETrdElossgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TrdEloss>*)nullptr)->GetClass();
      vectorlETrdElossgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETrdElossgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETrdElossgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TrdEloss> : new vector<TrdEloss>;
   }
   static void *newArray_vectorlETrdElossgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TrdEloss>[nElements] : new vector<TrdEloss>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETrdElossgR(void *p) {
      delete ((vector<TrdEloss>*)p);
   }
   static void deleteArray_vectorlETrdElossgR(void *p) {
      delete [] ((vector<TrdEloss>*)p);
   }
   static void destruct_vectorlETrdElossgR(void *p) {
      typedef vector<TrdEloss> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TrdEloss>

namespace ROOT {
   static TClass *vectorlETMultiLayerPerceptronmUgR_Dictionary();
   static void vectorlETMultiLayerPerceptronmUgR_TClassManip(TClass*);
   static void *new_vectorlETMultiLayerPerceptronmUgR(void *p = nullptr);
   static void *newArray_vectorlETMultiLayerPerceptronmUgR(Long_t size, void *p);
   static void delete_vectorlETMultiLayerPerceptronmUgR(void *p);
   static void deleteArray_vectorlETMultiLayerPerceptronmUgR(void *p);
   static void destruct_vectorlETMultiLayerPerceptronmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TMultiLayerPerceptron*>*)
   {
      vector<TMultiLayerPerceptron*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TMultiLayerPerceptron*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TMultiLayerPerceptron*>", -2, "vector", 389,
                  typeid(vector<TMultiLayerPerceptron*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETMultiLayerPerceptronmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TMultiLayerPerceptron*>) );
      instance.SetNew(&new_vectorlETMultiLayerPerceptronmUgR);
      instance.SetNewArray(&newArray_vectorlETMultiLayerPerceptronmUgR);
      instance.SetDelete(&delete_vectorlETMultiLayerPerceptronmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETMultiLayerPerceptronmUgR);
      instance.SetDestructor(&destruct_vectorlETMultiLayerPerceptronmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TMultiLayerPerceptron*> >()));

      ::ROOT::AddClassAlternate("vector<TMultiLayerPerceptron*>","std::vector<TMultiLayerPerceptron*, std::allocator<TMultiLayerPerceptron*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TMultiLayerPerceptron*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETMultiLayerPerceptronmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TMultiLayerPerceptron*>*)nullptr)->GetClass();
      vectorlETMultiLayerPerceptronmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETMultiLayerPerceptronmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETMultiLayerPerceptronmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TMultiLayerPerceptron*> : new vector<TMultiLayerPerceptron*>;
   }
   static void *newArray_vectorlETMultiLayerPerceptronmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TMultiLayerPerceptron*>[nElements] : new vector<TMultiLayerPerceptron*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETMultiLayerPerceptronmUgR(void *p) {
      delete ((vector<TMultiLayerPerceptron*>*)p);
   }
   static void deleteArray_vectorlETMultiLayerPerceptronmUgR(void *p) {
      delete [] ((vector<TMultiLayerPerceptron*>*)p);
   }
   static void destruct_vectorlETMultiLayerPerceptronmUgR(void *p) {
      typedef vector<TMultiLayerPerceptron*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TMultiLayerPerceptron*>

namespace ROOT {
   static TClass *vectorlETH1FmUgR_Dictionary();
   static void vectorlETH1FmUgR_TClassManip(TClass*);
   static void *new_vectorlETH1FmUgR(void *p = nullptr);
   static void *newArray_vectorlETH1FmUgR(Long_t size, void *p);
   static void delete_vectorlETH1FmUgR(void *p);
   static void deleteArray_vectorlETH1FmUgR(void *p);
   static void destruct_vectorlETH1FmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TH1F*>*)
   {
      vector<TH1F*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TH1F*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TH1F*>", -2, "vector", 389,
                  typeid(vector<TH1F*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETH1FmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TH1F*>) );
      instance.SetNew(&new_vectorlETH1FmUgR);
      instance.SetNewArray(&newArray_vectorlETH1FmUgR);
      instance.SetDelete(&delete_vectorlETH1FmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETH1FmUgR);
      instance.SetDestructor(&destruct_vectorlETH1FmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TH1F*> >()));

      ::ROOT::AddClassAlternate("vector<TH1F*>","std::vector<TH1F*, std::allocator<TH1F*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TH1F*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETH1FmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TH1F*>*)nullptr)->GetClass();
      vectorlETH1FmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETH1FmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETH1FmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH1F*> : new vector<TH1F*>;
   }
   static void *newArray_vectorlETH1FmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH1F*>[nElements] : new vector<TH1F*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETH1FmUgR(void *p) {
      delete ((vector<TH1F*>*)p);
   }
   static void deleteArray_vectorlETH1FmUgR(void *p) {
      delete [] ((vector<TH1F*>*)p);
   }
   static void destruct_vectorlETH1FmUgR(void *p) {
      typedef vector<TH1F*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TH1F*>

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
   static TClass *vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR_Dictionary();
   static void vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR_TClassManip(TClass*);
   static void *new_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR(void *p = nullptr);
   static void *newArray_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR(Long_t size, void *p);
   static void delete_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR(void *p);
   static void deleteArray_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR(void *p);
   static void destruct_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmTrdUnpackMonitor::eRawHistos>*)
   {
      vector<CbmTrdUnpackMonitor::eRawHistos> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmTrdUnpackMonitor::eRawHistos>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmTrdUnpackMonitor::eRawHistos>", -2, "vector", 389,
                  typeid(vector<CbmTrdUnpackMonitor::eRawHistos>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmTrdUnpackMonitor::eRawHistos>) );
      instance.SetNew(&new_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR);
      instance.SetNewArray(&newArray_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR);
      instance.SetDelete(&delete_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR);
      instance.SetDestructor(&destruct_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmTrdUnpackMonitor::eRawHistos> >()));

      ::ROOT::AddClassAlternate("vector<CbmTrdUnpackMonitor::eRawHistos>","std::vector<CbmTrdUnpackMonitor::eRawHistos, std::allocator<CbmTrdUnpackMonitor::eRawHistos> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmTrdUnpackMonitor::eRawHistos>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmTrdUnpackMonitor::eRawHistos>*)nullptr)->GetClass();
      vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTrdUnpackMonitor::eRawHistos> : new vector<CbmTrdUnpackMonitor::eRawHistos>;
   }
   static void *newArray_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTrdUnpackMonitor::eRawHistos>[nElements] : new vector<CbmTrdUnpackMonitor::eRawHistos>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR(void *p) {
      delete ((vector<CbmTrdUnpackMonitor::eRawHistos>*)p);
   }
   static void deleteArray_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR(void *p) {
      delete [] ((vector<CbmTrdUnpackMonitor::eRawHistos>*)p);
   }
   static void destruct_vectorlECbmTrdUnpackMonitorcLcLeRawHistosgR(void *p) {
      typedef vector<CbmTrdUnpackMonitor::eRawHistos> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmTrdUnpackMonitor::eRawHistos>

namespace ROOT {
   static TClass *vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR_Dictionary();
   static void vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR_TClassManip(TClass*);
   static void *new_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR(void *p = nullptr);
   static void *newArray_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR(Long_t size, void *p);
   static void delete_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR(void *p);
   static void deleteArray_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR(void *p);
   static void destruct_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmTrdUnpackMonitor::eOtherHistos>*)
   {
      vector<CbmTrdUnpackMonitor::eOtherHistos> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmTrdUnpackMonitor::eOtherHistos>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmTrdUnpackMonitor::eOtherHistos>", -2, "vector", 389,
                  typeid(vector<CbmTrdUnpackMonitor::eOtherHistos>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmTrdUnpackMonitor::eOtherHistos>) );
      instance.SetNew(&new_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR);
      instance.SetNewArray(&newArray_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR);
      instance.SetDelete(&delete_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR);
      instance.SetDestructor(&destruct_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmTrdUnpackMonitor::eOtherHistos> >()));

      ::ROOT::AddClassAlternate("vector<CbmTrdUnpackMonitor::eOtherHistos>","std::vector<CbmTrdUnpackMonitor::eOtherHistos, std::allocator<CbmTrdUnpackMonitor::eOtherHistos> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmTrdUnpackMonitor::eOtherHistos>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmTrdUnpackMonitor::eOtherHistos>*)nullptr)->GetClass();
      vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTrdUnpackMonitor::eOtherHistos> : new vector<CbmTrdUnpackMonitor::eOtherHistos>;
   }
   static void *newArray_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTrdUnpackMonitor::eOtherHistos>[nElements] : new vector<CbmTrdUnpackMonitor::eOtherHistos>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR(void *p) {
      delete ((vector<CbmTrdUnpackMonitor::eOtherHistos>*)p);
   }
   static void deleteArray_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR(void *p) {
      delete [] ((vector<CbmTrdUnpackMonitor::eOtherHistos>*)p);
   }
   static void destruct_vectorlECbmTrdUnpackMonitorcLcLeOtherHistosgR(void *p) {
      typedef vector<CbmTrdUnpackMonitor::eOtherHistos> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmTrdUnpackMonitor::eOtherHistos>

namespace ROOT {
   static TClass *vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR_Dictionary();
   static void vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR_TClassManip(TClass*);
   static void *new_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR(void *p = nullptr);
   static void *newArray_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR(Long_t size, void *p);
   static void delete_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR(void *p);
   static void deleteArray_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR(void *p);
   static void destruct_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmTrdUnpackMonitor::eDigiHistos>*)
   {
      vector<CbmTrdUnpackMonitor::eDigiHistos> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmTrdUnpackMonitor::eDigiHistos>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmTrdUnpackMonitor::eDigiHistos>", -2, "vector", 389,
                  typeid(vector<CbmTrdUnpackMonitor::eDigiHistos>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmTrdUnpackMonitor::eDigiHistos>) );
      instance.SetNew(&new_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR);
      instance.SetNewArray(&newArray_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR);
      instance.SetDelete(&delete_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR);
      instance.SetDestructor(&destruct_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmTrdUnpackMonitor::eDigiHistos> >()));

      ::ROOT::AddClassAlternate("vector<CbmTrdUnpackMonitor::eDigiHistos>","std::vector<CbmTrdUnpackMonitor::eDigiHistos, std::allocator<CbmTrdUnpackMonitor::eDigiHistos> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmTrdUnpackMonitor::eDigiHistos>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmTrdUnpackMonitor::eDigiHistos>*)nullptr)->GetClass();
      vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTrdUnpackMonitor::eDigiHistos> : new vector<CbmTrdUnpackMonitor::eDigiHistos>;
   }
   static void *newArray_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTrdUnpackMonitor::eDigiHistos>[nElements] : new vector<CbmTrdUnpackMonitor::eDigiHistos>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR(void *p) {
      delete ((vector<CbmTrdUnpackMonitor::eDigiHistos>*)p);
   }
   static void deleteArray_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR(void *p) {
      delete [] ((vector<CbmTrdUnpackMonitor::eDigiHistos>*)p);
   }
   static void destruct_vectorlECbmTrdUnpackMonitorcLcLeDigiHistosgR(void *p) {
      typedef vector<CbmTrdUnpackMonitor::eDigiHistos> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmTrdUnpackMonitor::eDigiHistos>

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
   static TClass *vectorlECbmQaCanvasmUgR_Dictionary();
   static void vectorlECbmQaCanvasmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmQaCanvasmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmQaCanvasmUgR(Long_t size, void *p);
   static void delete_vectorlECbmQaCanvasmUgR(void *p);
   static void deleteArray_vectorlECbmQaCanvasmUgR(void *p);
   static void destruct_vectorlECbmQaCanvasmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmQaCanvas*>*)
   {
      vector<CbmQaCanvas*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmQaCanvas*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmQaCanvas*>", -2, "vector", 389,
                  typeid(vector<CbmQaCanvas*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmQaCanvasmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmQaCanvas*>) );
      instance.SetNew(&new_vectorlECbmQaCanvasmUgR);
      instance.SetNewArray(&newArray_vectorlECbmQaCanvasmUgR);
      instance.SetDelete(&delete_vectorlECbmQaCanvasmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmQaCanvasmUgR);
      instance.SetDestructor(&destruct_vectorlECbmQaCanvasmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmQaCanvas*> >()));

      ::ROOT::AddClassAlternate("vector<CbmQaCanvas*>","std::vector<CbmQaCanvas*, std::allocator<CbmQaCanvas*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmQaCanvas*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmQaCanvasmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmQaCanvas*>*)nullptr)->GetClass();
      vectorlECbmQaCanvasmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmQaCanvasmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmQaCanvasmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmQaCanvas*> : new vector<CbmQaCanvas*>;
   }
   static void *newArray_vectorlECbmQaCanvasmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmQaCanvas*>[nElements] : new vector<CbmQaCanvas*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmQaCanvasmUgR(void *p) {
      delete ((vector<CbmQaCanvas*>*)p);
   }
   static void deleteArray_vectorlECbmQaCanvasmUgR(void *p) {
      delete [] ((vector<CbmQaCanvas*>*)p);
   }
   static void destruct_vectorlECbmQaCanvasmUgR(void *p) {
      typedef vector<CbmQaCanvas*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmQaCanvas*>

namespace ROOT {
   static TClass *maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR_Dictionary();
   static void maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR(Long_t size, void *p);
   static void delete_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR(void *p);
   static void deleteArray_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR(void *p);
   static void destruct_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned short,pair<unsigned short,unsigned short> >*)
   {
      map<unsigned short,pair<unsigned short,unsigned short> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned short,pair<unsigned short,unsigned short> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned short,pair<unsigned short,unsigned short> >", -2, "map", 100,
                  typeid(map<unsigned short,pair<unsigned short,unsigned short> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned short,pair<unsigned short,unsigned short> >) );
      instance.SetNew(&new_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR);
      instance.SetNewArray(&newArray_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR);
      instance.SetDelete(&delete_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR);
      instance.SetDestructor(&destruct_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned short,pair<unsigned short,unsigned short> > >()));

      ::ROOT::AddClassAlternate("map<unsigned short,pair<unsigned short,unsigned short> >","std::map<unsigned short, std::pair<unsigned short, unsigned short>, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, std::pair<unsigned short, unsigned short> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned short,pair<unsigned short,unsigned short> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned short,pair<unsigned short,unsigned short> >*)nullptr)->GetClass();
      maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,pair<unsigned short,unsigned short> > : new map<unsigned short,pair<unsigned short,unsigned short> >;
   }
   static void *newArray_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,pair<unsigned short,unsigned short> >[nElements] : new map<unsigned short,pair<unsigned short,unsigned short> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR(void *p) {
      delete ((map<unsigned short,pair<unsigned short,unsigned short> >*)p);
   }
   static void deleteArray_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR(void *p) {
      delete [] ((map<unsigned short,pair<unsigned short,unsigned short> >*)p);
   }
   static void destruct_maplEunsignedsPshortcOpairlEunsignedsPshortcOunsignedsPshortgRsPgR(void *p) {
      typedef map<unsigned short,pair<unsigned short,unsigned short> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned short,pair<unsigned short,unsigned short> >

namespace ROOT {
   static TClass *maplEunsignedsPlongcOvectorlEintgRsPgR_Dictionary();
   static void maplEunsignedsPlongcOvectorlEintgRsPgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPlongcOvectorlEintgRsPgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPlongcOvectorlEintgRsPgR(Long_t size, void *p);
   static void delete_maplEunsignedsPlongcOvectorlEintgRsPgR(void *p);
   static void deleteArray_maplEunsignedsPlongcOvectorlEintgRsPgR(void *p);
   static void destruct_maplEunsignedsPlongcOvectorlEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned long,vector<int> >*)
   {
      map<unsigned long,vector<int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned long,vector<int> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned long,vector<int> >", -2, "map", 100,
                  typeid(map<unsigned long,vector<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPlongcOvectorlEintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned long,vector<int> >) );
      instance.SetNew(&new_maplEunsignedsPlongcOvectorlEintgRsPgR);
      instance.SetNewArray(&newArray_maplEunsignedsPlongcOvectorlEintgRsPgR);
      instance.SetDelete(&delete_maplEunsignedsPlongcOvectorlEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPlongcOvectorlEintgRsPgR);
      instance.SetDestructor(&destruct_maplEunsignedsPlongcOvectorlEintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned long,vector<int> > >()));

      ::ROOT::AddClassAlternate("map<unsigned long,vector<int> >","std::map<unsigned long, std::vector<int, std::allocator<int> >, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, std::vector<int, std::allocator<int> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned long,vector<int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPlongcOvectorlEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned long,vector<int> >*)nullptr)->GetClass();
      maplEunsignedsPlongcOvectorlEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPlongcOvectorlEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPlongcOvectorlEintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned long,vector<int> > : new map<unsigned long,vector<int> >;
   }
   static void *newArray_maplEunsignedsPlongcOvectorlEintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned long,vector<int> >[nElements] : new map<unsigned long,vector<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPlongcOvectorlEintgRsPgR(void *p) {
      delete ((map<unsigned long,vector<int> >*)p);
   }
   static void deleteArray_maplEunsignedsPlongcOvectorlEintgRsPgR(void *p) {
      delete [] ((map<unsigned long,vector<int> >*)p);
   }
   static void destruct_maplEunsignedsPlongcOvectorlEintgRsPgR(void *p) {
      typedef map<unsigned long,vector<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned long,vector<int> >

namespace ROOT {
   static TClass *maplEunsignedsPlongcOintgR_Dictionary();
   static void maplEunsignedsPlongcOintgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPlongcOintgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPlongcOintgR(Long_t size, void *p);
   static void delete_maplEunsignedsPlongcOintgR(void *p);
   static void deleteArray_maplEunsignedsPlongcOintgR(void *p);
   static void destruct_maplEunsignedsPlongcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned long,int>*)
   {
      map<unsigned long,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned long,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned long,int>", -2, "map", 100,
                  typeid(map<unsigned long,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPlongcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned long,int>) );
      instance.SetNew(&new_maplEunsignedsPlongcOintgR);
      instance.SetNewArray(&newArray_maplEunsignedsPlongcOintgR);
      instance.SetDelete(&delete_maplEunsignedsPlongcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPlongcOintgR);
      instance.SetDestructor(&destruct_maplEunsignedsPlongcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned long,int> >()));

      ::ROOT::AddClassAlternate("map<unsigned long,int>","std::map<unsigned long, int, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned long,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPlongcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned long,int>*)nullptr)->GetClass();
      maplEunsignedsPlongcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPlongcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPlongcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned long,int> : new map<unsigned long,int>;
   }
   static void *newArray_maplEunsignedsPlongcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned long,int>[nElements] : new map<unsigned long,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPlongcOintgR(void *p) {
      delete ((map<unsigned long,int>*)p);
   }
   static void deleteArray_maplEunsignedsPlongcOintgR(void *p) {
      delete [] ((map<unsigned long,int>*)p);
   }
   static void destruct_maplEunsignedsPlongcOintgR(void *p) {
      typedef map<unsigned long,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned long,int>

namespace ROOT {
   static TClass *maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR_Dictionary();
   static void maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR(void *p);
   static void deleteArray_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR(void *p);
   static void destruct_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,vector<unsigned long> >*)
   {
      map<unsigned int,vector<unsigned long> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,vector<unsigned long> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,vector<unsigned long> >", -2, "map", 100,
                  typeid(map<unsigned int,vector<unsigned long> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,vector<unsigned long> >) );
      instance.SetNew(&new_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,vector<unsigned long> > >()));

      ::ROOT::AddClassAlternate("map<unsigned int,vector<unsigned long> >","std::map<unsigned int, std::vector<unsigned long, std::allocator<unsigned long> >, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::vector<unsigned long, std::allocator<unsigned long> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,vector<unsigned long> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,vector<unsigned long> >*)nullptr)->GetClass();
      maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,vector<unsigned long> > : new map<unsigned int,vector<unsigned long> >;
   }
   static void *newArray_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,vector<unsigned long> >[nElements] : new map<unsigned int,vector<unsigned long> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR(void *p) {
      delete ((map<unsigned int,vector<unsigned long> >*)p);
   }
   static void deleteArray_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR(void *p) {
      delete [] ((map<unsigned int,vector<unsigned long> >*)p);
   }
   static void destruct_maplEunsignedsPintcOvectorlEunsignedsPlonggRsPgR(void *p) {
      typedef map<unsigned int,vector<unsigned long> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,vector<unsigned long> >

namespace ROOT {
   static TClass *maplEunsignedsPintcOunsignedsPshortoB5cBgR_Dictionary();
   static void maplEunsignedsPintcOunsignedsPshortoB5cBgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOunsignedsPshortoB5cBgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOunsignedsPshortoB5cBgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOunsignedsPshortoB5cBgR(void *p);
   static void deleteArray_maplEunsignedsPintcOunsignedsPshortoB5cBgR(void *p);
   static void destruct_maplEunsignedsPintcOunsignedsPshortoB5cBgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,unsigned short[5]>*)
   {
      map<unsigned int,unsigned short[5]> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,unsigned short[5]>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,unsigned short[5]>", -2, "map", 100,
                  typeid(map<unsigned int,unsigned short[5]>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOunsignedsPshortoB5cBgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,unsigned short[5]>) );
      instance.SetNew(&new_maplEunsignedsPintcOunsignedsPshortoB5cBgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOunsignedsPshortoB5cBgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOunsignedsPshortoB5cBgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOunsignedsPshortoB5cBgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOunsignedsPshortoB5cBgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,unsigned short[5]> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,unsigned short[5]>","std::map<unsigned int, unsigned short [5], std::less<unsigned int>, std::allocator<std::pair<unsigned int const, unsigned short [5]> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,unsigned short[5]>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOunsignedsPshortoB5cBgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,unsigned short[5]>*)nullptr)->GetClass();
      maplEunsignedsPintcOunsignedsPshortoB5cBgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOunsignedsPshortoB5cBgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOunsignedsPshortoB5cBgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,unsigned short[5]> : new map<unsigned int,unsigned short[5]>;
   }
   static void *newArray_maplEunsignedsPintcOunsignedsPshortoB5cBgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,unsigned short[5]>[nElements] : new map<unsigned int,unsigned short[5]>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOunsignedsPshortoB5cBgR(void *p) {
      delete ((map<unsigned int,unsigned short[5]>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOunsignedsPshortoB5cBgR(void *p) {
      delete [] ((map<unsigned int,unsigned short[5]>*)p);
   }
   static void destruct_maplEunsignedsPintcOunsignedsPshortoB5cBgR(void *p) {
      typedef map<unsigned int,unsigned short[5]> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,unsigned short[5]>

namespace ROOT {
   static TClass *maplEunsignedsPintcOunsignedsPcharoB180cBgR_Dictionary();
   static void maplEunsignedsPintcOunsignedsPcharoB180cBgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOunsignedsPcharoB180cBgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOunsignedsPcharoB180cBgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOunsignedsPcharoB180cBgR(void *p);
   static void deleteArray_maplEunsignedsPintcOunsignedsPcharoB180cBgR(void *p);
   static void destruct_maplEunsignedsPintcOunsignedsPcharoB180cBgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,unsigned char[180]>*)
   {
      map<unsigned int,unsigned char[180]> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,unsigned char[180]>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,unsigned char[180]>", -2, "map", 100,
                  typeid(map<unsigned int,unsigned char[180]>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOunsignedsPcharoB180cBgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,unsigned char[180]>) );
      instance.SetNew(&new_maplEunsignedsPintcOunsignedsPcharoB180cBgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOunsignedsPcharoB180cBgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOunsignedsPcharoB180cBgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOunsignedsPcharoB180cBgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOunsignedsPcharoB180cBgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,unsigned char[180]> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,unsigned char[180]>","std::map<unsigned int, unsigned char [180], std::less<unsigned int>, std::allocator<std::pair<unsigned int const, unsigned char [180]> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,unsigned char[180]>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOunsignedsPcharoB180cBgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,unsigned char[180]>*)nullptr)->GetClass();
      maplEunsignedsPintcOunsignedsPcharoB180cBgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOunsignedsPcharoB180cBgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOunsignedsPcharoB180cBgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,unsigned char[180]> : new map<unsigned int,unsigned char[180]>;
   }
   static void *newArray_maplEunsignedsPintcOunsignedsPcharoB180cBgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,unsigned char[180]>[nElements] : new map<unsigned int,unsigned char[180]>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOunsignedsPcharoB180cBgR(void *p) {
      delete ((map<unsigned int,unsigned char[180]>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOunsignedsPcharoB180cBgR(void *p) {
      delete [] ((map<unsigned int,unsigned char[180]>*)p);
   }
   static void destruct_maplEunsignedsPintcOunsignedsPcharoB180cBgR(void *p) {
      typedef map<unsigned int,unsigned char[180]> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,unsigned char[180]>

namespace ROOT {
   static TClass *maplEunsignedsPintcOunsignedsPchargR_Dictionary();
   static void maplEunsignedsPintcOunsignedsPchargR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOunsignedsPchargR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOunsignedsPchargR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOunsignedsPchargR(void *p);
   static void deleteArray_maplEunsignedsPintcOunsignedsPchargR(void *p);
   static void destruct_maplEunsignedsPintcOunsignedsPchargR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,unsigned char>*)
   {
      map<unsigned int,unsigned char> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,unsigned char>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,unsigned char>", -2, "map", 100,
                  typeid(map<unsigned int,unsigned char>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOunsignedsPchargR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,unsigned char>) );
      instance.SetNew(&new_maplEunsignedsPintcOunsignedsPchargR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOunsignedsPchargR);
      instance.SetDelete(&delete_maplEunsignedsPintcOunsignedsPchargR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOunsignedsPchargR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOunsignedsPchargR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,unsigned char> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,unsigned char>","std::map<unsigned int, unsigned char, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, unsigned char> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,unsigned char>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOunsignedsPchargR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,unsigned char>*)nullptr)->GetClass();
      maplEunsignedsPintcOunsignedsPchargR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOunsignedsPchargR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOunsignedsPchargR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,unsigned char> : new map<unsigned int,unsigned char>;
   }
   static void *newArray_maplEunsignedsPintcOunsignedsPchargR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,unsigned char>[nElements] : new map<unsigned int,unsigned char>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOunsignedsPchargR(void *p) {
      delete ((map<unsigned int,unsigned char>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOunsignedsPchargR(void *p) {
      delete [] ((map<unsigned int,unsigned char>*)p);
   }
   static void destruct_maplEunsignedsPintcOunsignedsPchargR(void *p) {
      typedef map<unsigned int,unsigned char> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,unsigned char>

namespace ROOT {
   static TClass *maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR_Dictionary();
   static void maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR(void *p);
   static void deleteArray_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR(void *p);
   static void destruct_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> >*)
   {
      map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> >", -2, "map", 100,
                  typeid(map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> >) );
      instance.SetNew(&new_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> > >()));

      ::ROOT::AddClassAlternate("map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> >","std::map<unsigned int, std::tuple<std::vector<int, std::allocator<int> >, unsigned char, unsigned char>, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::tuple<std::vector<int, std::allocator<int> >, unsigned char, unsigned char> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> >*)nullptr)->GetClass();
      maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> > : new map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> >;
   }
   static void *newArray_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> >[nElements] : new map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR(void *p) {
      delete ((map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> >*)p);
   }
   static void deleteArray_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR(void *p) {
      delete [] ((map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> >*)p);
   }
   static void destruct_maplEunsignedsPintcOtuplelEvectorlEintgRcOunsignedsPcharcOunsignedsPchargRsPgR(void *p) {
      typedef map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,tuple<vector<int>,unsigned char,unsigned char> >

namespace ROOT {
   static TClass *maplEunsignedsPintcOshared_ptrlETH1gRsPgR_Dictionary();
   static void maplEunsignedsPintcOshared_ptrlETH1gRsPgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOshared_ptrlETH1gRsPgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOshared_ptrlETH1gRsPgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOshared_ptrlETH1gRsPgR(void *p);
   static void deleteArray_maplEunsignedsPintcOshared_ptrlETH1gRsPgR(void *p);
   static void destruct_maplEunsignedsPintcOshared_ptrlETH1gRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,shared_ptr<TH1> >*)
   {
      map<unsigned int,shared_ptr<TH1> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,shared_ptr<TH1> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,shared_ptr<TH1> >", -2, "map", 100,
                  typeid(map<unsigned int,shared_ptr<TH1> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOshared_ptrlETH1gRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,shared_ptr<TH1> >) );
      instance.SetNew(&new_maplEunsignedsPintcOshared_ptrlETH1gRsPgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOshared_ptrlETH1gRsPgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOshared_ptrlETH1gRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOshared_ptrlETH1gRsPgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOshared_ptrlETH1gRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,shared_ptr<TH1> > >()));

      ::ROOT::AddClassAlternate("map<unsigned int,shared_ptr<TH1> >","std::map<unsigned int, std::shared_ptr<TH1>, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::shared_ptr<TH1> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,shared_ptr<TH1> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOshared_ptrlETH1gRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,shared_ptr<TH1> >*)nullptr)->GetClass();
      maplEunsignedsPintcOshared_ptrlETH1gRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOshared_ptrlETH1gRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOshared_ptrlETH1gRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,shared_ptr<TH1> > : new map<unsigned int,shared_ptr<TH1> >;
   }
   static void *newArray_maplEunsignedsPintcOshared_ptrlETH1gRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,shared_ptr<TH1> >[nElements] : new map<unsigned int,shared_ptr<TH1> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOshared_ptrlETH1gRsPgR(void *p) {
      delete ((map<unsigned int,shared_ptr<TH1> >*)p);
   }
   static void deleteArray_maplEunsignedsPintcOshared_ptrlETH1gRsPgR(void *p) {
      delete [] ((map<unsigned int,shared_ptr<TH1> >*)p);
   }
   static void destruct_maplEunsignedsPintcOshared_ptrlETH1gRsPgR(void *p) {
      typedef map<unsigned int,shared_ptr<TH1> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,shared_ptr<TH1> >

namespace ROOT {
   static TClass *maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR_Dictionary();
   static void maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR(void *p);
   static void deleteArray_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR(void *p);
   static void destruct_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,map<unsigned int,ULong64_t> >*)
   {
      map<unsigned int,map<unsigned int,ULong64_t> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,map<unsigned int,ULong64_t> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,map<unsigned int,ULong64_t> >", -2, "map", 100,
                  typeid(map<unsigned int,map<unsigned int,ULong64_t> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,map<unsigned int,ULong64_t> >) );
      instance.SetNew(&new_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,map<unsigned int,ULong64_t> > >()));

      ::ROOT::AddClassAlternate("map<unsigned int,map<unsigned int,ULong64_t> >","std::map<unsigned int, std::map<unsigned int, unsigned long long, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, unsigned long long> > >, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::map<unsigned int, unsigned long long, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, unsigned long long> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,map<unsigned int,ULong64_t> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,map<unsigned int,ULong64_t> >*)nullptr)->GetClass();
      maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,map<unsigned int,ULong64_t> > : new map<unsigned int,map<unsigned int,ULong64_t> >;
   }
   static void *newArray_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,map<unsigned int,ULong64_t> >[nElements] : new map<unsigned int,map<unsigned int,ULong64_t> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR(void *p) {
      delete ((map<unsigned int,map<unsigned int,ULong64_t> >*)p);
   }
   static void deleteArray_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR(void *p) {
      delete [] ((map<unsigned int,map<unsigned int,ULong64_t> >*)p);
   }
   static void destruct_maplEunsignedsPintcOmaplEunsignedsPintcOULong64_tgRsPgR(void *p) {
      typedef map<unsigned int,map<unsigned int,ULong64_t> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,map<unsigned int,ULong64_t> >

namespace ROOT {
   static TClass *maplEunsignedsPintcOULong64_tgR_Dictionary();
   static void maplEunsignedsPintcOULong64_tgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOULong64_tgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOULong64_tgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOULong64_tgR(void *p);
   static void deleteArray_maplEunsignedsPintcOULong64_tgR(void *p);
   static void destruct_maplEunsignedsPintcOULong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,ULong64_t>*)
   {
      map<unsigned int,ULong64_t> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,ULong64_t>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,ULong64_t>", -2, "map", 100,
                  typeid(map<unsigned int,ULong64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOULong64_tgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,ULong64_t>) );
      instance.SetNew(&new_maplEunsignedsPintcOULong64_tgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOULong64_tgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOULong64_tgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOULong64_tgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOULong64_tgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,ULong64_t> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,ULong64_t>","std::map<unsigned int, unsigned long long, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, unsigned long long> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,ULong64_t>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOULong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,ULong64_t>*)nullptr)->GetClass();
      maplEunsignedsPintcOULong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOULong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOULong64_tgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,ULong64_t> : new map<unsigned int,ULong64_t>;
   }
   static void *newArray_maplEunsignedsPintcOULong64_tgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,ULong64_t>[nElements] : new map<unsigned int,ULong64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOULong64_tgR(void *p) {
      delete ((map<unsigned int,ULong64_t>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOULong64_tgR(void *p) {
      delete [] ((map<unsigned int,ULong64_t>*)p);
   }
   static void destruct_maplEunsignedsPintcOULong64_tgR(void *p) {
      typedef map<unsigned int,ULong64_t> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,ULong64_t>

namespace ROOT {
   static TClass *maplEintcOvectorlEintgRsPgR_Dictionary();
   static void maplEintcOvectorlEintgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOvectorlEintgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOvectorlEintgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOvectorlEintgRsPgR(void *p);
   static void deleteArray_maplEintcOvectorlEintgRsPgR(void *p);
   static void destruct_maplEintcOvectorlEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,vector<int> >*)
   {
      map<int,vector<int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,vector<int> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,vector<int> >", -2, "map", 100,
                  typeid(map<int,vector<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOvectorlEintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,vector<int> >) );
      instance.SetNew(&new_maplEintcOvectorlEintgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOvectorlEintgRsPgR);
      instance.SetDelete(&delete_maplEintcOvectorlEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOvectorlEintgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOvectorlEintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,vector<int> > >()));

      ::ROOT::AddClassAlternate("map<int,vector<int> >","std::map<int, std::vector<int, std::allocator<int> >, std::less<int>, std::allocator<std::pair<int const, std::vector<int, std::allocator<int> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,vector<int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOvectorlEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,vector<int> >*)nullptr)->GetClass();
      maplEintcOvectorlEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOvectorlEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOvectorlEintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<int> > : new map<int,vector<int> >;
   }
   static void *newArray_maplEintcOvectorlEintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<int> >[nElements] : new map<int,vector<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOvectorlEintgRsPgR(void *p) {
      delete ((map<int,vector<int> >*)p);
   }
   static void deleteArray_maplEintcOvectorlEintgRsPgR(void *p) {
      delete [] ((map<int,vector<int> >*)p);
   }
   static void destruct_maplEintcOvectorlEintgRsPgR(void *p) {
      typedef map<int,vector<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,vector<int> >

namespace ROOT {
   static TClass *maplEintcOvectorlETLinemUgRmUgR_Dictionary();
   static void maplEintcOvectorlETLinemUgRmUgR_TClassManip(TClass*);
   static void *new_maplEintcOvectorlETLinemUgRmUgR(void *p = nullptr);
   static void *newArray_maplEintcOvectorlETLinemUgRmUgR(Long_t size, void *p);
   static void delete_maplEintcOvectorlETLinemUgRmUgR(void *p);
   static void deleteArray_maplEintcOvectorlETLinemUgRmUgR(void *p);
   static void destruct_maplEintcOvectorlETLinemUgRmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,vector<TLine*>*>*)
   {
      map<int,vector<TLine*>*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,vector<TLine*>*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,vector<TLine*>*>", -2, "map", 100,
                  typeid(map<int,vector<TLine*>*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOvectorlETLinemUgRmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,vector<TLine*>*>) );
      instance.SetNew(&new_maplEintcOvectorlETLinemUgRmUgR);
      instance.SetNewArray(&newArray_maplEintcOvectorlETLinemUgRmUgR);
      instance.SetDelete(&delete_maplEintcOvectorlETLinemUgRmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOvectorlETLinemUgRmUgR);
      instance.SetDestructor(&destruct_maplEintcOvectorlETLinemUgRmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,vector<TLine*>*> >()));

      ::ROOT::AddClassAlternate("map<int,vector<TLine*>*>","std::map<int, std::vector<TLine*, std::allocator<TLine*> >*, std::less<int>, std::allocator<std::pair<int const, std::vector<TLine*, std::allocator<TLine*> >*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,vector<TLine*>*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOvectorlETLinemUgRmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,vector<TLine*>*>*)nullptr)->GetClass();
      maplEintcOvectorlETLinemUgRmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOvectorlETLinemUgRmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOvectorlETLinemUgRmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<TLine*>*> : new map<int,vector<TLine*>*>;
   }
   static void *newArray_maplEintcOvectorlETLinemUgRmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<TLine*>*>[nElements] : new map<int,vector<TLine*>*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOvectorlETLinemUgRmUgR(void *p) {
      delete ((map<int,vector<TLine*>*>*)p);
   }
   static void deleteArray_maplEintcOvectorlETLinemUgRmUgR(void *p) {
      delete [] ((map<int,vector<TLine*>*>*)p);
   }
   static void destruct_maplEintcOvectorlETLinemUgRmUgR(void *p) {
      typedef map<int,vector<TLine*>*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,vector<TLine*>*>

namespace ROOT {
   static TClass *maplEintcOmaplEintcOfloatgRsPgR_Dictionary();
   static void maplEintcOmaplEintcOfloatgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOmaplEintcOfloatgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOmaplEintcOfloatgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOmaplEintcOfloatgRsPgR(void *p);
   static void deleteArray_maplEintcOmaplEintcOfloatgRsPgR(void *p);
   static void destruct_maplEintcOmaplEintcOfloatgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,map<int,float> >*)
   {
      map<int,map<int,float> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,map<int,float> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,map<int,float> >", -2, "map", 100,
                  typeid(map<int,map<int,float> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOmaplEintcOfloatgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,map<int,float> >) );
      instance.SetNew(&new_maplEintcOmaplEintcOfloatgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOmaplEintcOfloatgRsPgR);
      instance.SetDelete(&delete_maplEintcOmaplEintcOfloatgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOmaplEintcOfloatgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOmaplEintcOfloatgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,map<int,float> > >()));

      ::ROOT::AddClassAlternate("map<int,map<int,float> >","std::map<int, std::map<int, float, std::less<int>, std::allocator<std::pair<int const, float> > >, std::less<int>, std::allocator<std::pair<int const, std::map<int, float, std::less<int>, std::allocator<std::pair<int const, float> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,map<int,float> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOmaplEintcOfloatgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,map<int,float> >*)nullptr)->GetClass();
      maplEintcOmaplEintcOfloatgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOmaplEintcOfloatgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOmaplEintcOfloatgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,map<int,float> > : new map<int,map<int,float> >;
   }
   static void *newArray_maplEintcOmaplEintcOfloatgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,map<int,float> >[nElements] : new map<int,map<int,float> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOmaplEintcOfloatgRsPgR(void *p) {
      delete ((map<int,map<int,float> >*)p);
   }
   static void deleteArray_maplEintcOmaplEintcOfloatgRsPgR(void *p) {
      delete [] ((map<int,map<int,float> >*)p);
   }
   static void destruct_maplEintcOmaplEintcOfloatgRsPgR(void *p) {
      typedef map<int,map<int,float> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,map<int,float> >

namespace ROOT {
   static TClass *maplEintcOlistlECbmTrdClustermUgRsPgR_Dictionary();
   static void maplEintcOlistlECbmTrdClustermUgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOlistlECbmTrdClustermUgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOlistlECbmTrdClustermUgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOlistlECbmTrdClustermUgRsPgR(void *p);
   static void deleteArray_maplEintcOlistlECbmTrdClustermUgRsPgR(void *p);
   static void destruct_maplEintcOlistlECbmTrdClustermUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,list<CbmTrdCluster*> >*)
   {
      map<int,list<CbmTrdCluster*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,list<CbmTrdCluster*> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,list<CbmTrdCluster*> >", -2, "map", 100,
                  typeid(map<int,list<CbmTrdCluster*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOlistlECbmTrdClustermUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,list<CbmTrdCluster*> >) );
      instance.SetNew(&new_maplEintcOlistlECbmTrdClustermUgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOlistlECbmTrdClustermUgRsPgR);
      instance.SetDelete(&delete_maplEintcOlistlECbmTrdClustermUgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOlistlECbmTrdClustermUgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOlistlECbmTrdClustermUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,list<CbmTrdCluster*> > >()));

      ::ROOT::AddClassAlternate("map<int,list<CbmTrdCluster*> >","std::map<int, std::__cxx11::list<CbmTrdCluster*, std::allocator<CbmTrdCluster*> >, std::less<int>, std::allocator<std::pair<int const, std::__cxx11::list<CbmTrdCluster*, std::allocator<CbmTrdCluster*> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,list<CbmTrdCluster*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOlistlECbmTrdClustermUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,list<CbmTrdCluster*> >*)nullptr)->GetClass();
      maplEintcOlistlECbmTrdClustermUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOlistlECbmTrdClustermUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOlistlECbmTrdClustermUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,list<CbmTrdCluster*> > : new map<int,list<CbmTrdCluster*> >;
   }
   static void *newArray_maplEintcOlistlECbmTrdClustermUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,list<CbmTrdCluster*> >[nElements] : new map<int,list<CbmTrdCluster*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOlistlECbmTrdClustermUgRsPgR(void *p) {
      delete ((map<int,list<CbmTrdCluster*> >*)p);
   }
   static void deleteArray_maplEintcOlistlECbmTrdClustermUgRsPgR(void *p) {
      delete [] ((map<int,list<CbmTrdCluster*> >*)p);
   }
   static void destruct_maplEintcOlistlECbmTrdClustermUgRsPgR(void *p) {
      typedef map<int,list<CbmTrdCluster*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,list<CbmTrdCluster*> >

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

namespace ROOT {
   static TClass *maplEintcOfloatgR_Dictionary();
   static void maplEintcOfloatgR_TClassManip(TClass*);
   static void *new_maplEintcOfloatgR(void *p = nullptr);
   static void *newArray_maplEintcOfloatgR(Long_t size, void *p);
   static void delete_maplEintcOfloatgR(void *p);
   static void deleteArray_maplEintcOfloatgR(void *p);
   static void destruct_maplEintcOfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,float>*)
   {
      map<int,float> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,float>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,float>", -2, "map", 100,
                  typeid(map<int,float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,float>) );
      instance.SetNew(&new_maplEintcOfloatgR);
      instance.SetNewArray(&newArray_maplEintcOfloatgR);
      instance.SetDelete(&delete_maplEintcOfloatgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOfloatgR);
      instance.SetDestructor(&destruct_maplEintcOfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,float> >()));

      ::ROOT::AddClassAlternate("map<int,float>","std::map<int, float, std::less<int>, std::allocator<std::pair<int const, float> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,float>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,float>*)nullptr)->GetClass();
      maplEintcOfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,float> : new map<int,float>;
   }
   static void *newArray_maplEintcOfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,float>[nElements] : new map<int,float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOfloatgR(void *p) {
      delete ((map<int,float>*)p);
   }
   static void deleteArray_maplEintcOfloatgR(void *p) {
      delete [] ((map<int,float>*)p);
   }
   static void destruct_maplEintcOfloatgR(void *p) {
      typedef map<int,float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,float>

namespace ROOT {
   static TClass *maplEintcOTH2ImUgR_Dictionary();
   static void maplEintcOTH2ImUgR_TClassManip(TClass*);
   static void *new_maplEintcOTH2ImUgR(void *p = nullptr);
   static void *newArray_maplEintcOTH2ImUgR(Long_t size, void *p);
   static void delete_maplEintcOTH2ImUgR(void *p);
   static void deleteArray_maplEintcOTH2ImUgR(void *p);
   static void destruct_maplEintcOTH2ImUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,TH2I*>*)
   {
      map<int,TH2I*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,TH2I*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,TH2I*>", -2, "map", 100,
                  typeid(map<int,TH2I*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOTH2ImUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,TH2I*>) );
      instance.SetNew(&new_maplEintcOTH2ImUgR);
      instance.SetNewArray(&newArray_maplEintcOTH2ImUgR);
      instance.SetDelete(&delete_maplEintcOTH2ImUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOTH2ImUgR);
      instance.SetDestructor(&destruct_maplEintcOTH2ImUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,TH2I*> >()));

      ::ROOT::AddClassAlternate("map<int,TH2I*>","std::map<int, TH2I*, std::less<int>, std::allocator<std::pair<int const, TH2I*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,TH2I*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOTH2ImUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,TH2I*>*)nullptr)->GetClass();
      maplEintcOTH2ImUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOTH2ImUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOTH2ImUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TH2I*> : new map<int,TH2I*>;
   }
   static void *newArray_maplEintcOTH2ImUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TH2I*>[nElements] : new map<int,TH2I*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOTH2ImUgR(void *p) {
      delete ((map<int,TH2I*>*)p);
   }
   static void deleteArray_maplEintcOTH2ImUgR(void *p) {
      delete [] ((map<int,TH2I*>*)p);
   }
   static void destruct_maplEintcOTH2ImUgR(void *p) {
      typedef map<int,TH2I*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,TH2I*>

namespace ROOT {
   static TClass *maplEintcOTH2DmUgR_Dictionary();
   static void maplEintcOTH2DmUgR_TClassManip(TClass*);
   static void *new_maplEintcOTH2DmUgR(void *p = nullptr);
   static void *newArray_maplEintcOTH2DmUgR(Long_t size, void *p);
   static void delete_maplEintcOTH2DmUgR(void *p);
   static void deleteArray_maplEintcOTH2DmUgR(void *p);
   static void destruct_maplEintcOTH2DmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,TH2D*>*)
   {
      map<int,TH2D*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,TH2D*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,TH2D*>", -2, "map", 100,
                  typeid(map<int,TH2D*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOTH2DmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,TH2D*>) );
      instance.SetNew(&new_maplEintcOTH2DmUgR);
      instance.SetNewArray(&newArray_maplEintcOTH2DmUgR);
      instance.SetDelete(&delete_maplEintcOTH2DmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOTH2DmUgR);
      instance.SetDestructor(&destruct_maplEintcOTH2DmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,TH2D*> >()));

      ::ROOT::AddClassAlternate("map<int,TH2D*>","std::map<int, TH2D*, std::less<int>, std::allocator<std::pair<int const, TH2D*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,TH2D*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOTH2DmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,TH2D*>*)nullptr)->GetClass();
      maplEintcOTH2DmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOTH2DmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOTH2DmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TH2D*> : new map<int,TH2D*>;
   }
   static void *newArray_maplEintcOTH2DmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TH2D*>[nElements] : new map<int,TH2D*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOTH2DmUgR(void *p) {
      delete ((map<int,TH2D*>*)p);
   }
   static void deleteArray_maplEintcOTH2DmUgR(void *p) {
      delete [] ((map<int,TH2D*>*)p);
   }
   static void destruct_maplEintcOTH2DmUgR(void *p) {
      typedef map<int,TH2D*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,TH2D*>

namespace ROOT {
   static TClass *maplEintcOTH1ImUgR_Dictionary();
   static void maplEintcOTH1ImUgR_TClassManip(TClass*);
   static void *new_maplEintcOTH1ImUgR(void *p = nullptr);
   static void *newArray_maplEintcOTH1ImUgR(Long_t size, void *p);
   static void delete_maplEintcOTH1ImUgR(void *p);
   static void deleteArray_maplEintcOTH1ImUgR(void *p);
   static void destruct_maplEintcOTH1ImUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,TH1I*>*)
   {
      map<int,TH1I*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,TH1I*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,TH1I*>", -2, "map", 100,
                  typeid(map<int,TH1I*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOTH1ImUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,TH1I*>) );
      instance.SetNew(&new_maplEintcOTH1ImUgR);
      instance.SetNewArray(&newArray_maplEintcOTH1ImUgR);
      instance.SetDelete(&delete_maplEintcOTH1ImUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOTH1ImUgR);
      instance.SetDestructor(&destruct_maplEintcOTH1ImUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,TH1I*> >()));

      ::ROOT::AddClassAlternate("map<int,TH1I*>","std::map<int, TH1I*, std::less<int>, std::allocator<std::pair<int const, TH1I*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,TH1I*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOTH1ImUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,TH1I*>*)nullptr)->GetClass();
      maplEintcOTH1ImUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOTH1ImUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOTH1ImUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TH1I*> : new map<int,TH1I*>;
   }
   static void *newArray_maplEintcOTH1ImUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TH1I*>[nElements] : new map<int,TH1I*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOTH1ImUgR(void *p) {
      delete ((map<int,TH1I*>*)p);
   }
   static void deleteArray_maplEintcOTH1ImUgR(void *p) {
      delete [] ((map<int,TH1I*>*)p);
   }
   static void destruct_maplEintcOTH1ImUgR(void *p) {
      typedef map<int,TH1I*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,TH1I*>

namespace ROOT {
   static TClass *maplEintcOTH1DmUgR_Dictionary();
   static void maplEintcOTH1DmUgR_TClassManip(TClass*);
   static void *new_maplEintcOTH1DmUgR(void *p = nullptr);
   static void *newArray_maplEintcOTH1DmUgR(Long_t size, void *p);
   static void delete_maplEintcOTH1DmUgR(void *p);
   static void deleteArray_maplEintcOTH1DmUgR(void *p);
   static void destruct_maplEintcOTH1DmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,TH1D*>*)
   {
      map<int,TH1D*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,TH1D*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,TH1D*>", -2, "map", 100,
                  typeid(map<int,TH1D*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOTH1DmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,TH1D*>) );
      instance.SetNew(&new_maplEintcOTH1DmUgR);
      instance.SetNewArray(&newArray_maplEintcOTH1DmUgR);
      instance.SetDelete(&delete_maplEintcOTH1DmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOTH1DmUgR);
      instance.SetDestructor(&destruct_maplEintcOTH1DmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,TH1D*> >()));

      ::ROOT::AddClassAlternate("map<int,TH1D*>","std::map<int, TH1D*, std::less<int>, std::allocator<std::pair<int const, TH1D*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,TH1D*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOTH1DmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,TH1D*>*)nullptr)->GetClass();
      maplEintcOTH1DmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOTH1DmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOTH1DmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TH1D*> : new map<int,TH1D*>;
   }
   static void *newArray_maplEintcOTH1DmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TH1D*>[nElements] : new map<int,TH1D*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOTH1DmUgR(void *p) {
      delete ((map<int,TH1D*>*)p);
   }
   static void deleteArray_maplEintcOTH1DmUgR(void *p) {
      delete [] ((map<int,TH1D*>*)p);
   }
   static void destruct_maplEintcOTH1DmUgR(void *p) {
      typedef map<int,TH1D*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,TH1D*>

namespace ROOT {
   static TClass *maplEintcOTGraphErrorsmUgR_Dictionary();
   static void maplEintcOTGraphErrorsmUgR_TClassManip(TClass*);
   static void *new_maplEintcOTGraphErrorsmUgR(void *p = nullptr);
   static void *newArray_maplEintcOTGraphErrorsmUgR(Long_t size, void *p);
   static void delete_maplEintcOTGraphErrorsmUgR(void *p);
   static void deleteArray_maplEintcOTGraphErrorsmUgR(void *p);
   static void destruct_maplEintcOTGraphErrorsmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,TGraphErrors*>*)
   {
      map<int,TGraphErrors*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,TGraphErrors*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,TGraphErrors*>", -2, "map", 100,
                  typeid(map<int,TGraphErrors*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOTGraphErrorsmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,TGraphErrors*>) );
      instance.SetNew(&new_maplEintcOTGraphErrorsmUgR);
      instance.SetNewArray(&newArray_maplEintcOTGraphErrorsmUgR);
      instance.SetDelete(&delete_maplEintcOTGraphErrorsmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOTGraphErrorsmUgR);
      instance.SetDestructor(&destruct_maplEintcOTGraphErrorsmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,TGraphErrors*> >()));

      ::ROOT::AddClassAlternate("map<int,TGraphErrors*>","std::map<int, TGraphErrors*, std::less<int>, std::allocator<std::pair<int const, TGraphErrors*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,TGraphErrors*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOTGraphErrorsmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,TGraphErrors*>*)nullptr)->GetClass();
      maplEintcOTGraphErrorsmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOTGraphErrorsmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOTGraphErrorsmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TGraphErrors*> : new map<int,TGraphErrors*>;
   }
   static void *newArray_maplEintcOTGraphErrorsmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TGraphErrors*>[nElements] : new map<int,TGraphErrors*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOTGraphErrorsmUgR(void *p) {
      delete ((map<int,TGraphErrors*>*)p);
   }
   static void deleteArray_maplEintcOTGraphErrorsmUgR(void *p) {
      delete [] ((map<int,TGraphErrors*>*)p);
   }
   static void destruct_maplEintcOTGraphErrorsmUgR(void *p) {
      typedef map<int,TGraphErrors*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,TGraphErrors*>

namespace ROOT {
   static TClass *maplEintcOTCanvasmUgR_Dictionary();
   static void maplEintcOTCanvasmUgR_TClassManip(TClass*);
   static void *new_maplEintcOTCanvasmUgR(void *p = nullptr);
   static void *newArray_maplEintcOTCanvasmUgR(Long_t size, void *p);
   static void delete_maplEintcOTCanvasmUgR(void *p);
   static void deleteArray_maplEintcOTCanvasmUgR(void *p);
   static void destruct_maplEintcOTCanvasmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,TCanvas*>*)
   {
      map<int,TCanvas*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,TCanvas*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,TCanvas*>", -2, "map", 100,
                  typeid(map<int,TCanvas*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOTCanvasmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,TCanvas*>) );
      instance.SetNew(&new_maplEintcOTCanvasmUgR);
      instance.SetNewArray(&newArray_maplEintcOTCanvasmUgR);
      instance.SetDelete(&delete_maplEintcOTCanvasmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOTCanvasmUgR);
      instance.SetDestructor(&destruct_maplEintcOTCanvasmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,TCanvas*> >()));

      ::ROOT::AddClassAlternate("map<int,TCanvas*>","std::map<int, TCanvas*, std::less<int>, std::allocator<std::pair<int const, TCanvas*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,TCanvas*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOTCanvasmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,TCanvas*>*)nullptr)->GetClass();
      maplEintcOTCanvasmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOTCanvasmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOTCanvasmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TCanvas*> : new map<int,TCanvas*>;
   }
   static void *newArray_maplEintcOTCanvasmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TCanvas*>[nElements] : new map<int,TCanvas*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOTCanvasmUgR(void *p) {
      delete ((map<int,TCanvas*>*)p);
   }
   static void deleteArray_maplEintcOTCanvasmUgR(void *p) {
      delete [] ((map<int,TCanvas*>*)p);
   }
   static void destruct_maplEintcOTCanvasmUgR(void *p) {
      typedef map<int,TCanvas*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,TCanvas*>

namespace ROOT {
   static TClass *maplEintcOCbmTrdModuleRecmUgR_Dictionary();
   static void maplEintcOCbmTrdModuleRecmUgR_TClassManip(TClass*);
   static void *new_maplEintcOCbmTrdModuleRecmUgR(void *p = nullptr);
   static void *newArray_maplEintcOCbmTrdModuleRecmUgR(Long_t size, void *p);
   static void delete_maplEintcOCbmTrdModuleRecmUgR(void *p);
   static void deleteArray_maplEintcOCbmTrdModuleRecmUgR(void *p);
   static void destruct_maplEintcOCbmTrdModuleRecmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,CbmTrdModuleRec*>*)
   {
      map<int,CbmTrdModuleRec*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,CbmTrdModuleRec*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,CbmTrdModuleRec*>", -2, "map", 100,
                  typeid(map<int,CbmTrdModuleRec*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOCbmTrdModuleRecmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,CbmTrdModuleRec*>) );
      instance.SetNew(&new_maplEintcOCbmTrdModuleRecmUgR);
      instance.SetNewArray(&newArray_maplEintcOCbmTrdModuleRecmUgR);
      instance.SetDelete(&delete_maplEintcOCbmTrdModuleRecmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOCbmTrdModuleRecmUgR);
      instance.SetDestructor(&destruct_maplEintcOCbmTrdModuleRecmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,CbmTrdModuleRec*> >()));

      ::ROOT::AddClassAlternate("map<int,CbmTrdModuleRec*>","std::map<int, CbmTrdModuleRec*, std::less<int>, std::allocator<std::pair<int const, CbmTrdModuleRec*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,CbmTrdModuleRec*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOCbmTrdModuleRecmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,CbmTrdModuleRec*>*)nullptr)->GetClass();
      maplEintcOCbmTrdModuleRecmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOCbmTrdModuleRecmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOCbmTrdModuleRecmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmTrdModuleRec*> : new map<int,CbmTrdModuleRec*>;
   }
   static void *newArray_maplEintcOCbmTrdModuleRecmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmTrdModuleRec*>[nElements] : new map<int,CbmTrdModuleRec*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOCbmTrdModuleRecmUgR(void *p) {
      delete ((map<int,CbmTrdModuleRec*>*)p);
   }
   static void deleteArray_maplEintcOCbmTrdModuleRecmUgR(void *p) {
      delete [] ((map<int,CbmTrdModuleRec*>*)p);
   }
   static void destruct_maplEintcOCbmTrdModuleRecmUgR(void *p) {
      typedef map<int,CbmTrdModuleRec*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,CbmTrdModuleRec*>

namespace ROOT {
   static TClass *maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_Dictionary();
   static void maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_TClassManip(TClass*);
   static void *new_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p = nullptr);
   static void *newArray_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(Long_t size, void *p);
   static void delete_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p);
   static void deleteArray_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p);
   static void destruct_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > >*)
   {
      map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > >));
      static ::ROOT::TGenericClassInfo 
         instance("map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > >", -2, "map", 100,
                  typeid(map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > >) );
      instance.SetNew(&new_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.SetNewArray(&newArray_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.SetDelete(&delete_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.SetDestructor(&destruct_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > > >()));

      ::ROOT::AddClassAlternate("map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > >","std::map<CbmTrdUnpackMonitor::eRawHistos, std::map<unsigned int, std::shared_ptr<TH1>, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::shared_ptr<TH1> > > >, std::less<CbmTrdUnpackMonitor::eRawHistos>, std::allocator<std::pair<CbmTrdUnpackMonitor::eRawHistos const, std::map<unsigned int, std::shared_ptr<TH1>, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::shared_ptr<TH1> > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > >*)nullptr)->GetClass();
      maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > > : new map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > >;
   }
   static void *newArray_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > >[nElements] : new map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p) {
      delete ((map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > >*)p);
   }
   static void deleteArray_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p) {
      delete [] ((map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > >*)p);
   }
   static void destruct_maplECbmTrdUnpackMonitorcLcLeRawHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p) {
      typedef map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<CbmTrdUnpackMonitor::eRawHistos,map<unsigned int,shared_ptr<TH1> > >

namespace ROOT {
   static TClass *maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_Dictionary();
   static void maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_TClassManip(TClass*);
   static void *new_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p = nullptr);
   static void *newArray_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(Long_t size, void *p);
   static void delete_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p);
   static void deleteArray_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p);
   static void destruct_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > >*)
   {
      map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > >));
      static ::ROOT::TGenericClassInfo 
         instance("map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > >", -2, "map", 100,
                  typeid(map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > >) );
      instance.SetNew(&new_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.SetNewArray(&newArray_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.SetDelete(&delete_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.SetDestructor(&destruct_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > > >()));

      ::ROOT::AddClassAlternate("map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > >","std::map<CbmTrdUnpackMonitor::eOtherHistos, std::map<unsigned int, std::shared_ptr<TH1>, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::shared_ptr<TH1> > > >, std::less<CbmTrdUnpackMonitor::eOtherHistos>, std::allocator<std::pair<CbmTrdUnpackMonitor::eOtherHistos const, std::map<unsigned int, std::shared_ptr<TH1>, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::shared_ptr<TH1> > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > >*)nullptr)->GetClass();
      maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > > : new map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > >;
   }
   static void *newArray_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > >[nElements] : new map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p) {
      delete ((map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > >*)p);
   }
   static void deleteArray_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p) {
      delete [] ((map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > >*)p);
   }
   static void destruct_maplECbmTrdUnpackMonitorcLcLeOtherHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p) {
      typedef map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<CbmTrdUnpackMonitor::eOtherHistos,map<unsigned int,shared_ptr<TH1> > >

namespace ROOT {
   static TClass *maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_Dictionary();
   static void maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_TClassManip(TClass*);
   static void *new_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p = nullptr);
   static void *newArray_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(Long_t size, void *p);
   static void delete_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p);
   static void deleteArray_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p);
   static void destruct_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > >*)
   {
      map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > >));
      static ::ROOT::TGenericClassInfo 
         instance("map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > >", -2, "map", 100,
                  typeid(map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > >) );
      instance.SetNew(&new_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.SetNewArray(&newArray_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.SetDelete(&delete_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.SetDestructor(&destruct_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > > >()));

      ::ROOT::AddClassAlternate("map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > >","std::map<CbmTrdUnpackMonitor::eDigiHistos, std::map<unsigned int, std::shared_ptr<TH1>, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::shared_ptr<TH1> > > >, std::less<CbmTrdUnpackMonitor::eDigiHistos>, std::allocator<std::pair<CbmTrdUnpackMonitor::eDigiHistos const, std::map<unsigned int, std::shared_ptr<TH1>, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::shared_ptr<TH1> > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > >*)nullptr)->GetClass();
      maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > > : new map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > >;
   }
   static void *newArray_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > >[nElements] : new map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p) {
      delete ((map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > >*)p);
   }
   static void deleteArray_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p) {
      delete [] ((map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > >*)p);
   }
   static void destruct_maplECbmTrdUnpackMonitorcLcLeDigiHistoscOmaplEunsignedsPintcOshared_ptrlETH1gRsPgRsPgR(void *p) {
      typedef map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<CbmTrdUnpackMonitor::eDigiHistos,map<unsigned int,shared_ptr<TH1> > >

namespace ROOT {
   static TClass *listlECbmTrdClustermUgR_Dictionary();
   static void listlECbmTrdClustermUgR_TClassManip(TClass*);
   static void *new_listlECbmTrdClustermUgR(void *p = nullptr);
   static void *newArray_listlECbmTrdClustermUgR(Long_t size, void *p);
   static void delete_listlECbmTrdClustermUgR(void *p);
   static void deleteArray_listlECbmTrdClustermUgR(void *p);
   static void destruct_listlECbmTrdClustermUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const list<CbmTrdCluster*>*)
   {
      list<CbmTrdCluster*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(list<CbmTrdCluster*>));
      static ::ROOT::TGenericClassInfo 
         instance("list<CbmTrdCluster*>", -2, "list", 556,
                  typeid(list<CbmTrdCluster*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &listlECbmTrdClustermUgR_Dictionary, isa_proxy, 0,
                  sizeof(list<CbmTrdCluster*>) );
      instance.SetNew(&new_listlECbmTrdClustermUgR);
      instance.SetNewArray(&newArray_listlECbmTrdClustermUgR);
      instance.SetDelete(&delete_listlECbmTrdClustermUgR);
      instance.SetDeleteArray(&deleteArray_listlECbmTrdClustermUgR);
      instance.SetDestructor(&destruct_listlECbmTrdClustermUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< list<CbmTrdCluster*> >()));

      ::ROOT::AddClassAlternate("list<CbmTrdCluster*>","std::__cxx11::list<CbmTrdCluster*, std::allocator<CbmTrdCluster*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const list<CbmTrdCluster*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *listlECbmTrdClustermUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const list<CbmTrdCluster*>*)nullptr)->GetClass();
      listlECbmTrdClustermUgR_TClassManip(theClass);
   return theClass;
   }

   static void listlECbmTrdClustermUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_listlECbmTrdClustermUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<CbmTrdCluster*> : new list<CbmTrdCluster*>;
   }
   static void *newArray_listlECbmTrdClustermUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<CbmTrdCluster*>[nElements] : new list<CbmTrdCluster*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_listlECbmTrdClustermUgR(void *p) {
      delete ((list<CbmTrdCluster*>*)p);
   }
   static void deleteArray_listlECbmTrdClustermUgR(void *p) {
      delete [] ((list<CbmTrdCluster*>*)p);
   }
   static void destruct_listlECbmTrdClustermUgR(void *p) {
      typedef list<CbmTrdCluster*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class list<CbmTrdCluster*>

namespace ROOT {
   static TClass *dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR_Dictionary();
   static void dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR_TClassManip(TClass*);
   static void *new_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR(void *p = nullptr);
   static void *newArray_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR(Long_t size, void *p);
   static void delete_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR(void *p);
   static void deleteArray_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR(void *p);
   static void destruct_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const deque<tuple<int,bool,const CbmTrdDigi*> >*)
   {
      deque<tuple<int,bool,const CbmTrdDigi*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(deque<tuple<int,bool,const CbmTrdDigi*> >));
      static ::ROOT::TGenericClassInfo 
         instance("deque<tuple<int,bool,const CbmTrdDigi*> >", -2, "deque", 766,
                  typeid(deque<tuple<int,bool,const CbmTrdDigi*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(deque<tuple<int,bool,const CbmTrdDigi*> >) );
      instance.SetNew(&new_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR);
      instance.SetNewArray(&newArray_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR);
      instance.SetDelete(&delete_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR);
      instance.SetDeleteArray(&deleteArray_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR);
      instance.SetDestructor(&destruct_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< deque<tuple<int,bool,const CbmTrdDigi*> > >()));

      ::ROOT::AddClassAlternate("deque<tuple<int,bool,const CbmTrdDigi*> >","std::deque<std::tuple<int, bool, CbmTrdDigi const*>, std::allocator<std::tuple<int, bool, CbmTrdDigi const*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const deque<tuple<int,bool,const CbmTrdDigi*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const deque<tuple<int,bool,const CbmTrdDigi*> >*)nullptr)->GetClass();
      dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) deque<tuple<int,bool,const CbmTrdDigi*> > : new deque<tuple<int,bool,const CbmTrdDigi*> >;
   }
   static void *newArray_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) deque<tuple<int,bool,const CbmTrdDigi*> >[nElements] : new deque<tuple<int,bool,const CbmTrdDigi*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR(void *p) {
      delete ((deque<tuple<int,bool,const CbmTrdDigi*> >*)p);
   }
   static void deleteArray_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR(void *p) {
      delete [] ((deque<tuple<int,bool,const CbmTrdDigi*> >*)p);
   }
   static void destruct_dequelEtuplelEintcOboolcOconstsPCbmTrdDigimUgRsPgR(void *p) {
      typedef deque<tuple<int,bool,const CbmTrdDigi*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class deque<tuple<int,bool,const CbmTrdDigi*> >

namespace ROOT {
   static TClass *dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR_Dictionary();
   static void dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR_TClassManip(TClass*);
   static void *new_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR(void *p = nullptr);
   static void *newArray_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR(Long_t size, void *p);
   static void delete_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR(void *p);
   static void deleteArray_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR(void *p);
   static void destruct_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const deque<pair<int,const CbmTrdDigi*> >*)
   {
      deque<pair<int,const CbmTrdDigi*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(deque<pair<int,const CbmTrdDigi*> >));
      static ::ROOT::TGenericClassInfo 
         instance("deque<pair<int,const CbmTrdDigi*> >", -2, "deque", 766,
                  typeid(deque<pair<int,const CbmTrdDigi*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(deque<pair<int,const CbmTrdDigi*> >) );
      instance.SetNew(&new_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR);
      instance.SetNewArray(&newArray_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR);
      instance.SetDelete(&delete_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR);
      instance.SetDeleteArray(&deleteArray_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR);
      instance.SetDestructor(&destruct_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< deque<pair<int,const CbmTrdDigi*> > >()));

      ::ROOT::AddClassAlternate("deque<pair<int,const CbmTrdDigi*> >","std::deque<std::pair<int, CbmTrdDigi const*>, std::allocator<std::pair<int, CbmTrdDigi const*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const deque<pair<int,const CbmTrdDigi*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const deque<pair<int,const CbmTrdDigi*> >*)nullptr)->GetClass();
      dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) deque<pair<int,const CbmTrdDigi*> > : new deque<pair<int,const CbmTrdDigi*> >;
   }
   static void *newArray_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) deque<pair<int,const CbmTrdDigi*> >[nElements] : new deque<pair<int,const CbmTrdDigi*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR(void *p) {
      delete ((deque<pair<int,const CbmTrdDigi*> >*)p);
   }
   static void deleteArray_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR(void *p) {
      delete [] ((deque<pair<int,const CbmTrdDigi*> >*)p);
   }
   static void destruct_dequelEpairlEintcOconstsPCbmTrdDigimUgRsPgR(void *p) {
      typedef deque<pair<int,const CbmTrdDigi*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class deque<pair<int,const CbmTrdDigi*> >

namespace ROOT {
   static TClass *dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR_Dictionary();
   static void dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR_TClassManip(TClass*);
   static void *new_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR(void *p = nullptr);
   static void *newArray_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR(Long_t size, void *p);
   static void delete_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR(void *p);
   static void deleteArray_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR(void *p);
   static void destruct_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const deque<deque<pair<int,const CbmTrdDigi*> > >*)
   {
      deque<deque<pair<int,const CbmTrdDigi*> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(deque<deque<pair<int,const CbmTrdDigi*> > >));
      static ::ROOT::TGenericClassInfo 
         instance("deque<deque<pair<int,const CbmTrdDigi*> > >", -2, "deque", 766,
                  typeid(deque<deque<pair<int,const CbmTrdDigi*> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(deque<deque<pair<int,const CbmTrdDigi*> > >) );
      instance.SetNew(&new_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR);
      instance.SetNewArray(&newArray_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR);
      instance.SetDelete(&delete_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR);
      instance.SetDestructor(&destruct_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< deque<deque<pair<int,const CbmTrdDigi*> > > >()));

      ::ROOT::AddClassAlternate("deque<deque<pair<int,const CbmTrdDigi*> > >","std::deque<std::deque<std::pair<int, CbmTrdDigi const*>, std::allocator<std::pair<int, CbmTrdDigi const*> > >, std::allocator<std::deque<std::pair<int, CbmTrdDigi const*>, std::allocator<std::pair<int, CbmTrdDigi const*> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const deque<deque<pair<int,const CbmTrdDigi*> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const deque<deque<pair<int,const CbmTrdDigi*> > >*)nullptr)->GetClass();
      dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) deque<deque<pair<int,const CbmTrdDigi*> > > : new deque<deque<pair<int,const CbmTrdDigi*> > >;
   }
   static void *newArray_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) deque<deque<pair<int,const CbmTrdDigi*> > >[nElements] : new deque<deque<pair<int,const CbmTrdDigi*> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR(void *p) {
      delete ((deque<deque<pair<int,const CbmTrdDigi*> > >*)p);
   }
   static void deleteArray_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR(void *p) {
      delete [] ((deque<deque<pair<int,const CbmTrdDigi*> > >*)p);
   }
   static void destruct_dequelEdequelEpairlEintcOconstsPCbmTrdDigimUgRsPgRsPgR(void *p) {
      typedef deque<deque<pair<int,const CbmTrdDigi*> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class deque<deque<pair<int,const CbmTrdDigi*> > >

namespace {
  void TriggerDictionaryInitialization_libCbmTrdReco_Impl() {
    static const char* headers[] = {
"CbmTrdTrackFinderIdeal.h",
"CbmTrdTrackFitterIdeal.h",
"CbmTrdClusterFinder.h",
"CbmTrdHitProducer.h",
"CbmTrdModuleRec.h",
"CbmTrdModuleRecR.h",
"CbmTrdModuleRec2D.h",
"CbmTrdDigiRec.h",
"CbmTrdUnpackConfig.h",
"CbmTrdUnpackAlgoBaseR.h",
"CbmTrdUnpackAlgoR.h",
"CbmTrdUnpackAlgoLegacy2020R.h",
"CbmTrdUnpackMonitor.h",
"CbmTrdUnpackFaspAlgo.h",
"CbmTrdUnpackFaspConfig.h",
"CbmTrdUnpackFaspMonitor.h",
"CbmTrdClusterizerFastQa.h",
"CbmTrdHitDensityQa.h",
"CbmTrdHitProducerClusterQa.h",
"CbmTrdHitProducerQa.h",
"CbmTrdOccupancyQa.h",
"CbmTrdQa.h",
"CbmTrdRecoQa.h",
"CbmTrdTracksPidQa.h",
"CbmTrdCalibTracker.h",
"CbmTrdHitMC.h",
"CbmTrdElectronsTrainAnn.h",
"CbmTrdSetTracksPidWkn.h",
"CbmTrdSetTracksPidModWkn.h",
"CbmTrdSetTracksPidLike.h",
"CbmTrdSetTracksPidANN.h",
"CbmTrdRawToDigiBaseR.h",
"CbmTrdRawToDigiMaxAdcR.h",
"CbmTrdRawToDigiFitR.h",
"CbmTrdRawToDigiLookUpCorrR.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa/data",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/pid",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/rawToDigiMethods",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/unpack",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa/data",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/pid",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/rawToDigiMethods",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/unpack",
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
"/home/heinemann/gpuunpacker/cbmroot/core/qa/checker",
"/home/heinemann/gpuunpacker/cbmroot/core/qa",
"/home/heinemann/gpuunpacker/cbmroot/external/yaml-cpp/include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/steer",
"/home/heinemann/gpuunpacker/cbmroot/core/config",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/geosetup",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
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
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/detectors/trd/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmTrdReco dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace std{template <typename _T1, typename _T2> struct __attribute__((annotate("$clingAutoload$bits/stl_iterator.h")))  __attribute__((annotate("$clingAutoload$string")))  pair;
}
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$CbmTrdTrackFinderIdeal.h")))  CbmTrdTrackFinderIdeal;
class __attribute__((annotate("$clingAutoload$CbmTrdTrackFitterIdeal.h")))  CbmTrdTrackFitterIdeal;
class __attribute__((annotate("$clingAutoload$CbmTrdClusterFinder.h")))  CbmTrdClusterFinder;
class __attribute__((annotate("$clingAutoload$CbmTrdHitProducer.h")))  CbmTrdHitProducer;
class __attribute__((annotate("$clingAutoload$CbmTrdModuleRec.h")))  CbmTrdModuleRec;
class __attribute__((annotate(R"ATTRDUMP(Rectangular pad module; Cluster finding and hit reconstruction algorithms)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdModuleRecR.h")))  CbmTrdModuleRecR;
class __attribute__((annotate(R"ATTRDUMP(Triangular pad module; Cluster finding and hit reconstruction algorithms)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdModuleRec2D.h")))  CbmTrdModuleRec2D;
class __attribute__((annotate(R"ATTRDUMP(Wrapper around the RAW TRD digi (CbmTrdDigi) to acount for calibration)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Wrapper around the RAW TRD digi (CbmTrdDigi) to acount for calibration)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Wrapper around the RAW TRD digi (CbmTrdDigi) to acount for calibration)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Wrapper around the RAW TRD digi (CbmTrdDigi) to acount for calibration)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdDigiRec.h")))  CbmTrdDigiRec;
class __attribute__((annotate("$clingAutoload$CbmTrdRawToDigiBaseR.h")))  __attribute__((annotate("$clingAutoload$CbmTrdUnpackConfig.h")))  CbmTrdRawToDigiBaseR;
class __attribute__((annotate("$clingAutoload$CbmTrdRawToDigiMaxAdcR.h")))  __attribute__((annotate("$clingAutoload$CbmTrdUnpackConfig.h")))  CbmTrdRawToDigiMaxAdcR;
class __attribute__((annotate("$clingAutoload$CbmTrdUnpackMonitor.h")))  __attribute__((annotate("$clingAutoload$CbmTrdUnpackConfig.h")))  CbmTrdUnpackMonitor;
class __attribute__((annotate("$clingAutoload$CbmTrdUnpackAlgoBaseR.h")))  __attribute__((annotate("$clingAutoload$CbmTrdUnpackConfig.h")))  CbmTrdUnpackAlgoBaseR;
class __attribute__((annotate("$clingAutoload$CbmTrdUnpackConfig.h")))  CbmTrdUnpackConfig;
class __attribute__((annotate("$clingAutoload$CbmTrdUnpackAlgoR.h")))  CbmTrdUnpackAlgoR;
class __attribute__((annotate("$clingAutoload$CbmTrdUnpackAlgoLegacy2020R.h")))  CbmTrdUnpackAlgoLegacy2020R;
class __attribute__((annotate("$clingAutoload$CbmTrdUnpackFaspMonitor.h")))  __attribute__((annotate("$clingAutoload$CbmTrdUnpackFaspAlgo.h")))  CbmTrdUnpackFaspMonitor;
class __attribute__((annotate(R"ATTRDUMP(unpack FASP read-out detectors)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdUnpackFaspAlgo.h")))  CbmTrdUnpackFaspAlgo;
class __attribute__((annotate("$clingAutoload$CbmTrdUnpackFaspConfig.h")))  CbmTrdUnpackFaspConfig;
class __attribute__((annotate("$clingAutoload$CbmTrdClusterizerFastQa.h")))  CbmTrdClusterizerFastQa;
class __attribute__((annotate("$clingAutoload$CbmTrdHitDensityQa.h")))  CbmTrdHitDensityQa;
class __attribute__((annotate("$clingAutoload$CbmTrdHitProducerClusterQa.h")))  CbmTrdHitProducerClusterQa;
class __attribute__((annotate("$clingAutoload$CbmTrdHitProducerQa.h")))  CbmTrdHitProducerQa;
class __attribute__((annotate("$clingAutoload$CbmTrdOccupancyQa.h")))  CbmTrdOccupancyQa;
class __attribute__((annotate("$clingAutoload$CbmTrdQa.h")))  CbmTrdQa;
class __attribute__((annotate("$clingAutoload$CbmTrdRecoQa.h")))  CbmTrdRecoQa;
class __attribute__((annotate("$clingAutoload$CbmTrdTracksPidQa.h")))  CbmTrdTracksPidQa;
class __attribute__((annotate("$clingAutoload$CbmTrdCalibTracker.h")))  CbmTrdCalibTracker;
class __attribute__((annotate(R"ATTRDUMP(Hit to MC point data correlation)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdHitMC.h")))  CbmTrdHitMC;
class __attribute__((annotate("$clingAutoload$CbmTrdElectronsTrainAnn.h")))  CbmTrdElectronsTrainAnn;
class __attribute__((annotate("$clingAutoload$CbmTrdSetTracksPidWkn.h")))  CbmTrdSetTracksPidWkn;
class __attribute__((annotate("$clingAutoload$CbmTrdSetTracksPidModWkn.h")))  CbmTrdSetTracksPidModWkn;
class __attribute__((annotate("$clingAutoload$CbmTrdSetTracksPidLike.h")))  CbmTrdSetTracksPidLike;
class __attribute__((annotate("$clingAutoload$CbmTrdSetTracksPidANN.h")))  CbmTrdSetTracksPidANN;
class __attribute__((annotate("$clingAutoload$CbmTrdRawToDigiFitR.h")))  CbmTrdRawToDigiFitR;
class __attribute__((annotate("$clingAutoload$CbmTrdRawToDigiLookUpCorrR.h")))  CbmTrdRawToDigiLookUpCorrR;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmTrdReco dictionary payload"

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
#include "CbmTrdTrackFinderIdeal.h"
#include "CbmTrdTrackFitterIdeal.h"
#include "CbmTrdClusterFinder.h"
#include "CbmTrdHitProducer.h"
#include "CbmTrdModuleRec.h"
#include "CbmTrdModuleRecR.h"
#include "CbmTrdModuleRec2D.h"
#include "CbmTrdDigiRec.h"
#include "CbmTrdUnpackConfig.h"
#include "CbmTrdUnpackAlgoBaseR.h"
#include "CbmTrdUnpackAlgoR.h"
#include "CbmTrdUnpackAlgoLegacy2020R.h"
#include "CbmTrdUnpackMonitor.h"
#include "CbmTrdUnpackFaspAlgo.h"
#include "CbmTrdUnpackFaspConfig.h"
#include "CbmTrdUnpackFaspMonitor.h"
#include "CbmTrdClusterizerFastQa.h"
#include "CbmTrdHitDensityQa.h"
#include "CbmTrdHitProducerClusterQa.h"
#include "CbmTrdHitProducerQa.h"
#include "CbmTrdOccupancyQa.h"
#include "CbmTrdQa.h"
#include "CbmTrdRecoQa.h"
#include "CbmTrdTracksPidQa.h"
#include "CbmTrdCalibTracker.h"
#include "CbmTrdHitMC.h"
#include "CbmTrdElectronsTrainAnn.h"
#include "CbmTrdSetTracksPidWkn.h"
#include "CbmTrdSetTracksPidModWkn.h"
#include "CbmTrdSetTracksPidLike.h"
#include "CbmTrdSetTracksPidANN.h"
#include "CbmTrdRawToDigiBaseR.h"
#include "CbmTrdRawToDigiMaxAdcR.h"
#include "CbmTrdRawToDigiFitR.h"
#include "CbmTrdRawToDigiLookUpCorrR.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmTrdCalibTracker", payloadCode, "@",
"CbmTrdClusterFinder", payloadCode, "@",
"CbmTrdClusterizerFastQa", payloadCode, "@",
"CbmTrdDigiRec", payloadCode, "@",
"CbmTrdElectronsTrainAnn", payloadCode, "@",
"CbmTrdHitDensityQa", payloadCode, "@",
"CbmTrdHitMC", payloadCode, "@",
"CbmTrdHitProducer", payloadCode, "@",
"CbmTrdHitProducerClusterQa", payloadCode, "@",
"CbmTrdHitProducerQa", payloadCode, "@",
"CbmTrdModuleRec", payloadCode, "@",
"CbmTrdModuleRec2D", payloadCode, "@",
"CbmTrdModuleRecR", payloadCode, "@",
"CbmTrdOccupancyQa", payloadCode, "@",
"CbmTrdQa", payloadCode, "@",
"CbmTrdRawToDigiBaseR", payloadCode, "@",
"CbmTrdRawToDigiFitR", payloadCode, "@",
"CbmTrdRawToDigiLookUpCorrR", payloadCode, "@",
"CbmTrdRawToDigiMaxAdcR", payloadCode, "@",
"CbmTrdRecoQa", payloadCode, "@",
"CbmTrdSetTracksPidANN", payloadCode, "@",
"CbmTrdSetTracksPidLike", payloadCode, "@",
"CbmTrdSetTracksPidModWkn", payloadCode, "@",
"CbmTrdSetTracksPidWkn", payloadCode, "@",
"CbmTrdTrackFinderIdeal", payloadCode, "@",
"CbmTrdTrackFitterIdeal", payloadCode, "@",
"CbmTrdTracksPidQa", payloadCode, "@",
"CbmTrdUnpackAlgoBaseR", payloadCode, "@",
"CbmTrdUnpackAlgoLegacy2020R", payloadCode, "@",
"CbmTrdUnpackAlgoR", payloadCode, "@",
"CbmTrdUnpackConfig", payloadCode, "@",
"CbmTrdUnpackFaspAlgo", payloadCode, "@",
"CbmTrdUnpackFaspConfig", payloadCode, "@",
"CbmTrdUnpackFaspMonitor", payloadCode, "@",
"CbmTrdUnpackMonitor", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmTrdReco",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmTrdReco_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmTrdReco_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmTrdReco() {
  TriggerDictionaryInitialization_libCbmTrdReco_Impl();
}
