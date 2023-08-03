// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmTrdBase
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
#include "CbmTrdGas.h"
#include "CbmTrdContFact.h"
#include "CbmTrdParManager.h"
#include "CbmTrdModuleAbstract.h"
#include "CbmMcbm2020TrdTshiftPar.h"
#include "CbmTrdParSet.h"
#include "CbmTrdParSetAsic.h"
#include "CbmTrdParSetGas.h"
#include "CbmTrdParSetGain.h"
#include "CbmTrdParSetGeo.h"
#include "CbmTrdParSetDigi.h"
#include "CbmTrdParMod.h"
#include "CbmTrdParAsic.h"
#include "CbmTrdParSpadic.h"
#include "CbmTrdSpadic.h"
#include "CbmTrdFASP.h"
#include "CbmTrdParFasp.h"
#include "CbmTrdParModDigi.h"
#include "CbmTrdParModGas.h"
#include "CbmTrdParModGain.h"
#include "CbmTrdParModGeo.h"
#include "CbmTrdTrackingInterface.h"
#include "CbmTrdGeoHandler.h"
#include "CbmTrdUtils.h"
#include "CbmTrddEdxUtils.h"
#include "CbmTrdHardwareSetupR.h"
#include "CbmTrdRadiator.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmTrdGas(void *p = nullptr);
   static void *newArray_CbmTrdGas(Long_t size, void *p);
   static void delete_CbmTrdGas(void *p);
   static void deleteArray_CbmTrdGas(void *p);
   static void destruct_CbmTrdGas(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdGas*)
   {
      ::CbmTrdGas *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdGas >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdGas", ::CbmTrdGas::Class_Version(), "CbmTrdGas.h", 18,
                  typeid(::CbmTrdGas), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdGas::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdGas) );
      instance.SetNew(&new_CbmTrdGas);
      instance.SetNewArray(&newArray_CbmTrdGas);
      instance.SetDelete(&delete_CbmTrdGas);
      instance.SetDeleteArray(&deleteArray_CbmTrdGas);
      instance.SetDestructor(&destruct_CbmTrdGas);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdGas*)
   {
      return GenerateInitInstanceLocal((::CbmTrdGas*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdGas*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdContFact(void *p = nullptr);
   static void *newArray_CbmTrdContFact(Long_t size, void *p);
   static void delete_CbmTrdContFact(void *p);
   static void deleteArray_CbmTrdContFact(void *p);
   static void destruct_CbmTrdContFact(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdContFact*)
   {
      ::CbmTrdContFact *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdContFact >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdContFact", ::CbmTrdContFact::Class_Version(), "CbmTrdContFact.h", 15,
                  typeid(::CbmTrdContFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdContFact::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdContFact) );
      instance.SetNew(&new_CbmTrdContFact);
      instance.SetNewArray(&newArray_CbmTrdContFact);
      instance.SetDelete(&delete_CbmTrdContFact);
      instance.SetDeleteArray(&deleteArray_CbmTrdContFact);
      instance.SetDestructor(&destruct_CbmTrdContFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdContFact*)
   {
      return GenerateInitInstanceLocal((::CbmTrdContFact*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdContFact*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParSet(void *p = nullptr);
   static void *newArray_CbmTrdParSet(Long_t size, void *p);
   static void delete_CbmTrdParSet(void *p);
   static void deleteArray_CbmTrdParSet(void *p);
   static void destruct_CbmTrdParSet(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParSet*)
   {
      ::CbmTrdParSet *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParSet >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParSet", ::CbmTrdParSet::Class_Version(), "CbmTrdParSet.h", 23,
                  typeid(::CbmTrdParSet), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParSet::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParSet) );
      instance.SetNew(&new_CbmTrdParSet);
      instance.SetNewArray(&newArray_CbmTrdParSet);
      instance.SetDelete(&delete_CbmTrdParSet);
      instance.SetDeleteArray(&deleteArray_CbmTrdParSet);
      instance.SetDestructor(&destruct_CbmTrdParSet);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParSet*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParSet*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParSet*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParSetAsic(void *p = nullptr);
   static void *newArray_CbmTrdParSetAsic(Long_t size, void *p);
   static void delete_CbmTrdParSetAsic(void *p);
   static void deleteArray_CbmTrdParSetAsic(void *p);
   static void destruct_CbmTrdParSetAsic(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParSetAsic*)
   {
      ::CbmTrdParSetAsic *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParSetAsic >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParSetAsic", ::CbmTrdParSetAsic::Class_Version(), "CbmTrdParSetAsic.h", 22,
                  typeid(::CbmTrdParSetAsic), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParSetAsic::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParSetAsic) );
      instance.SetNew(&new_CbmTrdParSetAsic);
      instance.SetNewArray(&newArray_CbmTrdParSetAsic);
      instance.SetDelete(&delete_CbmTrdParSetAsic);
      instance.SetDeleteArray(&deleteArray_CbmTrdParSetAsic);
      instance.SetDestructor(&destruct_CbmTrdParSetAsic);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParSetAsic*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParSetAsic*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParSetAsic*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdHardwareSetupR(void *p = nullptr);
   static void *newArray_CbmTrdHardwareSetupR(Long_t size, void *p);
   static void delete_CbmTrdHardwareSetupR(void *p);
   static void deleteArray_CbmTrdHardwareSetupR(void *p);
   static void destruct_CbmTrdHardwareSetupR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdHardwareSetupR*)
   {
      ::CbmTrdHardwareSetupR *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdHardwareSetupR >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdHardwareSetupR", ::CbmTrdHardwareSetupR::Class_Version(), "CbmTrdHardwareSetupR.h", 42,
                  typeid(::CbmTrdHardwareSetupR), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdHardwareSetupR::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdHardwareSetupR) );
      instance.SetNew(&new_CbmTrdHardwareSetupR);
      instance.SetNewArray(&newArray_CbmTrdHardwareSetupR);
      instance.SetDelete(&delete_CbmTrdHardwareSetupR);
      instance.SetDeleteArray(&deleteArray_CbmTrdHardwareSetupR);
      instance.SetDestructor(&destruct_CbmTrdHardwareSetupR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdHardwareSetupR*)
   {
      return GenerateInitInstanceLocal((::CbmTrdHardwareSetupR*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdHardwareSetupR*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParManager(void *p = nullptr);
   static void *newArray_CbmTrdParManager(Long_t size, void *p);
   static void delete_CbmTrdParManager(void *p);
   static void deleteArray_CbmTrdParManager(void *p);
   static void destruct_CbmTrdParManager(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParManager*)
   {
      ::CbmTrdParManager *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParManager >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParManager", ::CbmTrdParManager::Class_Version(), "CbmTrdParManager.h", 43,
                  typeid(::CbmTrdParManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParManager::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParManager) );
      instance.SetNew(&new_CbmTrdParManager);
      instance.SetNewArray(&newArray_CbmTrdParManager);
      instance.SetDelete(&delete_CbmTrdParManager);
      instance.SetDeleteArray(&deleteArray_CbmTrdParManager);
      instance.SetDestructor(&destruct_CbmTrdParManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParManager*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParManager*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParManager*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParMod(void *p = nullptr);
   static void *newArray_CbmTrdParMod(Long_t size, void *p);
   static void delete_CbmTrdParMod(void *p);
   static void deleteArray_CbmTrdParMod(void *p);
   static void destruct_CbmTrdParMod(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParMod*)
   {
      ::CbmTrdParMod *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParMod >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParMod", ::CbmTrdParMod::Class_Version(), "CbmTrdParMod.h", 13,
                  typeid(::CbmTrdParMod), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParMod::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParMod) );
      instance.SetNew(&new_CbmTrdParMod);
      instance.SetNewArray(&newArray_CbmTrdParMod);
      instance.SetDelete(&delete_CbmTrdParMod);
      instance.SetDeleteArray(&deleteArray_CbmTrdParMod);
      instance.SetDestructor(&destruct_CbmTrdParMod);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParMod*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParMod*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParMod*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParModDigi(void *p = nullptr);
   static void *newArray_CbmTrdParModDigi(Long_t size, void *p);
   static void delete_CbmTrdParModDigi(void *p);
   static void deleteArray_CbmTrdParModDigi(void *p);
   static void destruct_CbmTrdParModDigi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParModDigi*)
   {
      ::CbmTrdParModDigi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParModDigi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParModDigi", ::CbmTrdParModDigi::Class_Version(), "CbmTrdParModDigi.h", 18,
                  typeid(::CbmTrdParModDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParModDigi::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParModDigi) );
      instance.SetNew(&new_CbmTrdParModDigi);
      instance.SetNewArray(&newArray_CbmTrdParModDigi);
      instance.SetDelete(&delete_CbmTrdParModDigi);
      instance.SetDeleteArray(&deleteArray_CbmTrdParModDigi);
      instance.SetDestructor(&destruct_CbmTrdParModDigi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParModDigi*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParModDigi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParModDigi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParModGeo(void *p = nullptr);
   static void *newArray_CbmTrdParModGeo(Long_t size, void *p);
   static void delete_CbmTrdParModGeo(void *p);
   static void deleteArray_CbmTrdParModGeo(void *p);
   static void destruct_CbmTrdParModGeo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParModGeo*)
   {
      ::CbmTrdParModGeo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParModGeo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParModGeo", ::CbmTrdParModGeo::Class_Version(), "CbmTrdParModGeo.h", 16,
                  typeid(::CbmTrdParModGeo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParModGeo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParModGeo) );
      instance.SetNew(&new_CbmTrdParModGeo);
      instance.SetNewArray(&newArray_CbmTrdParModGeo);
      instance.SetDelete(&delete_CbmTrdParModGeo);
      instance.SetDeleteArray(&deleteArray_CbmTrdParModGeo);
      instance.SetDestructor(&destruct_CbmTrdParModGeo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParModGeo*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParModGeo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParModGeo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdModuleAbstract(void *p = nullptr);
   static void *newArray_CbmTrdModuleAbstract(Long_t size, void *p);
   static void delete_CbmTrdModuleAbstract(void *p);
   static void deleteArray_CbmTrdModuleAbstract(void *p);
   static void destruct_CbmTrdModuleAbstract(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdModuleAbstract*)
   {
      ::CbmTrdModuleAbstract *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdModuleAbstract >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdModuleAbstract", ::CbmTrdModuleAbstract::Class_Version(), "CbmTrdModuleAbstract.h", 22,
                  typeid(::CbmTrdModuleAbstract), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdModuleAbstract::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdModuleAbstract) );
      instance.SetNew(&new_CbmTrdModuleAbstract);
      instance.SetNewArray(&newArray_CbmTrdModuleAbstract);
      instance.SetDelete(&delete_CbmTrdModuleAbstract);
      instance.SetDeleteArray(&deleteArray_CbmTrdModuleAbstract);
      instance.SetDestructor(&destruct_CbmTrdModuleAbstract);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdModuleAbstract*)
   {
      return GenerateInitInstanceLocal((::CbmTrdModuleAbstract*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdModuleAbstract*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2020TrdTshiftPar(void *p = nullptr);
   static void *newArray_CbmMcbm2020TrdTshiftPar(Long_t size, void *p);
   static void delete_CbmMcbm2020TrdTshiftPar(void *p);
   static void deleteArray_CbmMcbm2020TrdTshiftPar(void *p);
   static void destruct_CbmMcbm2020TrdTshiftPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2020TrdTshiftPar*)
   {
      ::CbmMcbm2020TrdTshiftPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2020TrdTshiftPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2020TrdTshiftPar", ::CbmMcbm2020TrdTshiftPar::Class_Version(), "CbmMcbm2020TrdTshiftPar.h", 20,
                  typeid(::CbmMcbm2020TrdTshiftPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2020TrdTshiftPar::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2020TrdTshiftPar) );
      instance.SetNew(&new_CbmMcbm2020TrdTshiftPar);
      instance.SetNewArray(&newArray_CbmMcbm2020TrdTshiftPar);
      instance.SetDelete(&delete_CbmMcbm2020TrdTshiftPar);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2020TrdTshiftPar);
      instance.SetDestructor(&destruct_CbmMcbm2020TrdTshiftPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2020TrdTshiftPar*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2020TrdTshiftPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2020TrdTshiftPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParSetGas(void *p = nullptr);
   static void *newArray_CbmTrdParSetGas(Long_t size, void *p);
   static void delete_CbmTrdParSetGas(void *p);
   static void deleteArray_CbmTrdParSetGas(void *p);
   static void destruct_CbmTrdParSetGas(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParSetGas*)
   {
      ::CbmTrdParSetGas *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParSetGas >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParSetGas", ::CbmTrdParSetGas::Class_Version(), "CbmTrdParSetGas.h", 19,
                  typeid(::CbmTrdParSetGas), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParSetGas::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParSetGas) );
      instance.SetNew(&new_CbmTrdParSetGas);
      instance.SetNewArray(&newArray_CbmTrdParSetGas);
      instance.SetDelete(&delete_CbmTrdParSetGas);
      instance.SetDeleteArray(&deleteArray_CbmTrdParSetGas);
      instance.SetDestructor(&destruct_CbmTrdParSetGas);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParSetGas*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParSetGas*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParSetGas*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParSetGain(void *p = nullptr);
   static void *newArray_CbmTrdParSetGain(Long_t size, void *p);
   static void delete_CbmTrdParSetGain(void *p);
   static void deleteArray_CbmTrdParSetGain(void *p);
   static void destruct_CbmTrdParSetGain(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParSetGain*)
   {
      ::CbmTrdParSetGain *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParSetGain >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParSetGain", ::CbmTrdParSetGain::Class_Version(), "CbmTrdParSetGain.h", 20,
                  typeid(::CbmTrdParSetGain), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParSetGain::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParSetGain) );
      instance.SetNew(&new_CbmTrdParSetGain);
      instance.SetNewArray(&newArray_CbmTrdParSetGain);
      instance.SetDelete(&delete_CbmTrdParSetGain);
      instance.SetDeleteArray(&deleteArray_CbmTrdParSetGain);
      instance.SetDestructor(&destruct_CbmTrdParSetGain);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParSetGain*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParSetGain*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParSetGain*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParSetGeo(void *p = nullptr);
   static void *newArray_CbmTrdParSetGeo(Long_t size, void *p);
   static void delete_CbmTrdParSetGeo(void *p);
   static void deleteArray_CbmTrdParSetGeo(void *p);
   static void destruct_CbmTrdParSetGeo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParSetGeo*)
   {
      ::CbmTrdParSetGeo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParSetGeo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParSetGeo", ::CbmTrdParSetGeo::Class_Version(), "CbmTrdParSetGeo.h", 13,
                  typeid(::CbmTrdParSetGeo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParSetGeo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParSetGeo) );
      instance.SetNew(&new_CbmTrdParSetGeo);
      instance.SetNewArray(&newArray_CbmTrdParSetGeo);
      instance.SetDelete(&delete_CbmTrdParSetGeo);
      instance.SetDeleteArray(&deleteArray_CbmTrdParSetGeo);
      instance.SetDestructor(&destruct_CbmTrdParSetGeo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParSetGeo*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParSetGeo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParSetGeo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParSetDigi(void *p = nullptr);
   static void *newArray_CbmTrdParSetDigi(Long_t size, void *p);
   static void delete_CbmTrdParSetDigi(void *p);
   static void deleteArray_CbmTrdParSetDigi(void *p);
   static void destruct_CbmTrdParSetDigi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParSetDigi*)
   {
      ::CbmTrdParSetDigi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParSetDigi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParSetDigi", ::CbmTrdParSetDigi::Class_Version(), "CbmTrdParSetDigi.h", 19,
                  typeid(::CbmTrdParSetDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParSetDigi::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParSetDigi) );
      instance.SetNew(&new_CbmTrdParSetDigi);
      instance.SetNewArray(&newArray_CbmTrdParSetDigi);
      instance.SetDelete(&delete_CbmTrdParSetDigi);
      instance.SetDeleteArray(&deleteArray_CbmTrdParSetDigi);
      instance.SetDestructor(&destruct_CbmTrdParSetDigi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParSetDigi*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParSetDigi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParSetDigi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrdParAsic(void *p);
   static void deleteArray_CbmTrdParAsic(void *p);
   static void destruct_CbmTrdParAsic(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParAsic*)
   {
      ::CbmTrdParAsic *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParAsic >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParAsic", ::CbmTrdParAsic::Class_Version(), "CbmTrdParAsic.h", 20,
                  typeid(::CbmTrdParAsic), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParAsic::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParAsic) );
      instance.SetDelete(&delete_CbmTrdParAsic);
      instance.SetDeleteArray(&deleteArray_CbmTrdParAsic);
      instance.SetDestructor(&destruct_CbmTrdParAsic);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParAsic*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParAsic*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParAsic*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParSpadic(void *p = nullptr);
   static void *newArray_CbmTrdParSpadic(Long_t size, void *p);
   static void delete_CbmTrdParSpadic(void *p);
   static void deleteArray_CbmTrdParSpadic(void *p);
   static void destruct_CbmTrdParSpadic(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParSpadic*)
   {
      ::CbmTrdParSpadic *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParSpadic >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParSpadic", ::CbmTrdParSpadic::Class_Version(), "CbmTrdParSpadic.h", 23,
                  typeid(::CbmTrdParSpadic), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParSpadic::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParSpadic) );
      instance.SetNew(&new_CbmTrdParSpadic);
      instance.SetNewArray(&newArray_CbmTrdParSpadic);
      instance.SetDelete(&delete_CbmTrdParSpadic);
      instance.SetDeleteArray(&deleteArray_CbmTrdParSpadic);
      instance.SetDestructor(&destruct_CbmTrdParSpadic);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParSpadic*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParSpadic*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParSpadic*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrddEdxUtils(void *p = nullptr);
   static void *newArray_CbmTrddEdxUtils(Long_t size, void *p);
   static void delete_CbmTrddEdxUtils(void *p);
   static void deleteArray_CbmTrddEdxUtils(void *p);
   static void destruct_CbmTrddEdxUtils(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrddEdxUtils*)
   {
      ::CbmTrddEdxUtils *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrddEdxUtils >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrddEdxUtils", ::CbmTrddEdxUtils::Class_Version(), "CbmTrddEdxUtils.h", 22,
                  typeid(::CbmTrddEdxUtils), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrddEdxUtils::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrddEdxUtils) );
      instance.SetNew(&new_CbmTrddEdxUtils);
      instance.SetNewArray(&newArray_CbmTrddEdxUtils);
      instance.SetDelete(&delete_CbmTrddEdxUtils);
      instance.SetDeleteArray(&deleteArray_CbmTrddEdxUtils);
      instance.SetDestructor(&destruct_CbmTrddEdxUtils);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrddEdxUtils*)
   {
      return GenerateInitInstanceLocal((::CbmTrddEdxUtils*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrddEdxUtils*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdSpadic(void *p = nullptr);
   static void *newArray_CbmTrdSpadic(Long_t size, void *p);
   static void delete_CbmTrdSpadic(void *p);
   static void deleteArray_CbmTrdSpadic(void *p);
   static void destruct_CbmTrdSpadic(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdSpadic*)
   {
      ::CbmTrdSpadic *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdSpadic >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdSpadic", ::CbmTrdSpadic::Class_Version(), "CbmTrdSpadic.h", 36,
                  typeid(::CbmTrdSpadic), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdSpadic::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdSpadic) );
      instance.SetNew(&new_CbmTrdSpadic);
      instance.SetNewArray(&newArray_CbmTrdSpadic);
      instance.SetDelete(&delete_CbmTrdSpadic);
      instance.SetDeleteArray(&deleteArray_CbmTrdSpadic);
      instance.SetDestructor(&destruct_CbmTrdSpadic);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdSpadic*)
   {
      return GenerateInitInstanceLocal((::CbmTrdSpadic*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdSpadic*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParFaspChannel(void *p = nullptr);
   static void *newArray_CbmTrdParFaspChannel(Long_t size, void *p);
   static void delete_CbmTrdParFaspChannel(void *p);
   static void deleteArray_CbmTrdParFaspChannel(void *p);
   static void destruct_CbmTrdParFaspChannel(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParFaspChannel*)
   {
      ::CbmTrdParFaspChannel *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParFaspChannel >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParFaspChannel", ::CbmTrdParFaspChannel::Class_Version(), "CbmTrdParFasp.h", 27,
                  typeid(::CbmTrdParFaspChannel), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParFaspChannel::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParFaspChannel) );
      instance.SetNew(&new_CbmTrdParFaspChannel);
      instance.SetNewArray(&newArray_CbmTrdParFaspChannel);
      instance.SetDelete(&delete_CbmTrdParFaspChannel);
      instance.SetDeleteArray(&deleteArray_CbmTrdParFaspChannel);
      instance.SetDestructor(&destruct_CbmTrdParFaspChannel);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParFaspChannel*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParFaspChannel*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParFaspChannel*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParFasp(void *p = nullptr);
   static void *newArray_CbmTrdParFasp(Long_t size, void *p);
   static void delete_CbmTrdParFasp(void *p);
   static void deleteArray_CbmTrdParFasp(void *p);
   static void destruct_CbmTrdParFasp(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParFasp*)
   {
      ::CbmTrdParFasp *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParFasp >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParFasp", ::CbmTrdParFasp::Class_Version(), "CbmTrdParFasp.h", 75,
                  typeid(::CbmTrdParFasp), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParFasp::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParFasp) );
      instance.SetNew(&new_CbmTrdParFasp);
      instance.SetNewArray(&newArray_CbmTrdParFasp);
      instance.SetDelete(&delete_CbmTrdParFasp);
      instance.SetDeleteArray(&deleteArray_CbmTrdParFasp);
      instance.SetDestructor(&destruct_CbmTrdParFasp);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParFasp*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParFasp*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParFasp*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdFASP(void *p = nullptr);
   static void *newArray_CbmTrdFASP(Long_t size, void *p);
   static void delete_CbmTrdFASP(void *p);
   static void deleteArray_CbmTrdFASP(void *p);
   static void destruct_CbmTrdFASP(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdFASP*)
   {
      ::CbmTrdFASP *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdFASP >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdFASP", ::CbmTrdFASP::Class_Version(), "CbmTrdFASP.h", 30,
                  typeid(::CbmTrdFASP), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdFASP::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdFASP) );
      instance.SetNew(&new_CbmTrdFASP);
      instance.SetNewArray(&newArray_CbmTrdFASP);
      instance.SetDelete(&delete_CbmTrdFASP);
      instance.SetDeleteArray(&deleteArray_CbmTrdFASP);
      instance.SetDestructor(&destruct_CbmTrdFASP);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdFASP*)
   {
      return GenerateInitInstanceLocal((::CbmTrdFASP*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdFASP*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParModGas(void *p = nullptr);
   static void *newArray_CbmTrdParModGas(Long_t size, void *p);
   static void delete_CbmTrdParModGas(void *p);
   static void deleteArray_CbmTrdParModGas(void *p);
   static void destruct_CbmTrdParModGas(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParModGas*)
   {
      ::CbmTrdParModGas *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParModGas >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParModGas", ::CbmTrdParModGas::Class_Version(), "CbmTrdParModGas.h", 20,
                  typeid(::CbmTrdParModGas), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParModGas::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParModGas) );
      instance.SetNew(&new_CbmTrdParModGas);
      instance.SetNewArray(&newArray_CbmTrdParModGas);
      instance.SetDelete(&delete_CbmTrdParModGas);
      instance.SetDeleteArray(&deleteArray_CbmTrdParModGas);
      instance.SetDestructor(&destruct_CbmTrdParModGas);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParModGas*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParModGas*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParModGas*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdParModGain(void *p = nullptr);
   static void *newArray_CbmTrdParModGain(Long_t size, void *p);
   static void delete_CbmTrdParModGain(void *p);
   static void deleteArray_CbmTrdParModGain(void *p);
   static void destruct_CbmTrdParModGain(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdParModGain*)
   {
      ::CbmTrdParModGain *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdParModGain >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdParModGain", ::CbmTrdParModGain::Class_Version(), "CbmTrdParModGain.h", 13,
                  typeid(::CbmTrdParModGain), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdParModGain::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdParModGain) );
      instance.SetNew(&new_CbmTrdParModGain);
      instance.SetNewArray(&newArray_CbmTrdParModGain);
      instance.SetDelete(&delete_CbmTrdParModGain);
      instance.SetDeleteArray(&deleteArray_CbmTrdParModGain);
      instance.SetDestructor(&destruct_CbmTrdParModGain);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdParModGain*)
   {
      return GenerateInitInstanceLocal((::CbmTrdParModGain*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdParModGain*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdTrackingInterface(void *p = nullptr);
   static void *newArray_CbmTrdTrackingInterface(Long_t size, void *p);
   static void delete_CbmTrdTrackingInterface(void *p);
   static void deleteArray_CbmTrdTrackingInterface(void *p);
   static void destruct_CbmTrdTrackingInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdTrackingInterface*)
   {
      ::CbmTrdTrackingInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdTrackingInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdTrackingInterface", ::CbmTrdTrackingInterface::Class_Version(), "CbmTrdTrackingInterface.h", 33,
                  typeid(::CbmTrdTrackingInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdTrackingInterface::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdTrackingInterface) );
      instance.SetNew(&new_CbmTrdTrackingInterface);
      instance.SetNewArray(&newArray_CbmTrdTrackingInterface);
      instance.SetDelete(&delete_CbmTrdTrackingInterface);
      instance.SetDeleteArray(&deleteArray_CbmTrdTrackingInterface);
      instance.SetDestructor(&destruct_CbmTrdTrackingInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdTrackingInterface*)
   {
      return GenerateInitInstanceLocal((::CbmTrdTrackingInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdTrackingInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdGeoHandler(void *p = nullptr);
   static void *newArray_CbmTrdGeoHandler(Long_t size, void *p);
   static void delete_CbmTrdGeoHandler(void *p);
   static void deleteArray_CbmTrdGeoHandler(void *p);
   static void destruct_CbmTrdGeoHandler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdGeoHandler*)
   {
      ::CbmTrdGeoHandler *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdGeoHandler >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdGeoHandler", ::CbmTrdGeoHandler::Class_Version(), "CbmTrdGeoHandler.h", 33,
                  typeid(::CbmTrdGeoHandler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdGeoHandler::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdGeoHandler) );
      instance.SetNew(&new_CbmTrdGeoHandler);
      instance.SetNewArray(&newArray_CbmTrdGeoHandler);
      instance.SetDelete(&delete_CbmTrdGeoHandler);
      instance.SetDeleteArray(&deleteArray_CbmTrdGeoHandler);
      instance.SetDestructor(&destruct_CbmTrdGeoHandler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdGeoHandler*)
   {
      return GenerateInitInstanceLocal((::CbmTrdGeoHandler*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdGeoHandler*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmTrdUtils_Dictionary();
   static void CbmTrdUtils_TClassManip(TClass*);
   static void *new_CbmTrdUtils(void *p = nullptr);
   static void *newArray_CbmTrdUtils(Long_t size, void *p);
   static void delete_CbmTrdUtils(void *p);
   static void deleteArray_CbmTrdUtils(void *p);
   static void destruct_CbmTrdUtils(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdUtils*)
   {
      ::CbmTrdUtils *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmTrdUtils));
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdUtils", "CbmTrdUtils.h", 23,
                  typeid(::CbmTrdUtils), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmTrdUtils_Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdUtils) );
      instance.SetNew(&new_CbmTrdUtils);
      instance.SetNewArray(&newArray_CbmTrdUtils);
      instance.SetDelete(&delete_CbmTrdUtils);
      instance.SetDeleteArray(&deleteArray_CbmTrdUtils);
      instance.SetDestructor(&destruct_CbmTrdUtils);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdUtils*)
   {
      return GenerateInitInstanceLocal((::CbmTrdUtils*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdUtils*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmTrdUtils_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmTrdUtils*)nullptr)->GetClass();
      CbmTrdUtils_TClassManip(theClass);
   return theClass;
   }

   static void CbmTrdUtils_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdRadiator(void *p = nullptr);
   static void *newArray_CbmTrdRadiator(Long_t size, void *p);
   static void delete_CbmTrdRadiator(void *p);
   static void deleteArray_CbmTrdRadiator(void *p);
   static void destruct_CbmTrdRadiator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdRadiator*)
   {
      ::CbmTrdRadiator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdRadiator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdRadiator", ::CbmTrdRadiator::Class_Version(), "CbmTrdRadiator.h", 29,
                  typeid(::CbmTrdRadiator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdRadiator::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdRadiator) );
      instance.SetNew(&new_CbmTrdRadiator);
      instance.SetNewArray(&newArray_CbmTrdRadiator);
      instance.SetDelete(&delete_CbmTrdRadiator);
      instance.SetDeleteArray(&deleteArray_CbmTrdRadiator);
      instance.SetDestructor(&destruct_CbmTrdRadiator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdRadiator*)
   {
      return GenerateInitInstanceLocal((::CbmTrdRadiator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdRadiator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdGas::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdGas::Class_Name()
{
   return "CbmTrdGas";
}

//______________________________________________________________________________
const char *CbmTrdGas::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdGas*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdGas::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdGas*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdGas::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdGas*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdGas::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdGas*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdContFact::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdContFact::Class_Name()
{
   return "CbmTrdContFact";
}

//______________________________________________________________________________
const char *CbmTrdContFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdContFact*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdContFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdContFact*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdContFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdContFact*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdContFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdContFact*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParSet::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParSet::Class_Name()
{
   return "CbmTrdParSet";
}

//______________________________________________________________________________
const char *CbmTrdParSet::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSet*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParSet::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSet*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParSet::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSet*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParSet::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSet*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParSetAsic::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParSetAsic::Class_Name()
{
   return "CbmTrdParSetAsic";
}

//______________________________________________________________________________
const char *CbmTrdParSetAsic::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetAsic*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParSetAsic::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetAsic*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParSetAsic::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetAsic*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParSetAsic::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetAsic*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdHardwareSetupR::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdHardwareSetupR::Class_Name()
{
   return "CbmTrdHardwareSetupR";
}

//______________________________________________________________________________
const char *CbmTrdHardwareSetupR::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHardwareSetupR*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdHardwareSetupR::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHardwareSetupR*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdHardwareSetupR::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHardwareSetupR*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdHardwareSetupR::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHardwareSetupR*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParManager::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParManager::Class_Name()
{
   return "CbmTrdParManager";
}

//______________________________________________________________________________
const char *CbmTrdParManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParManager*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParManager*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParManager*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParManager*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParMod::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParMod::Class_Name()
{
   return "CbmTrdParMod";
}

//______________________________________________________________________________
const char *CbmTrdParMod::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParMod*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParMod::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParMod*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParMod::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParMod*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParMod::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParMod*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParModDigi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParModDigi::Class_Name()
{
   return "CbmTrdParModDigi";
}

//______________________________________________________________________________
const char *CbmTrdParModDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModDigi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParModDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModDigi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParModDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModDigi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParModDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModDigi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParModGeo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParModGeo::Class_Name()
{
   return "CbmTrdParModGeo";
}

//______________________________________________________________________________
const char *CbmTrdParModGeo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModGeo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParModGeo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModGeo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParModGeo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModGeo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParModGeo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModGeo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdModuleAbstract::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdModuleAbstract::Class_Name()
{
   return "CbmTrdModuleAbstract";
}

//______________________________________________________________________________
const char *CbmTrdModuleAbstract::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleAbstract*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdModuleAbstract::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleAbstract*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdModuleAbstract::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleAbstract*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdModuleAbstract::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleAbstract*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2020TrdTshiftPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2020TrdTshiftPar::Class_Name()
{
   return "CbmMcbm2020TrdTshiftPar";
}

//______________________________________________________________________________
const char *CbmMcbm2020TrdTshiftPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2020TrdTshiftPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2020TrdTshiftPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2020TrdTshiftPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2020TrdTshiftPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2020TrdTshiftPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2020TrdTshiftPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2020TrdTshiftPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParSetGas::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParSetGas::Class_Name()
{
   return "CbmTrdParSetGas";
}

//______________________________________________________________________________
const char *CbmTrdParSetGas::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetGas*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParSetGas::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetGas*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParSetGas::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetGas*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParSetGas::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetGas*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParSetGain::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParSetGain::Class_Name()
{
   return "CbmTrdParSetGain";
}

//______________________________________________________________________________
const char *CbmTrdParSetGain::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetGain*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParSetGain::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetGain*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParSetGain::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetGain*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParSetGain::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetGain*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParSetGeo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParSetGeo::Class_Name()
{
   return "CbmTrdParSetGeo";
}

//______________________________________________________________________________
const char *CbmTrdParSetGeo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetGeo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParSetGeo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetGeo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParSetGeo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetGeo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParSetGeo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetGeo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParSetDigi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParSetDigi::Class_Name()
{
   return "CbmTrdParSetDigi";
}

//______________________________________________________________________________
const char *CbmTrdParSetDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetDigi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParSetDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetDigi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParSetDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetDigi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParSetDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSetDigi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParAsic::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParAsic::Class_Name()
{
   return "CbmTrdParAsic";
}

//______________________________________________________________________________
const char *CbmTrdParAsic::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParAsic*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParAsic::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParAsic*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParAsic::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParAsic*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParAsic::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParAsic*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParSpadic::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParSpadic::Class_Name()
{
   return "CbmTrdParSpadic";
}

//______________________________________________________________________________
const char *CbmTrdParSpadic::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSpadic*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParSpadic::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSpadic*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParSpadic::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSpadic*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParSpadic::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParSpadic*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrddEdxUtils::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrddEdxUtils::Class_Name()
{
   return "CbmTrddEdxUtils";
}

//______________________________________________________________________________
const char *CbmTrddEdxUtils::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrddEdxUtils*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrddEdxUtils::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrddEdxUtils*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrddEdxUtils::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrddEdxUtils*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrddEdxUtils::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrddEdxUtils*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdSpadic::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdSpadic::Class_Name()
{
   return "CbmTrdSpadic";
}

//______________________________________________________________________________
const char *CbmTrdSpadic::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSpadic*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdSpadic::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSpadic*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdSpadic::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSpadic*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdSpadic::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdSpadic*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParFaspChannel::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParFaspChannel::Class_Name()
{
   return "CbmTrdParFaspChannel";
}

//______________________________________________________________________________
const char *CbmTrdParFaspChannel::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParFaspChannel*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParFaspChannel::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParFaspChannel*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParFaspChannel::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParFaspChannel*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParFaspChannel::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParFaspChannel*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParFasp::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParFasp::Class_Name()
{
   return "CbmTrdParFasp";
}

//______________________________________________________________________________
const char *CbmTrdParFasp::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParFasp*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParFasp::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParFasp*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParFasp::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParFasp*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParFasp::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParFasp*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdFASP::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdFASP::Class_Name()
{
   return "CbmTrdFASP";
}

//______________________________________________________________________________
const char *CbmTrdFASP::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdFASP*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdFASP::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdFASP*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdFASP::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdFASP*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdFASP::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdFASP*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParModGas::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParModGas::Class_Name()
{
   return "CbmTrdParModGas";
}

//______________________________________________________________________________
const char *CbmTrdParModGas::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModGas*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParModGas::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModGas*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParModGas::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModGas*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParModGas::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModGas*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdParModGain::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdParModGain::Class_Name()
{
   return "CbmTrdParModGain";
}

//______________________________________________________________________________
const char *CbmTrdParModGain::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModGain*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdParModGain::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModGain*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdParModGain::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModGain*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdParModGain::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdParModGain*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdTrackingInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdTrackingInterface::Class_Name()
{
   return "CbmTrdTrackingInterface";
}

//______________________________________________________________________________
const char *CbmTrdTrackingInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackingInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdTrackingInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackingInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdTrackingInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackingInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdTrackingInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackingInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdGeoHandler::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdGeoHandler::Class_Name()
{
   return "CbmTrdGeoHandler";
}

//______________________________________________________________________________
const char *CbmTrdGeoHandler::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdGeoHandler*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdGeoHandler::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdGeoHandler*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdGeoHandler::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdGeoHandler*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdGeoHandler::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdGeoHandler*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdRadiator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdRadiator::Class_Name()
{
   return "CbmTrdRadiator";
}

//______________________________________________________________________________
const char *CbmTrdRadiator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRadiator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdRadiator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRadiator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdRadiator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRadiator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdRadiator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRadiator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmTrdGas::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdGas.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdGas::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdGas::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdGas(void *p) {
      return  p ? new(p) ::CbmTrdGas : new ::CbmTrdGas;
   }
   static void *newArray_CbmTrdGas(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdGas[nElements] : new ::CbmTrdGas[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdGas(void *p) {
      delete ((::CbmTrdGas*)p);
   }
   static void deleteArray_CbmTrdGas(void *p) {
      delete [] ((::CbmTrdGas*)p);
   }
   static void destruct_CbmTrdGas(void *p) {
      typedef ::CbmTrdGas current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdGas

//______________________________________________________________________________
void CbmTrdContFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdContFact.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdContFact::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdContFact::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdContFact(void *p) {
      return  p ? new(p) ::CbmTrdContFact : new ::CbmTrdContFact;
   }
   static void *newArray_CbmTrdContFact(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdContFact[nElements] : new ::CbmTrdContFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdContFact(void *p) {
      delete ((::CbmTrdContFact*)p);
   }
   static void deleteArray_CbmTrdContFact(void *p) {
      delete [] ((::CbmTrdContFact*)p);
   }
   static void destruct_CbmTrdContFact(void *p) {
      typedef ::CbmTrdContFact current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdContFact

//______________________________________________________________________________
void CbmTrdParSet::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParSet.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParSet::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParSet::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParSet(void *p) {
      return  p ? new(p) ::CbmTrdParSet : new ::CbmTrdParSet;
   }
   static void *newArray_CbmTrdParSet(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParSet[nElements] : new ::CbmTrdParSet[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParSet(void *p) {
      delete ((::CbmTrdParSet*)p);
   }
   static void deleteArray_CbmTrdParSet(void *p) {
      delete [] ((::CbmTrdParSet*)p);
   }
   static void destruct_CbmTrdParSet(void *p) {
      typedef ::CbmTrdParSet current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParSet

//______________________________________________________________________________
void CbmTrdParSetAsic::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParSetAsic.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParSetAsic::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParSetAsic::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParSetAsic(void *p) {
      return  p ? new(p) ::CbmTrdParSetAsic : new ::CbmTrdParSetAsic;
   }
   static void *newArray_CbmTrdParSetAsic(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParSetAsic[nElements] : new ::CbmTrdParSetAsic[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParSetAsic(void *p) {
      delete ((::CbmTrdParSetAsic*)p);
   }
   static void deleteArray_CbmTrdParSetAsic(void *p) {
      delete [] ((::CbmTrdParSetAsic*)p);
   }
   static void destruct_CbmTrdParSetAsic(void *p) {
      typedef ::CbmTrdParSetAsic current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParSetAsic

//______________________________________________________________________________
void CbmTrdHardwareSetupR::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdHardwareSetupR.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdHardwareSetupR::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdHardwareSetupR::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdHardwareSetupR(void *p) {
      return  p ? new(p) ::CbmTrdHardwareSetupR : new ::CbmTrdHardwareSetupR;
   }
   static void *newArray_CbmTrdHardwareSetupR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdHardwareSetupR[nElements] : new ::CbmTrdHardwareSetupR[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdHardwareSetupR(void *p) {
      delete ((::CbmTrdHardwareSetupR*)p);
   }
   static void deleteArray_CbmTrdHardwareSetupR(void *p) {
      delete [] ((::CbmTrdHardwareSetupR*)p);
   }
   static void destruct_CbmTrdHardwareSetupR(void *p) {
      typedef ::CbmTrdHardwareSetupR current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdHardwareSetupR

//______________________________________________________________________________
void CbmTrdParManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParManager.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParManager::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParManager::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParManager(void *p) {
      return  p ? new(p) ::CbmTrdParManager : new ::CbmTrdParManager;
   }
   static void *newArray_CbmTrdParManager(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParManager[nElements] : new ::CbmTrdParManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParManager(void *p) {
      delete ((::CbmTrdParManager*)p);
   }
   static void deleteArray_CbmTrdParManager(void *p) {
      delete [] ((::CbmTrdParManager*)p);
   }
   static void destruct_CbmTrdParManager(void *p) {
      typedef ::CbmTrdParManager current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParManager

//______________________________________________________________________________
void CbmTrdParMod::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParMod.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParMod::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParMod::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParMod(void *p) {
      return  p ? new(p) ::CbmTrdParMod : new ::CbmTrdParMod;
   }
   static void *newArray_CbmTrdParMod(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParMod[nElements] : new ::CbmTrdParMod[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParMod(void *p) {
      delete ((::CbmTrdParMod*)p);
   }
   static void deleteArray_CbmTrdParMod(void *p) {
      delete [] ((::CbmTrdParMod*)p);
   }
   static void destruct_CbmTrdParMod(void *p) {
      typedef ::CbmTrdParMod current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParMod

//______________________________________________________________________________
void CbmTrdParModDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParModDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParModDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParModDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParModDigi(void *p) {
      return  p ? new(p) ::CbmTrdParModDigi : new ::CbmTrdParModDigi;
   }
   static void *newArray_CbmTrdParModDigi(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParModDigi[nElements] : new ::CbmTrdParModDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParModDigi(void *p) {
      delete ((::CbmTrdParModDigi*)p);
   }
   static void deleteArray_CbmTrdParModDigi(void *p) {
      delete [] ((::CbmTrdParModDigi*)p);
   }
   static void destruct_CbmTrdParModDigi(void *p) {
      typedef ::CbmTrdParModDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParModDigi

//______________________________________________________________________________
void CbmTrdParModGeo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParModGeo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParModGeo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParModGeo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParModGeo(void *p) {
      return  p ? new(p) ::CbmTrdParModGeo : new ::CbmTrdParModGeo;
   }
   static void *newArray_CbmTrdParModGeo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParModGeo[nElements] : new ::CbmTrdParModGeo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParModGeo(void *p) {
      delete ((::CbmTrdParModGeo*)p);
   }
   static void deleteArray_CbmTrdParModGeo(void *p) {
      delete [] ((::CbmTrdParModGeo*)p);
   }
   static void destruct_CbmTrdParModGeo(void *p) {
      typedef ::CbmTrdParModGeo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParModGeo

//______________________________________________________________________________
void CbmTrdModuleAbstract::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdModuleAbstract.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdModuleAbstract::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdModuleAbstract::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdModuleAbstract(void *p) {
      return  p ? new(p) ::CbmTrdModuleAbstract : new ::CbmTrdModuleAbstract;
   }
   static void *newArray_CbmTrdModuleAbstract(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdModuleAbstract[nElements] : new ::CbmTrdModuleAbstract[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdModuleAbstract(void *p) {
      delete ((::CbmTrdModuleAbstract*)p);
   }
   static void deleteArray_CbmTrdModuleAbstract(void *p) {
      delete [] ((::CbmTrdModuleAbstract*)p);
   }
   static void destruct_CbmTrdModuleAbstract(void *p) {
      typedef ::CbmTrdModuleAbstract current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdModuleAbstract

//______________________________________________________________________________
void CbmMcbm2020TrdTshiftPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2020TrdTshiftPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2020TrdTshiftPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2020TrdTshiftPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2020TrdTshiftPar(void *p) {
      return  p ? new(p) ::CbmMcbm2020TrdTshiftPar : new ::CbmMcbm2020TrdTshiftPar;
   }
   static void *newArray_CbmMcbm2020TrdTshiftPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2020TrdTshiftPar[nElements] : new ::CbmMcbm2020TrdTshiftPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2020TrdTshiftPar(void *p) {
      delete ((::CbmMcbm2020TrdTshiftPar*)p);
   }
   static void deleteArray_CbmMcbm2020TrdTshiftPar(void *p) {
      delete [] ((::CbmMcbm2020TrdTshiftPar*)p);
   }
   static void destruct_CbmMcbm2020TrdTshiftPar(void *p) {
      typedef ::CbmMcbm2020TrdTshiftPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2020TrdTshiftPar

//______________________________________________________________________________
void CbmTrdParSetGas::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParSetGas.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParSetGas::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParSetGas::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParSetGas(void *p) {
      return  p ? new(p) ::CbmTrdParSetGas : new ::CbmTrdParSetGas;
   }
   static void *newArray_CbmTrdParSetGas(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParSetGas[nElements] : new ::CbmTrdParSetGas[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParSetGas(void *p) {
      delete ((::CbmTrdParSetGas*)p);
   }
   static void deleteArray_CbmTrdParSetGas(void *p) {
      delete [] ((::CbmTrdParSetGas*)p);
   }
   static void destruct_CbmTrdParSetGas(void *p) {
      typedef ::CbmTrdParSetGas current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParSetGas

//______________________________________________________________________________
void CbmTrdParSetGain::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParSetGain.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParSetGain::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParSetGain::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParSetGain(void *p) {
      return  p ? new(p) ::CbmTrdParSetGain : new ::CbmTrdParSetGain;
   }
   static void *newArray_CbmTrdParSetGain(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParSetGain[nElements] : new ::CbmTrdParSetGain[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParSetGain(void *p) {
      delete ((::CbmTrdParSetGain*)p);
   }
   static void deleteArray_CbmTrdParSetGain(void *p) {
      delete [] ((::CbmTrdParSetGain*)p);
   }
   static void destruct_CbmTrdParSetGain(void *p) {
      typedef ::CbmTrdParSetGain current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParSetGain

//______________________________________________________________________________
void CbmTrdParSetGeo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParSetGeo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParSetGeo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParSetGeo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParSetGeo(void *p) {
      return  p ? new(p) ::CbmTrdParSetGeo : new ::CbmTrdParSetGeo;
   }
   static void *newArray_CbmTrdParSetGeo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParSetGeo[nElements] : new ::CbmTrdParSetGeo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParSetGeo(void *p) {
      delete ((::CbmTrdParSetGeo*)p);
   }
   static void deleteArray_CbmTrdParSetGeo(void *p) {
      delete [] ((::CbmTrdParSetGeo*)p);
   }
   static void destruct_CbmTrdParSetGeo(void *p) {
      typedef ::CbmTrdParSetGeo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParSetGeo

//______________________________________________________________________________
void CbmTrdParSetDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParSetDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParSetDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParSetDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParSetDigi(void *p) {
      return  p ? new(p) ::CbmTrdParSetDigi : new ::CbmTrdParSetDigi;
   }
   static void *newArray_CbmTrdParSetDigi(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParSetDigi[nElements] : new ::CbmTrdParSetDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParSetDigi(void *p) {
      delete ((::CbmTrdParSetDigi*)p);
   }
   static void deleteArray_CbmTrdParSetDigi(void *p) {
      delete [] ((::CbmTrdParSetDigi*)p);
   }
   static void destruct_CbmTrdParSetDigi(void *p) {
      typedef ::CbmTrdParSetDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParSetDigi

//______________________________________________________________________________
void CbmTrdParAsic::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParAsic.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParAsic::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParAsic::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrdParAsic(void *p) {
      delete ((::CbmTrdParAsic*)p);
   }
   static void deleteArray_CbmTrdParAsic(void *p) {
      delete [] ((::CbmTrdParAsic*)p);
   }
   static void destruct_CbmTrdParAsic(void *p) {
      typedef ::CbmTrdParAsic current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParAsic

//______________________________________________________________________________
void CbmTrdParSpadic::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParSpadic.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParSpadic::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParSpadic::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParSpadic(void *p) {
      return  p ? new(p) ::CbmTrdParSpadic : new ::CbmTrdParSpadic;
   }
   static void *newArray_CbmTrdParSpadic(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParSpadic[nElements] : new ::CbmTrdParSpadic[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParSpadic(void *p) {
      delete ((::CbmTrdParSpadic*)p);
   }
   static void deleteArray_CbmTrdParSpadic(void *p) {
      delete [] ((::CbmTrdParSpadic*)p);
   }
   static void destruct_CbmTrdParSpadic(void *p) {
      typedef ::CbmTrdParSpadic current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParSpadic

//______________________________________________________________________________
void CbmTrddEdxUtils::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrddEdxUtils.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrddEdxUtils::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrddEdxUtils::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrddEdxUtils(void *p) {
      return  p ? new(p) ::CbmTrddEdxUtils : new ::CbmTrddEdxUtils;
   }
   static void *newArray_CbmTrddEdxUtils(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrddEdxUtils[nElements] : new ::CbmTrddEdxUtils[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrddEdxUtils(void *p) {
      delete ((::CbmTrddEdxUtils*)p);
   }
   static void deleteArray_CbmTrddEdxUtils(void *p) {
      delete [] ((::CbmTrddEdxUtils*)p);
   }
   static void destruct_CbmTrddEdxUtils(void *p) {
      typedef ::CbmTrddEdxUtils current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrddEdxUtils

//______________________________________________________________________________
void CbmTrdSpadic::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdSpadic.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdSpadic::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdSpadic::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdSpadic(void *p) {
      return  p ? new(p) ::CbmTrdSpadic : new ::CbmTrdSpadic;
   }
   static void *newArray_CbmTrdSpadic(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdSpadic[nElements] : new ::CbmTrdSpadic[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdSpadic(void *p) {
      delete ((::CbmTrdSpadic*)p);
   }
   static void deleteArray_CbmTrdSpadic(void *p) {
      delete [] ((::CbmTrdSpadic*)p);
   }
   static void destruct_CbmTrdSpadic(void *p) {
      typedef ::CbmTrdSpadic current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdSpadic

//______________________________________________________________________________
void CbmTrdParFaspChannel::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParFaspChannel.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParFaspChannel::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParFaspChannel::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParFaspChannel(void *p) {
      return  p ? new(p) ::CbmTrdParFaspChannel : new ::CbmTrdParFaspChannel;
   }
   static void *newArray_CbmTrdParFaspChannel(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParFaspChannel[nElements] : new ::CbmTrdParFaspChannel[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParFaspChannel(void *p) {
      delete ((::CbmTrdParFaspChannel*)p);
   }
   static void deleteArray_CbmTrdParFaspChannel(void *p) {
      delete [] ((::CbmTrdParFaspChannel*)p);
   }
   static void destruct_CbmTrdParFaspChannel(void *p) {
      typedef ::CbmTrdParFaspChannel current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParFaspChannel

//______________________________________________________________________________
void CbmTrdParFasp::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParFasp.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParFasp::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParFasp::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParFasp(void *p) {
      return  p ? new(p) ::CbmTrdParFasp : new ::CbmTrdParFasp;
   }
   static void *newArray_CbmTrdParFasp(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParFasp[nElements] : new ::CbmTrdParFasp[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParFasp(void *p) {
      delete ((::CbmTrdParFasp*)p);
   }
   static void deleteArray_CbmTrdParFasp(void *p) {
      delete [] ((::CbmTrdParFasp*)p);
   }
   static void destruct_CbmTrdParFasp(void *p) {
      typedef ::CbmTrdParFasp current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParFasp

//______________________________________________________________________________
void CbmTrdFASP::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdFASP.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdFASP::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdFASP::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdFASP(void *p) {
      return  p ? new(p) ::CbmTrdFASP : new ::CbmTrdFASP;
   }
   static void *newArray_CbmTrdFASP(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdFASP[nElements] : new ::CbmTrdFASP[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdFASP(void *p) {
      delete ((::CbmTrdFASP*)p);
   }
   static void deleteArray_CbmTrdFASP(void *p) {
      delete [] ((::CbmTrdFASP*)p);
   }
   static void destruct_CbmTrdFASP(void *p) {
      typedef ::CbmTrdFASP current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdFASP

//______________________________________________________________________________
void CbmTrdParModGas::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParModGas.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParModGas::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParModGas::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParModGas(void *p) {
      return  p ? new(p) ::CbmTrdParModGas : new ::CbmTrdParModGas;
   }
   static void *newArray_CbmTrdParModGas(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParModGas[nElements] : new ::CbmTrdParModGas[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParModGas(void *p) {
      delete ((::CbmTrdParModGas*)p);
   }
   static void deleteArray_CbmTrdParModGas(void *p) {
      delete [] ((::CbmTrdParModGas*)p);
   }
   static void destruct_CbmTrdParModGas(void *p) {
      typedef ::CbmTrdParModGas current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParModGas

//______________________________________________________________________________
void CbmTrdParModGain::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdParModGain.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdParModGain::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdParModGain::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdParModGain(void *p) {
      return  p ? new(p) ::CbmTrdParModGain : new ::CbmTrdParModGain;
   }
   static void *newArray_CbmTrdParModGain(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdParModGain[nElements] : new ::CbmTrdParModGain[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdParModGain(void *p) {
      delete ((::CbmTrdParModGain*)p);
   }
   static void deleteArray_CbmTrdParModGain(void *p) {
      delete [] ((::CbmTrdParModGain*)p);
   }
   static void destruct_CbmTrdParModGain(void *p) {
      typedef ::CbmTrdParModGain current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdParModGain

//______________________________________________________________________________
void CbmTrdTrackingInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdTrackingInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdTrackingInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdTrackingInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdTrackingInterface(void *p) {
      return  p ? new(p) ::CbmTrdTrackingInterface : new ::CbmTrdTrackingInterface;
   }
   static void *newArray_CbmTrdTrackingInterface(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdTrackingInterface[nElements] : new ::CbmTrdTrackingInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdTrackingInterface(void *p) {
      delete ((::CbmTrdTrackingInterface*)p);
   }
   static void deleteArray_CbmTrdTrackingInterface(void *p) {
      delete [] ((::CbmTrdTrackingInterface*)p);
   }
   static void destruct_CbmTrdTrackingInterface(void *p) {
      typedef ::CbmTrdTrackingInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdTrackingInterface

//______________________________________________________________________________
void CbmTrdGeoHandler::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdGeoHandler.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdGeoHandler::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdGeoHandler::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdGeoHandler(void *p) {
      return  p ? new(p) ::CbmTrdGeoHandler : new ::CbmTrdGeoHandler;
   }
   static void *newArray_CbmTrdGeoHandler(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdGeoHandler[nElements] : new ::CbmTrdGeoHandler[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdGeoHandler(void *p) {
      delete ((::CbmTrdGeoHandler*)p);
   }
   static void deleteArray_CbmTrdGeoHandler(void *p) {
      delete [] ((::CbmTrdGeoHandler*)p);
   }
   static void destruct_CbmTrdGeoHandler(void *p) {
      typedef ::CbmTrdGeoHandler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdGeoHandler

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdUtils(void *p) {
      return  p ? new(p) ::CbmTrdUtils : new ::CbmTrdUtils;
   }
   static void *newArray_CbmTrdUtils(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdUtils[nElements] : new ::CbmTrdUtils[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdUtils(void *p) {
      delete ((::CbmTrdUtils*)p);
   }
   static void deleteArray_CbmTrdUtils(void *p) {
      delete [] ((::CbmTrdUtils*)p);
   }
   static void destruct_CbmTrdUtils(void *p) {
      typedef ::CbmTrdUtils current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdUtils

//______________________________________________________________________________
void CbmTrdRadiator::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdRadiator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdRadiator::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdRadiator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdRadiator(void *p) {
      return  p ? new(p) ::CbmTrdRadiator : new ::CbmTrdRadiator;
   }
   static void *newArray_CbmTrdRadiator(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdRadiator[nElements] : new ::CbmTrdRadiator[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdRadiator(void *p) {
      delete ((::CbmTrdRadiator*)p);
   }
   static void deleteArray_CbmTrdRadiator(void *p) {
      delete [] ((::CbmTrdRadiator*)p);
   }
   static void destruct_CbmTrdRadiator(void *p) {
      typedef ::CbmTrdRadiator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdRadiator

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
   static TClass *vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR_Dictionary();
   static void vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR(Long_t size, void *p);
   static void delete_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR(void *p);
   static void deleteArray_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR(void *p);
   static void destruct_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<tuple<unsigned int,unsigned int,unsigned int,bool> >*)
   {
      vector<tuple<unsigned int,unsigned int,unsigned int,bool> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<tuple<unsigned int,unsigned int,unsigned int,bool> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<tuple<unsigned int,unsigned int,unsigned int,bool> >", -2, "vector", 389,
                  typeid(vector<tuple<unsigned int,unsigned int,unsigned int,bool> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<tuple<unsigned int,unsigned int,unsigned int,bool> >) );
      instance.SetNew(&new_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR);
      instance.SetNewArray(&newArray_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR);
      instance.SetDelete(&delete_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR);
      instance.SetDestructor(&destruct_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<tuple<unsigned int,unsigned int,unsigned int,bool> > >()));

      ::ROOT::AddClassAlternate("vector<tuple<unsigned int,unsigned int,unsigned int,bool> >","std::vector<std::tuple<unsigned int, unsigned int, unsigned int, bool>, std::allocator<std::tuple<unsigned int, unsigned int, unsigned int, bool> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<tuple<unsigned int,unsigned int,unsigned int,bool> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<tuple<unsigned int,unsigned int,unsigned int,bool> >*)nullptr)->GetClass();
      vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tuple<unsigned int,unsigned int,unsigned int,bool> > : new vector<tuple<unsigned int,unsigned int,unsigned int,bool> >;
   }
   static void *newArray_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<tuple<unsigned int,unsigned int,unsigned int,bool> >[nElements] : new vector<tuple<unsigned int,unsigned int,unsigned int,bool> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR(void *p) {
      delete ((vector<tuple<unsigned int,unsigned int,unsigned int,bool> >*)p);
   }
   static void deleteArray_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR(void *p) {
      delete [] ((vector<tuple<unsigned int,unsigned int,unsigned int,bool> >*)p);
   }
   static void destruct_vectorlEtuplelEunsignedsPintcOunsignedsPintcOunsignedsPintcOboolgRsPgR(void *p) {
      typedef vector<tuple<unsigned int,unsigned int,unsigned int,bool> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<tuple<unsigned int,unsigned int,unsigned int,bool> >

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
   static TClass *vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR_Dictionary();
   static void vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p);
   static void deleteArray_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p);
   static void destruct_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<CbmTrdDigi*,CbmMatch*> >*)
   {
      vector<pair<CbmTrdDigi*,CbmMatch*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<CbmTrdDigi*,CbmMatch*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<CbmTrdDigi*,CbmMatch*> >", -2, "vector", 389,
                  typeid(vector<pair<CbmTrdDigi*,CbmMatch*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<CbmTrdDigi*,CbmMatch*> >) );
      instance.SetNew(&new_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<CbmTrdDigi*,CbmMatch*> > >()));

      ::ROOT::AddClassAlternate("vector<pair<CbmTrdDigi*,CbmMatch*> >","std::vector<std::pair<CbmTrdDigi*, CbmMatch*>, std::allocator<std::pair<CbmTrdDigi*, CbmMatch*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pair<CbmTrdDigi*,CbmMatch*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<CbmTrdDigi*,CbmMatch*> >*)nullptr)->GetClass();
      vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<CbmTrdDigi*,CbmMatch*> > : new vector<pair<CbmTrdDigi*,CbmMatch*> >;
   }
   static void *newArray_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<CbmTrdDigi*,CbmMatch*> >[nElements] : new vector<pair<CbmTrdDigi*,CbmMatch*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p) {
      delete ((vector<pair<CbmTrdDigi*,CbmMatch*> >*)p);
   }
   static void deleteArray_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p) {
      delete [] ((vector<pair<CbmTrdDigi*,CbmMatch*> >*)p);
   }
   static void destruct_vectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p) {
      typedef vector<pair<CbmTrdDigi*,CbmMatch*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<CbmTrdDigi*,CbmMatch*> >

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
   static TClass *maplEintcOunsignedsPlonggR_Dictionary();
   static void maplEintcOunsignedsPlonggR_TClassManip(TClass*);
   static void *new_maplEintcOunsignedsPlonggR(void *p = nullptr);
   static void *newArray_maplEintcOunsignedsPlonggR(Long_t size, void *p);
   static void delete_maplEintcOunsignedsPlonggR(void *p);
   static void deleteArray_maplEintcOunsignedsPlonggR(void *p);
   static void destruct_maplEintcOunsignedsPlonggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,unsigned long>*)
   {
      map<int,unsigned long> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,unsigned long>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,unsigned long>", -2, "map", 100,
                  typeid(map<int,unsigned long>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOunsignedsPlonggR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,unsigned long>) );
      instance.SetNew(&new_maplEintcOunsignedsPlonggR);
      instance.SetNewArray(&newArray_maplEintcOunsignedsPlonggR);
      instance.SetDelete(&delete_maplEintcOunsignedsPlonggR);
      instance.SetDeleteArray(&deleteArray_maplEintcOunsignedsPlonggR);
      instance.SetDestructor(&destruct_maplEintcOunsignedsPlonggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,unsigned long> >()));

      ::ROOT::AddClassAlternate("map<int,unsigned long>","std::map<int, unsigned long, std::less<int>, std::allocator<std::pair<int const, unsigned long> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,unsigned long>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOunsignedsPlonggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,unsigned long>*)nullptr)->GetClass();
      maplEintcOunsignedsPlonggR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOunsignedsPlonggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOunsignedsPlonggR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,unsigned long> : new map<int,unsigned long>;
   }
   static void *newArray_maplEintcOunsignedsPlonggR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,unsigned long>[nElements] : new map<int,unsigned long>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOunsignedsPlonggR(void *p) {
      delete ((map<int,unsigned long>*)p);
   }
   static void deleteArray_maplEintcOunsignedsPlonggR(void *p) {
      delete [] ((map<int,unsigned long>*)p);
   }
   static void destruct_maplEintcOunsignedsPlonggR(void *p) {
      typedef map<int,unsigned long> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,unsigned long>

namespace ROOT {
   static TClass *maplEintcOarraylEintcO16gRsPgR_Dictionary();
   static void maplEintcOarraylEintcO16gRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOarraylEintcO16gRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOarraylEintcO16gRsPgR(Long_t size, void *p);
   static void delete_maplEintcOarraylEintcO16gRsPgR(void *p);
   static void deleteArray_maplEintcOarraylEintcO16gRsPgR(void *p);
   static void destruct_maplEintcOarraylEintcO16gRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,array<int,16> >*)
   {
      map<int,array<int,16> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,array<int,16> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,array<int,16> >", -2, "map", 100,
                  typeid(map<int,array<int,16> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOarraylEintcO16gRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(map<int,array<int,16> >) );
      instance.SetNew(&new_maplEintcOarraylEintcO16gRsPgR);
      instance.SetNewArray(&newArray_maplEintcOarraylEintcO16gRsPgR);
      instance.SetDelete(&delete_maplEintcOarraylEintcO16gRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOarraylEintcO16gRsPgR);
      instance.SetDestructor(&destruct_maplEintcOarraylEintcO16gRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,array<int,16> > >()));

      ::ROOT::AddClassAlternate("map<int,array<int,16> >","std::map<int, std::array<int, 16ul>, std::less<int>, std::allocator<std::pair<int const, std::array<int, 16ul> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,array<int,16> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOarraylEintcO16gRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,array<int,16> >*)nullptr)->GetClass();
      maplEintcOarraylEintcO16gRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOarraylEintcO16gRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOarraylEintcO16gRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,array<int,16> > : new map<int,array<int,16> >;
   }
   static void *newArray_maplEintcOarraylEintcO16gRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,array<int,16> >[nElements] : new map<int,array<int,16> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOarraylEintcO16gRsPgR(void *p) {
      delete ((map<int,array<int,16> >*)p);
   }
   static void deleteArray_maplEintcOarraylEintcO16gRsPgR(void *p) {
      delete [] ((map<int,array<int,16> >*)p);
   }
   static void destruct_maplEintcOarraylEintcO16gRsPgR(void *p) {
      typedef map<int,array<int,16> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,array<int,16> >

namespace ROOT {
   static TClass *maplEintcOCbmTrdParSetAsicmUgR_Dictionary();
   static void maplEintcOCbmTrdParSetAsicmUgR_TClassManip(TClass*);
   static void *new_maplEintcOCbmTrdParSetAsicmUgR(void *p = nullptr);
   static void *newArray_maplEintcOCbmTrdParSetAsicmUgR(Long_t size, void *p);
   static void delete_maplEintcOCbmTrdParSetAsicmUgR(void *p);
   static void deleteArray_maplEintcOCbmTrdParSetAsicmUgR(void *p);
   static void destruct_maplEintcOCbmTrdParSetAsicmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,CbmTrdParSetAsic*>*)
   {
      map<int,CbmTrdParSetAsic*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,CbmTrdParSetAsic*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,CbmTrdParSetAsic*>", -2, "map", 100,
                  typeid(map<int,CbmTrdParSetAsic*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOCbmTrdParSetAsicmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,CbmTrdParSetAsic*>) );
      instance.SetNew(&new_maplEintcOCbmTrdParSetAsicmUgR);
      instance.SetNewArray(&newArray_maplEintcOCbmTrdParSetAsicmUgR);
      instance.SetDelete(&delete_maplEintcOCbmTrdParSetAsicmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOCbmTrdParSetAsicmUgR);
      instance.SetDestructor(&destruct_maplEintcOCbmTrdParSetAsicmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,CbmTrdParSetAsic*> >()));

      ::ROOT::AddClassAlternate("map<int,CbmTrdParSetAsic*>","std::map<int, CbmTrdParSetAsic*, std::less<int>, std::allocator<std::pair<int const, CbmTrdParSetAsic*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,CbmTrdParSetAsic*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOCbmTrdParSetAsicmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,CbmTrdParSetAsic*>*)nullptr)->GetClass();
      maplEintcOCbmTrdParSetAsicmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOCbmTrdParSetAsicmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOCbmTrdParSetAsicmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmTrdParSetAsic*> : new map<int,CbmTrdParSetAsic*>;
   }
   static void *newArray_maplEintcOCbmTrdParSetAsicmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmTrdParSetAsic*>[nElements] : new map<int,CbmTrdParSetAsic*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOCbmTrdParSetAsicmUgR(void *p) {
      delete ((map<int,CbmTrdParSetAsic*>*)p);
   }
   static void deleteArray_maplEintcOCbmTrdParSetAsicmUgR(void *p) {
      delete [] ((map<int,CbmTrdParSetAsic*>*)p);
   }
   static void destruct_maplEintcOCbmTrdParSetAsicmUgR(void *p) {
      typedef map<int,CbmTrdParSetAsic*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,CbmTrdParSetAsic*>

namespace ROOT {
   static TClass *maplEintcOCbmTrdParModmUgR_Dictionary();
   static void maplEintcOCbmTrdParModmUgR_TClassManip(TClass*);
   static void *new_maplEintcOCbmTrdParModmUgR(void *p = nullptr);
   static void *newArray_maplEintcOCbmTrdParModmUgR(Long_t size, void *p);
   static void delete_maplEintcOCbmTrdParModmUgR(void *p);
   static void deleteArray_maplEintcOCbmTrdParModmUgR(void *p);
   static void destruct_maplEintcOCbmTrdParModmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,CbmTrdParMod*>*)
   {
      map<int,CbmTrdParMod*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,CbmTrdParMod*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,CbmTrdParMod*>", -2, "map", 100,
                  typeid(map<int,CbmTrdParMod*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOCbmTrdParModmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,CbmTrdParMod*>) );
      instance.SetNew(&new_maplEintcOCbmTrdParModmUgR);
      instance.SetNewArray(&newArray_maplEintcOCbmTrdParModmUgR);
      instance.SetDelete(&delete_maplEintcOCbmTrdParModmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOCbmTrdParModmUgR);
      instance.SetDestructor(&destruct_maplEintcOCbmTrdParModmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,CbmTrdParMod*> >()));

      ::ROOT::AddClassAlternate("map<int,CbmTrdParMod*>","std::map<int, CbmTrdParMod*, std::less<int>, std::allocator<std::pair<int const, CbmTrdParMod*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,CbmTrdParMod*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOCbmTrdParModmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,CbmTrdParMod*>*)nullptr)->GetClass();
      maplEintcOCbmTrdParModmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOCbmTrdParModmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOCbmTrdParModmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmTrdParMod*> : new map<int,CbmTrdParMod*>;
   }
   static void *newArray_maplEintcOCbmTrdParModmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmTrdParMod*>[nElements] : new map<int,CbmTrdParMod*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOCbmTrdParModmUgR(void *p) {
      delete ((map<int,CbmTrdParMod*>*)p);
   }
   static void deleteArray_maplEintcOCbmTrdParModmUgR(void *p) {
      delete [] ((map<int,CbmTrdParMod*>*)p);
   }
   static void destruct_maplEintcOCbmTrdParModmUgR(void *p) {
      typedef map<int,CbmTrdParMod*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,CbmTrdParMod*>

namespace {
  void TriggerDictionaryInitialization_libCbmTrdBase_Impl() {
    static const char* headers[] = {
"CbmTrdGas.h",
"CbmTrdContFact.h",
"CbmTrdParManager.h",
"CbmTrdModuleAbstract.h",
"CbmMcbm2020TrdTshiftPar.h",
"CbmTrdParSet.h",
"CbmTrdParSetAsic.h",
"CbmTrdParSetGas.h",
"CbmTrdParSetGain.h",
"CbmTrdParSetGeo.h",
"CbmTrdParSetDigi.h",
"CbmTrdParMod.h",
"CbmTrdParAsic.h",
"CbmTrdParSpadic.h",
"CbmTrdSpadic.h",
"CbmTrdFASP.h",
"CbmTrdParFasp.h",
"CbmTrdParModDigi.h",
"CbmTrdParModGas.h",
"CbmTrdParModGain.h",
"CbmTrdParModGeo.h",
"CbmTrdTrackingInterface.h",
"CbmTrdGeoHandler.h",
"CbmTrdUtils.h",
"CbmTrddEdxUtils.h",
"CbmTrdHardwareSetupR.h",
"CbmTrdRadiator.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd",
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
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/core/detectors/trd/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmTrdBase dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmTrdGas.h")))  CbmTrdGas;
class __attribute__((annotate(R"ATTRDUMP(Factory for all TRD parameter containers)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdContFact.h")))  CbmTrdContFact;
class __attribute__((annotate("$clingAutoload$CbmTrdParSet.h")))  __attribute__((annotate("$clingAutoload$CbmTrdParManager.h")))  CbmTrdParSet;
class __attribute__((annotate(R"ATTRDUMP(The set of ASIC settings for all TRD modules)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(The set of ASIC settings for all TRD modules)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(The set of ASIC settings for all TRD modules)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(The set of ASIC settings for all TRD modules)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdParSetAsic.h")))  __attribute__((annotate("$clingAutoload$CbmTrdParManager.h")))  CbmTrdParSetAsic;
class __attribute__((annotate(R"ATTRDUMP(Definition of actual hardware setup)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdHardwareSetupR.h")))  __attribute__((annotate("$clingAutoload$CbmTrdParManager.h")))  CbmTrdHardwareSetupR;
class __attribute__((annotate("$clingAutoload$CbmTrdParManager.h")))  CbmTrdParManager;
class __attribute__((annotate(R"ATTRDUMP(Definition of generic parameters for one TRD module)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdParMod.h")))  __attribute__((annotate("$clingAutoload$CbmTrdModuleAbstract.h")))  CbmTrdParMod;
class __attribute__((annotate(R"ATTRDUMP(Definition of read-out parameters for one TRD module)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdParModDigi.h")))  __attribute__((annotate("$clingAutoload$CbmTrdModuleAbstract.h")))  CbmTrdParModDigi;
class __attribute__((annotate(R"ATTRDUMP(Wrapper around the TGeoPhysicalNode containing definition of geometry parameters for one TRD module)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdParModGeo.h")))  __attribute__((annotate("$clingAutoload$CbmTrdModuleAbstract.h")))  CbmTrdParModGeo;
class __attribute__((annotate("$clingAutoload$CbmTrdModuleAbstract.h")))  CbmTrdModuleAbstract;
class __attribute__((annotate("$clingAutoload$CbmMcbm2020TrdTshiftPar.h")))  CbmMcbm2020TrdTshiftPar;
class __attribute__((annotate(R"ATTRDUMP(Container of the chamber parameters for the TRD detector)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdParSetGas.h")))  CbmTrdParSetGas;
class __attribute__((annotate("$clingAutoload$CbmTrdParSetGain.h")))  CbmTrdParSetGain;
class __attribute__((annotate("$clingAutoload$CbmTrdParSetGeo.h")))  CbmTrdParSetGeo;
class __attribute__((annotate(R"ATTRDUMP(Container of the chamber parameters for the TRD detector)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdParSetDigi.h")))  CbmTrdParSetDigi;
class __attribute__((annotate(R"ATTRDUMP(Definition of common ASIC parameters)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdParAsic.h")))  CbmTrdParAsic;
class __attribute__((annotate(R"ATTRDUMP(Definition of SPADIC ASIC parameters)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdParSpadic.h")))  CbmTrdParSpadic;
class __attribute__((annotate("$clingAutoload$CbmTrddEdxUtils.h")))  __attribute__((annotate("$clingAutoload$CbmTrdSpadic.h")))  CbmTrddEdxUtils;
class __attribute__((annotate("$clingAutoload$CbmTrdSpadic.h")))  CbmTrdSpadic;
class __attribute__((annotate(R"ATTRDUMP(Definition of FASP channel calibration container)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdParFasp.h")))  __attribute__((annotate("$clingAutoload$CbmTrdFASP.h")))  CbmTrdParFaspChannel;
class __attribute__((annotate(R"ATTRDUMP(Definition of FASP ASIC parameters)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdParFasp.h")))  __attribute__((annotate("$clingAutoload$CbmTrdFASP.h")))  CbmTrdParFasp;
class __attribute__((annotate(R"ATTRDUMP(FASP ASIC simulator)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdFASP.h")))  CbmTrdFASP;
class __attribute__((annotate(R"ATTRDUMP(Definition of gas parameters for one TRD module)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdParModGas.h")))  CbmTrdParModGas;
class __attribute__((annotate(R"ATTRDUMP(Definition of gain parameters for one TRD module)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdParModGain.h")))  CbmTrdParModGain;
class __attribute__((annotate("$clingAutoload$CbmTrdTrackingInterface.h")))  CbmTrdTrackingInterface;
class __attribute__((annotate("$clingAutoload$CbmTrdGeoHandler.h")))  CbmTrdGeoHandler;
class __attribute__((annotate("$clingAutoload$CbmTrdUtils.h")))  CbmTrdUtils;
class __attribute__((annotate("$clingAutoload$CbmTrdRadiator.h")))  CbmTrdRadiator;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmTrdBase dictionary payload"

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
#include "CbmTrdGas.h"
#include "CbmTrdContFact.h"
#include "CbmTrdParManager.h"
#include "CbmTrdModuleAbstract.h"
#include "CbmMcbm2020TrdTshiftPar.h"
#include "CbmTrdParSet.h"
#include "CbmTrdParSetAsic.h"
#include "CbmTrdParSetGas.h"
#include "CbmTrdParSetGain.h"
#include "CbmTrdParSetGeo.h"
#include "CbmTrdParSetDigi.h"
#include "CbmTrdParMod.h"
#include "CbmTrdParAsic.h"
#include "CbmTrdParSpadic.h"
#include "CbmTrdSpadic.h"
#include "CbmTrdFASP.h"
#include "CbmTrdParFasp.h"
#include "CbmTrdParModDigi.h"
#include "CbmTrdParModGas.h"
#include "CbmTrdParModGain.h"
#include "CbmTrdParModGeo.h"
#include "CbmTrdTrackingInterface.h"
#include "CbmTrdGeoHandler.h"
#include "CbmTrdUtils.h"
#include "CbmTrddEdxUtils.h"
#include "CbmTrdHardwareSetupR.h"
#include "CbmTrdRadiator.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmMcbm2020TrdTshiftPar", payloadCode, "@",
"CbmTrdContFact", payloadCode, "@",
"CbmTrdFASP", payloadCode, "@",
"CbmTrdGas", payloadCode, "@",
"CbmTrdGeoHandler", payloadCode, "@",
"CbmTrdHardwareSetupR", payloadCode, "@",
"CbmTrdModuleAbstract", payloadCode, "@",
"CbmTrdParAsic", payloadCode, "@",
"CbmTrdParFasp", payloadCode, "@",
"CbmTrdParFaspChannel", payloadCode, "@",
"CbmTrdParManager", payloadCode, "@",
"CbmTrdParMod", payloadCode, "@",
"CbmTrdParModDigi", payloadCode, "@",
"CbmTrdParModGain", payloadCode, "@",
"CbmTrdParModGas", payloadCode, "@",
"CbmTrdParModGeo", payloadCode, "@",
"CbmTrdParSet", payloadCode, "@",
"CbmTrdParSetAsic", payloadCode, "@",
"CbmTrdParSetDigi", payloadCode, "@",
"CbmTrdParSetGain", payloadCode, "@",
"CbmTrdParSetGas", payloadCode, "@",
"CbmTrdParSetGeo", payloadCode, "@",
"CbmTrdParSpadic", payloadCode, "@",
"CbmTrdRadiator", payloadCode, "@",
"CbmTrdSpadic", payloadCode, "@",
"CbmTrdTrackingInterface", payloadCode, "@",
"CbmTrdUtils", payloadCode, "@",
"CbmTrddEdxUtils", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmTrdBase",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmTrdBase_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmTrdBase_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmTrdBase() {
  TriggerDictionaryInitialization_libCbmTrdBase_Impl();
}
