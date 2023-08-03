// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIheinemanndIgpuunpackerdIcbmrootdIbuild2dIexternaldINICAFEMTOmIprefixdIsrcdINICAFEMTOmIbuilddIanalysisdIv0sdIG__NicaV0Dict
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
#include "NicaV0BasicFinder.h"
#include "NicaV0CandBasicCut.h"
#include "NicaV0Candidate.h"
#include "NicaV0CandidateHelix.h"
#include "NicaV0MassInvCut.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_NicaV0BasicFinder(void *p = nullptr);
   static void *newArray_NicaV0BasicFinder(Long_t size, void *p);
   static void delete_NicaV0BasicFinder(void *p);
   static void deleteArray_NicaV0BasicFinder(void *p);
   static void destruct_NicaV0BasicFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaV0BasicFinder*)
   {
      ::NicaV0BasicFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaV0BasicFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaV0BasicFinder", ::NicaV0BasicFinder::Class_Version(), "NicaV0BasicFinder.h", 27,
                  typeid(::NicaV0BasicFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaV0BasicFinder::Dictionary, isa_proxy, 4,
                  sizeof(::NicaV0BasicFinder) );
      instance.SetNew(&new_NicaV0BasicFinder);
      instance.SetNewArray(&newArray_NicaV0BasicFinder);
      instance.SetDelete(&delete_NicaV0BasicFinder);
      instance.SetDeleteArray(&deleteArray_NicaV0BasicFinder);
      instance.SetDestructor(&destruct_NicaV0BasicFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaV0BasicFinder*)
   {
      return GenerateInitInstanceLocal((::NicaV0BasicFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaV0BasicFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaV0CandBasicCut(void *p = nullptr);
   static void *newArray_NicaV0CandBasicCut(Long_t size, void *p);
   static void delete_NicaV0CandBasicCut(void *p);
   static void deleteArray_NicaV0CandBasicCut(void *p);
   static void destruct_NicaV0CandBasicCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaV0CandBasicCut*)
   {
      ::NicaV0CandBasicCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaV0CandBasicCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaV0CandBasicCut", ::NicaV0CandBasicCut::Class_Version(), "NicaV0CandBasicCut.h", 15,
                  typeid(::NicaV0CandBasicCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaV0CandBasicCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaV0CandBasicCut) );
      instance.SetNew(&new_NicaV0CandBasicCut);
      instance.SetNewArray(&newArray_NicaV0CandBasicCut);
      instance.SetDelete(&delete_NicaV0CandBasicCut);
      instance.SetDeleteArray(&deleteArray_NicaV0CandBasicCut);
      instance.SetDestructor(&destruct_NicaV0CandBasicCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaV0CandBasicCut*)
   {
      return GenerateInitInstanceLocal((::NicaV0CandBasicCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaV0CandBasicCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaV0Candidate(void *p);
   static void deleteArray_NicaV0Candidate(void *p);
   static void destruct_NicaV0Candidate(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaV0Candidate*)
   {
      ::NicaV0Candidate *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaV0Candidate >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaV0Candidate", ::NicaV0Candidate::Class_Version(), "NicaV0Candidate.h", 17,
                  typeid(::NicaV0Candidate), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaV0Candidate::Dictionary, isa_proxy, 4,
                  sizeof(::NicaV0Candidate) );
      instance.SetDelete(&delete_NicaV0Candidate);
      instance.SetDeleteArray(&deleteArray_NicaV0Candidate);
      instance.SetDestructor(&destruct_NicaV0Candidate);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaV0Candidate*)
   {
      return GenerateInitInstanceLocal((::NicaV0Candidate*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaV0Candidate*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaV0CandidateHelix(void *p = nullptr);
   static void *newArray_NicaV0CandidateHelix(Long_t size, void *p);
   static void delete_NicaV0CandidateHelix(void *p);
   static void deleteArray_NicaV0CandidateHelix(void *p);
   static void destruct_NicaV0CandidateHelix(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaV0CandidateHelix*)
   {
      ::NicaV0CandidateHelix *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaV0CandidateHelix >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaV0CandidateHelix", ::NicaV0CandidateHelix::Class_Version(), "NicaV0CandidateHelix.h", 17,
                  typeid(::NicaV0CandidateHelix), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaV0CandidateHelix::Dictionary, isa_proxy, 4,
                  sizeof(::NicaV0CandidateHelix) );
      instance.SetNew(&new_NicaV0CandidateHelix);
      instance.SetNewArray(&newArray_NicaV0CandidateHelix);
      instance.SetDelete(&delete_NicaV0CandidateHelix);
      instance.SetDeleteArray(&deleteArray_NicaV0CandidateHelix);
      instance.SetDestructor(&destruct_NicaV0CandidateHelix);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaV0CandidateHelix*)
   {
      return GenerateInitInstanceLocal((::NicaV0CandidateHelix*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaV0CandidateHelix*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaV0MassInvCut(void *p = nullptr);
   static void *newArray_NicaV0MassInvCut(Long_t size, void *p);
   static void delete_NicaV0MassInvCut(void *p);
   static void deleteArray_NicaV0MassInvCut(void *p);
   static void destruct_NicaV0MassInvCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaV0MassInvCut*)
   {
      ::NicaV0MassInvCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaV0MassInvCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaV0MassInvCut", ::NicaV0MassInvCut::Class_Version(), "NicaV0MassInvCut.h", 14,
                  typeid(::NicaV0MassInvCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaV0MassInvCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaV0MassInvCut) );
      instance.SetNew(&new_NicaV0MassInvCut);
      instance.SetNewArray(&newArray_NicaV0MassInvCut);
      instance.SetDelete(&delete_NicaV0MassInvCut);
      instance.SetDeleteArray(&deleteArray_NicaV0MassInvCut);
      instance.SetDestructor(&destruct_NicaV0MassInvCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaV0MassInvCut*)
   {
      return GenerateInitInstanceLocal((::NicaV0MassInvCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaV0MassInvCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NicaV0BasicFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaV0BasicFinder::Class_Name()
{
   return "NicaV0BasicFinder";
}

//______________________________________________________________________________
const char *NicaV0BasicFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaV0BasicFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaV0BasicFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaV0BasicFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaV0BasicFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaV0BasicFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaV0BasicFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaV0BasicFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaV0CandBasicCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaV0CandBasicCut::Class_Name()
{
   return "NicaV0CandBasicCut";
}

//______________________________________________________________________________
const char *NicaV0CandBasicCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaV0CandBasicCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaV0CandBasicCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaV0CandBasicCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaV0CandBasicCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaV0CandBasicCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaV0CandBasicCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaV0CandBasicCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaV0Candidate::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaV0Candidate::Class_Name()
{
   return "NicaV0Candidate";
}

//______________________________________________________________________________
const char *NicaV0Candidate::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaV0Candidate*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaV0Candidate::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaV0Candidate*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaV0Candidate::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaV0Candidate*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaV0Candidate::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaV0Candidate*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaV0CandidateHelix::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaV0CandidateHelix::Class_Name()
{
   return "NicaV0CandidateHelix";
}

//______________________________________________________________________________
const char *NicaV0CandidateHelix::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaV0CandidateHelix*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaV0CandidateHelix::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaV0CandidateHelix*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaV0CandidateHelix::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaV0CandidateHelix*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaV0CandidateHelix::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaV0CandidateHelix*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaV0MassInvCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaV0MassInvCut::Class_Name()
{
   return "NicaV0MassInvCut";
}

//______________________________________________________________________________
const char *NicaV0MassInvCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaV0MassInvCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaV0MassInvCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaV0MassInvCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaV0MassInvCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaV0MassInvCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaV0MassInvCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaV0MassInvCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void NicaV0BasicFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaV0BasicFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaV0BasicFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaV0BasicFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaV0BasicFinder(void *p) {
      return  p ? new(p) ::NicaV0BasicFinder : new ::NicaV0BasicFinder;
   }
   static void *newArray_NicaV0BasicFinder(Long_t nElements, void *p) {
      return p ? new(p) ::NicaV0BasicFinder[nElements] : new ::NicaV0BasicFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaV0BasicFinder(void *p) {
      delete ((::NicaV0BasicFinder*)p);
   }
   static void deleteArray_NicaV0BasicFinder(void *p) {
      delete [] ((::NicaV0BasicFinder*)p);
   }
   static void destruct_NicaV0BasicFinder(void *p) {
      typedef ::NicaV0BasicFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaV0BasicFinder

//______________________________________________________________________________
void NicaV0CandBasicCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaV0CandBasicCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaV0CandBasicCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaV0CandBasicCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaV0CandBasicCut(void *p) {
      return  p ? new(p) ::NicaV0CandBasicCut : new ::NicaV0CandBasicCut;
   }
   static void *newArray_NicaV0CandBasicCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaV0CandBasicCut[nElements] : new ::NicaV0CandBasicCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaV0CandBasicCut(void *p) {
      delete ((::NicaV0CandBasicCut*)p);
   }
   static void deleteArray_NicaV0CandBasicCut(void *p) {
      delete [] ((::NicaV0CandBasicCut*)p);
   }
   static void destruct_NicaV0CandBasicCut(void *p) {
      typedef ::NicaV0CandBasicCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaV0CandBasicCut

//______________________________________________________________________________
void NicaV0Candidate::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaV0Candidate.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaV0Candidate::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaV0Candidate::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaV0Candidate(void *p) {
      delete ((::NicaV0Candidate*)p);
   }
   static void deleteArray_NicaV0Candidate(void *p) {
      delete [] ((::NicaV0Candidate*)p);
   }
   static void destruct_NicaV0Candidate(void *p) {
      typedef ::NicaV0Candidate current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaV0Candidate

//______________________________________________________________________________
void NicaV0CandidateHelix::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaV0CandidateHelix.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaV0CandidateHelix::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaV0CandidateHelix::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaV0CandidateHelix(void *p) {
      return  p ? new(p) ::NicaV0CandidateHelix : new ::NicaV0CandidateHelix;
   }
   static void *newArray_NicaV0CandidateHelix(Long_t nElements, void *p) {
      return p ? new(p) ::NicaV0CandidateHelix[nElements] : new ::NicaV0CandidateHelix[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaV0CandidateHelix(void *p) {
      delete ((::NicaV0CandidateHelix*)p);
   }
   static void deleteArray_NicaV0CandidateHelix(void *p) {
      delete [] ((::NicaV0CandidateHelix*)p);
   }
   static void destruct_NicaV0CandidateHelix(void *p) {
      typedef ::NicaV0CandidateHelix current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaV0CandidateHelix

//______________________________________________________________________________
void NicaV0MassInvCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaV0MassInvCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaV0MassInvCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaV0MassInvCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaV0MassInvCut(void *p) {
      return  p ? new(p) ::NicaV0MassInvCut : new ::NicaV0MassInvCut;
   }
   static void *newArray_NicaV0MassInvCut(Long_t nElements, void *p) {
      return p ? new(p) ::NicaV0MassInvCut[nElements] : new ::NicaV0MassInvCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaV0MassInvCut(void *p) {
      delete ((::NicaV0MassInvCut*)p);
   }
   static void deleteArray_NicaV0MassInvCut(void *p) {
      delete [] ((::NicaV0MassInvCut*)p);
   }
   static void destruct_NicaV0MassInvCut(void *p) {
      typedef ::NicaV0MassInvCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaV0MassInvCut

namespace {
  void TriggerDictionaryInitialization_G__NicaV0Dict_Impl() {
    static const char* headers[] = {
"NicaV0BasicFinder.h",
"NicaV0CandBasicCut.h",
"NicaV0Candidate.h",
"NicaV0CandidateHelix.h",
"NicaV0MassInvCut.h",
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
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/ana",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/weights",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerators",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerator_multidim",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/imaging",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/source_models",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/corrfunc",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto",
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
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/v0s",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/analysis/v0s/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__NicaV0Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$NicaV0BasicFinder.h")))  NicaV0BasicFinder;
class __attribute__((annotate("$clingAutoload$NicaV0CandBasicCut.h")))  NicaV0CandBasicCut;
class __attribute__((annotate("$clingAutoload$NicaV0Candidate.h")))  NicaV0Candidate;
class __attribute__((annotate("$clingAutoload$NicaV0CandidateHelix.h")))  NicaV0CandidateHelix;
class __attribute__((annotate("$clingAutoload$NicaV0MassInvCut.h")))  NicaV0MassInvCut;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__NicaV0Dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "NicaV0BasicFinder.h"
#include "NicaV0CandBasicCut.h"
#include "NicaV0Candidate.h"
#include "NicaV0CandidateHelix.h"
#include "NicaV0MassInvCut.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"NicaV0BasicFinder", payloadCode, "@",
"NicaV0CandBasicCut", payloadCode, "@",
"NicaV0Candidate", payloadCode, "@",
"NicaV0CandidateHelix", payloadCode, "@",
"NicaV0MassInvCut", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__NicaV0Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__NicaV0Dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__NicaV0Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__NicaV0Dict() {
  TriggerDictionaryInitialization_G__NicaV0Dict_Impl();
}
