// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmTrdSim
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
#include "CbmTrd.h"
#include "CbmTrdDigitizer.h"
#include "CbmTrdModuleSim.h"
#include "CbmTrdModuleSimR.h"
#include "CbmTrdModuleSim2D.h"
#include "CbmTrdTrianglePRF.h"
#include "CbmTrdRawToDigiR.h"
#include "CbmTrdCheckUtil.h"
#include "CbmTrdDigitizerPRFQa.h"
#include "CbmTrdMCQa.h"
#include "CbmTrdHitRateFastQa.h"
#include "CbmTrdHitRateQa.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmTrd(void *p = nullptr);
   static void *newArray_CbmTrd(Long_t size, void *p);
   static void delete_CbmTrd(void *p);
   static void deleteArray_CbmTrd(void *p);
   static void destruct_CbmTrd(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrd*)
   {
      ::CbmTrd *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrd >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrd", ::CbmTrd::Class_Version(), "CbmTrd.h", 34,
                  typeid(::CbmTrd), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrd::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrd) );
      instance.SetNew(&new_CbmTrd);
      instance.SetNewArray(&newArray_CbmTrd);
      instance.SetDelete(&delete_CbmTrd);
      instance.SetDeleteArray(&deleteArray_CbmTrd);
      instance.SetDestructor(&destruct_CbmTrd);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrd*)
   {
      return GenerateInitInstanceLocal((::CbmTrd*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrd*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigitizelECbmTrdDigigR_Dictionary();
   static void CbmDigitizelECbmTrdDigigR_TClassManip(TClass*);
   static void delete_CbmDigitizelECbmTrdDigigR(void *p);
   static void deleteArray_CbmDigitizelECbmTrdDigigR(void *p);
   static void destruct_CbmDigitizelECbmTrdDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigitize<CbmTrdDigi>*)
   {
      ::CbmDigitize<CbmTrdDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigitize<CbmTrdDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigitize<CbmTrdDigi>", ::CbmDigitize<CbmTrdDigi>::Class_Version(), "CbmDigitize.h", 44,
                  typeid(::CbmDigitize<CbmTrdDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigitizelECbmTrdDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigitize<CbmTrdDigi>) );
      instance.SetDelete(&delete_CbmDigitizelECbmTrdDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigitizelECbmTrdDigigR);
      instance.SetDestructor(&destruct_CbmDigitizelECbmTrdDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigitize<CbmTrdDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigitize<CbmTrdDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigitize<CbmTrdDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigitizelECbmTrdDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmTrdDigi>*)nullptr)->GetClass();
      CbmDigitizelECbmTrdDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigitizelECbmTrdDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdCheckUtil(void *p = nullptr);
   static void *newArray_CbmTrdCheckUtil(Long_t size, void *p);
   static void delete_CbmTrdCheckUtil(void *p);
   static void deleteArray_CbmTrdCheckUtil(void *p);
   static void destruct_CbmTrdCheckUtil(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdCheckUtil*)
   {
      ::CbmTrdCheckUtil *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdCheckUtil >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdCheckUtil", ::CbmTrdCheckUtil::Class_Version(), "CbmTrdCheckUtil.h", 25,
                  typeid(::CbmTrdCheckUtil), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdCheckUtil::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdCheckUtil) );
      instance.SetNew(&new_CbmTrdCheckUtil);
      instance.SetNewArray(&newArray_CbmTrdCheckUtil);
      instance.SetDelete(&delete_CbmTrdCheckUtil);
      instance.SetDeleteArray(&deleteArray_CbmTrdCheckUtil);
      instance.SetDestructor(&destruct_CbmTrdCheckUtil);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdCheckUtil*)
   {
      return GenerateInitInstanceLocal((::CbmTrdCheckUtil*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdCheckUtil*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdRawToDigiR(void *p = nullptr);
   static void *newArray_CbmTrdRawToDigiR(Long_t size, void *p);
   static void delete_CbmTrdRawToDigiR(void *p);
   static void deleteArray_CbmTrdRawToDigiR(void *p);
   static void destruct_CbmTrdRawToDigiR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdRawToDigiR*)
   {
      ::CbmTrdRawToDigiR *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdRawToDigiR >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdRawToDigiR", ::CbmTrdRawToDigiR::Class_Version(), "CbmTrdRawToDigiR.h", 22,
                  typeid(::CbmTrdRawToDigiR), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdRawToDigiR::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdRawToDigiR) );
      instance.SetNew(&new_CbmTrdRawToDigiR);
      instance.SetNewArray(&newArray_CbmTrdRawToDigiR);
      instance.SetDelete(&delete_CbmTrdRawToDigiR);
      instance.SetDeleteArray(&deleteArray_CbmTrdRawToDigiR);
      instance.SetDestructor(&destruct_CbmTrdRawToDigiR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdRawToDigiR*)
   {
      return GenerateInitInstanceLocal((::CbmTrdRawToDigiR*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdRawToDigiR*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdDigitizer(void *p = nullptr);
   static void *newArray_CbmTrdDigitizer(Long_t size, void *p);
   static void delete_CbmTrdDigitizer(void *p);
   static void deleteArray_CbmTrdDigitizer(void *p);
   static void destruct_CbmTrdDigitizer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdDigitizer*)
   {
      ::CbmTrdDigitizer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdDigitizer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdDigitizer", ::CbmTrdDigitizer::Class_Version(), "CbmTrdDigitizer.h", 42,
                  typeid(::CbmTrdDigitizer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdDigitizer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdDigitizer) );
      instance.SetNew(&new_CbmTrdDigitizer);
      instance.SetNewArray(&newArray_CbmTrdDigitizer);
      instance.SetDelete(&delete_CbmTrdDigitizer);
      instance.SetDeleteArray(&deleteArray_CbmTrdDigitizer);
      instance.SetDestructor(&destruct_CbmTrdDigitizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdDigitizer*)
   {
      return GenerateInitInstanceLocal((::CbmTrdDigitizer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdDigitizer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrdModuleSim(void *p);
   static void deleteArray_CbmTrdModuleSim(void *p);
   static void destruct_CbmTrdModuleSim(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdModuleSim*)
   {
      ::CbmTrdModuleSim *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdModuleSim >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdModuleSim", ::CbmTrdModuleSim::Class_Version(), "CbmTrdModuleSim.h", 24,
                  typeid(::CbmTrdModuleSim), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdModuleSim::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdModuleSim) );
      instance.SetDelete(&delete_CbmTrdModuleSim);
      instance.SetDeleteArray(&deleteArray_CbmTrdModuleSim);
      instance.SetDestructor(&destruct_CbmTrdModuleSim);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdModuleSim*)
   {
      return GenerateInitInstanceLocal((::CbmTrdModuleSim*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdModuleSim*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrdModuleSimR(void *p);
   static void deleteArray_CbmTrdModuleSimR(void *p);
   static void destruct_CbmTrdModuleSimR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdModuleSimR*)
   {
      ::CbmTrdModuleSimR *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdModuleSimR >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdModuleSimR", ::CbmTrdModuleSimR::Class_Version(), "CbmTrdModuleSimR.h", 23,
                  typeid(::CbmTrdModuleSimR), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdModuleSimR::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdModuleSimR) );
      instance.SetDelete(&delete_CbmTrdModuleSimR);
      instance.SetDeleteArray(&deleteArray_CbmTrdModuleSimR);
      instance.SetDestructor(&destruct_CbmTrdModuleSimR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdModuleSimR*)
   {
      return GenerateInitInstanceLocal((::CbmTrdModuleSimR*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdModuleSimR*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrdModuleSim2D(void *p);
   static void deleteArray_CbmTrdModuleSim2D(void *p);
   static void destruct_CbmTrdModuleSim2D(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdModuleSim2D*)
   {
      ::CbmTrdModuleSim2D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdModuleSim2D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdModuleSim2D", ::CbmTrdModuleSim2D::Class_Version(), "CbmTrdModuleSim2D.h", 25,
                  typeid(::CbmTrdModuleSim2D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdModuleSim2D::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdModuleSim2D) );
      instance.SetDelete(&delete_CbmTrdModuleSim2D);
      instance.SetDeleteArray(&deleteArray_CbmTrdModuleSim2D);
      instance.SetDestructor(&destruct_CbmTrdModuleSim2D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdModuleSim2D*)
   {
      return GenerateInitInstanceLocal((::CbmTrdModuleSim2D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdModuleSim2D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrdTrianglePRF(void *p);
   static void deleteArray_CbmTrdTrianglePRF(void *p);
   static void destruct_CbmTrdTrianglePRF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdTrianglePRF*)
   {
      ::CbmTrdTrianglePRF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdTrianglePRF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdTrianglePRF", ::CbmTrdTrianglePRF::Class_Version(), "CbmTrdTrianglePRF.h", 31,
                  typeid(::CbmTrdTrianglePRF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdTrianglePRF::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdTrianglePRF) );
      instance.SetDelete(&delete_CbmTrdTrianglePRF);
      instance.SetDeleteArray(&deleteArray_CbmTrdTrianglePRF);
      instance.SetDestructor(&destruct_CbmTrdTrianglePRF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdTrianglePRF*)
   {
      return GenerateInitInstanceLocal((::CbmTrdTrianglePRF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdTrianglePRF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdDigitizerPRFQa(void *p = nullptr);
   static void *newArray_CbmTrdDigitizerPRFQa(Long_t size, void *p);
   static void delete_CbmTrdDigitizerPRFQa(void *p);
   static void deleteArray_CbmTrdDigitizerPRFQa(void *p);
   static void destruct_CbmTrdDigitizerPRFQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdDigitizerPRFQa*)
   {
      ::CbmTrdDigitizerPRFQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdDigitizerPRFQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdDigitizerPRFQa", ::CbmTrdDigitizerPRFQa::Class_Version(), "CbmTrdDigitizerPRFQa.h", 14,
                  typeid(::CbmTrdDigitizerPRFQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdDigitizerPRFQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdDigitizerPRFQa) );
      instance.SetNew(&new_CbmTrdDigitizerPRFQa);
      instance.SetNewArray(&newArray_CbmTrdDigitizerPRFQa);
      instance.SetDelete(&delete_CbmTrdDigitizerPRFQa);
      instance.SetDeleteArray(&deleteArray_CbmTrdDigitizerPRFQa);
      instance.SetDestructor(&destruct_CbmTrdDigitizerPRFQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdDigitizerPRFQa*)
   {
      return GenerateInitInstanceLocal((::CbmTrdDigitizerPRFQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdDigitizerPRFQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdMCQa(void *p = nullptr);
   static void *newArray_CbmTrdMCQa(Long_t size, void *p);
   static void delete_CbmTrdMCQa(void *p);
   static void deleteArray_CbmTrdMCQa(void *p);
   static void destruct_CbmTrdMCQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdMCQa*)
   {
      ::CbmTrdMCQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdMCQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdMCQa", ::CbmTrdMCQa::Class_Version(), "CbmTrdMCQa.h", 15,
                  typeid(::CbmTrdMCQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdMCQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdMCQa) );
      instance.SetNew(&new_CbmTrdMCQa);
      instance.SetNewArray(&newArray_CbmTrdMCQa);
      instance.SetDelete(&delete_CbmTrdMCQa);
      instance.SetDeleteArray(&deleteArray_CbmTrdMCQa);
      instance.SetDestructor(&destruct_CbmTrdMCQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdMCQa*)
   {
      return GenerateInitInstanceLocal((::CbmTrdMCQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdMCQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdHitRateFastQa(void *p = nullptr);
   static void *newArray_CbmTrdHitRateFastQa(Long_t size, void *p);
   static void delete_CbmTrdHitRateFastQa(void *p);
   static void deleteArray_CbmTrdHitRateFastQa(void *p);
   static void destruct_CbmTrdHitRateFastQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdHitRateFastQa*)
   {
      ::CbmTrdHitRateFastQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdHitRateFastQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdHitRateFastQa", ::CbmTrdHitRateFastQa::Class_Version(), "CbmTrdHitRateFastQa.h", 59,
                  typeid(::CbmTrdHitRateFastQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdHitRateFastQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdHitRateFastQa) );
      instance.SetNew(&new_CbmTrdHitRateFastQa);
      instance.SetNewArray(&newArray_CbmTrdHitRateFastQa);
      instance.SetDelete(&delete_CbmTrdHitRateFastQa);
      instance.SetDeleteArray(&deleteArray_CbmTrdHitRateFastQa);
      instance.SetDestructor(&destruct_CbmTrdHitRateFastQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdHitRateFastQa*)
   {
      return GenerateInitInstanceLocal((::CbmTrdHitRateFastQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdHitRateFastQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdHitRateQa(void *p = nullptr);
   static void *newArray_CbmTrdHitRateQa(Long_t size, void *p);
   static void delete_CbmTrdHitRateQa(void *p);
   static void deleteArray_CbmTrdHitRateQa(void *p);
   static void destruct_CbmTrdHitRateQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdHitRateQa*)
   {
      ::CbmTrdHitRateQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdHitRateQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdHitRateQa", ::CbmTrdHitRateQa::Class_Version(), "CbmTrdHitRateQa.h", 58,
                  typeid(::CbmTrdHitRateQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdHitRateQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdHitRateQa) );
      instance.SetNew(&new_CbmTrdHitRateQa);
      instance.SetNewArray(&newArray_CbmTrdHitRateQa);
      instance.SetDelete(&delete_CbmTrdHitRateQa);
      instance.SetDeleteArray(&deleteArray_CbmTrdHitRateQa);
      instance.SetDestructor(&destruct_CbmTrdHitRateQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdHitRateQa*)
   {
      return GenerateInitInstanceLocal((::CbmTrdHitRateQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdHitRateQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmTrd::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrd::Class_Name()
{
   return "CbmTrd";
}

//______________________________________________________________________________
const char *CbmTrd::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrd*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrd::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrd*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrd::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrd*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrd::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrd*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigitize<CbmTrdDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmTrdDigi>::Class_Name()
{
   return "CbmDigitize<CbmTrdDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmTrdDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmTrdDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigitize<CbmTrdDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmTrdDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmTrdDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmTrdDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmTrdDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmTrdDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdCheckUtil::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdCheckUtil::Class_Name()
{
   return "CbmTrdCheckUtil";
}

//______________________________________________________________________________
const char *CbmTrdCheckUtil::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdCheckUtil*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdCheckUtil::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdCheckUtil*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdCheckUtil::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdCheckUtil*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdCheckUtil::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdCheckUtil*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdRawToDigiR::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdRawToDigiR::Class_Name()
{
   return "CbmTrdRawToDigiR";
}

//______________________________________________________________________________
const char *CbmTrdRawToDigiR::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiR*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdRawToDigiR::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiR*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdRawToDigiR::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiR*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdRawToDigiR::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawToDigiR*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdDigitizer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdDigitizer::Class_Name()
{
   return "CbmTrdDigitizer";
}

//______________________________________________________________________________
const char *CbmTrdDigitizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigitizer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdDigitizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigitizer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdDigitizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigitizer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdDigitizer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigitizer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdModuleSim::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdModuleSim::Class_Name()
{
   return "CbmTrdModuleSim";
}

//______________________________________________________________________________
const char *CbmTrdModuleSim::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleSim*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdModuleSim::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleSim*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdModuleSim::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleSim*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdModuleSim::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleSim*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdModuleSimR::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdModuleSimR::Class_Name()
{
   return "CbmTrdModuleSimR";
}

//______________________________________________________________________________
const char *CbmTrdModuleSimR::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleSimR*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdModuleSimR::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleSimR*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdModuleSimR::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleSimR*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdModuleSimR::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleSimR*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdModuleSim2D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdModuleSim2D::Class_Name()
{
   return "CbmTrdModuleSim2D";
}

//______________________________________________________________________________
const char *CbmTrdModuleSim2D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleSim2D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdModuleSim2D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleSim2D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdModuleSim2D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleSim2D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdModuleSim2D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdModuleSim2D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdTrianglePRF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdTrianglePRF::Class_Name()
{
   return "CbmTrdTrianglePRF";
}

//______________________________________________________________________________
const char *CbmTrdTrianglePRF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrianglePRF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdTrianglePRF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrianglePRF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdTrianglePRF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrianglePRF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdTrianglePRF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrianglePRF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdDigitizerPRFQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdDigitizerPRFQa::Class_Name()
{
   return "CbmTrdDigitizerPRFQa";
}

//______________________________________________________________________________
const char *CbmTrdDigitizerPRFQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigitizerPRFQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdDigitizerPRFQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigitizerPRFQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdDigitizerPRFQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigitizerPRFQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdDigitizerPRFQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigitizerPRFQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdMCQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdMCQa::Class_Name()
{
   return "CbmTrdMCQa";
}

//______________________________________________________________________________
const char *CbmTrdMCQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdMCQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdMCQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdMCQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdMCQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdMCQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdMCQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdMCQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdHitRateFastQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdHitRateFastQa::Class_Name()
{
   return "CbmTrdHitRateFastQa";
}

//______________________________________________________________________________
const char *CbmTrdHitRateFastQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitRateFastQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdHitRateFastQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitRateFastQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdHitRateFastQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitRateFastQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdHitRateFastQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitRateFastQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdHitRateQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdHitRateQa::Class_Name()
{
   return "CbmTrdHitRateQa";
}

//______________________________________________________________________________
const char *CbmTrdHitRateQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitRateQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdHitRateQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitRateQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdHitRateQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitRateQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdHitRateQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHitRateQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmTrd::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrd.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrd::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrd::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrd(void *p) {
      return  p ? new(p) ::CbmTrd : new ::CbmTrd;
   }
   static void *newArray_CbmTrd(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrd[nElements] : new ::CbmTrd[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrd(void *p) {
      delete ((::CbmTrd*)p);
   }
   static void deleteArray_CbmTrd(void *p) {
      delete [] ((::CbmTrd*)p);
   }
   static void destruct_CbmTrd(void *p) {
      typedef ::CbmTrd current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrd

//______________________________________________________________________________
template <> void CbmDigitize<CbmTrdDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigitize<CbmTrdDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigitize<CbmTrdDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigitize<CbmTrdDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmDigitizelECbmTrdDigigR(void *p) {
      delete ((::CbmDigitize<CbmTrdDigi>*)p);
   }
   static void deleteArray_CbmDigitizelECbmTrdDigigR(void *p) {
      delete [] ((::CbmDigitize<CbmTrdDigi>*)p);
   }
   static void destruct_CbmDigitizelECbmTrdDigigR(void *p) {
      typedef ::CbmDigitize<CbmTrdDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigitize<CbmTrdDigi>

//______________________________________________________________________________
void CbmTrdCheckUtil::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdCheckUtil.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdCheckUtil::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdCheckUtil::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdCheckUtil(void *p) {
      return  p ? new(p) ::CbmTrdCheckUtil : new ::CbmTrdCheckUtil;
   }
   static void *newArray_CbmTrdCheckUtil(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdCheckUtil[nElements] : new ::CbmTrdCheckUtil[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdCheckUtil(void *p) {
      delete ((::CbmTrdCheckUtil*)p);
   }
   static void deleteArray_CbmTrdCheckUtil(void *p) {
      delete [] ((::CbmTrdCheckUtil*)p);
   }
   static void destruct_CbmTrdCheckUtil(void *p) {
      typedef ::CbmTrdCheckUtil current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdCheckUtil

//______________________________________________________________________________
void CbmTrdRawToDigiR::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdRawToDigiR.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdRawToDigiR::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdRawToDigiR::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdRawToDigiR(void *p) {
      return  p ? new(p) ::CbmTrdRawToDigiR : new ::CbmTrdRawToDigiR;
   }
   static void *newArray_CbmTrdRawToDigiR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdRawToDigiR[nElements] : new ::CbmTrdRawToDigiR[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdRawToDigiR(void *p) {
      delete ((::CbmTrdRawToDigiR*)p);
   }
   static void deleteArray_CbmTrdRawToDigiR(void *p) {
      delete [] ((::CbmTrdRawToDigiR*)p);
   }
   static void destruct_CbmTrdRawToDigiR(void *p) {
      typedef ::CbmTrdRawToDigiR current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdRawToDigiR

//______________________________________________________________________________
void CbmTrdDigitizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdDigitizer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdDigitizer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdDigitizer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdDigitizer(void *p) {
      return  p ? new(p) ::CbmTrdDigitizer : new ::CbmTrdDigitizer;
   }
   static void *newArray_CbmTrdDigitizer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdDigitizer[nElements] : new ::CbmTrdDigitizer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdDigitizer(void *p) {
      delete ((::CbmTrdDigitizer*)p);
   }
   static void deleteArray_CbmTrdDigitizer(void *p) {
      delete [] ((::CbmTrdDigitizer*)p);
   }
   static void destruct_CbmTrdDigitizer(void *p) {
      typedef ::CbmTrdDigitizer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdDigitizer

//______________________________________________________________________________
void CbmTrdModuleSim::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdModuleSim.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdModuleSim::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdModuleSim::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrdModuleSim(void *p) {
      delete ((::CbmTrdModuleSim*)p);
   }
   static void deleteArray_CbmTrdModuleSim(void *p) {
      delete [] ((::CbmTrdModuleSim*)p);
   }
   static void destruct_CbmTrdModuleSim(void *p) {
      typedef ::CbmTrdModuleSim current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdModuleSim

//______________________________________________________________________________
void CbmTrdModuleSimR::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdModuleSimR.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdModuleSimR::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdModuleSimR::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrdModuleSimR(void *p) {
      delete ((::CbmTrdModuleSimR*)p);
   }
   static void deleteArray_CbmTrdModuleSimR(void *p) {
      delete [] ((::CbmTrdModuleSimR*)p);
   }
   static void destruct_CbmTrdModuleSimR(void *p) {
      typedef ::CbmTrdModuleSimR current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdModuleSimR

//______________________________________________________________________________
void CbmTrdModuleSim2D::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdModuleSim2D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdModuleSim2D::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdModuleSim2D::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrdModuleSim2D(void *p) {
      delete ((::CbmTrdModuleSim2D*)p);
   }
   static void deleteArray_CbmTrdModuleSim2D(void *p) {
      delete [] ((::CbmTrdModuleSim2D*)p);
   }
   static void destruct_CbmTrdModuleSim2D(void *p) {
      typedef ::CbmTrdModuleSim2D current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdModuleSim2D

//______________________________________________________________________________
void CbmTrdTrianglePRF::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdTrianglePRF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdTrianglePRF::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdTrianglePRF::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrdTrianglePRF(void *p) {
      delete ((::CbmTrdTrianglePRF*)p);
   }
   static void deleteArray_CbmTrdTrianglePRF(void *p) {
      delete [] ((::CbmTrdTrianglePRF*)p);
   }
   static void destruct_CbmTrdTrianglePRF(void *p) {
      typedef ::CbmTrdTrianglePRF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdTrianglePRF

//______________________________________________________________________________
void CbmTrdDigitizerPRFQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdDigitizerPRFQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdDigitizerPRFQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdDigitizerPRFQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdDigitizerPRFQa(void *p) {
      return  p ? new(p) ::CbmTrdDigitizerPRFQa : new ::CbmTrdDigitizerPRFQa;
   }
   static void *newArray_CbmTrdDigitizerPRFQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdDigitizerPRFQa[nElements] : new ::CbmTrdDigitizerPRFQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdDigitizerPRFQa(void *p) {
      delete ((::CbmTrdDigitizerPRFQa*)p);
   }
   static void deleteArray_CbmTrdDigitizerPRFQa(void *p) {
      delete [] ((::CbmTrdDigitizerPRFQa*)p);
   }
   static void destruct_CbmTrdDigitizerPRFQa(void *p) {
      typedef ::CbmTrdDigitizerPRFQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdDigitizerPRFQa

//______________________________________________________________________________
void CbmTrdMCQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdMCQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdMCQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdMCQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdMCQa(void *p) {
      return  p ? new(p) ::CbmTrdMCQa : new ::CbmTrdMCQa;
   }
   static void *newArray_CbmTrdMCQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdMCQa[nElements] : new ::CbmTrdMCQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdMCQa(void *p) {
      delete ((::CbmTrdMCQa*)p);
   }
   static void deleteArray_CbmTrdMCQa(void *p) {
      delete [] ((::CbmTrdMCQa*)p);
   }
   static void destruct_CbmTrdMCQa(void *p) {
      typedef ::CbmTrdMCQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdMCQa

//______________________________________________________________________________
void CbmTrdHitRateFastQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdHitRateFastQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdHitRateFastQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdHitRateFastQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdHitRateFastQa(void *p) {
      return  p ? new(p) ::CbmTrdHitRateFastQa : new ::CbmTrdHitRateFastQa;
   }
   static void *newArray_CbmTrdHitRateFastQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdHitRateFastQa[nElements] : new ::CbmTrdHitRateFastQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdHitRateFastQa(void *p) {
      delete ((::CbmTrdHitRateFastQa*)p);
   }
   static void deleteArray_CbmTrdHitRateFastQa(void *p) {
      delete [] ((::CbmTrdHitRateFastQa*)p);
   }
   static void destruct_CbmTrdHitRateFastQa(void *p) {
      typedef ::CbmTrdHitRateFastQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdHitRateFastQa

//______________________________________________________________________________
void CbmTrdHitRateQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdHitRateQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdHitRateQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdHitRateQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdHitRateQa(void *p) {
      return  p ? new(p) ::CbmTrdHitRateQa : new ::CbmTrdHitRateQa;
   }
   static void *newArray_CbmTrdHitRateQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdHitRateQa[nElements] : new ::CbmTrdHitRateQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdHitRateQa(void *p) {
      delete ((::CbmTrdHitRateQa*)p);
   }
   static void deleteArray_CbmTrdHitRateQa(void *p) {
      delete [] ((::CbmTrdHitRateQa*)p);
   }
   static void destruct_CbmTrdHitRateQa(void *p) {
      typedef ::CbmTrdHitRateQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdHitRateQa

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
   static TClass *vectorlEmaplETStringcOintgRsPgR_Dictionary();
   static void vectorlEmaplETStringcOintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEmaplETStringcOintgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEmaplETStringcOintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEmaplETStringcOintgRsPgR(void *p);
   static void deleteArray_vectorlEmaplETStringcOintgRsPgR(void *p);
   static void destruct_vectorlEmaplETStringcOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<map<TString,int> >*)
   {
      vector<map<TString,int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<map<TString,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<map<TString,int> >", -2, "vector", 389,
                  typeid(vector<map<TString,int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEmaplETStringcOintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<map<TString,int> >) );
      instance.SetNew(&new_vectorlEmaplETStringcOintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEmaplETStringcOintgRsPgR);
      instance.SetDelete(&delete_vectorlEmaplETStringcOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEmaplETStringcOintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEmaplETStringcOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<map<TString,int> > >()));

      ::ROOT::AddClassAlternate("vector<map<TString,int> >","std::vector<std::map<TString, int, std::less<TString>, std::allocator<std::pair<TString const, int> > >, std::allocator<std::map<TString, int, std::less<TString>, std::allocator<std::pair<TString const, int> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<map<TString,int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmaplETStringcOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<map<TString,int> >*)nullptr)->GetClass();
      vectorlEmaplETStringcOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmaplETStringcOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmaplETStringcOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<map<TString,int> > : new vector<map<TString,int> >;
   }
   static void *newArray_vectorlEmaplETStringcOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<map<TString,int> >[nElements] : new vector<map<TString,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmaplETStringcOintgRsPgR(void *p) {
      delete ((vector<map<TString,int> >*)p);
   }
   static void deleteArray_vectorlEmaplETStringcOintgRsPgR(void *p) {
      delete [] ((vector<map<TString,int> >*)p);
   }
   static void destruct_vectorlEmaplETStringcOintgRsPgR(void *p) {
      typedef vector<map<TString,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<map<TString,int> >

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
   static TClass *vectorlEchargR_Dictionary();
   static void vectorlEchargR_TClassManip(TClass*);
   static void *new_vectorlEchargR(void *p = nullptr);
   static void *newArray_vectorlEchargR(Long_t size, void *p);
   static void delete_vectorlEchargR(void *p);
   static void deleteArray_vectorlEchargR(void *p);
   static void destruct_vectorlEchargR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<char>*)
   {
      vector<char> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<char>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<char>", -2, "vector", 389,
                  typeid(vector<char>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEchargR_Dictionary, isa_proxy, 0,
                  sizeof(vector<char>) );
      instance.SetNew(&new_vectorlEchargR);
      instance.SetNewArray(&newArray_vectorlEchargR);
      instance.SetDelete(&delete_vectorlEchargR);
      instance.SetDeleteArray(&deleteArray_vectorlEchargR);
      instance.SetDestructor(&destruct_vectorlEchargR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<char> >()));

      ::ROOT::AddClassAlternate("vector<char>","std::vector<char, std::allocator<char> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<char>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEchargR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<char>*)nullptr)->GetClass();
      vectorlEchargR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEchargR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEchargR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<char> : new vector<char>;
   }
   static void *newArray_vectorlEchargR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<char>[nElements] : new vector<char>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEchargR(void *p) {
      delete ((vector<char>*)p);
   }
   static void deleteArray_vectorlEchargR(void *p) {
      delete [] ((vector<char>*)p);
   }
   static void destruct_vectorlEchargR(void *p) {
      typedef vector<char> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<char>

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
   static TClass *maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR_Dictionary();
   static void maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR_TClassManip(TClass*);
   static void *new_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR(void *p = nullptr);
   static void *newArray_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR(Long_t size, void *p);
   static void delete_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR(void *p);
   static void deleteArray_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR(void *p);
   static void destruct_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<pair<int,pair<int,int> >,CbmTrdDigi*>*)
   {
      map<pair<int,pair<int,int> >,CbmTrdDigi*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<pair<int,pair<int,int> >,CbmTrdDigi*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<pair<int,pair<int,int> >,CbmTrdDigi*>", -2, "map", 100,
                  typeid(map<pair<int,pair<int,int> >,CbmTrdDigi*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<pair<int,pair<int,int> >,CbmTrdDigi*>) );
      instance.SetNew(&new_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR);
      instance.SetNewArray(&newArray_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR);
      instance.SetDelete(&delete_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR);
      instance.SetDeleteArray(&deleteArray_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR);
      instance.SetDestructor(&destruct_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<pair<int,pair<int,int> >,CbmTrdDigi*> >()));

      ::ROOT::AddClassAlternate("map<pair<int,pair<int,int> >,CbmTrdDigi*>","std::map<std::pair<int, std::pair<int, int> >, CbmTrdDigi*, std::less<std::pair<int, std::pair<int, int> > >, std::allocator<std::pair<std::pair<int, std::pair<int, int> > const, CbmTrdDigi*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<pair<int,pair<int,int> >,CbmTrdDigi*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<pair<int,pair<int,int> >,CbmTrdDigi*>*)nullptr)->GetClass();
      maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,pair<int,int> >,CbmTrdDigi*> : new map<pair<int,pair<int,int> >,CbmTrdDigi*>;
   }
   static void *newArray_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,pair<int,int> >,CbmTrdDigi*>[nElements] : new map<pair<int,pair<int,int> >,CbmTrdDigi*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR(void *p) {
      delete ((map<pair<int,pair<int,int> >,CbmTrdDigi*>*)p);
   }
   static void deleteArray_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR(void *p) {
      delete [] ((map<pair<int,pair<int,int> >,CbmTrdDigi*>*)p);
   }
   static void destruct_maplEpairlEintcOpairlEintcOintgRsPgRcOCbmTrdDigimUgR(void *p) {
      typedef map<pair<int,pair<int,int> >,CbmTrdDigi*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<pair<int,pair<int,int> >,CbmTrdDigi*>

namespace ROOT {
   static TClass *maplEintcOvectorlEvectorlEintgRsPgRsPgR_Dictionary();
   static void maplEintcOvectorlEvectorlEintgRsPgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOvectorlEvectorlEintgRsPgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOvectorlEvectorlEintgRsPgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOvectorlEvectorlEintgRsPgRsPgR(void *p);
   static void deleteArray_maplEintcOvectorlEvectorlEintgRsPgRsPgR(void *p);
   static void destruct_maplEintcOvectorlEvectorlEintgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,vector<vector<int> > >*)
   {
      map<int,vector<vector<int> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,vector<vector<int> > >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,vector<vector<int> > >", -2, "map", 100,
                  typeid(map<int,vector<vector<int> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOvectorlEvectorlEintgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,vector<vector<int> > >) );
      instance.SetNew(&new_maplEintcOvectorlEvectorlEintgRsPgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOvectorlEvectorlEintgRsPgRsPgR);
      instance.SetDelete(&delete_maplEintcOvectorlEvectorlEintgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOvectorlEvectorlEintgRsPgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOvectorlEvectorlEintgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,vector<vector<int> > > >()));

      ::ROOT::AddClassAlternate("map<int,vector<vector<int> > >","std::map<int, std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,vector<vector<int> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOvectorlEvectorlEintgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,vector<vector<int> > >*)nullptr)->GetClass();
      maplEintcOvectorlEvectorlEintgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOvectorlEvectorlEintgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOvectorlEvectorlEintgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<vector<int> > > : new map<int,vector<vector<int> > >;
   }
   static void *newArray_maplEintcOvectorlEvectorlEintgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<vector<int> > >[nElements] : new map<int,vector<vector<int> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOvectorlEvectorlEintgRsPgRsPgR(void *p) {
      delete ((map<int,vector<vector<int> > >*)p);
   }
   static void deleteArray_maplEintcOvectorlEvectorlEintgRsPgRsPgR(void *p) {
      delete [] ((map<int,vector<vector<int> > >*)p);
   }
   static void destruct_maplEintcOvectorlEvectorlEintgRsPgRsPgR(void *p) {
      typedef map<int,vector<vector<int> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,vector<vector<int> > >

namespace ROOT {
   static TClass *maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR_Dictionary();
   static void maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR(void *p);
   static void deleteArray_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR(void *p);
   static void destruct_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > >*)
   {
      map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > >", -2, "map", 100,
                  typeid(map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > >) );
      instance.SetNew(&new_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR);
      instance.SetDelete(&delete_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > > >()));

      ::ROOT::AddClassAlternate("map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > >","std::map<int, std::vector<std::pair<CbmTrdDigi*, CbmMatch*>, std::allocator<std::pair<CbmTrdDigi*, CbmMatch*> > >, std::less<int>, std::allocator<std::pair<int const, std::vector<std::pair<CbmTrdDigi*, CbmMatch*>, std::allocator<std::pair<CbmTrdDigi*, CbmMatch*> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > >*)nullptr)->GetClass();
      maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > > : new map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > >;
   }
   static void *newArray_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > >[nElements] : new map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR(void *p) {
      delete ((map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > >*)p);
   }
   static void deleteArray_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR(void *p) {
      delete [] ((map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > >*)p);
   }
   static void destruct_maplEintcOvectorlEpairlECbmTrdDigimUcOCbmMatchmUgRsPgRsPgR(void *p) {
      typedef map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,vector<pair<CbmTrdDigi*,CbmMatch*> > >

namespace ROOT {
   static TClass *maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR_Dictionary();
   static void maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR(void *p);
   static void deleteArray_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR(void *p);
   static void destruct_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,vector<map<TString,int> > >*)
   {
      map<int,vector<map<TString,int> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,vector<map<TString,int> > >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,vector<map<TString,int> > >", -2, "map", 100,
                  typeid(map<int,vector<map<TString,int> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,vector<map<TString,int> > >) );
      instance.SetNew(&new_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR);
      instance.SetDelete(&delete_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,vector<map<TString,int> > > >()));

      ::ROOT::AddClassAlternate("map<int,vector<map<TString,int> > >","std::map<int, std::vector<std::map<TString, int, std::less<TString>, std::allocator<std::pair<TString const, int> > >, std::allocator<std::map<TString, int, std::less<TString>, std::allocator<std::pair<TString const, int> > > > >, std::less<int>, std::allocator<std::pair<int const, std::vector<std::map<TString, int, std::less<TString>, std::allocator<std::pair<TString const, int> > >, std::allocator<std::map<TString, int, std::less<TString>, std::allocator<std::pair<TString const, int> > > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,vector<map<TString,int> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,vector<map<TString,int> > >*)nullptr)->GetClass();
      maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<map<TString,int> > > : new map<int,vector<map<TString,int> > >;
   }
   static void *newArray_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<map<TString,int> > >[nElements] : new map<int,vector<map<TString,int> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR(void *p) {
      delete ((map<int,vector<map<TString,int> > >*)p);
   }
   static void deleteArray_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR(void *p) {
      delete [] ((map<int,vector<map<TString,int> > >*)p);
   }
   static void destruct_maplEintcOvectorlEmaplETStringcOintgRsPgRsPgR(void *p) {
      typedef map<int,vector<map<TString,int> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,vector<map<TString,int> > >

namespace ROOT {
   static TClass *maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR_Dictionary();
   static void maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR(void *p);
   static void deleteArray_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR(void *p);
   static void destruct_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,pair<vector<double>,CbmMatch*> >*)
   {
      map<int,pair<vector<double>,CbmMatch*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,pair<vector<double>,CbmMatch*> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,pair<vector<double>,CbmMatch*> >", -2, "map", 100,
                  typeid(map<int,pair<vector<double>,CbmMatch*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,pair<vector<double>,CbmMatch*> >) );
      instance.SetNew(&new_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR);
      instance.SetDelete(&delete_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,pair<vector<double>,CbmMatch*> > >()));

      ::ROOT::AddClassAlternate("map<int,pair<vector<double>,CbmMatch*> >","std::map<int, std::pair<std::vector<double, std::allocator<double> >, CbmMatch*>, std::less<int>, std::allocator<std::pair<int const, std::pair<std::vector<double, std::allocator<double> >, CbmMatch*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,pair<vector<double>,CbmMatch*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,pair<vector<double>,CbmMatch*> >*)nullptr)->GetClass();
      maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,pair<vector<double>,CbmMatch*> > : new map<int,pair<vector<double>,CbmMatch*> >;
   }
   static void *newArray_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,pair<vector<double>,CbmMatch*> >[nElements] : new map<int,pair<vector<double>,CbmMatch*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR(void *p) {
      delete ((map<int,pair<vector<double>,CbmMatch*> >*)p);
   }
   static void deleteArray_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR(void *p) {
      delete [] ((map<int,pair<vector<double>,CbmMatch*> >*)p);
   }
   static void destruct_maplEintcOpairlEvectorlEdoublegRcOCbmMatchmUgRsPgR(void *p) {
      typedef map<int,pair<vector<double>,CbmMatch*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,pair<vector<double>,CbmMatch*> >

namespace ROOT {
   static TClass *maplEintcOpairlEdoublecOintgRsPgR_Dictionary();
   static void maplEintcOpairlEdoublecOintgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOpairlEdoublecOintgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOpairlEdoublecOintgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOpairlEdoublecOintgRsPgR(void *p);
   static void deleteArray_maplEintcOpairlEdoublecOintgRsPgR(void *p);
   static void destruct_maplEintcOpairlEdoublecOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,pair<double,int> >*)
   {
      map<int,pair<double,int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,pair<double,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,pair<double,int> >", -2, "map", 100,
                  typeid(map<int,pair<double,int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOpairlEdoublecOintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,pair<double,int> >) );
      instance.SetNew(&new_maplEintcOpairlEdoublecOintgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOpairlEdoublecOintgRsPgR);
      instance.SetDelete(&delete_maplEintcOpairlEdoublecOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOpairlEdoublecOintgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOpairlEdoublecOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,pair<double,int> > >()));

      ::ROOT::AddClassAlternate("map<int,pair<double,int> >","std::map<int, std::pair<double, int>, std::less<int>, std::allocator<std::pair<int const, std::pair<double, int> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,pair<double,int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOpairlEdoublecOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,pair<double,int> >*)nullptr)->GetClass();
      maplEintcOpairlEdoublecOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOpairlEdoublecOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOpairlEdoublecOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,pair<double,int> > : new map<int,pair<double,int> >;
   }
   static void *newArray_maplEintcOpairlEdoublecOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,pair<double,int> >[nElements] : new map<int,pair<double,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOpairlEdoublecOintgRsPgR(void *p) {
      delete ((map<int,pair<double,int> >*)p);
   }
   static void deleteArray_maplEintcOpairlEdoublecOintgRsPgR(void *p) {
      delete [] ((map<int,pair<double,int> >*)p);
   }
   static void destruct_maplEintcOpairlEdoublecOintgRsPgR(void *p) {
      typedef map<int,pair<double,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,pair<double,int> >

namespace ROOT {
   static TClass *maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR_Dictionary();
   static void maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p);
   static void deleteArray_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p);
   static void destruct_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,pair<CbmTrdDigi*,CbmMatch*> >*)
   {
      map<int,pair<CbmTrdDigi*,CbmMatch*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,pair<CbmTrdDigi*,CbmMatch*> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,pair<CbmTrdDigi*,CbmMatch*> >", -2, "map", 100,
                  typeid(map<int,pair<CbmTrdDigi*,CbmMatch*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,pair<CbmTrdDigi*,CbmMatch*> >) );
      instance.SetNew(&new_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR);
      instance.SetDelete(&delete_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,pair<CbmTrdDigi*,CbmMatch*> > >()));

      ::ROOT::AddClassAlternate("map<int,pair<CbmTrdDigi*,CbmMatch*> >","std::map<int, std::pair<CbmTrdDigi*, CbmMatch*>, std::less<int>, std::allocator<std::pair<int const, std::pair<CbmTrdDigi*, CbmMatch*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,pair<CbmTrdDigi*,CbmMatch*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,pair<CbmTrdDigi*,CbmMatch*> >*)nullptr)->GetClass();
      maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,pair<CbmTrdDigi*,CbmMatch*> > : new map<int,pair<CbmTrdDigi*,CbmMatch*> >;
   }
   static void *newArray_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,pair<CbmTrdDigi*,CbmMatch*> >[nElements] : new map<int,pair<CbmTrdDigi*,CbmMatch*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p) {
      delete ((map<int,pair<CbmTrdDigi*,CbmMatch*> >*)p);
   }
   static void deleteArray_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p) {
      delete [] ((map<int,pair<CbmTrdDigi*,CbmMatch*> >*)p);
   }
   static void destruct_maplEintcOpairlECbmTrdDigimUcOCbmMatchmUgRsPgR(void *p) {
      typedef map<int,pair<CbmTrdDigi*,CbmMatch*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,pair<CbmTrdDigi*,CbmMatch*> >

namespace ROOT {
   static TClass *maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR_Dictionary();
   static void maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR(void *p);
   static void deleteArray_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR(void *p);
   static void destruct_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,map<int,map<int,int> > >*)
   {
      map<int,map<int,map<int,int> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,map<int,map<int,int> > >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,map<int,map<int,int> > >", -2, "map", 100,
                  typeid(map<int,map<int,map<int,int> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,map<int,map<int,int> > >) );
      instance.SetNew(&new_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR);
      instance.SetDelete(&delete_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,map<int,map<int,int> > > >()));

      ::ROOT::AddClassAlternate("map<int,map<int,map<int,int> > >","std::map<int, std::map<int, std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > >, std::less<int>, std::allocator<std::pair<int const, std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > > > > >, std::less<int>, std::allocator<std::pair<int const, std::map<int, std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > >, std::less<int>, std::allocator<std::pair<int const, std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > > > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,map<int,map<int,int> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,map<int,map<int,int> > >*)nullptr)->GetClass();
      maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,map<int,map<int,int> > > : new map<int,map<int,map<int,int> > >;
   }
   static void *newArray_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,map<int,map<int,int> > >[nElements] : new map<int,map<int,map<int,int> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR(void *p) {
      delete ((map<int,map<int,map<int,int> > >*)p);
   }
   static void deleteArray_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR(void *p) {
      delete [] ((map<int,map<int,map<int,int> > >*)p);
   }
   static void destruct_maplEintcOmaplEintcOmaplEintcOintgRsPgRsPgR(void *p) {
      typedef map<int,map<int,map<int,int> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,map<int,map<int,int> > >

namespace ROOT {
   static TClass *maplEintcOmaplEintcOintgRsPgR_Dictionary();
   static void maplEintcOmaplEintcOintgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOmaplEintcOintgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOmaplEintcOintgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOmaplEintcOintgRsPgR(void *p);
   static void deleteArray_maplEintcOmaplEintcOintgRsPgR(void *p);
   static void destruct_maplEintcOmaplEintcOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,map<int,int> >*)
   {
      map<int,map<int,int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,map<int,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,map<int,int> >", -2, "map", 100,
                  typeid(map<int,map<int,int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOmaplEintcOintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,map<int,int> >) );
      instance.SetNew(&new_maplEintcOmaplEintcOintgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOmaplEintcOintgRsPgR);
      instance.SetDelete(&delete_maplEintcOmaplEintcOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOmaplEintcOintgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOmaplEintcOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,map<int,int> > >()));

      ::ROOT::AddClassAlternate("map<int,map<int,int> >","std::map<int, std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > >, std::less<int>, std::allocator<std::pair<int const, std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,map<int,int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOmaplEintcOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,map<int,int> >*)nullptr)->GetClass();
      maplEintcOmaplEintcOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOmaplEintcOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOmaplEintcOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,map<int,int> > : new map<int,map<int,int> >;
   }
   static void *newArray_maplEintcOmaplEintcOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,map<int,int> >[nElements] : new map<int,map<int,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOmaplEintcOintgRsPgR(void *p) {
      delete ((map<int,map<int,int> >*)p);
   }
   static void deleteArray_maplEintcOmaplEintcOintgRsPgR(void *p) {
      delete [] ((map<int,map<int,int> >*)p);
   }
   static void destruct_maplEintcOmaplEintcOintgRsPgR(void *p) {
      typedef map<int,map<int,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,map<int,int> >

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
   static TClass *maplEintcOdoublegR_Dictionary();
   static void maplEintcOdoublegR_TClassManip(TClass*);
   static void *new_maplEintcOdoublegR(void *p = nullptr);
   static void *newArray_maplEintcOdoublegR(Long_t size, void *p);
   static void delete_maplEintcOdoublegR(void *p);
   static void deleteArray_maplEintcOdoublegR(void *p);
   static void destruct_maplEintcOdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,double>*)
   {
      map<int,double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,double>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,double>", -2, "map", 100,
                  typeid(map<int,double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,double>) );
      instance.SetNew(&new_maplEintcOdoublegR);
      instance.SetNewArray(&newArray_maplEintcOdoublegR);
      instance.SetDelete(&delete_maplEintcOdoublegR);
      instance.SetDeleteArray(&deleteArray_maplEintcOdoublegR);
      instance.SetDestructor(&destruct_maplEintcOdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,double> >()));

      ::ROOT::AddClassAlternate("map<int,double>","std::map<int, double, std::less<int>, std::allocator<std::pair<int const, double> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,double>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,double>*)nullptr)->GetClass();
      maplEintcOdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,double> : new map<int,double>;
   }
   static void *newArray_maplEintcOdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,double>[nElements] : new map<int,double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOdoublegR(void *p) {
      delete ((map<int,double>*)p);
   }
   static void deleteArray_maplEintcOdoublegR(void *p) {
      delete [] ((map<int,double>*)p);
   }
   static void destruct_maplEintcOdoublegR(void *p) {
      typedef map<int,double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,double>

namespace ROOT {
   static TClass *maplEintcOCbmTrdModuleSimmUgR_Dictionary();
   static void maplEintcOCbmTrdModuleSimmUgR_TClassManip(TClass*);
   static void *new_maplEintcOCbmTrdModuleSimmUgR(void *p = nullptr);
   static void *newArray_maplEintcOCbmTrdModuleSimmUgR(Long_t size, void *p);
   static void delete_maplEintcOCbmTrdModuleSimmUgR(void *p);
   static void deleteArray_maplEintcOCbmTrdModuleSimmUgR(void *p);
   static void destruct_maplEintcOCbmTrdModuleSimmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,CbmTrdModuleSim*>*)
   {
      map<int,CbmTrdModuleSim*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,CbmTrdModuleSim*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,CbmTrdModuleSim*>", -2, "map", 100,
                  typeid(map<int,CbmTrdModuleSim*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOCbmTrdModuleSimmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,CbmTrdModuleSim*>) );
      instance.SetNew(&new_maplEintcOCbmTrdModuleSimmUgR);
      instance.SetNewArray(&newArray_maplEintcOCbmTrdModuleSimmUgR);
      instance.SetDelete(&delete_maplEintcOCbmTrdModuleSimmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOCbmTrdModuleSimmUgR);
      instance.SetDestructor(&destruct_maplEintcOCbmTrdModuleSimmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,CbmTrdModuleSim*> >()));

      ::ROOT::AddClassAlternate("map<int,CbmTrdModuleSim*>","std::map<int, CbmTrdModuleSim*, std::less<int>, std::allocator<std::pair<int const, CbmTrdModuleSim*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,CbmTrdModuleSim*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOCbmTrdModuleSimmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,CbmTrdModuleSim*>*)nullptr)->GetClass();
      maplEintcOCbmTrdModuleSimmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOCbmTrdModuleSimmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOCbmTrdModuleSimmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmTrdModuleSim*> : new map<int,CbmTrdModuleSim*>;
   }
   static void *newArray_maplEintcOCbmTrdModuleSimmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmTrdModuleSim*>[nElements] : new map<int,CbmTrdModuleSim*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOCbmTrdModuleSimmUgR(void *p) {
      delete ((map<int,CbmTrdModuleSim*>*)p);
   }
   static void deleteArray_maplEintcOCbmTrdModuleSimmUgR(void *p) {
      delete [] ((map<int,CbmTrdModuleSim*>*)p);
   }
   static void destruct_maplEintcOCbmTrdModuleSimmUgR(void *p) {
      typedef map<int,CbmTrdModuleSim*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,CbmTrdModuleSim*>

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

namespace ROOT {
   static TClass *maplETStringcOTProfile3DmUgR_Dictionary();
   static void maplETStringcOTProfile3DmUgR_TClassManip(TClass*);
   static void *new_maplETStringcOTProfile3DmUgR(void *p = nullptr);
   static void *newArray_maplETStringcOTProfile3DmUgR(Long_t size, void *p);
   static void delete_maplETStringcOTProfile3DmUgR(void *p);
   static void deleteArray_maplETStringcOTProfile3DmUgR(void *p);
   static void destruct_maplETStringcOTProfile3DmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,TProfile3D*>*)
   {
      map<TString,TProfile3D*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,TProfile3D*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,TProfile3D*>", -2, "map", 100,
                  typeid(map<TString,TProfile3D*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOTProfile3DmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,TProfile3D*>) );
      instance.SetNew(&new_maplETStringcOTProfile3DmUgR);
      instance.SetNewArray(&newArray_maplETStringcOTProfile3DmUgR);
      instance.SetDelete(&delete_maplETStringcOTProfile3DmUgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOTProfile3DmUgR);
      instance.SetDestructor(&destruct_maplETStringcOTProfile3DmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,TProfile3D*> >()));

      ::ROOT::AddClassAlternate("map<TString,TProfile3D*>","std::map<TString, TProfile3D*, std::less<TString>, std::allocator<std::pair<TString const, TProfile3D*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,TProfile3D*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOTProfile3DmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,TProfile3D*>*)nullptr)->GetClass();
      maplETStringcOTProfile3DmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOTProfile3DmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOTProfile3DmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TProfile3D*> : new map<TString,TProfile3D*>;
   }
   static void *newArray_maplETStringcOTProfile3DmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TProfile3D*>[nElements] : new map<TString,TProfile3D*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOTProfile3DmUgR(void *p) {
      delete ((map<TString,TProfile3D*>*)p);
   }
   static void deleteArray_maplETStringcOTProfile3DmUgR(void *p) {
      delete [] ((map<TString,TProfile3D*>*)p);
   }
   static void destruct_maplETStringcOTProfile3DmUgR(void *p) {
      typedef map<TString,TProfile3D*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,TProfile3D*>

namespace ROOT {
   static TClass *maplETStringcOTProfile2DmUgR_Dictionary();
   static void maplETStringcOTProfile2DmUgR_TClassManip(TClass*);
   static void *new_maplETStringcOTProfile2DmUgR(void *p = nullptr);
   static void *newArray_maplETStringcOTProfile2DmUgR(Long_t size, void *p);
   static void delete_maplETStringcOTProfile2DmUgR(void *p);
   static void deleteArray_maplETStringcOTProfile2DmUgR(void *p);
   static void destruct_maplETStringcOTProfile2DmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,TProfile2D*>*)
   {
      map<TString,TProfile2D*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,TProfile2D*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,TProfile2D*>", -2, "map", 100,
                  typeid(map<TString,TProfile2D*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOTProfile2DmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,TProfile2D*>) );
      instance.SetNew(&new_maplETStringcOTProfile2DmUgR);
      instance.SetNewArray(&newArray_maplETStringcOTProfile2DmUgR);
      instance.SetDelete(&delete_maplETStringcOTProfile2DmUgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOTProfile2DmUgR);
      instance.SetDestructor(&destruct_maplETStringcOTProfile2DmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,TProfile2D*> >()));

      ::ROOT::AddClassAlternate("map<TString,TProfile2D*>","std::map<TString, TProfile2D*, std::less<TString>, std::allocator<std::pair<TString const, TProfile2D*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,TProfile2D*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOTProfile2DmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,TProfile2D*>*)nullptr)->GetClass();
      maplETStringcOTProfile2DmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOTProfile2DmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOTProfile2DmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TProfile2D*> : new map<TString,TProfile2D*>;
   }
   static void *newArray_maplETStringcOTProfile2DmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TProfile2D*>[nElements] : new map<TString,TProfile2D*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOTProfile2DmUgR(void *p) {
      delete ((map<TString,TProfile2D*>*)p);
   }
   static void deleteArray_maplETStringcOTProfile2DmUgR(void *p) {
      delete [] ((map<TString,TProfile2D*>*)p);
   }
   static void destruct_maplETStringcOTProfile2DmUgR(void *p) {
      typedef map<TString,TProfile2D*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,TProfile2D*>

namespace ROOT {
   static TClass *maplETStringcOTProfilemUgR_Dictionary();
   static void maplETStringcOTProfilemUgR_TClassManip(TClass*);
   static void *new_maplETStringcOTProfilemUgR(void *p = nullptr);
   static void *newArray_maplETStringcOTProfilemUgR(Long_t size, void *p);
   static void delete_maplETStringcOTProfilemUgR(void *p);
   static void deleteArray_maplETStringcOTProfilemUgR(void *p);
   static void destruct_maplETStringcOTProfilemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,TProfile*>*)
   {
      map<TString,TProfile*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,TProfile*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,TProfile*>", -2, "map", 100,
                  typeid(map<TString,TProfile*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOTProfilemUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,TProfile*>) );
      instance.SetNew(&new_maplETStringcOTProfilemUgR);
      instance.SetNewArray(&newArray_maplETStringcOTProfilemUgR);
      instance.SetDelete(&delete_maplETStringcOTProfilemUgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOTProfilemUgR);
      instance.SetDestructor(&destruct_maplETStringcOTProfilemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,TProfile*> >()));

      ::ROOT::AddClassAlternate("map<TString,TProfile*>","std::map<TString, TProfile*, std::less<TString>, std::allocator<std::pair<TString const, TProfile*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,TProfile*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOTProfilemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,TProfile*>*)nullptr)->GetClass();
      maplETStringcOTProfilemUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOTProfilemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOTProfilemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TProfile*> : new map<TString,TProfile*>;
   }
   static void *newArray_maplETStringcOTProfilemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TProfile*>[nElements] : new map<TString,TProfile*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOTProfilemUgR(void *p) {
      delete ((map<TString,TProfile*>*)p);
   }
   static void deleteArray_maplETStringcOTProfilemUgR(void *p) {
      delete [] ((map<TString,TProfile*>*)p);
   }
   static void destruct_maplETStringcOTProfilemUgR(void *p) {
      typedef map<TString,TProfile*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,TProfile*>

namespace ROOT {
   static TClass *maplETStringcOTH3DmUgR_Dictionary();
   static void maplETStringcOTH3DmUgR_TClassManip(TClass*);
   static void *new_maplETStringcOTH3DmUgR(void *p = nullptr);
   static void *newArray_maplETStringcOTH3DmUgR(Long_t size, void *p);
   static void delete_maplETStringcOTH3DmUgR(void *p);
   static void deleteArray_maplETStringcOTH3DmUgR(void *p);
   static void destruct_maplETStringcOTH3DmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,TH3D*>*)
   {
      map<TString,TH3D*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,TH3D*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,TH3D*>", -2, "map", 100,
                  typeid(map<TString,TH3D*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOTH3DmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,TH3D*>) );
      instance.SetNew(&new_maplETStringcOTH3DmUgR);
      instance.SetNewArray(&newArray_maplETStringcOTH3DmUgR);
      instance.SetDelete(&delete_maplETStringcOTH3DmUgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOTH3DmUgR);
      instance.SetDestructor(&destruct_maplETStringcOTH3DmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,TH3D*> >()));

      ::ROOT::AddClassAlternate("map<TString,TH3D*>","std::map<TString, TH3D*, std::less<TString>, std::allocator<std::pair<TString const, TH3D*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,TH3D*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOTH3DmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,TH3D*>*)nullptr)->GetClass();
      maplETStringcOTH3DmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOTH3DmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOTH3DmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TH3D*> : new map<TString,TH3D*>;
   }
   static void *newArray_maplETStringcOTH3DmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TH3D*>[nElements] : new map<TString,TH3D*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOTH3DmUgR(void *p) {
      delete ((map<TString,TH3D*>*)p);
   }
   static void deleteArray_maplETStringcOTH3DmUgR(void *p) {
      delete [] ((map<TString,TH3D*>*)p);
   }
   static void destruct_maplETStringcOTH3DmUgR(void *p) {
      typedef map<TString,TH3D*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,TH3D*>

namespace ROOT {
   static TClass *maplETStringcOTH2DmUgR_Dictionary();
   static void maplETStringcOTH2DmUgR_TClassManip(TClass*);
   static void *new_maplETStringcOTH2DmUgR(void *p = nullptr);
   static void *newArray_maplETStringcOTH2DmUgR(Long_t size, void *p);
   static void delete_maplETStringcOTH2DmUgR(void *p);
   static void deleteArray_maplETStringcOTH2DmUgR(void *p);
   static void destruct_maplETStringcOTH2DmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,TH2D*>*)
   {
      map<TString,TH2D*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,TH2D*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,TH2D*>", -2, "map", 100,
                  typeid(map<TString,TH2D*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOTH2DmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,TH2D*>) );
      instance.SetNew(&new_maplETStringcOTH2DmUgR);
      instance.SetNewArray(&newArray_maplETStringcOTH2DmUgR);
      instance.SetDelete(&delete_maplETStringcOTH2DmUgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOTH2DmUgR);
      instance.SetDestructor(&destruct_maplETStringcOTH2DmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,TH2D*> >()));

      ::ROOT::AddClassAlternate("map<TString,TH2D*>","std::map<TString, TH2D*, std::less<TString>, std::allocator<std::pair<TString const, TH2D*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,TH2D*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOTH2DmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,TH2D*>*)nullptr)->GetClass();
      maplETStringcOTH2DmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOTH2DmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOTH2DmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TH2D*> : new map<TString,TH2D*>;
   }
   static void *newArray_maplETStringcOTH2DmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TH2D*>[nElements] : new map<TString,TH2D*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOTH2DmUgR(void *p) {
      delete ((map<TString,TH2D*>*)p);
   }
   static void deleteArray_maplETStringcOTH2DmUgR(void *p) {
      delete [] ((map<TString,TH2D*>*)p);
   }
   static void destruct_maplETStringcOTH2DmUgR(void *p) {
      typedef map<TString,TH2D*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,TH2D*>

namespace ROOT {
   static TClass *maplETStringcOTH1DmUgR_Dictionary();
   static void maplETStringcOTH1DmUgR_TClassManip(TClass*);
   static void *new_maplETStringcOTH1DmUgR(void *p = nullptr);
   static void *newArray_maplETStringcOTH1DmUgR(Long_t size, void *p);
   static void delete_maplETStringcOTH1DmUgR(void *p);
   static void deleteArray_maplETStringcOTH1DmUgR(void *p);
   static void destruct_maplETStringcOTH1DmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,TH1D*>*)
   {
      map<TString,TH1D*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,TH1D*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,TH1D*>", -2, "map", 100,
                  typeid(map<TString,TH1D*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOTH1DmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,TH1D*>) );
      instance.SetNew(&new_maplETStringcOTH1DmUgR);
      instance.SetNewArray(&newArray_maplETStringcOTH1DmUgR);
      instance.SetDelete(&delete_maplETStringcOTH1DmUgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOTH1DmUgR);
      instance.SetDestructor(&destruct_maplETStringcOTH1DmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,TH1D*> >()));

      ::ROOT::AddClassAlternate("map<TString,TH1D*>","std::map<TString, TH1D*, std::less<TString>, std::allocator<std::pair<TString const, TH1D*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,TH1D*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOTH1DmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,TH1D*>*)nullptr)->GetClass();
      maplETStringcOTH1DmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOTH1DmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOTH1DmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TH1D*> : new map<TString,TH1D*>;
   }
   static void *newArray_maplETStringcOTH1DmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TH1D*>[nElements] : new map<TString,TH1D*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOTH1DmUgR(void *p) {
      delete ((map<TString,TH1D*>*)p);
   }
   static void deleteArray_maplETStringcOTH1DmUgR(void *p) {
      delete [] ((map<TString,TH1D*>*)p);
   }
   static void destruct_maplETStringcOTH1DmUgR(void *p) {
      typedef map<TString,TH1D*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,TH1D*>

namespace {
  void TriggerDictionaryInitialization_libCbmTrdSim_Impl() {
    static const char* headers[] = {
"CbmTrd.h",
"CbmTrdDigitizer.h",
"CbmTrdModuleSim.h",
"CbmTrdModuleSimR.h",
"CbmTrdModuleSim2D.h",
"CbmTrdTrianglePRF.h",
"CbmTrdRawToDigiR.h",
"CbmTrdCheckUtil.h",
"CbmTrdDigitizerPRFQa.h",
"CbmTrdMCQa.h",
"CbmTrdHitRateFastQa.h",
"CbmTrdHitRateQa.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd/qa",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd/qa",
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
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/trd/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmTrdSim dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmTrd.h")))  CbmTrd;
class __attribute__((annotate("$clingAutoload$CbmTrdDigi.h")))  __attribute__((annotate("$clingAutoload$CbmTrdDigitizer.h")))  CbmTrdDigi;
template <class Digi> class __attribute__((annotate("$clingAutoload$CbmDigitize.h")))  __attribute__((annotate("$clingAutoload$CbmTrdDigitizer.h")))  CbmDigitize;

class __attribute__((annotate("$clingAutoload$CbmTrdCheckUtil.h")))  __attribute__((annotate("$clingAutoload$CbmTrdDigitizer.h")))  CbmTrdCheckUtil;
class __attribute__((annotate("$clingAutoload$CbmTrdRawToDigiR.h")))  __attribute__((annotate("$clingAutoload$CbmTrdDigitizer.h")))  CbmTrdRawToDigiR;
class __attribute__((annotate("$clingAutoload$CbmTrdDigitizer.h")))  CbmTrdDigitizer;
class __attribute__((annotate("$clingAutoload$CbmTrdModuleSim.h")))  CbmTrdModuleSim;
class __attribute__((annotate(R"ATTRDUMP(Simulation module implementation for rectangular pad geometry)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdModuleSimR.h")))  CbmTrdModuleSimR;
class __attribute__((annotate("$clingAutoload$CbmTrdModuleSim2D.h")))  CbmTrdModuleSim2D;
class __attribute__((annotate("$clingAutoload$CbmTrdTrianglePRF.h")))  CbmTrdTrianglePRF;
class __attribute__((annotate("$clingAutoload$CbmTrdDigitizerPRFQa.h")))  CbmTrdDigitizerPRFQa;
class __attribute__((annotate("$clingAutoload$CbmTrdMCQa.h")))  CbmTrdMCQa;
class __attribute__((annotate("$clingAutoload$CbmTrdHitRateFastQa.h")))  CbmTrdHitRateFastQa;
class __attribute__((annotate("$clingAutoload$CbmTrdHitRateQa.h")))  CbmTrdHitRateQa;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmTrdSim dictionary payload"

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
#include "CbmTrd.h"
#include "CbmTrdDigitizer.h"
#include "CbmTrdModuleSim.h"
#include "CbmTrdModuleSimR.h"
#include "CbmTrdModuleSim2D.h"
#include "CbmTrdTrianglePRF.h"
#include "CbmTrdRawToDigiR.h"
#include "CbmTrdCheckUtil.h"
#include "CbmTrdDigitizerPRFQa.h"
#include "CbmTrdMCQa.h"
#include "CbmTrdHitRateFastQa.h"
#include "CbmTrdHitRateQa.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmDigitize<CbmTrdDigi>", payloadCode, "@",
"CbmTrd", payloadCode, "@",
"CbmTrdCheckUtil", payloadCode, "@",
"CbmTrdDigitizer", payloadCode, "@",
"CbmTrdDigitizerPRFQa", payloadCode, "@",
"CbmTrdHitRateFastQa", payloadCode, "@",
"CbmTrdHitRateQa", payloadCode, "@",
"CbmTrdMCQa", payloadCode, "@",
"CbmTrdModuleSim", payloadCode, "@",
"CbmTrdModuleSim2D", payloadCode, "@",
"CbmTrdModuleSimR", payloadCode, "@",
"CbmTrdRawToDigiR", payloadCode, "@",
"CbmTrdTrianglePRF", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmTrdSim",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmTrdSim_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmTrdSim_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmTrdSim() {
  TriggerDictionaryInitialization_libCbmTrdSim_Impl();
}
