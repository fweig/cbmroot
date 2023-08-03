// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmStsSim
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
#include "CbmStsDigitize.h"
#include "CbmStsMC.h"
#include "CbmStsSensorPoint.h"
#include "CbmStsSignal.h"
#include "CbmStsSimModule.h"
#include "CbmStsSimSensor.h"
#include "CbmStsSimSensorDssd.h"
#include "CbmStsSimSensorDssdOrtho.h"
#include "CbmStsSimSensorDssdStereo.h"
#include "CbmStsSimSensorFactory.h"
#include "CbmStsDigitizePixel.h"
#include "CbmStsDigitizeQa.h"
#include "CbmStsDigitizeQaReport.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *CbmDigitizelECbmStsDigigR_Dictionary();
   static void CbmDigitizelECbmStsDigigR_TClassManip(TClass*);
   static void delete_CbmDigitizelECbmStsDigigR(void *p);
   static void deleteArray_CbmDigitizelECbmStsDigigR(void *p);
   static void destruct_CbmDigitizelECbmStsDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigitize<CbmStsDigi>*)
   {
      ::CbmDigitize<CbmStsDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigitize<CbmStsDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigitize<CbmStsDigi>", ::CbmDigitize<CbmStsDigi>::Class_Version(), "CbmDigitize.h", 44,
                  typeid(::CbmDigitize<CbmStsDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigitizelECbmStsDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigitize<CbmStsDigi>) );
      instance.SetDelete(&delete_CbmDigitizelECbmStsDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigitizelECbmStsDigigR);
      instance.SetDestructor(&destruct_CbmDigitizelECbmStsDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigitize<CbmStsDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigitize<CbmStsDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigitize<CbmStsDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigitizelECbmStsDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmStsDigi>*)nullptr)->GetClass();
      CbmDigitizelECbmStsDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigitizelECbmStsDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsSignal(void *p = nullptr);
   static void *newArray_CbmStsSignal(Long_t size, void *p);
   static void delete_CbmStsSignal(void *p);
   static void deleteArray_CbmStsSignal(void *p);
   static void destruct_CbmStsSignal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsSignal*)
   {
      ::CbmStsSignal *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsSignal >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsSignal", ::CbmStsSignal::Class_Version(), "CbmStsSignal.h", 32,
                  typeid(::CbmStsSignal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsSignal::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsSignal) );
      instance.SetNew(&new_CbmStsSignal);
      instance.SetNewArray(&newArray_CbmStsSignal);
      instance.SetDelete(&delete_CbmStsSignal);
      instance.SetDeleteArray(&deleteArray_CbmStsSignal);
      instance.SetDestructor(&destruct_CbmStsSignal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsSignal*)
   {
      return GenerateInitInstanceLocal((::CbmStsSignal*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsSignal*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsSimModule(void *p = nullptr);
   static void *newArray_CbmStsSimModule(Long_t size, void *p);
   static void delete_CbmStsSimModule(void *p);
   static void deleteArray_CbmStsSimModule(void *p);
   static void destruct_CbmStsSimModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsSimModule*)
   {
      ::CbmStsSimModule *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsSimModule >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsSimModule", ::CbmStsSimModule::Class_Version(), "CbmStsSimModule.h", 48,
                  typeid(::CbmStsSimModule), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsSimModule::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsSimModule) );
      instance.SetNew(&new_CbmStsSimModule);
      instance.SetNewArray(&newArray_CbmStsSimModule);
      instance.SetDelete(&delete_CbmStsSimModule);
      instance.SetDeleteArray(&deleteArray_CbmStsSimModule);
      instance.SetDestructor(&destruct_CbmStsSimModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsSimModule*)
   {
      return GenerateInitInstanceLocal((::CbmStsSimModule*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsSimModule*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmStsSimSensor(void *p);
   static void deleteArray_CbmStsSimSensor(void *p);
   static void destruct_CbmStsSimSensor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsSimSensor*)
   {
      ::CbmStsSimSensor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsSimSensor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsSimSensor", ::CbmStsSimSensor::Class_Version(), "CbmStsSimSensor.h", 38,
                  typeid(::CbmStsSimSensor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsSimSensor::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsSimSensor) );
      instance.SetDelete(&delete_CbmStsSimSensor);
      instance.SetDeleteArray(&deleteArray_CbmStsSimSensor);
      instance.SetDestructor(&destruct_CbmStsSimSensor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsSimSensor*)
   {
      return GenerateInitInstanceLocal((::CbmStsSimSensor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsSimSensor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsDigitize(void *p = nullptr);
   static void *newArray_CbmStsDigitize(Long_t size, void *p);
   static void delete_CbmStsDigitize(void *p);
   static void deleteArray_CbmStsDigitize(void *p);
   static void destruct_CbmStsDigitize(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsDigitize*)
   {
      ::CbmStsDigitize *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsDigitize >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsDigitize", ::CbmStsDigitize::Class_Version(), "CbmStsDigitize.h", 55,
                  typeid(::CbmStsDigitize), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsDigitize::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsDigitize) );
      instance.SetNew(&new_CbmStsDigitize);
      instance.SetNewArray(&newArray_CbmStsDigitize);
      instance.SetDelete(&delete_CbmStsDigitize);
      instance.SetDeleteArray(&deleteArray_CbmStsDigitize);
      instance.SetDestructor(&destruct_CbmStsDigitize);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsDigitize*)
   {
      return GenerateInitInstanceLocal((::CbmStsDigitize*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsDigitize*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsMC(void *p = nullptr);
   static void *newArray_CbmStsMC(Long_t size, void *p);
   static void delete_CbmStsMC(void *p);
   static void deleteArray_CbmStsMC(void *p);
   static void destruct_CbmStsMC(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsMC*)
   {
      ::CbmStsMC *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsMC >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsMC", ::CbmStsMC::Class_Version(), "CbmStsMC.h", 40,
                  typeid(::CbmStsMC), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsMC::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsMC) );
      instance.SetNew(&new_CbmStsMC);
      instance.SetNewArray(&newArray_CbmStsMC);
      instance.SetDelete(&delete_CbmStsMC);
      instance.SetDeleteArray(&deleteArray_CbmStsMC);
      instance.SetDestructor(&destruct_CbmStsMC);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsMC*)
   {
      return GenerateInitInstanceLocal((::CbmStsMC*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsMC*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsSensorPoint(void *p = nullptr);
   static void *newArray_CbmStsSensorPoint(Long_t size, void *p);
   static void delete_CbmStsSensorPoint(void *p);
   static void deleteArray_CbmStsSensorPoint(void *p);
   static void destruct_CbmStsSensorPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsSensorPoint*)
   {
      ::CbmStsSensorPoint *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsSensorPoint >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsSensorPoint", ::CbmStsSensorPoint::Class_Version(), "CbmStsSensorPoint.h", 24,
                  typeid(::CbmStsSensorPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsSensorPoint::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsSensorPoint) );
      instance.SetNew(&new_CbmStsSensorPoint);
      instance.SetNewArray(&newArray_CbmStsSensorPoint);
      instance.SetDelete(&delete_CbmStsSensorPoint);
      instance.SetDeleteArray(&deleteArray_CbmStsSensorPoint);
      instance.SetDestructor(&destruct_CbmStsSensorPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsSensorPoint*)
   {
      return GenerateInitInstanceLocal((::CbmStsSensorPoint*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsSensorPoint*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmStsSimSensorDssd(void *p);
   static void deleteArray_CbmStsSimSensorDssd(void *p);
   static void destruct_CbmStsSimSensorDssd(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsSimSensorDssd*)
   {
      ::CbmStsSimSensorDssd *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsSimSensorDssd >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsSimSensorDssd", ::CbmStsSimSensorDssd::Class_Version(), "CbmStsSimSensorDssd.h", 50,
                  typeid(::CbmStsSimSensorDssd), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsSimSensorDssd::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsSimSensorDssd) );
      instance.SetDelete(&delete_CbmStsSimSensorDssd);
      instance.SetDeleteArray(&deleteArray_CbmStsSimSensorDssd);
      instance.SetDestructor(&destruct_CbmStsSimSensorDssd);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsSimSensorDssd*)
   {
      return GenerateInitInstanceLocal((::CbmStsSimSensorDssd*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsSimSensorDssd*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsSimSensorDssdOrtho(void *p = nullptr);
   static void *newArray_CbmStsSimSensorDssdOrtho(Long_t size, void *p);
   static void delete_CbmStsSimSensorDssdOrtho(void *p);
   static void deleteArray_CbmStsSimSensorDssdOrtho(void *p);
   static void destruct_CbmStsSimSensorDssdOrtho(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsSimSensorDssdOrtho*)
   {
      ::CbmStsSimSensorDssdOrtho *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsSimSensorDssdOrtho >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsSimSensorDssdOrtho", ::CbmStsSimSensorDssdOrtho::Class_Version(), "CbmStsSimSensorDssdOrtho.h", 32,
                  typeid(::CbmStsSimSensorDssdOrtho), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsSimSensorDssdOrtho::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsSimSensorDssdOrtho) );
      instance.SetNew(&new_CbmStsSimSensorDssdOrtho);
      instance.SetNewArray(&newArray_CbmStsSimSensorDssdOrtho);
      instance.SetDelete(&delete_CbmStsSimSensorDssdOrtho);
      instance.SetDeleteArray(&deleteArray_CbmStsSimSensorDssdOrtho);
      instance.SetDestructor(&destruct_CbmStsSimSensorDssdOrtho);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsSimSensorDssdOrtho*)
   {
      return GenerateInitInstanceLocal((::CbmStsSimSensorDssdOrtho*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsSimSensorDssdOrtho*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsSimSensorDssdStereo(void *p = nullptr);
   static void *newArray_CbmStsSimSensorDssdStereo(Long_t size, void *p);
   static void delete_CbmStsSimSensorDssdStereo(void *p);
   static void deleteArray_CbmStsSimSensorDssdStereo(void *p);
   static void destruct_CbmStsSimSensorDssdStereo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsSimSensorDssdStereo*)
   {
      ::CbmStsSimSensorDssdStereo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsSimSensorDssdStereo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsSimSensorDssdStereo", ::CbmStsSimSensorDssdStereo::Class_Version(), "CbmStsSimSensorDssdStereo.h", 44,
                  typeid(::CbmStsSimSensorDssdStereo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsSimSensorDssdStereo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsSimSensorDssdStereo) );
      instance.SetNew(&new_CbmStsSimSensorDssdStereo);
      instance.SetNewArray(&newArray_CbmStsSimSensorDssdStereo);
      instance.SetDelete(&delete_CbmStsSimSensorDssdStereo);
      instance.SetDeleteArray(&deleteArray_CbmStsSimSensorDssdStereo);
      instance.SetDestructor(&destruct_CbmStsSimSensorDssdStereo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsSimSensorDssdStereo*)
   {
      return GenerateInitInstanceLocal((::CbmStsSimSensorDssdStereo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsSimSensorDssdStereo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsSimSensorFactory(void *p = nullptr);
   static void *newArray_CbmStsSimSensorFactory(Long_t size, void *p);
   static void delete_CbmStsSimSensorFactory(void *p);
   static void deleteArray_CbmStsSimSensorFactory(void *p);
   static void destruct_CbmStsSimSensorFactory(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsSimSensorFactory*)
   {
      ::CbmStsSimSensorFactory *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsSimSensorFactory >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsSimSensorFactory", ::CbmStsSimSensorFactory::Class_Version(), "CbmStsSimSensorFactory.h", 35,
                  typeid(::CbmStsSimSensorFactory), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsSimSensorFactory::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsSimSensorFactory) );
      instance.SetNew(&new_CbmStsSimSensorFactory);
      instance.SetNewArray(&newArray_CbmStsSimSensorFactory);
      instance.SetDelete(&delete_CbmStsSimSensorFactory);
      instance.SetDeleteArray(&deleteArray_CbmStsSimSensorFactory);
      instance.SetDestructor(&destruct_CbmStsSimSensorFactory);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsSimSensorFactory*)
   {
      return GenerateInitInstanceLocal((::CbmStsSimSensorFactory*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsSimSensorFactory*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsDigitizePixel(void *p = nullptr);
   static void *newArray_CbmStsDigitizePixel(Long_t size, void *p);
   static void delete_CbmStsDigitizePixel(void *p);
   static void deleteArray_CbmStsDigitizePixel(void *p);
   static void destruct_CbmStsDigitizePixel(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsDigitizePixel*)
   {
      ::CbmStsDigitizePixel *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsDigitizePixel >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsDigitizePixel", ::CbmStsDigitizePixel::Class_Version(), "CbmStsDigitizePixel.h", 53,
                  typeid(::CbmStsDigitizePixel), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsDigitizePixel::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsDigitizePixel) );
      instance.SetNew(&new_CbmStsDigitizePixel);
      instance.SetNewArray(&newArray_CbmStsDigitizePixel);
      instance.SetDelete(&delete_CbmStsDigitizePixel);
      instance.SetDeleteArray(&deleteArray_CbmStsDigitizePixel);
      instance.SetDestructor(&destruct_CbmStsDigitizePixel);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsDigitizePixel*)
   {
      return GenerateInitInstanceLocal((::CbmStsDigitizePixel*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsDigitizePixel*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsDigitizeQa(void *p = nullptr);
   static void *newArray_CbmStsDigitizeQa(Long_t size, void *p);
   static void delete_CbmStsDigitizeQa(void *p);
   static void deleteArray_CbmStsDigitizeQa(void *p);
   static void destruct_CbmStsDigitizeQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsDigitizeQa*)
   {
      ::CbmStsDigitizeQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsDigitizeQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsDigitizeQa", ::CbmStsDigitizeQa::Class_Version(), "CbmStsDigitizeQa.h", 25,
                  typeid(::CbmStsDigitizeQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsDigitizeQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsDigitizeQa) );
      instance.SetNew(&new_CbmStsDigitizeQa);
      instance.SetNewArray(&newArray_CbmStsDigitizeQa);
      instance.SetDelete(&delete_CbmStsDigitizeQa);
      instance.SetDeleteArray(&deleteArray_CbmStsDigitizeQa);
      instance.SetDestructor(&destruct_CbmStsDigitizeQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsDigitizeQa*)
   {
      return GenerateInitInstanceLocal((::CbmStsDigitizeQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsDigitizeQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmStsDigitizeQaReport(void *p);
   static void deleteArray_CbmStsDigitizeQaReport(void *p);
   static void destruct_CbmStsDigitizeQaReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsDigitizeQaReport*)
   {
      ::CbmStsDigitizeQaReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsDigitizeQaReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsDigitizeQaReport", ::CbmStsDigitizeQaReport::Class_Version(), "CbmStsDigitizeQaReport.h", 15,
                  typeid(::CbmStsDigitizeQaReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsDigitizeQaReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsDigitizeQaReport) );
      instance.SetDelete(&delete_CbmStsDigitizeQaReport);
      instance.SetDeleteArray(&deleteArray_CbmStsDigitizeQaReport);
      instance.SetDestructor(&destruct_CbmStsDigitizeQaReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsDigitizeQaReport*)
   {
      return GenerateInitInstanceLocal((::CbmStsDigitizeQaReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsDigitizeQaReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigitize<CbmStsDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmStsDigi>::Class_Name()
{
   return "CbmDigitize<CbmStsDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmStsDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmStsDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigitize<CbmStsDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmStsDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmStsDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmStsDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmStsDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmStsDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsSignal::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsSignal::Class_Name()
{
   return "CbmStsSignal";
}

//______________________________________________________________________________
const char *CbmStsSignal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSignal*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsSignal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSignal*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsSignal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSignal*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsSignal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSignal*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsSimModule::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsSimModule::Class_Name()
{
   return "CbmStsSimModule";
}

//______________________________________________________________________________
const char *CbmStsSimModule::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimModule*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsSimModule::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimModule*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsSimModule::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimModule*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsSimModule::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimModule*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsSimSensor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsSimSensor::Class_Name()
{
   return "CbmStsSimSensor";
}

//______________________________________________________________________________
const char *CbmStsSimSensor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsSimSensor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsSimSensor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsSimSensor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsDigitize::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsDigitize::Class_Name()
{
   return "CbmStsDigitize";
}

//______________________________________________________________________________
const char *CbmStsDigitize::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitize*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsDigitize::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitize*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsDigitize::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitize*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsDigitize::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitize*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsMC::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsMC::Class_Name()
{
   return "CbmStsMC";
}

//______________________________________________________________________________
const char *CbmStsMC::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsMC*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsMC::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsMC*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsMC::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsMC*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsMC::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsMC*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsSensorPoint::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsSensorPoint::Class_Name()
{
   return "CbmStsSensorPoint";
}

//______________________________________________________________________________
const char *CbmStsSensorPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSensorPoint*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsSensorPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSensorPoint*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsSensorPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSensorPoint*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsSensorPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSensorPoint*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsSimSensorDssd::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsSimSensorDssd::Class_Name()
{
   return "CbmStsSimSensorDssd";
}

//______________________________________________________________________________
const char *CbmStsSimSensorDssd::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorDssd*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsSimSensorDssd::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorDssd*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsSimSensorDssd::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorDssd*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsSimSensorDssd::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorDssd*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsSimSensorDssdOrtho::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsSimSensorDssdOrtho::Class_Name()
{
   return "CbmStsSimSensorDssdOrtho";
}

//______________________________________________________________________________
const char *CbmStsSimSensorDssdOrtho::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorDssdOrtho*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsSimSensorDssdOrtho::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorDssdOrtho*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsSimSensorDssdOrtho::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorDssdOrtho*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsSimSensorDssdOrtho::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorDssdOrtho*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsSimSensorDssdStereo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsSimSensorDssdStereo::Class_Name()
{
   return "CbmStsSimSensorDssdStereo";
}

//______________________________________________________________________________
const char *CbmStsSimSensorDssdStereo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorDssdStereo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsSimSensorDssdStereo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorDssdStereo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsSimSensorDssdStereo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorDssdStereo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsSimSensorDssdStereo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorDssdStereo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsSimSensorFactory::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsSimSensorFactory::Class_Name()
{
   return "CbmStsSimSensorFactory";
}

//______________________________________________________________________________
const char *CbmStsSimSensorFactory::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorFactory*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsSimSensorFactory::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorFactory*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsSimSensorFactory::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorFactory*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsSimSensorFactory::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSimSensorFactory*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsDigitizePixel::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsDigitizePixel::Class_Name()
{
   return "CbmStsDigitizePixel";
}

//______________________________________________________________________________
const char *CbmStsDigitizePixel::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitizePixel*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsDigitizePixel::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitizePixel*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsDigitizePixel::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitizePixel*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsDigitizePixel::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitizePixel*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsDigitizeQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsDigitizeQa::Class_Name()
{
   return "CbmStsDigitizeQa";
}

//______________________________________________________________________________
const char *CbmStsDigitizeQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitizeQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsDigitizeQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitizeQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsDigitizeQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitizeQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsDigitizeQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitizeQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsDigitizeQaReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsDigitizeQaReport::Class_Name()
{
   return "CbmStsDigitizeQaReport";
}

//______________________________________________________________________________
const char *CbmStsDigitizeQaReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitizeQaReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsDigitizeQaReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitizeQaReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsDigitizeQaReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitizeQaReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsDigitizeQaReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigitizeQaReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> void CbmDigitize<CbmStsDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigitize<CbmStsDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigitize<CbmStsDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigitize<CbmStsDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmDigitizelECbmStsDigigR(void *p) {
      delete ((::CbmDigitize<CbmStsDigi>*)p);
   }
   static void deleteArray_CbmDigitizelECbmStsDigigR(void *p) {
      delete [] ((::CbmDigitize<CbmStsDigi>*)p);
   }
   static void destruct_CbmDigitizelECbmStsDigigR(void *p) {
      typedef ::CbmDigitize<CbmStsDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigitize<CbmStsDigi>

//______________________________________________________________________________
void CbmStsSignal::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsSignal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsSignal::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsSignal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsSignal(void *p) {
      return  p ? new(p) ::CbmStsSignal : new ::CbmStsSignal;
   }
   static void *newArray_CbmStsSignal(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsSignal[nElements] : new ::CbmStsSignal[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsSignal(void *p) {
      delete ((::CbmStsSignal*)p);
   }
   static void deleteArray_CbmStsSignal(void *p) {
      delete [] ((::CbmStsSignal*)p);
   }
   static void destruct_CbmStsSignal(void *p) {
      typedef ::CbmStsSignal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsSignal

//______________________________________________________________________________
void CbmStsSimModule::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsSimModule.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsSimModule::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsSimModule::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsSimModule(void *p) {
      return  p ? new(p) ::CbmStsSimModule : new ::CbmStsSimModule;
   }
   static void *newArray_CbmStsSimModule(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsSimModule[nElements] : new ::CbmStsSimModule[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsSimModule(void *p) {
      delete ((::CbmStsSimModule*)p);
   }
   static void deleteArray_CbmStsSimModule(void *p) {
      delete [] ((::CbmStsSimModule*)p);
   }
   static void destruct_CbmStsSimModule(void *p) {
      typedef ::CbmStsSimModule current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsSimModule

//______________________________________________________________________________
void CbmStsSimSensor::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsSimSensor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsSimSensor::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsSimSensor::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmStsSimSensor(void *p) {
      delete ((::CbmStsSimSensor*)p);
   }
   static void deleteArray_CbmStsSimSensor(void *p) {
      delete [] ((::CbmStsSimSensor*)p);
   }
   static void destruct_CbmStsSimSensor(void *p) {
      typedef ::CbmStsSimSensor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsSimSensor

//______________________________________________________________________________
void CbmStsDigitize::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsDigitize.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsDigitize::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsDigitize::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsDigitize(void *p) {
      return  p ? new(p) ::CbmStsDigitize : new ::CbmStsDigitize;
   }
   static void *newArray_CbmStsDigitize(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsDigitize[nElements] : new ::CbmStsDigitize[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsDigitize(void *p) {
      delete ((::CbmStsDigitize*)p);
   }
   static void deleteArray_CbmStsDigitize(void *p) {
      delete [] ((::CbmStsDigitize*)p);
   }
   static void destruct_CbmStsDigitize(void *p) {
      typedef ::CbmStsDigitize current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsDigitize

//______________________________________________________________________________
void CbmStsMC::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsMC.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsMC::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsMC::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsMC(void *p) {
      return  p ? new(p) ::CbmStsMC : new ::CbmStsMC;
   }
   static void *newArray_CbmStsMC(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsMC[nElements] : new ::CbmStsMC[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsMC(void *p) {
      delete ((::CbmStsMC*)p);
   }
   static void deleteArray_CbmStsMC(void *p) {
      delete [] ((::CbmStsMC*)p);
   }
   static void destruct_CbmStsMC(void *p) {
      typedef ::CbmStsMC current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsMC

//______________________________________________________________________________
void CbmStsSensorPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsSensorPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsSensorPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsSensorPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsSensorPoint(void *p) {
      return  p ? new(p) ::CbmStsSensorPoint : new ::CbmStsSensorPoint;
   }
   static void *newArray_CbmStsSensorPoint(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsSensorPoint[nElements] : new ::CbmStsSensorPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsSensorPoint(void *p) {
      delete ((::CbmStsSensorPoint*)p);
   }
   static void deleteArray_CbmStsSensorPoint(void *p) {
      delete [] ((::CbmStsSensorPoint*)p);
   }
   static void destruct_CbmStsSensorPoint(void *p) {
      typedef ::CbmStsSensorPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsSensorPoint

//______________________________________________________________________________
void CbmStsSimSensorDssd::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsSimSensorDssd.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsSimSensorDssd::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsSimSensorDssd::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmStsSimSensorDssd(void *p) {
      delete ((::CbmStsSimSensorDssd*)p);
   }
   static void deleteArray_CbmStsSimSensorDssd(void *p) {
      delete [] ((::CbmStsSimSensorDssd*)p);
   }
   static void destruct_CbmStsSimSensorDssd(void *p) {
      typedef ::CbmStsSimSensorDssd current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsSimSensorDssd

//______________________________________________________________________________
void CbmStsSimSensorDssdOrtho::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsSimSensorDssdOrtho.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsSimSensorDssdOrtho::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsSimSensorDssdOrtho::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsSimSensorDssdOrtho(void *p) {
      return  p ? new(p) ::CbmStsSimSensorDssdOrtho : new ::CbmStsSimSensorDssdOrtho;
   }
   static void *newArray_CbmStsSimSensorDssdOrtho(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsSimSensorDssdOrtho[nElements] : new ::CbmStsSimSensorDssdOrtho[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsSimSensorDssdOrtho(void *p) {
      delete ((::CbmStsSimSensorDssdOrtho*)p);
   }
   static void deleteArray_CbmStsSimSensorDssdOrtho(void *p) {
      delete [] ((::CbmStsSimSensorDssdOrtho*)p);
   }
   static void destruct_CbmStsSimSensorDssdOrtho(void *p) {
      typedef ::CbmStsSimSensorDssdOrtho current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsSimSensorDssdOrtho

//______________________________________________________________________________
void CbmStsSimSensorDssdStereo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsSimSensorDssdStereo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsSimSensorDssdStereo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsSimSensorDssdStereo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsSimSensorDssdStereo(void *p) {
      return  p ? new(p) ::CbmStsSimSensorDssdStereo : new ::CbmStsSimSensorDssdStereo;
   }
   static void *newArray_CbmStsSimSensorDssdStereo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsSimSensorDssdStereo[nElements] : new ::CbmStsSimSensorDssdStereo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsSimSensorDssdStereo(void *p) {
      delete ((::CbmStsSimSensorDssdStereo*)p);
   }
   static void deleteArray_CbmStsSimSensorDssdStereo(void *p) {
      delete [] ((::CbmStsSimSensorDssdStereo*)p);
   }
   static void destruct_CbmStsSimSensorDssdStereo(void *p) {
      typedef ::CbmStsSimSensorDssdStereo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsSimSensorDssdStereo

//______________________________________________________________________________
void CbmStsSimSensorFactory::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsSimSensorFactory.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsSimSensorFactory::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsSimSensorFactory::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsSimSensorFactory(void *p) {
      return  p ? new(p) ::CbmStsSimSensorFactory : new ::CbmStsSimSensorFactory;
   }
   static void *newArray_CbmStsSimSensorFactory(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsSimSensorFactory[nElements] : new ::CbmStsSimSensorFactory[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsSimSensorFactory(void *p) {
      delete ((::CbmStsSimSensorFactory*)p);
   }
   static void deleteArray_CbmStsSimSensorFactory(void *p) {
      delete [] ((::CbmStsSimSensorFactory*)p);
   }
   static void destruct_CbmStsSimSensorFactory(void *p) {
      typedef ::CbmStsSimSensorFactory current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsSimSensorFactory

//______________________________________________________________________________
void CbmStsDigitizePixel::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsDigitizePixel.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsDigitizePixel::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsDigitizePixel::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsDigitizePixel(void *p) {
      return  p ? new(p) ::CbmStsDigitizePixel : new ::CbmStsDigitizePixel;
   }
   static void *newArray_CbmStsDigitizePixel(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsDigitizePixel[nElements] : new ::CbmStsDigitizePixel[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsDigitizePixel(void *p) {
      delete ((::CbmStsDigitizePixel*)p);
   }
   static void deleteArray_CbmStsDigitizePixel(void *p) {
      delete [] ((::CbmStsDigitizePixel*)p);
   }
   static void destruct_CbmStsDigitizePixel(void *p) {
      typedef ::CbmStsDigitizePixel current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsDigitizePixel

//______________________________________________________________________________
void CbmStsDigitizeQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsDigitizeQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsDigitizeQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsDigitizeQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsDigitizeQa(void *p) {
      return  p ? new(p) ::CbmStsDigitizeQa : new ::CbmStsDigitizeQa;
   }
   static void *newArray_CbmStsDigitizeQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsDigitizeQa[nElements] : new ::CbmStsDigitizeQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsDigitizeQa(void *p) {
      delete ((::CbmStsDigitizeQa*)p);
   }
   static void deleteArray_CbmStsDigitizeQa(void *p) {
      delete [] ((::CbmStsDigitizeQa*)p);
   }
   static void destruct_CbmStsDigitizeQa(void *p) {
      typedef ::CbmStsDigitizeQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsDigitizeQa

//______________________________________________________________________________
void CbmStsDigitizeQaReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsDigitizeQaReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsDigitizeQaReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsDigitizeQaReport::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmStsDigitizeQaReport(void *p) {
      delete ((::CbmStsDigitizeQaReport*)p);
   }
   static void deleteArray_CbmStsDigitizeQaReport(void *p) {
      delete [] ((::CbmStsDigitizeQaReport*)p);
   }
   static void destruct_CbmStsDigitizeQaReport(void *p) {
      typedef ::CbmStsDigitizeQaReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsDigitizeQaReport

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<vector<vector<int> > > > >*)
   {
      vector<vector<vector<vector<vector<int> > > > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<vector<vector<int> > > > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<vector<vector<int> > > > >", -2, "vector", 389,
                  typeid(vector<vector<vector<vector<vector<int> > > > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<vector<vector<int> > > > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<vector<vector<int> > > > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<vector<vector<int> > > > >","std::vector<std::vector<std::vector<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >, std::allocator<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > > >, std::allocator<std::vector<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >, std::allocator<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > > > > >, std::allocator<std::vector<std::vector<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >, std::allocator<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > > >, std::allocator<std::vector<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >, std::allocator<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<vector<vector<int> > > > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<vector<vector<int> > > > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<vector<int> > > > > : new vector<vector<vector<vector<vector<int> > > > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<vector<int> > > > >[nElements] : new vector<vector<vector<vector<vector<int> > > > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<vector<vector<int> > > > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<vector<vector<int> > > > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<vector<vector<int> > > > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<vector<vector<int> > > > >

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
   static TClass *setlEunsignedsPshortgR_Dictionary();
   static void setlEunsignedsPshortgR_TClassManip(TClass*);
   static void *new_setlEunsignedsPshortgR(void *p = nullptr);
   static void *newArray_setlEunsignedsPshortgR(Long_t size, void *p);
   static void delete_setlEunsignedsPshortgR(void *p);
   static void deleteArray_setlEunsignedsPshortgR(void *p);
   static void destruct_setlEunsignedsPshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const set<unsigned short>*)
   {
      set<unsigned short> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(set<unsigned short>));
      static ::ROOT::TGenericClassInfo 
         instance("set<unsigned short>", -2, "set", 94,
                  typeid(set<unsigned short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &setlEunsignedsPshortgR_Dictionary, isa_proxy, 0,
                  sizeof(set<unsigned short>) );
      instance.SetNew(&new_setlEunsignedsPshortgR);
      instance.SetNewArray(&newArray_setlEunsignedsPshortgR);
      instance.SetDelete(&delete_setlEunsignedsPshortgR);
      instance.SetDeleteArray(&deleteArray_setlEunsignedsPshortgR);
      instance.SetDestructor(&destruct_setlEunsignedsPshortgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Insert< set<unsigned short> >()));

      ::ROOT::AddClassAlternate("set<unsigned short>","std::set<unsigned short, std::less<unsigned short>, std::allocator<unsigned short> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const set<unsigned short>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *setlEunsignedsPshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const set<unsigned short>*)nullptr)->GetClass();
      setlEunsignedsPshortgR_TClassManip(theClass);
   return theClass;
   }

   static void setlEunsignedsPshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_setlEunsignedsPshortgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<unsigned short> : new set<unsigned short>;
   }
   static void *newArray_setlEunsignedsPshortgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<unsigned short>[nElements] : new set<unsigned short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_setlEunsignedsPshortgR(void *p) {
      delete ((set<unsigned short>*)p);
   }
   static void deleteArray_setlEunsignedsPshortgR(void *p) {
      delete [] ((set<unsigned short>*)p);
   }
   static void destruct_setlEunsignedsPshortgR(void *p) {
      typedef set<unsigned short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class set<unsigned short>

namespace ROOT {
   static TClass *multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR_Dictionary();
   static void multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR_TClassManip(TClass*);
   static void *new_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR(void *p = nullptr);
   static void *newArray_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR(Long_t size, void *p);
   static void delete_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR(void *p);
   static void deleteArray_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR(void *p);
   static void destruct_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const multiset<CbmStsSignal*,CbmStsSignal::Before>*)
   {
      multiset<CbmStsSignal*,CbmStsSignal::Before> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(multiset<CbmStsSignal*,CbmStsSignal::Before>));
      static ::ROOT::TGenericClassInfo 
         instance("multiset<CbmStsSignal*,CbmStsSignal::Before>", -2, "set", 96,
                  typeid(multiset<CbmStsSignal*,CbmStsSignal::Before>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR_Dictionary, isa_proxy, 0,
                  sizeof(multiset<CbmStsSignal*,CbmStsSignal::Before>) );
      instance.SetNew(&new_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR);
      instance.SetNewArray(&newArray_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR);
      instance.SetDelete(&delete_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR);
      instance.SetDeleteArray(&deleteArray_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR);
      instance.SetDestructor(&destruct_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Insert< multiset<CbmStsSignal*,CbmStsSignal::Before> >()));

      ::ROOT::AddClassAlternate("multiset<CbmStsSignal*,CbmStsSignal::Before>","std::multiset<CbmStsSignal*, CbmStsSignal::Before, std::allocator<CbmStsSignal*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const multiset<CbmStsSignal*,CbmStsSignal::Before>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const multiset<CbmStsSignal*,CbmStsSignal::Before>*)nullptr)->GetClass();
      multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR_TClassManip(theClass);
   return theClass;
   }

   static void multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) multiset<CbmStsSignal*,CbmStsSignal::Before> : new multiset<CbmStsSignal*,CbmStsSignal::Before>;
   }
   static void *newArray_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) multiset<CbmStsSignal*,CbmStsSignal::Before>[nElements] : new multiset<CbmStsSignal*,CbmStsSignal::Before>[nElements];
   }
   // Wrapper around operator delete
   static void delete_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR(void *p) {
      delete ((multiset<CbmStsSignal*,CbmStsSignal::Before>*)p);
   }
   static void deleteArray_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR(void *p) {
      delete [] ((multiset<CbmStsSignal*,CbmStsSignal::Before>*)p);
   }
   static void destruct_multisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegR(void *p) {
      typedef multiset<CbmStsSignal*,CbmStsSignal::Before> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class multiset<CbmStsSignal*,CbmStsSignal::Before>

namespace ROOT {
   static TClass *maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR_Dictionary();
   static void maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR(Long_t size, void *p);
   static void delete_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR(void *p);
   static void deleteArray_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR(void *p);
   static void destruct_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> >*)
   {
      map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> >", -2, "map", 100,
                  typeid(map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> >) );
      instance.SetNew(&new_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR);
      instance.SetNewArray(&newArray_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR);
      instance.SetDelete(&delete_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR);
      instance.SetDestructor(&destruct_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> > >()));

      ::ROOT::AddClassAlternate("map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> >","std::map<unsigned short, std::multiset<CbmStsSignal*, CbmStsSignal::Before, std::allocator<CbmStsSignal*> >, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, std::multiset<CbmStsSignal*, CbmStsSignal::Before, std::allocator<CbmStsSignal*> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> >*)nullptr)->GetClass();
      maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> > : new map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> >;
   }
   static void *newArray_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> >[nElements] : new map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR(void *p) {
      delete ((map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> >*)p);
   }
   static void deleteArray_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR(void *p) {
      delete [] ((map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> >*)p);
   }
   static void destruct_maplEunsignedsPshortcOmultisetlECbmStsSignalmUcOCbmStsSignalcLcLBeforegRsPgR(void *p) {
      typedef map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned short,multiset<CbmStsSignal*,CbmStsSignal::Before> >

namespace ROOT {
   static TClass *maplEunsignedsPintcOCbmStsSimSensormUgR_Dictionary();
   static void maplEunsignedsPintcOCbmStsSimSensormUgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOCbmStsSimSensormUgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOCbmStsSimSensormUgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOCbmStsSimSensormUgR(void *p);
   static void deleteArray_maplEunsignedsPintcOCbmStsSimSensormUgR(void *p);
   static void destruct_maplEunsignedsPintcOCbmStsSimSensormUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,CbmStsSimSensor*>*)
   {
      map<unsigned int,CbmStsSimSensor*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,CbmStsSimSensor*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,CbmStsSimSensor*>", -2, "map", 100,
                  typeid(map<unsigned int,CbmStsSimSensor*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOCbmStsSimSensormUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,CbmStsSimSensor*>) );
      instance.SetNew(&new_maplEunsignedsPintcOCbmStsSimSensormUgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOCbmStsSimSensormUgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOCbmStsSimSensormUgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOCbmStsSimSensormUgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOCbmStsSimSensormUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,CbmStsSimSensor*> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,CbmStsSimSensor*>","std::map<unsigned int, CbmStsSimSensor*, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, CbmStsSimSensor*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,CbmStsSimSensor*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOCbmStsSimSensormUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,CbmStsSimSensor*>*)nullptr)->GetClass();
      maplEunsignedsPintcOCbmStsSimSensormUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOCbmStsSimSensormUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOCbmStsSimSensormUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,CbmStsSimSensor*> : new map<unsigned int,CbmStsSimSensor*>;
   }
   static void *newArray_maplEunsignedsPintcOCbmStsSimSensormUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,CbmStsSimSensor*>[nElements] : new map<unsigned int,CbmStsSimSensor*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOCbmStsSimSensormUgR(void *p) {
      delete ((map<unsigned int,CbmStsSimSensor*>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOCbmStsSimSensormUgR(void *p) {
      delete [] ((map<unsigned int,CbmStsSimSensor*>*)p);
   }
   static void destruct_maplEunsignedsPintcOCbmStsSimSensormUgR(void *p) {
      typedef map<unsigned int,CbmStsSimSensor*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,CbmStsSimSensor*>

namespace ROOT {
   static TClass *maplEunsignedsPintcOCbmStsSimModulemUgR_Dictionary();
   static void maplEunsignedsPintcOCbmStsSimModulemUgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOCbmStsSimModulemUgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOCbmStsSimModulemUgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOCbmStsSimModulemUgR(void *p);
   static void deleteArray_maplEunsignedsPintcOCbmStsSimModulemUgR(void *p);
   static void destruct_maplEunsignedsPintcOCbmStsSimModulemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,CbmStsSimModule*>*)
   {
      map<unsigned int,CbmStsSimModule*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,CbmStsSimModule*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,CbmStsSimModule*>", -2, "map", 100,
                  typeid(map<unsigned int,CbmStsSimModule*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOCbmStsSimModulemUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,CbmStsSimModule*>) );
      instance.SetNew(&new_maplEunsignedsPintcOCbmStsSimModulemUgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOCbmStsSimModulemUgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOCbmStsSimModulemUgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOCbmStsSimModulemUgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOCbmStsSimModulemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,CbmStsSimModule*> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,CbmStsSimModule*>","std::map<unsigned int, CbmStsSimModule*, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, CbmStsSimModule*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,CbmStsSimModule*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOCbmStsSimModulemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,CbmStsSimModule*>*)nullptr)->GetClass();
      maplEunsignedsPintcOCbmStsSimModulemUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOCbmStsSimModulemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOCbmStsSimModulemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,CbmStsSimModule*> : new map<unsigned int,CbmStsSimModule*>;
   }
   static void *newArray_maplEunsignedsPintcOCbmStsSimModulemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,CbmStsSimModule*>[nElements] : new map<unsigned int,CbmStsSimModule*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOCbmStsSimModulemUgR(void *p) {
      delete ((map<unsigned int,CbmStsSimModule*>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOCbmStsSimModulemUgR(void *p) {
      delete [] ((map<unsigned int,CbmStsSimModule*>*)p);
   }
   static void destruct_maplEunsignedsPintcOCbmStsSimModulemUgR(void *p) {
      typedef map<unsigned int,CbmStsSimModule*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,CbmStsSimModule*>

namespace ROOT {
   static TClass *maplEintcOsetlEunsignedsPshortgRsPgR_Dictionary();
   static void maplEintcOsetlEunsignedsPshortgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOsetlEunsignedsPshortgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOsetlEunsignedsPshortgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOsetlEunsignedsPshortgRsPgR(void *p);
   static void deleteArray_maplEintcOsetlEunsignedsPshortgRsPgR(void *p);
   static void destruct_maplEintcOsetlEunsignedsPshortgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,set<unsigned short> >*)
   {
      map<int,set<unsigned short> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,set<unsigned short> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,set<unsigned short> >", -2, "map", 100,
                  typeid(map<int,set<unsigned short> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOsetlEunsignedsPshortgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,set<unsigned short> >) );
      instance.SetNew(&new_maplEintcOsetlEunsignedsPshortgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOsetlEunsignedsPshortgRsPgR);
      instance.SetDelete(&delete_maplEintcOsetlEunsignedsPshortgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOsetlEunsignedsPshortgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOsetlEunsignedsPshortgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,set<unsigned short> > >()));

      ::ROOT::AddClassAlternate("map<int,set<unsigned short> >","std::map<int, std::set<unsigned short, std::less<unsigned short>, std::allocator<unsigned short> >, std::less<int>, std::allocator<std::pair<int const, std::set<unsigned short, std::less<unsigned short>, std::allocator<unsigned short> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,set<unsigned short> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOsetlEunsignedsPshortgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,set<unsigned short> >*)nullptr)->GetClass();
      maplEintcOsetlEunsignedsPshortgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOsetlEunsignedsPshortgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOsetlEunsignedsPshortgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,set<unsigned short> > : new map<int,set<unsigned short> >;
   }
   static void *newArray_maplEintcOsetlEunsignedsPshortgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,set<unsigned short> >[nElements] : new map<int,set<unsigned short> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOsetlEunsignedsPshortgRsPgR(void *p) {
      delete ((map<int,set<unsigned short> >*)p);
   }
   static void deleteArray_maplEintcOsetlEunsignedsPshortgRsPgR(void *p) {
      delete [] ((map<int,set<unsigned short> >*)p);
   }
   static void destruct_maplEintcOsetlEunsignedsPshortgRsPgR(void *p) {
      typedef map<int,set<unsigned short> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,set<unsigned short> >

namespace ROOT {
   static TClass *maplETStringcOintgR_Dictionary();
   static void maplETStringcOintgR_TClassManip(TClass*);
   static void *new_maplETStringcOintgR(void *p = nullptr);
   static void *newArray_maplETStringcOintgR(Long_t size, void *p);
   static void delete_maplETStringcOintgR(void *p);
   static void deleteArray_maplETStringcOintgR(void *p);
   static void destruct_maplETStringcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,int>*)
   {
      map<TString,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,int>", -2, "map", 100,
                  typeid(map<TString,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,int>) );
      instance.SetNew(&new_maplETStringcOintgR);
      instance.SetNewArray(&newArray_maplETStringcOintgR);
      instance.SetDelete(&delete_maplETStringcOintgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOintgR);
      instance.SetDestructor(&destruct_maplETStringcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,int> >()));

      ::ROOT::AddClassAlternate("map<TString,int>","std::map<TString, int, std::less<TString>, std::allocator<std::pair<TString const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,int>*)nullptr)->GetClass();
      maplETStringcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,int> : new map<TString,int>;
   }
   static void *newArray_maplETStringcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,int>[nElements] : new map<TString,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOintgR(void *p) {
      delete ((map<TString,int>*)p);
   }
   static void deleteArray_maplETStringcOintgR(void *p) {
      delete [] ((map<TString,int>*)p);
   }
   static void destruct_maplETStringcOintgR(void *p) {
      typedef map<TString,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,int>

namespace {
  void TriggerDictionaryInitialization_libCbmStsSim_Impl() {
    static const char* headers[] = {
"CbmStsDigitize.h",
"CbmStsMC.h",
"CbmStsSensorPoint.h",
"CbmStsSignal.h",
"CbmStsSimModule.h",
"CbmStsSimSensor.h",
"CbmStsSimSensorDssd.h",
"CbmStsSimSensorDssdOrtho.h",
"CbmStsSimSensorDssdStereo.h",
"CbmStsSimSensorFactory.h",
"CbmStsDigitizePixel.h",
"CbmStsDigitizeQa.h",
"CbmStsDigitizeQaReport.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/sts/qa",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/sts/qa",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/sts/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmStsSim dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmStsDigi.h")))  __attribute__((annotate("$clingAutoload$CbmStsDigitize.h")))  CbmStsDigi;
template <class Digi> class __attribute__((annotate("$clingAutoload$CbmDigitize.h")))  __attribute__((annotate("$clingAutoload$CbmStsDigitize.h")))  CbmDigitize;

class __attribute__((annotate("$clingAutoload$CbmStsSignal.h")))  __attribute__((annotate("$clingAutoload$CbmStsDigitize.h")))  CbmStsSignal;
class __attribute__((annotate("$clingAutoload$CbmStsSimModule.h")))  __attribute__((annotate("$clingAutoload$CbmStsDigitize.h")))  CbmStsSimModule;
class __attribute__((annotate("$clingAutoload$CbmStsSimSensor.h")))  __attribute__((annotate("$clingAutoload$CbmStsDigitize.h")))  CbmStsSimSensor;
class __attribute__((annotate("$clingAutoload$CbmStsDigitize.h")))  CbmStsDigitize;
class __attribute__((annotate("$clingAutoload$CbmStsMC.h")))  CbmStsMC;
class __attribute__((annotate("$clingAutoload$CbmStsSensorPoint.h")))  CbmStsSensorPoint;
class __attribute__((annotate("$clingAutoload$CbmStsSimSensorDssd.h")))  CbmStsSimSensorDssd;
class __attribute__((annotate("$clingAutoload$CbmStsSimSensorDssdOrtho.h")))  CbmStsSimSensorDssdOrtho;
class __attribute__((annotate("$clingAutoload$CbmStsSimSensorDssdStereo.h")))  CbmStsSimSensorDssdStereo;
class __attribute__((annotate("$clingAutoload$CbmStsSimSensorFactory.h")))  CbmStsSimSensorFactory;
class __attribute__((annotate("$clingAutoload$CbmStsDigitizePixel.h")))  CbmStsDigitizePixel;
class __attribute__((annotate("$clingAutoload$CbmStsDigitizeQa.h")))  CbmStsDigitizeQa;
class __attribute__((annotate("$clingAutoload$CbmStsDigitizeQaReport.h")))  CbmStsDigitizeQaReport;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmStsSim dictionary payload"

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
#include "CbmStsDigitize.h"
#include "CbmStsMC.h"
#include "CbmStsSensorPoint.h"
#include "CbmStsSignal.h"
#include "CbmStsSimModule.h"
#include "CbmStsSimSensor.h"
#include "CbmStsSimSensorDssd.h"
#include "CbmStsSimSensorDssdOrtho.h"
#include "CbmStsSimSensorDssdStereo.h"
#include "CbmStsSimSensorFactory.h"
#include "CbmStsDigitizePixel.h"
#include "CbmStsDigitizeQa.h"
#include "CbmStsDigitizeQaReport.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmDigitize<CbmStsDigi>", payloadCode, "@",
"CbmStsDigitize", payloadCode, "@",
"CbmStsDigitizePixel", payloadCode, "@",
"CbmStsDigitizeQa", payloadCode, "@",
"CbmStsDigitizeQaReport", payloadCode, "@",
"CbmStsMC", payloadCode, "@",
"CbmStsSensorPoint", payloadCode, "@",
"CbmStsSignal", payloadCode, "@",
"CbmStsSimModule", payloadCode, "@",
"CbmStsSimSensor", payloadCode, "@",
"CbmStsSimSensorDssd", payloadCode, "@",
"CbmStsSimSensorDssdOrtho", payloadCode, "@",
"CbmStsSimSensorDssdStereo", payloadCode, "@",
"CbmStsSimSensorFactory", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmStsSim",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmStsSim_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmStsSim_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmStsSim() {
  TriggerDictionaryInitialization_libCbmStsSim_Impl();
}
