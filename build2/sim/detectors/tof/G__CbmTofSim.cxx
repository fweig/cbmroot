// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmTofSim
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
#include "CbmTof.h"
#include "CbmTofMergeMcPoints.h"
#include "CbmTofDigitize.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmTof(void *p = nullptr);
   static void *newArray_CbmTof(Long_t size, void *p);
   static void delete_CbmTof(void *p);
   static void deleteArray_CbmTof(void *p);
   static void destruct_CbmTof(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTof*)
   {
      ::CbmTof *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTof >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTof", ::CbmTof::Class_Version(), "CbmTof.h", 39,
                  typeid(::CbmTof), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTof::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTof) );
      instance.SetNew(&new_CbmTof);
      instance.SetNewArray(&newArray_CbmTof);
      instance.SetDelete(&delete_CbmTof);
      instance.SetDeleteArray(&deleteArray_CbmTof);
      instance.SetDestructor(&destruct_CbmTof);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTof*)
   {
      return GenerateInitInstanceLocal((::CbmTof*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTof*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofMergeMcPoints(void *p = nullptr);
   static void *newArray_CbmTofMergeMcPoints(Long_t size, void *p);
   static void delete_CbmTofMergeMcPoints(void *p);
   static void deleteArray_CbmTofMergeMcPoints(void *p);
   static void destruct_CbmTofMergeMcPoints(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofMergeMcPoints*)
   {
      ::CbmTofMergeMcPoints *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofMergeMcPoints >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofMergeMcPoints", ::CbmTofMergeMcPoints::Class_Version(), "CbmTofMergeMcPoints.h", 24,
                  typeid(::CbmTofMergeMcPoints), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofMergeMcPoints::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofMergeMcPoints) );
      instance.SetNew(&new_CbmTofMergeMcPoints);
      instance.SetNewArray(&newArray_CbmTofMergeMcPoints);
      instance.SetDelete(&delete_CbmTofMergeMcPoints);
      instance.SetDeleteArray(&deleteArray_CbmTofMergeMcPoints);
      instance.SetDestructor(&destruct_CbmTofMergeMcPoints);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofMergeMcPoints*)
   {
      return GenerateInitInstanceLocal((::CbmTofMergeMcPoints*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofMergeMcPoints*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigitizelECbmTofDigigR_Dictionary();
   static void CbmDigitizelECbmTofDigigR_TClassManip(TClass*);
   static void delete_CbmDigitizelECbmTofDigigR(void *p);
   static void deleteArray_CbmDigitizelECbmTofDigigR(void *p);
   static void destruct_CbmDigitizelECbmTofDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigitize<CbmTofDigi>*)
   {
      ::CbmDigitize<CbmTofDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigitize<CbmTofDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigitize<CbmTofDigi>", ::CbmDigitize<CbmTofDigi>::Class_Version(), "CbmDigitize.h", 44,
                  typeid(::CbmDigitize<CbmTofDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigitizelECbmTofDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigitize<CbmTofDigi>) );
      instance.SetDelete(&delete_CbmDigitizelECbmTofDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigitizelECbmTofDigigR);
      instance.SetDestructor(&destruct_CbmDigitizelECbmTofDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigitize<CbmTofDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigitize<CbmTofDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigitize<CbmTofDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigitizelECbmTofDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmTofDigi>*)nullptr)->GetClass();
      CbmDigitizelECbmTofDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigitizelECbmTofDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofDigitize(void *p = nullptr);
   static void *newArray_CbmTofDigitize(Long_t size, void *p);
   static void delete_CbmTofDigitize(void *p);
   static void deleteArray_CbmTofDigitize(void *p);
   static void destruct_CbmTofDigitize(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofDigitize*)
   {
      ::CbmTofDigitize *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofDigitize >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofDigitize", ::CbmTofDigitize::Class_Version(), "CbmTofDigitize.h", 39,
                  typeid(::CbmTofDigitize), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofDigitize::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofDigitize) );
      instance.SetNew(&new_CbmTofDigitize);
      instance.SetNewArray(&newArray_CbmTofDigitize);
      instance.SetDelete(&delete_CbmTofDigitize);
      instance.SetDeleteArray(&deleteArray_CbmTofDigitize);
      instance.SetDestructor(&destruct_CbmTofDigitize);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofDigitize*)
   {
      return GenerateInitInstanceLocal((::CbmTofDigitize*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofDigitize*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmTof::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTof::Class_Name()
{
   return "CbmTof";
}

//______________________________________________________________________________
const char *CbmTof::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTof*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTof::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTof*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTof::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTof*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTof::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTof*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofMergeMcPoints::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofMergeMcPoints::Class_Name()
{
   return "CbmTofMergeMcPoints";
}

//______________________________________________________________________________
const char *CbmTofMergeMcPoints::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofMergeMcPoints*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofMergeMcPoints::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofMergeMcPoints*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofMergeMcPoints::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofMergeMcPoints*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofMergeMcPoints::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofMergeMcPoints*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigitize<CbmTofDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmTofDigi>::Class_Name()
{
   return "CbmDigitize<CbmTofDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmTofDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmTofDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigitize<CbmTofDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmTofDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmTofDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmTofDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmTofDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmTofDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofDigitize::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofDigitize::Class_Name()
{
   return "CbmTofDigitize";
}

//______________________________________________________________________________
const char *CbmTofDigitize::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigitize*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofDigitize::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigitize*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofDigitize::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigitize*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofDigitize::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigitize*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmTof::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTof.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTof::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTof::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTof(void *p) {
      return  p ? new(p) ::CbmTof : new ::CbmTof;
   }
   static void *newArray_CbmTof(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTof[nElements] : new ::CbmTof[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTof(void *p) {
      delete ((::CbmTof*)p);
   }
   static void deleteArray_CbmTof(void *p) {
      delete [] ((::CbmTof*)p);
   }
   static void destruct_CbmTof(void *p) {
      typedef ::CbmTof current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTof

//______________________________________________________________________________
void CbmTofMergeMcPoints::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofMergeMcPoints.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofMergeMcPoints::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofMergeMcPoints::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofMergeMcPoints(void *p) {
      return  p ? new(p) ::CbmTofMergeMcPoints : new ::CbmTofMergeMcPoints;
   }
   static void *newArray_CbmTofMergeMcPoints(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofMergeMcPoints[nElements] : new ::CbmTofMergeMcPoints[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofMergeMcPoints(void *p) {
      delete ((::CbmTofMergeMcPoints*)p);
   }
   static void deleteArray_CbmTofMergeMcPoints(void *p) {
      delete [] ((::CbmTofMergeMcPoints*)p);
   }
   static void destruct_CbmTofMergeMcPoints(void *p) {
      typedef ::CbmTofMergeMcPoints current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofMergeMcPoints

//______________________________________________________________________________
template <> void CbmDigitize<CbmTofDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigitize<CbmTofDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigitize<CbmTofDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigitize<CbmTofDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmDigitizelECbmTofDigigR(void *p) {
      delete ((::CbmDigitize<CbmTofDigi>*)p);
   }
   static void deleteArray_CbmDigitizelECbmTofDigigR(void *p) {
      delete [] ((::CbmDigitize<CbmTofDigi>*)p);
   }
   static void destruct_CbmDigitizelECbmTofDigigR(void *p) {
      typedef ::CbmDigitize<CbmTofDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigitize<CbmTofDigi>

//______________________________________________________________________________
void CbmTofDigitize::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofDigitize.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofDigitize::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofDigitize::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofDigitize(void *p) {
      return  p ? new(p) ::CbmTofDigitize : new ::CbmTofDigitize;
   }
   static void *newArray_CbmTofDigitize(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofDigitize[nElements] : new ::CbmTofDigitize[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofDigitize(void *p) {
      delete ((::CbmTofDigitize*)p);
   }
   static void deleteArray_CbmTofDigitize(void *p) {
      delete [] ((::CbmTofDigitize*)p);
   }
   static void destruct_CbmTofDigitize(void *p) {
      typedef ::CbmTofDigitize current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofDigitize

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >*)
   {
      vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >", -2, "vector", 389,
                  typeid(vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >","std::vector<std::vector<std::vector<std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > >, std::allocator<std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > > > >, std::allocator<std::vector<std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > >, std::allocator<std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > > > > > >, std::allocator<std::vector<std::vector<std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > >, std::allocator<std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > > > >, std::allocator<std::vector<std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > >, std::allocator<std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > > > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > > : new vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >[nElements] : new vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<vector<int> > > >*)
   {
      vector<vector<vector<vector<int> > > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<vector<int> > > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<vector<int> > > >", -2, "vector", 389,
                  typeid(vector<vector<vector<vector<int> > > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<vector<int> > > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<vector<int> > > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<vector<int> > > >","std::vector<std::vector<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >, std::allocator<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > > >, std::allocator<std::vector<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >, std::allocator<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<vector<int> > > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<vector<int> > > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<int> > > > : new vector<vector<vector<vector<int> > > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<vector<int> > > >[nElements] : new vector<vector<vector<vector<int> > > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<vector<int> > > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<vector<int> > > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEvectorlEintgRsPgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<vector<int> > > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<vector<int> > > >

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >*)
   {
      vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >", -2, "vector", 389,
                  typeid(vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >","std::vector<std::vector<std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > >, std::allocator<std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > > > >, std::allocator<std::vector<std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > >, std::allocator<std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > : new vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >[nElements] : new vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlEintgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEintgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlEintgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<int> > >*)
   {
      vector<vector<vector<int> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<int> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<int> > >", -2, "vector", 389,
                  typeid(vector<vector<vector<int> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEintgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<int> > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEintgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEintgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEintgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEintgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEintgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<int> > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<int> > >","std::vector<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >, std::allocator<std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<int> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEintgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<int> > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlEintgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEintgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<int> > > : new vector<vector<vector<int> > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEintgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<int> > >[nElements] : new vector<vector<vector<int> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<int> > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<int> > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEintgRsPgRsPgR(void *p) {
      typedef vector<vector<vector<int> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<int> > >

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlEdoublegRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEdoublegRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<double> > >*)
   {
      vector<vector<vector<double> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<double> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<double> > >", -2, "vector", 389,
                  typeid(vector<vector<vector<double> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEdoublegRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<double> > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<double> > > >()));

      ::ROOT::AddClassAlternate("vector<vector<vector<double> > >","std::vector<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >, std::allocator<std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<double> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEdoublegRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<double> > >*)nullptr)->GetClass();
      vectorlEvectorlEvectorlEdoublegRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEdoublegRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<double> > > : new vector<vector<vector<double> > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<double> > >[nElements] : new vector<vector<vector<double> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<double> > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<double> > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      typedef vector<vector<vector<double> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<double> > >

namespace ROOT {
   static TClass *vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<pair<CbmTofDigi*,CbmMatch*> > >*)
   {
      vector<vector<pair<CbmTofDigi*,CbmMatch*> > > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<pair<CbmTofDigi*,CbmMatch*> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<pair<CbmTofDigi*,CbmMatch*> > >", -2, "vector", 389,
                  typeid(vector<vector<pair<CbmTofDigi*,CbmMatch*> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<pair<CbmTofDigi*,CbmMatch*> > >) );
      instance.SetNew(&new_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<pair<CbmTofDigi*,CbmMatch*> > > >()));

      ::ROOT::AddClassAlternate("vector<vector<pair<CbmTofDigi*,CbmMatch*> > >","std::vector<std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > >, std::allocator<std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<pair<CbmTofDigi*,CbmMatch*> > >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<pair<CbmTofDigi*,CbmMatch*> > >*)nullptr)->GetClass();
      vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<pair<CbmTofDigi*,CbmMatch*> > > : new vector<vector<pair<CbmTofDigi*,CbmMatch*> > >;
   }
   static void *newArray_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<pair<CbmTofDigi*,CbmMatch*> > >[nElements] : new vector<vector<pair<CbmTofDigi*,CbmMatch*> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR(void *p) {
      delete ((vector<vector<pair<CbmTofDigi*,CbmMatch*> > >*)p);
   }
   static void deleteArray_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR(void *p) {
      delete [] ((vector<vector<pair<CbmTofDigi*,CbmMatch*> > >*)p);
   }
   static void destruct_vectorlEvectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgRsPgR(void *p) {
      typedef vector<vector<pair<CbmTofDigi*,CbmMatch*> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<pair<CbmTofDigi*,CbmMatch*> > >

namespace ROOT {
   static TClass *vectorlEvectorlEintgRsPgR_Dictionary();
   static void vectorlEvectorlEintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEintgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEintgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEintgRsPgR(void *p);
   static void destruct_vectorlEvectorlEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<int> >*)
   {
      vector<vector<int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<int> >", -2, "vector", 389,
                  typeid(vector<vector<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<int> >) );
      instance.SetNew(&new_vectorlEvectorlEintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEintgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<int> > >()));

      ::ROOT::AddClassAlternate("vector<vector<int> >","std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<int> >*)nullptr)->GetClass();
      vectorlEvectorlEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<int> > : new vector<vector<int> >;
   }
   static void *newArray_vectorlEvectorlEintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<int> >[nElements] : new vector<vector<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEintgRsPgR(void *p) {
      delete ((vector<vector<int> >*)p);
   }
   static void deleteArray_vectorlEvectorlEintgRsPgR(void *p) {
      delete [] ((vector<vector<int> >*)p);
   }
   static void destruct_vectorlEvectorlEintgRsPgR(void *p) {
      typedef vector<vector<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<int> >

namespace ROOT {
   static TClass *vectorlEvectorlEdoublegRsPgR_Dictionary();
   static void vectorlEvectorlEdoublegRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEdoublegRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEdoublegRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEdoublegRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEdoublegRsPgR(void *p);
   static void destruct_vectorlEvectorlEdoublegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<double> >*)
   {
      vector<vector<double> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<double> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<double> >", -2, "vector", 389,
                  typeid(vector<vector<double> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEdoublegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<double> >) );
      instance.SetNew(&new_vectorlEvectorlEdoublegRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEdoublegRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEdoublegRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEdoublegRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEdoublegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<double> > >()));

      ::ROOT::AddClassAlternate("vector<vector<double> >","std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<double> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEdoublegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<double> >*)nullptr)->GetClass();
      vectorlEvectorlEdoublegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEdoublegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEdoublegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<double> > : new vector<vector<double> >;
   }
   static void *newArray_vectorlEvectorlEdoublegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<double> >[nElements] : new vector<vector<double> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEdoublegRsPgR(void *p) {
      delete ((vector<vector<double> >*)p);
   }
   static void deleteArray_vectorlEvectorlEdoublegRsPgR(void *p) {
      delete [] ((vector<vector<double> >*)p);
   }
   static void destruct_vectorlEvectorlEdoublegRsPgR(void *p) {
      typedef vector<vector<double> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<double> >

namespace ROOT {
   static TClass *vectorlEvectorlEULong64_tgRsPgR_Dictionary();
   static void vectorlEvectorlEULong64_tgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEULong64_tgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEULong64_tgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEULong64_tgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEULong64_tgRsPgR(void *p);
   static void destruct_vectorlEvectorlEULong64_tgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<ULong64_t> >*)
   {
      vector<vector<ULong64_t> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<ULong64_t> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<ULong64_t> >", -2, "vector", 389,
                  typeid(vector<vector<ULong64_t> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEULong64_tgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<ULong64_t> >) );
      instance.SetNew(&new_vectorlEvectorlEULong64_tgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEULong64_tgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEULong64_tgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEULong64_tgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEULong64_tgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<ULong64_t> > >()));

      ::ROOT::AddClassAlternate("vector<vector<ULong64_t> >","std::vector<std::vector<unsigned long long, std::allocator<unsigned long long> >, std::allocator<std::vector<unsigned long long, std::allocator<unsigned long long> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<ULong64_t> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEULong64_tgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<ULong64_t> >*)nullptr)->GetClass();
      vectorlEvectorlEULong64_tgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEULong64_tgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEULong64_tgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<ULong64_t> > : new vector<vector<ULong64_t> >;
   }
   static void *newArray_vectorlEvectorlEULong64_tgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<ULong64_t> >[nElements] : new vector<vector<ULong64_t> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEULong64_tgRsPgR(void *p) {
      delete ((vector<vector<ULong64_t> >*)p);
   }
   static void deleteArray_vectorlEvectorlEULong64_tgRsPgR(void *p) {
      delete [] ((vector<vector<ULong64_t> >*)p);
   }
   static void destruct_vectorlEvectorlEULong64_tgRsPgR(void *p) {
      typedef vector<vector<ULong64_t> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<ULong64_t> >

namespace ROOT {
   static TClass *vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR_Dictionary();
   static void vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR(void *p);
   static void deleteArray_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR(void *p);
   static void destruct_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<CbmTofDigi*,CbmMatch*> >*)
   {
      vector<pair<CbmTofDigi*,CbmMatch*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<CbmTofDigi*,CbmMatch*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<CbmTofDigi*,CbmMatch*> >", -2, "vector", 389,
                  typeid(vector<pair<CbmTofDigi*,CbmMatch*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<CbmTofDigi*,CbmMatch*> >) );
      instance.SetNew(&new_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<CbmTofDigi*,CbmMatch*> > >()));

      ::ROOT::AddClassAlternate("vector<pair<CbmTofDigi*,CbmMatch*> >","std::vector<std::pair<CbmTofDigi*, CbmMatch*>, std::allocator<std::pair<CbmTofDigi*, CbmMatch*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pair<CbmTofDigi*,CbmMatch*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<CbmTofDigi*,CbmMatch*> >*)nullptr)->GetClass();
      vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<CbmTofDigi*,CbmMatch*> > : new vector<pair<CbmTofDigi*,CbmMatch*> >;
   }
   static void *newArray_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<CbmTofDigi*,CbmMatch*> >[nElements] : new vector<pair<CbmTofDigi*,CbmMatch*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR(void *p) {
      delete ((vector<pair<CbmTofDigi*,CbmMatch*> >*)p);
   }
   static void deleteArray_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR(void *p) {
      delete [] ((vector<pair<CbmTofDigi*,CbmMatch*> >*)p);
   }
   static void destruct_vectorlEpairlECbmTofDigimUcOCbmMatchmUgRsPgR(void *p) {
      typedef vector<pair<CbmTofDigi*,CbmMatch*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<CbmTofDigi*,CbmMatch*> >

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
   static TClass *vectorlEULong64_tgR_Dictionary();
   static void vectorlEULong64_tgR_TClassManip(TClass*);
   static void *new_vectorlEULong64_tgR(void *p = nullptr);
   static void *newArray_vectorlEULong64_tgR(Long_t size, void *p);
   static void delete_vectorlEULong64_tgR(void *p);
   static void deleteArray_vectorlEULong64_tgR(void *p);
   static void destruct_vectorlEULong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ULong64_t>*)
   {
      vector<ULong64_t> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ULong64_t>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ULong64_t>", -2, "vector", 389,
                  typeid(vector<ULong64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEULong64_tgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ULong64_t>) );
      instance.SetNew(&new_vectorlEULong64_tgR);
      instance.SetNewArray(&newArray_vectorlEULong64_tgR);
      instance.SetDelete(&delete_vectorlEULong64_tgR);
      instance.SetDeleteArray(&deleteArray_vectorlEULong64_tgR);
      instance.SetDestructor(&destruct_vectorlEULong64_tgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ULong64_t> >()));

      ::ROOT::AddClassAlternate("vector<ULong64_t>","std::vector<unsigned long long, std::allocator<unsigned long long> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<ULong64_t>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEULong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<ULong64_t>*)nullptr)->GetClass();
      vectorlEULong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEULong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEULong64_tgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ULong64_t> : new vector<ULong64_t>;
   }
   static void *newArray_vectorlEULong64_tgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ULong64_t>[nElements] : new vector<ULong64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEULong64_tgR(void *p) {
      delete ((vector<ULong64_t>*)p);
   }
   static void deleteArray_vectorlEULong64_tgR(void *p) {
      delete [] ((vector<ULong64_t>*)p);
   }
   static void destruct_vectorlEULong64_tgR(void *p) {
      typedef vector<ULong64_t> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<ULong64_t>

namespace ROOT {
   static TClass *vectorlETH1mUgR_Dictionary();
   static void vectorlETH1mUgR_TClassManip(TClass*);
   static void *new_vectorlETH1mUgR(void *p = nullptr);
   static void *newArray_vectorlETH1mUgR(Long_t size, void *p);
   static void delete_vectorlETH1mUgR(void *p);
   static void deleteArray_vectorlETH1mUgR(void *p);
   static void destruct_vectorlETH1mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TH1*>*)
   {
      vector<TH1*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TH1*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TH1*>", -2, "vector", 389,
                  typeid(vector<TH1*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETH1mUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TH1*>) );
      instance.SetNew(&new_vectorlETH1mUgR);
      instance.SetNewArray(&newArray_vectorlETH1mUgR);
      instance.SetDelete(&delete_vectorlETH1mUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETH1mUgR);
      instance.SetDestructor(&destruct_vectorlETH1mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TH1*> >()));

      ::ROOT::AddClassAlternate("vector<TH1*>","std::vector<TH1*, std::allocator<TH1*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TH1*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETH1mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TH1*>*)nullptr)->GetClass();
      vectorlETH1mUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETH1mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETH1mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH1*> : new vector<TH1*>;
   }
   static void *newArray_vectorlETH1mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TH1*>[nElements] : new vector<TH1*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETH1mUgR(void *p) {
      delete ((vector<TH1*>*)p);
   }
   static void deleteArray_vectorlETH1mUgR(void *p) {
      delete [] ((vector<TH1*>*)p);
   }
   static void destruct_vectorlETH1mUgR(void *p) {
      typedef vector<TH1*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TH1*>

namespace ROOT {
   static TClass *maplEintcOvectorlEintgRsPgR_Dictionary();
   static void maplEintcOvectorlEintgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOvectorlEintgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOvectorlEintgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOvectorlEintgRsPgR(void *p);
   static void deleteArray_maplEintcOvectorlEintgRsPgR(void *p);
   static void destruct_maplEintcOvectorlEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,vector<int> >*)
   {
      map<int,vector<int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,vector<int> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,vector<int> >", -2, "map", 100,
                  typeid(map<int,vector<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOvectorlEintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,vector<int> >) );
      instance.SetNew(&new_maplEintcOvectorlEintgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOvectorlEintgRsPgR);
      instance.SetDelete(&delete_maplEintcOvectorlEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOvectorlEintgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOvectorlEintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,vector<int> > >()));

      ::ROOT::AddClassAlternate("map<int,vector<int> >","std::map<int, std::vector<int, std::allocator<int> >, std::less<int>, std::allocator<std::pair<int const, std::vector<int, std::allocator<int> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,vector<int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOvectorlEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,vector<int> >*)nullptr)->GetClass();
      maplEintcOvectorlEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOvectorlEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOvectorlEintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<int> > : new map<int,vector<int> >;
   }
   static void *newArray_maplEintcOvectorlEintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<int> >[nElements] : new map<int,vector<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOvectorlEintgRsPgR(void *p) {
      delete ((map<int,vector<int> >*)p);
   }
   static void deleteArray_maplEintcOvectorlEintgRsPgR(void *p) {
      delete [] ((map<int,vector<int> >*)p);
   }
   static void destruct_maplEintcOvectorlEintgRsPgR(void *p) {
      typedef map<int,vector<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,vector<int> >

namespace {
  void TriggerDictionaryInitialization_libCbmTofSim_Impl() {
    static const char* headers[] = {
"CbmTof.h",
"CbmTofMergeMcPoints.h",
"CbmTofDigitize.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/tof",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/tof",
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
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/tof",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/tof/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmTofSim dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmTof.h")))  CbmTof;
class __attribute__((annotate("$clingAutoload$CbmTofMergeMcPoints.h")))  CbmTofMergeMcPoints;
class __attribute__((annotate("$clingAutoload$CbmTofDigi.h")))  __attribute__((annotate("$clingAutoload$CbmTofDigitize.h")))  CbmTofDigi;
template <class Digi> class __attribute__((annotate("$clingAutoload$CbmDigitize.h")))  __attribute__((annotate("$clingAutoload$CbmTofDigitize.h")))  CbmDigitize;

class __attribute__((annotate("$clingAutoload$CbmTofDigitize.h")))  CbmTofDigitize;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmTofSim dictionary payload"

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
#include "CbmTof.h"
#include "CbmTofMergeMcPoints.h"
#include "CbmTofDigitize.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmDigitize<CbmTofDigi>", payloadCode, "@",
"CbmTof", payloadCode, "@",
"CbmTofDigitize", payloadCode, "@",
"CbmTofMergeMcPoints", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmTofSim",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmTofSim_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmTofSim_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmTofSim() {
  TriggerDictionaryInitialization_libCbmTofSim_Impl();
}
