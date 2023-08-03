// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmRichBase
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
#include "CbmRichPmt.h"
#include "CbmRichEventDisplay.h"
#include "CbmRichContFact.h"
#include "CbmMcbm2018RichPar.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmRichPmt(void *p = nullptr);
   static void *newArray_CbmRichPmt(Long_t size, void *p);
   static void delete_CbmRichPmt(void *p);
   static void deleteArray_CbmRichPmt(void *p);
   static void destruct_CbmRichPmt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichPmt*)
   {
      ::CbmRichPmt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichPmt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichPmt", ::CbmRichPmt::Class_Version(), "CbmRichPmt.h", 37,
                  typeid(::CbmRichPmt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichPmt::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichPmt) );
      instance.SetNew(&new_CbmRichPmt);
      instance.SetNewArray(&newArray_CbmRichPmt);
      instance.SetDelete(&delete_CbmRichPmt);
      instance.SetDeleteArray(&deleteArray_CbmRichPmt);
      instance.SetDestructor(&destruct_CbmRichPmt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichPmt*)
   {
      return GenerateInitInstanceLocal((::CbmRichPmt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichPmt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichEventDisplay(void *p = nullptr);
   static void *newArray_CbmRichEventDisplay(Long_t size, void *p);
   static void delete_CbmRichEventDisplay(void *p);
   static void deleteArray_CbmRichEventDisplay(void *p);
   static void destruct_CbmRichEventDisplay(void *p);
   static void streamer_CbmRichEventDisplay(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichEventDisplay*)
   {
      ::CbmRichEventDisplay *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichEventDisplay >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichEventDisplay", ::CbmRichEventDisplay::Class_Version(), "CbmRichEventDisplay.h", 39,
                  typeid(::CbmRichEventDisplay), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichEventDisplay::Dictionary, isa_proxy, 16,
                  sizeof(::CbmRichEventDisplay) );
      instance.SetNew(&new_CbmRichEventDisplay);
      instance.SetNewArray(&newArray_CbmRichEventDisplay);
      instance.SetDelete(&delete_CbmRichEventDisplay);
      instance.SetDeleteArray(&deleteArray_CbmRichEventDisplay);
      instance.SetDestructor(&destruct_CbmRichEventDisplay);
      instance.SetStreamerFunc(&streamer_CbmRichEventDisplay);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichEventDisplay*)
   {
      return GenerateInitInstanceLocal((::CbmRichEventDisplay*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichEventDisplay*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichContFact(void *p = nullptr);
   static void *newArray_CbmRichContFact(Long_t size, void *p);
   static void delete_CbmRichContFact(void *p);
   static void deleteArray_CbmRichContFact(void *p);
   static void destruct_CbmRichContFact(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichContFact*)
   {
      ::CbmRichContFact *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichContFact >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichContFact", ::CbmRichContFact::Class_Version(), "CbmRichContFact.h", 26,
                  typeid(::CbmRichContFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichContFact::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichContFact) );
      instance.SetNew(&new_CbmRichContFact);
      instance.SetNewArray(&newArray_CbmRichContFact);
      instance.SetDelete(&delete_CbmRichContFact);
      instance.SetDeleteArray(&deleteArray_CbmRichContFact);
      instance.SetDestructor(&destruct_CbmRichContFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichContFact*)
   {
      return GenerateInitInstanceLocal((::CbmRichContFact*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichContFact*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018RichPar(void *p = nullptr);
   static void *newArray_CbmMcbm2018RichPar(Long_t size, void *p);
   static void delete_CbmMcbm2018RichPar(void *p);
   static void deleteArray_CbmMcbm2018RichPar(void *p);
   static void destruct_CbmMcbm2018RichPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018RichPar*)
   {
      ::CbmMcbm2018RichPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018RichPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018RichPar", ::CbmMcbm2018RichPar::Class_Version(), "CbmMcbm2018RichPar.h", 18,
                  typeid(::CbmMcbm2018RichPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018RichPar::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2018RichPar) );
      instance.SetNew(&new_CbmMcbm2018RichPar);
      instance.SetNewArray(&newArray_CbmMcbm2018RichPar);
      instance.SetDelete(&delete_CbmMcbm2018RichPar);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018RichPar);
      instance.SetDestructor(&destruct_CbmMcbm2018RichPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018RichPar*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018RichPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018RichPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmRichPmt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichPmt::Class_Name()
{
   return "CbmRichPmt";
}

//______________________________________________________________________________
const char *CbmRichPmt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichPmt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichPmt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichPmt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichPmt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichPmt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichPmt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichPmt*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichEventDisplay::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichEventDisplay::Class_Name()
{
   return "CbmRichEventDisplay";
}

//______________________________________________________________________________
const char *CbmRichEventDisplay::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichEventDisplay*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichEventDisplay::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichEventDisplay*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichEventDisplay::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichEventDisplay*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichEventDisplay::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichEventDisplay*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichContFact::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichContFact::Class_Name()
{
   return "CbmRichContFact";
}

//______________________________________________________________________________
const char *CbmRichContFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichContFact*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichContFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichContFact*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichContFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichContFact*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichContFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichContFact*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018RichPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018RichPar::Class_Name()
{
   return "CbmMcbm2018RichPar";
}

//______________________________________________________________________________
const char *CbmMcbm2018RichPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018RichPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018RichPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018RichPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018RichPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018RichPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018RichPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018RichPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmRichPmt::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichPmt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichPmt::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichPmt::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichPmt(void *p) {
      return  p ? new(p) ::CbmRichPmt : new ::CbmRichPmt;
   }
   static void *newArray_CbmRichPmt(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichPmt[nElements] : new ::CbmRichPmt[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichPmt(void *p) {
      delete ((::CbmRichPmt*)p);
   }
   static void deleteArray_CbmRichPmt(void *p) {
      delete [] ((::CbmRichPmt*)p);
   }
   static void destruct_CbmRichPmt(void *p) {
      typedef ::CbmRichPmt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichPmt

//______________________________________________________________________________
void CbmRichEventDisplay::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichEventDisplay.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      fRichRings->Streamer(R__b);
      fRichHits->Streamer(R__b);
      fRichPoints->Streamer(R__b);
      fRichMatches->Streamer(R__b);
      fRichProjections->Streamer(R__b);
      fMcTracks->Streamer(R__b);
      { TString R__str; R__str.Streamer(R__b); fOutputDir = R__str.Data(); }
      R__b >> fHM;
      R__b >> fEventNum;
      R__b >> fDrawRings;
      R__b >> fDrawHits;
      R__b >> fDrawPoints;
      R__b >> fDrawProjections;
      R__b.CheckByteCount(R__s, R__c, CbmRichEventDisplay::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmRichEventDisplay::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      fRichRings->Streamer(R__b);
      fRichHits->Streamer(R__b);
      fRichPoints->Streamer(R__b);
      fRichMatches->Streamer(R__b);
      fRichProjections->Streamer(R__b);
      fMcTracks->Streamer(R__b);
      { TString R__str = fOutputDir.c_str(); R__str.Streamer(R__b);}
      R__b << fHM;
      R__b << fEventNum;
      R__b << fDrawRings;
      R__b << fDrawHits;
      R__b << fDrawPoints;
      R__b << fDrawProjections;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichEventDisplay(void *p) {
      return  p ? new(p) ::CbmRichEventDisplay : new ::CbmRichEventDisplay;
   }
   static void *newArray_CbmRichEventDisplay(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichEventDisplay[nElements] : new ::CbmRichEventDisplay[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichEventDisplay(void *p) {
      delete ((::CbmRichEventDisplay*)p);
   }
   static void deleteArray_CbmRichEventDisplay(void *p) {
      delete [] ((::CbmRichEventDisplay*)p);
   }
   static void destruct_CbmRichEventDisplay(void *p) {
      typedef ::CbmRichEventDisplay current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmRichEventDisplay(TBuffer &buf, void *obj) {
      ((::CbmRichEventDisplay*)obj)->::CbmRichEventDisplay::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmRichEventDisplay

//______________________________________________________________________________
void CbmRichContFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichContFact.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichContFact::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichContFact::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichContFact(void *p) {
      return  p ? new(p) ::CbmRichContFact : new ::CbmRichContFact;
   }
   static void *newArray_CbmRichContFact(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichContFact[nElements] : new ::CbmRichContFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichContFact(void *p) {
      delete ((::CbmRichContFact*)p);
   }
   static void deleteArray_CbmRichContFact(void *p) {
      delete [] ((::CbmRichContFact*)p);
   }
   static void destruct_CbmRichContFact(void *p) {
      typedef ::CbmRichContFact current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichContFact

//______________________________________________________________________________
void CbmMcbm2018RichPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018RichPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2018RichPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2018RichPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018RichPar(void *p) {
      return  p ? new(p) ::CbmMcbm2018RichPar : new ::CbmMcbm2018RichPar;
   }
   static void *newArray_CbmMcbm2018RichPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018RichPar[nElements] : new ::CbmMcbm2018RichPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018RichPar(void *p) {
      delete ((::CbmMcbm2018RichPar*)p);
   }
   static void deleteArray_CbmMcbm2018RichPar(void *p) {
      delete [] ((::CbmMcbm2018RichPar*)p);
   }
   static void destruct_CbmMcbm2018RichPar(void *p) {
      typedef ::CbmMcbm2018RichPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2018RichPar

namespace ROOT {
   static TClass *maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR_Dictionary();
   static void maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR_TClassManip(TClass*);
   static void *new_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR(void *p = nullptr);
   static void *newArray_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR(Long_t size, void *p);
   static void delete_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR(void *p);
   static void deleteArray_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR(void *p);
   static void destruct_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<CbmRichPmtTypeEnum,CbmRichPmtQEData*>*)
   {
      map<CbmRichPmtTypeEnum,CbmRichPmtQEData*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<CbmRichPmtTypeEnum,CbmRichPmtQEData*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<CbmRichPmtTypeEnum,CbmRichPmtQEData*>", -2, "map", 100,
                  typeid(map<CbmRichPmtTypeEnum,CbmRichPmtQEData*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<CbmRichPmtTypeEnum,CbmRichPmtQEData*>) );
      instance.SetNew(&new_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR);
      instance.SetNewArray(&newArray_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR);
      instance.SetDelete(&delete_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR);
      instance.SetDeleteArray(&deleteArray_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR);
      instance.SetDestructor(&destruct_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<CbmRichPmtTypeEnum,CbmRichPmtQEData*> >()));

      ::ROOT::AddClassAlternate("map<CbmRichPmtTypeEnum,CbmRichPmtQEData*>","std::map<CbmRichPmtTypeEnum, CbmRichPmtQEData*, std::less<CbmRichPmtTypeEnum>, std::allocator<std::pair<CbmRichPmtTypeEnum const, CbmRichPmtQEData*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<CbmRichPmtTypeEnum,CbmRichPmtQEData*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<CbmRichPmtTypeEnum,CbmRichPmtQEData*>*)nullptr)->GetClass();
      maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<CbmRichPmtTypeEnum,CbmRichPmtQEData*> : new map<CbmRichPmtTypeEnum,CbmRichPmtQEData*>;
   }
   static void *newArray_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<CbmRichPmtTypeEnum,CbmRichPmtQEData*>[nElements] : new map<CbmRichPmtTypeEnum,CbmRichPmtQEData*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR(void *p) {
      delete ((map<CbmRichPmtTypeEnum,CbmRichPmtQEData*>*)p);
   }
   static void deleteArray_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR(void *p) {
      delete [] ((map<CbmRichPmtTypeEnum,CbmRichPmtQEData*>*)p);
   }
   static void destruct_maplECbmRichPmtTypeEnumcOCbmRichPmtQEDatamUgR(void *p) {
      typedef map<CbmRichPmtTypeEnum,CbmRichPmtQEData*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<CbmRichPmtTypeEnum,CbmRichPmtQEData*>

namespace {
  void TriggerDictionaryInitialization_libCbmRichBase_Impl() {
    static const char* headers[] = {
"CbmRichPmt.h",
"CbmRichEventDisplay.h",
"CbmRichContFact.h",
"CbmMcbm2018RichPar.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich/utils",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich/utils",
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
"/home/heinemann/gpuunpacker/cbmroot/build2/core/detectors/rich/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmRichBase dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmRichPmt.h")))  CbmRichPmt;
class __attribute__((annotate("$clingAutoload$CbmRichEventDisplay.h")))  CbmRichEventDisplay;
class __attribute__((annotate(R"ATTRDUMP(Factory for all Rich parameter containers)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmRichContFact.h")))  CbmRichContFact;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018RichPar.h")))  CbmMcbm2018RichPar;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmRichBase dictionary payload"

#ifndef HAVE_SSE
  #define HAVE_SSE 1
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
#include "CbmRichPmt.h"
#include "CbmRichEventDisplay.h"
#include "CbmRichContFact.h"
#include "CbmMcbm2018RichPar.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmMcbm2018RichPar", payloadCode, "@",
"CbmRichContFact", payloadCode, "@",
"CbmRichEventDisplay", payloadCode, "@",
"CbmRichPmt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmRichBase",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmRichBase_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmRichBase_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmRichBase() {
  TriggerDictionaryInitialization_libCbmRichBase_Impl();
}
