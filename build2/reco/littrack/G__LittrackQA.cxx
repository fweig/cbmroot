// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__LittrackQA
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
#include "CbmLitResultChecker.h"
#include "CbmLitCheckEnergyLossMuons.h"
#include "CbmLitCheckBrem.h"
#include "CbmLitMCTrack.h"
#include "CbmLitMCTrackCreator.h"
#include "CbmLitCreateStandaloneData.h"
#include "CbmLitTestMatrixMath.h"
#include "CbmLitTrackingQa.h"
#include "CbmLitTrackingQaStudyReport.h"
#include "CbmLitTrackingQaReport.h"
#include "CbmLitClusteringQa.h"
#include "CbmLitClusteringQaReport.h"
#include "CbmLitClusteringQaStudyReport.h"
#include "CbmLitFitQa.h"
#include "CbmLitFitQaReport.h"
#include "CbmLitFitQaStudyReport.h"
#include "CbmLitFieldQa.h"
#include "CbmLitFieldQaReport.h"
#include "CbmLitFieldApproximationQa.h"
#include "CbmLitFieldApproximationQaReport.h"
#include "CbmLitRadLengthQa.h"
#include "CbmLitRadLengthQaReport.h"
#include "CbmLitRadLengthGenerator.h"
#include "CbmLitTofQa.h"
#include "CbmLitTofQaReport.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmLitCheckEnergyLossMuons(void *p = nullptr);
   static void *newArray_CbmLitCheckEnergyLossMuons(Long_t size, void *p);
   static void delete_CbmLitCheckEnergyLossMuons(void *p);
   static void deleteArray_CbmLitCheckEnergyLossMuons(void *p);
   static void destruct_CbmLitCheckEnergyLossMuons(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitCheckEnergyLossMuons*)
   {
      ::CbmLitCheckEnergyLossMuons *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitCheckEnergyLossMuons >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitCheckEnergyLossMuons", ::CbmLitCheckEnergyLossMuons::Class_Version(), "CbmLitCheckEnergyLossMuons.h", 15,
                  typeid(::CbmLitCheckEnergyLossMuons), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitCheckEnergyLossMuons::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitCheckEnergyLossMuons) );
      instance.SetNew(&new_CbmLitCheckEnergyLossMuons);
      instance.SetNewArray(&newArray_CbmLitCheckEnergyLossMuons);
      instance.SetDelete(&delete_CbmLitCheckEnergyLossMuons);
      instance.SetDeleteArray(&deleteArray_CbmLitCheckEnergyLossMuons);
      instance.SetDestructor(&destruct_CbmLitCheckEnergyLossMuons);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitCheckEnergyLossMuons*)
   {
      return GenerateInitInstanceLocal((::CbmLitCheckEnergyLossMuons*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitCheckEnergyLossMuons*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitCheckBrem(void *p = nullptr);
   static void *newArray_CbmLitCheckBrem(Long_t size, void *p);
   static void delete_CbmLitCheckBrem(void *p);
   static void deleteArray_CbmLitCheckBrem(void *p);
   static void destruct_CbmLitCheckBrem(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitCheckBrem*)
   {
      ::CbmLitCheckBrem *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitCheckBrem >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitCheckBrem", ::CbmLitCheckBrem::Class_Version(), "CbmLitCheckBrem.h", 24,
                  typeid(::CbmLitCheckBrem), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitCheckBrem::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitCheckBrem) );
      instance.SetNew(&new_CbmLitCheckBrem);
      instance.SetNewArray(&newArray_CbmLitCheckBrem);
      instance.SetDelete(&delete_CbmLitCheckBrem);
      instance.SetDeleteArray(&deleteArray_CbmLitCheckBrem);
      instance.SetDestructor(&destruct_CbmLitCheckBrem);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitCheckBrem*)
   {
      return GenerateInitInstanceLocal((::CbmLitCheckBrem*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitCheckBrem*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitCreateStandaloneData(void *p = nullptr);
   static void *newArray_CbmLitCreateStandaloneData(Long_t size, void *p);
   static void delete_CbmLitCreateStandaloneData(void *p);
   static void deleteArray_CbmLitCreateStandaloneData(void *p);
   static void destruct_CbmLitCreateStandaloneData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitCreateStandaloneData*)
   {
      ::CbmLitCreateStandaloneData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitCreateStandaloneData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitCreateStandaloneData", ::CbmLitCreateStandaloneData::Class_Version(), "CbmLitCreateStandaloneData.h", 14,
                  typeid(::CbmLitCreateStandaloneData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitCreateStandaloneData::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitCreateStandaloneData) );
      instance.SetNew(&new_CbmLitCreateStandaloneData);
      instance.SetNewArray(&newArray_CbmLitCreateStandaloneData);
      instance.SetDelete(&delete_CbmLitCreateStandaloneData);
      instance.SetDeleteArray(&deleteArray_CbmLitCreateStandaloneData);
      instance.SetDestructor(&destruct_CbmLitCreateStandaloneData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitCreateStandaloneData*)
   {
      return GenerateInitInstanceLocal((::CbmLitCreateStandaloneData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitCreateStandaloneData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmLitTestMatrixMath_Dictionary();
   static void CbmLitTestMatrixMath_TClassManip(TClass*);
   static void *new_CbmLitTestMatrixMath(void *p = nullptr);
   static void *newArray_CbmLitTestMatrixMath(Long_t size, void *p);
   static void delete_CbmLitTestMatrixMath(void *p);
   static void deleteArray_CbmLitTestMatrixMath(void *p);
   static void destruct_CbmLitTestMatrixMath(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitTestMatrixMath*)
   {
      ::CbmLitTestMatrixMath *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmLitTestMatrixMath));
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitTestMatrixMath", "CbmLitTestMatrixMath.h", 14,
                  typeid(::CbmLitTestMatrixMath), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmLitTestMatrixMath_Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitTestMatrixMath) );
      instance.SetNew(&new_CbmLitTestMatrixMath);
      instance.SetNewArray(&newArray_CbmLitTestMatrixMath);
      instance.SetDelete(&delete_CbmLitTestMatrixMath);
      instance.SetDeleteArray(&deleteArray_CbmLitTestMatrixMath);
      instance.SetDestructor(&destruct_CbmLitTestMatrixMath);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitTestMatrixMath*)
   {
      return GenerateInitInstanceLocal((::CbmLitTestMatrixMath*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitTestMatrixMath*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmLitTestMatrixMath_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmLitTestMatrixMath*)nullptr)->GetClass();
      CbmLitTestMatrixMath_TClassManip(theClass);
   return theClass;
   }

   static void CbmLitTestMatrixMath_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitTrackingQa(void *p = nullptr);
   static void *newArray_CbmLitTrackingQa(Long_t size, void *p);
   static void delete_CbmLitTrackingQa(void *p);
   static void deleteArray_CbmLitTrackingQa(void *p);
   static void destruct_CbmLitTrackingQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitTrackingQa*)
   {
      ::CbmLitTrackingQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitTrackingQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitTrackingQa", ::CbmLitTrackingQa::Class_Version(), "CbmLitTrackingQa.h", 38,
                  typeid(::CbmLitTrackingQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitTrackingQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitTrackingQa) );
      instance.SetNew(&new_CbmLitTrackingQa);
      instance.SetNewArray(&newArray_CbmLitTrackingQa);
      instance.SetDelete(&delete_CbmLitTrackingQa);
      instance.SetDeleteArray(&deleteArray_CbmLitTrackingQa);
      instance.SetDestructor(&destruct_CbmLitTrackingQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitTrackingQa*)
   {
      return GenerateInitInstanceLocal((::CbmLitTrackingQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitTrackingQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitTrackingQaStudyReport(void *p = nullptr);
   static void *newArray_CbmLitTrackingQaStudyReport(Long_t size, void *p);
   static void delete_CbmLitTrackingQaStudyReport(void *p);
   static void deleteArray_CbmLitTrackingQaStudyReport(void *p);
   static void destruct_CbmLitTrackingQaStudyReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitTrackingQaStudyReport*)
   {
      ::CbmLitTrackingQaStudyReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitTrackingQaStudyReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitTrackingQaStudyReport", ::CbmLitTrackingQaStudyReport::Class_Version(), "CbmLitTrackingQaStudyReport.h", 33,
                  typeid(::CbmLitTrackingQaStudyReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitTrackingQaStudyReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitTrackingQaStudyReport) );
      instance.SetNew(&new_CbmLitTrackingQaStudyReport);
      instance.SetNewArray(&newArray_CbmLitTrackingQaStudyReport);
      instance.SetDelete(&delete_CbmLitTrackingQaStudyReport);
      instance.SetDeleteArray(&deleteArray_CbmLitTrackingQaStudyReport);
      instance.SetDestructor(&destruct_CbmLitTrackingQaStudyReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitTrackingQaStudyReport*)
   {
      return GenerateInitInstanceLocal((::CbmLitTrackingQaStudyReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitTrackingQaStudyReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitTrackingQaReport(void *p = nullptr);
   static void *newArray_CbmLitTrackingQaReport(Long_t size, void *p);
   static void delete_CbmLitTrackingQaReport(void *p);
   static void deleteArray_CbmLitTrackingQaReport(void *p);
   static void destruct_CbmLitTrackingQaReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitTrackingQaReport*)
   {
      ::CbmLitTrackingQaReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitTrackingQaReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitTrackingQaReport", ::CbmLitTrackingQaReport::Class_Version(), "CbmLitTrackingQaReport.h", 28,
                  typeid(::CbmLitTrackingQaReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitTrackingQaReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitTrackingQaReport) );
      instance.SetNew(&new_CbmLitTrackingQaReport);
      instance.SetNewArray(&newArray_CbmLitTrackingQaReport);
      instance.SetDelete(&delete_CbmLitTrackingQaReport);
      instance.SetDeleteArray(&deleteArray_CbmLitTrackingQaReport);
      instance.SetDestructor(&destruct_CbmLitTrackingQaReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitTrackingQaReport*)
   {
      return GenerateInitInstanceLocal((::CbmLitTrackingQaReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitTrackingQaReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitClusteringQa(void *p = nullptr);
   static void *newArray_CbmLitClusteringQa(Long_t size, void *p);
   static void delete_CbmLitClusteringQa(void *p);
   static void deleteArray_CbmLitClusteringQa(void *p);
   static void destruct_CbmLitClusteringQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitClusteringQa*)
   {
      ::CbmLitClusteringQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitClusteringQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitClusteringQa", ::CbmLitClusteringQa::Class_Version(), "CbmLitClusteringQa.h", 31,
                  typeid(::CbmLitClusteringQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitClusteringQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitClusteringQa) );
      instance.SetNew(&new_CbmLitClusteringQa);
      instance.SetNewArray(&newArray_CbmLitClusteringQa);
      instance.SetDelete(&delete_CbmLitClusteringQa);
      instance.SetDeleteArray(&deleteArray_CbmLitClusteringQa);
      instance.SetDestructor(&destruct_CbmLitClusteringQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitClusteringQa*)
   {
      return GenerateInitInstanceLocal((::CbmLitClusteringQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitClusteringQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitClusteringQaReport(void *p = nullptr);
   static void *newArray_CbmLitClusteringQaReport(Long_t size, void *p);
   static void delete_CbmLitClusteringQaReport(void *p);
   static void deleteArray_CbmLitClusteringQaReport(void *p);
   static void destruct_CbmLitClusteringQaReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitClusteringQaReport*)
   {
      ::CbmLitClusteringQaReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitClusteringQaReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitClusteringQaReport", ::CbmLitClusteringQaReport::Class_Version(), "CbmLitClusteringQaReport.h", 26,
                  typeid(::CbmLitClusteringQaReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitClusteringQaReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitClusteringQaReport) );
      instance.SetNew(&new_CbmLitClusteringQaReport);
      instance.SetNewArray(&newArray_CbmLitClusteringQaReport);
      instance.SetDelete(&delete_CbmLitClusteringQaReport);
      instance.SetDeleteArray(&deleteArray_CbmLitClusteringQaReport);
      instance.SetDestructor(&destruct_CbmLitClusteringQaReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitClusteringQaReport*)
   {
      return GenerateInitInstanceLocal((::CbmLitClusteringQaReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitClusteringQaReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitClusteringQaStudyReport(void *p = nullptr);
   static void *newArray_CbmLitClusteringQaStudyReport(Long_t size, void *p);
   static void delete_CbmLitClusteringQaStudyReport(void *p);
   static void deleteArray_CbmLitClusteringQaStudyReport(void *p);
   static void destruct_CbmLitClusteringQaStudyReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitClusteringQaStudyReport*)
   {
      ::CbmLitClusteringQaStudyReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitClusteringQaStudyReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitClusteringQaStudyReport", ::CbmLitClusteringQaStudyReport::Class_Version(), "CbmLitClusteringQaStudyReport.h", 26,
                  typeid(::CbmLitClusteringQaStudyReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitClusteringQaStudyReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitClusteringQaStudyReport) );
      instance.SetNew(&new_CbmLitClusteringQaStudyReport);
      instance.SetNewArray(&newArray_CbmLitClusteringQaStudyReport);
      instance.SetDelete(&delete_CbmLitClusteringQaStudyReport);
      instance.SetDeleteArray(&deleteArray_CbmLitClusteringQaStudyReport);
      instance.SetDestructor(&destruct_CbmLitClusteringQaStudyReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitClusteringQaStudyReport*)
   {
      return GenerateInitInstanceLocal((::CbmLitClusteringQaStudyReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitClusteringQaStudyReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitFitQa(void *p = nullptr);
   static void *newArray_CbmLitFitQa(Long_t size, void *p);
   static void delete_CbmLitFitQa(void *p);
   static void deleteArray_CbmLitFitQa(void *p);
   static void destruct_CbmLitFitQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitFitQa*)
   {
      ::CbmLitFitQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitFitQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitFitQa", ::CbmLitFitQa::Class_Version(), "CbmLitFitQa.h", 44,
                  typeid(::CbmLitFitQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitFitQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitFitQa) );
      instance.SetNew(&new_CbmLitFitQa);
      instance.SetNewArray(&newArray_CbmLitFitQa);
      instance.SetDelete(&delete_CbmLitFitQa);
      instance.SetDeleteArray(&deleteArray_CbmLitFitQa);
      instance.SetDestructor(&destruct_CbmLitFitQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitFitQa*)
   {
      return GenerateInitInstanceLocal((::CbmLitFitQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitFitQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitFitQaReport(void *p = nullptr);
   static void *newArray_CbmLitFitQaReport(Long_t size, void *p);
   static void delete_CbmLitFitQaReport(void *p);
   static void deleteArray_CbmLitFitQaReport(void *p);
   static void destruct_CbmLitFitQaReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitFitQaReport*)
   {
      ::CbmLitFitQaReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitFitQaReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitFitQaReport", ::CbmLitFitQaReport::Class_Version(), "CbmLitFitQaReport.h", 26,
                  typeid(::CbmLitFitQaReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitFitQaReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitFitQaReport) );
      instance.SetNew(&new_CbmLitFitQaReport);
      instance.SetNewArray(&newArray_CbmLitFitQaReport);
      instance.SetDelete(&delete_CbmLitFitQaReport);
      instance.SetDeleteArray(&deleteArray_CbmLitFitQaReport);
      instance.SetDestructor(&destruct_CbmLitFitQaReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitFitQaReport*)
   {
      return GenerateInitInstanceLocal((::CbmLitFitQaReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitFitQaReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitFitQaStudyReport(void *p = nullptr);
   static void *newArray_CbmLitFitQaStudyReport(Long_t size, void *p);
   static void delete_CbmLitFitQaStudyReport(void *p);
   static void deleteArray_CbmLitFitQaStudyReport(void *p);
   static void destruct_CbmLitFitQaStudyReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitFitQaStudyReport*)
   {
      ::CbmLitFitQaStudyReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitFitQaStudyReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitFitQaStudyReport", ::CbmLitFitQaStudyReport::Class_Version(), "CbmLitFitQaStudyReport.h", 26,
                  typeid(::CbmLitFitQaStudyReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitFitQaStudyReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitFitQaStudyReport) );
      instance.SetNew(&new_CbmLitFitQaStudyReport);
      instance.SetNewArray(&newArray_CbmLitFitQaStudyReport);
      instance.SetDelete(&delete_CbmLitFitQaStudyReport);
      instance.SetDeleteArray(&deleteArray_CbmLitFitQaStudyReport);
      instance.SetDestructor(&destruct_CbmLitFitQaStudyReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitFitQaStudyReport*)
   {
      return GenerateInitInstanceLocal((::CbmLitFitQaStudyReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitFitQaStudyReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitFieldQa(void *p = nullptr);
   static void *newArray_CbmLitFieldQa(Long_t size, void *p);
   static void delete_CbmLitFieldQa(void *p);
   static void deleteArray_CbmLitFieldQa(void *p);
   static void destruct_CbmLitFieldQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitFieldQa*)
   {
      ::CbmLitFieldQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitFieldQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitFieldQa", ::CbmLitFieldQa::Class_Version(), "CbmLitFieldQa.h", 34,
                  typeid(::CbmLitFieldQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitFieldQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitFieldQa) );
      instance.SetNew(&new_CbmLitFieldQa);
      instance.SetNewArray(&newArray_CbmLitFieldQa);
      instance.SetDelete(&delete_CbmLitFieldQa);
      instance.SetDeleteArray(&deleteArray_CbmLitFieldQa);
      instance.SetDestructor(&destruct_CbmLitFieldQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitFieldQa*)
   {
      return GenerateInitInstanceLocal((::CbmLitFieldQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitFieldQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitFieldQaReport(void *p = nullptr);
   static void *newArray_CbmLitFieldQaReport(Long_t size, void *p);
   static void delete_CbmLitFieldQaReport(void *p);
   static void deleteArray_CbmLitFieldQaReport(void *p);
   static void destruct_CbmLitFieldQaReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitFieldQaReport*)
   {
      ::CbmLitFieldQaReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitFieldQaReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitFieldQaReport", ::CbmLitFieldQaReport::Class_Version(), "CbmLitFieldQaReport.h", 26,
                  typeid(::CbmLitFieldQaReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitFieldQaReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitFieldQaReport) );
      instance.SetNew(&new_CbmLitFieldQaReport);
      instance.SetNewArray(&newArray_CbmLitFieldQaReport);
      instance.SetDelete(&delete_CbmLitFieldQaReport);
      instance.SetDeleteArray(&deleteArray_CbmLitFieldQaReport);
      instance.SetDestructor(&destruct_CbmLitFieldQaReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitFieldQaReport*)
   {
      return GenerateInitInstanceLocal((::CbmLitFieldQaReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitFieldQaReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitFieldApproximationQa(void *p = nullptr);
   static void *newArray_CbmLitFieldApproximationQa(Long_t size, void *p);
   static void delete_CbmLitFieldApproximationQa(void *p);
   static void deleteArray_CbmLitFieldApproximationQa(void *p);
   static void destruct_CbmLitFieldApproximationQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitFieldApproximationQa*)
   {
      ::CbmLitFieldApproximationQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitFieldApproximationQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitFieldApproximationQa", ::CbmLitFieldApproximationQa::Class_Version(), "CbmLitFieldApproximationQa.h", 39,
                  typeid(::CbmLitFieldApproximationQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitFieldApproximationQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitFieldApproximationQa) );
      instance.SetNew(&new_CbmLitFieldApproximationQa);
      instance.SetNewArray(&newArray_CbmLitFieldApproximationQa);
      instance.SetDelete(&delete_CbmLitFieldApproximationQa);
      instance.SetDeleteArray(&deleteArray_CbmLitFieldApproximationQa);
      instance.SetDestructor(&destruct_CbmLitFieldApproximationQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitFieldApproximationQa*)
   {
      return GenerateInitInstanceLocal((::CbmLitFieldApproximationQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitFieldApproximationQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitFieldApproximationQaReport(void *p = nullptr);
   static void *newArray_CbmLitFieldApproximationQaReport(Long_t size, void *p);
   static void delete_CbmLitFieldApproximationQaReport(void *p);
   static void deleteArray_CbmLitFieldApproximationQaReport(void *p);
   static void destruct_CbmLitFieldApproximationQaReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitFieldApproximationQaReport*)
   {
      ::CbmLitFieldApproximationQaReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitFieldApproximationQaReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitFieldApproximationQaReport", ::CbmLitFieldApproximationQaReport::Class_Version(), "CbmLitFieldApproximationQaReport.h", 26,
                  typeid(::CbmLitFieldApproximationQaReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitFieldApproximationQaReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitFieldApproximationQaReport) );
      instance.SetNew(&new_CbmLitFieldApproximationQaReport);
      instance.SetNewArray(&newArray_CbmLitFieldApproximationQaReport);
      instance.SetDelete(&delete_CbmLitFieldApproximationQaReport);
      instance.SetDeleteArray(&deleteArray_CbmLitFieldApproximationQaReport);
      instance.SetDestructor(&destruct_CbmLitFieldApproximationQaReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitFieldApproximationQaReport*)
   {
      return GenerateInitInstanceLocal((::CbmLitFieldApproximationQaReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitFieldApproximationQaReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitRadLengthQa(void *p = nullptr);
   static void *newArray_CbmLitRadLengthQa(Long_t size, void *p);
   static void delete_CbmLitRadLengthQa(void *p);
   static void deleteArray_CbmLitRadLengthQa(void *p);
   static void destruct_CbmLitRadLengthQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitRadLengthQa*)
   {
      ::CbmLitRadLengthQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitRadLengthQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitRadLengthQa", ::CbmLitRadLengthQa::Class_Version(), "CbmLitRadLengthQa.h", 27,
                  typeid(::CbmLitRadLengthQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitRadLengthQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitRadLengthQa) );
      instance.SetNew(&new_CbmLitRadLengthQa);
      instance.SetNewArray(&newArray_CbmLitRadLengthQa);
      instance.SetDelete(&delete_CbmLitRadLengthQa);
      instance.SetDeleteArray(&deleteArray_CbmLitRadLengthQa);
      instance.SetDestructor(&destruct_CbmLitRadLengthQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitRadLengthQa*)
   {
      return GenerateInitInstanceLocal((::CbmLitRadLengthQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitRadLengthQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitRadLengthQaReport(void *p = nullptr);
   static void *newArray_CbmLitRadLengthQaReport(Long_t size, void *p);
   static void delete_CbmLitRadLengthQaReport(void *p);
   static void deleteArray_CbmLitRadLengthQaReport(void *p);
   static void destruct_CbmLitRadLengthQaReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitRadLengthQaReport*)
   {
      ::CbmLitRadLengthQaReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitRadLengthQaReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitRadLengthQaReport", ::CbmLitRadLengthQaReport::Class_Version(), "CbmLitRadLengthQaReport.h", 28,
                  typeid(::CbmLitRadLengthQaReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitRadLengthQaReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitRadLengthQaReport) );
      instance.SetNew(&new_CbmLitRadLengthQaReport);
      instance.SetNewArray(&newArray_CbmLitRadLengthQaReport);
      instance.SetDelete(&delete_CbmLitRadLengthQaReport);
      instance.SetDeleteArray(&deleteArray_CbmLitRadLengthQaReport);
      instance.SetDestructor(&destruct_CbmLitRadLengthQaReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitRadLengthQaReport*)
   {
      return GenerateInitInstanceLocal((::CbmLitRadLengthQaReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitRadLengthQaReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitRadLengthGenerator(void *p = nullptr);
   static void *newArray_CbmLitRadLengthGenerator(Long_t size, void *p);
   static void delete_CbmLitRadLengthGenerator(void *p);
   static void deleteArray_CbmLitRadLengthGenerator(void *p);
   static void destruct_CbmLitRadLengthGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitRadLengthGenerator*)
   {
      ::CbmLitRadLengthGenerator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitRadLengthGenerator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitRadLengthGenerator", ::CbmLitRadLengthGenerator::Class_Version(), "CbmLitRadLengthGenerator.h", 16,
                  typeid(::CbmLitRadLengthGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitRadLengthGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitRadLengthGenerator) );
      instance.SetNew(&new_CbmLitRadLengthGenerator);
      instance.SetNewArray(&newArray_CbmLitRadLengthGenerator);
      instance.SetDelete(&delete_CbmLitRadLengthGenerator);
      instance.SetDeleteArray(&deleteArray_CbmLitRadLengthGenerator);
      instance.SetDestructor(&destruct_CbmLitRadLengthGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitRadLengthGenerator*)
   {
      return GenerateInitInstanceLocal((::CbmLitRadLengthGenerator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitRadLengthGenerator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitTofQa(void *p = nullptr);
   static void *newArray_CbmLitTofQa(Long_t size, void *p);
   static void delete_CbmLitTofQa(void *p);
   static void deleteArray_CbmLitTofQa(void *p);
   static void destruct_CbmLitTofQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitTofQa*)
   {
      ::CbmLitTofQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitTofQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitTofQa", ::CbmLitTofQa::Class_Version(), "CbmLitTofQa.h", 41,
                  typeid(::CbmLitTofQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitTofQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitTofQa) );
      instance.SetNew(&new_CbmLitTofQa);
      instance.SetNewArray(&newArray_CbmLitTofQa);
      instance.SetDelete(&delete_CbmLitTofQa);
      instance.SetDeleteArray(&deleteArray_CbmLitTofQa);
      instance.SetDestructor(&destruct_CbmLitTofQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitTofQa*)
   {
      return GenerateInitInstanceLocal((::CbmLitTofQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitTofQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLitTofQaReport(void *p = nullptr);
   static void *newArray_CbmLitTofQaReport(Long_t size, void *p);
   static void delete_CbmLitTofQaReport(void *p);
   static void deleteArray_CbmLitTofQaReport(void *p);
   static void destruct_CbmLitTofQaReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLitTofQaReport*)
   {
      ::CbmLitTofQaReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLitTofQaReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLitTofQaReport", ::CbmLitTofQaReport::Class_Version(), "CbmLitTofQaReport.h", 24,
                  typeid(::CbmLitTofQaReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLitTofQaReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLitTofQaReport) );
      instance.SetNew(&new_CbmLitTofQaReport);
      instance.SetNewArray(&newArray_CbmLitTofQaReport);
      instance.SetDelete(&delete_CbmLitTofQaReport);
      instance.SetDeleteArray(&deleteArray_CbmLitTofQaReport);
      instance.SetDestructor(&destruct_CbmLitTofQaReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLitTofQaReport*)
   {
      return GenerateInitInstanceLocal((::CbmLitTofQaReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLitTofQaReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmLitCheckEnergyLossMuons::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitCheckEnergyLossMuons::Class_Name()
{
   return "CbmLitCheckEnergyLossMuons";
}

//______________________________________________________________________________
const char *CbmLitCheckEnergyLossMuons::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitCheckEnergyLossMuons*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitCheckEnergyLossMuons::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitCheckEnergyLossMuons*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitCheckEnergyLossMuons::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitCheckEnergyLossMuons*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitCheckEnergyLossMuons::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitCheckEnergyLossMuons*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitCheckBrem::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitCheckBrem::Class_Name()
{
   return "CbmLitCheckBrem";
}

//______________________________________________________________________________
const char *CbmLitCheckBrem::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitCheckBrem*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitCheckBrem::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitCheckBrem*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitCheckBrem::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitCheckBrem*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitCheckBrem::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitCheckBrem*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitCreateStandaloneData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitCreateStandaloneData::Class_Name()
{
   return "CbmLitCreateStandaloneData";
}

//______________________________________________________________________________
const char *CbmLitCreateStandaloneData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitCreateStandaloneData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitCreateStandaloneData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitCreateStandaloneData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitCreateStandaloneData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitCreateStandaloneData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitCreateStandaloneData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitCreateStandaloneData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitTrackingQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitTrackingQa::Class_Name()
{
   return "CbmLitTrackingQa";
}

//______________________________________________________________________________
const char *CbmLitTrackingQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTrackingQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitTrackingQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTrackingQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitTrackingQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTrackingQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitTrackingQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTrackingQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitTrackingQaStudyReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitTrackingQaStudyReport::Class_Name()
{
   return "CbmLitTrackingQaStudyReport";
}

//______________________________________________________________________________
const char *CbmLitTrackingQaStudyReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTrackingQaStudyReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitTrackingQaStudyReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTrackingQaStudyReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitTrackingQaStudyReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTrackingQaStudyReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitTrackingQaStudyReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTrackingQaStudyReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitTrackingQaReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitTrackingQaReport::Class_Name()
{
   return "CbmLitTrackingQaReport";
}

//______________________________________________________________________________
const char *CbmLitTrackingQaReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTrackingQaReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitTrackingQaReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTrackingQaReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitTrackingQaReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTrackingQaReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitTrackingQaReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTrackingQaReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitClusteringQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitClusteringQa::Class_Name()
{
   return "CbmLitClusteringQa";
}

//______________________________________________________________________________
const char *CbmLitClusteringQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitClusteringQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitClusteringQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitClusteringQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitClusteringQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitClusteringQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitClusteringQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitClusteringQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitClusteringQaReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitClusteringQaReport::Class_Name()
{
   return "CbmLitClusteringQaReport";
}

//______________________________________________________________________________
const char *CbmLitClusteringQaReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitClusteringQaReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitClusteringQaReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitClusteringQaReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitClusteringQaReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitClusteringQaReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitClusteringQaReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitClusteringQaReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitClusteringQaStudyReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitClusteringQaStudyReport::Class_Name()
{
   return "CbmLitClusteringQaStudyReport";
}

//______________________________________________________________________________
const char *CbmLitClusteringQaStudyReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitClusteringQaStudyReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitClusteringQaStudyReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitClusteringQaStudyReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitClusteringQaStudyReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitClusteringQaStudyReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitClusteringQaStudyReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitClusteringQaStudyReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitFitQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitFitQa::Class_Name()
{
   return "CbmLitFitQa";
}

//______________________________________________________________________________
const char *CbmLitFitQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitFitQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitFitQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitFitQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitFitQaReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitFitQaReport::Class_Name()
{
   return "CbmLitFitQaReport";
}

//______________________________________________________________________________
const char *CbmLitFitQaReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitQaReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitFitQaReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitQaReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitFitQaReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitQaReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitFitQaReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitQaReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitFitQaStudyReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitFitQaStudyReport::Class_Name()
{
   return "CbmLitFitQaStudyReport";
}

//______________________________________________________________________________
const char *CbmLitFitQaStudyReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitQaStudyReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitFitQaStudyReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitQaStudyReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitFitQaStudyReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitQaStudyReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitFitQaStudyReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFitQaStudyReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitFieldQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitFieldQa::Class_Name()
{
   return "CbmLitFieldQa";
}

//______________________________________________________________________________
const char *CbmLitFieldQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitFieldQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitFieldQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitFieldQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitFieldQaReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitFieldQaReport::Class_Name()
{
   return "CbmLitFieldQaReport";
}

//______________________________________________________________________________
const char *CbmLitFieldQaReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldQaReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitFieldQaReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldQaReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitFieldQaReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldQaReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitFieldQaReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldQaReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitFieldApproximationQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitFieldApproximationQa::Class_Name()
{
   return "CbmLitFieldApproximationQa";
}

//______________________________________________________________________________
const char *CbmLitFieldApproximationQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldApproximationQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitFieldApproximationQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldApproximationQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitFieldApproximationQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldApproximationQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitFieldApproximationQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldApproximationQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitFieldApproximationQaReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitFieldApproximationQaReport::Class_Name()
{
   return "CbmLitFieldApproximationQaReport";
}

//______________________________________________________________________________
const char *CbmLitFieldApproximationQaReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldApproximationQaReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitFieldApproximationQaReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldApproximationQaReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitFieldApproximationQaReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldApproximationQaReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitFieldApproximationQaReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitFieldApproximationQaReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitRadLengthQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitRadLengthQa::Class_Name()
{
   return "CbmLitRadLengthQa";
}

//______________________________________________________________________________
const char *CbmLitRadLengthQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitRadLengthQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitRadLengthQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitRadLengthQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitRadLengthQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitRadLengthQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitRadLengthQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitRadLengthQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitRadLengthQaReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitRadLengthQaReport::Class_Name()
{
   return "CbmLitRadLengthQaReport";
}

//______________________________________________________________________________
const char *CbmLitRadLengthQaReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitRadLengthQaReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitRadLengthQaReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitRadLengthQaReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitRadLengthQaReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitRadLengthQaReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitRadLengthQaReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitRadLengthQaReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitRadLengthGenerator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitRadLengthGenerator::Class_Name()
{
   return "CbmLitRadLengthGenerator";
}

//______________________________________________________________________________
const char *CbmLitRadLengthGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitRadLengthGenerator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitRadLengthGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitRadLengthGenerator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitRadLengthGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitRadLengthGenerator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitRadLengthGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitRadLengthGenerator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitTofQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitTofQa::Class_Name()
{
   return "CbmLitTofQa";
}

//______________________________________________________________________________
const char *CbmLitTofQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTofQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitTofQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTofQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitTofQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTofQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitTofQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTofQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLitTofQaReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLitTofQaReport::Class_Name()
{
   return "CbmLitTofQaReport";
}

//______________________________________________________________________________
const char *CbmLitTofQaReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTofQaReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLitTofQaReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTofQaReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLitTofQaReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTofQaReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLitTofQaReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLitTofQaReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmLitCheckEnergyLossMuons::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitCheckEnergyLossMuons.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitCheckEnergyLossMuons::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitCheckEnergyLossMuons::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitCheckEnergyLossMuons(void *p) {
      return  p ? new(p) ::CbmLitCheckEnergyLossMuons : new ::CbmLitCheckEnergyLossMuons;
   }
   static void *newArray_CbmLitCheckEnergyLossMuons(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitCheckEnergyLossMuons[nElements] : new ::CbmLitCheckEnergyLossMuons[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitCheckEnergyLossMuons(void *p) {
      delete ((::CbmLitCheckEnergyLossMuons*)p);
   }
   static void deleteArray_CbmLitCheckEnergyLossMuons(void *p) {
      delete [] ((::CbmLitCheckEnergyLossMuons*)p);
   }
   static void destruct_CbmLitCheckEnergyLossMuons(void *p) {
      typedef ::CbmLitCheckEnergyLossMuons current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitCheckEnergyLossMuons

//______________________________________________________________________________
void CbmLitCheckBrem::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitCheckBrem.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitCheckBrem::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitCheckBrem::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitCheckBrem(void *p) {
      return  p ? new(p) ::CbmLitCheckBrem : new ::CbmLitCheckBrem;
   }
   static void *newArray_CbmLitCheckBrem(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitCheckBrem[nElements] : new ::CbmLitCheckBrem[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitCheckBrem(void *p) {
      delete ((::CbmLitCheckBrem*)p);
   }
   static void deleteArray_CbmLitCheckBrem(void *p) {
      delete [] ((::CbmLitCheckBrem*)p);
   }
   static void destruct_CbmLitCheckBrem(void *p) {
      typedef ::CbmLitCheckBrem current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitCheckBrem

//______________________________________________________________________________
void CbmLitCreateStandaloneData::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitCreateStandaloneData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitCreateStandaloneData::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitCreateStandaloneData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitCreateStandaloneData(void *p) {
      return  p ? new(p) ::CbmLitCreateStandaloneData : new ::CbmLitCreateStandaloneData;
   }
   static void *newArray_CbmLitCreateStandaloneData(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitCreateStandaloneData[nElements] : new ::CbmLitCreateStandaloneData[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitCreateStandaloneData(void *p) {
      delete ((::CbmLitCreateStandaloneData*)p);
   }
   static void deleteArray_CbmLitCreateStandaloneData(void *p) {
      delete [] ((::CbmLitCreateStandaloneData*)p);
   }
   static void destruct_CbmLitCreateStandaloneData(void *p) {
      typedef ::CbmLitCreateStandaloneData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitCreateStandaloneData

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitTestMatrixMath(void *p) {
      return  p ? new(p) ::CbmLitTestMatrixMath : new ::CbmLitTestMatrixMath;
   }
   static void *newArray_CbmLitTestMatrixMath(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitTestMatrixMath[nElements] : new ::CbmLitTestMatrixMath[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitTestMatrixMath(void *p) {
      delete ((::CbmLitTestMatrixMath*)p);
   }
   static void deleteArray_CbmLitTestMatrixMath(void *p) {
      delete [] ((::CbmLitTestMatrixMath*)p);
   }
   static void destruct_CbmLitTestMatrixMath(void *p) {
      typedef ::CbmLitTestMatrixMath current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitTestMatrixMath

//______________________________________________________________________________
void CbmLitTrackingQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitTrackingQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitTrackingQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitTrackingQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitTrackingQa(void *p) {
      return  p ? new(p) ::CbmLitTrackingQa : new ::CbmLitTrackingQa;
   }
   static void *newArray_CbmLitTrackingQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitTrackingQa[nElements] : new ::CbmLitTrackingQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitTrackingQa(void *p) {
      delete ((::CbmLitTrackingQa*)p);
   }
   static void deleteArray_CbmLitTrackingQa(void *p) {
      delete [] ((::CbmLitTrackingQa*)p);
   }
   static void destruct_CbmLitTrackingQa(void *p) {
      typedef ::CbmLitTrackingQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitTrackingQa

//______________________________________________________________________________
void CbmLitTrackingQaStudyReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitTrackingQaStudyReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitTrackingQaStudyReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitTrackingQaStudyReport::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitTrackingQaStudyReport(void *p) {
      return  p ? new(p) ::CbmLitTrackingQaStudyReport : new ::CbmLitTrackingQaStudyReport;
   }
   static void *newArray_CbmLitTrackingQaStudyReport(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitTrackingQaStudyReport[nElements] : new ::CbmLitTrackingQaStudyReport[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitTrackingQaStudyReport(void *p) {
      delete ((::CbmLitTrackingQaStudyReport*)p);
   }
   static void deleteArray_CbmLitTrackingQaStudyReport(void *p) {
      delete [] ((::CbmLitTrackingQaStudyReport*)p);
   }
   static void destruct_CbmLitTrackingQaStudyReport(void *p) {
      typedef ::CbmLitTrackingQaStudyReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitTrackingQaStudyReport

//______________________________________________________________________________
void CbmLitTrackingQaReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitTrackingQaReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitTrackingQaReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitTrackingQaReport::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitTrackingQaReport(void *p) {
      return  p ? new(p) ::CbmLitTrackingQaReport : new ::CbmLitTrackingQaReport;
   }
   static void *newArray_CbmLitTrackingQaReport(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitTrackingQaReport[nElements] : new ::CbmLitTrackingQaReport[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitTrackingQaReport(void *p) {
      delete ((::CbmLitTrackingQaReport*)p);
   }
   static void deleteArray_CbmLitTrackingQaReport(void *p) {
      delete [] ((::CbmLitTrackingQaReport*)p);
   }
   static void destruct_CbmLitTrackingQaReport(void *p) {
      typedef ::CbmLitTrackingQaReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitTrackingQaReport

//______________________________________________________________________________
void CbmLitClusteringQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitClusteringQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitClusteringQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitClusteringQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitClusteringQa(void *p) {
      return  p ? new(p) ::CbmLitClusteringQa : new ::CbmLitClusteringQa;
   }
   static void *newArray_CbmLitClusteringQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitClusteringQa[nElements] : new ::CbmLitClusteringQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitClusteringQa(void *p) {
      delete ((::CbmLitClusteringQa*)p);
   }
   static void deleteArray_CbmLitClusteringQa(void *p) {
      delete [] ((::CbmLitClusteringQa*)p);
   }
   static void destruct_CbmLitClusteringQa(void *p) {
      typedef ::CbmLitClusteringQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitClusteringQa

//______________________________________________________________________________
void CbmLitClusteringQaReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitClusteringQaReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitClusteringQaReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitClusteringQaReport::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitClusteringQaReport(void *p) {
      return  p ? new(p) ::CbmLitClusteringQaReport : new ::CbmLitClusteringQaReport;
   }
   static void *newArray_CbmLitClusteringQaReport(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitClusteringQaReport[nElements] : new ::CbmLitClusteringQaReport[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitClusteringQaReport(void *p) {
      delete ((::CbmLitClusteringQaReport*)p);
   }
   static void deleteArray_CbmLitClusteringQaReport(void *p) {
      delete [] ((::CbmLitClusteringQaReport*)p);
   }
   static void destruct_CbmLitClusteringQaReport(void *p) {
      typedef ::CbmLitClusteringQaReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitClusteringQaReport

//______________________________________________________________________________
void CbmLitClusteringQaStudyReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitClusteringQaStudyReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitClusteringQaStudyReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitClusteringQaStudyReport::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitClusteringQaStudyReport(void *p) {
      return  p ? new(p) ::CbmLitClusteringQaStudyReport : new ::CbmLitClusteringQaStudyReport;
   }
   static void *newArray_CbmLitClusteringQaStudyReport(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitClusteringQaStudyReport[nElements] : new ::CbmLitClusteringQaStudyReport[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitClusteringQaStudyReport(void *p) {
      delete ((::CbmLitClusteringQaStudyReport*)p);
   }
   static void deleteArray_CbmLitClusteringQaStudyReport(void *p) {
      delete [] ((::CbmLitClusteringQaStudyReport*)p);
   }
   static void destruct_CbmLitClusteringQaStudyReport(void *p) {
      typedef ::CbmLitClusteringQaStudyReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitClusteringQaStudyReport

//______________________________________________________________________________
void CbmLitFitQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitFitQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitFitQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitFitQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitFitQa(void *p) {
      return  p ? new(p) ::CbmLitFitQa : new ::CbmLitFitQa;
   }
   static void *newArray_CbmLitFitQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitFitQa[nElements] : new ::CbmLitFitQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitFitQa(void *p) {
      delete ((::CbmLitFitQa*)p);
   }
   static void deleteArray_CbmLitFitQa(void *p) {
      delete [] ((::CbmLitFitQa*)p);
   }
   static void destruct_CbmLitFitQa(void *p) {
      typedef ::CbmLitFitQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitFitQa

//______________________________________________________________________________
void CbmLitFitQaReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitFitQaReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitFitQaReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitFitQaReport::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitFitQaReport(void *p) {
      return  p ? new(p) ::CbmLitFitQaReport : new ::CbmLitFitQaReport;
   }
   static void *newArray_CbmLitFitQaReport(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitFitQaReport[nElements] : new ::CbmLitFitQaReport[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitFitQaReport(void *p) {
      delete ((::CbmLitFitQaReport*)p);
   }
   static void deleteArray_CbmLitFitQaReport(void *p) {
      delete [] ((::CbmLitFitQaReport*)p);
   }
   static void destruct_CbmLitFitQaReport(void *p) {
      typedef ::CbmLitFitQaReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitFitQaReport

//______________________________________________________________________________
void CbmLitFitQaStudyReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitFitQaStudyReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitFitQaStudyReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitFitQaStudyReport::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitFitQaStudyReport(void *p) {
      return  p ? new(p) ::CbmLitFitQaStudyReport : new ::CbmLitFitQaStudyReport;
   }
   static void *newArray_CbmLitFitQaStudyReport(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitFitQaStudyReport[nElements] : new ::CbmLitFitQaStudyReport[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitFitQaStudyReport(void *p) {
      delete ((::CbmLitFitQaStudyReport*)p);
   }
   static void deleteArray_CbmLitFitQaStudyReport(void *p) {
      delete [] ((::CbmLitFitQaStudyReport*)p);
   }
   static void destruct_CbmLitFitQaStudyReport(void *p) {
      typedef ::CbmLitFitQaStudyReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitFitQaStudyReport

//______________________________________________________________________________
void CbmLitFieldQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitFieldQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitFieldQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitFieldQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitFieldQa(void *p) {
      return  p ? new(p) ::CbmLitFieldQa : new ::CbmLitFieldQa;
   }
   static void *newArray_CbmLitFieldQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitFieldQa[nElements] : new ::CbmLitFieldQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitFieldQa(void *p) {
      delete ((::CbmLitFieldQa*)p);
   }
   static void deleteArray_CbmLitFieldQa(void *p) {
      delete [] ((::CbmLitFieldQa*)p);
   }
   static void destruct_CbmLitFieldQa(void *p) {
      typedef ::CbmLitFieldQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitFieldQa

//______________________________________________________________________________
void CbmLitFieldQaReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitFieldQaReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitFieldQaReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitFieldQaReport::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitFieldQaReport(void *p) {
      return  p ? new(p) ::CbmLitFieldQaReport : new ::CbmLitFieldQaReport;
   }
   static void *newArray_CbmLitFieldQaReport(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitFieldQaReport[nElements] : new ::CbmLitFieldQaReport[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitFieldQaReport(void *p) {
      delete ((::CbmLitFieldQaReport*)p);
   }
   static void deleteArray_CbmLitFieldQaReport(void *p) {
      delete [] ((::CbmLitFieldQaReport*)p);
   }
   static void destruct_CbmLitFieldQaReport(void *p) {
      typedef ::CbmLitFieldQaReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitFieldQaReport

//______________________________________________________________________________
void CbmLitFieldApproximationQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitFieldApproximationQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitFieldApproximationQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitFieldApproximationQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitFieldApproximationQa(void *p) {
      return  p ? new(p) ::CbmLitFieldApproximationQa : new ::CbmLitFieldApproximationQa;
   }
   static void *newArray_CbmLitFieldApproximationQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitFieldApproximationQa[nElements] : new ::CbmLitFieldApproximationQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitFieldApproximationQa(void *p) {
      delete ((::CbmLitFieldApproximationQa*)p);
   }
   static void deleteArray_CbmLitFieldApproximationQa(void *p) {
      delete [] ((::CbmLitFieldApproximationQa*)p);
   }
   static void destruct_CbmLitFieldApproximationQa(void *p) {
      typedef ::CbmLitFieldApproximationQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitFieldApproximationQa

//______________________________________________________________________________
void CbmLitFieldApproximationQaReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitFieldApproximationQaReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitFieldApproximationQaReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitFieldApproximationQaReport::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitFieldApproximationQaReport(void *p) {
      return  p ? new(p) ::CbmLitFieldApproximationQaReport : new ::CbmLitFieldApproximationQaReport;
   }
   static void *newArray_CbmLitFieldApproximationQaReport(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitFieldApproximationQaReport[nElements] : new ::CbmLitFieldApproximationQaReport[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitFieldApproximationQaReport(void *p) {
      delete ((::CbmLitFieldApproximationQaReport*)p);
   }
   static void deleteArray_CbmLitFieldApproximationQaReport(void *p) {
      delete [] ((::CbmLitFieldApproximationQaReport*)p);
   }
   static void destruct_CbmLitFieldApproximationQaReport(void *p) {
      typedef ::CbmLitFieldApproximationQaReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitFieldApproximationQaReport

//______________________________________________________________________________
void CbmLitRadLengthQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitRadLengthQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitRadLengthQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitRadLengthQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitRadLengthQa(void *p) {
      return  p ? new(p) ::CbmLitRadLengthQa : new ::CbmLitRadLengthQa;
   }
   static void *newArray_CbmLitRadLengthQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitRadLengthQa[nElements] : new ::CbmLitRadLengthQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitRadLengthQa(void *p) {
      delete ((::CbmLitRadLengthQa*)p);
   }
   static void deleteArray_CbmLitRadLengthQa(void *p) {
      delete [] ((::CbmLitRadLengthQa*)p);
   }
   static void destruct_CbmLitRadLengthQa(void *p) {
      typedef ::CbmLitRadLengthQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitRadLengthQa

//______________________________________________________________________________
void CbmLitRadLengthQaReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitRadLengthQaReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitRadLengthQaReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitRadLengthQaReport::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitRadLengthQaReport(void *p) {
      return  p ? new(p) ::CbmLitRadLengthQaReport : new ::CbmLitRadLengthQaReport;
   }
   static void *newArray_CbmLitRadLengthQaReport(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitRadLengthQaReport[nElements] : new ::CbmLitRadLengthQaReport[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitRadLengthQaReport(void *p) {
      delete ((::CbmLitRadLengthQaReport*)p);
   }
   static void deleteArray_CbmLitRadLengthQaReport(void *p) {
      delete [] ((::CbmLitRadLengthQaReport*)p);
   }
   static void destruct_CbmLitRadLengthQaReport(void *p) {
      typedef ::CbmLitRadLengthQaReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitRadLengthQaReport

//______________________________________________________________________________
void CbmLitRadLengthGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitRadLengthGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitRadLengthGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitRadLengthGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitRadLengthGenerator(void *p) {
      return  p ? new(p) ::CbmLitRadLengthGenerator : new ::CbmLitRadLengthGenerator;
   }
   static void *newArray_CbmLitRadLengthGenerator(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitRadLengthGenerator[nElements] : new ::CbmLitRadLengthGenerator[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitRadLengthGenerator(void *p) {
      delete ((::CbmLitRadLengthGenerator*)p);
   }
   static void deleteArray_CbmLitRadLengthGenerator(void *p) {
      delete [] ((::CbmLitRadLengthGenerator*)p);
   }
   static void destruct_CbmLitRadLengthGenerator(void *p) {
      typedef ::CbmLitRadLengthGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitRadLengthGenerator

//______________________________________________________________________________
void CbmLitTofQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitTofQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitTofQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitTofQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitTofQa(void *p) {
      return  p ? new(p) ::CbmLitTofQa : new ::CbmLitTofQa;
   }
   static void *newArray_CbmLitTofQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitTofQa[nElements] : new ::CbmLitTofQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitTofQa(void *p) {
      delete ((::CbmLitTofQa*)p);
   }
   static void deleteArray_CbmLitTofQa(void *p) {
      delete [] ((::CbmLitTofQa*)p);
   }
   static void destruct_CbmLitTofQa(void *p) {
      typedef ::CbmLitTofQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitTofQa

//______________________________________________________________________________
void CbmLitTofQaReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLitTofQaReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLitTofQaReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLitTofQaReport::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLitTofQaReport(void *p) {
      return  p ? new(p) ::CbmLitTofQaReport : new ::CbmLitTofQaReport;
   }
   static void *newArray_CbmLitTofQaReport(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLitTofQaReport[nElements] : new ::CbmLitTofQaReport[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLitTofQaReport(void *p) {
      delete ((::CbmLitTofQaReport*)p);
   }
   static void deleteArray_CbmLitTofQaReport(void *p) {
      delete [] ((::CbmLitTofQaReport*)p);
   }
   static void destruct_CbmLitTofQaReport(void *p) {
      typedef ::CbmLitTofQaReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLitTofQaReport

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
   static TClass *vectorlEpairlEdoublecOdoublegRsPgR_Dictionary();
   static void vectorlEpairlEdoublecOdoublegRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlEdoublecOdoublegRsPgR(void *p = nullptr);
   static void *newArray_vectorlEpairlEdoublecOdoublegRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlEdoublecOdoublegRsPgR(void *p);
   static void deleteArray_vectorlEpairlEdoublecOdoublegRsPgR(void *p);
   static void destruct_vectorlEpairlEdoublecOdoublegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<double,double> >*)
   {
      vector<pair<double,double> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<double,double> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<double,double> >", -2, "vector", 389,
                  typeid(vector<pair<double,double> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlEdoublecOdoublegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<double,double> >) );
      instance.SetNew(&new_vectorlEpairlEdoublecOdoublegRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlEdoublecOdoublegRsPgR);
      instance.SetDelete(&delete_vectorlEpairlEdoublecOdoublegRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlEdoublecOdoublegRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlEdoublecOdoublegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<double,double> > >()));

      ::ROOT::AddClassAlternate("vector<pair<double,double> >","std::vector<std::pair<double, double>, std::allocator<std::pair<double, double> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pair<double,double> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlEdoublecOdoublegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<double,double> >*)nullptr)->GetClass();
      vectorlEpairlEdoublecOdoublegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlEdoublecOdoublegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlEdoublecOdoublegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<double,double> > : new vector<pair<double,double> >;
   }
   static void *newArray_vectorlEpairlEdoublecOdoublegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<double,double> >[nElements] : new vector<pair<double,double> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlEdoublecOdoublegRsPgR(void *p) {
      delete ((vector<pair<double,double> >*)p);
   }
   static void deleteArray_vectorlEpairlEdoublecOdoublegRsPgR(void *p) {
      delete [] ((vector<pair<double,double> >*)p);
   }
   static void destruct_vectorlEpairlEdoublecOdoublegRsPgR(void *p) {
      typedef vector<pair<double,double> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<double,double> >

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
   static TClass *vectorlETGraphmUgR_Dictionary();
   static void vectorlETGraphmUgR_TClassManip(TClass*);
   static void *new_vectorlETGraphmUgR(void *p = nullptr);
   static void *newArray_vectorlETGraphmUgR(Long_t size, void *p);
   static void delete_vectorlETGraphmUgR(void *p);
   static void deleteArray_vectorlETGraphmUgR(void *p);
   static void destruct_vectorlETGraphmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TGraph*>*)
   {
      vector<TGraph*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TGraph*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TGraph*>", -2, "vector", 389,
                  typeid(vector<TGraph*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETGraphmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TGraph*>) );
      instance.SetNew(&new_vectorlETGraphmUgR);
      instance.SetNewArray(&newArray_vectorlETGraphmUgR);
      instance.SetDelete(&delete_vectorlETGraphmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETGraphmUgR);
      instance.SetDestructor(&destruct_vectorlETGraphmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TGraph*> >()));

      ::ROOT::AddClassAlternate("vector<TGraph*>","std::vector<TGraph*, std::allocator<TGraph*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TGraph*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETGraphmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TGraph*>*)nullptr)->GetClass();
      vectorlETGraphmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETGraphmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETGraphmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TGraph*> : new vector<TGraph*>;
   }
   static void *newArray_vectorlETGraphmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TGraph*>[nElements] : new vector<TGraph*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETGraphmUgR(void *p) {
      delete ((vector<TGraph*>*)p);
   }
   static void deleteArray_vectorlETGraphmUgR(void *p) {
      delete [] ((vector<TGraph*>*)p);
   }
   static void destruct_vectorlETGraphmUgR(void *p) {
      typedef vector<TGraph*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TGraph*>

namespace ROOT {
   static TClass *vectorlECbmLitSimpleMaterialgR_Dictionary();
   static void vectorlECbmLitSimpleMaterialgR_TClassManip(TClass*);
   static void *new_vectorlECbmLitSimpleMaterialgR(void *p = nullptr);
   static void *newArray_vectorlECbmLitSimpleMaterialgR(Long_t size, void *p);
   static void delete_vectorlECbmLitSimpleMaterialgR(void *p);
   static void deleteArray_vectorlECbmLitSimpleMaterialgR(void *p);
   static void destruct_vectorlECbmLitSimpleMaterialgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmLitSimpleMaterial>*)
   {
      vector<CbmLitSimpleMaterial> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmLitSimpleMaterial>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmLitSimpleMaterial>", -2, "vector", 389,
                  typeid(vector<CbmLitSimpleMaterial>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmLitSimpleMaterialgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmLitSimpleMaterial>) );
      instance.SetNew(&new_vectorlECbmLitSimpleMaterialgR);
      instance.SetNewArray(&newArray_vectorlECbmLitSimpleMaterialgR);
      instance.SetDelete(&delete_vectorlECbmLitSimpleMaterialgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmLitSimpleMaterialgR);
      instance.SetDestructor(&destruct_vectorlECbmLitSimpleMaterialgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmLitSimpleMaterial> >()));

      ::ROOT::AddClassAlternate("vector<CbmLitSimpleMaterial>","std::vector<CbmLitSimpleMaterial, std::allocator<CbmLitSimpleMaterial> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmLitSimpleMaterial>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmLitSimpleMaterialgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmLitSimpleMaterial>*)nullptr)->GetClass();
      vectorlECbmLitSimpleMaterialgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmLitSimpleMaterialgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmLitSimpleMaterialgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmLitSimpleMaterial> : new vector<CbmLitSimpleMaterial>;
   }
   static void *newArray_vectorlECbmLitSimpleMaterialgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmLitSimpleMaterial>[nElements] : new vector<CbmLitSimpleMaterial>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmLitSimpleMaterialgR(void *p) {
      delete ((vector<CbmLitSimpleMaterial>*)p);
   }
   static void deleteArray_vectorlECbmLitSimpleMaterialgR(void *p) {
      delete [] ((vector<CbmLitSimpleMaterial>*)p);
   }
   static void destruct_vectorlECbmLitSimpleMaterialgR(void *p) {
      typedef vector<CbmLitSimpleMaterial> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmLitSimpleMaterial>

namespace ROOT {
   static TClass *vectorlECbmLitFieldFittermUgR_Dictionary();
   static void vectorlECbmLitFieldFittermUgR_TClassManip(TClass*);
   static void *new_vectorlECbmLitFieldFittermUgR(void *p = nullptr);
   static void *newArray_vectorlECbmLitFieldFittermUgR(Long_t size, void *p);
   static void delete_vectorlECbmLitFieldFittermUgR(void *p);
   static void deleteArray_vectorlECbmLitFieldFittermUgR(void *p);
   static void destruct_vectorlECbmLitFieldFittermUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmLitFieldFitter*>*)
   {
      vector<CbmLitFieldFitter*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmLitFieldFitter*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmLitFieldFitter*>", -2, "vector", 389,
                  typeid(vector<CbmLitFieldFitter*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmLitFieldFittermUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmLitFieldFitter*>) );
      instance.SetNew(&new_vectorlECbmLitFieldFittermUgR);
      instance.SetNewArray(&newArray_vectorlECbmLitFieldFittermUgR);
      instance.SetDelete(&delete_vectorlECbmLitFieldFittermUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmLitFieldFittermUgR);
      instance.SetDestructor(&destruct_vectorlECbmLitFieldFittermUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmLitFieldFitter*> >()));

      ::ROOT::AddClassAlternate("vector<CbmLitFieldFitter*>","std::vector<CbmLitFieldFitter*, std::allocator<CbmLitFieldFitter*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmLitFieldFitter*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmLitFieldFittermUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmLitFieldFitter*>*)nullptr)->GetClass();
      vectorlECbmLitFieldFittermUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmLitFieldFittermUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmLitFieldFittermUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmLitFieldFitter*> : new vector<CbmLitFieldFitter*>;
   }
   static void *newArray_vectorlECbmLitFieldFittermUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmLitFieldFitter*>[nElements] : new vector<CbmLitFieldFitter*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmLitFieldFittermUgR(void *p) {
      delete ((vector<CbmLitFieldFitter*>*)p);
   }
   static void deleteArray_vectorlECbmLitFieldFittermUgR(void *p) {
      delete [] ((vector<CbmLitFieldFitter*>*)p);
   }
   static void destruct_vectorlECbmLitFieldFittermUgR(void *p) {
      typedef vector<CbmLitFieldFitter*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmLitFieldFitter*>

namespace ROOT {
   static TClass *setlEpairlEintcOintgRsPgR_Dictionary();
   static void setlEpairlEintcOintgRsPgR_TClassManip(TClass*);
   static void *new_setlEpairlEintcOintgRsPgR(void *p = nullptr);
   static void *newArray_setlEpairlEintcOintgRsPgR(Long_t size, void *p);
   static void delete_setlEpairlEintcOintgRsPgR(void *p);
   static void deleteArray_setlEpairlEintcOintgRsPgR(void *p);
   static void destruct_setlEpairlEintcOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const set<pair<int,int> >*)
   {
      set<pair<int,int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(set<pair<int,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("set<pair<int,int> >", -2, "set", 94,
                  typeid(set<pair<int,int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &setlEpairlEintcOintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(set<pair<int,int> >) );
      instance.SetNew(&new_setlEpairlEintcOintgRsPgR);
      instance.SetNewArray(&newArray_setlEpairlEintcOintgRsPgR);
      instance.SetDelete(&delete_setlEpairlEintcOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_setlEpairlEintcOintgRsPgR);
      instance.SetDestructor(&destruct_setlEpairlEintcOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Insert< set<pair<int,int> > >()));

      ::ROOT::AddClassAlternate("set<pair<int,int> >","std::set<std::pair<int, int>, std::less<std::pair<int, int> >, std::allocator<std::pair<int, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const set<pair<int,int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *setlEpairlEintcOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const set<pair<int,int> >*)nullptr)->GetClass();
      setlEpairlEintcOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void setlEpairlEintcOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_setlEpairlEintcOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<pair<int,int> > : new set<pair<int,int> >;
   }
   static void *newArray_setlEpairlEintcOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<pair<int,int> >[nElements] : new set<pair<int,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_setlEpairlEintcOintgRsPgR(void *p) {
      delete ((set<pair<int,int> >*)p);
   }
   static void deleteArray_setlEpairlEintcOintgRsPgR(void *p) {
      delete [] ((set<pair<int,int> >*)p);
   }
   static void destruct_setlEpairlEintcOintgRsPgR(void *p) {
      typedef set<pair<int,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class set<pair<int,int> >

namespace ROOT {
   static TClass *multimaplEpairlEintcOintgRcOintgR_Dictionary();
   static void multimaplEpairlEintcOintgRcOintgR_TClassManip(TClass*);
   static void *new_multimaplEpairlEintcOintgRcOintgR(void *p = nullptr);
   static void *newArray_multimaplEpairlEintcOintgRcOintgR(Long_t size, void *p);
   static void delete_multimaplEpairlEintcOintgRcOintgR(void *p);
   static void deleteArray_multimaplEpairlEintcOintgRcOintgR(void *p);
   static void destruct_multimaplEpairlEintcOintgRcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const multimap<pair<int,int>,int>*)
   {
      multimap<pair<int,int>,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(multimap<pair<int,int>,int>));
      static ::ROOT::TGenericClassInfo 
         instance("multimap<pair<int,int>,int>", -2, "map", 99,
                  typeid(multimap<pair<int,int>,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &multimaplEpairlEintcOintgRcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(multimap<pair<int,int>,int>) );
      instance.SetNew(&new_multimaplEpairlEintcOintgRcOintgR);
      instance.SetNewArray(&newArray_multimaplEpairlEintcOintgRcOintgR);
      instance.SetDelete(&delete_multimaplEpairlEintcOintgRcOintgR);
      instance.SetDeleteArray(&deleteArray_multimaplEpairlEintcOintgRcOintgR);
      instance.SetDestructor(&destruct_multimaplEpairlEintcOintgRcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< multimap<pair<int,int>,int> >()));

      ::ROOT::AddClassAlternate("multimap<pair<int,int>,int>","std::multimap<std::pair<int, int>, int, std::less<std::pair<int, int> >, std::allocator<std::pair<std::pair<int, int> const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const multimap<pair<int,int>,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *multimaplEpairlEintcOintgRcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const multimap<pair<int,int>,int>*)nullptr)->GetClass();
      multimaplEpairlEintcOintgRcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void multimaplEpairlEintcOintgRcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_multimaplEpairlEintcOintgRcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) multimap<pair<int,int>,int> : new multimap<pair<int,int>,int>;
   }
   static void *newArray_multimaplEpairlEintcOintgRcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) multimap<pair<int,int>,int>[nElements] : new multimap<pair<int,int>,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_multimaplEpairlEintcOintgRcOintgR(void *p) {
      delete ((multimap<pair<int,int>,int>*)p);
   }
   static void deleteArray_multimaplEpairlEintcOintgRcOintgR(void *p) {
      delete [] ((multimap<pair<int,int>,int>*)p);
   }
   static void destruct_multimaplEpairlEintcOintgRcOintgR(void *p) {
      typedef multimap<pair<int,int>,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class multimap<pair<int,int>,int>

namespace ROOT {
   static TClass *maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR_Dictionary();
   static void maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR(void *p = nullptr);
   static void *newArray_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR(void *p);
   static void deleteArray_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR(void *p);
   static void destruct_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,multimap<pair<int,int>,int> >*)
   {
      map<string,multimap<pair<int,int>,int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,multimap<pair<int,int>,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,multimap<pair<int,int>,int> >", -2, "map", 100,
                  typeid(map<string,multimap<pair<int,int>,int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,multimap<pair<int,int>,int> >) );
      instance.SetNew(&new_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR);
      instance.SetDelete(&delete_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,multimap<pair<int,int>,int> > >()));

      ::ROOT::AddClassAlternate("map<string,multimap<pair<int,int>,int> >","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::multimap<std::pair<int, int>, int, std::less<std::pair<int, int> >, std::allocator<std::pair<std::pair<int, int> const, int> > >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::multimap<std::pair<int, int>, int, std::less<std::pair<int, int> >, std::allocator<std::pair<std::pair<int, int> const, int> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,multimap<pair<int,int>,int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,multimap<pair<int,int>,int> >*)nullptr)->GetClass();
      maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,multimap<pair<int,int>,int> > : new map<string,multimap<pair<int,int>,int> >;
   }
   static void *newArray_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,multimap<pair<int,int>,int> >[nElements] : new map<string,multimap<pair<int,int>,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR(void *p) {
      delete ((map<string,multimap<pair<int,int>,int> >*)p);
   }
   static void deleteArray_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR(void *p) {
      delete [] ((map<string,multimap<pair<int,int>,int> >*)p);
   }
   static void destruct_maplEstringcOmultimaplEpairlEintcOintgRcOintgRsPgR(void *p) {
      typedef map<string,multimap<pair<int,int>,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,multimap<pair<int,int>,int> >

namespace ROOT {
   static TClass *maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR_Dictionary();
   static void maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR_TClassManip(TClass*);
   static void *new_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR(void *p = nullptr);
   static void *newArray_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR(Long_t size, void *p);
   static void delete_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR(void *p);
   static void deleteArray_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR(void *p);
   static void destruct_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)>*)
   {
      map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)>", -2, "map", 100,
                  typeid(map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)>) );
      instance.SetNew(&new_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR);
      instance.SetNewArray(&newArray_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR);
      instance.SetDelete(&delete_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR);
      instance.SetDestructor(&destruct_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)> >()));

      ::ROOT::AddClassAlternate("map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, bool (*)(TClonesArray const*, TClonesArray const*, TClonesArray const*, int), std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, bool (*)(TClonesArray const*, TClonesArray const*, TClonesArray const*, int)> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)>*)nullptr)->GetClass();
      maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)> : new map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)>;
   }
   static void *newArray_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)>[nElements] : new map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR(void *p) {
      delete ((map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)>*)p);
   }
   static void deleteArray_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR(void *p) {
      delete [] ((map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)>*)p);
   }
   static void destruct_maplEstringcOBool_toPmUcPoPconstsPTClonesArraymUcOconstsPTClonesArraymUcOconstsPTClonesArraymUcOInt_tcPgR(void *p) {
      typedef map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,Bool_t(*)(const TClonesArray*,const TClonesArray*,const TClonesArray*,Int_t)>

namespace ROOT {
   static TClass *maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR_Dictionary();
   static void maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR_TClassManip(TClass*);
   static void *new_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR(void *p = nullptr);
   static void *newArray_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR(Long_t size, void *p);
   static void delete_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR(void *p);
   static void deleteArray_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR(void *p);
   static void destruct_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)>*)
   {
      map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)>", -2, "map", 100,
                  typeid(map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)>) );
      instance.SetNew(&new_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR);
      instance.SetNewArray(&newArray_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR);
      instance.SetDelete(&delete_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR);
      instance.SetDestructor(&destruct_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)> >()));

      ::ROOT::AddClassAlternate("map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, bool (*)(CbmMCDataArray*, int, int, int), std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, bool (*)(CbmMCDataArray*, int, int, int)> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)>*)nullptr)->GetClass();
      maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)> : new map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)>;
   }
   static void *newArray_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)>[nElements] : new map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR(void *p) {
      delete ((map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)>*)p);
   }
   static void deleteArray_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR(void *p) {
      delete [] ((map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)>*)p);
   }
   static void destruct_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcOInt_tcPgR(void *p) {
      typedef map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t,Int_t)>

namespace ROOT {
   static TClass *maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR_Dictionary();
   static void maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR_TClassManip(TClass*);
   static void *new_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR(void *p = nullptr);
   static void *newArray_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR(Long_t size, void *p);
   static void delete_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR(void *p);
   static void deleteArray_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR(void *p);
   static void destruct_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)>*)
   {
      map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)>", -2, "map", 100,
                  typeid(map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)>) );
      instance.SetNew(&new_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR);
      instance.SetNewArray(&newArray_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR);
      instance.SetDelete(&delete_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR);
      instance.SetDestructor(&destruct_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)> >()));

      ::ROOT::AddClassAlternate("map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, bool (*)(CbmMCDataArray*, int, int), std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, bool (*)(CbmMCDataArray*, int, int)> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)>*)nullptr)->GetClass();
      maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)> : new map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)>;
   }
   static void *newArray_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)>[nElements] : new map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR(void *p) {
      delete ((map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)>*)p);
   }
   static void deleteArray_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR(void *p) {
      delete [] ((map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)>*)p);
   }
   static void destruct_maplEstringcOBool_toPmUcPoPCbmMCDataArraymUcOInt_tcOInt_tcPgR(void *p) {
      typedef map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,Bool_t(*)(CbmMCDataArray*,Int_t,Int_t)>

namespace {
  void TriggerDictionaryInitialization_libLittrackQA_Impl() {
    static const char* headers[] = {
"CbmLitResultChecker.h",
"CbmLitCheckEnergyLossMuons.h",
"CbmLitCheckBrem.h",
"CbmLitMCTrack.h",
"CbmLitMCTrackCreator.h",
"CbmLitCreateStandaloneData.h",
"CbmLitTestMatrixMath.h",
"CbmLitTrackingQa.h",
"CbmLitTrackingQaStudyReport.h",
"CbmLitTrackingQaReport.h",
"CbmLitClusteringQa.h",
"CbmLitClusteringQaReport.h",
"CbmLitClusteringQaStudyReport.h",
"CbmLitFitQa.h",
"CbmLitFitQaReport.h",
"CbmLitFitQaStudyReport.h",
"CbmLitFieldQa.h",
"CbmLitFieldQaReport.h",
"CbmLitFieldApproximationQa.h",
"CbmLitFieldApproximationQaReport.h",
"CbmLitRadLengthQa.h",
"CbmLitRadLengthQaReport.h",
"CbmLitRadLengthGenerator.h",
"CbmLitTofQa.h",
"CbmLitTofQaReport.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
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
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich/utils",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/finder",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/fitter",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/tracks",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/selection",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/alignment",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/mcbm",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/unpack",
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
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/KFQA",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/littrack/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libLittrackQA dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmLitCheckEnergyLossMuons.h")))  CbmLitCheckEnergyLossMuons;
class __attribute__((annotate("$clingAutoload$CbmLitCheckBrem.h")))  CbmLitCheckBrem;
class __attribute__((annotate("$clingAutoload$CbmLitCreateStandaloneData.h")))  CbmLitCreateStandaloneData;
class __attribute__((annotate("$clingAutoload$CbmLitTestMatrixMath.h")))  CbmLitTestMatrixMath;
class __attribute__((annotate("$clingAutoload$CbmLitTrackingQa.h")))  CbmLitTrackingQa;
class __attribute__((annotate("$clingAutoload$CbmLitTrackingQaStudyReport.h")))  CbmLitTrackingQaStudyReport;
class __attribute__((annotate("$clingAutoload$CbmLitTrackingQaReport.h")))  CbmLitTrackingQaReport;
class __attribute__((annotate("$clingAutoload$CbmLitClusteringQa.h")))  CbmLitClusteringQa;
class __attribute__((annotate("$clingAutoload$CbmLitClusteringQaReport.h")))  CbmLitClusteringQaReport;
class __attribute__((annotate("$clingAutoload$CbmLitClusteringQaStudyReport.h")))  CbmLitClusteringQaStudyReport;
class __attribute__((annotate("$clingAutoload$CbmLitFitQa.h")))  CbmLitFitQa;
class __attribute__((annotate("$clingAutoload$CbmLitFitQaReport.h")))  CbmLitFitQaReport;
class __attribute__((annotate("$clingAutoload$CbmLitFitQaStudyReport.h")))  CbmLitFitQaStudyReport;
class __attribute__((annotate("$clingAutoload$CbmLitFieldQa.h")))  CbmLitFieldQa;
class __attribute__((annotate("$clingAutoload$CbmLitFieldQaReport.h")))  CbmLitFieldQaReport;
class __attribute__((annotate("$clingAutoload$CbmLitFieldApproximationQa.h")))  CbmLitFieldApproximationQa;
class __attribute__((annotate("$clingAutoload$CbmLitFieldApproximationQaReport.h")))  CbmLitFieldApproximationQaReport;
class __attribute__((annotate("$clingAutoload$CbmLitRadLengthQa.h")))  CbmLitRadLengthQa;
class __attribute__((annotate("$clingAutoload$CbmLitRadLengthQaReport.h")))  CbmLitRadLengthQaReport;
class __attribute__((annotate("$clingAutoload$CbmLitRadLengthGenerator.h")))  CbmLitRadLengthGenerator;
class __attribute__((annotate("$clingAutoload$CbmLitTofQa.h")))  CbmLitTofQa;
class __attribute__((annotate("$clingAutoload$CbmLitTofQaReport.h")))  CbmLitTofQaReport;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libLittrackQA dictionary payload"

#ifndef HAVE_SSE
  #define HAVE_SSE 1
#endif
#ifndef HAVE_SSE
  #define HAVE_SSE 1
#endif
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
#include "CbmLitResultChecker.h"
#include "CbmLitCheckEnergyLossMuons.h"
#include "CbmLitCheckBrem.h"
#include "CbmLitMCTrack.h"
#include "CbmLitMCTrackCreator.h"
#include "CbmLitCreateStandaloneData.h"
#include "CbmLitTestMatrixMath.h"
#include "CbmLitTrackingQa.h"
#include "CbmLitTrackingQaStudyReport.h"
#include "CbmLitTrackingQaReport.h"
#include "CbmLitClusteringQa.h"
#include "CbmLitClusteringQaReport.h"
#include "CbmLitClusteringQaStudyReport.h"
#include "CbmLitFitQa.h"
#include "CbmLitFitQaReport.h"
#include "CbmLitFitQaStudyReport.h"
#include "CbmLitFieldQa.h"
#include "CbmLitFieldQaReport.h"
#include "CbmLitFieldApproximationQa.h"
#include "CbmLitFieldApproximationQaReport.h"
#include "CbmLitRadLengthQa.h"
#include "CbmLitRadLengthQaReport.h"
#include "CbmLitRadLengthGenerator.h"
#include "CbmLitTofQa.h"
#include "CbmLitTofQaReport.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmLitCheckBrem", payloadCode, "@",
"CbmLitCheckEnergyLossMuons", payloadCode, "@",
"CbmLitClusteringQa", payloadCode, "@",
"CbmLitClusteringQaReport", payloadCode, "@",
"CbmLitClusteringQaStudyReport", payloadCode, "@",
"CbmLitCreateStandaloneData", payloadCode, "@",
"CbmLitFieldApproximationQa", payloadCode, "@",
"CbmLitFieldApproximationQaReport", payloadCode, "@",
"CbmLitFieldQa", payloadCode, "@",
"CbmLitFieldQaReport", payloadCode, "@",
"CbmLitFitQa", payloadCode, "@",
"CbmLitFitQaReport", payloadCode, "@",
"CbmLitFitQaStudyReport", payloadCode, "@",
"CbmLitRadLengthGenerator", payloadCode, "@",
"CbmLitRadLengthQa", payloadCode, "@",
"CbmLitRadLengthQaReport", payloadCode, "@",
"CbmLitTestMatrixMath", payloadCode, "@",
"CbmLitTofQa", payloadCode, "@",
"CbmLitTofQaReport", payloadCode, "@",
"CbmLitTrackingQa", payloadCode, "@",
"CbmLitTrackingQaReport", payloadCode, "@",
"CbmLitTrackingQaStudyReport", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libLittrackQA",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libLittrackQA_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libLittrackQA_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libLittrackQA() {
  TriggerDictionaryInitialization_libLittrackQA_Impl();
}
