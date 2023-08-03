// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmRecoCal
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
#include "CbmOffsetDigiTimeTester.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *CbmOffsetDigiTimelECbmStsDigigR_Dictionary();
   static void CbmOffsetDigiTimelECbmStsDigigR_TClassManip(TClass*);
   static void delete_CbmOffsetDigiTimelECbmStsDigigR(void *p);
   static void deleteArray_CbmOffsetDigiTimelECbmStsDigigR(void *p);
   static void destruct_CbmOffsetDigiTimelECbmStsDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmOffsetDigiTime<CbmStsDigi>*)
   {
      ::CbmOffsetDigiTime<CbmStsDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmOffsetDigiTime<CbmStsDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmOffsetDigiTime<CbmStsDigi>", ::CbmOffsetDigiTime<CbmStsDigi>::Class_Version(), "CbmOffsetDigiTime.h", 29,
                  typeid(::CbmOffsetDigiTime<CbmStsDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmOffsetDigiTimelECbmStsDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmOffsetDigiTime<CbmStsDigi>) );
      instance.SetDelete(&delete_CbmOffsetDigiTimelECbmStsDigigR);
      instance.SetDeleteArray(&deleteArray_CbmOffsetDigiTimelECbmStsDigigR);
      instance.SetDestructor(&destruct_CbmOffsetDigiTimelECbmStsDigigR);

      ::ROOT::AddClassAlternate("CbmOffsetDigiTime<CbmStsDigi>","CbmOffsetDigiTime");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmOffsetDigiTime<CbmStsDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmOffsetDigiTime<CbmStsDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmStsDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmOffsetDigiTimelECbmStsDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmStsDigi>*)nullptr)->GetClass();
      CbmOffsetDigiTimelECbmStsDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmOffsetDigiTimelECbmStsDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmOffsetDigiTimelECbmTofDigigR_Dictionary();
   static void CbmOffsetDigiTimelECbmTofDigigR_TClassManip(TClass*);
   static void delete_CbmOffsetDigiTimelECbmTofDigigR(void *p);
   static void deleteArray_CbmOffsetDigiTimelECbmTofDigigR(void *p);
   static void destruct_CbmOffsetDigiTimelECbmTofDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmOffsetDigiTime<CbmTofDigi>*)
   {
      ::CbmOffsetDigiTime<CbmTofDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmOffsetDigiTime<CbmTofDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmOffsetDigiTime<CbmTofDigi>", ::CbmOffsetDigiTime<CbmTofDigi>::Class_Version(), "CbmOffsetDigiTime.h", 29,
                  typeid(::CbmOffsetDigiTime<CbmTofDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmOffsetDigiTimelECbmTofDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmOffsetDigiTime<CbmTofDigi>) );
      instance.SetDelete(&delete_CbmOffsetDigiTimelECbmTofDigigR);
      instance.SetDeleteArray(&deleteArray_CbmOffsetDigiTimelECbmTofDigigR);
      instance.SetDestructor(&destruct_CbmOffsetDigiTimelECbmTofDigigR);

      ::ROOT::AddClassAlternate("CbmOffsetDigiTime<CbmTofDigi>","CbmOffsetDigiTime");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmOffsetDigiTime<CbmTofDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmOffsetDigiTime<CbmTofDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmTofDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmOffsetDigiTimelECbmTofDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmTofDigi>*)nullptr)->GetClass();
      CbmOffsetDigiTimelECbmTofDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmOffsetDigiTimelECbmTofDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmOffsetDigiTimelECbmMuchBeamTimeDigigR_Dictionary();
   static void CbmOffsetDigiTimelECbmMuchBeamTimeDigigR_TClassManip(TClass*);
   static void delete_CbmOffsetDigiTimelECbmMuchBeamTimeDigigR(void *p);
   static void deleteArray_CbmOffsetDigiTimelECbmMuchBeamTimeDigigR(void *p);
   static void destruct_CbmOffsetDigiTimelECbmMuchBeamTimeDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>*)
   {
      ::CbmOffsetDigiTime<CbmMuchBeamTimeDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmOffsetDigiTime<CbmMuchBeamTimeDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmOffsetDigiTime<CbmMuchBeamTimeDigi>", ::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>::Class_Version(), "CbmOffsetDigiTime.h", 29,
                  typeid(::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmOffsetDigiTimelECbmMuchBeamTimeDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>) );
      instance.SetDelete(&delete_CbmOffsetDigiTimelECbmMuchBeamTimeDigigR);
      instance.SetDeleteArray(&deleteArray_CbmOffsetDigiTimelECbmMuchBeamTimeDigigR);
      instance.SetDestructor(&destruct_CbmOffsetDigiTimelECbmMuchBeamTimeDigigR);

      ::ROOT::AddClassAlternate("CbmOffsetDigiTime<CbmMuchBeamTimeDigi>","CbmOffsetDigiTime");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmOffsetDigiTimelECbmMuchBeamTimeDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>*)nullptr)->GetClass();
      CbmOffsetDigiTimelECbmMuchBeamTimeDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmOffsetDigiTimelECbmMuchBeamTimeDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmOffsetDigiTimeSts(void *p);
   static void deleteArray_CbmOffsetDigiTimeSts(void *p);
   static void destruct_CbmOffsetDigiTimeSts(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmOffsetDigiTimeSts*)
   {
      ::CbmOffsetDigiTimeSts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmOffsetDigiTimeSts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmOffsetDigiTimeSts", ::CbmOffsetDigiTimeSts::Class_Version(), "CbmOffsetDigiTimeTester.h", 28,
                  typeid(::CbmOffsetDigiTimeSts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmOffsetDigiTimeSts::Dictionary, isa_proxy, 4,
                  sizeof(::CbmOffsetDigiTimeSts) );
      instance.SetDelete(&delete_CbmOffsetDigiTimeSts);
      instance.SetDeleteArray(&deleteArray_CbmOffsetDigiTimeSts);
      instance.SetDestructor(&destruct_CbmOffsetDigiTimeSts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmOffsetDigiTimeSts*)
   {
      return GenerateInitInstanceLocal((::CbmOffsetDigiTimeSts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeSts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmOffsetDigiTimeTof(void *p);
   static void deleteArray_CbmOffsetDigiTimeTof(void *p);
   static void destruct_CbmOffsetDigiTimeTof(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmOffsetDigiTimeTof*)
   {
      ::CbmOffsetDigiTimeTof *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmOffsetDigiTimeTof >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmOffsetDigiTimeTof", ::CbmOffsetDigiTimeTof::Class_Version(), "CbmOffsetDigiTimeTester.h", 38,
                  typeid(::CbmOffsetDigiTimeTof), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmOffsetDigiTimeTof::Dictionary, isa_proxy, 4,
                  sizeof(::CbmOffsetDigiTimeTof) );
      instance.SetDelete(&delete_CbmOffsetDigiTimeTof);
      instance.SetDeleteArray(&deleteArray_CbmOffsetDigiTimeTof);
      instance.SetDestructor(&destruct_CbmOffsetDigiTimeTof);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmOffsetDigiTimeTof*)
   {
      return GenerateInitInstanceLocal((::CbmOffsetDigiTimeTof*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeTof*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmOffsetDigiTimeMuch(void *p);
   static void deleteArray_CbmOffsetDigiTimeMuch(void *p);
   static void destruct_CbmOffsetDigiTimeMuch(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmOffsetDigiTimeMuch*)
   {
      ::CbmOffsetDigiTimeMuch *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmOffsetDigiTimeMuch >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmOffsetDigiTimeMuch", ::CbmOffsetDigiTimeMuch::Class_Version(), "CbmOffsetDigiTimeTester.h", 48,
                  typeid(::CbmOffsetDigiTimeMuch), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmOffsetDigiTimeMuch::Dictionary, isa_proxy, 4,
                  sizeof(::CbmOffsetDigiTimeMuch) );
      instance.SetDelete(&delete_CbmOffsetDigiTimeMuch);
      instance.SetDeleteArray(&deleteArray_CbmOffsetDigiTimeMuch);
      instance.SetDestructor(&destruct_CbmOffsetDigiTimeMuch);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmOffsetDigiTimeMuch*)
   {
      return GenerateInitInstanceLocal((::CbmOffsetDigiTimeMuch*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeMuch*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmOffsetDigiTimeTester_Dictionary();
   static void CbmOffsetDigiTimeTester_TClassManip(TClass*);
   static void *new_CbmOffsetDigiTimeTester(void *p = nullptr);
   static void *newArray_CbmOffsetDigiTimeTester(Long_t size, void *p);
   static void delete_CbmOffsetDigiTimeTester(void *p);
   static void deleteArray_CbmOffsetDigiTimeTester(void *p);
   static void destruct_CbmOffsetDigiTimeTester(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmOffsetDigiTimeTester*)
   {
      ::CbmOffsetDigiTimeTester *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmOffsetDigiTimeTester));
      static ::ROOT::TGenericClassInfo 
         instance("CbmOffsetDigiTimeTester", "CbmOffsetDigiTimeTester.h", 58,
                  typeid(::CbmOffsetDigiTimeTester), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmOffsetDigiTimeTester_Dictionary, isa_proxy, 4,
                  sizeof(::CbmOffsetDigiTimeTester) );
      instance.SetNew(&new_CbmOffsetDigiTimeTester);
      instance.SetNewArray(&newArray_CbmOffsetDigiTimeTester);
      instance.SetDelete(&delete_CbmOffsetDigiTimeTester);
      instance.SetDeleteArray(&deleteArray_CbmOffsetDigiTimeTester);
      instance.SetDestructor(&destruct_CbmOffsetDigiTimeTester);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmOffsetDigiTimeTester*)
   {
      return GenerateInitInstanceLocal((::CbmOffsetDigiTimeTester*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeTester*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmOffsetDigiTimeTester_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeTester*)nullptr)->GetClass();
      CbmOffsetDigiTimeTester_TClassManip(theClass);
   return theClass;
   }

   static void CbmOffsetDigiTimeTester_TClassManip(TClass* ){
   }

} // end of namespace ROOT

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmOffsetDigiTime<CbmStsDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmOffsetDigiTime<CbmStsDigi>::Class_Name()
{
   return "CbmOffsetDigiTime<CbmStsDigi>";
}

//______________________________________________________________________________
template <> const char *CbmOffsetDigiTime<CbmStsDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmStsDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmOffsetDigiTime<CbmStsDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmStsDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmOffsetDigiTime<CbmStsDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmStsDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmOffsetDigiTime<CbmStsDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmStsDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmOffsetDigiTime<CbmTofDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmOffsetDigiTime<CbmTofDigi>::Class_Name()
{
   return "CbmOffsetDigiTime<CbmTofDigi>";
}

//______________________________________________________________________________
template <> const char *CbmOffsetDigiTime<CbmTofDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmTofDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmOffsetDigiTime<CbmTofDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmTofDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmOffsetDigiTime<CbmTofDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmTofDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmOffsetDigiTime<CbmTofDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmTofDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmOffsetDigiTime<CbmMuchBeamTimeDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmOffsetDigiTime<CbmMuchBeamTimeDigi>::Class_Name()
{
   return "CbmOffsetDigiTime<CbmMuchBeamTimeDigi>";
}

//______________________________________________________________________________
template <> const char *CbmOffsetDigiTime<CbmMuchBeamTimeDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmOffsetDigiTime<CbmMuchBeamTimeDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmOffsetDigiTime<CbmMuchBeamTimeDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmOffsetDigiTime<CbmMuchBeamTimeDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmOffsetDigiTimeSts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmOffsetDigiTimeSts::Class_Name()
{
   return "CbmOffsetDigiTimeSts";
}

//______________________________________________________________________________
const char *CbmOffsetDigiTimeSts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeSts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmOffsetDigiTimeSts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeSts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmOffsetDigiTimeSts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeSts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmOffsetDigiTimeSts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeSts*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmOffsetDigiTimeTof::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmOffsetDigiTimeTof::Class_Name()
{
   return "CbmOffsetDigiTimeTof";
}

//______________________________________________________________________________
const char *CbmOffsetDigiTimeTof::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeTof*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmOffsetDigiTimeTof::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeTof*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmOffsetDigiTimeTof::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeTof*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmOffsetDigiTimeTof::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeTof*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmOffsetDigiTimeMuch::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmOffsetDigiTimeMuch::Class_Name()
{
   return "CbmOffsetDigiTimeMuch";
}

//______________________________________________________________________________
const char *CbmOffsetDigiTimeMuch::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeMuch*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmOffsetDigiTimeMuch::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeMuch*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmOffsetDigiTimeMuch::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeMuch*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmOffsetDigiTimeMuch::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmOffsetDigiTimeMuch*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> void CbmOffsetDigiTime<CbmStsDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmOffsetDigiTime<CbmStsDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmOffsetDigiTime<CbmStsDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmOffsetDigiTime<CbmStsDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmOffsetDigiTimelECbmStsDigigR(void *p) {
      delete ((::CbmOffsetDigiTime<CbmStsDigi>*)p);
   }
   static void deleteArray_CbmOffsetDigiTimelECbmStsDigigR(void *p) {
      delete [] ((::CbmOffsetDigiTime<CbmStsDigi>*)p);
   }
   static void destruct_CbmOffsetDigiTimelECbmStsDigigR(void *p) {
      typedef ::CbmOffsetDigiTime<CbmStsDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmOffsetDigiTime<CbmStsDigi>

//______________________________________________________________________________
template <> void CbmOffsetDigiTime<CbmTofDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmOffsetDigiTime<CbmTofDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmOffsetDigiTime<CbmTofDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmOffsetDigiTime<CbmTofDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmOffsetDigiTimelECbmTofDigigR(void *p) {
      delete ((::CbmOffsetDigiTime<CbmTofDigi>*)p);
   }
   static void deleteArray_CbmOffsetDigiTimelECbmTofDigigR(void *p) {
      delete [] ((::CbmOffsetDigiTime<CbmTofDigi>*)p);
   }
   static void destruct_CbmOffsetDigiTimelECbmTofDigigR(void *p) {
      typedef ::CbmOffsetDigiTime<CbmTofDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmOffsetDigiTime<CbmTofDigi>

//______________________________________________________________________________
template <> void CbmOffsetDigiTime<CbmMuchBeamTimeDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmOffsetDigiTime<CbmMuchBeamTimeDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmOffsetDigiTime<CbmMuchBeamTimeDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmOffsetDigiTime<CbmMuchBeamTimeDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmOffsetDigiTimelECbmMuchBeamTimeDigigR(void *p) {
      delete ((::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>*)p);
   }
   static void deleteArray_CbmOffsetDigiTimelECbmMuchBeamTimeDigigR(void *p) {
      delete [] ((::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>*)p);
   }
   static void destruct_CbmOffsetDigiTimelECbmMuchBeamTimeDigigR(void *p) {
      typedef ::CbmOffsetDigiTime<CbmMuchBeamTimeDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmOffsetDigiTime<CbmMuchBeamTimeDigi>

//______________________________________________________________________________
void CbmOffsetDigiTimeSts::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmOffsetDigiTimeSts.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmOffsetDigiTimeSts::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmOffsetDigiTimeSts::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmOffsetDigiTimeSts(void *p) {
      delete ((::CbmOffsetDigiTimeSts*)p);
   }
   static void deleteArray_CbmOffsetDigiTimeSts(void *p) {
      delete [] ((::CbmOffsetDigiTimeSts*)p);
   }
   static void destruct_CbmOffsetDigiTimeSts(void *p) {
      typedef ::CbmOffsetDigiTimeSts current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmOffsetDigiTimeSts

//______________________________________________________________________________
void CbmOffsetDigiTimeTof::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmOffsetDigiTimeTof.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmOffsetDigiTimeTof::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmOffsetDigiTimeTof::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmOffsetDigiTimeTof(void *p) {
      delete ((::CbmOffsetDigiTimeTof*)p);
   }
   static void deleteArray_CbmOffsetDigiTimeTof(void *p) {
      delete [] ((::CbmOffsetDigiTimeTof*)p);
   }
   static void destruct_CbmOffsetDigiTimeTof(void *p) {
      typedef ::CbmOffsetDigiTimeTof current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmOffsetDigiTimeTof

//______________________________________________________________________________
void CbmOffsetDigiTimeMuch::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmOffsetDigiTimeMuch.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmOffsetDigiTimeMuch::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmOffsetDigiTimeMuch::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmOffsetDigiTimeMuch(void *p) {
      delete ((::CbmOffsetDigiTimeMuch*)p);
   }
   static void deleteArray_CbmOffsetDigiTimeMuch(void *p) {
      delete [] ((::CbmOffsetDigiTimeMuch*)p);
   }
   static void destruct_CbmOffsetDigiTimeMuch(void *p) {
      typedef ::CbmOffsetDigiTimeMuch current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmOffsetDigiTimeMuch

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmOffsetDigiTimeTester(void *p) {
      return  p ? new(p) ::CbmOffsetDigiTimeTester : new ::CbmOffsetDigiTimeTester;
   }
   static void *newArray_CbmOffsetDigiTimeTester(Long_t nElements, void *p) {
      return p ? new(p) ::CbmOffsetDigiTimeTester[nElements] : new ::CbmOffsetDigiTimeTester[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmOffsetDigiTimeTester(void *p) {
      delete ((::CbmOffsetDigiTimeTester*)p);
   }
   static void deleteArray_CbmOffsetDigiTimeTester(void *p) {
      delete [] ((::CbmOffsetDigiTimeTester*)p);
   }
   static void destruct_CbmOffsetDigiTimeTester(void *p) {
      typedef ::CbmOffsetDigiTimeTester current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmOffsetDigiTimeTester

namespace {
  void TriggerDictionaryInitialization_libCbmRecoCal_Impl() {
    static const char* headers[] = {
"CbmOffsetDigiTimeTester.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/calibration",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/calibration",
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
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/calibration/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmRecoCal dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmStsDigi.h")))  __attribute__((annotate("$clingAutoload$CbmOffsetDigiTimeTester.h")))  CbmStsDigi;
template <class TDigi> class __attribute__((annotate("$clingAutoload$CbmOffsetDigiTime.h")))  __attribute__((annotate("$clingAutoload$CbmOffsetDigiTimeTester.h")))  CbmOffsetDigiTime;

class __attribute__((annotate("$clingAutoload$CbmTofDigi.h")))  __attribute__((annotate("$clingAutoload$CbmOffsetDigiTimeTester.h")))  CbmTofDigi;
class __attribute__((annotate("$clingAutoload$CbmMuchBeamTimeDigi.h")))  __attribute__((annotate("$clingAutoload$CbmOffsetDigiTimeTester.h")))  CbmMuchBeamTimeDigi;
class __attribute__((annotate("$clingAutoload$CbmOffsetDigiTimeTester.h")))  CbmOffsetDigiTimeSts;
class __attribute__((annotate("$clingAutoload$CbmOffsetDigiTimeTester.h")))  CbmOffsetDigiTimeTof;
class __attribute__((annotate("$clingAutoload$CbmOffsetDigiTimeTester.h")))  CbmOffsetDigiTimeMuch;
class __attribute__((annotate("$clingAutoload$CbmOffsetDigiTimeTester.h")))  CbmOffsetDigiTimeTester;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmRecoCal dictionary payload"

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
#include "CbmOffsetDigiTimeTester.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmOffsetDigiTime", payloadCode, "@",
"CbmOffsetDigiTime<CbmMuchBeamTimeDigi>", payloadCode, "@",
"CbmOffsetDigiTime<CbmStsDigi>", payloadCode, "@",
"CbmOffsetDigiTime<CbmTofDigi>", payloadCode, "@",
"CbmOffsetDigiTimeMuch", payloadCode, "@",
"CbmOffsetDigiTimeSts", payloadCode, "@",
"CbmOffsetDigiTimeTester", payloadCode, "@",
"CbmOffsetDigiTimeTof", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmRecoCal",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmRecoCal_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmRecoCal_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmRecoCal() {
  TriggerDictionaryInitialization_libCbmRecoCal_Impl();
}
