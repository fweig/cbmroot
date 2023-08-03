// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmTofBase
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
#include "CbmTofCell.h"
#include "CbmTofContFact.h"
#include "CbmTofCreateDigiPar.h"
#include "CbmTofDigiBdfPar.h"
#include "CbmTofDigiPar.h"
#include "CbmMcbm2018TofPar.h"
#include "CbmTofGeoHandler.h"
#include "CbmTofTrackingInterface.h"
#include "TTrbHeader.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmTofCell(void *p = nullptr);
   static void *newArray_CbmTofCell(Long_t size, void *p);
   static void delete_CbmTofCell(void *p);
   static void deleteArray_CbmTofCell(void *p);
   static void destruct_CbmTofCell(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofCell*)
   {
      ::CbmTofCell *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofCell >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofCell", ::CbmTofCell::Class_Version(), "CbmTofCell.h", 12,
                  typeid(::CbmTofCell), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofCell::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofCell) );
      instance.SetNew(&new_CbmTofCell);
      instance.SetNewArray(&newArray_CbmTofCell);
      instance.SetDelete(&delete_CbmTofCell);
      instance.SetDeleteArray(&deleteArray_CbmTofCell);
      instance.SetDestructor(&destruct_CbmTofCell);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofCell*)
   {
      return GenerateInitInstanceLocal((::CbmTofCell*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofCell*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofContFact(void *p = nullptr);
   static void *newArray_CbmTofContFact(Long_t size, void *p);
   static void delete_CbmTofContFact(void *p);
   static void deleteArray_CbmTofContFact(void *p);
   static void destruct_CbmTofContFact(void *p);
   static void streamer_CbmTofContFact(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofContFact*)
   {
      ::CbmTofContFact *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofContFact >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofContFact", ::CbmTofContFact::Class_Version(), "CbmTofContFact.h", 15,
                  typeid(::CbmTofContFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofContFact::Dictionary, isa_proxy, 16,
                  sizeof(::CbmTofContFact) );
      instance.SetNew(&new_CbmTofContFact);
      instance.SetNewArray(&newArray_CbmTofContFact);
      instance.SetDelete(&delete_CbmTofContFact);
      instance.SetDeleteArray(&deleteArray_CbmTofContFact);
      instance.SetDestructor(&destruct_CbmTofContFact);
      instance.SetStreamerFunc(&streamer_CbmTofContFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofContFact*)
   {
      return GenerateInitInstanceLocal((::CbmTofContFact*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofContFact*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofCreateDigiPar(void *p = nullptr);
   static void *newArray_CbmTofCreateDigiPar(Long_t size, void *p);
   static void delete_CbmTofCreateDigiPar(void *p);
   static void deleteArray_CbmTofCreateDigiPar(void *p);
   static void destruct_CbmTofCreateDigiPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofCreateDigiPar*)
   {
      ::CbmTofCreateDigiPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofCreateDigiPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofCreateDigiPar", ::CbmTofCreateDigiPar::Class_Version(), "CbmTofCreateDigiPar.h", 27,
                  typeid(::CbmTofCreateDigiPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofCreateDigiPar::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofCreateDigiPar) );
      instance.SetNew(&new_CbmTofCreateDigiPar);
      instance.SetNewArray(&newArray_CbmTofCreateDigiPar);
      instance.SetDelete(&delete_CbmTofCreateDigiPar);
      instance.SetDeleteArray(&deleteArray_CbmTofCreateDigiPar);
      instance.SetDestructor(&destruct_CbmTofCreateDigiPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofCreateDigiPar*)
   {
      return GenerateInitInstanceLocal((::CbmTofCreateDigiPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofCreateDigiPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofDigiBdfPar(void *p = nullptr);
   static void *newArray_CbmTofDigiBdfPar(Long_t size, void *p);
   static void delete_CbmTofDigiBdfPar(void *p);
   static void deleteArray_CbmTofDigiBdfPar(void *p);
   static void destruct_CbmTofDigiBdfPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofDigiBdfPar*)
   {
      ::CbmTofDigiBdfPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofDigiBdfPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofDigiBdfPar", ::CbmTofDigiBdfPar::Class_Version(), "CbmTofDigiBdfPar.h", 37,
                  typeid(::CbmTofDigiBdfPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofDigiBdfPar::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofDigiBdfPar) );
      instance.SetNew(&new_CbmTofDigiBdfPar);
      instance.SetNewArray(&newArray_CbmTofDigiBdfPar);
      instance.SetDelete(&delete_CbmTofDigiBdfPar);
      instance.SetDeleteArray(&deleteArray_CbmTofDigiBdfPar);
      instance.SetDestructor(&destruct_CbmTofDigiBdfPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofDigiBdfPar*)
   {
      return GenerateInitInstanceLocal((::CbmTofDigiBdfPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofDigiBdfPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofDigiPar(void *p = nullptr);
   static void *newArray_CbmTofDigiPar(Long_t size, void *p);
   static void delete_CbmTofDigiPar(void *p);
   static void deleteArray_CbmTofDigiPar(void *p);
   static void destruct_CbmTofDigiPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofDigiPar*)
   {
      ::CbmTofDigiPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofDigiPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofDigiPar", ::CbmTofDigiPar::Class_Version(), "CbmTofDigiPar.h", 22,
                  typeid(::CbmTofDigiPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofDigiPar::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofDigiPar) );
      instance.SetNew(&new_CbmTofDigiPar);
      instance.SetNewArray(&newArray_CbmTofDigiPar);
      instance.SetDelete(&delete_CbmTofDigiPar);
      instance.SetDeleteArray(&deleteArray_CbmTofDigiPar);
      instance.SetDestructor(&destruct_CbmTofDigiPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofDigiPar*)
   {
      return GenerateInitInstanceLocal((::CbmTofDigiPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofDigiPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018TofPar(void *p = nullptr);
   static void *newArray_CbmMcbm2018TofPar(Long_t size, void *p);
   static void delete_CbmMcbm2018TofPar(void *p);
   static void deleteArray_CbmMcbm2018TofPar(void *p);
   static void destruct_CbmMcbm2018TofPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018TofPar*)
   {
      ::CbmMcbm2018TofPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018TofPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018TofPar", ::CbmMcbm2018TofPar::Class_Version(), "CbmMcbm2018TofPar.h", 22,
                  typeid(::CbmMcbm2018TofPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018TofPar::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2018TofPar) );
      instance.SetNew(&new_CbmMcbm2018TofPar);
      instance.SetNewArray(&newArray_CbmMcbm2018TofPar);
      instance.SetDelete(&delete_CbmMcbm2018TofPar);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018TofPar);
      instance.SetDestructor(&destruct_CbmMcbm2018TofPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018TofPar*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018TofPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018TofPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018BmonPar(void *p = nullptr);
   static void *newArray_CbmMcbm2018BmonPar(Long_t size, void *p);
   static void delete_CbmMcbm2018BmonPar(void *p);
   static void deleteArray_CbmMcbm2018BmonPar(void *p);
   static void destruct_CbmMcbm2018BmonPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018BmonPar*)
   {
      ::CbmMcbm2018BmonPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018BmonPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018BmonPar", ::CbmMcbm2018BmonPar::Class_Version(), "CbmMcbm2018TofPar.h", 172,
                  typeid(::CbmMcbm2018BmonPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018BmonPar::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2018BmonPar) );
      instance.SetNew(&new_CbmMcbm2018BmonPar);
      instance.SetNewArray(&newArray_CbmMcbm2018BmonPar);
      instance.SetDelete(&delete_CbmMcbm2018BmonPar);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018BmonPar);
      instance.SetDestructor(&destruct_CbmMcbm2018BmonPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018BmonPar*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018BmonPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018BmonPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofGeoHandler(void *p = nullptr);
   static void *newArray_CbmTofGeoHandler(Long_t size, void *p);
   static void delete_CbmTofGeoHandler(void *p);
   static void deleteArray_CbmTofGeoHandler(void *p);
   static void destruct_CbmTofGeoHandler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofGeoHandler*)
   {
      ::CbmTofGeoHandler *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofGeoHandler >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofGeoHandler", ::CbmTofGeoHandler::Class_Version(), "CbmTofGeoHandler.h", 41,
                  typeid(::CbmTofGeoHandler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofGeoHandler::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofGeoHandler) );
      instance.SetNew(&new_CbmTofGeoHandler);
      instance.SetNewArray(&newArray_CbmTofGeoHandler);
      instance.SetDelete(&delete_CbmTofGeoHandler);
      instance.SetDeleteArray(&deleteArray_CbmTofGeoHandler);
      instance.SetDestructor(&destruct_CbmTofGeoHandler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofGeoHandler*)
   {
      return GenerateInitInstanceLocal((::CbmTofGeoHandler*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofGeoHandler*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofTrackingInterface(void *p = nullptr);
   static void *newArray_CbmTofTrackingInterface(Long_t size, void *p);
   static void delete_CbmTofTrackingInterface(void *p);
   static void deleteArray_CbmTofTrackingInterface(void *p);
   static void destruct_CbmTofTrackingInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofTrackingInterface*)
   {
      ::CbmTofTrackingInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofTrackingInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofTrackingInterface", ::CbmTofTrackingInterface::Class_Version(), "CbmTofTrackingInterface.h", 34,
                  typeid(::CbmTofTrackingInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofTrackingInterface::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofTrackingInterface) );
      instance.SetNew(&new_CbmTofTrackingInterface);
      instance.SetNewArray(&newArray_CbmTofTrackingInterface);
      instance.SetDelete(&delete_CbmTofTrackingInterface);
      instance.SetDeleteArray(&deleteArray_CbmTofTrackingInterface);
      instance.SetDestructor(&destruct_CbmTofTrackingInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofTrackingInterface*)
   {
      return GenerateInitInstanceLocal((::CbmTofTrackingInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofTrackingInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TTrbHeader(void *p = nullptr);
   static void *newArray_TTrbHeader(Long_t size, void *p);
   static void delete_TTrbHeader(void *p);
   static void deleteArray_TTrbHeader(void *p);
   static void destruct_TTrbHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TTrbHeader*)
   {
      ::TTrbHeader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TTrbHeader >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("TTrbHeader", ::TTrbHeader::Class_Version(), "TTrbHeader.h", 14,
                  typeid(::TTrbHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TTrbHeader::Dictionary, isa_proxy, 4,
                  sizeof(::TTrbHeader) );
      instance.SetNew(&new_TTrbHeader);
      instance.SetNewArray(&newArray_TTrbHeader);
      instance.SetDelete(&delete_TTrbHeader);
      instance.SetDeleteArray(&deleteArray_TTrbHeader);
      instance.SetDestructor(&destruct_TTrbHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TTrbHeader*)
   {
      return GenerateInitInstanceLocal((::TTrbHeader*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TTrbHeader*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmTofCell::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofCell::Class_Name()
{
   return "CbmTofCell";
}

//______________________________________________________________________________
const char *CbmTofCell::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCell*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofCell::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCell*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofCell::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCell*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofCell::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCell*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofContFact::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofContFact::Class_Name()
{
   return "CbmTofContFact";
}

//______________________________________________________________________________
const char *CbmTofContFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofContFact*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofContFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofContFact*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofContFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofContFact*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofContFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofContFact*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofCreateDigiPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofCreateDigiPar::Class_Name()
{
   return "CbmTofCreateDigiPar";
}

//______________________________________________________________________________
const char *CbmTofCreateDigiPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCreateDigiPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofCreateDigiPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCreateDigiPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofCreateDigiPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCreateDigiPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofCreateDigiPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofCreateDigiPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofDigiBdfPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofDigiBdfPar::Class_Name()
{
   return "CbmTofDigiBdfPar";
}

//______________________________________________________________________________
const char *CbmTofDigiBdfPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigiBdfPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofDigiBdfPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigiBdfPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofDigiBdfPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigiBdfPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofDigiBdfPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigiBdfPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofDigiPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofDigiPar::Class_Name()
{
   return "CbmTofDigiPar";
}

//______________________________________________________________________________
const char *CbmTofDigiPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigiPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofDigiPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigiPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofDigiPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigiPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofDigiPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigiPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018TofPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018TofPar::Class_Name()
{
   return "CbmMcbm2018TofPar";
}

//______________________________________________________________________________
const char *CbmMcbm2018TofPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018TofPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018TofPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018TofPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018TofPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018TofPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018TofPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018TofPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018BmonPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018BmonPar::Class_Name()
{
   return "CbmMcbm2018BmonPar";
}

//______________________________________________________________________________
const char *CbmMcbm2018BmonPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018BmonPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018BmonPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018BmonPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018BmonPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018BmonPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018BmonPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018BmonPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofGeoHandler::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofGeoHandler::Class_Name()
{
   return "CbmTofGeoHandler";
}

//______________________________________________________________________________
const char *CbmTofGeoHandler::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofGeoHandler*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofGeoHandler::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofGeoHandler*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofGeoHandler::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofGeoHandler*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofGeoHandler::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofGeoHandler*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofTrackingInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofTrackingInterface::Class_Name()
{
   return "CbmTofTrackingInterface";
}

//______________________________________________________________________________
const char *CbmTofTrackingInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackingInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofTrackingInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackingInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofTrackingInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackingInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofTrackingInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackingInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TTrbHeader::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TTrbHeader::Class_Name()
{
   return "TTrbHeader";
}

//______________________________________________________________________________
const char *TTrbHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TTrbHeader*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TTrbHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TTrbHeader*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TTrbHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TTrbHeader*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TTrbHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TTrbHeader*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmTofCell::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofCell.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofCell::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofCell::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofCell(void *p) {
      return  p ? new(p) ::CbmTofCell : new ::CbmTofCell;
   }
   static void *newArray_CbmTofCell(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofCell[nElements] : new ::CbmTofCell[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofCell(void *p) {
      delete ((::CbmTofCell*)p);
   }
   static void deleteArray_CbmTofCell(void *p) {
      delete [] ((::CbmTofCell*)p);
   }
   static void destruct_CbmTofCell(void *p) {
      typedef ::CbmTofCell current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofCell

//______________________________________________________________________________
void CbmTofContFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofContFact.

   FairContFact::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofContFact(void *p) {
      return  p ? new(p) ::CbmTofContFact : new ::CbmTofContFact;
   }
   static void *newArray_CbmTofContFact(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofContFact[nElements] : new ::CbmTofContFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofContFact(void *p) {
      delete ((::CbmTofContFact*)p);
   }
   static void deleteArray_CbmTofContFact(void *p) {
      delete [] ((::CbmTofContFact*)p);
   }
   static void destruct_CbmTofContFact(void *p) {
      typedef ::CbmTofContFact current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmTofContFact(TBuffer &buf, void *obj) {
      ((::CbmTofContFact*)obj)->::CbmTofContFact::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmTofContFact

//______________________________________________________________________________
void CbmTofCreateDigiPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofCreateDigiPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofCreateDigiPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofCreateDigiPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofCreateDigiPar(void *p) {
      return  p ? new(p) ::CbmTofCreateDigiPar : new ::CbmTofCreateDigiPar;
   }
   static void *newArray_CbmTofCreateDigiPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofCreateDigiPar[nElements] : new ::CbmTofCreateDigiPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofCreateDigiPar(void *p) {
      delete ((::CbmTofCreateDigiPar*)p);
   }
   static void deleteArray_CbmTofCreateDigiPar(void *p) {
      delete [] ((::CbmTofCreateDigiPar*)p);
   }
   static void destruct_CbmTofCreateDigiPar(void *p) {
      typedef ::CbmTofCreateDigiPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofCreateDigiPar

//______________________________________________________________________________
void CbmTofDigiBdfPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofDigiBdfPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofDigiBdfPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofDigiBdfPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofDigiBdfPar(void *p) {
      return  p ? new(p) ::CbmTofDigiBdfPar : new ::CbmTofDigiBdfPar;
   }
   static void *newArray_CbmTofDigiBdfPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofDigiBdfPar[nElements] : new ::CbmTofDigiBdfPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofDigiBdfPar(void *p) {
      delete ((::CbmTofDigiBdfPar*)p);
   }
   static void deleteArray_CbmTofDigiBdfPar(void *p) {
      delete [] ((::CbmTofDigiBdfPar*)p);
   }
   static void destruct_CbmTofDigiBdfPar(void *p) {
      typedef ::CbmTofDigiBdfPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofDigiBdfPar

//______________________________________________________________________________
void CbmTofDigiPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofDigiPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofDigiPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofDigiPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofDigiPar(void *p) {
      return  p ? new(p) ::CbmTofDigiPar : new ::CbmTofDigiPar;
   }
   static void *newArray_CbmTofDigiPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofDigiPar[nElements] : new ::CbmTofDigiPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofDigiPar(void *p) {
      delete ((::CbmTofDigiPar*)p);
   }
   static void deleteArray_CbmTofDigiPar(void *p) {
      delete [] ((::CbmTofDigiPar*)p);
   }
   static void destruct_CbmTofDigiPar(void *p) {
      typedef ::CbmTofDigiPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofDigiPar

//______________________________________________________________________________
void CbmMcbm2018TofPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018TofPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2018TofPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2018TofPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018TofPar(void *p) {
      return  p ? new(p) ::CbmMcbm2018TofPar : new ::CbmMcbm2018TofPar;
   }
   static void *newArray_CbmMcbm2018TofPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018TofPar[nElements] : new ::CbmMcbm2018TofPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018TofPar(void *p) {
      delete ((::CbmMcbm2018TofPar*)p);
   }
   static void deleteArray_CbmMcbm2018TofPar(void *p) {
      delete [] ((::CbmMcbm2018TofPar*)p);
   }
   static void destruct_CbmMcbm2018TofPar(void *p) {
      typedef ::CbmMcbm2018TofPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2018TofPar

//______________________________________________________________________________
void CbmMcbm2018BmonPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018BmonPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2018BmonPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2018BmonPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018BmonPar(void *p) {
      return  p ? new(p) ::CbmMcbm2018BmonPar : new ::CbmMcbm2018BmonPar;
   }
   static void *newArray_CbmMcbm2018BmonPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018BmonPar[nElements] : new ::CbmMcbm2018BmonPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018BmonPar(void *p) {
      delete ((::CbmMcbm2018BmonPar*)p);
   }
   static void deleteArray_CbmMcbm2018BmonPar(void *p) {
      delete [] ((::CbmMcbm2018BmonPar*)p);
   }
   static void destruct_CbmMcbm2018BmonPar(void *p) {
      typedef ::CbmMcbm2018BmonPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2018BmonPar

//______________________________________________________________________________
void CbmTofGeoHandler::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofGeoHandler.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofGeoHandler::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofGeoHandler::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofGeoHandler(void *p) {
      return  p ? new(p) ::CbmTofGeoHandler : new ::CbmTofGeoHandler;
   }
   static void *newArray_CbmTofGeoHandler(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofGeoHandler[nElements] : new ::CbmTofGeoHandler[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofGeoHandler(void *p) {
      delete ((::CbmTofGeoHandler*)p);
   }
   static void deleteArray_CbmTofGeoHandler(void *p) {
      delete [] ((::CbmTofGeoHandler*)p);
   }
   static void destruct_CbmTofGeoHandler(void *p) {
      typedef ::CbmTofGeoHandler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofGeoHandler

//______________________________________________________________________________
void CbmTofTrackingInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofTrackingInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofTrackingInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofTrackingInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofTrackingInterface(void *p) {
      return  p ? new(p) ::CbmTofTrackingInterface : new ::CbmTofTrackingInterface;
   }
   static void *newArray_CbmTofTrackingInterface(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofTrackingInterface[nElements] : new ::CbmTofTrackingInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofTrackingInterface(void *p) {
      delete ((::CbmTofTrackingInterface*)p);
   }
   static void deleteArray_CbmTofTrackingInterface(void *p) {
      delete [] ((::CbmTofTrackingInterface*)p);
   }
   static void destruct_CbmTofTrackingInterface(void *p) {
      typedef ::CbmTofTrackingInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofTrackingInterface

//______________________________________________________________________________
void TTrbHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class TTrbHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TTrbHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(TTrbHeader::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TTrbHeader(void *p) {
      return  p ? new(p) ::TTrbHeader : new ::TTrbHeader;
   }
   static void *newArray_TTrbHeader(Long_t nElements, void *p) {
      return p ? new(p) ::TTrbHeader[nElements] : new ::TTrbHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_TTrbHeader(void *p) {
      delete ((::TTrbHeader*)p);
   }
   static void deleteArray_TTrbHeader(void *p) {
      delete [] ((::TTrbHeader*)p);
   }
   static void destruct_TTrbHeader(void *p) {
      typedef ::TTrbHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TTrbHeader

namespace ROOT {
   static TClass *vectorlEunsignedsPshortgR_Dictionary();
   static void vectorlEunsignedsPshortgR_TClassManip(TClass*);
   static void *new_vectorlEunsignedsPshortgR(void *p = nullptr);
   static void *newArray_vectorlEunsignedsPshortgR(Long_t size, void *p);
   static void delete_vectorlEunsignedsPshortgR(void *p);
   static void deleteArray_vectorlEunsignedsPshortgR(void *p);
   static void destruct_vectorlEunsignedsPshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned short>*)
   {
      vector<unsigned short> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned short>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<unsigned short>", -2, "vector", 389,
                  typeid(vector<unsigned short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEunsignedsPshortgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<unsigned short>) );
      instance.SetNew(&new_vectorlEunsignedsPshortgR);
      instance.SetNewArray(&newArray_vectorlEunsignedsPshortgR);
      instance.SetDelete(&delete_vectorlEunsignedsPshortgR);
      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPshortgR);
      instance.SetDestructor(&destruct_vectorlEunsignedsPshortgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned short> >()));

      ::ROOT::AddClassAlternate("vector<unsigned short>","std::vector<unsigned short, std::allocator<unsigned short> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<unsigned short>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEunsignedsPshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<unsigned short>*)nullptr)->GetClass();
      vectorlEunsignedsPshortgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEunsignedsPshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEunsignedsPshortgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned short> : new vector<unsigned short>;
   }
   static void *newArray_vectorlEunsignedsPshortgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned short>[nElements] : new vector<unsigned short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEunsignedsPshortgR(void *p) {
      delete ((vector<unsigned short>*)p);
   }
   static void deleteArray_vectorlEunsignedsPshortgR(void *p) {
      delete [] ((vector<unsigned short>*)p);
   }
   static void destruct_vectorlEunsignedsPshortgR(void *p) {
      typedef vector<unsigned short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<unsigned short>

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
   static TClass *vectorlETArrayIgR_Dictionary();
   static void vectorlETArrayIgR_TClassManip(TClass*);
   static void *new_vectorlETArrayIgR(void *p = nullptr);
   static void *newArray_vectorlETArrayIgR(Long_t size, void *p);
   static void delete_vectorlETArrayIgR(void *p);
   static void deleteArray_vectorlETArrayIgR(void *p);
   static void destruct_vectorlETArrayIgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TArrayI>*)
   {
      vector<TArrayI> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TArrayI>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TArrayI>", -2, "vector", 389,
                  typeid(vector<TArrayI>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETArrayIgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TArrayI>) );
      instance.SetNew(&new_vectorlETArrayIgR);
      instance.SetNewArray(&newArray_vectorlETArrayIgR);
      instance.SetDelete(&delete_vectorlETArrayIgR);
      instance.SetDeleteArray(&deleteArray_vectorlETArrayIgR);
      instance.SetDestructor(&destruct_vectorlETArrayIgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TArrayI> >()));

      ::ROOT::AddClassAlternate("vector<TArrayI>","std::vector<TArrayI, std::allocator<TArrayI> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TArrayI>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETArrayIgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TArrayI>*)nullptr)->GetClass();
      vectorlETArrayIgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETArrayIgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETArrayIgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TArrayI> : new vector<TArrayI>;
   }
   static void *newArray_vectorlETArrayIgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TArrayI>[nElements] : new vector<TArrayI>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETArrayIgR(void *p) {
      delete ((vector<TArrayI>*)p);
   }
   static void deleteArray_vectorlETArrayIgR(void *p) {
      delete [] ((vector<TArrayI>*)p);
   }
   static void destruct_vectorlETArrayIgR(void *p) {
      typedef vector<TArrayI> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TArrayI>

namespace ROOT {
   static TClass *vectorlETArrayDgR_Dictionary();
   static void vectorlETArrayDgR_TClassManip(TClass*);
   static void *new_vectorlETArrayDgR(void *p = nullptr);
   static void *newArray_vectorlETArrayDgR(Long_t size, void *p);
   static void delete_vectorlETArrayDgR(void *p);
   static void deleteArray_vectorlETArrayDgR(void *p);
   static void destruct_vectorlETArrayDgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TArrayD>*)
   {
      vector<TArrayD> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TArrayD>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TArrayD>", -2, "vector", 389,
                  typeid(vector<TArrayD>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETArrayDgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TArrayD>) );
      instance.SetNew(&new_vectorlETArrayDgR);
      instance.SetNewArray(&newArray_vectorlETArrayDgR);
      instance.SetDelete(&delete_vectorlETArrayDgR);
      instance.SetDeleteArray(&deleteArray_vectorlETArrayDgR);
      instance.SetDestructor(&destruct_vectorlETArrayDgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TArrayD> >()));

      ::ROOT::AddClassAlternate("vector<TArrayD>","std::vector<TArrayD, std::allocator<TArrayD> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TArrayD>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETArrayDgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TArrayD>*)nullptr)->GetClass();
      vectorlETArrayDgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETArrayDgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETArrayDgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TArrayD> : new vector<TArrayD>;
   }
   static void *newArray_vectorlETArrayDgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TArrayD>[nElements] : new vector<TArrayD>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETArrayDgR(void *p) {
      delete ((vector<TArrayD>*)p);
   }
   static void deleteArray_vectorlETArrayDgR(void *p) {
      delete [] ((vector<TArrayD>*)p);
   }
   static void destruct_vectorlETArrayDgR(void *p) {
      typedef vector<TArrayD> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TArrayD>

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

namespace ROOT {
   static TClass *maplEintcOTGeoNodemUgR_Dictionary();
   static void maplEintcOTGeoNodemUgR_TClassManip(TClass*);
   static void *new_maplEintcOTGeoNodemUgR(void *p = nullptr);
   static void *newArray_maplEintcOTGeoNodemUgR(Long_t size, void *p);
   static void delete_maplEintcOTGeoNodemUgR(void *p);
   static void deleteArray_maplEintcOTGeoNodemUgR(void *p);
   static void destruct_maplEintcOTGeoNodemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,TGeoNode*>*)
   {
      map<int,TGeoNode*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,TGeoNode*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,TGeoNode*>", -2, "map", 100,
                  typeid(map<int,TGeoNode*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOTGeoNodemUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,TGeoNode*>) );
      instance.SetNew(&new_maplEintcOTGeoNodemUgR);
      instance.SetNewArray(&newArray_maplEintcOTGeoNodemUgR);
      instance.SetDelete(&delete_maplEintcOTGeoNodemUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOTGeoNodemUgR);
      instance.SetDestructor(&destruct_maplEintcOTGeoNodemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,TGeoNode*> >()));

      ::ROOT::AddClassAlternate("map<int,TGeoNode*>","std::map<int, TGeoNode*, std::less<int>, std::allocator<std::pair<int const, TGeoNode*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,TGeoNode*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOTGeoNodemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,TGeoNode*>*)nullptr)->GetClass();
      maplEintcOTGeoNodemUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOTGeoNodemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOTGeoNodemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TGeoNode*> : new map<int,TGeoNode*>;
   }
   static void *newArray_maplEintcOTGeoNodemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TGeoNode*>[nElements] : new map<int,TGeoNode*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOTGeoNodemUgR(void *p) {
      delete ((map<int,TGeoNode*>*)p);
   }
   static void deleteArray_maplEintcOTGeoNodemUgR(void *p) {
      delete [] ((map<int,TGeoNode*>*)p);
   }
   static void destruct_maplEintcOTGeoNodemUgR(void *p) {
      typedef map<int,TGeoNode*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,TGeoNode*>

namespace ROOT {
   static TClass *maplEintcOCbmTofCellmUgR_Dictionary();
   static void maplEintcOCbmTofCellmUgR_TClassManip(TClass*);
   static void *new_maplEintcOCbmTofCellmUgR(void *p = nullptr);
   static void *newArray_maplEintcOCbmTofCellmUgR(Long_t size, void *p);
   static void delete_maplEintcOCbmTofCellmUgR(void *p);
   static void deleteArray_maplEintcOCbmTofCellmUgR(void *p);
   static void destruct_maplEintcOCbmTofCellmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,CbmTofCell*>*)
   {
      map<int,CbmTofCell*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,CbmTofCell*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,CbmTofCell*>", -2, "map", 100,
                  typeid(map<int,CbmTofCell*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOCbmTofCellmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,CbmTofCell*>) );
      instance.SetNew(&new_maplEintcOCbmTofCellmUgR);
      instance.SetNewArray(&newArray_maplEintcOCbmTofCellmUgR);
      instance.SetDelete(&delete_maplEintcOCbmTofCellmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOCbmTofCellmUgR);
      instance.SetDestructor(&destruct_maplEintcOCbmTofCellmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,CbmTofCell*> >()));

      ::ROOT::AddClassAlternate("map<int,CbmTofCell*>","std::map<int, CbmTofCell*, std::less<int>, std::allocator<std::pair<int const, CbmTofCell*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,CbmTofCell*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOCbmTofCellmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,CbmTofCell*>*)nullptr)->GetClass();
      maplEintcOCbmTofCellmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOCbmTofCellmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOCbmTofCellmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmTofCell*> : new map<int,CbmTofCell*>;
   }
   static void *newArray_maplEintcOCbmTofCellmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,CbmTofCell*>[nElements] : new map<int,CbmTofCell*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOCbmTofCellmUgR(void *p) {
      delete ((map<int,CbmTofCell*>*)p);
   }
   static void deleteArray_maplEintcOCbmTofCellmUgR(void *p) {
      delete [] ((map<int,CbmTofCell*>*)p);
   }
   static void destruct_maplEintcOCbmTofCellmUgR(void *p) {
      typedef map<int,CbmTofCell*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,CbmTofCell*>

namespace {
  void TriggerDictionaryInitialization_libCbmTofBase_Impl() {
    static const char* headers[] = {
"CbmTofCell.h",
"CbmTofContFact.h",
"CbmTofCreateDigiPar.h",
"CbmTofDigiBdfPar.h",
"CbmTofDigiPar.h",
"CbmMcbm2018TofPar.h",
"CbmTofGeoHandler.h",
"CbmTofTrackingInterface.h",
"TTrbHeader.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/tof",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/tof",
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
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/core/detectors/tof/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmTofBase dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmTofCell.h")))  CbmTofCell;
class __attribute__((annotate(R"ATTRDUMP(Factory for all TOF parameter containers)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTofContFact.h")))  CbmTofContFact;
class __attribute__((annotate("$clingAutoload$CbmTofCreateDigiPar.h")))  CbmTofCreateDigiPar;
class __attribute__((annotate("$clingAutoload$CbmTofDigiBdfPar.h")))  CbmTofDigiBdfPar;
class __attribute__((annotate("$clingAutoload$CbmTofDigiPar.h")))  CbmTofDigiPar;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018TofPar.h")))  CbmMcbm2018TofPar;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018TofPar.h")))  CbmMcbm2018BmonPar;
class __attribute__((annotate("$clingAutoload$CbmTofGeoHandler.h")))  CbmTofGeoHandler;
class __attribute__((annotate("$clingAutoload$CbmTofTrackingInterface.h")))  CbmTofTrackingInterface;
class __attribute__((annotate("$clingAutoload$TTrbHeader.h")))  TTrbHeader;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmTofBase dictionary payload"

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
#include "CbmTofCell.h"
#include "CbmTofContFact.h"
#include "CbmTofCreateDigiPar.h"
#include "CbmTofDigiBdfPar.h"
#include "CbmTofDigiPar.h"
#include "CbmMcbm2018TofPar.h"
#include "CbmTofGeoHandler.h"
#include "CbmTofTrackingInterface.h"
#include "TTrbHeader.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmMcbm2018BmonPar", payloadCode, "@",
"CbmMcbm2018TofPar", payloadCode, "@",
"CbmTofCell", payloadCode, "@",
"CbmTofContFact", payloadCode, "@",
"CbmTofCreateDigiPar", payloadCode, "@",
"CbmTofDigiBdfPar", payloadCode, "@",
"CbmTofDigiPar", payloadCode, "@",
"CbmTofGeoHandler", payloadCode, "@",
"CbmTofTrackingInterface", payloadCode, "@",
"TTrbHeader", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmTofBase",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmTofBase_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmTofBase_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmTofBase() {
  TriggerDictionaryInitialization_libCbmTofBase_Impl();
}
