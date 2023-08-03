// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmConversionAnalysis
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
#include "CbmAnaConversion.h"
#include "CbmAnaConversionKF.h"
#include "CbmAnaConversionTomography.h"
#include "CbmAnaConversionRich.h"
#include "CbmAnaConversionReco.h"
#include "CbmAnaConversionPhotons.h"
#include "CbmAnaConversionPhotons2.h"
#include "CbmAnaConversionRecoFull.h"
#include "CbmAnaConversionTest.h"
#include "CbmAnaConversionTest2.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmAnaConversionKF(void *p = nullptr);
   static void *newArray_CbmAnaConversionKF(Long_t size, void *p);
   static void delete_CbmAnaConversionKF(void *p);
   static void deleteArray_CbmAnaConversionKF(void *p);
   static void destruct_CbmAnaConversionKF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaConversionKF*)
   {
      ::CbmAnaConversionKF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaConversionKF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaConversionKF", ::CbmAnaConversionKF::Class_Version(), "CbmAnaConversionKF.h", 36,
                  typeid(::CbmAnaConversionKF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaConversionKF::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaConversionKF) );
      instance.SetNew(&new_CbmAnaConversionKF);
      instance.SetNewArray(&newArray_CbmAnaConversionKF);
      instance.SetDelete(&delete_CbmAnaConversionKF);
      instance.SetDeleteArray(&deleteArray_CbmAnaConversionKF);
      instance.SetDestructor(&destruct_CbmAnaConversionKF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaConversionKF*)
   {
      return GenerateInitInstanceLocal((::CbmAnaConversionKF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaConversionKF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaConversionPhotons(void *p = nullptr);
   static void *newArray_CbmAnaConversionPhotons(Long_t size, void *p);
   static void delete_CbmAnaConversionPhotons(void *p);
   static void deleteArray_CbmAnaConversionPhotons(void *p);
   static void destruct_CbmAnaConversionPhotons(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaConversionPhotons*)
   {
      ::CbmAnaConversionPhotons *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaConversionPhotons >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaConversionPhotons", ::CbmAnaConversionPhotons::Class_Version(), "CbmAnaConversionPhotons.h", 45,
                  typeid(::CbmAnaConversionPhotons), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaConversionPhotons::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaConversionPhotons) );
      instance.SetNew(&new_CbmAnaConversionPhotons);
      instance.SetNewArray(&newArray_CbmAnaConversionPhotons);
      instance.SetDelete(&delete_CbmAnaConversionPhotons);
      instance.SetDeleteArray(&deleteArray_CbmAnaConversionPhotons);
      instance.SetDestructor(&destruct_CbmAnaConversionPhotons);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaConversionPhotons*)
   {
      return GenerateInitInstanceLocal((::CbmAnaConversionPhotons*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaConversionPhotons*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaConversionPhotons2(void *p = nullptr);
   static void *newArray_CbmAnaConversionPhotons2(Long_t size, void *p);
   static void delete_CbmAnaConversionPhotons2(void *p);
   static void deleteArray_CbmAnaConversionPhotons2(void *p);
   static void destruct_CbmAnaConversionPhotons2(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaConversionPhotons2*)
   {
      ::CbmAnaConversionPhotons2 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaConversionPhotons2 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaConversionPhotons2", ::CbmAnaConversionPhotons2::Class_Version(), "CbmAnaConversionPhotons2.h", 31,
                  typeid(::CbmAnaConversionPhotons2), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaConversionPhotons2::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaConversionPhotons2) );
      instance.SetNew(&new_CbmAnaConversionPhotons2);
      instance.SetNewArray(&newArray_CbmAnaConversionPhotons2);
      instance.SetDelete(&delete_CbmAnaConversionPhotons2);
      instance.SetDeleteArray(&deleteArray_CbmAnaConversionPhotons2);
      instance.SetDestructor(&destruct_CbmAnaConversionPhotons2);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaConversionPhotons2*)
   {
      return GenerateInitInstanceLocal((::CbmAnaConversionPhotons2*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaConversionPhotons2*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaConversionReco(void *p = nullptr);
   static void *newArray_CbmAnaConversionReco(Long_t size, void *p);
   static void delete_CbmAnaConversionReco(void *p);
   static void deleteArray_CbmAnaConversionReco(void *p);
   static void destruct_CbmAnaConversionReco(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaConversionReco*)
   {
      ::CbmAnaConversionReco *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaConversionReco >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaConversionReco", ::CbmAnaConversionReco::Class_Version(), "CbmAnaConversionReco.h", 33,
                  typeid(::CbmAnaConversionReco), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaConversionReco::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaConversionReco) );
      instance.SetNew(&new_CbmAnaConversionReco);
      instance.SetNewArray(&newArray_CbmAnaConversionReco);
      instance.SetDelete(&delete_CbmAnaConversionReco);
      instance.SetDeleteArray(&deleteArray_CbmAnaConversionReco);
      instance.SetDestructor(&destruct_CbmAnaConversionReco);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaConversionReco*)
   {
      return GenerateInitInstanceLocal((::CbmAnaConversionReco*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaConversionReco*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaConversionRecoFull(void *p = nullptr);
   static void *newArray_CbmAnaConversionRecoFull(Long_t size, void *p);
   static void delete_CbmAnaConversionRecoFull(void *p);
   static void deleteArray_CbmAnaConversionRecoFull(void *p);
   static void destruct_CbmAnaConversionRecoFull(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaConversionRecoFull*)
   {
      ::CbmAnaConversionRecoFull *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaConversionRecoFull >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaConversionRecoFull", ::CbmAnaConversionRecoFull::Class_Version(), "CbmAnaConversionRecoFull.h", 34,
                  typeid(::CbmAnaConversionRecoFull), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaConversionRecoFull::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaConversionRecoFull) );
      instance.SetNew(&new_CbmAnaConversionRecoFull);
      instance.SetNewArray(&newArray_CbmAnaConversionRecoFull);
      instance.SetDelete(&delete_CbmAnaConversionRecoFull);
      instance.SetDeleteArray(&deleteArray_CbmAnaConversionRecoFull);
      instance.SetDestructor(&destruct_CbmAnaConversionRecoFull);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaConversionRecoFull*)
   {
      return GenerateInitInstanceLocal((::CbmAnaConversionRecoFull*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaConversionRecoFull*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaConversionRich(void *p = nullptr);
   static void *newArray_CbmAnaConversionRich(Long_t size, void *p);
   static void delete_CbmAnaConversionRich(void *p);
   static void deleteArray_CbmAnaConversionRich(void *p);
   static void destruct_CbmAnaConversionRich(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaConversionRich*)
   {
      ::CbmAnaConversionRich *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaConversionRich >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaConversionRich", ::CbmAnaConversionRich::Class_Version(), "CbmAnaConversionRich.h", 29,
                  typeid(::CbmAnaConversionRich), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaConversionRich::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaConversionRich) );
      instance.SetNew(&new_CbmAnaConversionRich);
      instance.SetNewArray(&newArray_CbmAnaConversionRich);
      instance.SetDelete(&delete_CbmAnaConversionRich);
      instance.SetDeleteArray(&deleteArray_CbmAnaConversionRich);
      instance.SetDestructor(&destruct_CbmAnaConversionRich);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaConversionRich*)
   {
      return GenerateInitInstanceLocal((::CbmAnaConversionRich*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaConversionRich*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaConversionTest(void *p = nullptr);
   static void *newArray_CbmAnaConversionTest(Long_t size, void *p);
   static void delete_CbmAnaConversionTest(void *p);
   static void deleteArray_CbmAnaConversionTest(void *p);
   static void destruct_CbmAnaConversionTest(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaConversionTest*)
   {
      ::CbmAnaConversionTest *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaConversionTest >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaConversionTest", ::CbmAnaConversionTest::Class_Version(), "CbmAnaConversionTest.h", 34,
                  typeid(::CbmAnaConversionTest), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaConversionTest::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaConversionTest) );
      instance.SetNew(&new_CbmAnaConversionTest);
      instance.SetNewArray(&newArray_CbmAnaConversionTest);
      instance.SetDelete(&delete_CbmAnaConversionTest);
      instance.SetDeleteArray(&deleteArray_CbmAnaConversionTest);
      instance.SetDestructor(&destruct_CbmAnaConversionTest);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaConversionTest*)
   {
      return GenerateInitInstanceLocal((::CbmAnaConversionTest*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaConversionTest*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaConversionTest2(void *p = nullptr);
   static void *newArray_CbmAnaConversionTest2(Long_t size, void *p);
   static void delete_CbmAnaConversionTest2(void *p);
   static void deleteArray_CbmAnaConversionTest2(void *p);
   static void destruct_CbmAnaConversionTest2(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaConversionTest2*)
   {
      ::CbmAnaConversionTest2 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaConversionTest2 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaConversionTest2", ::CbmAnaConversionTest2::Class_Version(), "CbmAnaConversionTest2.h", 34,
                  typeid(::CbmAnaConversionTest2), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaConversionTest2::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaConversionTest2) );
      instance.SetNew(&new_CbmAnaConversionTest2);
      instance.SetNewArray(&newArray_CbmAnaConversionTest2);
      instance.SetDelete(&delete_CbmAnaConversionTest2);
      instance.SetDeleteArray(&deleteArray_CbmAnaConversionTest2);
      instance.SetDestructor(&destruct_CbmAnaConversionTest2);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaConversionTest2*)
   {
      return GenerateInitInstanceLocal((::CbmAnaConversionTest2*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaConversionTest2*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaConversionTomography(void *p = nullptr);
   static void *newArray_CbmAnaConversionTomography(Long_t size, void *p);
   static void delete_CbmAnaConversionTomography(void *p);
   static void deleteArray_CbmAnaConversionTomography(void *p);
   static void destruct_CbmAnaConversionTomography(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaConversionTomography*)
   {
      ::CbmAnaConversionTomography *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaConversionTomography >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaConversionTomography", ::CbmAnaConversionTomography::Class_Version(), "CbmAnaConversionTomography.h", 32,
                  typeid(::CbmAnaConversionTomography), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaConversionTomography::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaConversionTomography) );
      instance.SetNew(&new_CbmAnaConversionTomography);
      instance.SetNewArray(&newArray_CbmAnaConversionTomography);
      instance.SetDelete(&delete_CbmAnaConversionTomography);
      instance.SetDeleteArray(&deleteArray_CbmAnaConversionTomography);
      instance.SetDestructor(&destruct_CbmAnaConversionTomography);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaConversionTomography*)
   {
      return GenerateInitInstanceLocal((::CbmAnaConversionTomography*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaConversionTomography*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaConversion(void *p = nullptr);
   static void *newArray_CbmAnaConversion(Long_t size, void *p);
   static void delete_CbmAnaConversion(void *p);
   static void deleteArray_CbmAnaConversion(void *p);
   static void destruct_CbmAnaConversion(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaConversion*)
   {
      ::CbmAnaConversion *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaConversion >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaConversion", ::CbmAnaConversion::Class_Version(), "CbmAnaConversion.h", 63,
                  typeid(::CbmAnaConversion), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaConversion::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaConversion) );
      instance.SetNew(&new_CbmAnaConversion);
      instance.SetNewArray(&newArray_CbmAnaConversion);
      instance.SetDelete(&delete_CbmAnaConversion);
      instance.SetDeleteArray(&deleteArray_CbmAnaConversion);
      instance.SetDestructor(&destruct_CbmAnaConversion);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaConversion*)
   {
      return GenerateInitInstanceLocal((::CbmAnaConversion*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaConversion*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaConversionKF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaConversionKF::Class_Name()
{
   return "CbmAnaConversionKF";
}

//______________________________________________________________________________
const char *CbmAnaConversionKF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionKF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaConversionKF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionKF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaConversionKF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionKF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaConversionKF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionKF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaConversionPhotons::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaConversionPhotons::Class_Name()
{
   return "CbmAnaConversionPhotons";
}

//______________________________________________________________________________
const char *CbmAnaConversionPhotons::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionPhotons*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaConversionPhotons::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionPhotons*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaConversionPhotons::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionPhotons*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaConversionPhotons::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionPhotons*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaConversionPhotons2::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaConversionPhotons2::Class_Name()
{
   return "CbmAnaConversionPhotons2";
}

//______________________________________________________________________________
const char *CbmAnaConversionPhotons2::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionPhotons2*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaConversionPhotons2::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionPhotons2*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaConversionPhotons2::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionPhotons2*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaConversionPhotons2::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionPhotons2*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaConversionReco::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaConversionReco::Class_Name()
{
   return "CbmAnaConversionReco";
}

//______________________________________________________________________________
const char *CbmAnaConversionReco::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionReco*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaConversionReco::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionReco*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaConversionReco::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionReco*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaConversionReco::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionReco*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaConversionRecoFull::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaConversionRecoFull::Class_Name()
{
   return "CbmAnaConversionRecoFull";
}

//______________________________________________________________________________
const char *CbmAnaConversionRecoFull::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionRecoFull*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaConversionRecoFull::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionRecoFull*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaConversionRecoFull::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionRecoFull*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaConversionRecoFull::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionRecoFull*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaConversionRich::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaConversionRich::Class_Name()
{
   return "CbmAnaConversionRich";
}

//______________________________________________________________________________
const char *CbmAnaConversionRich::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionRich*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaConversionRich::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionRich*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaConversionRich::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionRich*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaConversionRich::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionRich*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaConversionTest::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaConversionTest::Class_Name()
{
   return "CbmAnaConversionTest";
}

//______________________________________________________________________________
const char *CbmAnaConversionTest::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionTest*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaConversionTest::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionTest*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaConversionTest::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionTest*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaConversionTest::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionTest*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaConversionTest2::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaConversionTest2::Class_Name()
{
   return "CbmAnaConversionTest2";
}

//______________________________________________________________________________
const char *CbmAnaConversionTest2::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionTest2*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaConversionTest2::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionTest2*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaConversionTest2::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionTest2*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaConversionTest2::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionTest2*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaConversionTomography::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaConversionTomography::Class_Name()
{
   return "CbmAnaConversionTomography";
}

//______________________________________________________________________________
const char *CbmAnaConversionTomography::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionTomography*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaConversionTomography::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionTomography*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaConversionTomography::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionTomography*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaConversionTomography::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversionTomography*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaConversion::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaConversion::Class_Name()
{
   return "CbmAnaConversion";
}

//______________________________________________________________________________
const char *CbmAnaConversion::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversion*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaConversion::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversion*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaConversion::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversion*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaConversion::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaConversion*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmAnaConversionKF::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaConversionKF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaConversionKF::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaConversionKF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaConversionKF(void *p) {
      return  p ? new(p) ::CbmAnaConversionKF : new ::CbmAnaConversionKF;
   }
   static void *newArray_CbmAnaConversionKF(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaConversionKF[nElements] : new ::CbmAnaConversionKF[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaConversionKF(void *p) {
      delete ((::CbmAnaConversionKF*)p);
   }
   static void deleteArray_CbmAnaConversionKF(void *p) {
      delete [] ((::CbmAnaConversionKF*)p);
   }
   static void destruct_CbmAnaConversionKF(void *p) {
      typedef ::CbmAnaConversionKF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaConversionKF

//______________________________________________________________________________
void CbmAnaConversionPhotons::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaConversionPhotons.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaConversionPhotons::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaConversionPhotons::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaConversionPhotons(void *p) {
      return  p ? new(p) ::CbmAnaConversionPhotons : new ::CbmAnaConversionPhotons;
   }
   static void *newArray_CbmAnaConversionPhotons(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaConversionPhotons[nElements] : new ::CbmAnaConversionPhotons[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaConversionPhotons(void *p) {
      delete ((::CbmAnaConversionPhotons*)p);
   }
   static void deleteArray_CbmAnaConversionPhotons(void *p) {
      delete [] ((::CbmAnaConversionPhotons*)p);
   }
   static void destruct_CbmAnaConversionPhotons(void *p) {
      typedef ::CbmAnaConversionPhotons current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaConversionPhotons

//______________________________________________________________________________
void CbmAnaConversionPhotons2::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaConversionPhotons2.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaConversionPhotons2::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaConversionPhotons2::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaConversionPhotons2(void *p) {
      return  p ? new(p) ::CbmAnaConversionPhotons2 : new ::CbmAnaConversionPhotons2;
   }
   static void *newArray_CbmAnaConversionPhotons2(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaConversionPhotons2[nElements] : new ::CbmAnaConversionPhotons2[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaConversionPhotons2(void *p) {
      delete ((::CbmAnaConversionPhotons2*)p);
   }
   static void deleteArray_CbmAnaConversionPhotons2(void *p) {
      delete [] ((::CbmAnaConversionPhotons2*)p);
   }
   static void destruct_CbmAnaConversionPhotons2(void *p) {
      typedef ::CbmAnaConversionPhotons2 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaConversionPhotons2

//______________________________________________________________________________
void CbmAnaConversionReco::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaConversionReco.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaConversionReco::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaConversionReco::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaConversionReco(void *p) {
      return  p ? new(p) ::CbmAnaConversionReco : new ::CbmAnaConversionReco;
   }
   static void *newArray_CbmAnaConversionReco(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaConversionReco[nElements] : new ::CbmAnaConversionReco[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaConversionReco(void *p) {
      delete ((::CbmAnaConversionReco*)p);
   }
   static void deleteArray_CbmAnaConversionReco(void *p) {
      delete [] ((::CbmAnaConversionReco*)p);
   }
   static void destruct_CbmAnaConversionReco(void *p) {
      typedef ::CbmAnaConversionReco current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaConversionReco

//______________________________________________________________________________
void CbmAnaConversionRecoFull::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaConversionRecoFull.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaConversionRecoFull::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaConversionRecoFull::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaConversionRecoFull(void *p) {
      return  p ? new(p) ::CbmAnaConversionRecoFull : new ::CbmAnaConversionRecoFull;
   }
   static void *newArray_CbmAnaConversionRecoFull(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaConversionRecoFull[nElements] : new ::CbmAnaConversionRecoFull[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaConversionRecoFull(void *p) {
      delete ((::CbmAnaConversionRecoFull*)p);
   }
   static void deleteArray_CbmAnaConversionRecoFull(void *p) {
      delete [] ((::CbmAnaConversionRecoFull*)p);
   }
   static void destruct_CbmAnaConversionRecoFull(void *p) {
      typedef ::CbmAnaConversionRecoFull current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaConversionRecoFull

//______________________________________________________________________________
void CbmAnaConversionRich::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaConversionRich.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaConversionRich::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaConversionRich::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaConversionRich(void *p) {
      return  p ? new(p) ::CbmAnaConversionRich : new ::CbmAnaConversionRich;
   }
   static void *newArray_CbmAnaConversionRich(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaConversionRich[nElements] : new ::CbmAnaConversionRich[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaConversionRich(void *p) {
      delete ((::CbmAnaConversionRich*)p);
   }
   static void deleteArray_CbmAnaConversionRich(void *p) {
      delete [] ((::CbmAnaConversionRich*)p);
   }
   static void destruct_CbmAnaConversionRich(void *p) {
      typedef ::CbmAnaConversionRich current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaConversionRich

//______________________________________________________________________________
void CbmAnaConversionTest::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaConversionTest.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaConversionTest::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaConversionTest::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaConversionTest(void *p) {
      return  p ? new(p) ::CbmAnaConversionTest : new ::CbmAnaConversionTest;
   }
   static void *newArray_CbmAnaConversionTest(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaConversionTest[nElements] : new ::CbmAnaConversionTest[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaConversionTest(void *p) {
      delete ((::CbmAnaConversionTest*)p);
   }
   static void deleteArray_CbmAnaConversionTest(void *p) {
      delete [] ((::CbmAnaConversionTest*)p);
   }
   static void destruct_CbmAnaConversionTest(void *p) {
      typedef ::CbmAnaConversionTest current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaConversionTest

//______________________________________________________________________________
void CbmAnaConversionTest2::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaConversionTest2.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaConversionTest2::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaConversionTest2::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaConversionTest2(void *p) {
      return  p ? new(p) ::CbmAnaConversionTest2 : new ::CbmAnaConversionTest2;
   }
   static void *newArray_CbmAnaConversionTest2(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaConversionTest2[nElements] : new ::CbmAnaConversionTest2[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaConversionTest2(void *p) {
      delete ((::CbmAnaConversionTest2*)p);
   }
   static void deleteArray_CbmAnaConversionTest2(void *p) {
      delete [] ((::CbmAnaConversionTest2*)p);
   }
   static void destruct_CbmAnaConversionTest2(void *p) {
      typedef ::CbmAnaConversionTest2 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaConversionTest2

//______________________________________________________________________________
void CbmAnaConversionTomography::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaConversionTomography.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaConversionTomography::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaConversionTomography::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaConversionTomography(void *p) {
      return  p ? new(p) ::CbmAnaConversionTomography : new ::CbmAnaConversionTomography;
   }
   static void *newArray_CbmAnaConversionTomography(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaConversionTomography[nElements] : new ::CbmAnaConversionTomography[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaConversionTomography(void *p) {
      delete ((::CbmAnaConversionTomography*)p);
   }
   static void deleteArray_CbmAnaConversionTomography(void *p) {
      delete [] ((::CbmAnaConversionTomography*)p);
   }
   static void destruct_CbmAnaConversionTomography(void *p) {
      typedef ::CbmAnaConversionTomography current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaConversionTomography

//______________________________________________________________________________
void CbmAnaConversion::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaConversion.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaConversion::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaConversion::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaConversion(void *p) {
      return  p ? new(p) ::CbmAnaConversion : new ::CbmAnaConversion;
   }
   static void *newArray_CbmAnaConversion(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaConversion[nElements] : new ::CbmAnaConversion[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaConversion(void *p) {
      delete ((::CbmAnaConversion*)p);
   }
   static void deleteArray_CbmAnaConversion(void *p) {
      delete [] ((::CbmAnaConversion*)p);
   }
   static void destruct_CbmAnaConversion(void *p) {
      typedef ::CbmAnaConversion current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaConversion

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
   static TClass *vectorlEvectorlEboolgRsPgR_Dictionary();
   static void vectorlEvectorlEboolgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEboolgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEboolgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEboolgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEboolgRsPgR(void *p);
   static void destruct_vectorlEvectorlEboolgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<bool> >*)
   {
      vector<vector<bool> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<bool> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<bool> >", -2, "vector", 389,
                  typeid(vector<vector<bool> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEboolgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<bool> >) );
      instance.SetNew(&new_vectorlEvectorlEboolgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEboolgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEboolgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEboolgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEboolgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<bool> > >()));

      ::ROOT::AddClassAlternate("vector<vector<bool> >","std::vector<std::vector<bool, std::allocator<bool> >, std::allocator<std::vector<bool, std::allocator<bool> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<bool> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEboolgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<bool> >*)nullptr)->GetClass();
      vectorlEvectorlEboolgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEboolgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEboolgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<bool> > : new vector<vector<bool> >;
   }
   static void *newArray_vectorlEvectorlEboolgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<bool> >[nElements] : new vector<vector<bool> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEboolgRsPgR(void *p) {
      delete ((vector<vector<bool> >*)p);
   }
   static void deleteArray_vectorlEvectorlEboolgRsPgR(void *p) {
      delete [] ((vector<vector<bool> >*)p);
   }
   static void destruct_vectorlEvectorlEboolgRsPgR(void *p) {
      typedef vector<vector<bool> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<bool> >

namespace ROOT {
   static TClass *vectorlEvectorlETVector3gRsPgR_Dictionary();
   static void vectorlEvectorlETVector3gRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlETVector3gRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlETVector3gRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlETVector3gRsPgR(void *p);
   static void deleteArray_vectorlEvectorlETVector3gRsPgR(void *p);
   static void destruct_vectorlEvectorlETVector3gRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<TVector3> >*)
   {
      vector<vector<TVector3> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<TVector3> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<TVector3> >", -2, "vector", 389,
                  typeid(vector<vector<TVector3> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlETVector3gRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<TVector3> >) );
      instance.SetNew(&new_vectorlEvectorlETVector3gRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlETVector3gRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlETVector3gRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlETVector3gRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlETVector3gRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<TVector3> > >()));

      ::ROOT::AddClassAlternate("vector<vector<TVector3> >","std::vector<std::vector<TVector3, std::allocator<TVector3> >, std::allocator<std::vector<TVector3, std::allocator<TVector3> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<TVector3> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlETVector3gRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<TVector3> >*)nullptr)->GetClass();
      vectorlEvectorlETVector3gRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlETVector3gRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlETVector3gRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TVector3> > : new vector<vector<TVector3> >;
   }
   static void *newArray_vectorlEvectorlETVector3gRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<TVector3> >[nElements] : new vector<vector<TVector3> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlETVector3gRsPgR(void *p) {
      delete ((vector<vector<TVector3> >*)p);
   }
   static void deleteArray_vectorlEvectorlETVector3gRsPgR(void *p) {
      delete [] ((vector<vector<TVector3> >*)p);
   }
   static void destruct_vectorlEvectorlETVector3gRsPgR(void *p) {
      typedef vector<vector<TVector3> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<TVector3> >

namespace ROOT {
   static TClass *vectorlEvectorlECbmMCTrackmUgRsPgR_Dictionary();
   static void vectorlEvectorlECbmMCTrackmUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlECbmMCTrackmUgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p);
   static void destruct_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<CbmMCTrack*> >*)
   {
      vector<vector<CbmMCTrack*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<CbmMCTrack*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<CbmMCTrack*> >", -2, "vector", 389,
                  typeid(vector<vector<CbmMCTrack*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlECbmMCTrackmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<CbmMCTrack*> >) );
      instance.SetNew(&new_vectorlEvectorlECbmMCTrackmUgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlECbmMCTrackmUgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlECbmMCTrackmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlECbmMCTrackmUgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlECbmMCTrackmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<CbmMCTrack*> > >()));

      ::ROOT::AddClassAlternate("vector<vector<CbmMCTrack*> >","std::vector<std::vector<CbmMCTrack*, std::allocator<CbmMCTrack*> >, std::allocator<std::vector<CbmMCTrack*, std::allocator<CbmMCTrack*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<CbmMCTrack*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlECbmMCTrackmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<CbmMCTrack*> >*)nullptr)->GetClass();
      vectorlEvectorlECbmMCTrackmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlECbmMCTrackmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmMCTrack*> > : new vector<vector<CbmMCTrack*> >;
   }
   static void *newArray_vectorlEvectorlECbmMCTrackmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<CbmMCTrack*> >[nElements] : new vector<vector<CbmMCTrack*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p) {
      delete ((vector<vector<CbmMCTrack*> >*)p);
   }
   static void deleteArray_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p) {
      delete [] ((vector<vector<CbmMCTrack*> >*)p);
   }
   static void destruct_vectorlEvectorlECbmMCTrackmUgRsPgR(void *p) {
      typedef vector<vector<CbmMCTrack*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<CbmMCTrack*> >

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
   static TClass *vectorlETVector3gR_Dictionary();
   static void vectorlETVector3gR_TClassManip(TClass*);
   static void *new_vectorlETVector3gR(void *p = nullptr);
   static void *newArray_vectorlETVector3gR(Long_t size, void *p);
   static void delete_vectorlETVector3gR(void *p);
   static void deleteArray_vectorlETVector3gR(void *p);
   static void destruct_vectorlETVector3gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TVector3>*)
   {
      vector<TVector3> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TVector3>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TVector3>", -2, "vector", 389,
                  typeid(vector<TVector3>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETVector3gR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TVector3>) );
      instance.SetNew(&new_vectorlETVector3gR);
      instance.SetNewArray(&newArray_vectorlETVector3gR);
      instance.SetDelete(&delete_vectorlETVector3gR);
      instance.SetDeleteArray(&deleteArray_vectorlETVector3gR);
      instance.SetDestructor(&destruct_vectorlETVector3gR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TVector3> >()));

      ::ROOT::AddClassAlternate("vector<TVector3>","std::vector<TVector3, std::allocator<TVector3> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TVector3>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETVector3gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TVector3>*)nullptr)->GetClass();
      vectorlETVector3gR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETVector3gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETVector3gR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TVector3> : new vector<TVector3>;
   }
   static void *newArray_vectorlETVector3gR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TVector3>[nElements] : new vector<TVector3>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETVector3gR(void *p) {
      delete ((vector<TVector3>*)p);
   }
   static void deleteArray_vectorlETVector3gR(void *p) {
      delete [] ((vector<TVector3>*)p);
   }
   static void destruct_vectorlETVector3gR(void *p) {
      typedef vector<TVector3> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TVector3>

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
   static TClass *vectorlEKFParticlegR_Dictionary();
   static void vectorlEKFParticlegR_TClassManip(TClass*);
   static void *new_vectorlEKFParticlegR(void *p = nullptr);
   static void *newArray_vectorlEKFParticlegR(Long_t size, void *p);
   static void delete_vectorlEKFParticlegR(void *p);
   static void deleteArray_vectorlEKFParticlegR(void *p);
   static void destruct_vectorlEKFParticlegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<KFParticle>*)
   {
      vector<KFParticle> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<KFParticle>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<KFParticle>", -2, "vector", 389,
                  typeid(vector<KFParticle>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEKFParticlegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<KFParticle>) );
      instance.SetNew(&new_vectorlEKFParticlegR);
      instance.SetNewArray(&newArray_vectorlEKFParticlegR);
      instance.SetDelete(&delete_vectorlEKFParticlegR);
      instance.SetDeleteArray(&deleteArray_vectorlEKFParticlegR);
      instance.SetDestructor(&destruct_vectorlEKFParticlegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<KFParticle> >()));

      ::ROOT::AddClassAlternate("vector<KFParticle>","std::vector<KFParticle, std::allocator<KFParticle> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<KFParticle>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEKFParticlegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<KFParticle>*)nullptr)->GetClass();
      vectorlEKFParticlegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEKFParticlegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEKFParticlegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<KFParticle> : new vector<KFParticle>;
   }
   static void *newArray_vectorlEKFParticlegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<KFParticle>[nElements] : new vector<KFParticle>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEKFParticlegR(void *p) {
      delete ((vector<KFParticle>*)p);
   }
   static void deleteArray_vectorlEKFParticlegR(void *p) {
      delete [] ((vector<KFParticle>*)p);
   }
   static void destruct_vectorlEKFParticlegR(void *p) {
      typedef vector<KFParticle> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<KFParticle>

namespace ROOT {
   static TClass *vectorlEKFPartMatchgR_Dictionary();
   static void vectorlEKFPartMatchgR_TClassManip(TClass*);
   static void *new_vectorlEKFPartMatchgR(void *p = nullptr);
   static void *newArray_vectorlEKFPartMatchgR(Long_t size, void *p);
   static void delete_vectorlEKFPartMatchgR(void *p);
   static void deleteArray_vectorlEKFPartMatchgR(void *p);
   static void destruct_vectorlEKFPartMatchgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<KFPartMatch>*)
   {
      vector<KFPartMatch> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<KFPartMatch>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<KFPartMatch>", -2, "vector", 389,
                  typeid(vector<KFPartMatch>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEKFPartMatchgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<KFPartMatch>) );
      instance.SetNew(&new_vectorlEKFPartMatchgR);
      instance.SetNewArray(&newArray_vectorlEKFPartMatchgR);
      instance.SetDelete(&delete_vectorlEKFPartMatchgR);
      instance.SetDeleteArray(&deleteArray_vectorlEKFPartMatchgR);
      instance.SetDestructor(&destruct_vectorlEKFPartMatchgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<KFPartMatch> >()));

      ::ROOT::AddClassAlternate("vector<KFPartMatch>","std::vector<KFPartMatch, std::allocator<KFPartMatch> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<KFPartMatch>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEKFPartMatchgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<KFPartMatch>*)nullptr)->GetClass();
      vectorlEKFPartMatchgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEKFPartMatchgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEKFPartMatchgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<KFPartMatch> : new vector<KFPartMatch>;
   }
   static void *newArray_vectorlEKFPartMatchgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<KFPartMatch>[nElements] : new vector<KFPartMatch>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEKFPartMatchgR(void *p) {
      delete ((vector<KFPartMatch>*)p);
   }
   static void deleteArray_vectorlEKFPartMatchgR(void *p) {
      delete [] ((vector<KFPartMatch>*)p);
   }
   static void destruct_vectorlEKFPartMatchgR(void *p) {
      typedef vector<KFPartMatch> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<KFPartMatch>

namespace ROOT {
   static TClass *vectorlECbmMCTrackmUgR_Dictionary();
   static void vectorlECbmMCTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmMCTrackmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmMCTrackmUgR(Long_t size, void *p);
   static void delete_vectorlECbmMCTrackmUgR(void *p);
   static void deleteArray_vectorlECbmMCTrackmUgR(void *p);
   static void destruct_vectorlECbmMCTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMCTrack*>*)
   {
      vector<CbmMCTrack*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMCTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMCTrack*>", -2, "vector", 389,
                  typeid(vector<CbmMCTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMCTrackmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmMCTrack*>) );
      instance.SetNew(&new_vectorlECbmMCTrackmUgR);
      instance.SetNewArray(&newArray_vectorlECbmMCTrackmUgR);
      instance.SetDelete(&delete_vectorlECbmMCTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMCTrackmUgR);
      instance.SetDestructor(&destruct_vectorlECbmMCTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMCTrack*> >()));

      ::ROOT::AddClassAlternate("vector<CbmMCTrack*>","std::vector<CbmMCTrack*, std::allocator<CbmMCTrack*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMCTrack*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMCTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMCTrack*>*)nullptr)->GetClass();
      vectorlECbmMCTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMCTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMCTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMCTrack*> : new vector<CbmMCTrack*>;
   }
   static void *newArray_vectorlECbmMCTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMCTrack*>[nElements] : new vector<CbmMCTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMCTrackmUgR(void *p) {
      delete ((vector<CbmMCTrack*>*)p);
   }
   static void deleteArray_vectorlECbmMCTrackmUgR(void *p) {
      delete [] ((vector<CbmMCTrack*>*)p);
   }
   static void destruct_vectorlECbmMCTrackmUgR(void *p) {
      typedef vector<CbmMCTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMCTrack*>

namespace ROOT {
   static TClass *vectorlECbmGlobalTrackmUgR_Dictionary();
   static void vectorlECbmGlobalTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmGlobalTrackmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmGlobalTrackmUgR(Long_t size, void *p);
   static void delete_vectorlECbmGlobalTrackmUgR(void *p);
   static void deleteArray_vectorlECbmGlobalTrackmUgR(void *p);
   static void destruct_vectorlECbmGlobalTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmGlobalTrack*>*)
   {
      vector<CbmGlobalTrack*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmGlobalTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmGlobalTrack*>", -2, "vector", 389,
                  typeid(vector<CbmGlobalTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmGlobalTrackmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmGlobalTrack*>) );
      instance.SetNew(&new_vectorlECbmGlobalTrackmUgR);
      instance.SetNewArray(&newArray_vectorlECbmGlobalTrackmUgR);
      instance.SetDelete(&delete_vectorlECbmGlobalTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmGlobalTrackmUgR);
      instance.SetDestructor(&destruct_vectorlECbmGlobalTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmGlobalTrack*> >()));

      ::ROOT::AddClassAlternate("vector<CbmGlobalTrack*>","std::vector<CbmGlobalTrack*, std::allocator<CbmGlobalTrack*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmGlobalTrack*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmGlobalTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmGlobalTrack*>*)nullptr)->GetClass();
      vectorlECbmGlobalTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmGlobalTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmGlobalTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmGlobalTrack*> : new vector<CbmGlobalTrack*>;
   }
   static void *newArray_vectorlECbmGlobalTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmGlobalTrack*>[nElements] : new vector<CbmGlobalTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmGlobalTrackmUgR(void *p) {
      delete ((vector<CbmGlobalTrack*>*)p);
   }
   static void deleteArray_vectorlECbmGlobalTrackmUgR(void *p) {
      delete [] ((vector<CbmGlobalTrack*>*)p);
   }
   static void destruct_vectorlECbmGlobalTrackmUgR(void *p) {
      typedef vector<CbmGlobalTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmGlobalTrack*>

namespace {
  void TriggerDictionaryInitialization_libCbmConversionAnalysis_Impl() {
    static const char* headers[] = {
"CbmAnaConversion.h",
"CbmAnaConversionKF.h",
"CbmAnaConversionTomography.h",
"CbmAnaConversionRich.h",
"CbmAnaConversionReco.h",
"CbmAnaConversionPhotons.h",
"CbmAnaConversionPhotons2.h",
"CbmAnaConversionRecoFull.h",
"CbmAnaConversionTest.h",
"CbmAnaConversionTest2.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/conversion",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/conversion",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich/utils",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/lmvm",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/KFQA",
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
"/home/heinemann/gpuunpacker/cbmroot/algo/ca/simd",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/finder",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/fitter",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/tracks",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/selection",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/qa",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/alignment",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/mcbm",
"/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/unpack",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca",
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
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/PWGDIL/dielectron/conversion/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmConversionAnalysis dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmAnaConversionKF.h")))  __attribute__((annotate("$clingAutoload$CbmAnaConversion.h")))  CbmAnaConversionKF;
class __attribute__((annotate("$clingAutoload$CbmAnaConversionPhotons.h")))  __attribute__((annotate("$clingAutoload$CbmAnaConversion.h")))  CbmAnaConversionPhotons;
class __attribute__((annotate("$clingAutoload$CbmAnaConversionPhotons2.h")))  __attribute__((annotate("$clingAutoload$CbmAnaConversion.h")))  CbmAnaConversionPhotons2;
class __attribute__((annotate("$clingAutoload$CbmAnaConversionReco.h")))  __attribute__((annotate("$clingAutoload$CbmAnaConversion.h")))  CbmAnaConversionReco;
class __attribute__((annotate("$clingAutoload$CbmAnaConversionRecoFull.h")))  __attribute__((annotate("$clingAutoload$CbmAnaConversion.h")))  CbmAnaConversionRecoFull;
class __attribute__((annotate("$clingAutoload$CbmAnaConversionRich.h")))  __attribute__((annotate("$clingAutoload$CbmAnaConversion.h")))  CbmAnaConversionRich;
class __attribute__((annotate("$clingAutoload$CbmAnaConversionTest.h")))  __attribute__((annotate("$clingAutoload$CbmAnaConversion.h")))  CbmAnaConversionTest;
class __attribute__((annotate("$clingAutoload$CbmAnaConversionTest2.h")))  __attribute__((annotate("$clingAutoload$CbmAnaConversion.h")))  CbmAnaConversionTest2;
class __attribute__((annotate("$clingAutoload$CbmAnaConversionTomography.h")))  __attribute__((annotate("$clingAutoload$CbmAnaConversion.h")))  CbmAnaConversionTomography;
class __attribute__((annotate("$clingAutoload$CbmAnaConversion.h")))  CbmAnaConversion;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmConversionAnalysis dictionary payload"

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
#include "CbmAnaConversion.h"
#include "CbmAnaConversionKF.h"
#include "CbmAnaConversionTomography.h"
#include "CbmAnaConversionRich.h"
#include "CbmAnaConversionReco.h"
#include "CbmAnaConversionPhotons.h"
#include "CbmAnaConversionPhotons2.h"
#include "CbmAnaConversionRecoFull.h"
#include "CbmAnaConversionTest.h"
#include "CbmAnaConversionTest2.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmAnaConversion", payloadCode, "@",
"CbmAnaConversionKF", payloadCode, "@",
"CbmAnaConversionPhotons", payloadCode, "@",
"CbmAnaConversionPhotons2", payloadCode, "@",
"CbmAnaConversionReco", payloadCode, "@",
"CbmAnaConversionRecoFull", payloadCode, "@",
"CbmAnaConversionRich", payloadCode, "@",
"CbmAnaConversionTest", payloadCode, "@",
"CbmAnaConversionTest2", payloadCode, "@",
"CbmAnaConversionTomography", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmConversionAnalysis",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmConversionAnalysis_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmConversionAnalysis_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmConversionAnalysis() {
  TriggerDictionaryInitialization_libCbmConversionAnalysis_Impl();
}
