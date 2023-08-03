// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__AnalysisTreeKfpfInterface
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
#include "ATKFParticleFinder.h"
#include "CutsContainer.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *CutsContainer_Dictionary();
   static void CutsContainer_TClassManip(TClass*);
   static void *new_CutsContainer(void *p = nullptr);
   static void *newArray_CutsContainer(Long_t size, void *p);
   static void delete_CutsContainer(void *p);
   static void deleteArray_CutsContainer(void *p);
   static void destruct_CutsContainer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CutsContainer*)
   {
      ::CutsContainer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CutsContainer));
      static ::ROOT::TGenericClassInfo 
         instance("CutsContainer", "CutsContainer.h", 16,
                  typeid(::CutsContainer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CutsContainer_Dictionary, isa_proxy, 4,
                  sizeof(::CutsContainer) );
      instance.SetNew(&new_CutsContainer);
      instance.SetNewArray(&newArray_CutsContainer);
      instance.SetDelete(&delete_CutsContainer);
      instance.SetDeleteArray(&deleteArray_CutsContainer);
      instance.SetDestructor(&destruct_CutsContainer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CutsContainer*)
   {
      return GenerateInitInstanceLocal((::CutsContainer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CutsContainer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CutsContainer_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CutsContainer*)nullptr)->GetClass();
      CutsContainer_TClassManip(theClass);
   return theClass;
   }

   static void CutsContainer_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ATKFParticleFinder_Dictionary();
   static void ATKFParticleFinder_TClassManip(TClass*);
   static void *new_ATKFParticleFinder(void *p = nullptr);
   static void *newArray_ATKFParticleFinder(Long_t size, void *p);
   static void delete_ATKFParticleFinder(void *p);
   static void deleteArray_ATKFParticleFinder(void *p);
   static void destruct_ATKFParticleFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ATKFParticleFinder*)
   {
      ::ATKFParticleFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ATKFParticleFinder));
      static ::ROOT::TGenericClassInfo 
         instance("ATKFParticleFinder", "ATKFParticleFinder.h", 22,
                  typeid(::ATKFParticleFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ATKFParticleFinder_Dictionary, isa_proxy, 4,
                  sizeof(::ATKFParticleFinder) );
      instance.SetNew(&new_ATKFParticleFinder);
      instance.SetNewArray(&newArray_ATKFParticleFinder);
      instance.SetDelete(&delete_ATKFParticleFinder);
      instance.SetDeleteArray(&deleteArray_ATKFParticleFinder);
      instance.SetDestructor(&destruct_ATKFParticleFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ATKFParticleFinder*)
   {
      return GenerateInitInstanceLocal((::ATKFParticleFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ATKFParticleFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ATKFParticleFinder_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ATKFParticleFinder*)nullptr)->GetClass();
      ATKFParticleFinder_TClassManip(theClass);
   return theClass;
   }

   static void ATKFParticleFinder_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_CutsContainer(void *p) {
      return  p ? new(p) ::CutsContainer : new ::CutsContainer;
   }
   static void *newArray_CutsContainer(Long_t nElements, void *p) {
      return p ? new(p) ::CutsContainer[nElements] : new ::CutsContainer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CutsContainer(void *p) {
      delete ((::CutsContainer*)p);
   }
   static void deleteArray_CutsContainer(void *p) {
      delete [] ((::CutsContainer*)p);
   }
   static void destruct_CutsContainer(void *p) {
      typedef ::CutsContainer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CutsContainer

namespace ROOT {
   // Wrappers around operator new
   static void *new_ATKFParticleFinder(void *p) {
      return  p ? new(p) ::ATKFParticleFinder : new ::ATKFParticleFinder;
   }
   static void *newArray_ATKFParticleFinder(Long_t nElements, void *p) {
      return p ? new(p) ::ATKFParticleFinder[nElements] : new ::ATKFParticleFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_ATKFParticleFinder(void *p) {
      delete ((::ATKFParticleFinder*)p);
   }
   static void deleteArray_ATKFParticleFinder(void *p) {
      delete [] ((::ATKFParticleFinder*)p);
   }
   static void destruct_ATKFParticleFinder(void *p) {
      typedef ::ATKFParticleFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ATKFParticleFinder

namespace {
  void TriggerDictionaryInitialization_libAnalysisTreeKfpfInterface_Impl() {
    static const char* headers[] = {
"ATKFParticleFinder.h",
"CutsContainer.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/common/at_kfpf_interface",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/common/at_kfpf_interface",
"/home/heinemann/gpuunpacker/cbmroot/build2/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/common/at_kfpf_interface/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libAnalysisTreeKfpfInterface dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CutsContainer.h")))  __attribute__((annotate("$clingAutoload$ATKFParticleFinder.h")))  CutsContainer;
class __attribute__((annotate("$clingAutoload$ATKFParticleFinder.h")))  ATKFParticleFinder;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libAnalysisTreeKfpfInterface dictionary payload"

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

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "ATKFParticleFinder.h"
#include "CutsContainer.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"ATKFParticleFinder", payloadCode, "@",
"CutsContainer", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libAnalysisTreeKfpfInterface",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libAnalysisTreeKfpfInterface_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libAnalysisTreeKfpfInterface_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libAnalysisTreeKfpfInterface() {
  TriggerDictionaryInitialization_libAnalysisTreeKfpfInterface_Impl();
}
