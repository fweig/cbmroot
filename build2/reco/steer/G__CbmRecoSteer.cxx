// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmRecoSteer
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
#include "CbmRecoUnpack.h"
#include "CbmSourceTsArchive.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmRecoUnpack(void *p = nullptr);
   static void *newArray_CbmRecoUnpack(Long_t size, void *p);
   static void delete_CbmRecoUnpack(void *p);
   static void deleteArray_CbmRecoUnpack(void *p);
   static void destruct_CbmRecoUnpack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRecoUnpack*)
   {
      ::CbmRecoUnpack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRecoUnpack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRecoUnpack", ::CbmRecoUnpack::Class_Version(), "CbmRecoUnpack.h", 55,
                  typeid(::CbmRecoUnpack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRecoUnpack::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRecoUnpack) );
      instance.SetNew(&new_CbmRecoUnpack);
      instance.SetNewArray(&newArray_CbmRecoUnpack);
      instance.SetDelete(&delete_CbmRecoUnpack);
      instance.SetDeleteArray(&deleteArray_CbmRecoUnpack);
      instance.SetDestructor(&destruct_CbmRecoUnpack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRecoUnpack*)
   {
      return GenerateInitInstanceLocal((::CbmRecoUnpack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRecoUnpack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmSourceTsArchive(void *p = nullptr);
   static void *newArray_CbmSourceTsArchive(Long_t size, void *p);
   static void delete_CbmSourceTsArchive(void *p);
   static void deleteArray_CbmSourceTsArchive(void *p);
   static void destruct_CbmSourceTsArchive(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmSourceTsArchive*)
   {
      ::CbmSourceTsArchive *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmSourceTsArchive >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmSourceTsArchive", ::CbmSourceTsArchive::Class_Version(), "CbmSourceTsArchive.h", 26,
                  typeid(::CbmSourceTsArchive), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmSourceTsArchive::Dictionary, isa_proxy, 4,
                  sizeof(::CbmSourceTsArchive) );
      instance.SetNew(&new_CbmSourceTsArchive);
      instance.SetNewArray(&newArray_CbmSourceTsArchive);
      instance.SetDelete(&delete_CbmSourceTsArchive);
      instance.SetDeleteArray(&deleteArray_CbmSourceTsArchive);
      instance.SetDestructor(&destruct_CbmSourceTsArchive);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmSourceTsArchive*)
   {
      return GenerateInitInstanceLocal((::CbmSourceTsArchive*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmSourceTsArchive*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmRecoUnpack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRecoUnpack::Class_Name()
{
   return "CbmRecoUnpack";
}

//______________________________________________________________________________
const char *CbmRecoUnpack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoUnpack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRecoUnpack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoUnpack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRecoUnpack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoUnpack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRecoUnpack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoUnpack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmSourceTsArchive::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmSourceTsArchive::Class_Name()
{
   return "CbmSourceTsArchive";
}

//______________________________________________________________________________
const char *CbmSourceTsArchive::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmSourceTsArchive*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmSourceTsArchive::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmSourceTsArchive*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmSourceTsArchive::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmSourceTsArchive*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmSourceTsArchive::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmSourceTsArchive*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmRecoUnpack::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRecoUnpack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRecoUnpack::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRecoUnpack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRecoUnpack(void *p) {
      return  p ? new(p) ::CbmRecoUnpack : new ::CbmRecoUnpack;
   }
   static void *newArray_CbmRecoUnpack(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRecoUnpack[nElements] : new ::CbmRecoUnpack[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRecoUnpack(void *p) {
      delete ((::CbmRecoUnpack*)p);
   }
   static void deleteArray_CbmRecoUnpack(void *p) {
      delete [] ((::CbmRecoUnpack*)p);
   }
   static void destruct_CbmRecoUnpack(void *p) {
      typedef ::CbmRecoUnpack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRecoUnpack

//______________________________________________________________________________
void CbmSourceTsArchive::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmSourceTsArchive.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmSourceTsArchive::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmSourceTsArchive::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmSourceTsArchive(void *p) {
      return  p ? new(p) ::CbmSourceTsArchive : new ::CbmSourceTsArchive;
   }
   static void *newArray_CbmSourceTsArchive(Long_t nElements, void *p) {
      return p ? new(p) ::CbmSourceTsArchive[nElements] : new ::CbmSourceTsArchive[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmSourceTsArchive(void *p) {
      delete ((::CbmSourceTsArchive*)p);
   }
   static void deleteArray_CbmSourceTsArchive(void *p) {
      delete [] ((::CbmSourceTsArchive*)p);
   }
   static void destruct_CbmSourceTsArchive(void *p) {
      typedef ::CbmSourceTsArchive current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmSourceTsArchive

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
   static TClass *maplEunsignedsPshortcOTH1mUgR_Dictionary();
   static void maplEunsignedsPshortcOTH1mUgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPshortcOTH1mUgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPshortcOTH1mUgR(Long_t size, void *p);
   static void delete_maplEunsignedsPshortcOTH1mUgR(void *p);
   static void deleteArray_maplEunsignedsPshortcOTH1mUgR(void *p);
   static void destruct_maplEunsignedsPshortcOTH1mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned short,TH1*>*)
   {
      map<unsigned short,TH1*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned short,TH1*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned short,TH1*>", -2, "map", 100,
                  typeid(map<unsigned short,TH1*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPshortcOTH1mUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned short,TH1*>) );
      instance.SetNew(&new_maplEunsignedsPshortcOTH1mUgR);
      instance.SetNewArray(&newArray_maplEunsignedsPshortcOTH1mUgR);
      instance.SetDelete(&delete_maplEunsignedsPshortcOTH1mUgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPshortcOTH1mUgR);
      instance.SetDestructor(&destruct_maplEunsignedsPshortcOTH1mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned short,TH1*> >()));

      ::ROOT::AddClassAlternate("map<unsigned short,TH1*>","std::map<unsigned short, TH1*, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, TH1*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned short,TH1*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPshortcOTH1mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned short,TH1*>*)nullptr)->GetClass();
      maplEunsignedsPshortcOTH1mUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPshortcOTH1mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPshortcOTH1mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,TH1*> : new map<unsigned short,TH1*>;
   }
   static void *newArray_maplEunsignedsPshortcOTH1mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned short,TH1*>[nElements] : new map<unsigned short,TH1*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPshortcOTH1mUgR(void *p) {
      delete ((map<unsigned short,TH1*>*)p);
   }
   static void deleteArray_maplEunsignedsPshortcOTH1mUgR(void *p) {
      delete [] ((map<unsigned short,TH1*>*)p);
   }
   static void destruct_maplEunsignedsPshortcOTH1mUgR(void *p) {
      typedef map<unsigned short,TH1*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned short,TH1*>

namespace {
  void TriggerDictionaryInitialization_libCbmRecoSteer_Impl() {
    static const char* headers[] = {
"CbmRecoUnpack.h",
"CbmSourceTsArchive.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/steer",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/steer",
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
"/home/heinemann/gpuunpacker/cbmroot/fles/flestools",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/tof/unpack",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/much/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/much/unpack",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/psd",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/psd/unpack",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/psd",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/finder",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/fitter",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/tracks",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/selection",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/alignment",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/mcbm",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/unpack",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich/utils",
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
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
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
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts/sts",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts/unpack",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/sts/experimental",
"/home/heinemann/gpuunpacker/cbmroot/algo",
"/home/heinemann/gpuunpacker/cbmroot/algo/data",
"/home/heinemann/gpuunpacker/cbmroot/algo/base",
"/home/heinemann/gpuunpacker/cbmroot/algo/evbuild",
"/home/heinemann/gpuunpacker/cbmroot/algo/global",
"/home/heinemann/gpuunpacker/cbmroot/algo/trigger",
"/home/heinemann/gpuunpacker/cbmroot/algo/evselector",
"/home/heinemann/gpuunpacker/cbmroot/algo/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/algo/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/algo/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/algo/detectors/bmon",
"/home/heinemann/gpuunpacker/cbmroot/algo/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/algo/detectors/trd2d",
"/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/logging",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa/data",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/pid",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/rawToDigiMethods",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/unpack",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/bmon",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/bmon/unpack",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/steer/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmRecoSteer dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmRecoUnpack.h")))  CbmRecoUnpack;
class __attribute__((annotate("$clingAutoload$CbmSourceTsArchive.h")))  CbmSourceTsArchive;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmRecoSteer dictionary payload"

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
#ifndef NO_ROOT
  #define NO_ROOT 1
#endif
#ifndef NO_ROOT
  #define NO_ROOT 1
#endif
#ifndef BOOST_LOG_DYN_LINK
  #define BOOST_LOG_DYN_LINK 1
#endif
#ifndef BOOST_LOG_USE_NATIVE_SYSLOG
  #define BOOST_LOG_USE_NATIVE_SYSLOG 1
#endif
#ifndef BOOST_ERROR_CODE_HEADER_ONLY
  #define BOOST_ERROR_CODE_HEADER_ONLY 1
#endif
#ifndef BOOST_PROGRAM_OPTIONS_NO_LIB
  #define BOOST_PROGRAM_OPTIONS_NO_LIB 1
#endif
#ifndef BOOST_PROGRAM_OPTIONS_DYN_LINK
  #define BOOST_PROGRAM_OPTIONS_DYN_LINK 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "CbmRecoUnpack.h"
#include "CbmSourceTsArchive.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmRecoUnpack", payloadCode, "@",
"CbmSourceTsArchive", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmRecoSteer",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmRecoSteer_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmRecoSteer_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmRecoSteer() {
  TriggerDictionaryInitialization_libCbmRecoSteer_Impl();
}
