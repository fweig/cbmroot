// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIheinemanndIgpuunpackerdIcbmrootdIbuild2dIexternaldINICAFEMTOmIprefixdIsrcdINICAFEMTOmIbuilddIanalysisdIspectradIG__NicaSpectraDict
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
#include "NicaSpectraAna.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_NicaSpectraAna(void *p = nullptr);
   static void *newArray_NicaSpectraAna(Long_t size, void *p);
   static void delete_NicaSpectraAna(void *p);
   static void deleteArray_NicaSpectraAna(void *p);
   static void destruct_NicaSpectraAna(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaSpectraAna*)
   {
      ::NicaSpectraAna *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaSpectraAna >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaSpectraAna", ::NicaSpectraAna::Class_Version(), "NicaSpectraAna.h", 21,
                  typeid(::NicaSpectraAna), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaSpectraAna::Dictionary, isa_proxy, 4,
                  sizeof(::NicaSpectraAna) );
      instance.SetNew(&new_NicaSpectraAna);
      instance.SetNewArray(&newArray_NicaSpectraAna);
      instance.SetDelete(&delete_NicaSpectraAna);
      instance.SetDeleteArray(&deleteArray_NicaSpectraAna);
      instance.SetDestructor(&destruct_NicaSpectraAna);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaSpectraAna*)
   {
      return GenerateInitInstanceLocal((::NicaSpectraAna*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaSpectraAna*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NicaSpectraAna::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaSpectraAna::Class_Name()
{
   return "NicaSpectraAna";
}

//______________________________________________________________________________
const char *NicaSpectraAna::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSpectraAna*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaSpectraAna::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSpectraAna*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaSpectraAna::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSpectraAna*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaSpectraAna::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSpectraAna*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void NicaSpectraAna::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaSpectraAna.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaSpectraAna::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaSpectraAna::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaSpectraAna(void *p) {
      return  p ? new(p) ::NicaSpectraAna : new ::NicaSpectraAna;
   }
   static void *newArray_NicaSpectraAna(Long_t nElements, void *p) {
      return p ? new(p) ::NicaSpectraAna[nElements] : new ::NicaSpectraAna[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaSpectraAna(void *p) {
      delete ((::NicaSpectraAna*)p);
   }
   static void deleteArray_NicaSpectraAna(void *p) {
      delete [] ((::NicaSpectraAna*)p);
   }
   static void destruct_NicaSpectraAna(void *p) {
      typedef ::NicaSpectraAna current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaSpectraAna

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

namespace {
  void TriggerDictionaryInitialization_G__NicaSpectraDict_Impl() {
    static const char* headers[] = {
"NicaSpectraAna.h",
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
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/spectra",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/analysis/spectra/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__NicaSpectraDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$NicaSpectraAna.h")))  NicaSpectraAna;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__NicaSpectraDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "NicaSpectraAna.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"NicaSpectraAna", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__NicaSpectraDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__NicaSpectraDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__NicaSpectraDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__NicaSpectraDict() {
  TriggerDictionaryInitialization_G__NicaSpectraDict_Impl();
}
