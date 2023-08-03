// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmRecoQa
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
#include "CbmRecoQa.h"
#include "CbmTrackingTrdQa.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_CbmRecoQa(void *p);
   static void deleteArray_CbmRecoQa(void *p);
   static void destruct_CbmRecoQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRecoQa*)
   {
      ::CbmRecoQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRecoQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRecoQa", ::CbmRecoQa::Class_Version(), "CbmRecoQa.h", 27,
                  typeid(::CbmRecoQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRecoQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRecoQa) );
      instance.SetDelete(&delete_CbmRecoQa);
      instance.SetDeleteArray(&deleteArray_CbmRecoQa);
      instance.SetDestructor(&destruct_CbmRecoQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRecoQa*)
   {
      return GenerateInitInstanceLocal((::CbmRecoQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRecoQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrackingTrdQa(void *p = nullptr);
   static void *newArray_CbmTrackingTrdQa(Long_t size, void *p);
   static void delete_CbmTrackingTrdQa(void *p);
   static void deleteArray_CbmTrackingTrdQa(void *p);
   static void destruct_CbmTrackingTrdQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrackingTrdQa*)
   {
      ::CbmTrackingTrdQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrackingTrdQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrackingTrdQa", ::CbmTrackingTrdQa::Class_Version(), "CbmTrackingTrdQa.h", 39,
                  typeid(::CbmTrackingTrdQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrackingTrdQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrackingTrdQa) );
      instance.SetNew(&new_CbmTrackingTrdQa);
      instance.SetNewArray(&newArray_CbmTrackingTrdQa);
      instance.SetDelete(&delete_CbmTrackingTrdQa);
      instance.SetDeleteArray(&deleteArray_CbmTrackingTrdQa);
      instance.SetDestructor(&destruct_CbmTrackingTrdQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrackingTrdQa*)
   {
      return GenerateInitInstanceLocal((::CbmTrackingTrdQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrackingTrdQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmRecoQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRecoQa::Class_Name()
{
   return "CbmRecoQa";
}

//______________________________________________________________________________
const char *CbmRecoQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRecoQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRecoQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRecoQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrackingTrdQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrackingTrdQa::Class_Name()
{
   return "CbmTrackingTrdQa";
}

//______________________________________________________________________________
const char *CbmTrackingTrdQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackingTrdQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrackingTrdQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackingTrdQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrackingTrdQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackingTrdQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrackingTrdQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackingTrdQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmRecoQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRecoQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRecoQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRecoQa::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmRecoQa(void *p) {
      delete ((::CbmRecoQa*)p);
   }
   static void deleteArray_CbmRecoQa(void *p) {
      delete [] ((::CbmRecoQa*)p);
   }
   static void destruct_CbmRecoQa(void *p) {
      typedef ::CbmRecoQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRecoQa

//______________________________________________________________________________
void CbmTrackingTrdQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrackingTrdQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrackingTrdQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrackingTrdQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrackingTrdQa(void *p) {
      return  p ? new(p) ::CbmTrackingTrdQa : new ::CbmTrackingTrdQa;
   }
   static void *newArray_CbmTrackingTrdQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrackingTrdQa[nElements] : new ::CbmTrackingTrdQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrackingTrdQa(void *p) {
      delete ((::CbmTrackingTrdQa*)p);
   }
   static void deleteArray_CbmTrackingTrdQa(void *p) {
      delete [] ((::CbmTrackingTrdQa*)p);
   }
   static void destruct_CbmTrackingTrdQa(void *p) {
      typedef ::CbmTrackingTrdQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrackingTrdQa

namespace ROOT {
   static TClass *vectorlEvectorlETH1FmUgRsPgR_Dictionary();
   static void vectorlEvectorlETH1FmUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlETH1FmUgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlETH1FmUgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlETH1FmUgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlETH1FmUgRsPgR(void *p);
   static void destruct_vectorlEvectorlETH1FmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<TH1F*> >*)
   {
      vector<vector<TH1F*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<TH1F*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<TH1F*> >", -2, "vector", 389,
                  typeid(vector<vector<TH1F*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlETH1FmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<TH1F*> >) );
      instance.SetNew(&new_vectorlEvectorlETH1FmUgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlETH1FmUgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlETH1FmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlETH1FmUgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlETH1FmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<TH1F*> > >()));

      ::ROOT::AddClassAlternate("vector<vector<TH1F*> >","std::vector<std::vector<TH1F*, std::allocator<TH1F*> >, std::allocator<std::vector<TH1F*, std::allocator<TH1F*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<TH1F*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlETH1FmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<TH1F*> >*)nullptr)->GetClass();
      vectorlEvectorlETH1FmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlETH1FmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlETH1FmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TH1F*> > : new vector<vector<TH1F*> >;
   }
   static void *newArray_vectorlEvectorlETH1FmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TH1F*> >[nElements] : new vector<vector<TH1F*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlETH1FmUgRsPgR(void *p) {
      delete ((vector<vector<TH1F*> >*)p);
   }
   static void deleteArray_vectorlEvectorlETH1FmUgRsPgR(void *p) {
      delete [] ((vector<vector<TH1F*> >*)p);
   }
   static void destruct_vectorlEvectorlETH1FmUgRsPgR(void *p) {
      typedef vector<vector<TH1F*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<TH1F*> >

namespace ROOT {
   static TClass *vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR_Dictionary();
   static void vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR(void *p);
   static void deleteArray_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR(void *p);
   static void destruct_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<string,array<int,4> > >*)
   {
      vector<pair<string,array<int,4> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<string,array<int,4> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<string,array<int,4> > >", -2, "vector", 389,
                  typeid(vector<pair<string,array<int,4> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<string,array<int,4> > >) );
      instance.SetNew(&new_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<string,array<int,4> > > >()));

      ::ROOT::AddClassAlternate("vector<pair<string,array<int,4> > >","std::vector<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::array<int, 4ul> >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::array<int, 4ul> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pair<string,array<int,4> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<string,array<int,4> > >*)nullptr)->GetClass();
      vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<string,array<int,4> > > : new vector<pair<string,array<int,4> > >;
   }
   static void *newArray_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<string,array<int,4> > >[nElements] : new vector<pair<string,array<int,4> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR(void *p) {
      delete ((vector<pair<string,array<int,4> > >*)p);
   }
   static void deleteArray_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR(void *p) {
      delete [] ((vector<pair<string,array<int,4> > >*)p);
   }
   static void destruct_vectorlEpairlEstringcOarraylEintcO4gRsPgRsPgR(void *p) {
      typedef vector<pair<string,array<int,4> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<string,array<int,4> > >

namespace ROOT {
   static TClass *vectorlETH1FmUgR_Dictionary();
   static void vectorlETH1FmUgR_TClassManip(TClass*);
   static void *new_vectorlETH1FmUgR(void *p = nullptr);
   static void *newArray_vectorlETH1FmUgR(Long_t size, void *p);
   static void delete_vectorlETH1FmUgR(void *p);
   static void deleteArray_vectorlETH1FmUgR(void *p);
   static void destruct_vectorlETH1FmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TH1F*>*)
   {
      vector<TH1F*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TH1F*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TH1F*>", -2, "vector", 389,
                  typeid(vector<TH1F*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETH1FmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TH1F*>) );
      instance.SetNew(&new_vectorlETH1FmUgR);
      instance.SetNewArray(&newArray_vectorlETH1FmUgR);
      instance.SetDelete(&delete_vectorlETH1FmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETH1FmUgR);
      instance.SetDestructor(&destruct_vectorlETH1FmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TH1F*> >()));

      ::ROOT::AddClassAlternate("vector<TH1F*>","std::vector<TH1F*, std::allocator<TH1F*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TH1F*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETH1FmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TH1F*>*)nullptr)->GetClass();
      vectorlETH1FmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETH1FmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETH1FmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH1F*> : new vector<TH1F*>;
   }
   static void *newArray_vectorlETH1FmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH1F*>[nElements] : new vector<TH1F*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETH1FmUgR(void *p) {
      delete ((vector<TH1F*>*)p);
   }
   static void deleteArray_vectorlETH1FmUgR(void *p) {
      delete [] ((vector<TH1F*>*)p);
   }
   static void destruct_vectorlETH1FmUgR(void *p) {
      typedef vector<TH1F*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TH1F*>

namespace {
  void TriggerDictionaryInitialization_libCbmRecoQa_Impl() {
    static const char* headers[] = {
"CbmRecoQa.h",
"CbmTrackingTrdQa.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/qa",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/qa",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/core/qa/checker",
"/home/heinemann/gpuunpacker/cbmroot/core/qa",
"/home/heinemann/gpuunpacker/cbmroot/external/yaml-cpp/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/qa/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmRecoQa dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmRecoQa.h")))  CbmRecoQa;
class __attribute__((annotate("$clingAutoload$CbmTrackingTrdQa.h")))  CbmTrackingTrdQa;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmRecoQa dictionary payload"

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
#include "CbmRecoQa.h"
#include "CbmTrackingTrdQa.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmRecoQa", payloadCode, "@",
"CbmTrackingTrdQa", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmRecoQa",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmRecoQa_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmRecoQa_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmRecoQa() {
  TriggerDictionaryInitialization_libCbmRecoQa_Impl();
}
