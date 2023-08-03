// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__KFParticleInterface
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
#include "CbmKFParticleFinder.h"
#include "CbmKFParticleFinderPID.h"
#include "CbmKFParticleFinderQa.h"
#include "CbmKFParticleInterface.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmKFParticleFinder(void *p = nullptr);
   static void *newArray_CbmKFParticleFinder(Long_t size, void *p);
   static void delete_CbmKFParticleFinder(void *p);
   static void deleteArray_CbmKFParticleFinder(void *p);
   static void destruct_CbmKFParticleFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFParticleFinder*)
   {
      ::CbmKFParticleFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFParticleFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFParticleFinder", ::CbmKFParticleFinder::Class_Version(), "CbmKFParticleFinder.h", 32,
                  typeid(::CbmKFParticleFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFParticleFinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFParticleFinder) );
      instance.SetNew(&new_CbmKFParticleFinder);
      instance.SetNewArray(&newArray_CbmKFParticleFinder);
      instance.SetDelete(&delete_CbmKFParticleFinder);
      instance.SetDeleteArray(&deleteArray_CbmKFParticleFinder);
      instance.SetDestructor(&destruct_CbmKFParticleFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFParticleFinder*)
   {
      return GenerateInitInstanceLocal((::CbmKFParticleFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFParticleFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFParticleFinderPID(void *p = nullptr);
   static void *newArray_CbmKFParticleFinderPID(Long_t size, void *p);
   static void delete_CbmKFParticleFinderPID(void *p);
   static void deleteArray_CbmKFParticleFinderPID(void *p);
   static void destruct_CbmKFParticleFinderPID(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFParticleFinderPID*)
   {
      ::CbmKFParticleFinderPID *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFParticleFinderPID >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFParticleFinderPID", ::CbmKFParticleFinderPID::Class_Version(), "CbmKFParticleFinderPID.h", 24,
                  typeid(::CbmKFParticleFinderPID), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFParticleFinderPID::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFParticleFinderPID) );
      instance.SetNew(&new_CbmKFParticleFinderPID);
      instance.SetNewArray(&newArray_CbmKFParticleFinderPID);
      instance.SetDelete(&delete_CbmKFParticleFinderPID);
      instance.SetDeleteArray(&deleteArray_CbmKFParticleFinderPID);
      instance.SetDestructor(&destruct_CbmKFParticleFinderPID);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFParticleFinderPID*)
   {
      return GenerateInitInstanceLocal((::CbmKFParticleFinderPID*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFParticleFinderPID*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFParticleFinderQa(void *p = nullptr);
   static void *newArray_CbmKFParticleFinderQa(Long_t size, void *p);
   static void delete_CbmKFParticleFinderQa(void *p);
   static void deleteArray_CbmKFParticleFinderQa(void *p);
   static void destruct_CbmKFParticleFinderQa(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFParticleFinderQa*)
   {
      ::CbmKFParticleFinderQa *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFParticleFinderQa >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFParticleFinderQa", ::CbmKFParticleFinderQa::Class_Version(), "CbmKFParticleFinderQa.h", 25,
                  typeid(::CbmKFParticleFinderQa), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFParticleFinderQa::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFParticleFinderQa) );
      instance.SetNew(&new_CbmKFParticleFinderQa);
      instance.SetNewArray(&newArray_CbmKFParticleFinderQa);
      instance.SetDelete(&delete_CbmKFParticleFinderQa);
      instance.SetDeleteArray(&deleteArray_CbmKFParticleFinderQa);
      instance.SetDestructor(&destruct_CbmKFParticleFinderQa);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFParticleFinderQa*)
   {
      return GenerateInitInstanceLocal((::CbmKFParticleFinderQa*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFParticleFinderQa*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFParticleInterface(void *p = nullptr);
   static void *newArray_CbmKFParticleInterface(Long_t size, void *p);
   static void delete_CbmKFParticleInterface(void *p);
   static void deleteArray_CbmKFParticleInterface(void *p);
   static void destruct_CbmKFParticleInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFParticleInterface*)
   {
      ::CbmKFParticleInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFParticleInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFParticleInterface", ::CbmKFParticleInterface::Class_Version(), "CbmKFParticleInterface.h", 18,
                  typeid(::CbmKFParticleInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFParticleInterface::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFParticleInterface) );
      instance.SetNew(&new_CbmKFParticleInterface);
      instance.SetNewArray(&newArray_CbmKFParticleInterface);
      instance.SetDelete(&delete_CbmKFParticleInterface);
      instance.SetDeleteArray(&deleteArray_CbmKFParticleInterface);
      instance.SetDestructor(&destruct_CbmKFParticleInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFParticleInterface*)
   {
      return GenerateInitInstanceLocal((::CbmKFParticleInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFParticleInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmKFParticleFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFParticleFinder::Class_Name()
{
   return "CbmKFParticleFinder";
}

//______________________________________________________________________________
const char *CbmKFParticleFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFParticleFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFParticleFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFParticleFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFParticleFinderPID::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFParticleFinderPID::Class_Name()
{
   return "CbmKFParticleFinderPID";
}

//______________________________________________________________________________
const char *CbmKFParticleFinderPID::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleFinderPID*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFParticleFinderPID::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleFinderPID*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFParticleFinderPID::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleFinderPID*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFParticleFinderPID::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleFinderPID*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFParticleFinderQa::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFParticleFinderQa::Class_Name()
{
   return "CbmKFParticleFinderQa";
}

//______________________________________________________________________________
const char *CbmKFParticleFinderQa::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleFinderQa*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFParticleFinderQa::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleFinderQa*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFParticleFinderQa::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleFinderQa*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFParticleFinderQa::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleFinderQa*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFParticleInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFParticleInterface::Class_Name()
{
   return "CbmKFParticleInterface";
}

//______________________________________________________________________________
const char *CbmKFParticleInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFParticleInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFParticleInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFParticleInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFParticleInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmKFParticleFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFParticleFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFParticleFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFParticleFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFParticleFinder(void *p) {
      return  p ? new(p) ::CbmKFParticleFinder : new ::CbmKFParticleFinder;
   }
   static void *newArray_CbmKFParticleFinder(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFParticleFinder[nElements] : new ::CbmKFParticleFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFParticleFinder(void *p) {
      delete ((::CbmKFParticleFinder*)p);
   }
   static void deleteArray_CbmKFParticleFinder(void *p) {
      delete [] ((::CbmKFParticleFinder*)p);
   }
   static void destruct_CbmKFParticleFinder(void *p) {
      typedef ::CbmKFParticleFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFParticleFinder

//______________________________________________________________________________
void CbmKFParticleFinderPID::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFParticleFinderPID.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFParticleFinderPID::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFParticleFinderPID::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFParticleFinderPID(void *p) {
      return  p ? new(p) ::CbmKFParticleFinderPID : new ::CbmKFParticleFinderPID;
   }
   static void *newArray_CbmKFParticleFinderPID(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFParticleFinderPID[nElements] : new ::CbmKFParticleFinderPID[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFParticleFinderPID(void *p) {
      delete ((::CbmKFParticleFinderPID*)p);
   }
   static void deleteArray_CbmKFParticleFinderPID(void *p) {
      delete [] ((::CbmKFParticleFinderPID*)p);
   }
   static void destruct_CbmKFParticleFinderPID(void *p) {
      typedef ::CbmKFParticleFinderPID current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFParticleFinderPID

//______________________________________________________________________________
void CbmKFParticleFinderQa::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFParticleFinderQa.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFParticleFinderQa::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFParticleFinderQa::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFParticleFinderQa(void *p) {
      return  p ? new(p) ::CbmKFParticleFinderQa : new ::CbmKFParticleFinderQa;
   }
   static void *newArray_CbmKFParticleFinderQa(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFParticleFinderQa[nElements] : new ::CbmKFParticleFinderQa[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFParticleFinderQa(void *p) {
      delete ((::CbmKFParticleFinderQa*)p);
   }
   static void deleteArray_CbmKFParticleFinderQa(void *p) {
      delete [] ((::CbmKFParticleFinderQa*)p);
   }
   static void destruct_CbmKFParticleFinderQa(void *p) {
      typedef ::CbmKFParticleFinderQa current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFParticleFinderQa

//______________________________________________________________________________
void CbmKFParticleInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFParticleInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFParticleInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFParticleInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFParticleInterface(void *p) {
      return  p ? new(p) ::CbmKFParticleInterface : new ::CbmKFParticleInterface;
   }
   static void *newArray_CbmKFParticleInterface(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFParticleInterface[nElements] : new ::CbmKFParticleInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFParticleInterface(void *p) {
      delete ((::CbmKFParticleInterface*)p);
   }
   static void deleteArray_CbmKFParticleInterface(void *p) {
      delete [] ((::CbmKFParticleInterface*)p);
   }
   static void destruct_CbmKFParticleInterface(void *p) {
      typedef ::CbmKFParticleInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFParticleInterface

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
   static TClass *vectorlEKFFieldVectorgR_Dictionary();
   static void vectorlEKFFieldVectorgR_TClassManip(TClass*);
   static void *new_vectorlEKFFieldVectorgR(void *p = nullptr);
   static void *newArray_vectorlEKFFieldVectorgR(Long_t size, void *p);
   static void delete_vectorlEKFFieldVectorgR(void *p);
   static void deleteArray_vectorlEKFFieldVectorgR(void *p);
   static void destruct_vectorlEKFFieldVectorgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<KFFieldVector>*)
   {
      vector<KFFieldVector> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<KFFieldVector>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<KFFieldVector>", -2, "vector", 389,
                  typeid(vector<KFFieldVector>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEKFFieldVectorgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<KFFieldVector>) );
      instance.SetNew(&new_vectorlEKFFieldVectorgR);
      instance.SetNewArray(&newArray_vectorlEKFFieldVectorgR);
      instance.SetDelete(&delete_vectorlEKFFieldVectorgR);
      instance.SetDeleteArray(&deleteArray_vectorlEKFFieldVectorgR);
      instance.SetDestructor(&destruct_vectorlEKFFieldVectorgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<KFFieldVector> >()));

      ::ROOT::AddClassAlternate("vector<KFFieldVector>","std::vector<KFFieldVector, std::allocator<KFFieldVector> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<KFFieldVector>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEKFFieldVectorgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<KFFieldVector>*)nullptr)->GetClass();
      vectorlEKFFieldVectorgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEKFFieldVectorgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEKFFieldVectorgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<KFFieldVector> : new vector<KFFieldVector>;
   }
   static void *newArray_vectorlEKFFieldVectorgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<KFFieldVector>[nElements] : new vector<KFFieldVector>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEKFFieldVectorgR(void *p) {
      delete ((vector<KFFieldVector>*)p);
   }
   static void deleteArray_vectorlEKFFieldVectorgR(void *p) {
      delete [] ((vector<KFFieldVector>*)p);
   }
   static void destruct_vectorlEKFFieldVectorgR(void *p) {
      typedef vector<KFFieldVector> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<KFFieldVector>

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

namespace {
  void TriggerDictionaryInitialization_libKFParticleInterface_Impl() {
    static const char* headers[] = {
"CbmKFParticleFinder.h",
"CbmKFParticleFinderPID.h",
"CbmKFParticleFinderQa.h",
"CbmKFParticleInterface.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/KFQA",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/KFQA",
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
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
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
"/home/heinemann/gpuunpacker/cbmroot/build2/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/KF/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libKFParticleInterface dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmKFParticleFinder.h")))  CbmKFParticleFinder;
class __attribute__((annotate("$clingAutoload$CbmKFParticleFinderPID.h")))  CbmKFParticleFinderPID;
class __attribute__((annotate("$clingAutoload$CbmKFParticleFinderQa.h")))  CbmKFParticleFinderQa;
class __attribute__((annotate("$clingAutoload$CbmKFParticleInterface.h")))  CbmKFParticleInterface;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libKFParticleInterface dictionary payload"

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
#include "CbmKFParticleFinder.h"
#include "CbmKFParticleFinderPID.h"
#include "CbmKFParticleFinderQa.h"
#include "CbmKFParticleInterface.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmKFParticleFinder", payloadCode, "@",
"CbmKFParticleFinderPID", payloadCode, "@",
"CbmKFParticleFinderQa", payloadCode, "@",
"CbmKFParticleInterface", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libKFParticleInterface",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libKFParticleInterface_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libKFParticleInterface_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libKFParticleInterface() {
  TriggerDictionaryInitialization_libKFParticleInterface_Impl();
}
