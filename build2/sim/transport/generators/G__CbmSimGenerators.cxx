// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmSimGenerators
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
#include "CbmBeamGenerator.h"
#include "CbmPhsdGenerator.h"
#include "CbmPlutoGenerator.h"
#include "CbmShieldGenerator.h"
#include "CbmUnigenGenerator.h"
#include "URun.h"
#include "UEvent.h"
#include "UParticle.h"
#include "PDataBase.h"
#include "PMesh.h"
#include "PParticle.h"
#include "PStaticData.h"
#include "PStdData.h"
#include "PValues.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmBeamGenerator(void *p = nullptr);
   static void *newArray_CbmBeamGenerator(Long_t size, void *p);
   static void delete_CbmBeamGenerator(void *p);
   static void deleteArray_CbmBeamGenerator(void *p);
   static void destruct_CbmBeamGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBeamGenerator*)
   {
      ::CbmBeamGenerator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBeamGenerator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBeamGenerator", ::CbmBeamGenerator::Class_Version(), "CbmBeamGenerator.h", 44,
                  typeid(::CbmBeamGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBeamGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBeamGenerator) );
      instance.SetNew(&new_CbmBeamGenerator);
      instance.SetNewArray(&newArray_CbmBeamGenerator);
      instance.SetDelete(&delete_CbmBeamGenerator);
      instance.SetDeleteArray(&deleteArray_CbmBeamGenerator);
      instance.SetDestructor(&destruct_CbmBeamGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBeamGenerator*)
   {
      return GenerateInitInstanceLocal((::CbmBeamGenerator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBeamGenerator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPhsdGenerator(void *p = nullptr);
   static void *newArray_CbmPhsdGenerator(Long_t size, void *p);
   static void delete_CbmPhsdGenerator(void *p);
   static void deleteArray_CbmPhsdGenerator(void *p);
   static void destruct_CbmPhsdGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPhsdGenerator*)
   {
      ::CbmPhsdGenerator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPhsdGenerator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPhsdGenerator", ::CbmPhsdGenerator::Class_Version(), "CbmPhsdGenerator.h", 47,
                  typeid(::CbmPhsdGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPhsdGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPhsdGenerator) );
      instance.SetNew(&new_CbmPhsdGenerator);
      instance.SetNewArray(&newArray_CbmPhsdGenerator);
      instance.SetDelete(&delete_CbmPhsdGenerator);
      instance.SetDeleteArray(&deleteArray_CbmPhsdGenerator);
      instance.SetDestructor(&destruct_CbmPhsdGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPhsdGenerator*)
   {
      return GenerateInitInstanceLocal((::CbmPhsdGenerator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPhsdGenerator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PValues(void *p = nullptr);
   static void *newArray_PValues(Long_t size, void *p);
   static void delete_PValues(void *p);
   static void deleteArray_PValues(void *p);
   static void destruct_PValues(void *p);
   static void streamer_PValues(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PValues*)
   {
      ::PValues *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PValues >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PValues", ::PValues::Class_Version(), "PValues.h", 22,
                  typeid(::PValues), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PValues::Dictionary, isa_proxy, 16,
                  sizeof(::PValues) );
      instance.SetNew(&new_PValues);
      instance.SetNewArray(&newArray_PValues);
      instance.SetDelete(&delete_PValues);
      instance.SetDeleteArray(&deleteArray_PValues);
      instance.SetDestructor(&destruct_PValues);
      instance.SetStreamerFunc(&streamer_PValues);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PValues*)
   {
      return GenerateInitInstanceLocal((::PValues*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PValues*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PParticle(void *p = nullptr);
   static void *newArray_PParticle(Long_t size, void *p);
   static void delete_PParticle(void *p);
   static void deleteArray_PParticle(void *p);
   static void destruct_PParticle(void *p);
   static void streamer_PParticle(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PParticle*)
   {
      ::PParticle *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PParticle >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PParticle", ::PParticle::Class_Version(), "PParticle.h", 28,
                  typeid(::PParticle), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PParticle::Dictionary, isa_proxy, 16,
                  sizeof(::PParticle) );
      instance.SetNew(&new_PParticle);
      instance.SetNewArray(&newArray_PParticle);
      instance.SetDelete(&delete_PParticle);
      instance.SetDeleteArray(&deleteArray_PParticle);
      instance.SetDestructor(&destruct_PParticle);
      instance.SetStreamerFunc(&streamer_PParticle);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PParticle*)
   {
      return GenerateInitInstanceLocal((::PParticle*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PParticle*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PDataBase(void *p = nullptr);
   static void *newArray_PDataBase(Long_t size, void *p);
   static void delete_PDataBase(void *p);
   static void deleteArray_PDataBase(void *p);
   static void destruct_PDataBase(void *p);
   static void streamer_PDataBase(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PDataBase*)
   {
      ::PDataBase *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PDataBase >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PDataBase", ::PDataBase::Class_Version(), "PDataBase.h", 33,
                  typeid(::PDataBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PDataBase::Dictionary, isa_proxy, 16,
                  sizeof(::PDataBase) );
      instance.SetNew(&new_PDataBase);
      instance.SetNewArray(&newArray_PDataBase);
      instance.SetDelete(&delete_PDataBase);
      instance.SetDeleteArray(&deleteArray_PDataBase);
      instance.SetDestructor(&destruct_PDataBase);
      instance.SetStreamerFunc(&streamer_PDataBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PDataBase*)
   {
      return GenerateInitInstanceLocal((::PDataBase*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PDataBase*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_PMesh(void *p);
   static void deleteArray_PMesh(void *p);
   static void destruct_PMesh(void *p);
   static void streamer_PMesh(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PMesh*)
   {
      ::PMesh *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PMesh >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PMesh", ::PMesh::Class_Version(), "PMesh.h", 13,
                  typeid(::PMesh), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PMesh::Dictionary, isa_proxy, 16,
                  sizeof(::PMesh) );
      instance.SetDelete(&delete_PMesh);
      instance.SetDeleteArray(&deleteArray_PMesh);
      instance.SetDestructor(&destruct_PMesh);
      instance.SetStreamerFunc(&streamer_PMesh);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PMesh*)
   {
      return GenerateInitInstanceLocal((::PMesh*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PMesh*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PStaticData(void *p = nullptr);
   static void *newArray_PStaticData(Long_t size, void *p);
   static void delete_PStaticData(void *p);
   static void deleteArray_PStaticData(void *p);
   static void destruct_PStaticData(void *p);
   static void streamer_PStaticData(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PStaticData*)
   {
      ::PStaticData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PStaticData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PStaticData", ::PStaticData::Class_Version(), "PStaticData.h", 37,
                  typeid(::PStaticData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PStaticData::Dictionary, isa_proxy, 16,
                  sizeof(::PStaticData) );
      instance.SetNew(&new_PStaticData);
      instance.SetNewArray(&newArray_PStaticData);
      instance.SetDelete(&delete_PStaticData);
      instance.SetDeleteArray(&deleteArray_PStaticData);
      instance.SetDestructor(&destruct_PStaticData);
      instance.SetStreamerFunc(&streamer_PStaticData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PStaticData*)
   {
      return GenerateInitInstanceLocal((::PStaticData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PStaticData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPlutoGenerator(void *p = nullptr);
   static void *newArray_CbmPlutoGenerator(Long_t size, void *p);
   static void delete_CbmPlutoGenerator(void *p);
   static void deleteArray_CbmPlutoGenerator(void *p);
   static void destruct_CbmPlutoGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPlutoGenerator*)
   {
      ::CbmPlutoGenerator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPlutoGenerator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPlutoGenerator", ::CbmPlutoGenerator::Class_Version(), "CbmPlutoGenerator.h", 38,
                  typeid(::CbmPlutoGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPlutoGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPlutoGenerator) );
      instance.SetNew(&new_CbmPlutoGenerator);
      instance.SetNewArray(&newArray_CbmPlutoGenerator);
      instance.SetDelete(&delete_CbmPlutoGenerator);
      instance.SetDeleteArray(&deleteArray_CbmPlutoGenerator);
      instance.SetDestructor(&destruct_CbmPlutoGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPlutoGenerator*)
   {
      return GenerateInitInstanceLocal((::CbmPlutoGenerator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPlutoGenerator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmShieldGenerator(void *p = nullptr);
   static void *newArray_CbmShieldGenerator(Long_t size, void *p);
   static void delete_CbmShieldGenerator(void *p);
   static void deleteArray_CbmShieldGenerator(void *p);
   static void destruct_CbmShieldGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmShieldGenerator*)
   {
      ::CbmShieldGenerator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmShieldGenerator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmShieldGenerator", ::CbmShieldGenerator::Class_Version(), "CbmShieldGenerator.h", 42,
                  typeid(::CbmShieldGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmShieldGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::CbmShieldGenerator) );
      instance.SetNew(&new_CbmShieldGenerator);
      instance.SetNewArray(&newArray_CbmShieldGenerator);
      instance.SetDelete(&delete_CbmShieldGenerator);
      instance.SetDeleteArray(&deleteArray_CbmShieldGenerator);
      instance.SetDestructor(&destruct_CbmShieldGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmShieldGenerator*)
   {
      return GenerateInitInstanceLocal((::CbmShieldGenerator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmShieldGenerator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_UEvent(void *p = nullptr);
   static void *newArray_UEvent(Long_t size, void *p);
   static void delete_UEvent(void *p);
   static void deleteArray_UEvent(void *p);
   static void destruct_UEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::UEvent*)
   {
      ::UEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::UEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("UEvent", ::UEvent::Class_Version(), "UEvent.h", 17,
                  typeid(::UEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::UEvent::Dictionary, isa_proxy, 4,
                  sizeof(::UEvent) );
      instance.SetNew(&new_UEvent);
      instance.SetNewArray(&newArray_UEvent);
      instance.SetDelete(&delete_UEvent);
      instance.SetDeleteArray(&deleteArray_UEvent);
      instance.SetDestructor(&destruct_UEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::UEvent*)
   {
      return GenerateInitInstanceLocal((::UEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::UEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmUnigenGenerator(void *p = nullptr);
   static void *newArray_CbmUnigenGenerator(Long_t size, void *p);
   static void delete_CbmUnigenGenerator(void *p);
   static void deleteArray_CbmUnigenGenerator(void *p);
   static void destruct_CbmUnigenGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmUnigenGenerator*)
   {
      ::CbmUnigenGenerator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmUnigenGenerator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmUnigenGenerator", ::CbmUnigenGenerator::Class_Version(), "CbmUnigenGenerator.h", 51,
                  typeid(::CbmUnigenGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmUnigenGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::CbmUnigenGenerator) );
      instance.SetNew(&new_CbmUnigenGenerator);
      instance.SetNewArray(&newArray_CbmUnigenGenerator);
      instance.SetDelete(&delete_CbmUnigenGenerator);
      instance.SetDeleteArray(&deleteArray_CbmUnigenGenerator);
      instance.SetDestructor(&destruct_CbmUnigenGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmUnigenGenerator*)
   {
      return GenerateInitInstanceLocal((::CbmUnigenGenerator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmUnigenGenerator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_URun(void *p = nullptr);
   static void *newArray_URun(Long_t size, void *p);
   static void delete_URun(void *p);
   static void deleteArray_URun(void *p);
   static void destruct_URun(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::URun*)
   {
      ::URun *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::URun >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("URun", ::URun::Class_Version(), "URun.h", 12,
                  typeid(::URun), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::URun::Dictionary, isa_proxy, 4,
                  sizeof(::URun) );
      instance.SetNew(&new_URun);
      instance.SetNewArray(&newArray_URun);
      instance.SetDelete(&delete_URun);
      instance.SetDeleteArray(&deleteArray_URun);
      instance.SetDestructor(&destruct_URun);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::URun*)
   {
      return GenerateInitInstanceLocal((::URun*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::URun*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_UParticle(void *p = nullptr);
   static void *newArray_UParticle(Long_t size, void *p);
   static void delete_UParticle(void *p);
   static void deleteArray_UParticle(void *p);
   static void destruct_UParticle(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::UParticle*)
   {
      ::UParticle *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::UParticle >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("UParticle", ::UParticle::Class_Version(), "UParticle.h", 14,
                  typeid(::UParticle), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::UParticle::Dictionary, isa_proxy, 4,
                  sizeof(::UParticle) );
      instance.SetNew(&new_UParticle);
      instance.SetNewArray(&newArray_UParticle);
      instance.SetDelete(&delete_UParticle);
      instance.SetDeleteArray(&deleteArray_UParticle);
      instance.SetDestructor(&destruct_UParticle);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::UParticle*)
   {
      return GenerateInitInstanceLocal((::UParticle*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::UParticle*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PStdData(void *p = nullptr);
   static void *newArray_PStdData(Long_t size, void *p);
   static void delete_PStdData(void *p);
   static void deleteArray_PStdData(void *p);
   static void destruct_PStdData(void *p);
   static void streamer_PStdData(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PStdData*)
   {
      ::PStdData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PStdData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PStdData", ::PStdData::Class_Version(), "PStdData.h", 29,
                  typeid(::PStdData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PStdData::Dictionary, isa_proxy, 16,
                  sizeof(::PStdData) );
      instance.SetNew(&new_PStdData);
      instance.SetNewArray(&newArray_PStdData);
      instance.SetDelete(&delete_PStdData);
      instance.SetDeleteArray(&deleteArray_PStdData);
      instance.SetDestructor(&destruct_PStdData);
      instance.SetStreamerFunc(&streamer_PStdData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PStdData*)
   {
      return GenerateInitInstanceLocal((::PStdData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PStdData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmBeamGenerator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBeamGenerator::Class_Name()
{
   return "CbmBeamGenerator";
}

//______________________________________________________________________________
const char *CbmBeamGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBeamGenerator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBeamGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBeamGenerator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBeamGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBeamGenerator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBeamGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBeamGenerator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPhsdGenerator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPhsdGenerator::Class_Name()
{
   return "CbmPhsdGenerator";
}

//______________________________________________________________________________
const char *CbmPhsdGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPhsdGenerator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPhsdGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPhsdGenerator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPhsdGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPhsdGenerator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPhsdGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPhsdGenerator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PValues::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PValues::Class_Name()
{
   return "PValues";
}

//______________________________________________________________________________
const char *PValues::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PValues*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PValues::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PValues*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PValues::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PValues*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PValues::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PValues*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PParticle::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PParticle::Class_Name()
{
   return "PParticle";
}

//______________________________________________________________________________
const char *PParticle::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PParticle*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PParticle::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PParticle*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PParticle::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PParticle*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PParticle::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PParticle*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PDataBase::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PDataBase::Class_Name()
{
   return "PDataBase";
}

//______________________________________________________________________________
const char *PDataBase::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PDataBase*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PDataBase::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PDataBase*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PDataBase::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PDataBase*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PDataBase::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PDataBase*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PMesh::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PMesh::Class_Name()
{
   return "PMesh";
}

//______________________________________________________________________________
const char *PMesh::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PMesh*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PMesh::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PMesh*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PMesh::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PMesh*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PMesh::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PMesh*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PStaticData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PStaticData::Class_Name()
{
   return "PStaticData";
}

//______________________________________________________________________________
const char *PStaticData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PStaticData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PStaticData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PStaticData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PStaticData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PStaticData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PStaticData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PStaticData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPlutoGenerator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPlutoGenerator::Class_Name()
{
   return "CbmPlutoGenerator";
}

//______________________________________________________________________________
const char *CbmPlutoGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPlutoGenerator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPlutoGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPlutoGenerator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPlutoGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPlutoGenerator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPlutoGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPlutoGenerator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmShieldGenerator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmShieldGenerator::Class_Name()
{
   return "CbmShieldGenerator";
}

//______________________________________________________________________________
const char *CbmShieldGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmShieldGenerator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmShieldGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmShieldGenerator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmShieldGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmShieldGenerator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmShieldGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmShieldGenerator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr UEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *UEvent::Class_Name()
{
   return "UEvent";
}

//______________________________________________________________________________
const char *UEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::UEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int UEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::UEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *UEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::UEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *UEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::UEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmUnigenGenerator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmUnigenGenerator::Class_Name()
{
   return "CbmUnigenGenerator";
}

//______________________________________________________________________________
const char *CbmUnigenGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmUnigenGenerator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmUnigenGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmUnigenGenerator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmUnigenGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmUnigenGenerator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmUnigenGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmUnigenGenerator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr URun::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *URun::Class_Name()
{
   return "URun";
}

//______________________________________________________________________________
const char *URun::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::URun*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int URun::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::URun*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *URun::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::URun*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *URun::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::URun*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr UParticle::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *UParticle::Class_Name()
{
   return "UParticle";
}

//______________________________________________________________________________
const char *UParticle::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::UParticle*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int UParticle::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::UParticle*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *UParticle::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::UParticle*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *UParticle::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::UParticle*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PStdData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PStdData::Class_Name()
{
   return "PStdData";
}

//______________________________________________________________________________
const char *PStdData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PStdData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PStdData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PStdData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PStdData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PStdData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PStdData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PStdData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmBeamGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBeamGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBeamGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBeamGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBeamGenerator(void *p) {
      return  p ? new(p) ::CbmBeamGenerator : new ::CbmBeamGenerator;
   }
   static void *newArray_CbmBeamGenerator(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBeamGenerator[nElements] : new ::CbmBeamGenerator[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBeamGenerator(void *p) {
      delete ((::CbmBeamGenerator*)p);
   }
   static void deleteArray_CbmBeamGenerator(void *p) {
      delete [] ((::CbmBeamGenerator*)p);
   }
   static void destruct_CbmBeamGenerator(void *p) {
      typedef ::CbmBeamGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBeamGenerator

//______________________________________________________________________________
void CbmPhsdGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPhsdGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPhsdGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPhsdGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPhsdGenerator(void *p) {
      return  p ? new(p) ::CbmPhsdGenerator : new ::CbmPhsdGenerator;
   }
   static void *newArray_CbmPhsdGenerator(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPhsdGenerator[nElements] : new ::CbmPhsdGenerator[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPhsdGenerator(void *p) {
      delete ((::CbmPhsdGenerator*)p);
   }
   static void deleteArray_CbmPhsdGenerator(void *p) {
      delete [] ((::CbmPhsdGenerator*)p);
   }
   static void destruct_CbmPhsdGenerator(void *p) {
      typedef ::CbmPhsdGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPhsdGenerator

//______________________________________________________________________________
void PValues::Streamer(TBuffer &R__b)
{
   // Stream an object of class PValues.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PValues(void *p) {
      return  p ? new(p) ::PValues : new ::PValues;
   }
   static void *newArray_PValues(Long_t nElements, void *p) {
      return p ? new(p) ::PValues[nElements] : new ::PValues[nElements];
   }
   // Wrapper around operator delete
   static void delete_PValues(void *p) {
      delete ((::PValues*)p);
   }
   static void deleteArray_PValues(void *p) {
      delete [] ((::PValues*)p);
   }
   static void destruct_PValues(void *p) {
      typedef ::PValues current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_PValues(TBuffer &buf, void *obj) {
      ((::PValues*)obj)->::PValues::Streamer(buf);
   }
} // end of namespace ROOT for class ::PValues

//______________________________________________________________________________
void PParticle::Streamer(TBuffer &R__b)
{
   // Stream an object of class PParticle.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TLorentzVector::Streamer(R__b);
      R__b >> pid;
      R__b >> sourceId;
      R__b >> parentId;
      R__b >> parentIndex;
      R__b >> decayModeIndex;
      R__b >> destroyDecayModeIndex;
      R__b >> daughterIndex;
      R__b >> siblingIndex;
      R__b >> decayTime;
      R__b >> wt;
      fV.Streamer(R__b);
      R__b >> fT;
      R__b.CheckByteCount(R__s, R__c, PParticle::IsA());
   } else {
      R__c = R__b.WriteVersion(PParticle::IsA(), kTRUE);
      TLorentzVector::Streamer(R__b);
      R__b << pid;
      R__b << sourceId;
      R__b << parentId;
      R__b << parentIndex;
      R__b << decayModeIndex;
      R__b << destroyDecayModeIndex;
      R__b << daughterIndex;
      R__b << siblingIndex;
      R__b << decayTime;
      R__b << wt;
      fV.Streamer(R__b);
      R__b << fT;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PParticle(void *p) {
      return  p ? new(p) ::PParticle : new ::PParticle;
   }
   static void *newArray_PParticle(Long_t nElements, void *p) {
      return p ? new(p) ::PParticle[nElements] : new ::PParticle[nElements];
   }
   // Wrapper around operator delete
   static void delete_PParticle(void *p) {
      delete ((::PParticle*)p);
   }
   static void deleteArray_PParticle(void *p) {
      delete [] ((::PParticle*)p);
   }
   static void destruct_PParticle(void *p) {
      typedef ::PParticle current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_PParticle(TBuffer &buf, void *obj) {
      ((::PParticle*)obj)->::PParticle::Streamer(buf);
   }
} // end of namespace ROOT for class ::PParticle

//______________________________________________________________________________
void PDataBase::Streamer(TBuffer &R__b)
{
   // Stream an object of class PDataBase.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PDataBase(void *p) {
      return  p ? new(p) ::PDataBase : new ::PDataBase;
   }
   static void *newArray_PDataBase(Long_t nElements, void *p) {
      return p ? new(p) ::PDataBase[nElements] : new ::PDataBase[nElements];
   }
   // Wrapper around operator delete
   static void delete_PDataBase(void *p) {
      delete ((::PDataBase*)p);
   }
   static void deleteArray_PDataBase(void *p) {
      delete [] ((::PDataBase*)p);
   }
   static void destruct_PDataBase(void *p) {
      typedef ::PDataBase current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_PDataBase(TBuffer &buf, void *obj) {
      ((::PDataBase*)obj)->::PDataBase::Streamer(buf);
   }
} // end of namespace ROOT for class ::PDataBase

//______________________________________________________________________________
void PMesh::Streamer(TBuffer &R__b)
{
   // Stream an object of class PMesh.

   TF1::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_PMesh(void *p) {
      delete ((::PMesh*)p);
   }
   static void deleteArray_PMesh(void *p) {
      delete [] ((::PMesh*)p);
   }
   static void destruct_PMesh(void *p) {
      typedef ::PMesh current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_PMesh(TBuffer &buf, void *obj) {
      ((::PMesh*)obj)->::PMesh::Streamer(buf);
   }
} // end of namespace ROOT for class ::PMesh

//______________________________________________________________________________
void PStaticData::Streamer(TBuffer &R__b)
{
   // Stream an object of class PStaticData.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PStaticData(void *p) {
      return  p ? new(p) ::PStaticData : new ::PStaticData;
   }
   static void *newArray_PStaticData(Long_t nElements, void *p) {
      return p ? new(p) ::PStaticData[nElements] : new ::PStaticData[nElements];
   }
   // Wrapper around operator delete
   static void delete_PStaticData(void *p) {
      delete ((::PStaticData*)p);
   }
   static void deleteArray_PStaticData(void *p) {
      delete [] ((::PStaticData*)p);
   }
   static void destruct_PStaticData(void *p) {
      typedef ::PStaticData current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_PStaticData(TBuffer &buf, void *obj) {
      ((::PStaticData*)obj)->::PStaticData::Streamer(buf);
   }
} // end of namespace ROOT for class ::PStaticData

//______________________________________________________________________________
void CbmPlutoGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPlutoGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPlutoGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPlutoGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPlutoGenerator(void *p) {
      return  p ? new(p) ::CbmPlutoGenerator : new ::CbmPlutoGenerator;
   }
   static void *newArray_CbmPlutoGenerator(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPlutoGenerator[nElements] : new ::CbmPlutoGenerator[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPlutoGenerator(void *p) {
      delete ((::CbmPlutoGenerator*)p);
   }
   static void deleteArray_CbmPlutoGenerator(void *p) {
      delete [] ((::CbmPlutoGenerator*)p);
   }
   static void destruct_CbmPlutoGenerator(void *p) {
      typedef ::CbmPlutoGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPlutoGenerator

//______________________________________________________________________________
void CbmShieldGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmShieldGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmShieldGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmShieldGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmShieldGenerator(void *p) {
      return  p ? new(p) ::CbmShieldGenerator : new ::CbmShieldGenerator;
   }
   static void *newArray_CbmShieldGenerator(Long_t nElements, void *p) {
      return p ? new(p) ::CbmShieldGenerator[nElements] : new ::CbmShieldGenerator[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmShieldGenerator(void *p) {
      delete ((::CbmShieldGenerator*)p);
   }
   static void deleteArray_CbmShieldGenerator(void *p) {
      delete [] ((::CbmShieldGenerator*)p);
   }
   static void destruct_CbmShieldGenerator(void *p) {
      typedef ::CbmShieldGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmShieldGenerator

//______________________________________________________________________________
void UEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class UEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(UEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(UEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_UEvent(void *p) {
      return  p ? new(p) ::UEvent : new ::UEvent;
   }
   static void *newArray_UEvent(Long_t nElements, void *p) {
      return p ? new(p) ::UEvent[nElements] : new ::UEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_UEvent(void *p) {
      delete ((::UEvent*)p);
   }
   static void deleteArray_UEvent(void *p) {
      delete [] ((::UEvent*)p);
   }
   static void destruct_UEvent(void *p) {
      typedef ::UEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::UEvent

//______________________________________________________________________________
void CbmUnigenGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmUnigenGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmUnigenGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmUnigenGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmUnigenGenerator(void *p) {
      return  p ? new(p) ::CbmUnigenGenerator : new ::CbmUnigenGenerator;
   }
   static void *newArray_CbmUnigenGenerator(Long_t nElements, void *p) {
      return p ? new(p) ::CbmUnigenGenerator[nElements] : new ::CbmUnigenGenerator[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmUnigenGenerator(void *p) {
      delete ((::CbmUnigenGenerator*)p);
   }
   static void deleteArray_CbmUnigenGenerator(void *p) {
      delete [] ((::CbmUnigenGenerator*)p);
   }
   static void destruct_CbmUnigenGenerator(void *p) {
      typedef ::CbmUnigenGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmUnigenGenerator

//______________________________________________________________________________
void URun::Streamer(TBuffer &R__b)
{
   // Stream an object of class URun.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(URun::Class(),this);
   } else {
      R__b.WriteClassBuffer(URun::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_URun(void *p) {
      return  p ? new(p) ::URun : new ::URun;
   }
   static void *newArray_URun(Long_t nElements, void *p) {
      return p ? new(p) ::URun[nElements] : new ::URun[nElements];
   }
   // Wrapper around operator delete
   static void delete_URun(void *p) {
      delete ((::URun*)p);
   }
   static void deleteArray_URun(void *p) {
      delete [] ((::URun*)p);
   }
   static void destruct_URun(void *p) {
      typedef ::URun current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::URun

//______________________________________________________________________________
void UParticle::Streamer(TBuffer &R__b)
{
   // Stream an object of class UParticle.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(UParticle::Class(),this);
   } else {
      R__b.WriteClassBuffer(UParticle::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_UParticle(void *p) {
      return  p ? new(p) ::UParticle : new ::UParticle;
   }
   static void *newArray_UParticle(Long_t nElements, void *p) {
      return p ? new(p) ::UParticle[nElements] : new ::UParticle[nElements];
   }
   // Wrapper around operator delete
   static void delete_UParticle(void *p) {
      delete ((::UParticle*)p);
   }
   static void deleteArray_UParticle(void *p) {
      delete [] ((::UParticle*)p);
   }
   static void destruct_UParticle(void *p) {
      typedef ::UParticle current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::UParticle

//______________________________________________________________________________
void PStdData::Streamer(TBuffer &R__b)
{
   // Stream an object of class PStdData.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PStdData(void *p) {
      return  p ? new(p) ::PStdData : new ::PStdData;
   }
   static void *newArray_PStdData(Long_t nElements, void *p) {
      return p ? new(p) ::PStdData[nElements] : new ::PStdData[nElements];
   }
   // Wrapper around operator delete
   static void delete_PStdData(void *p) {
      delete ((::PStdData*)p);
   }
   static void deleteArray_PStdData(void *p) {
      delete [] ((::PStdData*)p);
   }
   static void destruct_PStdData(void *p) {
      typedef ::PStdData current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_PStdData(TBuffer &buf, void *obj) {
      ((::PStdData*)obj)->::PStdData::Streamer(buf);
   }
} // end of namespace ROOT for class ::PStdData

namespace ROOT {
   static TClass *maplETStringcOFairIonmUgR_Dictionary();
   static void maplETStringcOFairIonmUgR_TClassManip(TClass*);
   static void *new_maplETStringcOFairIonmUgR(void *p = nullptr);
   static void *newArray_maplETStringcOFairIonmUgR(Long_t size, void *p);
   static void delete_maplETStringcOFairIonmUgR(void *p);
   static void deleteArray_maplETStringcOFairIonmUgR(void *p);
   static void destruct_maplETStringcOFairIonmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,FairIon*>*)
   {
      map<TString,FairIon*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,FairIon*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,FairIon*>", -2, "map", 100,
                  typeid(map<TString,FairIon*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOFairIonmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,FairIon*>) );
      instance.SetNew(&new_maplETStringcOFairIonmUgR);
      instance.SetNewArray(&newArray_maplETStringcOFairIonmUgR);
      instance.SetDelete(&delete_maplETStringcOFairIonmUgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOFairIonmUgR);
      instance.SetDestructor(&destruct_maplETStringcOFairIonmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,FairIon*> >()));

      ::ROOT::AddClassAlternate("map<TString,FairIon*>","std::map<TString, FairIon*, std::less<TString>, std::allocator<std::pair<TString const, FairIon*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,FairIon*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOFairIonmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,FairIon*>*)nullptr)->GetClass();
      maplETStringcOFairIonmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOFairIonmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOFairIonmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,FairIon*> : new map<TString,FairIon*>;
   }
   static void *newArray_maplETStringcOFairIonmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,FairIon*>[nElements] : new map<TString,FairIon*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOFairIonmUgR(void *p) {
      delete ((map<TString,FairIon*>*)p);
   }
   static void deleteArray_maplETStringcOFairIonmUgR(void *p) {
      delete [] ((map<TString,FairIon*>*)p);
   }
   static void destruct_maplETStringcOFairIonmUgR(void *p) {
      typedef map<TString,FairIon*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,FairIon*>

namespace {
  void TriggerDictionaryInitialization_libCbmSimGenerators_Impl() {
    static const char* headers[] = {
"CbmBeamGenerator.h",
"CbmPhsdGenerator.h",
"CbmPlutoGenerator.h",
"CbmShieldGenerator.h",
"CbmUnigenGenerator.h",
"URun.h",
"UEvent.h",
"UParticle.h",
"PDataBase.h",
"PMesh.h",
"PParticle.h",
"PStaticData.h",
"PStdData.h",
"PValues.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators/unigen",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators/pluto",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators/unigen",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators/pluto",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
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
"/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc",
"/home/heinemann/gpuunpacker/cbmroot/external/xpu/src",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/sim/transport/generators/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmSimGenerators dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmBeamGenerator.h")))  CbmBeamGenerator;
class __attribute__((annotate("$clingAutoload$CbmPhsdGenerator.h")))  CbmPhsdGenerator;
class __attribute__((annotate(R"ATTRDUMP(User value container)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PValues.h")))  __attribute__((annotate("$clingAutoload$CbmPlutoGenerator.h")))  PValues;
class __attribute__((annotate(R"ATTRDUMP(Pluto Particle Class)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PParticle.h")))  __attribute__((annotate("$clingAutoload$CbmPlutoGenerator.h")))  PParticle;
class __attribute__((annotate(R"ATTRDUMP(Relational data base for pluto)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Relational data base for pluto)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Relational data base for pluto)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Relational data base for pluto)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PDataBase.h")))  __attribute__((annotate("$clingAutoload$CbmPlutoGenerator.h")))  PDataBase;
class __attribute__((annotate(R"ATTRDUMP(The linear mesh array)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PMesh.h")))  __attribute__((annotate("$clingAutoload$CbmPlutoGenerator.h")))  PMesh;
class __attribute__((annotate(R"ATTRDUMP(Pluto Static Data Wrapper)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Pluto Static Data Wrapper)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Pluto Static Data Wrapper)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(Pluto Static Data Wrapper)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PStaticData.h")))  __attribute__((annotate("$clingAutoload$CbmPlutoGenerator.h")))  PStaticData;
class __attribute__((annotate("$clingAutoload$CbmPlutoGenerator.h")))  CbmPlutoGenerator;
class __attribute__((annotate("$clingAutoload$CbmShieldGenerator.h")))  CbmShieldGenerator;
class __attribute__((annotate("$clingAutoload$UEvent.h")))  __attribute__((annotate("$clingAutoload$CbmUnigenGenerator.h")))  UEvent;
class __attribute__((annotate("$clingAutoload$CbmUnigenGenerator.h")))  CbmUnigenGenerator;
class __attribute__((annotate("$clingAutoload$URun.h")))  URun;
class __attribute__((annotate("$clingAutoload$UParticle.h")))  UParticle;
class __attribute__((annotate(R"ATTRDUMP(Pluto Particle Std Data Class)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$PStdData.h")))  PStdData;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmSimGenerators dictionary payload"

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
#include "CbmBeamGenerator.h"
#include "CbmPhsdGenerator.h"
#include "CbmPlutoGenerator.h"
#include "CbmShieldGenerator.h"
#include "CbmUnigenGenerator.h"
#include "URun.h"
#include "UEvent.h"
#include "UParticle.h"
#include "PDataBase.h"
#include "PMesh.h"
#include "PParticle.h"
#include "PStaticData.h"
#include "PStdData.h"
#include "PValues.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmBeamGenerator", payloadCode, "@",
"CbmPhsdGenerator", payloadCode, "@",
"CbmPlutoGenerator", payloadCode, "@",
"CbmShieldGenerator", payloadCode, "@",
"CbmUnigenGenerator", payloadCode, "@",
"PDataBase", payloadCode, "@",
"PMesh", payloadCode, "@",
"PParticle", payloadCode, "@",
"PStaticData", payloadCode, "@",
"PStdData", payloadCode, "@",
"PValues", payloadCode, "@",
"UEvent", payloadCode, "@",
"UParticle", payloadCode, "@",
"URun", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmSimGenerators",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmSimGenerators_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmSimGenerators_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmSimGenerators() {
  TriggerDictionaryInitialization_libCbmSimGenerators_Impl();
}
