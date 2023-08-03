// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmClustering
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
#include "CbmClusteringGeometry.h"
#include "CbmMuchClustering.h"
#include "CbmClusteringA1.h"
#include "CbmClusteringSL.h"
#include "CbmClusteringWard.h"
#include "CbmMuchTest.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_CbmMuchClustering(void *p);
   static void deleteArray_CbmMuchClustering(void *p);
   static void destruct_CbmMuchClustering(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchClustering*)
   {
      ::CbmMuchClustering *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchClustering >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchClustering", ::CbmMuchClustering::Class_Version(), "CbmMuchClustering.h", 32,
                  typeid(::CbmMuchClustering), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchClustering::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchClustering) );
      instance.SetDelete(&delete_CbmMuchClustering);
      instance.SetDeleteArray(&deleteArray_CbmMuchClustering);
      instance.SetDestructor(&destruct_CbmMuchClustering);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchClustering*)
   {
      return GenerateInitInstanceLocal((::CbmMuchClustering*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchClustering*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchClustering::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchClustering::Class_Name()
{
   return "CbmMuchClustering";
}

//______________________________________________________________________________
const char *CbmMuchClustering::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchClustering*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchClustering::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchClustering*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchClustering::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchClustering*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchClustering::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchClustering*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmMuchClustering::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchClustering.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchClustering::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchClustering::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmMuchClustering(void *p) {
      delete ((::CbmMuchClustering*)p);
   }
   static void deleteArray_CbmMuchClustering(void *p) {
      delete [] ((::CbmMuchClustering*)p);
   }
   static void destruct_CbmMuchClustering(void *p) {
      typedef ::CbmMuchClustering current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchClustering

namespace ROOT {
   static TClass *vectorlECbmClusteringGeometrymUgR_Dictionary();
   static void vectorlECbmClusteringGeometrymUgR_TClassManip(TClass*);
   static void *new_vectorlECbmClusteringGeometrymUgR(void *p = nullptr);
   static void *newArray_vectorlECbmClusteringGeometrymUgR(Long_t size, void *p);
   static void delete_vectorlECbmClusteringGeometrymUgR(void *p);
   static void deleteArray_vectorlECbmClusteringGeometrymUgR(void *p);
   static void destruct_vectorlECbmClusteringGeometrymUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmClusteringGeometry*>*)
   {
      vector<CbmClusteringGeometry*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmClusteringGeometry*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmClusteringGeometry*>", -2, "vector", 389,
                  typeid(vector<CbmClusteringGeometry*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmClusteringGeometrymUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmClusteringGeometry*>) );
      instance.SetNew(&new_vectorlECbmClusteringGeometrymUgR);
      instance.SetNewArray(&newArray_vectorlECbmClusteringGeometrymUgR);
      instance.SetDelete(&delete_vectorlECbmClusteringGeometrymUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmClusteringGeometrymUgR);
      instance.SetDestructor(&destruct_vectorlECbmClusteringGeometrymUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmClusteringGeometry*> >()));

      ::ROOT::AddClassAlternate("vector<CbmClusteringGeometry*>","std::vector<CbmClusteringGeometry*, std::allocator<CbmClusteringGeometry*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmClusteringGeometry*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmClusteringGeometrymUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmClusteringGeometry*>*)nullptr)->GetClass();
      vectorlECbmClusteringGeometrymUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmClusteringGeometrymUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmClusteringGeometrymUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmClusteringGeometry*> : new vector<CbmClusteringGeometry*>;
   }
   static void *newArray_vectorlECbmClusteringGeometrymUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmClusteringGeometry*>[nElements] : new vector<CbmClusteringGeometry*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmClusteringGeometrymUgR(void *p) {
      delete ((vector<CbmClusteringGeometry*>*)p);
   }
   static void deleteArray_vectorlECbmClusteringGeometrymUgR(void *p) {
      delete [] ((vector<CbmClusteringGeometry*>*)p);
   }
   static void destruct_vectorlECbmClusteringGeometrymUgR(void *p) {
      typedef vector<CbmClusteringGeometry*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmClusteringGeometry*>

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

namespace {
  void TriggerDictionaryInitialization_libCbmClustering_Impl() {
    static const char* headers[] = {
"CbmClusteringGeometry.h",
"CbmMuchClustering.h",
"CbmClusteringA1.h",
"CbmClusteringSL.h",
"CbmClusteringWard.h",
"CbmMuchTest.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/clustering",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/littrack/clustering",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
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
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/littrack/clustering/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmClustering dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmMuchClustering.h")))  CbmMuchClustering;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmClustering dictionary payload"

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
#include "CbmClusteringGeometry.h"
#include "CbmMuchClustering.h"
#include "CbmClusteringA1.h"
#include "CbmClusteringSL.h"
#include "CbmClusteringWard.h"
#include "CbmMuchTest.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmMuchClustering", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmClustering",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmClustering_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmClustering_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmClustering() {
  TriggerDictionaryInitialization_libCbmClustering_Impl();
}
