// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmRichSim
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
#include "CbmRich.h"
#include "CbmRichDigitizer.h"
#include "RichGeoCreator.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmRich(void *p = nullptr);
   static void *newArray_CbmRich(Long_t size, void *p);
   static void delete_CbmRich(void *p);
   static void deleteArray_CbmRich(void *p);
   static void destruct_CbmRich(void *p);
   static void streamer_CbmRich(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRich*)
   {
      ::CbmRich *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRich >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRich", ::CbmRich::Class_Version(), "CbmRich.h", 43,
                  typeid(::CbmRich), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRich::Dictionary, isa_proxy, 16,
                  sizeof(::CbmRich) );
      instance.SetNew(&new_CbmRich);
      instance.SetNewArray(&newArray_CbmRich);
      instance.SetDelete(&delete_CbmRich);
      instance.SetDeleteArray(&deleteArray_CbmRich);
      instance.SetDestructor(&destruct_CbmRich);
      instance.SetStreamerFunc(&streamer_CbmRich);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRich*)
   {
      return GenerateInitInstanceLocal((::CbmRich*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRich*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigitizelECbmRichDigigR_Dictionary();
   static void CbmDigitizelECbmRichDigigR_TClassManip(TClass*);
   static void delete_CbmDigitizelECbmRichDigigR(void *p);
   static void deleteArray_CbmDigitizelECbmRichDigigR(void *p);
   static void destruct_CbmDigitizelECbmRichDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigitize<CbmRichDigi>*)
   {
      ::CbmDigitize<CbmRichDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigitize<CbmRichDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigitize<CbmRichDigi>", ::CbmDigitize<CbmRichDigi>::Class_Version(), "CbmDigitize.h", 44,
                  typeid(::CbmDigitize<CbmRichDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigitizelECbmRichDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigitize<CbmRichDigi>) );
      instance.SetDelete(&delete_CbmDigitizelECbmRichDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigitizelECbmRichDigigR);
      instance.SetDestructor(&destruct_CbmDigitizelECbmRichDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigitize<CbmRichDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigitize<CbmRichDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigitize<CbmRichDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigitizelECbmRichDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmRichDigi>*)nullptr)->GetClass();
      CbmDigitizelECbmRichDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigitizelECbmRichDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichDigitizer(void *p = nullptr);
   static void *newArray_CbmRichDigitizer(Long_t size, void *p);
   static void delete_CbmRichDigitizer(void *p);
   static void deleteArray_CbmRichDigitizer(void *p);
   static void destruct_CbmRichDigitizer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichDigitizer*)
   {
      ::CbmRichDigitizer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichDigitizer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichDigitizer", ::CbmRichDigitizer::Class_Version(), "CbmRichDigitizer.h", 41,
                  typeid(::CbmRichDigitizer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichDigitizer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichDigitizer) );
      instance.SetNew(&new_CbmRichDigitizer);
      instance.SetNewArray(&newArray_CbmRichDigitizer);
      instance.SetDelete(&delete_CbmRichDigitizer);
      instance.SetDeleteArray(&deleteArray_CbmRichDigitizer);
      instance.SetDestructor(&destruct_CbmRichDigitizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichDigitizer*)
   {
      return GenerateInitInstanceLocal((::CbmRichDigitizer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichDigitizer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_RichGeoCreator(void *p = nullptr);
   static void *newArray_RichGeoCreator(Long_t size, void *p);
   static void delete_RichGeoCreator(void *p);
   static void deleteArray_RichGeoCreator(void *p);
   static void destruct_RichGeoCreator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RichGeoCreator*)
   {
      ::RichGeoCreator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RichGeoCreator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("RichGeoCreator", ::RichGeoCreator::Class_Version(), "RichGeoCreator.h", 23,
                  typeid(::RichGeoCreator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RichGeoCreator::Dictionary, isa_proxy, 4,
                  sizeof(::RichGeoCreator) );
      instance.SetNew(&new_RichGeoCreator);
      instance.SetNewArray(&newArray_RichGeoCreator);
      instance.SetDelete(&delete_RichGeoCreator);
      instance.SetDeleteArray(&deleteArray_RichGeoCreator);
      instance.SetDestructor(&destruct_RichGeoCreator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RichGeoCreator*)
   {
      return GenerateInitInstanceLocal((::RichGeoCreator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RichGeoCreator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmRich::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRich::Class_Name()
{
   return "CbmRich";
}

//______________________________________________________________________________
const char *CbmRich::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRich*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRich::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRich*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRich::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRich*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRich::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRich*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigitize<CbmRichDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmRichDigi>::Class_Name()
{
   return "CbmDigitize<CbmRichDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigitize<CbmRichDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmRichDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigitize<CbmRichDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmRichDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmRichDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmRichDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigitize<CbmRichDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigitize<CbmRichDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichDigitizer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichDigitizer::Class_Name()
{
   return "CbmRichDigitizer";
}

//______________________________________________________________________________
const char *CbmRichDigitizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichDigitizer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichDigitizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichDigitizer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichDigitizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichDigitizer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichDigitizer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichDigitizer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RichGeoCreator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *RichGeoCreator::Class_Name()
{
   return "RichGeoCreator";
}

//______________________________________________________________________________
const char *RichGeoCreator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RichGeoCreator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int RichGeoCreator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RichGeoCreator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RichGeoCreator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RichGeoCreator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RichGeoCreator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RichGeoCreator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmRich::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRich.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairDetector::Streamer(R__b);
      R__b >> fPosIndex;
      R__b >> fRegisterPhotonsOnSensitivePlane;
      fRichPoints->Streamer(R__b);
      fRichRefPlanePoints->Streamer(R__b);
      fRichMirrorPoints->Streamer(R__b);
      R__b >> fRotation;
      R__b >> fPositionRotation;
      R__b.CheckByteCount(R__s, R__c, CbmRich::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmRich::IsA(), kTRUE);
      FairDetector::Streamer(R__b);
      R__b << fPosIndex;
      R__b << fRegisterPhotonsOnSensitivePlane;
      fRichPoints->Streamer(R__b);
      fRichRefPlanePoints->Streamer(R__b);
      fRichMirrorPoints->Streamer(R__b);
      R__b << fRotation;
      R__b << fPositionRotation;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRich(void *p) {
      return  p ? new(p) ::CbmRich : new ::CbmRich;
   }
   static void *newArray_CbmRich(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRich[nElements] : new ::CbmRich[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRich(void *p) {
      delete ((::CbmRich*)p);
   }
   static void deleteArray_CbmRich(void *p) {
      delete [] ((::CbmRich*)p);
   }
   static void destruct_CbmRich(void *p) {
      typedef ::CbmRich current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmRich(TBuffer &buf, void *obj) {
      ((::CbmRich*)obj)->::CbmRich::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmRich

//______________________________________________________________________________
template <> void CbmDigitize<CbmRichDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigitize<CbmRichDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigitize<CbmRichDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigitize<CbmRichDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmDigitizelECbmRichDigigR(void *p) {
      delete ((::CbmDigitize<CbmRichDigi>*)p);
   }
   static void deleteArray_CbmDigitizelECbmRichDigigR(void *p) {
      delete [] ((::CbmDigitize<CbmRichDigi>*)p);
   }
   static void destruct_CbmDigitizelECbmRichDigigR(void *p) {
      typedef ::CbmDigitize<CbmRichDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigitize<CbmRichDigi>

//______________________________________________________________________________
void CbmRichDigitizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichDigitizer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichDigitizer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichDigitizer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichDigitizer(void *p) {
      return  p ? new(p) ::CbmRichDigitizer : new ::CbmRichDigitizer;
   }
   static void *newArray_CbmRichDigitizer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichDigitizer[nElements] : new ::CbmRichDigitizer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichDigitizer(void *p) {
      delete ((::CbmRichDigitizer*)p);
   }
   static void deleteArray_CbmRichDigitizer(void *p) {
      delete [] ((::CbmRichDigitizer*)p);
   }
   static void destruct_CbmRichDigitizer(void *p) {
      typedef ::CbmRichDigitizer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichDigitizer

//______________________________________________________________________________
void RichGeoCreator::Streamer(TBuffer &R__b)
{
   // Stream an object of class RichGeoCreator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RichGeoCreator::Class(),this);
   } else {
      R__b.WriteClassBuffer(RichGeoCreator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RichGeoCreator(void *p) {
      return  p ? new(p) ::RichGeoCreator : new ::RichGeoCreator;
   }
   static void *newArray_RichGeoCreator(Long_t nElements, void *p) {
      return p ? new(p) ::RichGeoCreator[nElements] : new ::RichGeoCreator[nElements];
   }
   // Wrapper around operator delete
   static void delete_RichGeoCreator(void *p) {
      delete ((::RichGeoCreator*)p);
   }
   static void deleteArray_RichGeoCreator(void *p) {
      delete [] ((::RichGeoCreator*)p);
   }
   static void destruct_RichGeoCreator(void *p) {
      typedef ::RichGeoCreator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RichGeoCreator

namespace ROOT {
   static TClass *maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR_Dictionary();
   static void maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR(void *p);
   static void deleteArray_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR(void *p);
   static void destruct_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,pair<CbmRichDigi*,CbmMatch*> >*)
   {
      map<int,pair<CbmRichDigi*,CbmMatch*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,pair<CbmRichDigi*,CbmMatch*> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,pair<CbmRichDigi*,CbmMatch*> >", -2, "map", 100,
                  typeid(map<int,pair<CbmRichDigi*,CbmMatch*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,pair<CbmRichDigi*,CbmMatch*> >) );
      instance.SetNew(&new_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR);
      instance.SetDelete(&delete_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,pair<CbmRichDigi*,CbmMatch*> > >()));

      ::ROOT::AddClassAlternate("map<int,pair<CbmRichDigi*,CbmMatch*> >","std::map<int, std::pair<CbmRichDigi*, CbmMatch*>, std::less<int>, std::allocator<std::pair<int const, std::pair<CbmRichDigi*, CbmMatch*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,pair<CbmRichDigi*,CbmMatch*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,pair<CbmRichDigi*,CbmMatch*> >*)nullptr)->GetClass();
      maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,pair<CbmRichDigi*,CbmMatch*> > : new map<int,pair<CbmRichDigi*,CbmMatch*> >;
   }
   static void *newArray_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,pair<CbmRichDigi*,CbmMatch*> >[nElements] : new map<int,pair<CbmRichDigi*,CbmMatch*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR(void *p) {
      delete ((map<int,pair<CbmRichDigi*,CbmMatch*> >*)p);
   }
   static void deleteArray_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR(void *p) {
      delete [] ((map<int,pair<CbmRichDigi*,CbmMatch*> >*)p);
   }
   static void destruct_maplEintcOpairlECbmRichDigimUcOCbmMatchmUgRsPgR(void *p) {
      typedef map<int,pair<CbmRichDigi*,CbmMatch*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,pair<CbmRichDigi*,CbmMatch*> >

namespace ROOT {
   static TClass *maplEintcOdoublegR_Dictionary();
   static void maplEintcOdoublegR_TClassManip(TClass*);
   static void *new_maplEintcOdoublegR(void *p = nullptr);
   static void *newArray_maplEintcOdoublegR(Long_t size, void *p);
   static void delete_maplEintcOdoublegR(void *p);
   static void deleteArray_maplEintcOdoublegR(void *p);
   static void destruct_maplEintcOdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,double>*)
   {
      map<int,double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,double>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,double>", -2, "map", 100,
                  typeid(map<int,double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,double>) );
      instance.SetNew(&new_maplEintcOdoublegR);
      instance.SetNewArray(&newArray_maplEintcOdoublegR);
      instance.SetDelete(&delete_maplEintcOdoublegR);
      instance.SetDeleteArray(&deleteArray_maplEintcOdoublegR);
      instance.SetDestructor(&destruct_maplEintcOdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,double> >()));

      ::ROOT::AddClassAlternate("map<int,double>","std::map<int, double, std::less<int>, std::allocator<std::pair<int const, double> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,double>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,double>*)nullptr)->GetClass();
      maplEintcOdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,double> : new map<int,double>;
   }
   static void *newArray_maplEintcOdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,double>[nElements] : new map<int,double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOdoublegR(void *p) {
      delete ((map<int,double>*)p);
   }
   static void deleteArray_maplEintcOdoublegR(void *p) {
      delete [] ((map<int,double>*)p);
   }
   static void destruct_maplEintcOdoublegR(void *p) {
      typedef map<int,double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,double>

namespace {
  void TriggerDictionaryInitialization_libCbmRichSim_Impl() {
    static const char* headers[] = {
"CbmRich.h",
"CbmRichDigitizer.h",
"RichGeoCreator.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/rich/geo",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/sim/detectors/rich/geo",
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
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/rich/utils",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/rich/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmRichSim dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmRich.h")))  CbmRich;
class __attribute__((annotate("$clingAutoload$CbmRichDigi.h")))  __attribute__((annotate("$clingAutoload$CbmRichDigitizer.h")))  CbmRichDigi;
template <class Digi> class __attribute__((annotate("$clingAutoload$CbmDigitize.h")))  __attribute__((annotate("$clingAutoload$CbmRichDigitizer.h")))  CbmDigitize;

class __attribute__((annotate("$clingAutoload$CbmRichDigitizer.h")))  CbmRichDigitizer;
class __attribute__((annotate("$clingAutoload$RichGeoCreator.h")))  RichGeoCreator;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmRichSim dictionary payload"

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
#include "CbmRich.h"
#include "CbmRichDigitizer.h"
#include "RichGeoCreator.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmDigitize<CbmRichDigi>", payloadCode, "@",
"CbmRich", payloadCode, "@",
"CbmRichDigitizer", payloadCode, "@",
"RichGeoCreator", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmRichSim",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmRichSim_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmRichSim_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmRichSim() {
  TriggerDictionaryInitialization_libCbmRichSim_Impl();
}
