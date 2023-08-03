// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmMvdSim
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
#include "CbmMvdSensorDigitizerTask.h"
#include "CbmMvd.h"
#include "CbmMvdDigitizer.h"
#include "CbmMvdPixelCharge.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmMvdSensorDigitizerTask(void *p = nullptr);
   static void *newArray_CbmMvdSensorDigitizerTask(Long_t size, void *p);
   static void delete_CbmMvdSensorDigitizerTask(void *p);
   static void deleteArray_CbmMvdSensorDigitizerTask(void *p);
   static void destruct_CbmMvdSensorDigitizerTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdSensorDigitizerTask*)
   {
      ::CbmMvdSensorDigitizerTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdSensorDigitizerTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdSensorDigitizerTask", ::CbmMvdSensorDigitizerTask::Class_Version(), "CbmMvdSensorDigitizerTask.h", 49,
                  typeid(::CbmMvdSensorDigitizerTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdSensorDigitizerTask::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdSensorDigitizerTask) );
      instance.SetNew(&new_CbmMvdSensorDigitizerTask);
      instance.SetNewArray(&newArray_CbmMvdSensorDigitizerTask);
      instance.SetDelete(&delete_CbmMvdSensorDigitizerTask);
      instance.SetDeleteArray(&deleteArray_CbmMvdSensorDigitizerTask);
      instance.SetDestructor(&destruct_CbmMvdSensorDigitizerTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdSensorDigitizerTask*)
   {
      return GenerateInitInstanceLocal((::CbmMvdSensorDigitizerTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdSensorDigitizerTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvd(void *p = nullptr);
   static void *newArray_CbmMvd(Long_t size, void *p);
   static void delete_CbmMvd(void *p);
   static void deleteArray_CbmMvd(void *p);
   static void destruct_CbmMvd(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvd*)
   {
      ::CbmMvd *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvd >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvd", ::CbmMvd::Class_Version(), "CbmMvd.h", 41,
                  typeid(::CbmMvd), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvd::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvd) );
      instance.SetNew(&new_CbmMvd);
      instance.SetNewArray(&newArray_CbmMvd);
      instance.SetDelete(&delete_CbmMvd);
      instance.SetDeleteArray(&deleteArray_CbmMvd);
      instance.SetDestructor(&destruct_CbmMvd);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvd*)
   {
      return GenerateInitInstanceLocal((::CbmMvd*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvd*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigitizelECbmMvdDigigR_Dictionary();
   static void CbmDigitizelECbmMvdDigigR_TClassManip(TClass*);
   static void delete_CbmDigitizelECbmMvdDigigR(void *p);
   static void deleteArray_CbmDigitizelECbmMvdDigigR(void *p);
   static void destruct_CbmDigitizelECbmMvdDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigitize<CbmMvdDigi>*)
   {
      ::CbmDigitize<CbmMvdDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigitize<CbmMvdDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigitize<CbmMvdDigi>", ::CbmDigitize<CbmMvdDigi>::Class_Version(), "CbmDigitize.h", 44,
                  typeid(::CbmDigitize<CbmMvdDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigitizelECbmMvdDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigitize<CbmMvdDigi>) );
      instance.SetDelete(&delete_CbmDigitizelECbmMvdDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigitizelECbmMvdDigigR);
      instance.SetDestructor(&destruct_CbmDigitizelECbmMvdDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigitize<CbmMvdDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigitize<CbmMvdDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigitize<CbmMvdDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigitizelECbmMvdDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmMvdDigi>*)nullptr)->GetClass();
      CbmDigitizelECbmMvdDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigitizelECbmMvdDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdDigitizer(void *p = nullptr);
   static void *newArray_CbmMvdDigitizer(Long_t size, void *p);
   static void delete_CbmMvdDigitizer(void *p);
   static void deleteArray_CbmMvdDigitizer(void *p);
   static void destruct_CbmMvdDigitizer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdDigitizer*)
   {
      ::CbmMvdDigitizer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdDigitizer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdDigitizer", ::CbmMvdDigitizer::Class_Version(), "CbmMvdDigitizer.h", 38,
                  typeid(::CbmMvdDigitizer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdDigitizer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdDigitizer) );
      instance.SetNew(&new_CbmMvdDigitizer);
      instance.SetNewArray(&newArray_CbmMvdDigitizer);
      instance.SetDelete(&delete_CbmMvdDigitizer);
      instance.SetDeleteArray(&deleteArray_CbmMvdDigitizer);
      instance.SetDestructor(&destruct_CbmMvdDigitizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdDigitizer*)
   {
      return GenerateInitInstanceLocal((::CbmMvdDigitizer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdDigitizer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdPixelCharge(void *p = nullptr);
   static void *newArray_CbmMvdPixelCharge(Long_t size, void *p);
   static void delete_CbmMvdPixelCharge(void *p);
   static void deleteArray_CbmMvdPixelCharge(void *p);
   static void destruct_CbmMvdPixelCharge(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdPixelCharge*)
   {
      ::CbmMvdPixelCharge *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdPixelCharge >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdPixelCharge", ::CbmMvdPixelCharge::Class_Version(), "CbmMvdPixelCharge.h", 19,
                  typeid(::CbmMvdPixelCharge), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdPixelCharge::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdPixelCharge) );
      instance.SetNew(&new_CbmMvdPixelCharge);
      instance.SetNewArray(&newArray_CbmMvdPixelCharge);
      instance.SetDelete(&delete_CbmMvdPixelCharge);
      instance.SetDeleteArray(&deleteArray_CbmMvdPixelCharge);
      instance.SetDestructor(&destruct_CbmMvdPixelCharge);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdPixelCharge*)
   {
      return GenerateInitInstanceLocal((::CbmMvdPixelCharge*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdPixelCharge*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdSensorDigitizerTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdSensorDigitizerTask::Class_Name()
{
   return "CbmMvdSensorDigitizerTask";
}

//______________________________________________________________________________
const char *CbmMvdSensorDigitizerTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorDigitizerTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdSensorDigitizerTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorDigitizerTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdSensorDigitizerTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorDigitizerTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdSensorDigitizerTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdSensorDigitizerTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvd::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvd::Class_Name()
{
   return "CbmMvd";
}

//______________________________________________________________________________
const char *CbmMvd::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvd*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvd::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvd*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvd::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvd*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvd::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvd*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigitize<CbmMvdDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmMvdDigi>::Class_Name()
{
   return "CbmDigitize<CbmMvdDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmMvdDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmMvdDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigitize<CbmMvdDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmMvdDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmMvdDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmMvdDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmMvdDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmMvdDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdDigitizer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdDigitizer::Class_Name()
{
   return "CbmMvdDigitizer";
}

//______________________________________________________________________________
const char *CbmMvdDigitizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDigitizer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdDigitizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDigitizer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdDigitizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDigitizer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdDigitizer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDigitizer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdPixelCharge::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdPixelCharge::Class_Name()
{
   return "CbmMvdPixelCharge";
}

//______________________________________________________________________________
const char *CbmMvdPixelCharge::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdPixelCharge*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdPixelCharge::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdPixelCharge*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdPixelCharge::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdPixelCharge*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdPixelCharge::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdPixelCharge*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmMvdSensorDigitizerTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdSensorDigitizerTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdSensorDigitizerTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdSensorDigitizerTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdSensorDigitizerTask(void *p) {
      return  p ? new(p) ::CbmMvdSensorDigitizerTask : new ::CbmMvdSensorDigitizerTask;
   }
   static void *newArray_CbmMvdSensorDigitizerTask(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdSensorDigitizerTask[nElements] : new ::CbmMvdSensorDigitizerTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdSensorDigitizerTask(void *p) {
      delete ((::CbmMvdSensorDigitizerTask*)p);
   }
   static void deleteArray_CbmMvdSensorDigitizerTask(void *p) {
      delete [] ((::CbmMvdSensorDigitizerTask*)p);
   }
   static void destruct_CbmMvdSensorDigitizerTask(void *p) {
      typedef ::CbmMvdSensorDigitizerTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdSensorDigitizerTask

//______________________________________________________________________________
void CbmMvd::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvd.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvd::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvd::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvd(void *p) {
      return  p ? new(p) ::CbmMvd : new ::CbmMvd;
   }
   static void *newArray_CbmMvd(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvd[nElements] : new ::CbmMvd[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvd(void *p) {
      delete ((::CbmMvd*)p);
   }
   static void deleteArray_CbmMvd(void *p) {
      delete [] ((::CbmMvd*)p);
   }
   static void destruct_CbmMvd(void *p) {
      typedef ::CbmMvd current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvd

//______________________________________________________________________________
template <> void CbmDigitize<CbmMvdDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigitize<CbmMvdDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigitize<CbmMvdDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigitize<CbmMvdDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmDigitizelECbmMvdDigigR(void *p) {
      delete ((::CbmDigitize<CbmMvdDigi>*)p);
   }
   static void deleteArray_CbmDigitizelECbmMvdDigigR(void *p) {
      delete [] ((::CbmDigitize<CbmMvdDigi>*)p);
   }
   static void destruct_CbmDigitizelECbmMvdDigigR(void *p) {
      typedef ::CbmDigitize<CbmMvdDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigitize<CbmMvdDigi>

//______________________________________________________________________________
void CbmMvdDigitizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdDigitizer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdDigitizer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdDigitizer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdDigitizer(void *p) {
      return  p ? new(p) ::CbmMvdDigitizer : new ::CbmMvdDigitizer;
   }
   static void *newArray_CbmMvdDigitizer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdDigitizer[nElements] : new ::CbmMvdDigitizer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdDigitizer(void *p) {
      delete ((::CbmMvdDigitizer*)p);
   }
   static void deleteArray_CbmMvdDigitizer(void *p) {
      delete [] ((::CbmMvdDigitizer*)p);
   }
   static void destruct_CbmMvdDigitizer(void *p) {
      typedef ::CbmMvdDigitizer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdDigitizer

//______________________________________________________________________________
void CbmMvdPixelCharge::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdPixelCharge.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdPixelCharge::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdPixelCharge::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdPixelCharge(void *p) {
      return  p ? new(p) ::CbmMvdPixelCharge : new ::CbmMvdPixelCharge;
   }
   static void *newArray_CbmMvdPixelCharge(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdPixelCharge[nElements] : new ::CbmMvdPixelCharge[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdPixelCharge(void *p) {
      delete ((::CbmMvdPixelCharge*)p);
   }
   static void deleteArray_CbmMvdPixelCharge(void *p) {
      delete [] ((::CbmMvdPixelCharge*)p);
   }
   static void destruct_CbmMvdPixelCharge(void *p) {
      typedef ::CbmMvdPixelCharge current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdPixelCharge

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
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = nullptr);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 389,
                  typeid(vector<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));

      ::ROOT::AddClassAlternate("vector<float>","std::vector<float, std::allocator<float> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<float>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<float>*)nullptr)->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete ((vector<float>*)p);
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] ((vector<float>*)p);
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<float>

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
   static TClass *vectorlECbmMvdPixelChargemUgR_Dictionary();
   static void vectorlECbmMvdPixelChargemUgR_TClassManip(TClass*);
   static void *new_vectorlECbmMvdPixelChargemUgR(void *p = nullptr);
   static void *newArray_vectorlECbmMvdPixelChargemUgR(Long_t size, void *p);
   static void delete_vectorlECbmMvdPixelChargemUgR(void *p);
   static void deleteArray_vectorlECbmMvdPixelChargemUgR(void *p);
   static void destruct_vectorlECbmMvdPixelChargemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMvdPixelCharge*>*)
   {
      vector<CbmMvdPixelCharge*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMvdPixelCharge*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMvdPixelCharge*>", -2, "vector", 389,
                  typeid(vector<CbmMvdPixelCharge*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMvdPixelChargemUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmMvdPixelCharge*>) );
      instance.SetNew(&new_vectorlECbmMvdPixelChargemUgR);
      instance.SetNewArray(&newArray_vectorlECbmMvdPixelChargemUgR);
      instance.SetDelete(&delete_vectorlECbmMvdPixelChargemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMvdPixelChargemUgR);
      instance.SetDestructor(&destruct_vectorlECbmMvdPixelChargemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMvdPixelCharge*> >()));

      ::ROOT::AddClassAlternate("vector<CbmMvdPixelCharge*>","std::vector<CbmMvdPixelCharge*, std::allocator<CbmMvdPixelCharge*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMvdPixelCharge*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMvdPixelChargemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMvdPixelCharge*>*)nullptr)->GetClass();
      vectorlECbmMvdPixelChargemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMvdPixelChargemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMvdPixelChargemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMvdPixelCharge*> : new vector<CbmMvdPixelCharge*>;
   }
   static void *newArray_vectorlECbmMvdPixelChargemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMvdPixelCharge*>[nElements] : new vector<CbmMvdPixelCharge*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMvdPixelChargemUgR(void *p) {
      delete ((vector<CbmMvdPixelCharge*>*)p);
   }
   static void deleteArray_vectorlECbmMvdPixelChargemUgR(void *p) {
      delete [] ((vector<CbmMvdPixelCharge*>*)p);
   }
   static void destruct_vectorlECbmMvdPixelChargemUgR(void *p) {
      typedef vector<CbmMvdPixelCharge*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMvdPixelCharge*>

namespace ROOT {
   static TClass *vectorlECbmLinkgR_Dictionary();
   static void vectorlECbmLinkgR_TClassManip(TClass*);
   static void *new_vectorlECbmLinkgR(void *p = nullptr);
   static void *newArray_vectorlECbmLinkgR(Long_t size, void *p);
   static void delete_vectorlECbmLinkgR(void *p);
   static void deleteArray_vectorlECbmLinkgR(void *p);
   static void destruct_vectorlECbmLinkgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmLink>*)
   {
      vector<CbmLink> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmLink>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmLink>", -2, "vector", 389,
                  typeid(vector<CbmLink>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmLinkgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmLink>) );
      instance.SetNew(&new_vectorlECbmLinkgR);
      instance.SetNewArray(&newArray_vectorlECbmLinkgR);
      instance.SetDelete(&delete_vectorlECbmLinkgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmLinkgR);
      instance.SetDestructor(&destruct_vectorlECbmLinkgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmLink> >()));

      ::ROOT::AddClassAlternate("vector<CbmLink>","std::vector<CbmLink, std::allocator<CbmLink> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmLink>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmLinkgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmLink>*)nullptr)->GetClass();
      vectorlECbmLinkgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmLinkgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmLinkgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmLink> : new vector<CbmLink>;
   }
   static void *newArray_vectorlECbmLinkgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmLink>[nElements] : new vector<CbmLink>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmLinkgR(void *p) {
      delete ((vector<CbmLink>*)p);
   }
   static void deleteArray_vectorlECbmLinkgR(void *p) {
      delete [] ((vector<CbmLink>*)p);
   }
   static void destruct_vectorlECbmLinkgR(void *p) {
      typedef vector<CbmLink> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmLink>

namespace ROOT {
   static TClass *maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR_Dictionary();
   static void maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR_TClassManip(TClass*);
   static void *new_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR(void *p = nullptr);
   static void *newArray_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR(Long_t size, void *p);
   static void delete_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR(void *p);
   static void deleteArray_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR(void *p);
   static void destruct_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<pair<int,int>,CbmMvdPixelCharge*>*)
   {
      map<pair<int,int>,CbmMvdPixelCharge*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<pair<int,int>,CbmMvdPixelCharge*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<pair<int,int>,CbmMvdPixelCharge*>", -2, "map", 100,
                  typeid(map<pair<int,int>,CbmMvdPixelCharge*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<pair<int,int>,CbmMvdPixelCharge*>) );
      instance.SetNew(&new_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR);
      instance.SetNewArray(&newArray_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR);
      instance.SetDelete(&delete_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR);
      instance.SetDeleteArray(&deleteArray_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR);
      instance.SetDestructor(&destruct_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<pair<int,int>,CbmMvdPixelCharge*> >()));

      ::ROOT::AddClassAlternate("map<pair<int,int>,CbmMvdPixelCharge*>","std::map<std::pair<int, int>, CbmMvdPixelCharge*, std::less<std::pair<int, int> >, std::allocator<std::pair<std::pair<int, int> const, CbmMvdPixelCharge*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<pair<int,int>,CbmMvdPixelCharge*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<pair<int,int>,CbmMvdPixelCharge*>*)nullptr)->GetClass();
      maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,int>,CbmMvdPixelCharge*> : new map<pair<int,int>,CbmMvdPixelCharge*>;
   }
   static void *newArray_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,int>,CbmMvdPixelCharge*>[nElements] : new map<pair<int,int>,CbmMvdPixelCharge*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR(void *p) {
      delete ((map<pair<int,int>,CbmMvdPixelCharge*>*)p);
   }
   static void deleteArray_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR(void *p) {
      delete [] ((map<pair<int,int>,CbmMvdPixelCharge*>*)p);
   }
   static void destruct_maplEpairlEintcOintgRcOCbmMvdPixelChargemUgR(void *p) {
      typedef map<pair<int,int>,CbmMvdPixelCharge*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<pair<int,int>,CbmMvdPixelCharge*>

namespace {
  void TriggerDictionaryInitialization_libCbmMvdSim_Impl() {
    static const char* headers[] = {
"CbmMvdSensorDigitizerTask.h",
"CbmMvd.h",
"CbmMvdDigitizer.h",
"CbmMvdPixelCharge.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/mvd/plugins/tasks",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/mvd/plugins/tasks",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/tools",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/SensorDataSheets",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/tasks",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/buffers",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/mvd/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmMvdSim dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmMvdSensorDigitizerTask.h")))  CbmMvdSensorDigitizerTask;
class __attribute__((annotate("$clingAutoload$CbmMvd.h")))  CbmMvd;
class __attribute__((annotate("$clingAutoload$CbmMvdDigi.h")))  __attribute__((annotate("$clingAutoload$CbmMvdDigitizer.h")))  CbmMvdDigi;
template <class Digi> class __attribute__((annotate("$clingAutoload$CbmDigitize.h")))  __attribute__((annotate("$clingAutoload$CbmMvdDigitizer.h")))  CbmDigitize;

class __attribute__((annotate("$clingAutoload$CbmMvdDigitizer.h")))  CbmMvdDigitizer;
class __attribute__((annotate("$clingAutoload$CbmMvdPixelCharge.h")))  CbmMvdPixelCharge;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmMvdSim dictionary payload"

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
#include "CbmMvdSensorDigitizerTask.h"
#include "CbmMvd.h"
#include "CbmMvdDigitizer.h"
#include "CbmMvdPixelCharge.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmDigitize<CbmMvdDigi>", payloadCode, "@",
"CbmMvd", payloadCode, "@",
"CbmMvdDigitizer", payloadCode, "@",
"CbmMvdPixelCharge", payloadCode, "@",
"CbmMvdSensorDigitizerTask", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmMvdSim",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmMvdSim_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmMvdSim_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmMvdSim() {
  TriggerDictionaryInitialization_libCbmMvdSim_Impl();
}
