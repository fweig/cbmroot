// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmStsBase
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
#include "CbmStsContFact.h"
#include "CbmStsElement.h"
#include "CbmStsModule.h"
#include "CbmStsParAsic.h"
#include "CbmStsParModule.h"
#include "CbmStsParSensor.h"
#include "CbmStsParSensorCond.h"
#include "CbmStsParSetModule.h"
#include "CbmStsParSetSensor.h"
#include "CbmStsParSetSensorCond.h"
#include "CbmStsParSim.h"
#include "CbmMcbm2018StsPar.h"
#include "CbmStsPhysics.h"
#include "CbmStsSensor.h"
#include "CbmStsSetup.h"
#include "CbmStsStation.h"
#include "CbmStsTrackingInterface.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmStsContFact(void *p = nullptr);
   static void *newArray_CbmStsContFact(Long_t size, void *p);
   static void delete_CbmStsContFact(void *p);
   static void deleteArray_CbmStsContFact(void *p);
   static void destruct_CbmStsContFact(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsContFact*)
   {
      ::CbmStsContFact *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsContFact >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsContFact", ::CbmStsContFact::Class_Version(), "CbmStsContFact.h", 31,
                  typeid(::CbmStsContFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsContFact::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsContFact) );
      instance.SetNew(&new_CbmStsContFact);
      instance.SetNewArray(&newArray_CbmStsContFact);
      instance.SetDelete(&delete_CbmStsContFact);
      instance.SetDeleteArray(&deleteArray_CbmStsContFact);
      instance.SetDestructor(&destruct_CbmStsContFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsContFact*)
   {
      return GenerateInitInstanceLocal((::CbmStsContFact*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsContFact*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsElement(void *p = nullptr);
   static void *newArray_CbmStsElement(Long_t size, void *p);
   static void delete_CbmStsElement(void *p);
   static void deleteArray_CbmStsElement(void *p);
   static void destruct_CbmStsElement(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsElement*)
   {
      ::CbmStsElement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsElement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsElement", ::CbmStsElement::Class_Version(), "CbmStsElement.h", 35,
                  typeid(::CbmStsElement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsElement::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsElement) );
      instance.SetNew(&new_CbmStsElement);
      instance.SetNewArray(&newArray_CbmStsElement);
      instance.SetDelete(&delete_CbmStsElement);
      instance.SetDeleteArray(&deleteArray_CbmStsElement);
      instance.SetDestructor(&destruct_CbmStsElement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsElement*)
   {
      return GenerateInitInstanceLocal((::CbmStsElement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsElement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsParAsic(void *p = nullptr);
   static void *newArray_CbmStsParAsic(Long_t size, void *p);
   static void delete_CbmStsParAsic(void *p);
   static void deleteArray_CbmStsParAsic(void *p);
   static void destruct_CbmStsParAsic(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsParAsic*)
   {
      ::CbmStsParAsic *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsParAsic >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsParAsic", ::CbmStsParAsic::Class_Version(), "CbmStsParAsic.h", 23,
                  typeid(::CbmStsParAsic), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsParAsic::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsParAsic) );
      instance.SetNew(&new_CbmStsParAsic);
      instance.SetNewArray(&newArray_CbmStsParAsic);
      instance.SetDelete(&delete_CbmStsParAsic);
      instance.SetDeleteArray(&deleteArray_CbmStsParAsic);
      instance.SetDestructor(&destruct_CbmStsParAsic);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsParAsic*)
   {
      return GenerateInitInstanceLocal((::CbmStsParAsic*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsParAsic*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsParModule(void *p = nullptr);
   static void *newArray_CbmStsParModule(Long_t size, void *p);
   static void delete_CbmStsParModule(void *p);
   static void deleteArray_CbmStsParModule(void *p);
   static void destruct_CbmStsParModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsParModule*)
   {
      ::CbmStsParModule *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsParModule >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsParModule", ::CbmStsParModule::Class_Version(), "CbmStsParModule.h", 27,
                  typeid(::CbmStsParModule), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsParModule::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsParModule) );
      instance.SetNew(&new_CbmStsParModule);
      instance.SetNewArray(&newArray_CbmStsParModule);
      instance.SetDelete(&delete_CbmStsParModule);
      instance.SetDeleteArray(&deleteArray_CbmStsParModule);
      instance.SetDestructor(&destruct_CbmStsParModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsParModule*)
   {
      return GenerateInitInstanceLocal((::CbmStsParModule*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsParModule*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsModule(void *p = nullptr);
   static void *newArray_CbmStsModule(Long_t size, void *p);
   static void delete_CbmStsModule(void *p);
   static void deleteArray_CbmStsModule(void *p);
   static void destruct_CbmStsModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsModule*)
   {
      ::CbmStsModule *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsModule >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsModule", ::CbmStsModule::Class_Version(), "CbmStsModule.h", 35,
                  typeid(::CbmStsModule), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsModule::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsModule) );
      instance.SetNew(&new_CbmStsModule);
      instance.SetNewArray(&newArray_CbmStsModule);
      instance.SetDelete(&delete_CbmStsModule);
      instance.SetDeleteArray(&deleteArray_CbmStsModule);
      instance.SetDestructor(&destruct_CbmStsModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsModule*)
   {
      return GenerateInitInstanceLocal((::CbmStsModule*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsModule*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsParSensor(void *p = nullptr);
   static void *newArray_CbmStsParSensor(Long_t size, void *p);
   static void delete_CbmStsParSensor(void *p);
   static void deleteArray_CbmStsParSensor(void *p);
   static void destruct_CbmStsParSensor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsParSensor*)
   {
      ::CbmStsParSensor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsParSensor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsParSensor", ::CbmStsParSensor::Class_Version(), "CbmStsParSensor.h", 42,
                  typeid(::CbmStsParSensor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsParSensor::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsParSensor) );
      instance.SetNew(&new_CbmStsParSensor);
      instance.SetNewArray(&newArray_CbmStsParSensor);
      instance.SetDelete(&delete_CbmStsParSensor);
      instance.SetDeleteArray(&deleteArray_CbmStsParSensor);
      instance.SetDestructor(&destruct_CbmStsParSensor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsParSensor*)
   {
      return GenerateInitInstanceLocal((::CbmStsParSensor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsParSensor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsParSensorCond(void *p = nullptr);
   static void *newArray_CbmStsParSensorCond(Long_t size, void *p);
   static void delete_CbmStsParSensorCond(void *p);
   static void deleteArray_CbmStsParSensorCond(void *p);
   static void destruct_CbmStsParSensorCond(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsParSensorCond*)
   {
      ::CbmStsParSensorCond *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsParSensorCond >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsParSensorCond", ::CbmStsParSensorCond::Class_Version(), "CbmStsParSensorCond.h", 32,
                  typeid(::CbmStsParSensorCond), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsParSensorCond::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsParSensorCond) );
      instance.SetNew(&new_CbmStsParSensorCond);
      instance.SetNewArray(&newArray_CbmStsParSensorCond);
      instance.SetDelete(&delete_CbmStsParSensorCond);
      instance.SetDeleteArray(&deleteArray_CbmStsParSensorCond);
      instance.SetDestructor(&destruct_CbmStsParSensorCond);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsParSensorCond*)
   {
      return GenerateInitInstanceLocal((::CbmStsParSensorCond*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsParSensorCond*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsParSetModule(void *p = nullptr);
   static void *newArray_CbmStsParSetModule(Long_t size, void *p);
   static void delete_CbmStsParSetModule(void *p);
   static void deleteArray_CbmStsParSetModule(void *p);
   static void destruct_CbmStsParSetModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsParSetModule*)
   {
      ::CbmStsParSetModule *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsParSetModule >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsParSetModule", ::CbmStsParSetModule::Class_Version(), "CbmStsParSetModule.h", 34,
                  typeid(::CbmStsParSetModule), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsParSetModule::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsParSetModule) );
      instance.SetNew(&new_CbmStsParSetModule);
      instance.SetNewArray(&newArray_CbmStsParSetModule);
      instance.SetDelete(&delete_CbmStsParSetModule);
      instance.SetDeleteArray(&deleteArray_CbmStsParSetModule);
      instance.SetDestructor(&destruct_CbmStsParSetModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsParSetModule*)
   {
      return GenerateInitInstanceLocal((::CbmStsParSetModule*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsParSetModule*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsParSetSensor(void *p = nullptr);
   static void *newArray_CbmStsParSetSensor(Long_t size, void *p);
   static void delete_CbmStsParSetSensor(void *p);
   static void deleteArray_CbmStsParSetSensor(void *p);
   static void destruct_CbmStsParSetSensor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsParSetSensor*)
   {
      ::CbmStsParSetSensor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsParSetSensor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsParSetSensor", ::CbmStsParSetSensor::Class_Version(), "CbmStsParSetSensor.h", 34,
                  typeid(::CbmStsParSetSensor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsParSetSensor::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsParSetSensor) );
      instance.SetNew(&new_CbmStsParSetSensor);
      instance.SetNewArray(&newArray_CbmStsParSetSensor);
      instance.SetDelete(&delete_CbmStsParSetSensor);
      instance.SetDeleteArray(&deleteArray_CbmStsParSetSensor);
      instance.SetDestructor(&destruct_CbmStsParSetSensor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsParSetSensor*)
   {
      return GenerateInitInstanceLocal((::CbmStsParSetSensor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsParSetSensor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsParSetSensorCond(void *p = nullptr);
   static void *newArray_CbmStsParSetSensorCond(Long_t size, void *p);
   static void delete_CbmStsParSetSensorCond(void *p);
   static void deleteArray_CbmStsParSetSensorCond(void *p);
   static void destruct_CbmStsParSetSensorCond(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsParSetSensorCond*)
   {
      ::CbmStsParSetSensorCond *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsParSetSensorCond >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsParSetSensorCond", ::CbmStsParSetSensorCond::Class_Version(), "CbmStsParSetSensorCond.h", 34,
                  typeid(::CbmStsParSetSensorCond), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsParSetSensorCond::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsParSetSensorCond) );
      instance.SetNew(&new_CbmStsParSetSensorCond);
      instance.SetNewArray(&newArray_CbmStsParSetSensorCond);
      instance.SetDelete(&delete_CbmStsParSetSensorCond);
      instance.SetDeleteArray(&deleteArray_CbmStsParSetSensorCond);
      instance.SetDestructor(&destruct_CbmStsParSetSensorCond);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsParSetSensorCond*)
   {
      return GenerateInitInstanceLocal((::CbmStsParSetSensorCond*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsParSetSensorCond*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsParSim(void *p = nullptr);
   static void *newArray_CbmStsParSim(Long_t size, void *p);
   static void delete_CbmStsParSim(void *p);
   static void deleteArray_CbmStsParSim(void *p);
   static void destruct_CbmStsParSim(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsParSim*)
   {
      ::CbmStsParSim *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsParSim >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsParSim", ::CbmStsParSim::Class_Version(), "CbmStsParSim.h", 29,
                  typeid(::CbmStsParSim), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsParSim::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsParSim) );
      instance.SetNew(&new_CbmStsParSim);
      instance.SetNewArray(&newArray_CbmStsParSim);
      instance.SetDelete(&delete_CbmStsParSim);
      instance.SetDeleteArray(&deleteArray_CbmStsParSim);
      instance.SetDestructor(&destruct_CbmStsParSim);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsParSim*)
   {
      return GenerateInitInstanceLocal((::CbmStsParSim*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsParSim*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018StsPar(void *p = nullptr);
   static void *newArray_CbmMcbm2018StsPar(Long_t size, void *p);
   static void delete_CbmMcbm2018StsPar(void *p);
   static void deleteArray_CbmMcbm2018StsPar(void *p);
   static void destruct_CbmMcbm2018StsPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018StsPar*)
   {
      ::CbmMcbm2018StsPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018StsPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018StsPar", ::CbmMcbm2018StsPar::Class_Version(), "CbmMcbm2018StsPar.h", 22,
                  typeid(::CbmMcbm2018StsPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018StsPar::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2018StsPar) );
      instance.SetNew(&new_CbmMcbm2018StsPar);
      instance.SetNewArray(&newArray_CbmMcbm2018StsPar);
      instance.SetDelete(&delete_CbmMcbm2018StsPar);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018StsPar);
      instance.SetDestructor(&destruct_CbmMcbm2018StsPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018StsPar*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018StsPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018StsPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmStsPhysics(void *p);
   static void deleteArray_CbmStsPhysics(void *p);
   static void destruct_CbmStsPhysics(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsPhysics*)
   {
      ::CbmStsPhysics *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsPhysics >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsPhysics", ::CbmStsPhysics::Class_Version(), "CbmStsPhysics.h", 30,
                  typeid(::CbmStsPhysics), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsPhysics::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsPhysics) );
      instance.SetDelete(&delete_CbmStsPhysics);
      instance.SetDeleteArray(&deleteArray_CbmStsPhysics);
      instance.SetDestructor(&destruct_CbmStsPhysics);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsPhysics*)
   {
      return GenerateInitInstanceLocal((::CbmStsPhysics*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsPhysics*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsSensor(void *p = nullptr);
   static void *newArray_CbmStsSensor(Long_t size, void *p);
   static void delete_CbmStsSensor(void *p);
   static void deleteArray_CbmStsSensor(void *p);
   static void destruct_CbmStsSensor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsSensor*)
   {
      ::CbmStsSensor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsSensor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsSensor", ::CbmStsSensor::Class_Version(), "CbmStsSensor.h", 42,
                  typeid(::CbmStsSensor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsSensor::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsSensor) );
      instance.SetNew(&new_CbmStsSensor);
      instance.SetNewArray(&newArray_CbmStsSensor);
      instance.SetDelete(&delete_CbmStsSensor);
      instance.SetDeleteArray(&deleteArray_CbmStsSensor);
      instance.SetDestructor(&destruct_CbmStsSensor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsSensor*)
   {
      return GenerateInitInstanceLocal((::CbmStsSensor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsSensor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmStsSetup(void *p);
   static void deleteArray_CbmStsSetup(void *p);
   static void destruct_CbmStsSetup(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsSetup*)
   {
      ::CbmStsSetup *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsSetup >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsSetup", ::CbmStsSetup::Class_Version(), "CbmStsSetup.h", 43,
                  typeid(::CbmStsSetup), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsSetup::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsSetup) );
      instance.SetDelete(&delete_CbmStsSetup);
      instance.SetDeleteArray(&deleteArray_CbmStsSetup);
      instance.SetDestructor(&destruct_CbmStsSetup);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsSetup*)
   {
      return GenerateInitInstanceLocal((::CbmStsSetup*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsSetup*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsStation(void *p = nullptr);
   static void *newArray_CbmStsStation(Long_t size, void *p);
   static void delete_CbmStsStation(void *p);
   static void deleteArray_CbmStsStation(void *p);
   static void destruct_CbmStsStation(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsStation*)
   {
      ::CbmStsStation *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsStation >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsStation", ::CbmStsStation::Class_Version(), "CbmStsStation.h", 33,
                  typeid(::CbmStsStation), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsStation::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsStation) );
      instance.SetNew(&new_CbmStsStation);
      instance.SetNewArray(&newArray_CbmStsStation);
      instance.SetDelete(&delete_CbmStsStation);
      instance.SetDeleteArray(&deleteArray_CbmStsStation);
      instance.SetDestructor(&destruct_CbmStsStation);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsStation*)
   {
      return GenerateInitInstanceLocal((::CbmStsStation*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsStation*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsTrackingInterface(void *p = nullptr);
   static void *newArray_CbmStsTrackingInterface(Long_t size, void *p);
   static void delete_CbmStsTrackingInterface(void *p);
   static void deleteArray_CbmStsTrackingInterface(void *p);
   static void destruct_CbmStsTrackingInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsTrackingInterface*)
   {
      ::CbmStsTrackingInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsTrackingInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsTrackingInterface", ::CbmStsTrackingInterface::Class_Version(), "CbmStsTrackingInterface.h", 33,
                  typeid(::CbmStsTrackingInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsTrackingInterface::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsTrackingInterface) );
      instance.SetNew(&new_CbmStsTrackingInterface);
      instance.SetNewArray(&newArray_CbmStsTrackingInterface);
      instance.SetDelete(&delete_CbmStsTrackingInterface);
      instance.SetDeleteArray(&deleteArray_CbmStsTrackingInterface);
      instance.SetDestructor(&destruct_CbmStsTrackingInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsTrackingInterface*)
   {
      return GenerateInitInstanceLocal((::CbmStsTrackingInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsTrackingInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmStsContFact::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsContFact::Class_Name()
{
   return "CbmStsContFact";
}

//______________________________________________________________________________
const char *CbmStsContFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsContFact*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsContFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsContFact*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsContFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsContFact*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsContFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsContFact*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsElement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsElement::Class_Name()
{
   return "CbmStsElement";
}

//______________________________________________________________________________
const char *CbmStsElement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsElement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsElement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsElement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsElement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsElement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsElement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsElement*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsParAsic::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsParAsic::Class_Name()
{
   return "CbmStsParAsic";
}

//______________________________________________________________________________
const char *CbmStsParAsic::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParAsic*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsParAsic::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParAsic*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsParAsic::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParAsic*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsParAsic::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParAsic*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsParModule::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsParModule::Class_Name()
{
   return "CbmStsParModule";
}

//______________________________________________________________________________
const char *CbmStsParModule::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParModule*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsParModule::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParModule*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsParModule::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParModule*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsParModule::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParModule*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsModule::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsModule::Class_Name()
{
   return "CbmStsModule";
}

//______________________________________________________________________________
const char *CbmStsModule::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsModule*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsModule::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsModule*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsModule::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsModule*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsModule::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsModule*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsParSensor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsParSensor::Class_Name()
{
   return "CbmStsParSensor";
}

//______________________________________________________________________________
const char *CbmStsParSensor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSensor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsParSensor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSensor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsParSensor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSensor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsParSensor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSensor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsParSensorCond::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsParSensorCond::Class_Name()
{
   return "CbmStsParSensorCond";
}

//______________________________________________________________________________
const char *CbmStsParSensorCond::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSensorCond*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsParSensorCond::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSensorCond*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsParSensorCond::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSensorCond*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsParSensorCond::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSensorCond*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsParSetModule::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsParSetModule::Class_Name()
{
   return "CbmStsParSetModule";
}

//______________________________________________________________________________
const char *CbmStsParSetModule::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSetModule*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsParSetModule::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSetModule*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsParSetModule::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSetModule*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsParSetModule::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSetModule*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsParSetSensor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsParSetSensor::Class_Name()
{
   return "CbmStsParSetSensor";
}

//______________________________________________________________________________
const char *CbmStsParSetSensor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSetSensor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsParSetSensor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSetSensor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsParSetSensor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSetSensor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsParSetSensor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSetSensor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsParSetSensorCond::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsParSetSensorCond::Class_Name()
{
   return "CbmStsParSetSensorCond";
}

//______________________________________________________________________________
const char *CbmStsParSetSensorCond::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSetSensorCond*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsParSetSensorCond::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSetSensorCond*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsParSetSensorCond::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSetSensorCond*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsParSetSensorCond::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSetSensorCond*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsParSim::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsParSim::Class_Name()
{
   return "CbmStsParSim";
}

//______________________________________________________________________________
const char *CbmStsParSim::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSim*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsParSim::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSim*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsParSim::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSim*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsParSim::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsParSim*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018StsPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018StsPar::Class_Name()
{
   return "CbmMcbm2018StsPar";
}

//______________________________________________________________________________
const char *CbmMcbm2018StsPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018StsPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018StsPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018StsPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018StsPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018StsPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018StsPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018StsPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsPhysics::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsPhysics::Class_Name()
{
   return "CbmStsPhysics";
}

//______________________________________________________________________________
const char *CbmStsPhysics::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsPhysics*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsPhysics::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsPhysics*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsPhysics::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsPhysics*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsPhysics::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsPhysics*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsSensor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsSensor::Class_Name()
{
   return "CbmStsSensor";
}

//______________________________________________________________________________
const char *CbmStsSensor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSensor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsSensor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSensor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsSensor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSensor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsSensor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSensor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsSetup::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsSetup::Class_Name()
{
   return "CbmStsSetup";
}

//______________________________________________________________________________
const char *CbmStsSetup::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSetup*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsSetup::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSetup*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsSetup::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSetup*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsSetup::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsSetup*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsStation::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsStation::Class_Name()
{
   return "CbmStsStation";
}

//______________________________________________________________________________
const char *CbmStsStation::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsStation*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsStation::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsStation*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsStation::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsStation*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsStation::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsStation*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsTrackingInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsTrackingInterface::Class_Name()
{
   return "CbmStsTrackingInterface";
}

//______________________________________________________________________________
const char *CbmStsTrackingInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackingInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsTrackingInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackingInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsTrackingInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackingInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsTrackingInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackingInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmStsContFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsContFact.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsContFact::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsContFact::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsContFact(void *p) {
      return  p ? new(p) ::CbmStsContFact : new ::CbmStsContFact;
   }
   static void *newArray_CbmStsContFact(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsContFact[nElements] : new ::CbmStsContFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsContFact(void *p) {
      delete ((::CbmStsContFact*)p);
   }
   static void deleteArray_CbmStsContFact(void *p) {
      delete [] ((::CbmStsContFact*)p);
   }
   static void destruct_CbmStsContFact(void *p) {
      typedef ::CbmStsContFact current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsContFact

//______________________________________________________________________________
void CbmStsElement::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsElement.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsElement::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsElement::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsElement(void *p) {
      return  p ? new(p) ::CbmStsElement : new ::CbmStsElement;
   }
   static void *newArray_CbmStsElement(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsElement[nElements] : new ::CbmStsElement[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsElement(void *p) {
      delete ((::CbmStsElement*)p);
   }
   static void deleteArray_CbmStsElement(void *p) {
      delete [] ((::CbmStsElement*)p);
   }
   static void destruct_CbmStsElement(void *p) {
      typedef ::CbmStsElement current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsElement

//______________________________________________________________________________
void CbmStsParAsic::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsParAsic.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsParAsic::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsParAsic::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsParAsic(void *p) {
      return  p ? new(p) ::CbmStsParAsic : new ::CbmStsParAsic;
   }
   static void *newArray_CbmStsParAsic(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsParAsic[nElements] : new ::CbmStsParAsic[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsParAsic(void *p) {
      delete ((::CbmStsParAsic*)p);
   }
   static void deleteArray_CbmStsParAsic(void *p) {
      delete [] ((::CbmStsParAsic*)p);
   }
   static void destruct_CbmStsParAsic(void *p) {
      typedef ::CbmStsParAsic current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsParAsic

//______________________________________________________________________________
void CbmStsParModule::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsParModule.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsParModule::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsParModule::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsParModule(void *p) {
      return  p ? new(p) ::CbmStsParModule : new ::CbmStsParModule;
   }
   static void *newArray_CbmStsParModule(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsParModule[nElements] : new ::CbmStsParModule[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsParModule(void *p) {
      delete ((::CbmStsParModule*)p);
   }
   static void deleteArray_CbmStsParModule(void *p) {
      delete [] ((::CbmStsParModule*)p);
   }
   static void destruct_CbmStsParModule(void *p) {
      typedef ::CbmStsParModule current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsParModule

//______________________________________________________________________________
void CbmStsModule::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsModule.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsModule::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsModule::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsModule(void *p) {
      return  p ? new(p) ::CbmStsModule : new ::CbmStsModule;
   }
   static void *newArray_CbmStsModule(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsModule[nElements] : new ::CbmStsModule[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsModule(void *p) {
      delete ((::CbmStsModule*)p);
   }
   static void deleteArray_CbmStsModule(void *p) {
      delete [] ((::CbmStsModule*)p);
   }
   static void destruct_CbmStsModule(void *p) {
      typedef ::CbmStsModule current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsModule

//______________________________________________________________________________
void CbmStsParSensor::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsParSensor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsParSensor::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsParSensor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsParSensor(void *p) {
      return  p ? new(p) ::CbmStsParSensor : new ::CbmStsParSensor;
   }
   static void *newArray_CbmStsParSensor(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsParSensor[nElements] : new ::CbmStsParSensor[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsParSensor(void *p) {
      delete ((::CbmStsParSensor*)p);
   }
   static void deleteArray_CbmStsParSensor(void *p) {
      delete [] ((::CbmStsParSensor*)p);
   }
   static void destruct_CbmStsParSensor(void *p) {
      typedef ::CbmStsParSensor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsParSensor

//______________________________________________________________________________
void CbmStsParSensorCond::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsParSensorCond.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsParSensorCond::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsParSensorCond::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsParSensorCond(void *p) {
      return  p ? new(p) ::CbmStsParSensorCond : new ::CbmStsParSensorCond;
   }
   static void *newArray_CbmStsParSensorCond(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsParSensorCond[nElements] : new ::CbmStsParSensorCond[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsParSensorCond(void *p) {
      delete ((::CbmStsParSensorCond*)p);
   }
   static void deleteArray_CbmStsParSensorCond(void *p) {
      delete [] ((::CbmStsParSensorCond*)p);
   }
   static void destruct_CbmStsParSensorCond(void *p) {
      typedef ::CbmStsParSensorCond current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsParSensorCond

//______________________________________________________________________________
void CbmStsParSetModule::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsParSetModule.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsParSetModule::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsParSetModule::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsParSetModule(void *p) {
      return  p ? new(p) ::CbmStsParSetModule : new ::CbmStsParSetModule;
   }
   static void *newArray_CbmStsParSetModule(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsParSetModule[nElements] : new ::CbmStsParSetModule[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsParSetModule(void *p) {
      delete ((::CbmStsParSetModule*)p);
   }
   static void deleteArray_CbmStsParSetModule(void *p) {
      delete [] ((::CbmStsParSetModule*)p);
   }
   static void destruct_CbmStsParSetModule(void *p) {
      typedef ::CbmStsParSetModule current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsParSetModule

//______________________________________________________________________________
void CbmStsParSetSensor::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsParSetSensor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsParSetSensor::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsParSetSensor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsParSetSensor(void *p) {
      return  p ? new(p) ::CbmStsParSetSensor : new ::CbmStsParSetSensor;
   }
   static void *newArray_CbmStsParSetSensor(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsParSetSensor[nElements] : new ::CbmStsParSetSensor[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsParSetSensor(void *p) {
      delete ((::CbmStsParSetSensor*)p);
   }
   static void deleteArray_CbmStsParSetSensor(void *p) {
      delete [] ((::CbmStsParSetSensor*)p);
   }
   static void destruct_CbmStsParSetSensor(void *p) {
      typedef ::CbmStsParSetSensor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsParSetSensor

//______________________________________________________________________________
void CbmStsParSetSensorCond::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsParSetSensorCond.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsParSetSensorCond::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsParSetSensorCond::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsParSetSensorCond(void *p) {
      return  p ? new(p) ::CbmStsParSetSensorCond : new ::CbmStsParSetSensorCond;
   }
   static void *newArray_CbmStsParSetSensorCond(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsParSetSensorCond[nElements] : new ::CbmStsParSetSensorCond[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsParSetSensorCond(void *p) {
      delete ((::CbmStsParSetSensorCond*)p);
   }
   static void deleteArray_CbmStsParSetSensorCond(void *p) {
      delete [] ((::CbmStsParSetSensorCond*)p);
   }
   static void destruct_CbmStsParSetSensorCond(void *p) {
      typedef ::CbmStsParSetSensorCond current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsParSetSensorCond

//______________________________________________________________________________
void CbmStsParSim::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsParSim.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsParSim::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsParSim::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsParSim(void *p) {
      return  p ? new(p) ::CbmStsParSim : new ::CbmStsParSim;
   }
   static void *newArray_CbmStsParSim(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsParSim[nElements] : new ::CbmStsParSim[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsParSim(void *p) {
      delete ((::CbmStsParSim*)p);
   }
   static void deleteArray_CbmStsParSim(void *p) {
      delete [] ((::CbmStsParSim*)p);
   }
   static void destruct_CbmStsParSim(void *p) {
      typedef ::CbmStsParSim current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsParSim

//______________________________________________________________________________
void CbmMcbm2018StsPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018StsPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2018StsPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2018StsPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018StsPar(void *p) {
      return  p ? new(p) ::CbmMcbm2018StsPar : new ::CbmMcbm2018StsPar;
   }
   static void *newArray_CbmMcbm2018StsPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018StsPar[nElements] : new ::CbmMcbm2018StsPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018StsPar(void *p) {
      delete ((::CbmMcbm2018StsPar*)p);
   }
   static void deleteArray_CbmMcbm2018StsPar(void *p) {
      delete [] ((::CbmMcbm2018StsPar*)p);
   }
   static void destruct_CbmMcbm2018StsPar(void *p) {
      typedef ::CbmMcbm2018StsPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2018StsPar

//______________________________________________________________________________
void CbmStsPhysics::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsPhysics.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsPhysics::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsPhysics::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmStsPhysics(void *p) {
      delete ((::CbmStsPhysics*)p);
   }
   static void deleteArray_CbmStsPhysics(void *p) {
      delete [] ((::CbmStsPhysics*)p);
   }
   static void destruct_CbmStsPhysics(void *p) {
      typedef ::CbmStsPhysics current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsPhysics

//______________________________________________________________________________
void CbmStsSensor::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsSensor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsSensor::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsSensor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsSensor(void *p) {
      return  p ? new(p) ::CbmStsSensor : new ::CbmStsSensor;
   }
   static void *newArray_CbmStsSensor(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsSensor[nElements] : new ::CbmStsSensor[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsSensor(void *p) {
      delete ((::CbmStsSensor*)p);
   }
   static void deleteArray_CbmStsSensor(void *p) {
      delete [] ((::CbmStsSensor*)p);
   }
   static void destruct_CbmStsSensor(void *p) {
      typedef ::CbmStsSensor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsSensor

//______________________________________________________________________________
void CbmStsSetup::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsSetup.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsSetup::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsSetup::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmStsSetup(void *p) {
      delete ((::CbmStsSetup*)p);
   }
   static void deleteArray_CbmStsSetup(void *p) {
      delete [] ((::CbmStsSetup*)p);
   }
   static void destruct_CbmStsSetup(void *p) {
      typedef ::CbmStsSetup current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsSetup

//______________________________________________________________________________
void CbmStsStation::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsStation.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsStation::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsStation::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsStation(void *p) {
      return  p ? new(p) ::CbmStsStation : new ::CbmStsStation;
   }
   static void *newArray_CbmStsStation(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsStation[nElements] : new ::CbmStsStation[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsStation(void *p) {
      delete ((::CbmStsStation*)p);
   }
   static void deleteArray_CbmStsStation(void *p) {
      delete [] ((::CbmStsStation*)p);
   }
   static void destruct_CbmStsStation(void *p) {
      typedef ::CbmStsStation current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsStation

//______________________________________________________________________________
void CbmStsTrackingInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsTrackingInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsTrackingInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsTrackingInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsTrackingInterface(void *p) {
      return  p ? new(p) ::CbmStsTrackingInterface : new ::CbmStsTrackingInterface;
   }
   static void *newArray_CbmStsTrackingInterface(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsTrackingInterface[nElements] : new ::CbmStsTrackingInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsTrackingInterface(void *p) {
      delete ((::CbmStsTrackingInterface*)p);
   }
   static void deleteArray_CbmStsTrackingInterface(void *p) {
      delete [] ((::CbmStsTrackingInterface*)p);
   }
   static void destruct_CbmStsTrackingInterface(void *p) {
      typedef ::CbmStsTrackingInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsTrackingInterface

namespace ROOT {
   static TClass *vectorlECbmStsSensormUgR_Dictionary();
   static void vectorlECbmStsSensormUgR_TClassManip(TClass*);
   static void *new_vectorlECbmStsSensormUgR(void *p = nullptr);
   static void *newArray_vectorlECbmStsSensormUgR(Long_t size, void *p);
   static void delete_vectorlECbmStsSensormUgR(void *p);
   static void deleteArray_vectorlECbmStsSensormUgR(void *p);
   static void destruct_vectorlECbmStsSensormUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsSensor*>*)
   {
      vector<CbmStsSensor*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsSensor*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsSensor*>", -2, "vector", 389,
                  typeid(vector<CbmStsSensor*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsSensormUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmStsSensor*>) );
      instance.SetNew(&new_vectorlECbmStsSensormUgR);
      instance.SetNewArray(&newArray_vectorlECbmStsSensormUgR);
      instance.SetDelete(&delete_vectorlECbmStsSensormUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsSensormUgR);
      instance.SetDestructor(&destruct_vectorlECbmStsSensormUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsSensor*> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsSensor*>","std::vector<CbmStsSensor*, std::allocator<CbmStsSensor*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsSensor*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsSensormUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsSensor*>*)nullptr)->GetClass();
      vectorlECbmStsSensormUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsSensormUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsSensormUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsSensor*> : new vector<CbmStsSensor*>;
   }
   static void *newArray_vectorlECbmStsSensormUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsSensor*>[nElements] : new vector<CbmStsSensor*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsSensormUgR(void *p) {
      delete ((vector<CbmStsSensor*>*)p);
   }
   static void deleteArray_vectorlECbmStsSensormUgR(void *p) {
      delete [] ((vector<CbmStsSensor*>*)p);
   }
   static void destruct_vectorlECbmStsSensormUgR(void *p) {
      typedef vector<CbmStsSensor*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsSensor*>

namespace ROOT {
   static TClass *vectorlECbmStsParAsicgR_Dictionary();
   static void vectorlECbmStsParAsicgR_TClassManip(TClass*);
   static void *new_vectorlECbmStsParAsicgR(void *p = nullptr);
   static void *newArray_vectorlECbmStsParAsicgR(Long_t size, void *p);
   static void delete_vectorlECbmStsParAsicgR(void *p);
   static void deleteArray_vectorlECbmStsParAsicgR(void *p);
   static void destruct_vectorlECbmStsParAsicgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsParAsic>*)
   {
      vector<CbmStsParAsic> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsParAsic>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsParAsic>", -2, "vector", 389,
                  typeid(vector<CbmStsParAsic>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsParAsicgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmStsParAsic>) );
      instance.SetNew(&new_vectorlECbmStsParAsicgR);
      instance.SetNewArray(&newArray_vectorlECbmStsParAsicgR);
      instance.SetDelete(&delete_vectorlECbmStsParAsicgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsParAsicgR);
      instance.SetDestructor(&destruct_vectorlECbmStsParAsicgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsParAsic> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsParAsic>","std::vector<CbmStsParAsic, std::allocator<CbmStsParAsic> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsParAsic>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsParAsicgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsParAsic>*)nullptr)->GetClass();
      vectorlECbmStsParAsicgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsParAsicgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsParAsicgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsParAsic> : new vector<CbmStsParAsic>;
   }
   static void *newArray_vectorlECbmStsParAsicgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsParAsic>[nElements] : new vector<CbmStsParAsic>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsParAsicgR(void *p) {
      delete ((vector<CbmStsParAsic>*)p);
   }
   static void deleteArray_vectorlECbmStsParAsicgR(void *p) {
      delete [] ((vector<CbmStsParAsic>*)p);
   }
   static void destruct_vectorlECbmStsParAsicgR(void *p) {
      typedef vector<CbmStsParAsic> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsParAsic>

namespace ROOT {
   static TClass *vectorlECbmStsModulemUgR_Dictionary();
   static void vectorlECbmStsModulemUgR_TClassManip(TClass*);
   static void *new_vectorlECbmStsModulemUgR(void *p = nullptr);
   static void *newArray_vectorlECbmStsModulemUgR(Long_t size, void *p);
   static void delete_vectorlECbmStsModulemUgR(void *p);
   static void deleteArray_vectorlECbmStsModulemUgR(void *p);
   static void destruct_vectorlECbmStsModulemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsModule*>*)
   {
      vector<CbmStsModule*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsModule*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsModule*>", -2, "vector", 389,
                  typeid(vector<CbmStsModule*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsModulemUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmStsModule*>) );
      instance.SetNew(&new_vectorlECbmStsModulemUgR);
      instance.SetNewArray(&newArray_vectorlECbmStsModulemUgR);
      instance.SetDelete(&delete_vectorlECbmStsModulemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsModulemUgR);
      instance.SetDestructor(&destruct_vectorlECbmStsModulemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsModule*> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsModule*>","std::vector<CbmStsModule*, std::allocator<CbmStsModule*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsModule*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsModulemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsModule*>*)nullptr)->GetClass();
      vectorlECbmStsModulemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsModulemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsModulemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsModule*> : new vector<CbmStsModule*>;
   }
   static void *newArray_vectorlECbmStsModulemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsModule*>[nElements] : new vector<CbmStsModule*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsModulemUgR(void *p) {
      delete ((vector<CbmStsModule*>*)p);
   }
   static void deleteArray_vectorlECbmStsModulemUgR(void *p) {
      delete [] ((vector<CbmStsModule*>*)p);
   }
   static void destruct_vectorlECbmStsModulemUgR(void *p) {
      typedef vector<CbmStsModule*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsModule*>

namespace ROOT {
   static TClass *vectorlECbmStsElementmUgR_Dictionary();
   static void vectorlECbmStsElementmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmStsElementmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmStsElementmUgR(Long_t size, void *p);
   static void delete_vectorlECbmStsElementmUgR(void *p);
   static void deleteArray_vectorlECbmStsElementmUgR(void *p);
   static void destruct_vectorlECbmStsElementmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsElement*>*)
   {
      vector<CbmStsElement*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsElement*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsElement*>", -2, "vector", 389,
                  typeid(vector<CbmStsElement*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsElementmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmStsElement*>) );
      instance.SetNew(&new_vectorlECbmStsElementmUgR);
      instance.SetNewArray(&newArray_vectorlECbmStsElementmUgR);
      instance.SetDelete(&delete_vectorlECbmStsElementmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsElementmUgR);
      instance.SetDestructor(&destruct_vectorlECbmStsElementmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsElement*> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsElement*>","std::vector<CbmStsElement*, std::allocator<CbmStsElement*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsElement*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsElementmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsElement*>*)nullptr)->GetClass();
      vectorlECbmStsElementmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsElementmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsElementmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsElement*> : new vector<CbmStsElement*>;
   }
   static void *newArray_vectorlECbmStsElementmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsElement*>[nElements] : new vector<CbmStsElement*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsElementmUgR(void *p) {
      delete ((vector<CbmStsElement*>*)p);
   }
   static void deleteArray_vectorlECbmStsElementmUgR(void *p) {
      delete [] ((vector<CbmStsElement*>*)p);
   }
   static void destruct_vectorlECbmStsElementmUgR(void *p) {
      typedef vector<CbmStsElement*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsElement*>

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
   static TClass *maplEunsignedsPintcOCbmStsParSensorCondgR_Dictionary();
   static void maplEunsignedsPintcOCbmStsParSensorCondgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOCbmStsParSensorCondgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOCbmStsParSensorCondgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOCbmStsParSensorCondgR(void *p);
   static void deleteArray_maplEunsignedsPintcOCbmStsParSensorCondgR(void *p);
   static void destruct_maplEunsignedsPintcOCbmStsParSensorCondgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,CbmStsParSensorCond>*)
   {
      map<unsigned int,CbmStsParSensorCond> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,CbmStsParSensorCond>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,CbmStsParSensorCond>", -2, "map", 100,
                  typeid(map<unsigned int,CbmStsParSensorCond>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOCbmStsParSensorCondgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,CbmStsParSensorCond>) );
      instance.SetNew(&new_maplEunsignedsPintcOCbmStsParSensorCondgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOCbmStsParSensorCondgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOCbmStsParSensorCondgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOCbmStsParSensorCondgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOCbmStsParSensorCondgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,CbmStsParSensorCond> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,CbmStsParSensorCond>","std::map<unsigned int, CbmStsParSensorCond, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, CbmStsParSensorCond> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,CbmStsParSensorCond>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOCbmStsParSensorCondgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,CbmStsParSensorCond>*)nullptr)->GetClass();
      maplEunsignedsPintcOCbmStsParSensorCondgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOCbmStsParSensorCondgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOCbmStsParSensorCondgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,CbmStsParSensorCond> : new map<unsigned int,CbmStsParSensorCond>;
   }
   static void *newArray_maplEunsignedsPintcOCbmStsParSensorCondgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,CbmStsParSensorCond>[nElements] : new map<unsigned int,CbmStsParSensorCond>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOCbmStsParSensorCondgR(void *p) {
      delete ((map<unsigned int,CbmStsParSensorCond>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOCbmStsParSensorCondgR(void *p) {
      delete [] ((map<unsigned int,CbmStsParSensorCond>*)p);
   }
   static void destruct_maplEunsignedsPintcOCbmStsParSensorCondgR(void *p) {
      typedef map<unsigned int,CbmStsParSensorCond> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,CbmStsParSensorCond>

namespace ROOT {
   static TClass *maplEunsignedsPintcOCbmStsParSensorgR_Dictionary();
   static void maplEunsignedsPintcOCbmStsParSensorgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOCbmStsParSensorgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOCbmStsParSensorgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOCbmStsParSensorgR(void *p);
   static void deleteArray_maplEunsignedsPintcOCbmStsParSensorgR(void *p);
   static void destruct_maplEunsignedsPintcOCbmStsParSensorgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,CbmStsParSensor>*)
   {
      map<unsigned int,CbmStsParSensor> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,CbmStsParSensor>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,CbmStsParSensor>", -2, "map", 100,
                  typeid(map<unsigned int,CbmStsParSensor>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOCbmStsParSensorgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,CbmStsParSensor>) );
      instance.SetNew(&new_maplEunsignedsPintcOCbmStsParSensorgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOCbmStsParSensorgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOCbmStsParSensorgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOCbmStsParSensorgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOCbmStsParSensorgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,CbmStsParSensor> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,CbmStsParSensor>","std::map<unsigned int, CbmStsParSensor, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, CbmStsParSensor> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,CbmStsParSensor>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOCbmStsParSensorgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,CbmStsParSensor>*)nullptr)->GetClass();
      maplEunsignedsPintcOCbmStsParSensorgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOCbmStsParSensorgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOCbmStsParSensorgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,CbmStsParSensor> : new map<unsigned int,CbmStsParSensor>;
   }
   static void *newArray_maplEunsignedsPintcOCbmStsParSensorgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,CbmStsParSensor>[nElements] : new map<unsigned int,CbmStsParSensor>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOCbmStsParSensorgR(void *p) {
      delete ((map<unsigned int,CbmStsParSensor>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOCbmStsParSensorgR(void *p) {
      delete [] ((map<unsigned int,CbmStsParSensor>*)p);
   }
   static void destruct_maplEunsignedsPintcOCbmStsParSensorgR(void *p) {
      typedef map<unsigned int,CbmStsParSensor> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,CbmStsParSensor>

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

namespace ROOT {
   static TClass *maplEintcOCbmStsSensormUgR_Dictionary();
   static void maplEintcOCbmStsSensormUgR_TClassManip(TClass*);
   static void *new_maplEintcOCbmStsSensormUgR(void *p = nullptr);
   static void *newArray_maplEintcOCbmStsSensormUgR(Long_t size, void *p);
   static void delete_maplEintcOCbmStsSensormUgR(void *p);
   static void deleteArray_maplEintcOCbmStsSensormUgR(void *p);
   static void destruct_maplEintcOCbmStsSensormUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,CbmStsSensor*>*)
   {
      map<int,CbmStsSensor*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,CbmStsSensor*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,CbmStsSensor*>", -2, "map", 100,
                  typeid(map<int,CbmStsSensor*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOCbmStsSensormUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,CbmStsSensor*>) );
      instance.SetNew(&new_maplEintcOCbmStsSensormUgR);
      instance.SetNewArray(&newArray_maplEintcOCbmStsSensormUgR);
      instance.SetDelete(&delete_maplEintcOCbmStsSensormUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOCbmStsSensormUgR);
      instance.SetDestructor(&destruct_maplEintcOCbmStsSensormUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,CbmStsSensor*> >()));

      ::ROOT::AddClassAlternate("map<int,CbmStsSensor*>","std::map<int, CbmStsSensor*, std::less<int>, std::allocator<std::pair<int const, CbmStsSensor*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,CbmStsSensor*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOCbmStsSensormUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,CbmStsSensor*>*)nullptr)->GetClass();
      maplEintcOCbmStsSensormUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOCbmStsSensormUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOCbmStsSensormUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmStsSensor*> : new map<int,CbmStsSensor*>;
   }
   static void *newArray_maplEintcOCbmStsSensormUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmStsSensor*>[nElements] : new map<int,CbmStsSensor*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOCbmStsSensormUgR(void *p) {
      delete ((map<int,CbmStsSensor*>*)p);
   }
   static void deleteArray_maplEintcOCbmStsSensormUgR(void *p) {
      delete [] ((map<int,CbmStsSensor*>*)p);
   }
   static void destruct_maplEintcOCbmStsSensormUgR(void *p) {
      typedef map<int,CbmStsSensor*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,CbmStsSensor*>

namespace ROOT {
   static TClass *maplEintcOCbmStsModulemUgR_Dictionary();
   static void maplEintcOCbmStsModulemUgR_TClassManip(TClass*);
   static void *new_maplEintcOCbmStsModulemUgR(void *p = nullptr);
   static void *newArray_maplEintcOCbmStsModulemUgR(Long_t size, void *p);
   static void delete_maplEintcOCbmStsModulemUgR(void *p);
   static void deleteArray_maplEintcOCbmStsModulemUgR(void *p);
   static void destruct_maplEintcOCbmStsModulemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,CbmStsModule*>*)
   {
      map<int,CbmStsModule*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,CbmStsModule*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,CbmStsModule*>", -2, "map", 100,
                  typeid(map<int,CbmStsModule*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOCbmStsModulemUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,CbmStsModule*>) );
      instance.SetNew(&new_maplEintcOCbmStsModulemUgR);
      instance.SetNewArray(&newArray_maplEintcOCbmStsModulemUgR);
      instance.SetDelete(&delete_maplEintcOCbmStsModulemUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOCbmStsModulemUgR);
      instance.SetDestructor(&destruct_maplEintcOCbmStsModulemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,CbmStsModule*> >()));

      ::ROOT::AddClassAlternate("map<int,CbmStsModule*>","std::map<int, CbmStsModule*, std::less<int>, std::allocator<std::pair<int const, CbmStsModule*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,CbmStsModule*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOCbmStsModulemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,CbmStsModule*>*)nullptr)->GetClass();
      maplEintcOCbmStsModulemUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOCbmStsModulemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOCbmStsModulemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmStsModule*> : new map<int,CbmStsModule*>;
   }
   static void *newArray_maplEintcOCbmStsModulemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmStsModule*>[nElements] : new map<int,CbmStsModule*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOCbmStsModulemUgR(void *p) {
      delete ((map<int,CbmStsModule*>*)p);
   }
   static void deleteArray_maplEintcOCbmStsModulemUgR(void *p) {
      delete [] ((map<int,CbmStsModule*>*)p);
   }
   static void destruct_maplEintcOCbmStsModulemUgR(void *p) {
      typedef map<int,CbmStsModule*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,CbmStsModule*>

namespace ROOT {
   static TClass *maplEdoublecOdoublegR_Dictionary();
   static void maplEdoublecOdoublegR_TClassManip(TClass*);
   static void *new_maplEdoublecOdoublegR(void *p = nullptr);
   static void *newArray_maplEdoublecOdoublegR(Long_t size, void *p);
   static void delete_maplEdoublecOdoublegR(void *p);
   static void deleteArray_maplEdoublecOdoublegR(void *p);
   static void destruct_maplEdoublecOdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<double,double>*)
   {
      map<double,double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<double,double>));
      static ::ROOT::TGenericClassInfo 
         instance("map<double,double>", -2, "map", 100,
                  typeid(map<double,double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEdoublecOdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(map<double,double>) );
      instance.SetNew(&new_maplEdoublecOdoublegR);
      instance.SetNewArray(&newArray_maplEdoublecOdoublegR);
      instance.SetDelete(&delete_maplEdoublecOdoublegR);
      instance.SetDeleteArray(&deleteArray_maplEdoublecOdoublegR);
      instance.SetDestructor(&destruct_maplEdoublecOdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<double,double> >()));

      ::ROOT::AddClassAlternate("map<double,double>","std::map<double, double, std::less<double>, std::allocator<std::pair<double const, double> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<double,double>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEdoublecOdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<double,double>*)nullptr)->GetClass();
      maplEdoublecOdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void maplEdoublecOdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEdoublecOdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<double,double> : new map<double,double>;
   }
   static void *newArray_maplEdoublecOdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<double,double>[nElements] : new map<double,double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEdoublecOdoublegR(void *p) {
      delete ((map<double,double>*)p);
   }
   static void deleteArray_maplEdoublecOdoublegR(void *p) {
      delete [] ((map<double,double>*)p);
   }
   static void destruct_maplEdoublecOdoublegR(void *p) {
      typedef map<double,double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<double,double>

namespace {
  void TriggerDictionaryInitialization_libCbmStsBase_Impl() {
    static const char* headers[] = {
"CbmStsContFact.h",
"CbmStsElement.h",
"CbmStsModule.h",
"CbmStsParAsic.h",
"CbmStsParModule.h",
"CbmStsParSensor.h",
"CbmStsParSensorCond.h",
"CbmStsParSetModule.h",
"CbmStsParSetSensor.h",
"CbmStsParSetSensorCond.h",
"CbmStsParSim.h",
"CbmMcbm2018StsPar.h",
"CbmStsPhysics.h",
"CbmStsSensor.h",
"CbmStsSetup.h",
"CbmStsStation.h",
"CbmStsTrackingInterface.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
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
"/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc",
"/home/heinemann/gpuunpacker/cbmroot/external/xpu/src",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/core/detectors/sts/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmStsBase dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmStsContFact.h")))  CbmStsContFact;
class __attribute__((annotate("$clingAutoload$CbmStsElement.h")))  CbmStsElement;
class __attribute__((annotate("$clingAutoload$CbmStsParAsic.h")))  __attribute__((annotate("$clingAutoload$CbmStsModule.h")))  CbmStsParAsic;
class __attribute__((annotate("$clingAutoload$CbmStsParModule.h")))  __attribute__((annotate("$clingAutoload$CbmStsModule.h")))  CbmStsParModule;
class __attribute__((annotate("$clingAutoload$CbmStsModule.h")))  CbmStsModule;
class __attribute__((annotate("$clingAutoload$CbmStsParSensor.h")))  CbmStsParSensor;
class __attribute__((annotate("$clingAutoload$CbmStsParSensorCond.h")))  CbmStsParSensorCond;
class __attribute__((annotate("$clingAutoload$CbmStsParSetModule.h")))  CbmStsParSetModule;
class __attribute__((annotate("$clingAutoload$CbmStsParSetSensor.h")))  CbmStsParSetSensor;
class __attribute__((annotate("$clingAutoload$CbmStsParSetSensorCond.h")))  CbmStsParSetSensorCond;
class __attribute__((annotate("$clingAutoload$CbmStsParSim.h")))  CbmStsParSim;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018StsPar.h")))  CbmMcbm2018StsPar;
class __attribute__((annotate("$clingAutoload$CbmStsPhysics.h")))  CbmStsPhysics;
class __attribute__((annotate("$clingAutoload$CbmStsSensor.h")))  CbmStsSensor;
class __attribute__((annotate("$clingAutoload$CbmStsSetup.h")))  CbmStsSetup;
class __attribute__((annotate("$clingAutoload$CbmStsStation.h")))  CbmStsStation;
class __attribute__((annotate("$clingAutoload$CbmStsTrackingInterface.h")))  CbmStsTrackingInterface;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmStsBase dictionary payload"

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
#include "CbmStsContFact.h"
#include "CbmStsElement.h"
#include "CbmStsModule.h"
#include "CbmStsParAsic.h"
#include "CbmStsParModule.h"
#include "CbmStsParSensor.h"
#include "CbmStsParSensorCond.h"
#include "CbmStsParSetModule.h"
#include "CbmStsParSetSensor.h"
#include "CbmStsParSetSensorCond.h"
#include "CbmStsParSim.h"
#include "CbmMcbm2018StsPar.h"
#include "CbmStsPhysics.h"
#include "CbmStsSensor.h"
#include "CbmStsSetup.h"
#include "CbmStsStation.h"
#include "CbmStsTrackingInterface.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmMcbm2018StsPar", payloadCode, "@",
"CbmStsContFact", payloadCode, "@",
"CbmStsELoss", payloadCode, "@",
"CbmStsElement", payloadCode, "@",
"CbmStsModule", payloadCode, "@",
"CbmStsParAsic", payloadCode, "@",
"CbmStsParModule", payloadCode, "@",
"CbmStsParSensor", payloadCode, "@",
"CbmStsParSensorCond", payloadCode, "@",
"CbmStsParSetModule", payloadCode, "@",
"CbmStsParSetSensor", payloadCode, "@",
"CbmStsParSetSensorCond", payloadCode, "@",
"CbmStsParSim", payloadCode, "@",
"CbmStsPhysics", payloadCode, "@",
"CbmStsSensor", payloadCode, "@",
"CbmStsSensorClass", payloadCode, "@",
"CbmStsSetup", payloadCode, "@",
"CbmStsStation", payloadCode, "@",
"CbmStsTrackingInterface", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmStsBase",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmStsBase_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmStsBase_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmStsBase() {
  TriggerDictionaryInitialization_libCbmStsBase_Impl();
}
