// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmSimBase
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
#include "CbmBeam.h"
#include "CbmBeamProfile.h"
#include "CbmEventGenerator.h"
#include "CbmMCEventFilter.h"
#include "CbmStack.h"
#include "CbmStackFilter.h"
#include "CbmTarget.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmTarget(void *p = nullptr);
   static void *newArray_CbmTarget(Long_t size, void *p);
   static void delete_CbmTarget(void *p);
   static void deleteArray_CbmTarget(void *p);
   static void destruct_CbmTarget(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTarget*)
   {
      ::CbmTarget *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTarget >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTarget", ::CbmTarget::Class_Version(), "CbmTarget.h", 37,
                  typeid(::CbmTarget), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTarget::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTarget) );
      instance.SetNew(&new_CbmTarget);
      instance.SetNewArray(&newArray_CbmTarget);
      instance.SetDelete(&delete_CbmTarget);
      instance.SetDeleteArray(&deleteArray_CbmTarget);
      instance.SetDestructor(&destruct_CbmTarget);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTarget*)
   {
      return GenerateInitInstanceLocal((::CbmTarget*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTarget*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmBeamProfile_Dictionary();
   static void CbmBeamProfile_TClassManip(TClass*);
   static void *new_CbmBeamProfile(void *p = nullptr);
   static void *newArray_CbmBeamProfile(Long_t size, void *p);
   static void delete_CbmBeamProfile(void *p);
   static void deleteArray_CbmBeamProfile(void *p);
   static void destruct_CbmBeamProfile(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBeamProfile*)
   {
      ::CbmBeamProfile *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmBeamProfile));
      static ::ROOT::TGenericClassInfo 
         instance("CbmBeamProfile", "CbmBeamProfile.h", 37,
                  typeid(::CbmBeamProfile), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmBeamProfile_Dictionary, isa_proxy, 4,
                  sizeof(::CbmBeamProfile) );
      instance.SetNew(&new_CbmBeamProfile);
      instance.SetNewArray(&newArray_CbmBeamProfile);
      instance.SetDelete(&delete_CbmBeamProfile);
      instance.SetDeleteArray(&deleteArray_CbmBeamProfile);
      instance.SetDestructor(&destruct_CbmBeamProfile);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBeamProfile*)
   {
      return GenerateInitInstanceLocal((::CbmBeamProfile*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBeamProfile*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmBeamProfile_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmBeamProfile*)nullptr)->GetClass();
      CbmBeamProfile_TClassManip(theClass);
   return theClass;
   }

   static void CbmBeamProfile_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmEventGenerator(void *p = nullptr);
   static void *newArray_CbmEventGenerator(Long_t size, void *p);
   static void delete_CbmEventGenerator(void *p);
   static void deleteArray_CbmEventGenerator(void *p);
   static void destruct_CbmEventGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmEventGenerator*)
   {
      ::CbmEventGenerator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmEventGenerator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmEventGenerator", ::CbmEventGenerator::Class_Version(), "CbmEventGenerator.h", 40,
                  typeid(::CbmEventGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmEventGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::CbmEventGenerator) );
      instance.SetNew(&new_CbmEventGenerator);
      instance.SetNewArray(&newArray_CbmEventGenerator);
      instance.SetDelete(&delete_CbmEventGenerator);
      instance.SetDeleteArray(&deleteArray_CbmEventGenerator);
      instance.SetDestructor(&destruct_CbmEventGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmEventGenerator*)
   {
      return GenerateInitInstanceLocal((::CbmEventGenerator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmEventGenerator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMCEventFilter(void *p = nullptr);
   static void *newArray_CbmMCEventFilter(Long_t size, void *p);
   static void delete_CbmMCEventFilter(void *p);
   static void deleteArray_CbmMCEventFilter(void *p);
   static void destruct_CbmMCEventFilter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMCEventFilter*)
   {
      ::CbmMCEventFilter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMCEventFilter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMCEventFilter", ::CbmMCEventFilter::Class_Version(), "CbmMCEventFilter.h", 41,
                  typeid(::CbmMCEventFilter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMCEventFilter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMCEventFilter) );
      instance.SetNew(&new_CbmMCEventFilter);
      instance.SetNewArray(&newArray_CbmMCEventFilter);
      instance.SetDelete(&delete_CbmMCEventFilter);
      instance.SetDeleteArray(&deleteArray_CbmMCEventFilter);
      instance.SetDestructor(&destruct_CbmMCEventFilter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMCEventFilter*)
   {
      return GenerateInitInstanceLocal((::CbmMCEventFilter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMCEventFilter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStackFilter(void *p = nullptr);
   static void *newArray_CbmStackFilter(Long_t size, void *p);
   static void delete_CbmStackFilter(void *p);
   static void deleteArray_CbmStackFilter(void *p);
   static void destruct_CbmStackFilter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStackFilter*)
   {
      ::CbmStackFilter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStackFilter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStackFilter", ::CbmStackFilter::Class_Version(), "CbmStackFilter.h", 67,
                  typeid(::CbmStackFilter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStackFilter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStackFilter) );
      instance.SetNew(&new_CbmStackFilter);
      instance.SetNewArray(&newArray_CbmStackFilter);
      instance.SetDelete(&delete_CbmStackFilter);
      instance.SetDeleteArray(&deleteArray_CbmStackFilter);
      instance.SetDestructor(&destruct_CbmStackFilter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStackFilter*)
   {
      return GenerateInitInstanceLocal((::CbmStackFilter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStackFilter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStack(void *p = nullptr);
   static void *newArray_CbmStack(Long_t size, void *p);
   static void delete_CbmStack(void *p);
   static void deleteArray_CbmStack(void *p);
   static void destruct_CbmStack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStack*)
   {
      ::CbmStack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStack", ::CbmStack::Class_Version(), "CbmStack.h", 50,
                  typeid(::CbmStack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStack::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStack) );
      instance.SetNew(&new_CbmStack);
      instance.SetNewArray(&newArray_CbmStack);
      instance.SetDelete(&delete_CbmStack);
      instance.SetDeleteArray(&deleteArray_CbmStack);
      instance.SetDestructor(&destruct_CbmStack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStack*)
   {
      return GenerateInitInstanceLocal((::CbmStack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmTarget::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTarget::Class_Name()
{
   return "CbmTarget";
}

//______________________________________________________________________________
const char *CbmTarget::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTarget*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTarget::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTarget*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTarget::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTarget*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTarget::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTarget*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmEventGenerator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmEventGenerator::Class_Name()
{
   return "CbmEventGenerator";
}

//______________________________________________________________________________
const char *CbmEventGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmEventGenerator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmEventGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmEventGenerator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmEventGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmEventGenerator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmEventGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmEventGenerator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMCEventFilter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMCEventFilter::Class_Name()
{
   return "CbmMCEventFilter";
}

//______________________________________________________________________________
const char *CbmMCEventFilter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCEventFilter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMCEventFilter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCEventFilter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMCEventFilter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCEventFilter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMCEventFilter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCEventFilter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStackFilter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStackFilter::Class_Name()
{
   return "CbmStackFilter";
}

//______________________________________________________________________________
const char *CbmStackFilter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStackFilter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStackFilter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStackFilter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStackFilter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStackFilter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStackFilter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStackFilter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStack::Class_Name()
{
   return "CbmStack";
}

//______________________________________________________________________________
const char *CbmStack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmTarget::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTarget.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTarget::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTarget::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTarget(void *p) {
      return  p ? new(p) ::CbmTarget : new ::CbmTarget;
   }
   static void *newArray_CbmTarget(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTarget[nElements] : new ::CbmTarget[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTarget(void *p) {
      delete ((::CbmTarget*)p);
   }
   static void deleteArray_CbmTarget(void *p) {
      delete [] ((::CbmTarget*)p);
   }
   static void destruct_CbmTarget(void *p) {
      typedef ::CbmTarget current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTarget

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBeamProfile(void *p) {
      return  p ? new(p) ::CbmBeamProfile : new ::CbmBeamProfile;
   }
   static void *newArray_CbmBeamProfile(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBeamProfile[nElements] : new ::CbmBeamProfile[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBeamProfile(void *p) {
      delete ((::CbmBeamProfile*)p);
   }
   static void deleteArray_CbmBeamProfile(void *p) {
      delete [] ((::CbmBeamProfile*)p);
   }
   static void destruct_CbmBeamProfile(void *p) {
      typedef ::CbmBeamProfile current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBeamProfile

//______________________________________________________________________________
void CbmEventGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmEventGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmEventGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmEventGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmEventGenerator(void *p) {
      return  p ? new(p) ::CbmEventGenerator : new ::CbmEventGenerator;
   }
   static void *newArray_CbmEventGenerator(Long_t nElements, void *p) {
      return p ? new(p) ::CbmEventGenerator[nElements] : new ::CbmEventGenerator[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmEventGenerator(void *p) {
      delete ((::CbmEventGenerator*)p);
   }
   static void deleteArray_CbmEventGenerator(void *p) {
      delete [] ((::CbmEventGenerator*)p);
   }
   static void destruct_CbmEventGenerator(void *p) {
      typedef ::CbmEventGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmEventGenerator

//______________________________________________________________________________
void CbmMCEventFilter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMCEventFilter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMCEventFilter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMCEventFilter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMCEventFilter(void *p) {
      return  p ? new(p) ::CbmMCEventFilter : new ::CbmMCEventFilter;
   }
   static void *newArray_CbmMCEventFilter(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMCEventFilter[nElements] : new ::CbmMCEventFilter[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMCEventFilter(void *p) {
      delete ((::CbmMCEventFilter*)p);
   }
   static void deleteArray_CbmMCEventFilter(void *p) {
      delete [] ((::CbmMCEventFilter*)p);
   }
   static void destruct_CbmMCEventFilter(void *p) {
      typedef ::CbmMCEventFilter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMCEventFilter

//______________________________________________________________________________
void CbmStackFilter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStackFilter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStackFilter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStackFilter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStackFilter(void *p) {
      return  p ? new(p) ::CbmStackFilter : new ::CbmStackFilter;
   }
   static void *newArray_CbmStackFilter(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStackFilter[nElements] : new ::CbmStackFilter[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStackFilter(void *p) {
      delete ((::CbmStackFilter*)p);
   }
   static void deleteArray_CbmStackFilter(void *p) {
      delete [] ((::CbmStackFilter*)p);
   }
   static void destruct_CbmStackFilter(void *p) {
      typedef ::CbmStackFilter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStackFilter

//______________________________________________________________________________
void CbmStack::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStack::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStack(void *p) {
      return  p ? new(p) ::CbmStack : new ::CbmStack;
   }
   static void *newArray_CbmStack(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStack[nElements] : new ::CbmStack[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStack(void *p) {
      delete ((::CbmStack*)p);
   }
   static void deleteArray_CbmStack(void *p) {
      delete [] ((::CbmStack*)p);
   }
   static void destruct_CbmStack(void *p) {
      typedef ::CbmStack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStack

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

namespace ROOT {
   static TClass *maplEECbmModuleIdcOunsignedsPintgR_Dictionary();
   static void maplEECbmModuleIdcOunsignedsPintgR_TClassManip(TClass*);
   static void *new_maplEECbmModuleIdcOunsignedsPintgR(void *p = nullptr);
   static void *newArray_maplEECbmModuleIdcOunsignedsPintgR(Long_t size, void *p);
   static void delete_maplEECbmModuleIdcOunsignedsPintgR(void *p);
   static void deleteArray_maplEECbmModuleIdcOunsignedsPintgR(void *p);
   static void destruct_maplEECbmModuleIdcOunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<ECbmModuleId,unsigned int>*)
   {
      map<ECbmModuleId,unsigned int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<ECbmModuleId,unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<ECbmModuleId,unsigned int>", -2, "map", 100,
                  typeid(map<ECbmModuleId,unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEECbmModuleIdcOunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<ECbmModuleId,unsigned int>) );
      instance.SetNew(&new_maplEECbmModuleIdcOunsignedsPintgR);
      instance.SetNewArray(&newArray_maplEECbmModuleIdcOunsignedsPintgR);
      instance.SetDelete(&delete_maplEECbmModuleIdcOunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_maplEECbmModuleIdcOunsignedsPintgR);
      instance.SetDestructor(&destruct_maplEECbmModuleIdcOunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<ECbmModuleId,unsigned int> >()));

      ::ROOT::AddClassAlternate("map<ECbmModuleId,unsigned int>","std::map<ECbmModuleId, unsigned int, std::less<ECbmModuleId>, std::allocator<std::pair<ECbmModuleId const, unsigned int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<ECbmModuleId,unsigned int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEECbmModuleIdcOunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<ECbmModuleId,unsigned int>*)nullptr)->GetClass();
      maplEECbmModuleIdcOunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEECbmModuleIdcOunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEECbmModuleIdcOunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,unsigned int> : new map<ECbmModuleId,unsigned int>;
   }
   static void *newArray_maplEECbmModuleIdcOunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,unsigned int>[nElements] : new map<ECbmModuleId,unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEECbmModuleIdcOunsignedsPintgR(void *p) {
      delete ((map<ECbmModuleId,unsigned int>*)p);
   }
   static void deleteArray_maplEECbmModuleIdcOunsignedsPintgR(void *p) {
      delete [] ((map<ECbmModuleId,unsigned int>*)p);
   }
   static void destruct_maplEECbmModuleIdcOunsignedsPintgR(void *p) {
      typedef map<ECbmModuleId,unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<ECbmModuleId,unsigned int>

namespace ROOT {
   static TClass *maplEECbmDataTypecOintgR_Dictionary();
   static void maplEECbmDataTypecOintgR_TClassManip(TClass*);
   static void *new_maplEECbmDataTypecOintgR(void *p = nullptr);
   static void *newArray_maplEECbmDataTypecOintgR(Long_t size, void *p);
   static void delete_maplEECbmDataTypecOintgR(void *p);
   static void deleteArray_maplEECbmDataTypecOintgR(void *p);
   static void destruct_maplEECbmDataTypecOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<ECbmDataType,int>*)
   {
      map<ECbmDataType,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<ECbmDataType,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<ECbmDataType,int>", -2, "map", 100,
                  typeid(map<ECbmDataType,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEECbmDataTypecOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<ECbmDataType,int>) );
      instance.SetNew(&new_maplEECbmDataTypecOintgR);
      instance.SetNewArray(&newArray_maplEECbmDataTypecOintgR);
      instance.SetDelete(&delete_maplEECbmDataTypecOintgR);
      instance.SetDeleteArray(&deleteArray_maplEECbmDataTypecOintgR);
      instance.SetDestructor(&destruct_maplEECbmDataTypecOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<ECbmDataType,int> >()));

      ::ROOT::AddClassAlternate("map<ECbmDataType,int>","std::map<ECbmDataType, int, std::less<ECbmDataType>, std::allocator<std::pair<ECbmDataType const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<ECbmDataType,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEECbmDataTypecOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<ECbmDataType,int>*)nullptr)->GetClass();
      maplEECbmDataTypecOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEECbmDataTypecOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEECbmDataTypecOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmDataType,int> : new map<ECbmDataType,int>;
   }
   static void *newArray_maplEECbmDataTypecOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmDataType,int>[nElements] : new map<ECbmDataType,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEECbmDataTypecOintgR(void *p) {
      delete ((map<ECbmDataType,int>*)p);
   }
   static void deleteArray_maplEECbmDataTypecOintgR(void *p) {
      delete [] ((map<ECbmDataType,int>*)p);
   }
   static void destruct_maplEECbmDataTypecOintgR(void *p) {
      typedef map<ECbmDataType,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<ECbmDataType,int>

namespace {
  void TriggerDictionaryInitialization_libCbmSimBase_Impl() {
    static const char* headers[] = {
"CbmBeam.h",
"CbmBeamProfile.h",
"CbmEventGenerator.h",
"CbmMCEventFilter.h",
"CbmStack.h",
"CbmStackFilter.h",
"CbmTarget.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
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
"/home/heinemann/gpuunpacker/cbmroot/build2/sim/transport/base/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmSimBase dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmTarget.h")))  __attribute__((annotate("$clingAutoload$CbmBeamProfile.h")))  CbmTarget;
class __attribute__((annotate("$clingAutoload$CbmBeamProfile.h")))  CbmBeamProfile;
class __attribute__((annotate("$clingAutoload$CbmEventGenerator.h")))  CbmEventGenerator;
class __attribute__((annotate("$clingAutoload$CbmMCEventFilter.h")))  CbmMCEventFilter;
class __attribute__((annotate("$clingAutoload$CbmStackFilter.h")))  __attribute__((annotate("$clingAutoload$CbmStack.h")))  CbmStackFilter;
class __attribute__((annotate("$clingAutoload$CbmStack.h")))  CbmStack;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmSimBase dictionary payload"

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
#include "CbmBeam.h"
#include "CbmBeamProfile.h"
#include "CbmEventGenerator.h"
#include "CbmMCEventFilter.h"
#include "CbmStack.h"
#include "CbmStackFilter.h"
#include "CbmTarget.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmBeamProfile", payloadCode, "@",
"CbmEventGenerator", payloadCode, "@",
"CbmMCEventFilter", payloadCode, "@",
"CbmStack", payloadCode, "@",
"CbmStackFilter", payloadCode, "@",
"CbmTarget", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmSimBase",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmSimBase_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmSimBase_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmSimBase() {
  TriggerDictionaryInitialization_libCbmSimBase_Impl();
}
