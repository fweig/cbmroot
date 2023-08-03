// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmConversion2Analysis
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
#include "CbmKresConversionMain.h"
#include "CbmKresConversionManual.h"
#include "CbmKresConversionReconstruction.h"
#include "CbmKresConversionKF.h"
#include "CbmKresConversionGeneral.h"
#include "CbmKresConversionBG.h"
#include "CbmKresConversionManualmbias1.h"
#include "CbmKresConversionManualmbias2.h"
#include "CbmKresConversionManualmbias3.h"
#include "CbmKresConversionManualmbias4.h"
#include "CbmKresTrainAnn.h"
#include "CbmKresSelectAnn.h"
#include "CbmKresTemperature.h"
#include "CbmKresTrainAnnDirectPhotons.h"
#include "CbmKresConversionPhotons.h"
#include "CbmKresSelectAnnPhotons.h"
#include "CbmKresGammaCorrection.h"
#include "CbmKresConversionCorrectedPhotons.h"
#include "CbmKresEta.h"
#include "CbmKresEtaMCAnalysis.h"
#include "CbmKresSelectGoodEvents.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmKresGammaCorrection(void *p = nullptr);
   static void *newArray_CbmKresGammaCorrection(Long_t size, void *p);
   static void delete_CbmKresGammaCorrection(void *p);
   static void deleteArray_CbmKresGammaCorrection(void *p);
   static void destruct_CbmKresGammaCorrection(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresGammaCorrection*)
   {
      ::CbmKresGammaCorrection *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresGammaCorrection >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresGammaCorrection", ::CbmKresGammaCorrection::Class_Version(), "CbmKresGammaCorrection.h", 13,
                  typeid(::CbmKresGammaCorrection), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresGammaCorrection::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresGammaCorrection) );
      instance.SetNew(&new_CbmKresGammaCorrection);
      instance.SetNewArray(&newArray_CbmKresGammaCorrection);
      instance.SetDelete(&delete_CbmKresGammaCorrection);
      instance.SetDeleteArray(&deleteArray_CbmKresGammaCorrection);
      instance.SetDestructor(&destruct_CbmKresGammaCorrection);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresGammaCorrection*)
   {
      return GenerateInitInstanceLocal((::CbmKresGammaCorrection*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresGammaCorrection*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresSelectAnnPhotons(void *p = nullptr);
   static void *newArray_CbmKresSelectAnnPhotons(Long_t size, void *p);
   static void delete_CbmKresSelectAnnPhotons(void *p);
   static void deleteArray_CbmKresSelectAnnPhotons(void *p);
   static void destruct_CbmKresSelectAnnPhotons(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresSelectAnnPhotons*)
   {
      ::CbmKresSelectAnnPhotons *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresSelectAnnPhotons >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresSelectAnnPhotons", ::CbmKresSelectAnnPhotons::Class_Version(), "CbmKresSelectAnnPhotons.h", 14,
                  typeid(::CbmKresSelectAnnPhotons), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresSelectAnnPhotons::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresSelectAnnPhotons) );
      instance.SetNew(&new_CbmKresSelectAnnPhotons);
      instance.SetNewArray(&newArray_CbmKresSelectAnnPhotons);
      instance.SetDelete(&delete_CbmKresSelectAnnPhotons);
      instance.SetDeleteArray(&deleteArray_CbmKresSelectAnnPhotons);
      instance.SetDestructor(&destruct_CbmKresSelectAnnPhotons);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresSelectAnnPhotons*)
   {
      return GenerateInitInstanceLocal((::CbmKresSelectAnnPhotons*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresSelectAnnPhotons*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresTrainAnnDirectPhotons(void *p = nullptr);
   static void *newArray_CbmKresTrainAnnDirectPhotons(Long_t size, void *p);
   static void delete_CbmKresTrainAnnDirectPhotons(void *p);
   static void deleteArray_CbmKresTrainAnnDirectPhotons(void *p);
   static void destruct_CbmKresTrainAnnDirectPhotons(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresTrainAnnDirectPhotons*)
   {
      ::CbmKresTrainAnnDirectPhotons *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresTrainAnnDirectPhotons >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresTrainAnnDirectPhotons", ::CbmKresTrainAnnDirectPhotons::Class_Version(), "CbmKresTrainAnnDirectPhotons.h", 17,
                  typeid(::CbmKresTrainAnnDirectPhotons), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresTrainAnnDirectPhotons::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresTrainAnnDirectPhotons) );
      instance.SetNew(&new_CbmKresTrainAnnDirectPhotons);
      instance.SetNewArray(&newArray_CbmKresTrainAnnDirectPhotons);
      instance.SetDelete(&delete_CbmKresTrainAnnDirectPhotons);
      instance.SetDeleteArray(&deleteArray_CbmKresTrainAnnDirectPhotons);
      instance.SetDestructor(&destruct_CbmKresTrainAnnDirectPhotons);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresTrainAnnDirectPhotons*)
   {
      return GenerateInitInstanceLocal((::CbmKresTrainAnnDirectPhotons*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresTrainAnnDirectPhotons*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresConversionCorrectedPhotons(void *p = nullptr);
   static void *newArray_CbmKresConversionCorrectedPhotons(Long_t size, void *p);
   static void delete_CbmKresConversionCorrectedPhotons(void *p);
   static void deleteArray_CbmKresConversionCorrectedPhotons(void *p);
   static void destruct_CbmKresConversionCorrectedPhotons(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresConversionCorrectedPhotons*)
   {
      ::CbmKresConversionCorrectedPhotons *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresConversionCorrectedPhotons >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresConversionCorrectedPhotons", ::CbmKresConversionCorrectedPhotons::Class_Version(), "CbmKresConversionCorrectedPhotons.h", 28,
                  typeid(::CbmKresConversionCorrectedPhotons), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresConversionCorrectedPhotons::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresConversionCorrectedPhotons) );
      instance.SetNew(&new_CbmKresConversionCorrectedPhotons);
      instance.SetNewArray(&newArray_CbmKresConversionCorrectedPhotons);
      instance.SetDelete(&delete_CbmKresConversionCorrectedPhotons);
      instance.SetDeleteArray(&deleteArray_CbmKresConversionCorrectedPhotons);
      instance.SetDestructor(&destruct_CbmKresConversionCorrectedPhotons);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresConversionCorrectedPhotons*)
   {
      return GenerateInitInstanceLocal((::CbmKresConversionCorrectedPhotons*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresConversionCorrectedPhotons*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresConversionGeneral(void *p = nullptr);
   static void *newArray_CbmKresConversionGeneral(Long_t size, void *p);
   static void delete_CbmKresConversionGeneral(void *p);
   static void deleteArray_CbmKresConversionGeneral(void *p);
   static void destruct_CbmKresConversionGeneral(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresConversionGeneral*)
   {
      ::CbmKresConversionGeneral *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresConversionGeneral >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresConversionGeneral", ::CbmKresConversionGeneral::Class_Version(), "CbmKresConversionGeneral.h", 27,
                  typeid(::CbmKresConversionGeneral), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresConversionGeneral::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresConversionGeneral) );
      instance.SetNew(&new_CbmKresConversionGeneral);
      instance.SetNewArray(&newArray_CbmKresConversionGeneral);
      instance.SetDelete(&delete_CbmKresConversionGeneral);
      instance.SetDeleteArray(&deleteArray_CbmKresConversionGeneral);
      instance.SetDestructor(&destruct_CbmKresConversionGeneral);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresConversionGeneral*)
   {
      return GenerateInitInstanceLocal((::CbmKresConversionGeneral*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresConversionGeneral*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresConversionBG(void *p = nullptr);
   static void *newArray_CbmKresConversionBG(Long_t size, void *p);
   static void delete_CbmKresConversionBG(void *p);
   static void deleteArray_CbmKresConversionBG(void *p);
   static void destruct_CbmKresConversionBG(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresConversionBG*)
   {
      ::CbmKresConversionBG *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresConversionBG >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresConversionBG", ::CbmKresConversionBG::Class_Version(), "CbmKresConversionBG.h", 15,
                  typeid(::CbmKresConversionBG), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresConversionBG::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresConversionBG) );
      instance.SetNew(&new_CbmKresConversionBG);
      instance.SetNewArray(&newArray_CbmKresConversionBG);
      instance.SetDelete(&delete_CbmKresConversionBG);
      instance.SetDeleteArray(&deleteArray_CbmKresConversionBG);
      instance.SetDestructor(&destruct_CbmKresConversionBG);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresConversionBG*)
   {
      return GenerateInitInstanceLocal((::CbmKresConversionBG*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresConversionBG*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresConversionKF(void *p = nullptr);
   static void *newArray_CbmKresConversionKF(Long_t size, void *p);
   static void delete_CbmKresConversionKF(void *p);
   static void deleteArray_CbmKresConversionKF(void *p);
   static void destruct_CbmKresConversionKF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresConversionKF*)
   {
      ::CbmKresConversionKF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresConversionKF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresConversionKF", ::CbmKresConversionKF::Class_Version(), "CbmKresConversionKF.h", 25,
                  typeid(::CbmKresConversionKF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresConversionKF::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresConversionKF) );
      instance.SetNew(&new_CbmKresConversionKF);
      instance.SetNewArray(&newArray_CbmKresConversionKF);
      instance.SetDelete(&delete_CbmKresConversionKF);
      instance.SetDeleteArray(&deleteArray_CbmKresConversionKF);
      instance.SetDestructor(&destruct_CbmKresConversionKF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresConversionKF*)
   {
      return GenerateInitInstanceLocal((::CbmKresConversionKF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresConversionKF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresSelectAnn(void *p = nullptr);
   static void *newArray_CbmKresSelectAnn(Long_t size, void *p);
   static void delete_CbmKresSelectAnn(void *p);
   static void deleteArray_CbmKresSelectAnn(void *p);
   static void destruct_CbmKresSelectAnn(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresSelectAnn*)
   {
      ::CbmKresSelectAnn *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresSelectAnn >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresSelectAnn", ::CbmKresSelectAnn::Class_Version(), "CbmKresSelectAnn.h", 14,
                  typeid(::CbmKresSelectAnn), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresSelectAnn::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresSelectAnn) );
      instance.SetNew(&new_CbmKresSelectAnn);
      instance.SetNewArray(&newArray_CbmKresSelectAnn);
      instance.SetDelete(&delete_CbmKresSelectAnn);
      instance.SetDeleteArray(&deleteArray_CbmKresSelectAnn);
      instance.SetDestructor(&destruct_CbmKresSelectAnn);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresSelectAnn*)
   {
      return GenerateInitInstanceLocal((::CbmKresSelectAnn*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresSelectAnn*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresTrainAnn(void *p = nullptr);
   static void *newArray_CbmKresTrainAnn(Long_t size, void *p);
   static void delete_CbmKresTrainAnn(void *p);
   static void deleteArray_CbmKresTrainAnn(void *p);
   static void destruct_CbmKresTrainAnn(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresTrainAnn*)
   {
      ::CbmKresTrainAnn *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresTrainAnn >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresTrainAnn", ::CbmKresTrainAnn::Class_Version(), "CbmKresTrainAnn.h", 17,
                  typeid(::CbmKresTrainAnn), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresTrainAnn::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresTrainAnn) );
      instance.SetNew(&new_CbmKresTrainAnn);
      instance.SetNewArray(&newArray_CbmKresTrainAnn);
      instance.SetDelete(&delete_CbmKresTrainAnn);
      instance.SetDeleteArray(&deleteArray_CbmKresTrainAnn);
      instance.SetDestructor(&destruct_CbmKresTrainAnn);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresTrainAnn*)
   {
      return GenerateInitInstanceLocal((::CbmKresTrainAnn*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresTrainAnn*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresConversionManual(void *p = nullptr);
   static void *newArray_CbmKresConversionManual(Long_t size, void *p);
   static void delete_CbmKresConversionManual(void *p);
   static void deleteArray_CbmKresConversionManual(void *p);
   static void destruct_CbmKresConversionManual(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresConversionManual*)
   {
      ::CbmKresConversionManual *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresConversionManual >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresConversionManual", ::CbmKresConversionManual::Class_Version(), "CbmKresConversionManual.h", 24,
                  typeid(::CbmKresConversionManual), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresConversionManual::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresConversionManual) );
      instance.SetNew(&new_CbmKresConversionManual);
      instance.SetNewArray(&newArray_CbmKresConversionManual);
      instance.SetDelete(&delete_CbmKresConversionManual);
      instance.SetDeleteArray(&deleteArray_CbmKresConversionManual);
      instance.SetDestructor(&destruct_CbmKresConversionManual);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresConversionManual*)
   {
      return GenerateInitInstanceLocal((::CbmKresConversionManual*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresConversionManual*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresConversionManualmbias1(void *p = nullptr);
   static void *newArray_CbmKresConversionManualmbias1(Long_t size, void *p);
   static void delete_CbmKresConversionManualmbias1(void *p);
   static void deleteArray_CbmKresConversionManualmbias1(void *p);
   static void destruct_CbmKresConversionManualmbias1(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresConversionManualmbias1*)
   {
      ::CbmKresConversionManualmbias1 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresConversionManualmbias1 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresConversionManualmbias1", ::CbmKresConversionManualmbias1::Class_Version(), "CbmKresConversionManualmbias1.h", 22,
                  typeid(::CbmKresConversionManualmbias1), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresConversionManualmbias1::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresConversionManualmbias1) );
      instance.SetNew(&new_CbmKresConversionManualmbias1);
      instance.SetNewArray(&newArray_CbmKresConversionManualmbias1);
      instance.SetDelete(&delete_CbmKresConversionManualmbias1);
      instance.SetDeleteArray(&deleteArray_CbmKresConversionManualmbias1);
      instance.SetDestructor(&destruct_CbmKresConversionManualmbias1);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresConversionManualmbias1*)
   {
      return GenerateInitInstanceLocal((::CbmKresConversionManualmbias1*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias1*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresConversionManualmbias2(void *p = nullptr);
   static void *newArray_CbmKresConversionManualmbias2(Long_t size, void *p);
   static void delete_CbmKresConversionManualmbias2(void *p);
   static void deleteArray_CbmKresConversionManualmbias2(void *p);
   static void destruct_CbmKresConversionManualmbias2(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresConversionManualmbias2*)
   {
      ::CbmKresConversionManualmbias2 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresConversionManualmbias2 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresConversionManualmbias2", ::CbmKresConversionManualmbias2::Class_Version(), "CbmKresConversionManualmbias2.h", 22,
                  typeid(::CbmKresConversionManualmbias2), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresConversionManualmbias2::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresConversionManualmbias2) );
      instance.SetNew(&new_CbmKresConversionManualmbias2);
      instance.SetNewArray(&newArray_CbmKresConversionManualmbias2);
      instance.SetDelete(&delete_CbmKresConversionManualmbias2);
      instance.SetDeleteArray(&deleteArray_CbmKresConversionManualmbias2);
      instance.SetDestructor(&destruct_CbmKresConversionManualmbias2);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresConversionManualmbias2*)
   {
      return GenerateInitInstanceLocal((::CbmKresConversionManualmbias2*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias2*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresConversionManualmbias3(void *p = nullptr);
   static void *newArray_CbmKresConversionManualmbias3(Long_t size, void *p);
   static void delete_CbmKresConversionManualmbias3(void *p);
   static void deleteArray_CbmKresConversionManualmbias3(void *p);
   static void destruct_CbmKresConversionManualmbias3(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresConversionManualmbias3*)
   {
      ::CbmKresConversionManualmbias3 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresConversionManualmbias3 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresConversionManualmbias3", ::CbmKresConversionManualmbias3::Class_Version(), "CbmKresConversionManualmbias3.h", 22,
                  typeid(::CbmKresConversionManualmbias3), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresConversionManualmbias3::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresConversionManualmbias3) );
      instance.SetNew(&new_CbmKresConversionManualmbias3);
      instance.SetNewArray(&newArray_CbmKresConversionManualmbias3);
      instance.SetDelete(&delete_CbmKresConversionManualmbias3);
      instance.SetDeleteArray(&deleteArray_CbmKresConversionManualmbias3);
      instance.SetDestructor(&destruct_CbmKresConversionManualmbias3);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresConversionManualmbias3*)
   {
      return GenerateInitInstanceLocal((::CbmKresConversionManualmbias3*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias3*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresConversionManualmbias4(void *p = nullptr);
   static void *newArray_CbmKresConversionManualmbias4(Long_t size, void *p);
   static void delete_CbmKresConversionManualmbias4(void *p);
   static void deleteArray_CbmKresConversionManualmbias4(void *p);
   static void destruct_CbmKresConversionManualmbias4(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresConversionManualmbias4*)
   {
      ::CbmKresConversionManualmbias4 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresConversionManualmbias4 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresConversionManualmbias4", ::CbmKresConversionManualmbias4::Class_Version(), "CbmKresConversionManualmbias4.h", 22,
                  typeid(::CbmKresConversionManualmbias4), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresConversionManualmbias4::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresConversionManualmbias4) );
      instance.SetNew(&new_CbmKresConversionManualmbias4);
      instance.SetNewArray(&newArray_CbmKresConversionManualmbias4);
      instance.SetDelete(&delete_CbmKresConversionManualmbias4);
      instance.SetDeleteArray(&deleteArray_CbmKresConversionManualmbias4);
      instance.SetDestructor(&destruct_CbmKresConversionManualmbias4);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresConversionManualmbias4*)
   {
      return GenerateInitInstanceLocal((::CbmKresConversionManualmbias4*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias4*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresConversionPhotons(void *p = nullptr);
   static void *newArray_CbmKresConversionPhotons(Long_t size, void *p);
   static void delete_CbmKresConversionPhotons(void *p);
   static void deleteArray_CbmKresConversionPhotons(void *p);
   static void destruct_CbmKresConversionPhotons(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresConversionPhotons*)
   {
      ::CbmKresConversionPhotons *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresConversionPhotons >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresConversionPhotons", ::CbmKresConversionPhotons::Class_Version(), "CbmKresConversionPhotons.h", 27,
                  typeid(::CbmKresConversionPhotons), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresConversionPhotons::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresConversionPhotons) );
      instance.SetNew(&new_CbmKresConversionPhotons);
      instance.SetNewArray(&newArray_CbmKresConversionPhotons);
      instance.SetDelete(&delete_CbmKresConversionPhotons);
      instance.SetDeleteArray(&deleteArray_CbmKresConversionPhotons);
      instance.SetDestructor(&destruct_CbmKresConversionPhotons);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresConversionPhotons*)
   {
      return GenerateInitInstanceLocal((::CbmKresConversionPhotons*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresConversionPhotons*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresConversionReconstruction(void *p = nullptr);
   static void *newArray_CbmKresConversionReconstruction(Long_t size, void *p);
   static void delete_CbmKresConversionReconstruction(void *p);
   static void deleteArray_CbmKresConversionReconstruction(void *p);
   static void destruct_CbmKresConversionReconstruction(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresConversionReconstruction*)
   {
      ::CbmKresConversionReconstruction *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresConversionReconstruction >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresConversionReconstruction", ::CbmKresConversionReconstruction::Class_Version(), "CbmKresConversionReconstruction.h", 22,
                  typeid(::CbmKresConversionReconstruction), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresConversionReconstruction::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresConversionReconstruction) );
      instance.SetNew(&new_CbmKresConversionReconstruction);
      instance.SetNewArray(&newArray_CbmKresConversionReconstruction);
      instance.SetDelete(&delete_CbmKresConversionReconstruction);
      instance.SetDeleteArray(&deleteArray_CbmKresConversionReconstruction);
      instance.SetDestructor(&destruct_CbmKresConversionReconstruction);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresConversionReconstruction*)
   {
      return GenerateInitInstanceLocal((::CbmKresConversionReconstruction*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresConversionReconstruction*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresEta(void *p = nullptr);
   static void *newArray_CbmKresEta(Long_t size, void *p);
   static void delete_CbmKresEta(void *p);
   static void deleteArray_CbmKresEta(void *p);
   static void destruct_CbmKresEta(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresEta*)
   {
      ::CbmKresEta *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresEta >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresEta", ::CbmKresEta::Class_Version(), "CbmKresEta.h", 21,
                  typeid(::CbmKresEta), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresEta::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresEta) );
      instance.SetNew(&new_CbmKresEta);
      instance.SetNewArray(&newArray_CbmKresEta);
      instance.SetDelete(&delete_CbmKresEta);
      instance.SetDeleteArray(&deleteArray_CbmKresEta);
      instance.SetDestructor(&destruct_CbmKresEta);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresEta*)
   {
      return GenerateInitInstanceLocal((::CbmKresEta*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresEta*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresEtaMCAnalysis(void *p = nullptr);
   static void *newArray_CbmKresEtaMCAnalysis(Long_t size, void *p);
   static void delete_CbmKresEtaMCAnalysis(void *p);
   static void deleteArray_CbmKresEtaMCAnalysis(void *p);
   static void destruct_CbmKresEtaMCAnalysis(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresEtaMCAnalysis*)
   {
      ::CbmKresEtaMCAnalysis *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresEtaMCAnalysis >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresEtaMCAnalysis", ::CbmKresEtaMCAnalysis::Class_Version(), "CbmKresEtaMCAnalysis.h", 20,
                  typeid(::CbmKresEtaMCAnalysis), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresEtaMCAnalysis::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresEtaMCAnalysis) );
      instance.SetNew(&new_CbmKresEtaMCAnalysis);
      instance.SetNewArray(&newArray_CbmKresEtaMCAnalysis);
      instance.SetDelete(&delete_CbmKresEtaMCAnalysis);
      instance.SetDeleteArray(&deleteArray_CbmKresEtaMCAnalysis);
      instance.SetDestructor(&destruct_CbmKresEtaMCAnalysis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresEtaMCAnalysis*)
   {
      return GenerateInitInstanceLocal((::CbmKresEtaMCAnalysis*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresEtaMCAnalysis*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresTemperature(void *p = nullptr);
   static void *newArray_CbmKresTemperature(Long_t size, void *p);
   static void delete_CbmKresTemperature(void *p);
   static void deleteArray_CbmKresTemperature(void *p);
   static void destruct_CbmKresTemperature(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresTemperature*)
   {
      ::CbmKresTemperature *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresTemperature >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresTemperature", ::CbmKresTemperature::Class_Version(), "CbmKresTemperature.h", 18,
                  typeid(::CbmKresTemperature), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresTemperature::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresTemperature) );
      instance.SetNew(&new_CbmKresTemperature);
      instance.SetNewArray(&newArray_CbmKresTemperature);
      instance.SetDelete(&delete_CbmKresTemperature);
      instance.SetDeleteArray(&deleteArray_CbmKresTemperature);
      instance.SetDestructor(&destruct_CbmKresTemperature);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresTemperature*)
   {
      return GenerateInitInstanceLocal((::CbmKresTemperature*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresTemperature*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresConversionMain(void *p = nullptr);
   static void *newArray_CbmKresConversionMain(Long_t size, void *p);
   static void delete_CbmKresConversionMain(void *p);
   static void deleteArray_CbmKresConversionMain(void *p);
   static void destruct_CbmKresConversionMain(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresConversionMain*)
   {
      ::CbmKresConversionMain *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresConversionMain >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresConversionMain", ::CbmKresConversionMain::Class_Version(), "CbmKresConversionMain.h", 31,
                  typeid(::CbmKresConversionMain), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresConversionMain::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresConversionMain) );
      instance.SetNew(&new_CbmKresConversionMain);
      instance.SetNewArray(&newArray_CbmKresConversionMain);
      instance.SetDelete(&delete_CbmKresConversionMain);
      instance.SetDeleteArray(&deleteArray_CbmKresConversionMain);
      instance.SetDestructor(&destruct_CbmKresConversionMain);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresConversionMain*)
   {
      return GenerateInitInstanceLocal((::CbmKresConversionMain*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresConversionMain*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKresSelectGoodEvents(void *p = nullptr);
   static void *newArray_CbmKresSelectGoodEvents(Long_t size, void *p);
   static void delete_CbmKresSelectGoodEvents(void *p);
   static void deleteArray_CbmKresSelectGoodEvents(void *p);
   static void destruct_CbmKresSelectGoodEvents(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKresSelectGoodEvents*)
   {
      ::CbmKresSelectGoodEvents *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKresSelectGoodEvents >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKresSelectGoodEvents", ::CbmKresSelectGoodEvents::Class_Version(), "CbmKresSelectGoodEvents.h", 19,
                  typeid(::CbmKresSelectGoodEvents), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKresSelectGoodEvents::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKresSelectGoodEvents) );
      instance.SetNew(&new_CbmKresSelectGoodEvents);
      instance.SetNewArray(&newArray_CbmKresSelectGoodEvents);
      instance.SetDelete(&delete_CbmKresSelectGoodEvents);
      instance.SetDeleteArray(&deleteArray_CbmKresSelectGoodEvents);
      instance.SetDestructor(&destruct_CbmKresSelectGoodEvents);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKresSelectGoodEvents*)
   {
      return GenerateInitInstanceLocal((::CbmKresSelectGoodEvents*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKresSelectGoodEvents*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmKresGammaCorrection::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresGammaCorrection::Class_Name()
{
   return "CbmKresGammaCorrection";
}

//______________________________________________________________________________
const char *CbmKresGammaCorrection::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresGammaCorrection*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresGammaCorrection::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresGammaCorrection*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresGammaCorrection::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresGammaCorrection*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresGammaCorrection::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresGammaCorrection*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresSelectAnnPhotons::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresSelectAnnPhotons::Class_Name()
{
   return "CbmKresSelectAnnPhotons";
}

//______________________________________________________________________________
const char *CbmKresSelectAnnPhotons::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresSelectAnnPhotons*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresSelectAnnPhotons::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresSelectAnnPhotons*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresSelectAnnPhotons::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresSelectAnnPhotons*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresSelectAnnPhotons::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresSelectAnnPhotons*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresTrainAnnDirectPhotons::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresTrainAnnDirectPhotons::Class_Name()
{
   return "CbmKresTrainAnnDirectPhotons";
}

//______________________________________________________________________________
const char *CbmKresTrainAnnDirectPhotons::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresTrainAnnDirectPhotons*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresTrainAnnDirectPhotons::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresTrainAnnDirectPhotons*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresTrainAnnDirectPhotons::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresTrainAnnDirectPhotons*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresTrainAnnDirectPhotons::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresTrainAnnDirectPhotons*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresConversionCorrectedPhotons::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresConversionCorrectedPhotons::Class_Name()
{
   return "CbmKresConversionCorrectedPhotons";
}

//______________________________________________________________________________
const char *CbmKresConversionCorrectedPhotons::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionCorrectedPhotons*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresConversionCorrectedPhotons::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionCorrectedPhotons*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresConversionCorrectedPhotons::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionCorrectedPhotons*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresConversionCorrectedPhotons::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionCorrectedPhotons*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresConversionGeneral::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresConversionGeneral::Class_Name()
{
   return "CbmKresConversionGeneral";
}

//______________________________________________________________________________
const char *CbmKresConversionGeneral::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionGeneral*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresConversionGeneral::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionGeneral*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresConversionGeneral::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionGeneral*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresConversionGeneral::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionGeneral*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresConversionBG::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresConversionBG::Class_Name()
{
   return "CbmKresConversionBG";
}

//______________________________________________________________________________
const char *CbmKresConversionBG::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionBG*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresConversionBG::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionBG*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresConversionBG::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionBG*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresConversionBG::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionBG*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresConversionKF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresConversionKF::Class_Name()
{
   return "CbmKresConversionKF";
}

//______________________________________________________________________________
const char *CbmKresConversionKF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionKF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresConversionKF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionKF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresConversionKF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionKF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresConversionKF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionKF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresSelectAnn::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresSelectAnn::Class_Name()
{
   return "CbmKresSelectAnn";
}

//______________________________________________________________________________
const char *CbmKresSelectAnn::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresSelectAnn*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresSelectAnn::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresSelectAnn*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresSelectAnn::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresSelectAnn*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresSelectAnn::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresSelectAnn*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresTrainAnn::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresTrainAnn::Class_Name()
{
   return "CbmKresTrainAnn";
}

//______________________________________________________________________________
const char *CbmKresTrainAnn::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresTrainAnn*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresTrainAnn::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresTrainAnn*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresTrainAnn::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresTrainAnn*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresTrainAnn::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresTrainAnn*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresConversionManual::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresConversionManual::Class_Name()
{
   return "CbmKresConversionManual";
}

//______________________________________________________________________________
const char *CbmKresConversionManual::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManual*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresConversionManual::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManual*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresConversionManual::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManual*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresConversionManual::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManual*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresConversionManualmbias1::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresConversionManualmbias1::Class_Name()
{
   return "CbmKresConversionManualmbias1";
}

//______________________________________________________________________________
const char *CbmKresConversionManualmbias1::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias1*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresConversionManualmbias1::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias1*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresConversionManualmbias1::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias1*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresConversionManualmbias1::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias1*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresConversionManualmbias2::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresConversionManualmbias2::Class_Name()
{
   return "CbmKresConversionManualmbias2";
}

//______________________________________________________________________________
const char *CbmKresConversionManualmbias2::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias2*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresConversionManualmbias2::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias2*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresConversionManualmbias2::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias2*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresConversionManualmbias2::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias2*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresConversionManualmbias3::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresConversionManualmbias3::Class_Name()
{
   return "CbmKresConversionManualmbias3";
}

//______________________________________________________________________________
const char *CbmKresConversionManualmbias3::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias3*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresConversionManualmbias3::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias3*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresConversionManualmbias3::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias3*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresConversionManualmbias3::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias3*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresConversionManualmbias4::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresConversionManualmbias4::Class_Name()
{
   return "CbmKresConversionManualmbias4";
}

//______________________________________________________________________________
const char *CbmKresConversionManualmbias4::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias4*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresConversionManualmbias4::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias4*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresConversionManualmbias4::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias4*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresConversionManualmbias4::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionManualmbias4*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresConversionPhotons::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresConversionPhotons::Class_Name()
{
   return "CbmKresConversionPhotons";
}

//______________________________________________________________________________
const char *CbmKresConversionPhotons::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionPhotons*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresConversionPhotons::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionPhotons*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresConversionPhotons::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionPhotons*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresConversionPhotons::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionPhotons*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresConversionReconstruction::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresConversionReconstruction::Class_Name()
{
   return "CbmKresConversionReconstruction";
}

//______________________________________________________________________________
const char *CbmKresConversionReconstruction::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionReconstruction*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresConversionReconstruction::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionReconstruction*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresConversionReconstruction::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionReconstruction*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresConversionReconstruction::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionReconstruction*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresEta::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresEta::Class_Name()
{
   return "CbmKresEta";
}

//______________________________________________________________________________
const char *CbmKresEta::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresEta*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresEta::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresEta*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresEta::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresEta*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresEta::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresEta*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresEtaMCAnalysis::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresEtaMCAnalysis::Class_Name()
{
   return "CbmKresEtaMCAnalysis";
}

//______________________________________________________________________________
const char *CbmKresEtaMCAnalysis::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresEtaMCAnalysis*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresEtaMCAnalysis::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresEtaMCAnalysis*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresEtaMCAnalysis::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresEtaMCAnalysis*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresEtaMCAnalysis::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresEtaMCAnalysis*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresTemperature::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresTemperature::Class_Name()
{
   return "CbmKresTemperature";
}

//______________________________________________________________________________
const char *CbmKresTemperature::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresTemperature*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresTemperature::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresTemperature*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresTemperature::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresTemperature*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresTemperature::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresTemperature*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresConversionMain::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresConversionMain::Class_Name()
{
   return "CbmKresConversionMain";
}

//______________________________________________________________________________
const char *CbmKresConversionMain::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionMain*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresConversionMain::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionMain*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresConversionMain::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionMain*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresConversionMain::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresConversionMain*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKresSelectGoodEvents::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKresSelectGoodEvents::Class_Name()
{
   return "CbmKresSelectGoodEvents";
}

//______________________________________________________________________________
const char *CbmKresSelectGoodEvents::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresSelectGoodEvents*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKresSelectGoodEvents::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKresSelectGoodEvents*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKresSelectGoodEvents::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresSelectGoodEvents*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKresSelectGoodEvents::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKresSelectGoodEvents*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmKresGammaCorrection::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresGammaCorrection.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresGammaCorrection::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresGammaCorrection::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresGammaCorrection(void *p) {
      return  p ? new(p) ::CbmKresGammaCorrection : new ::CbmKresGammaCorrection;
   }
   static void *newArray_CbmKresGammaCorrection(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresGammaCorrection[nElements] : new ::CbmKresGammaCorrection[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresGammaCorrection(void *p) {
      delete ((::CbmKresGammaCorrection*)p);
   }
   static void deleteArray_CbmKresGammaCorrection(void *p) {
      delete [] ((::CbmKresGammaCorrection*)p);
   }
   static void destruct_CbmKresGammaCorrection(void *p) {
      typedef ::CbmKresGammaCorrection current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresGammaCorrection

//______________________________________________________________________________
void CbmKresSelectAnnPhotons::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresSelectAnnPhotons.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresSelectAnnPhotons::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresSelectAnnPhotons::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresSelectAnnPhotons(void *p) {
      return  p ? new(p) ::CbmKresSelectAnnPhotons : new ::CbmKresSelectAnnPhotons;
   }
   static void *newArray_CbmKresSelectAnnPhotons(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresSelectAnnPhotons[nElements] : new ::CbmKresSelectAnnPhotons[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresSelectAnnPhotons(void *p) {
      delete ((::CbmKresSelectAnnPhotons*)p);
   }
   static void deleteArray_CbmKresSelectAnnPhotons(void *p) {
      delete [] ((::CbmKresSelectAnnPhotons*)p);
   }
   static void destruct_CbmKresSelectAnnPhotons(void *p) {
      typedef ::CbmKresSelectAnnPhotons current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresSelectAnnPhotons

//______________________________________________________________________________
void CbmKresTrainAnnDirectPhotons::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresTrainAnnDirectPhotons.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresTrainAnnDirectPhotons::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresTrainAnnDirectPhotons::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresTrainAnnDirectPhotons(void *p) {
      return  p ? new(p) ::CbmKresTrainAnnDirectPhotons : new ::CbmKresTrainAnnDirectPhotons;
   }
   static void *newArray_CbmKresTrainAnnDirectPhotons(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresTrainAnnDirectPhotons[nElements] : new ::CbmKresTrainAnnDirectPhotons[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresTrainAnnDirectPhotons(void *p) {
      delete ((::CbmKresTrainAnnDirectPhotons*)p);
   }
   static void deleteArray_CbmKresTrainAnnDirectPhotons(void *p) {
      delete [] ((::CbmKresTrainAnnDirectPhotons*)p);
   }
   static void destruct_CbmKresTrainAnnDirectPhotons(void *p) {
      typedef ::CbmKresTrainAnnDirectPhotons current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresTrainAnnDirectPhotons

//______________________________________________________________________________
void CbmKresConversionCorrectedPhotons::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresConversionCorrectedPhotons.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresConversionCorrectedPhotons::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresConversionCorrectedPhotons::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresConversionCorrectedPhotons(void *p) {
      return  p ? new(p) ::CbmKresConversionCorrectedPhotons : new ::CbmKresConversionCorrectedPhotons;
   }
   static void *newArray_CbmKresConversionCorrectedPhotons(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresConversionCorrectedPhotons[nElements] : new ::CbmKresConversionCorrectedPhotons[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresConversionCorrectedPhotons(void *p) {
      delete ((::CbmKresConversionCorrectedPhotons*)p);
   }
   static void deleteArray_CbmKresConversionCorrectedPhotons(void *p) {
      delete [] ((::CbmKresConversionCorrectedPhotons*)p);
   }
   static void destruct_CbmKresConversionCorrectedPhotons(void *p) {
      typedef ::CbmKresConversionCorrectedPhotons current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresConversionCorrectedPhotons

//______________________________________________________________________________
void CbmKresConversionGeneral::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresConversionGeneral.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresConversionGeneral::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresConversionGeneral::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresConversionGeneral(void *p) {
      return  p ? new(p) ::CbmKresConversionGeneral : new ::CbmKresConversionGeneral;
   }
   static void *newArray_CbmKresConversionGeneral(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresConversionGeneral[nElements] : new ::CbmKresConversionGeneral[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresConversionGeneral(void *p) {
      delete ((::CbmKresConversionGeneral*)p);
   }
   static void deleteArray_CbmKresConversionGeneral(void *p) {
      delete [] ((::CbmKresConversionGeneral*)p);
   }
   static void destruct_CbmKresConversionGeneral(void *p) {
      typedef ::CbmKresConversionGeneral current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresConversionGeneral

//______________________________________________________________________________
void CbmKresConversionBG::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresConversionBG.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresConversionBG::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresConversionBG::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresConversionBG(void *p) {
      return  p ? new(p) ::CbmKresConversionBG : new ::CbmKresConversionBG;
   }
   static void *newArray_CbmKresConversionBG(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresConversionBG[nElements] : new ::CbmKresConversionBG[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresConversionBG(void *p) {
      delete ((::CbmKresConversionBG*)p);
   }
   static void deleteArray_CbmKresConversionBG(void *p) {
      delete [] ((::CbmKresConversionBG*)p);
   }
   static void destruct_CbmKresConversionBG(void *p) {
      typedef ::CbmKresConversionBG current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresConversionBG

//______________________________________________________________________________
void CbmKresConversionKF::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresConversionKF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresConversionKF::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresConversionKF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresConversionKF(void *p) {
      return  p ? new(p) ::CbmKresConversionKF : new ::CbmKresConversionKF;
   }
   static void *newArray_CbmKresConversionKF(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresConversionKF[nElements] : new ::CbmKresConversionKF[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresConversionKF(void *p) {
      delete ((::CbmKresConversionKF*)p);
   }
   static void deleteArray_CbmKresConversionKF(void *p) {
      delete [] ((::CbmKresConversionKF*)p);
   }
   static void destruct_CbmKresConversionKF(void *p) {
      typedef ::CbmKresConversionKF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresConversionKF

//______________________________________________________________________________
void CbmKresSelectAnn::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresSelectAnn.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresSelectAnn::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresSelectAnn::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresSelectAnn(void *p) {
      return  p ? new(p) ::CbmKresSelectAnn : new ::CbmKresSelectAnn;
   }
   static void *newArray_CbmKresSelectAnn(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresSelectAnn[nElements] : new ::CbmKresSelectAnn[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresSelectAnn(void *p) {
      delete ((::CbmKresSelectAnn*)p);
   }
   static void deleteArray_CbmKresSelectAnn(void *p) {
      delete [] ((::CbmKresSelectAnn*)p);
   }
   static void destruct_CbmKresSelectAnn(void *p) {
      typedef ::CbmKresSelectAnn current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresSelectAnn

//______________________________________________________________________________
void CbmKresTrainAnn::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresTrainAnn.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresTrainAnn::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresTrainAnn::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresTrainAnn(void *p) {
      return  p ? new(p) ::CbmKresTrainAnn : new ::CbmKresTrainAnn;
   }
   static void *newArray_CbmKresTrainAnn(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresTrainAnn[nElements] : new ::CbmKresTrainAnn[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresTrainAnn(void *p) {
      delete ((::CbmKresTrainAnn*)p);
   }
   static void deleteArray_CbmKresTrainAnn(void *p) {
      delete [] ((::CbmKresTrainAnn*)p);
   }
   static void destruct_CbmKresTrainAnn(void *p) {
      typedef ::CbmKresTrainAnn current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresTrainAnn

//______________________________________________________________________________
void CbmKresConversionManual::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresConversionManual.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresConversionManual::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresConversionManual::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresConversionManual(void *p) {
      return  p ? new(p) ::CbmKresConversionManual : new ::CbmKresConversionManual;
   }
   static void *newArray_CbmKresConversionManual(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresConversionManual[nElements] : new ::CbmKresConversionManual[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresConversionManual(void *p) {
      delete ((::CbmKresConversionManual*)p);
   }
   static void deleteArray_CbmKresConversionManual(void *p) {
      delete [] ((::CbmKresConversionManual*)p);
   }
   static void destruct_CbmKresConversionManual(void *p) {
      typedef ::CbmKresConversionManual current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresConversionManual

//______________________________________________________________________________
void CbmKresConversionManualmbias1::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresConversionManualmbias1.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresConversionManualmbias1::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresConversionManualmbias1::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresConversionManualmbias1(void *p) {
      return  p ? new(p) ::CbmKresConversionManualmbias1 : new ::CbmKresConversionManualmbias1;
   }
   static void *newArray_CbmKresConversionManualmbias1(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresConversionManualmbias1[nElements] : new ::CbmKresConversionManualmbias1[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresConversionManualmbias1(void *p) {
      delete ((::CbmKresConversionManualmbias1*)p);
   }
   static void deleteArray_CbmKresConversionManualmbias1(void *p) {
      delete [] ((::CbmKresConversionManualmbias1*)p);
   }
   static void destruct_CbmKresConversionManualmbias1(void *p) {
      typedef ::CbmKresConversionManualmbias1 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresConversionManualmbias1

//______________________________________________________________________________
void CbmKresConversionManualmbias2::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresConversionManualmbias2.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresConversionManualmbias2::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresConversionManualmbias2::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresConversionManualmbias2(void *p) {
      return  p ? new(p) ::CbmKresConversionManualmbias2 : new ::CbmKresConversionManualmbias2;
   }
   static void *newArray_CbmKresConversionManualmbias2(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresConversionManualmbias2[nElements] : new ::CbmKresConversionManualmbias2[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresConversionManualmbias2(void *p) {
      delete ((::CbmKresConversionManualmbias2*)p);
   }
   static void deleteArray_CbmKresConversionManualmbias2(void *p) {
      delete [] ((::CbmKresConversionManualmbias2*)p);
   }
   static void destruct_CbmKresConversionManualmbias2(void *p) {
      typedef ::CbmKresConversionManualmbias2 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresConversionManualmbias2

//______________________________________________________________________________
void CbmKresConversionManualmbias3::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresConversionManualmbias3.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresConversionManualmbias3::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresConversionManualmbias3::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresConversionManualmbias3(void *p) {
      return  p ? new(p) ::CbmKresConversionManualmbias3 : new ::CbmKresConversionManualmbias3;
   }
   static void *newArray_CbmKresConversionManualmbias3(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresConversionManualmbias3[nElements] : new ::CbmKresConversionManualmbias3[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresConversionManualmbias3(void *p) {
      delete ((::CbmKresConversionManualmbias3*)p);
   }
   static void deleteArray_CbmKresConversionManualmbias3(void *p) {
      delete [] ((::CbmKresConversionManualmbias3*)p);
   }
   static void destruct_CbmKresConversionManualmbias3(void *p) {
      typedef ::CbmKresConversionManualmbias3 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresConversionManualmbias3

//______________________________________________________________________________
void CbmKresConversionManualmbias4::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresConversionManualmbias4.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresConversionManualmbias4::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresConversionManualmbias4::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresConversionManualmbias4(void *p) {
      return  p ? new(p) ::CbmKresConversionManualmbias4 : new ::CbmKresConversionManualmbias4;
   }
   static void *newArray_CbmKresConversionManualmbias4(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresConversionManualmbias4[nElements] : new ::CbmKresConversionManualmbias4[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresConversionManualmbias4(void *p) {
      delete ((::CbmKresConversionManualmbias4*)p);
   }
   static void deleteArray_CbmKresConversionManualmbias4(void *p) {
      delete [] ((::CbmKresConversionManualmbias4*)p);
   }
   static void destruct_CbmKresConversionManualmbias4(void *p) {
      typedef ::CbmKresConversionManualmbias4 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresConversionManualmbias4

//______________________________________________________________________________
void CbmKresConversionPhotons::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresConversionPhotons.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresConversionPhotons::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresConversionPhotons::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresConversionPhotons(void *p) {
      return  p ? new(p) ::CbmKresConversionPhotons : new ::CbmKresConversionPhotons;
   }
   static void *newArray_CbmKresConversionPhotons(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresConversionPhotons[nElements] : new ::CbmKresConversionPhotons[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresConversionPhotons(void *p) {
      delete ((::CbmKresConversionPhotons*)p);
   }
   static void deleteArray_CbmKresConversionPhotons(void *p) {
      delete [] ((::CbmKresConversionPhotons*)p);
   }
   static void destruct_CbmKresConversionPhotons(void *p) {
      typedef ::CbmKresConversionPhotons current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresConversionPhotons

//______________________________________________________________________________
void CbmKresConversionReconstruction::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresConversionReconstruction.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresConversionReconstruction::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresConversionReconstruction::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresConversionReconstruction(void *p) {
      return  p ? new(p) ::CbmKresConversionReconstruction : new ::CbmKresConversionReconstruction;
   }
   static void *newArray_CbmKresConversionReconstruction(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresConversionReconstruction[nElements] : new ::CbmKresConversionReconstruction[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresConversionReconstruction(void *p) {
      delete ((::CbmKresConversionReconstruction*)p);
   }
   static void deleteArray_CbmKresConversionReconstruction(void *p) {
      delete [] ((::CbmKresConversionReconstruction*)p);
   }
   static void destruct_CbmKresConversionReconstruction(void *p) {
      typedef ::CbmKresConversionReconstruction current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresConversionReconstruction

//______________________________________________________________________________
void CbmKresEta::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresEta.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresEta::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresEta::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresEta(void *p) {
      return  p ? new(p) ::CbmKresEta : new ::CbmKresEta;
   }
   static void *newArray_CbmKresEta(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresEta[nElements] : new ::CbmKresEta[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresEta(void *p) {
      delete ((::CbmKresEta*)p);
   }
   static void deleteArray_CbmKresEta(void *p) {
      delete [] ((::CbmKresEta*)p);
   }
   static void destruct_CbmKresEta(void *p) {
      typedef ::CbmKresEta current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresEta

//______________________________________________________________________________
void CbmKresEtaMCAnalysis::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresEtaMCAnalysis.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresEtaMCAnalysis::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresEtaMCAnalysis::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresEtaMCAnalysis(void *p) {
      return  p ? new(p) ::CbmKresEtaMCAnalysis : new ::CbmKresEtaMCAnalysis;
   }
   static void *newArray_CbmKresEtaMCAnalysis(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresEtaMCAnalysis[nElements] : new ::CbmKresEtaMCAnalysis[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresEtaMCAnalysis(void *p) {
      delete ((::CbmKresEtaMCAnalysis*)p);
   }
   static void deleteArray_CbmKresEtaMCAnalysis(void *p) {
      delete [] ((::CbmKresEtaMCAnalysis*)p);
   }
   static void destruct_CbmKresEtaMCAnalysis(void *p) {
      typedef ::CbmKresEtaMCAnalysis current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresEtaMCAnalysis

//______________________________________________________________________________
void CbmKresTemperature::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresTemperature.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresTemperature::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresTemperature::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresTemperature(void *p) {
      return  p ? new(p) ::CbmKresTemperature : new ::CbmKresTemperature;
   }
   static void *newArray_CbmKresTemperature(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresTemperature[nElements] : new ::CbmKresTemperature[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresTemperature(void *p) {
      delete ((::CbmKresTemperature*)p);
   }
   static void deleteArray_CbmKresTemperature(void *p) {
      delete [] ((::CbmKresTemperature*)p);
   }
   static void destruct_CbmKresTemperature(void *p) {
      typedef ::CbmKresTemperature current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresTemperature

//______________________________________________________________________________
void CbmKresConversionMain::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresConversionMain.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresConversionMain::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresConversionMain::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresConversionMain(void *p) {
      return  p ? new(p) ::CbmKresConversionMain : new ::CbmKresConversionMain;
   }
   static void *newArray_CbmKresConversionMain(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresConversionMain[nElements] : new ::CbmKresConversionMain[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresConversionMain(void *p) {
      delete ((::CbmKresConversionMain*)p);
   }
   static void deleteArray_CbmKresConversionMain(void *p) {
      delete [] ((::CbmKresConversionMain*)p);
   }
   static void destruct_CbmKresConversionMain(void *p) {
      typedef ::CbmKresConversionMain current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresConversionMain

//______________________________________________________________________________
void CbmKresSelectGoodEvents::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKresSelectGoodEvents.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKresSelectGoodEvents::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKresSelectGoodEvents::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKresSelectGoodEvents(void *p) {
      return  p ? new(p) ::CbmKresSelectGoodEvents : new ::CbmKresSelectGoodEvents;
   }
   static void *newArray_CbmKresSelectGoodEvents(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKresSelectGoodEvents[nElements] : new ::CbmKresSelectGoodEvents[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKresSelectGoodEvents(void *p) {
      delete ((::CbmKresSelectGoodEvents*)p);
   }
   static void deleteArray_CbmKresSelectGoodEvents(void *p) {
      delete [] ((::CbmKresSelectGoodEvents*)p);
   }
   static void destruct_CbmKresSelectGoodEvents(void *p) {
      typedef ::CbmKresSelectGoodEvents current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKresSelectGoodEvents

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
   static TClass *vectorlEvectorlETVector3gRsPgR_Dictionary();
   static void vectorlEvectorlETVector3gRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlETVector3gRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlETVector3gRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlETVector3gRsPgR(void *p);
   static void deleteArray_vectorlEvectorlETVector3gRsPgR(void *p);
   static void destruct_vectorlEvectorlETVector3gRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<TVector3> >*)
   {
      vector<vector<TVector3> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<TVector3> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<TVector3> >", -2, "vector", 389,
                  typeid(vector<vector<TVector3> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlETVector3gRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<TVector3> >) );
      instance.SetNew(&new_vectorlEvectorlETVector3gRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlETVector3gRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlETVector3gRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlETVector3gRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlETVector3gRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<TVector3> > >()));

      ::ROOT::AddClassAlternate("vector<vector<TVector3> >","std::vector<std::vector<TVector3, std::allocator<TVector3> >, std::allocator<std::vector<TVector3, std::allocator<TVector3> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<TVector3> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlETVector3gRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<TVector3> >*)nullptr)->GetClass();
      vectorlEvectorlETVector3gRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlETVector3gRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlETVector3gRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TVector3> > : new vector<vector<TVector3> >;
   }
   static void *newArray_vectorlEvectorlETVector3gRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TVector3> >[nElements] : new vector<vector<TVector3> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlETVector3gRsPgR(void *p) {
      delete ((vector<vector<TVector3> >*)p);
   }
   static void deleteArray_vectorlEvectorlETVector3gRsPgR(void *p) {
      delete [] ((vector<vector<TVector3> >*)p);
   }
   static void destruct_vectorlEvectorlETVector3gRsPgR(void *p) {
      typedef vector<vector<TVector3> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<TVector3> >

namespace ROOT {
   static TClass *vectorlEvectorlECbmMCTrackmUgRsPgR_Dictionary();
   static void vectorlEvectorlECbmMCTrackmUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlECbmMCTrackmUgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p);
   static void destruct_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<CbmMCTrack*> >*)
   {
      vector<vector<CbmMCTrack*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<CbmMCTrack*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<CbmMCTrack*> >", -2, "vector", 389,
                  typeid(vector<vector<CbmMCTrack*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlECbmMCTrackmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<CbmMCTrack*> >) );
      instance.SetNew(&new_vectorlEvectorlECbmMCTrackmUgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlECbmMCTrackmUgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlECbmMCTrackmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlECbmMCTrackmUgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlECbmMCTrackmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<CbmMCTrack*> > >()));

      ::ROOT::AddClassAlternate("vector<vector<CbmMCTrack*> >","std::vector<std::vector<CbmMCTrack*, std::allocator<CbmMCTrack*> >, std::allocator<std::vector<CbmMCTrack*, std::allocator<CbmMCTrack*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<CbmMCTrack*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlECbmMCTrackmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<CbmMCTrack*> >*)nullptr)->GetClass();
      vectorlEvectorlECbmMCTrackmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlECbmMCTrackmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmMCTrack*> > : new vector<vector<CbmMCTrack*> >;
   }
   static void *newArray_vectorlEvectorlECbmMCTrackmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmMCTrack*> >[nElements] : new vector<vector<CbmMCTrack*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p) {
      delete ((vector<vector<CbmMCTrack*> >*)p);
   }
   static void deleteArray_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p) {
      delete [] ((vector<vector<CbmMCTrack*> >*)p);
   }
   static void destruct_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p) {
      typedef vector<vector<CbmMCTrack*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<CbmMCTrack*> >

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
   static TClass *vectorlEKFParticlegR_Dictionary();
   static void vectorlEKFParticlegR_TClassManip(TClass*);
   static void *new_vectorlEKFParticlegR(void *p = nullptr);
   static void *newArray_vectorlEKFParticlegR(Long_t size, void *p);
   static void delete_vectorlEKFParticlegR(void *p);
   static void deleteArray_vectorlEKFParticlegR(void *p);
   static void destruct_vectorlEKFParticlegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<KFParticle>*)
   {
      vector<KFParticle> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<KFParticle>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<KFParticle>", -2, "vector", 389,
                  typeid(vector<KFParticle>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEKFParticlegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<KFParticle>) );
      instance.SetNew(&new_vectorlEKFParticlegR);
      instance.SetNewArray(&newArray_vectorlEKFParticlegR);
      instance.SetDelete(&delete_vectorlEKFParticlegR);
      instance.SetDeleteArray(&deleteArray_vectorlEKFParticlegR);
      instance.SetDestructor(&destruct_vectorlEKFParticlegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<KFParticle> >()));

      ::ROOT::AddClassAlternate("vector<KFParticle>","std::vector<KFParticle, std::allocator<KFParticle> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<KFParticle>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEKFParticlegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<KFParticle>*)nullptr)->GetClass();
      vectorlEKFParticlegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEKFParticlegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEKFParticlegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<KFParticle> : new vector<KFParticle>;
   }
   static void *newArray_vectorlEKFParticlegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<KFParticle>[nElements] : new vector<KFParticle>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEKFParticlegR(void *p) {
      delete ((vector<KFParticle>*)p);
   }
   static void deleteArray_vectorlEKFParticlegR(void *p) {
      delete [] ((vector<KFParticle>*)p);
   }
   static void destruct_vectorlEKFParticlegR(void *p) {
      typedef vector<KFParticle> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<KFParticle>

namespace ROOT {
   static TClass *vectorlECbmStsTrackmUgR_Dictionary();
   static void vectorlECbmStsTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmStsTrackmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmStsTrackmUgR(Long_t size, void *p);
   static void delete_vectorlECbmStsTrackmUgR(void *p);
   static void deleteArray_vectorlECbmStsTrackmUgR(void *p);
   static void destruct_vectorlECbmStsTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsTrack*>*)
   {
      vector<CbmStsTrack*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsTrack*>", -2, "vector", 389,
                  typeid(vector<CbmStsTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsTrackmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmStsTrack*>) );
      instance.SetNew(&new_vectorlECbmStsTrackmUgR);
      instance.SetNewArray(&newArray_vectorlECbmStsTrackmUgR);
      instance.SetDelete(&delete_vectorlECbmStsTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsTrackmUgR);
      instance.SetDestructor(&destruct_vectorlECbmStsTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsTrack*> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsTrack*>","std::vector<CbmStsTrack*, std::allocator<CbmStsTrack*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsTrack*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsTrack*>*)nullptr)->GetClass();
      vectorlECbmStsTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsTrack*> : new vector<CbmStsTrack*>;
   }
   static void *newArray_vectorlECbmStsTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsTrack*>[nElements] : new vector<CbmStsTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsTrackmUgR(void *p) {
      delete ((vector<CbmStsTrack*>*)p);
   }
   static void deleteArray_vectorlECbmStsTrackmUgR(void *p) {
      delete [] ((vector<CbmStsTrack*>*)p);
   }
   static void destruct_vectorlECbmStsTrackmUgR(void *p) {
      typedef vector<CbmStsTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsTrack*>

namespace ROOT {
   static TClass *vectorlECbmRichRingmUgR_Dictionary();
   static void vectorlECbmRichRingmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmRichRingmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmRichRingmUgR(Long_t size, void *p);
   static void delete_vectorlECbmRichRingmUgR(void *p);
   static void deleteArray_vectorlECbmRichRingmUgR(void *p);
   static void destruct_vectorlECbmRichRingmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmRichRing*>*)
   {
      vector<CbmRichRing*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmRichRing*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmRichRing*>", -2, "vector", 389,
                  typeid(vector<CbmRichRing*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmRichRingmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmRichRing*>) );
      instance.SetNew(&new_vectorlECbmRichRingmUgR);
      instance.SetNewArray(&newArray_vectorlECbmRichRingmUgR);
      instance.SetDelete(&delete_vectorlECbmRichRingmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmRichRingmUgR);
      instance.SetDestructor(&destruct_vectorlECbmRichRingmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmRichRing*> >()));

      ::ROOT::AddClassAlternate("vector<CbmRichRing*>","std::vector<CbmRichRing*, std::allocator<CbmRichRing*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmRichRing*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmRichRingmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmRichRing*>*)nullptr)->GetClass();
      vectorlECbmRichRingmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmRichRingmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmRichRingmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmRichRing*> : new vector<CbmRichRing*>;
   }
   static void *newArray_vectorlECbmRichRingmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmRichRing*>[nElements] : new vector<CbmRichRing*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmRichRingmUgR(void *p) {
      delete ((vector<CbmRichRing*>*)p);
   }
   static void deleteArray_vectorlECbmRichRingmUgR(void *p) {
      delete [] ((vector<CbmRichRing*>*)p);
   }
   static void destruct_vectorlECbmRichRingmUgR(void *p) {
      typedef vector<CbmRichRing*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmRichRing*>

namespace ROOT {
   static TClass *vectorlECbmMCTrackmUgR_Dictionary();
   static void vectorlECbmMCTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmMCTrackmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmMCTrackmUgR(Long_t size, void *p);
   static void delete_vectorlECbmMCTrackmUgR(void *p);
   static void deleteArray_vectorlECbmMCTrackmUgR(void *p);
   static void destruct_vectorlECbmMCTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMCTrack*>*)
   {
      vector<CbmMCTrack*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMCTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMCTrack*>", -2, "vector", 389,
                  typeid(vector<CbmMCTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMCTrackmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmMCTrack*>) );
      instance.SetNew(&new_vectorlECbmMCTrackmUgR);
      instance.SetNewArray(&newArray_vectorlECbmMCTrackmUgR);
      instance.SetDelete(&delete_vectorlECbmMCTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMCTrackmUgR);
      instance.SetDestructor(&destruct_vectorlECbmMCTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMCTrack*> >()));

      ::ROOT::AddClassAlternate("vector<CbmMCTrack*>","std::vector<CbmMCTrack*, std::allocator<CbmMCTrack*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMCTrack*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMCTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMCTrack*>*)nullptr)->GetClass();
      vectorlECbmMCTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMCTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMCTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMCTrack*> : new vector<CbmMCTrack*>;
   }
   static void *newArray_vectorlECbmMCTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMCTrack*>[nElements] : new vector<CbmMCTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMCTrackmUgR(void *p) {
      delete ((vector<CbmMCTrack*>*)p);
   }
   static void deleteArray_vectorlECbmMCTrackmUgR(void *p) {
      delete [] ((vector<CbmMCTrack*>*)p);
   }
   static void destruct_vectorlECbmMCTrackmUgR(void *p) {
      typedef vector<CbmMCTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMCTrack*>

namespace {
  void TriggerDictionaryInitialization_libCbmConversion2Analysis_Impl() {
    static const char* headers[] = {
"CbmKresConversionMain.h",
"CbmKresConversionManual.h",
"CbmKresConversionReconstruction.h",
"CbmKresConversionKF.h",
"CbmKresConversionGeneral.h",
"CbmKresConversionBG.h",
"CbmKresConversionManualmbias1.h",
"CbmKresConversionManualmbias2.h",
"CbmKresConversionManualmbias3.h",
"CbmKresConversionManualmbias4.h",
"CbmKresTrainAnn.h",
"CbmKresSelectAnn.h",
"CbmKresTemperature.h",
"CbmKresTrainAnnDirectPhotons.h",
"CbmKresConversionPhotons.h",
"CbmKresSelectAnnPhotons.h",
"CbmKresGammaCorrection.h",
"CbmKresConversionCorrectedPhotons.h",
"CbmKresEta.h",
"CbmKresEtaMCAnalysis.h",
"CbmKresSelectGoodEvents.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/pi0eta",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/pi0eta",
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
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/lmvm",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/KFQA",
"/home/heinemann/gpuunpacker/cbmroot/build2/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/PWGDIL/dielectron/pi0eta/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmConversion2Analysis dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmKresGammaCorrection.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresGammaCorrection;
class __attribute__((annotate("$clingAutoload$CbmKresSelectAnnPhotons.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresSelectAnnPhotons;
class __attribute__((annotate("$clingAutoload$CbmKresTrainAnnDirectPhotons.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresTrainAnnDirectPhotons;
class __attribute__((annotate("$clingAutoload$CbmKresConversionCorrectedPhotons.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresConversionCorrectedPhotons;
class __attribute__((annotate("$clingAutoload$CbmKresConversionGeneral.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresConversionGeneral;
class __attribute__((annotate("$clingAutoload$CbmKresConversionBG.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresConversionBG;
class __attribute__((annotate("$clingAutoload$CbmKresConversionKF.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresConversionKF;
class __attribute__((annotate("$clingAutoload$CbmKresSelectAnn.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresSelectAnn;
class __attribute__((annotate("$clingAutoload$CbmKresTrainAnn.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresTrainAnn;
class __attribute__((annotate("$clingAutoload$CbmKresConversionManual.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresConversionManual;
class __attribute__((annotate("$clingAutoload$CbmKresConversionManualmbias1.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresConversionManualmbias1;
class __attribute__((annotate("$clingAutoload$CbmKresConversionManualmbias2.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresConversionManualmbias2;
class __attribute__((annotate("$clingAutoload$CbmKresConversionManualmbias3.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresConversionManualmbias3;
class __attribute__((annotate("$clingAutoload$CbmKresConversionManualmbias4.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresConversionManualmbias4;
class __attribute__((annotate("$clingAutoload$CbmKresConversionPhotons.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresConversionPhotons;
class __attribute__((annotate("$clingAutoload$CbmKresConversionReconstruction.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresConversionReconstruction;
class __attribute__((annotate("$clingAutoload$CbmKresEta.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresEta;
class __attribute__((annotate("$clingAutoload$CbmKresEtaMCAnalysis.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresEtaMCAnalysis;
class __attribute__((annotate("$clingAutoload$CbmKresTemperature.h")))  __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresTemperature;
class __attribute__((annotate("$clingAutoload$CbmKresConversionMain.h")))  CbmKresConversionMain;
class __attribute__((annotate("$clingAutoload$CbmKresSelectGoodEvents.h")))  CbmKresSelectGoodEvents;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmConversion2Analysis dictionary payload"

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
#include "CbmKresConversionMain.h"
#include "CbmKresConversionManual.h"
#include "CbmKresConversionReconstruction.h"
#include "CbmKresConversionKF.h"
#include "CbmKresConversionGeneral.h"
#include "CbmKresConversionBG.h"
#include "CbmKresConversionManualmbias1.h"
#include "CbmKresConversionManualmbias2.h"
#include "CbmKresConversionManualmbias3.h"
#include "CbmKresConversionManualmbias4.h"
#include "CbmKresTrainAnn.h"
#include "CbmKresSelectAnn.h"
#include "CbmKresTemperature.h"
#include "CbmKresTrainAnnDirectPhotons.h"
#include "CbmKresConversionPhotons.h"
#include "CbmKresSelectAnnPhotons.h"
#include "CbmKresGammaCorrection.h"
#include "CbmKresConversionCorrectedPhotons.h"
#include "CbmKresEta.h"
#include "CbmKresEtaMCAnalysis.h"
#include "CbmKresSelectGoodEvents.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmKresConversionBG", payloadCode, "@",
"CbmKresConversionCorrectedPhotons", payloadCode, "@",
"CbmKresConversionGeneral", payloadCode, "@",
"CbmKresConversionKF", payloadCode, "@",
"CbmKresConversionMain", payloadCode, "@",
"CbmKresConversionManual", payloadCode, "@",
"CbmKresConversionManualmbias1", payloadCode, "@",
"CbmKresConversionManualmbias2", payloadCode, "@",
"CbmKresConversionManualmbias3", payloadCode, "@",
"CbmKresConversionManualmbias4", payloadCode, "@",
"CbmKresConversionPhotons", payloadCode, "@",
"CbmKresConversionReconstruction", payloadCode, "@",
"CbmKresEta", payloadCode, "@",
"CbmKresEtaMCAnalysis", payloadCode, "@",
"CbmKresGammaCorrection", payloadCode, "@",
"CbmKresSelectAnn", payloadCode, "@",
"CbmKresSelectAnnPhotons", payloadCode, "@",
"CbmKresSelectGoodEvents", payloadCode, "@",
"CbmKresTemperature", payloadCode, "@",
"CbmKresTrainAnn", payloadCode, "@",
"CbmKresTrainAnnDirectPhotons", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmConversion2Analysis",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmConversion2Analysis_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmConversion2Analysis_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmConversion2Analysis() {
  TriggerDictionaryInitialization_libCbmConversion2Analysis_Impl();
}
