// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmBase
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
#include "CbmDigiManager.h"
#include "CbmDigitizeBase.h"
#include "CbmDaq.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCDataObject.h"
#include "CbmRadDamage.h"
#include "CbmHistManager.h"
#include "CbmMatchRecoToMC.h"
#include "CbmTrackingDetectorInterfaceBase.h"
#include "CbmDrawHist.h"
#include "CbmReport.h"
#include "CbmStudyReport.h"
#include "CbmSimulationReport.h"
#include "CbmTextReportElement.h"
#include "CbmHtmlReportElement.h"
#include "CbmLatexReportElement.h"
#include "CbmUtils.h"
#include "CbmGeometryUtils.h"
#include "CbmMcbmUtils.h"
#include "CbmMediaList.h"
#include "CbmFileUtils.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace cbm {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *cbm_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("cbm", 0 /*version*/, "CbmMcbmUtils.h", 8,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &cbm_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *cbm_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace cbm {
   namespace mcbm {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *cbmcLcLmcbm_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("cbm::mcbm", 0 /*version*/, "CbmMcbmUtils.h", 10,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &cbmcLcLmcbm_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *cbmcLcLmcbm_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}
}

namespace ROOT {
   static TClass *pairlETStringcOTStringgR_Dictionary();
   static void pairlETStringcOTStringgR_TClassManip(TClass*);
   static void *new_pairlETStringcOTStringgR(void *p = nullptr);
   static void *newArray_pairlETStringcOTStringgR(Long_t size, void *p);
   static void delete_pairlETStringcOTStringgR(void *p);
   static void deleteArray_pairlETStringcOTStringgR(void *p);
   static void destruct_pairlETStringcOTStringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const pair<TString,TString>*)
   {
      pair<TString,TString> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(pair<TString,TString>));
      static ::ROOT::TGenericClassInfo 
         instance("pair<TString,TString>", "string", 211,
                  typeid(pair<TString,TString>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &pairlETStringcOTStringgR_Dictionary, isa_proxy, 0,
                  sizeof(pair<TString,TString>) );
      instance.SetNew(&new_pairlETStringcOTStringgR);
      instance.SetNewArray(&newArray_pairlETStringcOTStringgR);
      instance.SetDelete(&delete_pairlETStringcOTStringgR);
      instance.SetDeleteArray(&deleteArray_pairlETStringcOTStringgR);
      instance.SetDestructor(&destruct_pairlETStringcOTStringgR);

      ::ROOT::AddClassAlternate("pair<TString,TString>","std::pair<TString, TString>");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const pair<TString,TString>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *pairlETStringcOTStringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const pair<TString,TString>*)nullptr)->GetClass();
      pairlETStringcOTStringgR_TClassManip(theClass);
   return theClass;
   }

   static void pairlETStringcOTStringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmDigiManager(void *p);
   static void deleteArray_CbmDigiManager(void *p);
   static void destruct_CbmDigiManager(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiManager*)
   {
      ::CbmDigiManager *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigiManager >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiManager", ::CbmDigiManager::Class_Version(), "CbmDigiManager.h", 43,
                  typeid(::CbmDigiManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmDigiManager::Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiManager) );
      instance.SetDelete(&delete_CbmDigiManager);
      instance.SetDeleteArray(&deleteArray_CbmDigiManager);
      instance.SetDestructor(&destruct_CbmDigiManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiManager*)
   {
      return GenerateInitInstanceLocal((::CbmDigiManager*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiManager*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmDigitizeBase(void *p);
   static void deleteArray_CbmDigitizeBase(void *p);
   static void destruct_CbmDigitizeBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigitizeBase*)
   {
      ::CbmDigitizeBase *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigitizeBase >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigitizeBase", ::CbmDigitizeBase::Class_Version(), "CbmDigitizeBase.h", 33,
                  typeid(::CbmDigitizeBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmDigitizeBase::Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigitizeBase) );
      instance.SetDelete(&delete_CbmDigitizeBase);
      instance.SetDeleteArray(&deleteArray_CbmDigitizeBase);
      instance.SetDestructor(&destruct_CbmDigitizeBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigitizeBase*)
   {
      return GenerateInitInstanceLocal((::CbmDigitizeBase*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigitizeBase*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmDaq(void *p = nullptr);
   static void *newArray_CbmDaq(Long_t size, void *p);
   static void delete_CbmDaq(void *p);
   static void deleteArray_CbmDaq(void *p);
   static void destruct_CbmDaq(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDaq*)
   {
      ::CbmDaq *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDaq >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDaq", ::CbmDaq::Class_Version(), "CbmDaq.h", 44,
                  typeid(::CbmDaq), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmDaq::Dictionary, isa_proxy, 4,
                  sizeof(::CbmDaq) );
      instance.SetNew(&new_CbmDaq);
      instance.SetNewArray(&newArray_CbmDaq);
      instance.SetDelete(&delete_CbmDaq);
      instance.SetDeleteArray(&deleteArray_CbmDaq);
      instance.SetDestructor(&destruct_CbmDaq);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDaq*)
   {
      return GenerateInitInstanceLocal((::CbmDaq*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDaq*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmMCDataArray(void *p);
   static void deleteArray_CbmMCDataArray(void *p);
   static void destruct_CbmMCDataArray(void *p);
   static void streamer_CbmMCDataArray(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMCDataArray*)
   {
      ::CbmMCDataArray *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMCDataArray >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMCDataArray", ::CbmMCDataArray::Class_Version(), "CbmMCDataArray.h", 39,
                  typeid(::CbmMCDataArray), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMCDataArray::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMCDataArray) );
      instance.SetDelete(&delete_CbmMCDataArray);
      instance.SetDeleteArray(&deleteArray_CbmMCDataArray);
      instance.SetDestructor(&destruct_CbmMCDataArray);
      instance.SetStreamerFunc(&streamer_CbmMCDataArray);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMCDataArray*)
   {
      return GenerateInitInstanceLocal((::CbmMCDataArray*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMCDataArray*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMCDataManager(void *p = nullptr);
   static void *newArray_CbmMCDataManager(Long_t size, void *p);
   static void delete_CbmMCDataManager(void *p);
   static void deleteArray_CbmMCDataManager(void *p);
   static void destruct_CbmMCDataManager(void *p);
   static void streamer_CbmMCDataManager(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMCDataManager*)
   {
      ::CbmMCDataManager *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMCDataManager >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMCDataManager", ::CbmMCDataManager::Class_Version(), "CbmMCDataManager.h", 31,
                  typeid(::CbmMCDataManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMCDataManager::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMCDataManager) );
      instance.SetNew(&new_CbmMCDataManager);
      instance.SetNewArray(&newArray_CbmMCDataManager);
      instance.SetDelete(&delete_CbmMCDataManager);
      instance.SetDeleteArray(&deleteArray_CbmMCDataManager);
      instance.SetDestructor(&destruct_CbmMCDataManager);
      instance.SetStreamerFunc(&streamer_CbmMCDataManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMCDataManager*)
   {
      return GenerateInitInstanceLocal((::CbmMCDataManager*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMCDataManager*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmMCDataObject(void *p);
   static void deleteArray_CbmMCDataObject(void *p);
   static void destruct_CbmMCDataObject(void *p);
   static void streamer_CbmMCDataObject(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMCDataObject*)
   {
      ::CbmMCDataObject *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMCDataObject >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMCDataObject", ::CbmMCDataObject::Class_Version(), "CbmMCDataObject.h", 38,
                  typeid(::CbmMCDataObject), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMCDataObject::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMCDataObject) );
      instance.SetDelete(&delete_CbmMCDataObject);
      instance.SetDeleteArray(&deleteArray_CbmMCDataObject);
      instance.SetDestructor(&destruct_CbmMCDataObject);
      instance.SetStreamerFunc(&streamer_CbmMCDataObject);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMCDataObject*)
   {
      return GenerateInitInstanceLocal((::CbmMCDataObject*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMCDataObject*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRadDamage(void *p = nullptr);
   static void *newArray_CbmRadDamage(Long_t size, void *p);
   static void delete_CbmRadDamage(void *p);
   static void deleteArray_CbmRadDamage(void *p);
   static void destruct_CbmRadDamage(void *p);
   static void streamer_CbmRadDamage(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRadDamage*)
   {
      ::CbmRadDamage *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRadDamage >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRadDamage", ::CbmRadDamage::Class_Version(), "CbmRadDamage.h", 27,
                  typeid(::CbmRadDamage), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRadDamage::Dictionary, isa_proxy, 16,
                  sizeof(::CbmRadDamage) );
      instance.SetNew(&new_CbmRadDamage);
      instance.SetNewArray(&newArray_CbmRadDamage);
      instance.SetDelete(&delete_CbmRadDamage);
      instance.SetDeleteArray(&deleteArray_CbmRadDamage);
      instance.SetDestructor(&destruct_CbmRadDamage);
      instance.SetStreamerFunc(&streamer_CbmRadDamage);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRadDamage*)
   {
      return GenerateInitInstanceLocal((::CbmRadDamage*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRadDamage*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmHistManager(void *p = nullptr);
   static void *newArray_CbmHistManager(Long_t size, void *p);
   static void delete_CbmHistManager(void *p);
   static void deleteArray_CbmHistManager(void *p);
   static void destruct_CbmHistManager(void *p);
   static void streamer_CbmHistManager(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmHistManager*)
   {
      ::CbmHistManager *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmHistManager >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmHistManager", ::CbmHistManager::Class_Version(), "CbmHistManager.h", 45,
                  typeid(::CbmHistManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmHistManager::Dictionary, isa_proxy, 16,
                  sizeof(::CbmHistManager) );
      instance.SetNew(&new_CbmHistManager);
      instance.SetNewArray(&newArray_CbmHistManager);
      instance.SetDelete(&delete_CbmHistManager);
      instance.SetDeleteArray(&deleteArray_CbmHistManager);
      instance.SetDestructor(&destruct_CbmHistManager);
      instance.SetStreamerFunc(&streamer_CbmHistManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmHistManager*)
   {
      return GenerateInitInstanceLocal((::CbmHistManager*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmHistManager*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMatchRecoToMC(void *p = nullptr);
   static void *newArray_CbmMatchRecoToMC(Long_t size, void *p);
   static void delete_CbmMatchRecoToMC(void *p);
   static void deleteArray_CbmMatchRecoToMC(void *p);
   static void destruct_CbmMatchRecoToMC(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMatchRecoToMC*)
   {
      ::CbmMatchRecoToMC *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMatchRecoToMC >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMatchRecoToMC", ::CbmMatchRecoToMC::Class_Version(), "CbmMatchRecoToMC.h", 32,
                  typeid(::CbmMatchRecoToMC), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMatchRecoToMC::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMatchRecoToMC) );
      instance.SetNew(&new_CbmMatchRecoToMC);
      instance.SetNewArray(&newArray_CbmMatchRecoToMC);
      instance.SetDelete(&delete_CbmMatchRecoToMC);
      instance.SetDeleteArray(&deleteArray_CbmMatchRecoToMC);
      instance.SetDestructor(&destruct_CbmMatchRecoToMC);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMatchRecoToMC*)
   {
      return GenerateInitInstanceLocal((::CbmMatchRecoToMC*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMatchRecoToMC*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmReport(void *p);
   static void deleteArray_CbmReport(void *p);
   static void destruct_CbmReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmReport*)
   {
      ::CbmReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmReport", ::CbmReport::Class_Version(), "CbmReport.h", 45,
                  typeid(::CbmReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmReport) );
      instance.SetDelete(&delete_CbmReport);
      instance.SetDeleteArray(&deleteArray_CbmReport);
      instance.SetDestructor(&destruct_CbmReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmReport*)
   {
      return GenerateInitInstanceLocal((::CbmReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmStudyReport(void *p);
   static void deleteArray_CbmStudyReport(void *p);
   static void destruct_CbmStudyReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStudyReport*)
   {
      ::CbmStudyReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStudyReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStudyReport", ::CbmStudyReport::Class_Version(), "CbmStudyReport.h", 30,
                  typeid(::CbmStudyReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStudyReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStudyReport) );
      instance.SetDelete(&delete_CbmStudyReport);
      instance.SetDeleteArray(&deleteArray_CbmStudyReport);
      instance.SetDestructor(&destruct_CbmStudyReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStudyReport*)
   {
      return GenerateInitInstanceLocal((::CbmStudyReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStudyReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmSimulationReport(void *p);
   static void deleteArray_CbmSimulationReport(void *p);
   static void destruct_CbmSimulationReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmSimulationReport*)
   {
      ::CbmSimulationReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmSimulationReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmSimulationReport", ::CbmSimulationReport::Class_Version(), "CbmSimulationReport.h", 32,
                  typeid(::CbmSimulationReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmSimulationReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmSimulationReport) );
      instance.SetDelete(&delete_CbmSimulationReport);
      instance.SetDeleteArray(&deleteArray_CbmSimulationReport);
      instance.SetDestructor(&destruct_CbmSimulationReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmSimulationReport*)
   {
      return GenerateInitInstanceLocal((::CbmSimulationReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmSimulationReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmReportElement(void *p);
   static void deleteArray_CbmReportElement(void *p);
   static void destruct_CbmReportElement(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmReportElement*)
   {
      ::CbmReportElement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmReportElement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmReportElement", ::CbmReportElement::Class_Version(), "CbmReportElement.h", 36,
                  typeid(::CbmReportElement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmReportElement::Dictionary, isa_proxy, 4,
                  sizeof(::CbmReportElement) );
      instance.SetDelete(&delete_CbmReportElement);
      instance.SetDeleteArray(&deleteArray_CbmReportElement);
      instance.SetDestructor(&destruct_CbmReportElement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmReportElement*)
   {
      return GenerateInitInstanceLocal((::CbmReportElement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmReportElement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTextReportElement(void *p = nullptr);
   static void *newArray_CbmTextReportElement(Long_t size, void *p);
   static void delete_CbmTextReportElement(void *p);
   static void deleteArray_CbmTextReportElement(void *p);
   static void destruct_CbmTextReportElement(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTextReportElement*)
   {
      ::CbmTextReportElement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTextReportElement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTextReportElement", ::CbmTextReportElement::Class_Version(), "CbmTextReportElement.h", 28,
                  typeid(::CbmTextReportElement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTextReportElement::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTextReportElement) );
      instance.SetNew(&new_CbmTextReportElement);
      instance.SetNewArray(&newArray_CbmTextReportElement);
      instance.SetDelete(&delete_CbmTextReportElement);
      instance.SetDeleteArray(&deleteArray_CbmTextReportElement);
      instance.SetDestructor(&destruct_CbmTextReportElement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTextReportElement*)
   {
      return GenerateInitInstanceLocal((::CbmTextReportElement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTextReportElement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmHtmlReportElement(void *p = nullptr);
   static void *newArray_CbmHtmlReportElement(Long_t size, void *p);
   static void delete_CbmHtmlReportElement(void *p);
   static void deleteArray_CbmHtmlReportElement(void *p);
   static void destruct_CbmHtmlReportElement(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmHtmlReportElement*)
   {
      ::CbmHtmlReportElement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmHtmlReportElement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmHtmlReportElement", ::CbmHtmlReportElement::Class_Version(), "CbmHtmlReportElement.h", 27,
                  typeid(::CbmHtmlReportElement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmHtmlReportElement::Dictionary, isa_proxy, 4,
                  sizeof(::CbmHtmlReportElement) );
      instance.SetNew(&new_CbmHtmlReportElement);
      instance.SetNewArray(&newArray_CbmHtmlReportElement);
      instance.SetDelete(&delete_CbmHtmlReportElement);
      instance.SetDeleteArray(&deleteArray_CbmHtmlReportElement);
      instance.SetDestructor(&destruct_CbmHtmlReportElement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmHtmlReportElement*)
   {
      return GenerateInitInstanceLocal((::CbmHtmlReportElement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmHtmlReportElement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLatexReportElement(void *p = nullptr);
   static void *newArray_CbmLatexReportElement(Long_t size, void *p);
   static void delete_CbmLatexReportElement(void *p);
   static void deleteArray_CbmLatexReportElement(void *p);
   static void destruct_CbmLatexReportElement(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLatexReportElement*)
   {
      ::CbmLatexReportElement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLatexReportElement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLatexReportElement", ::CbmLatexReportElement::Class_Version(), "CbmLatexReportElement.h", 27,
                  typeid(::CbmLatexReportElement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLatexReportElement::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLatexReportElement) );
      instance.SetNew(&new_CbmLatexReportElement);
      instance.SetNewArray(&newArray_CbmLatexReportElement);
      instance.SetDelete(&delete_CbmLatexReportElement);
      instance.SetDeleteArray(&deleteArray_CbmLatexReportElement);
      instance.SetDestructor(&destruct_CbmLatexReportElement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLatexReportElement*)
   {
      return GenerateInitInstanceLocal((::CbmLatexReportElement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLatexReportElement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *cbmcLcLmcbmcLcLToForceLibLoad_Dictionary();
   static void cbmcLcLmcbmcLcLToForceLibLoad_TClassManip(TClass*);
   static void *new_cbmcLcLmcbmcLcLToForceLibLoad(void *p = nullptr);
   static void *newArray_cbmcLcLmcbmcLcLToForceLibLoad(Long_t size, void *p);
   static void delete_cbmcLcLmcbmcLcLToForceLibLoad(void *p);
   static void deleteArray_cbmcLcLmcbmcLcLToForceLibLoad(void *p);
   static void destruct_cbmcLcLmcbmcLcLToForceLibLoad(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cbm::mcbm::ToForceLibLoad*)
   {
      ::cbm::mcbm::ToForceLibLoad *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::cbm::mcbm::ToForceLibLoad));
      static ::ROOT::TGenericClassInfo 
         instance("cbm::mcbm::ToForceLibLoad", "CbmMcbmUtils.h", 15,
                  typeid(::cbm::mcbm::ToForceLibLoad), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &cbmcLcLmcbmcLcLToForceLibLoad_Dictionary, isa_proxy, 0,
                  sizeof(::cbm::mcbm::ToForceLibLoad) );
      instance.SetNew(&new_cbmcLcLmcbmcLcLToForceLibLoad);
      instance.SetNewArray(&newArray_cbmcLcLmcbmcLcLToForceLibLoad);
      instance.SetDelete(&delete_cbmcLcLmcbmcLcLToForceLibLoad);
      instance.SetDeleteArray(&deleteArray_cbmcLcLmcbmcLcLToForceLibLoad);
      instance.SetDestructor(&destruct_cbmcLcLmcbmcLcLToForceLibLoad);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cbm::mcbm::ToForceLibLoad*)
   {
      return GenerateInitInstanceLocal((::cbm::mcbm::ToForceLibLoad*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cbm::mcbm::ToForceLibLoad*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cbmcLcLmcbmcLcLToForceLibLoad_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cbm::mcbm::ToForceLibLoad*)nullptr)->GetClass();
      cbmcLcLmcbmcLcLToForceLibLoad_TClassManip(theClass);
   return theClass;
   }

   static void cbmcLcLmcbmcLcLToForceLibLoad_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMediaList(void *p = nullptr);
   static void *newArray_CbmMediaList(Long_t size, void *p);
   static void delete_CbmMediaList(void *p);
   static void deleteArray_CbmMediaList(void *p);
   static void destruct_CbmMediaList(void *p);
   static void streamer_CbmMediaList(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMediaList*)
   {
      ::CbmMediaList *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMediaList >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMediaList", ::CbmMediaList::Class_Version(), "CbmMediaList.h", 26,
                  typeid(::CbmMediaList), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMediaList::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMediaList) );
      instance.SetNew(&new_CbmMediaList);
      instance.SetNewArray(&newArray_CbmMediaList);
      instance.SetDelete(&delete_CbmMediaList);
      instance.SetDeleteArray(&deleteArray_CbmMediaList);
      instance.SetDestructor(&destruct_CbmMediaList);
      instance.SetStreamerFunc(&streamer_CbmMediaList);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMediaList*)
   {
      return GenerateInitInstanceLocal((::CbmMediaList*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMediaList*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmDigiManager::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmDigiManager::Class_Name()
{
   return "CbmDigiManager";
}

//______________________________________________________________________________
const char *CbmDigiManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiManager*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmDigiManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiManager*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmDigiManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiManager*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmDigiManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiManager*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmDigitizeBase::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmDigitizeBase::Class_Name()
{
   return "CbmDigitizeBase";
}

//______________________________________________________________________________
const char *CbmDigitizeBase::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizeBase*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmDigitizeBase::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizeBase*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmDigitizeBase::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizeBase*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmDigitizeBase::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitizeBase*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmDaq::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmDaq::Class_Name()
{
   return "CbmDaq";
}

//______________________________________________________________________________
const char *CbmDaq::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDaq*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmDaq::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDaq*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmDaq::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDaq*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmDaq::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDaq*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMCDataArray::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMCDataArray::Class_Name()
{
   return "CbmMCDataArray";
}

//______________________________________________________________________________
const char *CbmMCDataArray::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCDataArray*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMCDataArray::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCDataArray*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMCDataArray::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCDataArray*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMCDataArray::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCDataArray*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMCDataManager::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMCDataManager::Class_Name()
{
   return "CbmMCDataManager";
}

//______________________________________________________________________________
const char *CbmMCDataManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCDataManager*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMCDataManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCDataManager*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMCDataManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCDataManager*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMCDataManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCDataManager*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMCDataObject::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMCDataObject::Class_Name()
{
   return "CbmMCDataObject";
}

//______________________________________________________________________________
const char *CbmMCDataObject::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCDataObject*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMCDataObject::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCDataObject*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMCDataObject::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCDataObject*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMCDataObject::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCDataObject*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRadDamage::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRadDamage::Class_Name()
{
   return "CbmRadDamage";
}

//______________________________________________________________________________
const char *CbmRadDamage::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRadDamage*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRadDamage::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRadDamage*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRadDamage::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRadDamage*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRadDamage::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRadDamage*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmHistManager::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmHistManager::Class_Name()
{
   return "CbmHistManager";
}

//______________________________________________________________________________
const char *CbmHistManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHistManager*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmHistManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHistManager*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmHistManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHistManager*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmHistManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHistManager*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMatchRecoToMC::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMatchRecoToMC::Class_Name()
{
   return "CbmMatchRecoToMC";
}

//______________________________________________________________________________
const char *CbmMatchRecoToMC::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMatchRecoToMC*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMatchRecoToMC::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMatchRecoToMC*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMatchRecoToMC::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMatchRecoToMC*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMatchRecoToMC::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMatchRecoToMC*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmReport::Class_Name()
{
   return "CbmReport";
}

//______________________________________________________________________________
const char *CbmReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStudyReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStudyReport::Class_Name()
{
   return "CbmStudyReport";
}

//______________________________________________________________________________
const char *CbmStudyReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStudyReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStudyReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStudyReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStudyReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStudyReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStudyReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStudyReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmSimulationReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmSimulationReport::Class_Name()
{
   return "CbmSimulationReport";
}

//______________________________________________________________________________
const char *CbmSimulationReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmSimulationReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmSimulationReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmSimulationReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmSimulationReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmSimulationReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmSimulationReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmSimulationReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmReportElement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmReportElement::Class_Name()
{
   return "CbmReportElement";
}

//______________________________________________________________________________
const char *CbmReportElement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmReportElement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmReportElement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmReportElement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmReportElement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmReportElement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmReportElement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmReportElement*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTextReportElement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTextReportElement::Class_Name()
{
   return "CbmTextReportElement";
}

//______________________________________________________________________________
const char *CbmTextReportElement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTextReportElement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTextReportElement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTextReportElement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTextReportElement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTextReportElement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTextReportElement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTextReportElement*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmHtmlReportElement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmHtmlReportElement::Class_Name()
{
   return "CbmHtmlReportElement";
}

//______________________________________________________________________________
const char *CbmHtmlReportElement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHtmlReportElement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmHtmlReportElement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHtmlReportElement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmHtmlReportElement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHtmlReportElement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmHtmlReportElement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHtmlReportElement*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLatexReportElement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLatexReportElement::Class_Name()
{
   return "CbmLatexReportElement";
}

//______________________________________________________________________________
const char *CbmLatexReportElement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLatexReportElement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLatexReportElement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLatexReportElement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLatexReportElement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLatexReportElement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLatexReportElement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLatexReportElement*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMediaList::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMediaList::Class_Name()
{
   return "CbmMediaList";
}

//______________________________________________________________________________
const char *CbmMediaList::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMediaList*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMediaList::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMediaList*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMediaList::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMediaList*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMediaList::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMediaList*)nullptr)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_pairlETStringcOTStringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) pair<TString,TString> : new pair<TString,TString>;
   }
   static void *newArray_pairlETStringcOTStringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) pair<TString,TString>[nElements] : new pair<TString,TString>[nElements];
   }
   // Wrapper around operator delete
   static void delete_pairlETStringcOTStringgR(void *p) {
      delete ((pair<TString,TString>*)p);
   }
   static void deleteArray_pairlETStringcOTStringgR(void *p) {
      delete [] ((pair<TString,TString>*)p);
   }
   static void destruct_pairlETStringcOTStringgR(void *p) {
      typedef pair<TString,TString> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class pair<TString,TString>

//______________________________________________________________________________
void CbmDigiManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigiManager.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigiManager::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigiManager::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmDigiManager(void *p) {
      delete ((::CbmDigiManager*)p);
   }
   static void deleteArray_CbmDigiManager(void *p) {
      delete [] ((::CbmDigiManager*)p);
   }
   static void destruct_CbmDigiManager(void *p) {
      typedef ::CbmDigiManager current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiManager

//______________________________________________________________________________
void CbmDigitizeBase::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigitizeBase.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigitizeBase::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigitizeBase::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmDigitizeBase(void *p) {
      delete ((::CbmDigitizeBase*)p);
   }
   static void deleteArray_CbmDigitizeBase(void *p) {
      delete [] ((::CbmDigitizeBase*)p);
   }
   static void destruct_CbmDigitizeBase(void *p) {
      typedef ::CbmDigitizeBase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigitizeBase

//______________________________________________________________________________
void CbmDaq::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDaq.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDaq::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDaq::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDaq(void *p) {
      return  p ? new(p) ::CbmDaq : new ::CbmDaq;
   }
   static void *newArray_CbmDaq(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDaq[nElements] : new ::CbmDaq[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDaq(void *p) {
      delete ((::CbmDaq*)p);
   }
   static void deleteArray_CbmDaq(void *p) {
      delete [] ((::CbmDaq*)p);
   }
   static void destruct_CbmDaq(void *p) {
      typedef ::CbmDaq current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDaq

//______________________________________________________________________________
void CbmMCDataArray::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMCDataArray.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b.CheckByteCount(R__s, R__c, CbmMCDataArray::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMCDataArray::IsA(), kTRUE);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmMCDataArray(void *p) {
      delete ((::CbmMCDataArray*)p);
   }
   static void deleteArray_CbmMCDataArray(void *p) {
      delete [] ((::CbmMCDataArray*)p);
   }
   static void destruct_CbmMCDataArray(void *p) {
      typedef ::CbmMCDataArray current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMCDataArray(TBuffer &buf, void *obj) {
      ((::CbmMCDataArray*)obj)->::CbmMCDataArray::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMCDataArray

//______________________________________________________________________________
void CbmMCDataManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMCDataManager.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> fLegacy;
      R__b.CheckByteCount(R__s, R__c, CbmMCDataManager::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMCDataManager::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << fLegacy;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMCDataManager(void *p) {
      return  p ? new(p) ::CbmMCDataManager : new ::CbmMCDataManager;
   }
   static void *newArray_CbmMCDataManager(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMCDataManager[nElements] : new ::CbmMCDataManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMCDataManager(void *p) {
      delete ((::CbmMCDataManager*)p);
   }
   static void deleteArray_CbmMCDataManager(void *p) {
      delete [] ((::CbmMCDataManager*)p);
   }
   static void destruct_CbmMCDataManager(void *p) {
      typedef ::CbmMCDataManager current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMCDataManager(TBuffer &buf, void *obj) {
      ((::CbmMCDataManager*)obj)->::CbmMCDataManager::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMCDataManager

//______________________________________________________________________________
void CbmMCDataObject::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMCDataObject.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b.CheckByteCount(R__s, R__c, CbmMCDataObject::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMCDataObject::IsA(), kTRUE);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmMCDataObject(void *p) {
      delete ((::CbmMCDataObject*)p);
   }
   static void deleteArray_CbmMCDataObject(void *p) {
      delete [] ((::CbmMCDataObject*)p);
   }
   static void destruct_CbmMCDataObject(void *p) {
      typedef ::CbmMCDataObject current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMCDataObject(TBuffer &buf, void *obj) {
      ((::CbmMCDataObject*)obj)->::CbmMCDataObject::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMCDataObject

//______________________________________________________________________________
void CbmRadDamage::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRadDamage.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      {
         map<Double_t,Double_t> &R__stl =  niel_neutron;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            double R__t;
            R__b >> R__t;
            double R__t2;
            R__b >> R__t2;
            typedef double Value_t;
            std::pair<Value_t const, double > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         map<Double_t,Double_t> &R__stl =  niel_proton;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            double R__t;
            R__b >> R__t;
            double R__t2;
            R__b >> R__t2;
            typedef double Value_t;
            std::pair<Value_t const, double > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         map<Double_t,Double_t> &R__stl =  niel_pion;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            double R__t;
            R__b >> R__t;
            double R__t2;
            R__b >> R__t2;
            typedef double Value_t;
            std::pair<Value_t const, double > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         map<Double_t,Double_t> &R__stl =  niel_electron;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            double R__t;
            R__b >> R__t;
            double R__t2;
            R__b >> R__t2;
            typedef double Value_t;
            std::pair<Value_t const, double > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fIAlpha;
      R__b >> fEGap0;
      R__b >> fEGapAlpha;
      R__b >> fEGapBeta;
      R__b >> fNeff0;
      R__b >> fNeffC;
      R__b >> fNeffGc;
      R__b >> fEpsilon;
      R__b.CheckByteCount(R__s, R__c, CbmRadDamage::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmRadDamage::IsA(), kTRUE);
      TObject::Streamer(R__b);
      {
         map<Double_t,Double_t> &R__stl =  niel_neutron;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<Double_t,Double_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      {
         map<Double_t,Double_t> &R__stl =  niel_proton;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<Double_t,Double_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      {
         map<Double_t,Double_t> &R__stl =  niel_pion;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<Double_t,Double_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      {
         map<Double_t,Double_t> &R__stl =  niel_electron;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<Double_t,Double_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fIAlpha;
      R__b << fEGap0;
      R__b << fEGapAlpha;
      R__b << fEGapBeta;
      R__b << fNeff0;
      R__b << fNeffC;
      R__b << fNeffGc;
      R__b << fEpsilon;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRadDamage(void *p) {
      return  p ? new(p) ::CbmRadDamage : new ::CbmRadDamage;
   }
   static void *newArray_CbmRadDamage(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRadDamage[nElements] : new ::CbmRadDamage[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRadDamage(void *p) {
      delete ((::CbmRadDamage*)p);
   }
   static void deleteArray_CbmRadDamage(void *p) {
      delete [] ((::CbmRadDamage*)p);
   }
   static void destruct_CbmRadDamage(void *p) {
      typedef ::CbmRadDamage current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmRadDamage(TBuffer &buf, void *obj) {
      ((::CbmRadDamage*)obj)->::CbmRadDamage::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmRadDamage

//______________________________________________________________________________
void CbmHistManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmHistManager.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      {
         map<string,TNamed*> &R__stl =  fMap;
         R__stl.clear();
         TClass *R__tcl2 = TBuffer::GetClass(typeid(class TNamed *));
         if (R__tcl2==0) {
            Error("fMap streamer","Missing the TClass object for class TNamed *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            string R__t;
            {TString R__str;
             R__str.Streamer(R__b);
             R__t = R__str.Data();}
            TNamed* R__t2;
            R__t2 = (TNamed*)R__b.ReadObjectAny(R__tcl2);
            typedef class std::__cxx11::basic_string<char> Value_t;
            std::pair<Value_t const, class TNamed * > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         vector<TCanvas*> &R__stl =  fCanvases;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TCanvas *));
         if (R__tcl1==0) {
            Error("fCanvases streamer","Missing the TClass object for class TCanvas *!");
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
      R__b.CheckByteCount(R__s, R__c, CbmHistManager::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmHistManager::IsA(), kTRUE);
      TObject::Streamer(R__b);
      {
         map<string,TNamed*> &R__stl =  fMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<string,TNamed*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {TString R__str(((*R__k).first ).c_str());
             R__str.Streamer(R__b);};
            R__b << ((*R__k).second);
            }
         }
      }
      {
         vector<TCanvas*> &R__stl =  fCanvases;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TCanvas*>::iterator R__k;
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
   static void *new_CbmHistManager(void *p) {
      return  p ? new(p) ::CbmHistManager : new ::CbmHistManager;
   }
   static void *newArray_CbmHistManager(Long_t nElements, void *p) {
      return p ? new(p) ::CbmHistManager[nElements] : new ::CbmHistManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmHistManager(void *p) {
      delete ((::CbmHistManager*)p);
   }
   static void deleteArray_CbmHistManager(void *p) {
      delete [] ((::CbmHistManager*)p);
   }
   static void destruct_CbmHistManager(void *p) {
      typedef ::CbmHistManager current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmHistManager(TBuffer &buf, void *obj) {
      ((::CbmHistManager*)obj)->::CbmHistManager::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmHistManager

//______________________________________________________________________________
void CbmMatchRecoToMC::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMatchRecoToMC.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMatchRecoToMC::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMatchRecoToMC::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMatchRecoToMC(void *p) {
      return  p ? new(p) ::CbmMatchRecoToMC : new ::CbmMatchRecoToMC;
   }
   static void *newArray_CbmMatchRecoToMC(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMatchRecoToMC[nElements] : new ::CbmMatchRecoToMC[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMatchRecoToMC(void *p) {
      delete ((::CbmMatchRecoToMC*)p);
   }
   static void deleteArray_CbmMatchRecoToMC(void *p) {
      delete [] ((::CbmMatchRecoToMC*)p);
   }
   static void destruct_CbmMatchRecoToMC(void *p) {
      typedef ::CbmMatchRecoToMC current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMatchRecoToMC

//______________________________________________________________________________
void CbmReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmReport::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmReport(void *p) {
      delete ((::CbmReport*)p);
   }
   static void deleteArray_CbmReport(void *p) {
      delete [] ((::CbmReport*)p);
   }
   static void destruct_CbmReport(void *p) {
      typedef ::CbmReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmReport

//______________________________________________________________________________
void CbmStudyReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStudyReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStudyReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStudyReport::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmStudyReport(void *p) {
      delete ((::CbmStudyReport*)p);
   }
   static void deleteArray_CbmStudyReport(void *p) {
      delete [] ((::CbmStudyReport*)p);
   }
   static void destruct_CbmStudyReport(void *p) {
      typedef ::CbmStudyReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStudyReport

//______________________________________________________________________________
void CbmSimulationReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmSimulationReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmSimulationReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmSimulationReport::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmSimulationReport(void *p) {
      delete ((::CbmSimulationReport*)p);
   }
   static void deleteArray_CbmSimulationReport(void *p) {
      delete [] ((::CbmSimulationReport*)p);
   }
   static void destruct_CbmSimulationReport(void *p) {
      typedef ::CbmSimulationReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmSimulationReport

//______________________________________________________________________________
void CbmReportElement::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmReportElement.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmReportElement::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmReportElement::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmReportElement(void *p) {
      delete ((::CbmReportElement*)p);
   }
   static void deleteArray_CbmReportElement(void *p) {
      delete [] ((::CbmReportElement*)p);
   }
   static void destruct_CbmReportElement(void *p) {
      typedef ::CbmReportElement current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmReportElement

//______________________________________________________________________________
void CbmTextReportElement::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTextReportElement.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTextReportElement::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTextReportElement::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTextReportElement(void *p) {
      return  p ? new(p) ::CbmTextReportElement : new ::CbmTextReportElement;
   }
   static void *newArray_CbmTextReportElement(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTextReportElement[nElements] : new ::CbmTextReportElement[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTextReportElement(void *p) {
      delete ((::CbmTextReportElement*)p);
   }
   static void deleteArray_CbmTextReportElement(void *p) {
      delete [] ((::CbmTextReportElement*)p);
   }
   static void destruct_CbmTextReportElement(void *p) {
      typedef ::CbmTextReportElement current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTextReportElement

//______________________________________________________________________________
void CbmHtmlReportElement::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmHtmlReportElement.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmHtmlReportElement::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmHtmlReportElement::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmHtmlReportElement(void *p) {
      return  p ? new(p) ::CbmHtmlReportElement : new ::CbmHtmlReportElement;
   }
   static void *newArray_CbmHtmlReportElement(Long_t nElements, void *p) {
      return p ? new(p) ::CbmHtmlReportElement[nElements] : new ::CbmHtmlReportElement[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmHtmlReportElement(void *p) {
      delete ((::CbmHtmlReportElement*)p);
   }
   static void deleteArray_CbmHtmlReportElement(void *p) {
      delete [] ((::CbmHtmlReportElement*)p);
   }
   static void destruct_CbmHtmlReportElement(void *p) {
      typedef ::CbmHtmlReportElement current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmHtmlReportElement

//______________________________________________________________________________
void CbmLatexReportElement::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLatexReportElement.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLatexReportElement::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLatexReportElement::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLatexReportElement(void *p) {
      return  p ? new(p) ::CbmLatexReportElement : new ::CbmLatexReportElement;
   }
   static void *newArray_CbmLatexReportElement(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLatexReportElement[nElements] : new ::CbmLatexReportElement[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLatexReportElement(void *p) {
      delete ((::CbmLatexReportElement*)p);
   }
   static void deleteArray_CbmLatexReportElement(void *p) {
      delete [] ((::CbmLatexReportElement*)p);
   }
   static void destruct_CbmLatexReportElement(void *p) {
      typedef ::CbmLatexReportElement current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLatexReportElement

namespace ROOT {
   // Wrappers around operator new
   static void *new_cbmcLcLmcbmcLcLToForceLibLoad(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::cbm::mcbm::ToForceLibLoad : new ::cbm::mcbm::ToForceLibLoad;
   }
   static void *newArray_cbmcLcLmcbmcLcLToForceLibLoad(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::cbm::mcbm::ToForceLibLoad[nElements] : new ::cbm::mcbm::ToForceLibLoad[nElements];
   }
   // Wrapper around operator delete
   static void delete_cbmcLcLmcbmcLcLToForceLibLoad(void *p) {
      delete ((::cbm::mcbm::ToForceLibLoad*)p);
   }
   static void deleteArray_cbmcLcLmcbmcLcLToForceLibLoad(void *p) {
      delete [] ((::cbm::mcbm::ToForceLibLoad*)p);
   }
   static void destruct_cbmcLcLmcbmcLcLToForceLibLoad(void *p) {
      typedef ::cbm::mcbm::ToForceLibLoad current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cbm::mcbm::ToForceLibLoad

//______________________________________________________________________________
void CbmMediaList::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMediaList.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      {
         vector<pair<TString,TString> > &R__stl =  fMatList;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(struct std::pair<class TString, class TString>));
         if (R__tcl1==0) {
            Error("fMatList streamer","Missing the TClass object for struct std::pair<class TString, class TString>!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            pair<TString,TString> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, CbmMediaList::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMediaList::IsA(), kTRUE);
      TObject::Streamer(R__b);
      {
         vector<pair<TString,TString> > &R__stl =  fMatList;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(struct std::pair<class TString, class TString>));
         if (R__tcl1==0) {
            Error("fMatList streamer","Missing the TClass object for struct std::pair<class TString, class TString>!");
            return;
         }
            vector<pair<TString,TString> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((pair<TString,TString>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMediaList(void *p) {
      return  p ? new(p) ::CbmMediaList : new ::CbmMediaList;
   }
   static void *newArray_CbmMediaList(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMediaList[nElements] : new ::CbmMediaList[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMediaList(void *p) {
      delete ((::CbmMediaList*)p);
   }
   static void deleteArray_CbmMediaList(void *p) {
      delete [] ((::CbmMediaList*)p);
   }
   static void destruct_CbmMediaList(void *p) {
      typedef ::CbmMediaList current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMediaList(TBuffer &buf, void *obj) {
      ((::CbmMediaList*)obj)->::CbmMediaList::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMediaList

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
   static TClass *vectorlEpairlETStringcOTStringgRsPgR_Dictionary();
   static void vectorlEpairlETStringcOTStringgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlETStringcOTStringgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEpairlETStringcOTStringgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlETStringcOTStringgRsPgR(void *p);
   static void deleteArray_vectorlEpairlETStringcOTStringgRsPgR(void *p);
   static void destruct_vectorlEpairlETStringcOTStringgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<TString,TString> >*)
   {
      vector<pair<TString,TString> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<TString,TString> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<TString,TString> >", -2, "vector", 389,
                  typeid(vector<pair<TString,TString> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlETStringcOTStringgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<TString,TString> >) );
      instance.SetNew(&new_vectorlEpairlETStringcOTStringgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlETStringcOTStringgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlETStringcOTStringgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlETStringcOTStringgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlETStringcOTStringgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<TString,TString> > >()));

      ::ROOT::AddClassAlternate("vector<pair<TString,TString> >","std::vector<std::pair<TString, TString>, std::allocator<std::pair<TString, TString> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pair<TString,TString> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlETStringcOTStringgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<TString,TString> >*)nullptr)->GetClass();
      vectorlEpairlETStringcOTStringgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlETStringcOTStringgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlETStringcOTStringgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<TString,TString> > : new vector<pair<TString,TString> >;
   }
   static void *newArray_vectorlEpairlETStringcOTStringgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<TString,TString> >[nElements] : new vector<pair<TString,TString> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlETStringcOTStringgRsPgR(void *p) {
      delete ((vector<pair<TString,TString> >*)p);
   }
   static void deleteArray_vectorlEpairlETStringcOTStringgRsPgR(void *p) {
      delete [] ((vector<pair<TString,TString> >*)p);
   }
   static void destruct_vectorlEpairlETStringcOTStringgRsPgR(void *p) {
      typedef vector<pair<TString,TString> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<TString,TString> >

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
   static TClass *vectorlECbmHistManagermUgR_Dictionary();
   static void vectorlECbmHistManagermUgR_TClassManip(TClass*);
   static void *new_vectorlECbmHistManagermUgR(void *p = nullptr);
   static void *newArray_vectorlECbmHistManagermUgR(Long_t size, void *p);
   static void delete_vectorlECbmHistManagermUgR(void *p);
   static void deleteArray_vectorlECbmHistManagermUgR(void *p);
   static void destruct_vectorlECbmHistManagermUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmHistManager*>*)
   {
      vector<CbmHistManager*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmHistManager*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmHistManager*>", -2, "vector", 389,
                  typeid(vector<CbmHistManager*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmHistManagermUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmHistManager*>) );
      instance.SetNew(&new_vectorlECbmHistManagermUgR);
      instance.SetNewArray(&newArray_vectorlECbmHistManagermUgR);
      instance.SetDelete(&delete_vectorlECbmHistManagermUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmHistManagermUgR);
      instance.SetDestructor(&destruct_vectorlECbmHistManagermUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmHistManager*> >()));

      ::ROOT::AddClassAlternate("vector<CbmHistManager*>","std::vector<CbmHistManager*, std::allocator<CbmHistManager*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmHistManager*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmHistManagermUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmHistManager*>*)nullptr)->GetClass();
      vectorlECbmHistManagermUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmHistManagermUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmHistManagermUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmHistManager*> : new vector<CbmHistManager*>;
   }
   static void *newArray_vectorlECbmHistManagermUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmHistManager*>[nElements] : new vector<CbmHistManager*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmHistManagermUgR(void *p) {
      delete ((vector<CbmHistManager*>*)p);
   }
   static void deleteArray_vectorlECbmHistManagermUgR(void *p) {
      delete [] ((vector<CbmHistManager*>*)p);
   }
   static void destruct_vectorlECbmHistManagermUgR(void *p) {
      typedef vector<CbmHistManager*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmHistManager*>

namespace ROOT {
   static TClass *setlEunsignedsPintgR_Dictionary();
   static void setlEunsignedsPintgR_TClassManip(TClass*);
   static void *new_setlEunsignedsPintgR(void *p = nullptr);
   static void *newArray_setlEunsignedsPintgR(Long_t size, void *p);
   static void delete_setlEunsignedsPintgR(void *p);
   static void deleteArray_setlEunsignedsPintgR(void *p);
   static void destruct_setlEunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const set<unsigned int>*)
   {
      set<unsigned int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(set<unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("set<unsigned int>", -2, "set", 94,
                  typeid(set<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &setlEunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(set<unsigned int>) );
      instance.SetNew(&new_setlEunsignedsPintgR);
      instance.SetNewArray(&newArray_setlEunsignedsPintgR);
      instance.SetDelete(&delete_setlEunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_setlEunsignedsPintgR);
      instance.SetDestructor(&destruct_setlEunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Insert< set<unsigned int> >()));

      ::ROOT::AddClassAlternate("set<unsigned int>","std::set<unsigned int, std::less<unsigned int>, std::allocator<unsigned int> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const set<unsigned int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *setlEunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const set<unsigned int>*)nullptr)->GetClass();
      setlEunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void setlEunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_setlEunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<unsigned int> : new set<unsigned int>;
   }
   static void *newArray_setlEunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<unsigned int>[nElements] : new set<unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_setlEunsignedsPintgR(void *p) {
      delete ((set<unsigned int>*)p);
   }
   static void deleteArray_setlEunsignedsPintgR(void *p) {
      delete [] ((set<unsigned int>*)p);
   }
   static void destruct_setlEunsignedsPintgR(void *p) {
      typedef set<unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class set<unsigned int>

namespace ROOT {
   static TClass *maplEstringcOTNamedmUgR_Dictionary();
   static void maplEstringcOTNamedmUgR_TClassManip(TClass*);
   static void *new_maplEstringcOTNamedmUgR(void *p = nullptr);
   static void *newArray_maplEstringcOTNamedmUgR(Long_t size, void *p);
   static void delete_maplEstringcOTNamedmUgR(void *p);
   static void deleteArray_maplEstringcOTNamedmUgR(void *p);
   static void destruct_maplEstringcOTNamedmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,TNamed*>*)
   {
      map<string,TNamed*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,TNamed*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,TNamed*>", -2, "map", 100,
                  typeid(map<string,TNamed*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOTNamedmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,TNamed*>) );
      instance.SetNew(&new_maplEstringcOTNamedmUgR);
      instance.SetNewArray(&newArray_maplEstringcOTNamedmUgR);
      instance.SetDelete(&delete_maplEstringcOTNamedmUgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOTNamedmUgR);
      instance.SetDestructor(&destruct_maplEstringcOTNamedmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,TNamed*> >()));

      ::ROOT::AddClassAlternate("map<string,TNamed*>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, TNamed*, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, TNamed*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,TNamed*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOTNamedmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,TNamed*>*)nullptr)->GetClass();
      maplEstringcOTNamedmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOTNamedmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOTNamedmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,TNamed*> : new map<string,TNamed*>;
   }
   static void *newArray_maplEstringcOTNamedmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,TNamed*>[nElements] : new map<string,TNamed*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOTNamedmUgR(void *p) {
      delete ((map<string,TNamed*>*)p);
   }
   static void deleteArray_maplEstringcOTNamedmUgR(void *p) {
      delete [] ((map<string,TNamed*>*)p);
   }
   static void destruct_maplEstringcOTNamedmUgR(void *p) {
      typedef map<string,TNamed*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,TNamed*>

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

namespace ROOT {
   static TClass *maplEECbmModuleIdcOstringgR_Dictionary();
   static void maplEECbmModuleIdcOstringgR_TClassManip(TClass*);
   static void *new_maplEECbmModuleIdcOstringgR(void *p = nullptr);
   static void *newArray_maplEECbmModuleIdcOstringgR(Long_t size, void *p);
   static void delete_maplEECbmModuleIdcOstringgR(void *p);
   static void deleteArray_maplEECbmModuleIdcOstringgR(void *p);
   static void destruct_maplEECbmModuleIdcOstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<ECbmModuleId,string>*)
   {
      map<ECbmModuleId,string> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<ECbmModuleId,string>));
      static ::ROOT::TGenericClassInfo 
         instance("map<ECbmModuleId,string>", -2, "map", 100,
                  typeid(map<ECbmModuleId,string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEECbmModuleIdcOstringgR_Dictionary, isa_proxy, 0,
                  sizeof(map<ECbmModuleId,string>) );
      instance.SetNew(&new_maplEECbmModuleIdcOstringgR);
      instance.SetNewArray(&newArray_maplEECbmModuleIdcOstringgR);
      instance.SetDelete(&delete_maplEECbmModuleIdcOstringgR);
      instance.SetDeleteArray(&deleteArray_maplEECbmModuleIdcOstringgR);
      instance.SetDestructor(&destruct_maplEECbmModuleIdcOstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<ECbmModuleId,string> >()));

      ::ROOT::AddClassAlternate("map<ECbmModuleId,string>","std::map<ECbmModuleId, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<ECbmModuleId>, std::allocator<std::pair<ECbmModuleId const, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<ECbmModuleId,string>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEECbmModuleIdcOstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<ECbmModuleId,string>*)nullptr)->GetClass();
      maplEECbmModuleIdcOstringgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEECbmModuleIdcOstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEECbmModuleIdcOstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,string> : new map<ECbmModuleId,string>;
   }
   static void *newArray_maplEECbmModuleIdcOstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,string>[nElements] : new map<ECbmModuleId,string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEECbmModuleIdcOstringgR(void *p) {
      delete ((map<ECbmModuleId,string>*)p);
   }
   static void deleteArray_maplEECbmModuleIdcOstringgR(void *p) {
      delete [] ((map<ECbmModuleId,string>*)p);
   }
   static void destruct_maplEECbmModuleIdcOstringgR(void *p) {
      typedef map<ECbmModuleId,string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<ECbmModuleId,string>

namespace {
  void TriggerDictionaryInitialization_libCbmBase_Impl() {
    static const char* headers[] = {
"CbmDigiManager.h",
"CbmDigitizeBase.h",
"CbmDaq.h",
"CbmMCDataArray.h",
"CbmMCDataManager.h",
"CbmMCDataObject.h",
"CbmRadDamage.h",
"CbmHistManager.h",
"CbmMatchRecoToMC.h",
"CbmTrackingDetectorInterfaceBase.h",
"CbmDrawHist.h",
"CbmReport.h",
"CbmStudyReport.h",
"CbmSimulationReport.h",
"CbmTextReportElement.h",
"CbmHtmlReportElement.h",
"CbmLatexReportElement.h",
"CbmUtils.h",
"CbmGeometryUtils.h",
"CbmMcbmUtils.h",
"CbmMediaList.h",
"CbmFileUtils.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
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
"/home/heinemann/gpuunpacker/cbmroot/build2/core/base/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmBase dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$TString.h")))  __attribute__((annotate("$clingAutoload$CbmDigiManager.h")))  TString;
namespace std{template <typename _T1, typename _T2> struct __attribute__((annotate("$clingAutoload$bits/stl_iterator.h")))  __attribute__((annotate("$clingAutoload$string")))  pair;
}
class __attribute__((annotate("$clingAutoload$CbmDigiManager.h")))  CbmDigiManager;
class __attribute__((annotate("$clingAutoload$CbmDigitizeBase.h")))  CbmDigitizeBase;
class __attribute__((annotate("$clingAutoload$CbmDaq.h")))  CbmDaq;
class __attribute__((annotate("$clingAutoload$CbmMCDataArray.h")))  CbmMCDataArray;
class __attribute__((annotate("$clingAutoload$CbmMCDataManager.h")))  CbmMCDataManager;
class __attribute__((annotate("$clingAutoload$CbmMCDataObject.h")))  CbmMCDataObject;
class __attribute__((annotate("$clingAutoload$CbmRadDamage.h")))  CbmRadDamage;
class __attribute__((annotate("$clingAutoload$CbmHistManager.h")))  CbmHistManager;
class __attribute__((annotate("$clingAutoload$CbmMatchRecoToMC.h")))  CbmMatchRecoToMC;
class __attribute__((annotate("$clingAutoload$CbmReport.h")))  CbmReport;
class __attribute__((annotate("$clingAutoload$CbmStudyReport.h")))  CbmStudyReport;
class __attribute__((annotate("$clingAutoload$CbmSimulationReport.h")))  CbmSimulationReport;
class __attribute__((annotate("$clingAutoload$CbmReportElement.h")))  __attribute__((annotate("$clingAutoload$CbmTextReportElement.h")))  CbmReportElement;
class __attribute__((annotate("$clingAutoload$CbmTextReportElement.h")))  CbmTextReportElement;
class __attribute__((annotate("$clingAutoload$CbmHtmlReportElement.h")))  CbmHtmlReportElement;
class __attribute__((annotate("$clingAutoload$CbmLatexReportElement.h")))  CbmLatexReportElement;
namespace cbm{namespace mcbm{class __attribute__((annotate("$clingAutoload$CbmMcbmUtils.h")))  ToForceLibLoad;}}
class __attribute__((annotate("$clingAutoload$CbmMediaList.h")))  CbmMediaList;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmBase dictionary payload"

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
#include "CbmDigiManager.h"
#include "CbmDigitizeBase.h"
#include "CbmDaq.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCDataObject.h"
#include "CbmRadDamage.h"
#include "CbmHistManager.h"
#include "CbmMatchRecoToMC.h"
#include "CbmTrackingDetectorInterfaceBase.h"
#include "CbmDrawHist.h"
#include "CbmReport.h"
#include "CbmStudyReport.h"
#include "CbmSimulationReport.h"
#include "CbmTextReportElement.h"
#include "CbmHtmlReportElement.h"
#include "CbmLatexReportElement.h"
#include "CbmUtils.h"
#include "CbmGeometryUtils.h"
#include "CbmMcbmUtils.h"
#include "CbmMediaList.h"
#include "CbmFileUtils.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"Cbm::File::IsRootFile", payloadCode, "@",
"CbmDaq", payloadCode, "@",
"CbmDigiManager", payloadCode, "@",
"CbmDigitizeBase", payloadCode, "@",
"CbmHistManager", payloadCode, "@",
"CbmHtmlReportElement", payloadCode, "@",
"CbmLatexReportElement", payloadCode, "@",
"CbmMCDataArray", payloadCode, "@",
"CbmMCDataManager", payloadCode, "@",
"CbmMCDataObject", payloadCode, "@",
"CbmMatchRecoToMC", payloadCode, "@",
"CbmMediaList", payloadCode, "@",
"CbmRadDamage", payloadCode, "@",
"CbmReport", payloadCode, "@",
"CbmReportElement", payloadCode, "@",
"CbmSimulationReport", payloadCode, "@",
"CbmStudyReport", payloadCode, "@",
"CbmTextReportElement", payloadCode, "@",
"DrawGraph", payloadCode, "@",
"DrawGraph2D", payloadCode, "@",
"DrawH1", payloadCode, "@",
"DrawH2", payloadCode, "@",
"SetDefaultDrawStyle", payloadCode, "@",
"cbm::mcbm::ToForceLibLoad", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmBase",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmBase_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmBase_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmBase() {
  TriggerDictionaryInitialization_libCbmBase_Impl();
}
