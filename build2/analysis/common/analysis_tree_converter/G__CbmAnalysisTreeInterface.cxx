// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmAnalysisTreeInterface
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
#include "CbmConverterManager.h"
#include "CbmConverterTask.h"
#include "CbmSimEventHeaderConverter.h"
#include "CbmRecEventHeaderConverter.h"
#include "CbmStsTracksConverter.h"
#include "CbmSimTracksConverter.h"
#include "CbmPsdModulesConverter.h"
#include "CbmTofHitsConverter.h"
#include "CbmTrdTracksConverter.h"
#include "CbmRichRingsConverter.h"
#include "CbmMatchEvents.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmConverterManager(void *p = nullptr);
   static void *newArray_CbmConverterManager(Long_t size, void *p);
   static void delete_CbmConverterManager(void *p);
   static void deleteArray_CbmConverterManager(void *p);
   static void destruct_CbmConverterManager(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmConverterManager*)
   {
      ::CbmConverterManager *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmConverterManager >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmConverterManager", ::CbmConverterManager::Class_Version(), "CbmConverterManager.h", 24,
                  typeid(::CbmConverterManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmConverterManager::Dictionary, isa_proxy, 4,
                  sizeof(::CbmConverterManager) );
      instance.SetNew(&new_CbmConverterManager);
      instance.SetNewArray(&newArray_CbmConverterManager);
      instance.SetDelete(&delete_CbmConverterManager);
      instance.SetDeleteArray(&deleteArray_CbmConverterManager);
      instance.SetDestructor(&destruct_CbmConverterManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmConverterManager*)
   {
      return GenerateInitInstanceLocal((::CbmConverterManager*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmConverterManager*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmConverterTask_Dictionary();
   static void CbmConverterTask_TClassManip(TClass*);
   static void delete_CbmConverterTask(void *p);
   static void deleteArray_CbmConverterTask(void *p);
   static void destruct_CbmConverterTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmConverterTask*)
   {
      ::CbmConverterTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmConverterTask));
      static ::ROOT::TGenericClassInfo 
         instance("CbmConverterTask", "CbmConverterTask.h", 18,
                  typeid(::CbmConverterTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmConverterTask_Dictionary, isa_proxy, 4,
                  sizeof(::CbmConverterTask) );
      instance.SetDelete(&delete_CbmConverterTask);
      instance.SetDeleteArray(&deleteArray_CbmConverterTask);
      instance.SetDestructor(&destruct_CbmConverterTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmConverterTask*)
   {
      return GenerateInitInstanceLocal((::CbmConverterTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmConverterTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmConverterTask_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmConverterTask*)nullptr)->GetClass();
      CbmConverterTask_TClassManip(theClass);
   return theClass;
   }

   static void CbmConverterTask_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmSimEventHeaderConverter(void *p);
   static void deleteArray_CbmSimEventHeaderConverter(void *p);
   static void destruct_CbmSimEventHeaderConverter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmSimEventHeaderConverter*)
   {
      ::CbmSimEventHeaderConverter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmSimEventHeaderConverter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmSimEventHeaderConverter", ::CbmSimEventHeaderConverter::Class_Version(), "CbmSimEventHeaderConverter.h", 19,
                  typeid(::CbmSimEventHeaderConverter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmSimEventHeaderConverter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmSimEventHeaderConverter) );
      instance.SetDelete(&delete_CbmSimEventHeaderConverter);
      instance.SetDeleteArray(&deleteArray_CbmSimEventHeaderConverter);
      instance.SetDestructor(&destruct_CbmSimEventHeaderConverter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmSimEventHeaderConverter*)
   {
      return GenerateInitInstanceLocal((::CbmSimEventHeaderConverter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmSimEventHeaderConverter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmRecEventHeaderConverter(void *p);
   static void deleteArray_CbmRecEventHeaderConverter(void *p);
   static void destruct_CbmRecEventHeaderConverter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRecEventHeaderConverter*)
   {
      ::CbmRecEventHeaderConverter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRecEventHeaderConverter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRecEventHeaderConverter", ::CbmRecEventHeaderConverter::Class_Version(), "CbmRecEventHeaderConverter.h", 19,
                  typeid(::CbmRecEventHeaderConverter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRecEventHeaderConverter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRecEventHeaderConverter) );
      instance.SetDelete(&delete_CbmRecEventHeaderConverter);
      instance.SetDeleteArray(&deleteArray_CbmRecEventHeaderConverter);
      instance.SetDestructor(&destruct_CbmRecEventHeaderConverter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRecEventHeaderConverter*)
   {
      return GenerateInitInstanceLocal((::CbmRecEventHeaderConverter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRecEventHeaderConverter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmStsTracksConverter(void *p);
   static void deleteArray_CbmStsTracksConverter(void *p);
   static void destruct_CbmStsTracksConverter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsTracksConverter*)
   {
      ::CbmStsTracksConverter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsTracksConverter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsTracksConverter", ::CbmStsTracksConverter::Class_Version(), "CbmStsTracksConverter.h", 25,
                  typeid(::CbmStsTracksConverter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsTracksConverter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsTracksConverter) );
      instance.SetDelete(&delete_CbmStsTracksConverter);
      instance.SetDeleteArray(&deleteArray_CbmStsTracksConverter);
      instance.SetDestructor(&destruct_CbmStsTracksConverter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsTracksConverter*)
   {
      return GenerateInitInstanceLocal((::CbmStsTracksConverter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsTracksConverter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmSimTracksConverter(void *p);
   static void deleteArray_CbmSimTracksConverter(void *p);
   static void destruct_CbmSimTracksConverter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmSimTracksConverter*)
   {
      ::CbmSimTracksConverter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmSimTracksConverter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmSimTracksConverter", ::CbmSimTracksConverter::Class_Version(), "CbmSimTracksConverter.h", 22,
                  typeid(::CbmSimTracksConverter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmSimTracksConverter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmSimTracksConverter) );
      instance.SetDelete(&delete_CbmSimTracksConverter);
      instance.SetDeleteArray(&deleteArray_CbmSimTracksConverter);
      instance.SetDestructor(&destruct_CbmSimTracksConverter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmSimTracksConverter*)
   {
      return GenerateInitInstanceLocal((::CbmSimTracksConverter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmSimTracksConverter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmPsdModulesConverter(void *p);
   static void deleteArray_CbmPsdModulesConverter(void *p);
   static void destruct_CbmPsdModulesConverter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdModulesConverter*)
   {
      ::CbmPsdModulesConverter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdModulesConverter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdModulesConverter", ::CbmPsdModulesConverter::Class_Version(), "CbmPsdModulesConverter.h", 14,
                  typeid(::CbmPsdModulesConverter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdModulesConverter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPsdModulesConverter) );
      instance.SetDelete(&delete_CbmPsdModulesConverter);
      instance.SetDeleteArray(&deleteArray_CbmPsdModulesConverter);
      instance.SetDestructor(&destruct_CbmPsdModulesConverter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdModulesConverter*)
   {
      return GenerateInitInstanceLocal((::CbmPsdModulesConverter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdModulesConverter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTofHitsConverter(void *p);
   static void deleteArray_CbmTofHitsConverter(void *p);
   static void destruct_CbmTofHitsConverter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofHitsConverter*)
   {
      ::CbmTofHitsConverter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofHitsConverter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofHitsConverter", ::CbmTofHitsConverter::Class_Version(), "CbmTofHitsConverter.h", 22,
                  typeid(::CbmTofHitsConverter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofHitsConverter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofHitsConverter) );
      instance.SetDelete(&delete_CbmTofHitsConverter);
      instance.SetDeleteArray(&deleteArray_CbmTofHitsConverter);
      instance.SetDestructor(&destruct_CbmTofHitsConverter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofHitsConverter*)
   {
      return GenerateInitInstanceLocal((::CbmTofHitsConverter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofHitsConverter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmTrdTracksConverter(void *p);
   static void deleteArray_CbmTrdTracksConverter(void *p);
   static void destruct_CbmTrdTracksConverter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdTracksConverter*)
   {
      ::CbmTrdTracksConverter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdTracksConverter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdTracksConverter", ::CbmTrdTracksConverter::Class_Version(), "CbmTrdTracksConverter.h", 19,
                  typeid(::CbmTrdTracksConverter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdTracksConverter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdTracksConverter) );
      instance.SetDelete(&delete_CbmTrdTracksConverter);
      instance.SetDeleteArray(&deleteArray_CbmTrdTracksConverter);
      instance.SetDestructor(&destruct_CbmTrdTracksConverter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdTracksConverter*)
   {
      return GenerateInitInstanceLocal((::CbmTrdTracksConverter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdTracksConverter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmRichRingsConverter(void *p);
   static void deleteArray_CbmRichRingsConverter(void *p);
   static void destruct_CbmRichRingsConverter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichRingsConverter*)
   {
      ::CbmRichRingsConverter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichRingsConverter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichRingsConverter", ::CbmRichRingsConverter::Class_Version(), "CbmRichRingsConverter.h", 19,
                  typeid(::CbmRichRingsConverter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichRingsConverter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichRingsConverter) );
      instance.SetDelete(&delete_CbmRichRingsConverter);
      instance.SetDeleteArray(&deleteArray_CbmRichRingsConverter);
      instance.SetDestructor(&destruct_CbmRichRingsConverter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichRingsConverter*)
   {
      return GenerateInitInstanceLocal((::CbmRichRingsConverter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichRingsConverter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmConverterManager::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmConverterManager::Class_Name()
{
   return "CbmConverterManager";
}

//______________________________________________________________________________
const char *CbmConverterManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmConverterManager*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmConverterManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmConverterManager*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmConverterManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmConverterManager*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmConverterManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmConverterManager*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmSimEventHeaderConverter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmSimEventHeaderConverter::Class_Name()
{
   return "CbmSimEventHeaderConverter";
}

//______________________________________________________________________________
const char *CbmSimEventHeaderConverter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmSimEventHeaderConverter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmSimEventHeaderConverter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmSimEventHeaderConverter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmSimEventHeaderConverter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmSimEventHeaderConverter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmSimEventHeaderConverter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmSimEventHeaderConverter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRecEventHeaderConverter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRecEventHeaderConverter::Class_Name()
{
   return "CbmRecEventHeaderConverter";
}

//______________________________________________________________________________
const char *CbmRecEventHeaderConverter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecEventHeaderConverter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRecEventHeaderConverter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRecEventHeaderConverter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRecEventHeaderConverter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecEventHeaderConverter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRecEventHeaderConverter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRecEventHeaderConverter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsTracksConverter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsTracksConverter::Class_Name()
{
   return "CbmStsTracksConverter";
}

//______________________________________________________________________________
const char *CbmStsTracksConverter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTracksConverter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsTracksConverter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTracksConverter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsTracksConverter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTracksConverter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsTracksConverter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTracksConverter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmSimTracksConverter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmSimTracksConverter::Class_Name()
{
   return "CbmSimTracksConverter";
}

//______________________________________________________________________________
const char *CbmSimTracksConverter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmSimTracksConverter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmSimTracksConverter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmSimTracksConverter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmSimTracksConverter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmSimTracksConverter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmSimTracksConverter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmSimTracksConverter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdModulesConverter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdModulesConverter::Class_Name()
{
   return "CbmPsdModulesConverter";
}

//______________________________________________________________________________
const char *CbmPsdModulesConverter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdModulesConverter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdModulesConverter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdModulesConverter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdModulesConverter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdModulesConverter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdModulesConverter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdModulesConverter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofHitsConverter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofHitsConverter::Class_Name()
{
   return "CbmTofHitsConverter";
}

//______________________________________________________________________________
const char *CbmTofHitsConverter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitsConverter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofHitsConverter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitsConverter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofHitsConverter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitsConverter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofHitsConverter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHitsConverter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdTracksConverter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdTracksConverter::Class_Name()
{
   return "CbmTrdTracksConverter";
}

//______________________________________________________________________________
const char *CbmTrdTracksConverter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTracksConverter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdTracksConverter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTracksConverter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdTracksConverter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTracksConverter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdTracksConverter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTracksConverter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichRingsConverter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichRingsConverter::Class_Name()
{
   return "CbmRichRingsConverter";
}

//______________________________________________________________________________
const char *CbmRichRingsConverter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRingsConverter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichRingsConverter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRingsConverter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichRingsConverter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRingsConverter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichRingsConverter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRingsConverter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmConverterManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmConverterManager.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmConverterManager::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmConverterManager::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmConverterManager(void *p) {
      return  p ? new(p) ::CbmConverterManager : new ::CbmConverterManager;
   }
   static void *newArray_CbmConverterManager(Long_t nElements, void *p) {
      return p ? new(p) ::CbmConverterManager[nElements] : new ::CbmConverterManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmConverterManager(void *p) {
      delete ((::CbmConverterManager*)p);
   }
   static void deleteArray_CbmConverterManager(void *p) {
      delete [] ((::CbmConverterManager*)p);
   }
   static void destruct_CbmConverterManager(void *p) {
      typedef ::CbmConverterManager current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmConverterManager

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmConverterTask(void *p) {
      delete ((::CbmConverterTask*)p);
   }
   static void deleteArray_CbmConverterTask(void *p) {
      delete [] ((::CbmConverterTask*)p);
   }
   static void destruct_CbmConverterTask(void *p) {
      typedef ::CbmConverterTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmConverterTask

//______________________________________________________________________________
void CbmSimEventHeaderConverter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmSimEventHeaderConverter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmSimEventHeaderConverter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmSimEventHeaderConverter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmSimEventHeaderConverter(void *p) {
      delete ((::CbmSimEventHeaderConverter*)p);
   }
   static void deleteArray_CbmSimEventHeaderConverter(void *p) {
      delete [] ((::CbmSimEventHeaderConverter*)p);
   }
   static void destruct_CbmSimEventHeaderConverter(void *p) {
      typedef ::CbmSimEventHeaderConverter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmSimEventHeaderConverter

//______________________________________________________________________________
void CbmRecEventHeaderConverter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRecEventHeaderConverter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRecEventHeaderConverter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRecEventHeaderConverter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmRecEventHeaderConverter(void *p) {
      delete ((::CbmRecEventHeaderConverter*)p);
   }
   static void deleteArray_CbmRecEventHeaderConverter(void *p) {
      delete [] ((::CbmRecEventHeaderConverter*)p);
   }
   static void destruct_CbmRecEventHeaderConverter(void *p) {
      typedef ::CbmRecEventHeaderConverter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRecEventHeaderConverter

//______________________________________________________________________________
void CbmStsTracksConverter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsTracksConverter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsTracksConverter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsTracksConverter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmStsTracksConverter(void *p) {
      delete ((::CbmStsTracksConverter*)p);
   }
   static void deleteArray_CbmStsTracksConverter(void *p) {
      delete [] ((::CbmStsTracksConverter*)p);
   }
   static void destruct_CbmStsTracksConverter(void *p) {
      typedef ::CbmStsTracksConverter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsTracksConverter

//______________________________________________________________________________
void CbmSimTracksConverter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmSimTracksConverter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmSimTracksConverter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmSimTracksConverter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmSimTracksConverter(void *p) {
      delete ((::CbmSimTracksConverter*)p);
   }
   static void deleteArray_CbmSimTracksConverter(void *p) {
      delete [] ((::CbmSimTracksConverter*)p);
   }
   static void destruct_CbmSimTracksConverter(void *p) {
      typedef ::CbmSimTracksConverter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmSimTracksConverter

//______________________________________________________________________________
void CbmPsdModulesConverter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdModulesConverter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPsdModulesConverter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPsdModulesConverter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmPsdModulesConverter(void *p) {
      delete ((::CbmPsdModulesConverter*)p);
   }
   static void deleteArray_CbmPsdModulesConverter(void *p) {
      delete [] ((::CbmPsdModulesConverter*)p);
   }
   static void destruct_CbmPsdModulesConverter(void *p) {
      typedef ::CbmPsdModulesConverter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPsdModulesConverter

//______________________________________________________________________________
void CbmTofHitsConverter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofHitsConverter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofHitsConverter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofHitsConverter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTofHitsConverter(void *p) {
      delete ((::CbmTofHitsConverter*)p);
   }
   static void deleteArray_CbmTofHitsConverter(void *p) {
      delete [] ((::CbmTofHitsConverter*)p);
   }
   static void destruct_CbmTofHitsConverter(void *p) {
      typedef ::CbmTofHitsConverter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofHitsConverter

//______________________________________________________________________________
void CbmTrdTracksConverter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdTracksConverter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdTracksConverter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdTracksConverter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTrdTracksConverter(void *p) {
      delete ((::CbmTrdTracksConverter*)p);
   }
   static void deleteArray_CbmTrdTracksConverter(void *p) {
      delete [] ((::CbmTrdTracksConverter*)p);
   }
   static void destruct_CbmTrdTracksConverter(void *p) {
      typedef ::CbmTrdTracksConverter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdTracksConverter

//______________________________________________________________________________
void CbmRichRingsConverter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichRingsConverter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichRingsConverter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichRingsConverter::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmRichRingsConverter(void *p) {
      delete ((::CbmRichRingsConverter*)p);
   }
   static void deleteArray_CbmRichRingsConverter(void *p) {
      delete [] ((::CbmRichRingsConverter*)p);
   }
   static void destruct_CbmRichRingsConverter(void *p) {
      typedef ::CbmRichRingsConverter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichRingsConverter

namespace ROOT {
   static TClass *vectorlECbmConverterTaskmUgR_Dictionary();
   static void vectorlECbmConverterTaskmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmConverterTaskmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmConverterTaskmUgR(Long_t size, void *p);
   static void delete_vectorlECbmConverterTaskmUgR(void *p);
   static void deleteArray_vectorlECbmConverterTaskmUgR(void *p);
   static void destruct_vectorlECbmConverterTaskmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmConverterTask*>*)
   {
      vector<CbmConverterTask*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmConverterTask*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmConverterTask*>", -2, "vector", 389,
                  typeid(vector<CbmConverterTask*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmConverterTaskmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmConverterTask*>) );
      instance.SetNew(&new_vectorlECbmConverterTaskmUgR);
      instance.SetNewArray(&newArray_vectorlECbmConverterTaskmUgR);
      instance.SetDelete(&delete_vectorlECbmConverterTaskmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmConverterTaskmUgR);
      instance.SetDestructor(&destruct_vectorlECbmConverterTaskmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmConverterTask*> >()));

      ::ROOT::AddClassAlternate("vector<CbmConverterTask*>","std::vector<CbmConverterTask*, std::allocator<CbmConverterTask*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmConverterTask*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmConverterTaskmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmConverterTask*>*)nullptr)->GetClass();
      vectorlECbmConverterTaskmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmConverterTaskmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmConverterTaskmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmConverterTask*> : new vector<CbmConverterTask*>;
   }
   static void *newArray_vectorlECbmConverterTaskmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmConverterTask*>[nElements] : new vector<CbmConverterTask*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmConverterTaskmUgR(void *p) {
      delete ((vector<CbmConverterTask*>*)p);
   }
   static void deleteArray_vectorlECbmConverterTaskmUgR(void *p) {
      delete [] ((vector<CbmConverterTask*>*)p);
   }
   static void destruct_vectorlECbmConverterTaskmUgR(void *p) {
      typedef vector<CbmConverterTask*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmConverterTask*>

namespace ROOT {
   static TClass *maplEstringcOmaplEintcOintgRsPgR_Dictionary();
   static void maplEstringcOmaplEintcOintgRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOmaplEintcOintgRsPgR(void *p = nullptr);
   static void *newArray_maplEstringcOmaplEintcOintgRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOmaplEintcOintgRsPgR(void *p);
   static void deleteArray_maplEstringcOmaplEintcOintgRsPgR(void *p);
   static void destruct_maplEstringcOmaplEintcOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,map<int,int> >*)
   {
      map<string,map<int,int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,map<int,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,map<int,int> >", -2, "map", 100,
                  typeid(map<string,map<int,int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOmaplEintcOintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,map<int,int> >) );
      instance.SetNew(&new_maplEstringcOmaplEintcOintgRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOmaplEintcOintgRsPgR);
      instance.SetDelete(&delete_maplEstringcOmaplEintcOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOmaplEintcOintgRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOmaplEintcOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,map<int,int> > >()));

      ::ROOT::AddClassAlternate("map<string,map<int,int> >","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,map<int,int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOmaplEintcOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,map<int,int> >*)nullptr)->GetClass();
      maplEstringcOmaplEintcOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOmaplEintcOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOmaplEintcOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,map<int,int> > : new map<string,map<int,int> >;
   }
   static void *newArray_maplEstringcOmaplEintcOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,map<int,int> >[nElements] : new map<string,map<int,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOmaplEintcOintgRsPgR(void *p) {
      delete ((map<string,map<int,int> >*)p);
   }
   static void deleteArray_maplEstringcOmaplEintcOintgRsPgR(void *p) {
      delete [] ((map<string,map<int,int> >*)p);
   }
   static void destruct_maplEstringcOmaplEintcOintgRsPgR(void *p) {
      typedef map<string,map<int,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,map<int,int> >

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

namespace {
  void TriggerDictionaryInitialization_libCbmAnalysisTreeInterface_Impl() {
    static const char* headers[] = {
"CbmConverterManager.h",
"CbmConverterTask.h",
"CbmSimEventHeaderConverter.h",
"CbmRecEventHeaderConverter.h",
"CbmStsTracksConverter.h",
"CbmSimTracksConverter.h",
"CbmPsdModulesConverter.h",
"CbmTofHitsConverter.h",
"CbmTrdTracksConverter.h",
"CbmRichRingsConverter.h",
"CbmMatchEvents.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/heinemann/gpuunpacker/cbmroot/build2/include",
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
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/heinemann/gpuunpacker/cbmroot/core/base",
"/home/heinemann/gpuunpacker/cbmroot/core/base/report",
"/home/heinemann/gpuunpacker/cbmroot/core/base/draw",
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca",
"/home/heinemann/gpuunpacker/cbmroot/algo/ca/simd",
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
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators/unigen",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators/pluto",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/common/analysis_tree_converter/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmAnalysisTreeInterface dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmConverterManager.h")))  CbmConverterManager;
class __attribute__((annotate("$clingAutoload$CbmConverterTask.h")))  CbmConverterTask;
class __attribute__((annotate("$clingAutoload$CbmSimEventHeaderConverter.h")))  CbmSimEventHeaderConverter;
class __attribute__((annotate("$clingAutoload$CbmRecEventHeaderConverter.h")))  CbmRecEventHeaderConverter;
class __attribute__((annotate("$clingAutoload$CbmStsTracksConverter.h")))  CbmStsTracksConverter;
class __attribute__((annotate("$clingAutoload$CbmSimTracksConverter.h")))  CbmSimTracksConverter;
class __attribute__((annotate("$clingAutoload$CbmPsdModulesConverter.h")))  CbmPsdModulesConverter;
class __attribute__((annotate("$clingAutoload$CbmTofHitsConverter.h")))  CbmTofHitsConverter;
class __attribute__((annotate("$clingAutoload$CbmTrdTracksConverter.h")))  CbmTrdTracksConverter;
class __attribute__((annotate("$clingAutoload$CbmRichRingsConverter.h")))  CbmRichRingsConverter;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmAnalysisTreeInterface dictionary payload"

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
#include "CbmConverterManager.h"
#include "CbmConverterTask.h"
#include "CbmSimEventHeaderConverter.h"
#include "CbmRecEventHeaderConverter.h"
#include "CbmStsTracksConverter.h"
#include "CbmSimTracksConverter.h"
#include "CbmPsdModulesConverter.h"
#include "CbmTofHitsConverter.h"
#include "CbmTrdTracksConverter.h"
#include "CbmRichRingsConverter.h"
#include "CbmMatchEvents.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmConverterManager", payloadCode, "@",
"CbmConverterTask", payloadCode, "@",
"CbmPsdModulesConverter", payloadCode, "@",
"CbmRecEventHeaderConverter", payloadCode, "@",
"CbmRichRingsConverter", payloadCode, "@",
"CbmSimEventHeaderConverter", payloadCode, "@",
"CbmSimTracksConverter", payloadCode, "@",
"CbmStsTracksConverter", payloadCode, "@",
"CbmTofHitsConverter", payloadCode, "@",
"CbmTrdTracksConverter", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmAnalysisTreeInterface",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmAnalysisTreeInterface_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmAnalysisTreeInterface_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmAnalysisTreeInterface() {
  TriggerDictionaryInitialization_libCbmAnalysisTreeInterface_Impl();
}
