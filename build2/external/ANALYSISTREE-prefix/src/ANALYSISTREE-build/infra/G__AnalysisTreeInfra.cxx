// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__AnalysisTreeInfra
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
#include "Cuts.hpp"
#include "Field.hpp"
#include "Variable.hpp"
#include "Task.hpp"
#include "AnalysisTask.hpp"
#include "TaskManager.hpp"
#include "PlainTreeFiller.hpp"
#include "Chain.hpp"
#include "ChainDrawHelper.hpp"
#include "Branch.hpp"
#include "BranchChannel.hpp"
#include "AnalysisEntry.hpp"
#include "VariantMagic.hpp"
#include "ToyMC.hpp"
#include "Utils.hpp"
#include "BranchHashHelper.hpp"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_AnalysisTreecLcLField(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLField(Long_t size, void *p);
   static void delete_AnalysisTreecLcLField(void *p);
   static void deleteArray_AnalysisTreecLcLField(void *p);
   static void destruct_AnalysisTreecLcLField(void *p);
   static void streamer_AnalysisTreecLcLField(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Field*)
   {
      ::AnalysisTree::Field *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Field >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Field", ::AnalysisTree::Field::Class_Version(), "Field.hpp", 22,
                  typeid(::AnalysisTree::Field), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::Field::Dictionary, isa_proxy, 16,
                  sizeof(::AnalysisTree::Field) );
      instance.SetNew(&new_AnalysisTreecLcLField);
      instance.SetNewArray(&newArray_AnalysisTreecLcLField);
      instance.SetDelete(&delete_AnalysisTreecLcLField);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLField);
      instance.SetDestructor(&destruct_AnalysisTreecLcLField);
      instance.SetStreamerFunc(&streamer_AnalysisTreecLcLField);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Field*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Field*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Field*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLVariable(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLVariable(Long_t size, void *p);
   static void delete_AnalysisTreecLcLVariable(void *p);
   static void deleteArray_AnalysisTreecLcLVariable(void *p);
   static void destruct_AnalysisTreecLcLVariable(void *p);
   static void streamer_AnalysisTreecLcLVariable(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Variable*)
   {
      ::AnalysisTree::Variable *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Variable >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Variable", ::AnalysisTree::Variable::Class_Version(), "Variable.hpp", 23,
                  typeid(::AnalysisTree::Variable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::Variable::Dictionary, isa_proxy, 16,
                  sizeof(::AnalysisTree::Variable) );
      instance.SetNew(&new_AnalysisTreecLcLVariable);
      instance.SetNewArray(&newArray_AnalysisTreecLcLVariable);
      instance.SetDelete(&delete_AnalysisTreecLcLVariable);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLVariable);
      instance.SetDestructor(&destruct_AnalysisTreecLcLVariable);
      instance.SetStreamerFunc(&streamer_AnalysisTreecLcLVariable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Variable*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Variable*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Variable*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLSimpleCut(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLSimpleCut(Long_t size, void *p);
   static void delete_AnalysisTreecLcLSimpleCut(void *p);
   static void deleteArray_AnalysisTreecLcLSimpleCut(void *p);
   static void destruct_AnalysisTreecLcLSimpleCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::SimpleCut*)
   {
      ::AnalysisTree::SimpleCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::SimpleCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::SimpleCut", ::AnalysisTree::SimpleCut::Class_Version(), "SimpleCut.hpp", 22,
                  typeid(::AnalysisTree::SimpleCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::SimpleCut::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::SimpleCut) );
      instance.SetNew(&new_AnalysisTreecLcLSimpleCut);
      instance.SetNewArray(&newArray_AnalysisTreecLcLSimpleCut);
      instance.SetDelete(&delete_AnalysisTreecLcLSimpleCut);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLSimpleCut);
      instance.SetDestructor(&destruct_AnalysisTreecLcLSimpleCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::SimpleCut*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::SimpleCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::SimpleCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLCuts(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLCuts(Long_t size, void *p);
   static void delete_AnalysisTreecLcLCuts(void *p);
   static void deleteArray_AnalysisTreecLcLCuts(void *p);
   static void destruct_AnalysisTreecLcLCuts(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Cuts*)
   {
      ::AnalysisTree::Cuts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Cuts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Cuts", ::AnalysisTree::Cuts::Class_Version(), "Cuts.hpp", 25,
                  typeid(::AnalysisTree::Cuts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::Cuts::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Cuts) );
      instance.SetNew(&new_AnalysisTreecLcLCuts);
      instance.SetNewArray(&newArray_AnalysisTreecLcLCuts);
      instance.SetDelete(&delete_AnalysisTreecLcLCuts);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLCuts);
      instance.SetDestructor(&destruct_AnalysisTreecLcLCuts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Cuts*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Cuts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Cuts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLChain(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLChain(Long_t size, void *p);
   static void delete_AnalysisTreecLcLChain(void *p);
   static void deleteArray_AnalysisTreecLcLChain(void *p);
   static void destruct_AnalysisTreecLcLChain(void *p);
   static void directoryAutoAdd_AnalysisTreecLcLChain(void *obj, TDirectory *dir);
   static Long64_t merge_AnalysisTreecLcLChain(void *obj, TCollection *coll,TFileMergeInfo *info);
   static void reset_AnalysisTreecLcLChain(void *obj, TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Chain*)
   {
      ::AnalysisTree::Chain *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Chain >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Chain", ::AnalysisTree::Chain::Class_Version(), "Chain.hpp", 24,
                  typeid(::AnalysisTree::Chain), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::Chain::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Chain) );
      instance.SetNew(&new_AnalysisTreecLcLChain);
      instance.SetNewArray(&newArray_AnalysisTreecLcLChain);
      instance.SetDelete(&delete_AnalysisTreecLcLChain);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLChain);
      instance.SetDestructor(&destruct_AnalysisTreecLcLChain);
      instance.SetDirectoryAutoAdd(&directoryAutoAdd_AnalysisTreecLcLChain);
      instance.SetMerge(&merge_AnalysisTreecLcLChain);
      instance.SetResetAfterMerge(&reset_AnalysisTreecLcLChain);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Chain*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Chain*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Chain*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_AnalysisTreecLcLTask(void *p);
   static void deleteArray_AnalysisTreecLcLTask(void *p);
   static void destruct_AnalysisTreecLcLTask(void *p);
   static void streamer_AnalysisTreecLcLTask(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Task*)
   {
      ::AnalysisTree::Task *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Task >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Task", ::AnalysisTree::Task::Class_Version(), "Task.hpp", 25,
                  typeid(::AnalysisTree::Task), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::Task::Dictionary, isa_proxy, 16,
                  sizeof(::AnalysisTree::Task) );
      instance.SetDelete(&delete_AnalysisTreecLcLTask);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLTask);
      instance.SetDestructor(&destruct_AnalysisTreecLcLTask);
      instance.SetStreamerFunc(&streamer_AnalysisTreecLcLTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Task*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Task*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Task*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLAnalysisEntry(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLAnalysisEntry(Long_t size, void *p);
   static void delete_AnalysisTreecLcLAnalysisEntry(void *p);
   static void deleteArray_AnalysisTreecLcLAnalysisEntry(void *p);
   static void destruct_AnalysisTreecLcLAnalysisEntry(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::AnalysisEntry*)
   {
      ::AnalysisTree::AnalysisEntry *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::AnalysisEntry >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::AnalysisEntry", ::AnalysisTree::AnalysisEntry::Class_Version(), "AnalysisEntry.hpp", 20,
                  typeid(::AnalysisTree::AnalysisEntry), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::AnalysisEntry::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::AnalysisEntry) );
      instance.SetNew(&new_AnalysisTreecLcLAnalysisEntry);
      instance.SetNewArray(&newArray_AnalysisTreecLcLAnalysisEntry);
      instance.SetDelete(&delete_AnalysisTreecLcLAnalysisEntry);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLAnalysisEntry);
      instance.SetDestructor(&destruct_AnalysisTreecLcLAnalysisEntry);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::AnalysisEntry*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::AnalysisEntry*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::AnalysisEntry*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLAnalysisTask(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLAnalysisTask(Long_t size, void *p);
   static void delete_AnalysisTreecLcLAnalysisTask(void *p);
   static void deleteArray_AnalysisTreecLcLAnalysisTask(void *p);
   static void destruct_AnalysisTreecLcLAnalysisTask(void *p);
   static void streamer_AnalysisTreecLcLAnalysisTask(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::AnalysisTask*)
   {
      ::AnalysisTree::AnalysisTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::AnalysisTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::AnalysisTask", ::AnalysisTree::AnalysisTask::Class_Version(), "AnalysisTask.hpp", 21,
                  typeid(::AnalysisTree::AnalysisTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::AnalysisTask::Dictionary, isa_proxy, 16,
                  sizeof(::AnalysisTree::AnalysisTask) );
      instance.SetNew(&new_AnalysisTreecLcLAnalysisTask);
      instance.SetNewArray(&newArray_AnalysisTreecLcLAnalysisTask);
      instance.SetDelete(&delete_AnalysisTreecLcLAnalysisTask);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLAnalysisTask);
      instance.SetDestructor(&destruct_AnalysisTreecLcLAnalysisTask);
      instance.SetStreamerFunc(&streamer_AnalysisTreecLcLAnalysisTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::AnalysisTask*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::AnalysisTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::AnalysisTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_AnalysisTreecLcLTaskManager(void *p);
   static void deleteArray_AnalysisTreecLcLTaskManager(void *p);
   static void destruct_AnalysisTreecLcLTaskManager(void *p);
   static void streamer_AnalysisTreecLcLTaskManager(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::TaskManager*)
   {
      ::AnalysisTree::TaskManager *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::TaskManager >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::TaskManager", ::AnalysisTree::TaskManager::Class_Version(), "TaskManager.hpp", 32,
                  typeid(::AnalysisTree::TaskManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::TaskManager::Dictionary, isa_proxy, 16,
                  sizeof(::AnalysisTree::TaskManager) );
      instance.SetDelete(&delete_AnalysisTreecLcLTaskManager);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLTaskManager);
      instance.SetDestructor(&destruct_AnalysisTreecLcLTaskManager);
      instance.SetStreamerFunc(&streamer_AnalysisTreecLcLTaskManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::TaskManager*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::TaskManager*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::TaskManager*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr Field::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Field::Class_Name()
{
   return "AnalysisTree::Field";
}

//______________________________________________________________________________
const char *Field::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Field*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Field::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Field*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Field::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Field*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Field::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Field*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr Variable::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Variable::Class_Name()
{
   return "AnalysisTree::Variable";
}

//______________________________________________________________________________
const char *Variable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Variable*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Variable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Variable*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Variable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Variable*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Variable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Variable*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr SimpleCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *SimpleCut::Class_Name()
{
   return "AnalysisTree::SimpleCut";
}

//______________________________________________________________________________
const char *SimpleCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::SimpleCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int SimpleCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::SimpleCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SimpleCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::SimpleCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SimpleCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::SimpleCut*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr Cuts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Cuts::Class_Name()
{
   return "AnalysisTree::Cuts";
}

//______________________________________________________________________________
const char *Cuts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Cuts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Cuts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Cuts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Cuts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Cuts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Cuts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Cuts*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr Chain::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Chain::Class_Name()
{
   return "AnalysisTree::Chain";
}

//______________________________________________________________________________
const char *Chain::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Chain*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Chain::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Chain*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Chain::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Chain*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Chain::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Chain*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr Task::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Task::Class_Name()
{
   return "AnalysisTree::Task";
}

//______________________________________________________________________________
const char *Task::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Task*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Task::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Task*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Task::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Task*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Task::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Task*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr AnalysisEntry::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AnalysisEntry::Class_Name()
{
   return "AnalysisTree::AnalysisEntry";
}

//______________________________________________________________________________
const char *AnalysisEntry::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::AnalysisEntry*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AnalysisEntry::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::AnalysisEntry*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AnalysisEntry::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::AnalysisEntry*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AnalysisEntry::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::AnalysisEntry*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr AnalysisTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AnalysisTask::Class_Name()
{
   return "AnalysisTree::AnalysisTask";
}

//______________________________________________________________________________
const char *AnalysisTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::AnalysisTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AnalysisTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::AnalysisTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AnalysisTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::AnalysisTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AnalysisTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::AnalysisTask*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr TaskManager::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TaskManager::Class_Name()
{
   return "AnalysisTree::TaskManager";
}

//______________________________________________________________________________
const char *TaskManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::TaskManager*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TaskManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::TaskManager*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TaskManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::TaskManager*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TaskManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::TaskManager*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
void Field::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Field.

   ::Error("AnalysisTree::Field::Streamer", "version id <=0 in ClassDef, dummy Streamer() called"); if (R__b.IsReading()) { }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLField(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Field : new ::AnalysisTree::Field;
   }
   static void *newArray_AnalysisTreecLcLField(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Field[nElements] : new ::AnalysisTree::Field[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLField(void *p) {
      delete ((::AnalysisTree::Field*)p);
   }
   static void deleteArray_AnalysisTreecLcLField(void *p) {
      delete [] ((::AnalysisTree::Field*)p);
   }
   static void destruct_AnalysisTreecLcLField(void *p) {
      typedef ::AnalysisTree::Field current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AnalysisTreecLcLField(TBuffer &buf, void *obj) {
      ((::AnalysisTree::Field*)obj)->::AnalysisTree::Field::Streamer(buf);
   }
} // end of namespace ROOT for class ::AnalysisTree::Field

namespace AnalysisTree {
//______________________________________________________________________________
void Variable::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Variable.

   ::Error("AnalysisTree::Variable::Streamer", "version id <=0 in ClassDef, dummy Streamer() called"); if (R__b.IsReading()) { }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLVariable(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Variable : new ::AnalysisTree::Variable;
   }
   static void *newArray_AnalysisTreecLcLVariable(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Variable[nElements] : new ::AnalysisTree::Variable[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLVariable(void *p) {
      delete ((::AnalysisTree::Variable*)p);
   }
   static void deleteArray_AnalysisTreecLcLVariable(void *p) {
      delete [] ((::AnalysisTree::Variable*)p);
   }
   static void destruct_AnalysisTreecLcLVariable(void *p) {
      typedef ::AnalysisTree::Variable current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AnalysisTreecLcLVariable(TBuffer &buf, void *obj) {
      ((::AnalysisTree::Variable*)obj)->::AnalysisTree::Variable::Streamer(buf);
   }
} // end of namespace ROOT for class ::AnalysisTree::Variable

namespace AnalysisTree {
//______________________________________________________________________________
void SimpleCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::SimpleCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::SimpleCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::SimpleCut::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLSimpleCut(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::SimpleCut : new ::AnalysisTree::SimpleCut;
   }
   static void *newArray_AnalysisTreecLcLSimpleCut(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::SimpleCut[nElements] : new ::AnalysisTree::SimpleCut[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLSimpleCut(void *p) {
      delete ((::AnalysisTree::SimpleCut*)p);
   }
   static void deleteArray_AnalysisTreecLcLSimpleCut(void *p) {
      delete [] ((::AnalysisTree::SimpleCut*)p);
   }
   static void destruct_AnalysisTreecLcLSimpleCut(void *p) {
      typedef ::AnalysisTree::SimpleCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::SimpleCut

namespace AnalysisTree {
//______________________________________________________________________________
void Cuts::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Cuts.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Cuts::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Cuts::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLCuts(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Cuts : new ::AnalysisTree::Cuts;
   }
   static void *newArray_AnalysisTreecLcLCuts(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Cuts[nElements] : new ::AnalysisTree::Cuts[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLCuts(void *p) {
      delete ((::AnalysisTree::Cuts*)p);
   }
   static void deleteArray_AnalysisTreecLcLCuts(void *p) {
      delete [] ((::AnalysisTree::Cuts*)p);
   }
   static void destruct_AnalysisTreecLcLCuts(void *p) {
      typedef ::AnalysisTree::Cuts current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::Cuts

namespace AnalysisTree {
//______________________________________________________________________________
void Chain::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Chain.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Chain::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Chain::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLChain(void *p) {
      return  p ? new(p) ::AnalysisTree::Chain : new ::AnalysisTree::Chain;
   }
   static void *newArray_AnalysisTreecLcLChain(Long_t nElements, void *p) {
      return p ? new(p) ::AnalysisTree::Chain[nElements] : new ::AnalysisTree::Chain[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLChain(void *p) {
      delete ((::AnalysisTree::Chain*)p);
   }
   static void deleteArray_AnalysisTreecLcLChain(void *p) {
      delete [] ((::AnalysisTree::Chain*)p);
   }
   static void destruct_AnalysisTreecLcLChain(void *p) {
      typedef ::AnalysisTree::Chain current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the directory auto add.
   static void directoryAutoAdd_AnalysisTreecLcLChain(void *p, TDirectory *dir) {
      ((::AnalysisTree::Chain*)p)->DirectoryAutoAdd(dir);
   }
   // Wrapper around the merge function.
   static Long64_t merge_AnalysisTreecLcLChain(void *obj,TCollection *coll,TFileMergeInfo *info) {
      return ((::AnalysisTree::Chain*)obj)->Merge(coll,info);
   }
   // Wrapper around the Reset function.
   static void reset_AnalysisTreecLcLChain(void *obj,TFileMergeInfo *info) {
      ((::AnalysisTree::Chain*)obj)->ResetAfterMerge(info);
   }
} // end of namespace ROOT for class ::AnalysisTree::Chain

namespace AnalysisTree {
//______________________________________________________________________________
void Task::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Task.

   ::Error("AnalysisTree::Task::Streamer", "version id <=0 in ClassDef, dummy Streamer() called"); if (R__b.IsReading()) { }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLTask(void *p) {
      delete ((::AnalysisTree::Task*)p);
   }
   static void deleteArray_AnalysisTreecLcLTask(void *p) {
      delete [] ((::AnalysisTree::Task*)p);
   }
   static void destruct_AnalysisTreecLcLTask(void *p) {
      typedef ::AnalysisTree::Task current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AnalysisTreecLcLTask(TBuffer &buf, void *obj) {
      ((::AnalysisTree::Task*)obj)->::AnalysisTree::Task::Streamer(buf);
   }
} // end of namespace ROOT for class ::AnalysisTree::Task

namespace AnalysisTree {
//______________________________________________________________________________
void AnalysisEntry::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::AnalysisEntry.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::AnalysisEntry::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::AnalysisEntry::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLAnalysisEntry(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::AnalysisEntry : new ::AnalysisTree::AnalysisEntry;
   }
   static void *newArray_AnalysisTreecLcLAnalysisEntry(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::AnalysisEntry[nElements] : new ::AnalysisTree::AnalysisEntry[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLAnalysisEntry(void *p) {
      delete ((::AnalysisTree::AnalysisEntry*)p);
   }
   static void deleteArray_AnalysisTreecLcLAnalysisEntry(void *p) {
      delete [] ((::AnalysisTree::AnalysisEntry*)p);
   }
   static void destruct_AnalysisTreecLcLAnalysisEntry(void *p) {
      typedef ::AnalysisTree::AnalysisEntry current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::AnalysisEntry

namespace AnalysisTree {
//______________________________________________________________________________
void AnalysisTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::AnalysisTask.

   //This works around a msvc bug and should be harmless on other platforms
   typedef AnalysisTree::Task baseClass0;
   baseClass0::Streamer(R__b);
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLAnalysisTask(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::AnalysisTask : new ::AnalysisTree::AnalysisTask;
   }
   static void *newArray_AnalysisTreecLcLAnalysisTask(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::AnalysisTask[nElements] : new ::AnalysisTree::AnalysisTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLAnalysisTask(void *p) {
      delete ((::AnalysisTree::AnalysisTask*)p);
   }
   static void deleteArray_AnalysisTreecLcLAnalysisTask(void *p) {
      delete [] ((::AnalysisTree::AnalysisTask*)p);
   }
   static void destruct_AnalysisTreecLcLAnalysisTask(void *p) {
      typedef ::AnalysisTree::AnalysisTask current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AnalysisTreecLcLAnalysisTask(TBuffer &buf, void *obj) {
      ((::AnalysisTree::AnalysisTask*)obj)->::AnalysisTree::AnalysisTask::Streamer(buf);
   }
} // end of namespace ROOT for class ::AnalysisTree::AnalysisTask

namespace AnalysisTree {
//______________________________________________________________________________
void TaskManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::TaskManager.

   ::Error("AnalysisTree::TaskManager::Streamer", "version id <=0 in ClassDef, dummy Streamer() called"); if (R__b.IsReading()) { }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLTaskManager(void *p) {
      delete ((::AnalysisTree::TaskManager*)p);
   }
   static void deleteArray_AnalysisTreecLcLTaskManager(void *p) {
      delete [] ((::AnalysisTree::TaskManager*)p);
   }
   static void destruct_AnalysisTreecLcLTaskManager(void *p) {
      typedef ::AnalysisTree::TaskManager current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AnalysisTreecLcLTaskManager(TBuffer &buf, void *obj) {
      ((::AnalysisTree::TaskManager*)obj)->::AnalysisTree::TaskManager::Streamer(buf);
   }
} // end of namespace ROOT for class ::AnalysisTree::TaskManager

namespace ROOT {
   static TClass *vectorlEvectorlEdoublegRsPgR_Dictionary();
   static void vectorlEvectorlEdoublegRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEdoublegRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEdoublegRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEdoublegRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEdoublegRsPgR(void *p);
   static void destruct_vectorlEvectorlEdoublegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<double> >*)
   {
      vector<vector<double> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<double> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<double> >", -2, "vector", 389,
                  typeid(vector<vector<double> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEdoublegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<double> >) );
      instance.SetNew(&new_vectorlEvectorlEdoublegRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEdoublegRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEdoublegRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEdoublegRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEdoublegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<double> > >()));

      ::ROOT::AddClassAlternate("vector<vector<double> >","std::vector<std::vector<double, std::allocator<double> >, std::allocator<std::vector<double, std::allocator<double> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<double> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEdoublegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<double> >*)nullptr)->GetClass();
      vectorlEvectorlEdoublegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEdoublegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEdoublegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<double> > : new vector<vector<double> >;
   }
   static void *newArray_vectorlEvectorlEdoublegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<double> >[nElements] : new vector<vector<double> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEdoublegRsPgR(void *p) {
      delete ((vector<vector<double> >*)p);
   }
   static void deleteArray_vectorlEvectorlEdoublegRsPgR(void *p) {
      delete [] ((vector<vector<double> >*)p);
   }
   static void destruct_vectorlEvectorlEdoublegRsPgR(void *p) {
      typedef vector<vector<double> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<double> >

namespace ROOT {
   static TClass *vectorlEstringgR_Dictionary();
   static void vectorlEstringgR_TClassManip(TClass*);
   static void *new_vectorlEstringgR(void *p = nullptr);
   static void *newArray_vectorlEstringgR(Long_t size, void *p);
   static void delete_vectorlEstringgR(void *p);
   static void deleteArray_vectorlEstringgR(void *p);
   static void destruct_vectorlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<string>*)
   {
      vector<string> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<string>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<string>", -2, "vector", 389,
                  typeid(vector<string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<string>) );
      instance.SetNew(&new_vectorlEstringgR);
      instance.SetNewArray(&newArray_vectorlEstringgR);
      instance.SetDelete(&delete_vectorlEstringgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstringgR);
      instance.SetDestructor(&destruct_vectorlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<string> >()));

      ::ROOT::AddClassAlternate("vector<string>","std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<string>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<string>*)nullptr)->GetClass();
      vectorlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string> : new vector<string>;
   }
   static void *newArray_vectorlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string>[nElements] : new vector<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstringgR(void *p) {
      delete ((vector<string>*)p);
   }
   static void deleteArray_vectorlEstringgR(void *p) {
      delete [] ((vector<string>*)p);
   }
   static void destruct_vectorlEstringgR(void *p) {
      typedef vector<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<string>

namespace ROOT {
   static TClass *vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR_Dictionary();
   static void vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR(void *p);
   static void deleteArray_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR(void *p);
   static void destruct_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> >*)
   {
      vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> >", -2, "vector", 389,
                  typeid(vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> >) );
      instance.SetNew(&new_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> > >()));

      ::ROOT::AddClassAlternate("vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> >","std::vector<std::pair<AnalysisTree::Branch const*, AnalysisTree::Cuts*>, std::allocator<std::pair<AnalysisTree::Branch const*, AnalysisTree::Cuts*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> >*)nullptr)->GetClass();
      vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> > : new vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> >;
   }
   static void *newArray_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> >[nElements] : new vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR(void *p) {
      delete ((vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> >*)p);
   }
   static void deleteArray_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR(void *p) {
      delete [] ((vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> >*)p);
   }
   static void destruct_vectorlEpairlEconstsPAnalysisTreecLcLBranchmUcOAnalysisTreecLcLCutsmUgRsPgR(void *p) {
      typedef vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<const AnalysisTree::Branch*,AnalysisTree::Cuts*> >

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
   static TClass *vectorlEAnalysisTreecLcLVariablegR_Dictionary();
   static void vectorlEAnalysisTreecLcLVariablegR_TClassManip(TClass*);
   static void *new_vectorlEAnalysisTreecLcLVariablegR(void *p = nullptr);
   static void *newArray_vectorlEAnalysisTreecLcLVariablegR(Long_t size, void *p);
   static void delete_vectorlEAnalysisTreecLcLVariablegR(void *p);
   static void deleteArray_vectorlEAnalysisTreecLcLVariablegR(void *p);
   static void destruct_vectorlEAnalysisTreecLcLVariablegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<AnalysisTree::Variable>*)
   {
      vector<AnalysisTree::Variable> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<AnalysisTree::Variable>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<AnalysisTree::Variable>", -2, "vector", 389,
                  typeid(vector<AnalysisTree::Variable>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAnalysisTreecLcLVariablegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<AnalysisTree::Variable>) );
      instance.SetNew(&new_vectorlEAnalysisTreecLcLVariablegR);
      instance.SetNewArray(&newArray_vectorlEAnalysisTreecLcLVariablegR);
      instance.SetDelete(&delete_vectorlEAnalysisTreecLcLVariablegR);
      instance.SetDeleteArray(&deleteArray_vectorlEAnalysisTreecLcLVariablegR);
      instance.SetDestructor(&destruct_vectorlEAnalysisTreecLcLVariablegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<AnalysisTree::Variable> >()));

      ::ROOT::AddClassAlternate("vector<AnalysisTree::Variable>","std::vector<AnalysisTree::Variable, std::allocator<AnalysisTree::Variable> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<AnalysisTree::Variable>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAnalysisTreecLcLVariablegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<AnalysisTree::Variable>*)nullptr)->GetClass();
      vectorlEAnalysisTreecLcLVariablegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAnalysisTreecLcLVariablegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAnalysisTreecLcLVariablegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::Variable> : new vector<AnalysisTree::Variable>;
   }
   static void *newArray_vectorlEAnalysisTreecLcLVariablegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::Variable>[nElements] : new vector<AnalysisTree::Variable>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAnalysisTreecLcLVariablegR(void *p) {
      delete ((vector<AnalysisTree::Variable>*)p);
   }
   static void deleteArray_vectorlEAnalysisTreecLcLVariablegR(void *p) {
      delete [] ((vector<AnalysisTree::Variable>*)p);
   }
   static void destruct_vectorlEAnalysisTreecLcLVariablegR(void *p) {
      typedef vector<AnalysisTree::Variable> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<AnalysisTree::Variable>

namespace ROOT {
   static TClass *vectorlEAnalysisTreecLcLSimpleCutgR_Dictionary();
   static void vectorlEAnalysisTreecLcLSimpleCutgR_TClassManip(TClass*);
   static void *new_vectorlEAnalysisTreecLcLSimpleCutgR(void *p = nullptr);
   static void *newArray_vectorlEAnalysisTreecLcLSimpleCutgR(Long_t size, void *p);
   static void delete_vectorlEAnalysisTreecLcLSimpleCutgR(void *p);
   static void deleteArray_vectorlEAnalysisTreecLcLSimpleCutgR(void *p);
   static void destruct_vectorlEAnalysisTreecLcLSimpleCutgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<AnalysisTree::SimpleCut>*)
   {
      vector<AnalysisTree::SimpleCut> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<AnalysisTree::SimpleCut>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<AnalysisTree::SimpleCut>", -2, "vector", 389,
                  typeid(vector<AnalysisTree::SimpleCut>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAnalysisTreecLcLSimpleCutgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<AnalysisTree::SimpleCut>) );
      instance.SetNew(&new_vectorlEAnalysisTreecLcLSimpleCutgR);
      instance.SetNewArray(&newArray_vectorlEAnalysisTreecLcLSimpleCutgR);
      instance.SetDelete(&delete_vectorlEAnalysisTreecLcLSimpleCutgR);
      instance.SetDeleteArray(&deleteArray_vectorlEAnalysisTreecLcLSimpleCutgR);
      instance.SetDestructor(&destruct_vectorlEAnalysisTreecLcLSimpleCutgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<AnalysisTree::SimpleCut> >()));

      ::ROOT::AddClassAlternate("vector<AnalysisTree::SimpleCut>","std::vector<AnalysisTree::SimpleCut, std::allocator<AnalysisTree::SimpleCut> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<AnalysisTree::SimpleCut>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAnalysisTreecLcLSimpleCutgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<AnalysisTree::SimpleCut>*)nullptr)->GetClass();
      vectorlEAnalysisTreecLcLSimpleCutgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAnalysisTreecLcLSimpleCutgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAnalysisTreecLcLSimpleCutgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::SimpleCut> : new vector<AnalysisTree::SimpleCut>;
   }
   static void *newArray_vectorlEAnalysisTreecLcLSimpleCutgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::SimpleCut>[nElements] : new vector<AnalysisTree::SimpleCut>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAnalysisTreecLcLSimpleCutgR(void *p) {
      delete ((vector<AnalysisTree::SimpleCut>*)p);
   }
   static void deleteArray_vectorlEAnalysisTreecLcLSimpleCutgR(void *p) {
      delete [] ((vector<AnalysisTree::SimpleCut>*)p);
   }
   static void destruct_vectorlEAnalysisTreecLcLSimpleCutgR(void *p) {
      typedef vector<AnalysisTree::SimpleCut> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<AnalysisTree::SimpleCut>

namespace ROOT {
   static TClass *setlEunsignedsPlonggR_Dictionary();
   static void setlEunsignedsPlonggR_TClassManip(TClass*);
   static void *new_setlEunsignedsPlonggR(void *p = nullptr);
   static void *newArray_setlEunsignedsPlonggR(Long_t size, void *p);
   static void delete_setlEunsignedsPlonggR(void *p);
   static void deleteArray_setlEunsignedsPlonggR(void *p);
   static void destruct_setlEunsignedsPlonggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const set<unsigned long>*)
   {
      set<unsigned long> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(set<unsigned long>));
      static ::ROOT::TGenericClassInfo 
         instance("set<unsigned long>", -2, "set", 94,
                  typeid(set<unsigned long>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &setlEunsignedsPlonggR_Dictionary, isa_proxy, 0,
                  sizeof(set<unsigned long>) );
      instance.SetNew(&new_setlEunsignedsPlonggR);
      instance.SetNewArray(&newArray_setlEunsignedsPlonggR);
      instance.SetDelete(&delete_setlEunsignedsPlonggR);
      instance.SetDeleteArray(&deleteArray_setlEunsignedsPlonggR);
      instance.SetDestructor(&destruct_setlEunsignedsPlonggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Insert< set<unsigned long> >()));

      ::ROOT::AddClassAlternate("set<unsigned long>","std::set<unsigned long, std::less<unsigned long>, std::allocator<unsigned long> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const set<unsigned long>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *setlEunsignedsPlonggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const set<unsigned long>*)nullptr)->GetClass();
      setlEunsignedsPlonggR_TClassManip(theClass);
   return theClass;
   }

   static void setlEunsignedsPlonggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_setlEunsignedsPlonggR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<unsigned long> : new set<unsigned long>;
   }
   static void *newArray_setlEunsignedsPlonggR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<unsigned long>[nElements] : new set<unsigned long>[nElements];
   }
   // Wrapper around operator delete
   static void delete_setlEunsignedsPlonggR(void *p) {
      delete ((set<unsigned long>*)p);
   }
   static void deleteArray_setlEunsignedsPlonggR(void *p) {
      delete [] ((set<unsigned long>*)p);
   }
   static void destruct_setlEunsignedsPlonggR(void *p) {
      typedef set<unsigned long> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class set<unsigned long>

namespace ROOT {
   static TClass *setlEstringgR_Dictionary();
   static void setlEstringgR_TClassManip(TClass*);
   static void *new_setlEstringgR(void *p = nullptr);
   static void *newArray_setlEstringgR(Long_t size, void *p);
   static void delete_setlEstringgR(void *p);
   static void deleteArray_setlEstringgR(void *p);
   static void destruct_setlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const set<string>*)
   {
      set<string> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(set<string>));
      static ::ROOT::TGenericClassInfo 
         instance("set<string>", -2, "set", 94,
                  typeid(set<string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &setlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(set<string>) );
      instance.SetNew(&new_setlEstringgR);
      instance.SetNewArray(&newArray_setlEstringgR);
      instance.SetDelete(&delete_setlEstringgR);
      instance.SetDeleteArray(&deleteArray_setlEstringgR);
      instance.SetDestructor(&destruct_setlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Insert< set<string> >()));

      ::ROOT::AddClassAlternate("set<string>","std::set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const set<string>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *setlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const set<string>*)nullptr)->GetClass();
      setlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void setlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_setlEstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<string> : new set<string>;
   }
   static void *newArray_setlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) set<string>[nElements] : new set<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_setlEstringgR(void *p) {
      delete ((set<string>*)p);
   }
   static void deleteArray_setlEstringgR(void *p) {
      delete [] ((set<string>*)p);
   }
   static void destruct_setlEstringgR(void *p) {
      typedef set<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class set<string>

namespace ROOT {
   static TClass *maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR_Dictionary();
   static void maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR_TClassManip(TClass*);
   static void *new_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR(void *p = nullptr);
   static void *newArray_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR(Long_t size, void *p);
   static void delete_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR(void *p);
   static void deleteArray_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR(void *p);
   static void destruct_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> >*)
   {
      map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> >", -2, "map", 100,
                  typeid(map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> >) );
      instance.SetNew(&new_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR);
      instance.SetNewArray(&newArray_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR);
      instance.SetDelete(&delete_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR);
      instance.SetDestructor(&destruct_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> > >()));

      ::ROOT::AddClassAlternate("map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> >","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::variant<AnalysisTree::Detector<AnalysisTree::Hit>*, AnalysisTree::Detector<AnalysisTree::Module>*, AnalysisTree::Detector<AnalysisTree::Track>*, AnalysisTree::EventHeader*, AnalysisTree::Detector<AnalysisTree::Particle>*>, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::variant<AnalysisTree::Detector<AnalysisTree::Hit>*, AnalysisTree::Detector<AnalysisTree::Module>*, AnalysisTree::Detector<AnalysisTree::Track>*, AnalysisTree::EventHeader*, AnalysisTree::Detector<AnalysisTree::Particle>*> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> >*)nullptr)->GetClass();
      maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> > : new map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> >;
   }
   static void *newArray_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> >[nElements] : new map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR(void *p) {
      delete ((map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> >*)p);
   }
   static void deleteArray_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR(void *p) {
      delete [] ((map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> >*)p);
   }
   static void destruct_maplEstringcOvariantlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegRmUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgRmUcOAnalysisTreecLcLEventHeadermUcOAnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegRmUgRsPgR(void *p) {
      typedef map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,variant<AnalysisTree::Detector<AnalysisTree::Hit>*,AnalysisTree::Detector<AnalysisTree::Module>*,AnalysisTree::Detector<AnalysisTree::Track>*,AnalysisTree::EventHeader*,AnalysisTree::Detector<AnalysisTree::Particle>*> >

namespace ROOT {
   static TClass *maplEstringcOAnalysisTreecLcLMatchingmUgR_Dictionary();
   static void maplEstringcOAnalysisTreecLcLMatchingmUgR_TClassManip(TClass*);
   static void *new_maplEstringcOAnalysisTreecLcLMatchingmUgR(void *p = nullptr);
   static void *newArray_maplEstringcOAnalysisTreecLcLMatchingmUgR(Long_t size, void *p);
   static void delete_maplEstringcOAnalysisTreecLcLMatchingmUgR(void *p);
   static void deleteArray_maplEstringcOAnalysisTreecLcLMatchingmUgR(void *p);
   static void destruct_maplEstringcOAnalysisTreecLcLMatchingmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,AnalysisTree::Matching*>*)
   {
      map<string,AnalysisTree::Matching*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,AnalysisTree::Matching*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,AnalysisTree::Matching*>", -2, "map", 100,
                  typeid(map<string,AnalysisTree::Matching*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOAnalysisTreecLcLMatchingmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,AnalysisTree::Matching*>) );
      instance.SetNew(&new_maplEstringcOAnalysisTreecLcLMatchingmUgR);
      instance.SetNewArray(&newArray_maplEstringcOAnalysisTreecLcLMatchingmUgR);
      instance.SetDelete(&delete_maplEstringcOAnalysisTreecLcLMatchingmUgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOAnalysisTreecLcLMatchingmUgR);
      instance.SetDestructor(&destruct_maplEstringcOAnalysisTreecLcLMatchingmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,AnalysisTree::Matching*> >()));

      ::ROOT::AddClassAlternate("map<string,AnalysisTree::Matching*>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, AnalysisTree::Matching*, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, AnalysisTree::Matching*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,AnalysisTree::Matching*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOAnalysisTreecLcLMatchingmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,AnalysisTree::Matching*>*)nullptr)->GetClass();
      maplEstringcOAnalysisTreecLcLMatchingmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOAnalysisTreecLcLMatchingmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOAnalysisTreecLcLMatchingmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,AnalysisTree::Matching*> : new map<string,AnalysisTree::Matching*>;
   }
   static void *newArray_maplEstringcOAnalysisTreecLcLMatchingmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,AnalysisTree::Matching*>[nElements] : new map<string,AnalysisTree::Matching*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOAnalysisTreecLcLMatchingmUgR(void *p) {
      delete ((map<string,AnalysisTree::Matching*>*)p);
   }
   static void deleteArray_maplEstringcOAnalysisTreecLcLMatchingmUgR(void *p) {
      delete [] ((map<string,AnalysisTree::Matching*>*)p);
   }
   static void destruct_maplEstringcOAnalysisTreecLcLMatchingmUgR(void *p) {
      typedef map<string,AnalysisTree::Matching*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,AnalysisTree::Matching*>

namespace {
  void TriggerDictionaryInitialization_libAnalysisTreeInfra_Impl() {
    static const char* headers[] = {
"Cuts.hpp",
"Field.hpp",
"Variable.hpp",
"Task.hpp",
"AnalysisTask.hpp",
"TaskManager.hpp",
"PlainTreeFiller.hpp",
"Chain.hpp",
"ChainDrawHelper.hpp",
"Branch.hpp",
"BranchChannel.hpp",
"AnalysisEntry.hpp",
"VariantMagic.hpp",
"ToyMC.hpp",
"Utils.hpp",
"BranchHashHelper.hpp",
nullptr
    };
    static const char* includePaths[] = {
"/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREE-prefix/src/ANALYSISTREE-build/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree",
"/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/core",
"/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra",
"SimpleCut.hpp",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREE-prefix/src/ANALYSISTREE-build/infra/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libAnalysisTreeInfra dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Field.hpp")))  __attribute__((annotate("$clingAutoload$Cuts.hpp")))  Field;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Variable.hpp")))  __attribute__((annotate("$clingAutoload$Cuts.hpp")))  Variable;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$SimpleCut.hpp")))  __attribute__((annotate("$clingAutoload$Cuts.hpp")))  SimpleCut;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Cuts.hpp")))  Cuts;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Chain.hpp")))  __attribute__((annotate("$clingAutoload$Task.hpp")))  Chain;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Task.hpp")))  Task;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$AnalysisEntry.hpp")))  __attribute__((annotate("$clingAutoload$AnalysisTask.hpp")))  AnalysisEntry;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$AnalysisTask.hpp")))  AnalysisTask;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$TaskManager.hpp")))  TaskManager;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libAnalysisTreeInfra dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Cuts.hpp"
#include "Field.hpp"
#include "Variable.hpp"
#include "Task.hpp"
#include "AnalysisTask.hpp"
#include "TaskManager.hpp"
#include "PlainTreeFiller.hpp"
#include "Chain.hpp"
#include "ChainDrawHelper.hpp"
#include "Branch.hpp"
#include "BranchChannel.hpp"
#include "AnalysisEntry.hpp"
#include "VariantMagic.hpp"
#include "ToyMC.hpp"
#include "Utils.hpp"
#include "BranchHashHelper.hpp"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"AnalysisTree::AnalysisEntry", payloadCode, "@",
"AnalysisTree::AnalysisTask", payloadCode, "@",
"AnalysisTree::Chain", payloadCode, "@",
"AnalysisTree::Cuts", payloadCode, "@",
"AnalysisTree::EqualsCut", payloadCode, "@",
"AnalysisTree::Field", payloadCode, "@",
"AnalysisTree::RangeCut", payloadCode, "@",
"AnalysisTree::SimpleCut", payloadCode, "@",
"AnalysisTree::Task", payloadCode, "@",
"AnalysisTree::TaskManager", payloadCode, "@",
"AnalysisTree::Variable", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libAnalysisTreeInfra",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libAnalysisTreeInfra_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libAnalysisTreeInfra_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libAnalysisTreeInfra() {
  TriggerDictionaryInitialization_libAnalysisTreeInfra_Impl();
}
