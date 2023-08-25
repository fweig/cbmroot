// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmTofReco
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
#include "CbmTofBuildDigiEvents.h"
#include "CbmTofHitMaker.h"
#include "CbmTofCosmicClusterizer.h"
#include "CbmTofEventClusterizer.h"
#include "CbmTofFindTracks.h"
#include "CbmTofExtendTracks.h"
#include "CbmTofSimpClusterizer.h"
#include "CbmTofTrackFinderNN.h"
#include "CbmTofTestBeamClusterizer.h"
#include "CbmTofTrackletTools.h"
#include "CbmTofCalibrator.h"
#include "LKFMinuit.h"
#include "CbmTofUnpackAlgo.h"
#include "CbmTofUnpackConfig.h"
#include "CbmTofUnpackMonitor.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmTofBuildDigiEvents(void *p = nullptr);
   static void *newArray_CbmTofBuildDigiEvents(Long_t size, void *p);
   static void delete_CbmTofBuildDigiEvents(void *p);
   static void deleteArray_CbmTofBuildDigiEvents(void *p);
   static void destruct_CbmTofBuildDigiEvents(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofBuildDigiEvents*)
   {
      ::CbmTofBuildDigiEvents *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofBuildDigiEvents >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofBuildDigiEvents", ::CbmTofBuildDigiEvents::Class_Version(), "CbmTofBuildDigiEvents.h", 37,
                  typeid(::CbmTofBuildDigiEvents), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofBuildDigiEvents::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofBuildDigiEvents) );
      instance.SetNew(&new_CbmTofBuildDigiEvents);
      instance.SetNewArray(&newArray_CbmTofBuildDigiEvents);
      instance.SetDelete(&delete_CbmTofBuildDigiEvents);
      instance.SetDeleteArray(&deleteArray_CbmTofBuildDigiEvents);
      instance.SetDestructor(&destruct_CbmTofBuildDigiEvents);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofBuildDigiEvents*)
   {
      return GenerateInitInstanceLocal((::CbmTofBuildDigiEvents*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofBuildDigiEvents*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofHitMaker(void *p = nullptr);
   static void *newArray_CbmTofHitMaker(Long_t size, void *p);
   static void delete_CbmTofHitMaker(void *p);
   static void deleteArray_CbmTofHitMaker(void *p);
   static void destruct_CbmTofHitMaker(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofHitMaker*)
   {
      ::CbmTofHitMaker *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofHitMaker >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofHitMaker", ::CbmTofHitMaker::Class_Version(), "CbmTofHitMaker.h", 59,
                  typeid(::CbmTofHitMaker), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofHitMaker::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofHitMaker) );
      instance.SetNew(&new_CbmTofHitMaker);
      instance.SetNewArray(&newArray_CbmTofHitMaker);
      instance.SetDelete(&delete_CbmTofHitMaker);
      instance.SetDeleteArray(&deleteArray_CbmTofHitMaker);
      instance.SetDestructor(&destruct_CbmTofHitMaker);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofHitMaker*)
   {
      return GenerateInitInstanceLocal((::CbmTofHitMaker*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofHitMaker*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofCosmicClusterizer(void *p = nullptr);
   static void *newArray_CbmTofCosmicClusterizer(Long_t size, void *p);
   static void delete_CbmTofCosmicClusterizer(void *p);
   static void deleteArray_CbmTofCosmicClusterizer(void *p);
   static void destruct_CbmTofCosmicClusterizer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofCosmicClusterizer*)
   {
      ::CbmTofCosmicClusterizer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofCosmicClusterizer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofCosmicClusterizer", ::CbmTofCosmicClusterizer::Class_Version(), "CbmTofCosmicClusterizer.h", 56,
                  typeid(::CbmTofCosmicClusterizer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofCosmicClusterizer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofCosmicClusterizer) );
      instance.SetNew(&new_CbmTofCosmicClusterizer);
      instance.SetNewArray(&newArray_CbmTofCosmicClusterizer);
      instance.SetDelete(&delete_CbmTofCosmicClusterizer);
      instance.SetDeleteArray(&deleteArray_CbmTofCosmicClusterizer);
      instance.SetDestructor(&destruct_CbmTofCosmicClusterizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofCosmicClusterizer*)
   {
      return GenerateInitInstanceLocal((::CbmTofCosmicClusterizer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofCosmicClusterizer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofEventClusterizer(void *p = nullptr);
   static void *newArray_CbmTofEventClusterizer(Long_t size, void *p);
   static void delete_CbmTofEventClusterizer(void *p);
   static void deleteArray_CbmTofEventClusterizer(void *p);
   static void destruct_CbmTofEventClusterizer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofEventClusterizer*)
   {
      ::CbmTofEventClusterizer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofEventClusterizer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofEventClusterizer", ::CbmTofEventClusterizer::Class_Version(), "CbmTofEventClusterizer.h", 60,
                  typeid(::CbmTofEventClusterizer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofEventClusterizer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofEventClusterizer) );
      instance.SetNew(&new_CbmTofEventClusterizer);
      instance.SetNewArray(&newArray_CbmTofEventClusterizer);
      instance.SetDelete(&delete_CbmTofEventClusterizer);
      instance.SetDeleteArray(&deleteArray_CbmTofEventClusterizer);
      instance.SetDestructor(&destruct_CbmTofEventClusterizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofEventClusterizer*)
   {
      return GenerateInitInstanceLocal((::CbmTofEventClusterizer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofEventClusterizer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofFindTracks(void *p = nullptr);
   static void *newArray_CbmTofFindTracks(Long_t size, void *p);
   static void delete_CbmTofFindTracks(void *p);
   static void deleteArray_CbmTofFindTracks(void *p);
   static void destruct_CbmTofFindTracks(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofFindTracks*)
   {
      ::CbmTofFindTracks *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofFindTracks >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofFindTracks", ::CbmTofFindTracks::Class_Version(), "CbmTofFindTracks.h", 50,
                  typeid(::CbmTofFindTracks), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofFindTracks::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofFindTracks) );
      instance.SetNew(&new_CbmTofFindTracks);
      instance.SetNewArray(&newArray_CbmTofFindTracks);
      instance.SetDelete(&delete_CbmTofFindTracks);
      instance.SetDeleteArray(&deleteArray_CbmTofFindTracks);
      instance.SetDestructor(&destruct_CbmTofFindTracks);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofFindTracks*)
   {
      return GenerateInitInstanceLocal((::CbmTofFindTracks*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofFindTracks*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofExtendTracks(void *p = nullptr);
   static void *newArray_CbmTofExtendTracks(Long_t size, void *p);
   static void delete_CbmTofExtendTracks(void *p);
   static void deleteArray_CbmTofExtendTracks(void *p);
   static void destruct_CbmTofExtendTracks(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofExtendTracks*)
   {
      ::CbmTofExtendTracks *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofExtendTracks >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofExtendTracks", ::CbmTofExtendTracks::Class_Version(), "CbmTofExtendTracks.h", 49,
                  typeid(::CbmTofExtendTracks), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofExtendTracks::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofExtendTracks) );
      instance.SetNew(&new_CbmTofExtendTracks);
      instance.SetNewArray(&newArray_CbmTofExtendTracks);
      instance.SetDelete(&delete_CbmTofExtendTracks);
      instance.SetDeleteArray(&deleteArray_CbmTofExtendTracks);
      instance.SetDestructor(&destruct_CbmTofExtendTracks);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofExtendTracks*)
   {
      return GenerateInitInstanceLocal((::CbmTofExtendTracks*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofExtendTracks*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofSimpClusterizer(void *p = nullptr);
   static void *newArray_CbmTofSimpClusterizer(Long_t size, void *p);
   static void delete_CbmTofSimpClusterizer(void *p);
   static void deleteArray_CbmTofSimpClusterizer(void *p);
   static void destruct_CbmTofSimpClusterizer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofSimpClusterizer*)
   {
      ::CbmTofSimpClusterizer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofSimpClusterizer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofSimpClusterizer", ::CbmTofSimpClusterizer::Class_Version(), "CbmTofSimpClusterizer.h", 47,
                  typeid(::CbmTofSimpClusterizer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofSimpClusterizer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofSimpClusterizer) );
      instance.SetNew(&new_CbmTofSimpClusterizer);
      instance.SetNewArray(&newArray_CbmTofSimpClusterizer);
      instance.SetDelete(&delete_CbmTofSimpClusterizer);
      instance.SetDeleteArray(&deleteArray_CbmTofSimpClusterizer);
      instance.SetDestructor(&destruct_CbmTofSimpClusterizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofSimpClusterizer*)
   {
      return GenerateInitInstanceLocal((::CbmTofSimpClusterizer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofSimpClusterizer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *LKFMinuit_Dictionary();
   static void LKFMinuit_TClassManip(TClass*);
   static void *new_LKFMinuit(void *p = nullptr);
   static void *newArray_LKFMinuit(Long_t size, void *p);
   static void delete_LKFMinuit(void *p);
   static void deleteArray_LKFMinuit(void *p);
   static void destruct_LKFMinuit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LKFMinuit*)
   {
      ::LKFMinuit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::LKFMinuit));
      static ::ROOT::TGenericClassInfo 
         instance("LKFMinuit", "LKFMinuit.h", 24,
                  typeid(::LKFMinuit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &LKFMinuit_Dictionary, isa_proxy, 4,
                  sizeof(::LKFMinuit) );
      instance.SetNew(&new_LKFMinuit);
      instance.SetNewArray(&newArray_LKFMinuit);
      instance.SetDelete(&delete_LKFMinuit);
      instance.SetDeleteArray(&deleteArray_LKFMinuit);
      instance.SetDestructor(&destruct_LKFMinuit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LKFMinuit*)
   {
      return GenerateInitInstanceLocal((::LKFMinuit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LKFMinuit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *LKFMinuit_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::LKFMinuit*)nullptr)->GetClass();
      LKFMinuit_TClassManip(theClass);
   return theClass;
   }

   static void LKFMinuit_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofTrackFinderNN(void *p = nullptr);
   static void *newArray_CbmTofTrackFinderNN(Long_t size, void *p);
   static void delete_CbmTofTrackFinderNN(void *p);
   static void deleteArray_CbmTofTrackFinderNN(void *p);
   static void destruct_CbmTofTrackFinderNN(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofTrackFinderNN*)
   {
      ::CbmTofTrackFinderNN *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofTrackFinderNN >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofTrackFinderNN", ::CbmTofTrackFinderNN::Class_Version(), "CbmTofTrackFinderNN.h", 25,
                  typeid(::CbmTofTrackFinderNN), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofTrackFinderNN::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofTrackFinderNN) );
      instance.SetNew(&new_CbmTofTrackFinderNN);
      instance.SetNewArray(&newArray_CbmTofTrackFinderNN);
      instance.SetDelete(&delete_CbmTofTrackFinderNN);
      instance.SetDeleteArray(&deleteArray_CbmTofTrackFinderNN);
      instance.SetDestructor(&destruct_CbmTofTrackFinderNN);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofTrackFinderNN*)
   {
      return GenerateInitInstanceLocal((::CbmTofTrackFinderNN*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofTrackFinderNN*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofTestBeamClusterizer(void *p = nullptr);
   static void *newArray_CbmTofTestBeamClusterizer(Long_t size, void *p);
   static void delete_CbmTofTestBeamClusterizer(void *p);
   static void deleteArray_CbmTofTestBeamClusterizer(void *p);
   static void destruct_CbmTofTestBeamClusterizer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofTestBeamClusterizer*)
   {
      ::CbmTofTestBeamClusterizer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofTestBeamClusterizer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofTestBeamClusterizer", ::CbmTofTestBeamClusterizer::Class_Version(), "CbmTofTestBeamClusterizer.h", 55,
                  typeid(::CbmTofTestBeamClusterizer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofTestBeamClusterizer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofTestBeamClusterizer) );
      instance.SetNew(&new_CbmTofTestBeamClusterizer);
      instance.SetNewArray(&newArray_CbmTofTestBeamClusterizer);
      instance.SetDelete(&delete_CbmTofTestBeamClusterizer);
      instance.SetDeleteArray(&deleteArray_CbmTofTestBeamClusterizer);
      instance.SetDestructor(&destruct_CbmTofTestBeamClusterizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofTestBeamClusterizer*)
   {
      return GenerateInitInstanceLocal((::CbmTofTestBeamClusterizer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofTestBeamClusterizer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofTrackletTools(void *p = nullptr);
   static void *newArray_CbmTofTrackletTools(Long_t size, void *p);
   static void delete_CbmTofTrackletTools(void *p);
   static void deleteArray_CbmTofTrackletTools(void *p);
   static void destruct_CbmTofTrackletTools(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofTrackletTools*)
   {
      ::CbmTofTrackletTools *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofTrackletTools >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofTrackletTools", ::CbmTofTrackletTools::Class_Version(), "CbmTofTrackletTools.h", 27,
                  typeid(::CbmTofTrackletTools), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofTrackletTools::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofTrackletTools) );
      instance.SetNew(&new_CbmTofTrackletTools);
      instance.SetNewArray(&newArray_CbmTofTrackletTools);
      instance.SetDelete(&delete_CbmTofTrackletTools);
      instance.SetDeleteArray(&deleteArray_CbmTofTrackletTools);
      instance.SetDestructor(&destruct_CbmTofTrackletTools);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofTrackletTools*)
   {
      return GenerateInitInstanceLocal((::CbmTofTrackletTools*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofTrackletTools*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofCalibrator(void *p = nullptr);
   static void *newArray_CbmTofCalibrator(Long_t size, void *p);
   static void delete_CbmTofCalibrator(void *p);
   static void deleteArray_CbmTofCalibrator(void *p);
   static void destruct_CbmTofCalibrator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofCalibrator*)
   {
      ::CbmTofCalibrator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofCalibrator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofCalibrator", ::CbmTofCalibrator::Class_Version(), "CbmTofCalibrator.h", 53,
                  typeid(::CbmTofCalibrator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofCalibrator::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofCalibrator) );
      instance.SetNew(&new_CbmTofCalibrator);
      instance.SetNewArray(&newArray_CbmTofCalibrator);
      instance.SetDelete(&delete_CbmTofCalibrator);
      instance.SetDeleteArray(&deleteArray_CbmTofCalibrator);
      instance.SetDestructor(&destruct_CbmTofCalibrator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofCalibrator*)
   {
      return GenerateInitInstanceLocal((::CbmTofCalibrator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofCalibrator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofUnpackMonitor(void *p = nullptr);
   static void *newArray_CbmTofUnpackMonitor(Long_t size, void *p);
   static void delete_CbmTofUnpackMonitor(void *p);
   static void deleteArray_CbmTofUnpackMonitor(void *p);
   static void destruct_CbmTofUnpackMonitor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofUnpackMonitor*)
   {
      ::CbmTofUnpackMonitor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofUnpackMonitor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofUnpackMonitor", ::CbmTofUnpackMonitor::Class_Version(), "CbmTofUnpackMonitor.h", 22,
                  typeid(::CbmTofUnpackMonitor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofUnpackMonitor::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofUnpackMonitor) );
      instance.SetNew(&new_CbmTofUnpackMonitor);
      instance.SetNewArray(&newArray_CbmTofUnpackMonitor);
      instance.SetDelete(&delete_CbmTofUnpackMonitor);
      instance.SetDeleteArray(&deleteArray_CbmTofUnpackMonitor);
      instance.SetDestructor(&destruct_CbmTofUnpackMonitor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofUnpackMonitor*)
   {
      return GenerateInitInstanceLocal((::CbmTofUnpackMonitor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofUnpackMonitor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofUnpackAlgo(void *p = nullptr);
   static void *newArray_CbmTofUnpackAlgo(Long_t size, void *p);
   static void delete_CbmTofUnpackAlgo(void *p);
   static void deleteArray_CbmTofUnpackAlgo(void *p);
   static void destruct_CbmTofUnpackAlgo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofUnpackAlgo*)
   {
      ::CbmTofUnpackAlgo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofUnpackAlgo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofUnpackAlgo", ::CbmTofUnpackAlgo::Class_Version(), "CbmTofUnpackAlgo.h", 42,
                  typeid(::CbmTofUnpackAlgo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofUnpackAlgo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofUnpackAlgo) );
      instance.SetNew(&new_CbmTofUnpackAlgo);
      instance.SetNewArray(&newArray_CbmTofUnpackAlgo);
      instance.SetDelete(&delete_CbmTofUnpackAlgo);
      instance.SetDeleteArray(&deleteArray_CbmTofUnpackAlgo);
      instance.SetDestructor(&destruct_CbmTofUnpackAlgo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofUnpackAlgo*)
   {
      return GenerateInitInstanceLocal((::CbmTofUnpackAlgo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofUnpackAlgo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTofUnpackConfig(void *p);
   static void deleteArray_CbmTofUnpackConfig(void *p);
   static void destruct_CbmTofUnpackConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofUnpackConfig*)
   {
      ::CbmTofUnpackConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofUnpackConfig >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofUnpackConfig", ::CbmTofUnpackConfig::Class_Version(), "CbmTofUnpackConfig.h", 35,
                  typeid(::CbmTofUnpackConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofUnpackConfig::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofUnpackConfig) );
      instance.SetDelete(&delete_CbmTofUnpackConfig);
      instance.SetDeleteArray(&deleteArray_CbmTofUnpackConfig);
      instance.SetDestructor(&destruct_CbmTofUnpackConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofUnpackConfig*)
   {
      return GenerateInitInstanceLocal((::CbmTofUnpackConfig*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofUnpackConfig*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmTofBuildDigiEvents::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofBuildDigiEvents::Class_Name()
{
   return "CbmTofBuildDigiEvents";
}

//______________________________________________________________________________
const char *CbmTofBuildDigiEvents::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofBuildDigiEvents*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofBuildDigiEvents::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofBuildDigiEvents*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofBuildDigiEvents::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofBuildDigiEvents*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofBuildDigiEvents::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofBuildDigiEvents*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofHitMaker::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofHitMaker::Class_Name()
{
   return "CbmTofHitMaker";
}

//______________________________________________________________________________
const char *CbmTofHitMaker::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitMaker*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofHitMaker::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitMaker*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofHitMaker::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitMaker*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofHitMaker::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitMaker*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofCosmicClusterizer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofCosmicClusterizer::Class_Name()
{
   return "CbmTofCosmicClusterizer";
}

//______________________________________________________________________________
const char *CbmTofCosmicClusterizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCosmicClusterizer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofCosmicClusterizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCosmicClusterizer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofCosmicClusterizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCosmicClusterizer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofCosmicClusterizer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCosmicClusterizer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofEventClusterizer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofEventClusterizer::Class_Name()
{
   return "CbmTofEventClusterizer";
}

//______________________________________________________________________________
const char *CbmTofEventClusterizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofEventClusterizer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofEventClusterizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofEventClusterizer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofEventClusterizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofEventClusterizer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofEventClusterizer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofEventClusterizer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofFindTracks::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofFindTracks::Class_Name()
{
   return "CbmTofFindTracks";
}

//______________________________________________________________________________
const char *CbmTofFindTracks::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofFindTracks*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofFindTracks::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofFindTracks*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofFindTracks::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofFindTracks*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofFindTracks::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofFindTracks*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofExtendTracks::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofExtendTracks::Class_Name()
{
   return "CbmTofExtendTracks";
}

//______________________________________________________________________________
const char *CbmTofExtendTracks::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofExtendTracks*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofExtendTracks::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofExtendTracks*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofExtendTracks::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofExtendTracks*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofExtendTracks::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofExtendTracks*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofSimpClusterizer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofSimpClusterizer::Class_Name()
{
   return "CbmTofSimpClusterizer";
}

//______________________________________________________________________________
const char *CbmTofSimpClusterizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofSimpClusterizer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofSimpClusterizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofSimpClusterizer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofSimpClusterizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofSimpClusterizer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofSimpClusterizer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofSimpClusterizer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofTrackFinderNN::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofTrackFinderNN::Class_Name()
{
   return "CbmTofTrackFinderNN";
}

//______________________________________________________________________________
const char *CbmTofTrackFinderNN::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFinderNN*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofTrackFinderNN::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFinderNN*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofTrackFinderNN::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFinderNN*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofTrackFinderNN::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFinderNN*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofTestBeamClusterizer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofTestBeamClusterizer::Class_Name()
{
   return "CbmTofTestBeamClusterizer";
}

//______________________________________________________________________________
const char *CbmTofTestBeamClusterizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTestBeamClusterizer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofTestBeamClusterizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTestBeamClusterizer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofTestBeamClusterizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTestBeamClusterizer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofTestBeamClusterizer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTestBeamClusterizer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofTrackletTools::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofTrackletTools::Class_Name()
{
   return "CbmTofTrackletTools";
}

//______________________________________________________________________________
const char *CbmTofTrackletTools::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackletTools*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofTrackletTools::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackletTools*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofTrackletTools::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackletTools*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofTrackletTools::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackletTools*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofCalibrator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofCalibrator::Class_Name()
{
   return "CbmTofCalibrator";
}

//______________________________________________________________________________
const char *CbmTofCalibrator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCalibrator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofCalibrator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCalibrator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofCalibrator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCalibrator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofCalibrator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCalibrator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofUnpackMonitor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofUnpackMonitor::Class_Name()
{
   return "CbmTofUnpackMonitor";
}

//______________________________________________________________________________
const char *CbmTofUnpackMonitor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofUnpackMonitor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofUnpackMonitor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofUnpackMonitor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofUnpackMonitor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofUnpackMonitor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofUnpackMonitor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofUnpackMonitor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofUnpackAlgo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofUnpackAlgo::Class_Name()
{
   return "CbmTofUnpackAlgo";
}

//______________________________________________________________________________
const char *CbmTofUnpackAlgo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofUnpackAlgo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofUnpackAlgo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofUnpackAlgo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofUnpackAlgo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofUnpackAlgo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofUnpackAlgo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofUnpackAlgo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofUnpackConfig::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofUnpackConfig::Class_Name()
{
   return "CbmTofUnpackConfig";
}

//______________________________________________________________________________
const char *CbmTofUnpackConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofUnpackConfig*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofUnpackConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofUnpackConfig*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofUnpackConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofUnpackConfig*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofUnpackConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofUnpackConfig*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmTofBuildDigiEvents::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofBuildDigiEvents.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofBuildDigiEvents::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofBuildDigiEvents::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofBuildDigiEvents(void *p) {
      return  p ? new(p) ::CbmTofBuildDigiEvents : new ::CbmTofBuildDigiEvents;
   }
   static void *newArray_CbmTofBuildDigiEvents(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofBuildDigiEvents[nElements] : new ::CbmTofBuildDigiEvents[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofBuildDigiEvents(void *p) {
      delete ((::CbmTofBuildDigiEvents*)p);
   }
   static void deleteArray_CbmTofBuildDigiEvents(void *p) {
      delete [] ((::CbmTofBuildDigiEvents*)p);
   }
   static void destruct_CbmTofBuildDigiEvents(void *p) {
      typedef ::CbmTofBuildDigiEvents current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofBuildDigiEvents

//______________________________________________________________________________
void CbmTofHitMaker::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofHitMaker.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofHitMaker::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofHitMaker::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofHitMaker(void *p) {
      return  p ? new(p) ::CbmTofHitMaker : new ::CbmTofHitMaker;
   }
   static void *newArray_CbmTofHitMaker(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofHitMaker[nElements] : new ::CbmTofHitMaker[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofHitMaker(void *p) {
      delete ((::CbmTofHitMaker*)p);
   }
   static void deleteArray_CbmTofHitMaker(void *p) {
      delete [] ((::CbmTofHitMaker*)p);
   }
   static void destruct_CbmTofHitMaker(void *p) {
      typedef ::CbmTofHitMaker current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofHitMaker

//______________________________________________________________________________
void CbmTofCosmicClusterizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofCosmicClusterizer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofCosmicClusterizer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofCosmicClusterizer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofCosmicClusterizer(void *p) {
      return  p ? new(p) ::CbmTofCosmicClusterizer : new ::CbmTofCosmicClusterizer;
   }
   static void *newArray_CbmTofCosmicClusterizer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofCosmicClusterizer[nElements] : new ::CbmTofCosmicClusterizer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofCosmicClusterizer(void *p) {
      delete ((::CbmTofCosmicClusterizer*)p);
   }
   static void deleteArray_CbmTofCosmicClusterizer(void *p) {
      delete [] ((::CbmTofCosmicClusterizer*)p);
   }
   static void destruct_CbmTofCosmicClusterizer(void *p) {
      typedef ::CbmTofCosmicClusterizer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofCosmicClusterizer

//______________________________________________________________________________
void CbmTofEventClusterizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofEventClusterizer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofEventClusterizer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofEventClusterizer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofEventClusterizer(void *p) {
      return  p ? new(p) ::CbmTofEventClusterizer : new ::CbmTofEventClusterizer;
   }
   static void *newArray_CbmTofEventClusterizer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofEventClusterizer[nElements] : new ::CbmTofEventClusterizer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofEventClusterizer(void *p) {
      delete ((::CbmTofEventClusterizer*)p);
   }
   static void deleteArray_CbmTofEventClusterizer(void *p) {
      delete [] ((::CbmTofEventClusterizer*)p);
   }
   static void destruct_CbmTofEventClusterizer(void *p) {
      typedef ::CbmTofEventClusterizer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofEventClusterizer

//______________________________________________________________________________
void CbmTofFindTracks::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofFindTracks.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofFindTracks::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofFindTracks::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofFindTracks(void *p) {
      return  p ? new(p) ::CbmTofFindTracks : new ::CbmTofFindTracks;
   }
   static void *newArray_CbmTofFindTracks(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofFindTracks[nElements] : new ::CbmTofFindTracks[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofFindTracks(void *p) {
      delete ((::CbmTofFindTracks*)p);
   }
   static void deleteArray_CbmTofFindTracks(void *p) {
      delete [] ((::CbmTofFindTracks*)p);
   }
   static void destruct_CbmTofFindTracks(void *p) {
      typedef ::CbmTofFindTracks current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofFindTracks

//______________________________________________________________________________
void CbmTofExtendTracks::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofExtendTracks.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofExtendTracks::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofExtendTracks::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofExtendTracks(void *p) {
      return  p ? new(p) ::CbmTofExtendTracks : new ::CbmTofExtendTracks;
   }
   static void *newArray_CbmTofExtendTracks(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofExtendTracks[nElements] : new ::CbmTofExtendTracks[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofExtendTracks(void *p) {
      delete ((::CbmTofExtendTracks*)p);
   }
   static void deleteArray_CbmTofExtendTracks(void *p) {
      delete [] ((::CbmTofExtendTracks*)p);
   }
   static void destruct_CbmTofExtendTracks(void *p) {
      typedef ::CbmTofExtendTracks current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofExtendTracks

//______________________________________________________________________________
void CbmTofSimpClusterizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofSimpClusterizer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofSimpClusterizer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofSimpClusterizer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofSimpClusterizer(void *p) {
      return  p ? new(p) ::CbmTofSimpClusterizer : new ::CbmTofSimpClusterizer;
   }
   static void *newArray_CbmTofSimpClusterizer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofSimpClusterizer[nElements] : new ::CbmTofSimpClusterizer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofSimpClusterizer(void *p) {
      delete ((::CbmTofSimpClusterizer*)p);
   }
   static void deleteArray_CbmTofSimpClusterizer(void *p) {
      delete [] ((::CbmTofSimpClusterizer*)p);
   }
   static void destruct_CbmTofSimpClusterizer(void *p) {
      typedef ::CbmTofSimpClusterizer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofSimpClusterizer

namespace ROOT {
   // Wrappers around operator new
   static void *new_LKFMinuit(void *p) {
      return  p ? new(p) ::LKFMinuit : new ::LKFMinuit;
   }
   static void *newArray_LKFMinuit(Long_t nElements, void *p) {
      return p ? new(p) ::LKFMinuit[nElements] : new ::LKFMinuit[nElements];
   }
   // Wrapper around operator delete
   static void delete_LKFMinuit(void *p) {
      delete ((::LKFMinuit*)p);
   }
   static void deleteArray_LKFMinuit(void *p) {
      delete [] ((::LKFMinuit*)p);
   }
   static void destruct_LKFMinuit(void *p) {
      typedef ::LKFMinuit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LKFMinuit

//______________________________________________________________________________
void CbmTofTrackFinderNN::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofTrackFinderNN.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofTrackFinderNN::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofTrackFinderNN::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofTrackFinderNN(void *p) {
      return  p ? new(p) ::CbmTofTrackFinderNN : new ::CbmTofTrackFinderNN;
   }
   static void *newArray_CbmTofTrackFinderNN(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofTrackFinderNN[nElements] : new ::CbmTofTrackFinderNN[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofTrackFinderNN(void *p) {
      delete ((::CbmTofTrackFinderNN*)p);
   }
   static void deleteArray_CbmTofTrackFinderNN(void *p) {
      delete [] ((::CbmTofTrackFinderNN*)p);
   }
   static void destruct_CbmTofTrackFinderNN(void *p) {
      typedef ::CbmTofTrackFinderNN current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofTrackFinderNN

//______________________________________________________________________________
void CbmTofTestBeamClusterizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofTestBeamClusterizer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofTestBeamClusterizer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofTestBeamClusterizer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofTestBeamClusterizer(void *p) {
      return  p ? new(p) ::CbmTofTestBeamClusterizer : new ::CbmTofTestBeamClusterizer;
   }
   static void *newArray_CbmTofTestBeamClusterizer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofTestBeamClusterizer[nElements] : new ::CbmTofTestBeamClusterizer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofTestBeamClusterizer(void *p) {
      delete ((::CbmTofTestBeamClusterizer*)p);
   }
   static void deleteArray_CbmTofTestBeamClusterizer(void *p) {
      delete [] ((::CbmTofTestBeamClusterizer*)p);
   }
   static void destruct_CbmTofTestBeamClusterizer(void *p) {
      typedef ::CbmTofTestBeamClusterizer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofTestBeamClusterizer

//______________________________________________________________________________
void CbmTofTrackletTools::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofTrackletTools.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofTrackletTools::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofTrackletTools::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofTrackletTools(void *p) {
      return  p ? new(p) ::CbmTofTrackletTools : new ::CbmTofTrackletTools;
   }
   static void *newArray_CbmTofTrackletTools(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofTrackletTools[nElements] : new ::CbmTofTrackletTools[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofTrackletTools(void *p) {
      delete ((::CbmTofTrackletTools*)p);
   }
   static void deleteArray_CbmTofTrackletTools(void *p) {
      delete [] ((::CbmTofTrackletTools*)p);
   }
   static void destruct_CbmTofTrackletTools(void *p) {
      typedef ::CbmTofTrackletTools current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofTrackletTools

//______________________________________________________________________________
void CbmTofCalibrator::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofCalibrator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofCalibrator::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofCalibrator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofCalibrator(void *p) {
      return  p ? new(p) ::CbmTofCalibrator : new ::CbmTofCalibrator;
   }
   static void *newArray_CbmTofCalibrator(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofCalibrator[nElements] : new ::CbmTofCalibrator[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofCalibrator(void *p) {
      delete ((::CbmTofCalibrator*)p);
   }
   static void deleteArray_CbmTofCalibrator(void *p) {
      delete [] ((::CbmTofCalibrator*)p);
   }
   static void destruct_CbmTofCalibrator(void *p) {
      typedef ::CbmTofCalibrator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofCalibrator

//______________________________________________________________________________
void CbmTofUnpackMonitor::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofUnpackMonitor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofUnpackMonitor::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofUnpackMonitor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofUnpackMonitor(void *p) {
      return  p ? new(p) ::CbmTofUnpackMonitor : new ::CbmTofUnpackMonitor;
   }
   static void *newArray_CbmTofUnpackMonitor(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofUnpackMonitor[nElements] : new ::CbmTofUnpackMonitor[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofUnpackMonitor(void *p) {
      delete ((::CbmTofUnpackMonitor*)p);
   }
   static void deleteArray_CbmTofUnpackMonitor(void *p) {
      delete [] ((::CbmTofUnpackMonitor*)p);
   }
   static void destruct_CbmTofUnpackMonitor(void *p) {
      typedef ::CbmTofUnpackMonitor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofUnpackMonitor

//______________________________________________________________________________
void CbmTofUnpackAlgo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofUnpackAlgo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofUnpackAlgo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofUnpackAlgo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofUnpackAlgo(void *p) {
      return  p ? new(p) ::CbmTofUnpackAlgo : new ::CbmTofUnpackAlgo;
   }
   static void *newArray_CbmTofUnpackAlgo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofUnpackAlgo[nElements] : new ::CbmTofUnpackAlgo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofUnpackAlgo(void *p) {
      delete ((::CbmTofUnpackAlgo*)p);
   }
   static void deleteArray_CbmTofUnpackAlgo(void *p) {
      delete [] ((::CbmTofUnpackAlgo*)p);
   }
   static void destruct_CbmTofUnpackAlgo(void *p) {
      typedef ::CbmTofUnpackAlgo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofUnpackAlgo

//______________________________________________________________________________
void CbmTofUnpackConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofUnpackConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofUnpackConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofUnpackConfig::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTofUnpackConfig(void *p) {
      delete ((::CbmTofUnpackConfig*)p);
   }
   static void deleteArray_CbmTofUnpackConfig(void *p) {
      delete [] ((::CbmTofUnpackConfig*)p);
   }
   static void destruct_CbmTofUnpackConfig(void *p) {
      typedef ::CbmTofUnpackConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofUnpackConfig

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
   static TClass *vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<vector<list<CbmTofHit*> > > > >*)
   {
      vector<vector<vector<vector<list<CbmTofHit*> > > > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<vector<list<CbmTofHit*> > > > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<vector<list<CbmTofHit*> > > > >", -2, "vector", 389,
                  typeid(vector<vector<vector<vector<list<CbmTofHit*> > > > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<vector<list<CbmTofHit*> > > > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<vector<list<CbmTofHit*> > > > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<vector<list<CbmTofHit*> > > > >","std::vector<std::vector<std::vector<std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > >, std::allocator<std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > > > >, std::allocator<std::vector<std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > >, std::allocator<std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > > > > > >, std::allocator<std::vector<std::vector<std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > >, std::allocator<std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > > > >, std::allocator<std::vector<std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > >, std::allocator<std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > > > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<vector<list<CbmTofHit*> > > > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<vector<list<CbmTofHit*> > > > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<list<CbmTofHit*> > > > > : new vector<vector<vector<vector<list<CbmTofHit*> > > > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<list<CbmTofHit*> > > > >[nElements] : new vector<vector<vector<vector<list<CbmTofHit*> > > > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<vector<list<CbmTofHit*> > > > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<vector<list<CbmTofHit*> > > > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<vector<list<CbmTofHit*> > > > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<vector<list<CbmTofHit*> > > > >

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<vector<int> > > >*)
   {
      vector<vector<vector<vector<int> > > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<vector<int> > > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<vector<int> > > >", -2, "vector", 389,
                  typeid(vector<vector<vector<vector<int> > > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<vector<int> > > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<vector<int> > > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<vector<int> > > >","std::vector<std::vector<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >, std::allocator<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > > >, std::allocator<std::vector<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >, std::allocator<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<vector<int> > > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<vector<int> > > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<int> > > > : new vector<vector<vector<vector<int> > > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<int> > > >[nElements] : new vector<vector<vector<vector<int> > > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<vector<int> > > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<vector<int> > > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<vector<int> > > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<vector<int> > > >

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
   static TClass *vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<vector<TH2*> > > >*)
   {
      vector<vector<vector<vector<TH2*> > > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<vector<TH2*> > > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<vector<TH2*> > > >", -2, "vector", 389,
                  typeid(vector<vector<vector<vector<TH2*> > > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<vector<TH2*> > > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<vector<TH2*> > > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<vector<TH2*> > > >","std::vector<std::vector<std::vector<std::vector<TH2*, std::allocator<TH2*> >, std::allocator<std::vector<TH2*, std::allocator<TH2*> > > >, std::allocator<std::vector<std::vector<TH2*, std::allocator<TH2*> >, std::allocator<std::vector<TH2*, std::allocator<TH2*> > > > > >, std::allocator<std::vector<std::vector<std::vector<TH2*, std::allocator<TH2*> >, std::allocator<std::vector<TH2*, std::allocator<TH2*> > > >, std::allocator<std::vector<std::vector<TH2*, std::allocator<TH2*> >, std::allocator<std::vector<TH2*, std::allocator<TH2*> > > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<vector<TH2*> > > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<vector<TH2*> > > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<TH2*> > > > : new vector<vector<vector<vector<TH2*> > > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<TH2*> > > >[nElements] : new vector<vector<vector<vector<TH2*> > > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<vector<TH2*> > > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<vector<TH2*> > > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEvectorlETH2mUgRsPgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<vector<TH2*> > > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<vector<TH2*> > > >

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<vector<CbmTofDigi*> > > >*)
   {
      vector<vector<vector<vector<CbmTofDigi*> > > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<vector<CbmTofDigi*> > > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<vector<CbmTofDigi*> > > >", -2, "vector", 389,
                  typeid(vector<vector<vector<vector<CbmTofDigi*> > > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<vector<CbmTofDigi*> > > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<vector<CbmTofDigi*> > > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<vector<CbmTofDigi*> > > >","std::vector<std::vector<std::vector<std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> >, std::allocator<std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> > > >, std::allocator<std::vector<std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> >, std::allocator<std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> > > > > >, std::allocator<std::vector<std::vector<std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> >, std::allocator<std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> > > >, std::allocator<std::vector<std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> >, std::allocator<std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> > > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<vector<CbmTofDigi*> > > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<vector<CbmTofDigi*> > > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<CbmTofDigi*> > > > : new vector<vector<vector<vector<CbmTofDigi*> > > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<CbmTofDigi*> > > >[nElements] : new vector<vector<vector<vector<CbmTofDigi*> > > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<vector<CbmTofDigi*> > > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<vector<CbmTofDigi*> > > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<vector<CbmTofDigi*> > > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<vector<CbmTofDigi*> > > >

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<list<CbmTofHit*> > > >*)
   {
      vector<vector<vector<list<CbmTofHit*> > > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<list<CbmTofHit*> > > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<list<CbmTofHit*> > > >", -2, "vector", 389,
                  typeid(vector<vector<vector<list<CbmTofHit*> > > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<list<CbmTofHit*> > > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<list<CbmTofHit*> > > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<list<CbmTofHit*> > > >","std::vector<std::vector<std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > >, std::allocator<std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > > > >, std::allocator<std::vector<std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > >, std::allocator<std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<list<CbmTofHit*> > > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<list<CbmTofHit*> > > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<list<CbmTofHit*> > > > : new vector<vector<vector<list<CbmTofHit*> > > >;
   }
   static void *newArray_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<list<CbmTofHit*> > > >[nElements] : new vector<vector<vector<list<CbmTofHit*> > > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<list<CbmTofHit*> > > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<list<CbmTofHit*> > > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlElistlECbmTofHitmUgRsPgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<list<CbmTofHit*> > > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<list<CbmTofHit*> > > >

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
   static TClass *vectorlEvectorlEvectorlETH2mUgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlETH2mUgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<TH2*> > >*)
   {
      vector<vector<vector<TH2*> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<TH2*> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<TH2*> > >", -2, "vector", 389,
                  typeid(vector<vector<vector<TH2*> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlETH2mUgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<TH2*> > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<TH2*> > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<TH2*> > >","std::vector<std::vector<std::vector<TH2*, std::allocator<TH2*> >, std::allocator<std::vector<TH2*, std::allocator<TH2*> > > >, std::allocator<std::vector<std::vector<TH2*, std::allocator<TH2*> >, std::allocator<std::vector<TH2*, std::allocator<TH2*> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<TH2*> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlETH2mUgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<TH2*> > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlETH2mUgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlETH2mUgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<TH2*> > > : new vector<vector<vector<TH2*> > >;
   }
   static void *newArray_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<TH2*> > >[nElements] : new vector<vector<vector<TH2*> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<TH2*> > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<TH2*> > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlETH2mUgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<TH2*> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<TH2*> > >

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlETH1mUgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlETH1mUgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<TH1*> > >*)
   {
      vector<vector<vector<TH1*> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<TH1*> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<TH1*> > >", -2, "vector", 389,
                  typeid(vector<vector<vector<TH1*> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlETH1mUgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<TH1*> > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<TH1*> > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<TH1*> > >","std::vector<std::vector<std::vector<TH1*, std::allocator<TH1*> >, std::allocator<std::vector<TH1*, std::allocator<TH1*> > > >, std::allocator<std::vector<std::vector<TH1*, std::allocator<TH1*> >, std::allocator<std::vector<TH1*, std::allocator<TH1*> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<TH1*> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlETH1mUgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<TH1*> > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlETH1mUgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlETH1mUgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<TH1*> > > : new vector<vector<vector<TH1*> > >;
   }
   static void *newArray_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<TH1*> > >[nElements] : new vector<vector<vector<TH1*> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<TH1*> > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<TH1*> > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlETH1mUgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<TH1*> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<TH1*> > >

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<CbmTofDigi*> > >*)
   {
      vector<vector<vector<CbmTofDigi*> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<CbmTofDigi*> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<CbmTofDigi*> > >", -2, "vector", 389,
                  typeid(vector<vector<vector<CbmTofDigi*> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<CbmTofDigi*> > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<CbmTofDigi*> > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<CbmTofDigi*> > >","std::vector<std::vector<std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> >, std::allocator<std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> > > >, std::allocator<std::vector<std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> >, std::allocator<std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<CbmTofDigi*> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<CbmTofDigi*> > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<CbmTofDigi*> > > : new vector<vector<vector<CbmTofDigi*> > >;
   }
   static void *newArray_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<CbmTofDigi*> > >[nElements] : new vector<vector<vector<CbmTofDigi*> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<CbmTofDigi*> > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<CbmTofDigi*> > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlECbmTofDigimUgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<CbmTofDigi*> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<CbmTofDigi*> > >

namespace ROOT {
   static TClass *vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<list<CbmTofHit*> > >*)
   {
      vector<vector<list<CbmTofHit*> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<list<CbmTofHit*> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<list<CbmTofHit*> > >", -2, "vector", 389,
                  typeid(vector<vector<list<CbmTofHit*> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<list<CbmTofHit*> > >) );
      instance.SetNew(&new_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<list<CbmTofHit*> > > >()));

      ::ROOT::AddClassAlternate("vector<vector<list<CbmTofHit*> > >","std::vector<std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > >, std::allocator<std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<list<CbmTofHit*> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<list<CbmTofHit*> > >*)nullptr)->GetClass();
      vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<list<CbmTofHit*> > > : new vector<vector<list<CbmTofHit*> > >;
   }
   static void *newArray_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<list<CbmTofHit*> > >[nElements] : new vector<vector<list<CbmTofHit*> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR(void *p) {
      delete ((vector<vector<list<CbmTofHit*> > >*)p);
   }
   static void deleteArray_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<list<CbmTofHit*> > >*)p);
   }
   static void destruct_vectorlEvectorlElistlECbmTofHitmUgRsPgRsPgR(void *p) {
      typedef vector<vector<list<CbmTofHit*> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<list<CbmTofHit*> > >

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
   static TClass *vectorlEvectorlETProfilemUgRsPgR_Dictionary();
   static void vectorlEvectorlETProfilemUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlETProfilemUgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlETProfilemUgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlETProfilemUgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlETProfilemUgRsPgR(void *p);
   static void destruct_vectorlEvectorlETProfilemUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<TProfile*> >*)
   {
      vector<vector<TProfile*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<TProfile*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<TProfile*> >", -2, "vector", 389,
                  typeid(vector<vector<TProfile*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlETProfilemUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<TProfile*> >) );
      instance.SetNew(&new_vectorlEvectorlETProfilemUgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlETProfilemUgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlETProfilemUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlETProfilemUgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlETProfilemUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<TProfile*> > >()));

      ::ROOT::AddClassAlternate("vector<vector<TProfile*> >","std::vector<std::vector<TProfile*, std::allocator<TProfile*> >, std::allocator<std::vector<TProfile*, std::allocator<TProfile*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<TProfile*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlETProfilemUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<TProfile*> >*)nullptr)->GetClass();
      vectorlEvectorlETProfilemUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlETProfilemUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlETProfilemUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TProfile*> > : new vector<vector<TProfile*> >;
   }
   static void *newArray_vectorlEvectorlETProfilemUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TProfile*> >[nElements] : new vector<vector<TProfile*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlETProfilemUgRsPgR(void *p) {
      delete ((vector<vector<TProfile*> >*)p);
   }
   static void deleteArray_vectorlEvectorlETProfilemUgRsPgR(void *p) {
      delete [] ((vector<vector<TProfile*> >*)p);
   }
   static void destruct_vectorlEvectorlETProfilemUgRsPgR(void *p) {
      typedef vector<vector<TProfile*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<TProfile*> >

namespace ROOT {
   static TClass *vectorlEvectorlETH3mUgRsPgR_Dictionary();
   static void vectorlEvectorlETH3mUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlETH3mUgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlETH3mUgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlETH3mUgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlETH3mUgRsPgR(void *p);
   static void destruct_vectorlEvectorlETH3mUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<TH3*> >*)
   {
      vector<vector<TH3*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<TH3*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<TH3*> >", -2, "vector", 389,
                  typeid(vector<vector<TH3*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlETH3mUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<TH3*> >) );
      instance.SetNew(&new_vectorlEvectorlETH3mUgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlETH3mUgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlETH3mUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlETH3mUgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlETH3mUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<TH3*> > >()));

      ::ROOT::AddClassAlternate("vector<vector<TH3*> >","std::vector<std::vector<TH3*, std::allocator<TH3*> >, std::allocator<std::vector<TH3*, std::allocator<TH3*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<TH3*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlETH3mUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<TH3*> >*)nullptr)->GetClass();
      vectorlEvectorlETH3mUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlETH3mUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlETH3mUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TH3*> > : new vector<vector<TH3*> >;
   }
   static void *newArray_vectorlEvectorlETH3mUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TH3*> >[nElements] : new vector<vector<TH3*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlETH3mUgRsPgR(void *p) {
      delete ((vector<vector<TH3*> >*)p);
   }
   static void deleteArray_vectorlEvectorlETH3mUgRsPgR(void *p) {
      delete [] ((vector<vector<TH3*> >*)p);
   }
   static void destruct_vectorlEvectorlETH3mUgRsPgR(void *p) {
      typedef vector<vector<TH3*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<TH3*> >

namespace ROOT {
   static TClass *vectorlEvectorlETH2mUgRsPgR_Dictionary();
   static void vectorlEvectorlETH2mUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlETH2mUgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlETH2mUgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlETH2mUgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlETH2mUgRsPgR(void *p);
   static void destruct_vectorlEvectorlETH2mUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<TH2*> >*)
   {
      vector<vector<TH2*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<TH2*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<TH2*> >", -2, "vector", 389,
                  typeid(vector<vector<TH2*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlETH2mUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<TH2*> >) );
      instance.SetNew(&new_vectorlEvectorlETH2mUgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlETH2mUgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlETH2mUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlETH2mUgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlETH2mUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<TH2*> > >()));

      ::ROOT::AddClassAlternate("vector<vector<TH2*> >","std::vector<std::vector<TH2*, std::allocator<TH2*> >, std::allocator<std::vector<TH2*, std::allocator<TH2*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<TH2*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlETH2mUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<TH2*> >*)nullptr)->GetClass();
      vectorlEvectorlETH2mUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlETH2mUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlETH2mUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TH2*> > : new vector<vector<TH2*> >;
   }
   static void *newArray_vectorlEvectorlETH2mUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TH2*> >[nElements] : new vector<vector<TH2*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlETH2mUgRsPgR(void *p) {
      delete ((vector<vector<TH2*> >*)p);
   }
   static void deleteArray_vectorlEvectorlETH2mUgRsPgR(void *p) {
      delete [] ((vector<vector<TH2*> >*)p);
   }
   static void destruct_vectorlEvectorlETH2mUgRsPgR(void *p) {
      typedef vector<vector<TH2*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<TH2*> >

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
   static TClass *vectorlEvectorlECbmTofTrackletmUgRsPgR_Dictionary();
   static void vectorlEvectorlECbmTofTrackletmUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlECbmTofTrackletmUgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlECbmTofTrackletmUgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlECbmTofTrackletmUgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlECbmTofTrackletmUgRsPgR(void *p);
   static void destruct_vectorlEvectorlECbmTofTrackletmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<CbmTofTracklet*> >*)
   {
      vector<vector<CbmTofTracklet*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<CbmTofTracklet*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<CbmTofTracklet*> >", -2, "vector", 389,
                  typeid(vector<vector<CbmTofTracklet*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlECbmTofTrackletmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<CbmTofTracklet*> >) );
      instance.SetNew(&new_vectorlEvectorlECbmTofTrackletmUgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlECbmTofTrackletmUgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlECbmTofTrackletmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlECbmTofTrackletmUgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlECbmTofTrackletmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<CbmTofTracklet*> > >()));

      ::ROOT::AddClassAlternate("vector<vector<CbmTofTracklet*> >","std::vector<std::vector<CbmTofTracklet*, std::allocator<CbmTofTracklet*> >, std::allocator<std::vector<CbmTofTracklet*, std::allocator<CbmTofTracklet*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<CbmTofTracklet*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlECbmTofTrackletmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<CbmTofTracklet*> >*)nullptr)->GetClass();
      vectorlEvectorlECbmTofTrackletmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlECbmTofTrackletmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlECbmTofTrackletmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmTofTracklet*> > : new vector<vector<CbmTofTracklet*> >;
   }
   static void *newArray_vectorlEvectorlECbmTofTrackletmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmTofTracklet*> >[nElements] : new vector<vector<CbmTofTracklet*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlECbmTofTrackletmUgRsPgR(void *p) {
      delete ((vector<vector<CbmTofTracklet*> >*)p);
   }
   static void deleteArray_vectorlEvectorlECbmTofTrackletmUgRsPgR(void *p) {
      delete [] ((vector<vector<CbmTofTracklet*> >*)p);
   }
   static void destruct_vectorlEvectorlECbmTofTrackletmUgRsPgR(void *p) {
      typedef vector<vector<CbmTofTracklet*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<CbmTofTracklet*> >

namespace ROOT {
   static TClass *vectorlEvectorlECbmTofDigimUgRsPgR_Dictionary();
   static void vectorlEvectorlECbmTofDigimUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlECbmTofDigimUgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlECbmTofDigimUgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlECbmTofDigimUgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlECbmTofDigimUgRsPgR(void *p);
   static void destruct_vectorlEvectorlECbmTofDigimUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<CbmTofDigi*> >*)
   {
      vector<vector<CbmTofDigi*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<CbmTofDigi*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<CbmTofDigi*> >", -2, "vector", 389,
                  typeid(vector<vector<CbmTofDigi*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlECbmTofDigimUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<CbmTofDigi*> >) );
      instance.SetNew(&new_vectorlEvectorlECbmTofDigimUgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlECbmTofDigimUgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlECbmTofDigimUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlECbmTofDigimUgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlECbmTofDigimUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<CbmTofDigi*> > >()));

      ::ROOT::AddClassAlternate("vector<vector<CbmTofDigi*> >","std::vector<std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> >, std::allocator<std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<CbmTofDigi*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlECbmTofDigimUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<CbmTofDigi*> >*)nullptr)->GetClass();
      vectorlEvectorlECbmTofDigimUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlECbmTofDigimUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlECbmTofDigimUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmTofDigi*> > : new vector<vector<CbmTofDigi*> >;
   }
   static void *newArray_vectorlEvectorlECbmTofDigimUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmTofDigi*> >[nElements] : new vector<vector<CbmTofDigi*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlECbmTofDigimUgRsPgR(void *p) {
      delete ((vector<vector<CbmTofDigi*> >*)p);
   }
   static void deleteArray_vectorlEvectorlECbmTofDigimUgRsPgR(void *p) {
      delete [] ((vector<vector<CbmTofDigi*> >*)p);
   }
   static void destruct_vectorlEvectorlECbmTofDigimUgRsPgR(void *p) {
      typedef vector<vector<CbmTofDigi*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<CbmTofDigi*> >

namespace ROOT {
   static TClass *vectorlEvectorlECbmPixelHitmUgRsPgR_Dictionary();
   static void vectorlEvectorlECbmPixelHitmUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlECbmPixelHitmUgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlECbmPixelHitmUgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlECbmPixelHitmUgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlECbmPixelHitmUgRsPgR(void *p);
   static void destruct_vectorlEvectorlECbmPixelHitmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<CbmPixelHit*> >*)
   {
      vector<vector<CbmPixelHit*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<CbmPixelHit*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<CbmPixelHit*> >", -2, "vector", 389,
                  typeid(vector<vector<CbmPixelHit*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlECbmPixelHitmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<CbmPixelHit*> >) );
      instance.SetNew(&new_vectorlEvectorlECbmPixelHitmUgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlECbmPixelHitmUgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlECbmPixelHitmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlECbmPixelHitmUgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlECbmPixelHitmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<CbmPixelHit*> > >()));

      ::ROOT::AddClassAlternate("vector<vector<CbmPixelHit*> >","std::vector<std::vector<CbmPixelHit*, std::allocator<CbmPixelHit*> >, std::allocator<std::vector<CbmPixelHit*, std::allocator<CbmPixelHit*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<CbmPixelHit*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlECbmPixelHitmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<CbmPixelHit*> >*)nullptr)->GetClass();
      vectorlEvectorlECbmPixelHitmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlECbmPixelHitmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlECbmPixelHitmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmPixelHit*> > : new vector<vector<CbmPixelHit*> >;
   }
   static void *newArray_vectorlEvectorlECbmPixelHitmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmPixelHit*> >[nElements] : new vector<vector<CbmPixelHit*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlECbmPixelHitmUgRsPgR(void *p) {
      delete ((vector<vector<CbmPixelHit*> >*)p);
   }
   static void deleteArray_vectorlEvectorlECbmPixelHitmUgRsPgR(void *p) {
      delete [] ((vector<vector<CbmPixelHit*> >*)p);
   }
   static void destruct_vectorlEvectorlECbmPixelHitmUgRsPgR(void *p) {
      typedef vector<vector<CbmPixelHit*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<CbmPixelHit*> >

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
   static TClass *vectorlElistlECbmTofHitmUgRsPgR_Dictionary();
   static void vectorlElistlECbmTofHitmUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlElistlECbmTofHitmUgRsPgR(void *p = nullptr);
   static void *newArray_vectorlElistlECbmTofHitmUgRsPgR(Long_t size, void *p);
   static void delete_vectorlElistlECbmTofHitmUgRsPgR(void *p);
   static void deleteArray_vectorlElistlECbmTofHitmUgRsPgR(void *p);
   static void destruct_vectorlElistlECbmTofHitmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<list<CbmTofHit*> >*)
   {
      vector<list<CbmTofHit*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<list<CbmTofHit*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<list<CbmTofHit*> >", -2, "vector", 389,
                  typeid(vector<list<CbmTofHit*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlElistlECbmTofHitmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<list<CbmTofHit*> >) );
      instance.SetNew(&new_vectorlElistlECbmTofHitmUgRsPgR);
      instance.SetNewArray(&newArray_vectorlElistlECbmTofHitmUgRsPgR);
      instance.SetDelete(&delete_vectorlElistlECbmTofHitmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlElistlECbmTofHitmUgRsPgR);
      instance.SetDestructor(&destruct_vectorlElistlECbmTofHitmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<list<CbmTofHit*> > >()));

      ::ROOT::AddClassAlternate("vector<list<CbmTofHit*> >","std::vector<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >, std::allocator<std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<list<CbmTofHit*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlElistlECbmTofHitmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<list<CbmTofHit*> >*)nullptr)->GetClass();
      vectorlElistlECbmTofHitmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlElistlECbmTofHitmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlElistlECbmTofHitmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<list<CbmTofHit*> > : new vector<list<CbmTofHit*> >;
   }
   static void *newArray_vectorlElistlECbmTofHitmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<list<CbmTofHit*> >[nElements] : new vector<list<CbmTofHit*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlElistlECbmTofHitmUgRsPgR(void *p) {
      delete ((vector<list<CbmTofHit*> >*)p);
   }
   static void deleteArray_vectorlElistlECbmTofHitmUgRsPgR(void *p) {
      delete [] ((vector<list<CbmTofHit*> >*)p);
   }
   static void destruct_vectorlElistlECbmTofHitmUgRsPgR(void *p) {
      typedef vector<list<CbmTofHit*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<list<CbmTofHit*> >

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
   static TClass *vectorlETH3mUgR_Dictionary();
   static void vectorlETH3mUgR_TClassManip(TClass*);
   static void *new_vectorlETH3mUgR(void *p = nullptr);
   static void *newArray_vectorlETH3mUgR(Long_t size, void *p);
   static void delete_vectorlETH3mUgR(void *p);
   static void deleteArray_vectorlETH3mUgR(void *p);
   static void destruct_vectorlETH3mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TH3*>*)
   {
      vector<TH3*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TH3*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TH3*>", -2, "vector", 389,
                  typeid(vector<TH3*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETH3mUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TH3*>) );
      instance.SetNew(&new_vectorlETH3mUgR);
      instance.SetNewArray(&newArray_vectorlETH3mUgR);
      instance.SetDelete(&delete_vectorlETH3mUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETH3mUgR);
      instance.SetDestructor(&destruct_vectorlETH3mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TH3*> >()));

      ::ROOT::AddClassAlternate("vector<TH3*>","std::vector<TH3*, std::allocator<TH3*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TH3*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETH3mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TH3*>*)nullptr)->GetClass();
      vectorlETH3mUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETH3mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETH3mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH3*> : new vector<TH3*>;
   }
   static void *newArray_vectorlETH3mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH3*>[nElements] : new vector<TH3*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETH3mUgR(void *p) {
      delete ((vector<TH3*>*)p);
   }
   static void deleteArray_vectorlETH3mUgR(void *p) {
      delete [] ((vector<TH3*>*)p);
   }
   static void destruct_vectorlETH3mUgR(void *p) {
      typedef vector<TH3*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TH3*>

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
   static TClass *vectorlECbmTofTrackletParammUgR_Dictionary();
   static void vectorlECbmTofTrackletParammUgR_TClassManip(TClass*);
   static void *new_vectorlECbmTofTrackletParammUgR(void *p = nullptr);
   static void *newArray_vectorlECbmTofTrackletParammUgR(Long_t size, void *p);
   static void delete_vectorlECbmTofTrackletParammUgR(void *p);
   static void deleteArray_vectorlECbmTofTrackletParammUgR(void *p);
   static void destruct_vectorlECbmTofTrackletParammUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmTofTrackletParam*>*)
   {
      vector<CbmTofTrackletParam*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmTofTrackletParam*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmTofTrackletParam*>", -2, "vector", 389,
                  typeid(vector<CbmTofTrackletParam*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmTofTrackletParammUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmTofTrackletParam*>) );
      instance.SetNew(&new_vectorlECbmTofTrackletParammUgR);
      instance.SetNewArray(&newArray_vectorlECbmTofTrackletParammUgR);
      instance.SetDelete(&delete_vectorlECbmTofTrackletParammUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmTofTrackletParammUgR);
      instance.SetDestructor(&destruct_vectorlECbmTofTrackletParammUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmTofTrackletParam*> >()));

      ::ROOT::AddClassAlternate("vector<CbmTofTrackletParam*>","std::vector<CbmTofTrackletParam*, std::allocator<CbmTofTrackletParam*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmTofTrackletParam*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmTofTrackletParammUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmTofTrackletParam*>*)nullptr)->GetClass();
      vectorlECbmTofTrackletParammUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmTofTrackletParammUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmTofTrackletParammUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTofTrackletParam*> : new vector<CbmTofTrackletParam*>;
   }
   static void *newArray_vectorlECbmTofTrackletParammUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTofTrackletParam*>[nElements] : new vector<CbmTofTrackletParam*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmTofTrackletParammUgR(void *p) {
      delete ((vector<CbmTofTrackletParam*>*)p);
   }
   static void deleteArray_vectorlECbmTofTrackletParammUgR(void *p) {
      delete [] ((vector<CbmTofTrackletParam*>*)p);
   }
   static void destruct_vectorlECbmTofTrackletParammUgR(void *p) {
      typedef vector<CbmTofTrackletParam*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmTofTrackletParam*>

namespace ROOT {
   static TClass *vectorlECbmTofTrackletmUgR_Dictionary();
   static void vectorlECbmTofTrackletmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmTofTrackletmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmTofTrackletmUgR(Long_t size, void *p);
   static void delete_vectorlECbmTofTrackletmUgR(void *p);
   static void deleteArray_vectorlECbmTofTrackletmUgR(void *p);
   static void destruct_vectorlECbmTofTrackletmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmTofTracklet*>*)
   {
      vector<CbmTofTracklet*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmTofTracklet*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmTofTracklet*>", -2, "vector", 389,
                  typeid(vector<CbmTofTracklet*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmTofTrackletmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmTofTracklet*>) );
      instance.SetNew(&new_vectorlECbmTofTrackletmUgR);
      instance.SetNewArray(&newArray_vectorlECbmTofTrackletmUgR);
      instance.SetDelete(&delete_vectorlECbmTofTrackletmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmTofTrackletmUgR);
      instance.SetDestructor(&destruct_vectorlECbmTofTrackletmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmTofTracklet*> >()));

      ::ROOT::AddClassAlternate("vector<CbmTofTracklet*>","std::vector<CbmTofTracklet*, std::allocator<CbmTofTracklet*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmTofTracklet*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmTofTrackletmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmTofTracklet*>*)nullptr)->GetClass();
      vectorlECbmTofTrackletmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmTofTrackletmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmTofTrackletmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTofTracklet*> : new vector<CbmTofTracklet*>;
   }
   static void *newArray_vectorlECbmTofTrackletmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTofTracklet*>[nElements] : new vector<CbmTofTracklet*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmTofTrackletmUgR(void *p) {
      delete ((vector<CbmTofTracklet*>*)p);
   }
   static void deleteArray_vectorlECbmTofTrackletmUgR(void *p) {
      delete [] ((vector<CbmTofTracklet*>*)p);
   }
   static void destruct_vectorlECbmTofTrackletmUgR(void *p) {
      typedef vector<CbmTofTracklet*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmTofTracklet*>

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
   static TClass *vectorlECbmTofDigimUgR_Dictionary();
   static void vectorlECbmTofDigimUgR_TClassManip(TClass*);
   static void *new_vectorlECbmTofDigimUgR(void *p = nullptr);
   static void *newArray_vectorlECbmTofDigimUgR(Long_t size, void *p);
   static void delete_vectorlECbmTofDigimUgR(void *p);
   static void deleteArray_vectorlECbmTofDigimUgR(void *p);
   static void destruct_vectorlECbmTofDigimUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmTofDigi*>*)
   {
      vector<CbmTofDigi*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmTofDigi*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmTofDigi*>", -2, "vector", 389,
                  typeid(vector<CbmTofDigi*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmTofDigimUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmTofDigi*>) );
      instance.SetNew(&new_vectorlECbmTofDigimUgR);
      instance.SetNewArray(&newArray_vectorlECbmTofDigimUgR);
      instance.SetDelete(&delete_vectorlECbmTofDigimUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmTofDigimUgR);
      instance.SetDestructor(&destruct_vectorlECbmTofDigimUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmTofDigi*> >()));

      ::ROOT::AddClassAlternate("vector<CbmTofDigi*>","std::vector<CbmTofDigi*, std::allocator<CbmTofDigi*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmTofDigi*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmTofDigimUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmTofDigi*>*)nullptr)->GetClass();
      vectorlECbmTofDigimUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmTofDigimUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmTofDigimUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTofDigi*> : new vector<CbmTofDigi*>;
   }
   static void *newArray_vectorlECbmTofDigimUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTofDigi*>[nElements] : new vector<CbmTofDigi*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmTofDigimUgR(void *p) {
      delete ((vector<CbmTofDigi*>*)p);
   }
   static void deleteArray_vectorlECbmTofDigimUgR(void *p) {
      delete [] ((vector<CbmTofDigi*>*)p);
   }
   static void destruct_vectorlECbmTofDigimUgR(void *p) {
      typedef vector<CbmTofDigi*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmTofDigi*>

namespace ROOT {
   static TClass *vectorlECbmPixelHitmUgR_Dictionary();
   static void vectorlECbmPixelHitmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmPixelHitmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmPixelHitmUgR(Long_t size, void *p);
   static void delete_vectorlECbmPixelHitmUgR(void *p);
   static void deleteArray_vectorlECbmPixelHitmUgR(void *p);
   static void destruct_vectorlECbmPixelHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmPixelHit*>*)
   {
      vector<CbmPixelHit*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmPixelHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmPixelHit*>", -2, "vector", 389,
                  typeid(vector<CbmPixelHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmPixelHitmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmPixelHit*>) );
      instance.SetNew(&new_vectorlECbmPixelHitmUgR);
      instance.SetNewArray(&newArray_vectorlECbmPixelHitmUgR);
      instance.SetDelete(&delete_vectorlECbmPixelHitmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmPixelHitmUgR);
      instance.SetDestructor(&destruct_vectorlECbmPixelHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmPixelHit*> >()));

      ::ROOT::AddClassAlternate("vector<CbmPixelHit*>","std::vector<CbmPixelHit*, std::allocator<CbmPixelHit*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmPixelHit*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmPixelHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmPixelHit*>*)nullptr)->GetClass();
      vectorlECbmPixelHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmPixelHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmPixelHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmPixelHit*> : new vector<CbmPixelHit*>;
   }
   static void *newArray_vectorlECbmPixelHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmPixelHit*>[nElements] : new vector<CbmPixelHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmPixelHitmUgR(void *p) {
      delete ((vector<CbmPixelHit*>*)p);
   }
   static void deleteArray_vectorlECbmPixelHitmUgR(void *p) {
      delete [] ((vector<CbmPixelHit*>*)p);
   }
   static void destruct_vectorlECbmPixelHitmUgR(void *p) {
      typedef vector<CbmPixelHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmPixelHit*>

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

namespace ROOT {
   static TClass *maplEintcOTH1mUgR_Dictionary();
   static void maplEintcOTH1mUgR_TClassManip(TClass*);
   static void *new_maplEintcOTH1mUgR(void *p = nullptr);
   static void *newArray_maplEintcOTH1mUgR(Long_t size, void *p);
   static void delete_maplEintcOTH1mUgR(void *p);
   static void deleteArray_maplEintcOTH1mUgR(void *p);
   static void destruct_maplEintcOTH1mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,TH1*>*)
   {
      map<int,TH1*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,TH1*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,TH1*>", -2, "map", 100,
                  typeid(map<int,TH1*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOTH1mUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,TH1*>) );
      instance.SetNew(&new_maplEintcOTH1mUgR);
      instance.SetNewArray(&newArray_maplEintcOTH1mUgR);
      instance.SetDelete(&delete_maplEintcOTH1mUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOTH1mUgR);
      instance.SetDestructor(&destruct_maplEintcOTH1mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,TH1*> >()));

      ::ROOT::AddClassAlternate("map<int,TH1*>","std::map<int, TH1*, std::less<int>, std::allocator<std::pair<int const, TH1*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,TH1*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOTH1mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,TH1*>*)nullptr)->GetClass();
      maplEintcOTH1mUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOTH1mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOTH1mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TH1*> : new map<int,TH1*>;
   }
   static void *newArray_maplEintcOTH1mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TH1*>[nElements] : new map<int,TH1*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOTH1mUgR(void *p) {
      delete ((map<int,TH1*>*)p);
   }
   static void deleteArray_maplEintcOTH1mUgR(void *p) {
      delete [] ((map<int,TH1*>*)p);
   }
   static void destruct_maplEintcOTH1mUgR(void *p) {
      typedef map<int,TH1*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,TH1*>

namespace ROOT {
   static TClass *listlECbmTofHitmUgR_Dictionary();
   static void listlECbmTofHitmUgR_TClassManip(TClass*);
   static void *new_listlECbmTofHitmUgR(void *p = nullptr);
   static void *newArray_listlECbmTofHitmUgR(Long_t size, void *p);
   static void delete_listlECbmTofHitmUgR(void *p);
   static void deleteArray_listlECbmTofHitmUgR(void *p);
   static void destruct_listlECbmTofHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const list<CbmTofHit*>*)
   {
      list<CbmTofHit*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(list<CbmTofHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("list<CbmTofHit*>", -2, "list", 557,
                  typeid(list<CbmTofHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &listlECbmTofHitmUgR_Dictionary, isa_proxy, 0,
                  sizeof(list<CbmTofHit*>) );
      instance.SetNew(&new_listlECbmTofHitmUgR);
      instance.SetNewArray(&newArray_listlECbmTofHitmUgR);
      instance.SetDelete(&delete_listlECbmTofHitmUgR);
      instance.SetDeleteArray(&deleteArray_listlECbmTofHitmUgR);
      instance.SetDestructor(&destruct_listlECbmTofHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< list<CbmTofHit*> >()));

      ::ROOT::AddClassAlternate("list<CbmTofHit*>","std::__cxx11::list<CbmTofHit*, std::allocator<CbmTofHit*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const list<CbmTofHit*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *listlECbmTofHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const list<CbmTofHit*>*)nullptr)->GetClass();
      listlECbmTofHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void listlECbmTofHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_listlECbmTofHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<CbmTofHit*> : new list<CbmTofHit*>;
   }
   static void *newArray_listlECbmTofHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<CbmTofHit*>[nElements] : new list<CbmTofHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_listlECbmTofHitmUgR(void *p) {
      delete ((list<CbmTofHit*>*)p);
   }
   static void deleteArray_listlECbmTofHitmUgR(void *p) {
      delete [] ((list<CbmTofHit*>*)p);
   }
   static void destruct_listlECbmTofHitmUgR(void *p) {
      typedef list<CbmTofHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class list<CbmTofHit*>

namespace {
  void TriggerDictionaryInitialization_libCbmTofReco_Impl() {
    static const char* headers[] = {
"CbmTofBuildDigiEvents.h",
"CbmTofHitMaker.h",
"CbmTofCosmicClusterizer.h",
"CbmTofEventClusterizer.h",
"CbmTofFindTracks.h",
"CbmTofExtendTracks.h",
"CbmTofSimpClusterizer.h",
"CbmTofTrackFinderNN.h",
"CbmTofTestBeamClusterizer.h",
"CbmTofTrackletTools.h",
"CbmTofCalibrator.h",
"LKFMinuit.h",
"CbmTofUnpackAlgo.h",
"CbmTofUnpackConfig.h",
"CbmTofUnpackMonitor.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/fles/flestools",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/tof/unpack",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/fles/flestools",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/tof/unpack",
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
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/core/eventdisplay",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils/flestools",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/detectors/tof/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmTofReco dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmTofBuildDigiEvents.h")))  CbmTofBuildDigiEvents;
class __attribute__((annotate("$clingAutoload$CbmTofHitMaker.h")))  CbmTofHitMaker;
class __attribute__((annotate("$clingAutoload$CbmTofCosmicClusterizer.h")))  CbmTofCosmicClusterizer;
class __attribute__((annotate("$clingAutoload$CbmTofEventClusterizer.h")))  CbmTofEventClusterizer;
class __attribute__((annotate("$clingAutoload$CbmTofFindTracks.h")))  CbmTofFindTracks;
class __attribute__((annotate("$clingAutoload$CbmTofExtendTracks.h")))  CbmTofExtendTracks;
class __attribute__((annotate("$clingAutoload$CbmTofSimpClusterizer.h")))  CbmTofSimpClusterizer;
class __attribute__((annotate("$clingAutoload$LKFMinuit.h")))  __attribute__((annotate("$clingAutoload$CbmTofTrackFinderNN.h")))  LKFMinuit;
class __attribute__((annotate("$clingAutoload$CbmTofTrackFinderNN.h")))  CbmTofTrackFinderNN;
class __attribute__((annotate("$clingAutoload$CbmTofTestBeamClusterizer.h")))  CbmTofTestBeamClusterizer;
class __attribute__((annotate("$clingAutoload$CbmTofTrackletTools.h")))  CbmTofTrackletTools;
class __attribute__((annotate("$clingAutoload$CbmTofCalibrator.h")))  CbmTofCalibrator;
class __attribute__((annotate("$clingAutoload$CbmTofUnpackMonitor.h")))  __attribute__((annotate("$clingAutoload$CbmTofUnpackAlgo.h")))  CbmTofUnpackMonitor;
class __attribute__((annotate("$clingAutoload$CbmTofUnpackAlgo.h")))  CbmTofUnpackAlgo;
class __attribute__((annotate("$clingAutoload$CbmTofUnpackConfig.h")))  CbmTofUnpackConfig;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmTofReco dictionary payload"

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
#include "CbmTofBuildDigiEvents.h"
#include "CbmTofHitMaker.h"
#include "CbmTofCosmicClusterizer.h"
#include "CbmTofEventClusterizer.h"
#include "CbmTofFindTracks.h"
#include "CbmTofExtendTracks.h"
#include "CbmTofSimpClusterizer.h"
#include "CbmTofTrackFinderNN.h"
#include "CbmTofTestBeamClusterizer.h"
#include "CbmTofTrackletTools.h"
#include "CbmTofCalibrator.h"
#include "LKFMinuit.h"
#include "CbmTofUnpackAlgo.h"
#include "CbmTofUnpackConfig.h"
#include "CbmTofUnpackMonitor.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmTofBuildDigiEvents", payloadCode, "@",
"CbmTofCalibrator", payloadCode, "@",
"CbmTofCosmicClusterizer", payloadCode, "@",
"CbmTofEventClusterizer", payloadCode, "@",
"CbmTofExtendTracks", payloadCode, "@",
"CbmTofFindTracks", payloadCode, "@",
"CbmTofHitMaker", payloadCode, "@",
"CbmTofSimpClusterizer", payloadCode, "@",
"CbmTofTestBeamClusterizer", payloadCode, "@",
"CbmTofTrackFinderNN", payloadCode, "@",
"CbmTofTrackletTools", payloadCode, "@",
"CbmTofUnpackAlgo", payloadCode, "@",
"CbmTofUnpackConfig", payloadCode, "@",
"CbmTofUnpackMonitor", payloadCode, "@",
"LKFMinuit", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmTofReco",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmTofReco_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmTofReco_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmTofReco() {
  TriggerDictionaryInitialization_libCbmTofReco_Impl();
}
