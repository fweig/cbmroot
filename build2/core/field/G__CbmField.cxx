// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmField
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
#include "CbmFieldConst.h"
#include "CbmFieldContFact.h"
#include "CbmFieldMap.h"
#include "CbmFieldMapCreator.h"
#include "CbmFieldMapData.h"
#include "CbmFieldMapSym2.h"
#include "CbmFieldMapSym3.h"
#include "CbmFieldPar.h"
#include "CbmBsField.h"
#include "CbmFieldCreator.h"
#include "CbmFieldMapDistorted.h"
#include "CbmFieldMapSym1.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmFieldConst(void *p = nullptr);
   static void *newArray_CbmFieldConst(Long_t size, void *p);
   static void delete_CbmFieldConst(void *p);
   static void deleteArray_CbmFieldConst(void *p);
   static void destruct_CbmFieldConst(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFieldConst*)
   {
      ::CbmFieldConst *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFieldConst >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFieldConst", ::CbmFieldConst::Class_Version(), "CbmFieldConst.h", 32,
                  typeid(::CbmFieldConst), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFieldConst::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFieldConst) );
      instance.SetNew(&new_CbmFieldConst);
      instance.SetNewArray(&newArray_CbmFieldConst);
      instance.SetDelete(&delete_CbmFieldConst);
      instance.SetDeleteArray(&deleteArray_CbmFieldConst);
      instance.SetDestructor(&destruct_CbmFieldConst);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFieldConst*)
   {
      return GenerateInitInstanceLocal((::CbmFieldConst*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFieldConst*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmFieldContFact(void *p = nullptr);
   static void *newArray_CbmFieldContFact(Long_t size, void *p);
   static void delete_CbmFieldContFact(void *p);
   static void deleteArray_CbmFieldContFact(void *p);
   static void destruct_CbmFieldContFact(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFieldContFact*)
   {
      ::CbmFieldContFact *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFieldContFact >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFieldContFact", ::CbmFieldContFact::Class_Version(), "CbmFieldContFact.h", 30,
                  typeid(::CbmFieldContFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFieldContFact::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFieldContFact) );
      instance.SetNew(&new_CbmFieldContFact);
      instance.SetNewArray(&newArray_CbmFieldContFact);
      instance.SetDelete(&delete_CbmFieldContFact);
      instance.SetDeleteArray(&deleteArray_CbmFieldContFact);
      instance.SetDestructor(&destruct_CbmFieldContFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFieldContFact*)
   {
      return GenerateInitInstanceLocal((::CbmFieldContFact*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFieldContFact*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmFieldMap(void *p = nullptr);
   static void *newArray_CbmFieldMap(Long_t size, void *p);
   static void delete_CbmFieldMap(void *p);
   static void deleteArray_CbmFieldMap(void *p);
   static void destruct_CbmFieldMap(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFieldMap*)
   {
      ::CbmFieldMap *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFieldMap >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFieldMap", ::CbmFieldMap::Class_Version(), "CbmFieldMap.h", 38,
                  typeid(::CbmFieldMap), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFieldMap::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFieldMap) );
      instance.SetNew(&new_CbmFieldMap);
      instance.SetNewArray(&newArray_CbmFieldMap);
      instance.SetDelete(&delete_CbmFieldMap);
      instance.SetDeleteArray(&deleteArray_CbmFieldMap);
      instance.SetDestructor(&destruct_CbmFieldMap);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFieldMap*)
   {
      return GenerateInitInstanceLocal((::CbmFieldMap*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFieldMap*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmFieldMapCreator(void *p = nullptr);
   static void *newArray_CbmFieldMapCreator(Long_t size, void *p);
   static void delete_CbmFieldMapCreator(void *p);
   static void deleteArray_CbmFieldMapCreator(void *p);
   static void destruct_CbmFieldMapCreator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFieldMapCreator*)
   {
      ::CbmFieldMapCreator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFieldMapCreator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFieldMapCreator", ::CbmFieldMapCreator::Class_Version(), "CbmFieldMapCreator.h", 33,
                  typeid(::CbmFieldMapCreator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFieldMapCreator::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFieldMapCreator) );
      instance.SetNew(&new_CbmFieldMapCreator);
      instance.SetNewArray(&newArray_CbmFieldMapCreator);
      instance.SetDelete(&delete_CbmFieldMapCreator);
      instance.SetDeleteArray(&deleteArray_CbmFieldMapCreator);
      instance.SetDestructor(&destruct_CbmFieldMapCreator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFieldMapCreator*)
   {
      return GenerateInitInstanceLocal((::CbmFieldMapCreator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFieldMapCreator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmFieldMapData(void *p = nullptr);
   static void *newArray_CbmFieldMapData(Long_t size, void *p);
   static void delete_CbmFieldMapData(void *p);
   static void deleteArray_CbmFieldMapData(void *p);
   static void destruct_CbmFieldMapData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFieldMapData*)
   {
      ::CbmFieldMapData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFieldMapData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFieldMapData", ::CbmFieldMapData::Class_Version(), "CbmFieldMapData.h", 33,
                  typeid(::CbmFieldMapData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFieldMapData::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFieldMapData) );
      instance.SetNew(&new_CbmFieldMapData);
      instance.SetNewArray(&newArray_CbmFieldMapData);
      instance.SetDelete(&delete_CbmFieldMapData);
      instance.SetDeleteArray(&deleteArray_CbmFieldMapData);
      instance.SetDestructor(&destruct_CbmFieldMapData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFieldMapData*)
   {
      return GenerateInitInstanceLocal((::CbmFieldMapData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFieldMapData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmFieldMapSym2(void *p = nullptr);
   static void *newArray_CbmFieldMapSym2(Long_t size, void *p);
   static void delete_CbmFieldMapSym2(void *p);
   static void deleteArray_CbmFieldMapSym2(void *p);
   static void destruct_CbmFieldMapSym2(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFieldMapSym2*)
   {
      ::CbmFieldMapSym2 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFieldMapSym2 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFieldMapSym2", ::CbmFieldMapSym2::Class_Version(), "CbmFieldMapSym2.h", 39,
                  typeid(::CbmFieldMapSym2), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFieldMapSym2::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFieldMapSym2) );
      instance.SetNew(&new_CbmFieldMapSym2);
      instance.SetNewArray(&newArray_CbmFieldMapSym2);
      instance.SetDelete(&delete_CbmFieldMapSym2);
      instance.SetDeleteArray(&deleteArray_CbmFieldMapSym2);
      instance.SetDestructor(&destruct_CbmFieldMapSym2);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFieldMapSym2*)
   {
      return GenerateInitInstanceLocal((::CbmFieldMapSym2*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFieldMapSym2*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmFieldMapSym3(void *p = nullptr);
   static void *newArray_CbmFieldMapSym3(Long_t size, void *p);
   static void delete_CbmFieldMapSym3(void *p);
   static void deleteArray_CbmFieldMapSym3(void *p);
   static void destruct_CbmFieldMapSym3(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFieldMapSym3*)
   {
      ::CbmFieldMapSym3 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFieldMapSym3 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFieldMapSym3", ::CbmFieldMapSym3::Class_Version(), "CbmFieldMapSym3.h", 39,
                  typeid(::CbmFieldMapSym3), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFieldMapSym3::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFieldMapSym3) );
      instance.SetNew(&new_CbmFieldMapSym3);
      instance.SetNewArray(&newArray_CbmFieldMapSym3);
      instance.SetDelete(&delete_CbmFieldMapSym3);
      instance.SetDeleteArray(&deleteArray_CbmFieldMapSym3);
      instance.SetDestructor(&destruct_CbmFieldMapSym3);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFieldMapSym3*)
   {
      return GenerateInitInstanceLocal((::CbmFieldMapSym3*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFieldMapSym3*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmFieldPar(void *p = nullptr);
   static void *newArray_CbmFieldPar(Long_t size, void *p);
   static void delete_CbmFieldPar(void *p);
   static void deleteArray_CbmFieldPar(void *p);
   static void destruct_CbmFieldPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFieldPar*)
   {
      ::CbmFieldPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFieldPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFieldPar", ::CbmFieldPar::Class_Version(), "CbmFieldPar.h", 35,
                  typeid(::CbmFieldPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFieldPar::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFieldPar) );
      instance.SetNew(&new_CbmFieldPar);
      instance.SetNewArray(&newArray_CbmFieldPar);
      instance.SetDelete(&delete_CbmFieldPar);
      instance.SetDeleteArray(&deleteArray_CbmFieldPar);
      instance.SetDestructor(&destruct_CbmFieldPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFieldPar*)
   {
      return GenerateInitInstanceLocal((::CbmFieldPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFieldPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmBsField(void *p = nullptr);
   static void *newArray_CbmBsField(Long_t size, void *p);
   static void delete_CbmBsField(void *p);
   static void deleteArray_CbmBsField(void *p);
   static void destruct_CbmBsField(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBsField*)
   {
      ::CbmBsField *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBsField >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBsField", ::CbmBsField::Class_Version(), "CbmBsField.h", 37,
                  typeid(::CbmBsField), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBsField::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBsField) );
      instance.SetNew(&new_CbmBsField);
      instance.SetNewArray(&newArray_CbmBsField);
      instance.SetDelete(&delete_CbmBsField);
      instance.SetDeleteArray(&deleteArray_CbmBsField);
      instance.SetDestructor(&destruct_CbmBsField);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBsField*)
   {
      return GenerateInitInstanceLocal((::CbmBsField*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBsField*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmFieldCreator(void *p = nullptr);
   static void *newArray_CbmFieldCreator(Long_t size, void *p);
   static void delete_CbmFieldCreator(void *p);
   static void deleteArray_CbmFieldCreator(void *p);
   static void destruct_CbmFieldCreator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFieldCreator*)
   {
      ::CbmFieldCreator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFieldCreator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFieldCreator", ::CbmFieldCreator::Class_Version(), "CbmFieldCreator.h", 21,
                  typeid(::CbmFieldCreator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFieldCreator::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFieldCreator) );
      instance.SetNew(&new_CbmFieldCreator);
      instance.SetNewArray(&newArray_CbmFieldCreator);
      instance.SetDelete(&delete_CbmFieldCreator);
      instance.SetDeleteArray(&deleteArray_CbmFieldCreator);
      instance.SetDestructor(&destruct_CbmFieldCreator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFieldCreator*)
   {
      return GenerateInitInstanceLocal((::CbmFieldCreator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFieldCreator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmFieldMapDistorted(void *p = nullptr);
   static void *newArray_CbmFieldMapDistorted(Long_t size, void *p);
   static void delete_CbmFieldMapDistorted(void *p);
   static void deleteArray_CbmFieldMapDistorted(void *p);
   static void destruct_CbmFieldMapDistorted(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFieldMapDistorted*)
   {
      ::CbmFieldMapDistorted *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFieldMapDistorted >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFieldMapDistorted", ::CbmFieldMapDistorted::Class_Version(), "CbmFieldMapDistorted.h", 24,
                  typeid(::CbmFieldMapDistorted), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFieldMapDistorted::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFieldMapDistorted) );
      instance.SetNew(&new_CbmFieldMapDistorted);
      instance.SetNewArray(&newArray_CbmFieldMapDistorted);
      instance.SetDelete(&delete_CbmFieldMapDistorted);
      instance.SetDeleteArray(&deleteArray_CbmFieldMapDistorted);
      instance.SetDestructor(&destruct_CbmFieldMapDistorted);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFieldMapDistorted*)
   {
      return GenerateInitInstanceLocal((::CbmFieldMapDistorted*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFieldMapDistorted*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmFieldMapSym1(void *p = nullptr);
   static void *newArray_CbmFieldMapSym1(Long_t size, void *p);
   static void delete_CbmFieldMapSym1(void *p);
   static void deleteArray_CbmFieldMapSym1(void *p);
   static void destruct_CbmFieldMapSym1(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmFieldMapSym1*)
   {
      ::CbmFieldMapSym1 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmFieldMapSym1 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmFieldMapSym1", ::CbmFieldMapSym1::Class_Version(), "CbmFieldMapSym1.h", 37,
                  typeid(::CbmFieldMapSym1), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmFieldMapSym1::Dictionary, isa_proxy, 4,
                  sizeof(::CbmFieldMapSym1) );
      instance.SetNew(&new_CbmFieldMapSym1);
      instance.SetNewArray(&newArray_CbmFieldMapSym1);
      instance.SetDelete(&delete_CbmFieldMapSym1);
      instance.SetDeleteArray(&deleteArray_CbmFieldMapSym1);
      instance.SetDestructor(&destruct_CbmFieldMapSym1);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmFieldMapSym1*)
   {
      return GenerateInitInstanceLocal((::CbmFieldMapSym1*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmFieldMapSym1*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmFieldConst::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFieldConst::Class_Name()
{
   return "CbmFieldConst";
}

//______________________________________________________________________________
const char *CbmFieldConst::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldConst*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFieldConst::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldConst*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFieldConst::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldConst*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFieldConst::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldConst*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmFieldContFact::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFieldContFact::Class_Name()
{
   return "CbmFieldContFact";
}

//______________________________________________________________________________
const char *CbmFieldContFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldContFact*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFieldContFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldContFact*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFieldContFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldContFact*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFieldContFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldContFact*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmFieldMap::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFieldMap::Class_Name()
{
   return "CbmFieldMap";
}

//______________________________________________________________________________
const char *CbmFieldMap::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMap*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFieldMap::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMap*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFieldMap::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMap*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFieldMap::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMap*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmFieldMapCreator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFieldMapCreator::Class_Name()
{
   return "CbmFieldMapCreator";
}

//______________________________________________________________________________
const char *CbmFieldMapCreator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapCreator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFieldMapCreator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapCreator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFieldMapCreator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapCreator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFieldMapCreator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapCreator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmFieldMapData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFieldMapData::Class_Name()
{
   return "CbmFieldMapData";
}

//______________________________________________________________________________
const char *CbmFieldMapData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFieldMapData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFieldMapData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFieldMapData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmFieldMapSym2::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFieldMapSym2::Class_Name()
{
   return "CbmFieldMapSym2";
}

//______________________________________________________________________________
const char *CbmFieldMapSym2::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapSym2*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFieldMapSym2::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapSym2*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFieldMapSym2::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapSym2*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFieldMapSym2::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapSym2*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmFieldMapSym3::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFieldMapSym3::Class_Name()
{
   return "CbmFieldMapSym3";
}

//______________________________________________________________________________
const char *CbmFieldMapSym3::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapSym3*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFieldMapSym3::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapSym3*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFieldMapSym3::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapSym3*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFieldMapSym3::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapSym3*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmFieldPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFieldPar::Class_Name()
{
   return "CbmFieldPar";
}

//______________________________________________________________________________
const char *CbmFieldPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFieldPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFieldPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFieldPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmBsField::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBsField::Class_Name()
{
   return "CbmBsField";
}

//______________________________________________________________________________
const char *CbmBsField::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBsField*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBsField::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBsField*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBsField::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBsField*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBsField::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBsField*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmFieldCreator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFieldCreator::Class_Name()
{
   return "CbmFieldCreator";
}

//______________________________________________________________________________
const char *CbmFieldCreator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldCreator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFieldCreator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldCreator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFieldCreator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldCreator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFieldCreator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldCreator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmFieldMapDistorted::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFieldMapDistorted::Class_Name()
{
   return "CbmFieldMapDistorted";
}

//______________________________________________________________________________
const char *CbmFieldMapDistorted::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapDistorted*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFieldMapDistorted::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapDistorted*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFieldMapDistorted::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapDistorted*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFieldMapDistorted::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapDistorted*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmFieldMapSym1::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmFieldMapSym1::Class_Name()
{
   return "CbmFieldMapSym1";
}

//______________________________________________________________________________
const char *CbmFieldMapSym1::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapSym1*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmFieldMapSym1::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapSym1*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmFieldMapSym1::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapSym1*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmFieldMapSym1::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmFieldMapSym1*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmFieldConst::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFieldConst.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFieldConst::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFieldConst::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFieldConst(void *p) {
      return  p ? new(p) ::CbmFieldConst : new ::CbmFieldConst;
   }
   static void *newArray_CbmFieldConst(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFieldConst[nElements] : new ::CbmFieldConst[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFieldConst(void *p) {
      delete ((::CbmFieldConst*)p);
   }
   static void deleteArray_CbmFieldConst(void *p) {
      delete [] ((::CbmFieldConst*)p);
   }
   static void destruct_CbmFieldConst(void *p) {
      typedef ::CbmFieldConst current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFieldConst

//______________________________________________________________________________
void CbmFieldContFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFieldContFact.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFieldContFact::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFieldContFact::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFieldContFact(void *p) {
      return  p ? new(p) ::CbmFieldContFact : new ::CbmFieldContFact;
   }
   static void *newArray_CbmFieldContFact(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFieldContFact[nElements] : new ::CbmFieldContFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFieldContFact(void *p) {
      delete ((::CbmFieldContFact*)p);
   }
   static void deleteArray_CbmFieldContFact(void *p) {
      delete [] ((::CbmFieldContFact*)p);
   }
   static void destruct_CbmFieldContFact(void *p) {
      typedef ::CbmFieldContFact current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFieldContFact

//______________________________________________________________________________
void CbmFieldMap::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFieldMap.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFieldMap::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFieldMap::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFieldMap(void *p) {
      return  p ? new(p) ::CbmFieldMap : new ::CbmFieldMap;
   }
   static void *newArray_CbmFieldMap(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFieldMap[nElements] : new ::CbmFieldMap[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFieldMap(void *p) {
      delete ((::CbmFieldMap*)p);
   }
   static void deleteArray_CbmFieldMap(void *p) {
      delete [] ((::CbmFieldMap*)p);
   }
   static void destruct_CbmFieldMap(void *p) {
      typedef ::CbmFieldMap current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFieldMap

//______________________________________________________________________________
void CbmFieldMapCreator::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFieldMapCreator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFieldMapCreator::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFieldMapCreator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFieldMapCreator(void *p) {
      return  p ? new(p) ::CbmFieldMapCreator : new ::CbmFieldMapCreator;
   }
   static void *newArray_CbmFieldMapCreator(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFieldMapCreator[nElements] : new ::CbmFieldMapCreator[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFieldMapCreator(void *p) {
      delete ((::CbmFieldMapCreator*)p);
   }
   static void deleteArray_CbmFieldMapCreator(void *p) {
      delete [] ((::CbmFieldMapCreator*)p);
   }
   static void destruct_CbmFieldMapCreator(void *p) {
      typedef ::CbmFieldMapCreator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFieldMapCreator

//______________________________________________________________________________
void CbmFieldMapData::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFieldMapData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFieldMapData::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFieldMapData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFieldMapData(void *p) {
      return  p ? new(p) ::CbmFieldMapData : new ::CbmFieldMapData;
   }
   static void *newArray_CbmFieldMapData(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFieldMapData[nElements] : new ::CbmFieldMapData[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFieldMapData(void *p) {
      delete ((::CbmFieldMapData*)p);
   }
   static void deleteArray_CbmFieldMapData(void *p) {
      delete [] ((::CbmFieldMapData*)p);
   }
   static void destruct_CbmFieldMapData(void *p) {
      typedef ::CbmFieldMapData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFieldMapData

//______________________________________________________________________________
void CbmFieldMapSym2::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFieldMapSym2.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFieldMapSym2::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFieldMapSym2::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFieldMapSym2(void *p) {
      return  p ? new(p) ::CbmFieldMapSym2 : new ::CbmFieldMapSym2;
   }
   static void *newArray_CbmFieldMapSym2(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFieldMapSym2[nElements] : new ::CbmFieldMapSym2[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFieldMapSym2(void *p) {
      delete ((::CbmFieldMapSym2*)p);
   }
   static void deleteArray_CbmFieldMapSym2(void *p) {
      delete [] ((::CbmFieldMapSym2*)p);
   }
   static void destruct_CbmFieldMapSym2(void *p) {
      typedef ::CbmFieldMapSym2 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFieldMapSym2

//______________________________________________________________________________
void CbmFieldMapSym3::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFieldMapSym3.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFieldMapSym3::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFieldMapSym3::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFieldMapSym3(void *p) {
      return  p ? new(p) ::CbmFieldMapSym3 : new ::CbmFieldMapSym3;
   }
   static void *newArray_CbmFieldMapSym3(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFieldMapSym3[nElements] : new ::CbmFieldMapSym3[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFieldMapSym3(void *p) {
      delete ((::CbmFieldMapSym3*)p);
   }
   static void deleteArray_CbmFieldMapSym3(void *p) {
      delete [] ((::CbmFieldMapSym3*)p);
   }
   static void destruct_CbmFieldMapSym3(void *p) {
      typedef ::CbmFieldMapSym3 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFieldMapSym3

//______________________________________________________________________________
void CbmFieldPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFieldPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFieldPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFieldPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFieldPar(void *p) {
      return  p ? new(p) ::CbmFieldPar : new ::CbmFieldPar;
   }
   static void *newArray_CbmFieldPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFieldPar[nElements] : new ::CbmFieldPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFieldPar(void *p) {
      delete ((::CbmFieldPar*)p);
   }
   static void deleteArray_CbmFieldPar(void *p) {
      delete [] ((::CbmFieldPar*)p);
   }
   static void destruct_CbmFieldPar(void *p) {
      typedef ::CbmFieldPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFieldPar

//______________________________________________________________________________
void CbmBsField::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBsField.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBsField::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBsField::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBsField(void *p) {
      return  p ? new(p) ::CbmBsField : new ::CbmBsField;
   }
   static void *newArray_CbmBsField(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBsField[nElements] : new ::CbmBsField[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBsField(void *p) {
      delete ((::CbmBsField*)p);
   }
   static void deleteArray_CbmBsField(void *p) {
      delete [] ((::CbmBsField*)p);
   }
   static void destruct_CbmBsField(void *p) {
      typedef ::CbmBsField current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBsField

//______________________________________________________________________________
void CbmFieldCreator::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFieldCreator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFieldCreator::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFieldCreator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFieldCreator(void *p) {
      return  p ? new(p) ::CbmFieldCreator : new ::CbmFieldCreator;
   }
   static void *newArray_CbmFieldCreator(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFieldCreator[nElements] : new ::CbmFieldCreator[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFieldCreator(void *p) {
      delete ((::CbmFieldCreator*)p);
   }
   static void deleteArray_CbmFieldCreator(void *p) {
      delete [] ((::CbmFieldCreator*)p);
   }
   static void destruct_CbmFieldCreator(void *p) {
      typedef ::CbmFieldCreator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFieldCreator

//______________________________________________________________________________
void CbmFieldMapDistorted::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFieldMapDistorted.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFieldMapDistorted::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFieldMapDistorted::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFieldMapDistorted(void *p) {
      return  p ? new(p) ::CbmFieldMapDistorted : new ::CbmFieldMapDistorted;
   }
   static void *newArray_CbmFieldMapDistorted(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFieldMapDistorted[nElements] : new ::CbmFieldMapDistorted[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFieldMapDistorted(void *p) {
      delete ((::CbmFieldMapDistorted*)p);
   }
   static void deleteArray_CbmFieldMapDistorted(void *p) {
      delete [] ((::CbmFieldMapDistorted*)p);
   }
   static void destruct_CbmFieldMapDistorted(void *p) {
      typedef ::CbmFieldMapDistorted current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFieldMapDistorted

//______________________________________________________________________________
void CbmFieldMapSym1::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmFieldMapSym1.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmFieldMapSym1::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmFieldMapSym1::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmFieldMapSym1(void *p) {
      return  p ? new(p) ::CbmFieldMapSym1 : new ::CbmFieldMapSym1;
   }
   static void *newArray_CbmFieldMapSym1(Long_t nElements, void *p) {
      return p ? new(p) ::CbmFieldMapSym1[nElements] : new ::CbmFieldMapSym1[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmFieldMapSym1(void *p) {
      delete ((::CbmFieldMapSym1*)p);
   }
   static void deleteArray_CbmFieldMapSym1(void *p) {
      delete [] ((::CbmFieldMapSym1*)p);
   }
   static void destruct_CbmFieldMapSym1(void *p) {
      typedef ::CbmFieldMapSym1 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmFieldMapSym1

namespace {
  void TriggerDictionaryInitialization_libCbmField_Impl() {
    static const char* headers[] = {
"CbmFieldConst.h",
"CbmFieldContFact.h",
"CbmFieldMap.h",
"CbmFieldMapCreator.h",
"CbmFieldMapData.h",
"CbmFieldMapSym2.h",
"CbmFieldMapSym3.h",
"CbmFieldPar.h",
"CbmBsField.h",
"CbmFieldCreator.h",
"CbmFieldMapDistorted.h",
"CbmFieldMapSym1.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/field",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/field",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/core/field/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmField dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmFieldConst.h")))  CbmFieldConst;
class __attribute__((annotate("$clingAutoload$CbmFieldContFact.h")))  CbmFieldContFact;
class __attribute__((annotate("$clingAutoload$CbmFieldMap.h")))  CbmFieldMap;
class __attribute__((annotate("$clingAutoload$CbmFieldMapCreator.h")))  CbmFieldMapCreator;
class __attribute__((annotate("$clingAutoload$CbmFieldMapData.h")))  CbmFieldMapData;
class __attribute__((annotate("$clingAutoload$CbmFieldMapSym2.h")))  CbmFieldMapSym2;
class __attribute__((annotate("$clingAutoload$CbmFieldMapSym3.h")))  CbmFieldMapSym3;
class __attribute__((annotate("$clingAutoload$CbmFieldPar.h")))  CbmFieldPar;
class __attribute__((annotate("$clingAutoload$CbmBsField.h")))  CbmBsField;
class __attribute__((annotate("$clingAutoload$CbmFieldCreator.h")))  CbmFieldCreator;
class __attribute__((annotate("$clingAutoload$CbmFieldMapDistorted.h")))  CbmFieldMapDistorted;
class __attribute__((annotate("$clingAutoload$CbmFieldMapSym1.h")))  CbmFieldMapSym1;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmField dictionary payload"

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

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "CbmFieldConst.h"
#include "CbmFieldContFact.h"
#include "CbmFieldMap.h"
#include "CbmFieldMapCreator.h"
#include "CbmFieldMapData.h"
#include "CbmFieldMapSym2.h"
#include "CbmFieldMapSym3.h"
#include "CbmFieldPar.h"
#include "CbmBsField.h"
#include "CbmFieldCreator.h"
#include "CbmFieldMapDistorted.h"
#include "CbmFieldMapSym1.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmBsField", payloadCode, "@",
"CbmFieldConst", payloadCode, "@",
"CbmFieldContFact", payloadCode, "@",
"CbmFieldCreator", payloadCode, "@",
"CbmFieldMap", payloadCode, "@",
"CbmFieldMapCreator", payloadCode, "@",
"CbmFieldMapData", payloadCode, "@",
"CbmFieldMapDistorted", payloadCode, "@",
"CbmFieldMapSym1", payloadCode, "@",
"CbmFieldMapSym2", payloadCode, "@",
"CbmFieldMapSym3", payloadCode, "@",
"CbmFieldPar", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmField",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmField_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmField_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmField() {
  TriggerDictionaryInitialization_libCbmField_Impl();
}
