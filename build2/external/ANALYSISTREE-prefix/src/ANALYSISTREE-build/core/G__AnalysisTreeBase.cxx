// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__AnalysisTreeBase
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

#include "TBuffer.h"
#include "TVirtualObject.h"
#include <vector>
#include "TSchemaHelper.h"


// Header files passed as explicit arguments
#include "IndexedObject.hpp"
#include "Track.hpp"
#include "Hit.hpp"
#include "EventHeader.hpp"
#include "Module.hpp"
#include "Configuration.hpp"
#include "BranchConfig.hpp"
#include "Container.hpp"
#include "DataHeader.hpp"
#include "Matching.hpp"
#include "Particle.hpp"
#include "Constants.hpp"
#include "Detector.hpp"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace AnalysisTree {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *AnalysisTree_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("AnalysisTree", 0 /*version*/, "Constants.hpp", 25,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &AnalysisTree_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *AnalysisTree_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static TClass *AnalysisTreecLcLIndexAccessor_Dictionary();
   static void AnalysisTreecLcLIndexAccessor_TClassManip(TClass*);
   static void *new_AnalysisTreecLcLIndexAccessor(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLIndexAccessor(Long_t size, void *p);
   static void delete_AnalysisTreecLcLIndexAccessor(void *p);
   static void deleteArray_AnalysisTreecLcLIndexAccessor(void *p);
   static void destruct_AnalysisTreecLcLIndexAccessor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::IndexAccessor*)
   {
      ::AnalysisTree::IndexAccessor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::AnalysisTree::IndexAccessor));
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::IndexAccessor", "IndexedObject.hpp", 18,
                  typeid(::AnalysisTree::IndexAccessor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysisTreecLcLIndexAccessor_Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::IndexAccessor) );
      instance.SetNew(&new_AnalysisTreecLcLIndexAccessor);
      instance.SetNewArray(&newArray_AnalysisTreecLcLIndexAccessor);
      instance.SetDelete(&delete_AnalysisTreecLcLIndexAccessor);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLIndexAccessor);
      instance.SetDestructor(&destruct_AnalysisTreecLcLIndexAccessor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::IndexAccessor*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::IndexAccessor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::IndexAccessor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysisTreecLcLIndexAccessor_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::IndexAccessor*)nullptr)->GetClass();
      AnalysisTreecLcLIndexAccessor_TClassManip(theClass);
   return theClass;
   }

   static void AnalysisTreecLcLIndexAccessor_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLIndexedObject(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLIndexedObject(Long_t size, void *p);
   static void delete_AnalysisTreecLcLIndexedObject(void *p);
   static void deleteArray_AnalysisTreecLcLIndexedObject(void *p);
   static void destruct_AnalysisTreecLcLIndexedObject(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::IndexedObject*)
   {
      ::AnalysisTree::IndexedObject *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::IndexedObject >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::IndexedObject", ::AnalysisTree::IndexedObject::Class_Version(), "IndexedObject.hpp", 30,
                  typeid(::AnalysisTree::IndexedObject), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::IndexedObject::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::IndexedObject) );
      instance.SetNew(&new_AnalysisTreecLcLIndexedObject);
      instance.SetNewArray(&newArray_AnalysisTreecLcLIndexedObject);
      instance.SetDelete(&delete_AnalysisTreecLcLIndexedObject);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLIndexedObject);
      instance.SetDestructor(&destruct_AnalysisTreecLcLIndexedObject);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::IndexedObject*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::IndexedObject*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::IndexedObject*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLConfigElement(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLConfigElement(Long_t size, void *p);
   static void delete_AnalysisTreecLcLConfigElement(void *p);
   static void deleteArray_AnalysisTreecLcLConfigElement(void *p);
   static void destruct_AnalysisTreecLcLConfigElement(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::ConfigElement*)
   {
      ::AnalysisTree::ConfigElement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::ConfigElement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::ConfigElement", ::AnalysisTree::ConfigElement::Class_Version(), "BranchConfig.hpp", 23,
                  typeid(::AnalysisTree::ConfigElement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::ConfigElement::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::ConfigElement) );
      instance.SetNew(&new_AnalysisTreecLcLConfigElement);
      instance.SetNewArray(&newArray_AnalysisTreecLcLConfigElement);
      instance.SetDelete(&delete_AnalysisTreecLcLConfigElement);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLConfigElement);
      instance.SetDestructor(&destruct_AnalysisTreecLcLConfigElement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::ConfigElement*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::ConfigElement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::ConfigElement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *AnalysisTreecLcLVectorConfiglEintgR_Dictionary();
   static void AnalysisTreecLcLVectorConfiglEintgR_TClassManip(TClass*);
   static void *new_AnalysisTreecLcLVectorConfiglEintgR(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLVectorConfiglEintgR(Long_t size, void *p);
   static void delete_AnalysisTreecLcLVectorConfiglEintgR(void *p);
   static void deleteArray_AnalysisTreecLcLVectorConfiglEintgR(void *p);
   static void destruct_AnalysisTreecLcLVectorConfiglEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::VectorConfig<int>*)
   {
      ::AnalysisTree::VectorConfig<int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::VectorConfig<int> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::VectorConfig<int>", ::AnalysisTree::VectorConfig<int>::Class_Version(), "BranchConfig.hpp", 39,
                  typeid(::AnalysisTree::VectorConfig<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysisTreecLcLVectorConfiglEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::VectorConfig<int>) );
      instance.SetNew(&new_AnalysisTreecLcLVectorConfiglEintgR);
      instance.SetNewArray(&newArray_AnalysisTreecLcLVectorConfiglEintgR);
      instance.SetDelete(&delete_AnalysisTreecLcLVectorConfiglEintgR);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLVectorConfiglEintgR);
      instance.SetDestructor(&destruct_AnalysisTreecLcLVectorConfiglEintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::VectorConfig<int>*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::VectorConfig<int>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysisTreecLcLVectorConfiglEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<int>*)nullptr)->GetClass();
      AnalysisTreecLcLVectorConfiglEintgR_TClassManip(theClass);
   return theClass;
   }

   static void AnalysisTreecLcLVectorConfiglEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *AnalysisTreecLcLVectorConfiglEfloatgR_Dictionary();
   static void AnalysisTreecLcLVectorConfiglEfloatgR_TClassManip(TClass*);
   static void *new_AnalysisTreecLcLVectorConfiglEfloatgR(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLVectorConfiglEfloatgR(Long_t size, void *p);
   static void delete_AnalysisTreecLcLVectorConfiglEfloatgR(void *p);
   static void deleteArray_AnalysisTreecLcLVectorConfiglEfloatgR(void *p);
   static void destruct_AnalysisTreecLcLVectorConfiglEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::VectorConfig<float>*)
   {
      ::AnalysisTree::VectorConfig<float> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::VectorConfig<float> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::VectorConfig<float>", ::AnalysisTree::VectorConfig<float>::Class_Version(), "BranchConfig.hpp", 39,
                  typeid(::AnalysisTree::VectorConfig<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysisTreecLcLVectorConfiglEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::VectorConfig<float>) );
      instance.SetNew(&new_AnalysisTreecLcLVectorConfiglEfloatgR);
      instance.SetNewArray(&newArray_AnalysisTreecLcLVectorConfiglEfloatgR);
      instance.SetDelete(&delete_AnalysisTreecLcLVectorConfiglEfloatgR);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLVectorConfiglEfloatgR);
      instance.SetDestructor(&destruct_AnalysisTreecLcLVectorConfiglEfloatgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::VectorConfig<float>*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::VectorConfig<float>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<float>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysisTreecLcLVectorConfiglEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<float>*)nullptr)->GetClass();
      AnalysisTreecLcLVectorConfiglEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void AnalysisTreecLcLVectorConfiglEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *AnalysisTreecLcLVectorConfiglEboolgR_Dictionary();
   static void AnalysisTreecLcLVectorConfiglEboolgR_TClassManip(TClass*);
   static void *new_AnalysisTreecLcLVectorConfiglEboolgR(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLVectorConfiglEboolgR(Long_t size, void *p);
   static void delete_AnalysisTreecLcLVectorConfiglEboolgR(void *p);
   static void deleteArray_AnalysisTreecLcLVectorConfiglEboolgR(void *p);
   static void destruct_AnalysisTreecLcLVectorConfiglEboolgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::VectorConfig<bool>*)
   {
      ::AnalysisTree::VectorConfig<bool> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::VectorConfig<bool> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::VectorConfig<bool>", ::AnalysisTree::VectorConfig<bool>::Class_Version(), "BranchConfig.hpp", 39,
                  typeid(::AnalysisTree::VectorConfig<bool>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysisTreecLcLVectorConfiglEboolgR_Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::VectorConfig<bool>) );
      instance.SetNew(&new_AnalysisTreecLcLVectorConfiglEboolgR);
      instance.SetNewArray(&newArray_AnalysisTreecLcLVectorConfiglEboolgR);
      instance.SetDelete(&delete_AnalysisTreecLcLVectorConfiglEboolgR);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLVectorConfiglEboolgR);
      instance.SetDestructor(&destruct_AnalysisTreecLcLVectorConfiglEboolgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::VectorConfig<bool>*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::VectorConfig<bool>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<bool>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysisTreecLcLVectorConfiglEboolgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<bool>*)nullptr)->GetClass();
      AnalysisTreecLcLVectorConfiglEboolgR_TClassManip(theClass);
   return theClass;
   }

   static void AnalysisTreecLcLVectorConfiglEboolgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLBranchConfig(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLBranchConfig(Long_t size, void *p);
   static void delete_AnalysisTreecLcLBranchConfig(void *p);
   static void deleteArray_AnalysisTreecLcLBranchConfig(void *p);
   static void destruct_AnalysisTreecLcLBranchConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::BranchConfig*)
   {
      ::AnalysisTree::BranchConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::BranchConfig >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::BranchConfig", ::AnalysisTree::BranchConfig::Class_Version(), "BranchConfig.hpp", 102,
                  typeid(::AnalysisTree::BranchConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::BranchConfig::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::BranchConfig) );
      instance.SetNew(&new_AnalysisTreecLcLBranchConfig);
      instance.SetNewArray(&newArray_AnalysisTreecLcLBranchConfig);
      instance.SetDelete(&delete_AnalysisTreecLcLBranchConfig);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLBranchConfig);
      instance.SetDestructor(&destruct_AnalysisTreecLcLBranchConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::BranchConfig*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::BranchConfig*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::BranchConfig*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLContainer(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLContainer(Long_t size, void *p);
   static void delete_AnalysisTreecLcLContainer(void *p);
   static void deleteArray_AnalysisTreecLcLContainer(void *p);
   static void destruct_AnalysisTreecLcLContainer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Container*)
   {
      ::AnalysisTree::Container *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Container >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Container", ::AnalysisTree::Container::Class_Version(), "Container.hpp", 18,
                  typeid(::AnalysisTree::Container), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::Container::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Container) );
      instance.SetNew(&new_AnalysisTreecLcLContainer);
      instance.SetNewArray(&newArray_AnalysisTreecLcLContainer);
      instance.SetDelete(&delete_AnalysisTreecLcLContainer);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLContainer);
      instance.SetDestructor(&destruct_AnalysisTreecLcLContainer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Container*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Container*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Container*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLTrack(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLTrack(Long_t size, void *p);
   static void delete_AnalysisTreecLcLTrack(void *p);
   static void deleteArray_AnalysisTreecLcLTrack(void *p);
   static void destruct_AnalysisTreecLcLTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Track*)
   {
      ::AnalysisTree::Track *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Track >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Track", ::AnalysisTree::Track::Class_Version(), "Track.hpp", 22,
                  typeid(::AnalysisTree::Track), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::Track::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Track) );
      instance.SetNew(&new_AnalysisTreecLcLTrack);
      instance.SetNewArray(&newArray_AnalysisTreecLcLTrack);
      instance.SetDelete(&delete_AnalysisTreecLcLTrack);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLTrack);
      instance.SetDestructor(&destruct_AnalysisTreecLcLTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Track*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Track*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Track*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLHit(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLHit(Long_t size, void *p);
   static void delete_AnalysisTreecLcLHit(void *p);
   static void deleteArray_AnalysisTreecLcLHit(void *p);
   static void destruct_AnalysisTreecLcLHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Hit*)
   {
      ::AnalysisTree::Hit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Hit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Hit", ::AnalysisTree::Hit::Class_Version(), "Hit.hpp", 16,
                  typeid(::AnalysisTree::Hit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::Hit::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Hit) );
      instance.SetNew(&new_AnalysisTreecLcLHit);
      instance.SetNewArray(&newArray_AnalysisTreecLcLHit);
      instance.SetDelete(&delete_AnalysisTreecLcLHit);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLHit);
      instance.SetDestructor(&destruct_AnalysisTreecLcLHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Hit*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Hit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Hit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLEventHeader(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLEventHeader(Long_t size, void *p);
   static void delete_AnalysisTreecLcLEventHeader(void *p);
   static void deleteArray_AnalysisTreecLcLEventHeader(void *p);
   static void destruct_AnalysisTreecLcLEventHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::EventHeader*)
   {
      ::AnalysisTree::EventHeader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::EventHeader >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::EventHeader", ::AnalysisTree::EventHeader::Class_Version(), "EventHeader.hpp", 18,
                  typeid(::AnalysisTree::EventHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::EventHeader::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::EventHeader) );
      instance.SetNew(&new_AnalysisTreecLcLEventHeader);
      instance.SetNewArray(&newArray_AnalysisTreecLcLEventHeader);
      instance.SetDelete(&delete_AnalysisTreecLcLEventHeader);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLEventHeader);
      instance.SetDestructor(&destruct_AnalysisTreecLcLEventHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::EventHeader*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::EventHeader*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::EventHeader*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLModule(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLModule(Long_t size, void *p);
   static void delete_AnalysisTreecLcLModule(void *p);
   static void deleteArray_AnalysisTreecLcLModule(void *p);
   static void destruct_AnalysisTreecLcLModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Module*)
   {
      ::AnalysisTree::Module *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Module >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Module", ::AnalysisTree::Module::Class_Version(), "Module.hpp", 15,
                  typeid(::AnalysisTree::Module), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::Module::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Module) );
      instance.SetNew(&new_AnalysisTreecLcLModule);
      instance.SetNewArray(&newArray_AnalysisTreecLcLModule);
      instance.SetDelete(&delete_AnalysisTreecLcLModule);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLModule);
      instance.SetDestructor(&destruct_AnalysisTreecLcLModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Module*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Module*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Module*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *AnalysisTreecLcLModulePosition_Dictionary();
   static void AnalysisTreecLcLModulePosition_TClassManip(TClass*);
   static void *new_AnalysisTreecLcLModulePosition(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLModulePosition(Long_t size, void *p);
   static void delete_AnalysisTreecLcLModulePosition(void *p);
   static void deleteArray_AnalysisTreecLcLModulePosition(void *p);
   static void destruct_AnalysisTreecLcLModulePosition(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::ModulePosition*)
   {
      ::AnalysisTree::ModulePosition *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::AnalysisTree::ModulePosition));
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::ModulePosition", "Module.hpp", 73,
                  typeid(::AnalysisTree::ModulePosition), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysisTreecLcLModulePosition_Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::ModulePosition) );
      instance.SetNew(&new_AnalysisTreecLcLModulePosition);
      instance.SetNewArray(&newArray_AnalysisTreecLcLModulePosition);
      instance.SetDelete(&delete_AnalysisTreecLcLModulePosition);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLModulePosition);
      instance.SetDestructor(&destruct_AnalysisTreecLcLModulePosition);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::ModulePosition*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::ModulePosition*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::ModulePosition*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysisTreecLcLModulePosition_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::ModulePosition*)nullptr)->GetClass();
      AnalysisTreecLcLModulePosition_TClassManip(theClass);
   return theClass;
   }

   static void AnalysisTreecLcLModulePosition_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLMatchingConfig(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLMatchingConfig(Long_t size, void *p);
   static void delete_AnalysisTreecLcLMatchingConfig(void *p);
   static void deleteArray_AnalysisTreecLcLMatchingConfig(void *p);
   static void destruct_AnalysisTreecLcLMatchingConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::MatchingConfig*)
   {
      ::AnalysisTree::MatchingConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::MatchingConfig >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::MatchingConfig", ::AnalysisTree::MatchingConfig::Class_Version(), "Configuration.hpp", 23,
                  typeid(::AnalysisTree::MatchingConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::MatchingConfig::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::MatchingConfig) );
      instance.SetNew(&new_AnalysisTreecLcLMatchingConfig);
      instance.SetNewArray(&newArray_AnalysisTreecLcLMatchingConfig);
      instance.SetDelete(&delete_AnalysisTreecLcLMatchingConfig);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLMatchingConfig);
      instance.SetDestructor(&destruct_AnalysisTreecLcLMatchingConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::MatchingConfig*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::MatchingConfig*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::MatchingConfig*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLConfiguration_v3(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLConfiguration_v3(Long_t size, void *p);
   static void delete_AnalysisTreecLcLConfiguration_v3(void *p);
   static void deleteArray_AnalysisTreecLcLConfiguration_v3(void *p);
   static void destruct_AnalysisTreecLcLConfiguration_v3(void *p);

   // Schema evolution read functions

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Configuration_v3*)
   {
      ::AnalysisTree::Configuration_v3 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Configuration_v3 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Configuration_v3", ::AnalysisTree::Configuration_v3::Class_Version(), "Configuration.hpp", 43,
                  typeid(::AnalysisTree::Configuration_v3), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::Configuration_v3::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Configuration_v3) );
      instance.SetNew(&new_AnalysisTreecLcLConfiguration_v3);
      instance.SetNewArray(&newArray_AnalysisTreecLcLConfiguration_v3);
      instance.SetDelete(&delete_AnalysisTreecLcLConfiguration_v3);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLConfiguration_v3);
      instance.SetDestructor(&destruct_AnalysisTreecLcLConfiguration_v3);

      ::ROOT::Internal::TSchemaHelper* rule;

      // the io read rules
      std::vector<::ROOT::Internal::TSchemaHelper> readrules(1);
      rule = &readrules[0];
      rule->fSourceClass = "AnalysisTree::Configuration";
      rule->fTarget      = "";
      rule->fVersion     = "[1-]";
      instance.SetReadRules( readrules );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Configuration_v3*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Configuration_v3*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Configuration_v3*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLConfiguration(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLConfiguration(Long_t size, void *p);
   static void delete_AnalysisTreecLcLConfiguration(void *p);
   static void deleteArray_AnalysisTreecLcLConfiguration(void *p);
   static void destruct_AnalysisTreecLcLConfiguration(void *p);
   static void streamer_AnalysisTreecLcLConfiguration(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Configuration*)
   {
      ::AnalysisTree::Configuration *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Configuration >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Configuration", ::AnalysisTree::Configuration::Class_Version(), "Configuration.hpp", 58,
                  typeid(::AnalysisTree::Configuration), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::Configuration::Dictionary, isa_proxy, 17,
                  sizeof(::AnalysisTree::Configuration) );
      instance.SetNew(&new_AnalysisTreecLcLConfiguration);
      instance.SetNewArray(&newArray_AnalysisTreecLcLConfiguration);
      instance.SetDelete(&delete_AnalysisTreecLcLConfiguration);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLConfiguration);
      instance.SetDestructor(&destruct_AnalysisTreecLcLConfiguration);
      instance.SetStreamerFunc(&streamer_AnalysisTreecLcLConfiguration);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Configuration*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Configuration*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Configuration*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLParticle(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLParticle(Long_t size, void *p);
   static void delete_AnalysisTreecLcLParticle(void *p);
   static void deleteArray_AnalysisTreecLcLParticle(void *p);
   static void destruct_AnalysisTreecLcLParticle(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Particle*)
   {
      ::AnalysisTree::Particle *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Particle >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Particle", ::AnalysisTree::Particle::Class_Version(), "Particle.hpp", 11,
                  typeid(::AnalysisTree::Particle), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::Particle::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Particle) );
      instance.SetNew(&new_AnalysisTreecLcLParticle);
      instance.SetNewArray(&newArray_AnalysisTreecLcLParticle);
      instance.SetDelete(&delete_AnalysisTreecLcLParticle);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLParticle);
      instance.SetDestructor(&destruct_AnalysisTreecLcLParticle);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Particle*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Particle*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Particle*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR_Dictionary();
   static void AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR_TClassManip(TClass*);
   static void *new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR(Long_t size, void *p);
   static void delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR(void *p);
   static void deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR(void *p);
   static void destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Detector<AnalysisTree::Track>*)
   {
      ::AnalysisTree::Detector<AnalysisTree::Track> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Detector<AnalysisTree::Track> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Detector<AnalysisTree::Track>", ::AnalysisTree::Detector<AnalysisTree::Track>::Class_Version(), "Detector.hpp", 21,
                  typeid(::AnalysisTree::Detector<AnalysisTree::Track>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR_Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Detector<AnalysisTree::Track>) );
      instance.SetNew(&new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR);
      instance.SetNewArray(&newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR);
      instance.SetDelete(&delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR);
      instance.SetDestructor(&destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Detector<AnalysisTree::Track>*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Detector<AnalysisTree::Track>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Track>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Track>*)nullptr)->GetClass();
      AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR_TClassManip(theClass);
   return theClass;
   }

   static void AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR_Dictionary();
   static void AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR_TClassManip(TClass*);
   static void *new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR(Long_t size, void *p);
   static void delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR(void *p);
   static void deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR(void *p);
   static void destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Detector<AnalysisTree::Module>*)
   {
      ::AnalysisTree::Detector<AnalysisTree::Module> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Detector<AnalysisTree::Module> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Detector<AnalysisTree::Module>", ::AnalysisTree::Detector<AnalysisTree::Module>::Class_Version(), "Detector.hpp", 21,
                  typeid(::AnalysisTree::Detector<AnalysisTree::Module>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR_Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Detector<AnalysisTree::Module>) );
      instance.SetNew(&new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR);
      instance.SetNewArray(&newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR);
      instance.SetDelete(&delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR);
      instance.SetDestructor(&destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Detector<AnalysisTree::Module>*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Detector<AnalysisTree::Module>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Module>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Module>*)nullptr)->GetClass();
      AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR_TClassManip(theClass);
   return theClass;
   }

   static void AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR_Dictionary();
   static void AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR_TClassManip(TClass*);
   static void *new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR(Long_t size, void *p);
   static void delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR(void *p);
   static void deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR(void *p);
   static void destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Detector<AnalysisTree::ModulePosition>*)
   {
      ::AnalysisTree::Detector<AnalysisTree::ModulePosition> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Detector<AnalysisTree::ModulePosition> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Detector<AnalysisTree::ModulePosition>", ::AnalysisTree::Detector<AnalysisTree::ModulePosition>::Class_Version(), "Detector.hpp", 21,
                  typeid(::AnalysisTree::Detector<AnalysisTree::ModulePosition>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR_Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Detector<AnalysisTree::ModulePosition>) );
      instance.SetNew(&new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR);
      instance.SetNewArray(&newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR);
      instance.SetDelete(&delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR);
      instance.SetDestructor(&destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Detector<AnalysisTree::ModulePosition>*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Detector<AnalysisTree::ModulePosition>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::ModulePosition>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::ModulePosition>*)nullptr)->GetClass();
      AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR_TClassManip(theClass);
   return theClass;
   }

   static void AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR_Dictionary();
   static void AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR_TClassManip(TClass*);
   static void *new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR(Long_t size, void *p);
   static void delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR(void *p);
   static void deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR(void *p);
   static void destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Detector<AnalysisTree::Hit>*)
   {
      ::AnalysisTree::Detector<AnalysisTree::Hit> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Detector<AnalysisTree::Hit> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Detector<AnalysisTree::Hit>", ::AnalysisTree::Detector<AnalysisTree::Hit>::Class_Version(), "Detector.hpp", 21,
                  typeid(::AnalysisTree::Detector<AnalysisTree::Hit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR_Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Detector<AnalysisTree::Hit>) );
      instance.SetNew(&new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR);
      instance.SetNewArray(&newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR);
      instance.SetDelete(&delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR);
      instance.SetDestructor(&destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Detector<AnalysisTree::Hit>*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Detector<AnalysisTree::Hit>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Hit>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Hit>*)nullptr)->GetClass();
      AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR_TClassManip(theClass);
   return theClass;
   }

   static void AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR_Dictionary();
   static void AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR_TClassManip(TClass*);
   static void *new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR(Long_t size, void *p);
   static void delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR(void *p);
   static void deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR(void *p);
   static void destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Detector<AnalysisTree::Particle>*)
   {
      ::AnalysisTree::Detector<AnalysisTree::Particle> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Detector<AnalysisTree::Particle> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Detector<AnalysisTree::Particle>", ::AnalysisTree::Detector<AnalysisTree::Particle>::Class_Version(), "Detector.hpp", 21,
                  typeid(::AnalysisTree::Detector<AnalysisTree::Particle>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR_Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Detector<AnalysisTree::Particle>) );
      instance.SetNew(&new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR);
      instance.SetNewArray(&newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR);
      instance.SetDelete(&delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR);
      instance.SetDestructor(&destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Detector<AnalysisTree::Particle>*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Detector<AnalysisTree::Particle>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Particle>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Particle>*)nullptr)->GetClass();
      AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR_TClassManip(theClass);
   return theClass;
   }

   static void AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR_Dictionary();
   static void AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR_TClassManip(TClass*);
   static void *new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR(Long_t size, void *p);
   static void delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR(void *p);
   static void deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR(void *p);
   static void destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Detector<AnalysisTree::Container>*)
   {
      ::AnalysisTree::Detector<AnalysisTree::Container> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Detector<AnalysisTree::Container> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Detector<AnalysisTree::Container>", ::AnalysisTree::Detector<AnalysisTree::Container>::Class_Version(), "Detector.hpp", 21,
                  typeid(::AnalysisTree::Detector<AnalysisTree::Container>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR_Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Detector<AnalysisTree::Container>) );
      instance.SetNew(&new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR);
      instance.SetNewArray(&newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR);
      instance.SetDelete(&delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR);
      instance.SetDestructor(&destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Detector<AnalysisTree::Container>*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Detector<AnalysisTree::Container>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Container>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Container>*)nullptr)->GetClass();
      AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR_TClassManip(theClass);
   return theClass;
   }

   static void AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLDataHeader(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLDataHeader(Long_t size, void *p);
   static void delete_AnalysisTreecLcLDataHeader(void *p);
   static void deleteArray_AnalysisTreecLcLDataHeader(void *p);
   static void destruct_AnalysisTreecLcLDataHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::DataHeader*)
   {
      ::AnalysisTree::DataHeader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::DataHeader >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::DataHeader", ::AnalysisTree::DataHeader::Class_Version(), "DataHeader.hpp", 19,
                  typeid(::AnalysisTree::DataHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::DataHeader::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::DataHeader) );
      instance.SetNew(&new_AnalysisTreecLcLDataHeader);
      instance.SetNewArray(&newArray_AnalysisTreecLcLDataHeader);
      instance.SetDelete(&delete_AnalysisTreecLcLDataHeader);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLDataHeader);
      instance.SetDestructor(&destruct_AnalysisTreecLcLDataHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::DataHeader*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::DataHeader*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::DataHeader*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AnalysisTreecLcLMatching(void *p = nullptr);
   static void *newArray_AnalysisTreecLcLMatching(Long_t size, void *p);
   static void delete_AnalysisTreecLcLMatching(void *p);
   static void deleteArray_AnalysisTreecLcLMatching(void *p);
   static void destruct_AnalysisTreecLcLMatching(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalysisTree::Matching*)
   {
      ::AnalysisTree::Matching *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AnalysisTree::Matching >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AnalysisTree::Matching", ::AnalysisTree::Matching::Class_Version(), "Matching.hpp", 17,
                  typeid(::AnalysisTree::Matching), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AnalysisTree::Matching::Dictionary, isa_proxy, 4,
                  sizeof(::AnalysisTree::Matching) );
      instance.SetNew(&new_AnalysisTreecLcLMatching);
      instance.SetNewArray(&newArray_AnalysisTreecLcLMatching);
      instance.SetDelete(&delete_AnalysisTreecLcLMatching);
      instance.SetDeleteArray(&deleteArray_AnalysisTreecLcLMatching);
      instance.SetDestructor(&destruct_AnalysisTreecLcLMatching);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalysisTree::Matching*)
   {
      return GenerateInitInstanceLocal((::AnalysisTree::Matching*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AnalysisTree::Matching*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr IndexedObject::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *IndexedObject::Class_Name()
{
   return "AnalysisTree::IndexedObject";
}

//______________________________________________________________________________
const char *IndexedObject::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::IndexedObject*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int IndexedObject::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::IndexedObject*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *IndexedObject::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::IndexedObject*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *IndexedObject::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::IndexedObject*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr ConfigElement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ConfigElement::Class_Name()
{
   return "AnalysisTree::ConfigElement";
}

//______________________________________________________________________________
const char *ConfigElement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::ConfigElement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ConfigElement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::ConfigElement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ConfigElement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::ConfigElement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ConfigElement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::ConfigElement*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
template <> atomic_TClass_ptr VectorConfig<int>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *VectorConfig<int>::Class_Name()
{
   return "AnalysisTree::VectorConfig<int>";
}

//______________________________________________________________________________
template <> const char *VectorConfig<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<int>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int VectorConfig<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<int>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *VectorConfig<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<int>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *VectorConfig<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<int>*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
template <> atomic_TClass_ptr VectorConfig<float>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *VectorConfig<float>::Class_Name()
{
   return "AnalysisTree::VectorConfig<float>";
}

//______________________________________________________________________________
template <> const char *VectorConfig<float>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<float>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int VectorConfig<float>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<float>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *VectorConfig<float>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<float>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *VectorConfig<float>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<float>*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
template <> atomic_TClass_ptr VectorConfig<bool>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *VectorConfig<bool>::Class_Name()
{
   return "AnalysisTree::VectorConfig<bool>";
}

//______________________________________________________________________________
template <> const char *VectorConfig<bool>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<bool>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int VectorConfig<bool>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<bool>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *VectorConfig<bool>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<bool>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *VectorConfig<bool>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::VectorConfig<bool>*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr BranchConfig::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *BranchConfig::Class_Name()
{
   return "AnalysisTree::BranchConfig";
}

//______________________________________________________________________________
const char *BranchConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::BranchConfig*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int BranchConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::BranchConfig*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *BranchConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::BranchConfig*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *BranchConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::BranchConfig*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr Container::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Container::Class_Name()
{
   return "AnalysisTree::Container";
}

//______________________________________________________________________________
const char *Container::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Container*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Container::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Container*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Container::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Container*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Container::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Container*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr Track::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Track::Class_Name()
{
   return "AnalysisTree::Track";
}

//______________________________________________________________________________
const char *Track::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Track*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Track::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Track*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Track::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Track*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Track::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Track*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr Hit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Hit::Class_Name()
{
   return "AnalysisTree::Hit";
}

//______________________________________________________________________________
const char *Hit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Hit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Hit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Hit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Hit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Hit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Hit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Hit*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr EventHeader::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *EventHeader::Class_Name()
{
   return "AnalysisTree::EventHeader";
}

//______________________________________________________________________________
const char *EventHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::EventHeader*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int EventHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::EventHeader*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *EventHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::EventHeader*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EventHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::EventHeader*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr Module::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Module::Class_Name()
{
   return "AnalysisTree::Module";
}

//______________________________________________________________________________
const char *Module::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Module*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Module::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Module*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Module::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Module*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Module::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Module*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr MatchingConfig::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *MatchingConfig::Class_Name()
{
   return "AnalysisTree::MatchingConfig";
}

//______________________________________________________________________________
const char *MatchingConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::MatchingConfig*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int MatchingConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::MatchingConfig*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MatchingConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::MatchingConfig*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MatchingConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::MatchingConfig*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr Configuration_v3::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Configuration_v3::Class_Name()
{
   return "AnalysisTree::Configuration_v3";
}

//______________________________________________________________________________
const char *Configuration_v3::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Configuration_v3*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Configuration_v3::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Configuration_v3*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Configuration_v3::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Configuration_v3*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Configuration_v3::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Configuration_v3*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr Configuration::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Configuration::Class_Name()
{
   return "AnalysisTree::Configuration";
}

//______________________________________________________________________________
const char *Configuration::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Configuration*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Configuration::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Configuration*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Configuration::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Configuration*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Configuration::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Configuration*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr Particle::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Particle::Class_Name()
{
   return "AnalysisTree::Particle";
}

//______________________________________________________________________________
const char *Particle::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Particle*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Particle::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Particle*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Particle::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Particle*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Particle::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Particle*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
template <> atomic_TClass_ptr Detector<AnalysisTree::Track>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *Detector<AnalysisTree::Track>::Class_Name()
{
   return "AnalysisTree::Detector<AnalysisTree::Track>";
}

//______________________________________________________________________________
template <> const char *Detector<AnalysisTree::Track>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Track>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int Detector<AnalysisTree::Track>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Track>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *Detector<AnalysisTree::Track>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Track>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *Detector<AnalysisTree::Track>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Track>*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
template <> atomic_TClass_ptr Detector<AnalysisTree::Module>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *Detector<AnalysisTree::Module>::Class_Name()
{
   return "AnalysisTree::Detector<AnalysisTree::Module>";
}

//______________________________________________________________________________
template <> const char *Detector<AnalysisTree::Module>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Module>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int Detector<AnalysisTree::Module>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Module>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *Detector<AnalysisTree::Module>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Module>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *Detector<AnalysisTree::Module>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Module>*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
template <> atomic_TClass_ptr Detector<AnalysisTree::ModulePosition>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *Detector<AnalysisTree::ModulePosition>::Class_Name()
{
   return "AnalysisTree::Detector<AnalysisTree::ModulePosition>";
}

//______________________________________________________________________________
template <> const char *Detector<AnalysisTree::ModulePosition>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::ModulePosition>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int Detector<AnalysisTree::ModulePosition>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::ModulePosition>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *Detector<AnalysisTree::ModulePosition>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::ModulePosition>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *Detector<AnalysisTree::ModulePosition>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::ModulePosition>*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
template <> atomic_TClass_ptr Detector<AnalysisTree::Hit>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *Detector<AnalysisTree::Hit>::Class_Name()
{
   return "AnalysisTree::Detector<AnalysisTree::Hit>";
}

//______________________________________________________________________________
template <> const char *Detector<AnalysisTree::Hit>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Hit>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int Detector<AnalysisTree::Hit>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Hit>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *Detector<AnalysisTree::Hit>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Hit>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *Detector<AnalysisTree::Hit>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Hit>*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
template <> atomic_TClass_ptr Detector<AnalysisTree::Particle>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *Detector<AnalysisTree::Particle>::Class_Name()
{
   return "AnalysisTree::Detector<AnalysisTree::Particle>";
}

//______________________________________________________________________________
template <> const char *Detector<AnalysisTree::Particle>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Particle>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int Detector<AnalysisTree::Particle>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Particle>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *Detector<AnalysisTree::Particle>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Particle>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *Detector<AnalysisTree::Particle>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Particle>*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
template <> atomic_TClass_ptr Detector<AnalysisTree::Container>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *Detector<AnalysisTree::Container>::Class_Name()
{
   return "AnalysisTree::Detector<AnalysisTree::Container>";
}

//______________________________________________________________________________
template <> const char *Detector<AnalysisTree::Container>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Container>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int Detector<AnalysisTree::Container>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Container>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *Detector<AnalysisTree::Container>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Container>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *Detector<AnalysisTree::Container>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Detector<AnalysisTree::Container>*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr DataHeader::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DataHeader::Class_Name()
{
   return "AnalysisTree::DataHeader";
}

//______________________________________________________________________________
const char *DataHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::DataHeader*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DataHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::DataHeader*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DataHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::DataHeader*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DataHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::DataHeader*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace AnalysisTree {
//______________________________________________________________________________
atomic_TClass_ptr Matching::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Matching::Class_Name()
{
   return "AnalysisTree::Matching";
}

//______________________________________________________________________________
const char *Matching::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Matching*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Matching::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Matching*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Matching::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Matching*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Matching::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AnalysisTree::Matching*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLIndexAccessor(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::IndexAccessor : new ::AnalysisTree::IndexAccessor;
   }
   static void *newArray_AnalysisTreecLcLIndexAccessor(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::IndexAccessor[nElements] : new ::AnalysisTree::IndexAccessor[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLIndexAccessor(void *p) {
      delete ((::AnalysisTree::IndexAccessor*)p);
   }
   static void deleteArray_AnalysisTreecLcLIndexAccessor(void *p) {
      delete [] ((::AnalysisTree::IndexAccessor*)p);
   }
   static void destruct_AnalysisTreecLcLIndexAccessor(void *p) {
      typedef ::AnalysisTree::IndexAccessor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::IndexAccessor

namespace AnalysisTree {
//______________________________________________________________________________
void IndexedObject::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::IndexedObject.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::IndexedObject::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::IndexedObject::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLIndexedObject(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::IndexedObject : new ::AnalysisTree::IndexedObject;
   }
   static void *newArray_AnalysisTreecLcLIndexedObject(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::IndexedObject[nElements] : new ::AnalysisTree::IndexedObject[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLIndexedObject(void *p) {
      delete ((::AnalysisTree::IndexedObject*)p);
   }
   static void deleteArray_AnalysisTreecLcLIndexedObject(void *p) {
      delete [] ((::AnalysisTree::IndexedObject*)p);
   }
   static void destruct_AnalysisTreecLcLIndexedObject(void *p) {
      typedef ::AnalysisTree::IndexedObject current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::IndexedObject

namespace AnalysisTree {
//______________________________________________________________________________
void ConfigElement::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::ConfigElement.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::ConfigElement::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::ConfigElement::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLConfigElement(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::ConfigElement : new ::AnalysisTree::ConfigElement;
   }
   static void *newArray_AnalysisTreecLcLConfigElement(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::ConfigElement[nElements] : new ::AnalysisTree::ConfigElement[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLConfigElement(void *p) {
      delete ((::AnalysisTree::ConfigElement*)p);
   }
   static void deleteArray_AnalysisTreecLcLConfigElement(void *p) {
      delete [] ((::AnalysisTree::ConfigElement*)p);
   }
   static void destruct_AnalysisTreecLcLConfigElement(void *p) {
      typedef ::AnalysisTree::ConfigElement current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::ConfigElement

namespace AnalysisTree {
//______________________________________________________________________________
template <> void VectorConfig<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::VectorConfig<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::VectorConfig<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::VectorConfig<int>::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLVectorConfiglEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::VectorConfig<int> : new ::AnalysisTree::VectorConfig<int>;
   }
   static void *newArray_AnalysisTreecLcLVectorConfiglEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::VectorConfig<int>[nElements] : new ::AnalysisTree::VectorConfig<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLVectorConfiglEintgR(void *p) {
      delete ((::AnalysisTree::VectorConfig<int>*)p);
   }
   static void deleteArray_AnalysisTreecLcLVectorConfiglEintgR(void *p) {
      delete [] ((::AnalysisTree::VectorConfig<int>*)p);
   }
   static void destruct_AnalysisTreecLcLVectorConfiglEintgR(void *p) {
      typedef ::AnalysisTree::VectorConfig<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::VectorConfig<int>

namespace AnalysisTree {
//______________________________________________________________________________
template <> void VectorConfig<float>::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::VectorConfig<float>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::VectorConfig<float>::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::VectorConfig<float>::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLVectorConfiglEfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::VectorConfig<float> : new ::AnalysisTree::VectorConfig<float>;
   }
   static void *newArray_AnalysisTreecLcLVectorConfiglEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::VectorConfig<float>[nElements] : new ::AnalysisTree::VectorConfig<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLVectorConfiglEfloatgR(void *p) {
      delete ((::AnalysisTree::VectorConfig<float>*)p);
   }
   static void deleteArray_AnalysisTreecLcLVectorConfiglEfloatgR(void *p) {
      delete [] ((::AnalysisTree::VectorConfig<float>*)p);
   }
   static void destruct_AnalysisTreecLcLVectorConfiglEfloatgR(void *p) {
      typedef ::AnalysisTree::VectorConfig<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::VectorConfig<float>

namespace AnalysisTree {
//______________________________________________________________________________
template <> void VectorConfig<bool>::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::VectorConfig<bool>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::VectorConfig<bool>::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::VectorConfig<bool>::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLVectorConfiglEboolgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::VectorConfig<bool> : new ::AnalysisTree::VectorConfig<bool>;
   }
   static void *newArray_AnalysisTreecLcLVectorConfiglEboolgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::VectorConfig<bool>[nElements] : new ::AnalysisTree::VectorConfig<bool>[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLVectorConfiglEboolgR(void *p) {
      delete ((::AnalysisTree::VectorConfig<bool>*)p);
   }
   static void deleteArray_AnalysisTreecLcLVectorConfiglEboolgR(void *p) {
      delete [] ((::AnalysisTree::VectorConfig<bool>*)p);
   }
   static void destruct_AnalysisTreecLcLVectorConfiglEboolgR(void *p) {
      typedef ::AnalysisTree::VectorConfig<bool> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::VectorConfig<bool>

namespace AnalysisTree {
//______________________________________________________________________________
void BranchConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::BranchConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::BranchConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::BranchConfig::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLBranchConfig(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::BranchConfig : new ::AnalysisTree::BranchConfig;
   }
   static void *newArray_AnalysisTreecLcLBranchConfig(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::BranchConfig[nElements] : new ::AnalysisTree::BranchConfig[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLBranchConfig(void *p) {
      delete ((::AnalysisTree::BranchConfig*)p);
   }
   static void deleteArray_AnalysisTreecLcLBranchConfig(void *p) {
      delete [] ((::AnalysisTree::BranchConfig*)p);
   }
   static void destruct_AnalysisTreecLcLBranchConfig(void *p) {
      typedef ::AnalysisTree::BranchConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::BranchConfig

namespace AnalysisTree {
//______________________________________________________________________________
void Container::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Container.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Container::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Container::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLContainer(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Container : new ::AnalysisTree::Container;
   }
   static void *newArray_AnalysisTreecLcLContainer(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Container[nElements] : new ::AnalysisTree::Container[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLContainer(void *p) {
      delete ((::AnalysisTree::Container*)p);
   }
   static void deleteArray_AnalysisTreecLcLContainer(void *p) {
      delete [] ((::AnalysisTree::Container*)p);
   }
   static void destruct_AnalysisTreecLcLContainer(void *p) {
      typedef ::AnalysisTree::Container current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::Container

namespace AnalysisTree {
//______________________________________________________________________________
void Track::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Track.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Track::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Track::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLTrack(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Track : new ::AnalysisTree::Track;
   }
   static void *newArray_AnalysisTreecLcLTrack(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Track[nElements] : new ::AnalysisTree::Track[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLTrack(void *p) {
      delete ((::AnalysisTree::Track*)p);
   }
   static void deleteArray_AnalysisTreecLcLTrack(void *p) {
      delete [] ((::AnalysisTree::Track*)p);
   }
   static void destruct_AnalysisTreecLcLTrack(void *p) {
      typedef ::AnalysisTree::Track current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::Track

namespace AnalysisTree {
//______________________________________________________________________________
void Hit::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Hit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Hit::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Hit::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLHit(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Hit : new ::AnalysisTree::Hit;
   }
   static void *newArray_AnalysisTreecLcLHit(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Hit[nElements] : new ::AnalysisTree::Hit[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLHit(void *p) {
      delete ((::AnalysisTree::Hit*)p);
   }
   static void deleteArray_AnalysisTreecLcLHit(void *p) {
      delete [] ((::AnalysisTree::Hit*)p);
   }
   static void destruct_AnalysisTreecLcLHit(void *p) {
      typedef ::AnalysisTree::Hit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::Hit

namespace AnalysisTree {
//______________________________________________________________________________
void EventHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::EventHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::EventHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::EventHeader::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLEventHeader(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::EventHeader : new ::AnalysisTree::EventHeader;
   }
   static void *newArray_AnalysisTreecLcLEventHeader(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::EventHeader[nElements] : new ::AnalysisTree::EventHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLEventHeader(void *p) {
      delete ((::AnalysisTree::EventHeader*)p);
   }
   static void deleteArray_AnalysisTreecLcLEventHeader(void *p) {
      delete [] ((::AnalysisTree::EventHeader*)p);
   }
   static void destruct_AnalysisTreecLcLEventHeader(void *p) {
      typedef ::AnalysisTree::EventHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::EventHeader

namespace AnalysisTree {
//______________________________________________________________________________
void Module::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Module.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Module::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Module::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLModule(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Module : new ::AnalysisTree::Module;
   }
   static void *newArray_AnalysisTreecLcLModule(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Module[nElements] : new ::AnalysisTree::Module[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLModule(void *p) {
      delete ((::AnalysisTree::Module*)p);
   }
   static void deleteArray_AnalysisTreecLcLModule(void *p) {
      delete [] ((::AnalysisTree::Module*)p);
   }
   static void destruct_AnalysisTreecLcLModule(void *p) {
      typedef ::AnalysisTree::Module current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::Module

namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLModulePosition(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::ModulePosition : new ::AnalysisTree::ModulePosition;
   }
   static void *newArray_AnalysisTreecLcLModulePosition(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::ModulePosition[nElements] : new ::AnalysisTree::ModulePosition[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLModulePosition(void *p) {
      delete ((::AnalysisTree::ModulePosition*)p);
   }
   static void deleteArray_AnalysisTreecLcLModulePosition(void *p) {
      delete [] ((::AnalysisTree::ModulePosition*)p);
   }
   static void destruct_AnalysisTreecLcLModulePosition(void *p) {
      typedef ::AnalysisTree::ModulePosition current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::ModulePosition

namespace AnalysisTree {
//______________________________________________________________________________
void MatchingConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::MatchingConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::MatchingConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::MatchingConfig::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLMatchingConfig(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::MatchingConfig : new ::AnalysisTree::MatchingConfig;
   }
   static void *newArray_AnalysisTreecLcLMatchingConfig(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::MatchingConfig[nElements] : new ::AnalysisTree::MatchingConfig[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLMatchingConfig(void *p) {
      delete ((::AnalysisTree::MatchingConfig*)p);
   }
   static void deleteArray_AnalysisTreecLcLMatchingConfig(void *p) {
      delete [] ((::AnalysisTree::MatchingConfig*)p);
   }
   static void destruct_AnalysisTreecLcLMatchingConfig(void *p) {
      typedef ::AnalysisTree::MatchingConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::MatchingConfig

namespace AnalysisTree {
//______________________________________________________________________________
void Configuration_v3::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Configuration_v3.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Configuration_v3::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Configuration_v3::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLConfiguration_v3(void *p) {
      return  p ? new(p) ::AnalysisTree::Configuration_v3 : new ::AnalysisTree::Configuration_v3;
   }
   static void *newArray_AnalysisTreecLcLConfiguration_v3(Long_t nElements, void *p) {
      return p ? new(p) ::AnalysisTree::Configuration_v3[nElements] : new ::AnalysisTree::Configuration_v3[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLConfiguration_v3(void *p) {
      delete ((::AnalysisTree::Configuration_v3*)p);
   }
   static void deleteArray_AnalysisTreecLcLConfiguration_v3(void *p) {
      delete [] ((::AnalysisTree::Configuration_v3*)p);
   }
   static void destruct_AnalysisTreecLcLConfiguration_v3(void *p) {
      typedef ::AnalysisTree::Configuration_v3 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::Configuration_v3

namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLConfiguration(void *p) {
      return  p ? new(p) ::AnalysisTree::Configuration : new ::AnalysisTree::Configuration;
   }
   static void *newArray_AnalysisTreecLcLConfiguration(Long_t nElements, void *p) {
      return p ? new(p) ::AnalysisTree::Configuration[nElements] : new ::AnalysisTree::Configuration[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLConfiguration(void *p) {
      delete ((::AnalysisTree::Configuration*)p);
   }
   static void deleteArray_AnalysisTreecLcLConfiguration(void *p) {
      delete [] ((::AnalysisTree::Configuration*)p);
   }
   static void destruct_AnalysisTreecLcLConfiguration(void *p) {
      typedef ::AnalysisTree::Configuration current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AnalysisTreecLcLConfiguration(TBuffer &buf, void *obj) {
      ((::AnalysisTree::Configuration*)obj)->::AnalysisTree::Configuration::Streamer(buf);
   }
} // end of namespace ROOT for class ::AnalysisTree::Configuration

namespace AnalysisTree {
//______________________________________________________________________________
void Particle::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Particle.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Particle::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Particle::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLParticle(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Particle : new ::AnalysisTree::Particle;
   }
   static void *newArray_AnalysisTreecLcLParticle(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Particle[nElements] : new ::AnalysisTree::Particle[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLParticle(void *p) {
      delete ((::AnalysisTree::Particle*)p);
   }
   static void deleteArray_AnalysisTreecLcLParticle(void *p) {
      delete [] ((::AnalysisTree::Particle*)p);
   }
   static void destruct_AnalysisTreecLcLParticle(void *p) {
      typedef ::AnalysisTree::Particle current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::Particle

namespace AnalysisTree {
//______________________________________________________________________________
template <> void Detector<AnalysisTree::Track>::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Detector<AnalysisTree::Track>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Detector<AnalysisTree::Track>::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Detector<AnalysisTree::Track>::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Detector<AnalysisTree::Track> : new ::AnalysisTree::Detector<AnalysisTree::Track>;
   }
   static void *newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Detector<AnalysisTree::Track>[nElements] : new ::AnalysisTree::Detector<AnalysisTree::Track>[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR(void *p) {
      delete ((::AnalysisTree::Detector<AnalysisTree::Track>*)p);
   }
   static void deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR(void *p) {
      delete [] ((::AnalysisTree::Detector<AnalysisTree::Track>*)p);
   }
   static void destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLTrackgR(void *p) {
      typedef ::AnalysisTree::Detector<AnalysisTree::Track> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::Detector<AnalysisTree::Track>

namespace AnalysisTree {
//______________________________________________________________________________
template <> void Detector<AnalysisTree::Module>::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Detector<AnalysisTree::Module>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Detector<AnalysisTree::Module>::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Detector<AnalysisTree::Module>::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Detector<AnalysisTree::Module> : new ::AnalysisTree::Detector<AnalysisTree::Module>;
   }
   static void *newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Detector<AnalysisTree::Module>[nElements] : new ::AnalysisTree::Detector<AnalysisTree::Module>[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR(void *p) {
      delete ((::AnalysisTree::Detector<AnalysisTree::Module>*)p);
   }
   static void deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR(void *p) {
      delete [] ((::AnalysisTree::Detector<AnalysisTree::Module>*)p);
   }
   static void destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulegR(void *p) {
      typedef ::AnalysisTree::Detector<AnalysisTree::Module> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::Detector<AnalysisTree::Module>

namespace AnalysisTree {
//______________________________________________________________________________
template <> void Detector<AnalysisTree::ModulePosition>::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Detector<AnalysisTree::ModulePosition>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Detector<AnalysisTree::ModulePosition>::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Detector<AnalysisTree::ModulePosition>::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Detector<AnalysisTree::ModulePosition> : new ::AnalysisTree::Detector<AnalysisTree::ModulePosition>;
   }
   static void *newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Detector<AnalysisTree::ModulePosition>[nElements] : new ::AnalysisTree::Detector<AnalysisTree::ModulePosition>[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR(void *p) {
      delete ((::AnalysisTree::Detector<AnalysisTree::ModulePosition>*)p);
   }
   static void deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR(void *p) {
      delete [] ((::AnalysisTree::Detector<AnalysisTree::ModulePosition>*)p);
   }
   static void destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongR(void *p) {
      typedef ::AnalysisTree::Detector<AnalysisTree::ModulePosition> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::Detector<AnalysisTree::ModulePosition>

namespace AnalysisTree {
//______________________________________________________________________________
template <> void Detector<AnalysisTree::Hit>::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Detector<AnalysisTree::Hit>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Detector<AnalysisTree::Hit>::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Detector<AnalysisTree::Hit>::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Detector<AnalysisTree::Hit> : new ::AnalysisTree::Detector<AnalysisTree::Hit>;
   }
   static void *newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Detector<AnalysisTree::Hit>[nElements] : new ::AnalysisTree::Detector<AnalysisTree::Hit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR(void *p) {
      delete ((::AnalysisTree::Detector<AnalysisTree::Hit>*)p);
   }
   static void deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR(void *p) {
      delete [] ((::AnalysisTree::Detector<AnalysisTree::Hit>*)p);
   }
   static void destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLHitgR(void *p) {
      typedef ::AnalysisTree::Detector<AnalysisTree::Hit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::Detector<AnalysisTree::Hit>

namespace AnalysisTree {
//______________________________________________________________________________
template <> void Detector<AnalysisTree::Particle>::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Detector<AnalysisTree::Particle>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Detector<AnalysisTree::Particle>::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Detector<AnalysisTree::Particle>::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Detector<AnalysisTree::Particle> : new ::AnalysisTree::Detector<AnalysisTree::Particle>;
   }
   static void *newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Detector<AnalysisTree::Particle>[nElements] : new ::AnalysisTree::Detector<AnalysisTree::Particle>[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR(void *p) {
      delete ((::AnalysisTree::Detector<AnalysisTree::Particle>*)p);
   }
   static void deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR(void *p) {
      delete [] ((::AnalysisTree::Detector<AnalysisTree::Particle>*)p);
   }
   static void destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLParticlegR(void *p) {
      typedef ::AnalysisTree::Detector<AnalysisTree::Particle> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::Detector<AnalysisTree::Particle>

namespace AnalysisTree {
//______________________________________________________________________________
template <> void Detector<AnalysisTree::Container>::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Detector<AnalysisTree::Container>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Detector<AnalysisTree::Container>::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Detector<AnalysisTree::Container>::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Detector<AnalysisTree::Container> : new ::AnalysisTree::Detector<AnalysisTree::Container>;
   }
   static void *newArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Detector<AnalysisTree::Container>[nElements] : new ::AnalysisTree::Detector<AnalysisTree::Container>[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR(void *p) {
      delete ((::AnalysisTree::Detector<AnalysisTree::Container>*)p);
   }
   static void deleteArray_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR(void *p) {
      delete [] ((::AnalysisTree::Detector<AnalysisTree::Container>*)p);
   }
   static void destruct_AnalysisTreecLcLDetectorlEAnalysisTreecLcLContainergR(void *p) {
      typedef ::AnalysisTree::Detector<AnalysisTree::Container> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::Detector<AnalysisTree::Container>

namespace AnalysisTree {
//______________________________________________________________________________
void DataHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::DataHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::DataHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::DataHeader::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLDataHeader(void *p) {
      return  p ? new(p) ::AnalysisTree::DataHeader : new ::AnalysisTree::DataHeader;
   }
   static void *newArray_AnalysisTreecLcLDataHeader(Long_t nElements, void *p) {
      return p ? new(p) ::AnalysisTree::DataHeader[nElements] : new ::AnalysisTree::DataHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLDataHeader(void *p) {
      delete ((::AnalysisTree::DataHeader*)p);
   }
   static void deleteArray_AnalysisTreecLcLDataHeader(void *p) {
      delete [] ((::AnalysisTree::DataHeader*)p);
   }
   static void destruct_AnalysisTreecLcLDataHeader(void *p) {
      typedef ::AnalysisTree::DataHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::DataHeader

namespace AnalysisTree {
//______________________________________________________________________________
void Matching::Streamer(TBuffer &R__b)
{
   // Stream an object of class AnalysisTree::Matching.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AnalysisTree::Matching::Class(),this);
   } else {
      R__b.WriteClassBuffer(AnalysisTree::Matching::Class(),this);
   }
}

} // namespace AnalysisTree
namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalysisTreecLcLMatching(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Matching : new ::AnalysisTree::Matching;
   }
   static void *newArray_AnalysisTreecLcLMatching(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::AnalysisTree::Matching[nElements] : new ::AnalysisTree::Matching[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalysisTreecLcLMatching(void *p) {
      delete ((::AnalysisTree::Matching*)p);
   }
   static void deleteArray_AnalysisTreecLcLMatching(void *p) {
      delete [] ((::AnalysisTree::Matching*)p);
   }
   static void destruct_AnalysisTreecLcLMatching(void *p) {
      typedef ::AnalysisTree::Matching current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalysisTree::Matching

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
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = nullptr);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 389,
                  typeid(vector<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));

      ::ROOT::AddClassAlternate("vector<float>","std::vector<float, std::allocator<float> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<float>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<float>*)nullptr)->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete ((vector<float>*)p);
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] ((vector<float>*)p);
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<float>

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
   static TClass *vectorlETVector3gR_Dictionary();
   static void vectorlETVector3gR_TClassManip(TClass*);
   static void *new_vectorlETVector3gR(void *p = nullptr);
   static void *newArray_vectorlETVector3gR(Long_t size, void *p);
   static void delete_vectorlETVector3gR(void *p);
   static void deleteArray_vectorlETVector3gR(void *p);
   static void destruct_vectorlETVector3gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TVector3>*)
   {
      vector<TVector3> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TVector3>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TVector3>", -2, "vector", 389,
                  typeid(vector<TVector3>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETVector3gR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TVector3>) );
      instance.SetNew(&new_vectorlETVector3gR);
      instance.SetNewArray(&newArray_vectorlETVector3gR);
      instance.SetDelete(&delete_vectorlETVector3gR);
      instance.SetDeleteArray(&deleteArray_vectorlETVector3gR);
      instance.SetDestructor(&destruct_vectorlETVector3gR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TVector3> >()));

      ::ROOT::AddClassAlternate("vector<TVector3>","std::vector<TVector3, std::allocator<TVector3> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TVector3>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETVector3gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TVector3>*)nullptr)->GetClass();
      vectorlETVector3gR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETVector3gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETVector3gR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TVector3> : new vector<TVector3>;
   }
   static void *newArray_vectorlETVector3gR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TVector3>[nElements] : new vector<TVector3>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETVector3gR(void *p) {
      delete ((vector<TVector3>*)p);
   }
   static void deleteArray_vectorlETVector3gR(void *p) {
      delete [] ((vector<TVector3>*)p);
   }
   static void destruct_vectorlETVector3gR(void *p) {
      typedef vector<TVector3> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TVector3>

namespace ROOT {
   static TClass *vectorlEAnalysisTreecLcLTrackgR_Dictionary();
   static void vectorlEAnalysisTreecLcLTrackgR_TClassManip(TClass*);
   static void *new_vectorlEAnalysisTreecLcLTrackgR(void *p = nullptr);
   static void *newArray_vectorlEAnalysisTreecLcLTrackgR(Long_t size, void *p);
   static void delete_vectorlEAnalysisTreecLcLTrackgR(void *p);
   static void deleteArray_vectorlEAnalysisTreecLcLTrackgR(void *p);
   static void destruct_vectorlEAnalysisTreecLcLTrackgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<AnalysisTree::Track>*)
   {
      vector<AnalysisTree::Track> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<AnalysisTree::Track>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<AnalysisTree::Track>", -2, "vector", 389,
                  typeid(vector<AnalysisTree::Track>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAnalysisTreecLcLTrackgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<AnalysisTree::Track>) );
      instance.SetNew(&new_vectorlEAnalysisTreecLcLTrackgR);
      instance.SetNewArray(&newArray_vectorlEAnalysisTreecLcLTrackgR);
      instance.SetDelete(&delete_vectorlEAnalysisTreecLcLTrackgR);
      instance.SetDeleteArray(&deleteArray_vectorlEAnalysisTreecLcLTrackgR);
      instance.SetDestructor(&destruct_vectorlEAnalysisTreecLcLTrackgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<AnalysisTree::Track> >()));

      ::ROOT::AddClassAlternate("vector<AnalysisTree::Track>","std::vector<AnalysisTree::Track, std::allocator<AnalysisTree::Track> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<AnalysisTree::Track>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAnalysisTreecLcLTrackgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<AnalysisTree::Track>*)nullptr)->GetClass();
      vectorlEAnalysisTreecLcLTrackgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAnalysisTreecLcLTrackgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAnalysisTreecLcLTrackgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::Track> : new vector<AnalysisTree::Track>;
   }
   static void *newArray_vectorlEAnalysisTreecLcLTrackgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::Track>[nElements] : new vector<AnalysisTree::Track>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAnalysisTreecLcLTrackgR(void *p) {
      delete ((vector<AnalysisTree::Track>*)p);
   }
   static void deleteArray_vectorlEAnalysisTreecLcLTrackgR(void *p) {
      delete [] ((vector<AnalysisTree::Track>*)p);
   }
   static void destruct_vectorlEAnalysisTreecLcLTrackgR(void *p) {
      typedef vector<AnalysisTree::Track> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<AnalysisTree::Track>

namespace ROOT {
   static TClass *vectorlEAnalysisTreecLcLParticlegR_Dictionary();
   static void vectorlEAnalysisTreecLcLParticlegR_TClassManip(TClass*);
   static void *new_vectorlEAnalysisTreecLcLParticlegR(void *p = nullptr);
   static void *newArray_vectorlEAnalysisTreecLcLParticlegR(Long_t size, void *p);
   static void delete_vectorlEAnalysisTreecLcLParticlegR(void *p);
   static void deleteArray_vectorlEAnalysisTreecLcLParticlegR(void *p);
   static void destruct_vectorlEAnalysisTreecLcLParticlegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<AnalysisTree::Particle>*)
   {
      vector<AnalysisTree::Particle> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<AnalysisTree::Particle>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<AnalysisTree::Particle>", -2, "vector", 389,
                  typeid(vector<AnalysisTree::Particle>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAnalysisTreecLcLParticlegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<AnalysisTree::Particle>) );
      instance.SetNew(&new_vectorlEAnalysisTreecLcLParticlegR);
      instance.SetNewArray(&newArray_vectorlEAnalysisTreecLcLParticlegR);
      instance.SetDelete(&delete_vectorlEAnalysisTreecLcLParticlegR);
      instance.SetDeleteArray(&deleteArray_vectorlEAnalysisTreecLcLParticlegR);
      instance.SetDestructor(&destruct_vectorlEAnalysisTreecLcLParticlegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<AnalysisTree::Particle> >()));

      ::ROOT::AddClassAlternate("vector<AnalysisTree::Particle>","std::vector<AnalysisTree::Particle, std::allocator<AnalysisTree::Particle> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<AnalysisTree::Particle>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAnalysisTreecLcLParticlegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<AnalysisTree::Particle>*)nullptr)->GetClass();
      vectorlEAnalysisTreecLcLParticlegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAnalysisTreecLcLParticlegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAnalysisTreecLcLParticlegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::Particle> : new vector<AnalysisTree::Particle>;
   }
   static void *newArray_vectorlEAnalysisTreecLcLParticlegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::Particle>[nElements] : new vector<AnalysisTree::Particle>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAnalysisTreecLcLParticlegR(void *p) {
      delete ((vector<AnalysisTree::Particle>*)p);
   }
   static void deleteArray_vectorlEAnalysisTreecLcLParticlegR(void *p) {
      delete [] ((vector<AnalysisTree::Particle>*)p);
   }
   static void destruct_vectorlEAnalysisTreecLcLParticlegR(void *p) {
      typedef vector<AnalysisTree::Particle> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<AnalysisTree::Particle>

namespace ROOT {
   static TClass *vectorlEAnalysisTreecLcLModulePositiongR_Dictionary();
   static void vectorlEAnalysisTreecLcLModulePositiongR_TClassManip(TClass*);
   static void *new_vectorlEAnalysisTreecLcLModulePositiongR(void *p = nullptr);
   static void *newArray_vectorlEAnalysisTreecLcLModulePositiongR(Long_t size, void *p);
   static void delete_vectorlEAnalysisTreecLcLModulePositiongR(void *p);
   static void deleteArray_vectorlEAnalysisTreecLcLModulePositiongR(void *p);
   static void destruct_vectorlEAnalysisTreecLcLModulePositiongR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<AnalysisTree::ModulePosition>*)
   {
      vector<AnalysisTree::ModulePosition> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<AnalysisTree::ModulePosition>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<AnalysisTree::ModulePosition>", -2, "vector", 389,
                  typeid(vector<AnalysisTree::ModulePosition>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAnalysisTreecLcLModulePositiongR_Dictionary, isa_proxy, 0,
                  sizeof(vector<AnalysisTree::ModulePosition>) );
      instance.SetNew(&new_vectorlEAnalysisTreecLcLModulePositiongR);
      instance.SetNewArray(&newArray_vectorlEAnalysisTreecLcLModulePositiongR);
      instance.SetDelete(&delete_vectorlEAnalysisTreecLcLModulePositiongR);
      instance.SetDeleteArray(&deleteArray_vectorlEAnalysisTreecLcLModulePositiongR);
      instance.SetDestructor(&destruct_vectorlEAnalysisTreecLcLModulePositiongR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<AnalysisTree::ModulePosition> >()));

      ::ROOT::AddClassAlternate("vector<AnalysisTree::ModulePosition>","std::vector<AnalysisTree::ModulePosition, std::allocator<AnalysisTree::ModulePosition> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<AnalysisTree::ModulePosition>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAnalysisTreecLcLModulePositiongR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<AnalysisTree::ModulePosition>*)nullptr)->GetClass();
      vectorlEAnalysisTreecLcLModulePositiongR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAnalysisTreecLcLModulePositiongR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAnalysisTreecLcLModulePositiongR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::ModulePosition> : new vector<AnalysisTree::ModulePosition>;
   }
   static void *newArray_vectorlEAnalysisTreecLcLModulePositiongR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::ModulePosition>[nElements] : new vector<AnalysisTree::ModulePosition>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAnalysisTreecLcLModulePositiongR(void *p) {
      delete ((vector<AnalysisTree::ModulePosition>*)p);
   }
   static void deleteArray_vectorlEAnalysisTreecLcLModulePositiongR(void *p) {
      delete [] ((vector<AnalysisTree::ModulePosition>*)p);
   }
   static void destruct_vectorlEAnalysisTreecLcLModulePositiongR(void *p) {
      typedef vector<AnalysisTree::ModulePosition> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<AnalysisTree::ModulePosition>

namespace ROOT {
   static TClass *vectorlEAnalysisTreecLcLModulegR_Dictionary();
   static void vectorlEAnalysisTreecLcLModulegR_TClassManip(TClass*);
   static void *new_vectorlEAnalysisTreecLcLModulegR(void *p = nullptr);
   static void *newArray_vectorlEAnalysisTreecLcLModulegR(Long_t size, void *p);
   static void delete_vectorlEAnalysisTreecLcLModulegR(void *p);
   static void deleteArray_vectorlEAnalysisTreecLcLModulegR(void *p);
   static void destruct_vectorlEAnalysisTreecLcLModulegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<AnalysisTree::Module>*)
   {
      vector<AnalysisTree::Module> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<AnalysisTree::Module>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<AnalysisTree::Module>", -2, "vector", 389,
                  typeid(vector<AnalysisTree::Module>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAnalysisTreecLcLModulegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<AnalysisTree::Module>) );
      instance.SetNew(&new_vectorlEAnalysisTreecLcLModulegR);
      instance.SetNewArray(&newArray_vectorlEAnalysisTreecLcLModulegR);
      instance.SetDelete(&delete_vectorlEAnalysisTreecLcLModulegR);
      instance.SetDeleteArray(&deleteArray_vectorlEAnalysisTreecLcLModulegR);
      instance.SetDestructor(&destruct_vectorlEAnalysisTreecLcLModulegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<AnalysisTree::Module> >()));

      ::ROOT::AddClassAlternate("vector<AnalysisTree::Module>","std::vector<AnalysisTree::Module, std::allocator<AnalysisTree::Module> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<AnalysisTree::Module>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAnalysisTreecLcLModulegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<AnalysisTree::Module>*)nullptr)->GetClass();
      vectorlEAnalysisTreecLcLModulegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAnalysisTreecLcLModulegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAnalysisTreecLcLModulegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::Module> : new vector<AnalysisTree::Module>;
   }
   static void *newArray_vectorlEAnalysisTreecLcLModulegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::Module>[nElements] : new vector<AnalysisTree::Module>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAnalysisTreecLcLModulegR(void *p) {
      delete ((vector<AnalysisTree::Module>*)p);
   }
   static void deleteArray_vectorlEAnalysisTreecLcLModulegR(void *p) {
      delete [] ((vector<AnalysisTree::Module>*)p);
   }
   static void destruct_vectorlEAnalysisTreecLcLModulegR(void *p) {
      typedef vector<AnalysisTree::Module> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<AnalysisTree::Module>

namespace ROOT {
   static TClass *vectorlEAnalysisTreecLcLMatchingConfiggR_Dictionary();
   static void vectorlEAnalysisTreecLcLMatchingConfiggR_TClassManip(TClass*);
   static void *new_vectorlEAnalysisTreecLcLMatchingConfiggR(void *p = nullptr);
   static void *newArray_vectorlEAnalysisTreecLcLMatchingConfiggR(Long_t size, void *p);
   static void delete_vectorlEAnalysisTreecLcLMatchingConfiggR(void *p);
   static void deleteArray_vectorlEAnalysisTreecLcLMatchingConfiggR(void *p);
   static void destruct_vectorlEAnalysisTreecLcLMatchingConfiggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<AnalysisTree::MatchingConfig>*)
   {
      vector<AnalysisTree::MatchingConfig> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<AnalysisTree::MatchingConfig>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<AnalysisTree::MatchingConfig>", -2, "vector", 389,
                  typeid(vector<AnalysisTree::MatchingConfig>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAnalysisTreecLcLMatchingConfiggR_Dictionary, isa_proxy, 0,
                  sizeof(vector<AnalysisTree::MatchingConfig>) );
      instance.SetNew(&new_vectorlEAnalysisTreecLcLMatchingConfiggR);
      instance.SetNewArray(&newArray_vectorlEAnalysisTreecLcLMatchingConfiggR);
      instance.SetDelete(&delete_vectorlEAnalysisTreecLcLMatchingConfiggR);
      instance.SetDeleteArray(&deleteArray_vectorlEAnalysisTreecLcLMatchingConfiggR);
      instance.SetDestructor(&destruct_vectorlEAnalysisTreecLcLMatchingConfiggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<AnalysisTree::MatchingConfig> >()));

      ::ROOT::AddClassAlternate("vector<AnalysisTree::MatchingConfig>","std::vector<AnalysisTree::MatchingConfig, std::allocator<AnalysisTree::MatchingConfig> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<AnalysisTree::MatchingConfig>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAnalysisTreecLcLMatchingConfiggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<AnalysisTree::MatchingConfig>*)nullptr)->GetClass();
      vectorlEAnalysisTreecLcLMatchingConfiggR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAnalysisTreecLcLMatchingConfiggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAnalysisTreecLcLMatchingConfiggR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::MatchingConfig> : new vector<AnalysisTree::MatchingConfig>;
   }
   static void *newArray_vectorlEAnalysisTreecLcLMatchingConfiggR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::MatchingConfig>[nElements] : new vector<AnalysisTree::MatchingConfig>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAnalysisTreecLcLMatchingConfiggR(void *p) {
      delete ((vector<AnalysisTree::MatchingConfig>*)p);
   }
   static void deleteArray_vectorlEAnalysisTreecLcLMatchingConfiggR(void *p) {
      delete [] ((vector<AnalysisTree::MatchingConfig>*)p);
   }
   static void destruct_vectorlEAnalysisTreecLcLMatchingConfiggR(void *p) {
      typedef vector<AnalysisTree::MatchingConfig> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<AnalysisTree::MatchingConfig>

namespace ROOT {
   static TClass *vectorlEAnalysisTreecLcLHitgR_Dictionary();
   static void vectorlEAnalysisTreecLcLHitgR_TClassManip(TClass*);
   static void *new_vectorlEAnalysisTreecLcLHitgR(void *p = nullptr);
   static void *newArray_vectorlEAnalysisTreecLcLHitgR(Long_t size, void *p);
   static void delete_vectorlEAnalysisTreecLcLHitgR(void *p);
   static void deleteArray_vectorlEAnalysisTreecLcLHitgR(void *p);
   static void destruct_vectorlEAnalysisTreecLcLHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<AnalysisTree::Hit>*)
   {
      vector<AnalysisTree::Hit> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<AnalysisTree::Hit>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<AnalysisTree::Hit>", -2, "vector", 389,
                  typeid(vector<AnalysisTree::Hit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAnalysisTreecLcLHitgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<AnalysisTree::Hit>) );
      instance.SetNew(&new_vectorlEAnalysisTreecLcLHitgR);
      instance.SetNewArray(&newArray_vectorlEAnalysisTreecLcLHitgR);
      instance.SetDelete(&delete_vectorlEAnalysisTreecLcLHitgR);
      instance.SetDeleteArray(&deleteArray_vectorlEAnalysisTreecLcLHitgR);
      instance.SetDestructor(&destruct_vectorlEAnalysisTreecLcLHitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<AnalysisTree::Hit> >()));

      ::ROOT::AddClassAlternate("vector<AnalysisTree::Hit>","std::vector<AnalysisTree::Hit, std::allocator<AnalysisTree::Hit> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<AnalysisTree::Hit>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAnalysisTreecLcLHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<AnalysisTree::Hit>*)nullptr)->GetClass();
      vectorlEAnalysisTreecLcLHitgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAnalysisTreecLcLHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAnalysisTreecLcLHitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::Hit> : new vector<AnalysisTree::Hit>;
   }
   static void *newArray_vectorlEAnalysisTreecLcLHitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::Hit>[nElements] : new vector<AnalysisTree::Hit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAnalysisTreecLcLHitgR(void *p) {
      delete ((vector<AnalysisTree::Hit>*)p);
   }
   static void deleteArray_vectorlEAnalysisTreecLcLHitgR(void *p) {
      delete [] ((vector<AnalysisTree::Hit>*)p);
   }
   static void destruct_vectorlEAnalysisTreecLcLHitgR(void *p) {
      typedef vector<AnalysisTree::Hit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<AnalysisTree::Hit>

namespace ROOT {
   static TClass *vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR_Dictionary();
   static void vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR_TClassManip(TClass*);
   static void *new_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR(void *p = nullptr);
   static void *newArray_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR(Long_t size, void *p);
   static void delete_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR(void *p);
   static void deleteArray_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR(void *p);
   static void destruct_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> >*)
   {
      vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> >", -2, "vector", 389,
                  typeid(vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> >) );
      instance.SetNew(&new_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR);
      instance.SetNewArray(&newArray_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR);
      instance.SetDelete(&delete_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR);
      instance.SetDestructor(&destruct_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> > >()));

      ::ROOT::AddClassAlternate("vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> >","std::vector<AnalysisTree::Detector<AnalysisTree::ModulePosition>, std::allocator<AnalysisTree::Detector<AnalysisTree::ModulePosition> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> >*)nullptr)->GetClass();
      vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> > : new vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> >;
   }
   static void *newArray_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> >[nElements] : new vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR(void *p) {
      delete ((vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> >*)p);
   }
   static void deleteArray_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR(void *p) {
      delete [] ((vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> >*)p);
   }
   static void destruct_vectorlEAnalysisTreecLcLDetectorlEAnalysisTreecLcLModulePositiongRsPgR(void *p) {
      typedef vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<AnalysisTree::Detector<AnalysisTree::ModulePosition> >

namespace ROOT {
   static TClass *vectorlEAnalysisTreecLcLContainergR_Dictionary();
   static void vectorlEAnalysisTreecLcLContainergR_TClassManip(TClass*);
   static void *new_vectorlEAnalysisTreecLcLContainergR(void *p = nullptr);
   static void *newArray_vectorlEAnalysisTreecLcLContainergR(Long_t size, void *p);
   static void delete_vectorlEAnalysisTreecLcLContainergR(void *p);
   static void deleteArray_vectorlEAnalysisTreecLcLContainergR(void *p);
   static void destruct_vectorlEAnalysisTreecLcLContainergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<AnalysisTree::Container>*)
   {
      vector<AnalysisTree::Container> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<AnalysisTree::Container>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<AnalysisTree::Container>", -2, "vector", 389,
                  typeid(vector<AnalysisTree::Container>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAnalysisTreecLcLContainergR_Dictionary, isa_proxy, 0,
                  sizeof(vector<AnalysisTree::Container>) );
      instance.SetNew(&new_vectorlEAnalysisTreecLcLContainergR);
      instance.SetNewArray(&newArray_vectorlEAnalysisTreecLcLContainergR);
      instance.SetDelete(&delete_vectorlEAnalysisTreecLcLContainergR);
      instance.SetDeleteArray(&deleteArray_vectorlEAnalysisTreecLcLContainergR);
      instance.SetDestructor(&destruct_vectorlEAnalysisTreecLcLContainergR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<AnalysisTree::Container> >()));

      ::ROOT::AddClassAlternate("vector<AnalysisTree::Container>","std::vector<AnalysisTree::Container, std::allocator<AnalysisTree::Container> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<AnalysisTree::Container>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAnalysisTreecLcLContainergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<AnalysisTree::Container>*)nullptr)->GetClass();
      vectorlEAnalysisTreecLcLContainergR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAnalysisTreecLcLContainergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAnalysisTreecLcLContainergR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::Container> : new vector<AnalysisTree::Container>;
   }
   static void *newArray_vectorlEAnalysisTreecLcLContainergR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AnalysisTree::Container>[nElements] : new vector<AnalysisTree::Container>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAnalysisTreecLcLContainergR(void *p) {
      delete ((vector<AnalysisTree::Container>*)p);
   }
   static void deleteArray_vectorlEAnalysisTreecLcLContainergR(void *p) {
      delete [] ((vector<AnalysisTree::Container>*)p);
   }
   static void destruct_vectorlEAnalysisTreecLcLContainergR(void *p) {
      typedef vector<AnalysisTree::Container> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<AnalysisTree::Container>

namespace ROOT {
   static TClass *maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR_Dictionary();
   static void maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR_TClassManip(TClass*);
   static void *new_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR(void *p = nullptr);
   static void *newArray_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR(Long_t size, void *p);
   static void delete_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR(void *p);
   static void deleteArray_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR(void *p);
   static void destruct_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<unsigned long,AnalysisTree::BranchConfig>*)
   {
      map<unsigned long,AnalysisTree::BranchConfig> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<unsigned long,AnalysisTree::BranchConfig>));
      static ::ROOT::TGenericClassInfo 
         instance("map<unsigned long,AnalysisTree::BranchConfig>", -2, "map", 100,
                  typeid(map<unsigned long,AnalysisTree::BranchConfig>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR_Dictionary, isa_proxy, 0,
                  sizeof(map<unsigned long,AnalysisTree::BranchConfig>) );
      instance.SetNew(&new_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR);
      instance.SetNewArray(&newArray_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR);
      instance.SetDelete(&delete_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR);
      instance.SetDeleteArray(&deleteArray_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR);
      instance.SetDestructor(&destruct_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<unsigned long,AnalysisTree::BranchConfig> >()));

      ::ROOT::AddClassAlternate("map<unsigned long,AnalysisTree::BranchConfig>","std::map<unsigned long, AnalysisTree::BranchConfig, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, AnalysisTree::BranchConfig> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<unsigned long,AnalysisTree::BranchConfig>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<unsigned long,AnalysisTree::BranchConfig>*)nullptr)->GetClass();
      maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR_TClassManip(theClass);
   return theClass;
   }

   static void maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned long,AnalysisTree::BranchConfig> : new map<unsigned long,AnalysisTree::BranchConfig>;
   }
   static void *newArray_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<unsigned long,AnalysisTree::BranchConfig>[nElements] : new map<unsigned long,AnalysisTree::BranchConfig>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR(void *p) {
      delete ((map<unsigned long,AnalysisTree::BranchConfig>*)p);
   }
   static void deleteArray_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR(void *p) {
      delete [] ((map<unsigned long,AnalysisTree::BranchConfig>*)p);
   }
   static void destruct_maplEunsignedsPlongcOAnalysisTreecLcLBranchConfiggR(void *p) {
      typedef map<unsigned long,AnalysisTree::BranchConfig> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<unsigned long,AnalysisTree::BranchConfig>

namespace ROOT {
   static TClass *maplEstringcOAnalysisTreecLcLConfigElementgR_Dictionary();
   static void maplEstringcOAnalysisTreecLcLConfigElementgR_TClassManip(TClass*);
   static void *new_maplEstringcOAnalysisTreecLcLConfigElementgR(void *p = nullptr);
   static void *newArray_maplEstringcOAnalysisTreecLcLConfigElementgR(Long_t size, void *p);
   static void delete_maplEstringcOAnalysisTreecLcLConfigElementgR(void *p);
   static void deleteArray_maplEstringcOAnalysisTreecLcLConfigElementgR(void *p);
   static void destruct_maplEstringcOAnalysisTreecLcLConfigElementgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,AnalysisTree::ConfigElement>*)
   {
      map<string,AnalysisTree::ConfigElement> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,AnalysisTree::ConfigElement>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,AnalysisTree::ConfigElement>", -2, "map", 100,
                  typeid(map<string,AnalysisTree::ConfigElement>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOAnalysisTreecLcLConfigElementgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,AnalysisTree::ConfigElement>) );
      instance.SetNew(&new_maplEstringcOAnalysisTreecLcLConfigElementgR);
      instance.SetNewArray(&newArray_maplEstringcOAnalysisTreecLcLConfigElementgR);
      instance.SetDelete(&delete_maplEstringcOAnalysisTreecLcLConfigElementgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOAnalysisTreecLcLConfigElementgR);
      instance.SetDestructor(&destruct_maplEstringcOAnalysisTreecLcLConfigElementgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,AnalysisTree::ConfigElement> >()));

      ::ROOT::AddClassAlternate("map<string,AnalysisTree::ConfigElement>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, AnalysisTree::ConfigElement, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, AnalysisTree::ConfigElement> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,AnalysisTree::ConfigElement>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOAnalysisTreecLcLConfigElementgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,AnalysisTree::ConfigElement>*)nullptr)->GetClass();
      maplEstringcOAnalysisTreecLcLConfigElementgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOAnalysisTreecLcLConfigElementgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOAnalysisTreecLcLConfigElementgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,AnalysisTree::ConfigElement> : new map<string,AnalysisTree::ConfigElement>;
   }
   static void *newArray_maplEstringcOAnalysisTreecLcLConfigElementgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,AnalysisTree::ConfigElement>[nElements] : new map<string,AnalysisTree::ConfigElement>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOAnalysisTreecLcLConfigElementgR(void *p) {
      delete ((map<string,AnalysisTree::ConfigElement>*)p);
   }
   static void deleteArray_maplEstringcOAnalysisTreecLcLConfigElementgR(void *p) {
      delete [] ((map<string,AnalysisTree::ConfigElement>*)p);
   }
   static void destruct_maplEstringcOAnalysisTreecLcLConfigElementgR(void *p) {
      typedef map<string,AnalysisTree::ConfigElement> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,AnalysisTree::ConfigElement>

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

namespace ROOT {
   static TClass *maplEarraylEstringcO2gRcOstringgR_Dictionary();
   static void maplEarraylEstringcO2gRcOstringgR_TClassManip(TClass*);
   static void *new_maplEarraylEstringcO2gRcOstringgR(void *p = nullptr);
   static void *newArray_maplEarraylEstringcO2gRcOstringgR(Long_t size, void *p);
   static void delete_maplEarraylEstringcO2gRcOstringgR(void *p);
   static void deleteArray_maplEarraylEstringcO2gRcOstringgR(void *p);
   static void destruct_maplEarraylEstringcO2gRcOstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<array<string,2>,string>*)
   {
      map<array<string,2>,string> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<array<string,2>,string>));
      static ::ROOT::TGenericClassInfo 
         instance("map<array<string,2>,string>", -2, "map", 100,
                  typeid(map<array<string,2>,string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEarraylEstringcO2gRcOstringgR_Dictionary, isa_proxy, 0,
                  sizeof(map<array<string,2>,string>) );
      instance.SetNew(&new_maplEarraylEstringcO2gRcOstringgR);
      instance.SetNewArray(&newArray_maplEarraylEstringcO2gRcOstringgR);
      instance.SetDelete(&delete_maplEarraylEstringcO2gRcOstringgR);
      instance.SetDeleteArray(&deleteArray_maplEarraylEstringcO2gRcOstringgR);
      instance.SetDestructor(&destruct_maplEarraylEstringcO2gRcOstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<array<string,2>,string> >()));

      ::ROOT::AddClassAlternate("map<array<string,2>,string>","std::map<std::array<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, 2ul>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::array<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, 2ul> >, std::allocator<std::pair<std::array<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, 2ul> const, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<array<string,2>,string>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEarraylEstringcO2gRcOstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<array<string,2>,string>*)nullptr)->GetClass();
      maplEarraylEstringcO2gRcOstringgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEarraylEstringcO2gRcOstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEarraylEstringcO2gRcOstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<array<string,2>,string> : new map<array<string,2>,string>;
   }
   static void *newArray_maplEarraylEstringcO2gRcOstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<array<string,2>,string>[nElements] : new map<array<string,2>,string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEarraylEstringcO2gRcOstringgR(void *p) {
      delete ((map<array<string,2>,string>*)p);
   }
   static void deleteArray_maplEarraylEstringcO2gRcOstringgR(void *p) {
      delete [] ((map<array<string,2>,string>*)p);
   }
   static void destruct_maplEarraylEstringcO2gRcOstringgR(void *p) {
      typedef map<array<string,2>,string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<array<string,2>,string>

namespace {
  void TriggerDictionaryInitialization_libAnalysisTreeBase_Impl() {
    static const char* headers[] = {
"IndexedObject.hpp",
"Track.hpp",
"Hit.hpp",
"EventHeader.hpp",
"Module.hpp",
"Configuration.hpp",
"BranchConfig.hpp",
"Container.hpp",
"DataHeader.hpp",
"Matching.hpp",
"Particle.hpp",
"Constants.hpp",
"Detector.hpp",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree",
"/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/core",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREE-prefix/src/ANALYSISTREE-build/core/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libAnalysisTreeBase dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$IndexedObject.hpp")))  IndexAccessor;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$IndexedObject.hpp")))  IndexedObject;}
namespace AnalysisTree{struct __attribute__((annotate("$clingAutoload$BranchConfig.hpp")))  __attribute__((annotate("$clingAutoload$Track.hpp")))  ConfigElement;}
namespace AnalysisTree{template <typename T> class __attribute__((annotate("$clingAutoload$BranchConfig.hpp")))  __attribute__((annotate("$clingAutoload$Track.hpp")))  VectorConfig;
}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$BranchConfig.hpp")))  __attribute__((annotate("$clingAutoload$Track.hpp")))  BranchConfig;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Container.hpp")))  __attribute__((annotate("$clingAutoload$Track.hpp")))  Container;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Track.hpp")))  Track;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Hit.hpp")))  Hit;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$EventHeader.hpp")))  EventHeader;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Module.hpp")))  Module;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Module.hpp")))  ModulePosition;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Configuration.hpp")))  MatchingConfig;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Configuration.hpp")))  Configuration_v3;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Configuration.hpp")))  Configuration;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Particle.hpp")))  __attribute__((annotate("$clingAutoload$DataHeader.hpp")))  Particle;}
namespace AnalysisTree{template <class T> class __attribute__((annotate("$clingAutoload$Detector.hpp")))  __attribute__((annotate("$clingAutoload$DataHeader.hpp")))  Detector;
}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$DataHeader.hpp")))  DataHeader;}
namespace AnalysisTree{class __attribute__((annotate("$clingAutoload$Matching.hpp")))  Matching;}
namespace AnalysisTree{using TrackDetector __attribute__((annotate("$clingAutoload$Detector.hpp")))  __attribute__((annotate("$clingAutoload$DataHeader.hpp")))  = Detector<AnalysisTree::Track>;}
namespace AnalysisTree{using ModuleDetector __attribute__((annotate("$clingAutoload$Detector.hpp")))  __attribute__((annotate("$clingAutoload$DataHeader.hpp")))  = Detector<AnalysisTree::Module>;}
namespace AnalysisTree{using ModulePositions __attribute__((annotate("$clingAutoload$Detector.hpp")))  __attribute__((annotate("$clingAutoload$DataHeader.hpp")))  = Detector<AnalysisTree::ModulePosition>;}
namespace AnalysisTree{using HitDetector __attribute__((annotate("$clingAutoload$Detector.hpp")))  __attribute__((annotate("$clingAutoload$DataHeader.hpp")))  = Detector<AnalysisTree::Hit>;}
namespace AnalysisTree{using Particles __attribute__((annotate("$clingAutoload$Detector.hpp")))  __attribute__((annotate("$clingAutoload$DataHeader.hpp")))  = Detector<AnalysisTree::Particle>;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libAnalysisTreeBase dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "IndexedObject.hpp"
#include "Track.hpp"
#include "Hit.hpp"
#include "EventHeader.hpp"
#include "Module.hpp"
#include "Configuration.hpp"
#include "BranchConfig.hpp"
#include "Container.hpp"
#include "DataHeader.hpp"
#include "Matching.hpp"
#include "Particle.hpp"
#include "Constants.hpp"
#include "Detector.hpp"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"AnalysisTree::BranchConfig", payloadCode, "@",
"AnalysisTree::ConfigElement", payloadCode, "@",
"AnalysisTree::Configuration", payloadCode, "@",
"AnalysisTree::Configuration_v3", payloadCode, "@",
"AnalysisTree::Container", payloadCode, "@",
"AnalysisTree::DataHeader", payloadCode, "@",
"AnalysisTree::Detector<AnalysisTree::Container>", payloadCode, "@",
"AnalysisTree::Detector<AnalysisTree::Hit>", payloadCode, "@",
"AnalysisTree::Detector<AnalysisTree::Module>", payloadCode, "@",
"AnalysisTree::Detector<AnalysisTree::ModulePosition>", payloadCode, "@",
"AnalysisTree::Detector<AnalysisTree::Particle>", payloadCode, "@",
"AnalysisTree::Detector<AnalysisTree::Track>", payloadCode, "@",
"AnalysisTree::EventHeader", payloadCode, "@",
"AnalysisTree::Hit", payloadCode, "@",
"AnalysisTree::HitDetector", payloadCode, "@",
"AnalysisTree::IndexAccessor", payloadCode, "@",
"AnalysisTree::IndexedObject", payloadCode, "@",
"AnalysisTree::Matching", payloadCode, "@",
"AnalysisTree::MatchingConfig", payloadCode, "@",
"AnalysisTree::Module", payloadCode, "@",
"AnalysisTree::ModuleDetector", payloadCode, "@",
"AnalysisTree::ModulePosition", payloadCode, "@",
"AnalysisTree::ModulePositions", payloadCode, "@",
"AnalysisTree::Particle", payloadCode, "@",
"AnalysisTree::Particles", payloadCode, "@",
"AnalysisTree::Track", payloadCode, "@",
"AnalysisTree::TrackDetector", payloadCode, "@",
"AnalysisTree::VectorConfig<bool>", payloadCode, "@",
"AnalysisTree::VectorConfig<float>", payloadCode, "@",
"AnalysisTree::VectorConfig<int>", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libAnalysisTreeBase",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libAnalysisTreeBase_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libAnalysisTreeBase_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libAnalysisTreeBase() {
  TriggerDictionaryInitialization_libAnalysisTreeBase_Impl();
}
