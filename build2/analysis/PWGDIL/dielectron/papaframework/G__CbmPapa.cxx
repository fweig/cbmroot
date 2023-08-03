// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmPapa
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
#include "AnalysisCuts.h"
#include "AnalysisFilter.h"
#include "PairAnalysisTrack.h"
#include "PairAnalysisEvent.h"
#include "PairAnalysisPair.h"
#include "PairAnalysisPairKF.h"
#include "PairAnalysisPairLV.h"
#include "PairAnalysisHelper.h"
#include "PairAnalysisStyler.h"
#include "PairAnalysisCutQa.h"
#include "PairAnalysisVarCuts.h"
#include "PairAnalysisObjectCuts.h"
#include "PairAnalysisPairLegCuts.h"
#include "PairAnalysisCutCombi.h"
#include "PairAnalysisCutGroup.h"
#include "PairAnalysisVarManager.h"
#include "PairAnalysisMetaData.h"
#include "AnalysisTaskMultiPairAnalysis.h"
#include "PairAnalysis.h"
#include "PairAnalysisHF.h"
#include "PairAnalysisHistos.h"
#include "PairAnalysisMC.h"
#include "PairAnalysisSignalMC.h"
#include "PairAnalysisTrackRotator.h"
#include "PairAnalysisMixedEvent.h"
#include "PairAnalysisMixingHandler.h"
#include "PairAnalysisFunction.h"
#include "PairAnalysisSignalExt.h"
#include "PairAnalysisSpectrum.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace PairAnalysisHelper {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *PairAnalysisHelper_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("PairAnalysisHelper", 0 /*version*/, "PairAnalysisHelper.h", 31,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &PairAnalysisHelper_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *PairAnalysisHelper_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace PairAnalysisStyler {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *PairAnalysisStyler_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("PairAnalysisStyler", 0 /*version*/, "PairAnalysisStyler.h", 27,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &PairAnalysisStyler_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *PairAnalysisStyler_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void delete_AnalysisCuts(void *p);
   static void deleteArray_AnalysisCuts(void *p);
   static void destruct_AnalysisCuts(void *p);
   static Long64_t merge_AnalysisCuts(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisCuts*)
   {
      ::AnalysisCuts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisCuts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisCuts", ::AnalysisCuts::Class_Version(), "AnalysisCuts.h", 12,
                  typeid(::AnalysisCuts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisCuts::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisCuts) );
      instance.SetDelete(&delete_AnalysisCuts);
      instance.SetDeleteArray(&deleteArray_AnalysisCuts);
      instance.SetDestructor(&destruct_AnalysisCuts);
      instance.SetMerge(&merge_AnalysisCuts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisCuts*)
   {
      return GenerateInitInstanceLocal((::AnalysisCuts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisCuts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisFilter(void *p = nullptr);
   static void *newArray_AnalysisFilter(Long_t size, void *p);
   static void delete_AnalysisFilter(void *p);
   static void deleteArray_AnalysisFilter(void *p);
   static void destruct_AnalysisFilter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisFilter*)
   {
      ::AnalysisFilter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisFilter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisFilter", ::AnalysisFilter::Class_Version(), "AnalysisFilter.h", 15,
                  typeid(::AnalysisFilter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisFilter::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisFilter) );
      instance.SetNew(&new_AnalysisFilter);
      instance.SetNewArray(&newArray_AnalysisFilter);
      instance.SetDelete(&delete_AnalysisFilter);
      instance.SetDeleteArray(&deleteArray_AnalysisFilter);
      instance.SetDestructor(&destruct_AnalysisFilter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisFilter*)
   {
      return GenerateInitInstanceLocal((::AnalysisFilter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisFilter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisTrack(void *p = nullptr);
   static void *newArray_PairAnalysisTrack(Long_t size, void *p);
   static void delete_PairAnalysisTrack(void *p);
   static void deleteArray_PairAnalysisTrack(void *p);
   static void destruct_PairAnalysisTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisTrack*)
   {
      ::PairAnalysisTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisTrack", ::PairAnalysisTrack::Class_Version(), "PairAnalysisTrack.h", 41,
                  typeid(::PairAnalysisTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisTrack::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisTrack) );
      instance.SetNew(&new_PairAnalysisTrack);
      instance.SetNewArray(&newArray_PairAnalysisTrack);
      instance.SetDelete(&delete_PairAnalysisTrack);
      instance.SetDeleteArray(&deleteArray_PairAnalysisTrack);
      instance.SetDestructor(&destruct_PairAnalysisTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisTrack*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisEvent(void *p = nullptr);
   static void *newArray_PairAnalysisEvent(Long_t size, void *p);
   static void delete_PairAnalysisEvent(void *p);
   static void deleteArray_PairAnalysisEvent(void *p);
   static void destruct_PairAnalysisEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisEvent*)
   {
      ::PairAnalysisEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisEvent", ::PairAnalysisEvent::Class_Version(), "PairAnalysisEvent.h", 31,
                  typeid(::PairAnalysisEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisEvent::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisEvent) );
      instance.SetNew(&new_PairAnalysisEvent);
      instance.SetNewArray(&newArray_PairAnalysisEvent);
      instance.SetDelete(&delete_PairAnalysisEvent);
      instance.SetDeleteArray(&deleteArray_PairAnalysisEvent);
      instance.SetDestructor(&destruct_PairAnalysisEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisEvent*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisTrackRotator(void *p = nullptr);
   static void *newArray_PairAnalysisTrackRotator(Long_t size, void *p);
   static void delete_PairAnalysisTrackRotator(void *p);
   static void deleteArray_PairAnalysisTrackRotator(void *p);
   static void destruct_PairAnalysisTrackRotator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisTrackRotator*)
   {
      ::PairAnalysisTrackRotator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisTrackRotator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisTrackRotator", ::PairAnalysisTrackRotator::Class_Version(), "PairAnalysisTrackRotator.h", 21,
                  typeid(::PairAnalysisTrackRotator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisTrackRotator::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisTrackRotator) );
      instance.SetNew(&new_PairAnalysisTrackRotator);
      instance.SetNewArray(&newArray_PairAnalysisTrackRotator);
      instance.SetDelete(&delete_PairAnalysisTrackRotator);
      instance.SetDeleteArray(&deleteArray_PairAnalysisTrackRotator);
      instance.SetDestructor(&destruct_PairAnalysisTrackRotator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisTrackRotator*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisTrackRotator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisTrackRotator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_PairAnalysisPair(void *p);
   static void deleteArray_PairAnalysisPair(void *p);
   static void destruct_PairAnalysisPair(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisPair*)
   {
      ::PairAnalysisPair *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisPair >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisPair", ::PairAnalysisPair::Class_Version(), "PairAnalysisPair.h", 29,
                  typeid(::PairAnalysisPair), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisPair::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisPair) );
      instance.SetDelete(&delete_PairAnalysisPair);
      instance.SetDeleteArray(&deleteArray_PairAnalysisPair);
      instance.SetDestructor(&destruct_PairAnalysisPair);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisPair*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisPair*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisPair*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisPairKF(void *p = nullptr);
   static void *newArray_PairAnalysisPairKF(Long_t size, void *p);
   static void delete_PairAnalysisPairKF(void *p);
   static void deleteArray_PairAnalysisPairKF(void *p);
   static void destruct_PairAnalysisPairKF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisPairKF*)
   {
      ::PairAnalysisPairKF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisPairKF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisPairKF", ::PairAnalysisPairKF::Class_Version(), "PairAnalysisPairKF.h", 29,
                  typeid(::PairAnalysisPairKF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisPairKF::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisPairKF) );
      instance.SetNew(&new_PairAnalysisPairKF);
      instance.SetNewArray(&newArray_PairAnalysisPairKF);
      instance.SetDelete(&delete_PairAnalysisPairKF);
      instance.SetDeleteArray(&deleteArray_PairAnalysisPairKF);
      instance.SetDestructor(&destruct_PairAnalysisPairKF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisPairKF*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisPairKF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisPairKF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisPairLV(void *p = nullptr);
   static void *newArray_PairAnalysisPairLV(Long_t size, void *p);
   static void delete_PairAnalysisPairLV(void *p);
   static void deleteArray_PairAnalysisPairLV(void *p);
   static void destruct_PairAnalysisPairLV(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisPairLV*)
   {
      ::PairAnalysisPairLV *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisPairLV >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisPairLV", ::PairAnalysisPairLV::Class_Version(), "PairAnalysisPairLV.h", 29,
                  typeid(::PairAnalysisPairLV), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisPairLV::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisPairLV) );
      instance.SetNew(&new_PairAnalysisPairLV);
      instance.SetNewArray(&newArray_PairAnalysisPairLV);
      instance.SetDelete(&delete_PairAnalysisPairLV);
      instance.SetDeleteArray(&deleteArray_PairAnalysisPairLV);
      instance.SetDestructor(&destruct_PairAnalysisPairLV);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisPairLV*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisPairLV*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisPairLV*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisCutQa(void *p = nullptr);
   static void *newArray_PairAnalysisCutQa(Long_t size, void *p);
   static void delete_PairAnalysisCutQa(void *p);
   static void deleteArray_PairAnalysisCutQa(void *p);
   static void destruct_PairAnalysisCutQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisCutQa*)
   {
      ::PairAnalysisCutQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisCutQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisCutQa", ::PairAnalysisCutQa::Class_Version(), "PairAnalysisCutQa.h", 28,
                  typeid(::PairAnalysisCutQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisCutQa::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisCutQa) );
      instance.SetNew(&new_PairAnalysisCutQa);
      instance.SetNewArray(&newArray_PairAnalysisCutQa);
      instance.SetDelete(&delete_PairAnalysisCutQa);
      instance.SetDeleteArray(&deleteArray_PairAnalysisCutQa);
      instance.SetDestructor(&destruct_PairAnalysisCutQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisCutQa*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisCutQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisCutQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisSignalMC(void *p = nullptr);
   static void *newArray_PairAnalysisSignalMC(Long_t size, void *p);
   static void delete_PairAnalysisSignalMC(void *p);
   static void deleteArray_PairAnalysisSignalMC(void *p);
   static void destruct_PairAnalysisSignalMC(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisSignalMC*)
   {
      ::PairAnalysisSignalMC *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisSignalMC >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisSignalMC", ::PairAnalysisSignalMC::Class_Version(), "PairAnalysisSignalMC.h", 15,
                  typeid(::PairAnalysisSignalMC), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisSignalMC::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisSignalMC) );
      instance.SetNew(&new_PairAnalysisSignalMC);
      instance.SetNewArray(&newArray_PairAnalysisSignalMC);
      instance.SetDelete(&delete_PairAnalysisSignalMC);
      instance.SetDeleteArray(&deleteArray_PairAnalysisSignalMC);
      instance.SetDestructor(&destruct_PairAnalysisSignalMC);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisSignalMC*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisSignalMC*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisSignalMC*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisMC(void *p = nullptr);
   static void *newArray_PairAnalysisMC(Long_t size, void *p);
   static void delete_PairAnalysisMC(void *p);
   static void deleteArray_PairAnalysisMC(void *p);
   static void destruct_PairAnalysisMC(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisMC*)
   {
      ::PairAnalysisMC *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisMC >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisMC", ::PairAnalysisMC::Class_Version(), "PairAnalysisMC.h", 26,
                  typeid(::PairAnalysisMC), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisMC::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisMC) );
      instance.SetNew(&new_PairAnalysisMC);
      instance.SetNewArray(&newArray_PairAnalysisMC);
      instance.SetDelete(&delete_PairAnalysisMC);
      instance.SetDeleteArray(&deleteArray_PairAnalysisMC);
      instance.SetDestructor(&destruct_PairAnalysisMC);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisMC*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisMC*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisMC*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisVarManager(void *p = nullptr);
   static void *newArray_PairAnalysisVarManager(Long_t size, void *p);
   static void delete_PairAnalysisVarManager(void *p);
   static void deleteArray_PairAnalysisVarManager(void *p);
   static void destruct_PairAnalysisVarManager(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisVarManager*)
   {
      ::PairAnalysisVarManager *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisVarManager >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisVarManager", ::PairAnalysisVarManager::Class_Version(), "PairAnalysisVarManager.h", 66,
                  typeid(::PairAnalysisVarManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisVarManager::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisVarManager) );
      instance.SetNew(&new_PairAnalysisVarManager);
      instance.SetNewArray(&newArray_PairAnalysisVarManager);
      instance.SetDelete(&delete_PairAnalysisVarManager);
      instance.SetDeleteArray(&deleteArray_PairAnalysisVarManager);
      instance.SetDestructor(&destruct_PairAnalysisVarManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisVarManager*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisVarManager*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisVarManager*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisVarCuts(void *p = nullptr);
   static void *newArray_PairAnalysisVarCuts(Long_t size, void *p);
   static void delete_PairAnalysisVarCuts(void *p);
   static void deleteArray_PairAnalysisVarCuts(void *p);
   static void destruct_PairAnalysisVarCuts(void *p);
   static Long64_t merge_PairAnalysisVarCuts(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisVarCuts*)
   {
      ::PairAnalysisVarCuts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisVarCuts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisVarCuts", ::PairAnalysisVarCuts::Class_Version(), "PairAnalysisVarCuts.h", 23,
                  typeid(::PairAnalysisVarCuts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisVarCuts::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisVarCuts) );
      instance.SetNew(&new_PairAnalysisVarCuts);
      instance.SetNewArray(&newArray_PairAnalysisVarCuts);
      instance.SetDelete(&delete_PairAnalysisVarCuts);
      instance.SetDeleteArray(&deleteArray_PairAnalysisVarCuts);
      instance.SetDestructor(&destruct_PairAnalysisVarCuts);
      instance.SetMerge(&merge_PairAnalysisVarCuts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisVarCuts*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisVarCuts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisVarCuts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisObjectCuts(void *p = nullptr);
   static void *newArray_PairAnalysisObjectCuts(Long_t size, void *p);
   static void delete_PairAnalysisObjectCuts(void *p);
   static void deleteArray_PairAnalysisObjectCuts(void *p);
   static void destruct_PairAnalysisObjectCuts(void *p);
   static Long64_t merge_PairAnalysisObjectCuts(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisObjectCuts*)
   {
      ::PairAnalysisObjectCuts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisObjectCuts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisObjectCuts", ::PairAnalysisObjectCuts::Class_Version(), "PairAnalysisObjectCuts.h", 26,
                  typeid(::PairAnalysisObjectCuts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisObjectCuts::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisObjectCuts) );
      instance.SetNew(&new_PairAnalysisObjectCuts);
      instance.SetNewArray(&newArray_PairAnalysisObjectCuts);
      instance.SetDelete(&delete_PairAnalysisObjectCuts);
      instance.SetDeleteArray(&deleteArray_PairAnalysisObjectCuts);
      instance.SetDestructor(&destruct_PairAnalysisObjectCuts);
      instance.SetMerge(&merge_PairAnalysisObjectCuts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisObjectCuts*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisObjectCuts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisObjectCuts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisPairLegCuts(void *p = nullptr);
   static void *newArray_PairAnalysisPairLegCuts(Long_t size, void *p);
   static void delete_PairAnalysisPairLegCuts(void *p);
   static void deleteArray_PairAnalysisPairLegCuts(void *p);
   static void destruct_PairAnalysisPairLegCuts(void *p);
   static Long64_t merge_PairAnalysisPairLegCuts(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisPairLegCuts*)
   {
      ::PairAnalysisPairLegCuts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisPairLegCuts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisPairLegCuts", ::PairAnalysisPairLegCuts::Class_Version(), "PairAnalysisPairLegCuts.h", 20,
                  typeid(::PairAnalysisPairLegCuts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisPairLegCuts::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisPairLegCuts) );
      instance.SetNew(&new_PairAnalysisPairLegCuts);
      instance.SetNewArray(&newArray_PairAnalysisPairLegCuts);
      instance.SetDelete(&delete_PairAnalysisPairLegCuts);
      instance.SetDeleteArray(&deleteArray_PairAnalysisPairLegCuts);
      instance.SetDestructor(&destruct_PairAnalysisPairLegCuts);
      instance.SetMerge(&merge_PairAnalysisPairLegCuts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisPairLegCuts*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisPairLegCuts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisPairLegCuts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisCutCombi(void *p = nullptr);
   static void *newArray_PairAnalysisCutCombi(Long_t size, void *p);
   static void delete_PairAnalysisCutCombi(void *p);
   static void deleteArray_PairAnalysisCutCombi(void *p);
   static void destruct_PairAnalysisCutCombi(void *p);
   static Long64_t merge_PairAnalysisCutCombi(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisCutCombi*)
   {
      ::PairAnalysisCutCombi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisCutCombi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisCutCombi", ::PairAnalysisCutCombi::Class_Version(), "PairAnalysisCutCombi.h", 24,
                  typeid(::PairAnalysisCutCombi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisCutCombi::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisCutCombi) );
      instance.SetNew(&new_PairAnalysisCutCombi);
      instance.SetNewArray(&newArray_PairAnalysisCutCombi);
      instance.SetDelete(&delete_PairAnalysisCutCombi);
      instance.SetDeleteArray(&deleteArray_PairAnalysisCutCombi);
      instance.SetDestructor(&destruct_PairAnalysisCutCombi);
      instance.SetMerge(&merge_PairAnalysisCutCombi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisCutCombi*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisCutCombi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisCutCombi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisCutGroup(void *p = nullptr);
   static void *newArray_PairAnalysisCutGroup(Long_t size, void *p);
   static void delete_PairAnalysisCutGroup(void *p);
   static void deleteArray_PairAnalysisCutGroup(void *p);
   static void destruct_PairAnalysisCutGroup(void *p);
   static Long64_t merge_PairAnalysisCutGroup(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisCutGroup*)
   {
      ::PairAnalysisCutGroup *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisCutGroup >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisCutGroup", ::PairAnalysisCutGroup::Class_Version(), "PairAnalysisCutGroup.h", 23,
                  typeid(::PairAnalysisCutGroup), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisCutGroup::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisCutGroup) );
      instance.SetNew(&new_PairAnalysisCutGroup);
      instance.SetNewArray(&newArray_PairAnalysisCutGroup);
      instance.SetDelete(&delete_PairAnalysisCutGroup);
      instance.SetDeleteArray(&deleteArray_PairAnalysisCutGroup);
      instance.SetDestructor(&destruct_PairAnalysisCutGroup);
      instance.SetMerge(&merge_PairAnalysisCutGroup);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisCutGroup*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisCutGroup*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisCutGroup*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisMetaData(void *p = nullptr);
   static void *newArray_PairAnalysisMetaData(Long_t size, void *p);
   static void delete_PairAnalysisMetaData(void *p);
   static void deleteArray_PairAnalysisMetaData(void *p);
   static void destruct_PairAnalysisMetaData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisMetaData*)
   {
      ::PairAnalysisMetaData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisMetaData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisMetaData", ::PairAnalysisMetaData::Class_Version(), "PairAnalysisMetaData.h", 30,
                  typeid(::PairAnalysisMetaData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisMetaData::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisMetaData) );
      instance.SetNew(&new_PairAnalysisMetaData);
      instance.SetNewArray(&newArray_PairAnalysisMetaData);
      instance.SetDelete(&delete_PairAnalysisMetaData);
      instance.SetDeleteArray(&deleteArray_PairAnalysisMetaData);
      instance.SetDestructor(&destruct_PairAnalysisMetaData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisMetaData*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisMetaData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisMetaData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTaskMultiPairAnalysis(void *p = nullptr);
   static void *newArray_AnalysisTaskMultiPairAnalysis(Long_t size, void *p);
   static void delete_AnalysisTaskMultiPairAnalysis(void *p);
   static void deleteArray_AnalysisTaskMultiPairAnalysis(void *p);
   static void destruct_AnalysisTaskMultiPairAnalysis(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTaskMultiPairAnalysis*)
   {
      ::AnalysisTaskMultiPairAnalysis *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTaskMultiPairAnalysis >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTaskMultiPairAnalysis", ::AnalysisTaskMultiPairAnalysis::Class_Version(), "AnalysisTaskMultiPairAnalysis.h", 32,
                  typeid(::AnalysisTaskMultiPairAnalysis), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTaskMultiPairAnalysis::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTaskMultiPairAnalysis) );
      instance.SetNew(&new_AnalysisTaskMultiPairAnalysis);
      instance.SetNewArray(&newArray_AnalysisTaskMultiPairAnalysis);
      instance.SetDelete(&delete_AnalysisTaskMultiPairAnalysis);
      instance.SetDeleteArray(&deleteArray_AnalysisTaskMultiPairAnalysis);
      instance.SetDestructor(&destruct_AnalysisTaskMultiPairAnalysis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTaskMultiPairAnalysis*)
   {
      return GenerateInitInstanceLocal((::AnalysisTaskMultiPairAnalysis*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTaskMultiPairAnalysis*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisHn(void *p = nullptr);
   static void *newArray_PairAnalysisHn(Long_t size, void *p);
   static void delete_PairAnalysisHn(void *p);
   static void deleteArray_PairAnalysisHn(void *p);
   static void destruct_PairAnalysisHn(void *p);
   static Long64_t merge_PairAnalysisHn(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisHn*)
   {
      ::PairAnalysisHn *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisHn >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisHn", ::PairAnalysisHn::Class_Version(), "PairAnalysisHistos.h", 42,
                  typeid(::PairAnalysisHn), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisHn::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisHn) );
      instance.SetNew(&new_PairAnalysisHn);
      instance.SetNewArray(&newArray_PairAnalysisHn);
      instance.SetDelete(&delete_PairAnalysisHn);
      instance.SetDeleteArray(&deleteArray_PairAnalysisHn);
      instance.SetDestructor(&destruct_PairAnalysisHn);
      instance.SetMerge(&merge_PairAnalysisHn);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisHn*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisHn*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisHn*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisHistos(void *p = nullptr);
   static void *newArray_PairAnalysisHistos(Long_t size, void *p);
   static void delete_PairAnalysisHistos(void *p);
   static void deleteArray_PairAnalysisHistos(void *p);
   static void destruct_PairAnalysisHistos(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisHistos*)
   {
      ::PairAnalysisHistos *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisHistos >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisHistos", ::PairAnalysisHistos::Class_Version(), "PairAnalysisHistos.h", 82,
                  typeid(::PairAnalysisHistos), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisHistos::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisHistos) );
      instance.SetNew(&new_PairAnalysisHistos);
      instance.SetNewArray(&newArray_PairAnalysisHistos);
      instance.SetDelete(&delete_PairAnalysisHistos);
      instance.SetDeleteArray(&deleteArray_PairAnalysisHistos);
      instance.SetDestructor(&destruct_PairAnalysisHistos);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisHistos*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisHistos*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisHistos*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisHF(void *p = nullptr);
   static void *newArray_PairAnalysisHF(Long_t size, void *p);
   static void delete_PairAnalysisHF(void *p);
   static void deleteArray_PairAnalysisHF(void *p);
   static void destruct_PairAnalysisHF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisHF*)
   {
      ::PairAnalysisHF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisHF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisHF", ::PairAnalysisHF::Class_Version(), "PairAnalysisHF.h", 24,
                  typeid(::PairAnalysisHF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisHF::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisHF) );
      instance.SetNew(&new_PairAnalysisHF);
      instance.SetNewArray(&newArray_PairAnalysisHF);
      instance.SetDelete(&delete_PairAnalysisHF);
      instance.SetDeleteArray(&deleteArray_PairAnalysisHF);
      instance.SetDestructor(&destruct_PairAnalysisHF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisHF*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisHF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisHF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysis(void *p = nullptr);
   static void *newArray_PairAnalysis(Long_t size, void *p);
   static void delete_PairAnalysis(void *p);
   static void deleteArray_PairAnalysis(void *p);
   static void destruct_PairAnalysis(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysis*)
   {
      ::PairAnalysis *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysis >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysis", ::PairAnalysis::Class_Version(), "PairAnalysis.h", 26,
                  typeid(::PairAnalysis), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysis::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysis) );
      instance.SetNew(&new_PairAnalysis);
      instance.SetNewArray(&newArray_PairAnalysis);
      instance.SetDelete(&delete_PairAnalysis);
      instance.SetDeleteArray(&deleteArray_PairAnalysis);
      instance.SetDestructor(&destruct_PairAnalysis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysis*)
   {
      return GenerateInitInstanceLocal((::PairAnalysis*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysis*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisMixedEvent(void *p = nullptr);
   static void *newArray_PairAnalysisMixedEvent(Long_t size, void *p);
   static void delete_PairAnalysisMixedEvent(void *p);
   static void deleteArray_PairAnalysisMixedEvent(void *p);
   static void destruct_PairAnalysisMixedEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisMixedEvent*)
   {
      ::PairAnalysisMixedEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisMixedEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisMixedEvent", ::PairAnalysisMixedEvent::Class_Version(), "PairAnalysisMixedEvent.h", 24,
                  typeid(::PairAnalysisMixedEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisMixedEvent::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisMixedEvent) );
      instance.SetNew(&new_PairAnalysisMixedEvent);
      instance.SetNewArray(&newArray_PairAnalysisMixedEvent);
      instance.SetDelete(&delete_PairAnalysisMixedEvent);
      instance.SetDeleteArray(&deleteArray_PairAnalysisMixedEvent);
      instance.SetDestructor(&destruct_PairAnalysisMixedEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisMixedEvent*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisMixedEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisMixedEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisMixingHandler(void *p = nullptr);
   static void *newArray_PairAnalysisMixingHandler(Long_t size, void *p);
   static void delete_PairAnalysisMixingHandler(void *p);
   static void deleteArray_PairAnalysisMixingHandler(void *p);
   static void destruct_PairAnalysisMixingHandler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisMixingHandler*)
   {
      ::PairAnalysisMixingHandler *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisMixingHandler >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisMixingHandler", ::PairAnalysisMixingHandler::Class_Version(), "PairAnalysisMixingHandler.h", 26,
                  typeid(::PairAnalysisMixingHandler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisMixingHandler::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisMixingHandler) );
      instance.SetNew(&new_PairAnalysisMixingHandler);
      instance.SetNewArray(&newArray_PairAnalysisMixingHandler);
      instance.SetDelete(&delete_PairAnalysisMixingHandler);
      instance.SetDeleteArray(&deleteArray_PairAnalysisMixingHandler);
      instance.SetDestructor(&destruct_PairAnalysisMixingHandler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisMixingHandler*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisMixingHandler*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisMixingHandler*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisFunction(void *p = nullptr);
   static void *newArray_PairAnalysisFunction(Long_t size, void *p);
   static void delete_PairAnalysisFunction(void *p);
   static void deleteArray_PairAnalysisFunction(void *p);
   static void destruct_PairAnalysisFunction(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisFunction*)
   {
      ::PairAnalysisFunction *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisFunction >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisFunction", ::PairAnalysisFunction::Class_Version(), "PairAnalysisFunction.h", 26,
                  typeid(::PairAnalysisFunction), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisFunction::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisFunction) );
      instance.SetNew(&new_PairAnalysisFunction);
      instance.SetNewArray(&newArray_PairAnalysisFunction);
      instance.SetDelete(&delete_PairAnalysisFunction);
      instance.SetDeleteArray(&deleteArray_PairAnalysisFunction);
      instance.SetDestructor(&destruct_PairAnalysisFunction);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisFunction*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisFunction*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisFunction*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisSignalExt(void *p = nullptr);
   static void *newArray_PairAnalysisSignalExt(Long_t size, void *p);
   static void delete_PairAnalysisSignalExt(void *p);
   static void deleteArray_PairAnalysisSignalExt(void *p);
   static void destruct_PairAnalysisSignalExt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisSignalExt*)
   {
      ::PairAnalysisSignalExt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisSignalExt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisSignalExt", ::PairAnalysisSignalExt::Class_Version(), "PairAnalysisSignalExt.h", 29,
                  typeid(::PairAnalysisSignalExt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisSignalExt::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisSignalExt) );
      instance.SetNew(&new_PairAnalysisSignalExt);
      instance.SetNewArray(&newArray_PairAnalysisSignalExt);
      instance.SetDelete(&delete_PairAnalysisSignalExt);
      instance.SetDeleteArray(&deleteArray_PairAnalysisSignalExt);
      instance.SetDestructor(&destruct_PairAnalysisSignalExt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisSignalExt*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisSignalExt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisSignalExt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Extraction(void *p = nullptr);
   static void *newArray_Extraction(Long_t size, void *p);
   static void delete_Extraction(void *p);
   static void deleteArray_Extraction(void *p);
   static void destruct_Extraction(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Extraction*)
   {
      ::Extraction *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Extraction >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Extraction", ::Extraction::Class_Version(), "PairAnalysisSpectrum.h", 34,
                  typeid(::Extraction), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Extraction::Dictionary, isa_proxy, 4,
                  sizeof(::Extraction) );
      instance.SetNew(&new_Extraction);
      instance.SetNewArray(&newArray_Extraction);
      instance.SetDelete(&delete_Extraction);
      instance.SetDeleteArray(&deleteArray_Extraction);
      instance.SetDestructor(&destruct_Extraction);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Extraction*)
   {
      return GenerateInitInstanceLocal((::Extraction*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Extraction*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PairAnalysisSpectrum(void *p = nullptr);
   static void *newArray_PairAnalysisSpectrum(Long_t size, void *p);
   static void delete_PairAnalysisSpectrum(void *p);
   static void deleteArray_PairAnalysisSpectrum(void *p);
   static void destruct_PairAnalysisSpectrum(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PairAnalysisSpectrum*)
   {
      ::PairAnalysisSpectrum *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PairAnalysisSpectrum >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PairAnalysisSpectrum", ::PairAnalysisSpectrum::Class_Version(), "PairAnalysisSpectrum.h", 60,
                  typeid(::PairAnalysisSpectrum), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PairAnalysisSpectrum::Dictionary, isa_proxy, 4,
                  sizeof(::PairAnalysisSpectrum) );
      instance.SetNew(&new_PairAnalysisSpectrum);
      instance.SetNewArray(&newArray_PairAnalysisSpectrum);
      instance.SetDelete(&delete_PairAnalysisSpectrum);
      instance.SetDeleteArray(&deleteArray_PairAnalysisSpectrum);
      instance.SetDestructor(&destruct_PairAnalysisSpectrum);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PairAnalysisSpectrum*)
   {
      return GenerateInitInstanceLocal((::PairAnalysisSpectrum*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PairAnalysisSpectrum*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr AnalysisCuts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AnalysisCuts::Class_Name()
{
   return "AnalysisCuts";
}

//______________________________________________________________________________
const char *AnalysisCuts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisCuts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AnalysisCuts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisCuts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AnalysisCuts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisCuts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AnalysisCuts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisCuts*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr AnalysisFilter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AnalysisFilter::Class_Name()
{
   return "AnalysisFilter";
}

//______________________________________________________________________________
const char *AnalysisFilter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisFilter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AnalysisFilter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisFilter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AnalysisFilter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisFilter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AnalysisFilter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisFilter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisTrack::Class_Name()
{
   return "PairAnalysisTrack";
}

//______________________________________________________________________________
const char *PairAnalysisTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisEvent::Class_Name()
{
   return "PairAnalysisEvent";
}

//______________________________________________________________________________
const char *PairAnalysisEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisTrackRotator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisTrackRotator::Class_Name()
{
   return "PairAnalysisTrackRotator";
}

//______________________________________________________________________________
const char *PairAnalysisTrackRotator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisTrackRotator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisTrackRotator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisTrackRotator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisTrackRotator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisTrackRotator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisTrackRotator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisTrackRotator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisPair::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisPair::Class_Name()
{
   return "PairAnalysisPair";
}

//______________________________________________________________________________
const char *PairAnalysisPair::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPair*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisPair::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPair*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisPair::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPair*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisPair::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPair*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisPairKF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisPairKF::Class_Name()
{
   return "PairAnalysisPairKF";
}

//______________________________________________________________________________
const char *PairAnalysisPairKF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPairKF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisPairKF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPairKF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisPairKF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPairKF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisPairKF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPairKF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisPairLV::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisPairLV::Class_Name()
{
   return "PairAnalysisPairLV";
}

//______________________________________________________________________________
const char *PairAnalysisPairLV::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPairLV*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisPairLV::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPairLV*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisPairLV::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPairLV*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisPairLV::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPairLV*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisCutQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisCutQa::Class_Name()
{
   return "PairAnalysisCutQa";
}

//______________________________________________________________________________
const char *PairAnalysisCutQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisCutQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisCutQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisCutQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisCutQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisCutQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisCutQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisCutQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisSignalMC::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisSignalMC::Class_Name()
{
   return "PairAnalysisSignalMC";
}

//______________________________________________________________________________
const char *PairAnalysisSignalMC::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisSignalMC*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisSignalMC::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisSignalMC*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisSignalMC::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisSignalMC*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisSignalMC::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisSignalMC*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisMC::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisMC::Class_Name()
{
   return "PairAnalysisMC";
}

//______________________________________________________________________________
const char *PairAnalysisMC::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMC*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisMC::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMC*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisMC::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMC*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisMC::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMC*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisVarManager::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisVarManager::Class_Name()
{
   return "PairAnalysisVarManager";
}

//______________________________________________________________________________
const char *PairAnalysisVarManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisVarManager*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisVarManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisVarManager*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisVarManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisVarManager*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisVarManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisVarManager*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisVarCuts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisVarCuts::Class_Name()
{
   return "PairAnalysisVarCuts";
}

//______________________________________________________________________________
const char *PairAnalysisVarCuts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisVarCuts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisVarCuts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisVarCuts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisVarCuts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisVarCuts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisVarCuts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisVarCuts*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisObjectCuts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisObjectCuts::Class_Name()
{
   return "PairAnalysisObjectCuts";
}

//______________________________________________________________________________
const char *PairAnalysisObjectCuts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisObjectCuts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisObjectCuts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisObjectCuts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisObjectCuts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisObjectCuts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisObjectCuts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisObjectCuts*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisPairLegCuts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisPairLegCuts::Class_Name()
{
   return "PairAnalysisPairLegCuts";
}

//______________________________________________________________________________
const char *PairAnalysisPairLegCuts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPairLegCuts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisPairLegCuts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPairLegCuts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisPairLegCuts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPairLegCuts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisPairLegCuts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisPairLegCuts*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisCutCombi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisCutCombi::Class_Name()
{
   return "PairAnalysisCutCombi";
}

//______________________________________________________________________________
const char *PairAnalysisCutCombi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisCutCombi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisCutCombi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisCutCombi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisCutCombi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisCutCombi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisCutCombi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisCutCombi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisCutGroup::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisCutGroup::Class_Name()
{
   return "PairAnalysisCutGroup";
}

//______________________________________________________________________________
const char *PairAnalysisCutGroup::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisCutGroup*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisCutGroup::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisCutGroup*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisCutGroup::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisCutGroup*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisCutGroup::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisCutGroup*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisMetaData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisMetaData::Class_Name()
{
   return "PairAnalysisMetaData";
}

//______________________________________________________________________________
const char *PairAnalysisMetaData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMetaData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisMetaData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMetaData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisMetaData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMetaData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisMetaData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMetaData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr AnalysisTaskMultiPairAnalysis::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AnalysisTaskMultiPairAnalysis::Class_Name()
{
   return "AnalysisTaskMultiPairAnalysis";
}

//______________________________________________________________________________
const char *AnalysisTaskMultiPairAnalysis::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTaskMultiPairAnalysis*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AnalysisTaskMultiPairAnalysis::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTaskMultiPairAnalysis*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AnalysisTaskMultiPairAnalysis::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTaskMultiPairAnalysis*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AnalysisTaskMultiPairAnalysis::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTaskMultiPairAnalysis*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisHn::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisHn::Class_Name()
{
   return "PairAnalysisHn";
}

//______________________________________________________________________________
const char *PairAnalysisHn::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisHn*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisHn::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisHn*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisHn::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisHn*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisHn::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisHn*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisHistos::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisHistos::Class_Name()
{
   return "PairAnalysisHistos";
}

//______________________________________________________________________________
const char *PairAnalysisHistos::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisHistos*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisHistos::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisHistos*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisHistos::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisHistos*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisHistos::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisHistos*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisHF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisHF::Class_Name()
{
   return "PairAnalysisHF";
}

//______________________________________________________________________________
const char *PairAnalysisHF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisHF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisHF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisHF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisHF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisHF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisHF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisHF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysis::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysis::Class_Name()
{
   return "PairAnalysis";
}

//______________________________________________________________________________
const char *PairAnalysis::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysis*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysis::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysis*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysis::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysis*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysis::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysis*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisMixedEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisMixedEvent::Class_Name()
{
   return "PairAnalysisMixedEvent";
}

//______________________________________________________________________________
const char *PairAnalysisMixedEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMixedEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisMixedEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMixedEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisMixedEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMixedEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisMixedEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMixedEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisMixingHandler::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisMixingHandler::Class_Name()
{
   return "PairAnalysisMixingHandler";
}

//______________________________________________________________________________
const char *PairAnalysisMixingHandler::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMixingHandler*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisMixingHandler::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMixingHandler*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisMixingHandler::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMixingHandler*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisMixingHandler::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisMixingHandler*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisFunction::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisFunction::Class_Name()
{
   return "PairAnalysisFunction";
}

//______________________________________________________________________________
const char *PairAnalysisFunction::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisFunction*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisFunction::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisFunction*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisFunction::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisFunction*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisFunction::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisFunction*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisSignalExt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisSignalExt::Class_Name()
{
   return "PairAnalysisSignalExt";
}

//______________________________________________________________________________
const char *PairAnalysisSignalExt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisSignalExt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisSignalExt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisSignalExt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisSignalExt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisSignalExt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisSignalExt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisSignalExt*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Extraction::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Extraction::Class_Name()
{
   return "Extraction";
}

//______________________________________________________________________________
const char *Extraction::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Extraction*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Extraction::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Extraction*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Extraction::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Extraction*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Extraction::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Extraction*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PairAnalysisSpectrum::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PairAnalysisSpectrum::Class_Name()
{
   return "PairAnalysisSpectrum";
}

//______________________________________________________________________________
const char *PairAnalysisSpectrum::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisSpectrum*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PairAnalysisSpectrum::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisSpectrum*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PairAnalysisSpectrum::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisSpectrum*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PairAnalysisSpectrum::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PairAnalysisSpectrum*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void AnalysisCuts::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisCuts.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisCuts::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisCuts::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_AnalysisCuts(void *p) {
      delete ((::AnalysisCuts*)p);
   }
   static void deleteArray_AnalysisCuts(void *p) {
      delete [] ((::AnalysisCuts*)p);
   }
   static void destruct_AnalysisCuts(void *p) {
      typedef ::AnalysisCuts current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_AnalysisCuts(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::AnalysisCuts*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::AnalysisCuts

//______________________________________________________________________________
void AnalysisFilter::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisFilter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisFilter::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisFilter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisFilter(void *p) {
      return  p ? new(p) ::AnalysisFilter : new ::AnalysisFilter;
   }
   static void *newArray_AnalysisFilter(Long_t nElements, void *p) {
      return p ? new(p) ::AnalysisFilter[nElements] : new ::AnalysisFilter[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisFilter(void *p) {
      delete ((::AnalysisFilter*)p);
   }
   static void deleteArray_AnalysisFilter(void *p) {
      delete [] ((::AnalysisFilter*)p);
   }
   static void destruct_AnalysisFilter(void *p) {
      typedef ::AnalysisFilter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisFilter

//______________________________________________________________________________
void PairAnalysisTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisTrack(void *p) {
      return  p ? new(p) ::PairAnalysisTrack : new ::PairAnalysisTrack;
   }
   static void *newArray_PairAnalysisTrack(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisTrack[nElements] : new ::PairAnalysisTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisTrack(void *p) {
      delete ((::PairAnalysisTrack*)p);
   }
   static void deleteArray_PairAnalysisTrack(void *p) {
      delete [] ((::PairAnalysisTrack*)p);
   }
   static void destruct_PairAnalysisTrack(void *p) {
      typedef ::PairAnalysisTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisTrack

//______________________________________________________________________________
void PairAnalysisEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisEvent(void *p) {
      return  p ? new(p) ::PairAnalysisEvent : new ::PairAnalysisEvent;
   }
   static void *newArray_PairAnalysisEvent(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisEvent[nElements] : new ::PairAnalysisEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisEvent(void *p) {
      delete ((::PairAnalysisEvent*)p);
   }
   static void deleteArray_PairAnalysisEvent(void *p) {
      delete [] ((::PairAnalysisEvent*)p);
   }
   static void destruct_PairAnalysisEvent(void *p) {
      typedef ::PairAnalysisEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisEvent

//______________________________________________________________________________
void PairAnalysisTrackRotator::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisTrackRotator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisTrackRotator::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisTrackRotator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisTrackRotator(void *p) {
      return  p ? new(p) ::PairAnalysisTrackRotator : new ::PairAnalysisTrackRotator;
   }
   static void *newArray_PairAnalysisTrackRotator(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisTrackRotator[nElements] : new ::PairAnalysisTrackRotator[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisTrackRotator(void *p) {
      delete ((::PairAnalysisTrackRotator*)p);
   }
   static void deleteArray_PairAnalysisTrackRotator(void *p) {
      delete [] ((::PairAnalysisTrackRotator*)p);
   }
   static void destruct_PairAnalysisTrackRotator(void *p) {
      typedef ::PairAnalysisTrackRotator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisTrackRotator

//______________________________________________________________________________
void PairAnalysisPair::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisPair.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisPair::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisPair::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_PairAnalysisPair(void *p) {
      delete ((::PairAnalysisPair*)p);
   }
   static void deleteArray_PairAnalysisPair(void *p) {
      delete [] ((::PairAnalysisPair*)p);
   }
   static void destruct_PairAnalysisPair(void *p) {
      typedef ::PairAnalysisPair current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisPair

//______________________________________________________________________________
void PairAnalysisPairKF::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisPairKF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisPairKF::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisPairKF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisPairKF(void *p) {
      return  p ? new(p) ::PairAnalysisPairKF : new ::PairAnalysisPairKF;
   }
   static void *newArray_PairAnalysisPairKF(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisPairKF[nElements] : new ::PairAnalysisPairKF[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisPairKF(void *p) {
      delete ((::PairAnalysisPairKF*)p);
   }
   static void deleteArray_PairAnalysisPairKF(void *p) {
      delete [] ((::PairAnalysisPairKF*)p);
   }
   static void destruct_PairAnalysisPairKF(void *p) {
      typedef ::PairAnalysisPairKF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisPairKF

//______________________________________________________________________________
void PairAnalysisPairLV::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisPairLV.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisPairLV::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisPairLV::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisPairLV(void *p) {
      return  p ? new(p) ::PairAnalysisPairLV : new ::PairAnalysisPairLV;
   }
   static void *newArray_PairAnalysisPairLV(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisPairLV[nElements] : new ::PairAnalysisPairLV[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisPairLV(void *p) {
      delete ((::PairAnalysisPairLV*)p);
   }
   static void deleteArray_PairAnalysisPairLV(void *p) {
      delete [] ((::PairAnalysisPairLV*)p);
   }
   static void destruct_PairAnalysisPairLV(void *p) {
      typedef ::PairAnalysisPairLV current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisPairLV

//______________________________________________________________________________
void PairAnalysisCutQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisCutQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisCutQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisCutQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisCutQa(void *p) {
      return  p ? new(p) ::PairAnalysisCutQa : new ::PairAnalysisCutQa;
   }
   static void *newArray_PairAnalysisCutQa(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisCutQa[nElements] : new ::PairAnalysisCutQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisCutQa(void *p) {
      delete ((::PairAnalysisCutQa*)p);
   }
   static void deleteArray_PairAnalysisCutQa(void *p) {
      delete [] ((::PairAnalysisCutQa*)p);
   }
   static void destruct_PairAnalysisCutQa(void *p) {
      typedef ::PairAnalysisCutQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisCutQa

//______________________________________________________________________________
void PairAnalysisSignalMC::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisSignalMC.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisSignalMC::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisSignalMC::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisSignalMC(void *p) {
      return  p ? new(p) ::PairAnalysisSignalMC : new ::PairAnalysisSignalMC;
   }
   static void *newArray_PairAnalysisSignalMC(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisSignalMC[nElements] : new ::PairAnalysisSignalMC[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisSignalMC(void *p) {
      delete ((::PairAnalysisSignalMC*)p);
   }
   static void deleteArray_PairAnalysisSignalMC(void *p) {
      delete [] ((::PairAnalysisSignalMC*)p);
   }
   static void destruct_PairAnalysisSignalMC(void *p) {
      typedef ::PairAnalysisSignalMC current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisSignalMC

//______________________________________________________________________________
void PairAnalysisMC::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisMC.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisMC::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisMC::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisMC(void *p) {
      return  p ? new(p) ::PairAnalysisMC : new ::PairAnalysisMC;
   }
   static void *newArray_PairAnalysisMC(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisMC[nElements] : new ::PairAnalysisMC[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisMC(void *p) {
      delete ((::PairAnalysisMC*)p);
   }
   static void deleteArray_PairAnalysisMC(void *p) {
      delete [] ((::PairAnalysisMC*)p);
   }
   static void destruct_PairAnalysisMC(void *p) {
      typedef ::PairAnalysisMC current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisMC

//______________________________________________________________________________
void PairAnalysisVarManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisVarManager.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisVarManager::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisVarManager::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisVarManager(void *p) {
      return  p ? new(p) ::PairAnalysisVarManager : new ::PairAnalysisVarManager;
   }
   static void *newArray_PairAnalysisVarManager(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisVarManager[nElements] : new ::PairAnalysisVarManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisVarManager(void *p) {
      delete ((::PairAnalysisVarManager*)p);
   }
   static void deleteArray_PairAnalysisVarManager(void *p) {
      delete [] ((::PairAnalysisVarManager*)p);
   }
   static void destruct_PairAnalysisVarManager(void *p) {
      typedef ::PairAnalysisVarManager current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisVarManager

//______________________________________________________________________________
void PairAnalysisVarCuts::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisVarCuts.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisVarCuts::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisVarCuts::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisVarCuts(void *p) {
      return  p ? new(p) ::PairAnalysisVarCuts : new ::PairAnalysisVarCuts;
   }
   static void *newArray_PairAnalysisVarCuts(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisVarCuts[nElements] : new ::PairAnalysisVarCuts[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisVarCuts(void *p) {
      delete ((::PairAnalysisVarCuts*)p);
   }
   static void deleteArray_PairAnalysisVarCuts(void *p) {
      delete [] ((::PairAnalysisVarCuts*)p);
   }
   static void destruct_PairAnalysisVarCuts(void *p) {
      typedef ::PairAnalysisVarCuts current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_PairAnalysisVarCuts(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::PairAnalysisVarCuts*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::PairAnalysisVarCuts

//______________________________________________________________________________
void PairAnalysisObjectCuts::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisObjectCuts.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisObjectCuts::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisObjectCuts::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisObjectCuts(void *p) {
      return  p ? new(p) ::PairAnalysisObjectCuts : new ::PairAnalysisObjectCuts;
   }
   static void *newArray_PairAnalysisObjectCuts(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisObjectCuts[nElements] : new ::PairAnalysisObjectCuts[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisObjectCuts(void *p) {
      delete ((::PairAnalysisObjectCuts*)p);
   }
   static void deleteArray_PairAnalysisObjectCuts(void *p) {
      delete [] ((::PairAnalysisObjectCuts*)p);
   }
   static void destruct_PairAnalysisObjectCuts(void *p) {
      typedef ::PairAnalysisObjectCuts current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_PairAnalysisObjectCuts(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::PairAnalysisObjectCuts*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::PairAnalysisObjectCuts

//______________________________________________________________________________
void PairAnalysisPairLegCuts::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisPairLegCuts.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisPairLegCuts::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisPairLegCuts::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisPairLegCuts(void *p) {
      return  p ? new(p) ::PairAnalysisPairLegCuts : new ::PairAnalysisPairLegCuts;
   }
   static void *newArray_PairAnalysisPairLegCuts(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisPairLegCuts[nElements] : new ::PairAnalysisPairLegCuts[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisPairLegCuts(void *p) {
      delete ((::PairAnalysisPairLegCuts*)p);
   }
   static void deleteArray_PairAnalysisPairLegCuts(void *p) {
      delete [] ((::PairAnalysisPairLegCuts*)p);
   }
   static void destruct_PairAnalysisPairLegCuts(void *p) {
      typedef ::PairAnalysisPairLegCuts current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_PairAnalysisPairLegCuts(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::PairAnalysisPairLegCuts*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::PairAnalysisPairLegCuts

//______________________________________________________________________________
void PairAnalysisCutCombi::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisCutCombi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisCutCombi::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisCutCombi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisCutCombi(void *p) {
      return  p ? new(p) ::PairAnalysisCutCombi : new ::PairAnalysisCutCombi;
   }
   static void *newArray_PairAnalysisCutCombi(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisCutCombi[nElements] : new ::PairAnalysisCutCombi[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisCutCombi(void *p) {
      delete ((::PairAnalysisCutCombi*)p);
   }
   static void deleteArray_PairAnalysisCutCombi(void *p) {
      delete [] ((::PairAnalysisCutCombi*)p);
   }
   static void destruct_PairAnalysisCutCombi(void *p) {
      typedef ::PairAnalysisCutCombi current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_PairAnalysisCutCombi(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::PairAnalysisCutCombi*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::PairAnalysisCutCombi

//______________________________________________________________________________
void PairAnalysisCutGroup::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisCutGroup.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisCutGroup::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisCutGroup::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisCutGroup(void *p) {
      return  p ? new(p) ::PairAnalysisCutGroup : new ::PairAnalysisCutGroup;
   }
   static void *newArray_PairAnalysisCutGroup(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisCutGroup[nElements] : new ::PairAnalysisCutGroup[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisCutGroup(void *p) {
      delete ((::PairAnalysisCutGroup*)p);
   }
   static void deleteArray_PairAnalysisCutGroup(void *p) {
      delete [] ((::PairAnalysisCutGroup*)p);
   }
   static void destruct_PairAnalysisCutGroup(void *p) {
      typedef ::PairAnalysisCutGroup current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_PairAnalysisCutGroup(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::PairAnalysisCutGroup*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::PairAnalysisCutGroup

//______________________________________________________________________________
void PairAnalysisMetaData::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisMetaData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisMetaData::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisMetaData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisMetaData(void *p) {
      return  p ? new(p) ::PairAnalysisMetaData : new ::PairAnalysisMetaData;
   }
   static void *newArray_PairAnalysisMetaData(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisMetaData[nElements] : new ::PairAnalysisMetaData[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisMetaData(void *p) {
      delete ((::PairAnalysisMetaData*)p);
   }
   static void deleteArray_PairAnalysisMetaData(void *p) {
      delete [] ((::PairAnalysisMetaData*)p);
   }
   static void destruct_PairAnalysisMetaData(void *p) {
      typedef ::PairAnalysisMetaData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisMetaData

//______________________________________________________________________________
void AnalysisTaskMultiPairAnalysis::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTaskMultiPairAnalysis.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTaskMultiPairAnalysis::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTaskMultiPairAnalysis::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTaskMultiPairAnalysis(void *p) {
      return  p ? new(p) ::AnalysisTaskMultiPairAnalysis : new ::AnalysisTaskMultiPairAnalysis;
   }
   static void *newArray_AnalysisTaskMultiPairAnalysis(Long_t nElements, void *p) {
      return p ? new(p) ::AnalysisTaskMultiPairAnalysis[nElements] : new ::AnalysisTaskMultiPairAnalysis[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTaskMultiPairAnalysis(void *p) {
      delete ((::AnalysisTaskMultiPairAnalysis*)p);
   }
   static void deleteArray_AnalysisTaskMultiPairAnalysis(void *p) {
      delete [] ((::AnalysisTaskMultiPairAnalysis*)p);
   }
   static void destruct_AnalysisTaskMultiPairAnalysis(void *p) {
      typedef ::AnalysisTaskMultiPairAnalysis current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTaskMultiPairAnalysis

//______________________________________________________________________________
void PairAnalysisHn::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisHn.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisHn::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisHn::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisHn(void *p) {
      return  p ? new(p) ::PairAnalysisHn : new ::PairAnalysisHn;
   }
   static void *newArray_PairAnalysisHn(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisHn[nElements] : new ::PairAnalysisHn[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisHn(void *p) {
      delete ((::PairAnalysisHn*)p);
   }
   static void deleteArray_PairAnalysisHn(void *p) {
      delete [] ((::PairAnalysisHn*)p);
   }
   static void destruct_PairAnalysisHn(void *p) {
      typedef ::PairAnalysisHn current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_PairAnalysisHn(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::PairAnalysisHn*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::PairAnalysisHn

//______________________________________________________________________________
void PairAnalysisHistos::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisHistos.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisHistos::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisHistos::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisHistos(void *p) {
      return  p ? new(p) ::PairAnalysisHistos : new ::PairAnalysisHistos;
   }
   static void *newArray_PairAnalysisHistos(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisHistos[nElements] : new ::PairAnalysisHistos[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisHistos(void *p) {
      delete ((::PairAnalysisHistos*)p);
   }
   static void deleteArray_PairAnalysisHistos(void *p) {
      delete [] ((::PairAnalysisHistos*)p);
   }
   static void destruct_PairAnalysisHistos(void *p) {
      typedef ::PairAnalysisHistos current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisHistos

//______________________________________________________________________________
void PairAnalysisHF::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisHF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisHF::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisHF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisHF(void *p) {
      return  p ? new(p) ::PairAnalysisHF : new ::PairAnalysisHF;
   }
   static void *newArray_PairAnalysisHF(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisHF[nElements] : new ::PairAnalysisHF[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisHF(void *p) {
      delete ((::PairAnalysisHF*)p);
   }
   static void deleteArray_PairAnalysisHF(void *p) {
      delete [] ((::PairAnalysisHF*)p);
   }
   static void destruct_PairAnalysisHF(void *p) {
      typedef ::PairAnalysisHF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisHF

//______________________________________________________________________________
void PairAnalysis::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysis.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysis::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysis::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysis(void *p) {
      return  p ? new(p) ::PairAnalysis : new ::PairAnalysis;
   }
   static void *newArray_PairAnalysis(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysis[nElements] : new ::PairAnalysis[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysis(void *p) {
      delete ((::PairAnalysis*)p);
   }
   static void deleteArray_PairAnalysis(void *p) {
      delete [] ((::PairAnalysis*)p);
   }
   static void destruct_PairAnalysis(void *p) {
      typedef ::PairAnalysis current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysis

//______________________________________________________________________________
void PairAnalysisMixedEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisMixedEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisMixedEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisMixedEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisMixedEvent(void *p) {
      return  p ? new(p) ::PairAnalysisMixedEvent : new ::PairAnalysisMixedEvent;
   }
   static void *newArray_PairAnalysisMixedEvent(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisMixedEvent[nElements] : new ::PairAnalysisMixedEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisMixedEvent(void *p) {
      delete ((::PairAnalysisMixedEvent*)p);
   }
   static void deleteArray_PairAnalysisMixedEvent(void *p) {
      delete [] ((::PairAnalysisMixedEvent*)p);
   }
   static void destruct_PairAnalysisMixedEvent(void *p) {
      typedef ::PairAnalysisMixedEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisMixedEvent

//______________________________________________________________________________
void PairAnalysisMixingHandler::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisMixingHandler.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisMixingHandler::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisMixingHandler::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisMixingHandler(void *p) {
      return  p ? new(p) ::PairAnalysisMixingHandler : new ::PairAnalysisMixingHandler;
   }
   static void *newArray_PairAnalysisMixingHandler(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisMixingHandler[nElements] : new ::PairAnalysisMixingHandler[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisMixingHandler(void *p) {
      delete ((::PairAnalysisMixingHandler*)p);
   }
   static void deleteArray_PairAnalysisMixingHandler(void *p) {
      delete [] ((::PairAnalysisMixingHandler*)p);
   }
   static void destruct_PairAnalysisMixingHandler(void *p) {
      typedef ::PairAnalysisMixingHandler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisMixingHandler

//______________________________________________________________________________
void PairAnalysisFunction::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisFunction.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisFunction::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisFunction::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisFunction(void *p) {
      return  p ? new(p) ::PairAnalysisFunction : new ::PairAnalysisFunction;
   }
   static void *newArray_PairAnalysisFunction(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisFunction[nElements] : new ::PairAnalysisFunction[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisFunction(void *p) {
      delete ((::PairAnalysisFunction*)p);
   }
   static void deleteArray_PairAnalysisFunction(void *p) {
      delete [] ((::PairAnalysisFunction*)p);
   }
   static void destruct_PairAnalysisFunction(void *p) {
      typedef ::PairAnalysisFunction current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisFunction

//______________________________________________________________________________
void PairAnalysisSignalExt::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisSignalExt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisSignalExt::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisSignalExt::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisSignalExt(void *p) {
      return  p ? new(p) ::PairAnalysisSignalExt : new ::PairAnalysisSignalExt;
   }
   static void *newArray_PairAnalysisSignalExt(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisSignalExt[nElements] : new ::PairAnalysisSignalExt[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisSignalExt(void *p) {
      delete ((::PairAnalysisSignalExt*)p);
   }
   static void deleteArray_PairAnalysisSignalExt(void *p) {
      delete [] ((::PairAnalysisSignalExt*)p);
   }
   static void destruct_PairAnalysisSignalExt(void *p) {
      typedef ::PairAnalysisSignalExt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisSignalExt

//______________________________________________________________________________
void Extraction::Streamer(TBuffer &R__b)
{
   // Stream an object of class Extraction.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Extraction::Class(),this);
   } else {
      R__b.WriteClassBuffer(Extraction::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Extraction(void *p) {
      return  p ? new(p) ::Extraction : new ::Extraction;
   }
   static void *newArray_Extraction(Long_t nElements, void *p) {
      return p ? new(p) ::Extraction[nElements] : new ::Extraction[nElements];
   }
   // Wrapper around operator delete
   static void delete_Extraction(void *p) {
      delete ((::Extraction*)p);
   }
   static void deleteArray_Extraction(void *p) {
      delete [] ((::Extraction*)p);
   }
   static void destruct_Extraction(void *p) {
      typedef ::Extraction current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Extraction

//______________________________________________________________________________
void PairAnalysisSpectrum::Streamer(TBuffer &R__b)
{
   // Stream an object of class PairAnalysisSpectrum.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PairAnalysisSpectrum::Class(),this);
   } else {
      R__b.WriteClassBuffer(PairAnalysisSpectrum::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PairAnalysisSpectrum(void *p) {
      return  p ? new(p) ::PairAnalysisSpectrum : new ::PairAnalysisSpectrum;
   }
   static void *newArray_PairAnalysisSpectrum(Long_t nElements, void *p) {
      return p ? new(p) ::PairAnalysisSpectrum[nElements] : new ::PairAnalysisSpectrum[nElements];
   }
   // Wrapper around operator delete
   static void delete_PairAnalysisSpectrum(void *p) {
      delete ((::PairAnalysisSpectrum*)p);
   }
   static void deleteArray_PairAnalysisSpectrum(void *p) {
      delete [] ((::PairAnalysisSpectrum*)p);
   }
   static void destruct_PairAnalysisSpectrum(void *p) {
      typedef ::PairAnalysisSpectrum current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PairAnalysisSpectrum

namespace {
  void TriggerDictionaryInitialization_libCbmPapa_Impl() {
    static const char* headers[] = {
"AnalysisCuts.h",
"AnalysisFilter.h",
"PairAnalysisTrack.h",
"PairAnalysisEvent.h",
"PairAnalysisPair.h",
"PairAnalysisPairKF.h",
"PairAnalysisPairLV.h",
"PairAnalysisHelper.h",
"PairAnalysisStyler.h",
"PairAnalysisCutQa.h",
"PairAnalysisVarCuts.h",
"PairAnalysisObjectCuts.h",
"PairAnalysisPairLegCuts.h",
"PairAnalysisCutCombi.h",
"PairAnalysisCutGroup.h",
"PairAnalysisVarManager.h",
"PairAnalysisMetaData.h",
"AnalysisTaskMultiPairAnalysis.h",
"PairAnalysis.h",
"PairAnalysisHF.h",
"PairAnalysisHistos.h",
"PairAnalysisMC.h",
"PairAnalysisSignalMC.h",
"PairAnalysisTrackRotator.h",
"PairAnalysisMixedEvent.h",
"PairAnalysisMixingHandler.h",
"PairAnalysisFunction.h",
"PairAnalysisSignalExt.h",
"PairAnalysisSpectrum.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich/utils",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
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
"/home/heinemann/gpuunpacker/cbmroot/core/qa/checker",
"/home/heinemann/gpuunpacker/cbmroot/core/qa",
"/home/heinemann/gpuunpacker/cbmroot/external/yaml-cpp/include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/tools",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/SensorDataSheets",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/tasks",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/buffers",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/KFQA",
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
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators/unigen",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators/pluto",
"/home/heinemann/gpuunpacker/cbmroot/build2/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/PWGDIL/dielectron/papaframework/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmPapa dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate(R"ATTRDUMP(Base class for filter decisions)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Base class for filter decisions)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Base class for filter decisions)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Base class for filter decisions)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$AnalysisCuts.h")))  AnalysisCuts;
class __attribute__((annotate(R"ATTRDUMP(Manager class for filter decisions)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Manager class for filter decisions)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Manager class for filter decisions)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Manager class for filter decisions)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$AnalysisFilter.h")))  AnalysisFilter;
class __attribute__((annotate(R"ATTRDUMP(Track structure)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisTrack.h")))  PairAnalysisTrack;
class __attribute__((annotate(R"ATTRDUMP(Event structure)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisEvent.h")))  PairAnalysisEvent;
class __attribute__((annotate(R"ATTRDUMP(Track rotation configuration)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisTrackRotator.h")))  __attribute__((annotate("$clingAutoload$PairAnalysisPair.h")))  PairAnalysisTrackRotator;
class __attribute__((annotate(R"ATTRDUMP(Base class for pairs)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisPair.h")))  PairAnalysisPair;
class __attribute__((annotate("$clingAutoload$PairAnalysisPairKF.h")))  PairAnalysisPairKF;
class __attribute__((annotate(R"ATTRDUMP(Lorentz vector pairs)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisPairLV.h")))  PairAnalysisPairLV;
class __attribute__((annotate(R"ATTRDUMP(Simple automatic cut QA)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisCutQa.h")))  PairAnalysisCutQa;
class __attribute__((annotate(R"ATTRDUMP(MC signal defintions)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(MC signal defintions)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(MC signal defintions)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(MC signal defintions)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisSignalMC.h")))  __attribute__((annotate("$clingAutoload$PairAnalysisVarCuts.h")))  PairAnalysisSignalMC;
class __attribute__((annotate(R"ATTRDUMP(MC signal finder)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisMC.h")))  __attribute__((annotate("$clingAutoload$PairAnalysisVarCuts.h")))  PairAnalysisMC;
class __attribute__((annotate(R"ATTRDUMP(Variables management for event, pair, track, hit infos (static))ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Variables management for event, pair, track, hit infos (static))ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Variables management for event, pair, track, hit infos (static))ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Variables management for event, pair, track, hit infos (static))ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisVarManager.h")))  __attribute__((annotate("$clingAutoload$PairAnalysisVarCuts.h")))  PairAnalysisVarManager;
class __attribute__((annotate(R"ATTRDUMP(Cut class providing cuts based on all kind of variables)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisVarCuts.h")))  PairAnalysisVarCuts;
class __attribute__((annotate(R"ATTRDUMP(Cut class for special cuts (formulas, graphs, histograms))ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisObjectCuts.h")))  PairAnalysisObjectCuts;
class __attribute__((annotate(R"ATTRDUMP(Cut class providing cuts for both legs of a pair)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisPairLegCuts.h")))  PairAnalysisPairLegCuts;
class __attribute__((annotate(R"ATTRDUMP(Apply cuts for certain conditions/ranges)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisCutCombi.h")))  PairAnalysisCutCombi;
class __attribute__((annotate(R"ATTRDUMP(Collect cuts in groups with logical comparators)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisCutGroup.h")))  PairAnalysisCutGroup;
class __attribute__((annotate(R"ATTRDUMP(Meta Data Manager)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisMetaData.h")))  PairAnalysisMetaData;
class __attribute__((annotate(R"ATTRDUMP(Analysis Task handling multiple instances of PairAnalysis)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Analysis Task handling multiple instances of PairAnalysis)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Analysis Task handling multiple instances of PairAnalysis)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Analysis Task handling multiple instances of PairAnalysis)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$AnalysisTaskMultiPairAnalysis.h")))  AnalysisTaskMultiPairAnalysis;
class __attribute__((annotate(R"ATTRDUMP((temporary) class to hold functions in THnSparse)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisHistos.h")))  __attribute__((annotate("$clingAutoload$PairAnalysis.h")))  PairAnalysisHn;
class __attribute__((annotate(R"ATTRDUMP(Histogram management)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisHistos.h")))  __attribute__((annotate("$clingAutoload$PairAnalysis.h")))  PairAnalysisHistos;
class __attribute__((annotate(R"ATTRDUMP(Histogram matrix)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisHF.h")))  __attribute__((annotate("$clingAutoload$PairAnalysis.h")))  PairAnalysisHF;
class __attribute__((annotate(R"ATTRDUMP(Steering class to process the data)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Steering class to process the data)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Steering class to process the data)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Steering class to process the data)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysis.h")))  PairAnalysis;
class __attribute__((annotate(R"ATTRDUMP(Small mixed event structure)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisMixedEvent.h")))  PairAnalysisMixedEvent;
class __attribute__((annotate(R"ATTRDUMP(Mixed event handler)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisMixingHandler.h")))  PairAnalysisMixingHandler;
class __attribute__((annotate(R"ATTRDUMP(Combine functions for e.g. signal extractions)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisFunction.h")))  PairAnalysisFunction;
class __attribute__((annotate(R"ATTRDUMP(Class for signal extraction)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisSignalExt.h")))  PairAnalysisSignalExt;
class __attribute__((annotate(R"ATTRDUMP(mini object that holds members of the PairAnalysisSpectrum TTree)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisSpectrum.h")))  Extraction;
class __attribute__((annotate(R"ATTRDUMP(Build spectra from many signal extractions)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PairAnalysisSpectrum.h")))  PairAnalysisSpectrum;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmPapa dictionary payload"

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
#include "AnalysisCuts.h"
#include "AnalysisFilter.h"
#include "PairAnalysisTrack.h"
#include "PairAnalysisEvent.h"
#include "PairAnalysisPair.h"
#include "PairAnalysisPairKF.h"
#include "PairAnalysisPairLV.h"
#include "PairAnalysisHelper.h"
#include "PairAnalysisStyler.h"
#include "PairAnalysisCutQa.h"
#include "PairAnalysisVarCuts.h"
#include "PairAnalysisObjectCuts.h"
#include "PairAnalysisPairLegCuts.h"
#include "PairAnalysisCutCombi.h"
#include "PairAnalysisCutGroup.h"
#include "PairAnalysisVarManager.h"
#include "PairAnalysisMetaData.h"
#include "AnalysisTaskMultiPairAnalysis.h"
#include "PairAnalysis.h"
#include "PairAnalysisHF.h"
#include "PairAnalysisHistos.h"
#include "PairAnalysisMC.h"
#include "PairAnalysisSignalMC.h"
#include "PairAnalysisTrackRotator.h"
#include "PairAnalysisMixedEvent.h"
#include "PairAnalysisMixingHandler.h"
#include "PairAnalysisFunction.h"
#include "PairAnalysisSignalExt.h"
#include "PairAnalysisSpectrum.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"AnalysisCuts", payloadCode, "@",
"AnalysisFilter", payloadCode, "@",
"AnalysisTaskMultiPairAnalysis", payloadCode, "@",
"Extraction", payloadCode, "@",
"PairAnalysis", payloadCode, "@",
"PairAnalysisCutCombi", payloadCode, "@",
"PairAnalysisCutGroup", payloadCode, "@",
"PairAnalysisCutQa", payloadCode, "@",
"PairAnalysisEvent", payloadCode, "@",
"PairAnalysisFunction", payloadCode, "@",
"PairAnalysisHF", payloadCode, "@",
"PairAnalysisHistos", payloadCode, "@",
"PairAnalysisHn", payloadCode, "@",
"PairAnalysisMC", payloadCode, "@",
"PairAnalysisMetaData", payloadCode, "@",
"PairAnalysisMixedEvent", payloadCode, "@",
"PairAnalysisMixingHandler", payloadCode, "@",
"PairAnalysisObjectCuts", payloadCode, "@",
"PairAnalysisPair", payloadCode, "@",
"PairAnalysisPairKF", payloadCode, "@",
"PairAnalysisPairLV", payloadCode, "@",
"PairAnalysisPairLegCuts", payloadCode, "@",
"PairAnalysisSignalExt", payloadCode, "@",
"PairAnalysisSignalMC", payloadCode, "@",
"PairAnalysisSpectrum", payloadCode, "@",
"PairAnalysisTrack", payloadCode, "@",
"PairAnalysisTrackRotator", payloadCode, "@",
"PairAnalysisVarCuts", payloadCode, "@",
"PairAnalysisVarManager", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmPapa",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmPapa_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmPapa_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmPapa() {
  TriggerDictionaryInitialization_libCbmPapa_Impl();
}
