// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmFlibStar2019
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
#include "CbmStar2019TofPar.h"
#include "CbmStar2019ContFact.h"
#include "CbmStar2019MonitorAlgo.h"
#include "CbmStar2019MonitorTask.h"
#include "CbmStar2019MonitorPulserAlgo.h"
#include "CbmStar2019MonitorPulserTask.h"
#include "CbmTofStarData2019.h"
#include "CbmStar2019EventBuilderEtofAlgo.h"
#include "CbmStar2019EventBuilderEtof.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmStar2019TofPar(void *p = nullptr);
   static void *newArray_CbmStar2019TofPar(Long_t size, void *p);
   static void delete_CbmStar2019TofPar(void *p);
   static void deleteArray_CbmStar2019TofPar(void *p);
   static void destruct_CbmStar2019TofPar(void *p);
   static void streamer_CbmStar2019TofPar(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStar2019TofPar*)
   {
      ::CbmStar2019TofPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStar2019TofPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStar2019TofPar", ::CbmStar2019TofPar::Class_Version(), "CbmStar2019TofPar.h", 22,
                  typeid(::CbmStar2019TofPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStar2019TofPar::Dictionary, isa_proxy, 16,
                  sizeof(::CbmStar2019TofPar) );
      instance.SetNew(&new_CbmStar2019TofPar);
      instance.SetNewArray(&newArray_CbmStar2019TofPar);
      instance.SetDelete(&delete_CbmStar2019TofPar);
      instance.SetDeleteArray(&deleteArray_CbmStar2019TofPar);
      instance.SetDestructor(&destruct_CbmStar2019TofPar);
      instance.SetStreamerFunc(&streamer_CbmStar2019TofPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStar2019TofPar*)
   {
      return GenerateInitInstanceLocal((::CbmStar2019TofPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStar2019TofPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStar2019ContFact(void *p = nullptr);
   static void *newArray_CbmStar2019ContFact(Long_t size, void *p);
   static void delete_CbmStar2019ContFact(void *p);
   static void deleteArray_CbmStar2019ContFact(void *p);
   static void destruct_CbmStar2019ContFact(void *p);
   static void streamer_CbmStar2019ContFact(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStar2019ContFact*)
   {
      ::CbmStar2019ContFact *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStar2019ContFact >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStar2019ContFact", ::CbmStar2019ContFact::Class_Version(), "CbmStar2019ContFact.h", 13,
                  typeid(::CbmStar2019ContFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStar2019ContFact::Dictionary, isa_proxy, 16,
                  sizeof(::CbmStar2019ContFact) );
      instance.SetNew(&new_CbmStar2019ContFact);
      instance.SetNewArray(&newArray_CbmStar2019ContFact);
      instance.SetDelete(&delete_CbmStar2019ContFact);
      instance.SetDeleteArray(&deleteArray_CbmStar2019ContFact);
      instance.SetDestructor(&destruct_CbmStar2019ContFact);
      instance.SetStreamerFunc(&streamer_CbmStar2019ContFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStar2019ContFact*)
   {
      return GenerateInitInstanceLocal((::CbmStar2019ContFact*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStar2019ContFact*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStar2019MonitorAlgo(void *p = nullptr);
   static void *newArray_CbmStar2019MonitorAlgo(Long_t size, void *p);
   static void delete_CbmStar2019MonitorAlgo(void *p);
   static void deleteArray_CbmStar2019MonitorAlgo(void *p);
   static void destruct_CbmStar2019MonitorAlgo(void *p);
   static void streamer_CbmStar2019MonitorAlgo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStar2019MonitorAlgo*)
   {
      ::CbmStar2019MonitorAlgo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStar2019MonitorAlgo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStar2019MonitorAlgo", ::CbmStar2019MonitorAlgo::Class_Version(), "CbmStar2019MonitorAlgo.h", 36,
                  typeid(::CbmStar2019MonitorAlgo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStar2019MonitorAlgo::Dictionary, isa_proxy, 16,
                  sizeof(::CbmStar2019MonitorAlgo) );
      instance.SetNew(&new_CbmStar2019MonitorAlgo);
      instance.SetNewArray(&newArray_CbmStar2019MonitorAlgo);
      instance.SetDelete(&delete_CbmStar2019MonitorAlgo);
      instance.SetDeleteArray(&deleteArray_CbmStar2019MonitorAlgo);
      instance.SetDestructor(&destruct_CbmStar2019MonitorAlgo);
      instance.SetStreamerFunc(&streamer_CbmStar2019MonitorAlgo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStar2019MonitorAlgo*)
   {
      return GenerateInitInstanceLocal((::CbmStar2019MonitorAlgo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStar2019MonitorAlgo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStar2019MonitorTask(void *p = nullptr);
   static void *newArray_CbmStar2019MonitorTask(Long_t size, void *p);
   static void delete_CbmStar2019MonitorTask(void *p);
   static void deleteArray_CbmStar2019MonitorTask(void *p);
   static void destruct_CbmStar2019MonitorTask(void *p);
   static void streamer_CbmStar2019MonitorTask(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStar2019MonitorTask*)
   {
      ::CbmStar2019MonitorTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStar2019MonitorTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStar2019MonitorTask", ::CbmStar2019MonitorTask::Class_Version(), "CbmStar2019MonitorTask.h", 25,
                  typeid(::CbmStar2019MonitorTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStar2019MonitorTask::Dictionary, isa_proxy, 16,
                  sizeof(::CbmStar2019MonitorTask) );
      instance.SetNew(&new_CbmStar2019MonitorTask);
      instance.SetNewArray(&newArray_CbmStar2019MonitorTask);
      instance.SetDelete(&delete_CbmStar2019MonitorTask);
      instance.SetDeleteArray(&deleteArray_CbmStar2019MonitorTask);
      instance.SetDestructor(&destruct_CbmStar2019MonitorTask);
      instance.SetStreamerFunc(&streamer_CbmStar2019MonitorTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStar2019MonitorTask*)
   {
      return GenerateInitInstanceLocal((::CbmStar2019MonitorTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStar2019MonitorTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStar2019MonitorPulserAlgo(void *p = nullptr);
   static void *newArray_CbmStar2019MonitorPulserAlgo(Long_t size, void *p);
   static void delete_CbmStar2019MonitorPulserAlgo(void *p);
   static void deleteArray_CbmStar2019MonitorPulserAlgo(void *p);
   static void destruct_CbmStar2019MonitorPulserAlgo(void *p);
   static void streamer_CbmStar2019MonitorPulserAlgo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStar2019MonitorPulserAlgo*)
   {
      ::CbmStar2019MonitorPulserAlgo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStar2019MonitorPulserAlgo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStar2019MonitorPulserAlgo", ::CbmStar2019MonitorPulserAlgo::Class_Version(), "CbmStar2019MonitorPulserAlgo.h", 36,
                  typeid(::CbmStar2019MonitorPulserAlgo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStar2019MonitorPulserAlgo::Dictionary, isa_proxy, 16,
                  sizeof(::CbmStar2019MonitorPulserAlgo) );
      instance.SetNew(&new_CbmStar2019MonitorPulserAlgo);
      instance.SetNewArray(&newArray_CbmStar2019MonitorPulserAlgo);
      instance.SetDelete(&delete_CbmStar2019MonitorPulserAlgo);
      instance.SetDeleteArray(&deleteArray_CbmStar2019MonitorPulserAlgo);
      instance.SetDestructor(&destruct_CbmStar2019MonitorPulserAlgo);
      instance.SetStreamerFunc(&streamer_CbmStar2019MonitorPulserAlgo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStar2019MonitorPulserAlgo*)
   {
      return GenerateInitInstanceLocal((::CbmStar2019MonitorPulserAlgo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStar2019MonitorPulserAlgo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStar2019MonitorPulserTask(void *p = nullptr);
   static void *newArray_CbmStar2019MonitorPulserTask(Long_t size, void *p);
   static void delete_CbmStar2019MonitorPulserTask(void *p);
   static void deleteArray_CbmStar2019MonitorPulserTask(void *p);
   static void destruct_CbmStar2019MonitorPulserTask(void *p);
   static void streamer_CbmStar2019MonitorPulserTask(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStar2019MonitorPulserTask*)
   {
      ::CbmStar2019MonitorPulserTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStar2019MonitorPulserTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStar2019MonitorPulserTask", ::CbmStar2019MonitorPulserTask::Class_Version(), "CbmStar2019MonitorPulserTask.h", 25,
                  typeid(::CbmStar2019MonitorPulserTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStar2019MonitorPulserTask::Dictionary, isa_proxy, 16,
                  sizeof(::CbmStar2019MonitorPulserTask) );
      instance.SetNew(&new_CbmStar2019MonitorPulserTask);
      instance.SetNewArray(&newArray_CbmStar2019MonitorPulserTask);
      instance.SetDelete(&delete_CbmStar2019MonitorPulserTask);
      instance.SetDeleteArray(&deleteArray_CbmStar2019MonitorPulserTask);
      instance.SetDestructor(&destruct_CbmStar2019MonitorPulserTask);
      instance.SetStreamerFunc(&streamer_CbmStar2019MonitorPulserTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStar2019MonitorPulserTask*)
   {
      return GenerateInitInstanceLocal((::CbmStar2019MonitorPulserTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStar2019MonitorPulserTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmTofStarTrigger2019_Dictionary();
   static void CbmTofStarTrigger2019_TClassManip(TClass*);
   static void delete_CbmTofStarTrigger2019(void *p);
   static void deleteArray_CbmTofStarTrigger2019(void *p);
   static void destruct_CbmTofStarTrigger2019(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofStarTrigger2019*)
   {
      ::CbmTofStarTrigger2019 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmTofStarTrigger2019));
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofStarTrigger2019", "CbmTofStarData2019.h", 21,
                  typeid(::CbmTofStarTrigger2019), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmTofStarTrigger2019_Dictionary, isa_proxy, 0,
                  sizeof(::CbmTofStarTrigger2019) );
      instance.SetDelete(&delete_CbmTofStarTrigger2019);
      instance.SetDeleteArray(&deleteArray_CbmTofStarTrigger2019);
      instance.SetDestructor(&destruct_CbmTofStarTrigger2019);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofStarTrigger2019*)
   {
      return GenerateInitInstanceLocal((::CbmTofStarTrigger2019*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofStarTrigger2019*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmTofStarTrigger2019_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmTofStarTrigger2019*)nullptr)->GetClass();
      CbmTofStarTrigger2019_TClassManip(theClass);
   return theClass;
   }

   static void CbmTofStarTrigger2019_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmTofStarSubevent2019_Dictionary();
   static void CbmTofStarSubevent2019_TClassManip(TClass*);
   static void *new_CbmTofStarSubevent2019(void *p = nullptr);
   static void *newArray_CbmTofStarSubevent2019(Long_t size, void *p);
   static void delete_CbmTofStarSubevent2019(void *p);
   static void deleteArray_CbmTofStarSubevent2019(void *p);
   static void destruct_CbmTofStarSubevent2019(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofStarSubevent2019*)
   {
      ::CbmTofStarSubevent2019 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmTofStarSubevent2019));
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofStarSubevent2019", "CbmTofStarData2019.h", 64,
                  typeid(::CbmTofStarSubevent2019), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmTofStarSubevent2019_Dictionary, isa_proxy, 0,
                  sizeof(::CbmTofStarSubevent2019) );
      instance.SetNew(&new_CbmTofStarSubevent2019);
      instance.SetNewArray(&newArray_CbmTofStarSubevent2019);
      instance.SetDelete(&delete_CbmTofStarSubevent2019);
      instance.SetDeleteArray(&deleteArray_CbmTofStarSubevent2019);
      instance.SetDestructor(&destruct_CbmTofStarSubevent2019);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofStarSubevent2019*)
   {
      return GenerateInitInstanceLocal((::CbmTofStarSubevent2019*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofStarSubevent2019*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmTofStarSubevent2019_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmTofStarSubevent2019*)nullptr)->GetClass();
      CbmTofStarSubevent2019_TClassManip(theClass);
   return theClass;
   }

   static void CbmTofStarSubevent2019_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStar2019EventBuilderEtofAlgo(void *p = nullptr);
   static void *newArray_CbmStar2019EventBuilderEtofAlgo(Long_t size, void *p);
   static void delete_CbmStar2019EventBuilderEtofAlgo(void *p);
   static void deleteArray_CbmStar2019EventBuilderEtofAlgo(void *p);
   static void destruct_CbmStar2019EventBuilderEtofAlgo(void *p);
   static void streamer_CbmStar2019EventBuilderEtofAlgo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStar2019EventBuilderEtofAlgo*)
   {
      ::CbmStar2019EventBuilderEtofAlgo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStar2019EventBuilderEtofAlgo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStar2019EventBuilderEtofAlgo", ::CbmStar2019EventBuilderEtofAlgo::Class_Version(), "CbmStar2019EventBuilderEtofAlgo.h", 35,
                  typeid(::CbmStar2019EventBuilderEtofAlgo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStar2019EventBuilderEtofAlgo::Dictionary, isa_proxy, 16,
                  sizeof(::CbmStar2019EventBuilderEtofAlgo) );
      instance.SetNew(&new_CbmStar2019EventBuilderEtofAlgo);
      instance.SetNewArray(&newArray_CbmStar2019EventBuilderEtofAlgo);
      instance.SetDelete(&delete_CbmStar2019EventBuilderEtofAlgo);
      instance.SetDeleteArray(&deleteArray_CbmStar2019EventBuilderEtofAlgo);
      instance.SetDestructor(&destruct_CbmStar2019EventBuilderEtofAlgo);
      instance.SetStreamerFunc(&streamer_CbmStar2019EventBuilderEtofAlgo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStar2019EventBuilderEtofAlgo*)
   {
      return GenerateInitInstanceLocal((::CbmStar2019EventBuilderEtofAlgo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStar2019EventBuilderEtofAlgo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStar2019EventBuilderEtof(void *p = nullptr);
   static void *newArray_CbmStar2019EventBuilderEtof(Long_t size, void *p);
   static void delete_CbmStar2019EventBuilderEtof(void *p);
   static void deleteArray_CbmStar2019EventBuilderEtof(void *p);
   static void destruct_CbmStar2019EventBuilderEtof(void *p);
   static void streamer_CbmStar2019EventBuilderEtof(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStar2019EventBuilderEtof*)
   {
      ::CbmStar2019EventBuilderEtof *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStar2019EventBuilderEtof >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStar2019EventBuilderEtof", ::CbmStar2019EventBuilderEtof::Class_Version(), "CbmStar2019EventBuilderEtof.h", 36,
                  typeid(::CbmStar2019EventBuilderEtof), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStar2019EventBuilderEtof::Dictionary, isa_proxy, 16,
                  sizeof(::CbmStar2019EventBuilderEtof) );
      instance.SetNew(&new_CbmStar2019EventBuilderEtof);
      instance.SetNewArray(&newArray_CbmStar2019EventBuilderEtof);
      instance.SetDelete(&delete_CbmStar2019EventBuilderEtof);
      instance.SetDeleteArray(&deleteArray_CbmStar2019EventBuilderEtof);
      instance.SetDestructor(&destruct_CbmStar2019EventBuilderEtof);
      instance.SetStreamerFunc(&streamer_CbmStar2019EventBuilderEtof);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStar2019EventBuilderEtof*)
   {
      return GenerateInitInstanceLocal((::CbmStar2019EventBuilderEtof*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStar2019EventBuilderEtof*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmStar2019TofPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStar2019TofPar::Class_Name()
{
   return "CbmStar2019TofPar";
}

//______________________________________________________________________________
const char *CbmStar2019TofPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019TofPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStar2019TofPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019TofPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStar2019TofPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019TofPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStar2019TofPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019TofPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStar2019ContFact::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStar2019ContFact::Class_Name()
{
   return "CbmStar2019ContFact";
}

//______________________________________________________________________________
const char *CbmStar2019ContFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019ContFact*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStar2019ContFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019ContFact*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStar2019ContFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019ContFact*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStar2019ContFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019ContFact*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStar2019MonitorAlgo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStar2019MonitorAlgo::Class_Name()
{
   return "CbmStar2019MonitorAlgo";
}

//______________________________________________________________________________
const char *CbmStar2019MonitorAlgo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorAlgo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStar2019MonitorAlgo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorAlgo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStar2019MonitorAlgo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorAlgo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStar2019MonitorAlgo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorAlgo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStar2019MonitorTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStar2019MonitorTask::Class_Name()
{
   return "CbmStar2019MonitorTask";
}

//______________________________________________________________________________
const char *CbmStar2019MonitorTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStar2019MonitorTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStar2019MonitorTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStar2019MonitorTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStar2019MonitorPulserAlgo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStar2019MonitorPulserAlgo::Class_Name()
{
   return "CbmStar2019MonitorPulserAlgo";
}

//______________________________________________________________________________
const char *CbmStar2019MonitorPulserAlgo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorPulserAlgo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStar2019MonitorPulserAlgo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorPulserAlgo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStar2019MonitorPulserAlgo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorPulserAlgo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStar2019MonitorPulserAlgo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorPulserAlgo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStar2019MonitorPulserTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStar2019MonitorPulserTask::Class_Name()
{
   return "CbmStar2019MonitorPulserTask";
}

//______________________________________________________________________________
const char *CbmStar2019MonitorPulserTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorPulserTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStar2019MonitorPulserTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorPulserTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStar2019MonitorPulserTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorPulserTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStar2019MonitorPulserTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019MonitorPulserTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStar2019EventBuilderEtofAlgo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStar2019EventBuilderEtofAlgo::Class_Name()
{
   return "CbmStar2019EventBuilderEtofAlgo";
}

//______________________________________________________________________________
const char *CbmStar2019EventBuilderEtofAlgo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019EventBuilderEtofAlgo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStar2019EventBuilderEtofAlgo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019EventBuilderEtofAlgo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStar2019EventBuilderEtofAlgo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019EventBuilderEtofAlgo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStar2019EventBuilderEtofAlgo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019EventBuilderEtofAlgo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStar2019EventBuilderEtof::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStar2019EventBuilderEtof::Class_Name()
{
   return "CbmStar2019EventBuilderEtof";
}

//______________________________________________________________________________
const char *CbmStar2019EventBuilderEtof::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019EventBuilderEtof*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStar2019EventBuilderEtof::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019EventBuilderEtof*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStar2019EventBuilderEtof::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019EventBuilderEtof*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStar2019EventBuilderEtof::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStar2019EventBuilderEtof*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmStar2019TofPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStar2019TofPar.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairParGenericSet::Streamer(R__b);
      R__b.ReadStaticArray((unsigned int*)kuElinkToGet4);
      R__b.ReadStaticArray((unsigned int*)kuGet4ToElink);
      R__b.ReadStaticArray((unsigned int*)kuThrMeasCode);
      R__b.ReadStaticArray((double*)kdThrMeasVal);
      {
         vector<Double_t> &R__stl =  fvdPadiThrCodeToValue;
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
      R__b >> fiMonitorMode;
      R__b >> fiDebugMonitorMode;
      R__b >> fiNrOfGdpb;
      fiGdpbIdArray.Streamer(R__b);
      R__b >> fiNrOfGbtx;
      R__b >> fiNrOfModule;
      fiNrOfRpc.Streamer(R__b);
      fiRpcType.Streamer(R__b);
      fiRpcSide.Streamer(R__b);
      fiModuleId.Streamer(R__b);
      R__b >> fdSizeMsInNs;
      R__b >> fdStarTriggAllowedSpread;
      fdStarTriggerDeadtime.Streamer(R__b);
      fdStarTriggerDelay.Streamer(R__b);
      fdStarTriggerWinSize.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, CbmStar2019TofPar::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmStar2019TofPar::IsA(), kTRUE);
      FairParGenericSet::Streamer(R__b);
      R__b.WriteArray(kuElinkToGet4, 40);
      R__b.WriteArray(kuGet4ToElink, 40);
      R__b.WriteArray(kuThrMeasCode, 65);
      R__b.WriteArray(kdThrMeasVal, 65);
      {
         vector<Double_t> &R__stl =  fvdPadiThrCodeToValue;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Double_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fiMonitorMode;
      R__b << fiDebugMonitorMode;
      R__b << fiNrOfGdpb;
      fiGdpbIdArray.Streamer(R__b);
      R__b << fiNrOfGbtx;
      R__b << fiNrOfModule;
      fiNrOfRpc.Streamer(R__b);
      fiRpcType.Streamer(R__b);
      fiRpcSide.Streamer(R__b);
      fiModuleId.Streamer(R__b);
      R__b << fdSizeMsInNs;
      R__b << fdStarTriggAllowedSpread;
      fdStarTriggerDeadtime.Streamer(R__b);
      fdStarTriggerDelay.Streamer(R__b);
      fdStarTriggerWinSize.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStar2019TofPar(void *p) {
      return  p ? new(p) ::CbmStar2019TofPar : new ::CbmStar2019TofPar;
   }
   static void *newArray_CbmStar2019TofPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStar2019TofPar[nElements] : new ::CbmStar2019TofPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStar2019TofPar(void *p) {
      delete ((::CbmStar2019TofPar*)p);
   }
   static void deleteArray_CbmStar2019TofPar(void *p) {
      delete [] ((::CbmStar2019TofPar*)p);
   }
   static void destruct_CbmStar2019TofPar(void *p) {
      typedef ::CbmStar2019TofPar current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmStar2019TofPar(TBuffer &buf, void *obj) {
      ((::CbmStar2019TofPar*)obj)->::CbmStar2019TofPar::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmStar2019TofPar

//______________________________________________________________________________
void CbmStar2019ContFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStar2019ContFact.

   FairContFact::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStar2019ContFact(void *p) {
      return  p ? new(p) ::CbmStar2019ContFact : new ::CbmStar2019ContFact;
   }
   static void *newArray_CbmStar2019ContFact(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStar2019ContFact[nElements] : new ::CbmStar2019ContFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStar2019ContFact(void *p) {
      delete ((::CbmStar2019ContFact*)p);
   }
   static void deleteArray_CbmStar2019ContFact(void *p) {
      delete [] ((::CbmStar2019ContFact*)p);
   }
   static void destruct_CbmStar2019ContFact(void *p) {
      typedef ::CbmStar2019ContFact current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmStar2019ContFact(TBuffer &buf, void *obj) {
      ((::CbmStar2019ContFact*)obj)->::CbmStar2019ContFact::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmStar2019ContFact

//______________________________________________________________________________
void CbmStar2019MonitorAlgo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStar2019MonitorAlgo.

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
      R__b >> fiSectorIndex;
      R__b >> fuMinTotPulser;
      R__b >> fuMaxTotPulser;
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      {
         map<gdpbv100::MessageTypes,UInt_t> &R__stl =  fmMsgCounter;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            gdpbv100::MessageTypes R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<gdpbv100::MessageTypes>(readtemp);
            unsigned int R__t2;
            R__b >> R__t2;
            typedef enum gdpbv100::MessageTypes Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         vector<gdpbv100::Message> &R__stl =  fvmEpSupprBuffer;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class gdpbv100::Message));
         if (R__tcl1==0) {
            Error("fvmEpSupprBuffer streamer","Missing the TClass object for class gdpbv100::Message!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            gdpbv100::Message R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fulStartTs;
      R__b >> fdStartTime;
      R__b >> fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsMsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsLsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMsb;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMid;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsFullLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsFullLast;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ULong64_t R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTokenLast;
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
         vector<UInt_t> &R__stl =  fvuStarDaqCmdLast;
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
         vector<UInt_t> &R__stl =  fvuStarTrigCmdLast;
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
      R__b >> fbEpochSinceLastHit;
      R__b >> fuDuplicatesCount;
      R__b.StreamObject(&(fmLastHit),typeid(fmLastHit));
      R__b >> fuHistoryHistoSize;
      {
         vector<UInt_t> &R__stl =  fuNbMissmatchPattern;
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
      R__b.CheckByteCount(R__s, R__c, CbmStar2019MonitorAlgo::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmStar2019MonitorAlgo::IsA(), kTRUE);
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
      R__b << fiSectorIndex;
      R__b << fuMinTotPulser;
      R__b << fuMaxTotPulser;
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      {
         map<gdpbv100::MessageTypes,UInt_t> &R__stl =  fmMsgCounter;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<gdpbv100::MessageTypes,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      {
         vector<gdpbv100::Message> &R__stl =  fvmEpSupprBuffer;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class gdpbv100::Message));
         if (R__tcl1==0) {
            Error("fvmEpSupprBuffer streamer","Missing the TClass object for class gdpbv100::Message!");
            return;
         }
            vector<gdpbv100::Message>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((gdpbv100::Message*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << fulStartTs;
      R__b << fdStartTime;
      R__b << fdStartTimeMsSz;
      R__b.StreamObject(&(ftStartTimeUnix),typeid(ftStartTimeUnix));
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsMsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsLsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMsb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsMid;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulGdpbTsFullLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<ULong64_t> &R__stl =  fvulStarTsFullLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ULong64_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<UInt_t> &R__stl =  fvuStarTokenLast;
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
         vector<UInt_t> &R__stl =  fvuStarDaqCmdLast;
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
         vector<UInt_t> &R__stl =  fvuStarTrigCmdLast;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fbEpochSinceLastHit;
      R__b << fuDuplicatesCount;
      R__b.StreamObject(&(fmLastHit),typeid(fmLastHit));
      R__b << fuHistoryHistoSize;
      {
         vector<UInt_t> &R__stl =  fuNbMissmatchPattern;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStar2019MonitorAlgo(void *p) {
      return  p ? new(p) ::CbmStar2019MonitorAlgo : new ::CbmStar2019MonitorAlgo;
   }
   static void *newArray_CbmStar2019MonitorAlgo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStar2019MonitorAlgo[nElements] : new ::CbmStar2019MonitorAlgo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStar2019MonitorAlgo(void *p) {
      delete ((::CbmStar2019MonitorAlgo*)p);
   }
   static void deleteArray_CbmStar2019MonitorAlgo(void *p) {
      delete [] ((::CbmStar2019MonitorAlgo*)p);
   }
   static void destruct_CbmStar2019MonitorAlgo(void *p) {
      typedef ::CbmStar2019MonitorAlgo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmStar2019MonitorAlgo(TBuffer &buf, void *obj) {
      ((::CbmStar2019MonitorAlgo*)obj)->::CbmStar2019MonitorAlgo::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmStar2019MonitorAlgo

//______________________________________________________________________________
void CbmStar2019MonitorTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStar2019MonitorTask.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fuHistoryHistoSize;
      fsHistoFileName.Streamer(R__b);
      R__b >> fuMinTotPulser;
      R__b >> fuMaxTotPulser;
      R__b >> fiSectorIndex;
      R__b >> fParCList;
      R__b >> fulTsCounter;
      R__b >> fMonitorAlgo;
      R__b.CheckByteCount(R__s, R__c, CbmStar2019MonitorTask::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmStar2019MonitorTask::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fuHistoryHistoSize;
      fsHistoFileName.Streamer(R__b);
      R__b << fuMinTotPulser;
      R__b << fuMaxTotPulser;
      R__b << fiSectorIndex;
      R__b << fParCList;
      R__b << fulTsCounter;
      R__b << fMonitorAlgo;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStar2019MonitorTask(void *p) {
      return  p ? new(p) ::CbmStar2019MonitorTask : new ::CbmStar2019MonitorTask;
   }
   static void *newArray_CbmStar2019MonitorTask(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStar2019MonitorTask[nElements] : new ::CbmStar2019MonitorTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStar2019MonitorTask(void *p) {
      delete ((::CbmStar2019MonitorTask*)p);
   }
   static void deleteArray_CbmStar2019MonitorTask(void *p) {
      delete [] ((::CbmStar2019MonitorTask*)p);
   }
   static void destruct_CbmStar2019MonitorTask(void *p) {
      typedef ::CbmStar2019MonitorTask current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmStar2019MonitorTask(TBuffer &buf, void *obj) {
      ((::CbmStar2019MonitorTask*)obj)->::CbmStar2019MonitorTask::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmStar2019MonitorTask

//______________________________________________________________________________
void CbmStar2019MonitorPulserAlgo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStar2019MonitorPulserAlgo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b >> fbEtofFeeIndexing;
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
      R__b >> fiSectorIndex;
      R__b >> fuUpdateFreqTs;
      R__b >> fuPulserMinTot;
      R__b >> fuPulserMaxTot;
      R__b >> fuPulserChannel;
      R__b >> fulCurrentTsIdx;
      R__b >> fulCurrentMsIdx;
      {
         vector<gdpbv100::Message> &R__stl =  fvmEpSupprBuffer;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class gdpbv100::Message));
         if (R__tcl1==0) {
            Error("fvmEpSupprBuffer streamer","Missing the TClass object for class gdpbv100::Message!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            gdpbv100::Message R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> const_cast< Double_t &>( kdMaxDtPulserPs );
      R__b >> const_cast< UInt_t &>( kuNbBinsDt );
      R__b >> dMinDt;
      R__b >> dMaxDt;
      R__b >> const_cast< Double_t &>( kdFitZoomWidthPs );
      R__b.CheckByteCount(R__s, R__c, CbmStar2019MonitorPulserAlgo::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmStar2019MonitorPulserAlgo::IsA(), kTRUE);
      R__b << fbEtofFeeIndexing;
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
      R__b << fiSectorIndex;
      R__b << fuUpdateFreqTs;
      R__b << fuPulserMinTot;
      R__b << fuPulserMaxTot;
      R__b << fuPulserChannel;
      R__b << fulCurrentTsIdx;
      R__b << fulCurrentMsIdx;
      {
         vector<gdpbv100::Message> &R__stl =  fvmEpSupprBuffer;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class gdpbv100::Message));
         if (R__tcl1==0) {
            Error("fvmEpSupprBuffer streamer","Missing the TClass object for class gdpbv100::Message!");
            return;
         }
            vector<gdpbv100::Message>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((gdpbv100::Message*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << const_cast< Double_t &>( kdMaxDtPulserPs );
      R__b << const_cast< UInt_t &>( kuNbBinsDt );
      R__b << dMinDt;
      R__b << dMaxDt;
      R__b << const_cast< Double_t &>( kdFitZoomWidthPs );
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStar2019MonitorPulserAlgo(void *p) {
      return  p ? new(p) ::CbmStar2019MonitorPulserAlgo : new ::CbmStar2019MonitorPulserAlgo;
   }
   static void *newArray_CbmStar2019MonitorPulserAlgo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStar2019MonitorPulserAlgo[nElements] : new ::CbmStar2019MonitorPulserAlgo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStar2019MonitorPulserAlgo(void *p) {
      delete ((::CbmStar2019MonitorPulserAlgo*)p);
   }
   static void deleteArray_CbmStar2019MonitorPulserAlgo(void *p) {
      delete [] ((::CbmStar2019MonitorPulserAlgo*)p);
   }
   static void destruct_CbmStar2019MonitorPulserAlgo(void *p) {
      typedef ::CbmStar2019MonitorPulserAlgo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmStar2019MonitorPulserAlgo(TBuffer &buf, void *obj) {
      ((::CbmStar2019MonitorPulserAlgo*)obj)->::CbmStar2019MonitorPulserAlgo::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmStar2019MonitorPulserAlgo

//______________________________________________________________________________
void CbmStar2019MonitorPulserTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStar2019MonitorPulserTask.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fbEtofFeeIndexing;
      fsHistoFileName.Streamer(R__b);
      R__b >> fuUpdateFreqTs;
      R__b >> fuPulserMinTot;
      R__b >> fuPulserMaxTot;
      R__b >> fuPulserChannel;
      R__b >> fiSectorIndex;
      R__b >> fuHistoryHistoSize;
      R__b >> fParCList;
      R__b >> fulTsCounter;
      R__b >> fMonitorPulserAlgo;
      R__b.CheckByteCount(R__s, R__c, CbmStar2019MonitorPulserTask::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmStar2019MonitorPulserTask::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fbEtofFeeIndexing;
      fsHistoFileName.Streamer(R__b);
      R__b << fuUpdateFreqTs;
      R__b << fuPulserMinTot;
      R__b << fuPulserMaxTot;
      R__b << fuPulserChannel;
      R__b << fiSectorIndex;
      R__b << fuHistoryHistoSize;
      R__b << fParCList;
      R__b << fulTsCounter;
      R__b << fMonitorPulserAlgo;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStar2019MonitorPulserTask(void *p) {
      return  p ? new(p) ::CbmStar2019MonitorPulserTask : new ::CbmStar2019MonitorPulserTask;
   }
   static void *newArray_CbmStar2019MonitorPulserTask(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStar2019MonitorPulserTask[nElements] : new ::CbmStar2019MonitorPulserTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStar2019MonitorPulserTask(void *p) {
      delete ((::CbmStar2019MonitorPulserTask*)p);
   }
   static void deleteArray_CbmStar2019MonitorPulserTask(void *p) {
      delete [] ((::CbmStar2019MonitorPulserTask*)p);
   }
   static void destruct_CbmStar2019MonitorPulserTask(void *p) {
      typedef ::CbmStar2019MonitorPulserTask current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmStar2019MonitorPulserTask(TBuffer &buf, void *obj) {
      ((::CbmStar2019MonitorPulserTask*)obj)->::CbmStar2019MonitorPulserTask::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmStar2019MonitorPulserTask

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmTofStarTrigger2019(void *p) {
      delete ((::CbmTofStarTrigger2019*)p);
   }
   static void deleteArray_CbmTofStarTrigger2019(void *p) {
      delete [] ((::CbmTofStarTrigger2019*)p);
   }
   static void destruct_CbmTofStarTrigger2019(void *p) {
      typedef ::CbmTofStarTrigger2019 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofStarTrigger2019

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofStarSubevent2019(void *p) {
      return  p ? new(p) ::CbmTofStarSubevent2019 : new ::CbmTofStarSubevent2019;
   }
   static void *newArray_CbmTofStarSubevent2019(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofStarSubevent2019[nElements] : new ::CbmTofStarSubevent2019[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofStarSubevent2019(void *p) {
      delete ((::CbmTofStarSubevent2019*)p);
   }
   static void deleteArray_CbmTofStarSubevent2019(void *p) {
      delete [] ((::CbmTofStarSubevent2019*)p);
   }
   static void destruct_CbmTofStarSubevent2019(void *p) {
      typedef ::CbmTofStarSubevent2019 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofStarSubevent2019

//______________________________________________________________________________
void CbmStar2019EventBuilderEtofAlgo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStar2019EventBuilderEtofAlgo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b >> fuNrOfGdpbs;
      {
         map<UInt_t,UInt_t> &R__stl =  fGdpbIdIndexMap;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            unsigned int R__t;
            R__b >> R__t;
            unsigned int R__t2;
            R__b >> R__t2;
            typedef unsigned int Value_t;
            std::pair<Value_t const, unsigned int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fuNrOfFeePerGdpb;
      R__b >> fuNrOfGet4PerFee;
      R__b >> fuNrOfChannelsPerGet4;
      R__b >> fuNrOfChannelsPerFee;
      R__b >> fuNrOfGet4;
      R__b >> fuNrOfGet4PerGdpb;
      R__b >> fuNrOfChannelsPerGdpb;
      R__b >> fuNrOfGbtx;
      R__b >> fuNrOfModules;
      {
         vector<Int_t> &R__stl =  fviNrOfRpc;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcType;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcSide;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  fviModuleId;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> fdAllowedTriggersSpread;
      R__b >> fuMinTotPulser;
      R__b >> fuMaxTotPulser;
      R__b >> fulCurrentTsIndex;
      R__b >> fdTsStartTime;
      R__b >> fdTsStopTimeCore;
      R__b >> fuCurrentMs;
      R__b >> fdMsTime;
      R__b >> fuMsIndex;
      R__b >> fuGdpbId;
      R__b >> fuGdpbNr;
      R__b >> fuGet4Id;
      R__b >> fuGet4Nr;
      R__b >> fiEquipmentId;
      R__b >> fbTriggerFoundA;
      R__b >> fbTriggerFoundB;
      R__b >> fbTriggerFoundC;
      R__b >> fbEpochAfterCandWinFound;
      R__b >> fbTriggerAfterCandWinFound;
      R__b.CheckByteCount(R__s, R__c, CbmStar2019EventBuilderEtofAlgo::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmStar2019EventBuilderEtofAlgo::IsA(), kTRUE);
      R__b << fuNrOfGdpbs;
      {
         map<UInt_t,UInt_t> &R__stl =  fGdpbIdIndexMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<UInt_t,UInt_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fuNrOfFeePerGdpb;
      R__b << fuNrOfGet4PerFee;
      R__b << fuNrOfChannelsPerGet4;
      R__b << fuNrOfChannelsPerFee;
      R__b << fuNrOfGet4;
      R__b << fuNrOfGet4PerGdpb;
      R__b << fuNrOfChannelsPerGdpb;
      R__b << fuNrOfGbtx;
      R__b << fuNrOfModules;
      {
         vector<Int_t> &R__stl =  fviNrOfRpc;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcType;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviRpcSide;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  fviModuleId;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << fdAllowedTriggersSpread;
      R__b << fuMinTotPulser;
      R__b << fuMaxTotPulser;
      R__b << fulCurrentTsIndex;
      R__b << fdTsStartTime;
      R__b << fdTsStopTimeCore;
      R__b << fuCurrentMs;
      R__b << fdMsTime;
      R__b << fuMsIndex;
      R__b << fuGdpbId;
      R__b << fuGdpbNr;
      R__b << fuGet4Id;
      R__b << fuGet4Nr;
      R__b << fiEquipmentId;
      R__b << fbTriggerFoundA;
      R__b << fbTriggerFoundB;
      R__b << fbTriggerFoundC;
      R__b << fbEpochAfterCandWinFound;
      R__b << fbTriggerAfterCandWinFound;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStar2019EventBuilderEtofAlgo(void *p) {
      return  p ? new(p) ::CbmStar2019EventBuilderEtofAlgo : new ::CbmStar2019EventBuilderEtofAlgo;
   }
   static void *newArray_CbmStar2019EventBuilderEtofAlgo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStar2019EventBuilderEtofAlgo[nElements] : new ::CbmStar2019EventBuilderEtofAlgo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStar2019EventBuilderEtofAlgo(void *p) {
      delete ((::CbmStar2019EventBuilderEtofAlgo*)p);
   }
   static void deleteArray_CbmStar2019EventBuilderEtofAlgo(void *p) {
      delete [] ((::CbmStar2019EventBuilderEtofAlgo*)p);
   }
   static void destruct_CbmStar2019EventBuilderEtofAlgo(void *p) {
      typedef ::CbmStar2019EventBuilderEtofAlgo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmStar2019EventBuilderEtofAlgo(TBuffer &buf, void *obj) {
      ((::CbmStar2019EventBuilderEtofAlgo*)obj)->::CbmStar2019EventBuilderEtofAlgo::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmStar2019EventBuilderEtofAlgo

//______________________________________________________________________________
void CbmStar2019EventBuilderEtof::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStar2019EventBuilderEtof.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      CbmMcbmUnpack::Streamer(R__b);
      R__b >> fParCList;
      R__b >> fuMinTotPulser;
      R__b >> fuMaxTotPulser;
      fsHistoFileName.Streamer(R__b);
      R__b >> fulTsCounter;
      R__b >> fEventBuilderAlgo;
      fTimer.Streamer(R__b);
      R__b >> fdRealTime;
      R__b >> fdRealTimeMin;
      R__b >> fdRealTimeMax;
      R__b >> fdRealTimeAll;
      R__b >> fdRealTimeMinAll;
      R__b >> fdRealTimeMaxAll;
      R__b >> fulNbEvents;
      R__b >> fulNbEventsSinceLastPrintout;
      R__b >> fpBinDumpFile;
      R__b >> const_cast< UInt_t &>( kuBinDumpBegWord );
      R__b >> const_cast< UInt_t &>( kuBinDumpEndWord );
      R__b.CheckByteCount(R__s, R__c, CbmStar2019EventBuilderEtof::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmStar2019EventBuilderEtof::IsA(), kTRUE);
      CbmMcbmUnpack::Streamer(R__b);
      R__b << fParCList;
      R__b << fuMinTotPulser;
      R__b << fuMaxTotPulser;
      fsHistoFileName.Streamer(R__b);
      R__b << fulTsCounter;
      R__b << fEventBuilderAlgo;
      fTimer.Streamer(R__b);
      R__b << fdRealTime;
      R__b << fdRealTimeMin;
      R__b << fdRealTimeMax;
      R__b << fdRealTimeAll;
      R__b << fdRealTimeMinAll;
      R__b << fdRealTimeMaxAll;
      R__b << fulNbEvents;
      R__b << fulNbEventsSinceLastPrintout;
      R__b << fpBinDumpFile;
      R__b << const_cast< UInt_t &>( kuBinDumpBegWord );
      R__b << const_cast< UInt_t &>( kuBinDumpEndWord );
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStar2019EventBuilderEtof(void *p) {
      return  p ? new(p) ::CbmStar2019EventBuilderEtof : new ::CbmStar2019EventBuilderEtof;
   }
   static void *newArray_CbmStar2019EventBuilderEtof(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStar2019EventBuilderEtof[nElements] : new ::CbmStar2019EventBuilderEtof[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStar2019EventBuilderEtof(void *p) {
      delete ((::CbmStar2019EventBuilderEtof*)p);
   }
   static void deleteArray_CbmStar2019EventBuilderEtof(void *p) {
      delete [] ((::CbmStar2019EventBuilderEtof*)p);
   }
   static void destruct_CbmStar2019EventBuilderEtof(void *p) {
      typedef ::CbmStar2019EventBuilderEtof current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmStar2019EventBuilderEtof(TBuffer &buf, void *obj) {
      ((::CbmStar2019EventBuilderEtof*)obj)->::CbmStar2019EventBuilderEtof::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmStar2019EventBuilderEtof

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
   static TClass *vectorlEgdpbv100cLcLMessagegR_Dictionary();
   static void vectorlEgdpbv100cLcLMessagegR_TClassManip(TClass*);
   static void *new_vectorlEgdpbv100cLcLMessagegR(void *p = nullptr);
   static void *newArray_vectorlEgdpbv100cLcLMessagegR(Long_t size, void *p);
   static void delete_vectorlEgdpbv100cLcLMessagegR(void *p);
   static void deleteArray_vectorlEgdpbv100cLcLMessagegR(void *p);
   static void destruct_vectorlEgdpbv100cLcLMessagegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<gdpbv100::Message>*)
   {
      vector<gdpbv100::Message> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<gdpbv100::Message>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<gdpbv100::Message>", -2, "vector", 389,
                  typeid(vector<gdpbv100::Message>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEgdpbv100cLcLMessagegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<gdpbv100::Message>) );
      instance.SetNew(&new_vectorlEgdpbv100cLcLMessagegR);
      instance.SetNewArray(&newArray_vectorlEgdpbv100cLcLMessagegR);
      instance.SetDelete(&delete_vectorlEgdpbv100cLcLMessagegR);
      instance.SetDeleteArray(&deleteArray_vectorlEgdpbv100cLcLMessagegR);
      instance.SetDestructor(&destruct_vectorlEgdpbv100cLcLMessagegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<gdpbv100::Message> >()));

      ::ROOT::AddClassAlternate("vector<gdpbv100::Message>","std::vector<gdpbv100::Message, std::allocator<gdpbv100::Message> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<gdpbv100::Message>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEgdpbv100cLcLMessagegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<gdpbv100::Message>*)nullptr)->GetClass();
      vectorlEgdpbv100cLcLMessagegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEgdpbv100cLcLMessagegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEgdpbv100cLcLMessagegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<gdpbv100::Message> : new vector<gdpbv100::Message>;
   }
   static void *newArray_vectorlEgdpbv100cLcLMessagegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<gdpbv100::Message>[nElements] : new vector<gdpbv100::Message>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEgdpbv100cLcLMessagegR(void *p) {
      delete ((vector<gdpbv100::Message>*)p);
   }
   static void deleteArray_vectorlEgdpbv100cLcLMessagegR(void *p) {
      delete [] ((vector<gdpbv100::Message>*)p);
   }
   static void destruct_vectorlEgdpbv100cLcLMessagegR(void *p) {
      typedef vector<gdpbv100::Message> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<gdpbv100::Message>

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
   static TClass *vectorlEULong64_tgR_Dictionary();
   static void vectorlEULong64_tgR_TClassManip(TClass*);
   static void *new_vectorlEULong64_tgR(void *p = nullptr);
   static void *newArray_vectorlEULong64_tgR(Long_t size, void *p);
   static void delete_vectorlEULong64_tgR(void *p);
   static void deleteArray_vectorlEULong64_tgR(void *p);
   static void destruct_vectorlEULong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ULong64_t>*)
   {
      vector<ULong64_t> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ULong64_t>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ULong64_t>", -2, "vector", 389,
                  typeid(vector<ULong64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEULong64_tgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ULong64_t>) );
      instance.SetNew(&new_vectorlEULong64_tgR);
      instance.SetNewArray(&newArray_vectorlEULong64_tgR);
      instance.SetDelete(&delete_vectorlEULong64_tgR);
      instance.SetDeleteArray(&deleteArray_vectorlEULong64_tgR);
      instance.SetDestructor(&destruct_vectorlEULong64_tgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ULong64_t> >()));

      ::ROOT::AddClassAlternate("vector<ULong64_t>","std::vector<unsigned long long, std::allocator<unsigned long long> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<ULong64_t>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEULong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<ULong64_t>*)nullptr)->GetClass();
      vectorlEULong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEULong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEULong64_tgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ULong64_t> : new vector<ULong64_t>;
   }
   static void *newArray_vectorlEULong64_tgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ULong64_t>[nElements] : new vector<ULong64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEULong64_tgR(void *p) {
      delete ((vector<ULong64_t>*)p);
   }
   static void deleteArray_vectorlEULong64_tgR(void *p) {
      delete [] ((vector<ULong64_t>*)p);
   }
   static void destruct_vectorlEULong64_tgR(void *p) {
      typedef vector<ULong64_t> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<ULong64_t>

namespace ROOT {
   static TClass *maplEunsignedsPintcOunsignedsPintgR_Dictionary();
   static void maplEunsignedsPintcOunsignedsPintgR_TClassManip(TClass*);
   static void *new_maplEunsignedsPintcOunsignedsPintgR(void *p = nullptr);
   static void *newArray_maplEunsignedsPintcOunsignedsPintgR(Long_t size, void *p);
   static void delete_maplEunsignedsPintcOunsignedsPintgR(void *p);
   static void deleteArray_maplEunsignedsPintcOunsignedsPintgR(void *p);
   static void destruct_maplEunsignedsPintcOunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned int,unsigned int>*)
   {
      map<unsigned int,unsigned int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned int,unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned int,unsigned int>", -2, "map", 100,
                  typeid(map<unsigned int,unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPintcOunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned int,unsigned int>) );
      instance.SetNew(&new_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetNewArray(&newArray_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetDelete(&delete_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPintcOunsignedsPintgR);
      instance.SetDestructor(&destruct_maplEunsignedsPintcOunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned int,unsigned int> >()));

      ::ROOT::AddClassAlternate("map<unsigned int,unsigned int>","std::map<unsigned int, unsigned int, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, unsigned int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned int,unsigned int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPintcOunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned int,unsigned int>*)nullptr)->GetClass();
      maplEunsignedsPintcOunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPintcOunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,unsigned int> : new map<unsigned int,unsigned int>;
   }
   static void *newArray_maplEunsignedsPintcOunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned int,unsigned int>[nElements] : new map<unsigned int,unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      delete ((map<unsigned int,unsigned int>*)p);
   }
   static void deleteArray_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      delete [] ((map<unsigned int,unsigned int>*)p);
   }
   static void destruct_maplEunsignedsPintcOunsignedsPintgR(void *p) {
      typedef map<unsigned int,unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned int,unsigned int>

namespace ROOT {
   static TClass *maplEgdpbv100cLcLMessageTypescOunsignedsPintgR_Dictionary();
   static void maplEgdpbv100cLcLMessageTypescOunsignedsPintgR_TClassManip(TClass*);
   static void *new_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p = nullptr);
   static void *newArray_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(Long_t size, void *p);
   static void delete_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p);
   static void deleteArray_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p);
   static void destruct_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<gdpbv100::MessageTypes,unsigned int>*)
   {
      map<gdpbv100::MessageTypes,unsigned int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<gdpbv100::MessageTypes,unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<gdpbv100::MessageTypes,unsigned int>", -2, "map", 100,
                  typeid(map<gdpbv100::MessageTypes,unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEgdpbv100cLcLMessageTypescOunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<gdpbv100::MessageTypes,unsigned int>) );
      instance.SetNew(&new_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR);
      instance.SetNewArray(&newArray_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR);
      instance.SetDelete(&delete_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR);
      instance.SetDestructor(&destruct_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<gdpbv100::MessageTypes,unsigned int> >()));

      ::ROOT::AddClassAlternate("map<gdpbv100::MessageTypes,unsigned int>","std::map<gdpbv100::MessageTypes, unsigned int, std::less<gdpbv100::MessageTypes>, std::allocator<std::pair<gdpbv100::MessageTypes const, unsigned int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<gdpbv100::MessageTypes,unsigned int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEgdpbv100cLcLMessageTypescOunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<gdpbv100::MessageTypes,unsigned int>*)nullptr)->GetClass();
      maplEgdpbv100cLcLMessageTypescOunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEgdpbv100cLcLMessageTypescOunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<gdpbv100::MessageTypes,unsigned int> : new map<gdpbv100::MessageTypes,unsigned int>;
   }
   static void *newArray_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<gdpbv100::MessageTypes,unsigned int>[nElements] : new map<gdpbv100::MessageTypes,unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p) {
      delete ((map<gdpbv100::MessageTypes,unsigned int>*)p);
   }
   static void deleteArray_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p) {
      delete [] ((map<gdpbv100::MessageTypes,unsigned int>*)p);
   }
   static void destruct_maplEgdpbv100cLcLMessageTypescOunsignedsPintgR(void *p) {
      typedef map<gdpbv100::MessageTypes,unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<gdpbv100::MessageTypes,unsigned int>

namespace {
  void TriggerDictionaryInitialization_libCbmFlibStar2019_Impl() {
    static const char* headers[] = {
"CbmStar2019TofPar.h",
"CbmStar2019ContFact.h",
"CbmStar2019MonitorAlgo.h",
"CbmStar2019MonitorTask.h",
"CbmStar2019MonitorPulserAlgo.h",
"CbmStar2019MonitorPulserTask.h",
"CbmTofStarData2019.h",
"CbmStar2019EventBuilderEtofAlgo.h",
"CbmStar2019EventBuilderEtof.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/fles/star2019",
"/home/heinemann/gpuunpacker/cbmroot/fles/star2019/parameter",
"/home/heinemann/gpuunpacker/cbmroot/fles/star2019/monitor",
"/home/heinemann/gpuunpacker/cbmroot/fles/star2019/eventbuilder",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/fles/star2019",
"/home/heinemann/gpuunpacker/cbmroot/fles/star2019/parameter",
"/home/heinemann/gpuunpacker/cbmroot/fles/star2019/monitor",
"/home/heinemann/gpuunpacker/cbmroot/fles/star2019/eventbuilder",
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
"/home/heinemann/gpuunpacker/cbmroot/core/base/utils/flestools",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/dataformat",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/commonMQ",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks",
"/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/psd",
"/home/heinemann/gpuunpacker/cbmroot/external/flib_dpb/flib_dpb",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/fles/star2019/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmFlibStar2019 dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmStar2019TofPar.h")))  CbmStar2019TofPar;
class __attribute__((annotate(R"ATTRDUMP(Factory for all TRD parameter containers)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmStar2019ContFact.h")))  CbmStar2019ContFact;
class __attribute__((annotate("$clingAutoload$CbmStar2019MonitorAlgo.h")))  CbmStar2019MonitorAlgo;
class __attribute__((annotate("$clingAutoload$CbmStar2019MonitorTask.h")))  CbmStar2019MonitorTask;
class __attribute__((annotate("$clingAutoload$CbmStar2019MonitorPulserAlgo.h")))  CbmStar2019MonitorPulserAlgo;
class __attribute__((annotate("$clingAutoload$CbmStar2019MonitorPulserTask.h")))  CbmStar2019MonitorPulserTask;
class __attribute__((annotate("$clingAutoload$CbmTofStarData2019.h")))  CbmTofStarTrigger2019;
class __attribute__((annotate("$clingAutoload$CbmTofStarData2019.h")))  CbmTofStarSubevent2019;
class __attribute__((annotate("$clingAutoload$CbmStar2019EventBuilderEtofAlgo.h")))  CbmStar2019EventBuilderEtofAlgo;
class __attribute__((annotate("$clingAutoload$CbmStar2019EventBuilderEtof.h")))  CbmStar2019EventBuilderEtof;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmFlibStar2019 dictionary payload"

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
#include "CbmStar2019TofPar.h"
#include "CbmStar2019ContFact.h"
#include "CbmStar2019MonitorAlgo.h"
#include "CbmStar2019MonitorTask.h"
#include "CbmStar2019MonitorPulserAlgo.h"
#include "CbmStar2019MonitorPulserTask.h"
#include "CbmTofStarData2019.h"
#include "CbmStar2019EventBuilderEtofAlgo.h"
#include "CbmStar2019EventBuilderEtof.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmStar2019ContFact", payloadCode, "@",
"CbmStar2019EventBuilderEtof", payloadCode, "@",
"CbmStar2019EventBuilderEtofAlgo", payloadCode, "@",
"CbmStar2019MonitorAlgo", payloadCode, "@",
"CbmStar2019MonitorPulserAlgo", payloadCode, "@",
"CbmStar2019MonitorPulserTask", payloadCode, "@",
"CbmStar2019MonitorTask", payloadCode, "@",
"CbmStar2019TofPar", payloadCode, "@",
"CbmTofStarSubevent2019", payloadCode, "@",
"CbmTofStarTrigger2019", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmFlibStar2019",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmFlibStar2019_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmFlibStar2019_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmFlibStar2019() {
  TriggerDictionaryInitialization_libCbmFlibStar2019_Impl();
}
