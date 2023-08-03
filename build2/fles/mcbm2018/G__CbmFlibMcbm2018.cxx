// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmFlibMcbm2018
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
#include "CbmMcbmUnpack.h"
#include "CbmMcbm2018Source.h"
#include "CbmMcbm2018MsCrcCheck.h"
#include "CbmMcbm2018RawConverterSdpb.h"
#include "CbmMcbm2018RawConverterGdpb.h"
#include "CbmCriGet4RawPrint.h"
#include "CbmMcbm2018UnpackerAlgoSts.h"
#include "CbmMcbm2018UnpackerTaskSts.h"
#include "CbmMcbm2018UnpackerAlgoMuch.h"
#include "CbmMcbm2018UnpackerTaskMuch.h"
#include "CbmMcbm2018UnpackerAlgoTof.h"
#include "CbmMcbm2018UnpackerTaskTof.h"
#include "CbmMcbm2018UnpackerAlgoRich.h"
#include "CbmMcbm2018UnpackerTaskRich.h"
#include "CbmMcbm2018UnpackerTaskRich2020.h"
#include "CbmMcbm2018UnpackerAlgoRich2020.h"
#include "CbmMcbm2018UnpackerUtilRich2020.h"
#include "CbmMcbm2018UnpackerAlgoPsd.h"
#include "CbmMcbm2018UnpackerTaskPsd.h"
#include "CbmMcbm2018UnpackerAlgoTrdR.h"
#include "CbmMcbm2018UnpackerTaskTrdR.h"
#include "CbmCheckDataFormatGdpb2018.h"
#include "CbmMcbm2018MonitorDataRates.h"
#include "CbmMcbm2018MonitorAlgoT0.h"
#include "CbmMcbm2018MonitorTaskT0.h"
#include "CbmMcbm2018MonitorMcbmRate.h"
#include "CbmMcbm2018MonitorMcbmSync.h"
#include "CbmMcbm2018MonitorMcbmPulser.h"
#include "CbmMcbm2018MonitorSts.h"
#include "CbmMcbm2018MonitorMuch.h"
#include "CbmMcbm2018MonitorMuchLite.h"
#include "CbmMcbm2018MonitorAlgoMuchLite.h"
#include "CbmMcbm2018MonitorTaskMuchLite.h"
#include "CbmMcbm2018MonitorTof.h"
#include "CbmMcbm2018MonitorTofPulser.h"
#include "CbmMcbm2018MonitorAlgoTof.h"
#include "CbmMcbm2018MonitorTaskTof.h"
#include "CbmMcbm2018MonitorAlgoTofPulser.h"
#include "CbmMcbm2018MonitorTaskTofPulser.h"
#include "CbmMcbm2018TofTestFee.h"
#include "CbmMcbm2018TofFeeThr.h"
#include "CbmMcbm2018MonitorAlgoPsd.h"
#include "CbmMcbm2018MonitorTaskPsd.h"
#include "CbmCheckDigisNbCorr.h"
#include "CbmCheckTiming.h"
#include "CbmCheckEvents.h"
#include "CbmMcbm2018EventBuilder.h"
#include "CbmMcbm2019CheckPulser.h"
#include "CbmMcbm2019CheckDigisMuch.h"
#include "CbmMcbm2019CheckDigisSts.h"
#include "CbmMcbm2019CheckDtInDet.h"
#include "CbmMcbm2019CheckTimingPairs.h"
#include "CbmMcbm2019TimeWinEventBuilderAlgo.h"
#include "CbmMcbm2019TimeWinEventBuilderTask.h"
#include "CbmMcbmCheckTimingAlgo.h"
#include "CbmMcbmCheckTimingTask.h"
#include "CbmMcbmSpillFindAlgo.h"
#include "CbmMcbmSpillFindTask.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_CbmMcbmUnpack(void *p);
   static void deleteArray_CbmMcbmUnpack(void *p);
   static void destruct_CbmMcbmUnpack(void *p);
   static void streamer_CbmMcbmUnpack(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbmUnpack*)
   {
      ::CbmMcbmUnpack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbmUnpack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbmUnpack", ::CbmMcbmUnpack::Class_Version(), "CbmMcbmUnpack.h", 19,
                  typeid(::CbmMcbmUnpack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbmUnpack::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbmUnpack) );
      instance.SetDelete(&delete_CbmMcbmUnpack);
      instance.SetDeleteArray(&deleteArray_CbmMcbmUnpack);
      instance.SetDestructor(&destruct_CbmMcbmUnpack);
      instance.SetStreamerFunc(&streamer_CbmMcbmUnpack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbmUnpack*)
   {
      return GenerateInitInstanceLocal((::CbmMcbmUnpack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbmUnpack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018Source(void *p = nullptr);
   static void *newArray_CbmMcbm2018Source(Long_t size, void *p);
   static void delete_CbmMcbm2018Source(void *p);
   static void deleteArray_CbmMcbm2018Source(void *p);
   static void destruct_CbmMcbm2018Source(void *p);
   static void streamer_CbmMcbm2018Source(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018Source*)
   {
      ::CbmMcbm2018Source *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018Source >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018Source", ::CbmMcbm2018Source::Class_Version(), "CbmMcbm2018Source.h", 38,
                  typeid(::CbmMcbm2018Source), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018Source::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018Source) );
      instance.SetNew(&new_CbmMcbm2018Source);
      instance.SetNewArray(&newArray_CbmMcbm2018Source);
      instance.SetDelete(&delete_CbmMcbm2018Source);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018Source);
      instance.SetDestructor(&destruct_CbmMcbm2018Source);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018Source);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018Source*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018Source*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018Source*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MsCrcCheck(void *p = nullptr);
   static void *newArray_CbmMcbm2018MsCrcCheck(Long_t size, void *p);
   static void delete_CbmMcbm2018MsCrcCheck(void *p);
   static void deleteArray_CbmMcbm2018MsCrcCheck(void *p);
   static void destruct_CbmMcbm2018MsCrcCheck(void *p);
   static void streamer_CbmMcbm2018MsCrcCheck(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MsCrcCheck*)
   {
      ::CbmMcbm2018MsCrcCheck *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MsCrcCheck >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MsCrcCheck", ::CbmMcbm2018MsCrcCheck::Class_Version(), "CbmMcbm2018MsCrcCheck.h", 21,
                  typeid(::CbmMcbm2018MsCrcCheck), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MsCrcCheck::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MsCrcCheck) );
      instance.SetNew(&new_CbmMcbm2018MsCrcCheck);
      instance.SetNewArray(&newArray_CbmMcbm2018MsCrcCheck);
      instance.SetDelete(&delete_CbmMcbm2018MsCrcCheck);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MsCrcCheck);
      instance.SetDestructor(&destruct_CbmMcbm2018MsCrcCheck);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MsCrcCheck);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MsCrcCheck*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MsCrcCheck*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MsCrcCheck*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018RawConverterSdpb(void *p = nullptr);
   static void *newArray_CbmMcbm2018RawConverterSdpb(Long_t size, void *p);
   static void delete_CbmMcbm2018RawConverterSdpb(void *p);
   static void deleteArray_CbmMcbm2018RawConverterSdpb(void *p);
   static void destruct_CbmMcbm2018RawConverterSdpb(void *p);
   static void streamer_CbmMcbm2018RawConverterSdpb(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018RawConverterSdpb*)
   {
      ::CbmMcbm2018RawConverterSdpb *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018RawConverterSdpb >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018RawConverterSdpb", ::CbmMcbm2018RawConverterSdpb::Class_Version(), "CbmMcbm2018RawConverterSdpb.h", 29,
                  typeid(::CbmMcbm2018RawConverterSdpb), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018RawConverterSdpb::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018RawConverterSdpb) );
      instance.SetNew(&new_CbmMcbm2018RawConverterSdpb);
      instance.SetNewArray(&newArray_CbmMcbm2018RawConverterSdpb);
      instance.SetDelete(&delete_CbmMcbm2018RawConverterSdpb);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018RawConverterSdpb);
      instance.SetDestructor(&destruct_CbmMcbm2018RawConverterSdpb);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018RawConverterSdpb);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018RawConverterSdpb*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018RawConverterSdpb*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018RawConverterSdpb*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018RawConverterGdpb(void *p = nullptr);
   static void *newArray_CbmMcbm2018RawConverterGdpb(Long_t size, void *p);
   static void delete_CbmMcbm2018RawConverterGdpb(void *p);
   static void deleteArray_CbmMcbm2018RawConverterGdpb(void *p);
   static void destruct_CbmMcbm2018RawConverterGdpb(void *p);
   static void streamer_CbmMcbm2018RawConverterGdpb(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018RawConverterGdpb*)
   {
      ::CbmMcbm2018RawConverterGdpb *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018RawConverterGdpb >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018RawConverterGdpb", ::CbmMcbm2018RawConverterGdpb::Class_Version(), "CbmMcbm2018RawConverterGdpb.h", 29,
                  typeid(::CbmMcbm2018RawConverterGdpb), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018RawConverterGdpb::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018RawConverterGdpb) );
      instance.SetNew(&new_CbmMcbm2018RawConverterGdpb);
      instance.SetNewArray(&newArray_CbmMcbm2018RawConverterGdpb);
      instance.SetDelete(&delete_CbmMcbm2018RawConverterGdpb);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018RawConverterGdpb);
      instance.SetDestructor(&destruct_CbmMcbm2018RawConverterGdpb);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018RawConverterGdpb);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018RawConverterGdpb*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018RawConverterGdpb*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018RawConverterGdpb*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmCriGet4RawPrint(void *p = nullptr);
   static void *newArray_CbmCriGet4RawPrint(Long_t size, void *p);
   static void delete_CbmCriGet4RawPrint(void *p);
   static void deleteArray_CbmCriGet4RawPrint(void *p);
   static void destruct_CbmCriGet4RawPrint(void *p);
   static void streamer_CbmCriGet4RawPrint(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCriGet4RawPrint*)
   {
      ::CbmCriGet4RawPrint *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCriGet4RawPrint >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCriGet4RawPrint", ::CbmCriGet4RawPrint::Class_Version(), "CbmCriGet4RawPrint.h", 22,
                  typeid(::CbmCriGet4RawPrint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCriGet4RawPrint::Dictionary, isa_proxy, 16,
                  sizeof(::CbmCriGet4RawPrint) );
      instance.SetNew(&new_CbmCriGet4RawPrint);
      instance.SetNewArray(&newArray_CbmCriGet4RawPrint);
      instance.SetDelete(&delete_CbmCriGet4RawPrint);
      instance.SetDeleteArray(&deleteArray_CbmCriGet4RawPrint);
      instance.SetDestructor(&destruct_CbmCriGet4RawPrint);
      instance.SetStreamerFunc(&streamer_CbmCriGet4RawPrint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCriGet4RawPrint*)
   {
      return GenerateInitInstanceLocal((::CbmCriGet4RawPrint*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCriGet4RawPrint*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018UnpackerAlgoSts(void *p = nullptr);
   static void *newArray_CbmMcbm2018UnpackerAlgoSts(Long_t size, void *p);
   static void delete_CbmMcbm2018UnpackerAlgoSts(void *p);
   static void deleteArray_CbmMcbm2018UnpackerAlgoSts(void *p);
   static void destruct_CbmMcbm2018UnpackerAlgoSts(void *p);
   static void streamer_CbmMcbm2018UnpackerAlgoSts(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018UnpackerAlgoSts*)
   {
      ::CbmMcbm2018UnpackerAlgoSts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018UnpackerAlgoSts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018UnpackerAlgoSts", ::CbmMcbm2018UnpackerAlgoSts::Class_Version(), "CbmMcbm2018UnpackerAlgoSts.h", 42,
                  typeid(::CbmMcbm2018UnpackerAlgoSts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018UnpackerAlgoSts::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018UnpackerAlgoSts) );
      instance.SetNew(&new_CbmMcbm2018UnpackerAlgoSts);
      instance.SetNewArray(&newArray_CbmMcbm2018UnpackerAlgoSts);
      instance.SetDelete(&delete_CbmMcbm2018UnpackerAlgoSts);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018UnpackerAlgoSts);
      instance.SetDestructor(&destruct_CbmMcbm2018UnpackerAlgoSts);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018UnpackerAlgoSts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018UnpackerAlgoSts*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018UnpackerAlgoSts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoSts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018UnpackerTaskSts(void *p = nullptr);
   static void *newArray_CbmMcbm2018UnpackerTaskSts(Long_t size, void *p);
   static void delete_CbmMcbm2018UnpackerTaskSts(void *p);
   static void deleteArray_CbmMcbm2018UnpackerTaskSts(void *p);
   static void destruct_CbmMcbm2018UnpackerTaskSts(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018UnpackerTaskSts*)
   {
      ::CbmMcbm2018UnpackerTaskSts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018UnpackerTaskSts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018UnpackerTaskSts", ::CbmMcbm2018UnpackerTaskSts::Class_Version(), "CbmMcbm2018UnpackerTaskSts.h", 30,
                  typeid(::CbmMcbm2018UnpackerTaskSts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018UnpackerTaskSts::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2018UnpackerTaskSts) );
      instance.SetNew(&new_CbmMcbm2018UnpackerTaskSts);
      instance.SetNewArray(&newArray_CbmMcbm2018UnpackerTaskSts);
      instance.SetDelete(&delete_CbmMcbm2018UnpackerTaskSts);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018UnpackerTaskSts);
      instance.SetDestructor(&destruct_CbmMcbm2018UnpackerTaskSts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018UnpackerTaskSts*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018UnpackerTaskSts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskSts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018UnpackerAlgoMuch(void *p = nullptr);
   static void *newArray_CbmMcbm2018UnpackerAlgoMuch(Long_t size, void *p);
   static void delete_CbmMcbm2018UnpackerAlgoMuch(void *p);
   static void deleteArray_CbmMcbm2018UnpackerAlgoMuch(void *p);
   static void destruct_CbmMcbm2018UnpackerAlgoMuch(void *p);
   static void streamer_CbmMcbm2018UnpackerAlgoMuch(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018UnpackerAlgoMuch*)
   {
      ::CbmMcbm2018UnpackerAlgoMuch *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018UnpackerAlgoMuch >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018UnpackerAlgoMuch", ::CbmMcbm2018UnpackerAlgoMuch::Class_Version(), "CbmMcbm2018UnpackerAlgoMuch.h", 41,
                  typeid(::CbmMcbm2018UnpackerAlgoMuch), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018UnpackerAlgoMuch::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018UnpackerAlgoMuch) );
      instance.SetNew(&new_CbmMcbm2018UnpackerAlgoMuch);
      instance.SetNewArray(&newArray_CbmMcbm2018UnpackerAlgoMuch);
      instance.SetDelete(&delete_CbmMcbm2018UnpackerAlgoMuch);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018UnpackerAlgoMuch);
      instance.SetDestructor(&destruct_CbmMcbm2018UnpackerAlgoMuch);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018UnpackerAlgoMuch);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018UnpackerAlgoMuch*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018UnpackerAlgoMuch*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoMuch*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018UnpackerTaskMuch(void *p = nullptr);
   static void *newArray_CbmMcbm2018UnpackerTaskMuch(Long_t size, void *p);
   static void delete_CbmMcbm2018UnpackerTaskMuch(void *p);
   static void deleteArray_CbmMcbm2018UnpackerTaskMuch(void *p);
   static void destruct_CbmMcbm2018UnpackerTaskMuch(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018UnpackerTaskMuch*)
   {
      ::CbmMcbm2018UnpackerTaskMuch *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018UnpackerTaskMuch >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018UnpackerTaskMuch", ::CbmMcbm2018UnpackerTaskMuch::Class_Version(), "CbmMcbm2018UnpackerTaskMuch.h", 30,
                  typeid(::CbmMcbm2018UnpackerTaskMuch), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018UnpackerTaskMuch::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2018UnpackerTaskMuch) );
      instance.SetNew(&new_CbmMcbm2018UnpackerTaskMuch);
      instance.SetNewArray(&newArray_CbmMcbm2018UnpackerTaskMuch);
      instance.SetDelete(&delete_CbmMcbm2018UnpackerTaskMuch);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018UnpackerTaskMuch);
      instance.SetDestructor(&destruct_CbmMcbm2018UnpackerTaskMuch);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018UnpackerTaskMuch*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018UnpackerTaskMuch*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskMuch*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018UnpackerAlgoTof(void *p = nullptr);
   static void *newArray_CbmMcbm2018UnpackerAlgoTof(Long_t size, void *p);
   static void delete_CbmMcbm2018UnpackerAlgoTof(void *p);
   static void deleteArray_CbmMcbm2018UnpackerAlgoTof(void *p);
   static void destruct_CbmMcbm2018UnpackerAlgoTof(void *p);
   static void streamer_CbmMcbm2018UnpackerAlgoTof(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018UnpackerAlgoTof*)
   {
      ::CbmMcbm2018UnpackerAlgoTof *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018UnpackerAlgoTof >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018UnpackerAlgoTof", ::CbmMcbm2018UnpackerAlgoTof::Class_Version(), "CbmMcbm2018UnpackerAlgoTof.h", 40,
                  typeid(::CbmMcbm2018UnpackerAlgoTof), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018UnpackerAlgoTof::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018UnpackerAlgoTof) );
      instance.SetNew(&new_CbmMcbm2018UnpackerAlgoTof);
      instance.SetNewArray(&newArray_CbmMcbm2018UnpackerAlgoTof);
      instance.SetDelete(&delete_CbmMcbm2018UnpackerAlgoTof);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018UnpackerAlgoTof);
      instance.SetDestructor(&destruct_CbmMcbm2018UnpackerAlgoTof);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018UnpackerAlgoTof);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018UnpackerAlgoTof*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018UnpackerAlgoTof*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoTof*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018UnpackerTaskTof(void *p = nullptr);
   static void *newArray_CbmMcbm2018UnpackerTaskTof(Long_t size, void *p);
   static void delete_CbmMcbm2018UnpackerTaskTof(void *p);
   static void deleteArray_CbmMcbm2018UnpackerTaskTof(void *p);
   static void destruct_CbmMcbm2018UnpackerTaskTof(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018UnpackerTaskTof*)
   {
      ::CbmMcbm2018UnpackerTaskTof *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018UnpackerTaskTof >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018UnpackerTaskTof", ::CbmMcbm2018UnpackerTaskTof::Class_Version(), "CbmMcbm2018UnpackerTaskTof.h", 24,
                  typeid(::CbmMcbm2018UnpackerTaskTof), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018UnpackerTaskTof::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2018UnpackerTaskTof) );
      instance.SetNew(&new_CbmMcbm2018UnpackerTaskTof);
      instance.SetNewArray(&newArray_CbmMcbm2018UnpackerTaskTof);
      instance.SetDelete(&delete_CbmMcbm2018UnpackerTaskTof);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018UnpackerTaskTof);
      instance.SetDestructor(&destruct_CbmMcbm2018UnpackerTaskTof);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018UnpackerTaskTof*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018UnpackerTaskTof*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskTof*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018UnpackerAlgoRich(void *p = nullptr);
   static void *newArray_CbmMcbm2018UnpackerAlgoRich(Long_t size, void *p);
   static void delete_CbmMcbm2018UnpackerAlgoRich(void *p);
   static void deleteArray_CbmMcbm2018UnpackerAlgoRich(void *p);
   static void destruct_CbmMcbm2018UnpackerAlgoRich(void *p);
   static void streamer_CbmMcbm2018UnpackerAlgoRich(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018UnpackerAlgoRich*)
   {
      ::CbmMcbm2018UnpackerAlgoRich *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018UnpackerAlgoRich >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018UnpackerAlgoRich", ::CbmMcbm2018UnpackerAlgoRich::Class_Version(), "CbmMcbm2018UnpackerAlgoRich.h", 193,
                  typeid(::CbmMcbm2018UnpackerAlgoRich), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018UnpackerAlgoRich::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018UnpackerAlgoRich) );
      instance.SetNew(&new_CbmMcbm2018UnpackerAlgoRich);
      instance.SetNewArray(&newArray_CbmMcbm2018UnpackerAlgoRich);
      instance.SetDelete(&delete_CbmMcbm2018UnpackerAlgoRich);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018UnpackerAlgoRich);
      instance.SetDestructor(&destruct_CbmMcbm2018UnpackerAlgoRich);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018UnpackerAlgoRich);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018UnpackerAlgoRich*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018UnpackerAlgoRich*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoRich*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018UnpackerTaskRich(void *p = nullptr);
   static void *newArray_CbmMcbm2018UnpackerTaskRich(Long_t size, void *p);
   static void delete_CbmMcbm2018UnpackerTaskRich(void *p);
   static void deleteArray_CbmMcbm2018UnpackerTaskRich(void *p);
   static void destruct_CbmMcbm2018UnpackerTaskRich(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018UnpackerTaskRich*)
   {
      ::CbmMcbm2018UnpackerTaskRich *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018UnpackerTaskRich >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018UnpackerTaskRich", ::CbmMcbm2018UnpackerTaskRich::Class_Version(), "CbmMcbm2018UnpackerTaskRich.h", 21,
                  typeid(::CbmMcbm2018UnpackerTaskRich), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018UnpackerTaskRich::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2018UnpackerTaskRich) );
      instance.SetNew(&new_CbmMcbm2018UnpackerTaskRich);
      instance.SetNewArray(&newArray_CbmMcbm2018UnpackerTaskRich);
      instance.SetDelete(&delete_CbmMcbm2018UnpackerTaskRich);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018UnpackerTaskRich);
      instance.SetDestructor(&destruct_CbmMcbm2018UnpackerTaskRich);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018UnpackerTaskRich*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018UnpackerTaskRich*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskRich*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018UnpackerTaskRich2020(void *p = nullptr);
   static void *newArray_CbmMcbm2018UnpackerTaskRich2020(Long_t size, void *p);
   static void delete_CbmMcbm2018UnpackerTaskRich2020(void *p);
   static void deleteArray_CbmMcbm2018UnpackerTaskRich2020(void *p);
   static void destruct_CbmMcbm2018UnpackerTaskRich2020(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018UnpackerTaskRich2020*)
   {
      ::CbmMcbm2018UnpackerTaskRich2020 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018UnpackerTaskRich2020 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018UnpackerTaskRich2020", ::CbmMcbm2018UnpackerTaskRich2020::Class_Version(), "CbmMcbm2018UnpackerTaskRich2020.h", 21,
                  typeid(::CbmMcbm2018UnpackerTaskRich2020), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018UnpackerTaskRich2020::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2018UnpackerTaskRich2020) );
      instance.SetNew(&new_CbmMcbm2018UnpackerTaskRich2020);
      instance.SetNewArray(&newArray_CbmMcbm2018UnpackerTaskRich2020);
      instance.SetDelete(&delete_CbmMcbm2018UnpackerTaskRich2020);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018UnpackerTaskRich2020);
      instance.SetDestructor(&destruct_CbmMcbm2018UnpackerTaskRich2020);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018UnpackerTaskRich2020*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018UnpackerTaskRich2020*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskRich2020*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018UnpackerAlgoRich2020(void *p = nullptr);
   static void *newArray_CbmMcbm2018UnpackerAlgoRich2020(Long_t size, void *p);
   static void delete_CbmMcbm2018UnpackerAlgoRich2020(void *p);
   static void deleteArray_CbmMcbm2018UnpackerAlgoRich2020(void *p);
   static void destruct_CbmMcbm2018UnpackerAlgoRich2020(void *p);
   static void streamer_CbmMcbm2018UnpackerAlgoRich2020(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018UnpackerAlgoRich2020*)
   {
      ::CbmMcbm2018UnpackerAlgoRich2020 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018UnpackerAlgoRich2020 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018UnpackerAlgoRich2020", ::CbmMcbm2018UnpackerAlgoRich2020::Class_Version(), "CbmMcbm2018UnpackerAlgoRich2020.h", 59,
                  typeid(::CbmMcbm2018UnpackerAlgoRich2020), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018UnpackerAlgoRich2020::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018UnpackerAlgoRich2020) );
      instance.SetNew(&new_CbmMcbm2018UnpackerAlgoRich2020);
      instance.SetNewArray(&newArray_CbmMcbm2018UnpackerAlgoRich2020);
      instance.SetDelete(&delete_CbmMcbm2018UnpackerAlgoRich2020);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018UnpackerAlgoRich2020);
      instance.SetDestructor(&destruct_CbmMcbm2018UnpackerAlgoRich2020);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018UnpackerAlgoRich2020);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018UnpackerAlgoRich2020*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018UnpackerAlgoRich2020*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoRich2020*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018UnpackerAlgoPsd(void *p = nullptr);
   static void *newArray_CbmMcbm2018UnpackerAlgoPsd(Long_t size, void *p);
   static void delete_CbmMcbm2018UnpackerAlgoPsd(void *p);
   static void deleteArray_CbmMcbm2018UnpackerAlgoPsd(void *p);
   static void destruct_CbmMcbm2018UnpackerAlgoPsd(void *p);
   static void streamer_CbmMcbm2018UnpackerAlgoPsd(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018UnpackerAlgoPsd*)
   {
      ::CbmMcbm2018UnpackerAlgoPsd *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018UnpackerAlgoPsd >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018UnpackerAlgoPsd", ::CbmMcbm2018UnpackerAlgoPsd::Class_Version(), "CbmMcbm2018UnpackerAlgoPsd.h", 38,
                  typeid(::CbmMcbm2018UnpackerAlgoPsd), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018UnpackerAlgoPsd::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018UnpackerAlgoPsd) );
      instance.SetNew(&new_CbmMcbm2018UnpackerAlgoPsd);
      instance.SetNewArray(&newArray_CbmMcbm2018UnpackerAlgoPsd);
      instance.SetDelete(&delete_CbmMcbm2018UnpackerAlgoPsd);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018UnpackerAlgoPsd);
      instance.SetDestructor(&destruct_CbmMcbm2018UnpackerAlgoPsd);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018UnpackerAlgoPsd);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018UnpackerAlgoPsd*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018UnpackerAlgoPsd*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoPsd*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018UnpackerTaskPsd(void *p = nullptr);
   static void *newArray_CbmMcbm2018UnpackerTaskPsd(Long_t size, void *p);
   static void delete_CbmMcbm2018UnpackerTaskPsd(void *p);
   static void deleteArray_CbmMcbm2018UnpackerTaskPsd(void *p);
   static void destruct_CbmMcbm2018UnpackerTaskPsd(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018UnpackerTaskPsd*)
   {
      ::CbmMcbm2018UnpackerTaskPsd *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018UnpackerTaskPsd >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018UnpackerTaskPsd", ::CbmMcbm2018UnpackerTaskPsd::Class_Version(), "CbmMcbm2018UnpackerTaskPsd.h", 26,
                  typeid(::CbmMcbm2018UnpackerTaskPsd), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018UnpackerTaskPsd::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2018UnpackerTaskPsd) );
      instance.SetNew(&new_CbmMcbm2018UnpackerTaskPsd);
      instance.SetNewArray(&newArray_CbmMcbm2018UnpackerTaskPsd);
      instance.SetDelete(&delete_CbmMcbm2018UnpackerTaskPsd);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018UnpackerTaskPsd);
      instance.SetDestructor(&destruct_CbmMcbm2018UnpackerTaskPsd);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018UnpackerTaskPsd*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018UnpackerTaskPsd*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskPsd*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018UnpackerAlgoTrdR(void *p = nullptr);
   static void *newArray_CbmMcbm2018UnpackerAlgoTrdR(Long_t size, void *p);
   static void delete_CbmMcbm2018UnpackerAlgoTrdR(void *p);
   static void deleteArray_CbmMcbm2018UnpackerAlgoTrdR(void *p);
   static void destruct_CbmMcbm2018UnpackerAlgoTrdR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018UnpackerAlgoTrdR*)
   {
      ::CbmMcbm2018UnpackerAlgoTrdR *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018UnpackerAlgoTrdR >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018UnpackerAlgoTrdR", ::CbmMcbm2018UnpackerAlgoTrdR::Class_Version(), "CbmMcbm2018UnpackerAlgoTrdR.h", 34,
                  typeid(::CbmMcbm2018UnpackerAlgoTrdR), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018UnpackerAlgoTrdR::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2018UnpackerAlgoTrdR) );
      instance.SetNew(&new_CbmMcbm2018UnpackerAlgoTrdR);
      instance.SetNewArray(&newArray_CbmMcbm2018UnpackerAlgoTrdR);
      instance.SetDelete(&delete_CbmMcbm2018UnpackerAlgoTrdR);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018UnpackerAlgoTrdR);
      instance.SetDestructor(&destruct_CbmMcbm2018UnpackerAlgoTrdR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018UnpackerAlgoTrdR*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018UnpackerAlgoTrdR*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoTrdR*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018UnpackerTaskTrdR(void *p = nullptr);
   static void *newArray_CbmMcbm2018UnpackerTaskTrdR(Long_t size, void *p);
   static void delete_CbmMcbm2018UnpackerTaskTrdR(void *p);
   static void deleteArray_CbmMcbm2018UnpackerTaskTrdR(void *p);
   static void destruct_CbmMcbm2018UnpackerTaskTrdR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018UnpackerTaskTrdR*)
   {
      ::CbmMcbm2018UnpackerTaskTrdR *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018UnpackerTaskTrdR >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018UnpackerTaskTrdR", ::CbmMcbm2018UnpackerTaskTrdR::Class_Version(), "CbmMcbm2018UnpackerTaskTrdR.h", 29,
                  typeid(::CbmMcbm2018UnpackerTaskTrdR), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018UnpackerTaskTrdR::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2018UnpackerTaskTrdR) );
      instance.SetNew(&new_CbmMcbm2018UnpackerTaskTrdR);
      instance.SetNewArray(&newArray_CbmMcbm2018UnpackerTaskTrdR);
      instance.SetDelete(&delete_CbmMcbm2018UnpackerTaskTrdR);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018UnpackerTaskTrdR);
      instance.SetDestructor(&destruct_CbmMcbm2018UnpackerTaskTrdR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018UnpackerTaskTrdR*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018UnpackerTaskTrdR*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskTrdR*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmCheckDataFormatGdpb2018(void *p = nullptr);
   static void *newArray_CbmCheckDataFormatGdpb2018(Long_t size, void *p);
   static void delete_CbmCheckDataFormatGdpb2018(void *p);
   static void deleteArray_CbmCheckDataFormatGdpb2018(void *p);
   static void destruct_CbmCheckDataFormatGdpb2018(void *p);
   static void streamer_CbmCheckDataFormatGdpb2018(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCheckDataFormatGdpb2018*)
   {
      ::CbmCheckDataFormatGdpb2018 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCheckDataFormatGdpb2018 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCheckDataFormatGdpb2018", ::CbmCheckDataFormatGdpb2018::Class_Version(), "CbmCheckDataFormatGdpb2018.h", 38,
                  typeid(::CbmCheckDataFormatGdpb2018), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCheckDataFormatGdpb2018::Dictionary, isa_proxy, 16,
                  sizeof(::CbmCheckDataFormatGdpb2018) );
      instance.SetNew(&new_CbmCheckDataFormatGdpb2018);
      instance.SetNewArray(&newArray_CbmCheckDataFormatGdpb2018);
      instance.SetDelete(&delete_CbmCheckDataFormatGdpb2018);
      instance.SetDeleteArray(&deleteArray_CbmCheckDataFormatGdpb2018);
      instance.SetDestructor(&destruct_CbmCheckDataFormatGdpb2018);
      instance.SetStreamerFunc(&streamer_CbmCheckDataFormatGdpb2018);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCheckDataFormatGdpb2018*)
   {
      return GenerateInitInstanceLocal((::CbmCheckDataFormatGdpb2018*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCheckDataFormatGdpb2018*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorDataRates(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorDataRates(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorDataRates(void *p);
   static void deleteArray_CbmMcbm2018MonitorDataRates(void *p);
   static void destruct_CbmMcbm2018MonitorDataRates(void *p);
   static void streamer_CbmMcbm2018MonitorDataRates(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorDataRates*)
   {
      ::CbmMcbm2018MonitorDataRates *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorDataRates >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorDataRates", ::CbmMcbm2018MonitorDataRates::Class_Version(), "CbmMcbm2018MonitorDataRates.h", 34,
                  typeid(::CbmMcbm2018MonitorDataRates), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorDataRates::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorDataRates) );
      instance.SetNew(&new_CbmMcbm2018MonitorDataRates);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorDataRates);
      instance.SetDelete(&delete_CbmMcbm2018MonitorDataRates);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorDataRates);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorDataRates);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorDataRates);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorDataRates*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorDataRates*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorDataRates*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorAlgoT0(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorAlgoT0(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorAlgoT0(void *p);
   static void deleteArray_CbmMcbm2018MonitorAlgoT0(void *p);
   static void destruct_CbmMcbm2018MonitorAlgoT0(void *p);
   static void streamer_CbmMcbm2018MonitorAlgoT0(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorAlgoT0*)
   {
      ::CbmMcbm2018MonitorAlgoT0 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorAlgoT0 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorAlgoT0", ::CbmMcbm2018MonitorAlgoT0::Class_Version(), "CbmMcbm2018MonitorAlgoT0.h", 40,
                  typeid(::CbmMcbm2018MonitorAlgoT0), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorAlgoT0::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorAlgoT0) );
      instance.SetNew(&new_CbmMcbm2018MonitorAlgoT0);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorAlgoT0);
      instance.SetDelete(&delete_CbmMcbm2018MonitorAlgoT0);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorAlgoT0);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorAlgoT0);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorAlgoT0);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorAlgoT0*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorAlgoT0*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoT0*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorTaskT0(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorTaskT0(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorTaskT0(void *p);
   static void deleteArray_CbmMcbm2018MonitorTaskT0(void *p);
   static void destruct_CbmMcbm2018MonitorTaskT0(void *p);
   static void streamer_CbmMcbm2018MonitorTaskT0(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorTaskT0*)
   {
      ::CbmMcbm2018MonitorTaskT0 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorTaskT0 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorTaskT0", ::CbmMcbm2018MonitorTaskT0::Class_Version(), "CbmMcbm2018MonitorTaskT0.h", 22,
                  typeid(::CbmMcbm2018MonitorTaskT0), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorTaskT0::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorTaskT0) );
      instance.SetNew(&new_CbmMcbm2018MonitorTaskT0);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorTaskT0);
      instance.SetDelete(&delete_CbmMcbm2018MonitorTaskT0);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorTaskT0);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorTaskT0);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorTaskT0);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorTaskT0*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorTaskT0*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskT0*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorMcbmRate(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorMcbmRate(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorMcbmRate(void *p);
   static void deleteArray_CbmMcbm2018MonitorMcbmRate(void *p);
   static void destruct_CbmMcbm2018MonitorMcbmRate(void *p);
   static void streamer_CbmMcbm2018MonitorMcbmRate(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorMcbmRate*)
   {
      ::CbmMcbm2018MonitorMcbmRate *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorMcbmRate >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorMcbmRate", ::CbmMcbm2018MonitorMcbmRate::Class_Version(), "CbmMcbm2018MonitorMcbmRate.h", 38,
                  typeid(::CbmMcbm2018MonitorMcbmRate), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorMcbmRate::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorMcbmRate) );
      instance.SetNew(&new_CbmMcbm2018MonitorMcbmRate);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorMcbmRate);
      instance.SetDelete(&delete_CbmMcbm2018MonitorMcbmRate);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorMcbmRate);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorMcbmRate);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorMcbmRate);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorMcbmRate*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorMcbmRate*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmRate*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorMcbmSync(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorMcbmSync(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorMcbmSync(void *p);
   static void deleteArray_CbmMcbm2018MonitorMcbmSync(void *p);
   static void destruct_CbmMcbm2018MonitorMcbmSync(void *p);
   static void streamer_CbmMcbm2018MonitorMcbmSync(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorMcbmSync*)
   {
      ::CbmMcbm2018MonitorMcbmSync *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorMcbmSync >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorMcbmSync", ::CbmMcbm2018MonitorMcbmSync::Class_Version(), "CbmMcbm2018MonitorMcbmSync.h", 38,
                  typeid(::CbmMcbm2018MonitorMcbmSync), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorMcbmSync::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorMcbmSync) );
      instance.SetNew(&new_CbmMcbm2018MonitorMcbmSync);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorMcbmSync);
      instance.SetDelete(&delete_CbmMcbm2018MonitorMcbmSync);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorMcbmSync);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorMcbmSync);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorMcbmSync);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorMcbmSync*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorMcbmSync*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmSync*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorMcbmPulser(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorMcbmPulser(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorMcbmPulser(void *p);
   static void deleteArray_CbmMcbm2018MonitorMcbmPulser(void *p);
   static void destruct_CbmMcbm2018MonitorMcbmPulser(void *p);
   static void streamer_CbmMcbm2018MonitorMcbmPulser(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorMcbmPulser*)
   {
      ::CbmMcbm2018MonitorMcbmPulser *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorMcbmPulser >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorMcbmPulser", ::CbmMcbm2018MonitorMcbmPulser::Class_Version(), "CbmMcbm2018MonitorMcbmPulser.h", 40,
                  typeid(::CbmMcbm2018MonitorMcbmPulser), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorMcbmPulser::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorMcbmPulser) );
      instance.SetNew(&new_CbmMcbm2018MonitorMcbmPulser);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorMcbmPulser);
      instance.SetDelete(&delete_CbmMcbm2018MonitorMcbmPulser);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorMcbmPulser);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorMcbmPulser);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorMcbmPulser);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorMcbmPulser*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorMcbmPulser*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmPulser*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorSts(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorSts(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorSts(void *p);
   static void deleteArray_CbmMcbm2018MonitorSts(void *p);
   static void destruct_CbmMcbm2018MonitorSts(void *p);
   static void streamer_CbmMcbm2018MonitorSts(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorSts*)
   {
      ::CbmMcbm2018MonitorSts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorSts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorSts", ::CbmMcbm2018MonitorSts::Class_Version(), "CbmMcbm2018MonitorSts.h", 36,
                  typeid(::CbmMcbm2018MonitorSts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorSts::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorSts) );
      instance.SetNew(&new_CbmMcbm2018MonitorSts);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorSts);
      instance.SetDelete(&delete_CbmMcbm2018MonitorSts);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorSts);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorSts);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorSts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorSts*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorSts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorSts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorMuch(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorMuch(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorMuch(void *p);
   static void deleteArray_CbmMcbm2018MonitorMuch(void *p);
   static void destruct_CbmMcbm2018MonitorMuch(void *p);
   static void streamer_CbmMcbm2018MonitorMuch(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorMuch*)
   {
      ::CbmMcbm2018MonitorMuch *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorMuch >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorMuch", ::CbmMcbm2018MonitorMuch::Class_Version(), "CbmMcbm2018MonitorMuch.h", 39,
                  typeid(::CbmMcbm2018MonitorMuch), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorMuch::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorMuch) );
      instance.SetNew(&new_CbmMcbm2018MonitorMuch);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorMuch);
      instance.SetDelete(&delete_CbmMcbm2018MonitorMuch);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorMuch);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorMuch);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorMuch);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorMuch*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorMuch*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMuch*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorMuchLite(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorMuchLite(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorMuchLite(void *p);
   static void deleteArray_CbmMcbm2018MonitorMuchLite(void *p);
   static void destruct_CbmMcbm2018MonitorMuchLite(void *p);
   static void streamer_CbmMcbm2018MonitorMuchLite(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorMuchLite*)
   {
      ::CbmMcbm2018MonitorMuchLite *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorMuchLite >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorMuchLite", ::CbmMcbm2018MonitorMuchLite::Class_Version(), "CbmMcbm2018MonitorMuchLite.h", 40,
                  typeid(::CbmMcbm2018MonitorMuchLite), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorMuchLite::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorMuchLite) );
      instance.SetNew(&new_CbmMcbm2018MonitorMuchLite);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorMuchLite);
      instance.SetDelete(&delete_CbmMcbm2018MonitorMuchLite);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorMuchLite);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorMuchLite);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorMuchLite);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorMuchLite*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorMuchLite*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMuchLite*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorAlgoMuchLite(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorAlgoMuchLite(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorAlgoMuchLite(void *p);
   static void deleteArray_CbmMcbm2018MonitorAlgoMuchLite(void *p);
   static void destruct_CbmMcbm2018MonitorAlgoMuchLite(void *p);
   static void streamer_CbmMcbm2018MonitorAlgoMuchLite(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorAlgoMuchLite*)
   {
      ::CbmMcbm2018MonitorAlgoMuchLite *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorAlgoMuchLite >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorAlgoMuchLite", ::CbmMcbm2018MonitorAlgoMuchLite::Class_Version(), "CbmMcbm2018MonitorAlgoMuchLite.h", 42,
                  typeid(::CbmMcbm2018MonitorAlgoMuchLite), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorAlgoMuchLite::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorAlgoMuchLite) );
      instance.SetNew(&new_CbmMcbm2018MonitorAlgoMuchLite);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorAlgoMuchLite);
      instance.SetDelete(&delete_CbmMcbm2018MonitorAlgoMuchLite);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorAlgoMuchLite);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorAlgoMuchLite);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorAlgoMuchLite);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorAlgoMuchLite*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorAlgoMuchLite*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoMuchLite*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorTaskMuchLite(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorTaskMuchLite(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorTaskMuchLite(void *p);
   static void deleteArray_CbmMcbm2018MonitorTaskMuchLite(void *p);
   static void destruct_CbmMcbm2018MonitorTaskMuchLite(void *p);
   static void streamer_CbmMcbm2018MonitorTaskMuchLite(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorTaskMuchLite*)
   {
      ::CbmMcbm2018MonitorTaskMuchLite *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorTaskMuchLite >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorTaskMuchLite", ::CbmMcbm2018MonitorTaskMuchLite::Class_Version(), "CbmMcbm2018MonitorTaskMuchLite.h", 41,
                  typeid(::CbmMcbm2018MonitorTaskMuchLite), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorTaskMuchLite::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorTaskMuchLite) );
      instance.SetNew(&new_CbmMcbm2018MonitorTaskMuchLite);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorTaskMuchLite);
      instance.SetDelete(&delete_CbmMcbm2018MonitorTaskMuchLite);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorTaskMuchLite);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorTaskMuchLite);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorTaskMuchLite);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorTaskMuchLite*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorTaskMuchLite*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskMuchLite*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorTof(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorTof(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorTof(void *p);
   static void deleteArray_CbmMcbm2018MonitorTof(void *p);
   static void destruct_CbmMcbm2018MonitorTof(void *p);
   static void streamer_CbmMcbm2018MonitorTof(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorTof*)
   {
      ::CbmMcbm2018MonitorTof *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorTof >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorTof", ::CbmMcbm2018MonitorTof::Class_Version(), "CbmMcbm2018MonitorTof.h", 38,
                  typeid(::CbmMcbm2018MonitorTof), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorTof::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorTof) );
      instance.SetNew(&new_CbmMcbm2018MonitorTof);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorTof);
      instance.SetDelete(&delete_CbmMcbm2018MonitorTof);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorTof);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorTof);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorTof);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorTof*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorTof*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTof*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorTofPulser(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorTofPulser(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorTofPulser(void *p);
   static void deleteArray_CbmMcbm2018MonitorTofPulser(void *p);
   static void destruct_CbmMcbm2018MonitorTofPulser(void *p);
   static void streamer_CbmMcbm2018MonitorTofPulser(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorTofPulser*)
   {
      ::CbmMcbm2018MonitorTofPulser *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorTofPulser >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorTofPulser", ::CbmMcbm2018MonitorTofPulser::Class_Version(), "CbmMcbm2018MonitorTofPulser.h", 39,
                  typeid(::CbmMcbm2018MonitorTofPulser), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorTofPulser::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorTofPulser) );
      instance.SetNew(&new_CbmMcbm2018MonitorTofPulser);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorTofPulser);
      instance.SetDelete(&delete_CbmMcbm2018MonitorTofPulser);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorTofPulser);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorTofPulser);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorTofPulser);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorTofPulser*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorTofPulser*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTofPulser*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorAlgoTof(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorAlgoTof(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorAlgoTof(void *p);
   static void deleteArray_CbmMcbm2018MonitorAlgoTof(void *p);
   static void destruct_CbmMcbm2018MonitorAlgoTof(void *p);
   static void streamer_CbmMcbm2018MonitorAlgoTof(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorAlgoTof*)
   {
      ::CbmMcbm2018MonitorAlgoTof *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorAlgoTof >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorAlgoTof", ::CbmMcbm2018MonitorAlgoTof::Class_Version(), "CbmMcbm2018MonitorAlgoTof.h", 37,
                  typeid(::CbmMcbm2018MonitorAlgoTof), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorAlgoTof::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorAlgoTof) );
      instance.SetNew(&new_CbmMcbm2018MonitorAlgoTof);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorAlgoTof);
      instance.SetDelete(&delete_CbmMcbm2018MonitorAlgoTof);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorAlgoTof);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorAlgoTof);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorAlgoTof);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorAlgoTof*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorAlgoTof*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoTof*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorTaskTof(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorTaskTof(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorTaskTof(void *p);
   static void deleteArray_CbmMcbm2018MonitorTaskTof(void *p);
   static void destruct_CbmMcbm2018MonitorTaskTof(void *p);
   static void streamer_CbmMcbm2018MonitorTaskTof(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorTaskTof*)
   {
      ::CbmMcbm2018MonitorTaskTof *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorTaskTof >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorTaskTof", ::CbmMcbm2018MonitorTaskTof::Class_Version(), "CbmMcbm2018MonitorTaskTof.h", 23,
                  typeid(::CbmMcbm2018MonitorTaskTof), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorTaskTof::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorTaskTof) );
      instance.SetNew(&new_CbmMcbm2018MonitorTaskTof);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorTaskTof);
      instance.SetDelete(&delete_CbmMcbm2018MonitorTaskTof);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorTaskTof);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorTaskTof);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorTaskTof);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorTaskTof*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorTaskTof*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskTof*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorAlgoTofPulser(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorAlgoTofPulser(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorAlgoTofPulser(void *p);
   static void deleteArray_CbmMcbm2018MonitorAlgoTofPulser(void *p);
   static void destruct_CbmMcbm2018MonitorAlgoTofPulser(void *p);
   static void streamer_CbmMcbm2018MonitorAlgoTofPulser(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorAlgoTofPulser*)
   {
      ::CbmMcbm2018MonitorAlgoTofPulser *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorAlgoTofPulser >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorAlgoTofPulser", ::CbmMcbm2018MonitorAlgoTofPulser::Class_Version(), "CbmMcbm2018MonitorAlgoTofPulser.h", 36,
                  typeid(::CbmMcbm2018MonitorAlgoTofPulser), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorAlgoTofPulser::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorAlgoTofPulser) );
      instance.SetNew(&new_CbmMcbm2018MonitorAlgoTofPulser);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorAlgoTofPulser);
      instance.SetDelete(&delete_CbmMcbm2018MonitorAlgoTofPulser);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorAlgoTofPulser);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorAlgoTofPulser);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorAlgoTofPulser);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorAlgoTofPulser*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorAlgoTofPulser*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoTofPulser*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorTaskTofPulser(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorTaskTofPulser(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorTaskTofPulser(void *p);
   static void deleteArray_CbmMcbm2018MonitorTaskTofPulser(void *p);
   static void destruct_CbmMcbm2018MonitorTaskTofPulser(void *p);
   static void streamer_CbmMcbm2018MonitorTaskTofPulser(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorTaskTofPulser*)
   {
      ::CbmMcbm2018MonitorTaskTofPulser *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorTaskTofPulser >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorTaskTofPulser", ::CbmMcbm2018MonitorTaskTofPulser::Class_Version(), "CbmMcbm2018MonitorTaskTofPulser.h", 23,
                  typeid(::CbmMcbm2018MonitorTaskTofPulser), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorTaskTofPulser::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorTaskTofPulser) );
      instance.SetNew(&new_CbmMcbm2018MonitorTaskTofPulser);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorTaskTofPulser);
      instance.SetDelete(&delete_CbmMcbm2018MonitorTaskTofPulser);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorTaskTofPulser);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorTaskTofPulser);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorTaskTofPulser);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorTaskTofPulser*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorTaskTofPulser*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskTofPulser*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018TofTestFee(void *p = nullptr);
   static void *newArray_CbmMcbm2018TofTestFee(Long_t size, void *p);
   static void delete_CbmMcbm2018TofTestFee(void *p);
   static void deleteArray_CbmMcbm2018TofTestFee(void *p);
   static void destruct_CbmMcbm2018TofTestFee(void *p);
   static void streamer_CbmMcbm2018TofTestFee(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018TofTestFee*)
   {
      ::CbmMcbm2018TofTestFee *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018TofTestFee >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018TofTestFee", ::CbmMcbm2018TofTestFee::Class_Version(), "CbmMcbm2018TofTestFee.h", 38,
                  typeid(::CbmMcbm2018TofTestFee), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018TofTestFee::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018TofTestFee) );
      instance.SetNew(&new_CbmMcbm2018TofTestFee);
      instance.SetNewArray(&newArray_CbmMcbm2018TofTestFee);
      instance.SetDelete(&delete_CbmMcbm2018TofTestFee);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018TofTestFee);
      instance.SetDestructor(&destruct_CbmMcbm2018TofTestFee);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018TofTestFee);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018TofTestFee*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018TofTestFee*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018TofTestFee*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018TofFeeThr(void *p = nullptr);
   static void *newArray_CbmMcbm2018TofFeeThr(Long_t size, void *p);
   static void delete_CbmMcbm2018TofFeeThr(void *p);
   static void deleteArray_CbmMcbm2018TofFeeThr(void *p);
   static void destruct_CbmMcbm2018TofFeeThr(void *p);
   static void streamer_CbmMcbm2018TofFeeThr(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018TofFeeThr*)
   {
      ::CbmMcbm2018TofFeeThr *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018TofFeeThr >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018TofFeeThr", ::CbmMcbm2018TofFeeThr::Class_Version(), "CbmMcbm2018TofFeeThr.h", 38,
                  typeid(::CbmMcbm2018TofFeeThr), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018TofFeeThr::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018TofFeeThr) );
      instance.SetNew(&new_CbmMcbm2018TofFeeThr);
      instance.SetNewArray(&newArray_CbmMcbm2018TofFeeThr);
      instance.SetDelete(&delete_CbmMcbm2018TofFeeThr);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018TofFeeThr);
      instance.SetDestructor(&destruct_CbmMcbm2018TofFeeThr);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018TofFeeThr);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018TofFeeThr*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018TofFeeThr*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018TofFeeThr*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorAlgoPsd(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorAlgoPsd(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorAlgoPsd(void *p);
   static void deleteArray_CbmMcbm2018MonitorAlgoPsd(void *p);
   static void destruct_CbmMcbm2018MonitorAlgoPsd(void *p);
   static void streamer_CbmMcbm2018MonitorAlgoPsd(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorAlgoPsd*)
   {
      ::CbmMcbm2018MonitorAlgoPsd *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorAlgoPsd >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorAlgoPsd", ::CbmMcbm2018MonitorAlgoPsd::Class_Version(), "CbmMcbm2018MonitorAlgoPsd.h", 40,
                  typeid(::CbmMcbm2018MonitorAlgoPsd), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorAlgoPsd::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorAlgoPsd) );
      instance.SetNew(&new_CbmMcbm2018MonitorAlgoPsd);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorAlgoPsd);
      instance.SetDelete(&delete_CbmMcbm2018MonitorAlgoPsd);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorAlgoPsd);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorAlgoPsd);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorAlgoPsd);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorAlgoPsd*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorAlgoPsd*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoPsd*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MonitorTaskPsd(void *p = nullptr);
   static void *newArray_CbmMcbm2018MonitorTaskPsd(Long_t size, void *p);
   static void delete_CbmMcbm2018MonitorTaskPsd(void *p);
   static void deleteArray_CbmMcbm2018MonitorTaskPsd(void *p);
   static void destruct_CbmMcbm2018MonitorTaskPsd(void *p);
   static void streamer_CbmMcbm2018MonitorTaskPsd(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MonitorTaskPsd*)
   {
      ::CbmMcbm2018MonitorTaskPsd *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MonitorTaskPsd >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MonitorTaskPsd", ::CbmMcbm2018MonitorTaskPsd::Class_Version(), "CbmMcbm2018MonitorTaskPsd.h", 24,
                  typeid(::CbmMcbm2018MonitorTaskPsd), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MonitorTaskPsd::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018MonitorTaskPsd) );
      instance.SetNew(&new_CbmMcbm2018MonitorTaskPsd);
      instance.SetNewArray(&newArray_CbmMcbm2018MonitorTaskPsd);
      instance.SetDelete(&delete_CbmMcbm2018MonitorTaskPsd);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MonitorTaskPsd);
      instance.SetDestructor(&destruct_CbmMcbm2018MonitorTaskPsd);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018MonitorTaskPsd);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MonitorTaskPsd*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MonitorTaskPsd*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskPsd*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmCheckDigisNbCorr(void *p = nullptr);
   static void *newArray_CbmCheckDigisNbCorr(Long_t size, void *p);
   static void delete_CbmCheckDigisNbCorr(void *p);
   static void deleteArray_CbmCheckDigisNbCorr(void *p);
   static void destruct_CbmCheckDigisNbCorr(void *p);
   static void streamer_CbmCheckDigisNbCorr(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCheckDigisNbCorr*)
   {
      ::CbmCheckDigisNbCorr *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCheckDigisNbCorr >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCheckDigisNbCorr", ::CbmCheckDigisNbCorr::Class_Version(), "CbmCheckDigisNbCorr.h", 22,
                  typeid(::CbmCheckDigisNbCorr), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCheckDigisNbCorr::Dictionary, isa_proxy, 16,
                  sizeof(::CbmCheckDigisNbCorr) );
      instance.SetNew(&new_CbmCheckDigisNbCorr);
      instance.SetNewArray(&newArray_CbmCheckDigisNbCorr);
      instance.SetDelete(&delete_CbmCheckDigisNbCorr);
      instance.SetDeleteArray(&deleteArray_CbmCheckDigisNbCorr);
      instance.SetDestructor(&destruct_CbmCheckDigisNbCorr);
      instance.SetStreamerFunc(&streamer_CbmCheckDigisNbCorr);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCheckDigisNbCorr*)
   {
      return GenerateInitInstanceLocal((::CbmCheckDigisNbCorr*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCheckDigisNbCorr*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmCheckTiming(void *p = nullptr);
   static void *newArray_CbmCheckTiming(Long_t size, void *p);
   static void delete_CbmCheckTiming(void *p);
   static void deleteArray_CbmCheckTiming(void *p);
   static void destruct_CbmCheckTiming(void *p);
   static void streamer_CbmCheckTiming(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCheckTiming*)
   {
      ::CbmCheckTiming *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCheckTiming >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCheckTiming", ::CbmCheckTiming::Class_Version(), "CbmCheckTiming.h", 22,
                  typeid(::CbmCheckTiming), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCheckTiming::Dictionary, isa_proxy, 16,
                  sizeof(::CbmCheckTiming) );
      instance.SetNew(&new_CbmCheckTiming);
      instance.SetNewArray(&newArray_CbmCheckTiming);
      instance.SetDelete(&delete_CbmCheckTiming);
      instance.SetDeleteArray(&deleteArray_CbmCheckTiming);
      instance.SetDestructor(&destruct_CbmCheckTiming);
      instance.SetStreamerFunc(&streamer_CbmCheckTiming);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCheckTiming*)
   {
      return GenerateInitInstanceLocal((::CbmCheckTiming*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCheckTiming*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmCheckEvents(void *p = nullptr);
   static void *newArray_CbmCheckEvents(Long_t size, void *p);
   static void delete_CbmCheckEvents(void *p);
   static void deleteArray_CbmCheckEvents(void *p);
   static void destruct_CbmCheckEvents(void *p);
   static void streamer_CbmCheckEvents(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCheckEvents*)
   {
      ::CbmCheckEvents *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCheckEvents >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCheckEvents", ::CbmCheckEvents::Class_Version(), "CbmCheckEvents.h", 25,
                  typeid(::CbmCheckEvents), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCheckEvents::Dictionary, isa_proxy, 16,
                  sizeof(::CbmCheckEvents) );
      instance.SetNew(&new_CbmCheckEvents);
      instance.SetNewArray(&newArray_CbmCheckEvents);
      instance.SetDelete(&delete_CbmCheckEvents);
      instance.SetDeleteArray(&deleteArray_CbmCheckEvents);
      instance.SetDestructor(&destruct_CbmCheckEvents);
      instance.SetStreamerFunc(&streamer_CbmCheckEvents);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCheckEvents*)
   {
      return GenerateInitInstanceLocal((::CbmCheckEvents*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCheckEvents*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018EventBuilder(void *p = nullptr);
   static void *newArray_CbmMcbm2018EventBuilder(Long_t size, void *p);
   static void delete_CbmMcbm2018EventBuilder(void *p);
   static void deleteArray_CbmMcbm2018EventBuilder(void *p);
   static void destruct_CbmMcbm2018EventBuilder(void *p);
   static void streamer_CbmMcbm2018EventBuilder(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018EventBuilder*)
   {
      ::CbmMcbm2018EventBuilder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018EventBuilder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018EventBuilder", ::CbmMcbm2018EventBuilder::Class_Version(), "CbmMcbm2018EventBuilder.h", 50,
                  typeid(::CbmMcbm2018EventBuilder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018EventBuilder::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2018EventBuilder) );
      instance.SetNew(&new_CbmMcbm2018EventBuilder);
      instance.SetNewArray(&newArray_CbmMcbm2018EventBuilder);
      instance.SetDelete(&delete_CbmMcbm2018EventBuilder);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018EventBuilder);
      instance.SetDestructor(&destruct_CbmMcbm2018EventBuilder);
      instance.SetStreamerFunc(&streamer_CbmMcbm2018EventBuilder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018EventBuilder*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018EventBuilder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018EventBuilder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2019CheckPulser(void *p = nullptr);
   static void *newArray_CbmMcbm2019CheckPulser(Long_t size, void *p);
   static void delete_CbmMcbm2019CheckPulser(void *p);
   static void deleteArray_CbmMcbm2019CheckPulser(void *p);
   static void destruct_CbmMcbm2019CheckPulser(void *p);
   static void streamer_CbmMcbm2019CheckPulser(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2019CheckPulser*)
   {
      ::CbmMcbm2019CheckPulser *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2019CheckPulser >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2019CheckPulser", ::CbmMcbm2019CheckPulser::Class_Version(), "CbmMcbm2019CheckPulser.h", 29,
                  typeid(::CbmMcbm2019CheckPulser), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2019CheckPulser::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2019CheckPulser) );
      instance.SetNew(&new_CbmMcbm2019CheckPulser);
      instance.SetNewArray(&newArray_CbmMcbm2019CheckPulser);
      instance.SetDelete(&delete_CbmMcbm2019CheckPulser);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2019CheckPulser);
      instance.SetDestructor(&destruct_CbmMcbm2019CheckPulser);
      instance.SetStreamerFunc(&streamer_CbmMcbm2019CheckPulser);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2019CheckPulser*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2019CheckPulser*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2019CheckPulser*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2019CheckDigisMuch(void *p = nullptr);
   static void *newArray_CbmMcbm2019CheckDigisMuch(Long_t size, void *p);
   static void delete_CbmMcbm2019CheckDigisMuch(void *p);
   static void deleteArray_CbmMcbm2019CheckDigisMuch(void *p);
   static void destruct_CbmMcbm2019CheckDigisMuch(void *p);
   static void streamer_CbmMcbm2019CheckDigisMuch(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2019CheckDigisMuch*)
   {
      ::CbmMcbm2019CheckDigisMuch *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2019CheckDigisMuch >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2019CheckDigisMuch", ::CbmMcbm2019CheckDigisMuch::Class_Version(), "CbmMcbm2019CheckDigisMuch.h", 19,
                  typeid(::CbmMcbm2019CheckDigisMuch), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2019CheckDigisMuch::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2019CheckDigisMuch) );
      instance.SetNew(&new_CbmMcbm2019CheckDigisMuch);
      instance.SetNewArray(&newArray_CbmMcbm2019CheckDigisMuch);
      instance.SetDelete(&delete_CbmMcbm2019CheckDigisMuch);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2019CheckDigisMuch);
      instance.SetDestructor(&destruct_CbmMcbm2019CheckDigisMuch);
      instance.SetStreamerFunc(&streamer_CbmMcbm2019CheckDigisMuch);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2019CheckDigisMuch*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2019CheckDigisMuch*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDigisMuch*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2019CheckDigisSts(void *p = nullptr);
   static void *newArray_CbmMcbm2019CheckDigisSts(Long_t size, void *p);
   static void delete_CbmMcbm2019CheckDigisSts(void *p);
   static void deleteArray_CbmMcbm2019CheckDigisSts(void *p);
   static void destruct_CbmMcbm2019CheckDigisSts(void *p);
   static void streamer_CbmMcbm2019CheckDigisSts(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2019CheckDigisSts*)
   {
      ::CbmMcbm2019CheckDigisSts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2019CheckDigisSts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2019CheckDigisSts", ::CbmMcbm2019CheckDigisSts::Class_Version(), "CbmMcbm2019CheckDigisSts.h", 19,
                  typeid(::CbmMcbm2019CheckDigisSts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2019CheckDigisSts::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2019CheckDigisSts) );
      instance.SetNew(&new_CbmMcbm2019CheckDigisSts);
      instance.SetNewArray(&newArray_CbmMcbm2019CheckDigisSts);
      instance.SetDelete(&delete_CbmMcbm2019CheckDigisSts);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2019CheckDigisSts);
      instance.SetDestructor(&destruct_CbmMcbm2019CheckDigisSts);
      instance.SetStreamerFunc(&streamer_CbmMcbm2019CheckDigisSts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2019CheckDigisSts*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2019CheckDigisSts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDigisSts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2019CheckDtInDet(void *p = nullptr);
   static void *newArray_CbmMcbm2019CheckDtInDet(Long_t size, void *p);
   static void delete_CbmMcbm2019CheckDtInDet(void *p);
   static void deleteArray_CbmMcbm2019CheckDtInDet(void *p);
   static void destruct_CbmMcbm2019CheckDtInDet(void *p);
   static void streamer_CbmMcbm2019CheckDtInDet(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2019CheckDtInDet*)
   {
      ::CbmMcbm2019CheckDtInDet *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2019CheckDtInDet >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2019CheckDtInDet", ::CbmMcbm2019CheckDtInDet::Class_Version(), "CbmMcbm2019CheckDtInDet.h", 29,
                  typeid(::CbmMcbm2019CheckDtInDet), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2019CheckDtInDet::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMcbm2019CheckDtInDet) );
      instance.SetNew(&new_CbmMcbm2019CheckDtInDet);
      instance.SetNewArray(&newArray_CbmMcbm2019CheckDtInDet);
      instance.SetDelete(&delete_CbmMcbm2019CheckDtInDet);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2019CheckDtInDet);
      instance.SetDestructor(&destruct_CbmMcbm2019CheckDtInDet);
      instance.SetStreamerFunc(&streamer_CbmMcbm2019CheckDtInDet);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2019CheckDtInDet*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2019CheckDtInDet*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDtInDet*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2019CheckTimingPairs(void *p = nullptr);
   static void *newArray_CbmMcbm2019CheckTimingPairs(Long_t size, void *p);
   static void delete_CbmMcbm2019CheckTimingPairs(void *p);
   static void deleteArray_CbmMcbm2019CheckTimingPairs(void *p);
   static void destruct_CbmMcbm2019CheckTimingPairs(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2019CheckTimingPairs*)
   {
      ::CbmMcbm2019CheckTimingPairs *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2019CheckTimingPairs >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2019CheckTimingPairs", ::CbmMcbm2019CheckTimingPairs::Class_Version(), "CbmMcbm2019CheckTimingPairs.h", 34,
                  typeid(::CbmMcbm2019CheckTimingPairs), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2019CheckTimingPairs::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2019CheckTimingPairs) );
      instance.SetNew(&new_CbmMcbm2019CheckTimingPairs);
      instance.SetNewArray(&newArray_CbmMcbm2019CheckTimingPairs);
      instance.SetDelete(&delete_CbmMcbm2019CheckTimingPairs);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2019CheckTimingPairs);
      instance.SetDestructor(&destruct_CbmMcbm2019CheckTimingPairs);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2019CheckTimingPairs*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2019CheckTimingPairs*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2019CheckTimingPairs*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2019TimeWinEventBuilderAlgo(void *p = nullptr);
   static void *newArray_CbmMcbm2019TimeWinEventBuilderAlgo(Long_t size, void *p);
   static void delete_CbmMcbm2019TimeWinEventBuilderAlgo(void *p);
   static void deleteArray_CbmMcbm2019TimeWinEventBuilderAlgo(void *p);
   static void destruct_CbmMcbm2019TimeWinEventBuilderAlgo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2019TimeWinEventBuilderAlgo*)
   {
      ::CbmMcbm2019TimeWinEventBuilderAlgo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2019TimeWinEventBuilderAlgo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2019TimeWinEventBuilderAlgo", ::CbmMcbm2019TimeWinEventBuilderAlgo::Class_Version(), "CbmMcbm2019TimeWinEventBuilderAlgo.h", 99,
                  typeid(::CbmMcbm2019TimeWinEventBuilderAlgo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2019TimeWinEventBuilderAlgo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2019TimeWinEventBuilderAlgo) );
      instance.SetNew(&new_CbmMcbm2019TimeWinEventBuilderAlgo);
      instance.SetNewArray(&newArray_CbmMcbm2019TimeWinEventBuilderAlgo);
      instance.SetDelete(&delete_CbmMcbm2019TimeWinEventBuilderAlgo);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2019TimeWinEventBuilderAlgo);
      instance.SetDestructor(&destruct_CbmMcbm2019TimeWinEventBuilderAlgo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2019TimeWinEventBuilderAlgo*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2019TimeWinEventBuilderAlgo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2019TimeWinEventBuilderAlgo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2019TimeWinEventBuilderTask(void *p = nullptr);
   static void *newArray_CbmMcbm2019TimeWinEventBuilderTask(Long_t size, void *p);
   static void delete_CbmMcbm2019TimeWinEventBuilderTask(void *p);
   static void deleteArray_CbmMcbm2019TimeWinEventBuilderTask(void *p);
   static void destruct_CbmMcbm2019TimeWinEventBuilderTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2019TimeWinEventBuilderTask*)
   {
      ::CbmMcbm2019TimeWinEventBuilderTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2019TimeWinEventBuilderTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2019TimeWinEventBuilderTask", ::CbmMcbm2019TimeWinEventBuilderTask::Class_Version(), "CbmMcbm2019TimeWinEventBuilderTask.h", 28,
                  typeid(::CbmMcbm2019TimeWinEventBuilderTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2019TimeWinEventBuilderTask::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2019TimeWinEventBuilderTask) );
      instance.SetNew(&new_CbmMcbm2019TimeWinEventBuilderTask);
      instance.SetNewArray(&newArray_CbmMcbm2019TimeWinEventBuilderTask);
      instance.SetDelete(&delete_CbmMcbm2019TimeWinEventBuilderTask);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2019TimeWinEventBuilderTask);
      instance.SetDestructor(&destruct_CbmMcbm2019TimeWinEventBuilderTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2019TimeWinEventBuilderTask*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2019TimeWinEventBuilderTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2019TimeWinEventBuilderTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbmCheckTimingAlgo(void *p = nullptr);
   static void *newArray_CbmMcbmCheckTimingAlgo(Long_t size, void *p);
   static void delete_CbmMcbmCheckTimingAlgo(void *p);
   static void deleteArray_CbmMcbmCheckTimingAlgo(void *p);
   static void destruct_CbmMcbmCheckTimingAlgo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbmCheckTimingAlgo*)
   {
      ::CbmMcbmCheckTimingAlgo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbmCheckTimingAlgo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbmCheckTimingAlgo", ::CbmMcbmCheckTimingAlgo::Class_Version(), "CbmMcbmCheckTimingAlgo.h", 52,
                  typeid(::CbmMcbmCheckTimingAlgo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbmCheckTimingAlgo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbmCheckTimingAlgo) );
      instance.SetNew(&new_CbmMcbmCheckTimingAlgo);
      instance.SetNewArray(&newArray_CbmMcbmCheckTimingAlgo);
      instance.SetDelete(&delete_CbmMcbmCheckTimingAlgo);
      instance.SetDeleteArray(&deleteArray_CbmMcbmCheckTimingAlgo);
      instance.SetDestructor(&destruct_CbmMcbmCheckTimingAlgo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbmCheckTimingAlgo*)
   {
      return GenerateInitInstanceLocal((::CbmMcbmCheckTimingAlgo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbmCheckTimingAlgo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbmCheckTimingTask(void *p = nullptr);
   static void *newArray_CbmMcbmCheckTimingTask(Long_t size, void *p);
   static void delete_CbmMcbmCheckTimingTask(void *p);
   static void deleteArray_CbmMcbmCheckTimingTask(void *p);
   static void destruct_CbmMcbmCheckTimingTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbmCheckTimingTask*)
   {
      ::CbmMcbmCheckTimingTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbmCheckTimingTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbmCheckTimingTask", ::CbmMcbmCheckTimingTask::Class_Version(), "CbmMcbmCheckTimingTask.h", 20,
                  typeid(::CbmMcbmCheckTimingTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbmCheckTimingTask::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbmCheckTimingTask) );
      instance.SetNew(&new_CbmMcbmCheckTimingTask);
      instance.SetNewArray(&newArray_CbmMcbmCheckTimingTask);
      instance.SetDelete(&delete_CbmMcbmCheckTimingTask);
      instance.SetDeleteArray(&deleteArray_CbmMcbmCheckTimingTask);
      instance.SetDestructor(&destruct_CbmMcbmCheckTimingTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbmCheckTimingTask*)
   {
      return GenerateInitInstanceLocal((::CbmMcbmCheckTimingTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbmCheckTimingTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbmSpillFindAlgo(void *p = nullptr);
   static void *newArray_CbmMcbmSpillFindAlgo(Long_t size, void *p);
   static void delete_CbmMcbmSpillFindAlgo(void *p);
   static void deleteArray_CbmMcbmSpillFindAlgo(void *p);
   static void destruct_CbmMcbmSpillFindAlgo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbmSpillFindAlgo*)
   {
      ::CbmMcbmSpillFindAlgo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbmSpillFindAlgo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbmSpillFindAlgo", ::CbmMcbmSpillFindAlgo::Class_Version(), "CbmMcbmSpillFindAlgo.h", 40,
                  typeid(::CbmMcbmSpillFindAlgo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbmSpillFindAlgo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbmSpillFindAlgo) );
      instance.SetNew(&new_CbmMcbmSpillFindAlgo);
      instance.SetNewArray(&newArray_CbmMcbmSpillFindAlgo);
      instance.SetDelete(&delete_CbmMcbmSpillFindAlgo);
      instance.SetDeleteArray(&deleteArray_CbmMcbmSpillFindAlgo);
      instance.SetDestructor(&destruct_CbmMcbmSpillFindAlgo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbmSpillFindAlgo*)
   {
      return GenerateInitInstanceLocal((::CbmMcbmSpillFindAlgo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbmSpillFindAlgo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbmSpillFindTask(void *p = nullptr);
   static void *newArray_CbmMcbmSpillFindTask(Long_t size, void *p);
   static void delete_CbmMcbmSpillFindTask(void *p);
   static void deleteArray_CbmMcbmSpillFindTask(void *p);
   static void destruct_CbmMcbmSpillFindTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbmSpillFindTask*)
   {
      ::CbmMcbmSpillFindTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbmSpillFindTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbmSpillFindTask", ::CbmMcbmSpillFindTask::Class_Version(), "CbmMcbmSpillFindTask.h", 22,
                  typeid(::CbmMcbmSpillFindTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbmSpillFindTask::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbmSpillFindTask) );
      instance.SetNew(&new_CbmMcbmSpillFindTask);
      instance.SetNewArray(&newArray_CbmMcbmSpillFindTask);
      instance.SetDelete(&delete_CbmMcbmSpillFindTask);
      instance.SetDeleteArray(&deleteArray_CbmMcbmSpillFindTask);
      instance.SetDestructor(&destruct_CbmMcbmSpillFindTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbmSpillFindTask*)
   {
      return GenerateInitInstanceLocal((::CbmMcbmSpillFindTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbmSpillFindTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbmUnpack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbmUnpack::Class_Name()
{
   return "CbmMcbmUnpack";
}

//______________________________________________________________________________
const char *CbmMcbmUnpack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmUnpack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbmUnpack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmUnpack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbmUnpack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmUnpack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbmUnpack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmUnpack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018Source::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018Source::Class_Name()
{
   return "CbmMcbm2018Source";
}

//______________________________________________________________________________
const char *CbmMcbm2018Source::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018Source*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018Source::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018Source*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018Source::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018Source*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018Source::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018Source*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MsCrcCheck::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MsCrcCheck::Class_Name()
{
   return "CbmMcbm2018MsCrcCheck";
}

//______________________________________________________________________________
const char *CbmMcbm2018MsCrcCheck::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MsCrcCheck*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MsCrcCheck::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MsCrcCheck*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MsCrcCheck::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MsCrcCheck*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MsCrcCheck::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MsCrcCheck*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018RawConverterSdpb::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018RawConverterSdpb::Class_Name()
{
   return "CbmMcbm2018RawConverterSdpb";
}

//______________________________________________________________________________
const char *CbmMcbm2018RawConverterSdpb::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018RawConverterSdpb*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018RawConverterSdpb::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018RawConverterSdpb*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018RawConverterSdpb::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018RawConverterSdpb*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018RawConverterSdpb::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018RawConverterSdpb*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018RawConverterGdpb::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018RawConverterGdpb::Class_Name()
{
   return "CbmMcbm2018RawConverterGdpb";
}

//______________________________________________________________________________
const char *CbmMcbm2018RawConverterGdpb::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018RawConverterGdpb*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018RawConverterGdpb::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018RawConverterGdpb*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018RawConverterGdpb::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018RawConverterGdpb*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018RawConverterGdpb::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018RawConverterGdpb*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCriGet4RawPrint::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCriGet4RawPrint::Class_Name()
{
   return "CbmCriGet4RawPrint";
}

//______________________________________________________________________________
const char *CbmCriGet4RawPrint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCriGet4RawPrint*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCriGet4RawPrint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCriGet4RawPrint*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCriGet4RawPrint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCriGet4RawPrint*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCriGet4RawPrint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCriGet4RawPrint*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018UnpackerAlgoSts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerAlgoSts::Class_Name()
{
   return "CbmMcbm2018UnpackerAlgoSts";
}

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerAlgoSts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoSts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018UnpackerAlgoSts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoSts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerAlgoSts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoSts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerAlgoSts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoSts*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018UnpackerTaskSts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerTaskSts::Class_Name()
{
   return "CbmMcbm2018UnpackerTaskSts";
}

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerTaskSts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskSts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018UnpackerTaskSts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskSts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerTaskSts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskSts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerTaskSts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskSts*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018UnpackerAlgoMuch::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerAlgoMuch::Class_Name()
{
   return "CbmMcbm2018UnpackerAlgoMuch";
}

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerAlgoMuch::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoMuch*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018UnpackerAlgoMuch::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoMuch*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerAlgoMuch::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoMuch*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerAlgoMuch::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoMuch*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018UnpackerTaskMuch::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerTaskMuch::Class_Name()
{
   return "CbmMcbm2018UnpackerTaskMuch";
}

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerTaskMuch::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskMuch*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018UnpackerTaskMuch::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskMuch*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerTaskMuch::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskMuch*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerTaskMuch::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskMuch*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018UnpackerAlgoTof::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerAlgoTof::Class_Name()
{
   return "CbmMcbm2018UnpackerAlgoTof";
}

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerAlgoTof::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoTof*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018UnpackerAlgoTof::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoTof*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerAlgoTof::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoTof*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerAlgoTof::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoTof*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018UnpackerTaskTof::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerTaskTof::Class_Name()
{
   return "CbmMcbm2018UnpackerTaskTof";
}

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerTaskTof::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskTof*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018UnpackerTaskTof::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskTof*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerTaskTof::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskTof*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerTaskTof::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskTof*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018UnpackerAlgoRich::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerAlgoRich::Class_Name()
{
   return "CbmMcbm2018UnpackerAlgoRich";
}

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerAlgoRich::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoRich*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018UnpackerAlgoRich::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoRich*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerAlgoRich::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoRich*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerAlgoRich::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoRich*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018UnpackerTaskRich::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerTaskRich::Class_Name()
{
   return "CbmMcbm2018UnpackerTaskRich";
}

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerTaskRich::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskRich*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018UnpackerTaskRich::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskRich*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerTaskRich::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskRich*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerTaskRich::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskRich*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018UnpackerTaskRich2020::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerTaskRich2020::Class_Name()
{
   return "CbmMcbm2018UnpackerTaskRich2020";
}

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerTaskRich2020::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskRich2020*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018UnpackerTaskRich2020::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskRich2020*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerTaskRich2020::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskRich2020*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerTaskRich2020::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskRich2020*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018UnpackerAlgoRich2020::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerAlgoRich2020::Class_Name()
{
   return "CbmMcbm2018UnpackerAlgoRich2020";
}

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerAlgoRich2020::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoRich2020*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018UnpackerAlgoRich2020::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoRich2020*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerAlgoRich2020::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoRich2020*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerAlgoRich2020::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoRich2020*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018UnpackerAlgoPsd::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerAlgoPsd::Class_Name()
{
   return "CbmMcbm2018UnpackerAlgoPsd";
}

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerAlgoPsd::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoPsd*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018UnpackerAlgoPsd::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoPsd*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerAlgoPsd::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoPsd*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerAlgoPsd::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoPsd*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018UnpackerTaskPsd::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerTaskPsd::Class_Name()
{
   return "CbmMcbm2018UnpackerTaskPsd";
}

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerTaskPsd::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskPsd*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018UnpackerTaskPsd::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskPsd*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerTaskPsd::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskPsd*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerTaskPsd::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskPsd*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018UnpackerAlgoTrdR::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerAlgoTrdR::Class_Name()
{
   return "CbmMcbm2018UnpackerAlgoTrdR";
}

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerAlgoTrdR::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoTrdR*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018UnpackerAlgoTrdR::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoTrdR*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerAlgoTrdR::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoTrdR*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerAlgoTrdR::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerAlgoTrdR*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018UnpackerTaskTrdR::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerTaskTrdR::Class_Name()
{
   return "CbmMcbm2018UnpackerTaskTrdR";
}

//______________________________________________________________________________
const char *CbmMcbm2018UnpackerTaskTrdR::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskTrdR*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018UnpackerTaskTrdR::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskTrdR*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerTaskTrdR::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskTrdR*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018UnpackerTaskTrdR::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018UnpackerTaskTrdR*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCheckDataFormatGdpb2018::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCheckDataFormatGdpb2018::Class_Name()
{
   return "CbmCheckDataFormatGdpb2018";
}

//______________________________________________________________________________
const char *CbmCheckDataFormatGdpb2018::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckDataFormatGdpb2018*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCheckDataFormatGdpb2018::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckDataFormatGdpb2018*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCheckDataFormatGdpb2018::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckDataFormatGdpb2018*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCheckDataFormatGdpb2018::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckDataFormatGdpb2018*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorDataRates::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorDataRates::Class_Name()
{
   return "CbmMcbm2018MonitorDataRates";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorDataRates::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorDataRates*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorDataRates::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorDataRates*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorDataRates::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorDataRates*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorDataRates::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorDataRates*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorAlgoT0::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorAlgoT0::Class_Name()
{
   return "CbmMcbm2018MonitorAlgoT0";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorAlgoT0::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoT0*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorAlgoT0::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoT0*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorAlgoT0::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoT0*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorAlgoT0::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoT0*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorTaskT0::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorTaskT0::Class_Name()
{
   return "CbmMcbm2018MonitorTaskT0";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorTaskT0::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskT0*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorTaskT0::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskT0*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorTaskT0::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskT0*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorTaskT0::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskT0*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorMcbmRate::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorMcbmRate::Class_Name()
{
   return "CbmMcbm2018MonitorMcbmRate";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorMcbmRate::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmRate*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorMcbmRate::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmRate*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorMcbmRate::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmRate*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorMcbmRate::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmRate*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorMcbmSync::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorMcbmSync::Class_Name()
{
   return "CbmMcbm2018MonitorMcbmSync";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorMcbmSync::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmSync*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorMcbmSync::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmSync*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorMcbmSync::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmSync*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorMcbmSync::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmSync*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorMcbmPulser::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorMcbmPulser::Class_Name()
{
   return "CbmMcbm2018MonitorMcbmPulser";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorMcbmPulser::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmPulser*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorMcbmPulser::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmPulser*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorMcbmPulser::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmPulser*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorMcbmPulser::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMcbmPulser*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorSts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorSts::Class_Name()
{
   return "CbmMcbm2018MonitorSts";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorSts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorSts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorSts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorSts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorSts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorSts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorSts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorSts*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorMuch::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorMuch::Class_Name()
{
   return "CbmMcbm2018MonitorMuch";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorMuch::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMuch*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorMuch::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMuch*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorMuch::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMuch*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorMuch::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMuch*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorMuchLite::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorMuchLite::Class_Name()
{
   return "CbmMcbm2018MonitorMuchLite";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorMuchLite::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMuchLite*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorMuchLite::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMuchLite*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorMuchLite::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMuchLite*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorMuchLite::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorMuchLite*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorAlgoMuchLite::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorAlgoMuchLite::Class_Name()
{
   return "CbmMcbm2018MonitorAlgoMuchLite";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorAlgoMuchLite::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoMuchLite*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorAlgoMuchLite::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoMuchLite*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorAlgoMuchLite::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoMuchLite*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorAlgoMuchLite::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoMuchLite*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorTaskMuchLite::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorTaskMuchLite::Class_Name()
{
   return "CbmMcbm2018MonitorTaskMuchLite";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorTaskMuchLite::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskMuchLite*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorTaskMuchLite::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskMuchLite*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorTaskMuchLite::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskMuchLite*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorTaskMuchLite::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskMuchLite*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorTof::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorTof::Class_Name()
{
   return "CbmMcbm2018MonitorTof";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorTof::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTof*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorTof::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTof*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorTof::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTof*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorTof::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTof*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorTofPulser::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorTofPulser::Class_Name()
{
   return "CbmMcbm2018MonitorTofPulser";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorTofPulser::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTofPulser*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorTofPulser::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTofPulser*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorTofPulser::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTofPulser*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorTofPulser::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTofPulser*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorAlgoTof::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorAlgoTof::Class_Name()
{
   return "CbmMcbm2018MonitorAlgoTof";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorAlgoTof::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoTof*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorAlgoTof::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoTof*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorAlgoTof::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoTof*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorAlgoTof::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoTof*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorTaskTof::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorTaskTof::Class_Name()
{
   return "CbmMcbm2018MonitorTaskTof";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorTaskTof::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskTof*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorTaskTof::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskTof*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorTaskTof::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskTof*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorTaskTof::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskTof*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorAlgoTofPulser::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorAlgoTofPulser::Class_Name()
{
   return "CbmMcbm2018MonitorAlgoTofPulser";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorAlgoTofPulser::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoTofPulser*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorAlgoTofPulser::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoTofPulser*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorAlgoTofPulser::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoTofPulser*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorAlgoTofPulser::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoTofPulser*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorTaskTofPulser::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorTaskTofPulser::Class_Name()
{
   return "CbmMcbm2018MonitorTaskTofPulser";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorTaskTofPulser::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskTofPulser*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorTaskTofPulser::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskTofPulser*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorTaskTofPulser::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskTofPulser*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorTaskTofPulser::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskTofPulser*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018TofTestFee::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018TofTestFee::Class_Name()
{
   return "CbmMcbm2018TofTestFee";
}

//______________________________________________________________________________
const char *CbmMcbm2018TofTestFee::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018TofTestFee*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018TofTestFee::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018TofTestFee*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018TofTestFee::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018TofTestFee*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018TofTestFee::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018TofTestFee*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018TofFeeThr::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018TofFeeThr::Class_Name()
{
   return "CbmMcbm2018TofFeeThr";
}

//______________________________________________________________________________
const char *CbmMcbm2018TofFeeThr::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018TofFeeThr*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018TofFeeThr::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018TofFeeThr*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018TofFeeThr::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018TofFeeThr*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018TofFeeThr::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018TofFeeThr*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorAlgoPsd::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorAlgoPsd::Class_Name()
{
   return "CbmMcbm2018MonitorAlgoPsd";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorAlgoPsd::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoPsd*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorAlgoPsd::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoPsd*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorAlgoPsd::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoPsd*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorAlgoPsd::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorAlgoPsd*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MonitorTaskPsd::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MonitorTaskPsd::Class_Name()
{
   return "CbmMcbm2018MonitorTaskPsd";
}

//______________________________________________________________________________
const char *CbmMcbm2018MonitorTaskPsd::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskPsd*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MonitorTaskPsd::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskPsd*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorTaskPsd::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskPsd*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MonitorTaskPsd::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MonitorTaskPsd*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCheckDigisNbCorr::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCheckDigisNbCorr::Class_Name()
{
   return "CbmCheckDigisNbCorr";
}

//______________________________________________________________________________
const char *CbmCheckDigisNbCorr::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckDigisNbCorr*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCheckDigisNbCorr::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckDigisNbCorr*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCheckDigisNbCorr::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckDigisNbCorr*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCheckDigisNbCorr::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckDigisNbCorr*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCheckTiming::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCheckTiming::Class_Name()
{
   return "CbmCheckTiming";
}

//______________________________________________________________________________
const char *CbmCheckTiming::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckTiming*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCheckTiming::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckTiming*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCheckTiming::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckTiming*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCheckTiming::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckTiming*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCheckEvents::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCheckEvents::Class_Name()
{
   return "CbmCheckEvents";
}

//______________________________________________________________________________
const char *CbmCheckEvents::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckEvents*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCheckEvents::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckEvents*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCheckEvents::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckEvents*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCheckEvents::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCheckEvents*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018EventBuilder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018EventBuilder::Class_Name()
{
   return "CbmMcbm2018EventBuilder";
}

//______________________________________________________________________________
const char *CbmMcbm2018EventBuilder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018EventBuilder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018EventBuilder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018EventBuilder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018EventBuilder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018EventBuilder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018EventBuilder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018EventBuilder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2019CheckPulser::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2019CheckPulser::Class_Name()
{
   return "CbmMcbm2019CheckPulser";
}

//______________________________________________________________________________
const char *CbmMcbm2019CheckPulser::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckPulser*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2019CheckPulser::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckPulser*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2019CheckPulser::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckPulser*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2019CheckPulser::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckPulser*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2019CheckDigisMuch::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2019CheckDigisMuch::Class_Name()
{
   return "CbmMcbm2019CheckDigisMuch";
}

//______________________________________________________________________________
const char *CbmMcbm2019CheckDigisMuch::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDigisMuch*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2019CheckDigisMuch::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDigisMuch*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2019CheckDigisMuch::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDigisMuch*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2019CheckDigisMuch::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDigisMuch*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2019CheckDigisSts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2019CheckDigisSts::Class_Name()
{
   return "CbmMcbm2019CheckDigisSts";
}

//______________________________________________________________________________
const char *CbmMcbm2019CheckDigisSts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDigisSts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2019CheckDigisSts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDigisSts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2019CheckDigisSts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDigisSts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2019CheckDigisSts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDigisSts*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2019CheckDtInDet::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2019CheckDtInDet::Class_Name()
{
   return "CbmMcbm2019CheckDtInDet";
}

//______________________________________________________________________________
const char *CbmMcbm2019CheckDtInDet::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDtInDet*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2019CheckDtInDet::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDtInDet*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2019CheckDtInDet::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDtInDet*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2019CheckDtInDet::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckDtInDet*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2019CheckTimingPairs::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2019CheckTimingPairs::Class_Name()
{
   return "CbmMcbm2019CheckTimingPairs";
}

//______________________________________________________________________________
const char *CbmMcbm2019CheckTimingPairs::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckTimingPairs*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2019CheckTimingPairs::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckTimingPairs*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2019CheckTimingPairs::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckTimingPairs*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2019CheckTimingPairs::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019CheckTimingPairs*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2019TimeWinEventBuilderAlgo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2019TimeWinEventBuilderAlgo::Class_Name()
{
   return "CbmMcbm2019TimeWinEventBuilderAlgo";
}

//______________________________________________________________________________
const char *CbmMcbm2019TimeWinEventBuilderAlgo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019TimeWinEventBuilderAlgo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2019TimeWinEventBuilderAlgo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019TimeWinEventBuilderAlgo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2019TimeWinEventBuilderAlgo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019TimeWinEventBuilderAlgo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2019TimeWinEventBuilderAlgo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019TimeWinEventBuilderAlgo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2019TimeWinEventBuilderTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2019TimeWinEventBuilderTask::Class_Name()
{
   return "CbmMcbm2019TimeWinEventBuilderTask";
}

//______________________________________________________________________________
const char *CbmMcbm2019TimeWinEventBuilderTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019TimeWinEventBuilderTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2019TimeWinEventBuilderTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019TimeWinEventBuilderTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2019TimeWinEventBuilderTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019TimeWinEventBuilderTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2019TimeWinEventBuilderTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2019TimeWinEventBuilderTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbmCheckTimingAlgo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbmCheckTimingAlgo::Class_Name()
{
   return "CbmMcbmCheckTimingAlgo";
}

//______________________________________________________________________________
const char *CbmMcbmCheckTimingAlgo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmCheckTimingAlgo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbmCheckTimingAlgo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmCheckTimingAlgo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbmCheckTimingAlgo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmCheckTimingAlgo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbmCheckTimingAlgo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmCheckTimingAlgo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbmCheckTimingTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbmCheckTimingTask::Class_Name()
{
   return "CbmMcbmCheckTimingTask";
}

//______________________________________________________________________________
const char *CbmMcbmCheckTimingTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmCheckTimingTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbmCheckTimingTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmCheckTimingTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbmCheckTimingTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmCheckTimingTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbmCheckTimingTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmCheckTimingTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbmSpillFindAlgo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbmSpillFindAlgo::Class_Name()
{
   return "CbmMcbmSpillFindAlgo";
}

//______________________________________________________________________________
const char *CbmMcbmSpillFindAlgo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmSpillFindAlgo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbmSpillFindAlgo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmSpillFindAlgo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbmSpillFindAlgo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmSpillFindAlgo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbmSpillFindAlgo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmSpillFindAlgo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbmSpillFindTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbmSpillFindTask::Class_Name()
{
   return "CbmMcbmSpillFindTask";
}

//______________________________________________________________________________
const char *CbmMcbmSpillFindTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmSpillFindTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbmSpillFindTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmSpillFindTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbmSpillFindTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmSpillFindTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbmSpillFindTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbmSpillFindTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmMcbmUnpack::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbmUnpack.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmMcbmUnpack(void *p) {
      delete ((::CbmMcbmUnpack*)p);
   }
   static void deleteArray_CbmMcbmUnpack(void *p) {
      delete [] ((::CbmMcbmUnpack*)p);
   }
   static void destruct_CbmMcbmUnpack(void *p) {
      typedef ::CbmMcbmUnpack current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbmUnpack(TBuffer &buf, void *obj) {
      ((::CbmMcbmUnpack*)obj)->::CbmMcbmUnpack::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbmUnpack

//______________________________________________________________________________
void CbmMcbm2018Source::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018Source.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairSource::Streamer(R__b);
      fFileName.Streamer(R__b);
      fDirName.Streamer(R__b);
      fInputFileList.Streamer(R__b);
      fHost.Streamer(R__b);
      R__b >> fTSNumber;
      R__b >> fTSCounter;
      R__b >> fTSLength;
      R__b >> fTSOverlappLength;
      fTimer.Streamer(R__b);
      R__b >> fNofTSSinceLastTS;
      R__b >> fuTsReduction;
      R__b >> fuSubscriberHwm;
      fTimeSliceMetaDataArray->Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018Source::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018Source::IsA(), kTRUE);
      FairSource::Streamer(R__b);
      fFileName.Streamer(R__b);
      fDirName.Streamer(R__b);
      fInputFileList.Streamer(R__b);
      fHost.Streamer(R__b);
      R__b << fTSNumber;
      R__b << fTSCounter;
      R__b << fTSLength;
      R__b << fTSOverlappLength;
      fTimer.Streamer(R__b);
      R__b << fNofTSSinceLastTS;
      R__b << fuTsReduction;
      R__b << fuSubscriberHwm;
      fTimeSliceMetaDataArray->Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018Source(void *p) {
      return  p ? new(p) ::CbmMcbm2018Source : new ::CbmMcbm2018Source;
   }
   static void *newArray_CbmMcbm2018Source(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018Source[nElements] : new ::CbmMcbm2018Source[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018Source(void *p) {
      delete ((::CbmMcbm2018Source*)p);
   }
   static void deleteArray_CbmMcbm2018Source(void *p) {
      delete [] ((::CbmMcbm2018Source*)p);
   }
   static void destruct_CbmMcbm2018Source(void *p) {
      typedef ::CbmMcbm2018Source current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018Source(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018Source*)obj)->::CbmMcbm2018Source::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018Source

//______________________________________________________________________________
void CbmMcbm2018MsCrcCheck::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MsCrcCheck.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      fFileName.Streamer(R__b);
      fInputFileList.Streamer(R__b);
      R__b >> fFileCounter;
      R__b >> fTSNumber;
      R__b >> fTSCounter;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MsCrcCheck::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MsCrcCheck::IsA(), kTRUE);
      fFileName.Streamer(R__b);
      fInputFileList.Streamer(R__b);
      R__b << fFileCounter;
      R__b << fTSNumber;
      R__b << fTSCounter;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MsCrcCheck(void *p) {
      return  p ? new(p) ::CbmMcbm2018MsCrcCheck : new ::CbmMcbm2018MsCrcCheck;
   }
   static void *newArray_CbmMcbm2018MsCrcCheck(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MsCrcCheck[nElements] : new ::CbmMcbm2018MsCrcCheck[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MsCrcCheck(void *p) {
      delete ((::CbmMcbm2018MsCrcCheck*)p);
   }
   static void deleteArray_CbmMcbm2018MsCrcCheck(void *p) {
      delete [] ((::CbmMcbm2018MsCrcCheck*)p);
   }
   static void destruct_CbmMcbm2018MsCrcCheck(void *p) {
      typedef ::CbmMcbm2018MsCrcCheck current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MsCrcCheck(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MsCrcCheck*)obj)->::CbmMcbm2018MsCrcCheck::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MsCrcCheck

//______________________________________________________________________________
void CbmMcbm2018RawConverterSdpb::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018RawConverterSdpb.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fParCList;
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018RawConverterSdpb::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018RawConverterSdpb::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fParCList;
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018RawConverterSdpb(void *p) {
      return  p ? new(p) ::CbmMcbm2018RawConverterSdpb : new ::CbmMcbm2018RawConverterSdpb;
   }
   static void *newArray_CbmMcbm2018RawConverterSdpb(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018RawConverterSdpb[nElements] : new ::CbmMcbm2018RawConverterSdpb[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018RawConverterSdpb(void *p) {
      delete ((::CbmMcbm2018RawConverterSdpb*)p);
   }
   static void deleteArray_CbmMcbm2018RawConverterSdpb(void *p) {
      delete [] ((::CbmMcbm2018RawConverterSdpb*)p);
   }
   static void destruct_CbmMcbm2018RawConverterSdpb(void *p) {
      typedef ::CbmMcbm2018RawConverterSdpb current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018RawConverterSdpb(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018RawConverterSdpb*)obj)->::CbmMcbm2018RawConverterSdpb::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018RawConverterSdpb

//______________________________________________________________________________
void CbmMcbm2018RawConverterGdpb::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018RawConverterGdpb.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fParCList;
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018RawConverterGdpb::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018RawConverterGdpb::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fParCList;
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018RawConverterGdpb(void *p) {
      return  p ? new(p) ::CbmMcbm2018RawConverterGdpb : new ::CbmMcbm2018RawConverterGdpb;
   }
   static void *newArray_CbmMcbm2018RawConverterGdpb(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018RawConverterGdpb[nElements] : new ::CbmMcbm2018RawConverterGdpb[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018RawConverterGdpb(void *p) {
      delete ((::CbmMcbm2018RawConverterGdpb*)p);
   }
   static void deleteArray_CbmMcbm2018RawConverterGdpb(void *p) {
      delete [] ((::CbmMcbm2018RawConverterGdpb*)p);
   }
   static void destruct_CbmMcbm2018RawConverterGdpb(void *p) {
      typedef ::CbmMcbm2018RawConverterGdpb current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018RawConverterGdpb(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018RawConverterGdpb*)obj)->::CbmMcbm2018RawConverterGdpb::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018RawConverterGdpb

//______________________________________________________________________________
void CbmCriGet4RawPrint::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCriGet4RawPrint.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      R__b.CheckByteCount(R__s, R__c, CbmCriGet4RawPrint::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmCriGet4RawPrint::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmCriGet4RawPrint(void *p) {
      return  p ? new(p) ::CbmCriGet4RawPrint : new ::CbmCriGet4RawPrint;
   }
   static void *newArray_CbmCriGet4RawPrint(Long_t nElements, void *p) {
      return p ? new(p) ::CbmCriGet4RawPrint[nElements] : new ::CbmCriGet4RawPrint[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmCriGet4RawPrint(void *p) {
      delete ((::CbmCriGet4RawPrint*)p);
   }
   static void deleteArray_CbmCriGet4RawPrint(void *p) {
      delete [] ((::CbmCriGet4RawPrint*)p);
   }
   static void destruct_CbmCriGet4RawPrint(void *p) {
      typedef ::CbmCriGet4RawPrint current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmCriGet4RawPrint(TBuffer &buf, void *obj) {
      ((::CbmCriGet4RawPrint*)obj)->::CbmCriGet4RawPrint::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmCriGet4RawPrint

//______________________________________________________________________________
void CbmMcbm2018UnpackerAlgoSts::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018UnpackerAlgoSts.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fbBinningFw;
      R__b >> fdTimeOffsetNs;
      {
         vector<Double_t> &R__stl =  fvdTimeOffsetNsAsics;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            double R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fbUseChannelMask;
      R__b >> fdAdcCut;
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            stsxyter::MessType R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<stsxyter::MessType>(readtemp);
            unsigned int R__t2;
            R__b >> R__t2;
            typedef enum stsxyter::MessType Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fdStartTime;
      R__b >> fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      {
         vector<CbmStsDigi> &R__stl =  fPulserDigiVect;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            CbmStsDigi R__t;
            R__t.Streamer(R__b);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhVectorSize;
      R__b >> fhVectorCapacity;
      R__b >> fuTsMaxVectorSize;
      R__b >> fdCapacityIncFactor;
      R__b >> fhMsCntEvo;
      R__b >> fhMsErrorsEvo;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018UnpackerAlgoSts::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018UnpackerAlgoSts::IsA(), kTRUE);
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fbBinningFw;
      R__b << fdTimeOffsetNs;
      {
         vector<Double_t> &R__stl =  fvdTimeOffsetNsAsics;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Double_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fbUseChannelMask;
      R__b << fdAdcCut;
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<stsxyter::MessType,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fdStartTime;
      R__b << fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      {
         vector<CbmStsDigi> &R__stl =  fPulserDigiVect;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<CbmStsDigi>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            ((CbmStsDigi&)(*R__k)).Streamer(R__b);
            }
         }
      }
      R__b << fhVectorSize;
      R__b << fhVectorCapacity;
      R__b << fuTsMaxVectorSize;
      R__b << fdCapacityIncFactor;
      R__b << fhMsCntEvo;
      R__b << fhMsErrorsEvo;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018UnpackerAlgoSts(void *p) {
      return  p ? new(p) ::CbmMcbm2018UnpackerAlgoSts : new ::CbmMcbm2018UnpackerAlgoSts;
   }
   static void *newArray_CbmMcbm2018UnpackerAlgoSts(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018UnpackerAlgoSts[nElements] : new ::CbmMcbm2018UnpackerAlgoSts[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018UnpackerAlgoSts(void *p) {
      delete ((::CbmMcbm2018UnpackerAlgoSts*)p);
   }
   static void deleteArray_CbmMcbm2018UnpackerAlgoSts(void *p) {
      delete [] ((::CbmMcbm2018UnpackerAlgoSts*)p);
   }
   static void destruct_CbmMcbm2018UnpackerAlgoSts(void *p) {
      typedef ::CbmMcbm2018UnpackerAlgoSts current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018UnpackerAlgoSts(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018UnpackerAlgoSts*)obj)->::CbmMcbm2018UnpackerAlgoSts::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018UnpackerAlgoSts

//______________________________________________________________________________
void CbmMcbm2018UnpackerTaskSts::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018UnpackerTaskSts.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2018UnpackerTaskSts::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2018UnpackerTaskSts::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018UnpackerTaskSts(void *p) {
      return  p ? new(p) ::CbmMcbm2018UnpackerTaskSts : new ::CbmMcbm2018UnpackerTaskSts;
   }
   static void *newArray_CbmMcbm2018UnpackerTaskSts(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018UnpackerTaskSts[nElements] : new ::CbmMcbm2018UnpackerTaskSts[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018UnpackerTaskSts(void *p) {
      delete ((::CbmMcbm2018UnpackerTaskSts*)p);
   }
   static void deleteArray_CbmMcbm2018UnpackerTaskSts(void *p) {
      delete [] ((::CbmMcbm2018UnpackerTaskSts*)p);
   }
   static void destruct_CbmMcbm2018UnpackerTaskSts(void *p) {
      typedef ::CbmMcbm2018UnpackerTaskSts current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2018UnpackerTaskSts

//______________________________________________________________________________
void CbmMcbm2018UnpackerAlgoMuch::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018UnpackerAlgoMuch.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fbBinningFw;
      R__b >> fdTimeOffsetNs;
      {
         vector<Double_t> &R__stl =  fvdTimeOffsetNsAsics;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            double R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fbUseChannelMask;
      R__b >> fdAdcCut;
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            stsxyter::MessType R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<stsxyter::MessType>(readtemp);
            unsigned int R__t2;
            R__b >> R__t2;
            typedef enum stsxyter::MessType Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fdStartTime;
      R__b >> fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b >> fhVectorSize;
      R__b >> fhVectorCapacity;
      R__b >> fuTsMaxVectorSize;
      R__b >> fdCapacityIncFactor;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018UnpackerAlgoMuch::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018UnpackerAlgoMuch::IsA(), kTRUE);
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fbBinningFw;
      R__b << fdTimeOffsetNs;
      {
         vector<Double_t> &R__stl =  fvdTimeOffsetNsAsics;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Double_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fbUseChannelMask;
      R__b << fdAdcCut;
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<stsxyter::MessType,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fdStartTime;
      R__b << fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b << fhVectorSize;
      R__b << fhVectorCapacity;
      R__b << fuTsMaxVectorSize;
      R__b << fdCapacityIncFactor;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018UnpackerAlgoMuch(void *p) {
      return  p ? new(p) ::CbmMcbm2018UnpackerAlgoMuch : new ::CbmMcbm2018UnpackerAlgoMuch;
   }
   static void *newArray_CbmMcbm2018UnpackerAlgoMuch(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018UnpackerAlgoMuch[nElements] : new ::CbmMcbm2018UnpackerAlgoMuch[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018UnpackerAlgoMuch(void *p) {
      delete ((::CbmMcbm2018UnpackerAlgoMuch*)p);
   }
   static void deleteArray_CbmMcbm2018UnpackerAlgoMuch(void *p) {
      delete [] ((::CbmMcbm2018UnpackerAlgoMuch*)p);
   }
   static void destruct_CbmMcbm2018UnpackerAlgoMuch(void *p) {
      typedef ::CbmMcbm2018UnpackerAlgoMuch current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018UnpackerAlgoMuch(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018UnpackerAlgoMuch*)obj)->::CbmMcbm2018UnpackerAlgoMuch::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018UnpackerAlgoMuch

//______________________________________________________________________________
void CbmMcbm2018UnpackerTaskMuch::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018UnpackerTaskMuch.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2018UnpackerTaskMuch::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2018UnpackerTaskMuch::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018UnpackerTaskMuch(void *p) {
      return  p ? new(p) ::CbmMcbm2018UnpackerTaskMuch : new ::CbmMcbm2018UnpackerTaskMuch;
   }
   static void *newArray_CbmMcbm2018UnpackerTaskMuch(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018UnpackerTaskMuch[nElements] : new ::CbmMcbm2018UnpackerTaskMuch[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018UnpackerTaskMuch(void *p) {
      delete ((::CbmMcbm2018UnpackerTaskMuch*)p);
   }
   static void deleteArray_CbmMcbm2018UnpackerTaskMuch(void *p) {
      delete [] ((::CbmMcbm2018UnpackerTaskMuch*)p);
   }
   static void destruct_CbmMcbm2018UnpackerTaskMuch(void *p) {
      typedef ::CbmMcbm2018UnpackerTaskMuch current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2018UnpackerTaskMuch

//______________________________________________________________________________
void CbmMcbm2018UnpackerAlgoTof::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018UnpackerAlgoTof.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fuNrOfGbtx;
      R__b >> fuNrOfModules;
      {
         vector<Int_t> &R__stl =  fviNrOfRpc;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcType;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcSide;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviModuleId;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcChUId;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fdTimeOffsetNs;
      R__b >> fuDiamondDpbIdx;
      R__b >> fdStartTime;
      R__b >> fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b >> fbEpochFoundInThisMs;
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsMsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsLsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMid;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsFullLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsFullLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTokenLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarDaqCmdLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTrigCmdLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fdRefTime;
      R__b >> fdLastDigiTime;
      R__b >> fdFirstDigiTimeDif;
      R__b >> fdEvTime0;
      R__b >> fhRawTDigEvT0;
      R__b >> fhRawTDigRef0;
      R__b >> fhRawTDigRef;
      R__b >> fhRawTRefDig0;
      R__b >> fhRawTRefDig1;
      R__b >> fhRawDigiLastDigi;
      {
         vector<TH2*> &R__stl =  fhRawTotCh;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhRawTotCh streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fhChCount;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhChCount streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fhChCountRemap;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhChCountRemap streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<Bool_t> &R__stl =  fvbChanThere;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhChanCoinc;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhChanCoinc streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b.ReadStaticArray((bool*)fbDetChanThere);
      R__b >> fhDetChanCoinc;
      R__b >> fhVectorSize;
      R__b >> fhVectorCapacity;
      R__b >> fuTsMaxVectorSize;
      R__b >> fdCapacityIncFactor;
      R__b >> fMaxAsicInactive;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018UnpackerAlgoTof::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018UnpackerAlgoTof::IsA(), kTRUE);
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fuNrOfGbtx;
      R__b << fuNrOfModules;
      {
         vector<Int_t> &R__stl =  fviNrOfRpc;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcType;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcSide;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviModuleId;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcChUId;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fdTimeOffsetNs;
      R__b << fuDiamondDpbIdx;
      R__b << fdStartTime;
      R__b << fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b << fbEpochFoundInThisMs;
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsMsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsLsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMid;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsFullLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsFullLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTokenLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarDaqCmdLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTrigCmdLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fdRefTime;
      R__b << fdLastDigiTime;
      R__b << fdFirstDigiTimeDif;
      R__b << fdEvTime0;
      R__b << fhRawTDigEvT0;
      R__b << fhRawTDigRef0;
      R__b << fhRawTDigRef;
      R__b << fhRawTRefDig0;
      R__b << fhRawTRefDig1;
      R__b << fhRawDigiLastDigi;
      {
         vector<TH2*> &R__stl =  fhRawTotCh;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhChCount;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhChCountRemap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Bool_t> &R__stl =  fvbChanThere;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhChanCoinc;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b.WriteArray(fbDetChanThere, 64);
      R__b << fhDetChanCoinc;
      R__b << fhVectorSize;
      R__b << fhVectorCapacity;
      R__b << fuTsMaxVectorSize;
      R__b << fdCapacityIncFactor;
      R__b << fMaxAsicInactive;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018UnpackerAlgoTof(void *p) {
      return  p ? new(p) ::CbmMcbm2018UnpackerAlgoTof : new ::CbmMcbm2018UnpackerAlgoTof;
   }
   static void *newArray_CbmMcbm2018UnpackerAlgoTof(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018UnpackerAlgoTof[nElements] : new ::CbmMcbm2018UnpackerAlgoTof[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018UnpackerAlgoTof(void *p) {
      delete ((::CbmMcbm2018UnpackerAlgoTof*)p);
   }
   static void deleteArray_CbmMcbm2018UnpackerAlgoTof(void *p) {
      delete [] ((::CbmMcbm2018UnpackerAlgoTof*)p);
   }
   static void destruct_CbmMcbm2018UnpackerAlgoTof(void *p) {
      typedef ::CbmMcbm2018UnpackerAlgoTof current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018UnpackerAlgoTof(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018UnpackerAlgoTof*)obj)->::CbmMcbm2018UnpackerAlgoTof::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018UnpackerAlgoTof

//______________________________________________________________________________
void CbmMcbm2018UnpackerTaskTof::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018UnpackerTaskTof.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2018UnpackerTaskTof::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2018UnpackerTaskTof::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018UnpackerTaskTof(void *p) {
      return  p ? new(p) ::CbmMcbm2018UnpackerTaskTof : new ::CbmMcbm2018UnpackerTaskTof;
   }
   static void *newArray_CbmMcbm2018UnpackerTaskTof(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018UnpackerTaskTof[nElements] : new ::CbmMcbm2018UnpackerTaskTof[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018UnpackerTaskTof(void *p) {
      delete ((::CbmMcbm2018UnpackerTaskTof*)p);
   }
   static void deleteArray_CbmMcbm2018UnpackerTaskTof(void *p) {
      delete [] ((::CbmMcbm2018UnpackerTaskTof*)p);
   }
   static void destruct_CbmMcbm2018UnpackerTaskTof(void *p) {
      typedef ::CbmMcbm2018UnpackerTaskTof current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2018UnpackerTaskTof

//______________________________________________________________________________
void CbmMcbm2018UnpackerAlgoRich::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018UnpackerAlgoRich.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b >> fToTMin;
      R__b >> fToTMax;
      R__b >> fbMonitorMode;
      R__b >> fbDebugMonitorMode;
      R__b >> fRawDataMode;
      R__b >> fTsCounter;
      R__b >> fMsInd;
      R__b >> fMbsPrevTimeCh0;
      R__b >> fMbsPrevTimeCh1;
      {
         map<uint32_t,double> &R__stl =  fLastCh0ReTime;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            double R__t2;
            R__b >> R__t2;
            typedef unsigned int Value_t;
            std::pair<Value_t const, double > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         map<uint32_t,double> &R__stl =  fPrevLastCh0ReTime;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            double R__t2;
            R__b >> R__t2;
            typedef unsigned int Value_t;
            std::pair<Value_t const, double > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fbDoToTCorr;
      R__b >> fdTimeOffsetNs;
      R__b >> fRICHcompIdx;
      {
         vector<TCanvas*> &R__stl =  fcTot2d;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TCanvas *));
         if (R__tcl1==0) {
            Error("fcTot2d streamer","Missing the TClass object for class TCanvas *!");
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
      R__b >> fhTdcErrors;
      R__b >> fhEventSize;
      R__b >> fhSubEventSize;
      R__b >> fhSubSubEventSize;
      R__b >> fhChnlSize;
      {
         map<uint16_t,uint16_t> &R__stl =  fMapFEE;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned short R__t;
            R__b >> R__t;
            unsigned short R__t2;
            R__b >> R__t2;
            typedef unsigned short Value_t;
            std::pair<Value_t const, unsigned short > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         map<uint32_t,map<Int_t,TH1D*> > &R__stl =  fhTotMap;
         R__stl.clear();
         TClass *R__tcl2 = TBuffer::GetClass(typeid(class std::map<int, class TH1D *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1D *> > >));
         if (R__tcl2==0) {
            Error("fhTotMap streamer","Missing the TClass object for class std::map<int, class TH1D *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1D *> > >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            map<int,TH1D*> R__t2;
            R__b.StreamObject(&R__t2,R__tcl2);
            typedef unsigned int Value_t;
            std::pair<Value_t const, class std::map<int, class TH1D *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1D *> > > > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         map<uint32_t,TH2D*> &R__stl =  fhTot2dMap;
         R__stl.clear();
         TClass *R__tcl2 = TBuffer::GetClass(typeid(class TH2D *));
         if (R__tcl2==0) {
            Error("fhTot2dMap streamer","Missing the TClass object for class TH2D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            TH2D* R__t2;
            R__t2 = (TH2D*)R__b.ReadObjectAny(R__tcl2);
            typedef unsigned int Value_t;
            std::pair<Value_t const, class TH2D * > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fdTsStartTime;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018UnpackerAlgoRich::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018UnpackerAlgoRich::IsA(), kTRUE);
      R__b << fToTMin;
      R__b << fToTMax;
      R__b << fbMonitorMode;
      R__b << fbDebugMonitorMode;
      R__b << fRawDataMode;
      R__b << fTsCounter;
      R__b << fMsInd;
      R__b << fMbsPrevTimeCh0;
      R__b << fMbsPrevTimeCh1;
      {
         map<uint32_t,double> &R__stl =  fLastCh0ReTime;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<uint32_t,double>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      {
         map<uint32_t,double> &R__stl =  fPrevLastCh0ReTime;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<uint32_t,double>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fbDoToTCorr;
      R__b << fdTimeOffsetNs;
      R__b << fRICHcompIdx;
      {
         vector<TCanvas*> &R__stl =  fcTot2d;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TCanvas*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << (TObject*)fhTdcErrors;
      R__b << fhEventSize;
      R__b << fhSubEventSize;
      R__b << fhSubSubEventSize;
      R__b << fhChnlSize;
      {
         map<uint16_t,uint16_t> &R__stl =  fMapFEE;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<uint16_t,uint16_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      {
         map<uint32_t,map<Int_t,TH1D*> > &R__stl =  fhTotMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl2 = TBuffer::GetClass(typeid(class std::map<int, class TH1D *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1D *> > >));
         if (R__tcl2==0) {
            Error("fhTotMapstreamer","Missing the TClass object for class std::map<int, class TH1D *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1D *> > >!");
            return;
         }
            map<uint32_t,map<Int_t,TH1D*> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b.StreamObject((map<int,TH1D*>*)&((*R__k).second),R__tcl2);
            }
         }
      }
      {
         map<uint32_t,TH2D*> &R__stl =  fhTot2dMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<uint32_t,TH2D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fdTsStartTime;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018UnpackerAlgoRich(void *p) {
      return  p ? new(p) ::CbmMcbm2018UnpackerAlgoRich : new ::CbmMcbm2018UnpackerAlgoRich;
   }
   static void *newArray_CbmMcbm2018UnpackerAlgoRich(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018UnpackerAlgoRich[nElements] : new ::CbmMcbm2018UnpackerAlgoRich[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018UnpackerAlgoRich(void *p) {
      delete ((::CbmMcbm2018UnpackerAlgoRich*)p);
   }
   static void deleteArray_CbmMcbm2018UnpackerAlgoRich(void *p) {
      delete [] ((::CbmMcbm2018UnpackerAlgoRich*)p);
   }
   static void destruct_CbmMcbm2018UnpackerAlgoRich(void *p) {
      typedef ::CbmMcbm2018UnpackerAlgoRich current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018UnpackerAlgoRich(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018UnpackerAlgoRich*)obj)->::CbmMcbm2018UnpackerAlgoRich::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018UnpackerAlgoRich

//______________________________________________________________________________
void CbmMcbm2018UnpackerTaskRich::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018UnpackerTaskRich.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2018UnpackerTaskRich::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2018UnpackerTaskRich::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018UnpackerTaskRich(void *p) {
      return  p ? new(p) ::CbmMcbm2018UnpackerTaskRich : new ::CbmMcbm2018UnpackerTaskRich;
   }
   static void *newArray_CbmMcbm2018UnpackerTaskRich(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018UnpackerTaskRich[nElements] : new ::CbmMcbm2018UnpackerTaskRich[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018UnpackerTaskRich(void *p) {
      delete ((::CbmMcbm2018UnpackerTaskRich*)p);
   }
   static void deleteArray_CbmMcbm2018UnpackerTaskRich(void *p) {
      delete [] ((::CbmMcbm2018UnpackerTaskRich*)p);
   }
   static void destruct_CbmMcbm2018UnpackerTaskRich(void *p) {
      typedef ::CbmMcbm2018UnpackerTaskRich current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2018UnpackerTaskRich

//______________________________________________________________________________
void CbmMcbm2018UnpackerTaskRich2020::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018UnpackerTaskRich2020.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2018UnpackerTaskRich2020::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2018UnpackerTaskRich2020::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018UnpackerTaskRich2020(void *p) {
      return  p ? new(p) ::CbmMcbm2018UnpackerTaskRich2020 : new ::CbmMcbm2018UnpackerTaskRich2020;
   }
   static void *newArray_CbmMcbm2018UnpackerTaskRich2020(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018UnpackerTaskRich2020[nElements] : new ::CbmMcbm2018UnpackerTaskRich2020[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018UnpackerTaskRich2020(void *p) {
      delete ((::CbmMcbm2018UnpackerTaskRich2020*)p);
   }
   static void deleteArray_CbmMcbm2018UnpackerTaskRich2020(void *p) {
      delete [] ((::CbmMcbm2018UnpackerTaskRich2020*)p);
   }
   static void destruct_CbmMcbm2018UnpackerTaskRich2020(void *p) {
      typedef ::CbmMcbm2018UnpackerTaskRich2020 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2018UnpackerTaskRich2020

//______________________________________________________________________________
void CbmMcbm2018UnpackerAlgoRich2020::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018UnpackerAlgoRich2020.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b >> fRawDataMode;
      void *ptr_fTrbState = (void*)&fTrbState;
      R__b >> *reinterpret_cast<Int_t*>(ptr_fTrbState);
      R__b >> fErrorCorr;
      R__b >> fbDoToTCorr;
      R__b >> fSkipMs;
      R__b >> fdTimeOffsetNs;
      R__b >> fDataSize;
      R__b >> fRICHcompIdx;
      R__b >> fTScounter;
      R__b >> fLastFeeOnHub;
      {
         vector<Int_t> &R__stl =  fTDCAlignmentErrorPositions;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fTdcWordCorrectionCnt;
      R__b >> fTdcWordCorrectionGlobalCnt;
      R__b >> fSkipCnt;
      R__b >> fhVectorSize;
      R__b >> fhVectorCapacity;
      R__b >> fhTdcErrors;
      R__b >> fhEventErrors;
      R__b >> fhDiRICHWords;
      R__b >> fhChnlWords;
      R__b >> fhEventSize;
      R__b >> fhSubEventSize;
      R__b >> fhSubSubEventSize;
      R__b >> fhChnlSize;
      R__b.StreamObject(&(fChnlMsgCnt),typeid(fChnlMsgCnt));
      R__b >> fDebugPrint;
      {
         map<uint16_t,uint16_t> &R__stl =  fMapFEE;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned short R__t;
            R__b >> R__t;
            unsigned short R__t2;
            R__b >> R__t2;
            typedef unsigned short Value_t;
            std::pair<Value_t const, unsigned short > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         map<Int_t,map<Int_t,TH1D*> > &R__stl =  fhTotMap;
         R__stl.clear();
         TClass *R__tcl2 = TBuffer::GetClass(typeid(class std::map<int, class TH1D *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1D *> > >));
         if (R__tcl2==0) {
            Error("fhTotMap streamer","Missing the TClass object for class std::map<int, class TH1D *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1D *> > >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            map<int,TH1D*> R__t2;
            R__b.StreamObject(&R__t2,R__tcl2);
            typedef int Value_t;
            std::pair<Value_t const, class std::map<int, class TH1D *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1D *> > > > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         map<Int_t,TH2D*> &R__stl =  fhTot2dMap;
         R__stl.clear();
         TClass *R__tcl2 = TBuffer::GetClass(typeid(class TH2D *));
         if (R__tcl2==0) {
            Error("fhTot2dMap streamer","Missing the TClass object for class TH2D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            TH2D* R__t2;
            R__t2 = (TH2D*)R__b.ReadObjectAny(R__tcl2);
            typedef int Value_t;
            std::pair<Value_t const, class TH2D * > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fuTsMaxVectorSize;
      R__b >> fdCapacityIncFactor;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018UnpackerAlgoRich2020::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018UnpackerAlgoRich2020::IsA(), kTRUE);
      R__b << fRawDataMode;
      R__b << (Int_t)fTrbState;
      R__b << fErrorCorr;
      R__b << fbDoToTCorr;
      R__b << fSkipMs;
      R__b << fdTimeOffsetNs;
      R__b << fDataSize;
      R__b << fRICHcompIdx;
      R__b << fTScounter;
      R__b << fLastFeeOnHub;
      {
         vector<Int_t> &R__stl =  fTDCAlignmentErrorPositions;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fTdcWordCorrectionCnt;
      R__b << fTdcWordCorrectionGlobalCnt;
      R__b << fSkipCnt;
      R__b << fhVectorSize;
      R__b << fhVectorCapacity;
      R__b << (TObject*)fhTdcErrors;
      R__b << (TObject*)fhEventErrors;
      R__b << (TObject*)fhDiRICHWords;
      R__b << (TObject*)fhChnlWords;
      R__b << fhEventSize;
      R__b << fhSubEventSize;
      R__b << fhSubSubEventSize;
      R__b << fhChnlSize;
      R__b.StreamObject(&(fChnlMsgCnt),typeid(fChnlMsgCnt));
      R__b << fDebugPrint;
      {
         map<uint16_t,uint16_t> &R__stl =  fMapFEE;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<uint16_t,uint16_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      {
         map<Int_t,map<Int_t,TH1D*> > &R__stl =  fhTotMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl2 = TBuffer::GetClass(typeid(class std::map<int, class TH1D *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1D *> > >));
         if (R__tcl2==0) {
            Error("fhTotMapstreamer","Missing the TClass object for class std::map<int, class TH1D *, struct std::less<int>, class std::allocator<struct std::pair<const int, class TH1D *> > >!");
            return;
         }
            map<Int_t,map<Int_t,TH1D*> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b.StreamObject((map<int,TH1D*>*)&((*R__k).second),R__tcl2);
            }
         }
      }
      {
         map<Int_t,TH2D*> &R__stl =  fhTot2dMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<Int_t,TH2D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fuTsMaxVectorSize;
      R__b << fdCapacityIncFactor;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018UnpackerAlgoRich2020(void *p) {
      return  p ? new(p) ::CbmMcbm2018UnpackerAlgoRich2020 : new ::CbmMcbm2018UnpackerAlgoRich2020;
   }
   static void *newArray_CbmMcbm2018UnpackerAlgoRich2020(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018UnpackerAlgoRich2020[nElements] : new ::CbmMcbm2018UnpackerAlgoRich2020[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018UnpackerAlgoRich2020(void *p) {
      delete ((::CbmMcbm2018UnpackerAlgoRich2020*)p);
   }
   static void deleteArray_CbmMcbm2018UnpackerAlgoRich2020(void *p) {
      delete [] ((::CbmMcbm2018UnpackerAlgoRich2020*)p);
   }
   static void destruct_CbmMcbm2018UnpackerAlgoRich2020(void *p) {
      typedef ::CbmMcbm2018UnpackerAlgoRich2020 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018UnpackerAlgoRich2020(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018UnpackerAlgoRich2020*)obj)->::CbmMcbm2018UnpackerAlgoRich2020::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018UnpackerAlgoRich2020

//______________________________________________________________________________
void CbmMcbm2018UnpackerAlgoPsd::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018UnpackerAlgoPsd.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fuNrOfGbtx;
      R__b >> fuNrOfModules;
      {
         vector<Int_t> &R__stl =  fviPsdChUId;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fdTimeOffsetNs;
      R__b >> fdStartTime;
      R__b >> fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018UnpackerAlgoPsd::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018UnpackerAlgoPsd::IsA(), kTRUE);
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fuNrOfGbtx;
      R__b << fuNrOfModules;
      {
         vector<Int_t> &R__stl =  fviPsdChUId;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fdTimeOffsetNs;
      R__b << fdStartTime;
      R__b << fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018UnpackerAlgoPsd(void *p) {
      return  p ? new(p) ::CbmMcbm2018UnpackerAlgoPsd : new ::CbmMcbm2018UnpackerAlgoPsd;
   }
   static void *newArray_CbmMcbm2018UnpackerAlgoPsd(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018UnpackerAlgoPsd[nElements] : new ::CbmMcbm2018UnpackerAlgoPsd[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018UnpackerAlgoPsd(void *p) {
      delete ((::CbmMcbm2018UnpackerAlgoPsd*)p);
   }
   static void deleteArray_CbmMcbm2018UnpackerAlgoPsd(void *p) {
      delete [] ((::CbmMcbm2018UnpackerAlgoPsd*)p);
   }
   static void destruct_CbmMcbm2018UnpackerAlgoPsd(void *p) {
      typedef ::CbmMcbm2018UnpackerAlgoPsd current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018UnpackerAlgoPsd(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018UnpackerAlgoPsd*)obj)->::CbmMcbm2018UnpackerAlgoPsd::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018UnpackerAlgoPsd

//______________________________________________________________________________
void CbmMcbm2018UnpackerTaskPsd::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018UnpackerTaskPsd.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2018UnpackerTaskPsd::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2018UnpackerTaskPsd::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018UnpackerTaskPsd(void *p) {
      return  p ? new(p) ::CbmMcbm2018UnpackerTaskPsd : new ::CbmMcbm2018UnpackerTaskPsd;
   }
   static void *newArray_CbmMcbm2018UnpackerTaskPsd(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018UnpackerTaskPsd[nElements] : new ::CbmMcbm2018UnpackerTaskPsd[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018UnpackerTaskPsd(void *p) {
      delete ((::CbmMcbm2018UnpackerTaskPsd*)p);
   }
   static void deleteArray_CbmMcbm2018UnpackerTaskPsd(void *p) {
      delete [] ((::CbmMcbm2018UnpackerTaskPsd*)p);
   }
   static void destruct_CbmMcbm2018UnpackerTaskPsd(void *p) {
      typedef ::CbmMcbm2018UnpackerTaskPsd current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2018UnpackerTaskPsd

//______________________________________________________________________________
void CbmMcbm2018UnpackerAlgoTrdR::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018UnpackerAlgoTrdR.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2018UnpackerAlgoTrdR::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2018UnpackerAlgoTrdR::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018UnpackerAlgoTrdR(void *p) {
      return  p ? new(p) ::CbmMcbm2018UnpackerAlgoTrdR : new ::CbmMcbm2018UnpackerAlgoTrdR;
   }
   static void *newArray_CbmMcbm2018UnpackerAlgoTrdR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018UnpackerAlgoTrdR[nElements] : new ::CbmMcbm2018UnpackerAlgoTrdR[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018UnpackerAlgoTrdR(void *p) {
      delete ((::CbmMcbm2018UnpackerAlgoTrdR*)p);
   }
   static void deleteArray_CbmMcbm2018UnpackerAlgoTrdR(void *p) {
      delete [] ((::CbmMcbm2018UnpackerAlgoTrdR*)p);
   }
   static void destruct_CbmMcbm2018UnpackerAlgoTrdR(void *p) {
      typedef ::CbmMcbm2018UnpackerAlgoTrdR current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2018UnpackerAlgoTrdR

//______________________________________________________________________________
void CbmMcbm2018UnpackerTaskTrdR::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018UnpackerTaskTrdR.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2018UnpackerTaskTrdR::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2018UnpackerTaskTrdR::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018UnpackerTaskTrdR(void *p) {
      return  p ? new(p) ::CbmMcbm2018UnpackerTaskTrdR : new ::CbmMcbm2018UnpackerTaskTrdR;
   }
   static void *newArray_CbmMcbm2018UnpackerTaskTrdR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018UnpackerTaskTrdR[nElements] : new ::CbmMcbm2018UnpackerTaskTrdR[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018UnpackerTaskTrdR(void *p) {
      delete ((::CbmMcbm2018UnpackerTaskTrdR*)p);
   }
   static void deleteArray_CbmMcbm2018UnpackerTaskTrdR(void *p) {
      delete [] ((::CbmMcbm2018UnpackerTaskTrdR*)p);
   }
   static void destruct_CbmMcbm2018UnpackerTaskTrdR(void *p) {
      typedef ::CbmMcbm2018UnpackerTaskTrdR current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2018UnpackerTaskTrdR

//______________________________________________________________________________
void CbmCheckDataFormatGdpb2018::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCheckDataFormatGdpb2018.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fuMsAcceptsPercent;
      R__b >> fuTotalMsNb;
      R__b >> fuOverlapMsNb;
      R__b >> fuCoreMs;
      R__b >> fdMsSizeInNs;
      R__b >> fdTsCoreSizeInNs;
      R__b >> fuMinNbGdpb;
      R__b >> fuCurrNbGdpb;
      R__b >> fuNrOfGdpbs;
      R__b >> fuNrOfFeePerGdpb;
      R__b >> fuNrOfGet4PerFee;
      R__b >> fuNrOfChannelsPerGet4;
      R__b >> fuNrOfChannelsPerFee;
      R__b >> fuNrOfGet4;
      R__b >> fuNrOfGet4PerGdpb;
      R__b >> fuNrOfChannelsPerGdpb;
      R__b >> fuNrOfGbtx;
      R__b >> fuNrOfModules;
      {
         vector<Int_t> &R__stl =  fviNrOfRpc;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcType;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcSide;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviModuleId;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> const_cast< UInt_t &>( kuNbFeePerGbtx );
      R__b >> const_cast< UInt_t &>( kuNbGbtxPerGdpb );
      R__b >> fuDiamondDpbIdx;
      fsHistoFilename.Streamer(R__b);
      R__b >> fulCurrentTsIndex;
      R__b >> fuCurrentMs;
      R__b >> fuCurrentMsSysId;
      R__b >> fdMsIndex;
      R__b >> fuGdpbId;
      R__b >> fuGdpbNr;
      R__b >> fuGet4Id;
      R__b >> fuGet4Nr;
      R__b >> fiEquipmentId;
      R__b >> fulCurrentEpochTime;
      {
         map<UInt_t,UInt_t> &R__stl =  fGdpbIdIndexMap;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            unsigned int R__t2;
            R__b >> R__t2;
            typedef unsigned int Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fcMsSizeAll;
      R__b >> fdStartTimeMsSz;
      {
         vector<TH1*> &R__stl =  fvhMsSzPerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhMsSzPerLink streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TProfile*> &R__stl =  fvhMsSzTimePerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhMsSzTimePerLink streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhMessType;
      R__b >> fhSysMessType;
      R__b >> fhGet4MessType;
      R__b >> fhGet4ChanScm;
      R__b >> fhGet4ChanErrors;
      R__b >> fhGet4EpochFlags;
      R__b >> fhGdpbMessType;
      R__b >> fhGdpbSysMessType;
      R__b >> fhGdpbSysMessPattType;
      R__b >> fhGdpbEpochFlags;
      R__b >> fhGdpbEpochSyncEvo;
      R__b >> fhGdpbEpochMissEvo;
      {
         vector<TH2*> &R__stl =  fvhGdpbGet4MessType;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhGdpbGet4MessType streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbGet4ChanErrors;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhGdpbGet4ChanErrors streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhPatternMissmatch;
      R__b >> fhPatternEnable;
      R__b >> fhPatternResync;
      {
         vector<UInt_t> &R__stl =  fvuGdpbNbEpochPerMs;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<UInt_t> > &R__stl =  fvvuChanNbHitsPerMs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<unsigned int, class std::allocator<unsigned int> >));
         if (R__tcl1==0) {
            Error("fvvuChanNbHitsPerMs streamer","Missing the TClass object for class std::vector<unsigned int, class std::allocator<unsigned int> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<unsigned int> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fhEpochsPerMs_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhEpochsPerMs_gDPB streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fhEpochsPerMsPerTs_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhEpochsPerMsPerTs_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fhEpochsDiff_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhEpochsDiff_gDPB streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fhEpochsDiffPerTs_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhEpochsDiffPerTs_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhEpochsJumpBitsPre_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhEpochsJumpBitsPre_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhEpochsJumpBitsNew_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhEpochsJumpBitsNew_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhEpochsJumpDigitsPre_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhEpochsJumpDigitsPre_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhEpochsJumpDigitsNew_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhEpochsJumpDigitsNew_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStartEpochPerMs_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStartEpochPerMs_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhCloseEpochPerMs_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhCloseEpochPerMs_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhHitsPerMsFirstChan_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhHitsPerMsFirstChan_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fvhChannelRatePerMs_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile2D *));
         if (R__tcl1==0) {
            Error("fvhChannelRatePerMs_gDPB streamer","Missing the TClass object for class TProfile2D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile2D* R__t;
            R__t = (TProfile2D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fcSummary;
      {
         vector<TCanvas*> &R__stl =  fcFormatGdpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TCanvas *));
         if (R__tcl1==0) {
            Error("fcFormatGdpb streamer","Missing the TClass object for class TCanvas *!");
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
      R__b.StreamObject(&(fTimeLastHistoSaving),typeid(fTimeLastHistoSaving));
      R__b.CheckByteCount(R__s, R__c, CbmCheckDataFormatGdpb2018::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmCheckDataFormatGdpb2018::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fuMsAcceptsPercent;
      R__b << fuTotalMsNb;
      R__b << fuOverlapMsNb;
      R__b << fuCoreMs;
      R__b << fdMsSizeInNs;
      R__b << fdTsCoreSizeInNs;
      R__b << fuMinNbGdpb;
      R__b << fuCurrNbGdpb;
      R__b << fuNrOfGdpbs;
      R__b << fuNrOfFeePerGdpb;
      R__b << fuNrOfGet4PerFee;
      R__b << fuNrOfChannelsPerGet4;
      R__b << fuNrOfChannelsPerFee;
      R__b << fuNrOfGet4;
      R__b << fuNrOfGet4PerGdpb;
      R__b << fuNrOfChannelsPerGdpb;
      R__b << fuNrOfGbtx;
      R__b << fuNrOfModules;
      {
         vector<Int_t> &R__stl =  fviNrOfRpc;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcType;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcSide;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviModuleId;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << const_cast< UInt_t &>( kuNbFeePerGbtx );
      R__b << const_cast< UInt_t &>( kuNbGbtxPerGdpb );
      R__b << fuDiamondDpbIdx;
      fsHistoFilename.Streamer(R__b);
      R__b << fulCurrentTsIndex;
      R__b << fuCurrentMs;
      R__b << fuCurrentMsSysId;
      R__b << fdMsIndex;
      R__b << fuGdpbId;
      R__b << fuGdpbNr;
      R__b << fuGet4Id;
      R__b << fuGet4Nr;
      R__b << fiEquipmentId;
      R__b << fulCurrentEpochTime;
      {
         map<UInt_t,UInt_t> &R__stl =  fGdpbIdIndexMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<UInt_t,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fcMsSizeAll;
      R__b << fdStartTimeMsSz;
      {
         vector<TH1*> &R__stl =  fvhMsSzPerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhMsSzTimePerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fhMessType;
      R__b << fhSysMessType;
      R__b << fhGet4MessType;
      R__b << fhGet4ChanScm;
      R__b << fhGet4ChanErrors;
      R__b << fhGet4EpochFlags;
      R__b << fhGdpbMessType;
      R__b << fhGdpbSysMessType;
      R__b << fhGdpbSysMessPattType;
      R__b << fhGdpbEpochFlags;
      R__b << fhGdpbEpochSyncEvo;
      R__b << fhGdpbEpochMissEvo;
      {
         vector<TH2*> &R__stl =  fvhGdpbGet4MessType;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbGet4ChanErrors;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fhPatternMissmatch;
      R__b << fhPatternEnable;
      R__b << fhPatternResync;
      {
         vector<UInt_t> &R__stl =  fvuGdpbNbEpochPerMs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<UInt_t> > &R__stl =  fvvuChanNbHitsPerMs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<unsigned int, class std::allocator<unsigned int> >));
         if (R__tcl1==0) {
            Error("fvvuChanNbHitsPerMs streamer","Missing the TClass object for class std::vector<unsigned int, class std::allocator<unsigned int> >!");
            return;
         }
            vector<vector<UInt_t> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<unsigned int>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhEpochsPerMs_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhEpochsPerMsPerTs_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhEpochsDiff_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhEpochsDiffPerTs_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhEpochsJumpBitsPre_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhEpochsJumpBitsNew_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhEpochsJumpDigitsPre_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhEpochsJumpDigitsNew_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStartEpochPerMs_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhCloseEpochPerMs_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhHitsPerMsFirstChan_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fvhChannelRatePerMs_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile2D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fcSummary;
      {
         vector<TCanvas*> &R__stl =  fcFormatGdpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TCanvas*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b.StreamObject(&(fTimeLastHistoSaving),typeid(fTimeLastHistoSaving));
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmCheckDataFormatGdpb2018(void *p) {
      return  p ? new(p) ::CbmCheckDataFormatGdpb2018 : new ::CbmCheckDataFormatGdpb2018;
   }
   static void *newArray_CbmCheckDataFormatGdpb2018(Long_t nElements, void *p) {
      return p ? new(p) ::CbmCheckDataFormatGdpb2018[nElements] : new ::CbmCheckDataFormatGdpb2018[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmCheckDataFormatGdpb2018(void *p) {
      delete ((::CbmCheckDataFormatGdpb2018*)p);
   }
   static void deleteArray_CbmCheckDataFormatGdpb2018(void *p) {
      delete [] ((::CbmCheckDataFormatGdpb2018*)p);
   }
   static void destruct_CbmCheckDataFormatGdpb2018(void *p) {
      typedef ::CbmCheckDataFormatGdpb2018 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmCheckDataFormatGdpb2018(TBuffer &buf, void *obj) {
      ((::CbmCheckDataFormatGdpb2018*)obj)->::CbmCheckDataFormatGdpb2018::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmCheckDataFormatGdpb2018

//______________________________________________________________________________
void CbmMcbm2018MonitorDataRates::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorDataRates.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fuMsAcceptsPercent;
      R__b >> fuTotalMsNb;
      R__b >> fuOverlapMsNb;
      R__b >> fuCoreMs;
      R__b >> fdMsSizeInNs;
      R__b >> fdTsCoreSizeInNs;
      fsHistoFilename.Streamer(R__b);
      R__b >> fuNbFlimLinks;
      {
         map<UInt_t,UInt_t> &R__stl =  fmChannelsPerEqId;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            unsigned int R__t2;
            R__b >> R__t2;
            typedef unsigned int Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fulCurrentTsIndex;
      R__b >> fuCurrentMs;
      R__b >> fuCurrentMsSysId;
      R__b >> fdMsIndex;
      R__b >> fiEquipmentId;
      R__b >> fdStartTimeMsSz;
      R__b >> fuHistoryHistoSize;
      {
         vector<UInt_t> &R__stl =  fvuTsSzLink;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fcDataRateTimeAll;
      R__b >> fcTsSizeAll;
      R__b >> fcTsSizeTimeAll;
      R__b >> fcMsSizeAll;
      R__b >> fcMsSizeTimeAll;
      R__b >> fcMsMessAll;
      R__b >> fcMsMessTimeAll;
      R__b >> fcMsDataChAll;
      R__b >> fcMsDataChTimeAll;
      R__b >> fhDataRateTimeAllLinks;
      {
         vector<TH1*> &R__stl =  fvhDataRateTimePerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhDataRateTimePerLink streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fvhTsSzPerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhTsSzPerLink streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TProfile*> &R__stl =  fvhTsSzTimePerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhTsSzTimePerLink streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fvhMsSzPerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhMsSzPerLink streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TProfile*> &R__stl =  fvhMsSzTimePerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhMsSzTimePerLink streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fvhMsMessPerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhMsMessPerLink streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TProfile*> &R__stl =  fvhMsMessTimePerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhMsMessTimePerLink streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fvhMsMeanChDataPerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhMsMeanChDataPerLink streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fvhMsMeanChDataTimePerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhMsMeanChDataTimePerLink streamer","Missing the TClass object for class TH1 *!");
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
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorDataRates::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorDataRates::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fuMsAcceptsPercent;
      R__b << fuTotalMsNb;
      R__b << fuOverlapMsNb;
      R__b << fuCoreMs;
      R__b << fdMsSizeInNs;
      R__b << fdTsCoreSizeInNs;
      fsHistoFilename.Streamer(R__b);
      R__b << fuNbFlimLinks;
      {
         map<UInt_t,UInt_t> &R__stl =  fmChannelsPerEqId;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<UInt_t,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fulCurrentTsIndex;
      R__b << fuCurrentMs;
      R__b << fuCurrentMsSysId;
      R__b << fdMsIndex;
      R__b << fiEquipmentId;
      R__b << fdStartTimeMsSz;
      R__b << fuHistoryHistoSize;
      {
         vector<UInt_t> &R__stl =  fvuTsSzLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fcDataRateTimeAll;
      R__b << fcTsSizeAll;
      R__b << fcTsSizeTimeAll;
      R__b << fcMsSizeAll;
      R__b << fcMsSizeTimeAll;
      R__b << fcMsMessAll;
      R__b << fcMsMessTimeAll;
      R__b << fcMsDataChAll;
      R__b << fcMsDataChTimeAll;
      R__b << fhDataRateTimeAllLinks;
      {
         vector<TH1*> &R__stl =  fvhDataRateTimePerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhTsSzPerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhTsSzTimePerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhMsSzPerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhMsSzTimePerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhMsMessPerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhMsMessTimePerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhMsMeanChDataPerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhMsMeanChDataTimePerLink;
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
   static void *new_CbmMcbm2018MonitorDataRates(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorDataRates : new ::CbmMcbm2018MonitorDataRates;
   }
   static void *newArray_CbmMcbm2018MonitorDataRates(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorDataRates[nElements] : new ::CbmMcbm2018MonitorDataRates[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorDataRates(void *p) {
      delete ((::CbmMcbm2018MonitorDataRates*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorDataRates(void *p) {
      delete [] ((::CbmMcbm2018MonitorDataRates*)p);
   }
   static void destruct_CbmMcbm2018MonitorDataRates(void *p) {
      typedef ::CbmMcbm2018MonitorDataRates current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorDataRates(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorDataRates*)obj)->::CbmMcbm2018MonitorDataRates::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorDataRates

//______________________________________________________________________________
void CbmMcbm2018MonitorAlgoT0::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorAlgoT0.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fuMinTotPulser;
      R__b >> fuMaxTotPulser;
      R__b >> fuOffSpillCountLimit;
      R__b >> fuOffSpillCountLimitNonPulser;
      R__b >> fdSpillCheckInterval;
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      {
         map<gdpbv100::MessageTypes,UInt_t> &R__stl =  fmMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            gdpbv100::MessageTypes R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<gdpbv100::MessageTypes>(readtemp);
            unsigned int R__t2;
            R__b >> R__t2;
            typedef enum gdpbv100::MessageTypes Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fdStartTime;
      R__b >> fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b >> fbSpillOn;
      R__b >> fuCurrentSpillIdx;
      R__b >> fuCurrentSpillPlot;
      R__b >> fdStartTimeSpill;
      R__b >> fdLastInterTime;
      R__b >> fuCountsLastInter;
      R__b >> fuNonPulserCountsLastInter;
      R__b >> fuHistoryHistoSize;
      {
         vector<UInt_t> &R__stl =  fvuHitCntChanMs;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuErrorCntChanMs;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuEvtLostCntChanMs;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fvhMsgCntEvoChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhMsgCntEvoChan streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fvhMsgCntPerMsEvoChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMsgCntPerMsEvoChan streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fvhHitCntEvoChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhHitCntEvoChan streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fvhHitCntPerMsEvoChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhHitCntPerMsEvoChan streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fvhErrorCntEvoChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhErrorCntEvoChan streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fvhErrorCntPerMsEvoChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhErrorCntPerMsEvoChan streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fvhEvtLostCntEvoChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhEvtLostCntEvoChan streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fvhEvtLostCntPerMsEvoChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhEvtLostCntPerMsEvoChan streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhErrorFractEvoChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhErrorFractEvoChan streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhErrorFractPerMsEvoChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhErrorFractPerMsEvoChan streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhEvtLostFractEvoChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhEvtLostFractEvoChan streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhEvtLostFractPerMsEvoChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhEvtLostFractPerMsEvoChan streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhDpbMap;
      R__b >> fhChannelMap;
      R__b >> fhHitMapEvo;
      R__b >> fhHitTotEvo;
      R__b >> fhChanHitMap;
      R__b >> fhChanHitMapEvo;
      {
         vector<TH1*> &R__stl =  fvhDpbMapSpill;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhDpbMapSpill streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fvhChannelMapSpill;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhChannelMapSpill streamer","Missing the TClass object for class TH1 *!");
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
      R__b >> fhHitsPerSpill;
      R__b >> fhMsgCntEvo;
      R__b >> fhHitCntEvo;
      R__b >> fhErrorCntEvo;
      R__b >> fhLostEvtCntEvo;
      R__b >> fhErrorFractEvo;
      R__b >> fhLostEvtFractEvo;
      R__b >> fhMsgCntPerMsEvo;
      R__b >> fhHitCntPerMsEvo;
      R__b >> fhErrorCntPerMsEvo;
      R__b >> fhLostEvtCntPerMsEvo;
      R__b >> fhErrorFractPerMsEvo;
      R__b >> fhLostEvtFractPerMsEvo;
      R__b >> fhChannelMapPulser;
      R__b >> fhHitMapEvoPulser;
      R__b >> fcSummary;
      R__b >> fcSummaryMap;
      R__b >> fcHitMaps;
      R__b >> fcGenCntsPerMs;
      R__b >> fcSpillCounts;
      R__b >> fcSpillCountsHori;
      R__b >> fcSpillDpbCountsHori;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorAlgoT0::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorAlgoT0::IsA(), kTRUE);
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fuMinTotPulser;
      R__b << fuMaxTotPulser;
      R__b << fuOffSpillCountLimit;
      R__b << fuOffSpillCountLimitNonPulser;
      R__b << fdSpillCheckInterval;
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      {
         map<gdpbv100::MessageTypes,UInt_t> &R__stl =  fmMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<gdpbv100::MessageTypes,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fdStartTime;
      R__b << fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b << fbSpillOn;
      R__b << fuCurrentSpillIdx;
      R__b << fuCurrentSpillPlot;
      R__b << fdStartTimeSpill;
      R__b << fdLastInterTime;
      R__b << fuCountsLastInter;
      R__b << fuNonPulserCountsLastInter;
      R__b << fuHistoryHistoSize;
      {
         vector<UInt_t> &R__stl =  fvuHitCntChanMs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuErrorCntChanMs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuEvtLostCntChanMs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhMsgCntEvoChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMsgCntPerMsEvoChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhHitCntEvoChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhHitCntPerMsEvoChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhErrorCntEvoChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhErrorCntPerMsEvoChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhEvtLostCntEvoChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhEvtLostCntPerMsEvoChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhErrorFractEvoChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhErrorFractPerMsEvoChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhEvtLostFractEvoChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhEvtLostFractPerMsEvoChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fhDpbMap;
      R__b << fhChannelMap;
      R__b << fhHitMapEvo;
      R__b << fhHitTotEvo;
      R__b << fhChanHitMap;
      R__b << fhChanHitMapEvo;
      {
         vector<TH1*> &R__stl =  fvhDpbMapSpill;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhChannelMapSpill;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fhHitsPerSpill;
      R__b << fhMsgCntEvo;
      R__b << fhHitCntEvo;
      R__b << fhErrorCntEvo;
      R__b << fhLostEvtCntEvo;
      R__b << (TObject*)fhErrorFractEvo;
      R__b << (TObject*)fhLostEvtFractEvo;
      R__b << fhMsgCntPerMsEvo;
      R__b << fhHitCntPerMsEvo;
      R__b << fhErrorCntPerMsEvo;
      R__b << fhLostEvtCntPerMsEvo;
      R__b << fhErrorFractPerMsEvo;
      R__b << fhLostEvtFractPerMsEvo;
      R__b << fhChannelMapPulser;
      R__b << fhHitMapEvoPulser;
      R__b << fcSummary;
      R__b << fcSummaryMap;
      R__b << fcHitMaps;
      R__b << fcGenCntsPerMs;
      R__b << fcSpillCounts;
      R__b << fcSpillCountsHori;
      R__b << fcSpillDpbCountsHori;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorAlgoT0(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorAlgoT0 : new ::CbmMcbm2018MonitorAlgoT0;
   }
   static void *newArray_CbmMcbm2018MonitorAlgoT0(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorAlgoT0[nElements] : new ::CbmMcbm2018MonitorAlgoT0[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorAlgoT0(void *p) {
      delete ((::CbmMcbm2018MonitorAlgoT0*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorAlgoT0(void *p) {
      delete [] ((::CbmMcbm2018MonitorAlgoT0*)p);
   }
   static void destruct_CbmMcbm2018MonitorAlgoT0(void *p) {
      typedef ::CbmMcbm2018MonitorAlgoT0 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorAlgoT0(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorAlgoT0*)obj)->::CbmMcbm2018MonitorAlgoT0::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorAlgoT0

//______________________________________________________________________________
void CbmMcbm2018MonitorTaskT0::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorTaskT0.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fuHistoryHistoSize;
      fsHistoFileName.Streamer(R__b);
      R__b >> fuMinTotPulser;
      R__b >> fuMaxTotPulser;
      R__b >> fuOffSpillCountLimit;
      R__b >> fuOffSpillCountLimitNonPulser;
      R__b >> fdSpillCheckInterval;
      R__b >> fulTsCounter;
      R__b >> fMonitorAlgo;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorTaskT0::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorTaskT0::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fuHistoryHistoSize;
      fsHistoFileName.Streamer(R__b);
      R__b << fuMinTotPulser;
      R__b << fuMaxTotPulser;
      R__b << fuOffSpillCountLimit;
      R__b << fuOffSpillCountLimitNonPulser;
      R__b << fdSpillCheckInterval;
      R__b << fulTsCounter;
      R__b << fMonitorAlgo;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorTaskT0(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorTaskT0 : new ::CbmMcbm2018MonitorTaskT0;
   }
   static void *newArray_CbmMcbm2018MonitorTaskT0(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorTaskT0[nElements] : new ::CbmMcbm2018MonitorTaskT0[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorTaskT0(void *p) {
      delete ((::CbmMcbm2018MonitorTaskT0*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorTaskT0(void *p) {
      delete [] ((::CbmMcbm2018MonitorTaskT0*)p);
   }
   static void destruct_CbmMcbm2018MonitorTaskT0(void *p) {
      typedef ::CbmMcbm2018MonitorTaskT0 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorTaskT0(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorTaskT0*)obj)->::CbmMcbm2018MonitorTaskT0::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorTaskT0

//______________________________________________________________________________
void CbmMcbm2018MonitorMcbmRate::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorMcbmRate.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fuMuchDpbIdx;
      {
         map<UInt_t,UInt_t> &R__stl =  fmTofDpbIdIndexMap;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            unsigned int R__t2;
            R__b >> R__t2;
            typedef unsigned int Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fuDiamondDpbIdx;
      R__b >> fuTotalNrOfDpb;
      R__b >> fdStsTofOffsetNs;
      R__b >> fdMuchTofOffsetNs;
      fsHistoFileFullname.Streamer(R__b);
      R__b >> fbPrintMessages;
      void *ptr_fPrintMessCtrlSts = (void*)&fPrintMessCtrlSts;
      R__b >> *reinterpret_cast<Int_t*>(ptr_fPrintMessCtrlSts);
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      R__b >> fulCurrentTsStartTime;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            stsxyter::MessType R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<stsxyter::MessType>(readtemp);
            unsigned int R__t2;
            R__b >> R__t2;
            typedef enum stsxyter::MessType Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fulTofCurrentTsIndex;
      R__b >> fuTofCurrentMs;
      R__b >> fdTofMsIndex;
      R__b >> fuTofGdpbId;
      R__b >> fuTofGdpbNr;
      R__b >> fiTofEquipmentId;
      {
         vector<int> &R__stl =  fviTofMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fulTofCurrentEpochTime;
      {
         vector<vector<gdpbv100::Message> > &R__stl =  fvmTofEpSupprBuffer;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >));
         if (R__tcl1==0) {
            Error("fvmTofEpSupprBuffer streamer","Missing the TClass object for class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<gdpbv100::Message> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<gdpbv100::FullMessage> > &R__stl =  fvmTofGdpbHitsInMs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >));
         if (R__tcl1==0) {
            Error("fvmTofGdpbHitsInMs streamer","Missing the TClass object for class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<gdpbv100::FullMessage> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<gdpbv100::FullMessage> > &R__stl =  fvmTofGdpbHitsInTs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >));
         if (R__tcl1==0) {
            Error("fvmTofGdpbHitsInTs streamer","Missing the TClass object for class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<gdpbv100::FullMessage> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuHitsNbInTimeBinSts;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuHitsNbInTimeBinMuch;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuHitsNbInTimeBinTof;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuHitsNbInTimeBinDiam;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhMcbmHitsNbPerTsEvo;
      R__b >> fhMcbmHitsNbFineEvo;
      R__b >> fhMcbmHitsRateEvo;
      R__b >> fhDiamondHitsRateMapEvo;
      R__b >> fhDiamondHitsRateDerivative;
      R__b >> fhMuchVsStsHitsNbPerTimeBin;
      R__b >> fhTofVsStsHitsNbPerTimeBin;
      R__b >> fhDiamVsStsHitsNbPerTimeBin;
      R__b >> fhStsVsMuchHitsNbPerTimeBin;
      R__b >> fhTofVsMuchHitsNbPerTimeBin;
      R__b >> fhDiamVsMuchHitsNbPerTimeBin;
      R__b >> fhStsVsTofHitsNbPerTimeBin;
      R__b >> fhMuchVsTofHitsNbPerTimeBin;
      R__b >> fhDiamVsTofHitsNbPerTimeBin;
      R__b >> fhStsVsDiamHitsNbPerTimeBin;
      R__b >> fhTofVsDiamHitsNbPerTimeBin;
      R__b >> fhMuchVsDiamHitsNbPerTimeBin;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorMcbmRate::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorMcbmRate::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fuMuchDpbIdx;
      {
         map<UInt_t,UInt_t> &R__stl =  fmTofDpbIdIndexMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<UInt_t,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fuDiamondDpbIdx;
      R__b << fuTotalNrOfDpb;
      R__b << fdStsTofOffsetNs;
      R__b << fdMuchTofOffsetNs;
      fsHistoFileFullname.Streamer(R__b);
      R__b << fbPrintMessages;
      R__b << (Int_t)fPrintMessCtrlSts;
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      R__b << fulCurrentTsStartTime;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<stsxyter::MessType,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fulTofCurrentTsIndex;
      R__b << fuTofCurrentMs;
      R__b << fdTofMsIndex;
      R__b << fuTofGdpbId;
      R__b << fuTofGdpbNr;
      R__b << fiTofEquipmentId;
      {
         vector<int> &R__stl =  fviTofMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fulTofCurrentEpochTime;
      {
         vector<vector<gdpbv100::Message> > &R__stl =  fvmTofEpSupprBuffer;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >));
         if (R__tcl1==0) {
            Error("fvmTofEpSupprBuffer streamer","Missing the TClass object for class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >!");
            return;
         }
            vector<vector<gdpbv100::Message> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<gdpbv100::Message>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<vector<gdpbv100::FullMessage> > &R__stl =  fvmTofGdpbHitsInMs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >));
         if (R__tcl1==0) {
            Error("fvmTofGdpbHitsInMs streamer","Missing the TClass object for class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >!");
            return;
         }
            vector<vector<gdpbv100::FullMessage> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<gdpbv100::FullMessage>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<vector<gdpbv100::FullMessage> > &R__stl =  fvmTofGdpbHitsInTs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >));
         if (R__tcl1==0) {
            Error("fvmTofGdpbHitsInTs streamer","Missing the TClass object for class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >!");
            return;
         }
            vector<vector<gdpbv100::FullMessage> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<gdpbv100::FullMessage>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuHitsNbInTimeBinSts;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuHitsNbInTimeBinMuch;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuHitsNbInTimeBinTof;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuHitsNbInTimeBinDiam;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fhMcbmHitsNbPerTsEvo;
      R__b << fhMcbmHitsNbFineEvo;
      R__b << fhMcbmHitsRateEvo;
      R__b << fhDiamondHitsRateMapEvo;
      R__b << fhDiamondHitsRateDerivative;
      R__b << fhMuchVsStsHitsNbPerTimeBin;
      R__b << fhTofVsStsHitsNbPerTimeBin;
      R__b << fhDiamVsStsHitsNbPerTimeBin;
      R__b << fhStsVsMuchHitsNbPerTimeBin;
      R__b << fhTofVsMuchHitsNbPerTimeBin;
      R__b << fhDiamVsMuchHitsNbPerTimeBin;
      R__b << fhStsVsTofHitsNbPerTimeBin;
      R__b << fhMuchVsTofHitsNbPerTimeBin;
      R__b << fhDiamVsTofHitsNbPerTimeBin;
      R__b << fhStsVsDiamHitsNbPerTimeBin;
      R__b << fhTofVsDiamHitsNbPerTimeBin;
      R__b << fhMuchVsDiamHitsNbPerTimeBin;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorMcbmRate(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorMcbmRate : new ::CbmMcbm2018MonitorMcbmRate;
   }
   static void *newArray_CbmMcbm2018MonitorMcbmRate(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorMcbmRate[nElements] : new ::CbmMcbm2018MonitorMcbmRate[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorMcbmRate(void *p) {
      delete ((::CbmMcbm2018MonitorMcbmRate*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorMcbmRate(void *p) {
      delete [] ((::CbmMcbm2018MonitorMcbmRate*)p);
   }
   static void destruct_CbmMcbm2018MonitorMcbmRate(void *p) {
      typedef ::CbmMcbm2018MonitorMcbmRate current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorMcbmRate(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorMcbmRate*)obj)->::CbmMcbm2018MonitorMcbmRate::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorMcbmRate

//______________________________________________________________________________
void CbmMcbm2018MonitorMcbmSync::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorMcbmSync.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fuMuchDpbIdx;
      {
         map<UInt_t,UInt_t> &R__stl =  fmTofDpbIdIndexMap;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            unsigned int R__t2;
            R__b >> R__t2;
            typedef unsigned int Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fuDiamondDpbIdx;
      R__b >> fuTotalNrOfDpb;
      R__b >> fdStsTofOffsetNs;
      R__b >> fdMuchTofOffsetNs;
      R__b >> fbUseBestPair;
      R__b >> fbTsLevelAna;
      fsHistoFileFullname.Streamer(R__b);
      R__b >> fbPrintMessages;
      void *ptr_fPrintMessCtrlSts = (void*)&fPrintMessCtrlSts;
      R__b >> *reinterpret_cast<Int_t*>(ptr_fPrintMessCtrlSts);
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            stsxyter::MessType R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<stsxyter::MessType>(readtemp);
            unsigned int R__t2;
            R__b >> R__t2;
            typedef enum stsxyter::MessType Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fulTofCurrentTsIndex;
      R__b >> fuTofCurrentMs;
      R__b >> fdTofMsIndex;
      R__b >> fuTofGdpbId;
      R__b >> fuTofGdpbNr;
      R__b >> fiTofEquipmentId;
      {
         vector<int> &R__stl =  fviTofMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fulTofCurrentEpochTime;
      {
         vector<vector<gdpbv100::Message> > &R__stl =  fvmTofEpSupprBuffer;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >));
         if (R__tcl1==0) {
            Error("fvmTofEpSupprBuffer streamer","Missing the TClass object for class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<gdpbv100::Message> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<gdpbv100::FullMessage> > &R__stl =  fvmTofGdpbHitsInMs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >));
         if (R__tcl1==0) {
            Error("fvmTofGdpbHitsInMs streamer","Missing the TClass object for class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<gdpbv100::FullMessage> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<gdpbv100::FullMessage> > &R__stl =  fvmTofGdpbHitsInTs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >));
         if (R__tcl1==0) {
            Error("fvmTofGdpbHitsInTs streamer","Missing the TClass object for class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<gdpbv100::FullMessage> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhMcbmHitsNbPerMs;
      R__b >> fhMcbmTimeDiffToDiamond;
      R__b >> fhMcbmTimeDiffToDiamondWide;
      R__b >> fhMcbmTimeDiffToDiamondTs;
      R__b >> fhMcbmTimeDiffToMuch;
      R__b >> fhMcbmTimeDiffToMuchWide;
      R__b >> fhMcbmTimeDiffToMuchTs;
      R__b >> fhMcbmStsTimeDiffToMuchVsAdc;
      R__b >> fhMcbmStsTimeDiffToMuchWideVsAdc;
      R__b >> fhMcbmStsTimeDiffToMuchTsVsAdc;
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondEvoDpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToDiamondEvoDpb streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondWideEvoDpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToDiamondWideEvoDpb streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondTsEvoDpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToDiamondTsEvoDpb streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fdSpillStartA;
      R__b >> fdSpillStartB;
      R__b >> fdSpillStartC;
      {
         vector<TH1*> &R__stl =  fvhHitsTimeEvoSpillA;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhHitsTimeEvoSpillA streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fvhHitsTimeEvoSpillB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhHitsTimeEvoSpillB streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondEvoSpillA;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToDiamondEvoSpillA streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondEvoSpillB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToDiamondEvoSpillB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToMuchEvoSpillA;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToMuchEvoSpillA streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToMuchEvoSpillB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToMuchEvoSpillB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorMcbmSync::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorMcbmSync::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fuMuchDpbIdx;
      {
         map<UInt_t,UInt_t> &R__stl =  fmTofDpbIdIndexMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<UInt_t,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fuDiamondDpbIdx;
      R__b << fuTotalNrOfDpb;
      R__b << fdStsTofOffsetNs;
      R__b << fdMuchTofOffsetNs;
      R__b << fbUseBestPair;
      R__b << fbTsLevelAna;
      fsHistoFileFullname.Streamer(R__b);
      R__b << fbPrintMessages;
      R__b << (Int_t)fPrintMessCtrlSts;
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<stsxyter::MessType,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fulTofCurrentTsIndex;
      R__b << fuTofCurrentMs;
      R__b << fdTofMsIndex;
      R__b << fuTofGdpbId;
      R__b << fuTofGdpbNr;
      R__b << fiTofEquipmentId;
      {
         vector<int> &R__stl =  fviTofMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fulTofCurrentEpochTime;
      {
         vector<vector<gdpbv100::Message> > &R__stl =  fvmTofEpSupprBuffer;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >));
         if (R__tcl1==0) {
            Error("fvmTofEpSupprBuffer streamer","Missing the TClass object for class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >!");
            return;
         }
            vector<vector<gdpbv100::Message> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<gdpbv100::Message>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<vector<gdpbv100::FullMessage> > &R__stl =  fvmTofGdpbHitsInMs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >));
         if (R__tcl1==0) {
            Error("fvmTofGdpbHitsInMs streamer","Missing the TClass object for class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >!");
            return;
         }
            vector<vector<gdpbv100::FullMessage> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<gdpbv100::FullMessage>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<vector<gdpbv100::FullMessage> > &R__stl =  fvmTofGdpbHitsInTs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >));
         if (R__tcl1==0) {
            Error("fvmTofGdpbHitsInTs streamer","Missing the TClass object for class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >!");
            return;
         }
            vector<vector<gdpbv100::FullMessage> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<gdpbv100::FullMessage>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << fhMcbmHitsNbPerMs;
      R__b << fhMcbmTimeDiffToDiamond;
      R__b << fhMcbmTimeDiffToDiamondWide;
      R__b << fhMcbmTimeDiffToDiamondTs;
      R__b << fhMcbmTimeDiffToMuch;
      R__b << fhMcbmTimeDiffToMuchWide;
      R__b << fhMcbmTimeDiffToMuchTs;
      R__b << fhMcbmStsTimeDiffToMuchVsAdc;
      R__b << fhMcbmStsTimeDiffToMuchWideVsAdc;
      R__b << fhMcbmStsTimeDiffToMuchTsVsAdc;
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondEvoDpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondWideEvoDpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondTsEvoDpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fdSpillStartA;
      R__b << fdSpillStartB;
      R__b << fdSpillStartC;
      {
         vector<TH1*> &R__stl =  fvhHitsTimeEvoSpillA;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhHitsTimeEvoSpillB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondEvoSpillA;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondEvoSpillB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToMuchEvoSpillA;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToMuchEvoSpillB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
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
   static void *new_CbmMcbm2018MonitorMcbmSync(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorMcbmSync : new ::CbmMcbm2018MonitorMcbmSync;
   }
   static void *newArray_CbmMcbm2018MonitorMcbmSync(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorMcbmSync[nElements] : new ::CbmMcbm2018MonitorMcbmSync[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorMcbmSync(void *p) {
      delete ((::CbmMcbm2018MonitorMcbmSync*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorMcbmSync(void *p) {
      delete [] ((::CbmMcbm2018MonitorMcbmSync*)p);
   }
   static void destruct_CbmMcbm2018MonitorMcbmSync(void *p) {
      typedef ::CbmMcbm2018MonitorMcbmSync current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorMcbmSync(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorMcbmSync*)obj)->::CbmMcbm2018MonitorMcbmSync::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorMcbmSync

//______________________________________________________________________________
void CbmMcbm2018MonitorMcbmPulser::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorMcbmPulser.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fuMuchDpbIdx;
      {
         map<UInt_t,UInt_t> &R__stl =  fmTofDpbIdIndexMap;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            unsigned int R__t2;
            R__b >> R__t2;
            typedef unsigned int Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fuDiamondDpbIdx;
      R__b >> fuTotalNrOfDpb;
      R__b >> fdStsTofOffsetNs;
      R__b >> fdMuchTofOffsetNs;
      R__b >> fbUseBestPair;
      R__b >> fbTsLevelAna;
      fsHistoFileFullname.Streamer(R__b);
      R__b >> fbPrintMessages;
      void *ptr_fPrintMessCtrlSts = (void*)&fPrintMessCtrlSts;
      R__b >> *reinterpret_cast<Int_t*>(ptr_fPrintMessCtrlSts);
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            stsxyter::MessType R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<stsxyter::MessType>(readtemp);
            unsigned int R__t2;
            R__b >> R__t2;
            typedef enum stsxyter::MessType Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fulTofCurrentTsIndex;
      R__b >> fuTofCurrentMs;
      R__b >> fdTofMsIndex;
      R__b >> fuTofGdpbId;
      R__b >> fuTofGdpbNr;
      R__b >> fiTofEquipmentId;
      {
         vector<int> &R__stl =  fviTofMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fulTofCurrentEpochTime;
      {
         vector<vector<gdpbv100::Message> > &R__stl =  fvmTofEpSupprBuffer;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >));
         if (R__tcl1==0) {
            Error("fvmTofEpSupprBuffer streamer","Missing the TClass object for class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<gdpbv100::Message> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<gdpbv100::FullMessage> > &R__stl =  fvmTofGdpbHitsInMs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >));
         if (R__tcl1==0) {
            Error("fvmTofGdpbHitsInMs streamer","Missing the TClass object for class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<gdpbv100::FullMessage> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<gdpbv100::FullMessage> > &R__stl =  fvmTofGdpbHitsInTs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >));
         if (R__tcl1==0) {
            Error("fvmTofGdpbHitsInTs streamer","Missing the TClass object for class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<gdpbv100::FullMessage> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhMcbmHitsNbPerMs;
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToSelDpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToSelDpb streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToSelDpbWide;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToSelDpbWide streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToSelDpbTs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToSelDpbTs streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondEvoDpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToDiamondEvoDpb streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondWideEvoDpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToDiamondWideEvoDpb streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondTsEvoDpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToDiamondTsEvoDpb streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToTofEvoDpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToTofEvoDpb streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToTofWideEvoDpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToTofWideEvoDpb streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToTofTsEvoDpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhMcbmTimeDiffToTofTsEvoDpb streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorMcbmPulser::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorMcbmPulser::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fuMuchDpbIdx;
      {
         map<UInt_t,UInt_t> &R__stl =  fmTofDpbIdIndexMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<UInt_t,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fuDiamondDpbIdx;
      R__b << fuTotalNrOfDpb;
      R__b << fdStsTofOffsetNs;
      R__b << fdMuchTofOffsetNs;
      R__b << fbUseBestPair;
      R__b << fbTsLevelAna;
      fsHistoFileFullname.Streamer(R__b);
      R__b << fbPrintMessages;
      R__b << (Int_t)fPrintMessCtrlSts;
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<stsxyter::MessType,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fulTofCurrentTsIndex;
      R__b << fuTofCurrentMs;
      R__b << fdTofMsIndex;
      R__b << fuTofGdpbId;
      R__b << fuTofGdpbNr;
      R__b << fiTofEquipmentId;
      {
         vector<int> &R__stl =  fviTofMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fulTofCurrentEpochTime;
      {
         vector<vector<gdpbv100::Message> > &R__stl =  fvmTofEpSupprBuffer;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >));
         if (R__tcl1==0) {
            Error("fvmTofEpSupprBuffer streamer","Missing the TClass object for class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >!");
            return;
         }
            vector<vector<gdpbv100::Message> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<gdpbv100::Message>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<vector<gdpbv100::FullMessage> > &R__stl =  fvmTofGdpbHitsInMs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >));
         if (R__tcl1==0) {
            Error("fvmTofGdpbHitsInMs streamer","Missing the TClass object for class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >!");
            return;
         }
            vector<vector<gdpbv100::FullMessage> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<gdpbv100::FullMessage>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<vector<gdpbv100::FullMessage> > &R__stl =  fvmTofGdpbHitsInTs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >));
         if (R__tcl1==0) {
            Error("fvmTofGdpbHitsInTs streamer","Missing the TClass object for class std::vector<class gdpbv100::FullMessage, class std::allocator<class gdpbv100::FullMessage> >!");
            return;
         }
            vector<vector<gdpbv100::FullMessage> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<gdpbv100::FullMessage>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << fhMcbmHitsNbPerMs;
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToSelDpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToSelDpbWide;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToSelDpbTs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondEvoDpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondWideEvoDpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToDiamondTsEvoDpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToTofEvoDpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToTofWideEvoDpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhMcbmTimeDiffToTofTsEvoDpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
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
   static void *new_CbmMcbm2018MonitorMcbmPulser(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorMcbmPulser : new ::CbmMcbm2018MonitorMcbmPulser;
   }
   static void *newArray_CbmMcbm2018MonitorMcbmPulser(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorMcbmPulser[nElements] : new ::CbmMcbm2018MonitorMcbmPulser[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorMcbmPulser(void *p) {
      delete ((::CbmMcbm2018MonitorMcbmPulser*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorMcbmPulser(void *p) {
      delete [] ((::CbmMcbm2018MonitorMcbmPulser*)p);
   }
   static void destruct_CbmMcbm2018MonitorMcbmPulser(void *p) {
      typedef ::CbmMcbm2018MonitorMcbmPulser current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorMcbmPulser(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorMcbmPulser*)obj)->::CbmMcbm2018MonitorMcbmPulser::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorMcbmPulser

//______________________________________________________________________________
void CbmMcbm2018MonitorSts::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorSts.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      fsHistoFileFullname.Streamer(R__b);
      R__b >> fbPrintMessages;
      void *ptr_fPrintMessCtrl = (void*)&fPrintMessCtrl;
      R__b >> *reinterpret_cast<Int_t*>(ptr_fPrintMessCtrl);
      R__b >> fbEnableCheckBugSmx20;
      R__b >> fbBinningFw;
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            stsxyter::MessType R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<stsxyter::MessType>(readtemp);
            unsigned int R__t2;
            R__b >> R__t2;
            typedef enum stsxyter::MessType Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fdStartTime;
      R__b >> fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b >> fuMaxNbMicroslices;
      R__b >> fiTimeIntervalRateUpdate;
      {
         vector<Double_t> &R__stl =  fvdFebTimeSecLastRateUpdate;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            double R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviFebCountsSinceLastRateUpdate;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<Double_t> > &R__stl =  fvdFebChanCountsSinceLastRateUpdate;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fvdFebChanCountsSinceLastRateUpdate streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<double> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fbLongHistoEnable;
      R__b >> fuLongHistoNbSeconds;
      R__b >> fuLongHistoBinSizeSec;
      R__b >> fuLongHistoBinNb;
      R__b >> fdCoincCenter;
      R__b >> fdCoincBorder;
      R__b >> fdCoincMin;
      R__b >> fdCoincMax;
      R__b >> fhStsMessType;
      R__b >> fhStsSysMessType;
      R__b >> fhStsMessTypePerDpb;
      R__b >> fhStsSysMessTypePerDpb;
      R__b >> fhStsStatusMessType;
      R__b >> fhStsMsStatusFieldType;
      R__b >> fhStsMessTypePerElink;
      R__b >> fhStsHitsElinkPerDpb;
      R__b >> fhStsAllFebsHitRateEvo;
      R__b >> fhStsAllAsicsHitRateEvo;
      R__b >> fhStsFebAsicHitCounts;
      R__b >> fdFebChanCoincidenceLimit;
      {
         vector<TH1*> &R__stl =  fhStsFebChanCntRaw;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanCntRaw streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fhStsFebChanCntRawGood;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanCntRawGood streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fhStsFebChanAdcRaw;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanAdcRaw streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebChanAdcRawProf;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhStsFebChanAdcRawProf streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanRawTs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanRawTs streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanMissEvt;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanMissEvt streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanMissEvtEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanMissEvtEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebAsicMissEvtEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebAsicMissEvtEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebMissEvtEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhStsFebMissEvtEvo streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fhStsFebChanHitRateEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanHitRateEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebChanHitRateProf;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhStsFebChanHitRateProf streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebAsicHitRateEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebAsicHitRateEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebHitRateEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhStsFebHitRateEvo streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fhStsFebChanHitRateEvoLong;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanHitRateEvoLong streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebAsicHitRateEvoLong;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebAsicHitRateEvoLong streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebHitRateEvoLong;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhStsFebHitRateEvoLong streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<vector<Double_t> > &R__stl =  fdStsFebChanLastTimeForDist;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fdStsFebChanLastTimeForDist streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<double> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanDistT;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanDistT streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanCloseHitsCounts;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanCloseHitsCounts streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebChanCloseHitsRatio;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhStsFebChanCloseHitsRatio streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fbSmx2ErrorUseNoiseLevels;
      R__b >> fdSmxErrCoincWinM07;
      R__b >> fdSmxErrCoincWinM08;
      R__b >> fdSmxErrCoincWinM09;
      R__b >> fdSmxErrCoincWinM10;
      R__b >> fdSmxErrCoincWinM11;
      {
         vector<vector<UInt_t> > &R__stl =  fvuSmxErrIdxLastHit;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<unsigned int, class std::allocator<unsigned int> >));
         if (R__tcl1==0) {
            Error("fvuSmxErrIdxLastHit streamer","Missing the TClass object for class std::vector<unsigned int, class std::allocator<unsigned int> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<unsigned int> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebSmxErrRatioEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhStsFebSmxErrRatioEvo streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fhStsFebSmxErrRatioEvoAsic;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile2D *));
         if (R__tcl1==0) {
            Error("fhStsFebSmxErrRatioEvoAsic streamer","Missing the TClass object for class TProfile2D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile2D* R__t;
            R__t = (TProfile2D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebSmxErrRatioCopyEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhStsFebSmxErrRatioCopyEvo streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fhStsFebSmxErrRatioCopyEvoAsic;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile2D *));
         if (R__tcl1==0) {
            Error("fhStsFebSmxErrRatioCopyEvoAsic streamer","Missing the TClass object for class TProfile2D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile2D* R__t;
            R__t = (TProfile2D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebSmxErrRatioCopySameAdcEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhStsFebSmxErrRatioCopySameAdcEvo streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fhStsFebSmxErrRatioCopySameAdcEvoAsic;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile2D *));
         if (R__tcl1==0) {
            Error("fhStsFebSmxErrRatioCopySameAdcEvoAsic streamer","Missing the TClass object for class TProfile2D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile2D* R__t;
            R__t = (TProfile2D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhMsErrorsEvo;
      {
         vector<TCanvas*> &R__stl =  fvcStsSumm;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TCanvas *));
         if (R__tcl1==0) {
            Error("fvcStsSumm streamer","Missing the TClass object for class TCanvas *!");
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
      {
         vector<TCanvas*> &R__stl =  fvcStsSmxErr;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TCanvas *));
         if (R__tcl1==0) {
            Error("fvcStsSmxErr streamer","Missing the TClass object for class TCanvas *!");
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
      R__b >> fcMsSizeAll;
      int R__i;
      for (R__i = 0; R__i < 32; R__i++)
         R__b >> fhMsSz[R__i];
      for (R__i = 0; R__i < 32; R__i++)
         R__b >> fhMsSzTime[R__i];
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorSts::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorSts::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      fsHistoFileFullname.Streamer(R__b);
      R__b << fbPrintMessages;
      R__b << (Int_t)fPrintMessCtrl;
      R__b << fbEnableCheckBugSmx20;
      R__b << fbBinningFw;
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<stsxyter::MessType,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fdStartTime;
      R__b << fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b << fuMaxNbMicroslices;
      R__b << fiTimeIntervalRateUpdate;
      {
         vector<Double_t> &R__stl =  fvdFebTimeSecLastRateUpdate;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Double_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviFebCountsSinceLastRateUpdate;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<Double_t> > &R__stl =  fvdFebChanCountsSinceLastRateUpdate;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fvdFebChanCountsSinceLastRateUpdate streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
            vector<vector<Double_t> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<double>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << fbLongHistoEnable;
      R__b << fuLongHistoNbSeconds;
      R__b << fuLongHistoBinSizeSec;
      R__b << fuLongHistoBinNb;
      R__b << fdCoincCenter;
      R__b << fdCoincBorder;
      R__b << fdCoincMin;
      R__b << fdCoincMax;
      R__b << fhStsMessType;
      R__b << fhStsSysMessType;
      R__b << fhStsMessTypePerDpb;
      R__b << fhStsSysMessTypePerDpb;
      R__b << fhStsStatusMessType;
      R__b << fhStsMsStatusFieldType;
      R__b << fhStsMessTypePerElink;
      R__b << fhStsHitsElinkPerDpb;
      R__b << fhStsAllFebsHitRateEvo;
      R__b << fhStsAllAsicsHitRateEvo;
      R__b << fhStsFebAsicHitCounts;
      R__b << fdFebChanCoincidenceLimit;
      {
         vector<TH1*> &R__stl =  fhStsFebChanCntRaw;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebChanCntRawGood;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanAdcRaw;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebChanAdcRawProf;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanRawTs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanMissEvt;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanMissEvtEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebAsicMissEvtEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebMissEvtEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanHitRateEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebChanHitRateProf;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebAsicHitRateEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebHitRateEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanHitRateEvoLong;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebAsicHitRateEvoLong;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebHitRateEvoLong;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<Double_t> > &R__stl =  fdStsFebChanLastTimeForDist;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fdStsFebChanLastTimeForDist streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
            vector<vector<Double_t> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<double>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanDistT;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanCloseHitsCounts;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebChanCloseHitsRatio;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fbSmx2ErrorUseNoiseLevels;
      R__b << fdSmxErrCoincWinM07;
      R__b << fdSmxErrCoincWinM08;
      R__b << fdSmxErrCoincWinM09;
      R__b << fdSmxErrCoincWinM10;
      R__b << fdSmxErrCoincWinM11;
      {
         vector<vector<UInt_t> > &R__stl =  fvuSmxErrIdxLastHit;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<unsigned int, class std::allocator<unsigned int> >));
         if (R__tcl1==0) {
            Error("fvuSmxErrIdxLastHit streamer","Missing the TClass object for class std::vector<unsigned int, class std::allocator<unsigned int> >!");
            return;
         }
            vector<vector<UInt_t> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<unsigned int>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebSmxErrRatioEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fhStsFebSmxErrRatioEvoAsic;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile2D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebSmxErrRatioCopyEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fhStsFebSmxErrRatioCopyEvoAsic;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile2D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebSmxErrRatioCopySameAdcEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fhStsFebSmxErrRatioCopySameAdcEvoAsic;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile2D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fhMsErrorsEvo;
      {
         vector<TCanvas*> &R__stl =  fvcStsSumm;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TCanvas*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TCanvas*> &R__stl =  fvcStsSmxErr;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TCanvas*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fcMsSizeAll;
      int R__i;
      for (R__i = 0; R__i < 32; R__i++)
         R__b << fhMsSz[R__i];
      for (R__i = 0; R__i < 32; R__i++)
         R__b << (TObject*)fhMsSzTime[R__i];
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorSts(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorSts : new ::CbmMcbm2018MonitorSts;
   }
   static void *newArray_CbmMcbm2018MonitorSts(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorSts[nElements] : new ::CbmMcbm2018MonitorSts[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorSts(void *p) {
      delete ((::CbmMcbm2018MonitorSts*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorSts(void *p) {
      delete [] ((::CbmMcbm2018MonitorSts*)p);
   }
   static void destruct_CbmMcbm2018MonitorSts(void *p) {
      typedef ::CbmMcbm2018MonitorSts current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorSts(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorSts*)obj)->::CbmMcbm2018MonitorSts::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorSts

//______________________________________________________________________________
void CbmMcbm2018MonitorMuch::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorMuch.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fbMuchMode;
      fsHistoFileFullname.Streamer(R__b);
      R__b >> fbPrintMessages;
      void *ptr_fPrintMessCtrl = (void*)&fPrintMessCtrl;
      R__b >> *reinterpret_cast<Int_t*>(ptr_fPrintMessCtrl);
      R__b >> fbEnableCoincidenceMaps;
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            stsxyter::MessType R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<stsxyter::MessType>(readtemp);
            unsigned int R__t2;
            R__b >> R__t2;
            typedef enum stsxyter::MessType Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fdStartTime;
      R__b >> prevtime;
      R__b >> fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b >> fuMaxNbMicroslices;
      R__b >> fiTimeIntervalRateUpdate;
      {
         vector<Int_t> &R__stl =  fviFebTimeSecLastRateUpdate;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviFebCountsSinceLastRateUpdate;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<Double_t> > &R__stl =  fvdFebChanCountsSinceLastRateUpdate;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fvdFebChanCountsSinceLastRateUpdate streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<double> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fbLongHistoEnable;
      R__b >> fuLongHistoNbSeconds;
      R__b >> fuLongHistoBinSizeSec;
      R__b >> fuLongHistoBinNb;
      R__b >> Counter;
      R__b >> Counter1;
      R__b >> fdCoincCenter;
      R__b >> fdCoincBorder;
      R__b >> fdCoincMin;
      R__b >> fdCoincMax;
      R__b >> fhStsMessType;
      R__b >> fhStsSysMessType;
      R__b >> fhStsFebChanAdcRaw_combined;
      R__b >> fhStsMessTypePerDpb;
      R__b >> fhStsSysMessTypePerDpb;
      R__b >> fhPulserStatusMessType;
      R__b >> fhPulserMsStatusFieldType;
      R__b >> fhStsHitsElinkPerDpb;
      R__b >> fhRate;
      R__b >> fhRateAdcCut;
      {
         vector<TH2*> &R__stl =  fHistPadDistr;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fHistPadDistr streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fRealHistPadDistr;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fRealHistPadDistr streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fdFebChanCoincidenceLimit;
      {
         vector<TH1*> &R__stl =  fhStsFebChanCntRaw;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanCntRaw streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fhStsFebChanCntRawGood;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanCntRawGood streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fhStsFebChanAdcRaw;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanAdcRaw streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebChanAdcRawProf;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhStsFebChanAdcRawProf streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanAdcCal;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanAdcCal streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebChanAdcCalProf;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhStsFebChanAdcCalProf streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanRawTs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanRawTs streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanMissEvt;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanMissEvt streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanMissEvtEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanMissEvtEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebAsicMissEvtEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebAsicMissEvtEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebMissEvtEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhStsFebMissEvtEvo streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fhStsFebChanHitRateEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanHitRateEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebChanHitRateProf;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhStsFebChanHitRateProf streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebAsicHitRateEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebAsicHitRateEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebHitRateEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhStsFebHitRateEvo streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fhStsFebHitRateEvo_mskch;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhStsFebHitRateEvo_mskch streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fhStsFebHitRateEvo_mskch_adccut;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhStsFebHitRateEvo_mskch_adccut streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fhStsFebChanHitRateEvoLong;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanHitRateEvoLong streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebAsicHitRateEvoLong;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebAsicHitRateEvoLong streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebHitRateEvoLong;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhStsFebHitRateEvoLong streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<vector<Double_t> > &R__stl =  fdStsFebChanLastTimeForDist;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fdStsFebChanLastTimeForDist streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<double> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanDistT;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhStsFebChanDistT streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<TH1*> > &R__stl =  fhStsFebChanDtCoinc;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class TH1 *, class std::allocator<class TH1 *> >));
         if (R__tcl1==0) {
            Error("fhStsFebChanDtCoinc streamer","Missing the TClass object for class std::vector<class TH1 *, class std::allocator<class TH1 *> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<TH1*> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<TH2*> > &R__stl =  fhStsFebChanCoinc;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class TH2 *, class std::allocator<class TH2 *> >));
         if (R__tcl1==0) {
            Error("fhStsFebChanCoinc streamer","Missing the TClass object for class std::vector<class TH2 *, class std::allocator<class TH2 *> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<TH2*> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fbSmx2ErrorUseNoiseLevels;
      R__b >> fdSmxErrCoincWinM07;
      R__b >> fdSmxErrCoincWinM08;
      R__b >> fdSmxErrCoincWinM09;
      R__b >> fdSmxErrCoincWinM10;
      R__b >> fdSmxErrCoincWinM11;
      {
         vector<vector<UInt_t> > &R__stl =  fvuSmxErrIdxLastHit;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<unsigned int, class std::allocator<unsigned int> >));
         if (R__tcl1==0) {
            Error("fvuSmxErrIdxLastHit streamer","Missing the TClass object for class std::vector<unsigned int, class std::allocator<unsigned int> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<unsigned int> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebSmxErrRatioEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhStsFebSmxErrRatioEvo streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fhStsFebSmxErrRatioEvoAsic;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile2D *));
         if (R__tcl1==0) {
            Error("fhStsFebSmxErrRatioEvoAsic streamer","Missing the TClass object for class TProfile2D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile2D* R__t;
            R__t = (TProfile2D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebSmxErrRatioCopyEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhStsFebSmxErrRatioCopyEvo streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fhStsFebSmxErrRatioCopyEvoAsic;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile2D *));
         if (R__tcl1==0) {
            Error("fhStsFebSmxErrRatioCopyEvoAsic streamer","Missing the TClass object for class TProfile2D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile2D* R__t;
            R__t = (TProfile2D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebSmxErrRatioCopySameAdcEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhStsFebSmxErrRatioCopySameAdcEvo streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fhStsFebSmxErrRatioCopySameAdcEvoAsic;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile2D *));
         if (R__tcl1==0) {
            Error("fhStsFebSmxErrRatioCopySameAdcEvoAsic streamer","Missing the TClass object for class TProfile2D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile2D* R__t;
            R__t = (TProfile2D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fcMsSizeAll;
      int R__i;
      for (R__i = 0; R__i < 32; R__i++)
         R__b >> fhMsSz[R__i];
      for (R__i = 0; R__i < 32; R__i++)
         R__b >> fhMsSzTime[R__i];
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorMuch::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorMuch::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fbMuchMode;
      fsHistoFileFullname.Streamer(R__b);
      R__b << fbPrintMessages;
      R__b << (Int_t)fPrintMessCtrl;
      R__b << fbEnableCoincidenceMaps;
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<stsxyter::MessType,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fdStartTime;
      R__b << prevtime;
      R__b << fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b << fuMaxNbMicroslices;
      R__b << fiTimeIntervalRateUpdate;
      {
         vector<Int_t> &R__stl =  fviFebTimeSecLastRateUpdate;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviFebCountsSinceLastRateUpdate;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<Double_t> > &R__stl =  fvdFebChanCountsSinceLastRateUpdate;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fvdFebChanCountsSinceLastRateUpdate streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
            vector<vector<Double_t> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<double>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << fbLongHistoEnable;
      R__b << fuLongHistoNbSeconds;
      R__b << fuLongHistoBinSizeSec;
      R__b << fuLongHistoBinNb;
      R__b << Counter;
      R__b << Counter1;
      R__b << fdCoincCenter;
      R__b << fdCoincBorder;
      R__b << fdCoincMin;
      R__b << fdCoincMax;
      R__b << fhStsMessType;
      R__b << fhStsSysMessType;
      R__b << fhStsFebChanAdcRaw_combined;
      R__b << fhStsMessTypePerDpb;
      R__b << fhStsSysMessTypePerDpb;
      R__b << fhPulserStatusMessType;
      R__b << fhPulserMsStatusFieldType;
      R__b << fhStsHitsElinkPerDpb;
      R__b << fhRate;
      R__b << fhRateAdcCut;
      {
         vector<TH2*> &R__stl =  fHistPadDistr;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fRealHistPadDistr;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fdFebChanCoincidenceLimit;
      {
         vector<TH1*> &R__stl =  fhStsFebChanCntRaw;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebChanCntRawGood;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanAdcRaw;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebChanAdcRawProf;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanAdcCal;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebChanAdcCalProf;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanRawTs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanMissEvt;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanMissEvtEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebAsicMissEvtEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebMissEvtEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanHitRateEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebChanHitRateProf;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebAsicHitRateEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebHitRateEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebHitRateEvo_mskch;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebHitRateEvo_mskch_adccut;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanHitRateEvoLong;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebAsicHitRateEvoLong;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhStsFebHitRateEvoLong;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<Double_t> > &R__stl =  fdStsFebChanLastTimeForDist;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fdStsFebChanLastTimeForDist streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
            vector<vector<Double_t> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<double>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhStsFebChanDistT;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<TH1*> > &R__stl =  fhStsFebChanDtCoinc;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class TH1 *, class std::allocator<class TH1 *> >));
         if (R__tcl1==0) {
            Error("fhStsFebChanDtCoinc streamer","Missing the TClass object for class std::vector<class TH1 *, class std::allocator<class TH1 *> >!");
            return;
         }
            vector<vector<TH1*> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<TH1*>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<vector<TH2*> > &R__stl =  fhStsFebChanCoinc;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class TH2 *, class std::allocator<class TH2 *> >));
         if (R__tcl1==0) {
            Error("fhStsFebChanCoinc streamer","Missing the TClass object for class std::vector<class TH2 *, class std::allocator<class TH2 *> >!");
            return;
         }
            vector<vector<TH2*> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<TH2*>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << fbSmx2ErrorUseNoiseLevels;
      R__b << fdSmxErrCoincWinM07;
      R__b << fdSmxErrCoincWinM08;
      R__b << fdSmxErrCoincWinM09;
      R__b << fdSmxErrCoincWinM10;
      R__b << fdSmxErrCoincWinM11;
      {
         vector<vector<UInt_t> > &R__stl =  fvuSmxErrIdxLastHit;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<unsigned int, class std::allocator<unsigned int> >));
         if (R__tcl1==0) {
            Error("fvuSmxErrIdxLastHit streamer","Missing the TClass object for class std::vector<unsigned int, class std::allocator<unsigned int> >!");
            return;
         }
            vector<vector<UInt_t> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<unsigned int>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebSmxErrRatioEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fhStsFebSmxErrRatioEvoAsic;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile2D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebSmxErrRatioCopyEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fhStsFebSmxErrRatioCopyEvoAsic;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile2D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhStsFebSmxErrRatioCopySameAdcEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fhStsFebSmxErrRatioCopySameAdcEvoAsic;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile2D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fcMsSizeAll;
      int R__i;
      for (R__i = 0; R__i < 32; R__i++)
         R__b << fhMsSz[R__i];
      for (R__i = 0; R__i < 32; R__i++)
         R__b << (TObject*)fhMsSzTime[R__i];
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorMuch(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorMuch : new ::CbmMcbm2018MonitorMuch;
   }
   static void *newArray_CbmMcbm2018MonitorMuch(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorMuch[nElements] : new ::CbmMcbm2018MonitorMuch[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorMuch(void *p) {
      delete ((::CbmMcbm2018MonitorMuch*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorMuch(void *p) {
      delete [] ((::CbmMcbm2018MonitorMuch*)p);
   }
   static void destruct_CbmMcbm2018MonitorMuch(void *p) {
      typedef ::CbmMcbm2018MonitorMuch current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorMuch(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorMuch*)obj)->::CbmMcbm2018MonitorMuch::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorMuch

//______________________________________________________________________________
void CbmMcbm2018MonitorMuchLite::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorMuchLite.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fbMuchMode;
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fbBinningFw;
      fsHistoFileFullname.Streamer(R__b);
      R__b >> fbPrintMessages;
      void *ptr_fPrintMessCtrl = (void*)&fPrintMessCtrl;
      R__b >> *reinterpret_cast<Int_t*>(ptr_fPrintMessCtrl);
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            stsxyter::MessType R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<stsxyter::MessType>(readtemp);
            unsigned int R__t2;
            R__b >> R__t2;
            typedef enum stsxyter::MessType Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> prevtime_new;
      R__b >> prevTime;
      R__b >> prevAsic;
      R__b >> prevChan;
      R__b >> fdStartTime;
      R__b >> fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b >> fuMaxNbMicroslices;
      R__b >> fiTimeIntervalRateUpdate;
      {
         vector<Int_t> &R__stl =  fviFebTimeSecLastRateUpdate;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviFebCountsSinceLastRateUpdate;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<Double_t> > &R__stl =  fvdFebChanCountsSinceLastRateUpdate;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fvdFebChanCountsSinceLastRateUpdate streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<double> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> Counter;
      R__b >> Counter1;
      R__b >> fhMuchMessType;
      R__b >> fhMuchSysMessType;
      R__b >> fhMuchFebChanAdcRaw_combined;
      R__b >> fhMuchMessTypePerDpb;
      R__b >> fhMuchSysMessTypePerDpb;
      R__b >> fhStatusMessType;
      R__b >> fhMsStatusFieldType;
      R__b >> fhMuchHitsElinkPerDpb;
      R__b >> fhRate;
      R__b >> fhRateAdcCut;
      R__b >> fhFEBcount;
      {
         vector<TH2*> &R__stl =  fHistPadDistr;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fHistPadDistr streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fRealHistPadDistr;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fRealHistPadDistr streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebChanCntRaw;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhMuchFebChanCntRaw streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fhMuchFebSpill;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhMuchFebSpill streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fhMuchFebADC;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhMuchFebADC streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanAdcRaw;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhMuchFebChanAdcRaw streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhMuchFebChanAdcRawProf;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhMuchFebChanAdcRawProf streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanRawTs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhMuchFebChanRawTs streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchChannelTime;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhMuchChannelTime streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanHitRateEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhMuchFebChanHitRateEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhMuchFebChanHitRateProf;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhMuchFebChanHitRateProf streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhMuchFebHitRateEvo streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo_mskch;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhMuchFebHitRateEvo_mskch streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo_mskch_adccut;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhMuchFebHitRateEvo_mskch_adccut streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo_WithoutDupli;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhMuchFebHitRateEvo_WithoutDupli streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<vector<Double_t> > &R__stl =  fdMuchFebChanLastTimeForDist;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fdMuchFebChanLastTimeForDist streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<double> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanDistT;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhMuchFebChanDistT streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhMuchFebDuplicateHitProf;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhMuchFebDuplicateHitProf streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhDpbMsErrors;
      R__b >> fcMsSizeAll;
      int R__i;
      for (R__i = 0; R__i < 32; R__i++)
         R__b >> fhMsSz[R__i];
      for (R__i = 0; R__i < 32; R__i++)
         R__b >> fhMsSzTime[R__i];
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorMuchLite::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorMuchLite::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fbMuchMode;
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fbBinningFw;
      fsHistoFileFullname.Streamer(R__b);
      R__b << fbPrintMessages;
      R__b << (Int_t)fPrintMessCtrl;
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<stsxyter::MessType,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << prevtime_new;
      R__b << prevTime;
      R__b << prevAsic;
      R__b << prevChan;
      R__b << fdStartTime;
      R__b << fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b << fuMaxNbMicroslices;
      R__b << fiTimeIntervalRateUpdate;
      {
         vector<Int_t> &R__stl =  fviFebTimeSecLastRateUpdate;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviFebCountsSinceLastRateUpdate;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<Double_t> > &R__stl =  fvdFebChanCountsSinceLastRateUpdate;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fvdFebChanCountsSinceLastRateUpdate streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
            vector<vector<Double_t> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<double>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << Counter;
      R__b << Counter1;
      R__b << fhMuchMessType;
      R__b << fhMuchSysMessType;
      R__b << fhMuchFebChanAdcRaw_combined;
      R__b << fhMuchMessTypePerDpb;
      R__b << fhMuchSysMessTypePerDpb;
      R__b << fhStatusMessType;
      R__b << fhMsStatusFieldType;
      R__b << fhMuchHitsElinkPerDpb;
      R__b << fhRate;
      R__b << fhRateAdcCut;
      R__b << fhFEBcount;
      {
         vector<TH2*> &R__stl =  fHistPadDistr;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fRealHistPadDistr;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebChanCntRaw;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebSpill;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebADC;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanAdcRaw;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhMuchFebChanAdcRawProf;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanRawTs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchChannelTime;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanHitRateEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhMuchFebChanHitRateProf;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo_mskch;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo_mskch_adccut;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo_WithoutDupli;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<Double_t> > &R__stl =  fdMuchFebChanLastTimeForDist;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fdMuchFebChanLastTimeForDist streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
            vector<vector<Double_t> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<double>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanDistT;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhMuchFebDuplicateHitProf;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fhDpbMsErrors;
      R__b << fcMsSizeAll;
      int R__i;
      for (R__i = 0; R__i < 32; R__i++)
         R__b << fhMsSz[R__i];
      for (R__i = 0; R__i < 32; R__i++)
         R__b << (TObject*)fhMsSzTime[R__i];
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorMuchLite(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorMuchLite : new ::CbmMcbm2018MonitorMuchLite;
   }
   static void *newArray_CbmMcbm2018MonitorMuchLite(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorMuchLite[nElements] : new ::CbmMcbm2018MonitorMuchLite[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorMuchLite(void *p) {
      delete ((::CbmMcbm2018MonitorMuchLite*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorMuchLite(void *p) {
      delete [] ((::CbmMcbm2018MonitorMuchLite*)p);
   }
   static void destruct_CbmMcbm2018MonitorMuchLite(void *p) {
      typedef ::CbmMcbm2018MonitorMuchLite current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorMuchLite(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorMuchLite*)obj)->::CbmMcbm2018MonitorMuchLite::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorMuchLite

//______________________________________________________________________________
void CbmMcbm2018MonitorAlgoMuchLite::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorAlgoMuchLite.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b >> fbMuchMode;
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      fsHistoFilename.Streamer(R__b);
      R__b >> fbPrintMessages;
      void *ptr_fPrintMessCtrl = (void*)&fPrintMessCtrl;
      R__b >> *reinterpret_cast<Int_t*>(ptr_fPrintMessCtrl);
      R__b >> fulCurrentTsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            stsxyter::MessType R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<stsxyter::MessType>(readtemp);
            unsigned int R__t2;
            R__b >> R__t2;
            typedef enum stsxyter::MessType Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fbBinningFw;
      R__b >> fuHistoryHistoSize;
      R__b >> fuOffSpillCountLimit;
      fsHistoFileFullname.Streamer(R__b);
      R__b >> fulCurrentMsIdx;
      R__b >> fiTimeIntervalRateUpdate;
      {
         vector<Int_t> &R__stl =  fviFebTimeSecLastRateUpdate;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviFebCountsSinceLastRateUpdate;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<Double_t> > &R__stl =  fvdFebChanCountsSinceLastRateUpdate;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fvdFebChanCountsSinceLastRateUpdate streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<double> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> Counter;
      R__b >> Counter1;
      R__b >> fhMuchMessType;
      R__b >> fhMuchSysMessType;
      R__b >> fhMuchFebChanAdcRaw_combined;
      R__b >> fhMuchMessTypePerDpb;
      R__b >> fhMuchSysMessTypePerDpb;
      R__b >> fhStatusMessType;
      R__b >> fhMsStatusFieldType;
      R__b >> fhMuchHitsElinkPerDpb;
      R__b >> fhMuchMessTypePerElink;
      R__b >> fhRate;
      R__b >> fhRateAdcCut;
      R__b >> fhFEBcount;
      {
         vector<TH2*> &R__stl =  fHistPadDistr;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fHistPadDistr streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fRealHistPadDistr;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fRealHistPadDistr streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebChanCntRaw;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhMuchFebChanCntRaw streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fhMuchFebSpill;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhMuchFebSpill streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fhMuchFebADC;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhMuchFebADC streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanAdcRaw;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhMuchFebChanAdcRaw streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhMuchFebChanAdcRawProf;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhMuchFebChanAdcRawProf streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanRawTs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhMuchFebChanRawTs streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchChannelTime;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhMuchChannelTime streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanHitRateEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhMuchFebChanHitRateEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhMuchFebChanHitRateProf;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhMuchFebChanHitRateProf streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhMuchFebHitRateEvo streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo_mskch;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhMuchFebHitRateEvo_mskch streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo_mskch_adccut;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhMuchFebHitRateEvo_mskch_adccut streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo_WithoutDupli;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fhMuchFebHitRateEvo_WithoutDupli streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<vector<Double_t> > &R__stl =  fdMuchFebChanLastTimeForDist;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fdMuchFebChanLastTimeForDist streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<double> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanDistT;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhMuchFebChanDistT streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fhMuchFebDuplicateHitProf;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fhMuchFebDuplicateHitProf streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhDpbMsErrors;
      R__b >> fcMsSizeAll;
      R__b >> fuMaxNbMicroslices;
      int R__i;
      for (R__i = 0; R__i < 32; R__i++)
         R__b >> fhMsSz[R__i];
      for (R__i = 0; R__i < 32; R__i++)
         R__b >> fhMsSzTime[R__i];
      R__b >> prevtime_new;
      R__b >> prevTime;
      R__b >> prevAsic;
      R__b >> prevChan;
      R__b >> fdStartTime;
      R__b >> fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b >> fhElinkIdxHit;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorAlgoMuchLite::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorAlgoMuchLite::IsA(), kTRUE);
      R__b << fbMuchMode;
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      fsHistoFilename.Streamer(R__b);
      R__b << fbPrintMessages;
      R__b << (Int_t)fPrintMessCtrl;
      R__b << fulCurrentTsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<stsxyter::MessType,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fbBinningFw;
      R__b << fuHistoryHistoSize;
      R__b << fuOffSpillCountLimit;
      fsHistoFileFullname.Streamer(R__b);
      R__b << fulCurrentMsIdx;
      R__b << fiTimeIntervalRateUpdate;
      {
         vector<Int_t> &R__stl =  fviFebTimeSecLastRateUpdate;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviFebCountsSinceLastRateUpdate;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<Double_t> > &R__stl =  fvdFebChanCountsSinceLastRateUpdate;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fvdFebChanCountsSinceLastRateUpdate streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
            vector<vector<Double_t> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<double>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << Counter;
      R__b << Counter1;
      R__b << fhMuchMessType;
      R__b << fhMuchSysMessType;
      R__b << fhMuchFebChanAdcRaw_combined;
      R__b << fhMuchMessTypePerDpb;
      R__b << fhMuchSysMessTypePerDpb;
      R__b << fhStatusMessType;
      R__b << fhMsStatusFieldType;
      R__b << fhMuchHitsElinkPerDpb;
      R__b << fhMuchMessTypePerElink;
      R__b << fhRate;
      R__b << fhRateAdcCut;
      R__b << fhFEBcount;
      {
         vector<TH2*> &R__stl =  fHistPadDistr;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fRealHistPadDistr;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebChanCntRaw;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebSpill;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebADC;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanAdcRaw;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhMuchFebChanAdcRawProf;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanRawTs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchChannelTime;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanHitRateEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhMuchFebChanHitRateProf;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo_mskch;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo_mskch_adccut;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fhMuchFebHitRateEvo_WithoutDupli;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<Double_t> > &R__stl =  fdMuchFebChanLastTimeForDist;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> >));
         if (R__tcl1==0) {
            Error("fdMuchFebChanLastTimeForDist streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> >!");
            return;
         }
            vector<vector<Double_t> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<double>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhMuchFebChanDistT;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fhMuchFebDuplicateHitProf;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fhDpbMsErrors;
      R__b << fcMsSizeAll;
      R__b << fuMaxNbMicroslices;
      int R__i;
      for (R__i = 0; R__i < 32; R__i++)
         R__b << fhMsSz[R__i];
      for (R__i = 0; R__i < 32; R__i++)
         R__b << (TObject*)fhMsSzTime[R__i];
      R__b << prevtime_new;
      R__b << prevTime;
      R__b << prevAsic;
      R__b << prevChan;
      R__b << fdStartTime;
      R__b << fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b << fhElinkIdxHit;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorAlgoMuchLite(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorAlgoMuchLite : new ::CbmMcbm2018MonitorAlgoMuchLite;
   }
   static void *newArray_CbmMcbm2018MonitorAlgoMuchLite(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorAlgoMuchLite[nElements] : new ::CbmMcbm2018MonitorAlgoMuchLite[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorAlgoMuchLite(void *p) {
      delete ((::CbmMcbm2018MonitorAlgoMuchLite*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorAlgoMuchLite(void *p) {
      delete [] ((::CbmMcbm2018MonitorAlgoMuchLite*)p);
   }
   static void destruct_CbmMcbm2018MonitorAlgoMuchLite(void *p) {
      typedef ::CbmMcbm2018MonitorAlgoMuchLite current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorAlgoMuchLite(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorAlgoMuchLite*)obj)->::CbmMcbm2018MonitorAlgoMuchLite::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorAlgoMuchLite

//______________________________________________________________________________
void CbmMcbm2018MonitorTaskMuchLite::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorTaskMuchLite.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fbMonitorMode;
      R__b >> fbDebugMonitorMode;
      R__b >> fbMuchMode;
      R__b >> fbBinningFw;
      R__b >> fuHistoryHistoSize;
      fsHistoFilename.Streamer(R__b);
      R__b >> fuOffSpillCountLimit;
      R__b >> fulTsCounter;
      R__b >> fbPrintMessages;
      void *ptr_fPrintMessCtrl = (void*)&fPrintMessCtrl;
      R__b >> *reinterpret_cast<Int_t*>(ptr_fPrintMessCtrl);
      R__b >> fMonitorAlgo;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorTaskMuchLite::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorTaskMuchLite::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fbMonitorMode;
      R__b << fbDebugMonitorMode;
      R__b << fbMuchMode;
      R__b << fbBinningFw;
      R__b << fuHistoryHistoSize;
      fsHistoFilename.Streamer(R__b);
      R__b << fuOffSpillCountLimit;
      R__b << fulTsCounter;
      R__b << fbPrintMessages;
      R__b << (Int_t)fPrintMessCtrl;
      R__b << fMonitorAlgo;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorTaskMuchLite(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorTaskMuchLite : new ::CbmMcbm2018MonitorTaskMuchLite;
   }
   static void *newArray_CbmMcbm2018MonitorTaskMuchLite(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorTaskMuchLite[nElements] : new ::CbmMcbm2018MonitorTaskMuchLite[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorTaskMuchLite(void *p) {
      delete ((::CbmMcbm2018MonitorTaskMuchLite*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorTaskMuchLite(void *p) {
      delete [] ((::CbmMcbm2018MonitorTaskMuchLite*)p);
   }
   static void destruct_CbmMcbm2018MonitorTaskMuchLite(void *p) {
      typedef ::CbmMcbm2018MonitorTaskMuchLite current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorTaskMuchLite(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorTaskMuchLite*)obj)->::CbmMcbm2018MonitorTaskMuchLite::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorTaskMuchLite

//______________________________________________________________________________
void CbmMcbm2018MonitorTof::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorTof.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fuMsAcceptsPercent;
      R__b >> fuTotalMsNb;
      R__b >> fuOverlapMsNb;
      R__b >> fuCoreMs;
      R__b >> fdMsSizeInNs;
      R__b >> fdTsCoreSizeInNs;
      R__b >> fuMinNbGdpb;
      R__b >> fuCurrNbGdpb;
      R__b >> fuNrOfGdpbs;
      R__b >> fuNrOfFeePerGdpb;
      R__b >> fuNrOfGet4PerFee;
      R__b >> fuNrOfChannelsPerGet4;
      R__b >> fuNrOfChannelsPerFee;
      R__b >> fuNrOfGet4;
      R__b >> fuNrOfGet4PerGdpb;
      R__b >> fuNrOfChannelsPerGdpb;
      R__b >> fuNrOfGbtx;
      R__b >> fuNrOfModules;
      {
         vector<Int_t> &R__stl =  fviNrOfRpc;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcType;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcSide;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviModuleId;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> const_cast< UInt_t &>( kuNbFeePerGbtx );
      R__b >> const_cast< UInt_t &>( kuNbGbtxPerGdpb );
      R__b >> fuRawDataPrintMsgNb;
      R__b >> fuRawDataPrintMsgIdx;
      R__b >> fbPrintAllHitsEnable;
      R__b >> fbPrintAllEpochsEnable;
      R__b >> fbPulserModeEnable;
      R__b >> fbCoincMapsEnable;
      R__b >> fbOldFwData;
      R__b >> fuDiamondDpbIdx;
      fsHistoFilename.Streamer(R__b);
      R__b >> fulCurrentTsIndex;
      R__b >> fuCurrentMs;
      R__b >> fuCurrentMsSysId;
      R__b >> fdMsIndex;
      R__b >> fuGdpbId;
      R__b >> fuGdpbNr;
      R__b >> fuGet4Id;
      R__b >> fuGet4Nr;
      R__b >> fiEquipmentId;
      {
         vector<int> &R__stl =  fviMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsMsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsLsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMid;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsFullLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsFullLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTokenLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarDaqCmdLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTrigCmdLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fulCurrentEpochTime;
      {
         map<UInt_t,UInt_t> &R__stl =  fGdpbIdIndexMap;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            unsigned int R__t2;
            R__b >> R__t2;
            typedef unsigned int Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         vector<vector<gdpbv100::Message> > &R__stl =  fvmEpSupprBuffer;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >));
         if (R__tcl1==0) {
            Error("fvmEpSupprBuffer streamer","Missing the TClass object for class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<gdpbv100::Message> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> const_cast< UInt_t &>( kuNbBinsDt );
      R__b >> dMinDt;
      R__b >> dMaxDt;
      R__b >> fuNbFeePlot;
      R__b >> fuNbFeePlotsPerGdpb;
      R__b >> fdStartTime;
      R__b >> fdStartTimeLong;
      R__b >> fdStartTimeMsSz;
      R__b >> fuHistoryHistoSize;
      R__b >> fuHistoryHistoSizeLong;
      R__b >> fdLastRmsUpdateTime;
      R__b >> fdFitZoomWidthPs;
      R__b >> fcMsSizeAll;
      {
         vector<TH1*> &R__stl =  fvhMsSzPerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhMsSzPerLink streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TProfile*> &R__stl =  fvhMsSzTimePerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhMsSzTimePerLink streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhMessType;
      R__b >> fhSysMessType;
      R__b >> fhGet4MessType;
      R__b >> fhGet4ChanScm;
      R__b >> fhGet4ChanErrors;
      R__b >> fhGet4EpochFlags;
      R__b >> fhGdpbMessType;
      R__b >> fhGdpbSysMessType;
      R__b >> fhGdpbSysMessPattType;
      R__b >> fhGdpbEpochFlags;
      R__b >> fhGdpbEpochSyncEvo;
      R__b >> fhGdpbEpochMissEvo;
      {
         vector<TH2*> &R__stl =  fvhGdpbGet4MessType;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhGdpbGet4MessType streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbGet4ChanScm;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhGdpbGet4ChanScm streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbGet4ChanErrors;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhGdpbGet4ChanErrors streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhScmScalerCounters;
      R__b >> fhScmDeadtimeCounters;
      R__b >> fhScmSeuCounters;
      R__b >> fhScmSeuCountersEvo;
      R__b >> fhPatternMissmatch;
      R__b >> fhPatternEnable;
      R__b >> fhPatternResync;
      {
         vector<TH2*> &R__stl =  fvhGdpbPatternMissmatchEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhGdpbPatternMissmatchEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbPatternEnableEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhGdpbPatternEnableEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbPatternResyncEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhGdpbPatternResyncEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbMissmatchEvoPerTs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhGdpbMissmatchEvoPerTs streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbMissmatchEnaEvoPerTs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhGdpbMissmatchEnaEvoPerTs streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbEnableEvoPerTs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhGdpbEnableEvoPerTs streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbResyncEvoPerTs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhGdpbResyncEvoPerTs streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbResyncEnaEvoPerTs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhGdpbResyncEnaEvoPerTs streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbStateEvoPerTs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhGdpbStateEvoPerTs streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhRawFt_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhRawFt_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhRawTot_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhRawTot_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fvhChCount_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhChCount_gDPB streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fvhChannelRate_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhChannelRate_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhRemapTot_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhRemapTot_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fvhRemapChCount_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhRemapChCount_gDPB streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fvhRemapChRate_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhRemapChRate_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fvhFeeRate_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhFeeRate_gDPB streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fvhFeeErrorRate_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhFeeErrorRate_gDPB streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TProfile*> &R__stl =  fvhFeeErrorRatio_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhFeeErrorRatio_gDPB streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fvhFeeRateLong_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhFeeRateLong_gDPB streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fvhFeeErrorRateLong_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhFeeErrorRateLong_gDPB streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TProfile*> &R__stl =  fvhFeeErrorRatioLong_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhFeeErrorRatioLong_gDPB streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<UInt_t> > &R__stl =  fvvuChanNbHitsPerMs;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<unsigned int, class std::allocator<unsigned int> >));
         if (R__tcl1==0) {
            Error("fvvuChanNbHitsPerMs streamer","Missing the TClass object for class std::vector<unsigned int, class std::allocator<unsigned int> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<unsigned int> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhHitsPerMsFirstChan_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhHitsPerMsFirstChan_gDPB streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fvhChannelRatePerMs_gDPB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile2D *));
         if (R__tcl1==0) {
            Error("fvhChannelRatePerMs_gDPB streamer","Missing the TClass object for class TProfile2D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile2D* R__t;
            R__t = (TProfile2D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fvhTokenMsgType;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhTokenMsgType streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fvhTriggerRate;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhTriggerRate streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fvhCmdDaqVsTrig;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhCmdDaqVsTrig streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhStarTokenEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhStarTokenEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhStarTrigGdpbTsEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhStarTrigGdpbTsEvo streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhStarTrigStarTsEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhStarTrigStarTsEvo streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> const_cast< Double_t &>( kdMaxDtPulserPs );
      {
         vector<vector<TH1*> > &R__stl =  fvhTimeDiffPulser;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class TH1 *, class std::allocator<class TH1 *> >));
         if (R__tcl1==0) {
            Error("fvhTimeDiffPulser streamer","Missing the TClass object for class std::vector<class TH1 *, class std::allocator<class TH1 *> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<TH1*> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhTimeMeanPulser;
      R__b >> fhTimeRmsPulser;
      R__b >> fhTimeRmsZoomFitPuls;
      R__b >> fhTimeResFitPuls;
      {
         vector<TProfile*> &R__stl =  fvhPulserTimeDiffEvoGbtxGbtx;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhPulserTimeDiffEvoGbtxGbtx streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<TProfile*> > &R__stl =  fvvhPulserTimeDiffEvoGdpbGdpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class TProfile *, class std::allocator<class TProfile *> >));
         if (R__tcl1==0) {
            Error("fvvhPulserTimeDiffEvoGdpbGdpb streamer","Missing the TClass object for class std::vector<class TProfile *, class std::allocator<class TProfile *> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<TProfile*> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhCoincMapAllChanGdpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhCoincMapAllChanGdpb streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fvhCoincMeanAllChanGdpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile2D *));
         if (R__tcl1==0) {
            Error("fvhCoincMeanAllChanGdpb streamer","Missing the TClass object for class TProfile2D *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile2D* R__t;
            R__t = (TProfile2D*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fcSummary;
      R__b >> fcSummaryGdpb;
      R__b >> fcGdpbChannelCount;
      R__b >> fcGdpbRemapChCount;
      R__b >> fcGdpbChannelRate;
      R__b >> fcGdpbRemapChRate;
      {
         vector<UInt_t> &R__stl =  fvuPadiToGet4;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToPadi;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuElinkToGet4;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToElink;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b.StreamObject(&(fTimeLastHistoSaving),typeid(fTimeLastHistoSaving));
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorTof::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorTof::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fuMsAcceptsPercent;
      R__b << fuTotalMsNb;
      R__b << fuOverlapMsNb;
      R__b << fuCoreMs;
      R__b << fdMsSizeInNs;
      R__b << fdTsCoreSizeInNs;
      R__b << fuMinNbGdpb;
      R__b << fuCurrNbGdpb;
      R__b << fuNrOfGdpbs;
      R__b << fuNrOfFeePerGdpb;
      R__b << fuNrOfGet4PerFee;
      R__b << fuNrOfChannelsPerGet4;
      R__b << fuNrOfChannelsPerFee;
      R__b << fuNrOfGet4;
      R__b << fuNrOfGet4PerGdpb;
      R__b << fuNrOfChannelsPerGdpb;
      R__b << fuNrOfGbtx;
      R__b << fuNrOfModules;
      {
         vector<Int_t> &R__stl =  fviNrOfRpc;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcType;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcSide;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviModuleId;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << const_cast< UInt_t &>( kuNbFeePerGbtx );
      R__b << const_cast< UInt_t &>( kuNbGbtxPerGdpb );
      R__b << fuRawDataPrintMsgNb;
      R__b << fuRawDataPrintMsgIdx;
      R__b << fbPrintAllHitsEnable;
      R__b << fbPrintAllEpochsEnable;
      R__b << fbPulserModeEnable;
      R__b << fbCoincMapsEnable;
      R__b << fbOldFwData;
      R__b << fuDiamondDpbIdx;
      fsHistoFilename.Streamer(R__b);
      R__b << fulCurrentTsIndex;
      R__b << fuCurrentMs;
      R__b << fuCurrentMsSysId;
      R__b << fdMsIndex;
      R__b << fuGdpbId;
      R__b << fuGdpbNr;
      R__b << fuGet4Id;
      R__b << fuGet4Nr;
      R__b << fiEquipmentId;
      {
         vector<int> &R__stl =  fviMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsMsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsLsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMid;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsFullLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsFullLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTokenLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarDaqCmdLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTrigCmdLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fulCurrentEpochTime;
      {
         map<UInt_t,UInt_t> &R__stl =  fGdpbIdIndexMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<UInt_t,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      {
         vector<vector<gdpbv100::Message> > &R__stl =  fvmEpSupprBuffer;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >));
         if (R__tcl1==0) {
            Error("fvmEpSupprBuffer streamer","Missing the TClass object for class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >!");
            return;
         }
            vector<vector<gdpbv100::Message> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<gdpbv100::Message>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << const_cast< UInt_t &>( kuNbBinsDt );
      R__b << dMinDt;
      R__b << dMaxDt;
      R__b << fuNbFeePlot;
      R__b << fuNbFeePlotsPerGdpb;
      R__b << fdStartTime;
      R__b << fdStartTimeLong;
      R__b << fdStartTimeMsSz;
      R__b << fuHistoryHistoSize;
      R__b << fuHistoryHistoSizeLong;
      R__b << fdLastRmsUpdateTime;
      R__b << fdFitZoomWidthPs;
      R__b << fcMsSizeAll;
      {
         vector<TH1*> &R__stl =  fvhMsSzPerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhMsSzTimePerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fhMessType;
      R__b << fhSysMessType;
      R__b << fhGet4MessType;
      R__b << fhGet4ChanScm;
      R__b << fhGet4ChanErrors;
      R__b << fhGet4EpochFlags;
      R__b << fhGdpbMessType;
      R__b << fhGdpbSysMessType;
      R__b << fhGdpbSysMessPattType;
      R__b << fhGdpbEpochFlags;
      R__b << fhGdpbEpochSyncEvo;
      R__b << fhGdpbEpochMissEvo;
      {
         vector<TH2*> &R__stl =  fvhGdpbGet4MessType;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbGet4ChanScm;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbGet4ChanErrors;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fhScmScalerCounters;
      R__b << fhScmDeadtimeCounters;
      R__b << fhScmSeuCounters;
      R__b << fhScmSeuCountersEvo;
      R__b << fhPatternMissmatch;
      R__b << fhPatternEnable;
      R__b << fhPatternResync;
      {
         vector<TH2*> &R__stl =  fvhGdpbPatternMissmatchEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbPatternEnableEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbPatternResyncEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbMissmatchEvoPerTs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbMissmatchEnaEvoPerTs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbEnableEvoPerTs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbResyncEvoPerTs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbResyncEnaEvoPerTs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhGdpbStateEvoPerTs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhRawFt_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhRawTot_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhChCount_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhChannelRate_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhRemapTot_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhRemapChCount_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhRemapChRate_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhFeeRate_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhFeeErrorRate_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhFeeErrorRatio_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhFeeRateLong_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhFeeErrorRateLong_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhFeeErrorRatioLong_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<UInt_t> > &R__stl =  fvvuChanNbHitsPerMs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<unsigned int, class std::allocator<unsigned int> >));
         if (R__tcl1==0) {
            Error("fvvuChanNbHitsPerMs streamer","Missing the TClass object for class std::vector<unsigned int, class std::allocator<unsigned int> >!");
            return;
         }
            vector<vector<UInt_t> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<unsigned int>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhHitsPerMsFirstChan_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fvhChannelRatePerMs_gDPB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile2D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhTokenMsgType;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhTriggerRate;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhCmdDaqVsTrig;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhStarTokenEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhStarTrigGdpbTsEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhStarTrigStarTsEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << const_cast< Double_t &>( kdMaxDtPulserPs );
      {
         vector<vector<TH1*> > &R__stl =  fvhTimeDiffPulser;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class TH1 *, class std::allocator<class TH1 *> >));
         if (R__tcl1==0) {
            Error("fvhTimeDiffPulser streamer","Missing the TClass object for class std::vector<class TH1 *, class std::allocator<class TH1 *> >!");
            return;
         }
            vector<vector<TH1*> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<TH1*>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << fhTimeMeanPulser;
      R__b << fhTimeRmsPulser;
      R__b << fhTimeRmsZoomFitPuls;
      R__b << fhTimeResFitPuls;
      {
         vector<TProfile*> &R__stl =  fvhPulserTimeDiffEvoGbtxGbtx;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<TProfile*> > &R__stl =  fvvhPulserTimeDiffEvoGdpbGdpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class TProfile *, class std::allocator<class TProfile *> >));
         if (R__tcl1==0) {
            Error("fvvhPulserTimeDiffEvoGdpbGdpb streamer","Missing the TClass object for class std::vector<class TProfile *, class std::allocator<class TProfile *> >!");
            return;
         }
            vector<vector<TProfile*> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<TProfile*>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhCoincMapAllChanGdpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile2D*> &R__stl =  fvhCoincMeanAllChanGdpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile2D*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fcSummary;
      R__b << fcSummaryGdpb;
      R__b << fcGdpbChannelCount;
      R__b << fcGdpbRemapChCount;
      R__b << fcGdpbChannelRate;
      R__b << fcGdpbRemapChRate;
      {
         vector<UInt_t> &R__stl =  fvuPadiToGet4;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToPadi;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuElinkToGet4;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToElink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b.StreamObject(&(fTimeLastHistoSaving),typeid(fTimeLastHistoSaving));
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorTof(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorTof : new ::CbmMcbm2018MonitorTof;
   }
   static void *newArray_CbmMcbm2018MonitorTof(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorTof[nElements] : new ::CbmMcbm2018MonitorTof[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorTof(void *p) {
      delete ((::CbmMcbm2018MonitorTof*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorTof(void *p) {
      delete [] ((::CbmMcbm2018MonitorTof*)p);
   }
   static void destruct_CbmMcbm2018MonitorTof(void *p) {
      typedef ::CbmMcbm2018MonitorTof current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorTof(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorTof*)obj)->::CbmMcbm2018MonitorTof::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorTof

//______________________________________________________________________________
void CbmMcbm2018MonitorTofPulser::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorTofPulser.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      fsHistoFileFullname.Streamer(R__b);
      R__b >> fuMsAcceptsPercent;
      R__b >> fuTotalMsNb;
      R__b >> fuOverlapMsNb;
      R__b >> fuCoreMs;
      R__b >> fdMsSizeInNs;
      R__b >> fdTsCoreSizeInNs;
      R__b >> fuMinNbGdpb;
      R__b >> fuCurrNbGdpb;
      R__b >> fuNrOfGdpbs;
      R__b >> fuNrOfFeePerGdpb;
      R__b >> fuNrOfGet4PerFee;
      R__b >> fuNrOfChannelsPerGet4;
      R__b >> fuNrOfChannelsPerFee;
      R__b >> fuNrOfGet4;
      R__b >> fuNrOfGet4PerGdpb;
      R__b >> fuNrOfChannelsPerGdpb;
      R__b >> fuNrOfGbtx;
      R__b >> fuNrOfModules;
      {
         vector<Int_t> &R__stl =  fviNrOfRpc;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcType;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcSide;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviModuleId;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> const_cast< UInt_t &>( kuNbFeePerGbtx );
      R__b >> const_cast< UInt_t &>( kuNbGbtxPerGdpb );
      R__b >> fuRawDataPrintMsgNb;
      R__b >> fuRawDataPrintMsgIdx;
      R__b >> fbPrintAllHitsEnable;
      R__b >> fbPrintAllEpochsEnable;
      R__b >> fbOldFwData;
      R__b >> fuDiamondDpbIdx;
      R__b >> fulCurrentTsIndex;
      R__b >> fuCurrentMs;
      R__b >> fuCurrentMsSysId;
      R__b >> fdMsIndex;
      R__b >> fuGdpbId;
      R__b >> fuGdpbNr;
      R__b >> fuGet4Id;
      R__b >> fuGet4Nr;
      R__b >> fiEquipmentId;
      {
         vector<int> &R__stl =  fviMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsMsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsLsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMid;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsFullLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsFullLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTokenLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarDaqCmdLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTrigCmdLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fulCurrentEpochTime;
      {
         map<UInt_t,UInt_t> &R__stl =  fGdpbIdIndexMap;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            unsigned int R__t2;
            R__b >> R__t2;
            typedef unsigned int Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         vector<vector<gdpbv100::Message> > &R__stl =  fvmEpSupprBuffer;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >));
         if (R__tcl1==0) {
            Error("fvmEpSupprBuffer streamer","Missing the TClass object for class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<gdpbv100::Message> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> const_cast< UInt_t &>( kuNbBinsDt );
      R__b >> dMinDt;
      R__b >> dMaxDt;
      R__b >> fuNbFeePlot;
      R__b >> fuNbFeePlotsPerGdpb;
      R__b >> fdStartTime;
      R__b >> fdStartTimeLong;
      R__b >> fdStartTimeMsSz;
      R__b >> fuHistoryHistoSize;
      R__b >> fuHistoryHistoSizeLong;
      R__b >> fdLastRmsUpdateTime;
      R__b >> fdFitZoomWidthPs;
      R__b >> fcMsSizeAll;
      {
         vector<TH1*> &R__stl =  fvhMsSzPerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhMsSzPerLink streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TProfile*> &R__stl =  fvhMsSzTimePerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhMsSzTimePerLink streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> const_cast< Double_t &>( kdMaxDtPulserPs );
      {
         vector<vector<TH1*> > &R__stl =  fvhTimeDiffPulser;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class TH1 *, class std::allocator<class TH1 *> >));
         if (R__tcl1==0) {
            Error("fvhTimeDiffPulser streamer","Missing the TClass object for class std::vector<class TH1 *, class std::allocator<class TH1 *> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<TH1*> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhTimeMeanPulser;
      R__b >> fhTimeRmsPulser;
      R__b >> fhTimeRmsZoomFitPuls;
      R__b >> fhTimeResFitPuls;
      {
         vector<TH2*> &R__stl =  fvhPulserCountEvoPerFeeGdpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhPulserCountEvoPerFeeGdpb streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhPulserTimeDiffEvoGbtxGbtx;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhPulserTimeDiffEvoGbtxGbtx streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<TProfile*> > &R__stl =  fvvhPulserTimeDiffEvoGdpbGdpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class TProfile *, class std::allocator<class TProfile *> >));
         if (R__tcl1==0) {
            Error("fvvhPulserTimeDiffEvoGdpbGdpb streamer","Missing the TClass object for class std::vector<class TProfile *, class std::allocator<class TProfile *> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<TProfile*> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b.ReadStaticArray((unsigned int*)kuRefFeeEvoIdx);
      {
         vector<vector<TProfile*> > &R__stl =  fvvhPulserTimeDiffEvoFeeFee;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class TProfile *, class std::allocator<class TProfile *> >));
         if (R__tcl1==0) {
            Error("fvvhPulserTimeDiffEvoFeeFee streamer","Missing the TClass object for class std::vector<class TProfile *, class std::allocator<class TProfile *> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<TProfile*> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhFeeFtDistribPerCh;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhFeeFtDistribPerCh streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhFeeFtNormDnl;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhFeeFtNormDnl streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fhFeeFtNormInl;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fhFeeFtNormInl streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuPadiToGet4;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToPadi;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuElinkToGet4;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToElink;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b.StreamObject(&(fTimeLastHistoSaving),typeid(fTimeLastHistoSaving));
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorTofPulser::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorTofPulser::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      fsHistoFileFullname.Streamer(R__b);
      R__b << fuMsAcceptsPercent;
      R__b << fuTotalMsNb;
      R__b << fuOverlapMsNb;
      R__b << fuCoreMs;
      R__b << fdMsSizeInNs;
      R__b << fdTsCoreSizeInNs;
      R__b << fuMinNbGdpb;
      R__b << fuCurrNbGdpb;
      R__b << fuNrOfGdpbs;
      R__b << fuNrOfFeePerGdpb;
      R__b << fuNrOfGet4PerFee;
      R__b << fuNrOfChannelsPerGet4;
      R__b << fuNrOfChannelsPerFee;
      R__b << fuNrOfGet4;
      R__b << fuNrOfGet4PerGdpb;
      R__b << fuNrOfChannelsPerGdpb;
      R__b << fuNrOfGbtx;
      R__b << fuNrOfModules;
      {
         vector<Int_t> &R__stl =  fviNrOfRpc;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcType;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcSide;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviModuleId;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << const_cast< UInt_t &>( kuNbFeePerGbtx );
      R__b << const_cast< UInt_t &>( kuNbGbtxPerGdpb );
      R__b << fuRawDataPrintMsgNb;
      R__b << fuRawDataPrintMsgIdx;
      R__b << fbPrintAllHitsEnable;
      R__b << fbPrintAllEpochsEnable;
      R__b << fbOldFwData;
      R__b << fuDiamondDpbIdx;
      R__b << fulCurrentTsIndex;
      R__b << fuCurrentMs;
      R__b << fuCurrentMsSysId;
      R__b << fdMsIndex;
      R__b << fuGdpbId;
      R__b << fuGdpbNr;
      R__b << fuGet4Id;
      R__b << fuGet4Nr;
      R__b << fiEquipmentId;
      {
         vector<int> &R__stl =  fviMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsMsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsLsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMid;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsFullLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsFullLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTokenLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarDaqCmdLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTrigCmdLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fulCurrentEpochTime;
      {
         map<UInt_t,UInt_t> &R__stl =  fGdpbIdIndexMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<UInt_t,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      {
         vector<vector<gdpbv100::Message> > &R__stl =  fvmEpSupprBuffer;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >));
         if (R__tcl1==0) {
            Error("fvmEpSupprBuffer streamer","Missing the TClass object for class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >!");
            return;
         }
            vector<vector<gdpbv100::Message> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<gdpbv100::Message>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << const_cast< UInt_t &>( kuNbBinsDt );
      R__b << dMinDt;
      R__b << dMaxDt;
      R__b << fuNbFeePlot;
      R__b << fuNbFeePlotsPerGdpb;
      R__b << fdStartTime;
      R__b << fdStartTimeLong;
      R__b << fdStartTimeMsSz;
      R__b << fuHistoryHistoSize;
      R__b << fuHistoryHistoSizeLong;
      R__b << fdLastRmsUpdateTime;
      R__b << fdFitZoomWidthPs;
      R__b << fcMsSizeAll;
      {
         vector<TH1*> &R__stl =  fvhMsSzPerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhMsSzTimePerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << const_cast< Double_t &>( kdMaxDtPulserPs );
      {
         vector<vector<TH1*> > &R__stl =  fvhTimeDiffPulser;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class TH1 *, class std::allocator<class TH1 *> >));
         if (R__tcl1==0) {
            Error("fvhTimeDiffPulser streamer","Missing the TClass object for class std::vector<class TH1 *, class std::allocator<class TH1 *> >!");
            return;
         }
            vector<vector<TH1*> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<TH1*>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << fhTimeMeanPulser;
      R__b << fhTimeRmsPulser;
      R__b << fhTimeRmsZoomFitPuls;
      R__b << fhTimeResFitPuls;
      {
         vector<TH2*> &R__stl =  fvhPulserCountEvoPerFeeGdpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhPulserTimeDiffEvoGbtxGbtx;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<vector<TProfile*> > &R__stl =  fvvhPulserTimeDiffEvoGdpbGdpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class TProfile *, class std::allocator<class TProfile *> >));
         if (R__tcl1==0) {
            Error("fvvhPulserTimeDiffEvoGdpbGdpb streamer","Missing the TClass object for class std::vector<class TProfile *, class std::allocator<class TProfile *> >!");
            return;
         }
            vector<vector<TProfile*> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<TProfile*>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b.WriteArray(kuRefFeeEvoIdx, 2);
      {
         vector<vector<TProfile*> > &R__stl =  fvvhPulserTimeDiffEvoFeeFee;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class TProfile *, class std::allocator<class TProfile *> >));
         if (R__tcl1==0) {
            Error("fvvhPulserTimeDiffEvoFeeFee streamer","Missing the TClass object for class std::vector<class TProfile *, class std::allocator<class TProfile *> >!");
            return;
         }
            vector<vector<TProfile*> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<TProfile*>*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhFeeFtDistribPerCh;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhFeeFtNormDnl;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fhFeeFtNormInl;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuPadiToGet4;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToPadi;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuElinkToGet4;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToElink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b.StreamObject(&(fTimeLastHistoSaving),typeid(fTimeLastHistoSaving));
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorTofPulser(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorTofPulser : new ::CbmMcbm2018MonitorTofPulser;
   }
   static void *newArray_CbmMcbm2018MonitorTofPulser(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorTofPulser[nElements] : new ::CbmMcbm2018MonitorTofPulser[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorTofPulser(void *p) {
      delete ((::CbmMcbm2018MonitorTofPulser*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorTofPulser(void *p) {
      delete [] ((::CbmMcbm2018MonitorTofPulser*)p);
   }
   static void destruct_CbmMcbm2018MonitorTofPulser(void *p) {
      typedef ::CbmMcbm2018MonitorTofPulser current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorTofPulser(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorTofPulser*)obj)->::CbmMcbm2018MonitorTofPulser::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorTofPulser

//______________________________________________________________________________
void CbmMcbm2018MonitorAlgoTof::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorAlgoTof.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fiGdpbIndex;
      R__b >> fuMinTotPulser;
      R__b >> fuMaxTotPulser;
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      {
         map<gdpbv100::MessageTypes,UInt_t> &R__stl =  fmMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            gdpbv100::MessageTypes R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<gdpbv100::MessageTypes>(readtemp);
            unsigned int R__t2;
            R__b >> R__t2;
            typedef enum gdpbv100::MessageTypes Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         vector<gdpbv100::Message> &R__stl =  fvmEpSupprBuffer;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class gdpbv100::Message));
         if (R__tcl1==0) {
            Error("fvmEpSupprBuffer streamer","Missing the TClass object for class gdpbv100::Message!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            gdpbv100::Message R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fdStartTime;
      R__b >> fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsMsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsLsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMid;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsFullLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsFullLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTokenLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarDaqCmdLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTrigCmdLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fbEpochSinceLastHit;
      R__b >> fuDuplicatesCount;
      R__b.StreamObject(&(fmLastHit),typeid(fmLastHit));
      R__b >> fuHistoryHistoSize;
      {
         vector<UInt_t> &R__stl =  fuNbMissmatchPattern;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorAlgoTof::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorAlgoTof::IsA(), kTRUE);
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fiGdpbIndex;
      R__b << fuMinTotPulser;
      R__b << fuMaxTotPulser;
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      {
         map<gdpbv100::MessageTypes,UInt_t> &R__stl =  fmMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<gdpbv100::MessageTypes,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      {
         vector<gdpbv100::Message> &R__stl =  fvmEpSupprBuffer;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class gdpbv100::Message));
         if (R__tcl1==0) {
            Error("fvmEpSupprBuffer streamer","Missing the TClass object for class gdpbv100::Message!");
            return;
         }
            vector<gdpbv100::Message>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((gdpbv100::Message*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << fdStartTime;
      R__b << fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsMsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsLsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMid;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsFullLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsFullLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTokenLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarDaqCmdLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTrigCmdLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fbEpochSinceLastHit;
      R__b << fuDuplicatesCount;
      R__b.StreamObject(&(fmLastHit),typeid(fmLastHit));
      R__b << fuHistoryHistoSize;
      {
         vector<UInt_t> &R__stl =  fuNbMissmatchPattern;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
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
   static void *new_CbmMcbm2018MonitorAlgoTof(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorAlgoTof : new ::CbmMcbm2018MonitorAlgoTof;
   }
   static void *newArray_CbmMcbm2018MonitorAlgoTof(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorAlgoTof[nElements] : new ::CbmMcbm2018MonitorAlgoTof[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorAlgoTof(void *p) {
      delete ((::CbmMcbm2018MonitorAlgoTof*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorAlgoTof(void *p) {
      delete [] ((::CbmMcbm2018MonitorAlgoTof*)p);
   }
   static void destruct_CbmMcbm2018MonitorAlgoTof(void *p) {
      typedef ::CbmMcbm2018MonitorAlgoTof current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorAlgoTof(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorAlgoTof*)obj)->::CbmMcbm2018MonitorAlgoTof::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorAlgoTof

//______________________________________________________________________________
void CbmMcbm2018MonitorTaskTof::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorTaskTof.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fuHistoryHistoSize;
      fsHistoFileName.Streamer(R__b);
      R__b >> fuMinTotPulser;
      R__b >> fuMaxTotPulser;
      R__b >> fiGdpbIndex;
      R__b >> fulTsCounter;
      R__b >> fMonitorAlgo;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorTaskTof::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorTaskTof::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fuHistoryHistoSize;
      fsHistoFileName.Streamer(R__b);
      R__b << fuMinTotPulser;
      R__b << fuMaxTotPulser;
      R__b << fiGdpbIndex;
      R__b << fulTsCounter;
      R__b << fMonitorAlgo;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorTaskTof(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorTaskTof : new ::CbmMcbm2018MonitorTaskTof;
   }
   static void *newArray_CbmMcbm2018MonitorTaskTof(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorTaskTof[nElements] : new ::CbmMcbm2018MonitorTaskTof[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorTaskTof(void *p) {
      delete ((::CbmMcbm2018MonitorTaskTof*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorTaskTof(void *p) {
      delete [] ((::CbmMcbm2018MonitorTaskTof*)p);
   }
   static void destruct_CbmMcbm2018MonitorTaskTof(void *p) {
      typedef ::CbmMcbm2018MonitorTaskTof current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorTaskTof(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorTaskTof*)obj)->::CbmMcbm2018MonitorTaskTof::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorTaskTof

//______________________________________________________________________________
void CbmMcbm2018MonitorAlgoTofPulser::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorAlgoTofPulser.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fiGdpbIndex;
      R__b >> fuUpdateFreqTs;
      R__b >> fuPulserMinTot;
      R__b >> fuPulserMaxTot;
      R__b >> fuPulserChannel;
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      {
         vector<gdpbv100::Message> &R__stl =  fvmEpSupprBuffer;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class gdpbv100::Message));
         if (R__tcl1==0) {
            Error("fvmEpSupprBuffer streamer","Missing the TClass object for class gdpbv100::Message!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            gdpbv100::Message R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> const_cast< Double_t &>( kdMaxDtPulserPs );
      R__b >> const_cast< UInt_t &>( kuNbBinsDt );
      R__b >> dMinDt;
      R__b >> dMaxDt;
      R__b >> const_cast< Double_t &>( kdFitZoomWidthPs );
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorAlgoTofPulser::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorAlgoTofPulser::IsA(), kTRUE);
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fiGdpbIndex;
      R__b << fuUpdateFreqTs;
      R__b << fuPulserMinTot;
      R__b << fuPulserMaxTot;
      R__b << fuPulserChannel;
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      {
         vector<gdpbv100::Message> &R__stl =  fvmEpSupprBuffer;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class gdpbv100::Message));
         if (R__tcl1==0) {
            Error("fvmEpSupprBuffer streamer","Missing the TClass object for class gdpbv100::Message!");
            return;
         }
            vector<gdpbv100::Message>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((gdpbv100::Message*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << const_cast< Double_t &>( kdMaxDtPulserPs );
      R__b << const_cast< UInt_t &>( kuNbBinsDt );
      R__b << dMinDt;
      R__b << dMaxDt;
      R__b << const_cast< Double_t &>( kdFitZoomWidthPs );
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorAlgoTofPulser(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorAlgoTofPulser : new ::CbmMcbm2018MonitorAlgoTofPulser;
   }
   static void *newArray_CbmMcbm2018MonitorAlgoTofPulser(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorAlgoTofPulser[nElements] : new ::CbmMcbm2018MonitorAlgoTofPulser[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorAlgoTofPulser(void *p) {
      delete ((::CbmMcbm2018MonitorAlgoTofPulser*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorAlgoTofPulser(void *p) {
      delete [] ((::CbmMcbm2018MonitorAlgoTofPulser*)p);
   }
   static void destruct_CbmMcbm2018MonitorAlgoTofPulser(void *p) {
      typedef ::CbmMcbm2018MonitorAlgoTofPulser current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorAlgoTofPulser(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorAlgoTofPulser*)obj)->::CbmMcbm2018MonitorAlgoTofPulser::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorAlgoTofPulser

//______________________________________________________________________________
void CbmMcbm2018MonitorTaskTofPulser::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorTaskTofPulser.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      fsHistoFileName.Streamer(R__b);
      R__b >> fuUpdateFreqTs;
      R__b >> fuPulserMinTot;
      R__b >> fuPulserMaxTot;
      R__b >> fuPulserChannel;
      R__b >> fiGdpbIndex;
      R__b >> fuHistoryHistoSize;
      R__b >> fulTsCounter;
      R__b >> fMonitorPulserAlgo;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorTaskTofPulser::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorTaskTofPulser::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      fsHistoFileName.Streamer(R__b);
      R__b << fuUpdateFreqTs;
      R__b << fuPulserMinTot;
      R__b << fuPulserMaxTot;
      R__b << fuPulserChannel;
      R__b << fiGdpbIndex;
      R__b << fuHistoryHistoSize;
      R__b << fulTsCounter;
      R__b << fMonitorPulserAlgo;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorTaskTofPulser(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorTaskTofPulser : new ::CbmMcbm2018MonitorTaskTofPulser;
   }
   static void *newArray_CbmMcbm2018MonitorTaskTofPulser(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorTaskTofPulser[nElements] : new ::CbmMcbm2018MonitorTaskTofPulser[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorTaskTofPulser(void *p) {
      delete ((::CbmMcbm2018MonitorTaskTofPulser*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorTaskTofPulser(void *p) {
      delete [] ((::CbmMcbm2018MonitorTaskTofPulser*)p);
   }
   static void destruct_CbmMcbm2018MonitorTaskTofPulser(void *p) {
      typedef ::CbmMcbm2018MonitorTaskTofPulser current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorTaskTofPulser(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorTaskTofPulser*)obj)->::CbmMcbm2018MonitorTaskTofPulser::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorTaskTofPulser

//______________________________________________________________________________
void CbmMcbm2018TofTestFee::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018TofTestFee.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      fsHistoFileFullname.Streamer(R__b);
      R__b >> fuMsAcceptsPercent;
      R__b >> fuTotalMsNb;
      R__b >> fuOverlapMsNb;
      R__b >> fuCoreMs;
      R__b >> fdMsSizeInNs;
      R__b >> fdTsCoreSizeInNs;
      R__b >> fuMinNbGdpb;
      R__b >> fuCurrNbGdpb;
      R__b >> fuNrOfGdpbs;
      R__b >> fuNrOfFeePerGdpb;
      R__b >> fuNrOfGet4PerFee;
      R__b >> fuNrOfChannelsPerGet4;
      R__b >> fuNrOfChannelsPerFee;
      R__b >> fuNrOfGet4;
      R__b >> fuNrOfGet4PerGdpb;
      R__b >> fuNrOfChannelsPerGdpb;
      R__b >> fuNrOfGbtx;
      R__b >> fuNrOfModules;
      {
         vector<Int_t> &R__stl =  fviNrOfRpc;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcType;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcSide;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviModuleId;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> const_cast< UInt_t &>( kuNbFeePerGbtx );
      R__b >> const_cast< UInt_t &>( kuNbGbtxPerGdpb );
      R__b >> fuRawDataPrintMsgNb;
      R__b >> fuRawDataPrintMsgIdx;
      R__b >> fbPrintAllHitsEnable;
      R__b >> fbPrintAllEpochsEnable;
      R__b >> fulCurrentTsIndex;
      R__b >> fuCurrentMs;
      R__b >> fuCurrentMsSysId;
      R__b >> fdMsIndex;
      R__b >> fuGdpbId;
      R__b >> fuGdpbNr;
      R__b >> fuGet4Id;
      R__b >> fuGet4Nr;
      R__b >> fiEquipmentId;
      {
         vector<int> &R__stl =  fviMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsMsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsLsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMid;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsFullLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsFullLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTokenLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarDaqCmdLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTrigCmdLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fulCurrentEpochTime;
      {
         map<UInt_t,UInt_t> &R__stl =  fGdpbIdIndexMap;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            unsigned int R__t2;
            R__b >> R__t2;
            typedef unsigned int Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         vector<vector<gdpbv100::Message> > &R__stl =  fvmEpSupprBuffer;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >));
         if (R__tcl1==0) {
            Error("fvmEpSupprBuffer streamer","Missing the TClass object for class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<gdpbv100::Message> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> const_cast< UInt_t &>( kuNbBinsDt );
      R__b >> dMinDt;
      R__b >> dMaxDt;
      R__b >> fuNbFeePlot;
      R__b >> fuNbFeePlotsPerGdpb;
      R__b >> fdStartTime;
      R__b >> fdStartTimeLong;
      R__b >> fdStartTimeMsSz;
      R__b >> fuHistoryHistoSize;
      R__b >> fuHistoryHistoSizeLong;
      R__b >> fdLastRmsUpdateTime;
      R__b >> fdFitZoomWidthPs;
      R__b >> fcMsSizeAll;
      {
         vector<TH1*> &R__stl =  fvhMsSzPerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhMsSzPerLink streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TProfile*> &R__stl =  fvhMsSzTimePerLink;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TProfile *));
         if (R__tcl1==0) {
            Error("fvhMsSzTimePerLink streamer","Missing the TClass object for class TProfile *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TProfile* R__t;
            R__t = (TProfile*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> const_cast< Double_t &>( kdMaxDtPulserPs );
      R__b >> fuGdpbA;
      R__b >> fuGbtxA;
      R__b >> fuFeeA;
      R__b >> fuGlobalIdxFeeA;
      R__b >> fuGdpbB;
      R__b >> fuGbtxB;
      R__b >> fuFeeB;
      R__b >> fuGlobalIdxFeeB;
      R__b >> fhChanTotFeeA;
      R__b >> fhChanTotFeeB;
      R__b >> fhChanPulseIntervalFeeA;
      R__b >> fhChanPulseIntervalFeeB;
      {
         vector<TH2*> &R__stl =  fvhPulserCountEvoPerFeeGdpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhPulserCountEvoPerFeeGdpb streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fcPulserFeeA;
      R__b >> fcPulserFeeB;
      R__b >> fcPulserFeeFee;
      R__b >> fcPulseProp;
      {
         vector<UInt_t> &R__stl =  fvuPadiToGet4;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToPadi;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuElinkToGet4;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToElink;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b.StreamObject(&(fTimeLastHistoSaving),typeid(fTimeLastHistoSaving));
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018TofTestFee::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018TofTestFee::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      fsHistoFileFullname.Streamer(R__b);
      R__b << fuMsAcceptsPercent;
      R__b << fuTotalMsNb;
      R__b << fuOverlapMsNb;
      R__b << fuCoreMs;
      R__b << fdMsSizeInNs;
      R__b << fdTsCoreSizeInNs;
      R__b << fuMinNbGdpb;
      R__b << fuCurrNbGdpb;
      R__b << fuNrOfGdpbs;
      R__b << fuNrOfFeePerGdpb;
      R__b << fuNrOfGet4PerFee;
      R__b << fuNrOfChannelsPerGet4;
      R__b << fuNrOfChannelsPerFee;
      R__b << fuNrOfGet4;
      R__b << fuNrOfGet4PerGdpb;
      R__b << fuNrOfChannelsPerGdpb;
      R__b << fuNrOfGbtx;
      R__b << fuNrOfModules;
      {
         vector<Int_t> &R__stl =  fviNrOfRpc;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcType;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcSide;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviModuleId;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << const_cast< UInt_t &>( kuNbFeePerGbtx );
      R__b << const_cast< UInt_t &>( kuNbGbtxPerGdpb );
      R__b << fuRawDataPrintMsgNb;
      R__b << fuRawDataPrintMsgIdx;
      R__b << fbPrintAllHitsEnable;
      R__b << fbPrintAllEpochsEnable;
      R__b << fulCurrentTsIndex;
      R__b << fuCurrentMs;
      R__b << fuCurrentMsSysId;
      R__b << fdMsIndex;
      R__b << fuGdpbId;
      R__b << fuGdpbNr;
      R__b << fuGet4Id;
      R__b << fuGet4Nr;
      R__b << fiEquipmentId;
      {
         vector<int> &R__stl =  fviMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsMsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsLsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMid;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsFullLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsFullLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTokenLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarDaqCmdLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTrigCmdLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fulCurrentEpochTime;
      {
         map<UInt_t,UInt_t> &R__stl =  fGdpbIdIndexMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<UInt_t,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      {
         vector<vector<gdpbv100::Message> > &R__stl =  fvmEpSupprBuffer;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >));
         if (R__tcl1==0) {
            Error("fvmEpSupprBuffer streamer","Missing the TClass object for class std::vector<class gdpbv100::Message, class std::allocator<class gdpbv100::Message> >!");
            return;
         }
            vector<vector<gdpbv100::Message> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<gdpbv100::Message>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << const_cast< UInt_t &>( kuNbBinsDt );
      R__b << dMinDt;
      R__b << dMaxDt;
      R__b << fuNbFeePlot;
      R__b << fuNbFeePlotsPerGdpb;
      R__b << fdStartTime;
      R__b << fdStartTimeLong;
      R__b << fdStartTimeMsSz;
      R__b << fuHistoryHistoSize;
      R__b << fuHistoryHistoSizeLong;
      R__b << fdLastRmsUpdateTime;
      R__b << fdFitZoomWidthPs;
      R__b << fcMsSizeAll;
      {
         vector<TH1*> &R__stl =  fvhMsSzPerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TProfile*> &R__stl =  fvhMsSzTimePerLink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TProfile*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << const_cast< Double_t &>( kdMaxDtPulserPs );
      R__b << fuGdpbA;
      R__b << fuGbtxA;
      R__b << fuFeeA;
      R__b << fuGlobalIdxFeeA;
      R__b << fuGdpbB;
      R__b << fuGbtxB;
      R__b << fuFeeB;
      R__b << fuGlobalIdxFeeB;
      R__b << fhChanTotFeeA;
      R__b << fhChanTotFeeB;
      R__b << fhChanPulseIntervalFeeA;
      R__b << fhChanPulseIntervalFeeB;
      {
         vector<TH2*> &R__stl =  fvhPulserCountEvoPerFeeGdpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fcPulserFeeA;
      R__b << fcPulserFeeB;
      R__b << fcPulserFeeFee;
      R__b << fcPulseProp;
      {
         vector<UInt_t> &R__stl =  fvuPadiToGet4;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToPadi;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuElinkToGet4;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToElink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b.StreamObject(&(fTimeLastHistoSaving),typeid(fTimeLastHistoSaving));
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018TofTestFee(void *p) {
      return  p ? new(p) ::CbmMcbm2018TofTestFee : new ::CbmMcbm2018TofTestFee;
   }
   static void *newArray_CbmMcbm2018TofTestFee(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018TofTestFee[nElements] : new ::CbmMcbm2018TofTestFee[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018TofTestFee(void *p) {
      delete ((::CbmMcbm2018TofTestFee*)p);
   }
   static void deleteArray_CbmMcbm2018TofTestFee(void *p) {
      delete [] ((::CbmMcbm2018TofTestFee*)p);
   }
   static void destruct_CbmMcbm2018TofTestFee(void *p) {
      typedef ::CbmMcbm2018TofTestFee current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018TofTestFee(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018TofTestFee*)obj)->::CbmMcbm2018TofTestFee::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018TofTestFee

//______________________________________________________________________________
void CbmMcbm2018TofFeeThr::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018TofFeeThr.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      fsHistoFileFullname.Streamer(R__b);
      R__b >> fuMsAcceptsPercent;
      R__b >> fuTotalMsNb;
      R__b >> fuOverlapMsNb;
      R__b >> fuCoreMs;
      R__b >> fdMsSizeInNs;
      R__b >> fdTsCoreSizeInNs;
      R__b >> fuMinNbGdpb;
      R__b >> fuCurrNbGdpb;
      R__b >> fuNrOfGdpbs;
      R__b >> fuNrOfFeePerGdpb;
      R__b >> fuNrOfGet4PerFee;
      R__b >> fuNrOfChannelsPerGet4;
      R__b >> fuNrOfChannelsPerFee;
      R__b >> fuNrOfGet4;
      R__b >> fuNrOfGet4PerGdpb;
      R__b >> fuNrOfChannelsPerGdpb;
      R__b >> fuNrOfGbtx;
      R__b >> const_cast< UInt_t &>( kuNbFeePerGbtx );
      R__b >> const_cast< UInt_t &>( kuNbGbtxPerGdpb );
      {
         map<UInt_t,UInt_t> &R__stl =  fGdpbIdIndexMap;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            unsigned int R__t2;
            R__b >> R__t2;
            typedef unsigned int Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fulCurrentTsIndex;
      R__b >> fuCurrentMs;
      R__b >> fuCurrentMsSysId;
      R__b >> fdMsIndex;
      R__b >> fuGdpbId;
      R__b >> fuGdpbNr;
      R__b >> fuGet4Id;
      R__b >> fuGet4Nr;
      R__b >> fiEquipmentId;
      {
         vector<int> &R__stl =  fviMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fhGdpbAsicSpiCounts;
      {
         vector<UInt_t> &R__stl =  fvuPadiToGet4;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToPadi;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuElinkToGet4;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToElink;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Double_t> &R__stl =  fvdPadiThrCodeToValue;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            double R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018TofFeeThr::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018TofFeeThr::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      fsHistoFileFullname.Streamer(R__b);
      R__b << fuMsAcceptsPercent;
      R__b << fuTotalMsNb;
      R__b << fuOverlapMsNb;
      R__b << fuCoreMs;
      R__b << fdMsSizeInNs;
      R__b << fdTsCoreSizeInNs;
      R__b << fuMinNbGdpb;
      R__b << fuCurrNbGdpb;
      R__b << fuNrOfGdpbs;
      R__b << fuNrOfFeePerGdpb;
      R__b << fuNrOfGet4PerFee;
      R__b << fuNrOfChannelsPerGet4;
      R__b << fuNrOfChannelsPerFee;
      R__b << fuNrOfGet4;
      R__b << fuNrOfGet4PerGdpb;
      R__b << fuNrOfChannelsPerGdpb;
      R__b << fuNrOfGbtx;
      R__b << const_cast< UInt_t &>( kuNbFeePerGbtx );
      R__b << const_cast< UInt_t &>( kuNbGbtxPerGdpb );
      {
         map<UInt_t,UInt_t> &R__stl =  fGdpbIdIndexMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<UInt_t,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fulCurrentTsIndex;
      R__b << fuCurrentMs;
      R__b << fuCurrentMsSysId;
      R__b << fdMsIndex;
      R__b << fuGdpbId;
      R__b << fuGdpbNr;
      R__b << fuGet4Id;
      R__b << fuGet4Nr;
      R__b << fiEquipmentId;
      {
         vector<int> &R__stl =  fviMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fhGdpbAsicSpiCounts;
      {
         vector<UInt_t> &R__stl =  fvuPadiToGet4;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToPadi;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuElinkToGet4;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuGet4ToElink;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Double_t> &R__stl =  fvdPadiThrCodeToValue;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Double_t>::iterator R__k;
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
   static void *new_CbmMcbm2018TofFeeThr(void *p) {
      return  p ? new(p) ::CbmMcbm2018TofFeeThr : new ::CbmMcbm2018TofFeeThr;
   }
   static void *newArray_CbmMcbm2018TofFeeThr(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018TofFeeThr[nElements] : new ::CbmMcbm2018TofFeeThr[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018TofFeeThr(void *p) {
      delete ((::CbmMcbm2018TofFeeThr*)p);
   }
   static void deleteArray_CbmMcbm2018TofFeeThr(void *p) {
      delete [] ((::CbmMcbm2018TofFeeThr*)p);
   }
   static void destruct_CbmMcbm2018TofFeeThr(void *p) {
      typedef ::CbmMcbm2018TofFeeThr current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018TofFeeThr(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018TofFeeThr*)obj)->::CbmMcbm2018TofFeeThr::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018TofFeeThr

//______________________________________________________________________________
void CbmMcbm2018MonitorAlgoPsd::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorAlgoPsd.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b >> fbFirstPackageError;
      R__b >> fbPsdMissedData;
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> const_cast< float &>( kfAdc_to_mV );
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      R__b >> fdStartTime;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b >> fuHistoryHistoSize;
      {
         vector<Int_t> &R__stl =  fviHistoChargeArgs;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviHistoAmplArgs;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviHistoZLArgs;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fuMsgsCntInMs;
      R__b >> fuReadMsgsCntInMs;
      R__b >> fuLostMsgsCntInMs;
      R__b >> fuReadEvtCntInMs;
      {
         vector<TH2*> &R__stl =  fvhHitZLChanEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhHitZLChanEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhHitLPChanEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhHitLPChanEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhHitFAChanEvo;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhHitFAChanEvo streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fvhHitChargeChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhHitChargeChan streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fvhHitZeroLevelChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhHitZeroLevelChan streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fvhHitAmplChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhHitAmplChan streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fvhHitChargeByWfmChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhHitChargeByWfmChan streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH1*> &R__stl =  fvhHitWfmChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhHitWfmChan streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<UInt_t> &R__stl =  kvuWfmRanges;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  kvuWfmInRangeToChangeChan;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH1*> &R__stl =  fv3hHitWfmFlattenedChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fv3hHitWfmFlattenedChan streamer","Missing the TClass object for class TH1 *!");
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
      R__b >> fbSpillOn;
      R__b >> fuCurrentSpillIdx;
      R__b >> fuCurrentSpillPlot;
      R__b >> fdStartTimeSpill;
      R__b >> fdLastSecondTime;
      R__b >> fuCountsLastSecond;
      R__b.ReadStaticArray((unsigned int*)kuPsdChanMap);
      R__b >> fhHitChargeMap;
      R__b >> fhHitMapEvo;
      R__b >> fhChanHitMapEvo;
      R__b >> fhMissedData;
      R__b >> fhAdcTime;
      R__b >> fhMsLengthEvo;
      R__b >> fhMsgsCntPerMsEvo;
      R__b >> fhReadMsgsCntPerMsEvo;
      R__b >> fhLostMsgsCntPerMsEvo;
      R__b >> fhReadEvtsCntPerMsEvo;
      {
         vector<TH1*> &R__stl =  fvhHitFitWfmChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH1 *));
         if (R__tcl1==0) {
            Error("fvhHitFitWfmChan streamer","Missing the TClass object for class TH1 *!");
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
      {
         vector<TH2*> &R__stl =  fvhFitHarmonic1Chan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhFitHarmonic1Chan streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhFitHarmonic2Chan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhFitHarmonic2Chan streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TH2*> &R__stl =  fvhFitQaChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TH2 *));
         if (R__tcl1==0) {
            Error("fvhFitQaChan streamer","Missing the TClass object for class TH2 *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TH2* R__t;
            R__t = (TH2*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fcSummary;
      R__b >> fcHitMaps;
      R__b >> fcZLevo;
      R__b >> fcChargesFPGA;
      R__b >> fcChargesWfm;
      R__b >> fcAmplitudes;
      R__b >> fcZeroLevels;
      R__b >> fcGenCntsPerMs;
      R__b >> fcWfmsAllChannels;
      {
         vector<TCanvas*> &R__stl =  fvcWfmsChan;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TCanvas *));
         if (R__tcl1==0) {
            Error("fvcWfmsChan streamer","Missing the TClass object for class TCanvas *!");
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
      R__b >> fcPronyFit;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorAlgoPsd::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorAlgoPsd::IsA(), kTRUE);
      R__b << fbFirstPackageError;
      R__b << fbPsdMissedData;
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << const_cast< float &>( kfAdc_to_mV );
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      R__b << fdStartTime;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b << fuHistoryHistoSize;
      {
         vector<Int_t> &R__stl =  fviHistoChargeArgs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviHistoAmplArgs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviHistoZLArgs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fuMsgsCntInMs;
      R__b << fuReadMsgsCntInMs;
      R__b << fuLostMsgsCntInMs;
      R__b << fuReadEvtCntInMs;
      {
         vector<TH2*> &R__stl =  fvhHitZLChanEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhHitLPChanEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhHitFAChanEvo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhHitChargeChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhHitZeroLevelChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhHitAmplChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhHitChargeByWfmChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fvhHitWfmChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  kvuWfmRanges;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  kvuWfmInRangeToChangeChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH1*> &R__stl =  fv3hHitWfmFlattenedChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fbSpillOn;
      R__b << fuCurrentSpillIdx;
      R__b << fuCurrentSpillPlot;
      R__b << fdStartTimeSpill;
      R__b << fdLastSecondTime;
      R__b << fuCountsLastSecond;
      R__b.WriteArray(kuPsdChanMap, 12);
      R__b << fhHitChargeMap;
      R__b << fhHitMapEvo;
      R__b << fhChanHitMapEvo;
      R__b << fhMissedData;
      R__b << fhAdcTime;
      R__b << fhMsLengthEvo;
      R__b << fhMsgsCntPerMsEvo;
      R__b << fhReadMsgsCntPerMsEvo;
      R__b << fhLostMsgsCntPerMsEvo;
      R__b << fhReadEvtsCntPerMsEvo;
      {
         vector<TH1*> &R__stl =  fvhHitFitWfmChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH1*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhFitHarmonic1Chan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhFitHarmonic2Chan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TH2*> &R__stl =  fvhFitQaChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TH2*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fcSummary;
      R__b << fcHitMaps;
      R__b << fcZLevo;
      R__b << fcChargesFPGA;
      R__b << fcChargesWfm;
      R__b << fcAmplitudes;
      R__b << fcZeroLevels;
      R__b << fcGenCntsPerMs;
      R__b << fcWfmsAllChannels;
      {
         vector<TCanvas*> &R__stl =  fvcWfmsChan;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TCanvas*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fcPronyFit;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorAlgoPsd(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorAlgoPsd : new ::CbmMcbm2018MonitorAlgoPsd;
   }
   static void *newArray_CbmMcbm2018MonitorAlgoPsd(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorAlgoPsd[nElements] : new ::CbmMcbm2018MonitorAlgoPsd[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorAlgoPsd(void *p) {
      delete ((::CbmMcbm2018MonitorAlgoPsd*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorAlgoPsd(void *p) {
      delete [] ((::CbmMcbm2018MonitorAlgoPsd*)p);
   }
   static void destruct_CbmMcbm2018MonitorAlgoPsd(void *p) {
      typedef ::CbmMcbm2018MonitorAlgoPsd current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorAlgoPsd(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorAlgoPsd*)obj)->::CbmMcbm2018MonitorAlgoPsd::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorAlgoPsd

//______________________________________________________________________________
void CbmMcbm2018MonitorTaskPsd::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MonitorTaskPsd.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fuHistoryHistoSize;
      {
         vector<Int_t> &R__stl =  fviHistoChargeArgs;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviHistoAmplArgs;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviHistoZLArgs;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      fsHistoFileName.Streamer(R__b);
      R__b >> fulTsCounter;
      R__b >> fMonitorAlgo;
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018MonitorTaskPsd::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018MonitorTaskPsd::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fuHistoryHistoSize;
      {
         vector<Int_t> &R__stl =  fviHistoChargeArgs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviHistoAmplArgs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviHistoZLArgs;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      fsHistoFileName.Streamer(R__b);
      R__b << fulTsCounter;
      R__b << fMonitorAlgo;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MonitorTaskPsd(void *p) {
      return  p ? new(p) ::CbmMcbm2018MonitorTaskPsd : new ::CbmMcbm2018MonitorTaskPsd;
   }
   static void *newArray_CbmMcbm2018MonitorTaskPsd(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MonitorTaskPsd[nElements] : new ::CbmMcbm2018MonitorTaskPsd[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MonitorTaskPsd(void *p) {
      delete ((::CbmMcbm2018MonitorTaskPsd*)p);
   }
   static void deleteArray_CbmMcbm2018MonitorTaskPsd(void *p) {
      delete [] ((::CbmMcbm2018MonitorTaskPsd*)p);
   }
   static void destruct_CbmMcbm2018MonitorTaskPsd(void *p) {
      typedef ::CbmMcbm2018MonitorTaskPsd current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018MonitorTaskPsd(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018MonitorTaskPsd*)obj)->::CbmMcbm2018MonitorTaskPsd::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018MonitorTaskPsd

//______________________________________________________________________________
void CbmCheckDigisNbCorr::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCheckDigisNbCorr.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      fTsMetaData->Streamer(R__b);
      R__b >> fuMinTotPulserT0;
      R__b >> fuMaxTotPulserT0;
      R__b >> fNrTs;
      R__b >> fNrOfT0Errors;
      R__b >> fNrOfT0Digis;
      R__b >> fNrOfStsErrors;
      R__b >> fNrOfStsDigis;
      R__b >> fNrOfMuchErrors;
      R__b >> fNrOfMuchDigis;
      R__b >> fNrOfTofErrors;
      R__b >> fNrOfTofDigis;
      R__b >> fNrOfRichErrors;
      R__b >> fNrOfRichDigis;
      R__b >> fdStsOffset;
      R__b >> fdMuchOffset;
      R__b >> fdTofOffset;
      R__b >> fdRichOffset;
      R__b >> fdTsLengthNs;
      R__b >> fdBinWidthNs;
      R__b >> fiBinNb;
      R__b >> fdTsStartOffset;
      {
         vector<UInt_t> &R__stl =  fvuNbDigisPerBinT0;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuNbDigisPerBinSts;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuNbDigisPerBinMuch;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuNbDigisPerBinTof;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuNbDigisPerBinRich;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fT0StsCorr;
      R__b >> fT0MuchCorr;
      R__b >> fT0TofCorr;
      R__b >> fT0RichCorr;
      R__b >> fStsMuchCorr;
      R__b >> fStsTofCorr;
      R__b >> fStsRichCorr;
      R__b >> fMuchTofCorr;
      R__b >> fMuchRichCorr;
      R__b >> fTofRichCorr;
      R__b >> fT0StsCorrProf;
      R__b >> fT0MuchCorrProf;
      R__b >> fT0TofCorrProf;
      R__b >> fT0RichCorrProf;
      R__b >> fStsMuchCorrProf;
      R__b >> fStsTofCorrProf;
      R__b >> fStsRichCorrProf;
      R__b >> fMuchTofCorrProf;
      R__b >> fMuchRichCorrProf;
      R__b >> fTofRichCorrProf;
      {
         vector<vector<UInt_t> > &R__stl =  fvuNbDigisPerBinStsDpb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<unsigned int, class std::allocator<unsigned int> >));
         if (R__tcl1==0) {
            Error("fvuNbDigisPerBinStsDpb streamer","Missing the TClass object for class std::vector<unsigned int, class std::allocator<unsigned int> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<unsigned int> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      int R__i;
      for (R__i = 0; R__i < 2; R__i++)
         R__b >> fT0StsDpbCorr[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b >> fStsMuchDpbCorr[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b >> fStsTofDpbCorr[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b >> fStsRichDpbCorr[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b >> fT0StsDpbCorrProf[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b >> fStsMuchDpbCorrProf[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b >> fStsTofDpbCorrProf[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b >> fStsRichDpbCorrProf[R__i];
      fOutFileName.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, CbmCheckDigisNbCorr::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmCheckDigisNbCorr::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      fTsMetaData->Streamer(R__b);
      R__b << fuMinTotPulserT0;
      R__b << fuMaxTotPulserT0;
      R__b << fNrTs;
      R__b << fNrOfT0Errors;
      R__b << fNrOfT0Digis;
      R__b << fNrOfStsErrors;
      R__b << fNrOfStsDigis;
      R__b << fNrOfMuchErrors;
      R__b << fNrOfMuchDigis;
      R__b << fNrOfTofErrors;
      R__b << fNrOfTofDigis;
      R__b << fNrOfRichErrors;
      R__b << fNrOfRichDigis;
      R__b << fdStsOffset;
      R__b << fdMuchOffset;
      R__b << fdTofOffset;
      R__b << fdRichOffset;
      R__b << fdTsLengthNs;
      R__b << fdBinWidthNs;
      R__b << fiBinNb;
      R__b << fdTsStartOffset;
      {
         vector<UInt_t> &R__stl =  fvuNbDigisPerBinT0;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuNbDigisPerBinSts;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuNbDigisPerBinMuch;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuNbDigisPerBinTof;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuNbDigisPerBinRich;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fT0StsCorr;
      R__b << fT0MuchCorr;
      R__b << fT0TofCorr;
      R__b << fT0RichCorr;
      R__b << fStsMuchCorr;
      R__b << fStsTofCorr;
      R__b << fStsRichCorr;
      R__b << fMuchTofCorr;
      R__b << fMuchRichCorr;
      R__b << fTofRichCorr;
      R__b << (TObject*)fT0StsCorrProf;
      R__b << (TObject*)fT0MuchCorrProf;
      R__b << (TObject*)fT0TofCorrProf;
      R__b << (TObject*)fT0RichCorrProf;
      R__b << (TObject*)fStsMuchCorrProf;
      R__b << (TObject*)fStsTofCorrProf;
      R__b << (TObject*)fStsRichCorrProf;
      R__b << (TObject*)fMuchTofCorrProf;
      R__b << (TObject*)fMuchRichCorrProf;
      R__b << (TObject*)fTofRichCorrProf;
      {
         vector<vector<UInt_t> > &R__stl =  fvuNbDigisPerBinStsDpb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::vector<unsigned int, class std::allocator<unsigned int> >));
         if (R__tcl1==0) {
            Error("fvuNbDigisPerBinStsDpb streamer","Missing the TClass object for class std::vector<unsigned int, class std::allocator<unsigned int> >!");
            return;
         }
            vector<vector<UInt_t> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<unsigned int>*)&(*R__k),R__tcl1);
            }
         }
      }
      int R__i;
      for (R__i = 0; R__i < 2; R__i++)
         R__b << fT0StsDpbCorr[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b << fStsMuchDpbCorr[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b << fStsTofDpbCorr[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b << fStsRichDpbCorr[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b << (TObject*)fT0StsDpbCorrProf[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b << (TObject*)fStsMuchDpbCorrProf[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b << (TObject*)fStsTofDpbCorrProf[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b << (TObject*)fStsRichDpbCorrProf[R__i];
      fOutFileName.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmCheckDigisNbCorr(void *p) {
      return  p ? new(p) ::CbmCheckDigisNbCorr : new ::CbmCheckDigisNbCorr;
   }
   static void *newArray_CbmCheckDigisNbCorr(Long_t nElements, void *p) {
      return p ? new(p) ::CbmCheckDigisNbCorr[nElements] : new ::CbmCheckDigisNbCorr[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmCheckDigisNbCorr(void *p) {
      delete ((::CbmCheckDigisNbCorr*)p);
   }
   static void deleteArray_CbmCheckDigisNbCorr(void *p) {
      delete [] ((::CbmCheckDigisNbCorr*)p);
   }
   static void destruct_CbmCheckDigisNbCorr(void *p) {
      typedef ::CbmCheckDigisNbCorr current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmCheckDigisNbCorr(TBuffer &buf, void *obj) {
      ((::CbmCheckDigisNbCorr*)obj)->::CbmCheckDigisNbCorr::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmCheckDigisNbCorr

//______________________________________________________________________________
void CbmCheckTiming::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCheckTiming.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> trd_peak_pos;
      R__b >> sts_peak_pos;
      R__b >> much_peak_pos;
      R__b >> tof_peak_pos;
      R__b >> rich_peak_pos;
      R__b >> psd_peak_pos;
      R__b >> trd_coin_peak_pos;
      R__b >> sts_coin_peak_pos;
      R__b >> much_coin_peak_pos;
      R__b >> rich_coin_peak_pos;
      R__b >> psd_coin_peak_pos;
      R__b >> trd_average;
      R__b >> sts_average;
      R__b >> much_average;
      R__b >> tof_average;
      R__b >> rich_average;
      R__b >> psd_average;
      R__b >> trd_width0_ns;
      R__b >> sts_width0_ns;
      R__b >> much_width0_ns;
      R__b >> tof_width0_ns;
      R__b >> rich_width0_ns;
      R__b >> psd_width0_ns;
      R__b >> fPrevTimeT0;
      R__b >> fPrevTimeSts;
      R__b >> fPrevTimeMuch;
      R__b >> fPrevTimeTrd;
      R__b >> fPrevTimeTof;
      R__b >> fPrevTimeRich;
      R__b >> fPrevTimePsd;
      R__b >> fPrevT0FirstDigiSts;
      R__b >> fPrevT0FirstDigiMuch;
      R__b >> fPrevT0FirstDigiTrd;
      R__b >> fPrevT0FirstDigiTof;
      R__b >> fPrevT0FirstDigiRich;
      R__b >> fPrevT0FirstDigiPsd;
      R__b >> fuMinTotPulserT0;
      R__b >> fuMaxTotPulserT0;
      R__b >> fuMinTotPulserTof;
      R__b >> fuMaxTotPulserTof;
      R__b >> fNrTs;
      R__b >> fNrOfT0Errors;
      R__b >> fNrOfT0Digis;
      R__b >> fNrOfStsErrors;
      R__b >> fNrOfStsDigis;
      R__b >> fNrOfMuchErrors;
      R__b >> fNrOfMuchDigis;
      R__b >> fNrOfTrdErrors;
      R__b >> fNrOfTrdDigis;
      R__b >> fNrOfTofErrors;
      R__b >> fNrOfTofDigis;
      R__b >> fNrOfRichErrors;
      R__b >> fNrOfRichDigis;
      R__b >> fNrOfPsdErrors;
      R__b >> fNrOfPsdDigis;
      R__b >> fCheckTimeOrdering;
      R__b >> fCheckInterSystemOffset;
      R__b >> fOffsetRange;
      R__b >> fStsOffsetRange;
      R__b >> fMuchOffsetRange;
      R__b >> fTrdOffsetRange;
      R__b >> fTofOffsetRange;
      R__b >> fRichOffsetRange;
      R__b >> fPsdOffsetRange;
      R__b >> fNrTsForFit;
      R__b >> fTrdPeakWidthNs;
      R__b >> fStsPeakWidthNs;
      R__b >> fMuchPeakWidthNs;
      R__b >> fTofPeakWidthNs;
      R__b >> fRichPeakWidthNs;
      R__b >> fPsdPeakWidthNs;
      R__b >> fBinWidth;
      R__b >> fT0StsDiff;
      R__b >> fT0MuchDiff;
      R__b >> fT0TrdDiff;
      R__b >> fT0TofDiff;
      R__b >> fT0RichDiff;
      R__b >> fT0PsdDiff;
      R__b >> fT0StsDiffCharge;
      R__b >> fT0MuchDiffCharge;
      R__b >> fT0TrdDiffCharge;
      R__b >> fT0TofDiffCharge;
      R__b >> fT0RichDiffCharge;
      R__b >> fT0PsdDiffCharge;
      R__b >> fT0StsDiffEvo;
      R__b >> fT0MuchDiffEvo;
      R__b >> fT0TrdDiffEvo;
      R__b >> fT0TofDiffEvo;
      R__b >> fT0RichDiffEvo;
      R__b >> fT0PsdDiffEvo;
      R__b >> fT0StsDiffEvoLong;
      R__b >> fT0MuchDiffEvoLong;
      R__b >> fT0TrdDiffEvoLong;
      R__b >> fT0TofDiffEvoLong;
      R__b >> fT0RichDiffEvoLong;
      R__b >> fT0PsdDiffEvoLong;
      R__b >> fT0T0Diff;
      R__b >> fStsStsDiff;
      R__b >> fMuchMuchDiff;
      R__b >> fTrdTrdDiff;
      R__b >> fTofTofDiff;
      R__b >> fRichRichDiff;
      R__b >> fPsdPsdDiff;
      R__b >> fT0Address;
      R__b >> fT0Channel;
      R__b >> fT0StsDpbDiff;
      int R__i;
      for (R__i = 0; R__i < 2; R__i++)
         R__b >> fT0StsDpbDiffEvo[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b >> fStsDpbCntsEvo[R__i];
      R__b >> fT0MuchRocDiff;
      R__b >> fT0MuchAsicDiff;
      for (R__i = 0; R__i < 36; R__i++)
         R__b >> fT0MuchAsicDiffEvo[R__i];
      R__b >> fuNbTofDigisSel;
      R__b >> fuNbTofDigiInSync;
      R__b >> fSelT0StsDiff;
      R__b >> fSelT0MuchDiff;
      R__b >> fSelT0TrdDiff;
      R__b >> fSelT0TofDiff;
      R__b >> fSelT0RichDiff;
      R__b >> fSelT0PsdDiff;
      fOutFileName.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, CbmCheckTiming::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmCheckTiming::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << trd_peak_pos;
      R__b << sts_peak_pos;
      R__b << much_peak_pos;
      R__b << tof_peak_pos;
      R__b << rich_peak_pos;
      R__b << psd_peak_pos;
      R__b << trd_coin_peak_pos;
      R__b << sts_coin_peak_pos;
      R__b << much_coin_peak_pos;
      R__b << rich_coin_peak_pos;
      R__b << psd_coin_peak_pos;
      R__b << trd_average;
      R__b << sts_average;
      R__b << much_average;
      R__b << tof_average;
      R__b << rich_average;
      R__b << psd_average;
      R__b << trd_width0_ns;
      R__b << sts_width0_ns;
      R__b << much_width0_ns;
      R__b << tof_width0_ns;
      R__b << rich_width0_ns;
      R__b << psd_width0_ns;
      R__b << fPrevTimeT0;
      R__b << fPrevTimeSts;
      R__b << fPrevTimeMuch;
      R__b << fPrevTimeTrd;
      R__b << fPrevTimeTof;
      R__b << fPrevTimeRich;
      R__b << fPrevTimePsd;
      R__b << fPrevT0FirstDigiSts;
      R__b << fPrevT0FirstDigiMuch;
      R__b << fPrevT0FirstDigiTrd;
      R__b << fPrevT0FirstDigiTof;
      R__b << fPrevT0FirstDigiRich;
      R__b << fPrevT0FirstDigiPsd;
      R__b << fuMinTotPulserT0;
      R__b << fuMaxTotPulserT0;
      R__b << fuMinTotPulserTof;
      R__b << fuMaxTotPulserTof;
      R__b << fNrTs;
      R__b << fNrOfT0Errors;
      R__b << fNrOfT0Digis;
      R__b << fNrOfStsErrors;
      R__b << fNrOfStsDigis;
      R__b << fNrOfMuchErrors;
      R__b << fNrOfMuchDigis;
      R__b << fNrOfTrdErrors;
      R__b << fNrOfTrdDigis;
      R__b << fNrOfTofErrors;
      R__b << fNrOfTofDigis;
      R__b << fNrOfRichErrors;
      R__b << fNrOfRichDigis;
      R__b << fNrOfPsdErrors;
      R__b << fNrOfPsdDigis;
      R__b << fCheckTimeOrdering;
      R__b << fCheckInterSystemOffset;
      R__b << fOffsetRange;
      R__b << fStsOffsetRange;
      R__b << fMuchOffsetRange;
      R__b << fTrdOffsetRange;
      R__b << fTofOffsetRange;
      R__b << fRichOffsetRange;
      R__b << fPsdOffsetRange;
      R__b << fNrTsForFit;
      R__b << fTrdPeakWidthNs;
      R__b << fStsPeakWidthNs;
      R__b << fMuchPeakWidthNs;
      R__b << fTofPeakWidthNs;
      R__b << fRichPeakWidthNs;
      R__b << fPsdPeakWidthNs;
      R__b << fBinWidth;
      R__b << fT0StsDiff;
      R__b << fT0MuchDiff;
      R__b << fT0TrdDiff;
      R__b << fT0TofDiff;
      R__b << fT0RichDiff;
      R__b << fT0PsdDiff;
      R__b << fT0StsDiffCharge;
      R__b << fT0MuchDiffCharge;
      R__b << fT0TrdDiffCharge;
      R__b << fT0TofDiffCharge;
      R__b << fT0RichDiffCharge;
      R__b << fT0PsdDiffCharge;
      R__b << fT0StsDiffEvo;
      R__b << fT0MuchDiffEvo;
      R__b << fT0TrdDiffEvo;
      R__b << fT0TofDiffEvo;
      R__b << fT0RichDiffEvo;
      R__b << fT0PsdDiffEvo;
      R__b << fT0StsDiffEvoLong;
      R__b << fT0MuchDiffEvoLong;
      R__b << fT0TrdDiffEvoLong;
      R__b << fT0TofDiffEvoLong;
      R__b << fT0RichDiffEvoLong;
      R__b << fT0PsdDiffEvoLong;
      R__b << fT0T0Diff;
      R__b << fStsStsDiff;
      R__b << fMuchMuchDiff;
      R__b << fTrdTrdDiff;
      R__b << fTofTofDiff;
      R__b << fRichRichDiff;
      R__b << fPsdPsdDiff;
      R__b << fT0Address;
      R__b << fT0Channel;
      R__b << fT0StsDpbDiff;
      int R__i;
      for (R__i = 0; R__i < 2; R__i++)
         R__b << fT0StsDpbDiffEvo[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b << fStsDpbCntsEvo[R__i];
      R__b << fT0MuchRocDiff;
      R__b << fT0MuchAsicDiff;
      for (R__i = 0; R__i < 36; R__i++)
         R__b << fT0MuchAsicDiffEvo[R__i];
      R__b << fuNbTofDigisSel;
      R__b << fuNbTofDigiInSync;
      R__b << fSelT0StsDiff;
      R__b << fSelT0MuchDiff;
      R__b << fSelT0TrdDiff;
      R__b << fSelT0TofDiff;
      R__b << fSelT0RichDiff;
      R__b << fSelT0PsdDiff;
      fOutFileName.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmCheckTiming(void *p) {
      return  p ? new(p) ::CbmCheckTiming : new ::CbmCheckTiming;
   }
   static void *newArray_CbmCheckTiming(Long_t nElements, void *p) {
      return p ? new(p) ::CbmCheckTiming[nElements] : new ::CbmCheckTiming[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmCheckTiming(void *p) {
      delete ((::CbmCheckTiming*)p);
   }
   static void deleteArray_CbmCheckTiming(void *p) {
      delete [] ((::CbmCheckTiming*)p);
   }
   static void destruct_CbmCheckTiming(void *p) {
      typedef ::CbmCheckTiming current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmCheckTiming(TBuffer &buf, void *obj) {
      ((::CbmCheckTiming*)obj)->::CbmCheckTiming::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmCheckTiming

//______________________________________________________________________________
void CbmCheckEvents::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCheckEvents.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      fEvents->Streamer(R__b);
      R__b >> fPrevEventTime;
      R__b >> fNrTs;
      R__b >> fEventSize;
      R__b >> fEventLength;
      R__b >> fEventsPerTS;
      R__b >> fT0InEvent;
      R__b >> fStsInEvent;
      R__b >> fMuchInEvent;
      R__b >> fTofInEvent;
      R__b >> fT0DeltaT;
      R__b >> fStsDeltaT;
      R__b >> fMuchDeltaT;
      R__b >> fTofDeltaT;
      R__b >> fEventsvsTS;
      R__b >> fLengthvsTS;
      R__b.CheckByteCount(R__s, R__c, CbmCheckEvents::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmCheckEvents::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      fEvents->Streamer(R__b);
      R__b << fPrevEventTime;
      R__b << fNrTs;
      R__b << fEventSize;
      R__b << fEventLength;
      R__b << fEventsPerTS;
      R__b << fT0InEvent;
      R__b << fStsInEvent;
      R__b << fMuchInEvent;
      R__b << fTofInEvent;
      R__b << fT0DeltaT;
      R__b << fStsDeltaT;
      R__b << fMuchDeltaT;
      R__b << fTofDeltaT;
      R__b << fEventsvsTS;
      R__b << (TObject*)fLengthvsTS;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmCheckEvents(void *p) {
      return  p ? new(p) ::CbmCheckEvents : new ::CbmCheckEvents;
   }
   static void *newArray_CbmCheckEvents(Long_t nElements, void *p) {
      return p ? new(p) ::CbmCheckEvents[nElements] : new ::CbmCheckEvents[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmCheckEvents(void *p) {
      delete ((::CbmCheckEvents*)p);
   }
   static void deleteArray_CbmCheckEvents(void *p) {
      delete [] ((::CbmCheckEvents*)p);
   }
   static void destruct_CbmCheckEvents(void *p) {
      typedef ::CbmCheckEvents current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmCheckEvents(TBuffer &buf, void *obj) {
      ((::CbmCheckEvents*)obj)->::CbmCheckEvents::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmCheckEvents

//______________________________________________________________________________
void CbmMcbm2018EventBuilder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018EventBuilder.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> fbUseBaseMuchDigi;
      void *ptr_fEventBuilderAlgo = (void*)&fEventBuilderAlgo;
      R__b >> *reinterpret_cast<Int_t*>(ptr_fEventBuilderAlgo);
      R__b >> fFixedTimeWindow;
      R__b >> fStartTimeEvent;
      R__b >> fMaximumTimeGap;
      R__b >> fTriggerMinT0Digis;
      R__b >> fTriggerMinStsDigis;
      R__b >> fTriggerMinMuchDigis;
      R__b >> fTriggerMinTrdDigis;
      R__b >> fTriggerMinTofDigis;
      R__b >> fTriggerMinRichDigis;
      R__b >> fTriggerMinPsdDigis;
      R__b >> fTriggerMaxT0Digis;
      R__b >> fTriggerMaxStsDigis;
      R__b >> fTriggerMaxMuchDigis;
      R__b >> fTriggerMaxTrdDigis;
      R__b >> fTriggerMaxTofDigis;
      R__b >> fTriggerMaxRichDigis;
      R__b >> fTriggerMaxPsdDigis;
      fOutFileName.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2018EventBuilder::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2018EventBuilder::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << fbUseBaseMuchDigi;
      R__b << (Int_t)fEventBuilderAlgo;
      R__b << fFixedTimeWindow;
      R__b << fStartTimeEvent;
      R__b << fMaximumTimeGap;
      R__b << fTriggerMinT0Digis;
      R__b << fTriggerMinStsDigis;
      R__b << fTriggerMinMuchDigis;
      R__b << fTriggerMinTrdDigis;
      R__b << fTriggerMinTofDigis;
      R__b << fTriggerMinRichDigis;
      R__b << fTriggerMinPsdDigis;
      R__b << fTriggerMaxT0Digis;
      R__b << fTriggerMaxStsDigis;
      R__b << fTriggerMaxMuchDigis;
      R__b << fTriggerMaxTrdDigis;
      R__b << fTriggerMaxTofDigis;
      R__b << fTriggerMaxRichDigis;
      R__b << fTriggerMaxPsdDigis;
      fOutFileName.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018EventBuilder(void *p) {
      return  p ? new(p) ::CbmMcbm2018EventBuilder : new ::CbmMcbm2018EventBuilder;
   }
   static void *newArray_CbmMcbm2018EventBuilder(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018EventBuilder[nElements] : new ::CbmMcbm2018EventBuilder[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018EventBuilder(void *p) {
      delete ((::CbmMcbm2018EventBuilder*)p);
   }
   static void deleteArray_CbmMcbm2018EventBuilder(void *p) {
      delete [] ((::CbmMcbm2018EventBuilder*)p);
   }
   static void destruct_CbmMcbm2018EventBuilder(void *p) {
      typedef ::CbmMcbm2018EventBuilder current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2018EventBuilder(TBuffer &buf, void *obj) {
      ((::CbmMcbm2018EventBuilder*)obj)->::CbmMcbm2018EventBuilder::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2018EventBuilder

//______________________________________________________________________________
void CbmMcbm2019CheckPulser::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2019CheckPulser.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> pTsMetaData;
      R__b >> fPrevTimeT0;
      R__b >> fPrevTimeSts;
      R__b >> fPrevTimeMuch;
      R__b >> fPrevTimeTrd;
      R__b >> fPrevTimeTof;
      R__b >> fPrevTimeRich;
      R__b >> fPrevTimePsd;
      R__b >> fPrevT0FirstDigiSts;
      R__b >> fPrevT0FirstDigiMuch;
      R__b >> fPrevT0FirstDigiTrd;
      R__b >> fPrevT0FirstDigiTof;
      R__b >> fPrevT0FirstDigiRich;
      R__b >> fPrevT0FirstDigiPsd;
      R__b >> fuMinTotPulserT0;
      R__b >> fuMaxTotPulserT0;
      R__b >> fuMinAdcPulserSts;
      R__b >> fuMaxAdcPulserSts;
      R__b >> fuMinAdcPulserMuch;
      R__b >> fuMaxAdcPulserMuch;
      R__b >> fuMinChargePulserTrd;
      R__b >> fuMaxChargePulserTrd;
      R__b >> fuMinTotPulserTof;
      R__b >> fuMaxTotPulserTof;
      R__b >> fuMinTotPulserRich;
      R__b >> fuMaxTotPulserRich;
      R__b >> fuMinAdcPulserPsd;
      R__b >> fuMaxAdcPulserPsd;
      R__b >> fuStsAddress;
      R__b >> fuStsFirstCha;
      R__b >> fuStsLastChan;
      R__b >> fuMuchAsic;
      R__b >> fuMuchFirstCha;
      R__b >> fuMuchLastChan;
      R__b >> fuTrdAddress;
      R__b >> fuPsdAddress;
      R__b >> fNrTs;
      R__b >> fOffsetRange;
      R__b >> fStsOffsetRange;
      R__b >> fMuchOffsetRange;
      R__b >> fTrdOffsetRange;
      R__b >> fTofOffsetRange;
      R__b >> fRichOffsetRange;
      R__b >> fPsdOffsetRange;
      R__b >> fBinWidth;
      R__b >> fT0StsDiff;
      R__b >> fT0MuchDiff;
      R__b >> fT0TrdDiff;
      R__b >> fT0TofDiff;
      R__b >> fT0RichDiff;
      R__b >> fT0PsdDiff;
      R__b >> fT0PsdDiffCharge;
      R__b >> fT0StsDiffEvo;
      R__b >> fT0MuchDiffEvo;
      R__b >> fT0TrdDiffEvo;
      R__b >> fT0TofDiffEvo;
      R__b >> fT0RichDiffEvo;
      R__b >> fT0PsdDiffEvo;
      R__b >> fT0StsDiffEvoLong;
      R__b >> fT0MuchDiffEvoLong;
      R__b >> fT0TrdDiffEvoLong;
      R__b >> fT0TofDiffEvoLong;
      R__b >> fT0RichDiffEvoLong;
      R__b >> fT0PsdDiffEvoLong;
      R__b >> fdStartTime;
      R__b >> fT0StsMeanEvo;
      R__b >> fT0MuchMeanEvo;
      R__b >> fT0TrdMeanEvo;
      R__b >> fT0TofMeanEvo;
      R__b >> fT0RichMeanEvo;
      R__b >> fT0PsdMeanEvo;
      R__b >> fT0T0Diff;
      R__b >> fStsStsDiff;
      R__b >> fMuchMuchDiff;
      R__b >> fTrdTrdDiff;
      R__b >> fTofTofDiff;
      R__b >> fRichRichDiff;
      R__b >> fPsdPsdDiff;
      R__b >> fT0StsNb;
      R__b >> fT0MuchNb;
      R__b >> fT0TrdNb;
      R__b >> fT0TofNb;
      R__b >> fT0RichNb;
      R__b >> fT0PsdNb;
      R__b >> fiT0Nb;
      R__b >> fiStsNb;
      R__b >> fiMuchNb;
      R__b >> fiTrdNb;
      R__b >> fiTofNb;
      R__b >> fiRichNb;
      R__b >> fiPsdNb;
      R__b >> fT0Address;
      R__b >> fT0Channel;
      R__b >> fT0StsDpbDiff;
      int R__i;
      for (R__i = 0; R__i < 2; R__i++)
         R__b >> fT0StsDpbDiffEvo[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b >> fStsDpbCntsEvo[R__i];
      R__b >> fT0MuchRocDiff;
      R__b >> fT0MuchAsicDiff;
      for (R__i = 0; R__i < 36; R__i++)
         R__b >> fT0MuchAsicDiffEvo[R__i];
      R__b >> fDigisPerAsicEvo;
      R__b.ReadStaticArray((double*)fdLastMuchDigi);
      R__b.ReadStaticArray((double*)fdLastMuchDigiPulser);
      R__b >> fSameChanDigisDistEvo;
      R__b >> fdLastT0DigiPulser;
      R__b >> fDigiTimeEvoT0;
      R__b >> fDigiTimeEvoSts;
      R__b >> fDigiTimeEvoMuch;
      R__b >> fDigiTimeEvoTof;
      fOutFileName.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2019CheckPulser::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2019CheckPulser::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << pTsMetaData;
      R__b << fPrevTimeT0;
      R__b << fPrevTimeSts;
      R__b << fPrevTimeMuch;
      R__b << fPrevTimeTrd;
      R__b << fPrevTimeTof;
      R__b << fPrevTimeRich;
      R__b << fPrevTimePsd;
      R__b << fPrevT0FirstDigiSts;
      R__b << fPrevT0FirstDigiMuch;
      R__b << fPrevT0FirstDigiTrd;
      R__b << fPrevT0FirstDigiTof;
      R__b << fPrevT0FirstDigiRich;
      R__b << fPrevT0FirstDigiPsd;
      R__b << fuMinTotPulserT0;
      R__b << fuMaxTotPulserT0;
      R__b << fuMinAdcPulserSts;
      R__b << fuMaxAdcPulserSts;
      R__b << fuMinAdcPulserMuch;
      R__b << fuMaxAdcPulserMuch;
      R__b << fuMinChargePulserTrd;
      R__b << fuMaxChargePulserTrd;
      R__b << fuMinTotPulserTof;
      R__b << fuMaxTotPulserTof;
      R__b << fuMinTotPulserRich;
      R__b << fuMaxTotPulserRich;
      R__b << fuMinAdcPulserPsd;
      R__b << fuMaxAdcPulserPsd;
      R__b << fuStsAddress;
      R__b << fuStsFirstCha;
      R__b << fuStsLastChan;
      R__b << fuMuchAsic;
      R__b << fuMuchFirstCha;
      R__b << fuMuchLastChan;
      R__b << fuTrdAddress;
      R__b << fuPsdAddress;
      R__b << fNrTs;
      R__b << fOffsetRange;
      R__b << fStsOffsetRange;
      R__b << fMuchOffsetRange;
      R__b << fTrdOffsetRange;
      R__b << fTofOffsetRange;
      R__b << fRichOffsetRange;
      R__b << fPsdOffsetRange;
      R__b << fBinWidth;
      R__b << fT0StsDiff;
      R__b << fT0MuchDiff;
      R__b << fT0TrdDiff;
      R__b << fT0TofDiff;
      R__b << fT0RichDiff;
      R__b << fT0PsdDiff;
      R__b << fT0PsdDiffCharge;
      R__b << fT0StsDiffEvo;
      R__b << fT0MuchDiffEvo;
      R__b << fT0TrdDiffEvo;
      R__b << fT0TofDiffEvo;
      R__b << fT0RichDiffEvo;
      R__b << fT0PsdDiffEvo;
      R__b << fT0StsDiffEvoLong;
      R__b << fT0MuchDiffEvoLong;
      R__b << fT0TrdDiffEvoLong;
      R__b << fT0TofDiffEvoLong;
      R__b << fT0RichDiffEvoLong;
      R__b << fT0PsdDiffEvoLong;
      R__b << fdStartTime;
      R__b << (TObject*)fT0StsMeanEvo;
      R__b << (TObject*)fT0MuchMeanEvo;
      R__b << (TObject*)fT0TrdMeanEvo;
      R__b << (TObject*)fT0TofMeanEvo;
      R__b << (TObject*)fT0RichMeanEvo;
      R__b << (TObject*)fT0PsdMeanEvo;
      R__b << fT0T0Diff;
      R__b << fStsStsDiff;
      R__b << fMuchMuchDiff;
      R__b << fTrdTrdDiff;
      R__b << fTofTofDiff;
      R__b << fRichRichDiff;
      R__b << fPsdPsdDiff;
      R__b << fT0StsNb;
      R__b << fT0MuchNb;
      R__b << fT0TrdNb;
      R__b << fT0TofNb;
      R__b << fT0RichNb;
      R__b << fT0PsdNb;
      R__b << fiT0Nb;
      R__b << fiStsNb;
      R__b << fiMuchNb;
      R__b << fiTrdNb;
      R__b << fiTofNb;
      R__b << fiRichNb;
      R__b << fiPsdNb;
      R__b << fT0Address;
      R__b << fT0Channel;
      R__b << fT0StsDpbDiff;
      int R__i;
      for (R__i = 0; R__i < 2; R__i++)
         R__b << fT0StsDpbDiffEvo[R__i];
      for (R__i = 0; R__i < 2; R__i++)
         R__b << fStsDpbCntsEvo[R__i];
      R__b << fT0MuchRocDiff;
      R__b << fT0MuchAsicDiff;
      for (R__i = 0; R__i < 36; R__i++)
         R__b << fT0MuchAsicDiffEvo[R__i];
      R__b << fDigisPerAsicEvo;
      R__b.WriteArray((double*)fdLastMuchDigi, 4608);
      R__b.WriteArray((double*)fdLastMuchDigiPulser, 4608);
      R__b << fSameChanDigisDistEvo;
      R__b << fdLastT0DigiPulser;
      R__b << fDigiTimeEvoT0;
      R__b << fDigiTimeEvoSts;
      R__b << fDigiTimeEvoMuch;
      R__b << fDigiTimeEvoTof;
      fOutFileName.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2019CheckPulser(void *p) {
      return  p ? new(p) ::CbmMcbm2019CheckPulser : new ::CbmMcbm2019CheckPulser;
   }
   static void *newArray_CbmMcbm2019CheckPulser(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2019CheckPulser[nElements] : new ::CbmMcbm2019CheckPulser[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2019CheckPulser(void *p) {
      delete ((::CbmMcbm2019CheckPulser*)p);
   }
   static void deleteArray_CbmMcbm2019CheckPulser(void *p) {
      delete [] ((::CbmMcbm2019CheckPulser*)p);
   }
   static void destruct_CbmMcbm2019CheckPulser(void *p) {
      typedef ::CbmMcbm2019CheckPulser current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2019CheckPulser(TBuffer &buf, void *obj) {
      ((::CbmMcbm2019CheckPulser*)obj)->::CbmMcbm2019CheckPulser::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2019CheckPulser

//______________________________________________________________________________
void CbmMcbm2019CheckDigisMuch::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2019CheckDigisMuch.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> fNrTs;
      R__b >> fSameChanDigisDistEvo;
      R__b >> fuTsJump;
      R__b >> fdFirstTsOffs;
      R__b >> fuStartTs;
      R__b >> fuStopTs;
      R__b >> fdTsLength;
      R__b >> fdStartTime;
      R__b >> fdDigiDistStart;
      R__b >> fdDigiDistStop;
      R__b >> fuMinAdcPulserMuch;
      R__b >> fuMaxAdcPulserMuch;
      R__b >> fDigisPerAsicEvo;
      R__b.ReadStaticArray((double*)fdLastMuchDigi);
      R__b.ReadStaticArray((double*)fdLastMuchDigiPulser);
      R__b >> fDigisPerChanEvo;
      fOutFileName.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2019CheckDigisMuch::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2019CheckDigisMuch::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << fNrTs;
      R__b << fSameChanDigisDistEvo;
      R__b << fuTsJump;
      R__b << fdFirstTsOffs;
      R__b << fuStartTs;
      R__b << fuStopTs;
      R__b << fdTsLength;
      R__b << fdStartTime;
      R__b << fdDigiDistStart;
      R__b << fdDigiDistStop;
      R__b << fuMinAdcPulserMuch;
      R__b << fuMaxAdcPulserMuch;
      R__b << fDigisPerAsicEvo;
      R__b.WriteArray((double*)fdLastMuchDigi, 4608);
      R__b.WriteArray((double*)fdLastMuchDigiPulser, 4608);
      R__b << fDigisPerChanEvo;
      fOutFileName.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2019CheckDigisMuch(void *p) {
      return  p ? new(p) ::CbmMcbm2019CheckDigisMuch : new ::CbmMcbm2019CheckDigisMuch;
   }
   static void *newArray_CbmMcbm2019CheckDigisMuch(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2019CheckDigisMuch[nElements] : new ::CbmMcbm2019CheckDigisMuch[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2019CheckDigisMuch(void *p) {
      delete ((::CbmMcbm2019CheckDigisMuch*)p);
   }
   static void deleteArray_CbmMcbm2019CheckDigisMuch(void *p) {
      delete [] ((::CbmMcbm2019CheckDigisMuch*)p);
   }
   static void destruct_CbmMcbm2019CheckDigisMuch(void *p) {
      typedef ::CbmMcbm2019CheckDigisMuch current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2019CheckDigisMuch(TBuffer &buf, void *obj) {
      ((::CbmMcbm2019CheckDigisMuch*)obj)->::CbmMcbm2019CheckDigisMuch::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2019CheckDigisMuch

//______________________________________________________________________________
void CbmMcbm2019CheckDigisSts::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2019CheckDigisSts.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> fNrTs;
      R__b >> fSameChanDigisDistEvo;
      R__b >> fuTsJump;
      R__b >> fdFirstTsOffs;
      R__b >> fuStartTs;
      R__b >> fuStopTs;
      R__b >> fdTsLength;
      R__b >> fdStartTime;
      R__b >> fdDigiDistStart;
      R__b >> fdDigiDistStop;
      R__b >> fuMinAdcPulserSts;
      R__b >> fuMaxAdcPulserSts;
      R__b >> fDigisPerAsicEvo;
      R__b.ReadStaticArray((double*)fdLastStsDigi);
      R__b.ReadStaticArray((double*)fdLastStsDigiPulser);
      fOutFileName.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2019CheckDigisSts::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2019CheckDigisSts::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << fNrTs;
      R__b << fSameChanDigisDistEvo;
      R__b << fuTsJump;
      R__b << fdFirstTsOffs;
      R__b << fuStartTs;
      R__b << fuStopTs;
      R__b << fdTsLength;
      R__b << fdStartTime;
      R__b << fdDigiDistStart;
      R__b << fdDigiDistStop;
      R__b << fuMinAdcPulserSts;
      R__b << fuMaxAdcPulserSts;
      R__b << fDigisPerAsicEvo;
      R__b.WriteArray((double*)fdLastStsDigi, 2048);
      R__b.WriteArray((double*)fdLastStsDigiPulser, 2048);
      fOutFileName.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2019CheckDigisSts(void *p) {
      return  p ? new(p) ::CbmMcbm2019CheckDigisSts : new ::CbmMcbm2019CheckDigisSts;
   }
   static void *newArray_CbmMcbm2019CheckDigisSts(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2019CheckDigisSts[nElements] : new ::CbmMcbm2019CheckDigisSts[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2019CheckDigisSts(void *p) {
      delete ((::CbmMcbm2019CheckDigisSts*)p);
   }
   static void deleteArray_CbmMcbm2019CheckDigisSts(void *p) {
      delete [] ((::CbmMcbm2019CheckDigisSts*)p);
   }
   static void destruct_CbmMcbm2019CheckDigisSts(void *p) {
      typedef ::CbmMcbm2019CheckDigisSts current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2019CheckDigisSts(TBuffer &buf, void *obj) {
      ((::CbmMcbm2019CheckDigisSts*)obj)->::CbmMcbm2019CheckDigisSts::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2019CheckDigisSts

//______________________________________________________________________________
void CbmMcbm2019CheckDtInDet::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2019CheckDtInDet.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> pTsMetaData;
      R__b >> fPrevTimeT0;
      R__b >> fPrevTimeSts;
      R__b >> fPrevTimeMuch;
      R__b >> fPrevTimeTrd;
      R__b >> fPrevTimeTof;
      R__b >> fPrevTimeRich;
      R__b >> fPrevTimePsd;
      R__b >> fuNbChanT0;
      R__b >> fuNbChanSts;
      R__b >> fuNbChanMuch;
      R__b >> fuNbChanTrd;
      R__b >> fuNbChanTof;
      R__b >> fuNbChanRich;
      R__b >> fuNbChanPsd;
      R__b >> fNrTs;
      R__b >> fT0T0SameTime;
      R__b >> fStsStsSameTime;
      R__b >> fMuchMuchSameTime;
      R__b >> fTrdTrdSameTime;
      R__b >> fTofTofSameTime;
      R__b >> fRichRichSameTime;
      R__b >> fPsdPsdSameTime;
      R__b >> fT0T0Diff;
      R__b >> fStsStsDiff;
      R__b >> fMuchMuchDiff;
      R__b >> fTrdTrdDiff;
      R__b >> fTofTofDiff;
      R__b >> fRichRichDiff;
      R__b >> fPsdPsdDiff;
      R__b >> fT0T0DiffLog;
      R__b >> fStsStsDiffLog;
      R__b >> fMuchMuchDiffLog;
      R__b >> fTrdTrdDiffLog;
      R__b >> fTofTofDiffLog;
      R__b >> fRichRichDiffLog;
      R__b >> fPsdPsdDiffLog;
      R__b >> fT0T0DiffPerChan;
      R__b >> fStsStsDiffPerChan;
      R__b >> fMuchMuchDiffPerChan;
      R__b >> fTrdTrdDiffPerChan;
      R__b >> fTofTofDiffPerChan;
      R__b >> fRichRichDiffPerChan;
      R__b >> fPsdPsdDiffPerChan;
      fOutFileName.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, CbmMcbm2019CheckDtInDet::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMcbm2019CheckDtInDet::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << pTsMetaData;
      R__b << fPrevTimeT0;
      R__b << fPrevTimeSts;
      R__b << fPrevTimeMuch;
      R__b << fPrevTimeTrd;
      R__b << fPrevTimeTof;
      R__b << fPrevTimeRich;
      R__b << fPrevTimePsd;
      R__b << fuNbChanT0;
      R__b << fuNbChanSts;
      R__b << fuNbChanMuch;
      R__b << fuNbChanTrd;
      R__b << fuNbChanTof;
      R__b << fuNbChanRich;
      R__b << fuNbChanPsd;
      R__b << fNrTs;
      R__b << fT0T0SameTime;
      R__b << fStsStsSameTime;
      R__b << fMuchMuchSameTime;
      R__b << fTrdTrdSameTime;
      R__b << fTofTofSameTime;
      R__b << fRichRichSameTime;
      R__b << fPsdPsdSameTime;
      R__b << fT0T0Diff;
      R__b << fStsStsDiff;
      R__b << fMuchMuchDiff;
      R__b << fTrdTrdDiff;
      R__b << fTofTofDiff;
      R__b << fRichRichDiff;
      R__b << fPsdPsdDiff;
      R__b << fT0T0DiffLog;
      R__b << fStsStsDiffLog;
      R__b << fMuchMuchDiffLog;
      R__b << fTrdTrdDiffLog;
      R__b << fTofTofDiffLog;
      R__b << fRichRichDiffLog;
      R__b << fPsdPsdDiffLog;
      R__b << fT0T0DiffPerChan;
      R__b << fStsStsDiffPerChan;
      R__b << fMuchMuchDiffPerChan;
      R__b << fTrdTrdDiffPerChan;
      R__b << fTofTofDiffPerChan;
      R__b << fRichRichDiffPerChan;
      R__b << fPsdPsdDiffPerChan;
      fOutFileName.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2019CheckDtInDet(void *p) {
      return  p ? new(p) ::CbmMcbm2019CheckDtInDet : new ::CbmMcbm2019CheckDtInDet;
   }
   static void *newArray_CbmMcbm2019CheckDtInDet(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2019CheckDtInDet[nElements] : new ::CbmMcbm2019CheckDtInDet[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2019CheckDtInDet(void *p) {
      delete ((::CbmMcbm2019CheckDtInDet*)p);
   }
   static void deleteArray_CbmMcbm2019CheckDtInDet(void *p) {
      delete [] ((::CbmMcbm2019CheckDtInDet*)p);
   }
   static void destruct_CbmMcbm2019CheckDtInDet(void *p) {
      typedef ::CbmMcbm2019CheckDtInDet current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMcbm2019CheckDtInDet(TBuffer &buf, void *obj) {
      ((::CbmMcbm2019CheckDtInDet*)obj)->::CbmMcbm2019CheckDtInDet::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMcbm2019CheckDtInDet

//______________________________________________________________________________
void CbmMcbm2019CheckTimingPairs::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2019CheckTimingPairs.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2019CheckTimingPairs::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2019CheckTimingPairs::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2019CheckTimingPairs(void *p) {
      return  p ? new(p) ::CbmMcbm2019CheckTimingPairs : new ::CbmMcbm2019CheckTimingPairs;
   }
   static void *newArray_CbmMcbm2019CheckTimingPairs(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2019CheckTimingPairs[nElements] : new ::CbmMcbm2019CheckTimingPairs[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2019CheckTimingPairs(void *p) {
      delete ((::CbmMcbm2019CheckTimingPairs*)p);
   }
   static void deleteArray_CbmMcbm2019CheckTimingPairs(void *p) {
      delete [] ((::CbmMcbm2019CheckTimingPairs*)p);
   }
   static void destruct_CbmMcbm2019CheckTimingPairs(void *p) {
      typedef ::CbmMcbm2019CheckTimingPairs current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2019CheckTimingPairs

//______________________________________________________________________________
void CbmMcbm2019TimeWinEventBuilderAlgo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2019TimeWinEventBuilderAlgo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2019TimeWinEventBuilderAlgo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2019TimeWinEventBuilderAlgo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2019TimeWinEventBuilderAlgo(void *p) {
      return  p ? new(p) ::CbmMcbm2019TimeWinEventBuilderAlgo : new ::CbmMcbm2019TimeWinEventBuilderAlgo;
   }
   static void *newArray_CbmMcbm2019TimeWinEventBuilderAlgo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2019TimeWinEventBuilderAlgo[nElements] : new ::CbmMcbm2019TimeWinEventBuilderAlgo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2019TimeWinEventBuilderAlgo(void *p) {
      delete ((::CbmMcbm2019TimeWinEventBuilderAlgo*)p);
   }
   static void deleteArray_CbmMcbm2019TimeWinEventBuilderAlgo(void *p) {
      delete [] ((::CbmMcbm2019TimeWinEventBuilderAlgo*)p);
   }
   static void destruct_CbmMcbm2019TimeWinEventBuilderAlgo(void *p) {
      typedef ::CbmMcbm2019TimeWinEventBuilderAlgo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2019TimeWinEventBuilderAlgo

//______________________________________________________________________________
void CbmMcbm2019TimeWinEventBuilderTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2019TimeWinEventBuilderTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2019TimeWinEventBuilderTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2019TimeWinEventBuilderTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2019TimeWinEventBuilderTask(void *p) {
      return  p ? new(p) ::CbmMcbm2019TimeWinEventBuilderTask : new ::CbmMcbm2019TimeWinEventBuilderTask;
   }
   static void *newArray_CbmMcbm2019TimeWinEventBuilderTask(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2019TimeWinEventBuilderTask[nElements] : new ::CbmMcbm2019TimeWinEventBuilderTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2019TimeWinEventBuilderTask(void *p) {
      delete ((::CbmMcbm2019TimeWinEventBuilderTask*)p);
   }
   static void deleteArray_CbmMcbm2019TimeWinEventBuilderTask(void *p) {
      delete [] ((::CbmMcbm2019TimeWinEventBuilderTask*)p);
   }
   static void destruct_CbmMcbm2019TimeWinEventBuilderTask(void *p) {
      typedef ::CbmMcbm2019TimeWinEventBuilderTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2019TimeWinEventBuilderTask

//______________________________________________________________________________
void CbmMcbmCheckTimingAlgo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbmCheckTimingAlgo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbmCheckTimingAlgo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbmCheckTimingAlgo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbmCheckTimingAlgo(void *p) {
      return  p ? new(p) ::CbmMcbmCheckTimingAlgo : new ::CbmMcbmCheckTimingAlgo;
   }
   static void *newArray_CbmMcbmCheckTimingAlgo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbmCheckTimingAlgo[nElements] : new ::CbmMcbmCheckTimingAlgo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbmCheckTimingAlgo(void *p) {
      delete ((::CbmMcbmCheckTimingAlgo*)p);
   }
   static void deleteArray_CbmMcbmCheckTimingAlgo(void *p) {
      delete [] ((::CbmMcbmCheckTimingAlgo*)p);
   }
   static void destruct_CbmMcbmCheckTimingAlgo(void *p) {
      typedef ::CbmMcbmCheckTimingAlgo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbmCheckTimingAlgo

//______________________________________________________________________________
void CbmMcbmCheckTimingTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbmCheckTimingTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbmCheckTimingTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbmCheckTimingTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbmCheckTimingTask(void *p) {
      return  p ? new(p) ::CbmMcbmCheckTimingTask : new ::CbmMcbmCheckTimingTask;
   }
   static void *newArray_CbmMcbmCheckTimingTask(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbmCheckTimingTask[nElements] : new ::CbmMcbmCheckTimingTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbmCheckTimingTask(void *p) {
      delete ((::CbmMcbmCheckTimingTask*)p);
   }
   static void deleteArray_CbmMcbmCheckTimingTask(void *p) {
      delete [] ((::CbmMcbmCheckTimingTask*)p);
   }
   static void destruct_CbmMcbmCheckTimingTask(void *p) {
      typedef ::CbmMcbmCheckTimingTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbmCheckTimingTask

//______________________________________________________________________________
void CbmMcbmSpillFindAlgo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbmSpillFindAlgo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbmSpillFindAlgo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbmSpillFindAlgo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbmSpillFindAlgo(void *p) {
      return  p ? new(p) ::CbmMcbmSpillFindAlgo : new ::CbmMcbmSpillFindAlgo;
   }
   static void *newArray_CbmMcbmSpillFindAlgo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbmSpillFindAlgo[nElements] : new ::CbmMcbmSpillFindAlgo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbmSpillFindAlgo(void *p) {
      delete ((::CbmMcbmSpillFindAlgo*)p);
   }
   static void deleteArray_CbmMcbmSpillFindAlgo(void *p) {
      delete [] ((::CbmMcbmSpillFindAlgo*)p);
   }
   static void destruct_CbmMcbmSpillFindAlgo(void *p) {
      typedef ::CbmMcbmSpillFindAlgo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbmSpillFindAlgo

//______________________________________________________________________________
void CbmMcbmSpillFindTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbmSpillFindTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbmSpillFindTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbmSpillFindTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbmSpillFindTask(void *p) {
      return  p ? new(p) ::CbmMcbmSpillFindTask : new ::CbmMcbmSpillFindTask;
   }
   static void *newArray_CbmMcbmSpillFindTask(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbmSpillFindTask[nElements] : new ::CbmMcbmSpillFindTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbmSpillFindTask(void *p) {
      delete ((::CbmMcbmSpillFindTask*)p);
   }
   static void deleteArray_CbmMcbmSpillFindTask(void *p) {
      delete [] ((::CbmMcbmSpillFindTask*)p);
   }
   static void destruct_CbmMcbmSpillFindTask(void *p) {
      typedef ::CbmMcbmSpillFindTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbmSpillFindTask

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
   static TClass *vectorlEvectorlEgdpbv100cLcLMessagegRsPgR_Dictionary();
   static void vectorlEvectorlEgdpbv100cLcLMessagegRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR(void *p);
   static void destruct_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<gdpbv100::Message> >*)
   {
      vector<vector<gdpbv100::Message> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<gdpbv100::Message> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<gdpbv100::Message> >", -2, "vector", 389,
                  typeid(vector<vector<gdpbv100::Message> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEgdpbv100cLcLMessagegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<gdpbv100::Message> >) );
      instance.SetNew(&new_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<gdpbv100::Message> > >()));

      ::ROOT::AddClassAlternate("vector<vector<gdpbv100::Message> >","std::vector<std::vector<gdpbv100::Message, std::allocator<gdpbv100::Message> >, std::allocator<std::vector<gdpbv100::Message, std::allocator<gdpbv100::Message> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<gdpbv100::Message> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEgdpbv100cLcLMessagegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<gdpbv100::Message> >*)nullptr)->GetClass();
      vectorlEvectorlEgdpbv100cLcLMessagegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEgdpbv100cLcLMessagegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<gdpbv100::Message> > : new vector<vector<gdpbv100::Message> >;
   }
   static void *newArray_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<gdpbv100::Message> >[nElements] : new vector<vector<gdpbv100::Message> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR(void *p) {
      delete ((vector<vector<gdpbv100::Message> >*)p);
   }
   static void deleteArray_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR(void *p) {
      delete [] ((vector<vector<gdpbv100::Message> >*)p);
   }
   static void destruct_vectorlEvectorlEgdpbv100cLcLMessagegRsPgR(void *p) {
      typedef vector<vector<gdpbv100::Message> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<gdpbv100::Message> >

namespace ROOT {
   static TClass *vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR_Dictionary();
   static void vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR(void *p);
   static void destruct_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<gdpbv100::FullMessage> >*)
   {
      vector<vector<gdpbv100::FullMessage> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<gdpbv100::FullMessage> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<gdpbv100::FullMessage> >", -2, "vector", 389,
                  typeid(vector<vector<gdpbv100::FullMessage> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<gdpbv100::FullMessage> >) );
      instance.SetNew(&new_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<gdpbv100::FullMessage> > >()));

      ::ROOT::AddClassAlternate("vector<vector<gdpbv100::FullMessage> >","std::vector<std::vector<gdpbv100::FullMessage, std::allocator<gdpbv100::FullMessage> >, std::allocator<std::vector<gdpbv100::FullMessage, std::allocator<gdpbv100::FullMessage> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<gdpbv100::FullMessage> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<gdpbv100::FullMessage> >*)nullptr)->GetClass();
      vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<gdpbv100::FullMessage> > : new vector<vector<gdpbv100::FullMessage> >;
   }
   static void *newArray_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<gdpbv100::FullMessage> >[nElements] : new vector<vector<gdpbv100::FullMessage> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR(void *p) {
      delete ((vector<vector<gdpbv100::FullMessage> >*)p);
   }
   static void deleteArray_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR(void *p) {
      delete [] ((vector<vector<gdpbv100::FullMessage> >*)p);
   }
   static void destruct_vectorlEvectorlEgdpbv100cLcLFullMessagegRsPgR(void *p) {
      typedef vector<vector<gdpbv100::FullMessage> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<gdpbv100::FullMessage> >

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
                  &vectorlEpairlEunsignedsPlongcOunsignedsPlonggRsPgR_Dictionary, isa_proxy, 0,
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
   static TClass *vectorlEgdpbv100cLcLMessagegR_Dictionary();
   static void vectorlEgdpbv100cLcLMessagegR_TClassManip(TClass*);
   static void *new_vectorlEgdpbv100cLcLMessagegR(void *p = nullptr);
   static void *newArray_vectorlEgdpbv100cLcLMessagegR(Long_t size, void *p);
   static void delete_vectorlEgdpbv100cLcLMessagegR(void *p);
   static void deleteArray_vectorlEgdpbv100cLcLMessagegR(void *p);
   static void destruct_vectorlEgdpbv100cLcLMessagegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<gdpbv100::Message>*)
   {
      vector<gdpbv100::Message> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<gdpbv100::Message>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<gdpbv100::Message>", -2, "vector", 389,
                  typeid(vector<gdpbv100::Message>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEgdpbv100cLcLMessagegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<gdpbv100::Message>) );
      instance.SetNew(&new_vectorlEgdpbv100cLcLMessagegR);
      instance.SetNewArray(&newArray_vectorlEgdpbv100cLcLMessagegR);
      instance.SetDelete(&delete_vectorlEgdpbv100cLcLMessagegR);
      instance.SetDeleteArray(&deleteArray_vectorlEgdpbv100cLcLMessagegR);
      instance.SetDestructor(&destruct_vectorlEgdpbv100cLcLMessagegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<gdpbv100::Message> >()));

      ::ROOT::AddClassAlternate("vector<gdpbv100::Message>","std::vector<gdpbv100::Message, std::allocator<gdpbv100::Message> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<gdpbv100::Message>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEgdpbv100cLcLMessagegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<gdpbv100::Message>*)nullptr)->GetClass();
      vectorlEgdpbv100cLcLMessagegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEgdpbv100cLcLMessagegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEgdpbv100cLcLMessagegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<gdpbv100::Message> : new vector<gdpbv100::Message>;
   }
   static void *newArray_vectorlEgdpbv100cLcLMessagegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<gdpbv100::Message>[nElements] : new vector<gdpbv100::Message>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEgdpbv100cLcLMessagegR(void *p) {
      delete ((vector<gdpbv100::Message>*)p);
   }
   static void deleteArray_vectorlEgdpbv100cLcLMessagegR(void *p) {
      delete [] ((vector<gdpbv100::Message>*)p);
   }
   static void destruct_vectorlEgdpbv100cLcLMessagegR(void *p) {
      typedef vector<gdpbv100::Message> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<gdpbv100::Message>

namespace ROOT {
   static TClass *vectorlEgdpbv100cLcLFullMessagegR_Dictionary();
   static void vectorlEgdpbv100cLcLFullMessagegR_TClassManip(TClass*);
   static void *new_vectorlEgdpbv100cLcLFullMessagegR(void *p = nullptr);
   static void *newArray_vectorlEgdpbv100cLcLFullMessagegR(Long_t size, void *p);
   static void delete_vectorlEgdpbv100cLcLFullMessagegR(void *p);
   static void deleteArray_vectorlEgdpbv100cLcLFullMessagegR(void *p);
   static void destruct_vectorlEgdpbv100cLcLFullMessagegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<gdpbv100::FullMessage>*)
   {
      vector<gdpbv100::FullMessage> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<gdpbv100::FullMessage>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<gdpbv100::FullMessage>", -2, "vector", 389,
                  typeid(vector<gdpbv100::FullMessage>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEgdpbv100cLcLFullMessagegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<gdpbv100::FullMessage>) );
      instance.SetNew(&new_vectorlEgdpbv100cLcLFullMessagegR);
      instance.SetNewArray(&newArray_vectorlEgdpbv100cLcLFullMessagegR);
      instance.SetDelete(&delete_vectorlEgdpbv100cLcLFullMessagegR);
      instance.SetDeleteArray(&deleteArray_vectorlEgdpbv100cLcLFullMessagegR);
      instance.SetDestructor(&destruct_vectorlEgdpbv100cLcLFullMessagegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<gdpbv100::FullMessage> >()));

      ::ROOT::AddClassAlternate("vector<gdpbv100::FullMessage>","std::vector<gdpbv100::FullMessage, std::allocator<gdpbv100::FullMessage> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<gdpbv100::FullMessage>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEgdpbv100cLcLFullMessagegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<gdpbv100::FullMessage>*)nullptr)->GetClass();
      vectorlEgdpbv100cLcLFullMessagegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEgdpbv100cLcLFullMessagegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEgdpbv100cLcLFullMessagegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<gdpbv100::FullMessage> : new vector<gdpbv100::FullMessage>;
   }
   static void *newArray_vectorlEgdpbv100cLcLFullMessagegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<gdpbv100::FullMessage>[nElements] : new vector<gdpbv100::FullMessage>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEgdpbv100cLcLFullMessagegR(void *p) {
      delete ((vector<gdpbv100::FullMessage>*)p);
   }
   static void deleteArray_vectorlEgdpbv100cLcLFullMessagegR(void *p) {
      delete [] ((vector<gdpbv100::FullMessage>*)p);
   }
   static void destruct_vectorlEgdpbv100cLcLFullMessagegR(void *p) {
      typedef vector<gdpbv100::FullMessage> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<gdpbv100::FullMessage>

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
   static TClass *vectorlEULong64_tgR_Dictionary();
   static void vectorlEULong64_tgR_TClassManip(TClass*);
   static void *new_vectorlEULong64_tgR(void *p = nullptr);
   static void *newArray_vectorlEULong64_tgR(Long_t size, void *p);
   static void delete_vectorlEULong64_tgR(void *p);
   static void deleteArray_vectorlEULong64_tgR(void *p);
   static void destruct_vectorlEULong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ULong64_t>*)
   {
      vector<ULong64_t> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ULong64_t>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ULong64_t>", -2, "vector", 389,
                  typeid(vector<ULong64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEULong64_tgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ULong64_t>) );
      instance.SetNew(&new_vectorlEULong64_tgR);
      instance.SetNewArray(&newArray_vectorlEULong64_tgR);
      instance.SetDelete(&delete_vectorlEULong64_tgR);
      instance.SetDeleteArray(&deleteArray_vectorlEULong64_tgR);
      instance.SetDestructor(&destruct_vectorlEULong64_tgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ULong64_t> >()));

      ::ROOT::AddClassAlternate("vector<ULong64_t>","std::vector<unsigned long long, std::allocator<unsigned long long> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<ULong64_t>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEULong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<ULong64_t>*)nullptr)->GetClass();
      vectorlEULong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEULong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEULong64_tgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ULong64_t> : new vector<ULong64_t>;
   }
   static void *newArray_vectorlEULong64_tgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ULong64_t>[nElements] : new vector<ULong64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEULong64_tgR(void *p) {
      delete ((vector<ULong64_t>*)p);
   }
   static void deleteArray_vectorlEULong64_tgR(void *p) {
      delete [] ((vector<ULong64_t>*)p);
   }
   static void destruct_vectorlEULong64_tgR(void *p) {
      typedef vector<ULong64_t> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<ULong64_t>

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
   static TClass *vectorlEMuchFebChanMaskgR_Dictionary();
   static void vectorlEMuchFebChanMaskgR_TClassManip(TClass*);
   static void *new_vectorlEMuchFebChanMaskgR(void *p = nullptr);
   static void *newArray_vectorlEMuchFebChanMaskgR(Long_t size, void *p);
   static void delete_vectorlEMuchFebChanMaskgR(void *p);
   static void deleteArray_vectorlEMuchFebChanMaskgR(void *p);
   static void destruct_vectorlEMuchFebChanMaskgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<MuchFebChanMask>*)
   {
      vector<MuchFebChanMask> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<MuchFebChanMask>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<MuchFebChanMask>", -2, "vector", 389,
                  typeid(vector<MuchFebChanMask>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEMuchFebChanMaskgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<MuchFebChanMask>) );
      instance.SetNew(&new_vectorlEMuchFebChanMaskgR);
      instance.SetNewArray(&newArray_vectorlEMuchFebChanMaskgR);
      instance.SetDelete(&delete_vectorlEMuchFebChanMaskgR);
      instance.SetDeleteArray(&deleteArray_vectorlEMuchFebChanMaskgR);
      instance.SetDestructor(&destruct_vectorlEMuchFebChanMaskgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<MuchFebChanMask> >()));

      ::ROOT::AddClassAlternate("vector<MuchFebChanMask>","std::vector<MuchFebChanMask, std::allocator<MuchFebChanMask> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<MuchFebChanMask>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEMuchFebChanMaskgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<MuchFebChanMask>*)nullptr)->GetClass();
      vectorlEMuchFebChanMaskgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEMuchFebChanMaskgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEMuchFebChanMaskgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<MuchFebChanMask> : new vector<MuchFebChanMask>;
   }
   static void *newArray_vectorlEMuchFebChanMaskgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<MuchFebChanMask>[nElements] : new vector<MuchFebChanMask>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEMuchFebChanMaskgR(void *p) {
      delete ((vector<MuchFebChanMask>*)p);
   }
   static void deleteArray_vectorlEMuchFebChanMaskgR(void *p) {
      delete [] ((vector<MuchFebChanMask>*)p);
   }
   static void destruct_vectorlEMuchFebChanMaskgR(void *p) {
      typedef vector<MuchFebChanMask> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<MuchFebChanMask>

namespace ROOT {
   static TClass *vectorlEFebChanMaskgR_Dictionary();
   static void vectorlEFebChanMaskgR_TClassManip(TClass*);
   static void *new_vectorlEFebChanMaskgR(void *p = nullptr);
   static void *newArray_vectorlEFebChanMaskgR(Long_t size, void *p);
   static void delete_vectorlEFebChanMaskgR(void *p);
   static void deleteArray_vectorlEFebChanMaskgR(void *p);
   static void destruct_vectorlEFebChanMaskgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<FebChanMask>*)
   {
      vector<FebChanMask> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<FebChanMask>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<FebChanMask>", -2, "vector", 389,
                  typeid(vector<FebChanMask>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEFebChanMaskgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<FebChanMask>) );
      instance.SetNew(&new_vectorlEFebChanMaskgR);
      instance.SetNewArray(&newArray_vectorlEFebChanMaskgR);
      instance.SetDelete(&delete_vectorlEFebChanMaskgR);
      instance.SetDeleteArray(&deleteArray_vectorlEFebChanMaskgR);
      instance.SetDestructor(&destruct_vectorlEFebChanMaskgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<FebChanMask> >()));

      ::ROOT::AddClassAlternate("vector<FebChanMask>","std::vector<FebChanMask, std::allocator<FebChanMask> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<FebChanMask>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEFebChanMaskgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<FebChanMask>*)nullptr)->GetClass();
      vectorlEFebChanMaskgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEFebChanMaskgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEFebChanMaskgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<FebChanMask> : new vector<FebChanMask>;
   }
   static void *newArray_vectorlEFebChanMaskgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<FebChanMask>[nElements] : new vector<FebChanMask>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEFebChanMaskgR(void *p) {
      delete ((vector<FebChanMask>*)p);
   }
   static void deleteArray_vectorlEFebChanMaskgR(void *p) {
      delete [] ((vector<FebChanMask>*)p);
   }
   static void destruct_vectorlEFebChanMaskgR(void *p) {
      typedef vector<FebChanMask> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<FebChanMask>

namespace ROOT {
   static TClass *vectorlEEventBuilderDetectorgR_Dictionary();
   static void vectorlEEventBuilderDetectorgR_TClassManip(TClass*);
   static void *new_vectorlEEventBuilderDetectorgR(void *p = nullptr);
   static void *newArray_vectorlEEventBuilderDetectorgR(Long_t size, void *p);
   static void delete_vectorlEEventBuilderDetectorgR(void *p);
   static void deleteArray_vectorlEEventBuilderDetectorgR(void *p);
   static void destruct_vectorlEEventBuilderDetectorgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<EventBuilderDetector>*)
   {
      vector<EventBuilderDetector> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<EventBuilderDetector>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<EventBuilderDetector>", -2, "vector", 389,
                  typeid(vector<EventBuilderDetector>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEEventBuilderDetectorgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<EventBuilderDetector>) );
      instance.SetNew(&new_vectorlEEventBuilderDetectorgR);
      instance.SetNewArray(&newArray_vectorlEEventBuilderDetectorgR);
      instance.SetDelete(&delete_vectorlEEventBuilderDetectorgR);
      instance.SetDeleteArray(&deleteArray_vectorlEEventBuilderDetectorgR);
      instance.SetDestructor(&destruct_vectorlEEventBuilderDetectorgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<EventBuilderDetector> >()));

      ::ROOT::AddClassAlternate("vector<EventBuilderDetector>","std::vector<EventBuilderDetector, std::allocator<EventBuilderDetector> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<EventBuilderDetector>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEEventBuilderDetectorgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<EventBuilderDetector>*)nullptr)->GetClass();
      vectorlEEventBuilderDetectorgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEEventBuilderDetectorgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEEventBuilderDetectorgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<EventBuilderDetector> : new vector<EventBuilderDetector>;
   }
   static void *newArray_vectorlEEventBuilderDetectorgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<EventBuilderDetector>[nElements] : new vector<EventBuilderDetector>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEEventBuilderDetectorgR(void *p) {
      delete ((vector<EventBuilderDetector>*)p);
   }
   static void deleteArray_vectorlEEventBuilderDetectorgR(void *p) {
      delete [] ((vector<EventBuilderDetector>*)p);
   }
   static void destruct_vectorlEEventBuilderDetectorgR(void *p) {
      typedef vector<EventBuilderDetector> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<EventBuilderDetector>

namespace ROOT {
   static TClass *vectorlECheckTimingDetectorgR_Dictionary();
   static void vectorlECheckTimingDetectorgR_TClassManip(TClass*);
   static void *new_vectorlECheckTimingDetectorgR(void *p = nullptr);
   static void *newArray_vectorlECheckTimingDetectorgR(Long_t size, void *p);
   static void delete_vectorlECheckTimingDetectorgR(void *p);
   static void deleteArray_vectorlECheckTimingDetectorgR(void *p);
   static void destruct_vectorlECheckTimingDetectorgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CheckTimingDetector>*)
   {
      vector<CheckTimingDetector> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CheckTimingDetector>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CheckTimingDetector>", -2, "vector", 389,
                  typeid(vector<CheckTimingDetector>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECheckTimingDetectorgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CheckTimingDetector>) );
      instance.SetNew(&new_vectorlECheckTimingDetectorgR);
      instance.SetNewArray(&newArray_vectorlECheckTimingDetectorgR);
      instance.SetDelete(&delete_vectorlECheckTimingDetectorgR);
      instance.SetDeleteArray(&deleteArray_vectorlECheckTimingDetectorgR);
      instance.SetDestructor(&destruct_vectorlECheckTimingDetectorgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CheckTimingDetector> >()));

      ::ROOT::AddClassAlternate("vector<CheckTimingDetector>","std::vector<CheckTimingDetector, std::allocator<CheckTimingDetector> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CheckTimingDetector>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECheckTimingDetectorgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CheckTimingDetector>*)nullptr)->GetClass();
      vectorlECheckTimingDetectorgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECheckTimingDetectorgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECheckTimingDetectorgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CheckTimingDetector> : new vector<CheckTimingDetector>;
   }
   static void *newArray_vectorlECheckTimingDetectorgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CheckTimingDetector>[nElements] : new vector<CheckTimingDetector>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECheckTimingDetectorgR(void *p) {
      delete ((vector<CheckTimingDetector>*)p);
   }
   static void deleteArray_vectorlECheckTimingDetectorgR(void *p) {
      delete [] ((vector<CheckTimingDetector>*)p);
   }
   static void destruct_vectorlECheckTimingDetectorgR(void *p) {
      typedef vector<CheckTimingDetector> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CheckTimingDetector>

namespace ROOT {
   static TClass *vectorlECbmTrdRawMessageSpadicgR_Dictionary();
   static void vectorlECbmTrdRawMessageSpadicgR_TClassManip(TClass*);
   static void *new_vectorlECbmTrdRawMessageSpadicgR(void *p = nullptr);
   static void *newArray_vectorlECbmTrdRawMessageSpadicgR(Long_t size, void *p);
   static void delete_vectorlECbmTrdRawMessageSpadicgR(void *p);
   static void deleteArray_vectorlECbmTrdRawMessageSpadicgR(void *p);
   static void destruct_vectorlECbmTrdRawMessageSpadicgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmTrdRawMessageSpadic>*)
   {
      vector<CbmTrdRawMessageSpadic> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmTrdRawMessageSpadic>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmTrdRawMessageSpadic>", -2, "vector", 389,
                  typeid(vector<CbmTrdRawMessageSpadic>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmTrdRawMessageSpadicgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmTrdRawMessageSpadic>) );
      instance.SetNew(&new_vectorlECbmTrdRawMessageSpadicgR);
      instance.SetNewArray(&newArray_vectorlECbmTrdRawMessageSpadicgR);
      instance.SetDelete(&delete_vectorlECbmTrdRawMessageSpadicgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmTrdRawMessageSpadicgR);
      instance.SetDestructor(&destruct_vectorlECbmTrdRawMessageSpadicgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmTrdRawMessageSpadic> >()));

      ::ROOT::AddClassAlternate("vector<CbmTrdRawMessageSpadic>","std::vector<CbmTrdRawMessageSpadic, std::allocator<CbmTrdRawMessageSpadic> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmTrdRawMessageSpadic>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmTrdRawMessageSpadicgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmTrdRawMessageSpadic>*)nullptr)->GetClass();
      vectorlECbmTrdRawMessageSpadicgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmTrdRawMessageSpadicgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmTrdRawMessageSpadicgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTrdRawMessageSpadic> : new vector<CbmTrdRawMessageSpadic>;
   }
   static void *newArray_vectorlECbmTrdRawMessageSpadicgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTrdRawMessageSpadic>[nElements] : new vector<CbmTrdRawMessageSpadic>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmTrdRawMessageSpadicgR(void *p) {
      delete ((vector<CbmTrdRawMessageSpadic>*)p);
   }
   static void deleteArray_vectorlECbmTrdRawMessageSpadicgR(void *p) {
      delete [] ((vector<CbmTrdRawMessageSpadic>*)p);
   }
   static void destruct_vectorlECbmTrdRawMessageSpadicgR(void *p) {
      typedef vector<CbmTrdRawMessageSpadic> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmTrdRawMessageSpadic>

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
   static TClass *vectorlECbmStsDigigR_Dictionary();
   static void vectorlECbmStsDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmStsDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmStsDigigR(Long_t size, void *p);
   static void delete_vectorlECbmStsDigigR(void *p);
   static void deleteArray_vectorlECbmStsDigigR(void *p);
   static void destruct_vectorlECbmStsDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsDigi>*)
   {
      vector<CbmStsDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsDigi>", -2, "vector", 389,
                  typeid(vector<CbmStsDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsDigigR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmStsDigi>) );
      instance.SetNew(&new_vectorlECbmStsDigigR);
      instance.SetNewArray(&newArray_vectorlECbmStsDigigR);
      instance.SetDelete(&delete_vectorlECbmStsDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsDigigR);
      instance.SetDestructor(&destruct_vectorlECbmStsDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsDigi>","std::vector<CbmStsDigi, std::allocator<CbmStsDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsDigi>*)nullptr)->GetClass();
      vectorlECbmStsDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsDigi> : new vector<CbmStsDigi>;
   }
   static void *newArray_vectorlECbmStsDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsDigi>[nElements] : new vector<CbmStsDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsDigigR(void *p) {
      delete ((vector<CbmStsDigi>*)p);
   }
   static void deleteArray_vectorlECbmStsDigigR(void *p) {
      delete [] ((vector<CbmStsDigi>*)p);
   }
   static void destruct_vectorlECbmStsDigigR(void *p) {
      typedef vector<CbmStsDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsDigi>

namespace ROOT {
   static TClass *vectorlECbmRichDigigR_Dictionary();
   static void vectorlECbmRichDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmRichDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmRichDigigR(Long_t size, void *p);
   static void delete_vectorlECbmRichDigigR(void *p);
   static void deleteArray_vectorlECbmRichDigigR(void *p);
   static void destruct_vectorlECbmRichDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmRichDigi>*)
   {
      vector<CbmRichDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmRichDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmRichDigi>", -2, "vector", 389,
                  typeid(vector<CbmRichDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmRichDigigR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmRichDigi>) );
      instance.SetNew(&new_vectorlECbmRichDigigR);
      instance.SetNewArray(&newArray_vectorlECbmRichDigigR);
      instance.SetDelete(&delete_vectorlECbmRichDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmRichDigigR);
      instance.SetDestructor(&destruct_vectorlECbmRichDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmRichDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmRichDigi>","std::vector<CbmRichDigi, std::allocator<CbmRichDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmRichDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmRichDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmRichDigi>*)nullptr)->GetClass();
      vectorlECbmRichDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmRichDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmRichDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmRichDigi> : new vector<CbmRichDigi>;
   }
   static void *newArray_vectorlECbmRichDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmRichDigi>[nElements] : new vector<CbmRichDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmRichDigigR(void *p) {
      delete ((vector<CbmRichDigi>*)p);
   }
   static void deleteArray_vectorlECbmRichDigigR(void *p) {
      delete [] ((vector<CbmRichDigi>*)p);
   }
   static void destruct_vectorlECbmRichDigigR(void *p) {
      typedef vector<CbmRichDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmRichDigi>

namespace ROOT {
   static TClass *vectorlECbmPsdDspgR_Dictionary();
   static void vectorlECbmPsdDspgR_TClassManip(TClass*);
   static void *new_vectorlECbmPsdDspgR(void *p = nullptr);
   static void *newArray_vectorlECbmPsdDspgR(Long_t size, void *p);
   static void delete_vectorlECbmPsdDspgR(void *p);
   static void deleteArray_vectorlECbmPsdDspgR(void *p);
   static void destruct_vectorlECbmPsdDspgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmPsdDsp>*)
   {
      vector<CbmPsdDsp> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmPsdDsp>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmPsdDsp>", -2, "vector", 389,
                  typeid(vector<CbmPsdDsp>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmPsdDspgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmPsdDsp>) );
      instance.SetNew(&new_vectorlECbmPsdDspgR);
      instance.SetNewArray(&newArray_vectorlECbmPsdDspgR);
      instance.SetDelete(&delete_vectorlECbmPsdDspgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmPsdDspgR);
      instance.SetDestructor(&destruct_vectorlECbmPsdDspgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmPsdDsp> >()));

      ::ROOT::AddClassAlternate("vector<CbmPsdDsp>","std::vector<CbmPsdDsp, std::allocator<CbmPsdDsp> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmPsdDsp>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmPsdDspgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmPsdDsp>*)nullptr)->GetClass();
      vectorlECbmPsdDspgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmPsdDspgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmPsdDspgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmPsdDsp> : new vector<CbmPsdDsp>;
   }
   static void *newArray_vectorlECbmPsdDspgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmPsdDsp>[nElements] : new vector<CbmPsdDsp>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmPsdDspgR(void *p) {
      delete ((vector<CbmPsdDsp>*)p);
   }
   static void deleteArray_vectorlECbmPsdDspgR(void *p) {
      delete [] ((vector<CbmPsdDsp>*)p);
   }
   static void destruct_vectorlECbmPsdDspgR(void *p) {
      typedef vector<CbmPsdDsp> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmPsdDsp>

namespace ROOT {
   static TClass *vectorlECbmPsdDigigR_Dictionary();
   static void vectorlECbmPsdDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmPsdDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmPsdDigigR(Long_t size, void *p);
   static void delete_vectorlECbmPsdDigigR(void *p);
   static void deleteArray_vectorlECbmPsdDigigR(void *p);
   static void destruct_vectorlECbmPsdDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmPsdDigi>*)
   {
      vector<CbmPsdDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmPsdDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmPsdDigi>", -2, "vector", 389,
                  typeid(vector<CbmPsdDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmPsdDigigR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmPsdDigi>) );
      instance.SetNew(&new_vectorlECbmPsdDigigR);
      instance.SetNewArray(&newArray_vectorlECbmPsdDigigR);
      instance.SetDelete(&delete_vectorlECbmPsdDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmPsdDigigR);
      instance.SetDestructor(&destruct_vectorlECbmPsdDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmPsdDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmPsdDigi>","std::vector<CbmPsdDigi, std::allocator<CbmPsdDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmPsdDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmPsdDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmPsdDigi>*)nullptr)->GetClass();
      vectorlECbmPsdDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmPsdDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmPsdDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmPsdDigi> : new vector<CbmPsdDigi>;
   }
   static void *newArray_vectorlECbmPsdDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmPsdDigi>[nElements] : new vector<CbmPsdDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmPsdDigigR(void *p) {
      delete ((vector<CbmPsdDigi>*)p);
   }
   static void deleteArray_vectorlECbmPsdDigigR(void *p) {
      delete [] ((vector<CbmPsdDigi>*)p);
   }
   static void destruct_vectorlECbmPsdDigigR(void *p) {
      typedef vector<CbmPsdDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmPsdDigi>

namespace ROOT {
   static TClass *vectorlECbmMuchBeamTimeDigigR_Dictionary();
   static void vectorlECbmMuchBeamTimeDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmMuchBeamTimeDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmMuchBeamTimeDigigR(Long_t size, void *p);
   static void delete_vectorlECbmMuchBeamTimeDigigR(void *p);
   static void deleteArray_vectorlECbmMuchBeamTimeDigigR(void *p);
   static void destruct_vectorlECbmMuchBeamTimeDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMuchBeamTimeDigi>*)
   {
      vector<CbmMuchBeamTimeDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMuchBeamTimeDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMuchBeamTimeDigi>", -2, "vector", 389,
                  typeid(vector<CbmMuchBeamTimeDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMuchBeamTimeDigigR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmMuchBeamTimeDigi>) );
      instance.SetNew(&new_vectorlECbmMuchBeamTimeDigigR);
      instance.SetNewArray(&newArray_vectorlECbmMuchBeamTimeDigigR);
      instance.SetDelete(&delete_vectorlECbmMuchBeamTimeDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMuchBeamTimeDigigR);
      instance.SetDestructor(&destruct_vectorlECbmMuchBeamTimeDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMuchBeamTimeDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmMuchBeamTimeDigi>","std::vector<CbmMuchBeamTimeDigi, std::allocator<CbmMuchBeamTimeDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMuchBeamTimeDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMuchBeamTimeDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMuchBeamTimeDigi>*)nullptr)->GetClass();
      vectorlECbmMuchBeamTimeDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMuchBeamTimeDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMuchBeamTimeDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMuchBeamTimeDigi> : new vector<CbmMuchBeamTimeDigi>;
   }
   static void *newArray_vectorlECbmMuchBeamTimeDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMuchBeamTimeDigi>[nElements] : new vector<CbmMuchBeamTimeDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMuchBeamTimeDigigR(void *p) {
      delete ((vector<CbmMuchBeamTimeDigi>*)p);
   }
   static void deleteArray_vectorlECbmMuchBeamTimeDigigR(void *p) {
      delete [] ((vector<CbmMuchBeamTimeDigi>*)p);
   }
   static void destruct_vectorlECbmMuchBeamTimeDigigR(void *p) {
      typedef vector<CbmMuchBeamTimeDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMuchBeamTimeDigi>

namespace ROOT {
   static TClass *vectorlECbmErrorMessagegR_Dictionary();
   static void vectorlECbmErrorMessagegR_TClassManip(TClass*);
   static void *new_vectorlECbmErrorMessagegR(void *p = nullptr);
   static void *newArray_vectorlECbmErrorMessagegR(Long_t size, void *p);
   static void delete_vectorlECbmErrorMessagegR(void *p);
   static void deleteArray_vectorlECbmErrorMessagegR(void *p);
   static void destruct_vectorlECbmErrorMessagegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmErrorMessage>*)
   {
      vector<CbmErrorMessage> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmErrorMessage>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmErrorMessage>", -2, "vector", 389,
                  typeid(vector<CbmErrorMessage>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmErrorMessagegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmErrorMessage>) );
      instance.SetNew(&new_vectorlECbmErrorMessagegR);
      instance.SetNewArray(&newArray_vectorlECbmErrorMessagegR);
      instance.SetDelete(&delete_vectorlECbmErrorMessagegR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmErrorMessagegR);
      instance.SetDestructor(&destruct_vectorlECbmErrorMessagegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmErrorMessage> >()));

      ::ROOT::AddClassAlternate("vector<CbmErrorMessage>","std::vector<CbmErrorMessage, std::allocator<CbmErrorMessage> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmErrorMessage>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmErrorMessagegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmErrorMessage>*)nullptr)->GetClass();
      vectorlECbmErrorMessagegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmErrorMessagegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmErrorMessagegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmErrorMessage> : new vector<CbmErrorMessage>;
   }
   static void *newArray_vectorlECbmErrorMessagegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmErrorMessage>[nElements] : new vector<CbmErrorMessage>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmErrorMessagegR(void *p) {
      delete ((vector<CbmErrorMessage>*)p);
   }
   static void deleteArray_vectorlECbmErrorMessagegR(void *p) {
      delete [] ((vector<CbmErrorMessage>*)p);
   }
   static void destruct_vectorlECbmErrorMessagegR(void *p) {
      typedef vector<CbmErrorMessage> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmErrorMessage>

namespace ROOT {
   static TClass *maplEunsignedsPshortcOunsignedsPshortgR_Dictionary();
   static void maplEunsignedsPshortcOunsignedsPshortgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPshortcOunsignedsPshortgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPshortcOunsignedsPshortgR(Long_t size, void *p);
   static void delete_maplEunsignedsPshortcOunsignedsPshortgR(void *p);
   static void deleteArray_maplEunsignedsPshortcOunsignedsPshortgR(void *p);
   static void destruct_maplEunsignedsPshortcOunsignedsPshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned short,unsigned short>*)
   {
      map<unsigned short,unsigned short> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned short,unsigned short>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned short,unsigned short>", -2, "map", 100,
                  typeid(map<unsigned short,unsigned short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPshortcOunsignedsPshortgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned short,unsigned short>) );
      instance.SetNew(&new_maplEunsignedsPshortcOunsignedsPshortgR);
      instance.SetNewArray(&newArray_maplEunsignedsPshortcOunsignedsPshortgR);
      instance.SetDelete(&delete_maplEunsignedsPshortcOunsignedsPshortgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPshortcOunsignedsPshortgR);
      instance.SetDestructor(&destruct_maplEunsignedsPshortcOunsignedsPshortgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned short,unsigned short> >()));

      ::ROOT::AddClassAlternate("map<unsigned short,unsigned short>","std::map<unsigned short, unsigned short, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, unsigned short> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned short,unsigned short>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPshortcOunsignedsPshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned short,unsigned short>*)nullptr)->GetClass();
      maplEunsignedsPshortcOunsignedsPshortgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPshortcOunsignedsPshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPshortcOunsignedsPshortgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,unsigned short> : new map<unsigned short,unsigned short>;
   }
   static void *newArray_maplEunsignedsPshortcOunsignedsPshortgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,unsigned short>[nElements] : new map<unsigned short,unsigned short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPshortcOunsignedsPshortgR(void *p) {
      delete ((map<unsigned short,unsigned short>*)p);
   }
   static void deleteArray_maplEunsignedsPshortcOunsignedsPshortgR(void *p) {
      delete [] ((map<unsigned short,unsigned short>*)p);
   }
   static void destruct_maplEunsignedsPshortcOunsignedsPshortgR(void *p) {
      typedef map<unsigned short,unsigned short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned short,unsigned short>

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
   static TClass *maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR_Dictionary();
   static void maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR(void *p);
   static void deleteArray_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR(void *p);
   static void destruct_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,map<int,TH1D*> >*)
   {
      map<unsigned int,map<int,TH1D*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,map<int,TH1D*> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,map<int,TH1D*> >", -2, "map", 100,
                  typeid(map<unsigned int,map<int,TH1D*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,map<int,TH1D*> >) );
      instance.SetNew(&new_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,map<int,TH1D*> > >()));

      ::ROOT::AddClassAlternate("map<unsigned int,map<int,TH1D*> >","std::map<unsigned int, std::map<int, TH1D*, std::less<int>, std::allocator<std::pair<int const, TH1D*> > >, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, std::map<int, TH1D*, std::less<int>, std::allocator<std::pair<int const, TH1D*> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,map<int,TH1D*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,map<int,TH1D*> >*)nullptr)->GetClass();
      maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,map<int,TH1D*> > : new map<unsigned int,map<int,TH1D*> >;
   }
   static void *newArray_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,map<int,TH1D*> >[nElements] : new map<unsigned int,map<int,TH1D*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR(void *p) {
      delete ((map<unsigned int,map<int,TH1D*> >*)p);
   }
   static void deleteArray_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR(void *p) {
      delete [] ((map<unsigned int,map<int,TH1D*> >*)p);
   }
   static void destruct_maplEunsignedsPintcOmaplEintcOTH1DmUgRsPgR(void *p) {
      typedef map<unsigned int,map<int,TH1D*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,map<int,TH1D*> >

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
   static TClass *maplEunsignedsPintcOTH2DmUgR_Dictionary();
   static void maplEunsignedsPintcOTH2DmUgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOTH2DmUgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOTH2DmUgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOTH2DmUgR(void *p);
   static void deleteArray_maplEunsignedsPintcOTH2DmUgR(void *p);
   static void destruct_maplEunsignedsPintcOTH2DmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,TH2D*>*)
   {
      map<unsigned int,TH2D*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,TH2D*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,TH2D*>", -2, "map", 100,
                  typeid(map<unsigned int,TH2D*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOTH2DmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,TH2D*>) );
      instance.SetNew(&new_maplEunsignedsPintcOTH2DmUgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOTH2DmUgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOTH2DmUgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOTH2DmUgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOTH2DmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,TH2D*> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,TH2D*>","std::map<unsigned int, TH2D*, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, TH2D*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,TH2D*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOTH2DmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,TH2D*>*)nullptr)->GetClass();
      maplEunsignedsPintcOTH2DmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOTH2DmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOTH2DmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,TH2D*> : new map<unsigned int,TH2D*>;
   }
   static void *newArray_maplEunsignedsPintcOTH2DmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,TH2D*>[nElements] : new map<unsigned int,TH2D*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOTH2DmUgR(void *p) {
      delete ((map<unsigned int,TH2D*>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOTH2DmUgR(void *p) {
      delete [] ((map<unsigned int,TH2D*>*)p);
   }
   static void destruct_maplEunsignedsPintcOTH2DmUgR(void *p) {
      typedef map<unsigned int,TH2D*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,TH2D*>

namespace ROOT {
   static TClass *maplEstsxytercLcLMessTypecOunsignedsPintgR_Dictionary();
   static void maplEstsxytercLcLMessTypecOunsignedsPintgR_TClassManip(TClass*);
   static void *new_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p = nullptr);
   static void *newArray_maplEstsxytercLcLMessTypecOunsignedsPintgR(Long_t size, void *p);
   static void delete_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p);
   static void deleteArray_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p);
   static void destruct_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<stsxyter::MessType,unsigned int>*)
   {
      map<stsxyter::MessType,unsigned int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<stsxyter::MessType,unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<stsxyter::MessType,unsigned int>", -2, "map", 100,
                  typeid(map<stsxyter::MessType,unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstsxytercLcLMessTypecOunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<stsxyter::MessType,unsigned int>) );
      instance.SetNew(&new_maplEstsxytercLcLMessTypecOunsignedsPintgR);
      instance.SetNewArray(&newArray_maplEstsxytercLcLMessTypecOunsignedsPintgR);
      instance.SetDelete(&delete_maplEstsxytercLcLMessTypecOunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_maplEstsxytercLcLMessTypecOunsignedsPintgR);
      instance.SetDestructor(&destruct_maplEstsxytercLcLMessTypecOunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<stsxyter::MessType,unsigned int> >()));

      ::ROOT::AddClassAlternate("map<stsxyter::MessType,unsigned int>","std::map<stsxyter::MessType, unsigned int, std::less<stsxyter::MessType>, std::allocator<std::pair<stsxyter::MessType const, unsigned int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<stsxyter::MessType,unsigned int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstsxytercLcLMessTypecOunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<stsxyter::MessType,unsigned int>*)nullptr)->GetClass();
      maplEstsxytercLcLMessTypecOunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstsxytercLcLMessTypecOunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<stsxyter::MessType,unsigned int> : new map<stsxyter::MessType,unsigned int>;
   }
   static void *newArray_maplEstsxytercLcLMessTypecOunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<stsxyter::MessType,unsigned int>[nElements] : new map<stsxyter::MessType,unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p) {
      delete ((map<stsxyter::MessType,unsigned int>*)p);
   }
   static void deleteArray_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p) {
      delete [] ((map<stsxyter::MessType,unsigned int>*)p);
   }
   static void destruct_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p) {
      typedef map<stsxyter::MessType,unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<stsxyter::MessType,unsigned int>

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
   static TClass *maplEintcOmaplEintcOTH1DmUgRsPgR_Dictionary();
   static void maplEintcOmaplEintcOTH1DmUgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOmaplEintcOTH1DmUgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOmaplEintcOTH1DmUgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOmaplEintcOTH1DmUgRsPgR(void *p);
   static void deleteArray_maplEintcOmaplEintcOTH1DmUgRsPgR(void *p);
   static void destruct_maplEintcOmaplEintcOTH1DmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,map<int,TH1D*> >*)
   {
      map<int,map<int,TH1D*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,map<int,TH1D*> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,map<int,TH1D*> >", -2, "map", 100,
                  typeid(map<int,map<int,TH1D*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOmaplEintcOTH1DmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,map<int,TH1D*> >) );
      instance.SetNew(&new_maplEintcOmaplEintcOTH1DmUgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOmaplEintcOTH1DmUgRsPgR);
      instance.SetDelete(&delete_maplEintcOmaplEintcOTH1DmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOmaplEintcOTH1DmUgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOmaplEintcOTH1DmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,map<int,TH1D*> > >()));

      ::ROOT::AddClassAlternate("map<int,map<int,TH1D*> >","std::map<int, std::map<int, TH1D*, std::less<int>, std::allocator<std::pair<int const, TH1D*> > >, std::less<int>, std::allocator<std::pair<int const, std::map<int, TH1D*, std::less<int>, std::allocator<std::pair<int const, TH1D*> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,map<int,TH1D*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOmaplEintcOTH1DmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,map<int,TH1D*> >*)nullptr)->GetClass();
      maplEintcOmaplEintcOTH1DmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOmaplEintcOTH1DmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOmaplEintcOTH1DmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,map<int,TH1D*> > : new map<int,map<int,TH1D*> >;
   }
   static void *newArray_maplEintcOmaplEintcOTH1DmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,map<int,TH1D*> >[nElements] : new map<int,map<int,TH1D*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOmaplEintcOTH1DmUgRsPgR(void *p) {
      delete ((map<int,map<int,TH1D*> >*)p);
   }
   static void deleteArray_maplEintcOmaplEintcOTH1DmUgRsPgR(void *p) {
      delete [] ((map<int,map<int,TH1D*> >*)p);
   }
   static void destruct_maplEintcOmaplEintcOTH1DmUgRsPgR(void *p) {
      typedef map<int,map<int,TH1D*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,map<int,TH1D*> >

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
   static TClass *maplEgdpbv100cLcLMessageTypescOunsignedsPintgR_Dictionary();
   static void maplEgdpbv100cLcLMessageTypescOunsignedsPintgR_TClassManip(TClass*);
   static void *new_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p = nullptr);
   static void *newArray_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(Long_t size, void *p);
   static void delete_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p);
   static void deleteArray_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p);
   static void destruct_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<gdpbv100::MessageTypes,unsigned int>*)
   {
      map<gdpbv100::MessageTypes,unsigned int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<gdpbv100::MessageTypes,unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<gdpbv100::MessageTypes,unsigned int>", -2, "map", 100,
                  typeid(map<gdpbv100::MessageTypes,unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEgdpbv100cLcLMessageTypescOunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<gdpbv100::MessageTypes,unsigned int>) );
      instance.SetNew(&new_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR);
      instance.SetNewArray(&newArray_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR);
      instance.SetDelete(&delete_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR);
      instance.SetDestructor(&destruct_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<gdpbv100::MessageTypes,unsigned int> >()));

      ::ROOT::AddClassAlternate("map<gdpbv100::MessageTypes,unsigned int>","std::map<gdpbv100::MessageTypes, unsigned int, std::less<gdpbv100::MessageTypes>, std::allocator<std::pair<gdpbv100::MessageTypes const, unsigned int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<gdpbv100::MessageTypes,unsigned int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEgdpbv100cLcLMessageTypescOunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<gdpbv100::MessageTypes,unsigned int>*)nullptr)->GetClass();
      maplEgdpbv100cLcLMessageTypescOunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEgdpbv100cLcLMessageTypescOunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<gdpbv100::MessageTypes,unsigned int> : new map<gdpbv100::MessageTypes,unsigned int>;
   }
   static void *newArray_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<gdpbv100::MessageTypes,unsigned int>[nElements] : new map<gdpbv100::MessageTypes,unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p) {
      delete ((map<gdpbv100::MessageTypes,unsigned int>*)p);
   }
   static void deleteArray_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p) {
      delete [] ((map<gdpbv100::MessageTypes,unsigned int>*)p);
   }
   static void destruct_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p) {
      typedef map<gdpbv100::MessageTypes,unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<gdpbv100::MessageTypes,unsigned int>

namespace ROOT {
   static TClass *maplEECbmModuleIdcOvectorlETH2mUgRsPgR_Dictionary();
   static void maplEECbmModuleIdcOvectorlETH2mUgRsPgR_TClassManip(TClass*);
   static void *new_maplEECbmModuleIdcOvectorlETH2mUgRsPgR(void *p = nullptr);
   static void *newArray_maplEECbmModuleIdcOvectorlETH2mUgRsPgR(Long_t size, void *p);
   static void delete_maplEECbmModuleIdcOvectorlETH2mUgRsPgR(void *p);
   static void deleteArray_maplEECbmModuleIdcOvectorlETH2mUgRsPgR(void *p);
   static void destruct_maplEECbmModuleIdcOvectorlETH2mUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<ECbmModuleId,vector<TH2*> >*)
   {
      map<ECbmModuleId,vector<TH2*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<ECbmModuleId,vector<TH2*> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<ECbmModuleId,vector<TH2*> >", -2, "map", 100,
                  typeid(map<ECbmModuleId,vector<TH2*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEECbmModuleIdcOvectorlETH2mUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<ECbmModuleId,vector<TH2*> >) );
      instance.SetNew(&new_maplEECbmModuleIdcOvectorlETH2mUgRsPgR);
      instance.SetNewArray(&newArray_maplEECbmModuleIdcOvectorlETH2mUgRsPgR);
      instance.SetDelete(&delete_maplEECbmModuleIdcOvectorlETH2mUgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEECbmModuleIdcOvectorlETH2mUgRsPgR);
      instance.SetDestructor(&destruct_maplEECbmModuleIdcOvectorlETH2mUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<ECbmModuleId,vector<TH2*> > >()));

      ::ROOT::AddClassAlternate("map<ECbmModuleId,vector<TH2*> >","std::map<ECbmModuleId, std::vector<TH2*, std::allocator<TH2*> >, std::less<ECbmModuleId>, std::allocator<std::pair<ECbmModuleId const, std::vector<TH2*, std::allocator<TH2*> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<ECbmModuleId,vector<TH2*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEECbmModuleIdcOvectorlETH2mUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<ECbmModuleId,vector<TH2*> >*)nullptr)->GetClass();
      maplEECbmModuleIdcOvectorlETH2mUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEECbmModuleIdcOvectorlETH2mUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEECbmModuleIdcOvectorlETH2mUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,vector<TH2*> > : new map<ECbmModuleId,vector<TH2*> >;
   }
   static void *newArray_maplEECbmModuleIdcOvectorlETH2mUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,vector<TH2*> >[nElements] : new map<ECbmModuleId,vector<TH2*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEECbmModuleIdcOvectorlETH2mUgRsPgR(void *p) {
      delete ((map<ECbmModuleId,vector<TH2*> >*)p);
   }
   static void deleteArray_maplEECbmModuleIdcOvectorlETH2mUgRsPgR(void *p) {
      delete [] ((map<ECbmModuleId,vector<TH2*> >*)p);
   }
   static void destruct_maplEECbmModuleIdcOvectorlETH2mUgRsPgR(void *p) {
      typedef map<ECbmModuleId,vector<TH2*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<ECbmModuleId,vector<TH2*> >

namespace ROOT {
   static TClass *maplEECbmModuleIdcOvectorlETH1mUgRsPgR_Dictionary();
   static void maplEECbmModuleIdcOvectorlETH1mUgRsPgR_TClassManip(TClass*);
   static void *new_maplEECbmModuleIdcOvectorlETH1mUgRsPgR(void *p = nullptr);
   static void *newArray_maplEECbmModuleIdcOvectorlETH1mUgRsPgR(Long_t size, void *p);
   static void delete_maplEECbmModuleIdcOvectorlETH1mUgRsPgR(void *p);
   static void deleteArray_maplEECbmModuleIdcOvectorlETH1mUgRsPgR(void *p);
   static void destruct_maplEECbmModuleIdcOvectorlETH1mUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<ECbmModuleId,vector<TH1*> >*)
   {
      map<ECbmModuleId,vector<TH1*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<ECbmModuleId,vector<TH1*> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<ECbmModuleId,vector<TH1*> >", -2, "map", 100,
                  typeid(map<ECbmModuleId,vector<TH1*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEECbmModuleIdcOvectorlETH1mUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<ECbmModuleId,vector<TH1*> >) );
      instance.SetNew(&new_maplEECbmModuleIdcOvectorlETH1mUgRsPgR);
      instance.SetNewArray(&newArray_maplEECbmModuleIdcOvectorlETH1mUgRsPgR);
      instance.SetDelete(&delete_maplEECbmModuleIdcOvectorlETH1mUgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEECbmModuleIdcOvectorlETH1mUgRsPgR);
      instance.SetDestructor(&destruct_maplEECbmModuleIdcOvectorlETH1mUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<ECbmModuleId,vector<TH1*> > >()));

      ::ROOT::AddClassAlternate("map<ECbmModuleId,vector<TH1*> >","std::map<ECbmModuleId, std::vector<TH1*, std::allocator<TH1*> >, std::less<ECbmModuleId>, std::allocator<std::pair<ECbmModuleId const, std::vector<TH1*, std::allocator<TH1*> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<ECbmModuleId,vector<TH1*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEECbmModuleIdcOvectorlETH1mUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<ECbmModuleId,vector<TH1*> >*)nullptr)->GetClass();
      maplEECbmModuleIdcOvectorlETH1mUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEECbmModuleIdcOvectorlETH1mUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEECbmModuleIdcOvectorlETH1mUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,vector<TH1*> > : new map<ECbmModuleId,vector<TH1*> >;
   }
   static void *newArray_maplEECbmModuleIdcOvectorlETH1mUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,vector<TH1*> >[nElements] : new map<ECbmModuleId,vector<TH1*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEECbmModuleIdcOvectorlETH1mUgRsPgR(void *p) {
      delete ((map<ECbmModuleId,vector<TH1*> >*)p);
   }
   static void deleteArray_maplEECbmModuleIdcOvectorlETH1mUgRsPgR(void *p) {
      delete [] ((map<ECbmModuleId,vector<TH1*> >*)p);
   }
   static void destruct_maplEECbmModuleIdcOvectorlETH1mUgRsPgR(void *p) {
      typedef map<ECbmModuleId,vector<TH1*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<ECbmModuleId,vector<TH1*> >

namespace {
  void TriggerDictionaryInitialization_libCbmFlibMcbm2018_Impl() {
    static const char* headers[] = {
"CbmMcbmUnpack.h",
"CbmMcbm2018Source.h",
"CbmMcbm2018MsCrcCheck.h",
"CbmMcbm2018RawConverterSdpb.h",
"CbmMcbm2018RawConverterGdpb.h",
"CbmCriGet4RawPrint.h",
"CbmMcbm2018UnpackerAlgoSts.h",
"CbmMcbm2018UnpackerTaskSts.h",
"CbmMcbm2018UnpackerAlgoMuch.h",
"CbmMcbm2018UnpackerTaskMuch.h",
"CbmMcbm2018UnpackerAlgoTof.h",
"CbmMcbm2018UnpackerTaskTof.h",
"CbmMcbm2018UnpackerAlgoRich.h",
"CbmMcbm2018UnpackerTaskRich.h",
"CbmMcbm2018UnpackerTaskRich2020.h",
"CbmMcbm2018UnpackerAlgoRich2020.h",
"CbmMcbm2018UnpackerUtilRich2020.h",
"CbmMcbm2018UnpackerAlgoPsd.h",
"CbmMcbm2018UnpackerTaskPsd.h",
"CbmMcbm2018UnpackerAlgoTrdR.h",
"CbmMcbm2018UnpackerTaskTrdR.h",
"CbmCheckDataFormatGdpb2018.h",
"CbmMcbm2018MonitorDataRates.h",
"CbmMcbm2018MonitorAlgoT0.h",
"CbmMcbm2018MonitorTaskT0.h",
"CbmMcbm2018MonitorMcbmRate.h",
"CbmMcbm2018MonitorMcbmSync.h",
"CbmMcbm2018MonitorMcbmPulser.h",
"CbmMcbm2018MonitorSts.h",
"CbmMcbm2018MonitorMuch.h",
"CbmMcbm2018MonitorMuchLite.h",
"CbmMcbm2018MonitorAlgoMuchLite.h",
"CbmMcbm2018MonitorTaskMuchLite.h",
"CbmMcbm2018MonitorTof.h",
"CbmMcbm2018MonitorTofPulser.h",
"CbmMcbm2018MonitorAlgoTof.h",
"CbmMcbm2018MonitorTaskTof.h",
"CbmMcbm2018MonitorAlgoTofPulser.h",
"CbmMcbm2018MonitorTaskTofPulser.h",
"CbmMcbm2018TofTestFee.h",
"CbmMcbm2018TofFeeThr.h",
"CbmMcbm2018MonitorAlgoPsd.h",
"CbmMcbm2018MonitorTaskPsd.h",
"CbmCheckDigisNbCorr.h",
"CbmCheckTiming.h",
"CbmCheckEvents.h",
"CbmMcbm2018EventBuilder.h",
"CbmMcbm2019CheckPulser.h",
"CbmMcbm2019CheckDigisMuch.h",
"CbmMcbm2019CheckDigisSts.h",
"CbmMcbm2019CheckDtInDet.h",
"CbmMcbm2019CheckTimingPairs.h",
"CbmMcbm2019TimeWinEventBuilderAlgo.h",
"CbmMcbm2019TimeWinEventBuilderTask.h",
"CbmMcbmCheckTimingAlgo.h",
"CbmMcbmCheckTimingTask.h",
"CbmMcbmSpillFindAlgo.h",
"CbmMcbmSpillFindTask.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/dataformat",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/commonMQ",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/dataformat",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/commonMQ",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/psd",
"/home/heinemann/gpuunpacker/cbmroot/external/flib_dpb/flib_dpb",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils/flestools",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich/utils",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/external/cppzmq",
"/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/logging",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/fles/mcbm2018/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmFlibMcbm2018 dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmMcbmUnpack.h")))  CbmMcbmUnpack;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018Source.h")))  CbmMcbm2018Source;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MsCrcCheck.h")))  CbmMcbm2018MsCrcCheck;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018RawConverterSdpb.h")))  CbmMcbm2018RawConverterSdpb;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018RawConverterGdpb.h")))  CbmMcbm2018RawConverterGdpb;
class __attribute__((annotate("$clingAutoload$CbmCriGet4RawPrint.h")))  CbmCriGet4RawPrint;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018UnpackerAlgoSts.h")))  CbmMcbm2018UnpackerAlgoSts;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018UnpackerTaskSts.h")))  CbmMcbm2018UnpackerTaskSts;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018UnpackerAlgoMuch.h")))  CbmMcbm2018UnpackerAlgoMuch;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018UnpackerTaskMuch.h")))  CbmMcbm2018UnpackerTaskMuch;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018UnpackerAlgoTof.h")))  CbmMcbm2018UnpackerAlgoTof;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018UnpackerTaskTof.h")))  CbmMcbm2018UnpackerTaskTof;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018UnpackerAlgoRich.h")))  CbmMcbm2018UnpackerAlgoRich;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018UnpackerTaskRich.h")))  CbmMcbm2018UnpackerTaskRich;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018UnpackerTaskRich2020.h")))  CbmMcbm2018UnpackerTaskRich2020;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018UnpackerAlgoRich2020.h")))  CbmMcbm2018UnpackerAlgoRich2020;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018UnpackerAlgoPsd.h")))  CbmMcbm2018UnpackerAlgoPsd;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018UnpackerTaskPsd.h")))  CbmMcbm2018UnpackerTaskPsd;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018UnpackerAlgoTrdR.h")))  CbmMcbm2018UnpackerAlgoTrdR;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018UnpackerTaskTrdR.h")))  CbmMcbm2018UnpackerTaskTrdR;
class __attribute__((annotate("$clingAutoload$CbmCheckDataFormatGdpb2018.h")))  CbmCheckDataFormatGdpb2018;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorDataRates.h")))  CbmMcbm2018MonitorDataRates;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorAlgoT0.h")))  CbmMcbm2018MonitorAlgoT0;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorTaskT0.h")))  CbmMcbm2018MonitorTaskT0;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorMcbmRate.h")))  CbmMcbm2018MonitorMcbmRate;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorMcbmSync.h")))  CbmMcbm2018MonitorMcbmSync;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorMcbmPulser.h")))  CbmMcbm2018MonitorMcbmPulser;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorSts.h")))  CbmMcbm2018MonitorSts;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorMuch.h")))  CbmMcbm2018MonitorMuch;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorMuchLite.h")))  CbmMcbm2018MonitorMuchLite;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorAlgoMuchLite.h")))  CbmMcbm2018MonitorAlgoMuchLite;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorTaskMuchLite.h")))  CbmMcbm2018MonitorTaskMuchLite;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorTof.h")))  CbmMcbm2018MonitorTof;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorTofPulser.h")))  CbmMcbm2018MonitorTofPulser;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorAlgoTof.h")))  CbmMcbm2018MonitorAlgoTof;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorTaskTof.h")))  CbmMcbm2018MonitorTaskTof;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorAlgoTofPulser.h")))  CbmMcbm2018MonitorAlgoTofPulser;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorTaskTofPulser.h")))  CbmMcbm2018MonitorTaskTofPulser;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018TofTestFee.h")))  CbmMcbm2018TofTestFee;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018TofFeeThr.h")))  CbmMcbm2018TofFeeThr;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorAlgoPsd.h")))  CbmMcbm2018MonitorAlgoPsd;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MonitorTaskPsd.h")))  CbmMcbm2018MonitorTaskPsd;
class __attribute__((annotate("$clingAutoload$CbmCheckDigisNbCorr.h")))  CbmCheckDigisNbCorr;
class __attribute__((annotate("$clingAutoload$CbmCheckTiming.h")))  CbmCheckTiming;
class __attribute__((annotate("$clingAutoload$CbmCheckEvents.h")))  CbmCheckEvents;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018EventBuilder.h")))  CbmMcbm2018EventBuilder;
class __attribute__((annotate("$clingAutoload$CbmMcbm2019CheckPulser.h")))  CbmMcbm2019CheckPulser;
class __attribute__((annotate("$clingAutoload$CbmMcbm2019CheckDigisMuch.h")))  CbmMcbm2019CheckDigisMuch;
class __attribute__((annotate("$clingAutoload$CbmMcbm2019CheckDigisSts.h")))  CbmMcbm2019CheckDigisSts;
class __attribute__((annotate("$clingAutoload$CbmMcbm2019CheckDtInDet.h")))  CbmMcbm2019CheckDtInDet;
class __attribute__((annotate("$clingAutoload$CbmMcbm2019CheckTimingPairs.h")))  CbmMcbm2019CheckTimingPairs;
class __attribute__((annotate("$clingAutoload$CbmMcbm2019TimeWinEventBuilderAlgo.h")))  CbmMcbm2019TimeWinEventBuilderAlgo;
class __attribute__((annotate("$clingAutoload$CbmMcbm2019TimeWinEventBuilderTask.h")))  CbmMcbm2019TimeWinEventBuilderTask;
class __attribute__((annotate("$clingAutoload$CbmMcbmCheckTimingAlgo.h")))  CbmMcbmCheckTimingAlgo;
class __attribute__((annotate("$clingAutoload$CbmMcbmCheckTimingTask.h")))  CbmMcbmCheckTimingTask;
class __attribute__((annotate("$clingAutoload$CbmMcbmSpillFindAlgo.h")))  CbmMcbmSpillFindAlgo;
class __attribute__((annotate("$clingAutoload$CbmMcbmSpillFindTask.h")))  CbmMcbmSpillFindTask;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmFlibMcbm2018 dictionary payload"

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
#ifndef BOOST_LOG_DYN_LINK
  #define BOOST_LOG_DYN_LINK 1
#endif
#ifndef BOOST_LOG_USE_NATIVE_SYSLOG
  #define BOOST_LOG_USE_NATIVE_SYSLOG 1
#endif
#ifndef BOOST_ERROR_CODE_HEADER_ONLY
  #define BOOST_ERROR_CODE_HEADER_ONLY 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "CbmMcbmUnpack.h"
#include "CbmMcbm2018Source.h"
#include "CbmMcbm2018MsCrcCheck.h"
#include "CbmMcbm2018RawConverterSdpb.h"
#include "CbmMcbm2018RawConverterGdpb.h"
#include "CbmCriGet4RawPrint.h"
#include "CbmMcbm2018UnpackerAlgoSts.h"
#include "CbmMcbm2018UnpackerTaskSts.h"
#include "CbmMcbm2018UnpackerAlgoMuch.h"
#include "CbmMcbm2018UnpackerTaskMuch.h"
#include "CbmMcbm2018UnpackerAlgoTof.h"
#include "CbmMcbm2018UnpackerTaskTof.h"
#include "CbmMcbm2018UnpackerAlgoRich.h"
#include "CbmMcbm2018UnpackerTaskRich.h"
#include "CbmMcbm2018UnpackerTaskRich2020.h"
#include "CbmMcbm2018UnpackerAlgoRich2020.h"
#include "CbmMcbm2018UnpackerUtilRich2020.h"
#include "CbmMcbm2018UnpackerAlgoPsd.h"
#include "CbmMcbm2018UnpackerTaskPsd.h"
#include "CbmMcbm2018UnpackerAlgoTrdR.h"
#include "CbmMcbm2018UnpackerTaskTrdR.h"
#include "CbmCheckDataFormatGdpb2018.h"
#include "CbmMcbm2018MonitorDataRates.h"
#include "CbmMcbm2018MonitorAlgoT0.h"
#include "CbmMcbm2018MonitorTaskT0.h"
#include "CbmMcbm2018MonitorMcbmRate.h"
#include "CbmMcbm2018MonitorMcbmSync.h"
#include "CbmMcbm2018MonitorMcbmPulser.h"
#include "CbmMcbm2018MonitorSts.h"
#include "CbmMcbm2018MonitorMuch.h"
#include "CbmMcbm2018MonitorMuchLite.h"
#include "CbmMcbm2018MonitorAlgoMuchLite.h"
#include "CbmMcbm2018MonitorTaskMuchLite.h"
#include "CbmMcbm2018MonitorTof.h"
#include "CbmMcbm2018MonitorTofPulser.h"
#include "CbmMcbm2018MonitorAlgoTof.h"
#include "CbmMcbm2018MonitorTaskTof.h"
#include "CbmMcbm2018MonitorAlgoTofPulser.h"
#include "CbmMcbm2018MonitorTaskTofPulser.h"
#include "CbmMcbm2018TofTestFee.h"
#include "CbmMcbm2018TofFeeThr.h"
#include "CbmMcbm2018MonitorAlgoPsd.h"
#include "CbmMcbm2018MonitorTaskPsd.h"
#include "CbmCheckDigisNbCorr.h"
#include "CbmCheckTiming.h"
#include "CbmCheckEvents.h"
#include "CbmMcbm2018EventBuilder.h"
#include "CbmMcbm2019CheckPulser.h"
#include "CbmMcbm2019CheckDigisMuch.h"
#include "CbmMcbm2019CheckDigisSts.h"
#include "CbmMcbm2019CheckDtInDet.h"
#include "CbmMcbm2019CheckTimingPairs.h"
#include "CbmMcbm2019TimeWinEventBuilderAlgo.h"
#include "CbmMcbm2019TimeWinEventBuilderTask.h"
#include "CbmMcbmCheckTimingAlgo.h"
#include "CbmMcbmCheckTimingTask.h"
#include "CbmMcbmSpillFindAlgo.h"
#include "CbmMcbmSpillFindTask.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmCheckDataFormatGdpb2018", payloadCode, "@",
"CbmCheckDigisNbCorr", payloadCode, "@",
"CbmCheckEvents", payloadCode, "@",
"CbmCheckTiming", payloadCode, "@",
"CbmCriGet4RawPrint", payloadCode, "@",
"CbmMcbm2018EventBuilder", payloadCode, "@",
"CbmMcbm2018MonitorAlgoMuchLite", payloadCode, "@",
"CbmMcbm2018MonitorAlgoPsd", payloadCode, "@",
"CbmMcbm2018MonitorAlgoT0", payloadCode, "@",
"CbmMcbm2018MonitorAlgoTof", payloadCode, "@",
"CbmMcbm2018MonitorAlgoTofPulser", payloadCode, "@",
"CbmMcbm2018MonitorDataRates", payloadCode, "@",
"CbmMcbm2018MonitorMcbmPulser", payloadCode, "@",
"CbmMcbm2018MonitorMcbmRate", payloadCode, "@",
"CbmMcbm2018MonitorMcbmSync", payloadCode, "@",
"CbmMcbm2018MonitorMuch", payloadCode, "@",
"CbmMcbm2018MonitorMuchLite", payloadCode, "@",
"CbmMcbm2018MonitorSts", payloadCode, "@",
"CbmMcbm2018MonitorTaskMuchLite", payloadCode, "@",
"CbmMcbm2018MonitorTaskPsd", payloadCode, "@",
"CbmMcbm2018MonitorTaskT0", payloadCode, "@",
"CbmMcbm2018MonitorTaskTof", payloadCode, "@",
"CbmMcbm2018MonitorTaskTofPulser", payloadCode, "@",
"CbmMcbm2018MonitorTof", payloadCode, "@",
"CbmMcbm2018MonitorTofPulser", payloadCode, "@",
"CbmMcbm2018MsCrcCheck", payloadCode, "@",
"CbmMcbm2018RawConverterGdpb", payloadCode, "@",
"CbmMcbm2018RawConverterSdpb", payloadCode, "@",
"CbmMcbm2018Source", payloadCode, "@",
"CbmMcbm2018TofFeeThr", payloadCode, "@",
"CbmMcbm2018TofTestFee", payloadCode, "@",
"CbmMcbm2018UnpackerAlgoMuch", payloadCode, "@",
"CbmMcbm2018UnpackerAlgoPsd", payloadCode, "@",
"CbmMcbm2018UnpackerAlgoRich", payloadCode, "@",
"CbmMcbm2018UnpackerAlgoRich2020", payloadCode, "@",
"CbmMcbm2018UnpackerAlgoSts", payloadCode, "@",
"CbmMcbm2018UnpackerAlgoTof", payloadCode, "@",
"CbmMcbm2018UnpackerAlgoTrdR", payloadCode, "@",
"CbmMcbm2018UnpackerTaskMuch", payloadCode, "@",
"CbmMcbm2018UnpackerTaskPsd", payloadCode, "@",
"CbmMcbm2018UnpackerTaskRich", payloadCode, "@",
"CbmMcbm2018UnpackerTaskRich2020", payloadCode, "@",
"CbmMcbm2018UnpackerTaskSts", payloadCode, "@",
"CbmMcbm2018UnpackerTaskTof", payloadCode, "@",
"CbmMcbm2018UnpackerTaskTrdR", payloadCode, "@",
"CbmMcbm2019CheckDigisMuch", payloadCode, "@",
"CbmMcbm2019CheckDigisSts", payloadCode, "@",
"CbmMcbm2019CheckDtInDet", payloadCode, "@",
"CbmMcbm2019CheckPulser", payloadCode, "@",
"CbmMcbm2019CheckTimingPairs", payloadCode, "@",
"CbmMcbm2019TimeWinEventBuilderAlgo", payloadCode, "@",
"CbmMcbm2019TimeWinEventBuilderTask", payloadCode, "@",
"CbmMcbmCheckTimingAlgo", payloadCode, "@",
"CbmMcbmCheckTimingTask", payloadCode, "@",
"CbmMcbmSpillFindAlgo", payloadCode, "@",
"CbmMcbmSpillFindTask", payloadCode, "@",
"CbmMcbmUnpack", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmFlibMcbm2018",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmFlibMcbm2018_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmFlibMcbm2018_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmFlibMcbm2018() {
  TriggerDictionaryInitialization_libCbmFlibMcbm2018_Impl();
}
