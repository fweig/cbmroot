// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmJPsiAnalysis
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
#include "CbmAnaJpsiTask.h"
#include "CbmAnaJpsiReport.h"
#include "CbmAnaJpsiHist.h"
#include "CbmAnaJpsiCandidate.h"
#include "CbmAnaJpsiSuperEvent.h"
#include "CbmAnaJpsiSuperEventReport.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmAnaJpsiCandidate(void *p = nullptr);
   static void *newArray_CbmAnaJpsiCandidate(Long_t size, void *p);
   static void delete_CbmAnaJpsiCandidate(void *p);
   static void deleteArray_CbmAnaJpsiCandidate(void *p);
   static void destruct_CbmAnaJpsiCandidate(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaJpsiCandidate*)
   {
      ::CbmAnaJpsiCandidate *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaJpsiCandidate >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaJpsiCandidate", ::CbmAnaJpsiCandidate::Class_Version(), "CbmAnaJpsiCandidate.h", 11,
                  typeid(::CbmAnaJpsiCandidate), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaJpsiCandidate::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaJpsiCandidate) );
      instance.SetNew(&new_CbmAnaJpsiCandidate);
      instance.SetNewArray(&newArray_CbmAnaJpsiCandidate);
      instance.SetDelete(&delete_CbmAnaJpsiCandidate);
      instance.SetDeleteArray(&deleteArray_CbmAnaJpsiCandidate);
      instance.SetDestructor(&destruct_CbmAnaJpsiCandidate);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaJpsiCandidate*)
   {
      return GenerateInitInstanceLocal((::CbmAnaJpsiCandidate*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaJpsiCandidate*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaJpsiTask(void *p = nullptr);
   static void *newArray_CbmAnaJpsiTask(Long_t size, void *p);
   static void delete_CbmAnaJpsiTask(void *p);
   static void deleteArray_CbmAnaJpsiTask(void *p);
   static void destruct_CbmAnaJpsiTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaJpsiTask*)
   {
      ::CbmAnaJpsiTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaJpsiTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaJpsiTask", ::CbmAnaJpsiTask::Class_Version(), "CbmAnaJpsiTask.h", 31,
                  typeid(::CbmAnaJpsiTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaJpsiTask::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaJpsiTask) );
      instance.SetNew(&new_CbmAnaJpsiTask);
      instance.SetNewArray(&newArray_CbmAnaJpsiTask);
      instance.SetDelete(&delete_CbmAnaJpsiTask);
      instance.SetDeleteArray(&deleteArray_CbmAnaJpsiTask);
      instance.SetDestructor(&destruct_CbmAnaJpsiTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaJpsiTask*)
   {
      return GenerateInitInstanceLocal((::CbmAnaJpsiTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaJpsiTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaJpsiReport(void *p = nullptr);
   static void *newArray_CbmAnaJpsiReport(Long_t size, void *p);
   static void delete_CbmAnaJpsiReport(void *p);
   static void deleteArray_CbmAnaJpsiReport(void *p);
   static void destruct_CbmAnaJpsiReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaJpsiReport*)
   {
      ::CbmAnaJpsiReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaJpsiReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaJpsiReport", ::CbmAnaJpsiReport::Class_Version(), "CbmAnaJpsiReport.h", 16,
                  typeid(::CbmAnaJpsiReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaJpsiReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaJpsiReport) );
      instance.SetNew(&new_CbmAnaJpsiReport);
      instance.SetNewArray(&newArray_CbmAnaJpsiReport);
      instance.SetDelete(&delete_CbmAnaJpsiReport);
      instance.SetDeleteArray(&deleteArray_CbmAnaJpsiReport);
      instance.SetDestructor(&destruct_CbmAnaJpsiReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaJpsiReport*)
   {
      return GenerateInitInstanceLocal((::CbmAnaJpsiReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaJpsiReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaJpsiSuperEvent(void *p = nullptr);
   static void *newArray_CbmAnaJpsiSuperEvent(Long_t size, void *p);
   static void delete_CbmAnaJpsiSuperEvent(void *p);
   static void deleteArray_CbmAnaJpsiSuperEvent(void *p);
   static void destruct_CbmAnaJpsiSuperEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaJpsiSuperEvent*)
   {
      ::CbmAnaJpsiSuperEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaJpsiSuperEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaJpsiSuperEvent", ::CbmAnaJpsiSuperEvent::Class_Version(), "CbmAnaJpsiSuperEvent.h", 29,
                  typeid(::CbmAnaJpsiSuperEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaJpsiSuperEvent::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaJpsiSuperEvent) );
      instance.SetNew(&new_CbmAnaJpsiSuperEvent);
      instance.SetNewArray(&newArray_CbmAnaJpsiSuperEvent);
      instance.SetDelete(&delete_CbmAnaJpsiSuperEvent);
      instance.SetDeleteArray(&deleteArray_CbmAnaJpsiSuperEvent);
      instance.SetDestructor(&destruct_CbmAnaJpsiSuperEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaJpsiSuperEvent*)
   {
      return GenerateInitInstanceLocal((::CbmAnaJpsiSuperEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaJpsiSuperEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaJpsiSuperEventReport(void *p = nullptr);
   static void *newArray_CbmAnaJpsiSuperEventReport(Long_t size, void *p);
   static void delete_CbmAnaJpsiSuperEventReport(void *p);
   static void deleteArray_CbmAnaJpsiSuperEventReport(void *p);
   static void destruct_CbmAnaJpsiSuperEventReport(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaJpsiSuperEventReport*)
   {
      ::CbmAnaJpsiSuperEventReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaJpsiSuperEventReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaJpsiSuperEventReport", ::CbmAnaJpsiSuperEventReport::Class_Version(), "CbmAnaJpsiSuperEventReport.h", 15,
                  typeid(::CbmAnaJpsiSuperEventReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaJpsiSuperEventReport::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaJpsiSuperEventReport) );
      instance.SetNew(&new_CbmAnaJpsiSuperEventReport);
      instance.SetNewArray(&newArray_CbmAnaJpsiSuperEventReport);
      instance.SetDelete(&delete_CbmAnaJpsiSuperEventReport);
      instance.SetDeleteArray(&deleteArray_CbmAnaJpsiSuperEventReport);
      instance.SetDestructor(&destruct_CbmAnaJpsiSuperEventReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaJpsiSuperEventReport*)
   {
      return GenerateInitInstanceLocal((::CbmAnaJpsiSuperEventReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaJpsiSuperEventReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaJpsiCandidate::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaJpsiCandidate::Class_Name()
{
   return "CbmAnaJpsiCandidate";
}

//______________________________________________________________________________
const char *CbmAnaJpsiCandidate::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiCandidate*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaJpsiCandidate::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiCandidate*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaJpsiCandidate::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiCandidate*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaJpsiCandidate::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiCandidate*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaJpsiTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaJpsiTask::Class_Name()
{
   return "CbmAnaJpsiTask";
}

//______________________________________________________________________________
const char *CbmAnaJpsiTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaJpsiTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaJpsiTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaJpsiTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaJpsiReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaJpsiReport::Class_Name()
{
   return "CbmAnaJpsiReport";
}

//______________________________________________________________________________
const char *CbmAnaJpsiReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaJpsiReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaJpsiReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaJpsiReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaJpsiSuperEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaJpsiSuperEvent::Class_Name()
{
   return "CbmAnaJpsiSuperEvent";
}

//______________________________________________________________________________
const char *CbmAnaJpsiSuperEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiSuperEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaJpsiSuperEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiSuperEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaJpsiSuperEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiSuperEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaJpsiSuperEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiSuperEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaJpsiSuperEventReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaJpsiSuperEventReport::Class_Name()
{
   return "CbmAnaJpsiSuperEventReport";
}

//______________________________________________________________________________
const char *CbmAnaJpsiSuperEventReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiSuperEventReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaJpsiSuperEventReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiSuperEventReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaJpsiSuperEventReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiSuperEventReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaJpsiSuperEventReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaJpsiSuperEventReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmAnaJpsiCandidate::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaJpsiCandidate.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaJpsiCandidate::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaJpsiCandidate::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaJpsiCandidate(void *p) {
      return  p ? new(p) ::CbmAnaJpsiCandidate : new ::CbmAnaJpsiCandidate;
   }
   static void *newArray_CbmAnaJpsiCandidate(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaJpsiCandidate[nElements] : new ::CbmAnaJpsiCandidate[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaJpsiCandidate(void *p) {
      delete ((::CbmAnaJpsiCandidate*)p);
   }
   static void deleteArray_CbmAnaJpsiCandidate(void *p) {
      delete [] ((::CbmAnaJpsiCandidate*)p);
   }
   static void destruct_CbmAnaJpsiCandidate(void *p) {
      typedef ::CbmAnaJpsiCandidate current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaJpsiCandidate

//______________________________________________________________________________
void CbmAnaJpsiTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaJpsiTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaJpsiTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaJpsiTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaJpsiTask(void *p) {
      return  p ? new(p) ::CbmAnaJpsiTask : new ::CbmAnaJpsiTask;
   }
   static void *newArray_CbmAnaJpsiTask(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaJpsiTask[nElements] : new ::CbmAnaJpsiTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaJpsiTask(void *p) {
      delete ((::CbmAnaJpsiTask*)p);
   }
   static void deleteArray_CbmAnaJpsiTask(void *p) {
      delete [] ((::CbmAnaJpsiTask*)p);
   }
   static void destruct_CbmAnaJpsiTask(void *p) {
      typedef ::CbmAnaJpsiTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaJpsiTask

//______________________________________________________________________________
void CbmAnaJpsiReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaJpsiReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaJpsiReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaJpsiReport::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaJpsiReport(void *p) {
      return  p ? new(p) ::CbmAnaJpsiReport : new ::CbmAnaJpsiReport;
   }
   static void *newArray_CbmAnaJpsiReport(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaJpsiReport[nElements] : new ::CbmAnaJpsiReport[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaJpsiReport(void *p) {
      delete ((::CbmAnaJpsiReport*)p);
   }
   static void deleteArray_CbmAnaJpsiReport(void *p) {
      delete [] ((::CbmAnaJpsiReport*)p);
   }
   static void destruct_CbmAnaJpsiReport(void *p) {
      typedef ::CbmAnaJpsiReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaJpsiReport

//______________________________________________________________________________
void CbmAnaJpsiSuperEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaJpsiSuperEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaJpsiSuperEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaJpsiSuperEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaJpsiSuperEvent(void *p) {
      return  p ? new(p) ::CbmAnaJpsiSuperEvent : new ::CbmAnaJpsiSuperEvent;
   }
   static void *newArray_CbmAnaJpsiSuperEvent(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaJpsiSuperEvent[nElements] : new ::CbmAnaJpsiSuperEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaJpsiSuperEvent(void *p) {
      delete ((::CbmAnaJpsiSuperEvent*)p);
   }
   static void deleteArray_CbmAnaJpsiSuperEvent(void *p) {
      delete [] ((::CbmAnaJpsiSuperEvent*)p);
   }
   static void destruct_CbmAnaJpsiSuperEvent(void *p) {
      typedef ::CbmAnaJpsiSuperEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaJpsiSuperEvent

//______________________________________________________________________________
void CbmAnaJpsiSuperEventReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaJpsiSuperEventReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaJpsiSuperEventReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaJpsiSuperEventReport::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaJpsiSuperEventReport(void *p) {
      return  p ? new(p) ::CbmAnaJpsiSuperEventReport : new ::CbmAnaJpsiSuperEventReport;
   }
   static void *newArray_CbmAnaJpsiSuperEventReport(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaJpsiSuperEventReport[nElements] : new ::CbmAnaJpsiSuperEventReport[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaJpsiSuperEventReport(void *p) {
      delete ((::CbmAnaJpsiSuperEventReport*)p);
   }
   static void deleteArray_CbmAnaJpsiSuperEventReport(void *p) {
      delete [] ((::CbmAnaJpsiSuperEventReport*)p);
   }
   static void destruct_CbmAnaJpsiSuperEventReport(void *p) {
      typedef ::CbmAnaJpsiSuperEventReport current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaJpsiSuperEventReport

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
   static TClass *vectorlECbmAnaJpsiCandidategR_Dictionary();
   static void vectorlECbmAnaJpsiCandidategR_TClassManip(TClass*);
   static void *new_vectorlECbmAnaJpsiCandidategR(void *p = nullptr);
   static void *newArray_vectorlECbmAnaJpsiCandidategR(Long_t size, void *p);
   static void delete_vectorlECbmAnaJpsiCandidategR(void *p);
   static void deleteArray_vectorlECbmAnaJpsiCandidategR(void *p);
   static void destruct_vectorlECbmAnaJpsiCandidategR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmAnaJpsiCandidate>*)
   {
      vector<CbmAnaJpsiCandidate> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmAnaJpsiCandidate>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmAnaJpsiCandidate>", -2, "vector", 389,
                  typeid(vector<CbmAnaJpsiCandidate>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmAnaJpsiCandidategR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmAnaJpsiCandidate>) );
      instance.SetNew(&new_vectorlECbmAnaJpsiCandidategR);
      instance.SetNewArray(&newArray_vectorlECbmAnaJpsiCandidategR);
      instance.SetDelete(&delete_vectorlECbmAnaJpsiCandidategR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmAnaJpsiCandidategR);
      instance.SetDestructor(&destruct_vectorlECbmAnaJpsiCandidategR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmAnaJpsiCandidate> >()));

      ::ROOT::AddClassAlternate("vector<CbmAnaJpsiCandidate>","std::vector<CbmAnaJpsiCandidate, std::allocator<CbmAnaJpsiCandidate> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmAnaJpsiCandidate>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmAnaJpsiCandidategR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmAnaJpsiCandidate>*)nullptr)->GetClass();
      vectorlECbmAnaJpsiCandidategR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmAnaJpsiCandidategR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmAnaJpsiCandidategR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmAnaJpsiCandidate> : new vector<CbmAnaJpsiCandidate>;
   }
   static void *newArray_vectorlECbmAnaJpsiCandidategR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmAnaJpsiCandidate>[nElements] : new vector<CbmAnaJpsiCandidate>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmAnaJpsiCandidategR(void *p) {
      delete ((vector<CbmAnaJpsiCandidate>*)p);
   }
   static void deleteArray_vectorlECbmAnaJpsiCandidategR(void *p) {
      delete [] ((vector<CbmAnaJpsiCandidate>*)p);
   }
   static void destruct_vectorlECbmAnaJpsiCandidategR(void *p) {
      typedef vector<CbmAnaJpsiCandidate> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmAnaJpsiCandidate>

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
  void TriggerDictionaryInitialization_libCbmJPsiAnalysis_Impl() {
    static const char* headers[] = {
"CbmAnaJpsiTask.h",
"CbmAnaJpsiReport.h",
"CbmAnaJpsiHist.h",
"CbmAnaJpsiCandidate.h",
"CbmAnaJpsiSuperEvent.h",
"CbmAnaJpsiSuperEventReport.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGCHA/jpsi",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGCHA/jpsi",
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
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/PWGCHA/jpsi/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmJPsiAnalysis dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmAnaJpsiCandidate.h")))  __attribute__((annotate("$clingAutoload$CbmAnaJpsiTask.h")))  CbmAnaJpsiCandidate;
class __attribute__((annotate("$clingAutoload$CbmAnaJpsiTask.h")))  CbmAnaJpsiTask;
class __attribute__((annotate("$clingAutoload$CbmAnaJpsiReport.h")))  CbmAnaJpsiReport;
class __attribute__((annotate("$clingAutoload$CbmAnaJpsiSuperEvent.h")))  CbmAnaJpsiSuperEvent;
class __attribute__((annotate("$clingAutoload$CbmAnaJpsiSuperEventReport.h")))  CbmAnaJpsiSuperEventReport;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmJPsiAnalysis dictionary payload"

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
#include "CbmAnaJpsiTask.h"
#include "CbmAnaJpsiReport.h"
#include "CbmAnaJpsiHist.h"
#include "CbmAnaJpsiCandidate.h"
#include "CbmAnaJpsiSuperEvent.h"
#include "CbmAnaJpsiSuperEventReport.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmAnaJpsiCandidate", payloadCode, "@",
"CbmAnaJpsiReport", payloadCode, "@",
"CbmAnaJpsiSuperEvent", payloadCode, "@",
"CbmAnaJpsiSuperEventReport", payloadCode, "@",
"CbmAnaJpsiTask", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmJPsiAnalysis",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmJPsiAnalysis_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmJPsiAnalysis_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmJPsiAnalysis() {
  TriggerDictionaryInitialization_libCbmJPsiAnalysis_Impl();
}
