// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIheinemanndIgpuunpackerdIcbmrootdIbuild2dIexternaldINICAFEMTOmIprefixdIsrcdINICAFEMTOmIbuilddIanalysisdIfluctuationsdIG__NicaFluctDict
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
#include "NicaChargedFluctuationsAna.h"
#include "NicaFluctuation1D.h"
#include "NicaFluctuationsAna.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_NicaChargedFluctuationsAna(void *p = nullptr);
   static void *newArray_NicaChargedFluctuationsAna(Long_t size, void *p);
   static void delete_NicaChargedFluctuationsAna(void *p);
   static void deleteArray_NicaChargedFluctuationsAna(void *p);
   static void destruct_NicaChargedFluctuationsAna(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaChargedFluctuationsAna*)
   {
      ::NicaChargedFluctuationsAna *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaChargedFluctuationsAna >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaChargedFluctuationsAna", ::NicaChargedFluctuationsAna::Class_Version(), "NicaChargedFluctuationsAna.h", 23,
                  typeid(::NicaChargedFluctuationsAna), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaChargedFluctuationsAna::Dictionary, isa_proxy, 4,
                  sizeof(::NicaChargedFluctuationsAna) );
      instance.SetNew(&new_NicaChargedFluctuationsAna);
      instance.SetNewArray(&newArray_NicaChargedFluctuationsAna);
      instance.SetDelete(&delete_NicaChargedFluctuationsAna);
      instance.SetDeleteArray(&deleteArray_NicaChargedFluctuationsAna);
      instance.SetDestructor(&destruct_NicaChargedFluctuationsAna);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaChargedFluctuationsAna*)
   {
      return GenerateInitInstanceLocal((::NicaChargedFluctuationsAna*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaChargedFluctuationsAna*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFluctuation1D(void *p = nullptr);
   static void *newArray_NicaFluctuation1D(Long_t size, void *p);
   static void delete_NicaFluctuation1D(void *p);
   static void deleteArray_NicaFluctuation1D(void *p);
   static void destruct_NicaFluctuation1D(void *p);
   static Long64_t merge_NicaFluctuation1D(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFluctuation1D*)
   {
      ::NicaFluctuation1D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFluctuation1D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFluctuation1D", ::NicaFluctuation1D::Class_Version(), "NicaFluctuation1D.h", 21,
                  typeid(::NicaFluctuation1D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFluctuation1D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFluctuation1D) );
      instance.SetNew(&new_NicaFluctuation1D);
      instance.SetNewArray(&newArray_NicaFluctuation1D);
      instance.SetDelete(&delete_NicaFluctuation1D);
      instance.SetDeleteArray(&deleteArray_NicaFluctuation1D);
      instance.SetDestructor(&destruct_NicaFluctuation1D);
      instance.SetMerge(&merge_NicaFluctuation1D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFluctuation1D*)
   {
      return GenerateInitInstanceLocal((::NicaFluctuation1D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFluctuation1D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFluctuationsAna(void *p = nullptr);
   static void *newArray_NicaFluctuationsAna(Long_t size, void *p);
   static void delete_NicaFluctuationsAna(void *p);
   static void deleteArray_NicaFluctuationsAna(void *p);
   static void destruct_NicaFluctuationsAna(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFluctuationsAna*)
   {
      ::NicaFluctuationsAna *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFluctuationsAna >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFluctuationsAna", ::NicaFluctuationsAna::Class_Version(), "NicaFluctuationsAna.h", 19,
                  typeid(::NicaFluctuationsAna), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFluctuationsAna::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFluctuationsAna) );
      instance.SetNew(&new_NicaFluctuationsAna);
      instance.SetNewArray(&newArray_NicaFluctuationsAna);
      instance.SetDelete(&delete_NicaFluctuationsAna);
      instance.SetDeleteArray(&deleteArray_NicaFluctuationsAna);
      instance.SetDestructor(&destruct_NicaFluctuationsAna);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFluctuationsAna*)
   {
      return GenerateInitInstanceLocal((::NicaFluctuationsAna*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFluctuationsAna*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NicaChargedFluctuationsAna::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaChargedFluctuationsAna::Class_Name()
{
   return "NicaChargedFluctuationsAna";
}

//______________________________________________________________________________
const char *NicaChargedFluctuationsAna::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaChargedFluctuationsAna*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaChargedFluctuationsAna::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaChargedFluctuationsAna*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaChargedFluctuationsAna::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaChargedFluctuationsAna*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaChargedFluctuationsAna::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaChargedFluctuationsAna*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFluctuation1D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFluctuation1D::Class_Name()
{
   return "NicaFluctuation1D";
}

//______________________________________________________________________________
const char *NicaFluctuation1D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFluctuation1D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFluctuation1D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFluctuation1D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFluctuation1D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFluctuation1D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFluctuation1D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFluctuation1D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFluctuationsAna::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFluctuationsAna::Class_Name()
{
   return "NicaFluctuationsAna";
}

//______________________________________________________________________________
const char *NicaFluctuationsAna::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFluctuationsAna*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFluctuationsAna::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFluctuationsAna*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFluctuationsAna::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFluctuationsAna*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFluctuationsAna::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFluctuationsAna*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void NicaChargedFluctuationsAna::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaChargedFluctuationsAna.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaChargedFluctuationsAna::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaChargedFluctuationsAna::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaChargedFluctuationsAna(void *p) {
      return  p ? new(p) ::NicaChargedFluctuationsAna : new ::NicaChargedFluctuationsAna;
   }
   static void *newArray_NicaChargedFluctuationsAna(Long_t nElements, void *p) {
      return p ? new(p) ::NicaChargedFluctuationsAna[nElements] : new ::NicaChargedFluctuationsAna[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaChargedFluctuationsAna(void *p) {
      delete ((::NicaChargedFluctuationsAna*)p);
   }
   static void deleteArray_NicaChargedFluctuationsAna(void *p) {
      delete [] ((::NicaChargedFluctuationsAna*)p);
   }
   static void destruct_NicaChargedFluctuationsAna(void *p) {
      typedef ::NicaChargedFluctuationsAna current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaChargedFluctuationsAna

//______________________________________________________________________________
void NicaFluctuation1D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFluctuation1D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFluctuation1D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFluctuation1D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFluctuation1D(void *p) {
      return  p ? new(p) ::NicaFluctuation1D : new ::NicaFluctuation1D;
   }
   static void *newArray_NicaFluctuation1D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFluctuation1D[nElements] : new ::NicaFluctuation1D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFluctuation1D(void *p) {
      delete ((::NicaFluctuation1D*)p);
   }
   static void deleteArray_NicaFluctuation1D(void *p) {
      delete [] ((::NicaFluctuation1D*)p);
   }
   static void destruct_NicaFluctuation1D(void *p) {
      typedef ::NicaFluctuation1D current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaFluctuation1D(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaFluctuation1D*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaFluctuation1D

//______________________________________________________________________________
void NicaFluctuationsAna::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFluctuationsAna.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFluctuationsAna::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFluctuationsAna::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFluctuationsAna(void *p) {
      return  p ? new(p) ::NicaFluctuationsAna : new ::NicaFluctuationsAna;
   }
   static void *newArray_NicaFluctuationsAna(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFluctuationsAna[nElements] : new ::NicaFluctuationsAna[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFluctuationsAna(void *p) {
      delete ((::NicaFluctuationsAna*)p);
   }
   static void deleteArray_NicaFluctuationsAna(void *p) {
      delete [] ((::NicaFluctuationsAna*)p);
   }
   static void destruct_NicaFluctuationsAna(void *p) {
      typedef ::NicaFluctuationsAna current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFluctuationsAna

namespace {
  void TriggerDictionaryInitialization_G__NicaFluctDict_Impl() {
    static const char* headers[] = {
"NicaChargedFluctuationsAna.h",
"NicaFluctuation1D.h",
"NicaFluctuationsAna.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/detector",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/compound",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/hiddeninfo",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/hist",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io/html",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/std",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/minimizer",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/chains",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly/smear_codes",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/eventcuts",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/detector",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/mc",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/resolution",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/detector",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/mc",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/cutmonitors",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/fluctuations",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/analysis/fluctuations/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__NicaFluctDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$NicaChargedFluctuationsAna.h")))  NicaChargedFluctuationsAna;
class __attribute__((annotate("$clingAutoload$NicaFluctuation1D.h")))  NicaFluctuation1D;
class __attribute__((annotate("$clingAutoload$NicaFluctuationsAna.h")))  NicaFluctuationsAna;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__NicaFluctDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "NicaChargedFluctuationsAna.h"
#include "NicaFluctuation1D.h"
#include "NicaFluctuationsAna.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"NicaChargedFluctuationsAna", payloadCode, "@",
"NicaFluctuation1D", payloadCode, "@",
"NicaFluctuationsAna", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__NicaFluctDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__NicaFluctDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__NicaFluctDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__NicaFluctDict() {
  TriggerDictionaryInitialization_G__NicaFluctDict_Impl();
}
