// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmPsdReco
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
#include "CbmPsdHitProducer.h"
#include "CbmPsdMCbmHitProducer.h"
#include "CbmPsdUnpackAlgo.h"
#include "CbmPsdUnpackConfig.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmPsdHitProducer(void *p = nullptr);
   static void *newArray_CbmPsdHitProducer(Long_t size, void *p);
   static void delete_CbmPsdHitProducer(void *p);
   static void deleteArray_CbmPsdHitProducer(void *p);
   static void destruct_CbmPsdHitProducer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdHitProducer*)
   {
      ::CbmPsdHitProducer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdHitProducer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdHitProducer", ::CbmPsdHitProducer::Class_Version(), "CbmPsdHitProducer.h", 34,
                  typeid(::CbmPsdHitProducer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdHitProducer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPsdHitProducer) );
      instance.SetNew(&new_CbmPsdHitProducer);
      instance.SetNewArray(&newArray_CbmPsdHitProducer);
      instance.SetDelete(&delete_CbmPsdHitProducer);
      instance.SetDeleteArray(&deleteArray_CbmPsdHitProducer);
      instance.SetDestructor(&destruct_CbmPsdHitProducer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdHitProducer*)
   {
      return GenerateInitInstanceLocal((::CbmPsdHitProducer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdHitProducer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPsdMCbmHitProducer(void *p = nullptr);
   static void *newArray_CbmPsdMCbmHitProducer(Long_t size, void *p);
   static void delete_CbmPsdMCbmHitProducer(void *p);
   static void deleteArray_CbmPsdMCbmHitProducer(void *p);
   static void destruct_CbmPsdMCbmHitProducer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdMCbmHitProducer*)
   {
      ::CbmPsdMCbmHitProducer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdMCbmHitProducer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdMCbmHitProducer", ::CbmPsdMCbmHitProducer::Class_Version(), "CbmPsdMCbmHitProducer.h", 17,
                  typeid(::CbmPsdMCbmHitProducer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdMCbmHitProducer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPsdMCbmHitProducer) );
      instance.SetNew(&new_CbmPsdMCbmHitProducer);
      instance.SetNewArray(&newArray_CbmPsdMCbmHitProducer);
      instance.SetDelete(&delete_CbmPsdMCbmHitProducer);
      instance.SetDeleteArray(&deleteArray_CbmPsdMCbmHitProducer);
      instance.SetDestructor(&destruct_CbmPsdMCbmHitProducer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdMCbmHitProducer*)
   {
      return GenerateInitInstanceLocal((::CbmPsdMCbmHitProducer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdMCbmHitProducer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPsdUnpackAlgo(void *p = nullptr);
   static void *newArray_CbmPsdUnpackAlgo(Long_t size, void *p);
   static void delete_CbmPsdUnpackAlgo(void *p);
   static void deleteArray_CbmPsdUnpackAlgo(void *p);
   static void destruct_CbmPsdUnpackAlgo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdUnpackAlgo*)
   {
      ::CbmPsdUnpackAlgo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdUnpackAlgo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdUnpackAlgo", ::CbmPsdUnpackAlgo::Class_Version(), "CbmPsdUnpackAlgo.h", 39,
                  typeid(::CbmPsdUnpackAlgo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdUnpackAlgo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPsdUnpackAlgo) );
      instance.SetNew(&new_CbmPsdUnpackAlgo);
      instance.SetNewArray(&newArray_CbmPsdUnpackAlgo);
      instance.SetDelete(&delete_CbmPsdUnpackAlgo);
      instance.SetDeleteArray(&deleteArray_CbmPsdUnpackAlgo);
      instance.SetDestructor(&destruct_CbmPsdUnpackAlgo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdUnpackAlgo*)
   {
      return GenerateInitInstanceLocal((::CbmPsdUnpackAlgo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdUnpackAlgo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmPsdUnpackConfig(void *p);
   static void deleteArray_CbmPsdUnpackConfig(void *p);
   static void destruct_CbmPsdUnpackConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdUnpackConfig*)
   {
      ::CbmPsdUnpackConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdUnpackConfig >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdUnpackConfig", ::CbmPsdUnpackConfig::Class_Version(), "CbmPsdUnpackConfig.h", 33,
                  typeid(::CbmPsdUnpackConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdUnpackConfig::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPsdUnpackConfig) );
      instance.SetDelete(&delete_CbmPsdUnpackConfig);
      instance.SetDeleteArray(&deleteArray_CbmPsdUnpackConfig);
      instance.SetDestructor(&destruct_CbmPsdUnpackConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdUnpackConfig*)
   {
      return GenerateInitInstanceLocal((::CbmPsdUnpackConfig*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdUnpackConfig*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdHitProducer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdHitProducer::Class_Name()
{
   return "CbmPsdHitProducer";
}

//______________________________________________________________________________
const char *CbmPsdHitProducer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdHitProducer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdHitProducer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdHitProducer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdHitProducer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdHitProducer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdHitProducer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdHitProducer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdMCbmHitProducer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdMCbmHitProducer::Class_Name()
{
   return "CbmPsdMCbmHitProducer";
}

//______________________________________________________________________________
const char *CbmPsdMCbmHitProducer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMCbmHitProducer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdMCbmHitProducer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMCbmHitProducer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdMCbmHitProducer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMCbmHitProducer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdMCbmHitProducer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMCbmHitProducer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdUnpackAlgo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdUnpackAlgo::Class_Name()
{
   return "CbmPsdUnpackAlgo";
}

//______________________________________________________________________________
const char *CbmPsdUnpackAlgo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdUnpackAlgo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdUnpackAlgo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdUnpackAlgo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdUnpackAlgo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdUnpackAlgo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdUnpackAlgo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdUnpackAlgo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdUnpackConfig::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdUnpackConfig::Class_Name()
{
   return "CbmPsdUnpackConfig";
}

//______________________________________________________________________________
const char *CbmPsdUnpackConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdUnpackConfig*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdUnpackConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdUnpackConfig*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdUnpackConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdUnpackConfig*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdUnpackConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdUnpackConfig*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmPsdHitProducer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdHitProducer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPsdHitProducer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPsdHitProducer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdHitProducer(void *p) {
      return  p ? new(p) ::CbmPsdHitProducer : new ::CbmPsdHitProducer;
   }
   static void *newArray_CbmPsdHitProducer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdHitProducer[nElements] : new ::CbmPsdHitProducer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdHitProducer(void *p) {
      delete ((::CbmPsdHitProducer*)p);
   }
   static void deleteArray_CbmPsdHitProducer(void *p) {
      delete [] ((::CbmPsdHitProducer*)p);
   }
   static void destruct_CbmPsdHitProducer(void *p) {
      typedef ::CbmPsdHitProducer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPsdHitProducer

//______________________________________________________________________________
void CbmPsdMCbmHitProducer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdMCbmHitProducer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPsdMCbmHitProducer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPsdMCbmHitProducer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdMCbmHitProducer(void *p) {
      return  p ? new(p) ::CbmPsdMCbmHitProducer : new ::CbmPsdMCbmHitProducer;
   }
   static void *newArray_CbmPsdMCbmHitProducer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdMCbmHitProducer[nElements] : new ::CbmPsdMCbmHitProducer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdMCbmHitProducer(void *p) {
      delete ((::CbmPsdMCbmHitProducer*)p);
   }
   static void deleteArray_CbmPsdMCbmHitProducer(void *p) {
      delete [] ((::CbmPsdMCbmHitProducer*)p);
   }
   static void destruct_CbmPsdMCbmHitProducer(void *p) {
      typedef ::CbmPsdMCbmHitProducer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPsdMCbmHitProducer

//______________________________________________________________________________
void CbmPsdUnpackAlgo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdUnpackAlgo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPsdUnpackAlgo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPsdUnpackAlgo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdUnpackAlgo(void *p) {
      return  p ? new(p) ::CbmPsdUnpackAlgo : new ::CbmPsdUnpackAlgo;
   }
   static void *newArray_CbmPsdUnpackAlgo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdUnpackAlgo[nElements] : new ::CbmPsdUnpackAlgo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdUnpackAlgo(void *p) {
      delete ((::CbmPsdUnpackAlgo*)p);
   }
   static void deleteArray_CbmPsdUnpackAlgo(void *p) {
      delete [] ((::CbmPsdUnpackAlgo*)p);
   }
   static void destruct_CbmPsdUnpackAlgo(void *p) {
      typedef ::CbmPsdUnpackAlgo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPsdUnpackAlgo

//______________________________________________________________________________
void CbmPsdUnpackConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdUnpackConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPsdUnpackConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPsdUnpackConfig::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmPsdUnpackConfig(void *p) {
      delete ((::CbmPsdUnpackConfig*)p);
   }
   static void deleteArray_CbmPsdUnpackConfig(void *p) {
      delete [] ((::CbmPsdUnpackConfig*)p);
   }
   static void destruct_CbmPsdUnpackConfig(void *p) {
      typedef ::CbmPsdUnpackConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPsdUnpackConfig

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

namespace ROOT {
   static TClass *vectorlEboolgR_Dictionary();
   static void vectorlEboolgR_TClassManip(TClass*);
   static void *new_vectorlEboolgR(void *p = nullptr);
   static void *newArray_vectorlEboolgR(Long_t size, void *p);
   static void delete_vectorlEboolgR(void *p);
   static void deleteArray_vectorlEboolgR(void *p);
   static void destruct_vectorlEboolgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<bool>*)
   {
      vector<bool> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<bool>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<bool>", -2, "vector", 596,
                  typeid(vector<bool>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEboolgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<bool>) );
      instance.SetNew(&new_vectorlEboolgR);
      instance.SetNewArray(&newArray_vectorlEboolgR);
      instance.SetDelete(&delete_vectorlEboolgR);
      instance.SetDeleteArray(&deleteArray_vectorlEboolgR);
      instance.SetDestructor(&destruct_vectorlEboolgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<bool> >()));

      ::ROOT::AddClassAlternate("vector<bool>","std::vector<bool, std::allocator<bool> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<bool>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEboolgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<bool>*)nullptr)->GetClass();
      vectorlEboolgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEboolgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEboolgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bool> : new vector<bool>;
   }
   static void *newArray_vectorlEboolgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bool>[nElements] : new vector<bool>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEboolgR(void *p) {
      delete ((vector<bool>*)p);
   }
   static void deleteArray_vectorlEboolgR(void *p) {
      delete [] ((vector<bool>*)p);
   }
   static void destruct_vectorlEboolgR(void *p) {
      typedef vector<bool> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<bool>

namespace {
  void TriggerDictionaryInitialization_libCbmPsdReco_Impl() {
    static const char* headers[] = {
"CbmPsdHitProducer.h",
"CbmPsdMCbmHitProducer.h",
"CbmPsdUnpackAlgo.h",
"CbmPsdUnpackConfig.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/psd",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/psd/unpack",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/psd",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/psd/unpack",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/psd",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/detectors/psd/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmPsdReco dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmPsdHitProducer.h")))  CbmPsdHitProducer;
class __attribute__((annotate("$clingAutoload$CbmPsdMCbmHitProducer.h")))  CbmPsdMCbmHitProducer;
class __attribute__((annotate("$clingAutoload$CbmPsdUnpackAlgo.h")))  CbmPsdUnpackAlgo;
class __attribute__((annotate("$clingAutoload$CbmPsdUnpackConfig.h")))  CbmPsdUnpackConfig;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmPsdReco dictionary payload"

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
#include "CbmPsdHitProducer.h"
#include "CbmPsdMCbmHitProducer.h"
#include "CbmPsdUnpackAlgo.h"
#include "CbmPsdUnpackConfig.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmPsdHitProducer", payloadCode, "@",
"CbmPsdMCbmHitProducer", payloadCode, "@",
"CbmPsdUnpackAlgo", payloadCode, "@",
"CbmPsdUnpackConfig", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmPsdReco",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmPsdReco_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmPsdReco_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmPsdReco() {
  TriggerDictionaryInitialization_libCbmPsdReco_Impl();
}
