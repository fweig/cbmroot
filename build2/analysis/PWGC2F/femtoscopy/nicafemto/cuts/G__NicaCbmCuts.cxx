// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__NicaCbmCuts
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
#include "CbmBasicFemtoPairCut.h"
#include "CbmDeltaPhiDeltaThetaCut.h"
#include "CbmStsExitSepCut.h"
#include "CbmBasicTrackCuts.h"
#include "CbmNHitsCut.h"
#include "CbmTofCut.h"
#include "CbmTofM2PolyCut.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmBasicFemtoPairCut(void *p = nullptr);
   static void *newArray_CbmBasicFemtoPairCut(Long_t size, void *p);
   static void delete_CbmBasicFemtoPairCut(void *p);
   static void deleteArray_CbmBasicFemtoPairCut(void *p);
   static void destruct_CbmBasicFemtoPairCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBasicFemtoPairCut*)
   {
      ::CbmBasicFemtoPairCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBasicFemtoPairCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBasicFemtoPairCut", ::CbmBasicFemtoPairCut::Class_Version(), "CbmBasicFemtoPairCut.h", 23,
                  typeid(::CbmBasicFemtoPairCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBasicFemtoPairCut::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBasicFemtoPairCut) );
      instance.SetNew(&new_CbmBasicFemtoPairCut);
      instance.SetNewArray(&newArray_CbmBasicFemtoPairCut);
      instance.SetDelete(&delete_CbmBasicFemtoPairCut);
      instance.SetDeleteArray(&deleteArray_CbmBasicFemtoPairCut);
      instance.SetDestructor(&destruct_CbmBasicFemtoPairCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBasicFemtoPairCut*)
   {
      return GenerateInitInstanceLocal((::CbmBasicFemtoPairCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBasicFemtoPairCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmDeltaPhiDeltaThetaStarCut(void *p = nullptr);
   static void *newArray_CbmDeltaPhiDeltaThetaStarCut(Long_t size, void *p);
   static void delete_CbmDeltaPhiDeltaThetaStarCut(void *p);
   static void deleteArray_CbmDeltaPhiDeltaThetaStarCut(void *p);
   static void destruct_CbmDeltaPhiDeltaThetaStarCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDeltaPhiDeltaThetaStarCut*)
   {
      ::CbmDeltaPhiDeltaThetaStarCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDeltaPhiDeltaThetaStarCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDeltaPhiDeltaThetaStarCut", ::CbmDeltaPhiDeltaThetaStarCut::Class_Version(), "CbmDeltaPhiDeltaThetaCut.h", 18,
                  typeid(::CbmDeltaPhiDeltaThetaStarCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmDeltaPhiDeltaThetaStarCut::Dictionary, isa_proxy, 4,
                  sizeof(::CbmDeltaPhiDeltaThetaStarCut) );
      instance.SetNew(&new_CbmDeltaPhiDeltaThetaStarCut);
      instance.SetNewArray(&newArray_CbmDeltaPhiDeltaThetaStarCut);
      instance.SetDelete(&delete_CbmDeltaPhiDeltaThetaStarCut);
      instance.SetDeleteArray(&deleteArray_CbmDeltaPhiDeltaThetaStarCut);
      instance.SetDestructor(&destruct_CbmDeltaPhiDeltaThetaStarCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDeltaPhiDeltaThetaStarCut*)
   {
      return GenerateInitInstanceLocal((::CbmDeltaPhiDeltaThetaStarCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDeltaPhiDeltaThetaStarCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmDeltaPhiDeltaThetaCut(void *p = nullptr);
   static void *newArray_CbmDeltaPhiDeltaThetaCut(Long_t size, void *p);
   static void delete_CbmDeltaPhiDeltaThetaCut(void *p);
   static void deleteArray_CbmDeltaPhiDeltaThetaCut(void *p);
   static void destruct_CbmDeltaPhiDeltaThetaCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDeltaPhiDeltaThetaCut*)
   {
      ::CbmDeltaPhiDeltaThetaCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDeltaPhiDeltaThetaCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDeltaPhiDeltaThetaCut", ::CbmDeltaPhiDeltaThetaCut::Class_Version(), "CbmDeltaPhiDeltaThetaCut.h", 33,
                  typeid(::CbmDeltaPhiDeltaThetaCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmDeltaPhiDeltaThetaCut::Dictionary, isa_proxy, 4,
                  sizeof(::CbmDeltaPhiDeltaThetaCut) );
      instance.SetNew(&new_CbmDeltaPhiDeltaThetaCut);
      instance.SetNewArray(&newArray_CbmDeltaPhiDeltaThetaCut);
      instance.SetDelete(&delete_CbmDeltaPhiDeltaThetaCut);
      instance.SetDeleteArray(&deleteArray_CbmDeltaPhiDeltaThetaCut);
      instance.SetDestructor(&destruct_CbmDeltaPhiDeltaThetaCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDeltaPhiDeltaThetaCut*)
   {
      return GenerateInitInstanceLocal((::CbmDeltaPhiDeltaThetaCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDeltaPhiDeltaThetaCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsExitSepCut(void *p = nullptr);
   static void *newArray_CbmStsExitSepCut(Long_t size, void *p);
   static void delete_CbmStsExitSepCut(void *p);
   static void deleteArray_CbmStsExitSepCut(void *p);
   static void destruct_CbmStsExitSepCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsExitSepCut*)
   {
      ::CbmStsExitSepCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsExitSepCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsExitSepCut", ::CbmStsExitSepCut::Class_Version(), "CbmStsExitSepCut.h", 20,
                  typeid(::CbmStsExitSepCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsExitSepCut::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsExitSepCut) );
      instance.SetNew(&new_CbmStsExitSepCut);
      instance.SetNewArray(&newArray_CbmStsExitSepCut);
      instance.SetDelete(&delete_CbmStsExitSepCut);
      instance.SetDeleteArray(&deleteArray_CbmStsExitSepCut);
      instance.SetDestructor(&destruct_CbmStsExitSepCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsExitSepCut*)
   {
      return GenerateInitInstanceLocal((::CbmStsExitSepCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsExitSepCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmBasicTrackCuts(void *p = nullptr);
   static void *newArray_CbmBasicTrackCuts(Long_t size, void *p);
   static void delete_CbmBasicTrackCuts(void *p);
   static void deleteArray_CbmBasicTrackCuts(void *p);
   static void destruct_CbmBasicTrackCuts(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBasicTrackCuts*)
   {
      ::CbmBasicTrackCuts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBasicTrackCuts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBasicTrackCuts", ::CbmBasicTrackCuts::Class_Version(), "CbmBasicTrackCuts.h", 32,
                  typeid(::CbmBasicTrackCuts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBasicTrackCuts::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBasicTrackCuts) );
      instance.SetNew(&new_CbmBasicTrackCuts);
      instance.SetNewArray(&newArray_CbmBasicTrackCuts);
      instance.SetDelete(&delete_CbmBasicTrackCuts);
      instance.SetDeleteArray(&deleteArray_CbmBasicTrackCuts);
      instance.SetDestructor(&destruct_CbmBasicTrackCuts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBasicTrackCuts*)
   {
      return GenerateInitInstanceLocal((::CbmBasicTrackCuts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBasicTrackCuts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmNHitsCut(void *p = nullptr);
   static void *newArray_CbmNHitsCut(Long_t size, void *p);
   static void delete_CbmNHitsCut(void *p);
   static void deleteArray_CbmNHitsCut(void *p);
   static void destruct_CbmNHitsCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmNHitsCut*)
   {
      ::CbmNHitsCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmNHitsCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmNHitsCut", ::CbmNHitsCut::Class_Version(), "CbmNHitsCut.h", 18,
                  typeid(::CbmNHitsCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmNHitsCut::Dictionary, isa_proxy, 4,
                  sizeof(::CbmNHitsCut) );
      instance.SetNew(&new_CbmNHitsCut);
      instance.SetNewArray(&newArray_CbmNHitsCut);
      instance.SetDelete(&delete_CbmNHitsCut);
      instance.SetDeleteArray(&deleteArray_CbmNHitsCut);
      instance.SetDestructor(&destruct_CbmNHitsCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmNHitsCut*)
   {
      return GenerateInitInstanceLocal((::CbmNHitsCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmNHitsCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofCut(void *p = nullptr);
   static void *newArray_CbmTofCut(Long_t size, void *p);
   static void delete_CbmTofCut(void *p);
   static void deleteArray_CbmTofCut(void *p);
   static void destruct_CbmTofCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofCut*)
   {
      ::CbmTofCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofCut", ::CbmTofCut::Class_Version(), "CbmTofCut.h", 18,
                  typeid(::CbmTofCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofCut::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofCut) );
      instance.SetNew(&new_CbmTofCut);
      instance.SetNewArray(&newArray_CbmTofCut);
      instance.SetDelete(&delete_CbmTofCut);
      instance.SetDeleteArray(&deleteArray_CbmTofCut);
      instance.SetDestructor(&destruct_CbmTofCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofCut*)
   {
      return GenerateInitInstanceLocal((::CbmTofCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofM2PolyCut(void *p = nullptr);
   static void *newArray_CbmTofM2PolyCut(Long_t size, void *p);
   static void delete_CbmTofM2PolyCut(void *p);
   static void deleteArray_CbmTofM2PolyCut(void *p);
   static void destruct_CbmTofM2PolyCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofM2PolyCut*)
   {
      ::CbmTofM2PolyCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofM2PolyCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofM2PolyCut", ::CbmTofM2PolyCut::Class_Version(), "CbmTofM2PolyCut.h", 18,
                  typeid(::CbmTofM2PolyCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofM2PolyCut::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofM2PolyCut) );
      instance.SetNew(&new_CbmTofM2PolyCut);
      instance.SetNewArray(&newArray_CbmTofM2PolyCut);
      instance.SetDelete(&delete_CbmTofM2PolyCut);
      instance.SetDeleteArray(&deleteArray_CbmTofM2PolyCut);
      instance.SetDestructor(&destruct_CbmTofM2PolyCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofM2PolyCut*)
   {
      return GenerateInitInstanceLocal((::CbmTofM2PolyCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofM2PolyCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmBasicFemtoPairCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBasicFemtoPairCut::Class_Name()
{
   return "CbmBasicFemtoPairCut";
}

//______________________________________________________________________________
const char *CbmBasicFemtoPairCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBasicFemtoPairCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBasicFemtoPairCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBasicFemtoPairCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBasicFemtoPairCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBasicFemtoPairCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBasicFemtoPairCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBasicFemtoPairCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmDeltaPhiDeltaThetaStarCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmDeltaPhiDeltaThetaStarCut::Class_Name()
{
   return "CbmDeltaPhiDeltaThetaStarCut";
}

//______________________________________________________________________________
const char *CbmDeltaPhiDeltaThetaStarCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDeltaPhiDeltaThetaStarCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmDeltaPhiDeltaThetaStarCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDeltaPhiDeltaThetaStarCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmDeltaPhiDeltaThetaStarCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDeltaPhiDeltaThetaStarCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmDeltaPhiDeltaThetaStarCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDeltaPhiDeltaThetaStarCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmDeltaPhiDeltaThetaCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmDeltaPhiDeltaThetaCut::Class_Name()
{
   return "CbmDeltaPhiDeltaThetaCut";
}

//______________________________________________________________________________
const char *CbmDeltaPhiDeltaThetaCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDeltaPhiDeltaThetaCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmDeltaPhiDeltaThetaCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDeltaPhiDeltaThetaCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmDeltaPhiDeltaThetaCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDeltaPhiDeltaThetaCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmDeltaPhiDeltaThetaCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDeltaPhiDeltaThetaCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsExitSepCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsExitSepCut::Class_Name()
{
   return "CbmStsExitSepCut";
}

//______________________________________________________________________________
const char *CbmStsExitSepCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsExitSepCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsExitSepCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsExitSepCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsExitSepCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsExitSepCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsExitSepCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsExitSepCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmBasicTrackCuts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBasicTrackCuts::Class_Name()
{
   return "CbmBasicTrackCuts";
}

//______________________________________________________________________________
const char *CbmBasicTrackCuts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBasicTrackCuts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBasicTrackCuts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBasicTrackCuts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBasicTrackCuts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBasicTrackCuts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBasicTrackCuts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBasicTrackCuts*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmNHitsCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmNHitsCut::Class_Name()
{
   return "CbmNHitsCut";
}

//______________________________________________________________________________
const char *CbmNHitsCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmNHitsCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmNHitsCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmNHitsCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmNHitsCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmNHitsCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmNHitsCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmNHitsCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofCut::Class_Name()
{
   return "CbmTofCut";
}

//______________________________________________________________________________
const char *CbmTofCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofM2PolyCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofM2PolyCut::Class_Name()
{
   return "CbmTofM2PolyCut";
}

//______________________________________________________________________________
const char *CbmTofM2PolyCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofM2PolyCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofM2PolyCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofM2PolyCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofM2PolyCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofM2PolyCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofM2PolyCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofM2PolyCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmBasicFemtoPairCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBasicFemtoPairCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBasicFemtoPairCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBasicFemtoPairCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBasicFemtoPairCut(void *p) {
      return  p ? new(p) ::CbmBasicFemtoPairCut : new ::CbmBasicFemtoPairCut;
   }
   static void *newArray_CbmBasicFemtoPairCut(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBasicFemtoPairCut[nElements] : new ::CbmBasicFemtoPairCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBasicFemtoPairCut(void *p) {
      delete ((::CbmBasicFemtoPairCut*)p);
   }
   static void deleteArray_CbmBasicFemtoPairCut(void *p) {
      delete [] ((::CbmBasicFemtoPairCut*)p);
   }
   static void destruct_CbmBasicFemtoPairCut(void *p) {
      typedef ::CbmBasicFemtoPairCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBasicFemtoPairCut

//______________________________________________________________________________
void CbmDeltaPhiDeltaThetaStarCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDeltaPhiDeltaThetaStarCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDeltaPhiDeltaThetaStarCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDeltaPhiDeltaThetaStarCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDeltaPhiDeltaThetaStarCut(void *p) {
      return  p ? new(p) ::CbmDeltaPhiDeltaThetaStarCut : new ::CbmDeltaPhiDeltaThetaStarCut;
   }
   static void *newArray_CbmDeltaPhiDeltaThetaStarCut(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDeltaPhiDeltaThetaStarCut[nElements] : new ::CbmDeltaPhiDeltaThetaStarCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDeltaPhiDeltaThetaStarCut(void *p) {
      delete ((::CbmDeltaPhiDeltaThetaStarCut*)p);
   }
   static void deleteArray_CbmDeltaPhiDeltaThetaStarCut(void *p) {
      delete [] ((::CbmDeltaPhiDeltaThetaStarCut*)p);
   }
   static void destruct_CbmDeltaPhiDeltaThetaStarCut(void *p) {
      typedef ::CbmDeltaPhiDeltaThetaStarCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDeltaPhiDeltaThetaStarCut

//______________________________________________________________________________
void CbmDeltaPhiDeltaThetaCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDeltaPhiDeltaThetaCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDeltaPhiDeltaThetaCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDeltaPhiDeltaThetaCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDeltaPhiDeltaThetaCut(void *p) {
      return  p ? new(p) ::CbmDeltaPhiDeltaThetaCut : new ::CbmDeltaPhiDeltaThetaCut;
   }
   static void *newArray_CbmDeltaPhiDeltaThetaCut(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDeltaPhiDeltaThetaCut[nElements] : new ::CbmDeltaPhiDeltaThetaCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDeltaPhiDeltaThetaCut(void *p) {
      delete ((::CbmDeltaPhiDeltaThetaCut*)p);
   }
   static void deleteArray_CbmDeltaPhiDeltaThetaCut(void *p) {
      delete [] ((::CbmDeltaPhiDeltaThetaCut*)p);
   }
   static void destruct_CbmDeltaPhiDeltaThetaCut(void *p) {
      typedef ::CbmDeltaPhiDeltaThetaCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDeltaPhiDeltaThetaCut

//______________________________________________________________________________
void CbmStsExitSepCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsExitSepCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsExitSepCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsExitSepCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsExitSepCut(void *p) {
      return  p ? new(p) ::CbmStsExitSepCut : new ::CbmStsExitSepCut;
   }
   static void *newArray_CbmStsExitSepCut(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsExitSepCut[nElements] : new ::CbmStsExitSepCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsExitSepCut(void *p) {
      delete ((::CbmStsExitSepCut*)p);
   }
   static void deleteArray_CbmStsExitSepCut(void *p) {
      delete [] ((::CbmStsExitSepCut*)p);
   }
   static void destruct_CbmStsExitSepCut(void *p) {
      typedef ::CbmStsExitSepCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsExitSepCut

//______________________________________________________________________________
void CbmBasicTrackCuts::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBasicTrackCuts.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBasicTrackCuts::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBasicTrackCuts::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBasicTrackCuts(void *p) {
      return  p ? new(p) ::CbmBasicTrackCuts : new ::CbmBasicTrackCuts;
   }
   static void *newArray_CbmBasicTrackCuts(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBasicTrackCuts[nElements] : new ::CbmBasicTrackCuts[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBasicTrackCuts(void *p) {
      delete ((::CbmBasicTrackCuts*)p);
   }
   static void deleteArray_CbmBasicTrackCuts(void *p) {
      delete [] ((::CbmBasicTrackCuts*)p);
   }
   static void destruct_CbmBasicTrackCuts(void *p) {
      typedef ::CbmBasicTrackCuts current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBasicTrackCuts

//______________________________________________________________________________
void CbmNHitsCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmNHitsCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmNHitsCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmNHitsCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmNHitsCut(void *p) {
      return  p ? new(p) ::CbmNHitsCut : new ::CbmNHitsCut;
   }
   static void *newArray_CbmNHitsCut(Long_t nElements, void *p) {
      return p ? new(p) ::CbmNHitsCut[nElements] : new ::CbmNHitsCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmNHitsCut(void *p) {
      delete ((::CbmNHitsCut*)p);
   }
   static void deleteArray_CbmNHitsCut(void *p) {
      delete [] ((::CbmNHitsCut*)p);
   }
   static void destruct_CbmNHitsCut(void *p) {
      typedef ::CbmNHitsCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmNHitsCut

//______________________________________________________________________________
void CbmTofCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofCut(void *p) {
      return  p ? new(p) ::CbmTofCut : new ::CbmTofCut;
   }
   static void *newArray_CbmTofCut(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofCut[nElements] : new ::CbmTofCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofCut(void *p) {
      delete ((::CbmTofCut*)p);
   }
   static void deleteArray_CbmTofCut(void *p) {
      delete [] ((::CbmTofCut*)p);
   }
   static void destruct_CbmTofCut(void *p) {
      typedef ::CbmTofCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofCut

//______________________________________________________________________________
void CbmTofM2PolyCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofM2PolyCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofM2PolyCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofM2PolyCut::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofM2PolyCut(void *p) {
      return  p ? new(p) ::CbmTofM2PolyCut : new ::CbmTofM2PolyCut;
   }
   static void *newArray_CbmTofM2PolyCut(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofM2PolyCut[nElements] : new ::CbmTofM2PolyCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofM2PolyCut(void *p) {
      delete ((::CbmTofM2PolyCut*)p);
   }
   static void deleteArray_CbmTofM2PolyCut(void *p) {
      delete [] ((::CbmTofM2PolyCut*)p);
   }
   static void destruct_CbmTofM2PolyCut(void *p) {
      typedef ::CbmTofM2PolyCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofM2PolyCut

namespace {
  void TriggerDictionaryInitialization_libNicaCbmCuts_Impl() {
    static const char* headers[] = {
"CbmBasicFemtoPairCut.h",
"CbmDeltaPhiDeltaThetaCut.h",
"CbmStsExitSepCut.h",
"CbmBasicTrackCuts.h",
"CbmNHitsCut.h",
"CbmTofCut.h",
"CbmTofM2PolyCut.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/cuts",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/cuts/pair",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/cuts/track",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/cuts",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/cuts/pair",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/cuts/track",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/combined",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/unigen",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/mc",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/reco",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/femto",
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
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc",
"/home/heinemann/gpuunpacker/cbmroot/external/xpu/src",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/helpers",
"/home/heinemann/gpuunpacker/cbmroot/build2/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/PWGC2F/femtoscopy/nicafemto/cuts/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libNicaCbmCuts dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmBasicFemtoPairCut.h")))  CbmBasicFemtoPairCut;
class __attribute__((annotate("$clingAutoload$CbmDeltaPhiDeltaThetaCut.h")))  CbmDeltaPhiDeltaThetaStarCut;
class __attribute__((annotate("$clingAutoload$CbmDeltaPhiDeltaThetaCut.h")))  CbmDeltaPhiDeltaThetaCut;
class __attribute__((annotate("$clingAutoload$CbmStsExitSepCut.h")))  CbmStsExitSepCut;
class __attribute__((annotate("$clingAutoload$CbmBasicTrackCuts.h")))  CbmBasicTrackCuts;
class __attribute__((annotate("$clingAutoload$CbmNHitsCut.h")))  CbmNHitsCut;
class __attribute__((annotate("$clingAutoload$CbmTofCut.h")))  CbmTofCut;
class __attribute__((annotate("$clingAutoload$CbmTofM2PolyCut.h")))  CbmTofM2PolyCut;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libNicaCbmCuts dictionary payload"

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
#include "CbmBasicFemtoPairCut.h"
#include "CbmDeltaPhiDeltaThetaCut.h"
#include "CbmStsExitSepCut.h"
#include "CbmBasicTrackCuts.h"
#include "CbmNHitsCut.h"
#include "CbmTofCut.h"
#include "CbmTofM2PolyCut.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmBasicFemtoPairCut", payloadCode, "@",
"CbmBasicTrackCuts", payloadCode, "@",
"CbmDeltaPhiDeltaThetaCut", payloadCode, "@",
"CbmDeltaPhiDeltaThetaStarCut", payloadCode, "@",
"CbmNHitsCut", payloadCode, "@",
"CbmStsExitSepCut", payloadCode, "@",
"CbmTofCut", payloadCode, "@",
"CbmTofM2PolyCut", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libNicaCbmCuts",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libNicaCbmCuts_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libNicaCbmCuts_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libNicaCbmCuts() {
  TriggerDictionaryInitialization_libNicaCbmCuts_Impl();
}
