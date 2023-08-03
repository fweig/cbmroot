// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmBeamtimeCosy2019
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
#include "CbmCosy2019HodoPar.h"
#include "CbmCosy2019ContFact.h"
#include "CbmCosy2019UnpackerAlgoHodo.h"
#include "CbmCosy2019UnpackerTaskHodo.h"
#include "CbmStsCoincHodo.h"
#include "CbmCosy2019MonitorAlgoHodo.h"
#include "CbmCosy2019MonitorTaskHodo.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmCosy2019HodoPar(void *p = nullptr);
   static void *newArray_CbmCosy2019HodoPar(Long_t size, void *p);
   static void delete_CbmCosy2019HodoPar(void *p);
   static void deleteArray_CbmCosy2019HodoPar(void *p);
   static void destruct_CbmCosy2019HodoPar(void *p);
   static void streamer_CbmCosy2019HodoPar(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCosy2019HodoPar*)
   {
      ::CbmCosy2019HodoPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCosy2019HodoPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCosy2019HodoPar", ::CbmCosy2019HodoPar::Class_Version(), "CbmCosy2019HodoPar.h", 22,
                  typeid(::CbmCosy2019HodoPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCosy2019HodoPar::Dictionary, isa_proxy, 16,
                  sizeof(::CbmCosy2019HodoPar) );
      instance.SetNew(&new_CbmCosy2019HodoPar);
      instance.SetNewArray(&newArray_CbmCosy2019HodoPar);
      instance.SetDelete(&delete_CbmCosy2019HodoPar);
      instance.SetDeleteArray(&deleteArray_CbmCosy2019HodoPar);
      instance.SetDestructor(&destruct_CbmCosy2019HodoPar);
      instance.SetStreamerFunc(&streamer_CbmCosy2019HodoPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCosy2019HodoPar*)
   {
      return GenerateInitInstanceLocal((::CbmCosy2019HodoPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCosy2019HodoPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmCosy2019ContFact(void *p = nullptr);
   static void *newArray_CbmCosy2019ContFact(Long_t size, void *p);
   static void delete_CbmCosy2019ContFact(void *p);
   static void deleteArray_CbmCosy2019ContFact(void *p);
   static void destruct_CbmCosy2019ContFact(void *p);
   static void streamer_CbmCosy2019ContFact(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCosy2019ContFact*)
   {
      ::CbmCosy2019ContFact *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCosy2019ContFact >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCosy2019ContFact", ::CbmCosy2019ContFact::Class_Version(), "CbmCosy2019ContFact.h", 13,
                  typeid(::CbmCosy2019ContFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCosy2019ContFact::Dictionary, isa_proxy, 16,
                  sizeof(::CbmCosy2019ContFact) );
      instance.SetNew(&new_CbmCosy2019ContFact);
      instance.SetNewArray(&newArray_CbmCosy2019ContFact);
      instance.SetDelete(&delete_CbmCosy2019ContFact);
      instance.SetDeleteArray(&deleteArray_CbmCosy2019ContFact);
      instance.SetDestructor(&destruct_CbmCosy2019ContFact);
      instance.SetStreamerFunc(&streamer_CbmCosy2019ContFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCosy2019ContFact*)
   {
      return GenerateInitInstanceLocal((::CbmCosy2019ContFact*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCosy2019ContFact*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmCosy2019UnpackerAlgoHodo(void *p = nullptr);
   static void *newArray_CbmCosy2019UnpackerAlgoHodo(Long_t size, void *p);
   static void delete_CbmCosy2019UnpackerAlgoHodo(void *p);
   static void deleteArray_CbmCosy2019UnpackerAlgoHodo(void *p);
   static void destruct_CbmCosy2019UnpackerAlgoHodo(void *p);
   static void streamer_CbmCosy2019UnpackerAlgoHodo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCosy2019UnpackerAlgoHodo*)
   {
      ::CbmCosy2019UnpackerAlgoHodo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCosy2019UnpackerAlgoHodo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCosy2019UnpackerAlgoHodo", ::CbmCosy2019UnpackerAlgoHodo::Class_Version(), "CbmCosy2019UnpackerAlgoHodo.h", 40,
                  typeid(::CbmCosy2019UnpackerAlgoHodo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCosy2019UnpackerAlgoHodo::Dictionary, isa_proxy, 16,
                  sizeof(::CbmCosy2019UnpackerAlgoHodo) );
      instance.SetNew(&new_CbmCosy2019UnpackerAlgoHodo);
      instance.SetNewArray(&newArray_CbmCosy2019UnpackerAlgoHodo);
      instance.SetDelete(&delete_CbmCosy2019UnpackerAlgoHodo);
      instance.SetDeleteArray(&deleteArray_CbmCosy2019UnpackerAlgoHodo);
      instance.SetDestructor(&destruct_CbmCosy2019UnpackerAlgoHodo);
      instance.SetStreamerFunc(&streamer_CbmCosy2019UnpackerAlgoHodo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCosy2019UnpackerAlgoHodo*)
   {
      return GenerateInitInstanceLocal((::CbmCosy2019UnpackerAlgoHodo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCosy2019UnpackerAlgoHodo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmCosy2019UnpackerTaskHodo(void *p = nullptr);
   static void *newArray_CbmCosy2019UnpackerTaskHodo(Long_t size, void *p);
   static void delete_CbmCosy2019UnpackerTaskHodo(void *p);
   static void deleteArray_CbmCosy2019UnpackerTaskHodo(void *p);
   static void destruct_CbmCosy2019UnpackerTaskHodo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCosy2019UnpackerTaskHodo*)
   {
      ::CbmCosy2019UnpackerTaskHodo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCosy2019UnpackerTaskHodo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCosy2019UnpackerTaskHodo", ::CbmCosy2019UnpackerTaskHodo::Class_Version(), "CbmCosy2019UnpackerTaskHodo.h", 30,
                  typeid(::CbmCosy2019UnpackerTaskHodo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCosy2019UnpackerTaskHodo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmCosy2019UnpackerTaskHodo) );
      instance.SetNew(&new_CbmCosy2019UnpackerTaskHodo);
      instance.SetNewArray(&newArray_CbmCosy2019UnpackerTaskHodo);
      instance.SetDelete(&delete_CbmCosy2019UnpackerTaskHodo);
      instance.SetDeleteArray(&deleteArray_CbmCosy2019UnpackerTaskHodo);
      instance.SetDestructor(&destruct_CbmCosy2019UnpackerTaskHodo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCosy2019UnpackerTaskHodo*)
   {
      return GenerateInitInstanceLocal((::CbmCosy2019UnpackerTaskHodo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCosy2019UnpackerTaskHodo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsCoincHodo(void *p = nullptr);
   static void *newArray_CbmStsCoincHodo(Long_t size, void *p);
   static void delete_CbmStsCoincHodo(void *p);
   static void deleteArray_CbmStsCoincHodo(void *p);
   static void destruct_CbmStsCoincHodo(void *p);
   static void streamer_CbmStsCoincHodo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsCoincHodo*)
   {
      ::CbmStsCoincHodo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsCoincHodo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsCoincHodo", ::CbmStsCoincHodo::Class_Version(), "CbmStsCoincHodo.h", 19,
                  typeid(::CbmStsCoincHodo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsCoincHodo::Dictionary, isa_proxy, 16,
                  sizeof(::CbmStsCoincHodo) );
      instance.SetNew(&new_CbmStsCoincHodo);
      instance.SetNewArray(&newArray_CbmStsCoincHodo);
      instance.SetDelete(&delete_CbmStsCoincHodo);
      instance.SetDeleteArray(&deleteArray_CbmStsCoincHodo);
      instance.SetDestructor(&destruct_CbmStsCoincHodo);
      instance.SetStreamerFunc(&streamer_CbmStsCoincHodo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsCoincHodo*)
   {
      return GenerateInitInstanceLocal((::CbmStsCoincHodo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsCoincHodo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmCosy2019MonitorAlgoHodo(void *p = nullptr);
   static void *newArray_CbmCosy2019MonitorAlgoHodo(Long_t size, void *p);
   static void delete_CbmCosy2019MonitorAlgoHodo(void *p);
   static void deleteArray_CbmCosy2019MonitorAlgoHodo(void *p);
   static void destruct_CbmCosy2019MonitorAlgoHodo(void *p);
   static void streamer_CbmCosy2019MonitorAlgoHodo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCosy2019MonitorAlgoHodo*)
   {
      ::CbmCosy2019MonitorAlgoHodo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCosy2019MonitorAlgoHodo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCosy2019MonitorAlgoHodo", ::CbmCosy2019MonitorAlgoHodo::Class_Version(), "CbmCosy2019MonitorAlgoHodo.h", 39,
                  typeid(::CbmCosy2019MonitorAlgoHodo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCosy2019MonitorAlgoHodo::Dictionary, isa_proxy, 16,
                  sizeof(::CbmCosy2019MonitorAlgoHodo) );
      instance.SetNew(&new_CbmCosy2019MonitorAlgoHodo);
      instance.SetNewArray(&newArray_CbmCosy2019MonitorAlgoHodo);
      instance.SetDelete(&delete_CbmCosy2019MonitorAlgoHodo);
      instance.SetDeleteArray(&deleteArray_CbmCosy2019MonitorAlgoHodo);
      instance.SetDestructor(&destruct_CbmCosy2019MonitorAlgoHodo);
      instance.SetStreamerFunc(&streamer_CbmCosy2019MonitorAlgoHodo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCosy2019MonitorAlgoHodo*)
   {
      return GenerateInitInstanceLocal((::CbmCosy2019MonitorAlgoHodo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCosy2019MonitorAlgoHodo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmCosy2019MonitorTaskHodo(void *p = nullptr);
   static void *newArray_CbmCosy2019MonitorTaskHodo(Long_t size, void *p);
   static void delete_CbmCosy2019MonitorTaskHodo(void *p);
   static void deleteArray_CbmCosy2019MonitorTaskHodo(void *p);
   static void destruct_CbmCosy2019MonitorTaskHodo(void *p);
   static void streamer_CbmCosy2019MonitorTaskHodo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCosy2019MonitorTaskHodo*)
   {
      ::CbmCosy2019MonitorTaskHodo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCosy2019MonitorTaskHodo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCosy2019MonitorTaskHodo", ::CbmCosy2019MonitorTaskHodo::Class_Version(), "CbmCosy2019MonitorTaskHodo.h", 21,
                  typeid(::CbmCosy2019MonitorTaskHodo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCosy2019MonitorTaskHodo::Dictionary, isa_proxy, 16,
                  sizeof(::CbmCosy2019MonitorTaskHodo) );
      instance.SetNew(&new_CbmCosy2019MonitorTaskHodo);
      instance.SetNewArray(&newArray_CbmCosy2019MonitorTaskHodo);
      instance.SetDelete(&delete_CbmCosy2019MonitorTaskHodo);
      instance.SetDeleteArray(&deleteArray_CbmCosy2019MonitorTaskHodo);
      instance.SetDestructor(&destruct_CbmCosy2019MonitorTaskHodo);
      instance.SetStreamerFunc(&streamer_CbmCosy2019MonitorTaskHodo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCosy2019MonitorTaskHodo*)
   {
      return GenerateInitInstanceLocal((::CbmCosy2019MonitorTaskHodo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCosy2019MonitorTaskHodo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmCosy2019HodoPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCosy2019HodoPar::Class_Name()
{
   return "CbmCosy2019HodoPar";
}

//______________________________________________________________________________
const char *CbmCosy2019HodoPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019HodoPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCosy2019HodoPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019HodoPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCosy2019HodoPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019HodoPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCosy2019HodoPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019HodoPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCosy2019ContFact::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCosy2019ContFact::Class_Name()
{
   return "CbmCosy2019ContFact";
}

//______________________________________________________________________________
const char *CbmCosy2019ContFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019ContFact*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCosy2019ContFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019ContFact*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCosy2019ContFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019ContFact*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCosy2019ContFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019ContFact*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCosy2019UnpackerAlgoHodo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCosy2019UnpackerAlgoHodo::Class_Name()
{
   return "CbmCosy2019UnpackerAlgoHodo";
}

//______________________________________________________________________________
const char *CbmCosy2019UnpackerAlgoHodo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019UnpackerAlgoHodo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCosy2019UnpackerAlgoHodo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019UnpackerAlgoHodo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCosy2019UnpackerAlgoHodo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019UnpackerAlgoHodo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCosy2019UnpackerAlgoHodo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019UnpackerAlgoHodo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCosy2019UnpackerTaskHodo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCosy2019UnpackerTaskHodo::Class_Name()
{
   return "CbmCosy2019UnpackerTaskHodo";
}

//______________________________________________________________________________
const char *CbmCosy2019UnpackerTaskHodo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019UnpackerTaskHodo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCosy2019UnpackerTaskHodo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019UnpackerTaskHodo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCosy2019UnpackerTaskHodo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019UnpackerTaskHodo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCosy2019UnpackerTaskHodo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019UnpackerTaskHodo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsCoincHodo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsCoincHodo::Class_Name()
{
   return "CbmStsCoincHodo";
}

//______________________________________________________________________________
const char *CbmStsCoincHodo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsCoincHodo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsCoincHodo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsCoincHodo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsCoincHodo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsCoincHodo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsCoincHodo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsCoincHodo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCosy2019MonitorAlgoHodo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCosy2019MonitorAlgoHodo::Class_Name()
{
   return "CbmCosy2019MonitorAlgoHodo";
}

//______________________________________________________________________________
const char *CbmCosy2019MonitorAlgoHodo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019MonitorAlgoHodo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCosy2019MonitorAlgoHodo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019MonitorAlgoHodo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCosy2019MonitorAlgoHodo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019MonitorAlgoHodo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCosy2019MonitorAlgoHodo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019MonitorAlgoHodo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCosy2019MonitorTaskHodo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCosy2019MonitorTaskHodo::Class_Name()
{
   return "CbmCosy2019MonitorTaskHodo";
}

//______________________________________________________________________________
const char *CbmCosy2019MonitorTaskHodo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019MonitorTaskHodo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCosy2019MonitorTaskHodo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019MonitorTaskHodo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCosy2019MonitorTaskHodo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019MonitorTaskHodo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCosy2019MonitorTaskHodo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCosy2019MonitorTaskHodo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmCosy2019HodoPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCosy2019HodoPar.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairParGenericSet::Streamer(R__b);
      R__b >> fuNbModules;
      fiModAddress.Streamer(R__b);
      fdModCenterPosX.Streamer(R__b);
      fdModCenterPosY.Streamer(R__b);
      fiModSwapXY.Streamer(R__b);
      fiModInvertX.Streamer(R__b);
      fiModInvertY.Streamer(R__b);
      R__b >> fuNrOfDpbs;
      fiDbpIdArray.Streamer(R__b);
      fiCrobActiveFlag.Streamer(R__b);
      fiFebModuleIdx.Streamer(R__b);
      fdFebAdcGain.Streamer(R__b);
      fdFebAdcBase.Streamer(R__b);
      fdFebAdcThrGain.Streamer(R__b);
      fiFebAdcThrOffs.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, CbmCosy2019HodoPar::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmCosy2019HodoPar::IsA(), kTRUE);
      FairParGenericSet::Streamer(R__b);
      R__b << fuNbModules;
      fiModAddress.Streamer(R__b);
      fdModCenterPosX.Streamer(R__b);
      fdModCenterPosY.Streamer(R__b);
      fiModSwapXY.Streamer(R__b);
      fiModInvertX.Streamer(R__b);
      fiModInvertY.Streamer(R__b);
      R__b << fuNrOfDpbs;
      fiDbpIdArray.Streamer(R__b);
      fiCrobActiveFlag.Streamer(R__b);
      fiFebModuleIdx.Streamer(R__b);
      fdFebAdcGain.Streamer(R__b);
      fdFebAdcBase.Streamer(R__b);
      fdFebAdcThrGain.Streamer(R__b);
      fiFebAdcThrOffs.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmCosy2019HodoPar(void *p) {
      return  p ? new(p) ::CbmCosy2019HodoPar : new ::CbmCosy2019HodoPar;
   }
   static void *newArray_CbmCosy2019HodoPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmCosy2019HodoPar[nElements] : new ::CbmCosy2019HodoPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmCosy2019HodoPar(void *p) {
      delete ((::CbmCosy2019HodoPar*)p);
   }
   static void deleteArray_CbmCosy2019HodoPar(void *p) {
      delete [] ((::CbmCosy2019HodoPar*)p);
   }
   static void destruct_CbmCosy2019HodoPar(void *p) {
      typedef ::CbmCosy2019HodoPar current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmCosy2019HodoPar(TBuffer &buf, void *obj) {
      ((::CbmCosy2019HodoPar*)obj)->::CbmCosy2019HodoPar::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmCosy2019HodoPar

//______________________________________________________________________________
void CbmCosy2019ContFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCosy2019ContFact.

   FairContFact::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmCosy2019ContFact(void *p) {
      return  p ? new(p) ::CbmCosy2019ContFact : new ::CbmCosy2019ContFact;
   }
   static void *newArray_CbmCosy2019ContFact(Long_t nElements, void *p) {
      return p ? new(p) ::CbmCosy2019ContFact[nElements] : new ::CbmCosy2019ContFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmCosy2019ContFact(void *p) {
      delete ((::CbmCosy2019ContFact*)p);
   }
   static void deleteArray_CbmCosy2019ContFact(void *p) {
      delete [] ((::CbmCosy2019ContFact*)p);
   }
   static void destruct_CbmCosy2019ContFact(void *p) {
      typedef ::CbmCosy2019ContFact current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmCosy2019ContFact(TBuffer &buf, void *obj) {
      ((::CbmCosy2019ContFact*)obj)->::CbmCosy2019ContFact::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmCosy2019ContFact

//______________________________________________________________________________
void CbmCosy2019UnpackerAlgoHodo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCosy2019UnpackerAlgoHodo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fdTimeOffsetNs;
      {
         vector<Double_t> &R__stl =  fvdTimeOffsetNsAsics;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            double R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            stsxyter::MessType R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<stsxyter::MessType>(readtemp);
            unsigned int R__t2;
            R__b >> R__t2;
            typedef enum stsxyter::MessType Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fdStartTime;
      R__b >> fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b >> fhVectorSize;
      R__b >> fhVectorCapacity;
      R__b >> fuTsMaxVectorSize;
      R__b >> fdCapacityIncFactor;
      R__b.CheckByteCount(R__s, R__c, CbmCosy2019UnpackerAlgoHodo::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmCosy2019UnpackerAlgoHodo::IsA(), kTRUE);
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fdTimeOffsetNs;
      {
         vector<Double_t> &R__stl =  fvdTimeOffsetNsAsics;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Double_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<stsxyter::MessType,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fdStartTime;
      R__b << fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b << fhVectorSize;
      R__b << fhVectorCapacity;
      R__b << fuTsMaxVectorSize;
      R__b << fdCapacityIncFactor;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmCosy2019UnpackerAlgoHodo(void *p) {
      return  p ? new(p) ::CbmCosy2019UnpackerAlgoHodo : new ::CbmCosy2019UnpackerAlgoHodo;
   }
   static void *newArray_CbmCosy2019UnpackerAlgoHodo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmCosy2019UnpackerAlgoHodo[nElements] : new ::CbmCosy2019UnpackerAlgoHodo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmCosy2019UnpackerAlgoHodo(void *p) {
      delete ((::CbmCosy2019UnpackerAlgoHodo*)p);
   }
   static void deleteArray_CbmCosy2019UnpackerAlgoHodo(void *p) {
      delete [] ((::CbmCosy2019UnpackerAlgoHodo*)p);
   }
   static void destruct_CbmCosy2019UnpackerAlgoHodo(void *p) {
      typedef ::CbmCosy2019UnpackerAlgoHodo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmCosy2019UnpackerAlgoHodo(TBuffer &buf, void *obj) {
      ((::CbmCosy2019UnpackerAlgoHodo*)obj)->::CbmCosy2019UnpackerAlgoHodo::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmCosy2019UnpackerAlgoHodo

//______________________________________________________________________________
void CbmCosy2019UnpackerTaskHodo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCosy2019UnpackerTaskHodo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmCosy2019UnpackerTaskHodo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmCosy2019UnpackerTaskHodo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmCosy2019UnpackerTaskHodo(void *p) {
      return  p ? new(p) ::CbmCosy2019UnpackerTaskHodo : new ::CbmCosy2019UnpackerTaskHodo;
   }
   static void *newArray_CbmCosy2019UnpackerTaskHodo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmCosy2019UnpackerTaskHodo[nElements] : new ::CbmCosy2019UnpackerTaskHodo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmCosy2019UnpackerTaskHodo(void *p) {
      delete ((::CbmCosy2019UnpackerTaskHodo*)p);
   }
   static void deleteArray_CbmCosy2019UnpackerTaskHodo(void *p) {
      delete [] ((::CbmCosy2019UnpackerTaskHodo*)p);
   }
   static void destruct_CbmCosy2019UnpackerTaskHodo(void *p) {
      typedef ::CbmCosy2019UnpackerTaskHodo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmCosy2019UnpackerTaskHodo

//______________________________________________________________________________
void CbmStsCoincHodo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsCoincHodo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      arrayClusters->Streamer(R__b);
      arrayHits->Streamer(R__b);
      R__b >> dPosZHodoA;
      R__b >> dPosZSts;
      R__b >> dPosZHodoB;
      R__b >> dHodoDistZ;
      R__b >> dStsDistZ;
      R__b >> dMidStsHodoA;
      R__b >> dMidStsHodoB;
      R__b >> iCoincLimitClk;
      R__b >> dClockCycle;
      R__b >> dCoincLimit;
      R__b >> fNbTs;
      R__b >> fNrOfStsDigis;
      R__b >> TsStart;
      R__b >> TsStop;
      R__b >> phHitsStsTime;
      R__b >> phHitsHodoATime;
      R__b >> phHitsHodoBTime;
      R__b >> phHitsPositionHodoA;
      R__b >> phHitsPositionSts;
      R__b >> phHitsPositionHodoB;
      R__b >> phNbHitsCompHodo;
      R__b >> phNbHitsCompStsHodoA;
      R__b >> phNbHitsCompStsHodoB;
      R__b >> phHitsCoincCorrXX;
      R__b >> phHitsCoincCorrYY;
      R__b >> phHitsCoincCorrXY;
      R__b >> phHitsCoincCorrYX;
      R__b >> phHitsPositionCoincA;
      R__b >> phHitsPositionCoincB;
      R__b >> phHitsPositionDiff;
      R__b >> phHitsTimeDiff;
      R__b >> phHitsCoincDist;
      R__b >> phHitsCoincAngle;
      R__b >> phHitsSingleCoincCorrXX;
      R__b >> phHitsSingleCoincCorrYY;
      R__b >> phHitsSingleCoincCorrXY;
      R__b >> phHitsSingleCoincCorrYX;
      R__b >> phHitsSinglePositionCoincA;
      R__b >> phHitsSinglePositionCoincB;
      R__b >> phHitsSinglePositionDiff;
      R__b >> phHitsSingleTimeDiff;
      R__b >> phHitsSingleCoincDist;
      R__b >> phHitsSingleCoincAngle;
      R__b >> phHitsBestCoincCorrXX;
      R__b >> phHitsBestCoincCorrYY;
      R__b >> phHitsBestCoincCorrXY;
      R__b >> phHitsBestCoincCorrYX;
      R__b >> phHitsBestPositionCoincA;
      R__b >> phHitsBestPositionCoincB;
      R__b >> phHitsBestPositionDiff;
      R__b >> phHitsBestTimeDiff;
      R__b >> phHitsBestCoincDist;
      R__b >> phHitsBestCoincAngle;
      R__b >> phHitsPositionCoincExtr;
      R__b >> phHitsStsCoincCorrXX;
      R__b >> phHitsStsCoincCorrYY;
      R__b >> phHitsStsCoincCorrXY;
      R__b >> phHitsStsCoincCorrYX;
      R__b >> phHitsStsPositionCoincExtr;
      R__b >> phHitsStsPositionCoinc;
      R__b >> phHitsStsTimeDiff;
      R__b >> phHitsStsPositionDiff;
      R__b >> phHitsStsPositionDiffInv;
      R__b >> phHitsStsBestCoincCorrXX;
      R__b >> phHitsStsBestCoincCorrYY;
      R__b >> phHitsStsBestCoincCorrXY;
      R__b >> phHitsStsBestCoincCorrYX;
      R__b >> phHitsStsBestPositionCoincExtr;
      R__b >> phHitsStsBestPositionCoinc;
      R__b >> phHitsStsBestPositionShiftCoinc;
      R__b >> phHitsStsBestTimeDiff;
      R__b >> phHitsStsBestPositionDiff;
      R__b >> phHitsStsBestPositionDiffInv;
      R__b >> phHitsStsBestDiff;
      R__b >> phHitsStsEff;
      fOutFileName.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, CbmStsCoincHodo::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmStsCoincHodo::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      arrayClusters->Streamer(R__b);
      arrayHits->Streamer(R__b);
      R__b << dPosZHodoA;
      R__b << dPosZSts;
      R__b << dPosZHodoB;
      R__b << dHodoDistZ;
      R__b << dStsDistZ;
      R__b << dMidStsHodoA;
      R__b << dMidStsHodoB;
      R__b << iCoincLimitClk;
      R__b << dClockCycle;
      R__b << dCoincLimit;
      R__b << fNbTs;
      R__b << fNrOfStsDigis;
      R__b << TsStart;
      R__b << TsStop;
      R__b << phHitsStsTime;
      R__b << phHitsHodoATime;
      R__b << phHitsHodoBTime;
      R__b << phHitsPositionHodoA;
      R__b << phHitsPositionSts;
      R__b << phHitsPositionHodoB;
      R__b << phNbHitsCompHodo;
      R__b << phNbHitsCompStsHodoA;
      R__b << phNbHitsCompStsHodoB;
      R__b << phHitsCoincCorrXX;
      R__b << phHitsCoincCorrYY;
      R__b << phHitsCoincCorrXY;
      R__b << phHitsCoincCorrYX;
      R__b << phHitsPositionCoincA;
      R__b << phHitsPositionCoincB;
      R__b << phHitsPositionDiff;
      R__b << phHitsTimeDiff;
      R__b << phHitsCoincDist;
      R__b << phHitsCoincAngle;
      R__b << phHitsSingleCoincCorrXX;
      R__b << phHitsSingleCoincCorrYY;
      R__b << phHitsSingleCoincCorrXY;
      R__b << phHitsSingleCoincCorrYX;
      R__b << phHitsSinglePositionCoincA;
      R__b << phHitsSinglePositionCoincB;
      R__b << phHitsSinglePositionDiff;
      R__b << phHitsSingleTimeDiff;
      R__b << phHitsSingleCoincDist;
      R__b << phHitsSingleCoincAngle;
      R__b << phHitsBestCoincCorrXX;
      R__b << phHitsBestCoincCorrYY;
      R__b << phHitsBestCoincCorrXY;
      R__b << phHitsBestCoincCorrYX;
      R__b << phHitsBestPositionCoincA;
      R__b << phHitsBestPositionCoincB;
      R__b << phHitsBestPositionDiff;
      R__b << phHitsBestTimeDiff;
      R__b << phHitsBestCoincDist;
      R__b << phHitsBestCoincAngle;
      R__b << phHitsPositionCoincExtr;
      R__b << phHitsStsCoincCorrXX;
      R__b << phHitsStsCoincCorrYY;
      R__b << phHitsStsCoincCorrXY;
      R__b << phHitsStsCoincCorrYX;
      R__b << phHitsStsPositionCoincExtr;
      R__b << phHitsStsPositionCoinc;
      R__b << phHitsStsTimeDiff;
      R__b << phHitsStsPositionDiff;
      R__b << phHitsStsPositionDiffInv;
      R__b << phHitsStsBestCoincCorrXX;
      R__b << phHitsStsBestCoincCorrYY;
      R__b << phHitsStsBestCoincCorrXY;
      R__b << phHitsStsBestCoincCorrYX;
      R__b << phHitsStsBestPositionCoincExtr;
      R__b << phHitsStsBestPositionCoinc;
      R__b << phHitsStsBestPositionShiftCoinc;
      R__b << phHitsStsBestTimeDiff;
      R__b << phHitsStsBestPositionDiff;
      R__b << phHitsStsBestPositionDiffInv;
      R__b << phHitsStsBestDiff;
      R__b << phHitsStsEff;
      fOutFileName.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsCoincHodo(void *p) {
      return  p ? new(p) ::CbmStsCoincHodo : new ::CbmStsCoincHodo;
   }
   static void *newArray_CbmStsCoincHodo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsCoincHodo[nElements] : new ::CbmStsCoincHodo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsCoincHodo(void *p) {
      delete ((::CbmStsCoincHodo*)p);
   }
   static void deleteArray_CbmStsCoincHodo(void *p) {
      delete [] ((::CbmStsCoincHodo*)p);
   }
   static void destruct_CbmStsCoincHodo(void *p) {
      typedef ::CbmStsCoincHodo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmStsCoincHodo(TBuffer &buf, void *obj) {
      ((::CbmStsCoincHodo*)obj)->::CbmStsCoincHodo::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmStsCoincHodo

//______________________________________________________________________________
void CbmCosy2019MonitorAlgoHodo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCosy2019MonitorAlgoHodo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fuDpbId;
      {
         vector<UInt_t> &R__stl =  fvuElinkIdxHodo;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Bool_t> &R__stl =  fvbHodoSwapXY;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Bool_t> &R__stl =  fvbHodoInvertX;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Bool_t> &R__stl =  fvbHodoInvertY;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            bool R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuAdcGainHodo;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuAdcOffsHodo;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fdTimeOffsetNs;
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            stsxyter::MessType R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<stsxyter::MessType>(readtemp);
            unsigned int R__t2;
            R__b >> R__t2;
            typedef enum stsxyter::MessType Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fdStartTime;
      R__b >> fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b >> fhHodoMessType;
      R__b >> fhHodoStatusMessType;
      R__b >> fhHodoMsStatusFieldType;
      R__b >> fhHodoMessTypePerElink;
      R__b >> fdTimeCoincLimit;
      R__b >> fdTimeCoincLimitNs;
      R__b >> fhPrevHitDtAllAsics;
      R__b >> fhPrevHitDtAsicA;
      R__b >> fhPrevHitDtAsicB;
      R__b >> fhPrevHitDtAsicsAB;
      R__b >> fiTimeIntervalRateUpdate;
      R__b >> fuPreviousHitAsic;
      R__b >> fcSummary;
      {
         vector<TCanvas*> &R__stl =  fcHodoSummaryRaw;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TCanvas *));
         if (R__tcl1==0) {
            Error("fcHodoSummaryRaw streamer","Missing the TClass object for class TCanvas *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TCanvas* R__t;
            R__t = (TCanvas*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<TCanvas*> &R__stl =  fcHodoSummaryFiber;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class TCanvas *));
         if (R__tcl1==0) {
            Error("fcHodoSummaryFiber streamer","Missing the TClass object for class TCanvas *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            TCanvas* R__t;
            R__t = (TCanvas*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fcHodoFiberCoinc;
      R__b >> fcHodoFiberCoincAB;
      R__b >> fcHodoFullCoinc;
      R__b >> fcHodoFullCoincPos;
      R__b >> fcHodoFullCoincPosEvo;
      R__b >> fcHodoPrevHitDt;
      R__b.CheckByteCount(R__s, R__c, CbmCosy2019MonitorAlgoHodo::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmCosy2019MonitorAlgoHodo::IsA(), kTRUE);
      {
         vector<Bool_t> &R__stl =  fvbMaskedComponents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fuDpbId;
      {
         vector<UInt_t> &R__stl =  fvuElinkIdxHodo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Bool_t> &R__stl =  fvbHodoSwapXY;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Bool_t> &R__stl =  fvbHodoInvertX;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Bool_t> &R__stl =  fvbHodoInvertY;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Bool_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuAdcGainHodo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuAdcOffsHodo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fdTimeOffsetNs;
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      {
         map<stsxyter::MessType,UInt_t> &R__stl =  fmMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<stsxyter::MessType,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fdStartTime;
      R__b << fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      R__b << fhHodoMessType;
      R__b << fhHodoStatusMessType;
      R__b << fhHodoMsStatusFieldType;
      R__b << fhHodoMessTypePerElink;
      R__b << fdTimeCoincLimit;
      R__b << fdTimeCoincLimitNs;
      R__b << fhPrevHitDtAllAsics;
      R__b << fhPrevHitDtAsicA;
      R__b << fhPrevHitDtAsicB;
      R__b << fhPrevHitDtAsicsAB;
      R__b << fiTimeIntervalRateUpdate;
      R__b << fuPreviousHitAsic;
      R__b << fcSummary;
      {
         vector<TCanvas*> &R__stl =  fcHodoSummaryRaw;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TCanvas*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<TCanvas*> &R__stl =  fcHodoSummaryFiber;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<TCanvas*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fcHodoFiberCoinc;
      R__b << fcHodoFiberCoincAB;
      R__b << fcHodoFullCoinc;
      R__b << fcHodoFullCoincPos;
      R__b << fcHodoFullCoincPosEvo;
      R__b << fcHodoPrevHitDt;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmCosy2019MonitorAlgoHodo(void *p) {
      return  p ? new(p) ::CbmCosy2019MonitorAlgoHodo : new ::CbmCosy2019MonitorAlgoHodo;
   }
   static void *newArray_CbmCosy2019MonitorAlgoHodo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmCosy2019MonitorAlgoHodo[nElements] : new ::CbmCosy2019MonitorAlgoHodo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmCosy2019MonitorAlgoHodo(void *p) {
      delete ((::CbmCosy2019MonitorAlgoHodo*)p);
   }
   static void deleteArray_CbmCosy2019MonitorAlgoHodo(void *p) {
      delete [] ((::CbmCosy2019MonitorAlgoHodo*)p);
   }
   static void destruct_CbmCosy2019MonitorAlgoHodo(void *p) {
      typedef ::CbmCosy2019MonitorAlgoHodo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmCosy2019MonitorAlgoHodo(TBuffer &buf, void *obj) {
      ((::CbmCosy2019MonitorAlgoHodo*)obj)->::CbmCosy2019MonitorAlgoHodo::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmCosy2019MonitorAlgoHodo

//______________________________________________________________________________
void CbmCosy2019MonitorTaskHodo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCosy2019MonitorTaskHodo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fulTsCounter;
      R__b >> fMonitorAlgo;
      R__b.CheckByteCount(R__s, R__c, CbmCosy2019MonitorTaskHodo::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmCosy2019MonitorTaskHodo::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fulTsCounter;
      R__b << fMonitorAlgo;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmCosy2019MonitorTaskHodo(void *p) {
      return  p ? new(p) ::CbmCosy2019MonitorTaskHodo : new ::CbmCosy2019MonitorTaskHodo;
   }
   static void *newArray_CbmCosy2019MonitorTaskHodo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmCosy2019MonitorTaskHodo[nElements] : new ::CbmCosy2019MonitorTaskHodo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmCosy2019MonitorTaskHodo(void *p) {
      delete ((::CbmCosy2019MonitorTaskHodo*)p);
   }
   static void deleteArray_CbmCosy2019MonitorTaskHodo(void *p) {
      delete [] ((::CbmCosy2019MonitorTaskHodo*)p);
   }
   static void destruct_CbmCosy2019MonitorTaskHodo(void *p) {
      typedef ::CbmCosy2019MonitorTaskHodo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmCosy2019MonitorTaskHodo(TBuffer &buf, void *obj) {
      ((::CbmCosy2019MonitorTaskHodo*)obj)->::CbmCosy2019MonitorTaskHodo::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmCosy2019MonitorTaskHodo

namespace ROOT {
   static TClass *vectorlEunsignedsPintgR_Dictionary();
   static void vectorlEunsignedsPintgR_TClassManip(TClass*);
   static void *new_vectorlEunsignedsPintgR(void *p = nullptr);
   static void *newArray_vectorlEunsignedsPintgR(Long_t size, void *p);
   static void delete_vectorlEunsignedsPintgR(void *p);
   static void deleteArray_vectorlEunsignedsPintgR(void *p);
   static void destruct_vectorlEunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned int>*)
   {
      vector<unsigned int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<unsigned int>", -2, "vector", 389,
                  typeid(vector<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<unsigned int>) );
      instance.SetNew(&new_vectorlEunsignedsPintgR);
      instance.SetNewArray(&newArray_vectorlEunsignedsPintgR);
      instance.SetDelete(&delete_vectorlEunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPintgR);
      instance.SetDestructor(&destruct_vectorlEunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned int> >()));

      ::ROOT::AddClassAlternate("vector<unsigned int>","std::vector<unsigned int, std::allocator<unsigned int> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<unsigned int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<unsigned int>*)nullptr)->GetClass();
      vectorlEunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned int> : new vector<unsigned int>;
   }
   static void *newArray_vectorlEunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned int>[nElements] : new vector<unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEunsignedsPintgR(void *p) {
      delete ((vector<unsigned int>*)p);
   }
   static void deleteArray_vectorlEunsignedsPintgR(void *p) {
      delete [] ((vector<unsigned int>*)p);
   }
   static void destruct_vectorlEunsignedsPintgR(void *p) {
      typedef vector<unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<unsigned int>

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
   static TClass *vectorlETCanvasmUgR_Dictionary();
   static void vectorlETCanvasmUgR_TClassManip(TClass*);
   static void *new_vectorlETCanvasmUgR(void *p = nullptr);
   static void *newArray_vectorlETCanvasmUgR(Long_t size, void *p);
   static void delete_vectorlETCanvasmUgR(void *p);
   static void deleteArray_vectorlETCanvasmUgR(void *p);
   static void destruct_vectorlETCanvasmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TCanvas*>*)
   {
      vector<TCanvas*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TCanvas*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TCanvas*>", -2, "vector", 389,
                  typeid(vector<TCanvas*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETCanvasmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TCanvas*>) );
      instance.SetNew(&new_vectorlETCanvasmUgR);
      instance.SetNewArray(&newArray_vectorlETCanvasmUgR);
      instance.SetDelete(&delete_vectorlETCanvasmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETCanvasmUgR);
      instance.SetDestructor(&destruct_vectorlETCanvasmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TCanvas*> >()));

      ::ROOT::AddClassAlternate("vector<TCanvas*>","std::vector<TCanvas*, std::allocator<TCanvas*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TCanvas*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETCanvasmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TCanvas*>*)nullptr)->GetClass();
      vectorlETCanvasmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETCanvasmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETCanvasmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TCanvas*> : new vector<TCanvas*>;
   }
   static void *newArray_vectorlETCanvasmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TCanvas*>[nElements] : new vector<TCanvas*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETCanvasmUgR(void *p) {
      delete ((vector<TCanvas*>*)p);
   }
   static void deleteArray_vectorlETCanvasmUgR(void *p) {
      delete [] ((vector<TCanvas*>*)p);
   }
   static void destruct_vectorlETCanvasmUgR(void *p) {
      typedef vector<TCanvas*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TCanvas*>

namespace ROOT {
   static TClass *vectorlEFebChanMaskStsgR_Dictionary();
   static void vectorlEFebChanMaskStsgR_TClassManip(TClass*);
   static void *new_vectorlEFebChanMaskStsgR(void *p = nullptr);
   static void *newArray_vectorlEFebChanMaskStsgR(Long_t size, void *p);
   static void delete_vectorlEFebChanMaskStsgR(void *p);
   static void deleteArray_vectorlEFebChanMaskStsgR(void *p);
   static void destruct_vectorlEFebChanMaskStsgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<FebChanMaskSts>*)
   {
      vector<FebChanMaskSts> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<FebChanMaskSts>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<FebChanMaskSts>", -2, "vector", 389,
                  typeid(vector<FebChanMaskSts>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEFebChanMaskStsgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<FebChanMaskSts>) );
      instance.SetNew(&new_vectorlEFebChanMaskStsgR);
      instance.SetNewArray(&newArray_vectorlEFebChanMaskStsgR);
      instance.SetDelete(&delete_vectorlEFebChanMaskStsgR);
      instance.SetDeleteArray(&deleteArray_vectorlEFebChanMaskStsgR);
      instance.SetDestructor(&destruct_vectorlEFebChanMaskStsgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<FebChanMaskSts> >()));

      ::ROOT::AddClassAlternate("vector<FebChanMaskSts>","std::vector<FebChanMaskSts, std::allocator<FebChanMaskSts> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<FebChanMaskSts>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEFebChanMaskStsgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<FebChanMaskSts>*)nullptr)->GetClass();
      vectorlEFebChanMaskStsgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEFebChanMaskStsgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEFebChanMaskStsgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<FebChanMaskSts> : new vector<FebChanMaskSts>;
   }
   static void *newArray_vectorlEFebChanMaskStsgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<FebChanMaskSts>[nElements] : new vector<FebChanMaskSts>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEFebChanMaskStsgR(void *p) {
      delete ((vector<FebChanMaskSts>*)p);
   }
   static void deleteArray_vectorlEFebChanMaskStsgR(void *p) {
      delete [] ((vector<FebChanMaskSts>*)p);
   }
   static void destruct_vectorlEFebChanMaskStsgR(void *p) {
      typedef vector<FebChanMaskSts> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<FebChanMaskSts>

namespace ROOT {
   static TClass *vectorlECbmStsDigigR_Dictionary();
   static void vectorlECbmStsDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmStsDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmStsDigigR(Long_t size, void *p);
   static void delete_vectorlECbmStsDigigR(void *p);
   static void deleteArray_vectorlECbmStsDigigR(void *p);
   static void destruct_vectorlECbmStsDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsDigi>*)
   {
      vector<CbmStsDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsDigi>", -2, "vector", 389,
                  typeid(vector<CbmStsDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsDigigR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmStsDigi>) );
      instance.SetNew(&new_vectorlECbmStsDigigR);
      instance.SetNewArray(&newArray_vectorlECbmStsDigigR);
      instance.SetDelete(&delete_vectorlECbmStsDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsDigigR);
      instance.SetDestructor(&destruct_vectorlECbmStsDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsDigi>","std::vector<CbmStsDigi, std::allocator<CbmStsDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsDigi>*)nullptr)->GetClass();
      vectorlECbmStsDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsDigi> : new vector<CbmStsDigi>;
   }
   static void *newArray_vectorlECbmStsDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsDigi>[nElements] : new vector<CbmStsDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsDigigR(void *p) {
      delete ((vector<CbmStsDigi>*)p);
   }
   static void deleteArray_vectorlECbmStsDigigR(void *p) {
      delete [] ((vector<CbmStsDigi>*)p);
   }
   static void destruct_vectorlECbmStsDigigR(void *p) {
      typedef vector<CbmStsDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsDigi>

namespace ROOT {
   static TClass *vectorlECbmErrorMessagegR_Dictionary();
   static void vectorlECbmErrorMessagegR_TClassManip(TClass*);
   static void *new_vectorlECbmErrorMessagegR(void *p = nullptr);
   static void *newArray_vectorlECbmErrorMessagegR(Long_t size, void *p);
   static void delete_vectorlECbmErrorMessagegR(void *p);
   static void deleteArray_vectorlECbmErrorMessagegR(void *p);
   static void destruct_vectorlECbmErrorMessagegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmErrorMessage>*)
   {
      vector<CbmErrorMessage> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmErrorMessage>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmErrorMessage>", -2, "vector", 389,
                  typeid(vector<CbmErrorMessage>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmErrorMessagegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmErrorMessage>) );
      instance.SetNew(&new_vectorlECbmErrorMessagegR);
      instance.SetNewArray(&newArray_vectorlECbmErrorMessagegR);
      instance.SetDelete(&delete_vectorlECbmErrorMessagegR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmErrorMessagegR);
      instance.SetDestructor(&destruct_vectorlECbmErrorMessagegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmErrorMessage> >()));

      ::ROOT::AddClassAlternate("vector<CbmErrorMessage>","std::vector<CbmErrorMessage, std::allocator<CbmErrorMessage> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmErrorMessage>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmErrorMessagegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmErrorMessage>*)nullptr)->GetClass();
      vectorlECbmErrorMessagegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmErrorMessagegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmErrorMessagegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmErrorMessage> : new vector<CbmErrorMessage>;
   }
   static void *newArray_vectorlECbmErrorMessagegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmErrorMessage>[nElements] : new vector<CbmErrorMessage>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmErrorMessagegR(void *p) {
      delete ((vector<CbmErrorMessage>*)p);
   }
   static void deleteArray_vectorlECbmErrorMessagegR(void *p) {
      delete [] ((vector<CbmErrorMessage>*)p);
   }
   static void destruct_vectorlECbmErrorMessagegR(void *p) {
      typedef vector<CbmErrorMessage> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmErrorMessage>

namespace ROOT {
   static TClass *maplEstsxytercLcLMessTypecOunsignedsPintgR_Dictionary();
   static void maplEstsxytercLcLMessTypecOunsignedsPintgR_TClassManip(TClass*);
   static void *new_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p = nullptr);
   static void *newArray_maplEstsxytercLcLMessTypecOunsignedsPintgR(Long_t size, void *p);
   static void delete_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p);
   static void deleteArray_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p);
   static void destruct_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<stsxyter::MessType,unsigned int>*)
   {
      map<stsxyter::MessType,unsigned int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<stsxyter::MessType,unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<stsxyter::MessType,unsigned int>", -2, "map", 100,
                  typeid(map<stsxyter::MessType,unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstsxytercLcLMessTypecOunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<stsxyter::MessType,unsigned int>) );
      instance.SetNew(&new_maplEstsxytercLcLMessTypecOunsignedsPintgR);
      instance.SetNewArray(&newArray_maplEstsxytercLcLMessTypecOunsignedsPintgR);
      instance.SetDelete(&delete_maplEstsxytercLcLMessTypecOunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_maplEstsxytercLcLMessTypecOunsignedsPintgR);
      instance.SetDestructor(&destruct_maplEstsxytercLcLMessTypecOunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<stsxyter::MessType,unsigned int> >()));

      ::ROOT::AddClassAlternate("map<stsxyter::MessType,unsigned int>","std::map<stsxyter::MessType, unsigned int, std::less<stsxyter::MessType>, std::allocator<std::pair<stsxyter::MessType const, unsigned int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<stsxyter::MessType,unsigned int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstsxytercLcLMessTypecOunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<stsxyter::MessType,unsigned int>*)nullptr)->GetClass();
      maplEstsxytercLcLMessTypecOunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstsxytercLcLMessTypecOunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<stsxyter::MessType,unsigned int> : new map<stsxyter::MessType,unsigned int>;
   }
   static void *newArray_maplEstsxytercLcLMessTypecOunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<stsxyter::MessType,unsigned int>[nElements] : new map<stsxyter::MessType,unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p) {
      delete ((map<stsxyter::MessType,unsigned int>*)p);
   }
   static void deleteArray_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p) {
      delete [] ((map<stsxyter::MessType,unsigned int>*)p);
   }
   static void destruct_maplEstsxytercLcLMessTypecOunsignedsPintgR(void *p) {
      typedef map<stsxyter::MessType,unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<stsxyter::MessType,unsigned int>

namespace {
  void TriggerDictionaryInitialization_libCbmBeamtimeCosy2019_Impl() {
    static const char* headers[] = {
"CbmCosy2019HodoPar.h",
"CbmCosy2019ContFact.h",
"CbmCosy2019UnpackerAlgoHodo.h",
"CbmCosy2019UnpackerTaskHodo.h",
"CbmStsCoincHodo.h",
"CbmCosy2019MonitorAlgoHodo.h",
"CbmCosy2019MonitorTaskHodo.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/fles/cosy2019",
"/home/heinemann/gpuunpacker/cbmroot/fles/cosy2019/parameter",
"/home/heinemann/gpuunpacker/cbmroot/fles/cosy2019/unpacker",
"/home/heinemann/gpuunpacker/cbmroot/fles/cosy2019/monitor",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/fles/cosy2019",
"/home/heinemann/gpuunpacker/cbmroot/fles/cosy2019/parameter",
"/home/heinemann/gpuunpacker/cbmroot/fles/cosy2019/unpacker",
"/home/heinemann/gpuunpacker/cbmroot/fles/cosy2019/monitor",
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
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/dataformat",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/commonMQ",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/psd",
"/home/heinemann/gpuunpacker/cbmroot/external/flib_dpb/flib_dpb",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils/flestools",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/fles/cosy2019/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmBeamtimeCosy2019 dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmCosy2019HodoPar.h")))  CbmCosy2019HodoPar;
class __attribute__((annotate(R"ATTRDUMP(Factory for all mcbm parameter containers)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmCosy2019ContFact.h")))  CbmCosy2019ContFact;
class __attribute__((annotate("$clingAutoload$CbmCosy2019UnpackerAlgoHodo.h")))  CbmCosy2019UnpackerAlgoHodo;
class __attribute__((annotate("$clingAutoload$CbmCosy2019UnpackerTaskHodo.h")))  CbmCosy2019UnpackerTaskHodo;
class __attribute__((annotate("$clingAutoload$CbmStsCoincHodo.h")))  CbmStsCoincHodo;
class __attribute__((annotate("$clingAutoload$CbmCosy2019MonitorAlgoHodo.h")))  CbmCosy2019MonitorAlgoHodo;
class __attribute__((annotate("$clingAutoload$CbmCosy2019MonitorTaskHodo.h")))  CbmCosy2019MonitorTaskHodo;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmBeamtimeCosy2019 dictionary payload"

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
#include "CbmCosy2019HodoPar.h"
#include "CbmCosy2019ContFact.h"
#include "CbmCosy2019UnpackerAlgoHodo.h"
#include "CbmCosy2019UnpackerTaskHodo.h"
#include "CbmStsCoincHodo.h"
#include "CbmCosy2019MonitorAlgoHodo.h"
#include "CbmCosy2019MonitorTaskHodo.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmCosy2019ContFact", payloadCode, "@",
"CbmCosy2019HodoPar", payloadCode, "@",
"CbmCosy2019MonitorAlgoHodo", payloadCode, "@",
"CbmCosy2019MonitorTaskHodo", payloadCode, "@",
"CbmCosy2019UnpackerAlgoHodo", payloadCode, "@",
"CbmCosy2019UnpackerTaskHodo", payloadCode, "@",
"CbmStsCoincHodo", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmBeamtimeCosy2019",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmBeamtimeCosy2019_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmBeamtimeCosy2019_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmBeamtimeCosy2019() {
  TriggerDictionaryInitialization_libCbmBeamtimeCosy2019_Impl();
}
