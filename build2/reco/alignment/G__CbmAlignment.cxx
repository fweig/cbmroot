// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmAlignment
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
#include "CbmBbaAlignmentTask.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmBbaAlignmentTask(void *p = nullptr);
   static void *newArray_CbmBbaAlignmentTask(Long_t size, void *p);
   static void delete_CbmBbaAlignmentTask(void *p);
   static void deleteArray_CbmBbaAlignmentTask(void *p);
   static void destruct_CbmBbaAlignmentTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBbaAlignmentTask*)
   {
      ::CbmBbaAlignmentTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBbaAlignmentTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBbaAlignmentTask", ::CbmBbaAlignmentTask::Class_Version(), "CbmBbaAlignmentTask.h", 40,
                  typeid(::CbmBbaAlignmentTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBbaAlignmentTask::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBbaAlignmentTask) );
      instance.SetNew(&new_CbmBbaAlignmentTask);
      instance.SetNewArray(&newArray_CbmBbaAlignmentTask);
      instance.SetDelete(&delete_CbmBbaAlignmentTask);
      instance.SetDeleteArray(&deleteArray_CbmBbaAlignmentTask);
      instance.SetDestructor(&destruct_CbmBbaAlignmentTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBbaAlignmentTask*)
   {
      return GenerateInitInstanceLocal((::CbmBbaAlignmentTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBbaAlignmentTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmBbaAlignmentTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBbaAlignmentTask::Class_Name()
{
   return "CbmBbaAlignmentTask";
}

//______________________________________________________________________________
const char *CbmBbaAlignmentTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBbaAlignmentTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBbaAlignmentTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBbaAlignmentTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBbaAlignmentTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBbaAlignmentTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBbaAlignmentTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBbaAlignmentTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmBbaAlignmentTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBbaAlignmentTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBbaAlignmentTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBbaAlignmentTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBbaAlignmentTask(void *p) {
      return  p ? new(p) ::CbmBbaAlignmentTask : new ::CbmBbaAlignmentTask;
   }
   static void *newArray_CbmBbaAlignmentTask(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBbaAlignmentTask[nElements] : new ::CbmBbaAlignmentTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBbaAlignmentTask(void *p) {
      delete ((::CbmBbaAlignmentTask*)p);
   }
   static void deleteArray_CbmBbaAlignmentTask(void *p) {
      delete [] ((::CbmBbaAlignmentTask*)p);
   }
   static void destruct_CbmBbaAlignmentTask(void *p) {
      typedef ::CbmBbaAlignmentTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBbaAlignmentTask

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
   static TClass *vectorlECbmStsTrackgR_Dictionary();
   static void vectorlECbmStsTrackgR_TClassManip(TClass*);
   static void *new_vectorlECbmStsTrackgR(void *p = nullptr);
   static void *newArray_vectorlECbmStsTrackgR(Long_t size, void *p);
   static void delete_vectorlECbmStsTrackgR(void *p);
   static void deleteArray_vectorlECbmStsTrackgR(void *p);
   static void destruct_vectorlECbmStsTrackgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsTrack>*)
   {
      vector<CbmStsTrack> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsTrack>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsTrack>", -2, "vector", 389,
                  typeid(vector<CbmStsTrack>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsTrackgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmStsTrack>) );
      instance.SetNew(&new_vectorlECbmStsTrackgR);
      instance.SetNewArray(&newArray_vectorlECbmStsTrackgR);
      instance.SetDelete(&delete_vectorlECbmStsTrackgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsTrackgR);
      instance.SetDestructor(&destruct_vectorlECbmStsTrackgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsTrack> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsTrack>","std::vector<CbmStsTrack, std::allocator<CbmStsTrack> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsTrack>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsTrackgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsTrack>*)nullptr)->GetClass();
      vectorlECbmStsTrackgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsTrackgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsTrackgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsTrack> : new vector<CbmStsTrack>;
   }
   static void *newArray_vectorlECbmStsTrackgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsTrack>[nElements] : new vector<CbmStsTrack>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsTrackgR(void *p) {
      delete ((vector<CbmStsTrack>*)p);
   }
   static void deleteArray_vectorlECbmStsTrackgR(void *p) {
      delete [] ((vector<CbmStsTrack>*)p);
   }
   static void destruct_vectorlECbmStsTrackgR(void *p) {
      typedef vector<CbmStsTrack> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsTrack>

namespace ROOT {
   static TClass *vectorlECbmStsHitgR_Dictionary();
   static void vectorlECbmStsHitgR_TClassManip(TClass*);
   static void *new_vectorlECbmStsHitgR(void *p = nullptr);
   static void *newArray_vectorlECbmStsHitgR(Long_t size, void *p);
   static void delete_vectorlECbmStsHitgR(void *p);
   static void deleteArray_vectorlECbmStsHitgR(void *p);
   static void destruct_vectorlECbmStsHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsHit>*)
   {
      vector<CbmStsHit> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsHit>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsHit>", -2, "vector", 389,
                  typeid(vector<CbmStsHit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsHitgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmStsHit>) );
      instance.SetNew(&new_vectorlECbmStsHitgR);
      instance.SetNewArray(&newArray_vectorlECbmStsHitgR);
      instance.SetDelete(&delete_vectorlECbmStsHitgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsHitgR);
      instance.SetDestructor(&destruct_vectorlECbmStsHitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsHit> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsHit>","std::vector<CbmStsHit, std::allocator<CbmStsHit> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsHit>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsHit>*)nullptr)->GetClass();
      vectorlECbmStsHitgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsHitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsHit> : new vector<CbmStsHit>;
   }
   static void *newArray_vectorlECbmStsHitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsHit>[nElements] : new vector<CbmStsHit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsHitgR(void *p) {
      delete ((vector<CbmStsHit>*)p);
   }
   static void deleteArray_vectorlECbmStsHitgR(void *p) {
      delete [] ((vector<CbmStsHit>*)p);
   }
   static void destruct_vectorlECbmStsHitgR(void *p) {
      typedef vector<CbmStsHit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsHit>

namespace ROOT {
   static TClass *vectorlECbmMvdHitgR_Dictionary();
   static void vectorlECbmMvdHitgR_TClassManip(TClass*);
   static void *new_vectorlECbmMvdHitgR(void *p = nullptr);
   static void *newArray_vectorlECbmMvdHitgR(Long_t size, void *p);
   static void delete_vectorlECbmMvdHitgR(void *p);
   static void deleteArray_vectorlECbmMvdHitgR(void *p);
   static void destruct_vectorlECbmMvdHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMvdHit>*)
   {
      vector<CbmMvdHit> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMvdHit>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMvdHit>", -2, "vector", 389,
                  typeid(vector<CbmMvdHit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMvdHitgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmMvdHit>) );
      instance.SetNew(&new_vectorlECbmMvdHitgR);
      instance.SetNewArray(&newArray_vectorlECbmMvdHitgR);
      instance.SetDelete(&delete_vectorlECbmMvdHitgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMvdHitgR);
      instance.SetDestructor(&destruct_vectorlECbmMvdHitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMvdHit> >()));

      ::ROOT::AddClassAlternate("vector<CbmMvdHit>","std::vector<CbmMvdHit, std::allocator<CbmMvdHit> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMvdHit>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMvdHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMvdHit>*)nullptr)->GetClass();
      vectorlECbmMvdHitgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMvdHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMvdHitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMvdHit> : new vector<CbmMvdHit>;
   }
   static void *newArray_vectorlECbmMvdHitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMvdHit>[nElements] : new vector<CbmMvdHit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMvdHitgR(void *p) {
      delete ((vector<CbmMvdHit>*)p);
   }
   static void deleteArray_vectorlECbmMvdHitgR(void *p) {
      delete [] ((vector<CbmMvdHit>*)p);
   }
   static void destruct_vectorlECbmMvdHitgR(void *p) {
      typedef vector<CbmMvdHit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMvdHit>

namespace {
  void TriggerDictionaryInitialization_libCbmAlignment_Impl() {
    static const char* headers[] = {
"CbmBbaAlignmentTask.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/alignment",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/alignment",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca/simd",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/OffLineInterface",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/ParticleFinder",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/utils",
"/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools",
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
"/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc",
"/home/heinemann/gpuunpacker/cbmroot/external/xpu/src",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/core/qa/checker",
"/home/heinemann/gpuunpacker/cbmroot/core/qa",
"/home/heinemann/gpuunpacker/cbmroot/external/yaml-cpp/include",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/external/bba/include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/alignment/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmAlignment dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmBbaAlignmentTask.h")))  CbmBbaAlignmentTask;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmAlignment dictionary payload"

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
#include "CbmBbaAlignmentTask.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmBbaAlignmentTask", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmAlignment",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmAlignment_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmAlignment_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmAlignment() {
  TriggerDictionaryInitialization_libCbmAlignment_Impl();
}
