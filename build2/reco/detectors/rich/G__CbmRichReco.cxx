// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmRichReco
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
#include "CbmRichTrainAnnSelect.h"
#include "CbmRichTrainAnnElectrons.h"
#include "CbmRichReconstruction.h"
#include "CbmRichHitProducer.h"
#include "CbmRichMCbmQa.h"
#include "CbmRichMCbmQaReal.h"
#include "CbmRichMCbmQaRichOnly.h"
#include "CbmRichMCbmHitProducer.h"
#include "CbmRichMCbmAerogelAna.h"
#include "CbmRichMCbmToTShifter.h"
#include "CbmRichMCbmSEDisplay.h"
#include "CbmRichRingFitterQa.h"
#include "CbmRichGeoOpt.h"
#include "CbmRichRecoTbQa.h"
#include "CbmRichGeoTest.h"
#include "CbmRichUrqmdTest.h"
#include "CbmRichGeoTestOpt.h"
#include "CbmRichRecoQa.h"
#include "CbmL1RichRingQa.h"
#include "CbmRichUnpackAlgo.h"
#include "CbmRichUnpackAlgo2022.h"
#include "CbmRichUnpackAlgoBase.h"
#include "CbmRichUnpackMonitor.h"
#include "CbmRichUnpackConfig.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmRichTrainAnnSelect(void *p = nullptr);
   static void *newArray_CbmRichTrainAnnSelect(Long_t size, void *p);
   static void delete_CbmRichTrainAnnSelect(void *p);
   static void deleteArray_CbmRichTrainAnnSelect(void *p);
   static void destruct_CbmRichTrainAnnSelect(void *p);
   static void streamer_CbmRichTrainAnnSelect(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichTrainAnnSelect*)
   {
      ::CbmRichTrainAnnSelect *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichTrainAnnSelect >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichTrainAnnSelect", ::CbmRichTrainAnnSelect::Class_Version(), "CbmRichTrainAnnSelect.h", 61,
                  typeid(::CbmRichTrainAnnSelect), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichTrainAnnSelect::Dictionary, isa_proxy, 16,
                  sizeof(::CbmRichTrainAnnSelect) );
      instance.SetNew(&new_CbmRichTrainAnnSelect);
      instance.SetNewArray(&newArray_CbmRichTrainAnnSelect);
      instance.SetDelete(&delete_CbmRichTrainAnnSelect);
      instance.SetDeleteArray(&deleteArray_CbmRichTrainAnnSelect);
      instance.SetDestructor(&destruct_CbmRichTrainAnnSelect);
      instance.SetStreamerFunc(&streamer_CbmRichTrainAnnSelect);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichTrainAnnSelect*)
   {
      return GenerateInitInstanceLocal((::CbmRichTrainAnnSelect*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichTrainAnnSelect*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichTrainAnnElectrons(void *p = nullptr);
   static void *newArray_CbmRichTrainAnnElectrons(Long_t size, void *p);
   static void delete_CbmRichTrainAnnElectrons(void *p);
   static void deleteArray_CbmRichTrainAnnElectrons(void *p);
   static void destruct_CbmRichTrainAnnElectrons(void *p);
   static void streamer_CbmRichTrainAnnElectrons(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichTrainAnnElectrons*)
   {
      ::CbmRichTrainAnnElectrons *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichTrainAnnElectrons >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichTrainAnnElectrons", ::CbmRichTrainAnnElectrons::Class_Version(), "CbmRichTrainAnnElectrons.h", 63,
                  typeid(::CbmRichTrainAnnElectrons), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichTrainAnnElectrons::Dictionary, isa_proxy, 16,
                  sizeof(::CbmRichTrainAnnElectrons) );
      instance.SetNew(&new_CbmRichTrainAnnElectrons);
      instance.SetNewArray(&newArray_CbmRichTrainAnnElectrons);
      instance.SetDelete(&delete_CbmRichTrainAnnElectrons);
      instance.SetDeleteArray(&deleteArray_CbmRichTrainAnnElectrons);
      instance.SetDestructor(&destruct_CbmRichTrainAnnElectrons);
      instance.SetStreamerFunc(&streamer_CbmRichTrainAnnElectrons);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichTrainAnnElectrons*)
   {
      return GenerateInitInstanceLocal((::CbmRichTrainAnnElectrons*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichTrainAnnElectrons*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichReconstruction(void *p = nullptr);
   static void *newArray_CbmRichReconstruction(Long_t size, void *p);
   static void delete_CbmRichReconstruction(void *p);
   static void deleteArray_CbmRichReconstruction(void *p);
   static void destruct_CbmRichReconstruction(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichReconstruction*)
   {
      ::CbmRichReconstruction *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichReconstruction >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichReconstruction", ::CbmRichReconstruction::Class_Version(), "CbmRichReconstruction.h", 39,
                  typeid(::CbmRichReconstruction), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichReconstruction::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichReconstruction) );
      instance.SetNew(&new_CbmRichReconstruction);
      instance.SetNewArray(&newArray_CbmRichReconstruction);
      instance.SetDelete(&delete_CbmRichReconstruction);
      instance.SetDeleteArray(&deleteArray_CbmRichReconstruction);
      instance.SetDestructor(&destruct_CbmRichReconstruction);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichReconstruction*)
   {
      return GenerateInitInstanceLocal((::CbmRichReconstruction*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichReconstruction*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichHitProducer(void *p = nullptr);
   static void *newArray_CbmRichHitProducer(Long_t size, void *p);
   static void delete_CbmRichHitProducer(void *p);
   static void deleteArray_CbmRichHitProducer(void *p);
   static void destruct_CbmRichHitProducer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichHitProducer*)
   {
      ::CbmRichHitProducer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichHitProducer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichHitProducer", ::CbmRichHitProducer::Class_Version(), "CbmRichHitProducer.h", 35,
                  typeid(::CbmRichHitProducer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichHitProducer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichHitProducer) );
      instance.SetNew(&new_CbmRichHitProducer);
      instance.SetNewArray(&newArray_CbmRichHitProducer);
      instance.SetDelete(&delete_CbmRichHitProducer);
      instance.SetDeleteArray(&deleteArray_CbmRichHitProducer);
      instance.SetDestructor(&destruct_CbmRichHitProducer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichHitProducer*)
   {
      return GenerateInitInstanceLocal((::CbmRichHitProducer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichHitProducer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichMCbmQa(void *p = nullptr);
   static void *newArray_CbmRichMCbmQa(Long_t size, void *p);
   static void delete_CbmRichMCbmQa(void *p);
   static void deleteArray_CbmRichMCbmQa(void *p);
   static void destruct_CbmRichMCbmQa(void *p);
   static void streamer_CbmRichMCbmQa(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichMCbmQa*)
   {
      ::CbmRichMCbmQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichMCbmQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichMCbmQa", ::CbmRichMCbmQa::Class_Version(), "CbmRichMCbmQa.h", 20,
                  typeid(::CbmRichMCbmQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichMCbmQa::Dictionary, isa_proxy, 16,
                  sizeof(::CbmRichMCbmQa) );
      instance.SetNew(&new_CbmRichMCbmQa);
      instance.SetNewArray(&newArray_CbmRichMCbmQa);
      instance.SetDelete(&delete_CbmRichMCbmQa);
      instance.SetDeleteArray(&deleteArray_CbmRichMCbmQa);
      instance.SetDestructor(&destruct_CbmRichMCbmQa);
      instance.SetStreamerFunc(&streamer_CbmRichMCbmQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichMCbmQa*)
   {
      return GenerateInitInstanceLocal((::CbmRichMCbmQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichMCbmQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichMCbmQaReal(void *p = nullptr);
   static void *newArray_CbmRichMCbmQaReal(Long_t size, void *p);
   static void delete_CbmRichMCbmQaReal(void *p);
   static void deleteArray_CbmRichMCbmQaReal(void *p);
   static void destruct_CbmRichMCbmQaReal(void *p);
   static void streamer_CbmRichMCbmQaReal(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichMCbmQaReal*)
   {
      ::CbmRichMCbmQaReal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichMCbmQaReal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichMCbmQaReal", ::CbmRichMCbmQaReal::Class_Version(), "CbmRichMCbmQaReal.h", 30,
                  typeid(::CbmRichMCbmQaReal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichMCbmQaReal::Dictionary, isa_proxy, 16,
                  sizeof(::CbmRichMCbmQaReal) );
      instance.SetNew(&new_CbmRichMCbmQaReal);
      instance.SetNewArray(&newArray_CbmRichMCbmQaReal);
      instance.SetDelete(&delete_CbmRichMCbmQaReal);
      instance.SetDeleteArray(&deleteArray_CbmRichMCbmQaReal);
      instance.SetDestructor(&destruct_CbmRichMCbmQaReal);
      instance.SetStreamerFunc(&streamer_CbmRichMCbmQaReal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichMCbmQaReal*)
   {
      return GenerateInitInstanceLocal((::CbmRichMCbmQaReal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichMCbmQaReal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichMCbmQaRichOnly(void *p = nullptr);
   static void *newArray_CbmRichMCbmQaRichOnly(Long_t size, void *p);
   static void delete_CbmRichMCbmQaRichOnly(void *p);
   static void deleteArray_CbmRichMCbmQaRichOnly(void *p);
   static void destruct_CbmRichMCbmQaRichOnly(void *p);
   static void streamer_CbmRichMCbmQaRichOnly(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichMCbmQaRichOnly*)
   {
      ::CbmRichMCbmQaRichOnly *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichMCbmQaRichOnly >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichMCbmQaRichOnly", ::CbmRichMCbmQaRichOnly::Class_Version(), "CbmRichMCbmQaRichOnly.h", 30,
                  typeid(::CbmRichMCbmQaRichOnly), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichMCbmQaRichOnly::Dictionary, isa_proxy, 16,
                  sizeof(::CbmRichMCbmQaRichOnly) );
      instance.SetNew(&new_CbmRichMCbmQaRichOnly);
      instance.SetNewArray(&newArray_CbmRichMCbmQaRichOnly);
      instance.SetDelete(&delete_CbmRichMCbmQaRichOnly);
      instance.SetDeleteArray(&deleteArray_CbmRichMCbmQaRichOnly);
      instance.SetDestructor(&destruct_CbmRichMCbmQaRichOnly);
      instance.SetStreamerFunc(&streamer_CbmRichMCbmQaRichOnly);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichMCbmQaRichOnly*)
   {
      return GenerateInitInstanceLocal((::CbmRichMCbmQaRichOnly*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichMCbmQaRichOnly*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichMCbmHitProducer(void *p = nullptr);
   static void *newArray_CbmRichMCbmHitProducer(Long_t size, void *p);
   static void delete_CbmRichMCbmHitProducer(void *p);
   static void deleteArray_CbmRichMCbmHitProducer(void *p);
   static void destruct_CbmRichMCbmHitProducer(void *p);
   static void streamer_CbmRichMCbmHitProducer(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichMCbmHitProducer*)
   {
      ::CbmRichMCbmHitProducer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichMCbmHitProducer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichMCbmHitProducer", ::CbmRichMCbmHitProducer::Class_Version(), "CbmRichMCbmHitProducer.h", 27,
                  typeid(::CbmRichMCbmHitProducer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichMCbmHitProducer::Dictionary, isa_proxy, 16,
                  sizeof(::CbmRichMCbmHitProducer) );
      instance.SetNew(&new_CbmRichMCbmHitProducer);
      instance.SetNewArray(&newArray_CbmRichMCbmHitProducer);
      instance.SetDelete(&delete_CbmRichMCbmHitProducer);
      instance.SetDeleteArray(&deleteArray_CbmRichMCbmHitProducer);
      instance.SetDestructor(&destruct_CbmRichMCbmHitProducer);
      instance.SetStreamerFunc(&streamer_CbmRichMCbmHitProducer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichMCbmHitProducer*)
   {
      return GenerateInitInstanceLocal((::CbmRichMCbmHitProducer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichMCbmHitProducer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichMCbmAerogelAna(void *p = nullptr);
   static void *newArray_CbmRichMCbmAerogelAna(Long_t size, void *p);
   static void delete_CbmRichMCbmAerogelAna(void *p);
   static void deleteArray_CbmRichMCbmAerogelAna(void *p);
   static void destruct_CbmRichMCbmAerogelAna(void *p);
   static void streamer_CbmRichMCbmAerogelAna(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichMCbmAerogelAna*)
   {
      ::CbmRichMCbmAerogelAna *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichMCbmAerogelAna >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichMCbmAerogelAna", ::CbmRichMCbmAerogelAna::Class_Version(), "CbmRichMCbmAerogelAna.h", 26,
                  typeid(::CbmRichMCbmAerogelAna), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichMCbmAerogelAna::Dictionary, isa_proxy, 16,
                  sizeof(::CbmRichMCbmAerogelAna) );
      instance.SetNew(&new_CbmRichMCbmAerogelAna);
      instance.SetNewArray(&newArray_CbmRichMCbmAerogelAna);
      instance.SetDelete(&delete_CbmRichMCbmAerogelAna);
      instance.SetDeleteArray(&deleteArray_CbmRichMCbmAerogelAna);
      instance.SetDestructor(&destruct_CbmRichMCbmAerogelAna);
      instance.SetStreamerFunc(&streamer_CbmRichMCbmAerogelAna);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichMCbmAerogelAna*)
   {
      return GenerateInitInstanceLocal((::CbmRichMCbmAerogelAna*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichMCbmAerogelAna*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichMCbmToTShifter(void *p = nullptr);
   static void *newArray_CbmRichMCbmToTShifter(Long_t size, void *p);
   static void delete_CbmRichMCbmToTShifter(void *p);
   static void deleteArray_CbmRichMCbmToTShifter(void *p);
   static void destruct_CbmRichMCbmToTShifter(void *p);
   static void streamer_CbmRichMCbmToTShifter(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichMCbmToTShifter*)
   {
      ::CbmRichMCbmToTShifter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichMCbmToTShifter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichMCbmToTShifter", ::CbmRichMCbmToTShifter::Class_Version(), "CbmRichMCbmToTShifter.h", 23,
                  typeid(::CbmRichMCbmToTShifter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichMCbmToTShifter::Dictionary, isa_proxy, 16,
                  sizeof(::CbmRichMCbmToTShifter) );
      instance.SetNew(&new_CbmRichMCbmToTShifter);
      instance.SetNewArray(&newArray_CbmRichMCbmToTShifter);
      instance.SetDelete(&delete_CbmRichMCbmToTShifter);
      instance.SetDeleteArray(&deleteArray_CbmRichMCbmToTShifter);
      instance.SetDestructor(&destruct_CbmRichMCbmToTShifter);
      instance.SetStreamerFunc(&streamer_CbmRichMCbmToTShifter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichMCbmToTShifter*)
   {
      return GenerateInitInstanceLocal((::CbmRichMCbmToTShifter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichMCbmToTShifter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichMCbmSEDisplay(void *p = nullptr);
   static void *newArray_CbmRichMCbmSEDisplay(Long_t size, void *p);
   static void delete_CbmRichMCbmSEDisplay(void *p);
   static void deleteArray_CbmRichMCbmSEDisplay(void *p);
   static void destruct_CbmRichMCbmSEDisplay(void *p);
   static void streamer_CbmRichMCbmSEDisplay(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichMCbmSEDisplay*)
   {
      ::CbmRichMCbmSEDisplay *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichMCbmSEDisplay >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichMCbmSEDisplay", ::CbmRichMCbmSEDisplay::Class_Version(), "CbmRichMCbmSEDisplay.h", 21,
                  typeid(::CbmRichMCbmSEDisplay), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichMCbmSEDisplay::Dictionary, isa_proxy, 16,
                  sizeof(::CbmRichMCbmSEDisplay) );
      instance.SetNew(&new_CbmRichMCbmSEDisplay);
      instance.SetNewArray(&newArray_CbmRichMCbmSEDisplay);
      instance.SetDelete(&delete_CbmRichMCbmSEDisplay);
      instance.SetDeleteArray(&deleteArray_CbmRichMCbmSEDisplay);
      instance.SetDestructor(&destruct_CbmRichMCbmSEDisplay);
      instance.SetStreamerFunc(&streamer_CbmRichMCbmSEDisplay);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichMCbmSEDisplay*)
   {
      return GenerateInitInstanceLocal((::CbmRichMCbmSEDisplay*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichMCbmSEDisplay*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichRingFitterQa(void *p = nullptr);
   static void *newArray_CbmRichRingFitterQa(Long_t size, void *p);
   static void delete_CbmRichRingFitterQa(void *p);
   static void deleteArray_CbmRichRingFitterQa(void *p);
   static void destruct_CbmRichRingFitterQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichRingFitterQa*)
   {
      ::CbmRichRingFitterQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichRingFitterQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichRingFitterQa", ::CbmRichRingFitterQa::Class_Version(), "CbmRichRingFitterQa.h", 35,
                  typeid(::CbmRichRingFitterQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichRingFitterQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichRingFitterQa) );
      instance.SetNew(&new_CbmRichRingFitterQa);
      instance.SetNewArray(&newArray_CbmRichRingFitterQa);
      instance.SetDelete(&delete_CbmRichRingFitterQa);
      instance.SetDeleteArray(&deleteArray_CbmRichRingFitterQa);
      instance.SetDestructor(&destruct_CbmRichRingFitterQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichRingFitterQa*)
   {
      return GenerateInitInstanceLocal((::CbmRichRingFitterQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichRingFitterQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichGeoOpt(void *p = nullptr);
   static void *newArray_CbmRichGeoOpt(Long_t size, void *p);
   static void delete_CbmRichGeoOpt(void *p);
   static void deleteArray_CbmRichGeoOpt(void *p);
   static void destruct_CbmRichGeoOpt(void *p);
   static void streamer_CbmRichGeoOpt(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichGeoOpt*)
   {
      ::CbmRichGeoOpt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichGeoOpt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichGeoOpt", ::CbmRichGeoOpt::Class_Version(), "CbmRichGeoOpt.h", 51,
                  typeid(::CbmRichGeoOpt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichGeoOpt::Dictionary, isa_proxy, 16,
                  sizeof(::CbmRichGeoOpt) );
      instance.SetNew(&new_CbmRichGeoOpt);
      instance.SetNewArray(&newArray_CbmRichGeoOpt);
      instance.SetDelete(&delete_CbmRichGeoOpt);
      instance.SetDeleteArray(&deleteArray_CbmRichGeoOpt);
      instance.SetDestructor(&destruct_CbmRichGeoOpt);
      instance.SetStreamerFunc(&streamer_CbmRichGeoOpt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichGeoOpt*)
   {
      return GenerateInitInstanceLocal((::CbmRichGeoOpt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichGeoOpt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichRecoTbQa(void *p = nullptr);
   static void *newArray_CbmRichRecoTbQa(Long_t size, void *p);
   static void delete_CbmRichRecoTbQa(void *p);
   static void deleteArray_CbmRichRecoTbQa(void *p);
   static void destruct_CbmRichRecoTbQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichRecoTbQa*)
   {
      ::CbmRichRecoTbQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichRecoTbQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichRecoTbQa", ::CbmRichRecoTbQa::Class_Version(), "CbmRichRecoTbQa.h", 22,
                  typeid(::CbmRichRecoTbQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichRecoTbQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichRecoTbQa) );
      instance.SetNew(&new_CbmRichRecoTbQa);
      instance.SetNewArray(&newArray_CbmRichRecoTbQa);
      instance.SetDelete(&delete_CbmRichRecoTbQa);
      instance.SetDeleteArray(&deleteArray_CbmRichRecoTbQa);
      instance.SetDestructor(&destruct_CbmRichRecoTbQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichRecoTbQa*)
   {
      return GenerateInitInstanceLocal((::CbmRichRecoTbQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichRecoTbQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichGeoTest(void *p = nullptr);
   static void *newArray_CbmRichGeoTest(Long_t size, void *p);
   static void delete_CbmRichGeoTest(void *p);
   static void deleteArray_CbmRichGeoTest(void *p);
   static void destruct_CbmRichGeoTest(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichGeoTest*)
   {
      ::CbmRichGeoTest *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichGeoTest >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichGeoTest", ::CbmRichGeoTest::Class_Version(), "CbmRichGeoTest.h", 46,
                  typeid(::CbmRichGeoTest), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichGeoTest::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichGeoTest) );
      instance.SetNew(&new_CbmRichGeoTest);
      instance.SetNewArray(&newArray_CbmRichGeoTest);
      instance.SetDelete(&delete_CbmRichGeoTest);
      instance.SetDeleteArray(&deleteArray_CbmRichGeoTest);
      instance.SetDestructor(&destruct_CbmRichGeoTest);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichGeoTest*)
   {
      return GenerateInitInstanceLocal((::CbmRichGeoTest*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichGeoTest*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichUrqmdTest(void *p = nullptr);
   static void *newArray_CbmRichUrqmdTest(Long_t size, void *p);
   static void delete_CbmRichUrqmdTest(void *p);
   static void deleteArray_CbmRichUrqmdTest(void *p);
   static void destruct_CbmRichUrqmdTest(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichUrqmdTest*)
   {
      ::CbmRichUrqmdTest *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichUrqmdTest >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichUrqmdTest", ::CbmRichUrqmdTest::Class_Version(), "CbmRichUrqmdTest.h", 21,
                  typeid(::CbmRichUrqmdTest), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichUrqmdTest::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichUrqmdTest) );
      instance.SetNew(&new_CbmRichUrqmdTest);
      instance.SetNewArray(&newArray_CbmRichUrqmdTest);
      instance.SetDelete(&delete_CbmRichUrqmdTest);
      instance.SetDeleteArray(&deleteArray_CbmRichUrqmdTest);
      instance.SetDestructor(&destruct_CbmRichUrqmdTest);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichUrqmdTest*)
   {
      return GenerateInitInstanceLocal((::CbmRichUrqmdTest*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichUrqmdTest*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichGeoTestOpt(void *p = nullptr);
   static void *newArray_CbmRichGeoTestOpt(Long_t size, void *p);
   static void delete_CbmRichGeoTestOpt(void *p);
   static void deleteArray_CbmRichGeoTestOpt(void *p);
   static void destruct_CbmRichGeoTestOpt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichGeoTestOpt*)
   {
      ::CbmRichGeoTestOpt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichGeoTestOpt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichGeoTestOpt", ::CbmRichGeoTestOpt::Class_Version(), "CbmRichGeoTestOpt.h", 40,
                  typeid(::CbmRichGeoTestOpt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichGeoTestOpt::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichGeoTestOpt) );
      instance.SetNew(&new_CbmRichGeoTestOpt);
      instance.SetNewArray(&newArray_CbmRichGeoTestOpt);
      instance.SetDelete(&delete_CbmRichGeoTestOpt);
      instance.SetDeleteArray(&deleteArray_CbmRichGeoTestOpt);
      instance.SetDestructor(&destruct_CbmRichGeoTestOpt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichGeoTestOpt*)
   {
      return GenerateInitInstanceLocal((::CbmRichGeoTestOpt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichGeoTestOpt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichRecoQa(void *p = nullptr);
   static void *newArray_CbmRichRecoQa(Long_t size, void *p);
   static void delete_CbmRichRecoQa(void *p);
   static void deleteArray_CbmRichRecoQa(void *p);
   static void destruct_CbmRichRecoQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichRecoQa*)
   {
      ::CbmRichRecoQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichRecoQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichRecoQa", ::CbmRichRecoQa::Class_Version(), "CbmRichRecoQa.h", 25,
                  typeid(::CbmRichRecoQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichRecoQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichRecoQa) );
      instance.SetNew(&new_CbmRichRecoQa);
      instance.SetNewArray(&newArray_CbmRichRecoQa);
      instance.SetDelete(&delete_CbmRichRecoQa);
      instance.SetDeleteArray(&deleteArray_CbmRichRecoQa);
      instance.SetDestructor(&destruct_CbmRichRecoQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichRecoQa*)
   {
      return GenerateInitInstanceLocal((::CbmRichRecoQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichRecoQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmL1RichRingQa(void *p = nullptr);
   static void *newArray_CbmL1RichRingQa(Long_t size, void *p);
   static void delete_CbmL1RichRingQa(void *p);
   static void deleteArray_CbmL1RichRingQa(void *p);
   static void destruct_CbmL1RichRingQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmL1RichRingQa*)
   {
      ::CbmL1RichRingQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmL1RichRingQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmL1RichRingQa", ::CbmL1RichRingQa::Class_Version(), "CbmL1RichRingQa.h", 31,
                  typeid(::CbmL1RichRingQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmL1RichRingQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmL1RichRingQa) );
      instance.SetNew(&new_CbmL1RichRingQa);
      instance.SetNewArray(&newArray_CbmL1RichRingQa);
      instance.SetDelete(&delete_CbmL1RichRingQa);
      instance.SetDeleteArray(&deleteArray_CbmL1RichRingQa);
      instance.SetDestructor(&destruct_CbmL1RichRingQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmL1RichRingQa*)
   {
      return GenerateInitInstanceLocal((::CbmL1RichRingQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmL1RichRingQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichUnpackMonitor(void *p = nullptr);
   static void *newArray_CbmRichUnpackMonitor(Long_t size, void *p);
   static void delete_CbmRichUnpackMonitor(void *p);
   static void deleteArray_CbmRichUnpackMonitor(void *p);
   static void destruct_CbmRichUnpackMonitor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichUnpackMonitor*)
   {
      ::CbmRichUnpackMonitor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichUnpackMonitor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichUnpackMonitor", ::CbmRichUnpackMonitor::Class_Version(), "CbmRichUnpackMonitor.h", 30,
                  typeid(::CbmRichUnpackMonitor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichUnpackMonitor::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichUnpackMonitor) );
      instance.SetNew(&new_CbmRichUnpackMonitor);
      instance.SetNewArray(&newArray_CbmRichUnpackMonitor);
      instance.SetDelete(&delete_CbmRichUnpackMonitor);
      instance.SetDeleteArray(&deleteArray_CbmRichUnpackMonitor);
      instance.SetDestructor(&destruct_CbmRichUnpackMonitor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichUnpackMonitor*)
   {
      return GenerateInitInstanceLocal((::CbmRichUnpackMonitor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichUnpackMonitor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmRichUnpackAlgoBase(void *p);
   static void deleteArray_CbmRichUnpackAlgoBase(void *p);
   static void destruct_CbmRichUnpackAlgoBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichUnpackAlgoBase*)
   {
      ::CbmRichUnpackAlgoBase *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichUnpackAlgoBase >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichUnpackAlgoBase", ::CbmRichUnpackAlgoBase::Class_Version(), "CbmRichUnpackAlgoBase.h", 197,
                  typeid(::CbmRichUnpackAlgoBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichUnpackAlgoBase::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichUnpackAlgoBase) );
      instance.SetDelete(&delete_CbmRichUnpackAlgoBase);
      instance.SetDeleteArray(&deleteArray_CbmRichUnpackAlgoBase);
      instance.SetDestructor(&destruct_CbmRichUnpackAlgoBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichUnpackAlgoBase*)
   {
      return GenerateInitInstanceLocal((::CbmRichUnpackAlgoBase*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichUnpackAlgoBase*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichUnpackAlgo(void *p = nullptr);
   static void *newArray_CbmRichUnpackAlgo(Long_t size, void *p);
   static void delete_CbmRichUnpackAlgo(void *p);
   static void deleteArray_CbmRichUnpackAlgo(void *p);
   static void destruct_CbmRichUnpackAlgo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichUnpackAlgo*)
   {
      ::CbmRichUnpackAlgo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichUnpackAlgo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichUnpackAlgo", ::CbmRichUnpackAlgo::Class_Version(), "CbmRichUnpackAlgo.h", 41,
                  typeid(::CbmRichUnpackAlgo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichUnpackAlgo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichUnpackAlgo) );
      instance.SetNew(&new_CbmRichUnpackAlgo);
      instance.SetNewArray(&newArray_CbmRichUnpackAlgo);
      instance.SetDelete(&delete_CbmRichUnpackAlgo);
      instance.SetDeleteArray(&deleteArray_CbmRichUnpackAlgo);
      instance.SetDestructor(&destruct_CbmRichUnpackAlgo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichUnpackAlgo*)
   {
      return GenerateInitInstanceLocal((::CbmRichUnpackAlgo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichUnpackAlgo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichUnpackAlgo2022(void *p = nullptr);
   static void *newArray_CbmRichUnpackAlgo2022(Long_t size, void *p);
   static void delete_CbmRichUnpackAlgo2022(void *p);
   static void deleteArray_CbmRichUnpackAlgo2022(void *p);
   static void destruct_CbmRichUnpackAlgo2022(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichUnpackAlgo2022*)
   {
      ::CbmRichUnpackAlgo2022 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichUnpackAlgo2022 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichUnpackAlgo2022", ::CbmRichUnpackAlgo2022::Class_Version(), "CbmRichUnpackAlgo2022.h", 40,
                  typeid(::CbmRichUnpackAlgo2022), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichUnpackAlgo2022::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichUnpackAlgo2022) );
      instance.SetNew(&new_CbmRichUnpackAlgo2022);
      instance.SetNewArray(&newArray_CbmRichUnpackAlgo2022);
      instance.SetDelete(&delete_CbmRichUnpackAlgo2022);
      instance.SetDeleteArray(&deleteArray_CbmRichUnpackAlgo2022);
      instance.SetDestructor(&destruct_CbmRichUnpackAlgo2022);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichUnpackAlgo2022*)
   {
      return GenerateInitInstanceLocal((::CbmRichUnpackAlgo2022*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichUnpackAlgo2022*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmRichUnpackConfig(void *p);
   static void deleteArray_CbmRichUnpackConfig(void *p);
   static void destruct_CbmRichUnpackConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichUnpackConfig*)
   {
      ::CbmRichUnpackConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichUnpackConfig >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichUnpackConfig", ::CbmRichUnpackConfig::Class_Version(), "CbmRichUnpackConfig.h", 40,
                  typeid(::CbmRichUnpackConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichUnpackConfig::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichUnpackConfig) );
      instance.SetDelete(&delete_CbmRichUnpackConfig);
      instance.SetDeleteArray(&deleteArray_CbmRichUnpackConfig);
      instance.SetDestructor(&destruct_CbmRichUnpackConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichUnpackConfig*)
   {
      return GenerateInitInstanceLocal((::CbmRichUnpackConfig*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichUnpackConfig*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmRichTrainAnnSelect::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichTrainAnnSelect::Class_Name()
{
   return "CbmRichTrainAnnSelect";
}

//______________________________________________________________________________
const char *CbmRichTrainAnnSelect::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichTrainAnnSelect*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichTrainAnnSelect::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichTrainAnnSelect*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichTrainAnnSelect::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichTrainAnnSelect*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichTrainAnnSelect::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichTrainAnnSelect*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichTrainAnnElectrons::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichTrainAnnElectrons::Class_Name()
{
   return "CbmRichTrainAnnElectrons";
}

//______________________________________________________________________________
const char *CbmRichTrainAnnElectrons::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichTrainAnnElectrons*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichTrainAnnElectrons::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichTrainAnnElectrons*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichTrainAnnElectrons::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichTrainAnnElectrons*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichTrainAnnElectrons::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichTrainAnnElectrons*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichReconstruction::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichReconstruction::Class_Name()
{
   return "CbmRichReconstruction";
}

//______________________________________________________________________________
const char *CbmRichReconstruction::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichReconstruction*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichReconstruction::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichReconstruction*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichReconstruction::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichReconstruction*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichReconstruction::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichReconstruction*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichHitProducer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichHitProducer::Class_Name()
{
   return "CbmRichHitProducer";
}

//______________________________________________________________________________
const char *CbmRichHitProducer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichHitProducer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichHitProducer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichHitProducer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichHitProducer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichHitProducer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichHitProducer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichHitProducer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichMCbmQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichMCbmQa::Class_Name()
{
   return "CbmRichMCbmQa";
}

//______________________________________________________________________________
const char *CbmRichMCbmQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichMCbmQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichMCbmQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichMCbmQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichMCbmQaReal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichMCbmQaReal::Class_Name()
{
   return "CbmRichMCbmQaReal";
}

//______________________________________________________________________________
const char *CbmRichMCbmQaReal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmQaReal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichMCbmQaReal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmQaReal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichMCbmQaReal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmQaReal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichMCbmQaReal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmQaReal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichMCbmQaRichOnly::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichMCbmQaRichOnly::Class_Name()
{
   return "CbmRichMCbmQaRichOnly";
}

//______________________________________________________________________________
const char *CbmRichMCbmQaRichOnly::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmQaRichOnly*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichMCbmQaRichOnly::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmQaRichOnly*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichMCbmQaRichOnly::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmQaRichOnly*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichMCbmQaRichOnly::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmQaRichOnly*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichMCbmHitProducer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichMCbmHitProducer::Class_Name()
{
   return "CbmRichMCbmHitProducer";
}

//______________________________________________________________________________
const char *CbmRichMCbmHitProducer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmHitProducer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichMCbmHitProducer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmHitProducer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichMCbmHitProducer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmHitProducer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichMCbmHitProducer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmHitProducer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichMCbmAerogelAna::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichMCbmAerogelAna::Class_Name()
{
   return "CbmRichMCbmAerogelAna";
}

//______________________________________________________________________________
const char *CbmRichMCbmAerogelAna::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmAerogelAna*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichMCbmAerogelAna::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmAerogelAna*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichMCbmAerogelAna::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmAerogelAna*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichMCbmAerogelAna::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmAerogelAna*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichMCbmToTShifter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichMCbmToTShifter::Class_Name()
{
   return "CbmRichMCbmToTShifter";
}

//______________________________________________________________________________
const char *CbmRichMCbmToTShifter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmToTShifter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichMCbmToTShifter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmToTShifter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichMCbmToTShifter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmToTShifter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichMCbmToTShifter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmToTShifter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichMCbmSEDisplay::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichMCbmSEDisplay::Class_Name()
{
   return "CbmRichMCbmSEDisplay";
}

//______________________________________________________________________________
const char *CbmRichMCbmSEDisplay::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmSEDisplay*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichMCbmSEDisplay::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmSEDisplay*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichMCbmSEDisplay::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmSEDisplay*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichMCbmSEDisplay::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMCbmSEDisplay*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichRingFitterQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichRingFitterQa::Class_Name()
{
   return "CbmRichRingFitterQa";
}

//______________________________________________________________________________
const char *CbmRichRingFitterQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRingFitterQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichRingFitterQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRingFitterQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichRingFitterQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRingFitterQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichRingFitterQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRingFitterQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichGeoOpt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichGeoOpt::Class_Name()
{
   return "CbmRichGeoOpt";
}

//______________________________________________________________________________
const char *CbmRichGeoOpt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichGeoOpt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichGeoOpt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichGeoOpt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichGeoOpt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichGeoOpt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichGeoOpt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichGeoOpt*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichRecoTbQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichRecoTbQa::Class_Name()
{
   return "CbmRichRecoTbQa";
}

//______________________________________________________________________________
const char *CbmRichRecoTbQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRecoTbQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichRecoTbQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRecoTbQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichRecoTbQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRecoTbQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichRecoTbQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRecoTbQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichGeoTest::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichGeoTest::Class_Name()
{
   return "CbmRichGeoTest";
}

//______________________________________________________________________________
const char *CbmRichGeoTest::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichGeoTest*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichGeoTest::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichGeoTest*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichGeoTest::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichGeoTest*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichGeoTest::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichGeoTest*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichUrqmdTest::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichUrqmdTest::Class_Name()
{
   return "CbmRichUrqmdTest";
}

//______________________________________________________________________________
const char *CbmRichUrqmdTest::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUrqmdTest*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichUrqmdTest::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUrqmdTest*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichUrqmdTest::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUrqmdTest*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichUrqmdTest::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUrqmdTest*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichGeoTestOpt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichGeoTestOpt::Class_Name()
{
   return "CbmRichGeoTestOpt";
}

//______________________________________________________________________________
const char *CbmRichGeoTestOpt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichGeoTestOpt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichGeoTestOpt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichGeoTestOpt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichGeoTestOpt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichGeoTestOpt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichGeoTestOpt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichGeoTestOpt*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichRecoQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichRecoQa::Class_Name()
{
   return "CbmRichRecoQa";
}

//______________________________________________________________________________
const char *CbmRichRecoQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRecoQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichRecoQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRecoQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichRecoQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRecoQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichRecoQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRecoQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmL1RichRingQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmL1RichRingQa::Class_Name()
{
   return "CbmL1RichRingQa";
}

//______________________________________________________________________________
const char *CbmL1RichRingQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1RichRingQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmL1RichRingQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmL1RichRingQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmL1RichRingQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1RichRingQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmL1RichRingQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmL1RichRingQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichUnpackMonitor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichUnpackMonitor::Class_Name()
{
   return "CbmRichUnpackMonitor";
}

//______________________________________________________________________________
const char *CbmRichUnpackMonitor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackMonitor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichUnpackMonitor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackMonitor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichUnpackMonitor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackMonitor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichUnpackMonitor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackMonitor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichUnpackAlgoBase::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichUnpackAlgoBase::Class_Name()
{
   return "CbmRichUnpackAlgoBase";
}

//______________________________________________________________________________
const char *CbmRichUnpackAlgoBase::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackAlgoBase*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichUnpackAlgoBase::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackAlgoBase*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichUnpackAlgoBase::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackAlgoBase*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichUnpackAlgoBase::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackAlgoBase*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichUnpackAlgo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichUnpackAlgo::Class_Name()
{
   return "CbmRichUnpackAlgo";
}

//______________________________________________________________________________
const char *CbmRichUnpackAlgo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackAlgo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichUnpackAlgo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackAlgo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichUnpackAlgo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackAlgo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichUnpackAlgo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackAlgo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichUnpackAlgo2022::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichUnpackAlgo2022::Class_Name()
{
   return "CbmRichUnpackAlgo2022";
}

//______________________________________________________________________________
const char *CbmRichUnpackAlgo2022::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackAlgo2022*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichUnpackAlgo2022::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackAlgo2022*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichUnpackAlgo2022::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackAlgo2022*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichUnpackAlgo2022::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackAlgo2022*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichUnpackConfig::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichUnpackConfig::Class_Name()
{
   return "CbmRichUnpackConfig";
}

//______________________________________________________________________________
const char *CbmRichUnpackConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackConfig*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichUnpackConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackConfig*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichUnpackConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackConfig*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichUnpackConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichUnpackConfig*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmRichTrainAnnSelect::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichTrainAnnSelect.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      fRichRings->Streamer(R__b);
      fMcTracks->Streamer(R__b);
      fRichRingMatches->Streamer(R__b);
      R__b >> fEventNumber;
      R__b >> fQuota;
      R__b >> fMaxNofTrainSamples;
      R__b >> fNofFakeLikeTrue;
      R__b >> fNofTrueLikeFake;
      R__b >> fAnnCut;
      {
         vector<TH1D*> &R__stl =  fhNofHits;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhNofHits streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1D*> &R__stl =  fhAngle;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhAngle streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1D*> &R__stl =  fhNofHitsOnRing;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhNofHitsOnRing streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1D*> &R__stl =  fhChi2;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhChi2 streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1D*> &R__stl =  fhRadPos;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhRadPos streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1D*> &R__stl =  fhRadius;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhRadius streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1D*> &R__stl =  fhAnnOutput;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhAnnOutput streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1D*> &R__stl =  fhCumProb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhCumProb streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<RingSelectParam> > &R__stl =  fRSParams;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class RingSelectParam, class std::allocator<class RingSelectParam> >));
         if (R__tcl1==0) {
            Error("fRSParams streamer","Missing the TClass object for class std::vector<class RingSelectParam, class std::allocator<class RingSelectParam> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<RingSelectParam> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fFitCOP;
      R__b >> fSelectImpl;
      {
         vector<TH1*> &R__stl =  fHists;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fHists streamer","Missing the TClass object for class TH1 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1* R__t;
            R__t = (TH1*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, CbmRichTrainAnnSelect::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmRichTrainAnnSelect::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      fRichRings->Streamer(R__b);
      fMcTracks->Streamer(R__b);
      fRichRingMatches->Streamer(R__b);
      R__b << fEventNumber;
      R__b << fQuota;
      R__b << fMaxNofTrainSamples;
      R__b << fNofFakeLikeTrue;
      R__b << fNofTrueLikeFake;
      R__b << fAnnCut;
      {
         vector<TH1D*> &R__stl =  fhNofHits;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1D*> &R__stl =  fhAngle;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1D*> &R__stl =  fhNofHitsOnRing;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1D*> &R__stl =  fhChi2;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1D*> &R__stl =  fhRadPos;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1D*> &R__stl =  fhRadius;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1D*> &R__stl =  fhAnnOutput;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1D*> &R__stl =  fhCumProb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<RingSelectParam> > &R__stl =  fRSParams;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class RingSelectParam, class std::allocator<class RingSelectParam> >));
         if (R__tcl1==0) {
            Error("fRSParams streamer","Missing the TClass object for class std::vector<class RingSelectParam, class std::allocator<class RingSelectParam> >!");
            return;
         }
            vector<vector<RingSelectParam> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<RingSelectParam>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << fFitCOP;
      R__b << fSelectImpl;
      {
         vector<TH1*> &R__stl =  fHists;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichTrainAnnSelect(void *p) {
      return  p ? new(p) ::CbmRichTrainAnnSelect : new ::CbmRichTrainAnnSelect;
   }
   static void *newArray_CbmRichTrainAnnSelect(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichTrainAnnSelect[nElements] : new ::CbmRichTrainAnnSelect[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichTrainAnnSelect(void *p) {
      delete ((::CbmRichTrainAnnSelect*)p);
   }
   static void deleteArray_CbmRichTrainAnnSelect(void *p) {
      delete [] ((::CbmRichTrainAnnSelect*)p);
   }
   static void destruct_CbmRichTrainAnnSelect(void *p) {
      typedef ::CbmRichTrainAnnSelect current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmRichTrainAnnSelect(TBuffer &buf, void *obj) {
      ((::CbmRichTrainAnnSelect*)obj)->::CbmRichTrainAnnSelect::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmRichTrainAnnSelect

//______________________________________________________________________________
void CbmRichTrainAnnElectrons::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichTrainAnnElectrons.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> fEventNum;
      fRichHits->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fRichPoints->Streamer(R__b);
      fMCTracks->Streamer(R__b);
      fRichRingMatches->Streamer(R__b);
      fRichProj->Streamer(R__b);
      fStsTrackMatches->Streamer(R__b);
      fGlobalTracks->Streamer(R__b);
      fStsTracks->Streamer(R__b);
      R__b >> fMinNofHitsInRichRing;
      R__b >> fQuota;
      R__b >> fMaxNofTrainSamples;
      R__b >> fNofPiLikeEl;
      R__b >> fNofElLikePi;
      R__b >> fAnnCut;
      {
         vector<TH1D*> &R__stl =  fhAnnOutput;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhAnnOutput streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1D*> &R__stl =  fhCumProb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhCumProb streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<RingElectronParam> > &R__stl =  fRElIdParams;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class RingElectronParam, class std::allocator<class RingElectronParam> >));
         if (R__tcl1==0) {
            Error("fRElIdParams streamer","Missing the TClass object for class std::vector<class RingElectronParam, class std::allocator<class RingElectronParam> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<RingElectronParam> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TCanvas*> &R__stl =  fCanvas;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TCanvas *));
         if (R__tcl1==0) {
            Error("fCanvas streamer","Missing the TClass object for class TCanvas *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TCanvas* R__t;
            R__t = (TCanvas*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      { TString R__str; R__str.Streamer(R__b); fOutputDir = R__str.Data(); }
      {
         vector<TH1D*> &R__stl =  fhAaxis;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhAaxis streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1D*> &R__stl =  fhBaxis;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhBaxis streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1D*> &R__stl =  fhDistTrueMatch;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhDistTrueMatch streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1D*> &R__stl =  fhDistMisMatch;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhDistMisMatch streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1D*> &R__stl =  fhNofHits;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhNofHits streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1D*> &R__stl =  fhChi2;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhChi2 streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1D*> &R__stl =  fhRadPos;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1D *));
         if (R__tcl1==0) {
            Error("fhRadPos streamer","Missing the TClass object for class TH1D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1D* R__t;
            R__t = (TH1D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2D*> &R__stl =  fhAaxisVsMom;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2D *));
         if (R__tcl1==0) {
            Error("fhAaxisVsMom streamer","Missing the TClass object for class TH2D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2D* R__t;
            R__t = (TH2D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2D*> &R__stl =  fhBaxisVsMom;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2D *));
         if (R__tcl1==0) {
            Error("fhBaxisVsMom streamer","Missing the TClass object for class TH2D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2D* R__t;
            R__t = (TH2D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2D*> &R__stl =  fhPhiVsRadAng;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2D *));
         if (R__tcl1==0) {
            Error("fhPhiVsRadAng streamer","Missing the TClass object for class TH2D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2D* R__t;
            R__t = (TH2D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fHists;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fHists streamer","Missing the TClass object for class TH1 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH1* R__t;
            R__t = (TH1*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, CbmRichTrainAnnElectrons::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmRichTrainAnnElectrons::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << fEventNum;
      fRichHits->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fRichPoints->Streamer(R__b);
      fMCTracks->Streamer(R__b);
      fRichRingMatches->Streamer(R__b);
      fRichProj->Streamer(R__b);
      fStsTrackMatches->Streamer(R__b);
      fGlobalTracks->Streamer(R__b);
      fStsTracks->Streamer(R__b);
      R__b << fMinNofHitsInRichRing;
      R__b << fQuota;
      R__b << fMaxNofTrainSamples;
      R__b << fNofPiLikeEl;
      R__b << fNofElLikePi;
      R__b << fAnnCut;
      {
         vector<TH1D*> &R__stl =  fhAnnOutput;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1D*> &R__stl =  fhCumProb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<RingElectronParam> > &R__stl =  fRElIdParams;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class RingElectronParam, class std::allocator<class RingElectronParam> >));
         if (R__tcl1==0) {
            Error("fRElIdParams streamer","Missing the TClass object for class std::vector<class RingElectronParam, class std::allocator<class RingElectronParam> >!");
            return;
         }
            vector<vector<RingElectronParam> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<RingElectronParam>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<TCanvas*> &R__stl =  fCanvas;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TCanvas*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      { TString R__str = fOutputDir.c_str(); R__str.Streamer(R__b);}
      {
         vector<TH1D*> &R__stl =  fhAaxis;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1D*> &R__stl =  fhBaxis;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1D*> &R__stl =  fhDistTrueMatch;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1D*> &R__stl =  fhDistMisMatch;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1D*> &R__stl =  fhNofHits;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1D*> &R__stl =  fhChi2;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1D*> &R__stl =  fhRadPos;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2D*> &R__stl =  fhAaxisVsMom;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2D*> &R__stl =  fhBaxisVsMom;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2D*> &R__stl =  fhPhiVsRadAng;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fHists;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichTrainAnnElectrons(void *p) {
      return  p ? new(p) ::CbmRichTrainAnnElectrons : new ::CbmRichTrainAnnElectrons;
   }
   static void *newArray_CbmRichTrainAnnElectrons(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichTrainAnnElectrons[nElements] : new ::CbmRichTrainAnnElectrons[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichTrainAnnElectrons(void *p) {
      delete ((::CbmRichTrainAnnElectrons*)p);
   }
   static void deleteArray_CbmRichTrainAnnElectrons(void *p) {
      delete [] ((::CbmRichTrainAnnElectrons*)p);
   }
   static void destruct_CbmRichTrainAnnElectrons(void *p) {
      typedef ::CbmRichTrainAnnElectrons current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmRichTrainAnnElectrons(TBuffer &buf, void *obj) {
      ((::CbmRichTrainAnnElectrons*)obj)->::CbmRichTrainAnnElectrons::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmRichTrainAnnElectrons

//______________________________________________________________________________
void CbmRichReconstruction::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichReconstruction.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichReconstruction::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichReconstruction::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichReconstruction(void *p) {
      return  p ? new(p) ::CbmRichReconstruction : new ::CbmRichReconstruction;
   }
   static void *newArray_CbmRichReconstruction(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichReconstruction[nElements] : new ::CbmRichReconstruction[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichReconstruction(void *p) {
      delete ((::CbmRichReconstruction*)p);
   }
   static void deleteArray_CbmRichReconstruction(void *p) {
      delete [] ((::CbmRichReconstruction*)p);
   }
   static void destruct_CbmRichReconstruction(void *p) {
      typedef ::CbmRichReconstruction current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichReconstruction

//______________________________________________________________________________
void CbmRichHitProducer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichHitProducer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichHitProducer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichHitProducer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichHitProducer(void *p) {
      return  p ? new(p) ::CbmRichHitProducer : new ::CbmRichHitProducer;
   }
   static void *newArray_CbmRichHitProducer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichHitProducer[nElements] : new ::CbmRichHitProducer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichHitProducer(void *p) {
      delete ((::CbmRichHitProducer*)p);
   }
   static void deleteArray_CbmRichHitProducer(void *p) {
      delete [] ((::CbmRichHitProducer*)p);
   }
   static void destruct_CbmRichHitProducer(void *p) {
      typedef ::CbmRichHitProducer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichHitProducer

//______________________________________________________________________________
void CbmRichMCbmQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichMCbmQa.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> fHM;
      R__b >> fEventNum;
      { TString R__str; R__str.Streamer(R__b); fOutputDir = R__str.Data(); }
      fMCTracks->Streamer(R__b);
      fRichPoints->Streamer(R__b);
      fRichDigis->Streamer(R__b);
      fRichHits->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fRichRingMatches->Streamer(R__b);
      fRefPlanePoints->Streamer(R__b);
      fGlobalTracks->Streamer(R__b);
      fTrdTracks->Streamer(R__b);
      fTofHits->Streamer(R__b);
      fTofPoints->Streamer(R__b);
      fTofHitMatches->Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, CbmRichMCbmQa::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmRichMCbmQa::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << fHM;
      R__b << fEventNum;
      { TString R__str = fOutputDir.c_str(); R__str.Streamer(R__b);}
      fMCTracks->Streamer(R__b);
      fRichPoints->Streamer(R__b);
      fRichDigis->Streamer(R__b);
      fRichHits->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fRichRingMatches->Streamer(R__b);
      fRefPlanePoints->Streamer(R__b);
      fGlobalTracks->Streamer(R__b);
      fTrdTracks->Streamer(R__b);
      fTofHits->Streamer(R__b);
      fTofPoints->Streamer(R__b);
      fTofHitMatches->Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichMCbmQa(void *p) {
      return  p ? new(p) ::CbmRichMCbmQa : new ::CbmRichMCbmQa;
   }
   static void *newArray_CbmRichMCbmQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichMCbmQa[nElements] : new ::CbmRichMCbmQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichMCbmQa(void *p) {
      delete ((::CbmRichMCbmQa*)p);
   }
   static void deleteArray_CbmRichMCbmQa(void *p) {
      delete [] ((::CbmRichMCbmQa*)p);
   }
   static void destruct_CbmRichMCbmQa(void *p) {
      typedef ::CbmRichMCbmQa current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmRichMCbmQa(TBuffer &buf, void *obj) {
      ((::CbmRichMCbmQa*)obj)->::CbmRichMCbmQa::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmRichMCbmQa

//______________________________________________________________________________
void CbmRichMCbmQaReal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichMCbmQaReal.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> fDigiMan;
      R__b >> fT0Digis;
      fRichHits->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fTofHits->Streamer(R__b);
      fTofTracks->Streamer(R__b);
      R__b >> fTSHeader;
      fCbmEvent->Streamer(R__b);
      R__b >> fHM;
      R__b >> fXOffsetHisto;
      R__b >> fTotRichMin;
      R__b >> fTotRichMax;
      R__b >> fEventNum;
      R__b >> fNofDrawnRings;
      R__b >> fNofDrawnRichTofEv;
      R__b >> fMaxNofDrawnEvents;
      R__b >> fTriggerRichHits;
      R__b >> fTriggerTofHits;
      R__b >> fTracksinRich;
      R__b.ReadStaticArray((int*)fRingsWithTrack);
      R__b.ReadStaticArray((int*)fTracksinRichWithRichHits);
      R__b >> fTSMinTime;
      { TString R__str; R__str.Streamer(R__b); fOutputDir = R__str.Data(); }
      R__b >> fRestrictToAcc;
      R__b >> fRestrictToFullAcc;
      R__b >> fDoWriteHistToFile;
      R__b >> fDoDrawCanvas;
      R__b >> bSeDisplayRingOnly;
      R__b >> fDigiHitsInitialized;
      R__b >> fDoTimePlots;
      R__b >> fCbmEventStartTime;
      R__b >> fEventPnt;
      R__b.StreamObject(&(offset_read),typeid(offset_read));
      R__b.StreamObject(&(offset),typeid(offset));
      R__b.StreamObject(&(offset_cnt),typeid(offset_cnt));
      R__b >> fSeDisplay;
      R__b >> fSeDsply_TR;
      R__b.CheckByteCount(R__s, R__c, CbmRichMCbmQaReal::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmRichMCbmQaReal::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << fDigiMan;
      R__b << fT0Digis;
      fRichHits->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fTofHits->Streamer(R__b);
      fTofTracks->Streamer(R__b);
      R__b << fTSHeader;
      fCbmEvent->Streamer(R__b);
      R__b << fHM;
      R__b << fXOffsetHisto;
      R__b << fTotRichMin;
      R__b << fTotRichMax;
      R__b << fEventNum;
      R__b << fNofDrawnRings;
      R__b << fNofDrawnRichTofEv;
      R__b << fMaxNofDrawnEvents;
      R__b << fTriggerRichHits;
      R__b << fTriggerTofHits;
      R__b << fTracksinRich;
      R__b.WriteArray(fRingsWithTrack, 6);
      R__b.WriteArray(fTracksinRichWithRichHits, 4);
      R__b << fTSMinTime;
      { TString R__str = fOutputDir.c_str(); R__str.Streamer(R__b);}
      R__b << fRestrictToAcc;
      R__b << fRestrictToFullAcc;
      R__b << fDoWriteHistToFile;
      R__b << fDoDrawCanvas;
      R__b << bSeDisplayRingOnly;
      R__b << fDigiHitsInitialized;
      R__b << fDoTimePlots;
      R__b << fCbmEventStartTime;
      R__b << fEventPnt;
      R__b.StreamObject(&(offset_read),typeid(offset_read));
      R__b.StreamObject(&(offset),typeid(offset));
      R__b.StreamObject(&(offset_cnt),typeid(offset_cnt));
      R__b << fSeDisplay;
      R__b << fSeDsply_TR;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichMCbmQaReal(void *p) {
      return  p ? new(p) ::CbmRichMCbmQaReal : new ::CbmRichMCbmQaReal;
   }
   static void *newArray_CbmRichMCbmQaReal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichMCbmQaReal[nElements] : new ::CbmRichMCbmQaReal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichMCbmQaReal(void *p) {
      delete ((::CbmRichMCbmQaReal*)p);
   }
   static void deleteArray_CbmRichMCbmQaReal(void *p) {
      delete [] ((::CbmRichMCbmQaReal*)p);
   }
   static void destruct_CbmRichMCbmQaReal(void *p) {
      typedef ::CbmRichMCbmQaReal current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmRichMCbmQaReal(TBuffer &buf, void *obj) {
      ((::CbmRichMCbmQaReal*)obj)->::CbmRichMCbmQaReal::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmRichMCbmQaReal

//______________________________________________________________________________
void CbmRichMCbmQaRichOnly::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichMCbmQaRichOnly.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> fDigiMan;
      fRichHits->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fCbmEvent->Streamer(R__b);
      R__b >> fHM;
      R__b >> fXOffsetHisto;
      R__b >> fTotMin;
      R__b >> fTotMax;
      R__b >> fEventNum;
      R__b >> fNofDrawnRings;
      R__b >> fNofDrawnRichTofEv;
      R__b >> fNofDrawnEvents;
      R__b >> fMaxNofDrawnEvents;
      R__b >> fTriggerRichHits;
      { TString R__str; R__str.Streamer(R__b); fOutputDir = R__str.Data(); }
      R__b >> fRestrictToAcc;
      R__b >> fRestrictToFullAcc;
      R__b >> fDoWriteHistToFile;
      R__b >> fDoDrawCanvas;
      R__b >> fGenerateICD;
      R__b.StreamObject(&(ICD_offset_read),typeid(ICD_offset_read));
      R__b.StreamObject(&(ICD_offset),typeid(ICD_offset));
      R__b.StreamObject(&(ICD_offset_cnt),typeid(ICD_offset_cnt));
      R__b >> fSeDisplay;
      R__b.CheckByteCount(R__s, R__c, CbmRichMCbmQaRichOnly::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmRichMCbmQaRichOnly::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << fDigiMan;
      fRichHits->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fCbmEvent->Streamer(R__b);
      R__b << fHM;
      R__b << fXOffsetHisto;
      R__b << fTotMin;
      R__b << fTotMax;
      R__b << fEventNum;
      R__b << fNofDrawnRings;
      R__b << fNofDrawnRichTofEv;
      R__b << fNofDrawnEvents;
      R__b << fMaxNofDrawnEvents;
      R__b << fTriggerRichHits;
      { TString R__str = fOutputDir.c_str(); R__str.Streamer(R__b);}
      R__b << fRestrictToAcc;
      R__b << fRestrictToFullAcc;
      R__b << fDoWriteHistToFile;
      R__b << fDoDrawCanvas;
      R__b << fGenerateICD;
      R__b.StreamObject(&(ICD_offset_read),typeid(ICD_offset_read));
      R__b.StreamObject(&(ICD_offset),typeid(ICD_offset));
      R__b.StreamObject(&(ICD_offset_cnt),typeid(ICD_offset_cnt));
      R__b << fSeDisplay;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichMCbmQaRichOnly(void *p) {
      return  p ? new(p) ::CbmRichMCbmQaRichOnly : new ::CbmRichMCbmQaRichOnly;
   }
   static void *newArray_CbmRichMCbmQaRichOnly(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichMCbmQaRichOnly[nElements] : new ::CbmRichMCbmQaRichOnly[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichMCbmQaRichOnly(void *p) {
      delete ((::CbmRichMCbmQaRichOnly*)p);
   }
   static void deleteArray_CbmRichMCbmQaRichOnly(void *p) {
      delete [] ((::CbmRichMCbmQaRichOnly*)p);
   }
   static void destruct_CbmRichMCbmQaRichOnly(void *p) {
      typedef ::CbmRichMCbmQaRichOnly current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmRichMCbmQaRichOnly(TBuffer &buf, void *obj) {
      ((::CbmRichMCbmQaRichOnly*)obj)->::CbmRichMCbmQaRichOnly::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmRichMCbmQaRichOnly

//______________________________________________________________________________
void CbmRichMCbmHitProducer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichMCbmHitProducer.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> fDigiMan;
      fRichHits->Streamer(R__b);
      fCbmEvents->Streamer(R__b);
      R__b >> fDoToT;
      R__b >> fDoICD;
      R__b >> fRestrictToAcc;
      R__b >> fRestrictToFullAcc;
      R__b >> fRestrictToAerogelAccDec2019;
      R__b >> fToTLimitLow;
      R__b >> fToTLimitHigh;
      {
         map<Int_t,CbmRichMCbmMappingData> &R__stl =  fRichMapping;
         R__stl.clear();
         TClass *R__tcl2 = TBuffer::GetClass(typeid(class CbmRichMCbmMappingData));
         if (R__tcl2==0) {
            Error("fRichMapping streamer","Missing the TClass object for class CbmRichMCbmMappingData!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            CbmRichMCbmMappingData R__t2;
            R__b.StreamObject(&R__t2,R__tcl2);
            typedef int Value_t;
            std::pair<Value_t const, class CbmRichMCbmMappingData > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fEventNum;
      R__b >> fNofHits;
      R__b >> fHitError;
      { TString R__str; R__str.Streamer(R__b); fMappingFile = R__str.Data(); }
      { TString R__str; R__str.Streamer(R__b); fIcdFilenameBase = R__str.Data(); }
      R__b.StreamObject(&(fICD_offset_read),typeid(fICD_offset_read));
      R__b.CheckByteCount(R__s, R__c, CbmRichMCbmHitProducer::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmRichMCbmHitProducer::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << fDigiMan;
      fRichHits->Streamer(R__b);
      fCbmEvents->Streamer(R__b);
      R__b << fDoToT;
      R__b << fDoICD;
      R__b << fRestrictToAcc;
      R__b << fRestrictToFullAcc;
      R__b << fRestrictToAerogelAccDec2019;
      R__b << fToTLimitLow;
      R__b << fToTLimitHigh;
      {
         map<Int_t,CbmRichMCbmMappingData> &R__stl =  fRichMapping;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl2 = TBuffer::GetClass(typeid(class CbmRichMCbmMappingData));
         if (R__tcl2==0) {
            Error("fRichMappingstreamer","Missing the TClass object for class CbmRichMCbmMappingData!");
            return;
         }
            map<Int_t,CbmRichMCbmMappingData>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b.StreamObject((CbmRichMCbmMappingData*)&((*R__k).second),R__tcl2);
            }
         }
      }
      R__b << fEventNum;
      R__b << fNofHits;
      R__b << fHitError;
      { TString R__str = fMappingFile.c_str(); R__str.Streamer(R__b);}
      { TString R__str = fIcdFilenameBase.c_str(); R__str.Streamer(R__b);}
      R__b.StreamObject(&(fICD_offset_read),typeid(fICD_offset_read));
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichMCbmHitProducer(void *p) {
      return  p ? new(p) ::CbmRichMCbmHitProducer : new ::CbmRichMCbmHitProducer;
   }
   static void *newArray_CbmRichMCbmHitProducer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichMCbmHitProducer[nElements] : new ::CbmRichMCbmHitProducer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichMCbmHitProducer(void *p) {
      delete ((::CbmRichMCbmHitProducer*)p);
   }
   static void deleteArray_CbmRichMCbmHitProducer(void *p) {
      delete [] ((::CbmRichMCbmHitProducer*)p);
   }
   static void destruct_CbmRichMCbmHitProducer(void *p) {
      typedef ::CbmRichMCbmHitProducer current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmRichMCbmHitProducer(TBuffer &buf, void *obj) {
      ((::CbmRichMCbmHitProducer*)obj)->::CbmRichMCbmHitProducer::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmRichMCbmHitProducer

//______________________________________________________________________________
void CbmRichMCbmAerogelAna::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichMCbmAerogelAna.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> fDigiMan;
      fRichHits->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fCbmEvent->Streamer(R__b);
      R__b >> fHM;
      R__b >> fXOffsetHisto;
      R__b >> fEventNum;
      R__b >> fNofDrawnRings;
      R__b >> fNofDrawnRichTofEv;
      R__b >> fNofDrawnEvents;
      { TString R__str; R__str.Streamer(R__b); fOutputDir = R__str.Data(); }
      R__b >> fDoWriteHistToFile;
      R__b >> fDoDrawCanvas;
      R__b.CheckByteCount(R__s, R__c, CbmRichMCbmAerogelAna::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmRichMCbmAerogelAna::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << fDigiMan;
      fRichHits->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fCbmEvent->Streamer(R__b);
      R__b << fHM;
      R__b << fXOffsetHisto;
      R__b << fEventNum;
      R__b << fNofDrawnRings;
      R__b << fNofDrawnRichTofEv;
      R__b << fNofDrawnEvents;
      { TString R__str = fOutputDir.c_str(); R__str.Streamer(R__b);}
      R__b << fDoWriteHistToFile;
      R__b << fDoDrawCanvas;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichMCbmAerogelAna(void *p) {
      return  p ? new(p) ::CbmRichMCbmAerogelAna : new ::CbmRichMCbmAerogelAna;
   }
   static void *newArray_CbmRichMCbmAerogelAna(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichMCbmAerogelAna[nElements] : new ::CbmRichMCbmAerogelAna[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichMCbmAerogelAna(void *p) {
      delete ((::CbmRichMCbmAerogelAna*)p);
   }
   static void deleteArray_CbmRichMCbmAerogelAna(void *p) {
      delete [] ((::CbmRichMCbmAerogelAna*)p);
   }
   static void destruct_CbmRichMCbmAerogelAna(void *p) {
      typedef ::CbmRichMCbmAerogelAna current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmRichMCbmAerogelAna(TBuffer &buf, void *obj) {
      ((::CbmRichMCbmAerogelAna*)obj)->::CbmRichMCbmAerogelAna::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmRichMCbmAerogelAna

//______________________________________________________________________________
void CbmRichMCbmToTShifter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichMCbmToTShifter.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> fDigiMan;
      R__b >> fEventNum;
      { TString R__str; R__str.Streamer(R__b); fOutputDir = R__str.Data(); }
      {
         map<Int_t,map<Int_t,TH1*> > &R__stl =  fhTotMap;
         R__stl.clear();
         TClass *R__tcl2 = TBuffer::GetClass(typeid(class std::map<int, class TH1 *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1 *> > >));
         if (R__tcl2==0) {
            Error("fhTotMap streamer","Missing the TClass object for class std::map<int, class TH1 *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1 *> > >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            map<int,TH1*> R__t2;
            R__b.StreamObject(&R__t2,R__tcl2);
            typedef int Value_t;
            std::pair<Value_t const, class std::map<int, class TH1 *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1 *> > > > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fGeneratePDFs;
      R__b >> fShowTdcId;
      R__b.CheckByteCount(R__s, R__c, CbmRichMCbmToTShifter::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmRichMCbmToTShifter::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << fDigiMan;
      R__b << fEventNum;
      { TString R__str = fOutputDir.c_str(); R__str.Streamer(R__b);}
      {
         map<Int_t,map<Int_t,TH1*> > &R__stl =  fhTotMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl2 = TBuffer::GetClass(typeid(class std::map<int, class TH1 *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1 *> > >));
         if (R__tcl2==0) {
            Error("fhTotMapstreamer","Missing the TClass object for class std::map<int, class TH1 *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1 *> > >!");
            return;
         }
            map<Int_t,map<Int_t,TH1*> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b.StreamObject((map<int,TH1*>*)&((*R__k).second),R__tcl2);
            }
         }
      }
      R__b << fGeneratePDFs;
      R__b << fShowTdcId;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichMCbmToTShifter(void *p) {
      return  p ? new(p) ::CbmRichMCbmToTShifter : new ::CbmRichMCbmToTShifter;
   }
   static void *newArray_CbmRichMCbmToTShifter(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichMCbmToTShifter[nElements] : new ::CbmRichMCbmToTShifter[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichMCbmToTShifter(void *p) {
      delete ((::CbmRichMCbmToTShifter*)p);
   }
   static void deleteArray_CbmRichMCbmToTShifter(void *p) {
      delete [] ((::CbmRichMCbmToTShifter*)p);
   }
   static void destruct_CbmRichMCbmToTShifter(void *p) {
      typedef ::CbmRichMCbmToTShifter current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmRichMCbmToTShifter(TBuffer &buf, void *obj) {
      ((::CbmRichMCbmToTShifter*)obj)->::CbmRichMCbmToTShifter::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmRichMCbmToTShifter

//______________________________________________________________________________
void CbmRichMCbmSEDisplay::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichMCbmSEDisplay.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      fRichHits->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fTofTracks->Streamer(R__b);
      R__b >> fXOffsetHisto;
      R__b >> fTotRichMin;
      R__b >> fTotRichMax;
      R__b >> fNofDrawnEvents;
      R__b >> fMaxNofDrawnEvents;
      { TString R__str; R__str.Streamer(R__b); fOutputDir = R__str.Data(); }
      { TString R__str; R__str.Streamer(R__b); fFileName = R__str.Data(); }
      R__b >> fLEMin;
      R__b >> fLEMax;
      R__b >> fHM;
      R__b.CheckByteCount(R__s, R__c, CbmRichMCbmSEDisplay::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmRichMCbmSEDisplay::IsA(), kTRUE);
      fRichHits->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fTofTracks->Streamer(R__b);
      R__b << fXOffsetHisto;
      R__b << fTotRichMin;
      R__b << fTotRichMax;
      R__b << fNofDrawnEvents;
      R__b << fMaxNofDrawnEvents;
      { TString R__str = fOutputDir.c_str(); R__str.Streamer(R__b);}
      { TString R__str = fFileName.c_str(); R__str.Streamer(R__b);}
      R__b << fLEMin;
      R__b << fLEMax;
      R__b << fHM;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichMCbmSEDisplay(void *p) {
      return  p ? new(p) ::CbmRichMCbmSEDisplay : new ::CbmRichMCbmSEDisplay;
   }
   static void *newArray_CbmRichMCbmSEDisplay(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichMCbmSEDisplay[nElements] : new ::CbmRichMCbmSEDisplay[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichMCbmSEDisplay(void *p) {
      delete ((::CbmRichMCbmSEDisplay*)p);
   }
   static void deleteArray_CbmRichMCbmSEDisplay(void *p) {
      delete [] ((::CbmRichMCbmSEDisplay*)p);
   }
   static void destruct_CbmRichMCbmSEDisplay(void *p) {
      typedef ::CbmRichMCbmSEDisplay current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmRichMCbmSEDisplay(TBuffer &buf, void *obj) {
      ((::CbmRichMCbmSEDisplay*)obj)->::CbmRichMCbmSEDisplay::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmRichMCbmSEDisplay

//______________________________________________________________________________
void CbmRichRingFitterQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichRingFitterQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichRingFitterQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichRingFitterQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichRingFitterQa(void *p) {
      return  p ? new(p) ::CbmRichRingFitterQa : new ::CbmRichRingFitterQa;
   }
   static void *newArray_CbmRichRingFitterQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichRingFitterQa[nElements] : new ::CbmRichRingFitterQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichRingFitterQa(void *p) {
      delete ((::CbmRichRingFitterQa*)p);
   }
   static void deleteArray_CbmRichRingFitterQa(void *p) {
      delete [] ((::CbmRichRingFitterQa*)p);
   }
   static void destruct_CbmRichRingFitterQa(void *p) {
      typedef ::CbmRichRingFitterQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichRingFitterQa

//______________________________________________________________________________
void CbmRichGeoOpt::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichGeoOpt.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      fRichPoints->Streamer(R__b);
      fMcTracks->Streamer(R__b);
      fRefPoints->Streamer(R__b);
      fRichHits->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fRichRingMatches->Streamer(R__b);
      R__b >> fEventNum;
      R__b >> PMTPointsFilled;
      R__b >> fMinNofHits;
      R__b >> nPhotonsNotOnPlane;
      R__b >> nPhotonsNotOnSphere;
      R__b >> nTotalPhorons;
      {
         vector<TVector3> &R__stl =  PMTPlanePoints;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TVector3 R__t;
            R__t.Streamer(R__b);
            R__stl.push_back(R__t);
         }
      }
      PMTPlaneCenter.Streamer(R__b);
      ReadPMTPlaneCenter.Streamer(R__b);
      ReadPMTPlaneCenterOrig.Streamer(R__b);
      MirrorCenter.Streamer(R__b);
      R__b >> RotX;
      R__b >> RotY;
      PMT_r1.Streamer(R__b);
      PMT_r2.Streamer(R__b);
      PMT_norm.Streamer(R__b);
      R__b >> PMTPlaneCenterX;
      R__b >> PMTPlaneCenterY;
      R__b >> PMTPlaneThirdX;
      MirrPosition.Streamer(R__b);
      R__b >> SensPointsFilled;
      {
         vector<TVector3> &R__stl =  SensPlanePoints;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TVector3 R__t;
            R__t.Streamer(R__b);
            R__stl.push_back(R__t);
         }
      }
      Sens_r1.Streamer(R__b);
      Sens_r2.Streamer(R__b);
      Sens_norm.Streamer(R__b);
      R__b >> H_Diff_LineRefPMT_MomAtPMT;
      R__b >> H_Theta_TwoVectors;
      R__b >> H_DistancePMTtoMirrCenter;
      R__b >> H_DistancePMTtoMirr;
      R__b >> H_MomPrim;
      R__b >> H_PtPrim;
      R__b >> H_MomPt;
      R__b >> H_Mom_Theta_MC;
      R__b >> H_Pt_Theta_MC;
      R__b >> H_Mom_Theta_Rec;
      R__b >> H_Pt_Theta_Rec;
      R__b >> H_Mom_Theta_Acc;
      R__b >> H_Pt_Theta_Acc;
      R__b >> H_MomRing;
      R__b >> H_acc_mom_el;
      R__b >> H_acc_pty_el;
      R__b >> H_Mom_XY_Theta25;
      R__b >> H_MomPrim_RegularTheta;
      R__b >> H_acc_mom_el_RegularTheta;
      R__b >> H_Hits_XY;
      R__b >> H_Hits_XY_LeftHalf;
      R__b >> H_Hits_XY_RightHalf;
      R__b >> H_Hits_XY_Left2Thirds;
      R__b >> H_Hits_XY_RightThird;
      R__b >> H_PointsIn_XY;
      R__b >> H_PointsIn_XY_LeftHalf;
      R__b >> H_PointsIn_XY_RightHalf;
      R__b >> H_PointsIn_XY_Left2Thirds;
      R__b >> H_PointsIn_XY_RightThird;
      R__b >> H_PointsOut_XY;
      R__b >> H_NofPhotonsPerEv;
      R__b >> H_NofPhotonsPerHit;
      R__b >> H_NofPhotonsSmallerThan30;
      R__b >> H_DiffXhit;
      R__b >> H_DiffYhit;
      R__b >> H_dFocalPoint_Delta;
      R__b >> H_dFocalPoint_Rho;
      R__b >> H_Alpha;
      R__b >> H_Alpha_UpLeft;
      R__b >> H_Alpha_UpLeft_II;
      R__b >> H_Alpha_UpLeft_RegularTheta;
      R__b >> H_Alpha_UpLeft_LeftHalf;
      R__b >> H_Alpha_UpLeft_RightHalf;
      R__b >> H_Alpha_UpLeft_RightThird;
      R__b >> H_Alpha_UpLeft_Left2Thirds;
      R__b >> H_Alpha_XYposAtDet;
      R__b >> H_Alpha_XYposAtDet_RegularTheta;
      R__b >> H_Alpha_XYposAtDet_LeftHalf;
      R__b >> H_Alpha_XYposAtDet_RightHalf;
      R__b >> H_Alpha_XYposAtDet_RightThird;
      R__b >> H_Alpha_XYposAtDet_Left2Thirds;
      R__b >> H_NofHitsAll;
      R__b >> H_NofRings;
      R__b >> H_NofRings_NofHits;
      R__b >> H_RingCenterX;
      R__b >> H_RingCenterY;
      R__b >> H_RingCenter;
      R__b >> H_Radius;
      R__b >> H_aAxis;
      R__b >> H_bAxis;
      R__b >> H_boa;
      R__b >> H_boa_RegularTheta;
      R__b >> H_boa_LeftHalf;
      R__b >> H_boa_RightHalf;
      R__b >> H_boa_RightThird;
      R__b >> H_boa_Left2Thirds;
      R__b >> H_dR;
      R__b >> H_dR_aa;
      R__b >> H_dR_RegularTheta;
      R__b >> H_dR_LeftHalf;
      R__b >> H_dR_RightHalf;
      R__b >> H_dR_RightThird;
      R__b >> H_dR_Left2Thirds;
      R__b >> H_RingCenter_Aaxis;
      R__b >> H_RingCenter_Baxis;
      R__b >> H_RingCenter_boa;
      R__b >> H_RingCenter_boa_RegularTheta;
      R__b >> H_RingCenter_boa_LeftHalf;
      R__b >> H_RingCenter_boa_RightHalf;
      R__b >> H_RingCenter_boa_RightThird;
      R__b >> H_RingCenter_boa_Left2Thirds;
      R__b >> H_RingCenter_dR;
      R__b >> H_RingCenter_dR_RegularTheta;
      R__b >> H_RingCenter_dR_LeftHalf;
      R__b >> H_RingCenter_dR_RightHalf;
      R__b >> H_RingCenter_dR_RightThird;
      R__b >> H_RingCenter_dR_Left2Thirds;
      R__b.CheckByteCount(R__s, R__c, CbmRichGeoOpt::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmRichGeoOpt::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      fRichPoints->Streamer(R__b);
      fMcTracks->Streamer(R__b);
      fRefPoints->Streamer(R__b);
      fRichHits->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fRichRingMatches->Streamer(R__b);
      R__b << fEventNum;
      R__b << PMTPointsFilled;
      R__b << fMinNofHits;
      R__b << nPhotonsNotOnPlane;
      R__b << nPhotonsNotOnSphere;
      R__b << nTotalPhorons;
      {
         vector<TVector3> &R__stl =  PMTPlanePoints;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TVector3>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            ((TVector3&)(*R__k)).Streamer(R__b);
            }
         }
      }
      PMTPlaneCenter.Streamer(R__b);
      ReadPMTPlaneCenter.Streamer(R__b);
      ReadPMTPlaneCenterOrig.Streamer(R__b);
      MirrorCenter.Streamer(R__b);
      R__b << RotX;
      R__b << RotY;
      PMT_r1.Streamer(R__b);
      PMT_r2.Streamer(R__b);
      PMT_norm.Streamer(R__b);
      R__b << PMTPlaneCenterX;
      R__b << PMTPlaneCenterY;
      R__b << PMTPlaneThirdX;
      MirrPosition.Streamer(R__b);
      R__b << SensPointsFilled;
      {
         vector<TVector3> &R__stl =  SensPlanePoints;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TVector3>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            ((TVector3&)(*R__k)).Streamer(R__b);
            }
         }
      }
      Sens_r1.Streamer(R__b);
      Sens_r2.Streamer(R__b);
      Sens_norm.Streamer(R__b);
      R__b << (TObject*)H_Diff_LineRefPMT_MomAtPMT;
      R__b << (TObject*)H_Theta_TwoVectors;
      R__b << (TObject*)H_DistancePMTtoMirrCenter;
      R__b << (TObject*)H_DistancePMTtoMirr;
      R__b << (TObject*)H_MomPrim;
      R__b << (TObject*)H_PtPrim;
      R__b << (TObject*)H_MomPt;
      R__b << (TObject*)H_Mom_Theta_MC;
      R__b << (TObject*)H_Pt_Theta_MC;
      R__b << (TObject*)H_Mom_Theta_Rec;
      R__b << (TObject*)H_Pt_Theta_Rec;
      R__b << (TObject*)H_Mom_Theta_Acc;
      R__b << (TObject*)H_Pt_Theta_Acc;
      R__b << (TObject*)H_MomRing;
      R__b << (TObject*)H_acc_mom_el;
      R__b << (TObject*)H_acc_pty_el;
      R__b << (TObject*)H_Mom_XY_Theta25;
      R__b << (TObject*)H_MomPrim_RegularTheta;
      R__b << (TObject*)H_acc_mom_el_RegularTheta;
      R__b << (TObject*)H_Hits_XY;
      R__b << (TObject*)H_Hits_XY_LeftHalf;
      R__b << (TObject*)H_Hits_XY_RightHalf;
      R__b << (TObject*)H_Hits_XY_Left2Thirds;
      R__b << (TObject*)H_Hits_XY_RightThird;
      R__b << (TObject*)H_PointsIn_XY;
      R__b << (TObject*)H_PointsIn_XY_LeftHalf;
      R__b << (TObject*)H_PointsIn_XY_RightHalf;
      R__b << (TObject*)H_PointsIn_XY_Left2Thirds;
      R__b << (TObject*)H_PointsIn_XY_RightThird;
      R__b << (TObject*)H_PointsOut_XY;
      R__b << (TObject*)H_NofPhotonsPerEv;
      R__b << (TObject*)H_NofPhotonsPerHit;
      R__b << (TObject*)H_NofPhotonsSmallerThan30;
      R__b << (TObject*)H_DiffXhit;
      R__b << (TObject*)H_DiffYhit;
      R__b << (TObject*)H_dFocalPoint_Delta;
      R__b << (TObject*)H_dFocalPoint_Rho;
      R__b << (TObject*)H_Alpha;
      R__b << (TObject*)H_Alpha_UpLeft;
      R__b << (TObject*)H_Alpha_UpLeft_II;
      R__b << (TObject*)H_Alpha_UpLeft_RegularTheta;
      R__b << (TObject*)H_Alpha_UpLeft_LeftHalf;
      R__b << (TObject*)H_Alpha_UpLeft_RightHalf;
      R__b << (TObject*)H_Alpha_UpLeft_RightThird;
      R__b << (TObject*)H_Alpha_UpLeft_Left2Thirds;
      R__b << (TObject*)H_Alpha_XYposAtDet;
      R__b << (TObject*)H_Alpha_XYposAtDet_RegularTheta;
      R__b << (TObject*)H_Alpha_XYposAtDet_LeftHalf;
      R__b << (TObject*)H_Alpha_XYposAtDet_RightHalf;
      R__b << (TObject*)H_Alpha_XYposAtDet_RightThird;
      R__b << (TObject*)H_Alpha_XYposAtDet_Left2Thirds;
      R__b << (TObject*)H_NofHitsAll;
      R__b << (TObject*)H_NofRings;
      R__b << (TObject*)H_NofRings_NofHits;
      R__b << (TObject*)H_RingCenterX;
      R__b << (TObject*)H_RingCenterY;
      R__b << (TObject*)H_RingCenter;
      R__b << (TObject*)H_Radius;
      R__b << (TObject*)H_aAxis;
      R__b << (TObject*)H_bAxis;
      R__b << (TObject*)H_boa;
      R__b << (TObject*)H_boa_RegularTheta;
      R__b << (TObject*)H_boa_LeftHalf;
      R__b << (TObject*)H_boa_RightHalf;
      R__b << (TObject*)H_boa_RightThird;
      R__b << (TObject*)H_boa_Left2Thirds;
      R__b << (TObject*)H_dR;
      R__b << (TObject*)H_dR_aa;
      R__b << (TObject*)H_dR_RegularTheta;
      R__b << (TObject*)H_dR_LeftHalf;
      R__b << (TObject*)H_dR_RightHalf;
      R__b << (TObject*)H_dR_RightThird;
      R__b << (TObject*)H_dR_Left2Thirds;
      R__b << (TObject*)H_RingCenter_Aaxis;
      R__b << (TObject*)H_RingCenter_Baxis;
      R__b << (TObject*)H_RingCenter_boa;
      R__b << (TObject*)H_RingCenter_boa_RegularTheta;
      R__b << (TObject*)H_RingCenter_boa_LeftHalf;
      R__b << (TObject*)H_RingCenter_boa_RightHalf;
      R__b << (TObject*)H_RingCenter_boa_RightThird;
      R__b << (TObject*)H_RingCenter_boa_Left2Thirds;
      R__b << (TObject*)H_RingCenter_dR;
      R__b << (TObject*)H_RingCenter_dR_RegularTheta;
      R__b << (TObject*)H_RingCenter_dR_LeftHalf;
      R__b << (TObject*)H_RingCenter_dR_RightHalf;
      R__b << (TObject*)H_RingCenter_dR_RightThird;
      R__b << (TObject*)H_RingCenter_dR_Left2Thirds;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichGeoOpt(void *p) {
      return  p ? new(p) ::CbmRichGeoOpt : new ::CbmRichGeoOpt;
   }
   static void *newArray_CbmRichGeoOpt(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichGeoOpt[nElements] : new ::CbmRichGeoOpt[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichGeoOpt(void *p) {
      delete ((::CbmRichGeoOpt*)p);
   }
   static void deleteArray_CbmRichGeoOpt(void *p) {
      delete [] ((::CbmRichGeoOpt*)p);
   }
   static void destruct_CbmRichGeoOpt(void *p) {
      typedef ::CbmRichGeoOpt current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmRichGeoOpt(TBuffer &buf, void *obj) {
      ((::CbmRichGeoOpt*)obj)->::CbmRichGeoOpt::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmRichGeoOpt

//______________________________________________________________________________
void CbmRichRecoTbQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichRecoTbQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichRecoTbQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichRecoTbQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichRecoTbQa(void *p) {
      return  p ? new(p) ::CbmRichRecoTbQa : new ::CbmRichRecoTbQa;
   }
   static void *newArray_CbmRichRecoTbQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichRecoTbQa[nElements] : new ::CbmRichRecoTbQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichRecoTbQa(void *p) {
      delete ((::CbmRichRecoTbQa*)p);
   }
   static void deleteArray_CbmRichRecoTbQa(void *p) {
      delete [] ((::CbmRichRecoTbQa*)p);
   }
   static void destruct_CbmRichRecoTbQa(void *p) {
      typedef ::CbmRichRecoTbQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichRecoTbQa

//______________________________________________________________________________
void CbmRichGeoTest::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichGeoTest.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichGeoTest::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichGeoTest::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichGeoTest(void *p) {
      return  p ? new(p) ::CbmRichGeoTest : new ::CbmRichGeoTest;
   }
   static void *newArray_CbmRichGeoTest(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichGeoTest[nElements] : new ::CbmRichGeoTest[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichGeoTest(void *p) {
      delete ((::CbmRichGeoTest*)p);
   }
   static void deleteArray_CbmRichGeoTest(void *p) {
      delete [] ((::CbmRichGeoTest*)p);
   }
   static void destruct_CbmRichGeoTest(void *p) {
      typedef ::CbmRichGeoTest current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichGeoTest

//______________________________________________________________________________
void CbmRichUrqmdTest::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichUrqmdTest.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichUrqmdTest::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichUrqmdTest::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichUrqmdTest(void *p) {
      return  p ? new(p) ::CbmRichUrqmdTest : new ::CbmRichUrqmdTest;
   }
   static void *newArray_CbmRichUrqmdTest(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichUrqmdTest[nElements] : new ::CbmRichUrqmdTest[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichUrqmdTest(void *p) {
      delete ((::CbmRichUrqmdTest*)p);
   }
   static void deleteArray_CbmRichUrqmdTest(void *p) {
      delete [] ((::CbmRichUrqmdTest*)p);
   }
   static void destruct_CbmRichUrqmdTest(void *p) {
      typedef ::CbmRichUrqmdTest current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichUrqmdTest

//______________________________________________________________________________
void CbmRichGeoTestOpt::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichGeoTestOpt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichGeoTestOpt::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichGeoTestOpt::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichGeoTestOpt(void *p) {
      return  p ? new(p) ::CbmRichGeoTestOpt : new ::CbmRichGeoTestOpt;
   }
   static void *newArray_CbmRichGeoTestOpt(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichGeoTestOpt[nElements] : new ::CbmRichGeoTestOpt[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichGeoTestOpt(void *p) {
      delete ((::CbmRichGeoTestOpt*)p);
   }
   static void deleteArray_CbmRichGeoTestOpt(void *p) {
      delete [] ((::CbmRichGeoTestOpt*)p);
   }
   static void destruct_CbmRichGeoTestOpt(void *p) {
      typedef ::CbmRichGeoTestOpt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichGeoTestOpt

//______________________________________________________________________________
void CbmRichRecoQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichRecoQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichRecoQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichRecoQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichRecoQa(void *p) {
      return  p ? new(p) ::CbmRichRecoQa : new ::CbmRichRecoQa;
   }
   static void *newArray_CbmRichRecoQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichRecoQa[nElements] : new ::CbmRichRecoQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichRecoQa(void *p) {
      delete ((::CbmRichRecoQa*)p);
   }
   static void deleteArray_CbmRichRecoQa(void *p) {
      delete [] ((::CbmRichRecoQa*)p);
   }
   static void destruct_CbmRichRecoQa(void *p) {
      typedef ::CbmRichRecoQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichRecoQa

//______________________________________________________________________________
void CbmL1RichRingQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmL1RichRingQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmL1RichRingQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmL1RichRingQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmL1RichRingQa(void *p) {
      return  p ? new(p) ::CbmL1RichRingQa : new ::CbmL1RichRingQa;
   }
   static void *newArray_CbmL1RichRingQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmL1RichRingQa[nElements] : new ::CbmL1RichRingQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmL1RichRingQa(void *p) {
      delete ((::CbmL1RichRingQa*)p);
   }
   static void deleteArray_CbmL1RichRingQa(void *p) {
      delete [] ((::CbmL1RichRingQa*)p);
   }
   static void destruct_CbmL1RichRingQa(void *p) {
      typedef ::CbmL1RichRingQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmL1RichRingQa

//______________________________________________________________________________
void CbmRichUnpackMonitor::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichUnpackMonitor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichUnpackMonitor::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichUnpackMonitor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichUnpackMonitor(void *p) {
      return  p ? new(p) ::CbmRichUnpackMonitor : new ::CbmRichUnpackMonitor;
   }
   static void *newArray_CbmRichUnpackMonitor(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichUnpackMonitor[nElements] : new ::CbmRichUnpackMonitor[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichUnpackMonitor(void *p) {
      delete ((::CbmRichUnpackMonitor*)p);
   }
   static void deleteArray_CbmRichUnpackMonitor(void *p) {
      delete [] ((::CbmRichUnpackMonitor*)p);
   }
   static void destruct_CbmRichUnpackMonitor(void *p) {
      typedef ::CbmRichUnpackMonitor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichUnpackMonitor

//______________________________________________________________________________
void CbmRichUnpackAlgoBase::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichUnpackAlgoBase.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichUnpackAlgoBase::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichUnpackAlgoBase::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmRichUnpackAlgoBase(void *p) {
      delete ((::CbmRichUnpackAlgoBase*)p);
   }
   static void deleteArray_CbmRichUnpackAlgoBase(void *p) {
      delete [] ((::CbmRichUnpackAlgoBase*)p);
   }
   static void destruct_CbmRichUnpackAlgoBase(void *p) {
      typedef ::CbmRichUnpackAlgoBase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichUnpackAlgoBase

//______________________________________________________________________________
void CbmRichUnpackAlgo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichUnpackAlgo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichUnpackAlgo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichUnpackAlgo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichUnpackAlgo(void *p) {
      return  p ? new(p) ::CbmRichUnpackAlgo : new ::CbmRichUnpackAlgo;
   }
   static void *newArray_CbmRichUnpackAlgo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichUnpackAlgo[nElements] : new ::CbmRichUnpackAlgo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichUnpackAlgo(void *p) {
      delete ((::CbmRichUnpackAlgo*)p);
   }
   static void deleteArray_CbmRichUnpackAlgo(void *p) {
      delete [] ((::CbmRichUnpackAlgo*)p);
   }
   static void destruct_CbmRichUnpackAlgo(void *p) {
      typedef ::CbmRichUnpackAlgo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichUnpackAlgo

//______________________________________________________________________________
void CbmRichUnpackAlgo2022::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichUnpackAlgo2022.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichUnpackAlgo2022::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichUnpackAlgo2022::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichUnpackAlgo2022(void *p) {
      return  p ? new(p) ::CbmRichUnpackAlgo2022 : new ::CbmRichUnpackAlgo2022;
   }
   static void *newArray_CbmRichUnpackAlgo2022(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichUnpackAlgo2022[nElements] : new ::CbmRichUnpackAlgo2022[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichUnpackAlgo2022(void *p) {
      delete ((::CbmRichUnpackAlgo2022*)p);
   }
   static void deleteArray_CbmRichUnpackAlgo2022(void *p) {
      delete [] ((::CbmRichUnpackAlgo2022*)p);
   }
   static void destruct_CbmRichUnpackAlgo2022(void *p) {
      typedef ::CbmRichUnpackAlgo2022 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichUnpackAlgo2022

//______________________________________________________________________________
void CbmRichUnpackConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichUnpackConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichUnpackConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichUnpackConfig::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmRichUnpackConfig(void *p) {
      delete ((::CbmRichUnpackConfig*)p);
   }
   static void deleteArray_CbmRichUnpackConfig(void *p) {
      delete [] ((::CbmRichUnpackConfig*)p);
   }
   static void destruct_CbmRichUnpackConfig(void *p) {
      typedef ::CbmRichUnpackConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichUnpackConfig

namespace ROOT {
   static TClass *vectorlEvectorlERingSelectParamgRsPgR_Dictionary();
   static void vectorlEvectorlERingSelectParamgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlERingSelectParamgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlERingSelectParamgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlERingSelectParamgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlERingSelectParamgRsPgR(void *p);
   static void destruct_vectorlEvectorlERingSelectParamgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<RingSelectParam> >*)
   {
      vector<vector<RingSelectParam> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<RingSelectParam> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<RingSelectParam> >", -2, "vector", 389,
                  typeid(vector<vector<RingSelectParam> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlERingSelectParamgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<RingSelectParam> >) );
      instance.SetNew(&new_vectorlEvectorlERingSelectParamgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlERingSelectParamgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlERingSelectParamgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlERingSelectParamgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlERingSelectParamgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<RingSelectParam> > >()));

      ::ROOT::AddClassAlternate("vector<vector<RingSelectParam> >","std::vector<std::vector<RingSelectParam, std::allocator<RingSelectParam> >, std::allocator<std::vector<RingSelectParam, std::allocator<RingSelectParam> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<RingSelectParam> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlERingSelectParamgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<RingSelectParam> >*)nullptr)->GetClass();
      vectorlEvectorlERingSelectParamgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlERingSelectParamgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlERingSelectParamgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<RingSelectParam> > : new vector<vector<RingSelectParam> >;
   }
   static void *newArray_vectorlEvectorlERingSelectParamgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<RingSelectParam> >[nElements] : new vector<vector<RingSelectParam> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlERingSelectParamgRsPgR(void *p) {
      delete ((vector<vector<RingSelectParam> >*)p);
   }
   static void deleteArray_vectorlEvectorlERingSelectParamgRsPgR(void *p) {
      delete [] ((vector<vector<RingSelectParam> >*)p);
   }
   static void destruct_vectorlEvectorlERingSelectParamgRsPgR(void *p) {
      typedef vector<vector<RingSelectParam> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<RingSelectParam> >

namespace ROOT {
   static TClass *vectorlEvectorlERingElectronParamgRsPgR_Dictionary();
   static void vectorlEvectorlERingElectronParamgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlERingElectronParamgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlERingElectronParamgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlERingElectronParamgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlERingElectronParamgRsPgR(void *p);
   static void destruct_vectorlEvectorlERingElectronParamgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<RingElectronParam> >*)
   {
      vector<vector<RingElectronParam> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<RingElectronParam> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<RingElectronParam> >", -2, "vector", 389,
                  typeid(vector<vector<RingElectronParam> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlERingElectronParamgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<RingElectronParam> >) );
      instance.SetNew(&new_vectorlEvectorlERingElectronParamgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlERingElectronParamgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlERingElectronParamgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlERingElectronParamgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlERingElectronParamgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<RingElectronParam> > >()));

      ::ROOT::AddClassAlternate("vector<vector<RingElectronParam> >","std::vector<std::vector<RingElectronParam, std::allocator<RingElectronParam> >, std::allocator<std::vector<RingElectronParam, std::allocator<RingElectronParam> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<RingElectronParam> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlERingElectronParamgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<RingElectronParam> >*)nullptr)->GetClass();
      vectorlEvectorlERingElectronParamgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlERingElectronParamgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlERingElectronParamgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<RingElectronParam> > : new vector<vector<RingElectronParam> >;
   }
   static void *newArray_vectorlEvectorlERingElectronParamgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<RingElectronParam> >[nElements] : new vector<vector<RingElectronParam> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlERingElectronParamgRsPgR(void *p) {
      delete ((vector<vector<RingElectronParam> >*)p);
   }
   static void deleteArray_vectorlEvectorlERingElectronParamgRsPgR(void *p) {
      delete [] ((vector<vector<RingElectronParam> >*)p);
   }
   static void destruct_vectorlEvectorlERingElectronParamgRsPgR(void *p) {
      typedef vector<vector<RingElectronParam> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<RingElectronParam> >

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
   static TClass *vectorlEpairlEintcOintgRsPgR_Dictionary();
   static void vectorlEpairlEintcOintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlEintcOintgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEpairlEintcOintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlEintcOintgRsPgR(void *p);
   static void deleteArray_vectorlEpairlEintcOintgRsPgR(void *p);
   static void destruct_vectorlEpairlEintcOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<int,int> >*)
   {
      vector<pair<int,int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<int,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<int,int> >", -2, "vector", 389,
                  typeid(vector<pair<int,int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlEintcOintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<int,int> >) );
      instance.SetNew(&new_vectorlEpairlEintcOintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlEintcOintgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlEintcOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlEintcOintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlEintcOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<int,int> > >()));

      ::ROOT::AddClassAlternate("vector<pair<int,int> >","std::vector<std::pair<int, int>, std::allocator<std::pair<int, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pair<int,int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlEintcOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<int,int> >*)nullptr)->GetClass();
      vectorlEpairlEintcOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlEintcOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlEintcOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<int,int> > : new vector<pair<int,int> >;
   }
   static void *newArray_vectorlEpairlEintcOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<int,int> >[nElements] : new vector<pair<int,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlEintcOintgRsPgR(void *p) {
      delete ((vector<pair<int,int> >*)p);
   }
   static void deleteArray_vectorlEpairlEintcOintgRsPgR(void *p) {
      delete [] ((vector<pair<int,int> >*)p);
   }
   static void destruct_vectorlEpairlEintcOintgRsPgR(void *p) {
      typedef vector<pair<int,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<int,int> >

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
   static TClass *vectorlETVector3gR_Dictionary();
   static void vectorlETVector3gR_TClassManip(TClass*);
   static void *new_vectorlETVector3gR(void *p = nullptr);
   static void *newArray_vectorlETVector3gR(Long_t size, void *p);
   static void delete_vectorlETVector3gR(void *p);
   static void deleteArray_vectorlETVector3gR(void *p);
   static void destruct_vectorlETVector3gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TVector3>*)
   {
      vector<TVector3> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TVector3>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TVector3>", -2, "vector", 389,
                  typeid(vector<TVector3>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETVector3gR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TVector3>) );
      instance.SetNew(&new_vectorlETVector3gR);
      instance.SetNewArray(&newArray_vectorlETVector3gR);
      instance.SetDelete(&delete_vectorlETVector3gR);
      instance.SetDeleteArray(&deleteArray_vectorlETVector3gR);
      instance.SetDestructor(&destruct_vectorlETVector3gR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TVector3> >()));

      ::ROOT::AddClassAlternate("vector<TVector3>","std::vector<TVector3, std::allocator<TVector3> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TVector3>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETVector3gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TVector3>*)nullptr)->GetClass();
      vectorlETVector3gR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETVector3gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETVector3gR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TVector3> : new vector<TVector3>;
   }
   static void *newArray_vectorlETVector3gR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TVector3>[nElements] : new vector<TVector3>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETVector3gR(void *p) {
      delete ((vector<TVector3>*)p);
   }
   static void deleteArray_vectorlETVector3gR(void *p) {
      delete [] ((vector<TVector3>*)p);
   }
   static void destruct_vectorlETVector3gR(void *p) {
      typedef vector<TVector3> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TVector3>

namespace ROOT {
   static TClass *vectorlETH2DmUgR_Dictionary();
   static void vectorlETH2DmUgR_TClassManip(TClass*);
   static void *new_vectorlETH2DmUgR(void *p = nullptr);
   static void *newArray_vectorlETH2DmUgR(Long_t size, void *p);
   static void delete_vectorlETH2DmUgR(void *p);
   static void deleteArray_vectorlETH2DmUgR(void *p);
   static void destruct_vectorlETH2DmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TH2D*>*)
   {
      vector<TH2D*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TH2D*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TH2D*>", -2, "vector", 389,
                  typeid(vector<TH2D*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETH2DmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TH2D*>) );
      instance.SetNew(&new_vectorlETH2DmUgR);
      instance.SetNewArray(&newArray_vectorlETH2DmUgR);
      instance.SetDelete(&delete_vectorlETH2DmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETH2DmUgR);
      instance.SetDestructor(&destruct_vectorlETH2DmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TH2D*> >()));

      ::ROOT::AddClassAlternate("vector<TH2D*>","std::vector<TH2D*, std::allocator<TH2D*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TH2D*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETH2DmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TH2D*>*)nullptr)->GetClass();
      vectorlETH2DmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETH2DmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETH2DmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH2D*> : new vector<TH2D*>;
   }
   static void *newArray_vectorlETH2DmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH2D*>[nElements] : new vector<TH2D*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETH2DmUgR(void *p) {
      delete ((vector<TH2D*>*)p);
   }
   static void deleteArray_vectorlETH2DmUgR(void *p) {
      delete [] ((vector<TH2D*>*)p);
   }
   static void destruct_vectorlETH2DmUgR(void *p) {
      typedef vector<TH2D*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TH2D*>

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
   static TClass *vectorlETH1DmUgR_Dictionary();
   static void vectorlETH1DmUgR_TClassManip(TClass*);
   static void *new_vectorlETH1DmUgR(void *p = nullptr);
   static void *newArray_vectorlETH1DmUgR(Long_t size, void *p);
   static void delete_vectorlETH1DmUgR(void *p);
   static void deleteArray_vectorlETH1DmUgR(void *p);
   static void destruct_vectorlETH1DmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TH1D*>*)
   {
      vector<TH1D*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TH1D*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TH1D*>", -2, "vector", 389,
                  typeid(vector<TH1D*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETH1DmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TH1D*>) );
      instance.SetNew(&new_vectorlETH1DmUgR);
      instance.SetNewArray(&newArray_vectorlETH1DmUgR);
      instance.SetDelete(&delete_vectorlETH1DmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETH1DmUgR);
      instance.SetDestructor(&destruct_vectorlETH1DmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TH1D*> >()));

      ::ROOT::AddClassAlternate("vector<TH1D*>","std::vector<TH1D*, std::allocator<TH1D*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TH1D*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETH1DmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TH1D*>*)nullptr)->GetClass();
      vectorlETH1DmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETH1DmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETH1DmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH1D*> : new vector<TH1D*>;
   }
   static void *newArray_vectorlETH1DmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH1D*>[nElements] : new vector<TH1D*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETH1DmUgR(void *p) {
      delete ((vector<TH1D*>*)p);
   }
   static void deleteArray_vectorlETH1DmUgR(void *p) {
      delete [] ((vector<TH1D*>*)p);
   }
   static void destruct_vectorlETH1DmUgR(void *p) {
      typedef vector<TH1D*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TH1D*>

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
   static TClass *vectorlERingSelectParamgR_Dictionary();
   static void vectorlERingSelectParamgR_TClassManip(TClass*);
   static void *new_vectorlERingSelectParamgR(void *p = nullptr);
   static void *newArray_vectorlERingSelectParamgR(Long_t size, void *p);
   static void delete_vectorlERingSelectParamgR(void *p);
   static void deleteArray_vectorlERingSelectParamgR(void *p);
   static void destruct_vectorlERingSelectParamgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<RingSelectParam>*)
   {
      vector<RingSelectParam> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<RingSelectParam>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<RingSelectParam>", -2, "vector", 389,
                  typeid(vector<RingSelectParam>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlERingSelectParamgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<RingSelectParam>) );
      instance.SetNew(&new_vectorlERingSelectParamgR);
      instance.SetNewArray(&newArray_vectorlERingSelectParamgR);
      instance.SetDelete(&delete_vectorlERingSelectParamgR);
      instance.SetDeleteArray(&deleteArray_vectorlERingSelectParamgR);
      instance.SetDestructor(&destruct_vectorlERingSelectParamgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<RingSelectParam> >()));

      ::ROOT::AddClassAlternate("vector<RingSelectParam>","std::vector<RingSelectParam, std::allocator<RingSelectParam> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<RingSelectParam>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlERingSelectParamgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<RingSelectParam>*)nullptr)->GetClass();
      vectorlERingSelectParamgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlERingSelectParamgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlERingSelectParamgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<RingSelectParam> : new vector<RingSelectParam>;
   }
   static void *newArray_vectorlERingSelectParamgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<RingSelectParam>[nElements] : new vector<RingSelectParam>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlERingSelectParamgR(void *p) {
      delete ((vector<RingSelectParam>*)p);
   }
   static void deleteArray_vectorlERingSelectParamgR(void *p) {
      delete [] ((vector<RingSelectParam>*)p);
   }
   static void destruct_vectorlERingSelectParamgR(void *p) {
      typedef vector<RingSelectParam> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<RingSelectParam>

namespace ROOT {
   static TClass *vectorlERingElectronParamgR_Dictionary();
   static void vectorlERingElectronParamgR_TClassManip(TClass*);
   static void *new_vectorlERingElectronParamgR(void *p = nullptr);
   static void *newArray_vectorlERingElectronParamgR(Long_t size, void *p);
   static void delete_vectorlERingElectronParamgR(void *p);
   static void deleteArray_vectorlERingElectronParamgR(void *p);
   static void destruct_vectorlERingElectronParamgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<RingElectronParam>*)
   {
      vector<RingElectronParam> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<RingElectronParam>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<RingElectronParam>", -2, "vector", 389,
                  typeid(vector<RingElectronParam>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlERingElectronParamgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<RingElectronParam>) );
      instance.SetNew(&new_vectorlERingElectronParamgR);
      instance.SetNewArray(&newArray_vectorlERingElectronParamgR);
      instance.SetDelete(&delete_vectorlERingElectronParamgR);
      instance.SetDeleteArray(&deleteArray_vectorlERingElectronParamgR);
      instance.SetDestructor(&destruct_vectorlERingElectronParamgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<RingElectronParam> >()));

      ::ROOT::AddClassAlternate("vector<RingElectronParam>","std::vector<RingElectronParam, std::allocator<RingElectronParam> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<RingElectronParam>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlERingElectronParamgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<RingElectronParam>*)nullptr)->GetClass();
      vectorlERingElectronParamgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlERingElectronParamgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlERingElectronParamgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<RingElectronParam> : new vector<RingElectronParam>;
   }
   static void *newArray_vectorlERingElectronParamgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<RingElectronParam>[nElements] : new vector<RingElectronParam>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlERingElectronParamgR(void *p) {
      delete ((vector<RingElectronParam>*)p);
   }
   static void deleteArray_vectorlERingElectronParamgR(void *p) {
      delete [] ((vector<RingElectronParam>*)p);
   }
   static void destruct_vectorlERingElectronParamgR(void *p) {
      typedef vector<RingElectronParam> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<RingElectronParam>

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
   static TClass *maplEunsignedsPintcOdoublegR_Dictionary();
   static void maplEunsignedsPintcOdoublegR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOdoublegR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOdoublegR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOdoublegR(void *p);
   static void deleteArray_maplEunsignedsPintcOdoublegR(void *p);
   static void destruct_maplEunsignedsPintcOdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,double>*)
   {
      map<unsigned int,double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,double>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,double>", -2, "map", 100,
                  typeid(map<unsigned int,double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,double>) );
      instance.SetNew(&new_maplEunsignedsPintcOdoublegR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOdoublegR);
      instance.SetDelete(&delete_maplEunsignedsPintcOdoublegR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOdoublegR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,double> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,double>","std::map<unsigned int, double, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, double> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,double>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,double>*)nullptr)->GetClass();
      maplEunsignedsPintcOdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,double> : new map<unsigned int,double>;
   }
   static void *newArray_maplEunsignedsPintcOdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,double>[nElements] : new map<unsigned int,double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOdoublegR(void *p) {
      delete ((map<unsigned int,double>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOdoublegR(void *p) {
      delete [] ((map<unsigned int,double>*)p);
   }
   static void destruct_maplEunsignedsPintcOdoublegR(void *p) {
      typedef map<unsigned int,double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,double>

namespace ROOT {
   static TClass *maplEpairlEintcOintgRcOintgR_Dictionary();
   static void maplEpairlEintcOintgRcOintgR_TClassManip(TClass*);
   static void *new_maplEpairlEintcOintgRcOintgR(void *p = nullptr);
   static void *newArray_maplEpairlEintcOintgRcOintgR(Long_t size, void *p);
   static void delete_maplEpairlEintcOintgRcOintgR(void *p);
   static void deleteArray_maplEpairlEintcOintgRcOintgR(void *p);
   static void destruct_maplEpairlEintcOintgRcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<pair<int,int>,int>*)
   {
      map<pair<int,int>,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<pair<int,int>,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<pair<int,int>,int>", -2, "map", 100,
                  typeid(map<pair<int,int>,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEpairlEintcOintgRcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<pair<int,int>,int>) );
      instance.SetNew(&new_maplEpairlEintcOintgRcOintgR);
      instance.SetNewArray(&newArray_maplEpairlEintcOintgRcOintgR);
      instance.SetDelete(&delete_maplEpairlEintcOintgRcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEpairlEintcOintgRcOintgR);
      instance.SetDestructor(&destruct_maplEpairlEintcOintgRcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<pair<int,int>,int> >()));

      ::ROOT::AddClassAlternate("map<pair<int,int>,int>","std::map<std::pair<int, int>, int, std::less<std::pair<int, int> >, std::allocator<std::pair<std::pair<int, int> const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<pair<int,int>,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEpairlEintcOintgRcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<pair<int,int>,int>*)nullptr)->GetClass();
      maplEpairlEintcOintgRcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEpairlEintcOintgRcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEpairlEintcOintgRcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,int>,int> : new map<pair<int,int>,int>;
   }
   static void *newArray_maplEpairlEintcOintgRcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,int>,int>[nElements] : new map<pair<int,int>,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEpairlEintcOintgRcOintgR(void *p) {
      delete ((map<pair<int,int>,int>*)p);
   }
   static void deleteArray_maplEpairlEintcOintgRcOintgR(void *p) {
      delete [] ((map<pair<int,int>,int>*)p);
   }
   static void destruct_maplEpairlEintcOintgRcOintgR(void *p) {
      typedef map<pair<int,int>,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<pair<int,int>,int>

namespace ROOT {
   static TClass *maplEintcOmaplEintcOTH1mUgRsPgR_Dictionary();
   static void maplEintcOmaplEintcOTH1mUgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOmaplEintcOTH1mUgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOmaplEintcOTH1mUgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOmaplEintcOTH1mUgRsPgR(void *p);
   static void deleteArray_maplEintcOmaplEintcOTH1mUgRsPgR(void *p);
   static void destruct_maplEintcOmaplEintcOTH1mUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,map<int,TH1*> >*)
   {
      map<int,map<int,TH1*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,map<int,TH1*> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,map<int,TH1*> >", -2, "map", 100,
                  typeid(map<int,map<int,TH1*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOmaplEintcOTH1mUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,map<int,TH1*> >) );
      instance.SetNew(&new_maplEintcOmaplEintcOTH1mUgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOmaplEintcOTH1mUgRsPgR);
      instance.SetDelete(&delete_maplEintcOmaplEintcOTH1mUgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOmaplEintcOTH1mUgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOmaplEintcOTH1mUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,map<int,TH1*> > >()));

      ::ROOT::AddClassAlternate("map<int,map<int,TH1*> >","std::map<int, std::map<int, TH1*, std::less<int>, std::allocator<std::pair<int const, TH1*> > >, std::less<int>, std::allocator<std::pair<int const, std::map<int, TH1*, std::less<int>, std::allocator<std::pair<int const, TH1*> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,map<int,TH1*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOmaplEintcOTH1mUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,map<int,TH1*> >*)nullptr)->GetClass();
      maplEintcOmaplEintcOTH1mUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOmaplEintcOTH1mUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOmaplEintcOTH1mUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,map<int,TH1*> > : new map<int,map<int,TH1*> >;
   }
   static void *newArray_maplEintcOmaplEintcOTH1mUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,map<int,TH1*> >[nElements] : new map<int,map<int,TH1*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOmaplEintcOTH1mUgRsPgR(void *p) {
      delete ((map<int,map<int,TH1*> >*)p);
   }
   static void deleteArray_maplEintcOmaplEintcOTH1mUgRsPgR(void *p) {
      delete [] ((map<int,map<int,TH1*> >*)p);
   }
   static void destruct_maplEintcOmaplEintcOTH1mUgRsPgR(void *p) {
      typedef map<int,map<int,TH1*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,map<int,TH1*> >

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
   static TClass *maplEintcOCbmRichMCbmMappingDatagR_Dictionary();
   static void maplEintcOCbmRichMCbmMappingDatagR_TClassManip(TClass*);
   static void *new_maplEintcOCbmRichMCbmMappingDatagR(void *p = nullptr);
   static void *newArray_maplEintcOCbmRichMCbmMappingDatagR(Long_t size, void *p);
   static void delete_maplEintcOCbmRichMCbmMappingDatagR(void *p);
   static void deleteArray_maplEintcOCbmRichMCbmMappingDatagR(void *p);
   static void destruct_maplEintcOCbmRichMCbmMappingDatagR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,CbmRichMCbmMappingData>*)
   {
      map<int,CbmRichMCbmMappingData> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,CbmRichMCbmMappingData>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,CbmRichMCbmMappingData>", -2, "map", 100,
                  typeid(map<int,CbmRichMCbmMappingData>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOCbmRichMCbmMappingDatagR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,CbmRichMCbmMappingData>) );
      instance.SetNew(&new_maplEintcOCbmRichMCbmMappingDatagR);
      instance.SetNewArray(&newArray_maplEintcOCbmRichMCbmMappingDatagR);
      instance.SetDelete(&delete_maplEintcOCbmRichMCbmMappingDatagR);
      instance.SetDeleteArray(&deleteArray_maplEintcOCbmRichMCbmMappingDatagR);
      instance.SetDestructor(&destruct_maplEintcOCbmRichMCbmMappingDatagR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,CbmRichMCbmMappingData> >()));

      ::ROOT::AddClassAlternate("map<int,CbmRichMCbmMappingData>","std::map<int, CbmRichMCbmMappingData, std::less<int>, std::allocator<std::pair<int const, CbmRichMCbmMappingData> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,CbmRichMCbmMappingData>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOCbmRichMCbmMappingDatagR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,CbmRichMCbmMappingData>*)nullptr)->GetClass();
      maplEintcOCbmRichMCbmMappingDatagR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOCbmRichMCbmMappingDatagR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOCbmRichMCbmMappingDatagR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmRichMCbmMappingData> : new map<int,CbmRichMCbmMappingData>;
   }
   static void *newArray_maplEintcOCbmRichMCbmMappingDatagR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmRichMCbmMappingData>[nElements] : new map<int,CbmRichMCbmMappingData>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOCbmRichMCbmMappingDatagR(void *p) {
      delete ((map<int,CbmRichMCbmMappingData>*)p);
   }
   static void deleteArray_maplEintcOCbmRichMCbmMappingDatagR(void *p) {
      delete [] ((map<int,CbmRichMCbmMappingData>*)p);
   }
   static void destruct_maplEintcOCbmRichMCbmMappingDatagR(void *p) {
      typedef map<int,CbmRichMCbmMappingData> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,CbmRichMCbmMappingData>

namespace {
  void TriggerDictionaryInitialization_libCbmRichReco_Impl() {
    static const char* headers[] = {
"CbmRichTrainAnnSelect.h",
"CbmRichTrainAnnElectrons.h",
"CbmRichReconstruction.h",
"CbmRichHitProducer.h",
"CbmRichMCbmQa.h",
"CbmRichMCbmQaReal.h",
"CbmRichMCbmQaRichOnly.h",
"CbmRichMCbmHitProducer.h",
"CbmRichMCbmAerogelAna.h",
"CbmRichMCbmToTShifter.h",
"CbmRichMCbmSEDisplay.h",
"CbmRichRingFitterQa.h",
"CbmRichGeoOpt.h",
"CbmRichRecoTbQa.h",
"CbmRichGeoTest.h",
"CbmRichUrqmdTest.h",
"CbmRichGeoTestOpt.h",
"CbmRichRecoQa.h",
"CbmL1RichRingQa.h",
"CbmRichUnpackAlgo.h",
"CbmRichUnpackAlgo2022.h",
"CbmRichUnpackAlgoBase.h",
"CbmRichUnpackMonitor.h",
"CbmRichUnpackConfig.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/finder",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/fitter",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/tracks",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/selection",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/alignment",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/mcbm",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/unpack",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/finder",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/fitter",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/tracks",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/selection",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/alignment",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/mcbm",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/unpack",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich/utils",
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
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
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
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/steer",
"/home/heinemann/gpuunpacker/cbmroot/core/config",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/geosetup",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/detectors/rich/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmRichReco dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmRichTrainAnnSelect.h")))  CbmRichTrainAnnSelect;
class __attribute__((annotate("$clingAutoload$CbmRichTrainAnnElectrons.h")))  CbmRichTrainAnnElectrons;
class __attribute__((annotate("$clingAutoload$CbmRichReconstruction.h")))  CbmRichReconstruction;
class __attribute__((annotate("$clingAutoload$CbmRichHitProducer.h")))  CbmRichHitProducer;
class __attribute__((annotate("$clingAutoload$CbmRichMCbmQa.h")))  CbmRichMCbmQa;
class __attribute__((annotate("$clingAutoload$CbmRichMCbmQaReal.h")))  CbmRichMCbmQaReal;
class __attribute__((annotate("$clingAutoload$CbmRichMCbmQaRichOnly.h")))  CbmRichMCbmQaRichOnly;
class __attribute__((annotate("$clingAutoload$CbmRichMCbmHitProducer.h")))  CbmRichMCbmHitProducer;
class __attribute__((annotate("$clingAutoload$CbmRichMCbmAerogelAna.h")))  CbmRichMCbmAerogelAna;
class __attribute__((annotate("$clingAutoload$CbmRichMCbmToTShifter.h")))  CbmRichMCbmToTShifter;
class __attribute__((annotate("$clingAutoload$CbmRichMCbmSEDisplay.h")))  CbmRichMCbmSEDisplay;
class __attribute__((annotate("$clingAutoload$CbmRichRingFitterQa.h")))  CbmRichRingFitterQa;
class __attribute__((annotate("$clingAutoload$CbmRichGeoOpt.h")))  CbmRichGeoOpt;
class __attribute__((annotate("$clingAutoload$CbmRichRecoTbQa.h")))  CbmRichRecoTbQa;
class __attribute__((annotate("$clingAutoload$CbmRichGeoTest.h")))  CbmRichGeoTest;
class __attribute__((annotate("$clingAutoload$CbmRichUrqmdTest.h")))  CbmRichUrqmdTest;
class __attribute__((annotate("$clingAutoload$CbmRichGeoTestOpt.h")))  CbmRichGeoTestOpt;
class __attribute__((annotate("$clingAutoload$CbmRichRecoQa.h")))  CbmRichRecoQa;
class __attribute__((annotate("$clingAutoload$CbmL1RichRingQa.h")))  CbmL1RichRingQa;
class __attribute__((annotate("$clingAutoload$CbmRichUnpackMonitor.h")))  __attribute__((annotate("$clingAutoload$CbmRichUnpackAlgo.h")))  CbmRichUnpackMonitor;
class __attribute__((annotate("$clingAutoload$CbmRichUnpackAlgoBase.h")))  __attribute__((annotate("$clingAutoload$CbmRichUnpackAlgo.h")))  CbmRichUnpackAlgoBase;
class __attribute__((annotate("$clingAutoload$CbmRichUnpackAlgo.h")))  CbmRichUnpackAlgo;
class __attribute__((annotate("$clingAutoload$CbmRichUnpackAlgo2022.h")))  CbmRichUnpackAlgo2022;
class __attribute__((annotate("$clingAutoload$CbmRichUnpackConfig.h")))  CbmRichUnpackConfig;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmRichReco dictionary payload"

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
#include "CbmRichTrainAnnSelect.h"
#include "CbmRichTrainAnnElectrons.h"
#include "CbmRichReconstruction.h"
#include "CbmRichHitProducer.h"
#include "CbmRichMCbmQa.h"
#include "CbmRichMCbmQaReal.h"
#include "CbmRichMCbmQaRichOnly.h"
#include "CbmRichMCbmHitProducer.h"
#include "CbmRichMCbmAerogelAna.h"
#include "CbmRichMCbmToTShifter.h"
#include "CbmRichMCbmSEDisplay.h"
#include "CbmRichRingFitterQa.h"
#include "CbmRichGeoOpt.h"
#include "CbmRichRecoTbQa.h"
#include "CbmRichGeoTest.h"
#include "CbmRichUrqmdTest.h"
#include "CbmRichGeoTestOpt.h"
#include "CbmRichRecoQa.h"
#include "CbmL1RichRingQa.h"
#include "CbmRichUnpackAlgo.h"
#include "CbmRichUnpackAlgo2022.h"
#include "CbmRichUnpackAlgoBase.h"
#include "CbmRichUnpackMonitor.h"
#include "CbmRichUnpackConfig.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmL1RichRingQa", payloadCode, "@",
"CbmRichGeoOpt", payloadCode, "@",
"CbmRichGeoTest", payloadCode, "@",
"CbmRichGeoTestOpt", payloadCode, "@",
"CbmRichHitProducer", payloadCode, "@",
"CbmRichMCbmAerogelAna", payloadCode, "@",
"CbmRichMCbmHitProducer", payloadCode, "@",
"CbmRichMCbmQa", payloadCode, "@",
"CbmRichMCbmQaReal", payloadCode, "@",
"CbmRichMCbmQaRichOnly", payloadCode, "@",
"CbmRichMCbmSEDisplay", payloadCode, "@",
"CbmRichMCbmToTShifter", payloadCode, "@",
"CbmRichRecoQa", payloadCode, "@",
"CbmRichRecoTbQa", payloadCode, "@",
"CbmRichReconstruction", payloadCode, "@",
"CbmRichRingFitterQa", payloadCode, "@",
"CbmRichTrainAnnElectrons", payloadCode, "@",
"CbmRichTrainAnnSelect", payloadCode, "@",
"CbmRichUnpackAlgo", payloadCode, "@",
"CbmRichUnpackAlgo2022", payloadCode, "@",
"CbmRichUnpackAlgoBase", payloadCode, "@",
"CbmRichUnpackConfig", payloadCode, "@",
"CbmRichUnpackMonitor", payloadCode, "@",
"CbmRichUrqmdTest", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmRichReco",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmRichReco_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmRichReco_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmRichReco() {
  TriggerDictionaryInitialization_libCbmRichReco_Impl();
}
