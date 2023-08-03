// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmLmvm
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
#include "LmvmTask.h"
#include "LmvmDraw.h"
#include "LmvmDrawAll.h"
#include "LmvmHist.h"
#include "LmvmUtils.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_LmvmHist(void *p = nullptr);
   static void *newArray_LmvmHist(Long_t size, void *p);
   static void delete_LmvmHist(void *p);
   static void deleteArray_LmvmHist(void *p);
   static void destruct_LmvmHist(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LmvmHist*)
   {
      ::LmvmHist *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LmvmHist >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("LmvmHist", ::LmvmHist::Class_Version(), "LmvmHist.h", 20,
                  typeid(::LmvmHist), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::LmvmHist::Dictionary, isa_proxy, 4,
                  sizeof(::LmvmHist) );
      instance.SetNew(&new_LmvmHist);
      instance.SetNewArray(&newArray_LmvmHist);
      instance.SetDelete(&delete_LmvmHist);
      instance.SetDeleteArray(&deleteArray_LmvmHist);
      instance.SetDestructor(&destruct_LmvmHist);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LmvmHist*)
   {
      return GenerateInitInstanceLocal((::LmvmHist*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LmvmHist*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_LmvmTask(void *p = nullptr);
   static void *newArray_LmvmTask(Long_t size, void *p);
   static void delete_LmvmTask(void *p);
   static void deleteArray_LmvmTask(void *p);
   static void destruct_LmvmTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LmvmTask*)
   {
      ::LmvmTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LmvmTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("LmvmTask", ::LmvmTask::Class_Version(), "LmvmTask.h", 34,
                  typeid(::LmvmTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::LmvmTask::Dictionary, isa_proxy, 4,
                  sizeof(::LmvmTask) );
      instance.SetNew(&new_LmvmTask);
      instance.SetNewArray(&newArray_LmvmTask);
      instance.SetDelete(&delete_LmvmTask);
      instance.SetDeleteArray(&deleteArray_LmvmTask);
      instance.SetDestructor(&destruct_LmvmTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LmvmTask*)
   {
      return GenerateInitInstanceLocal((::LmvmTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LmvmTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_LmvmDraw(void *p = nullptr);
   static void *newArray_LmvmDraw(Long_t size, void *p);
   static void delete_LmvmDraw(void *p);
   static void deleteArray_LmvmDraw(void *p);
   static void destruct_LmvmDraw(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LmvmDraw*)
   {
      ::LmvmDraw *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LmvmDraw >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("LmvmDraw", ::LmvmDraw::Class_Version(), "LmvmDraw.h", 28,
                  typeid(::LmvmDraw), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::LmvmDraw::Dictionary, isa_proxy, 4,
                  sizeof(::LmvmDraw) );
      instance.SetNew(&new_LmvmDraw);
      instance.SetNewArray(&newArray_LmvmDraw);
      instance.SetDelete(&delete_LmvmDraw);
      instance.SetDeleteArray(&deleteArray_LmvmDraw);
      instance.SetDestructor(&destruct_LmvmDraw);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LmvmDraw*)
   {
      return GenerateInitInstanceLocal((::LmvmDraw*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LmvmDraw*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_LmvmDrawAll(void *p = nullptr);
   static void *newArray_LmvmDrawAll(Long_t size, void *p);
   static void delete_LmvmDrawAll(void *p);
   static void deleteArray_LmvmDrawAll(void *p);
   static void destruct_LmvmDrawAll(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LmvmDrawAll*)
   {
      ::LmvmDrawAll *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LmvmDrawAll >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("LmvmDrawAll", ::LmvmDrawAll::Class_Version(), "LmvmDrawAll.h", 23,
                  typeid(::LmvmDrawAll), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::LmvmDrawAll::Dictionary, isa_proxy, 4,
                  sizeof(::LmvmDrawAll) );
      instance.SetNew(&new_LmvmDrawAll);
      instance.SetNewArray(&newArray_LmvmDrawAll);
      instance.SetDelete(&delete_LmvmDrawAll);
      instance.SetDeleteArray(&deleteArray_LmvmDrawAll);
      instance.SetDestructor(&destruct_LmvmDrawAll);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LmvmDrawAll*)
   {
      return GenerateInitInstanceLocal((::LmvmDrawAll*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LmvmDrawAll*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_LmvmUtils(void *p = nullptr);
   static void *newArray_LmvmUtils(Long_t size, void *p);
   static void delete_LmvmUtils(void *p);
   static void deleteArray_LmvmUtils(void *p);
   static void destruct_LmvmUtils(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LmvmUtils*)
   {
      ::LmvmUtils *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LmvmUtils >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("LmvmUtils", ::LmvmUtils::Class_Version(), "LmvmUtils.h", 16,
                  typeid(::LmvmUtils), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::LmvmUtils::Dictionary, isa_proxy, 4,
                  sizeof(::LmvmUtils) );
      instance.SetNew(&new_LmvmUtils);
      instance.SetNewArray(&newArray_LmvmUtils);
      instance.SetDelete(&delete_LmvmUtils);
      instance.SetDeleteArray(&deleteArray_LmvmUtils);
      instance.SetDestructor(&destruct_LmvmUtils);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LmvmUtils*)
   {
      return GenerateInitInstanceLocal((::LmvmUtils*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LmvmUtils*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr LmvmHist::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *LmvmHist::Class_Name()
{
   return "LmvmHist";
}

//______________________________________________________________________________
const char *LmvmHist::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LmvmHist*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int LmvmHist::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LmvmHist*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LmvmHist::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LmvmHist*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LmvmHist::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LmvmHist*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr LmvmTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *LmvmTask::Class_Name()
{
   return "LmvmTask";
}

//______________________________________________________________________________
const char *LmvmTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LmvmTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int LmvmTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LmvmTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LmvmTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LmvmTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LmvmTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LmvmTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr LmvmDraw::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *LmvmDraw::Class_Name()
{
   return "LmvmDraw";
}

//______________________________________________________________________________
const char *LmvmDraw::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LmvmDraw*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int LmvmDraw::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LmvmDraw*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LmvmDraw::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LmvmDraw*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LmvmDraw::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LmvmDraw*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr LmvmDrawAll::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *LmvmDrawAll::Class_Name()
{
   return "LmvmDrawAll";
}

//______________________________________________________________________________
const char *LmvmDrawAll::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LmvmDrawAll*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int LmvmDrawAll::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LmvmDrawAll*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LmvmDrawAll::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LmvmDrawAll*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LmvmDrawAll::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LmvmDrawAll*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr LmvmUtils::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *LmvmUtils::Class_Name()
{
   return "LmvmUtils";
}

//______________________________________________________________________________
const char *LmvmUtils::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LmvmUtils*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int LmvmUtils::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LmvmUtils*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LmvmUtils::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LmvmUtils*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LmvmUtils::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LmvmUtils*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void LmvmHist::Streamer(TBuffer &R__b)
{
   // Stream an object of class LmvmHist.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(LmvmHist::Class(),this);
   } else {
      R__b.WriteClassBuffer(LmvmHist::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_LmvmHist(void *p) {
      return  p ? new(p) ::LmvmHist : new ::LmvmHist;
   }
   static void *newArray_LmvmHist(Long_t nElements, void *p) {
      return p ? new(p) ::LmvmHist[nElements] : new ::LmvmHist[nElements];
   }
   // Wrapper around operator delete
   static void delete_LmvmHist(void *p) {
      delete ((::LmvmHist*)p);
   }
   static void deleteArray_LmvmHist(void *p) {
      delete [] ((::LmvmHist*)p);
   }
   static void destruct_LmvmHist(void *p) {
      typedef ::LmvmHist current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LmvmHist

//______________________________________________________________________________
void LmvmTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class LmvmTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(LmvmTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(LmvmTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_LmvmTask(void *p) {
      return  p ? new(p) ::LmvmTask : new ::LmvmTask;
   }
   static void *newArray_LmvmTask(Long_t nElements, void *p) {
      return p ? new(p) ::LmvmTask[nElements] : new ::LmvmTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_LmvmTask(void *p) {
      delete ((::LmvmTask*)p);
   }
   static void deleteArray_LmvmTask(void *p) {
      delete [] ((::LmvmTask*)p);
   }
   static void destruct_LmvmTask(void *p) {
      typedef ::LmvmTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LmvmTask

//______________________________________________________________________________
void LmvmDraw::Streamer(TBuffer &R__b)
{
   // Stream an object of class LmvmDraw.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(LmvmDraw::Class(),this);
   } else {
      R__b.WriteClassBuffer(LmvmDraw::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_LmvmDraw(void *p) {
      return  p ? new(p) ::LmvmDraw : new ::LmvmDraw;
   }
   static void *newArray_LmvmDraw(Long_t nElements, void *p) {
      return p ? new(p) ::LmvmDraw[nElements] : new ::LmvmDraw[nElements];
   }
   // Wrapper around operator delete
   static void delete_LmvmDraw(void *p) {
      delete ((::LmvmDraw*)p);
   }
   static void deleteArray_LmvmDraw(void *p) {
      delete [] ((::LmvmDraw*)p);
   }
   static void destruct_LmvmDraw(void *p) {
      typedef ::LmvmDraw current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LmvmDraw

//______________________________________________________________________________
void LmvmDrawAll::Streamer(TBuffer &R__b)
{
   // Stream an object of class LmvmDrawAll.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(LmvmDrawAll::Class(),this);
   } else {
      R__b.WriteClassBuffer(LmvmDrawAll::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_LmvmDrawAll(void *p) {
      return  p ? new(p) ::LmvmDrawAll : new ::LmvmDrawAll;
   }
   static void *newArray_LmvmDrawAll(Long_t nElements, void *p) {
      return p ? new(p) ::LmvmDrawAll[nElements] : new ::LmvmDrawAll[nElements];
   }
   // Wrapper around operator delete
   static void delete_LmvmDrawAll(void *p) {
      delete ((::LmvmDrawAll*)p);
   }
   static void deleteArray_LmvmDrawAll(void *p) {
      delete [] ((::LmvmDrawAll*)p);
   }
   static void destruct_LmvmDrawAll(void *p) {
      typedef ::LmvmDrawAll current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LmvmDrawAll

//______________________________________________________________________________
void LmvmUtils::Streamer(TBuffer &R__b)
{
   // Stream an object of class LmvmUtils.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(LmvmUtils::Class(),this);
   } else {
      R__b.WriteClassBuffer(LmvmUtils::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_LmvmUtils(void *p) {
      return  p ? new(p) ::LmvmUtils : new ::LmvmUtils;
   }
   static void *newArray_LmvmUtils(Long_t nElements, void *p) {
      return p ? new(p) ::LmvmUtils[nElements] : new ::LmvmUtils[nElements];
   }
   // Wrapper around operator delete
   static void delete_LmvmUtils(void *p) {
      delete ((::LmvmUtils*)p);
   }
   static void deleteArray_LmvmUtils(void *p) {
      delete [] ((::LmvmUtils*)p);
   }
   static void destruct_LmvmUtils(void *p) {
      typedef ::LmvmUtils current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LmvmUtils

namespace ROOT {
   static TClass *vectorlELmvmHistmUgR_Dictionary();
   static void vectorlELmvmHistmUgR_TClassManip(TClass*);
   static void *new_vectorlELmvmHistmUgR(void *p = nullptr);
   static void *newArray_vectorlELmvmHistmUgR(Long_t size, void *p);
   static void delete_vectorlELmvmHistmUgR(void *p);
   static void deleteArray_vectorlELmvmHistmUgR(void *p);
   static void destruct_vectorlELmvmHistmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<LmvmHist*>*)
   {
      vector<LmvmHist*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<LmvmHist*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<LmvmHist*>", -2, "vector", 389,
                  typeid(vector<LmvmHist*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlELmvmHistmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<LmvmHist*>) );
      instance.SetNew(&new_vectorlELmvmHistmUgR);
      instance.SetNewArray(&newArray_vectorlELmvmHistmUgR);
      instance.SetDelete(&delete_vectorlELmvmHistmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlELmvmHistmUgR);
      instance.SetDestructor(&destruct_vectorlELmvmHistmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<LmvmHist*> >()));

      ::ROOT::AddClassAlternate("vector<LmvmHist*>","std::vector<LmvmHist*, std::allocator<LmvmHist*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<LmvmHist*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlELmvmHistmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<LmvmHist*>*)nullptr)->GetClass();
      vectorlELmvmHistmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlELmvmHistmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlELmvmHistmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<LmvmHist*> : new vector<LmvmHist*>;
   }
   static void *newArray_vectorlELmvmHistmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<LmvmHist*>[nElements] : new vector<LmvmHist*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlELmvmHistmUgR(void *p) {
      delete ((vector<LmvmHist*>*)p);
   }
   static void deleteArray_vectorlELmvmHistmUgR(void *p) {
      delete [] ((vector<LmvmHist*>*)p);
   }
   static void destruct_vectorlELmvmHistmUgR(void *p) {
      typedef vector<LmvmHist*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<LmvmHist*>

namespace ROOT {
   static TClass *vectorlELmvmCandgR_Dictionary();
   static void vectorlELmvmCandgR_TClassManip(TClass*);
   static void *new_vectorlELmvmCandgR(void *p = nullptr);
   static void *newArray_vectorlELmvmCandgR(Long_t size, void *p);
   static void delete_vectorlELmvmCandgR(void *p);
   static void deleteArray_vectorlELmvmCandgR(void *p);
   static void destruct_vectorlELmvmCandgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<LmvmCand>*)
   {
      vector<LmvmCand> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<LmvmCand>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<LmvmCand>", -2, "vector", 389,
                  typeid(vector<LmvmCand>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlELmvmCandgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<LmvmCand>) );
      instance.SetNew(&new_vectorlELmvmCandgR);
      instance.SetNewArray(&newArray_vectorlELmvmCandgR);
      instance.SetDelete(&delete_vectorlELmvmCandgR);
      instance.SetDeleteArray(&deleteArray_vectorlELmvmCandgR);
      instance.SetDestructor(&destruct_vectorlELmvmCandgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<LmvmCand> >()));

      ::ROOT::AddClassAlternate("vector<LmvmCand>","std::vector<LmvmCand, std::allocator<LmvmCand> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<LmvmCand>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlELmvmCandgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<LmvmCand>*)nullptr)->GetClass();
      vectorlELmvmCandgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlELmvmCandgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlELmvmCandgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<LmvmCand> : new vector<LmvmCand>;
   }
   static void *newArray_vectorlELmvmCandgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<LmvmCand>[nElements] : new vector<LmvmCand>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlELmvmCandgR(void *p) {
      delete ((vector<LmvmCand>*)p);
   }
   static void deleteArray_vectorlELmvmCandgR(void *p) {
      delete [] ((vector<LmvmCand>*)p);
   }
   static void destruct_vectorlELmvmCandgR(void *p) {
      typedef vector<LmvmCand> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<LmvmCand>

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
  void TriggerDictionaryInitialization_libCbmLmvm_Impl() {
    static const char* headers[] = {
"LmvmTask.h",
"LmvmDraw.h",
"LmvmDrawAll.h",
"LmvmHist.h",
"LmvmUtils.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/lmvm",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/lmvm",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich/utils",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/PWGDIL/dielectron/lmvm/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmLmvm dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$LmvmHist.h")))  __attribute__((annotate("$clingAutoload$LmvmTask.h")))  LmvmHist;
class __attribute__((annotate("$clingAutoload$LmvmTask.h")))  LmvmTask;
class __attribute__((annotate("$clingAutoload$LmvmDraw.h")))  LmvmDraw;
class __attribute__((annotate("$clingAutoload$LmvmDrawAll.h")))  LmvmDrawAll;
class __attribute__((annotate("$clingAutoload$LmvmUtils.h")))  LmvmUtils;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmLmvm dictionary payload"

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
#include "LmvmTask.h"
#include "LmvmDraw.h"
#include "LmvmDrawAll.h"
#include "LmvmHist.h"
#include "LmvmUtils.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"LmvmDraw", payloadCode, "@",
"LmvmDrawAll", payloadCode, "@",
"LmvmHist", payloadCode, "@",
"LmvmTask", payloadCode, "@",
"LmvmUtils", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmLmvm",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmLmvm_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmLmvm_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmLmvm() {
  TriggerDictionaryInitialization_libCbmLmvm_Impl();
}
