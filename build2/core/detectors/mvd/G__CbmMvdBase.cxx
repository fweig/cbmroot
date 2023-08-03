// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmMvdBase
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
#include "CbmMvdSensorPlugin.h"
#include "CbmMvdSensorTask.h"
#include "CbmMvdSensorBuffer.h"
#include "CbmMvdSensorFrameBuffer.h"
#include "CbmMvdMimosa26AHR.h"
#include "CbmMvdMimosa34.h"
#include "CbmMvdMimosis.h"
#include "CbmMvdSensorDataSheet.h"
#include "CbmMvdGeoHandler.h"
#include "CbmMvdHelper.h"
#include "CbmMvdStationPar.h"
#include "CbmMvdDetector.h"
#include "CbmMvdSensor.h"
#include "CbmMvdPileupManager.h"
#include "CbmMvdTrackingInterface.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_CbmMvdSensorPlugin(void *p);
   static void deleteArray_CbmMvdSensorPlugin(void *p);
   static void destruct_CbmMvdSensorPlugin(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdSensorPlugin*)
   {
      ::CbmMvdSensorPlugin *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdSensorPlugin >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdSensorPlugin", ::CbmMvdSensorPlugin::Class_Version(), "CbmMvdSensorPlugin.h", 37,
                  typeid(::CbmMvdSensorPlugin), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdSensorPlugin::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdSensorPlugin) );
      instance.SetDelete(&delete_CbmMvdSensorPlugin);
      instance.SetDeleteArray(&deleteArray_CbmMvdSensorPlugin);
      instance.SetDestructor(&destruct_CbmMvdSensorPlugin);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdSensorPlugin*)
   {
      return GenerateInitInstanceLocal((::CbmMvdSensorPlugin*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdSensorPlugin*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdSensorTask(void *p = nullptr);
   static void *newArray_CbmMvdSensorTask(Long_t size, void *p);
   static void delete_CbmMvdSensorTask(void *p);
   static void deleteArray_CbmMvdSensorTask(void *p);
   static void destruct_CbmMvdSensorTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdSensorTask*)
   {
      ::CbmMvdSensorTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdSensorTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdSensorTask", ::CbmMvdSensorTask::Class_Version(), "CbmMvdSensorTask.h", 31,
                  typeid(::CbmMvdSensorTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdSensorTask::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdSensorTask) );
      instance.SetNew(&new_CbmMvdSensorTask);
      instance.SetNewArray(&newArray_CbmMvdSensorTask);
      instance.SetDelete(&delete_CbmMvdSensorTask);
      instance.SetDeleteArray(&deleteArray_CbmMvdSensorTask);
      instance.SetDestructor(&destruct_CbmMvdSensorTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdSensorTask*)
   {
      return GenerateInitInstanceLocal((::CbmMvdSensorTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdSensorTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdSensorBuffer(void *p = nullptr);
   static void *newArray_CbmMvdSensorBuffer(Long_t size, void *p);
   static void delete_CbmMvdSensorBuffer(void *p);
   static void deleteArray_CbmMvdSensorBuffer(void *p);
   static void destruct_CbmMvdSensorBuffer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdSensorBuffer*)
   {
      ::CbmMvdSensorBuffer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdSensorBuffer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdSensorBuffer", ::CbmMvdSensorBuffer::Class_Version(), "CbmMvdSensorBuffer.h", 34,
                  typeid(::CbmMvdSensorBuffer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdSensorBuffer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdSensorBuffer) );
      instance.SetNew(&new_CbmMvdSensorBuffer);
      instance.SetNewArray(&newArray_CbmMvdSensorBuffer);
      instance.SetDelete(&delete_CbmMvdSensorBuffer);
      instance.SetDeleteArray(&deleteArray_CbmMvdSensorBuffer);
      instance.SetDestructor(&destruct_CbmMvdSensorBuffer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdSensorBuffer*)
   {
      return GenerateInitInstanceLocal((::CbmMvdSensorBuffer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdSensorBuffer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdSensorFrameBuffer(void *p = nullptr);
   static void *newArray_CbmMvdSensorFrameBuffer(Long_t size, void *p);
   static void delete_CbmMvdSensorFrameBuffer(void *p);
   static void deleteArray_CbmMvdSensorFrameBuffer(void *p);
   static void destruct_CbmMvdSensorFrameBuffer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdSensorFrameBuffer*)
   {
      ::CbmMvdSensorFrameBuffer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdSensorFrameBuffer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdSensorFrameBuffer", ::CbmMvdSensorFrameBuffer::Class_Version(), "CbmMvdSensorFrameBuffer.h", 38,
                  typeid(::CbmMvdSensorFrameBuffer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdSensorFrameBuffer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdSensorFrameBuffer) );
      instance.SetNew(&new_CbmMvdSensorFrameBuffer);
      instance.SetNewArray(&newArray_CbmMvdSensorFrameBuffer);
      instance.SetDelete(&delete_CbmMvdSensorFrameBuffer);
      instance.SetDeleteArray(&deleteArray_CbmMvdSensorFrameBuffer);
      instance.SetDestructor(&destruct_CbmMvdSensorFrameBuffer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdSensorFrameBuffer*)
   {
      return GenerateInitInstanceLocal((::CbmMvdSensorFrameBuffer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdSensorFrameBuffer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdSensorDataSheet(void *p = nullptr);
   static void *newArray_CbmMvdSensorDataSheet(Long_t size, void *p);
   static void delete_CbmMvdSensorDataSheet(void *p);
   static void deleteArray_CbmMvdSensorDataSheet(void *p);
   static void destruct_CbmMvdSensorDataSheet(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdSensorDataSheet*)
   {
      ::CbmMvdSensorDataSheet *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdSensorDataSheet >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdSensorDataSheet", ::CbmMvdSensorDataSheet::Class_Version(), "CbmMvdSensorDataSheet.h", 33,
                  typeid(::CbmMvdSensorDataSheet), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdSensorDataSheet::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdSensorDataSheet) );
      instance.SetNew(&new_CbmMvdSensorDataSheet);
      instance.SetNewArray(&newArray_CbmMvdSensorDataSheet);
      instance.SetDelete(&delete_CbmMvdSensorDataSheet);
      instance.SetDeleteArray(&deleteArray_CbmMvdSensorDataSheet);
      instance.SetDestructor(&destruct_CbmMvdSensorDataSheet);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdSensorDataSheet*)
   {
      return GenerateInitInstanceLocal((::CbmMvdSensorDataSheet*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdSensorDataSheet*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdMimosa26AHR(void *p = nullptr);
   static void *newArray_CbmMvdMimosa26AHR(Long_t size, void *p);
   static void delete_CbmMvdMimosa26AHR(void *p);
   static void deleteArray_CbmMvdMimosa26AHR(void *p);
   static void destruct_CbmMvdMimosa26AHR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdMimosa26AHR*)
   {
      ::CbmMvdMimosa26AHR *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdMimosa26AHR >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdMimosa26AHR", ::CbmMvdMimosa26AHR::Class_Version(), "CbmMvdMimosa26AHR.h", 31,
                  typeid(::CbmMvdMimosa26AHR), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdMimosa26AHR::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdMimosa26AHR) );
      instance.SetNew(&new_CbmMvdMimosa26AHR);
      instance.SetNewArray(&newArray_CbmMvdMimosa26AHR);
      instance.SetDelete(&delete_CbmMvdMimosa26AHR);
      instance.SetDeleteArray(&deleteArray_CbmMvdMimosa26AHR);
      instance.SetDestructor(&destruct_CbmMvdMimosa26AHR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdMimosa26AHR*)
   {
      return GenerateInitInstanceLocal((::CbmMvdMimosa26AHR*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdMimosa26AHR*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdMimosa34(void *p = nullptr);
   static void *newArray_CbmMvdMimosa34(Long_t size, void *p);
   static void delete_CbmMvdMimosa34(void *p);
   static void deleteArray_CbmMvdMimosa34(void *p);
   static void destruct_CbmMvdMimosa34(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdMimosa34*)
   {
      ::CbmMvdMimosa34 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdMimosa34 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdMimosa34", ::CbmMvdMimosa34::Class_Version(), "CbmMvdMimosa34.h", 30,
                  typeid(::CbmMvdMimosa34), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdMimosa34::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdMimosa34) );
      instance.SetNew(&new_CbmMvdMimosa34);
      instance.SetNewArray(&newArray_CbmMvdMimosa34);
      instance.SetDelete(&delete_CbmMvdMimosa34);
      instance.SetDeleteArray(&deleteArray_CbmMvdMimosa34);
      instance.SetDestructor(&destruct_CbmMvdMimosa34);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdMimosa34*)
   {
      return GenerateInitInstanceLocal((::CbmMvdMimosa34*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdMimosa34*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdMimosis(void *p = nullptr);
   static void *newArray_CbmMvdMimosis(Long_t size, void *p);
   static void delete_CbmMvdMimosis(void *p);
   static void deleteArray_CbmMvdMimosis(void *p);
   static void destruct_CbmMvdMimosis(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdMimosis*)
   {
      ::CbmMvdMimosis *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdMimosis >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdMimosis", ::CbmMvdMimosis::Class_Version(), "CbmMvdMimosis.h", 29,
                  typeid(::CbmMvdMimosis), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdMimosis::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdMimosis) );
      instance.SetNew(&new_CbmMvdMimosis);
      instance.SetNewArray(&newArray_CbmMvdMimosis);
      instance.SetDelete(&delete_CbmMvdMimosis);
      instance.SetDeleteArray(&deleteArray_CbmMvdMimosis);
      instance.SetDestructor(&destruct_CbmMvdMimosis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdMimosis*)
   {
      return GenerateInitInstanceLocal((::CbmMvdMimosis*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdMimosis*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdGeoHandler(void *p = nullptr);
   static void *newArray_CbmMvdGeoHandler(Long_t size, void *p);
   static void delete_CbmMvdGeoHandler(void *p);
   static void deleteArray_CbmMvdGeoHandler(void *p);
   static void destruct_CbmMvdGeoHandler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdGeoHandler*)
   {
      ::CbmMvdGeoHandler *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdGeoHandler >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdGeoHandler", ::CbmMvdGeoHandler::Class_Version(), "CbmMvdGeoHandler.h", 47,
                  typeid(::CbmMvdGeoHandler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdGeoHandler::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdGeoHandler) );
      instance.SetNew(&new_CbmMvdGeoHandler);
      instance.SetNewArray(&newArray_CbmMvdGeoHandler);
      instance.SetDelete(&delete_CbmMvdGeoHandler);
      instance.SetDeleteArray(&deleteArray_CbmMvdGeoHandler);
      instance.SetDestructor(&destruct_CbmMvdGeoHandler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdGeoHandler*)
   {
      return GenerateInitInstanceLocal((::CbmMvdGeoHandler*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdGeoHandler*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdStationPar(void *p = nullptr);
   static void *newArray_CbmMvdStationPar(Long_t size, void *p);
   static void delete_CbmMvdStationPar(void *p);
   static void deleteArray_CbmMvdStationPar(void *p);
   static void destruct_CbmMvdStationPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdStationPar*)
   {
      ::CbmMvdStationPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdStationPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdStationPar", ::CbmMvdStationPar::Class_Version(), "CbmMvdStationPar.h", 32,
                  typeid(::CbmMvdStationPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdStationPar::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdStationPar) );
      instance.SetNew(&new_CbmMvdStationPar);
      instance.SetNewArray(&newArray_CbmMvdStationPar);
      instance.SetDelete(&delete_CbmMvdStationPar);
      instance.SetDeleteArray(&deleteArray_CbmMvdStationPar);
      instance.SetDestructor(&destruct_CbmMvdStationPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdStationPar*)
   {
      return GenerateInitInstanceLocal((::CbmMvdStationPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdStationPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmMvdDetector(void *p);
   static void deleteArray_CbmMvdDetector(void *p);
   static void destruct_CbmMvdDetector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdDetector*)
   {
      ::CbmMvdDetector *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdDetector >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdDetector", ::CbmMvdDetector::Class_Version(), "CbmMvdDetector.h", 43,
                  typeid(::CbmMvdDetector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdDetector::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdDetector) );
      instance.SetDelete(&delete_CbmMvdDetector);
      instance.SetDeleteArray(&deleteArray_CbmMvdDetector);
      instance.SetDestructor(&destruct_CbmMvdDetector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdDetector*)
   {
      return GenerateInitInstanceLocal((::CbmMvdDetector*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdDetector*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdSensor(void *p = nullptr);
   static void *newArray_CbmMvdSensor(Long_t size, void *p);
   static void delete_CbmMvdSensor(void *p);
   static void deleteArray_CbmMvdSensor(void *p);
   static void destruct_CbmMvdSensor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdSensor*)
   {
      ::CbmMvdSensor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdSensor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdSensor", ::CbmMvdSensor::Class_Version(), "CbmMvdSensor.h", 43,
                  typeid(::CbmMvdSensor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdSensor::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdSensor) );
      instance.SetNew(&new_CbmMvdSensor);
      instance.SetNewArray(&newArray_CbmMvdSensor);
      instance.SetDelete(&delete_CbmMvdSensor);
      instance.SetDeleteArray(&deleteArray_CbmMvdSensor);
      instance.SetDestructor(&destruct_CbmMvdSensor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdSensor*)
   {
      return GenerateInitInstanceLocal((::CbmMvdSensor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdSensor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdPileupManager(void *p = nullptr);
   static void *newArray_CbmMvdPileupManager(Long_t size, void *p);
   static void delete_CbmMvdPileupManager(void *p);
   static void deleteArray_CbmMvdPileupManager(void *p);
   static void destruct_CbmMvdPileupManager(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdPileupManager*)
   {
      ::CbmMvdPileupManager *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdPileupManager >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdPileupManager", ::CbmMvdPileupManager::Class_Version(), "CbmMvdPileupManager.h", 35,
                  typeid(::CbmMvdPileupManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdPileupManager::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdPileupManager) );
      instance.SetNew(&new_CbmMvdPileupManager);
      instance.SetNewArray(&newArray_CbmMvdPileupManager);
      instance.SetDelete(&delete_CbmMvdPileupManager);
      instance.SetDeleteArray(&deleteArray_CbmMvdPileupManager);
      instance.SetDestructor(&destruct_CbmMvdPileupManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdPileupManager*)
   {
      return GenerateInitInstanceLocal((::CbmMvdPileupManager*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdPileupManager*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdTrackingInterface(void *p = nullptr);
   static void *newArray_CbmMvdTrackingInterface(Long_t size, void *p);
   static void delete_CbmMvdTrackingInterface(void *p);
   static void deleteArray_CbmMvdTrackingInterface(void *p);
   static void destruct_CbmMvdTrackingInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdTrackingInterface*)
   {
      ::CbmMvdTrackingInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdTrackingInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdTrackingInterface", ::CbmMvdTrackingInterface::Class_Version(), "CbmMvdTrackingInterface.h", 36,
                  typeid(::CbmMvdTrackingInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdTrackingInterface::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdTrackingInterface) );
      instance.SetNew(&new_CbmMvdTrackingInterface);
      instance.SetNewArray(&newArray_CbmMvdTrackingInterface);
      instance.SetDelete(&delete_CbmMvdTrackingInterface);
      instance.SetDeleteArray(&deleteArray_CbmMvdTrackingInterface);
      instance.SetDestructor(&destruct_CbmMvdTrackingInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdTrackingInterface*)
   {
      return GenerateInitInstanceLocal((::CbmMvdTrackingInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdTrackingInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdSensorPlugin::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdSensorPlugin::Class_Name()
{
   return "CbmMvdSensorPlugin";
}

//______________________________________________________________________________
const char *CbmMvdSensorPlugin::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorPlugin*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdSensorPlugin::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorPlugin*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdSensorPlugin::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorPlugin*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdSensorPlugin::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorPlugin*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdSensorTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdSensorTask::Class_Name()
{
   return "CbmMvdSensorTask";
}

//______________________________________________________________________________
const char *CbmMvdSensorTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdSensorTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdSensorTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdSensorTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdSensorBuffer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdSensorBuffer::Class_Name()
{
   return "CbmMvdSensorBuffer";
}

//______________________________________________________________________________
const char *CbmMvdSensorBuffer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorBuffer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdSensorBuffer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorBuffer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdSensorBuffer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorBuffer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdSensorBuffer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorBuffer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdSensorFrameBuffer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdSensorFrameBuffer::Class_Name()
{
   return "CbmMvdSensorFrameBuffer";
}

//______________________________________________________________________________
const char *CbmMvdSensorFrameBuffer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorFrameBuffer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdSensorFrameBuffer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorFrameBuffer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdSensorFrameBuffer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorFrameBuffer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdSensorFrameBuffer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorFrameBuffer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdSensorDataSheet::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdSensorDataSheet::Class_Name()
{
   return "CbmMvdSensorDataSheet";
}

//______________________________________________________________________________
const char *CbmMvdSensorDataSheet::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorDataSheet*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdSensorDataSheet::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorDataSheet*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdSensorDataSheet::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorDataSheet*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdSensorDataSheet::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorDataSheet*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdMimosa26AHR::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdMimosa26AHR::Class_Name()
{
   return "CbmMvdMimosa26AHR";
}

//______________________________________________________________________________
const char *CbmMvdMimosa26AHR::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdMimosa26AHR*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdMimosa26AHR::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdMimosa26AHR*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdMimosa26AHR::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdMimosa26AHR*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdMimosa26AHR::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdMimosa26AHR*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdMimosa34::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdMimosa34::Class_Name()
{
   return "CbmMvdMimosa34";
}

//______________________________________________________________________________
const char *CbmMvdMimosa34::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdMimosa34*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdMimosa34::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdMimosa34*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdMimosa34::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdMimosa34*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdMimosa34::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdMimosa34*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdMimosis::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdMimosis::Class_Name()
{
   return "CbmMvdMimosis";
}

//______________________________________________________________________________
const char *CbmMvdMimosis::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdMimosis*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdMimosis::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdMimosis*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdMimosis::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdMimosis*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdMimosis::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdMimosis*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdGeoHandler::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdGeoHandler::Class_Name()
{
   return "CbmMvdGeoHandler";
}

//______________________________________________________________________________
const char *CbmMvdGeoHandler::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdGeoHandler*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdGeoHandler::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdGeoHandler*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdGeoHandler::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdGeoHandler*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdGeoHandler::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdGeoHandler*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdStationPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdStationPar::Class_Name()
{
   return "CbmMvdStationPar";
}

//______________________________________________________________________________
const char *CbmMvdStationPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdStationPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdStationPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdStationPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdStationPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdStationPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdStationPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdStationPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdDetector::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdDetector::Class_Name()
{
   return "CbmMvdDetector";
}

//______________________________________________________________________________
const char *CbmMvdDetector::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDetector*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdDetector::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDetector*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdDetector::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDetector*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdDetector::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDetector*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdSensor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdSensor::Class_Name()
{
   return "CbmMvdSensor";
}

//______________________________________________________________________________
const char *CbmMvdSensor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdSensor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdSensor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdSensor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdPileupManager::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdPileupManager::Class_Name()
{
   return "CbmMvdPileupManager";
}

//______________________________________________________________________________
const char *CbmMvdPileupManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdPileupManager*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdPileupManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdPileupManager*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdPileupManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdPileupManager*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdPileupManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdPileupManager*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdTrackingInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdTrackingInterface::Class_Name()
{
   return "CbmMvdTrackingInterface";
}

//______________________________________________________________________________
const char *CbmMvdTrackingInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdTrackingInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdTrackingInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdTrackingInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdTrackingInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdTrackingInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdTrackingInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdTrackingInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmMvdSensorPlugin::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdSensorPlugin.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdSensorPlugin::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdSensorPlugin::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmMvdSensorPlugin(void *p) {
      delete ((::CbmMvdSensorPlugin*)p);
   }
   static void deleteArray_CbmMvdSensorPlugin(void *p) {
      delete [] ((::CbmMvdSensorPlugin*)p);
   }
   static void destruct_CbmMvdSensorPlugin(void *p) {
      typedef ::CbmMvdSensorPlugin current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdSensorPlugin

//______________________________________________________________________________
void CbmMvdSensorTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdSensorTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdSensorTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdSensorTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdSensorTask(void *p) {
      return  p ? new(p) ::CbmMvdSensorTask : new ::CbmMvdSensorTask;
   }
   static void *newArray_CbmMvdSensorTask(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdSensorTask[nElements] : new ::CbmMvdSensorTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdSensorTask(void *p) {
      delete ((::CbmMvdSensorTask*)p);
   }
   static void deleteArray_CbmMvdSensorTask(void *p) {
      delete [] ((::CbmMvdSensorTask*)p);
   }
   static void destruct_CbmMvdSensorTask(void *p) {
      typedef ::CbmMvdSensorTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdSensorTask

//______________________________________________________________________________
void CbmMvdSensorBuffer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdSensorBuffer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdSensorBuffer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdSensorBuffer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdSensorBuffer(void *p) {
      return  p ? new(p) ::CbmMvdSensorBuffer : new ::CbmMvdSensorBuffer;
   }
   static void *newArray_CbmMvdSensorBuffer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdSensorBuffer[nElements] : new ::CbmMvdSensorBuffer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdSensorBuffer(void *p) {
      delete ((::CbmMvdSensorBuffer*)p);
   }
   static void deleteArray_CbmMvdSensorBuffer(void *p) {
      delete [] ((::CbmMvdSensorBuffer*)p);
   }
   static void destruct_CbmMvdSensorBuffer(void *p) {
      typedef ::CbmMvdSensorBuffer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdSensorBuffer

//______________________________________________________________________________
void CbmMvdSensorFrameBuffer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdSensorFrameBuffer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdSensorFrameBuffer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdSensorFrameBuffer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdSensorFrameBuffer(void *p) {
      return  p ? new(p) ::CbmMvdSensorFrameBuffer : new ::CbmMvdSensorFrameBuffer;
   }
   static void *newArray_CbmMvdSensorFrameBuffer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdSensorFrameBuffer[nElements] : new ::CbmMvdSensorFrameBuffer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdSensorFrameBuffer(void *p) {
      delete ((::CbmMvdSensorFrameBuffer*)p);
   }
   static void deleteArray_CbmMvdSensorFrameBuffer(void *p) {
      delete [] ((::CbmMvdSensorFrameBuffer*)p);
   }
   static void destruct_CbmMvdSensorFrameBuffer(void *p) {
      typedef ::CbmMvdSensorFrameBuffer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdSensorFrameBuffer

//______________________________________________________________________________
void CbmMvdSensorDataSheet::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdSensorDataSheet.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdSensorDataSheet::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdSensorDataSheet::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdSensorDataSheet(void *p) {
      return  p ? new(p) ::CbmMvdSensorDataSheet : new ::CbmMvdSensorDataSheet;
   }
   static void *newArray_CbmMvdSensorDataSheet(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdSensorDataSheet[nElements] : new ::CbmMvdSensorDataSheet[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdSensorDataSheet(void *p) {
      delete ((::CbmMvdSensorDataSheet*)p);
   }
   static void deleteArray_CbmMvdSensorDataSheet(void *p) {
      delete [] ((::CbmMvdSensorDataSheet*)p);
   }
   static void destruct_CbmMvdSensorDataSheet(void *p) {
      typedef ::CbmMvdSensorDataSheet current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdSensorDataSheet

//______________________________________________________________________________
void CbmMvdMimosa26AHR::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdMimosa26AHR.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdMimosa26AHR::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdMimosa26AHR::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdMimosa26AHR(void *p) {
      return  p ? new(p) ::CbmMvdMimosa26AHR : new ::CbmMvdMimosa26AHR;
   }
   static void *newArray_CbmMvdMimosa26AHR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdMimosa26AHR[nElements] : new ::CbmMvdMimosa26AHR[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdMimosa26AHR(void *p) {
      delete ((::CbmMvdMimosa26AHR*)p);
   }
   static void deleteArray_CbmMvdMimosa26AHR(void *p) {
      delete [] ((::CbmMvdMimosa26AHR*)p);
   }
   static void destruct_CbmMvdMimosa26AHR(void *p) {
      typedef ::CbmMvdMimosa26AHR current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdMimosa26AHR

//______________________________________________________________________________
void CbmMvdMimosa34::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdMimosa34.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdMimosa34::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdMimosa34::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdMimosa34(void *p) {
      return  p ? new(p) ::CbmMvdMimosa34 : new ::CbmMvdMimosa34;
   }
   static void *newArray_CbmMvdMimosa34(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdMimosa34[nElements] : new ::CbmMvdMimosa34[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdMimosa34(void *p) {
      delete ((::CbmMvdMimosa34*)p);
   }
   static void deleteArray_CbmMvdMimosa34(void *p) {
      delete [] ((::CbmMvdMimosa34*)p);
   }
   static void destruct_CbmMvdMimosa34(void *p) {
      typedef ::CbmMvdMimosa34 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdMimosa34

//______________________________________________________________________________
void CbmMvdMimosis::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdMimosis.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdMimosis::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdMimosis::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdMimosis(void *p) {
      return  p ? new(p) ::CbmMvdMimosis : new ::CbmMvdMimosis;
   }
   static void *newArray_CbmMvdMimosis(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdMimosis[nElements] : new ::CbmMvdMimosis[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdMimosis(void *p) {
      delete ((::CbmMvdMimosis*)p);
   }
   static void deleteArray_CbmMvdMimosis(void *p) {
      delete [] ((::CbmMvdMimosis*)p);
   }
   static void destruct_CbmMvdMimosis(void *p) {
      typedef ::CbmMvdMimosis current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdMimosis

//______________________________________________________________________________
void CbmMvdGeoHandler::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdGeoHandler.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdGeoHandler::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdGeoHandler::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdGeoHandler(void *p) {
      return  p ? new(p) ::CbmMvdGeoHandler : new ::CbmMvdGeoHandler;
   }
   static void *newArray_CbmMvdGeoHandler(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdGeoHandler[nElements] : new ::CbmMvdGeoHandler[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdGeoHandler(void *p) {
      delete ((::CbmMvdGeoHandler*)p);
   }
   static void deleteArray_CbmMvdGeoHandler(void *p) {
      delete [] ((::CbmMvdGeoHandler*)p);
   }
   static void destruct_CbmMvdGeoHandler(void *p) {
      typedef ::CbmMvdGeoHandler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdGeoHandler

//______________________________________________________________________________
void CbmMvdStationPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdStationPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdStationPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdStationPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdStationPar(void *p) {
      return  p ? new(p) ::CbmMvdStationPar : new ::CbmMvdStationPar;
   }
   static void *newArray_CbmMvdStationPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdStationPar[nElements] : new ::CbmMvdStationPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdStationPar(void *p) {
      delete ((::CbmMvdStationPar*)p);
   }
   static void deleteArray_CbmMvdStationPar(void *p) {
      delete [] ((::CbmMvdStationPar*)p);
   }
   static void destruct_CbmMvdStationPar(void *p) {
      typedef ::CbmMvdStationPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdStationPar

//______________________________________________________________________________
void CbmMvdDetector::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdDetector.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdDetector::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdDetector::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmMvdDetector(void *p) {
      delete ((::CbmMvdDetector*)p);
   }
   static void deleteArray_CbmMvdDetector(void *p) {
      delete [] ((::CbmMvdDetector*)p);
   }
   static void destruct_CbmMvdDetector(void *p) {
      typedef ::CbmMvdDetector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdDetector

//______________________________________________________________________________
void CbmMvdSensor::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdSensor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdSensor::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdSensor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdSensor(void *p) {
      return  p ? new(p) ::CbmMvdSensor : new ::CbmMvdSensor;
   }
   static void *newArray_CbmMvdSensor(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdSensor[nElements] : new ::CbmMvdSensor[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdSensor(void *p) {
      delete ((::CbmMvdSensor*)p);
   }
   static void deleteArray_CbmMvdSensor(void *p) {
      delete [] ((::CbmMvdSensor*)p);
   }
   static void destruct_CbmMvdSensor(void *p) {
      typedef ::CbmMvdSensor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdSensor

//______________________________________________________________________________
void CbmMvdPileupManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdPileupManager.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdPileupManager::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdPileupManager::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdPileupManager(void *p) {
      return  p ? new(p) ::CbmMvdPileupManager : new ::CbmMvdPileupManager;
   }
   static void *newArray_CbmMvdPileupManager(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdPileupManager[nElements] : new ::CbmMvdPileupManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdPileupManager(void *p) {
      delete ((::CbmMvdPileupManager*)p);
   }
   static void deleteArray_CbmMvdPileupManager(void *p) {
      delete [] ((::CbmMvdPileupManager*)p);
   }
   static void destruct_CbmMvdPileupManager(void *p) {
      typedef ::CbmMvdPileupManager current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdPileupManager

//______________________________________________________________________________
void CbmMvdTrackingInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdTrackingInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdTrackingInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdTrackingInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdTrackingInterface(void *p) {
      return  p ? new(p) ::CbmMvdTrackingInterface : new ::CbmMvdTrackingInterface;
   }
   static void *newArray_CbmMvdTrackingInterface(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdTrackingInterface[nElements] : new ::CbmMvdTrackingInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdTrackingInterface(void *p) {
      delete ((::CbmMvdTrackingInterface*)p);
   }
   static void deleteArray_CbmMvdTrackingInterface(void *p) {
      delete [] ((::CbmMvdTrackingInterface*)p);
   }
   static void destruct_CbmMvdTrackingInterface(void *p) {
      typedef ::CbmMvdTrackingInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdTrackingInterface

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
   static TClass *maplEintcOCbmMvdSensormUgR_Dictionary();
   static void maplEintcOCbmMvdSensormUgR_TClassManip(TClass*);
   static void *new_maplEintcOCbmMvdSensormUgR(void *p = nullptr);
   static void *newArray_maplEintcOCbmMvdSensormUgR(Long_t size, void *p);
   static void delete_maplEintcOCbmMvdSensormUgR(void *p);
   static void deleteArray_maplEintcOCbmMvdSensormUgR(void *p);
   static void destruct_maplEintcOCbmMvdSensormUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,CbmMvdSensor*>*)
   {
      map<int,CbmMvdSensor*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,CbmMvdSensor*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,CbmMvdSensor*>", -2, "map", 100,
                  typeid(map<int,CbmMvdSensor*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOCbmMvdSensormUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,CbmMvdSensor*>) );
      instance.SetNew(&new_maplEintcOCbmMvdSensormUgR);
      instance.SetNewArray(&newArray_maplEintcOCbmMvdSensormUgR);
      instance.SetDelete(&delete_maplEintcOCbmMvdSensormUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOCbmMvdSensormUgR);
      instance.SetDestructor(&destruct_maplEintcOCbmMvdSensormUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,CbmMvdSensor*> >()));

      ::ROOT::AddClassAlternate("map<int,CbmMvdSensor*>","std::map<int, CbmMvdSensor*, std::less<int>, std::allocator<std::pair<int const, CbmMvdSensor*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,CbmMvdSensor*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOCbmMvdSensormUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,CbmMvdSensor*>*)nullptr)->GetClass();
      maplEintcOCbmMvdSensormUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOCbmMvdSensormUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOCbmMvdSensormUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmMvdSensor*> : new map<int,CbmMvdSensor*>;
   }
   static void *newArray_maplEintcOCbmMvdSensormUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmMvdSensor*>[nElements] : new map<int,CbmMvdSensor*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOCbmMvdSensormUgR(void *p) {
      delete ((map<int,CbmMvdSensor*>*)p);
   }
   static void deleteArray_maplEintcOCbmMvdSensormUgR(void *p) {
      delete [] ((map<int,CbmMvdSensor*>*)p);
   }
   static void destruct_maplEintcOCbmMvdSensormUgR(void *p) {
      typedef map<int,CbmMvdSensor*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,CbmMvdSensor*>

namespace {
  void TriggerDictionaryInitialization_libCbmMvdBase_Impl() {
    static const char* headers[] = {
"CbmMvdSensorPlugin.h",
"CbmMvdSensorTask.h",
"CbmMvdSensorBuffer.h",
"CbmMvdSensorFrameBuffer.h",
"CbmMvdMimosa26AHR.h",
"CbmMvdMimosa34.h",
"CbmMvdMimosis.h",
"CbmMvdSensorDataSheet.h",
"CbmMvdGeoHandler.h",
"CbmMvdHelper.h",
"CbmMvdStationPar.h",
"CbmMvdDetector.h",
"CbmMvdSensor.h",
"CbmMvdPileupManager.h",
"CbmMvdTrackingInterface.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/tools",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/SensorDataSheets",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/tasks",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/buffers",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/tools",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/SensorDataSheets",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/tasks",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/buffers",
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
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/core/detectors/mvd/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmMvdBase dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmMvdSensorPlugin.h")))  CbmMvdSensorPlugin;
class __attribute__((annotate("$clingAutoload$CbmMvdSensorTask.h")))  CbmMvdSensorTask;
class __attribute__((annotate("$clingAutoload$CbmMvdSensorBuffer.h")))  CbmMvdSensorBuffer;
class __attribute__((annotate("$clingAutoload$CbmMvdSensorFrameBuffer.h")))  CbmMvdSensorFrameBuffer;
class __attribute__((annotate("$clingAutoload$CbmMvdSensorDataSheet.h")))  __attribute__((annotate("$clingAutoload$CbmMvdMimosa26AHR.h")))  CbmMvdSensorDataSheet;
class __attribute__((annotate("$clingAutoload$CbmMvdMimosa26AHR.h")))  CbmMvdMimosa26AHR;
class __attribute__((annotate("$clingAutoload$CbmMvdMimosa34.h")))  CbmMvdMimosa34;
class __attribute__((annotate("$clingAutoload$CbmMvdMimosis.h")))  CbmMvdMimosis;
class __attribute__((annotate("$clingAutoload$CbmMvdGeoHandler.h")))  CbmMvdGeoHandler;
class __attribute__((annotate("$clingAutoload$CbmMvdStationPar.h")))  CbmMvdStationPar;
class __attribute__((annotate("$clingAutoload$CbmMvdDetector.h")))  CbmMvdDetector;
class __attribute__((annotate("$clingAutoload$CbmMvdSensor.h")))  CbmMvdSensor;
class __attribute__((annotate("$clingAutoload$CbmMvdPileupManager.h")))  CbmMvdPileupManager;
class __attribute__((annotate("$clingAutoload$CbmMvdTrackingInterface.h")))  CbmMvdTrackingInterface;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmMvdBase dictionary payload"

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
#include "CbmMvdSensorPlugin.h"
#include "CbmMvdSensorTask.h"
#include "CbmMvdSensorBuffer.h"
#include "CbmMvdSensorFrameBuffer.h"
#include "CbmMvdMimosa26AHR.h"
#include "CbmMvdMimosa34.h"
#include "CbmMvdMimosis.h"
#include "CbmMvdSensorDataSheet.h"
#include "CbmMvdGeoHandler.h"
#include "CbmMvdHelper.h"
#include "CbmMvdStationPar.h"
#include "CbmMvdDetector.h"
#include "CbmMvdSensor.h"
#include "CbmMvdPileupManager.h"
#include "CbmMvdTrackingInterface.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmMvdDetector", payloadCode, "@",
"CbmMvdGeoHandler", payloadCode, "@",
"CbmMvdMimosa26AHR", payloadCode, "@",
"CbmMvdMimosa34", payloadCode, "@",
"CbmMvdMimosis", payloadCode, "@",
"CbmMvdPileupManager", payloadCode, "@",
"CbmMvdSensor", payloadCode, "@",
"CbmMvdSensorBuffer", payloadCode, "@",
"CbmMvdSensorDataSheet", payloadCode, "@",
"CbmMvdSensorFrameBuffer", payloadCode, "@",
"CbmMvdSensorPlugin", payloadCode, "@",
"CbmMvdSensorTask", payloadCode, "@",
"CbmMvdStationPar", payloadCode, "@",
"CbmMvdTrackingInterface", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmMvdBase",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmMvdBase_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmMvdBase_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmMvdBase() {
  TriggerDictionaryInitialization_libCbmMvdBase_Impl();
}
