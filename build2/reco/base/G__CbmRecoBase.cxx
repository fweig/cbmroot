// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmRecoBase
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
#include "CbmRecoUnpackConfig.tmpl"
#include "CbmRecoUnpackAlgo.tmpl"
#include "CbmUnpackTaskBase.h"
#include "CbmTrdTrackFinder.h"
#include "CbmTrdTrackFitter.h"
#include "CbmTofTrackFinder.h"
#include "CbmTofTrackFitter.h"
#include "CbmStsTrackFinder.h"
#include "CbmStsTrackFitter.h"
#include "CbmMuchTrackFinder.h"
#include "CbmRichRingFinder.h"
#include "CbmGlobalTrackFitter.h"
#include "CbmPrimaryVertexFinder.h"
#include "CbmTofMerger.h"
#include "CbmTrackMerger.h"
#include "CbmRichMerger.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *CbmUnpackTaskBase_Dictionary();
   static void CbmUnpackTaskBase_TClassManip(TClass*);
   static void delete_CbmUnpackTaskBase(void *p);
   static void deleteArray_CbmUnpackTaskBase(void *p);
   static void destruct_CbmUnpackTaskBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmUnpackTaskBase*)
   {
      ::CbmUnpackTaskBase *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmUnpackTaskBase));
      static ::ROOT::TGenericClassInfo 
         instance("CbmUnpackTaskBase", "CbmUnpackTaskBase.h", 25,
                  typeid(::CbmUnpackTaskBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmUnpackTaskBase_Dictionary, isa_proxy, 4,
                  sizeof(::CbmUnpackTaskBase) );
      instance.SetDelete(&delete_CbmUnpackTaskBase);
      instance.SetDeleteArray(&deleteArray_CbmUnpackTaskBase);
      instance.SetDestructor(&destruct_CbmUnpackTaskBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmUnpackTaskBase*)
   {
      return GenerateInitInstanceLocal((::CbmUnpackTaskBase*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmUnpackTaskBase*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmUnpackTaskBase_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmUnpackTaskBase*)nullptr)->GetClass();
      CbmUnpackTaskBase_TClassManip(theClass);
   return theClass;
   }

   static void CbmUnpackTaskBase_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrdTrackFinder(void *p);
   static void deleteArray_CbmTrdTrackFinder(void *p);
   static void destruct_CbmTrdTrackFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdTrackFinder*)
   {
      ::CbmTrdTrackFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdTrackFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdTrackFinder", ::CbmTrdTrackFinder::Class_Version(), "CbmTrdTrackFinder.h", 28,
                  typeid(::CbmTrdTrackFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdTrackFinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdTrackFinder) );
      instance.SetDelete(&delete_CbmTrdTrackFinder);
      instance.SetDeleteArray(&deleteArray_CbmTrdTrackFinder);
      instance.SetDestructor(&destruct_CbmTrdTrackFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdTrackFinder*)
   {
      return GenerateInitInstanceLocal((::CbmTrdTrackFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdTrackFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrdTrackFitter(void *p);
   static void deleteArray_CbmTrdTrackFitter(void *p);
   static void destruct_CbmTrdTrackFitter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdTrackFitter*)
   {
      ::CbmTrdTrackFitter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdTrackFitter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdTrackFitter", ::CbmTrdTrackFitter::Class_Version(), "CbmTrdTrackFitter.h", 29,
                  typeid(::CbmTrdTrackFitter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdTrackFitter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdTrackFitter) );
      instance.SetDelete(&delete_CbmTrdTrackFitter);
      instance.SetDeleteArray(&deleteArray_CbmTrdTrackFitter);
      instance.SetDestructor(&destruct_CbmTrdTrackFitter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdTrackFitter*)
   {
      return GenerateInitInstanceLocal((::CbmTrdTrackFitter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdTrackFitter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTofTrackFinder(void *p);
   static void deleteArray_CbmTofTrackFinder(void *p);
   static void destruct_CbmTofTrackFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofTrackFinder*)
   {
      ::CbmTofTrackFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofTrackFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofTrackFinder", ::CbmTofTrackFinder::Class_Version(), "CbmTofTrackFinder.h", 31,
                  typeid(::CbmTofTrackFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofTrackFinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofTrackFinder) );
      instance.SetDelete(&delete_CbmTofTrackFinder);
      instance.SetDeleteArray(&deleteArray_CbmTofTrackFinder);
      instance.SetDestructor(&destruct_CbmTofTrackFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofTrackFinder*)
   {
      return GenerateInitInstanceLocal((::CbmTofTrackFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofTrackFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTofTrackFitter(void *p);
   static void deleteArray_CbmTofTrackFitter(void *p);
   static void destruct_CbmTofTrackFitter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofTrackFitter*)
   {
      ::CbmTofTrackFitter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofTrackFitter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofTrackFitter", ::CbmTofTrackFitter::Class_Version(), "CbmTofTrackFitter.h", 32,
                  typeid(::CbmTofTrackFitter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofTrackFitter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofTrackFitter) );
      instance.SetDelete(&delete_CbmTofTrackFitter);
      instance.SetDeleteArray(&deleteArray_CbmTofTrackFitter);
      instance.SetDestructor(&destruct_CbmTofTrackFitter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofTrackFitter*)
   {
      return GenerateInitInstanceLocal((::CbmTofTrackFitter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofTrackFitter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmStsTrackFinder(void *p);
   static void deleteArray_CbmStsTrackFinder(void *p);
   static void destruct_CbmStsTrackFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsTrackFinder*)
   {
      ::CbmStsTrackFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsTrackFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsTrackFinder", ::CbmStsTrackFinder::Class_Version(), "CbmStsTrackFinder.h", 34,
                  typeid(::CbmStsTrackFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsTrackFinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsTrackFinder) );
      instance.SetDelete(&delete_CbmStsTrackFinder);
      instance.SetDeleteArray(&deleteArray_CbmStsTrackFinder);
      instance.SetDestructor(&destruct_CbmStsTrackFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsTrackFinder*)
   {
      return GenerateInitInstanceLocal((::CbmStsTrackFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsTrackFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmStsTrackFitter(void *p);
   static void deleteArray_CbmStsTrackFitter(void *p);
   static void destruct_CbmStsTrackFitter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsTrackFitter*)
   {
      ::CbmStsTrackFitter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsTrackFitter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsTrackFitter", ::CbmStsTrackFitter::Class_Version(), "CbmStsTrackFitter.h", 30,
                  typeid(::CbmStsTrackFitter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsTrackFitter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsTrackFitter) );
      instance.SetDelete(&delete_CbmStsTrackFitter);
      instance.SetDeleteArray(&deleteArray_CbmStsTrackFitter);
      instance.SetDestructor(&destruct_CbmStsTrackFitter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsTrackFitter*)
   {
      return GenerateInitInstanceLocal((::CbmStsTrackFitter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsTrackFitter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmMuchTrackFinder(void *p);
   static void deleteArray_CbmMuchTrackFinder(void *p);
   static void destruct_CbmMuchTrackFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchTrackFinder*)
   {
      ::CbmMuchTrackFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchTrackFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchTrackFinder", ::CbmMuchTrackFinder::Class_Version(), "CbmMuchTrackFinder.h", 19,
                  typeid(::CbmMuchTrackFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchTrackFinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchTrackFinder) );
      instance.SetDelete(&delete_CbmMuchTrackFinder);
      instance.SetDeleteArray(&deleteArray_CbmMuchTrackFinder);
      instance.SetDestructor(&destruct_CbmMuchTrackFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchTrackFinder*)
   {
      return GenerateInitInstanceLocal((::CbmMuchTrackFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchTrackFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmRichRingFinder(void *p);
   static void deleteArray_CbmRichRingFinder(void *p);
   static void destruct_CbmRichRingFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichRingFinder*)
   {
      ::CbmRichRingFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichRingFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichRingFinder", ::CbmRichRingFinder::Class_Version(), "CbmRichRingFinder.h", 37,
                  typeid(::CbmRichRingFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichRingFinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichRingFinder) );
      instance.SetDelete(&delete_CbmRichRingFinder);
      instance.SetDeleteArray(&deleteArray_CbmRichRingFinder);
      instance.SetDestructor(&destruct_CbmRichRingFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichRingFinder*)
   {
      return GenerateInitInstanceLocal((::CbmRichRingFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichRingFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmGlobalTrackFitter(void *p);
   static void deleteArray_CbmGlobalTrackFitter(void *p);
   static void destruct_CbmGlobalTrackFitter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGlobalTrackFitter*)
   {
      ::CbmGlobalTrackFitter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGlobalTrackFitter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGlobalTrackFitter", ::CbmGlobalTrackFitter::Class_Version(), "CbmGlobalTrackFitter.h", 17,
                  typeid(::CbmGlobalTrackFitter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGlobalTrackFitter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGlobalTrackFitter) );
      instance.SetDelete(&delete_CbmGlobalTrackFitter);
      instance.SetDeleteArray(&deleteArray_CbmGlobalTrackFitter);
      instance.SetDestructor(&destruct_CbmGlobalTrackFitter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGlobalTrackFitter*)
   {
      return GenerateInitInstanceLocal((::CbmGlobalTrackFitter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGlobalTrackFitter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmPrimaryVertexFinder(void *p);
   static void deleteArray_CbmPrimaryVertexFinder(void *p);
   static void destruct_CbmPrimaryVertexFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPrimaryVertexFinder*)
   {
      ::CbmPrimaryVertexFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPrimaryVertexFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPrimaryVertexFinder", ::CbmPrimaryVertexFinder::Class_Version(), "CbmPrimaryVertexFinder.h", 33,
                  typeid(::CbmPrimaryVertexFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPrimaryVertexFinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPrimaryVertexFinder) );
      instance.SetDelete(&delete_CbmPrimaryVertexFinder);
      instance.SetDeleteArray(&deleteArray_CbmPrimaryVertexFinder);
      instance.SetDestructor(&destruct_CbmPrimaryVertexFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPrimaryVertexFinder*)
   {
      return GenerateInitInstanceLocal((::CbmPrimaryVertexFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPrimaryVertexFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTofMerger(void *p);
   static void deleteArray_CbmTofMerger(void *p);
   static void destruct_CbmTofMerger(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofMerger*)
   {
      ::CbmTofMerger *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofMerger >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofMerger", ::CbmTofMerger::Class_Version(), "CbmTofMerger.h", 17,
                  typeid(::CbmTofMerger), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofMerger::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofMerger) );
      instance.SetDelete(&delete_CbmTofMerger);
      instance.SetDeleteArray(&deleteArray_CbmTofMerger);
      instance.SetDestructor(&destruct_CbmTofMerger);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofMerger*)
   {
      return GenerateInitInstanceLocal((::CbmTofMerger*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofMerger*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrackMerger(void *p);
   static void deleteArray_CbmTrackMerger(void *p);
   static void destruct_CbmTrackMerger(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrackMerger*)
   {
      ::CbmTrackMerger *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrackMerger >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrackMerger", ::CbmTrackMerger::Class_Version(), "CbmTrackMerger.h", 29,
                  typeid(::CbmTrackMerger), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrackMerger::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrackMerger) );
      instance.SetDelete(&delete_CbmTrackMerger);
      instance.SetDeleteArray(&deleteArray_CbmTrackMerger);
      instance.SetDestructor(&destruct_CbmTrackMerger);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrackMerger*)
   {
      return GenerateInitInstanceLocal((::CbmTrackMerger*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrackMerger*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmRichMerger(void *p);
   static void deleteArray_CbmRichMerger(void *p);
   static void destruct_CbmRichMerger(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichMerger*)
   {
      ::CbmRichMerger *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichMerger >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichMerger", ::CbmRichMerger::Class_Version(), "CbmRichMerger.h", 29,
                  typeid(::CbmRichMerger), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichMerger::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichMerger) );
      instance.SetDelete(&delete_CbmRichMerger);
      instance.SetDeleteArray(&deleteArray_CbmRichMerger);
      instance.SetDestructor(&destruct_CbmRichMerger);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichMerger*)
   {
      return GenerateInitInstanceLocal((::CbmRichMerger*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichMerger*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdTrackFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdTrackFinder::Class_Name()
{
   return "CbmTrdTrackFinder";
}

//______________________________________________________________________________
const char *CbmTrdTrackFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdTrackFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdTrackFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdTrackFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdTrackFitter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdTrackFitter::Class_Name()
{
   return "CbmTrdTrackFitter";
}

//______________________________________________________________________________
const char *CbmTrdTrackFitter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFitter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdTrackFitter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFitter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdTrackFitter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFitter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdTrackFitter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFitter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofTrackFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofTrackFinder::Class_Name()
{
   return "CbmTofTrackFinder";
}

//______________________________________________________________________________
const char *CbmTofTrackFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofTrackFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofTrackFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofTrackFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofTrackFitter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofTrackFitter::Class_Name()
{
   return "CbmTofTrackFitter";
}

//______________________________________________________________________________
const char *CbmTofTrackFitter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFitter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofTrackFitter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFitter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofTrackFitter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFitter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofTrackFitter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFitter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsTrackFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsTrackFinder::Class_Name()
{
   return "CbmStsTrackFinder";
}

//______________________________________________________________________________
const char *CbmStsTrackFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsTrackFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsTrackFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsTrackFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsTrackFitter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsTrackFitter::Class_Name()
{
   return "CbmStsTrackFitter";
}

//______________________________________________________________________________
const char *CbmStsTrackFitter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackFitter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsTrackFitter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackFitter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsTrackFitter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackFitter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsTrackFitter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrackFitter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchTrackFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchTrackFinder::Class_Name()
{
   return "CbmMuchTrackFinder";
}

//______________________________________________________________________________
const char *CbmMuchTrackFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrackFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchTrackFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrackFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchTrackFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrackFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchTrackFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrackFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichRingFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichRingFinder::Class_Name()
{
   return "CbmRichRingFinder";
}

//______________________________________________________________________________
const char *CbmRichRingFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRingFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichRingFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRingFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichRingFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRingFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichRingFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRingFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGlobalTrackFitter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGlobalTrackFitter::Class_Name()
{
   return "CbmGlobalTrackFitter";
}

//______________________________________________________________________________
const char *CbmGlobalTrackFitter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackFitter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGlobalTrackFitter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackFitter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGlobalTrackFitter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackFitter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGlobalTrackFitter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackFitter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPrimaryVertexFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPrimaryVertexFinder::Class_Name()
{
   return "CbmPrimaryVertexFinder";
}

//______________________________________________________________________________
const char *CbmPrimaryVertexFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPrimaryVertexFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPrimaryVertexFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPrimaryVertexFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPrimaryVertexFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPrimaryVertexFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPrimaryVertexFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPrimaryVertexFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofMerger::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofMerger::Class_Name()
{
   return "CbmTofMerger";
}

//______________________________________________________________________________
const char *CbmTofMerger::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofMerger*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofMerger::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofMerger*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofMerger::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofMerger*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofMerger::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofMerger*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrackMerger::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrackMerger::Class_Name()
{
   return "CbmTrackMerger";
}

//______________________________________________________________________________
const char *CbmTrackMerger::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMerger*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrackMerger::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMerger*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrackMerger::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMerger*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrackMerger::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMerger*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichMerger::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichMerger::Class_Name()
{
   return "CbmRichMerger";
}

//______________________________________________________________________________
const char *CbmRichMerger::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMerger*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichMerger::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMerger*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichMerger::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMerger*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichMerger::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichMerger*)nullptr)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmUnpackTaskBase(void *p) {
      delete ((::CbmUnpackTaskBase*)p);
   }
   static void deleteArray_CbmUnpackTaskBase(void *p) {
      delete [] ((::CbmUnpackTaskBase*)p);
   }
   static void destruct_CbmUnpackTaskBase(void *p) {
      typedef ::CbmUnpackTaskBase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmUnpackTaskBase

//______________________________________________________________________________
void CbmTrdTrackFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdTrackFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdTrackFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdTrackFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrdTrackFinder(void *p) {
      delete ((::CbmTrdTrackFinder*)p);
   }
   static void deleteArray_CbmTrdTrackFinder(void *p) {
      delete [] ((::CbmTrdTrackFinder*)p);
   }
   static void destruct_CbmTrdTrackFinder(void *p) {
      typedef ::CbmTrdTrackFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdTrackFinder

//______________________________________________________________________________
void CbmTrdTrackFitter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdTrackFitter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdTrackFitter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdTrackFitter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrdTrackFitter(void *p) {
      delete ((::CbmTrdTrackFitter*)p);
   }
   static void deleteArray_CbmTrdTrackFitter(void *p) {
      delete [] ((::CbmTrdTrackFitter*)p);
   }
   static void destruct_CbmTrdTrackFitter(void *p) {
      typedef ::CbmTrdTrackFitter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdTrackFitter

//______________________________________________________________________________
void CbmTofTrackFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofTrackFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofTrackFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofTrackFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTofTrackFinder(void *p) {
      delete ((::CbmTofTrackFinder*)p);
   }
   static void deleteArray_CbmTofTrackFinder(void *p) {
      delete [] ((::CbmTofTrackFinder*)p);
   }
   static void destruct_CbmTofTrackFinder(void *p) {
      typedef ::CbmTofTrackFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofTrackFinder

//______________________________________________________________________________
void CbmTofTrackFitter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofTrackFitter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofTrackFitter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofTrackFitter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTofTrackFitter(void *p) {
      delete ((::CbmTofTrackFitter*)p);
   }
   static void deleteArray_CbmTofTrackFitter(void *p) {
      delete [] ((::CbmTofTrackFitter*)p);
   }
   static void destruct_CbmTofTrackFitter(void *p) {
      typedef ::CbmTofTrackFitter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofTrackFitter

//______________________________________________________________________________
void CbmStsTrackFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsTrackFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsTrackFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsTrackFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmStsTrackFinder(void *p) {
      delete ((::CbmStsTrackFinder*)p);
   }
   static void deleteArray_CbmStsTrackFinder(void *p) {
      delete [] ((::CbmStsTrackFinder*)p);
   }
   static void destruct_CbmStsTrackFinder(void *p) {
      typedef ::CbmStsTrackFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsTrackFinder

//______________________________________________________________________________
void CbmStsTrackFitter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsTrackFitter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsTrackFitter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsTrackFitter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmStsTrackFitter(void *p) {
      delete ((::CbmStsTrackFitter*)p);
   }
   static void deleteArray_CbmStsTrackFitter(void *p) {
      delete [] ((::CbmStsTrackFitter*)p);
   }
   static void destruct_CbmStsTrackFitter(void *p) {
      typedef ::CbmStsTrackFitter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsTrackFitter

//______________________________________________________________________________
void CbmMuchTrackFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchTrackFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchTrackFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchTrackFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmMuchTrackFinder(void *p) {
      delete ((::CbmMuchTrackFinder*)p);
   }
   static void deleteArray_CbmMuchTrackFinder(void *p) {
      delete [] ((::CbmMuchTrackFinder*)p);
   }
   static void destruct_CbmMuchTrackFinder(void *p) {
      typedef ::CbmMuchTrackFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchTrackFinder

//______________________________________________________________________________
void CbmRichRingFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichRingFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichRingFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichRingFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmRichRingFinder(void *p) {
      delete ((::CbmRichRingFinder*)p);
   }
   static void deleteArray_CbmRichRingFinder(void *p) {
      delete [] ((::CbmRichRingFinder*)p);
   }
   static void destruct_CbmRichRingFinder(void *p) {
      typedef ::CbmRichRingFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichRingFinder

//______________________________________________________________________________
void CbmGlobalTrackFitter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGlobalTrackFitter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGlobalTrackFitter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGlobalTrackFitter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmGlobalTrackFitter(void *p) {
      delete ((::CbmGlobalTrackFitter*)p);
   }
   static void deleteArray_CbmGlobalTrackFitter(void *p) {
      delete [] ((::CbmGlobalTrackFitter*)p);
   }
   static void destruct_CbmGlobalTrackFitter(void *p) {
      typedef ::CbmGlobalTrackFitter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGlobalTrackFitter

//______________________________________________________________________________
void CbmPrimaryVertexFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPrimaryVertexFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPrimaryVertexFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPrimaryVertexFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmPrimaryVertexFinder(void *p) {
      delete ((::CbmPrimaryVertexFinder*)p);
   }
   static void deleteArray_CbmPrimaryVertexFinder(void *p) {
      delete [] ((::CbmPrimaryVertexFinder*)p);
   }
   static void destruct_CbmPrimaryVertexFinder(void *p) {
      typedef ::CbmPrimaryVertexFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPrimaryVertexFinder

//______________________________________________________________________________
void CbmTofMerger::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofMerger.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofMerger::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofMerger::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTofMerger(void *p) {
      delete ((::CbmTofMerger*)p);
   }
   static void deleteArray_CbmTofMerger(void *p) {
      delete [] ((::CbmTofMerger*)p);
   }
   static void destruct_CbmTofMerger(void *p) {
      typedef ::CbmTofMerger current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofMerger

//______________________________________________________________________________
void CbmTrackMerger::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrackMerger.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrackMerger::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrackMerger::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrackMerger(void *p) {
      delete ((::CbmTrackMerger*)p);
   }
   static void deleteArray_CbmTrackMerger(void *p) {
      delete [] ((::CbmTrackMerger*)p);
   }
   static void destruct_CbmTrackMerger(void *p) {
      typedef ::CbmTrackMerger current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrackMerger

//______________________________________________________________________________
void CbmRichMerger::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichMerger.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichMerger::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichMerger::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmRichMerger(void *p) {
      delete ((::CbmRichMerger*)p);
   }
   static void deleteArray_CbmRichMerger(void *p) {
      delete [] ((::CbmRichMerger*)p);
   }
   static void destruct_CbmRichMerger(void *p) {
      typedef ::CbmRichMerger current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichMerger

namespace {
  void TriggerDictionaryInitialization_libCbmRecoBase_Impl() {
    static const char* headers[] = {
"CbmUnpackTaskBase.h",
"CbmTrdTrackFinder.h",
"CbmTrdTrackFitter.h",
"CbmTofTrackFinder.h",
"CbmTofTrackFitter.h",
"CbmStsTrackFinder.h",
"CbmStsTrackFitter.h",
"CbmMuchTrackFinder.h",
"CbmRichRingFinder.h",
"CbmGlobalTrackFitter.h",
"CbmPrimaryVertexFinder.h",
"CbmTofMerger.h",
"CbmTrackMerger.h",
"CbmRichMerger.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
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
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/base/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmRecoBase dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmUnpackTaskBase.h")))  CbmUnpackTaskBase;
class __attribute__((annotate("$clingAutoload$CbmTrdTrackFinder.h")))  CbmTrdTrackFinder;
class __attribute__((annotate("$clingAutoload$CbmTrdTrackFitter.h")))  CbmTrdTrackFitter;
class __attribute__((annotate("$clingAutoload$CbmTofTrackFinder.h")))  CbmTofTrackFinder;
class __attribute__((annotate("$clingAutoload$CbmTofTrackFitter.h")))  CbmTofTrackFitter;
class __attribute__((annotate("$clingAutoload$CbmStsTrackFinder.h")))  CbmStsTrackFinder;
class __attribute__((annotate("$clingAutoload$CbmStsTrackFitter.h")))  CbmStsTrackFitter;
class __attribute__((annotate("$clingAutoload$CbmMuchTrackFinder.h")))  CbmMuchTrackFinder;
class __attribute__((annotate("$clingAutoload$CbmRichRingFinder.h")))  CbmRichRingFinder;
class __attribute__((annotate("$clingAutoload$CbmGlobalTrackFitter.h")))  CbmGlobalTrackFitter;
class __attribute__((annotate("$clingAutoload$CbmPrimaryVertexFinder.h")))  CbmPrimaryVertexFinder;
class __attribute__((annotate("$clingAutoload$CbmTofMerger.h")))  CbmTofMerger;
class __attribute__((annotate("$clingAutoload$CbmTrackMerger.h")))  CbmTrackMerger;
class __attribute__((annotate("$clingAutoload$CbmRichMerger.h")))  CbmRichMerger;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmRecoBase dictionary payload"

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
#include "CbmUnpackTaskBase.h"
#include "CbmTrdTrackFinder.h"
#include "CbmTrdTrackFitter.h"
#include "CbmTofTrackFinder.h"
#include "CbmTofTrackFitter.h"
#include "CbmStsTrackFinder.h"
#include "CbmStsTrackFitter.h"
#include "CbmMuchTrackFinder.h"
#include "CbmRichRingFinder.h"
#include "CbmGlobalTrackFitter.h"
#include "CbmPrimaryVertexFinder.h"
#include "CbmTofMerger.h"
#include "CbmTrackMerger.h"
#include "CbmRichMerger.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmGlobalTrackFitter", payloadCode, "@",
"CbmMuchTrackFinder", payloadCode, "@",
"CbmPrimaryVertexFinder", payloadCode, "@",
"CbmRichMerger", payloadCode, "@",
"CbmRichRingFinder", payloadCode, "@",
"CbmStsTrackFinder", payloadCode, "@",
"CbmStsTrackFitter", payloadCode, "@",
"CbmTofMerger", payloadCode, "@",
"CbmTofTrackFinder", payloadCode, "@",
"CbmTofTrackFitter", payloadCode, "@",
"CbmTrackMerger", payloadCode, "@",
"CbmTrdTrackFinder", payloadCode, "@",
"CbmTrdTrackFitter", payloadCode, "@",
"CbmUnpackTaskBase", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmRecoBase",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmRecoBase_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmRecoBase_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmRecoBase() {
  TriggerDictionaryInitialization_libCbmRecoBase_Impl();
}
