// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__AnalysisTreeQA
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
#include "EntryConfig.hpp"
#include "Task.hpp"
#include "BasicQA.hpp"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace AnalysisTree {
   namespace QA {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *AnalysisTreecLcLQA_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("AnalysisTree::QA", 0 /*version*/, "EntryConfig.hpp", 17,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &AnalysisTreecLcLQA_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *AnalysisTreecLcLQA_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}
}

namespace ROOT {
   static void *new_AnalysisTreecLcLQAcLcLAxis(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLQAcLcLAxis(Long_t size, void *p);
   static void delete_AnalysisTreecLcLQAcLcLAxis(void *p);
   static void deleteArray_AnalysisTreecLcLQAcLcLAxis(void *p);
   static void destruct_AnalysisTreecLcLQAcLcLAxis(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::QA::Axis*)
   {
      ::AnalysisTree::QA::Axis *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::QA::Axis >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::QA::Axis", ::AnalysisTree::QA::Axis::Class_Version(), "EntryConfig.hpp", 19,
                  typeid(::AnalysisTree::QA::Axis), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::QA::Axis::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::QA::Axis) );
      instance.SetNew(&new_AnalysisTreecLcLQAcLcLAxis);
      instance.SetNewArray(&newArray_AnalysisTreecLcLQAcLcLAxis);
      instance.SetDelete(&delete_AnalysisTreecLcLQAcLcLAxis);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLQAcLcLAxis);
      instance.SetDestructor(&destruct_AnalysisTreecLcLQAcLcLAxis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::QA::Axis*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::QA::Axis*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::QA::Axis*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLQAcLcLEntryConfig(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLQAcLcLEntryConfig(Long_t size, void *p);
   static void delete_AnalysisTreecLcLQAcLcLEntryConfig(void *p);
   static void deleteArray_AnalysisTreecLcLQAcLcLEntryConfig(void *p);
   static void destruct_AnalysisTreecLcLQAcLcLEntryConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::QA::EntryConfig*)
   {
      ::AnalysisTree::QA::EntryConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::QA::EntryConfig >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::QA::EntryConfig", ::AnalysisTree::QA::EntryConfig::Class_Version(), "EntryConfig.hpp", 37,
                  typeid(::AnalysisTree::QA::EntryConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::QA::EntryConfig::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::QA::EntryConfig) );
      instance.SetNew(&new_AnalysisTreecLcLQAcLcLEntryConfig);
      instance.SetNewArray(&newArray_AnalysisTreecLcLQAcLcLEntryConfig);
      instance.SetDelete(&delete_AnalysisTreecLcLQAcLcLEntryConfig);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLQAcLcLEntryConfig);
      instance.SetDestructor(&destruct_AnalysisTreecLcLQAcLcLEntryConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::QA::EntryConfig*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::QA::EntryConfig*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::QA::EntryConfig*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLQAcLcLTask(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLQAcLcLTask(Long_t size, void *p);
   static void delete_AnalysisTreecLcLQAcLcLTask(void *p);
   static void deleteArray_AnalysisTreecLcLQAcLcLTask(void *p);
   static void destruct_AnalysisTreecLcLQAcLcLTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::QA::Task*)
   {
      ::AnalysisTree::QA::Task *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::QA::Task >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::QA::Task", ::AnalysisTree::QA::Task::Class_Version(), "Task.hpp", 15,
                  typeid(::AnalysisTree::QA::Task), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::QA::Task::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::QA::Task) );
      instance.SetNew(&new_AnalysisTreecLcLQAcLcLTask);
      instance.SetNewArray(&newArray_AnalysisTreecLcLQAcLcLTask);
      instance.SetDelete(&delete_AnalysisTreecLcLQAcLcLTask);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLQAcLcLTask);
      instance.SetDestructor(&destruct_AnalysisTreecLcLQAcLcLTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::QA::Task*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::QA::Task*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::QA::Task*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace AnalysisTree {
   namespace QA {
//______________________________________________________________________________
atomic_TClass_ptr Axis::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Axis::Class_Name()
{
   return "AnalysisTree::QA::Axis";
}

//______________________________________________________________________________
const char *Axis::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::QA::Axis*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Axis::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::QA::Axis*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Axis::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::QA::Axis*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Axis::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::QA::Axis*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree::QA
} // namespace AnalysisTree::QA
namespace AnalysisTree {
   namespace QA {
//______________________________________________________________________________
atomic_TClass_ptr EntryConfig::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *EntryConfig::Class_Name()
{
   return "AnalysisTree::QA::EntryConfig";
}

//______________________________________________________________________________
const char *EntryConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::QA::EntryConfig*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int EntryConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::QA::EntryConfig*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *EntryConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::QA::EntryConfig*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EntryConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::QA::EntryConfig*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree::QA
} // namespace AnalysisTree::QA
namespace AnalysisTree {
   namespace QA {
//______________________________________________________________________________
atomic_TClass_ptr Task::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Task::Class_Name()
{
   return "AnalysisTree::QA::Task";
}

//______________________________________________________________________________
const char *Task::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::QA::Task*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Task::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::QA::Task*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Task::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::QA::Task*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Task::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::QA::Task*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree::QA
} // namespace AnalysisTree::QA
namespace AnalysisTree {
   namespace QA {
//______________________________________________________________________________
void Axis::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::QA::Axis.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::QA::Axis::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::QA::Axis::Class(),this);
   }
}

} // namespace AnalysisTree::QA
} // namespace AnalysisTree::QA
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLQAcLcLAxis(void *p) {
      return  p ? new(p) ::AnalysisTree::QA::Axis : new ::AnalysisTree::QA::Axis;
   }
   static void *newArray_AnalysisTreecLcLQAcLcLAxis(Long_t nElements, void *p) {
      return p ? new(p) ::AnalysisTree::QA::Axis[nElements] : new ::AnalysisTree::QA::Axis[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLQAcLcLAxis(void *p) {
      delete ((::AnalysisTree::QA::Axis*)p);
   }
   static void deleteArray_AnalysisTreecLcLQAcLcLAxis(void *p) {
      delete [] ((::AnalysisTree::QA::Axis*)p);
   }
   static void destruct_AnalysisTreecLcLQAcLcLAxis(void *p) {
      typedef ::AnalysisTree::QA::Axis current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::QA::Axis

namespace AnalysisTree {
   namespace QA {
//______________________________________________________________________________
void EntryConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::QA::EntryConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::QA::EntryConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::QA::EntryConfig::Class(),this);
   }
}

} // namespace AnalysisTree::QA
} // namespace AnalysisTree::QA
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLQAcLcLEntryConfig(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::QA::EntryConfig : new ::AnalysisTree::QA::EntryConfig;
   }
   static void *newArray_AnalysisTreecLcLQAcLcLEntryConfig(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::QA::EntryConfig[nElements] : new ::AnalysisTree::QA::EntryConfig[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLQAcLcLEntryConfig(void *p) {
      delete ((::AnalysisTree::QA::EntryConfig*)p);
   }
   static void deleteArray_AnalysisTreecLcLQAcLcLEntryConfig(void *p) {
      delete [] ((::AnalysisTree::QA::EntryConfig*)p);
   }
   static void destruct_AnalysisTreecLcLQAcLcLEntryConfig(void *p) {
      typedef ::AnalysisTree::QA::EntryConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::QA::EntryConfig

namespace AnalysisTree {
   namespace QA {
//______________________________________________________________________________
void Task::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::QA::Task.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::QA::Task::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::QA::Task::Class(),this);
   }
}

} // namespace AnalysisTree::QA
} // namespace AnalysisTree::QA
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLQAcLcLTask(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::QA::Task : new ::AnalysisTree::QA::Task;
   }
   static void *newArray_AnalysisTreecLcLQAcLcLTask(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::QA::Task[nElements] : new ::AnalysisTree::QA::Task[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLQAcLcLTask(void *p) {
      delete ((::AnalysisTree::QA::Task*)p);
   }
   static void deleteArray_AnalysisTreecLcLQAcLcLTask(void *p) {
      delete [] ((::AnalysisTree::QA::Task*)p);
   }
   static void destruct_AnalysisTreecLcLQAcLcLTask(void *p) {
      typedef ::AnalysisTree::QA::Task current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::QA::Task

namespace ROOT {
   static TClass *vectorlEpairlEintcOintgRsPgR_Dictionary();
   static void vectorlEpairlEintcOintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlEintcOintgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEpairlEintcOintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlEintcOintgRsPgR(void *p);
   static void deleteArray_vectorlEpairlEintcOintgRsPgR(void *p);
   static void destruct_vectorlEpairlEintcOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<int,int> >*)
   {
      vector<pair<int,int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<int,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<int,int> >", -2, "vector", 389,
                  typeid(vector<pair<int,int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlEintcOintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<int,int> >) );
      instance.SetNew(&new_vectorlEpairlEintcOintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlEintcOintgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlEintcOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlEintcOintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlEintcOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<int,int> > >()));

      ::ROOT::AddClassAlternate("vector<pair<int,int> >","std::vector<std::pair<int, int>, std::allocator<std::pair<int, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pair<int,int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlEintcOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<int,int> >*)nullptr)->GetClass();
      vectorlEpairlEintcOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlEintcOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlEintcOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<int,int> > : new vector<pair<int,int> >;
   }
   static void *newArray_vectorlEpairlEintcOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<int,int> >[nElements] : new vector<pair<int,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlEintcOintgRsPgR(void *p) {
      delete ((vector<pair<int,int> >*)p);
   }
   static void deleteArray_vectorlEpairlEintcOintgRsPgR(void *p) {
      delete [] ((vector<pair<int,int> >*)p);
   }
   static void destruct_vectorlEpairlEintcOintgRsPgR(void *p) {
      typedef vector<pair<int,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<int,int> >

namespace ROOT {
   static TClass *vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR_Dictionary();
   static void vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR_TClassManip(TClass*);
   static void *new_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR(void *p = nullptr);
   static void *newArray_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR(Long_t size, void *p);
   static void delete_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR(void *p);
   static void deleteArray_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR(void *p);
   static void destruct_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<AnalysisTree::QA::EntryConfig>*)
   {
      vector<AnalysisTree::QA::EntryConfig> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<AnalysisTree::QA::EntryConfig>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<AnalysisTree::QA::EntryConfig>", -2, "vector", 389,
                  typeid(vector<AnalysisTree::QA::EntryConfig>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR_Dictionary, isa_proxy, 0,
                  sizeof(vector<AnalysisTree::QA::EntryConfig>) );
      instance.SetNew(&new_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR);
      instance.SetNewArray(&newArray_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR);
      instance.SetDelete(&delete_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR);
      instance.SetDeleteArray(&deleteArray_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR);
      instance.SetDestructor(&destruct_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<AnalysisTree::QA::EntryConfig> >()));

      ::ROOT::AddClassAlternate("vector<AnalysisTree::QA::EntryConfig>","std::vector<AnalysisTree::QA::EntryConfig, std::allocator<AnalysisTree::QA::EntryConfig> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<AnalysisTree::QA::EntryConfig>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<AnalysisTree::QA::EntryConfig>*)nullptr)->GetClass();
      vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::QA::EntryConfig> : new vector<AnalysisTree::QA::EntryConfig>;
   }
   static void *newArray_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::QA::EntryConfig>[nElements] : new vector<AnalysisTree::QA::EntryConfig>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR(void *p) {
      delete ((vector<AnalysisTree::QA::EntryConfig>*)p);
   }
   static void deleteArray_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR(void *p) {
      delete [] ((vector<AnalysisTree::QA::EntryConfig>*)p);
   }
   static void destruct_vectorlEAnalysisTreecLcLQAcLcLEntryConfiggR(void *p) {
      typedef vector<AnalysisTree::QA::EntryConfig> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<AnalysisTree::QA::EntryConfig>

namespace ROOT {
   static TClass *vectorlEAnalysisTreecLcLQAcLcLAxisgR_Dictionary();
   static void vectorlEAnalysisTreecLcLQAcLcLAxisgR_TClassManip(TClass*);
   static void *new_vectorlEAnalysisTreecLcLQAcLcLAxisgR(void *p = nullptr);
   static void *newArray_vectorlEAnalysisTreecLcLQAcLcLAxisgR(Long_t size, void *p);
   static void delete_vectorlEAnalysisTreecLcLQAcLcLAxisgR(void *p);
   static void deleteArray_vectorlEAnalysisTreecLcLQAcLcLAxisgR(void *p);
   static void destruct_vectorlEAnalysisTreecLcLQAcLcLAxisgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<AnalysisTree::QA::Axis>*)
   {
      vector<AnalysisTree::QA::Axis> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<AnalysisTree::QA::Axis>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<AnalysisTree::QA::Axis>", -2, "vector", 389,
                  typeid(vector<AnalysisTree::QA::Axis>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAnalysisTreecLcLQAcLcLAxisgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<AnalysisTree::QA::Axis>) );
      instance.SetNew(&new_vectorlEAnalysisTreecLcLQAcLcLAxisgR);
      instance.SetNewArray(&newArray_vectorlEAnalysisTreecLcLQAcLcLAxisgR);
      instance.SetDelete(&delete_vectorlEAnalysisTreecLcLQAcLcLAxisgR);
      instance.SetDeleteArray(&deleteArray_vectorlEAnalysisTreecLcLQAcLcLAxisgR);
      instance.SetDestructor(&destruct_vectorlEAnalysisTreecLcLQAcLcLAxisgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<AnalysisTree::QA::Axis> >()));

      ::ROOT::AddClassAlternate("vector<AnalysisTree::QA::Axis>","std::vector<AnalysisTree::QA::Axis, std::allocator<AnalysisTree::QA::Axis> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<AnalysisTree::QA::Axis>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAnalysisTreecLcLQAcLcLAxisgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<AnalysisTree::QA::Axis>*)nullptr)->GetClass();
      vectorlEAnalysisTreecLcLQAcLcLAxisgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAnalysisTreecLcLQAcLcLAxisgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAnalysisTreecLcLQAcLcLAxisgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::QA::Axis> : new vector<AnalysisTree::QA::Axis>;
   }
   static void *newArray_vectorlEAnalysisTreecLcLQAcLcLAxisgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::QA::Axis>[nElements] : new vector<AnalysisTree::QA::Axis>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAnalysisTreecLcLQAcLcLAxisgR(void *p) {
      delete ((vector<AnalysisTree::QA::Axis>*)p);
   }
   static void deleteArray_vectorlEAnalysisTreecLcLQAcLcLAxisgR(void *p) {
      delete [] ((vector<AnalysisTree::QA::Axis>*)p);
   }
   static void destruct_vectorlEAnalysisTreecLcLQAcLcLAxisgR(void *p) {
      typedef vector<AnalysisTree::QA::Axis> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<AnalysisTree::QA::Axis>

namespace ROOT {
   static TClass *maplEstringcOTDirectorymUgR_Dictionary();
   static void maplEstringcOTDirectorymUgR_TClassManip(TClass*);
   static void *new_maplEstringcOTDirectorymUgR(void *p = nullptr);
   static void *newArray_maplEstringcOTDirectorymUgR(Long_t size, void *p);
   static void delete_maplEstringcOTDirectorymUgR(void *p);
   static void deleteArray_maplEstringcOTDirectorymUgR(void *p);
   static void destruct_maplEstringcOTDirectorymUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,TDirectory*>*)
   {
      map<string,TDirectory*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,TDirectory*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,TDirectory*>", -2, "map", 100,
                  typeid(map<string,TDirectory*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOTDirectorymUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,TDirectory*>) );
      instance.SetNew(&new_maplEstringcOTDirectorymUgR);
      instance.SetNewArray(&newArray_maplEstringcOTDirectorymUgR);
      instance.SetDelete(&delete_maplEstringcOTDirectorymUgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOTDirectorymUgR);
      instance.SetDestructor(&destruct_maplEstringcOTDirectorymUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,TDirectory*> >()));

      ::ROOT::AddClassAlternate("map<string,TDirectory*>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, TDirectory*, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, TDirectory*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,TDirectory*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOTDirectorymUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,TDirectory*>*)nullptr)->GetClass();
      maplEstringcOTDirectorymUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOTDirectorymUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOTDirectorymUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,TDirectory*> : new map<string,TDirectory*>;
   }
   static void *newArray_maplEstringcOTDirectorymUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,TDirectory*>[nElements] : new map<string,TDirectory*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOTDirectorymUgR(void *p) {
      delete ((map<string,TDirectory*>*)p);
   }
   static void deleteArray_maplEstringcOTDirectorymUgR(void *p) {
      delete [] ((map<string,TDirectory*>*)p);
   }
   static void destruct_maplEstringcOTDirectorymUgR(void *p) {
      typedef map<string,TDirectory*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,TDirectory*>

namespace {
  void TriggerDictionaryInitialization_libAnalysisTreeQA_Impl() {
    static const char* headers[] = {
"EntryConfig.hpp",
"Task.hpp",
"BasicQA.hpp",
nullptr
    };
    static const char* includePaths[] = {
"/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/include",
"/include",
"/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src",
"/home/heinemann/gpuunpacker/cbmroot/build2/include",
"/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libAnalysisTreeQA dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace AnalysisTree{namespace QA{class __attribute__((annotate("$clingAutoload$EntryConfig.hpp")))  Axis;}}
namespace AnalysisTree{namespace QA{class __attribute__((annotate("$clingAutoload$EntryConfig.hpp")))  EntryConfig;}}
namespace AnalysisTree{namespace QA{class __attribute__((annotate("$clingAutoload$Task.hpp")))  Task;}}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libAnalysisTreeQA dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "EntryConfig.hpp"
#include "Task.hpp"
#include "BasicQA.hpp"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"AnalysisTree::QA::Axis", payloadCode, "@",
"AnalysisTree::QA::EntryConfig", payloadCode, "@",
"AnalysisTree::QA::Task", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libAnalysisTreeQA",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libAnalysisTreeQA_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libAnalysisTreeQA_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libAnalysisTreeQA() {
  TriggerDictionaryInitialization_libAnalysisTreeQA_Impl();
}
