// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIheinemanndIgpuunpackerdIcbmrootdIbuild2dIexternaldINICAFEMTOmIprefixdIsrcdINICAFEMTOmIbuilddIanalysisdIflowdIG__NicaFlowDict
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
#include "NicaFlowPtVariable.h"
#include "NicaFlowVariable.h"
#include "NicaFlowYVariable.h"
#include "NicaSimpleVnAnalysis.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_NicaFlowVariable(void *p);
   static void deleteArray_NicaFlowVariable(void *p);
   static void destruct_NicaFlowVariable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFlowVariable*)
   {
      ::NicaFlowVariable *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFlowVariable >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFlowVariable", ::NicaFlowVariable::Class_Version(), "NicaFlowVariable.h", 22,
                  typeid(::NicaFlowVariable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFlowVariable::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFlowVariable) );
      instance.SetDelete(&delete_NicaFlowVariable);
      instance.SetDeleteArray(&deleteArray_NicaFlowVariable);
      instance.SetDestructor(&destruct_NicaFlowVariable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFlowVariable*)
   {
      return GenerateInitInstanceLocal((::NicaFlowVariable*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFlowVariable*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFlowVirtualVariable(void *p = nullptr);
   static void *newArray_NicaFlowVirtualVariable(Long_t size, void *p);
   static void delete_NicaFlowVirtualVariable(void *p);
   static void deleteArray_NicaFlowVirtualVariable(void *p);
   static void destruct_NicaFlowVirtualVariable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFlowVirtualVariable*)
   {
      ::NicaFlowVirtualVariable *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFlowVirtualVariable >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFlowVirtualVariable", ::NicaFlowVirtualVariable::Class_Version(), "NicaFlowVariable.h", 57,
                  typeid(::NicaFlowVirtualVariable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFlowVirtualVariable::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFlowVirtualVariable) );
      instance.SetNew(&new_NicaFlowVirtualVariable);
      instance.SetNewArray(&newArray_NicaFlowVirtualVariable);
      instance.SetDelete(&delete_NicaFlowVirtualVariable);
      instance.SetDeleteArray(&deleteArray_NicaFlowVirtualVariable);
      instance.SetDestructor(&destruct_NicaFlowVirtualVariable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFlowVirtualVariable*)
   {
      return GenerateInitInstanceLocal((::NicaFlowVirtualVariable*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFlowVirtualVariable*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFlowPtVariable(void *p = nullptr);
   static void *newArray_NicaFlowPtVariable(Long_t size, void *p);
   static void delete_NicaFlowPtVariable(void *p);
   static void deleteArray_NicaFlowPtVariable(void *p);
   static void destruct_NicaFlowPtVariable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFlowPtVariable*)
   {
      ::NicaFlowPtVariable *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFlowPtVariable >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFlowPtVariable", ::NicaFlowPtVariable::Class_Version(), "NicaFlowPtVariable.h", 17,
                  typeid(::NicaFlowPtVariable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFlowPtVariable::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFlowPtVariable) );
      instance.SetNew(&new_NicaFlowPtVariable);
      instance.SetNewArray(&newArray_NicaFlowPtVariable);
      instance.SetDelete(&delete_NicaFlowPtVariable);
      instance.SetDeleteArray(&deleteArray_NicaFlowPtVariable);
      instance.SetDestructor(&destruct_NicaFlowPtVariable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFlowPtVariable*)
   {
      return GenerateInitInstanceLocal((::NicaFlowPtVariable*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFlowPtVariable*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFlowYVariable(void *p = nullptr);
   static void *newArray_NicaFlowYVariable(Long_t size, void *p);
   static void delete_NicaFlowYVariable(void *p);
   static void deleteArray_NicaFlowYVariable(void *p);
   static void destruct_NicaFlowYVariable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFlowYVariable*)
   {
      ::NicaFlowYVariable *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFlowYVariable >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFlowYVariable", ::NicaFlowYVariable::Class_Version(), "NicaFlowYVariable.h", 17,
                  typeid(::NicaFlowYVariable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFlowYVariable::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFlowYVariable) );
      instance.SetNew(&new_NicaFlowYVariable);
      instance.SetNewArray(&newArray_NicaFlowYVariable);
      instance.SetDelete(&delete_NicaFlowYVariable);
      instance.SetDeleteArray(&deleteArray_NicaFlowYVariable);
      instance.SetDestructor(&destruct_NicaFlowYVariable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFlowYVariable*)
   {
      return GenerateInitInstanceLocal((::NicaFlowYVariable*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFlowYVariable*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaSimpleVnAnalysis(void *p = nullptr);
   static void *newArray_NicaSimpleVnAnalysis(Long_t size, void *p);
   static void delete_NicaSimpleVnAnalysis(void *p);
   static void deleteArray_NicaSimpleVnAnalysis(void *p);
   static void destruct_NicaSimpleVnAnalysis(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaSimpleVnAnalysis*)
   {
      ::NicaSimpleVnAnalysis *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaSimpleVnAnalysis >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaSimpleVnAnalysis", ::NicaSimpleVnAnalysis::Class_Version(), "NicaSimpleVnAnalysis.h", 26,
                  typeid(::NicaSimpleVnAnalysis), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaSimpleVnAnalysis::Dictionary, isa_proxy, 4,
                  sizeof(::NicaSimpleVnAnalysis) );
      instance.SetNew(&new_NicaSimpleVnAnalysis);
      instance.SetNewArray(&newArray_NicaSimpleVnAnalysis);
      instance.SetDelete(&delete_NicaSimpleVnAnalysis);
      instance.SetDeleteArray(&deleteArray_NicaSimpleVnAnalysis);
      instance.SetDestructor(&destruct_NicaSimpleVnAnalysis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaSimpleVnAnalysis*)
   {
      return GenerateInitInstanceLocal((::NicaSimpleVnAnalysis*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaSimpleVnAnalysis*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NicaFlowVariable::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFlowVariable::Class_Name()
{
   return "NicaFlowVariable";
}

//______________________________________________________________________________
const char *NicaFlowVariable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowVariable*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFlowVariable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowVariable*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFlowVariable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowVariable*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFlowVariable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowVariable*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFlowVirtualVariable::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFlowVirtualVariable::Class_Name()
{
   return "NicaFlowVirtualVariable";
}

//______________________________________________________________________________
const char *NicaFlowVirtualVariable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowVirtualVariable*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFlowVirtualVariable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowVirtualVariable*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFlowVirtualVariable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowVirtualVariable*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFlowVirtualVariable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowVirtualVariable*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFlowPtVariable::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFlowPtVariable::Class_Name()
{
   return "NicaFlowPtVariable";
}

//______________________________________________________________________________
const char *NicaFlowPtVariable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowPtVariable*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFlowPtVariable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowPtVariable*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFlowPtVariable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowPtVariable*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFlowPtVariable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowPtVariable*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFlowYVariable::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFlowYVariable::Class_Name()
{
   return "NicaFlowYVariable";
}

//______________________________________________________________________________
const char *NicaFlowYVariable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowYVariable*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFlowYVariable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowYVariable*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFlowYVariable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowYVariable*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFlowYVariable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFlowYVariable*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaSimpleVnAnalysis::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaSimpleVnAnalysis::Class_Name()
{
   return "NicaSimpleVnAnalysis";
}

//______________________________________________________________________________
const char *NicaSimpleVnAnalysis::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSimpleVnAnalysis*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaSimpleVnAnalysis::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSimpleVnAnalysis*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaSimpleVnAnalysis::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSimpleVnAnalysis*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaSimpleVnAnalysis::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSimpleVnAnalysis*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void NicaFlowVariable::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFlowVariable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFlowVariable::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFlowVariable::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaFlowVariable(void *p) {
      delete ((::NicaFlowVariable*)p);
   }
   static void deleteArray_NicaFlowVariable(void *p) {
      delete [] ((::NicaFlowVariable*)p);
   }
   static void destruct_NicaFlowVariable(void *p) {
      typedef ::NicaFlowVariable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFlowVariable

//______________________________________________________________________________
void NicaFlowVirtualVariable::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFlowVirtualVariable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFlowVirtualVariable::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFlowVirtualVariable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFlowVirtualVariable(void *p) {
      return  p ? new(p) ::NicaFlowVirtualVariable : new ::NicaFlowVirtualVariable;
   }
   static void *newArray_NicaFlowVirtualVariable(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFlowVirtualVariable[nElements] : new ::NicaFlowVirtualVariable[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFlowVirtualVariable(void *p) {
      delete ((::NicaFlowVirtualVariable*)p);
   }
   static void deleteArray_NicaFlowVirtualVariable(void *p) {
      delete [] ((::NicaFlowVirtualVariable*)p);
   }
   static void destruct_NicaFlowVirtualVariable(void *p) {
      typedef ::NicaFlowVirtualVariable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFlowVirtualVariable

//______________________________________________________________________________
void NicaFlowPtVariable::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFlowPtVariable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFlowPtVariable::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFlowPtVariable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFlowPtVariable(void *p) {
      return  p ? new(p) ::NicaFlowPtVariable : new ::NicaFlowPtVariable;
   }
   static void *newArray_NicaFlowPtVariable(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFlowPtVariable[nElements] : new ::NicaFlowPtVariable[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFlowPtVariable(void *p) {
      delete ((::NicaFlowPtVariable*)p);
   }
   static void deleteArray_NicaFlowPtVariable(void *p) {
      delete [] ((::NicaFlowPtVariable*)p);
   }
   static void destruct_NicaFlowPtVariable(void *p) {
      typedef ::NicaFlowPtVariable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFlowPtVariable

//______________________________________________________________________________
void NicaFlowYVariable::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFlowYVariable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFlowYVariable::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFlowYVariable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFlowYVariable(void *p) {
      return  p ? new(p) ::NicaFlowYVariable : new ::NicaFlowYVariable;
   }
   static void *newArray_NicaFlowYVariable(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFlowYVariable[nElements] : new ::NicaFlowYVariable[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFlowYVariable(void *p) {
      delete ((::NicaFlowYVariable*)p);
   }
   static void deleteArray_NicaFlowYVariable(void *p) {
      delete [] ((::NicaFlowYVariable*)p);
   }
   static void destruct_NicaFlowYVariable(void *p) {
      typedef ::NicaFlowYVariable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFlowYVariable

//______________________________________________________________________________
void NicaSimpleVnAnalysis::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaSimpleVnAnalysis.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaSimpleVnAnalysis::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaSimpleVnAnalysis::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaSimpleVnAnalysis(void *p) {
      return  p ? new(p) ::NicaSimpleVnAnalysis : new ::NicaSimpleVnAnalysis;
   }
   static void *newArray_NicaSimpleVnAnalysis(Long_t nElements, void *p) {
      return p ? new(p) ::NicaSimpleVnAnalysis[nElements] : new ::NicaSimpleVnAnalysis[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaSimpleVnAnalysis(void *p) {
      delete ((::NicaSimpleVnAnalysis*)p);
   }
   static void deleteArray_NicaSimpleVnAnalysis(void *p) {
      delete [] ((::NicaSimpleVnAnalysis*)p);
   }
   static void destruct_NicaSimpleVnAnalysis(void *p) {
      typedef ::NicaSimpleVnAnalysis current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaSimpleVnAnalysis

namespace {
  void TriggerDictionaryInitialization_G__NicaFlowDict_Impl() {
    static const char* headers[] = {
"NicaFlowPtVariable.h",
"NicaFlowVariable.h",
"NicaFlowYVariable.h",
"NicaSimpleVnAnalysis.h",
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
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/flow",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/analysis/flow/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__NicaFlowDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$NicaFlowVariable.h")))  __attribute__((annotate("$clingAutoload$NicaFlowPtVariable.h")))  NicaFlowVariable;
class __attribute__((annotate("$clingAutoload$NicaFlowVariable.h")))  __attribute__((annotate("$clingAutoload$NicaFlowPtVariable.h")))  NicaFlowVirtualVariable;
class __attribute__((annotate("$clingAutoload$NicaFlowPtVariable.h")))  NicaFlowPtVariable;
class __attribute__((annotate("$clingAutoload$NicaFlowYVariable.h")))  NicaFlowYVariable;
class __attribute__((annotate("$clingAutoload$NicaSimpleVnAnalysis.h")))  NicaSimpleVnAnalysis;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__NicaFlowDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "NicaFlowPtVariable.h"
#include "NicaFlowVariable.h"
#include "NicaFlowYVariable.h"
#include "NicaSimpleVnAnalysis.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"NicaFlowPtVariable", payloadCode, "@",
"NicaFlowVariable", payloadCode, "@",
"NicaFlowVirtualVariable", payloadCode, "@",
"NicaFlowYVariable", payloadCode, "@",
"NicaSimpleVnAnalysis", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__NicaFlowDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__NicaFlowDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__NicaFlowDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__NicaFlowDict() {
  TriggerDictionaryInitialization_G__NicaFlowDict_Impl();
}
