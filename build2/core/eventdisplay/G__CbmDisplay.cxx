// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmDisplay
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
#include "CbmTrdDigiDraw.h"
#include "CbmPixelHitSetDraw.h"
#include "CbmEvDisTracks.h"
#include "CbmRecoTracks.h"
#include "CbmPointSetArray.h"
#include "CbmPointSetArrayDraw.h"
#include "CbmPointSetArrayEditor.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmTrdDigiDraw(void *p = nullptr);
   static void *newArray_CbmTrdDigiDraw(Long_t size, void *p);
   static void delete_CbmTrdDigiDraw(void *p);
   static void deleteArray_CbmTrdDigiDraw(void *p);
   static void destruct_CbmTrdDigiDraw(void *p);
   static void streamer_CbmTrdDigiDraw(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdDigiDraw*)
   {
      ::CbmTrdDigiDraw *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdDigiDraw >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdDigiDraw", ::CbmTrdDigiDraw::Class_Version(), "CbmTrdDigiDraw.h", 32,
                  typeid(::CbmTrdDigiDraw), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdDigiDraw::Dictionary, isa_proxy, 16,
                  sizeof(::CbmTrdDigiDraw) );
      instance.SetNew(&new_CbmTrdDigiDraw);
      instance.SetNewArray(&newArray_CbmTrdDigiDraw);
      instance.SetDelete(&delete_CbmTrdDigiDraw);
      instance.SetDeleteArray(&deleteArray_CbmTrdDigiDraw);
      instance.SetDestructor(&destruct_CbmTrdDigiDraw);
      instance.SetStreamerFunc(&streamer_CbmTrdDigiDraw);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdDigiDraw*)
   {
      return GenerateInitInstanceLocal((::CbmTrdDigiDraw*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdDigiDraw*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPixelHitSetDraw(void *p = nullptr);
   static void *newArray_CbmPixelHitSetDraw(Long_t size, void *p);
   static void delete_CbmPixelHitSetDraw(void *p);
   static void deleteArray_CbmPixelHitSetDraw(void *p);
   static void destruct_CbmPixelHitSetDraw(void *p);
   static void streamer_CbmPixelHitSetDraw(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPixelHitSetDraw*)
   {
      ::CbmPixelHitSetDraw *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPixelHitSetDraw >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPixelHitSetDraw", ::CbmPixelHitSetDraw::Class_Version(), "CbmPixelHitSetDraw.h", 16,
                  typeid(::CbmPixelHitSetDraw), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPixelHitSetDraw::Dictionary, isa_proxy, 16,
                  sizeof(::CbmPixelHitSetDraw) );
      instance.SetNew(&new_CbmPixelHitSetDraw);
      instance.SetNewArray(&newArray_CbmPixelHitSetDraw);
      instance.SetDelete(&delete_CbmPixelHitSetDraw);
      instance.SetDeleteArray(&deleteArray_CbmPixelHitSetDraw);
      instance.SetDestructor(&destruct_CbmPixelHitSetDraw);
      instance.SetStreamerFunc(&streamer_CbmPixelHitSetDraw);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPixelHitSetDraw*)
   {
      return GenerateInitInstanceLocal((::CbmPixelHitSetDraw*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPixelHitSetDraw*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmEvDisTracks(void *p = nullptr);
   static void *newArray_CbmEvDisTracks(Long_t size, void *p);
   static void delete_CbmEvDisTracks(void *p);
   static void deleteArray_CbmEvDisTracks(void *p);
   static void destruct_CbmEvDisTracks(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmEvDisTracks*)
   {
      ::CbmEvDisTracks *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmEvDisTracks >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmEvDisTracks", ::CbmEvDisTracks::Class_Version(), "CbmEvDisTracks.h", 35,
                  typeid(::CbmEvDisTracks), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmEvDisTracks::Dictionary, isa_proxy, 4,
                  sizeof(::CbmEvDisTracks) );
      instance.SetNew(&new_CbmEvDisTracks);
      instance.SetNewArray(&newArray_CbmEvDisTracks);
      instance.SetDelete(&delete_CbmEvDisTracks);
      instance.SetDeleteArray(&deleteArray_CbmEvDisTracks);
      instance.SetDestructor(&destruct_CbmEvDisTracks);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmEvDisTracks*)
   {
      return GenerateInitInstanceLocal((::CbmEvDisTracks*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmEvDisTracks*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRecoTracks(void *p = nullptr);
   static void *newArray_CbmRecoTracks(Long_t size, void *p);
   static void delete_CbmRecoTracks(void *p);
   static void deleteArray_CbmRecoTracks(void *p);
   static void destruct_CbmRecoTracks(void *p);
   static void streamer_CbmRecoTracks(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRecoTracks*)
   {
      ::CbmRecoTracks *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRecoTracks >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRecoTracks", ::CbmRecoTracks::Class_Version(), "CbmRecoTracks.h", 47,
                  typeid(::CbmRecoTracks), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRecoTracks::Dictionary, isa_proxy, 16,
                  sizeof(::CbmRecoTracks) );
      instance.SetNew(&new_CbmRecoTracks);
      instance.SetNewArray(&newArray_CbmRecoTracks);
      instance.SetDelete(&delete_CbmRecoTracks);
      instance.SetDeleteArray(&deleteArray_CbmRecoTracks);
      instance.SetDestructor(&destruct_CbmRecoTracks);
      instance.SetStreamerFunc(&streamer_CbmRecoTracks);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRecoTracks*)
   {
      return GenerateInitInstanceLocal((::CbmRecoTracks*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRecoTracks*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPointSetArray(void *p = nullptr);
   static void *newArray_CbmPointSetArray(Long_t size, void *p);
   static void delete_CbmPointSetArray(void *p);
   static void deleteArray_CbmPointSetArray(void *p);
   static void destruct_CbmPointSetArray(void *p);
   static void streamer_CbmPointSetArray(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPointSetArray*)
   {
      ::CbmPointSetArray *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPointSetArray >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPointSetArray", ::CbmPointSetArray::Class_Version(), "CbmPointSetArray.h", 27,
                  typeid(::CbmPointSetArray), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPointSetArray::Dictionary, isa_proxy, 16,
                  sizeof(::CbmPointSetArray) );
      instance.SetNew(&new_CbmPointSetArray);
      instance.SetNewArray(&newArray_CbmPointSetArray);
      instance.SetDelete(&delete_CbmPointSetArray);
      instance.SetDeleteArray(&deleteArray_CbmPointSetArray);
      instance.SetDestructor(&destruct_CbmPointSetArray);
      instance.SetStreamerFunc(&streamer_CbmPointSetArray);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPointSetArray*)
   {
      return GenerateInitInstanceLocal((::CbmPointSetArray*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPointSetArray*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPointSetArrayDraw(void *p = nullptr);
   static void *newArray_CbmPointSetArrayDraw(Long_t size, void *p);
   static void delete_CbmPointSetArrayDraw(void *p);
   static void deleteArray_CbmPointSetArrayDraw(void *p);
   static void destruct_CbmPointSetArrayDraw(void *p);
   static void streamer_CbmPointSetArrayDraw(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPointSetArrayDraw*)
   {
      ::CbmPointSetArrayDraw *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPointSetArrayDraw >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPointSetArrayDraw", ::CbmPointSetArrayDraw::Class_Version(), "CbmPointSetArrayDraw.h", 33,
                  typeid(::CbmPointSetArrayDraw), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPointSetArrayDraw::Dictionary, isa_proxy, 16,
                  sizeof(::CbmPointSetArrayDraw) );
      instance.SetNew(&new_CbmPointSetArrayDraw);
      instance.SetNewArray(&newArray_CbmPointSetArrayDraw);
      instance.SetDelete(&delete_CbmPointSetArrayDraw);
      instance.SetDeleteArray(&deleteArray_CbmPointSetArrayDraw);
      instance.SetDestructor(&destruct_CbmPointSetArrayDraw);
      instance.SetStreamerFunc(&streamer_CbmPointSetArrayDraw);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPointSetArrayDraw*)
   {
      return GenerateInitInstanceLocal((::CbmPointSetArrayDraw*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPointSetArrayDraw*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPointSetArrayEditor(void *p = nullptr);
   static void *newArray_CbmPointSetArrayEditor(Long_t size, void *p);
   static void delete_CbmPointSetArrayEditor(void *p);
   static void deleteArray_CbmPointSetArrayEditor(void *p);
   static void destruct_CbmPointSetArrayEditor(void *p);
   static void streamer_CbmPointSetArrayEditor(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPointSetArrayEditor*)
   {
      ::CbmPointSetArrayEditor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPointSetArrayEditor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPointSetArrayEditor", ::CbmPointSetArrayEditor::Class_Version(), "CbmPointSetArrayEditor.h", 37,
                  typeid(::CbmPointSetArrayEditor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPointSetArrayEditor::Dictionary, isa_proxy, 16,
                  sizeof(::CbmPointSetArrayEditor) );
      instance.SetNew(&new_CbmPointSetArrayEditor);
      instance.SetNewArray(&newArray_CbmPointSetArrayEditor);
      instance.SetDelete(&delete_CbmPointSetArrayEditor);
      instance.SetDeleteArray(&deleteArray_CbmPointSetArrayEditor);
      instance.SetDestructor(&destruct_CbmPointSetArrayEditor);
      instance.SetStreamerFunc(&streamer_CbmPointSetArrayEditor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPointSetArrayEditor*)
   {
      return GenerateInitInstanceLocal((::CbmPointSetArrayEditor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPointSetArrayEditor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdDigiDraw::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdDigiDraw::Class_Name()
{
   return "CbmTrdDigiDraw";
}

//______________________________________________________________________________
const char *CbmTrdDigiDraw::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigiDraw*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdDigiDraw::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigiDraw*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdDigiDraw::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigiDraw*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdDigiDraw::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigiDraw*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPixelHitSetDraw::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPixelHitSetDraw::Class_Name()
{
   return "CbmPixelHitSetDraw";
}

//______________________________________________________________________________
const char *CbmPixelHitSetDraw::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPixelHitSetDraw*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPixelHitSetDraw::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPixelHitSetDraw*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPixelHitSetDraw::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPixelHitSetDraw*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPixelHitSetDraw::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPixelHitSetDraw*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmEvDisTracks::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmEvDisTracks::Class_Name()
{
   return "CbmEvDisTracks";
}

//______________________________________________________________________________
const char *CbmEvDisTracks::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmEvDisTracks*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmEvDisTracks::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmEvDisTracks*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmEvDisTracks::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmEvDisTracks*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmEvDisTracks::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmEvDisTracks*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRecoTracks::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRecoTracks::Class_Name()
{
   return "CbmRecoTracks";
}

//______________________________________________________________________________
const char *CbmRecoTracks::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoTracks*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRecoTracks::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoTracks*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRecoTracks::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoTracks*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRecoTracks::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecoTracks*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPointSetArray::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPointSetArray::Class_Name()
{
   return "CbmPointSetArray";
}

//______________________________________________________________________________
const char *CbmPointSetArray::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPointSetArray*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPointSetArray::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPointSetArray*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPointSetArray::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPointSetArray*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPointSetArray::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPointSetArray*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPointSetArrayDraw::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPointSetArrayDraw::Class_Name()
{
   return "CbmPointSetArrayDraw";
}

//______________________________________________________________________________
const char *CbmPointSetArrayDraw::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPointSetArrayDraw*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPointSetArrayDraw::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPointSetArrayDraw*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPointSetArrayDraw::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPointSetArrayDraw*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPointSetArrayDraw::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPointSetArrayDraw*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPointSetArrayEditor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPointSetArrayEditor::Class_Name()
{
   return "CbmPointSetArrayEditor";
}

//______________________________________________________________________________
const char *CbmPointSetArrayEditor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPointSetArrayEditor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPointSetArrayEditor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPointSetArrayEditor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPointSetArrayEditor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPointSetArrayEditor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPointSetArrayEditor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPointSetArrayEditor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmTrdDigiDraw::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdDigiDraw.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> fVerbose;
      R__b.ReadStaticArray((bool*)fActiveLayers);
      R__b.CheckByteCount(R__s, R__c, CbmTrdDigiDraw::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmTrdDigiDraw::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << fVerbose;
      R__b.WriteArray(fActiveLayers, 15);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdDigiDraw(void *p) {
      return  p ? new(p) ::CbmTrdDigiDraw : new ::CbmTrdDigiDraw;
   }
   static void *newArray_CbmTrdDigiDraw(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdDigiDraw[nElements] : new ::CbmTrdDigiDraw[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdDigiDraw(void *p) {
      delete ((::CbmTrdDigiDraw*)p);
   }
   static void deleteArray_CbmTrdDigiDraw(void *p) {
      delete [] ((::CbmTrdDigiDraw*)p);
   }
   static void destruct_CbmTrdDigiDraw(void *p) {
      typedef ::CbmTrdDigiDraw current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmTrdDigiDraw(TBuffer &buf, void *obj) {
      ((::CbmTrdDigiDraw*)obj)->::CbmTrdDigiDraw::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmTrdDigiDraw

//______________________________________________________________________________
void CbmPixelHitSetDraw::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPixelHitSetDraw.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairPointSetDraw::Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, CbmPixelHitSetDraw::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmPixelHitSetDraw::IsA(), kTRUE);
      FairPointSetDraw::Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPixelHitSetDraw(void *p) {
      return  p ? new(p) ::CbmPixelHitSetDraw : new ::CbmPixelHitSetDraw;
   }
   static void *newArray_CbmPixelHitSetDraw(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPixelHitSetDraw[nElements] : new ::CbmPixelHitSetDraw[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPixelHitSetDraw(void *p) {
      delete ((::CbmPixelHitSetDraw*)p);
   }
   static void deleteArray_CbmPixelHitSetDraw(void *p) {
      delete [] ((::CbmPixelHitSetDraw*)p);
   }
   static void destruct_CbmPixelHitSetDraw(void *p) {
      typedef ::CbmPixelHitSetDraw current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmPixelHitSetDraw(TBuffer &buf, void *obj) {
      ((::CbmPixelHitSetDraw*)obj)->::CbmPixelHitSetDraw::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmPixelHitSetDraw

//______________________________________________________________________________
void CbmEvDisTracks::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmEvDisTracks.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmEvDisTracks::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmEvDisTracks::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmEvDisTracks(void *p) {
      return  p ? new(p) ::CbmEvDisTracks : new ::CbmEvDisTracks;
   }
   static void *newArray_CbmEvDisTracks(Long_t nElements, void *p) {
      return p ? new(p) ::CbmEvDisTracks[nElements] : new ::CbmEvDisTracks[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmEvDisTracks(void *p) {
      delete ((::CbmEvDisTracks*)p);
   }
   static void deleteArray_CbmEvDisTracks(void *p) {
      delete [] ((::CbmEvDisTracks*)p);
   }
   static void destruct_CbmEvDisTracks(void *p) {
      typedef ::CbmEvDisTracks current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmEvDisTracks

//______________________________________________________________________________
void CbmRecoTracks::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRecoTracks.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      fGlobalTracks->Streamer(R__b);
      fMvdHits->Streamer(R__b);
      fStsHits->Streamer(R__b);
      fStsTracks->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fRichHits->Streamer(R__b);
      fMuchPixelHits->Streamer(R__b);
      fMuchTracks->Streamer(R__b);
      fTrdHits->Streamer(R__b);
      fTrdTracks->Streamer(R__b);
      fTofHits->Streamer(R__b);
      R__b >> fTrPr;
      R__b >> fEveTrList;
      R__b >> MinEnergyLimit;
      R__b >> MaxEnergyLimit;
      R__b >> PEnergy;
      R__b.CheckByteCount(R__s, R__c, CbmRecoTracks::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmRecoTracks::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      fGlobalTracks->Streamer(R__b);
      fMvdHits->Streamer(R__b);
      fStsHits->Streamer(R__b);
      fStsTracks->Streamer(R__b);
      fRichRings->Streamer(R__b);
      fRichHits->Streamer(R__b);
      fMuchPixelHits->Streamer(R__b);
      fMuchTracks->Streamer(R__b);
      fTrdHits->Streamer(R__b);
      fTrdTracks->Streamer(R__b);
      fTofHits->Streamer(R__b);
      R__b << fTrPr;
      R__b << fEveTrList;
      R__b << MinEnergyLimit;
      R__b << MaxEnergyLimit;
      R__b << PEnergy;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRecoTracks(void *p) {
      return  p ? new(p) ::CbmRecoTracks : new ::CbmRecoTracks;
   }
   static void *newArray_CbmRecoTracks(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRecoTracks[nElements] : new ::CbmRecoTracks[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRecoTracks(void *p) {
      delete ((::CbmRecoTracks*)p);
   }
   static void deleteArray_CbmRecoTracks(void *p) {
      delete [] ((::CbmRecoTracks*)p);
   }
   static void destruct_CbmRecoTracks(void *p) {
      typedef ::CbmRecoTracks current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmRecoTracks(TBuffer &buf, void *obj) {
      ((::CbmRecoTracks*)obj)->::CbmRecoTracks::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmRecoTracks

//______________________________________________________________________________
void CbmPointSetArray::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPointSetArray.

   TEvePointSetArray::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPointSetArray(void *p) {
      return  p ? new(p) ::CbmPointSetArray : new ::CbmPointSetArray;
   }
   static void *newArray_CbmPointSetArray(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPointSetArray[nElements] : new ::CbmPointSetArray[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPointSetArray(void *p) {
      delete ((::CbmPointSetArray*)p);
   }
   static void deleteArray_CbmPointSetArray(void *p) {
      delete [] ((::CbmPointSetArray*)p);
   }
   static void destruct_CbmPointSetArray(void *p) {
      typedef ::CbmPointSetArray current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmPointSetArray(TBuffer &buf, void *obj) {
      ((::CbmPointSetArray*)obj)->::CbmPointSetArray::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmPointSetArray

//______________________________________________________________________________
void CbmPointSetArrayDraw::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPointSetArrayDraw.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTask::Streamer(R__b);
      R__b >> fVerbose;
      fPointList->Streamer(R__b);
      R__b >> fEventManager;
      R__b >> fl;
      R__b >> fColor;
      R__b >> fStyle;
      R__b >> fTimeOffset;
      R__b >> fTimeMax;
      R__b >> fColorMode;
      R__b >> fMarkerMode;
      R__b >> fRender;
      R__b.CheckByteCount(R__s, R__c, CbmPointSetArrayDraw::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmPointSetArrayDraw::IsA(), kTRUE);
      FairTask::Streamer(R__b);
      R__b << fVerbose;
      fPointList->Streamer(R__b);
      R__b << fEventManager;
      R__b << fl;
      R__b << fColor;
      R__b << fStyle;
      R__b << fTimeOffset;
      R__b << fTimeMax;
      R__b << fColorMode;
      R__b << fMarkerMode;
      R__b << fRender;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPointSetArrayDraw(void *p) {
      return  p ? new(p) ::CbmPointSetArrayDraw : new ::CbmPointSetArrayDraw;
   }
   static void *newArray_CbmPointSetArrayDraw(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPointSetArrayDraw[nElements] : new ::CbmPointSetArrayDraw[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPointSetArrayDraw(void *p) {
      delete ((::CbmPointSetArrayDraw*)p);
   }
   static void deleteArray_CbmPointSetArrayDraw(void *p) {
      delete [] ((::CbmPointSetArrayDraw*)p);
   }
   static void destruct_CbmPointSetArrayDraw(void *p) {
      typedef ::CbmPointSetArrayDraw current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmPointSetArrayDraw(TBuffer &buf, void *obj) {
      ((::CbmPointSetArrayDraw*)obj)->::CbmPointSetArrayDraw::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmPointSetArrayDraw

//______________________________________________________________________________
void CbmPointSetArrayEditor::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPointSetArrayEditor.

   TGedFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPointSetArrayEditor(void *p) {
      return  p ? new(p) ::CbmPointSetArrayEditor : new ::CbmPointSetArrayEditor;
   }
   static void *newArray_CbmPointSetArrayEditor(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPointSetArrayEditor[nElements] : new ::CbmPointSetArrayEditor[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPointSetArrayEditor(void *p) {
      delete ((::CbmPointSetArrayEditor*)p);
   }
   static void deleteArray_CbmPointSetArrayEditor(void *p) {
      delete [] ((::CbmPointSetArrayEditor*)p);
   }
   static void destruct_CbmPointSetArrayEditor(void *p) {
      typedef ::CbmPointSetArrayEditor current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmPointSetArrayEditor(TBuffer &buf, void *obj) {
      ((::CbmPointSetArrayEditor*)obj)->::CbmPointSetArrayEditor::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmPointSetArrayEditor

namespace {
  void TriggerDictionaryInitialization_libCbmDisplay_Impl() {
    static const char* headers[] = {
"CbmTrdDigiDraw.h",
"CbmPixelHitSetDraw.h",
"CbmEvDisTracks.h",
"CbmRecoTracks.h",
"CbmPointSetArray.h",
"CbmPointSetArrayDraw.h",
"CbmPointSetArrayEditor.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/eventdisplay",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/eventdisplay",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
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
"/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc",
"/home/heinemann/gpuunpacker/cbmroot/external/xpu/src",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/core/eventdisplay/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmDisplay dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmTrdDigiDraw.h")))  CbmTrdDigiDraw;
class __attribute__((annotate("$clingAutoload$CbmPixelHitSetDraw.h")))  CbmPixelHitSetDraw;
class __attribute__((annotate("$clingAutoload$CbmEvDisTracks.h")))  CbmEvDisTracks;
class __attribute__((annotate("$clingAutoload$CbmRecoTracks.h")))  CbmRecoTracks;
class __attribute__((annotate(R"ATTRDUMP(CbmPointSetArray to implement Additional ColorCoding and other functionalities)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(CbmPointSetArray to implement Additional ColorCoding and other functionalities)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(CbmPointSetArray to implement Additional ColorCoding and other functionalities)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(CbmPointSetArray to implement Additional ColorCoding and other functionalities)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmPointSetArray.h")))  CbmPointSetArray;
class __attribute__((annotate("$clingAutoload$CbmPointSetArrayDraw.h")))  CbmPointSetArrayDraw;
class __attribute__((annotate(R"ATTRDUMP(Editor for CbmPointSetArray class.)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Editor for CbmPointSetArray class.)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Editor for CbmPointSetArray class.)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Editor for CbmPointSetArray class.)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmPointSetArrayEditor.h")))  CbmPointSetArrayEditor;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmDisplay dictionary payload"

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
#include "CbmTrdDigiDraw.h"
#include "CbmPixelHitSetDraw.h"
#include "CbmEvDisTracks.h"
#include "CbmRecoTracks.h"
#include "CbmPointSetArray.h"
#include "CbmPointSetArrayDraw.h"
#include "CbmPointSetArrayEditor.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmEvDisTracks", payloadCode, "@",
"CbmPixelHitSetDraw", payloadCode, "@",
"CbmPointSetArray", payloadCode, "@",
"CbmPointSetArrayDraw", payloadCode, "@",
"CbmPointSetArrayEditor", payloadCode, "@",
"CbmRecoTracks", payloadCode, "@",
"CbmTrdDigiDraw", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmDisplay",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmDisplay_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmDisplay_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmDisplay() {
  TriggerDictionaryInitialization_libCbmDisplay_Impl();
}
