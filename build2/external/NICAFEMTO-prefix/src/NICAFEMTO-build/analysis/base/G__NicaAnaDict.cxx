// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIheinemanndIgpuunpackerdIcbmrootdIbuild2dIexternaldINICAFEMTOmIprefixdIsrcdINICAFEMTOmIbuilddIanalysisdIbasedIG__NicaAnaDict
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
#include "NicaEventAna.h"
#include "NicaMemoryMapManager.h"
#include "NicaMultiTrackAna.h"
#include "NicaTrackAna.h"
#include "NicaTwoTrackAna.h"
#include "NicaEventAnaChain.h"
#include "NicaTrackAnaChain.h"
#include "NicaTwoTrackAnaChain.h"
#include "NicaSplitedTrackToStatusTask.h"
#include "NicaBoostTask.h"
#include "NicaMiniEventTask.h"
#include "NicaCopyDataTask.h"
#include "NicaSmearTask.h"
#include "NicaSmearAlgorithm.h"
#include "NicaTrackSmearMomentumPercent.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_NicaEventAna(void *p = nullptr);
   static void *newArray_NicaEventAna(Long_t size, void *p);
   static void delete_NicaEventAna(void *p);
   static void deleteArray_NicaEventAna(void *p);
   static void destruct_NicaEventAna(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventAna*)
   {
      ::NicaEventAna *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventAna >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventAna", ::NicaEventAna::Class_Version(), "NicaEventAna.h", 34,
                  typeid(::NicaEventAna), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventAna::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventAna) );
      instance.SetNew(&new_NicaEventAna);
      instance.SetNewArray(&newArray_NicaEventAna);
      instance.SetDelete(&delete_NicaEventAna);
      instance.SetDeleteArray(&deleteArray_NicaEventAna);
      instance.SetDestructor(&destruct_NicaEventAna);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventAna*)
   {
      return GenerateInitInstanceLocal((::NicaEventAna*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventAna*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaEventArray(void *p);
   static void deleteArray_NicaEventArray(void *p);
   static void destruct_NicaEventArray(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventArray*)
   {
      ::NicaEventArray *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventArray >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventArray", ::NicaEventArray::Class_Version(), "NicaMemoryMapManager.h", 24,
                  typeid(::NicaEventArray), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventArray::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventArray) );
      instance.SetDelete(&delete_NicaEventArray);
      instance.SetDeleteArray(&deleteArray_NicaEventArray);
      instance.SetDestructor(&destruct_NicaEventArray);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventArray*)
   {
      return GenerateInitInstanceLocal((::NicaEventArray*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventArray*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaMemoryMapManager(void *p = nullptr);
   static void *newArray_NicaMemoryMapManager(Long_t size, void *p);
   static void delete_NicaMemoryMapManager(void *p);
   static void deleteArray_NicaMemoryMapManager(void *p);
   static void destruct_NicaMemoryMapManager(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaMemoryMapManager*)
   {
      ::NicaMemoryMapManager *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaMemoryMapManager >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaMemoryMapManager", ::NicaMemoryMapManager::Class_Version(), "NicaMemoryMapManager.h", 56,
                  typeid(::NicaMemoryMapManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaMemoryMapManager::Dictionary, isa_proxy, 4,
                  sizeof(::NicaMemoryMapManager) );
      instance.SetNew(&new_NicaMemoryMapManager);
      instance.SetNewArray(&newArray_NicaMemoryMapManager);
      instance.SetDelete(&delete_NicaMemoryMapManager);
      instance.SetDeleteArray(&deleteArray_NicaMemoryMapManager);
      instance.SetDestructor(&destruct_NicaMemoryMapManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaMemoryMapManager*)
   {
      return GenerateInitInstanceLocal((::NicaMemoryMapManager*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaMemoryMapManager*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackAna(void *p = nullptr);
   static void *newArray_NicaTrackAna(Long_t size, void *p);
   static void delete_NicaTrackAna(void *p);
   static void deleteArray_NicaTrackAna(void *p);
   static void destruct_NicaTrackAna(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackAna*)
   {
      ::NicaTrackAna *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackAna >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackAna", ::NicaTrackAna::Class_Version(), "NicaTrackAna.h", 23,
                  typeid(::NicaTrackAna), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackAna::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackAna) );
      instance.SetNew(&new_NicaTrackAna);
      instance.SetNewArray(&newArray_NicaTrackAna);
      instance.SetDelete(&delete_NicaTrackAna);
      instance.SetDeleteArray(&deleteArray_NicaTrackAna);
      instance.SetDestructor(&destruct_NicaTrackAna);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackAna*)
   {
      return GenerateInitInstanceLocal((::NicaTrackAna*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackAna*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaMultiTrackAna(void *p = nullptr);
   static void *newArray_NicaMultiTrackAna(Long_t size, void *p);
   static void delete_NicaMultiTrackAna(void *p);
   static void deleteArray_NicaMultiTrackAna(void *p);
   static void destruct_NicaMultiTrackAna(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaMultiTrackAna*)
   {
      ::NicaMultiTrackAna *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaMultiTrackAna >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaMultiTrackAna", ::NicaMultiTrackAna::Class_Version(), "NicaMultiTrackAna.h", 18,
                  typeid(::NicaMultiTrackAna), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaMultiTrackAna::Dictionary, isa_proxy, 4,
                  sizeof(::NicaMultiTrackAna) );
      instance.SetNew(&new_NicaMultiTrackAna);
      instance.SetNewArray(&newArray_NicaMultiTrackAna);
      instance.SetDelete(&delete_NicaMultiTrackAna);
      instance.SetDeleteArray(&deleteArray_NicaMultiTrackAna);
      instance.SetDestructor(&destruct_NicaMultiTrackAna);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaMultiTrackAna*)
   {
      return GenerateInitInstanceLocal((::NicaMultiTrackAna*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaMultiTrackAna*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackAna(void *p = nullptr);
   static void *newArray_NicaTwoTrackAna(Long_t size, void *p);
   static void delete_NicaTwoTrackAna(void *p);
   static void deleteArray_NicaTwoTrackAna(void *p);
   static void destruct_NicaTwoTrackAna(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackAna*)
   {
      ::NicaTwoTrackAna *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackAna >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackAna", ::NicaTwoTrackAna::Class_Version(), "NicaTwoTrackAna.h", 27,
                  typeid(::NicaTwoTrackAna), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackAna::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackAna) );
      instance.SetNew(&new_NicaTwoTrackAna);
      instance.SetNewArray(&newArray_NicaTwoTrackAna);
      instance.SetDelete(&delete_NicaTwoTrackAna);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackAna);
      instance.SetDestructor(&destruct_NicaTwoTrackAna);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackAna*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackAna*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackAna*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventAnaChain(void *p = nullptr);
   static void *newArray_NicaEventAnaChain(Long_t size, void *p);
   static void delete_NicaEventAnaChain(void *p);
   static void deleteArray_NicaEventAnaChain(void *p);
   static void destruct_NicaEventAnaChain(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventAnaChain*)
   {
      ::NicaEventAnaChain *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventAnaChain >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventAnaChain", ::NicaEventAnaChain::Class_Version(), "NicaEventAnaChain.h", 19,
                  typeid(::NicaEventAnaChain), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventAnaChain::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventAnaChain) );
      instance.SetNew(&new_NicaEventAnaChain);
      instance.SetNewArray(&newArray_NicaEventAnaChain);
      instance.SetDelete(&delete_NicaEventAnaChain);
      instance.SetDeleteArray(&deleteArray_NicaEventAnaChain);
      instance.SetDestructor(&destruct_NicaEventAnaChain);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventAnaChain*)
   {
      return GenerateInitInstanceLocal((::NicaEventAnaChain*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventAnaChain*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackAnaChain(void *p = nullptr);
   static void *newArray_NicaTrackAnaChain(Long_t size, void *p);
   static void delete_NicaTrackAnaChain(void *p);
   static void deleteArray_NicaTrackAnaChain(void *p);
   static void destruct_NicaTrackAnaChain(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackAnaChain*)
   {
      ::NicaTrackAnaChain *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackAnaChain >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackAnaChain", ::NicaTrackAnaChain::Class_Version(), "NicaTrackAnaChain.h", 18,
                  typeid(::NicaTrackAnaChain), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackAnaChain::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackAnaChain) );
      instance.SetNew(&new_NicaTrackAnaChain);
      instance.SetNewArray(&newArray_NicaTrackAnaChain);
      instance.SetDelete(&delete_NicaTrackAnaChain);
      instance.SetDeleteArray(&deleteArray_NicaTrackAnaChain);
      instance.SetDestructor(&destruct_NicaTrackAnaChain);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackAnaChain*)
   {
      return GenerateInitInstanceLocal((::NicaTrackAnaChain*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackAnaChain*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrackAnaChain(void *p = nullptr);
   static void *newArray_NicaTwoTrackAnaChain(Long_t size, void *p);
   static void delete_NicaTwoTrackAnaChain(void *p);
   static void deleteArray_NicaTwoTrackAnaChain(void *p);
   static void destruct_NicaTwoTrackAnaChain(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrackAnaChain*)
   {
      ::NicaTwoTrackAnaChain *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrackAnaChain >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrackAnaChain", ::NicaTwoTrackAnaChain::Class_Version(), "NicaTwoTrackAnaChain.h", 19,
                  typeid(::NicaTwoTrackAnaChain), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrackAnaChain::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrackAnaChain) );
      instance.SetNew(&new_NicaTwoTrackAnaChain);
      instance.SetNewArray(&newArray_NicaTwoTrackAnaChain);
      instance.SetDelete(&delete_NicaTwoTrackAnaChain);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrackAnaChain);
      instance.SetDestructor(&destruct_NicaTwoTrackAnaChain);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrackAnaChain*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrackAnaChain*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrackAnaChain*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaSplitedTrackToStatusTask(void *p = nullptr);
   static void *newArray_NicaSplitedTrackToStatusTask(Long_t size, void *p);
   static void delete_NicaSplitedTrackToStatusTask(void *p);
   static void deleteArray_NicaSplitedTrackToStatusTask(void *p);
   static void destruct_NicaSplitedTrackToStatusTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaSplitedTrackToStatusTask*)
   {
      ::NicaSplitedTrackToStatusTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaSplitedTrackToStatusTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaSplitedTrackToStatusTask", ::NicaSplitedTrackToStatusTask::Class_Version(), "NicaSplitedTrackToStatusTask.h", 22,
                  typeid(::NicaSplitedTrackToStatusTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaSplitedTrackToStatusTask::Dictionary, isa_proxy, 4,
                  sizeof(::NicaSplitedTrackToStatusTask) );
      instance.SetNew(&new_NicaSplitedTrackToStatusTask);
      instance.SetNewArray(&newArray_NicaSplitedTrackToStatusTask);
      instance.SetDelete(&delete_NicaSplitedTrackToStatusTask);
      instance.SetDeleteArray(&deleteArray_NicaSplitedTrackToStatusTask);
      instance.SetDestructor(&destruct_NicaSplitedTrackToStatusTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaSplitedTrackToStatusTask*)
   {
      return GenerateInitInstanceLocal((::NicaSplitedTrackToStatusTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaSplitedTrackToStatusTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaBoostTask(void *p = nullptr);
   static void *newArray_NicaBoostTask(Long_t size, void *p);
   static void delete_NicaBoostTask(void *p);
   static void deleteArray_NicaBoostTask(void *p);
   static void destruct_NicaBoostTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaBoostTask*)
   {
      ::NicaBoostTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaBoostTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaBoostTask", ::NicaBoostTask::Class_Version(), "NicaBoostTask.h", 24,
                  typeid(::NicaBoostTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaBoostTask::Dictionary, isa_proxy, 4,
                  sizeof(::NicaBoostTask) );
      instance.SetNew(&new_NicaBoostTask);
      instance.SetNewArray(&newArray_NicaBoostTask);
      instance.SetDelete(&delete_NicaBoostTask);
      instance.SetDeleteArray(&deleteArray_NicaBoostTask);
      instance.SetDestructor(&destruct_NicaBoostTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaBoostTask*)
   {
      return GenerateInitInstanceLocal((::NicaBoostTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaBoostTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaMiniEventTask(void *p = nullptr);
   static void *newArray_NicaMiniEventTask(Long_t size, void *p);
   static void delete_NicaMiniEventTask(void *p);
   static void deleteArray_NicaMiniEventTask(void *p);
   static void destruct_NicaMiniEventTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaMiniEventTask*)
   {
      ::NicaMiniEventTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaMiniEventTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaMiniEventTask", ::NicaMiniEventTask::Class_Version(), "NicaMiniEventTask.h", 19,
                  typeid(::NicaMiniEventTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaMiniEventTask::Dictionary, isa_proxy, 4,
                  sizeof(::NicaMiniEventTask) );
      instance.SetNew(&new_NicaMiniEventTask);
      instance.SetNewArray(&newArray_NicaMiniEventTask);
      instance.SetDelete(&delete_NicaMiniEventTask);
      instance.SetDeleteArray(&deleteArray_NicaMiniEventTask);
      instance.SetDestructor(&destruct_NicaMiniEventTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaMiniEventTask*)
   {
      return GenerateInitInstanceLocal((::NicaMiniEventTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaMiniEventTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCopyDataTask(void *p = nullptr);
   static void *newArray_NicaCopyDataTask(Long_t size, void *p);
   static void delete_NicaCopyDataTask(void *p);
   static void deleteArray_NicaCopyDataTask(void *p);
   static void destruct_NicaCopyDataTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCopyDataTask*)
   {
      ::NicaCopyDataTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCopyDataTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCopyDataTask", ::NicaCopyDataTask::Class_Version(), "NicaCopyDataTask.h", 19,
                  typeid(::NicaCopyDataTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCopyDataTask::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCopyDataTask) );
      instance.SetNew(&new_NicaCopyDataTask);
      instance.SetNewArray(&newArray_NicaCopyDataTask);
      instance.SetDelete(&delete_NicaCopyDataTask);
      instance.SetDeleteArray(&deleteArray_NicaCopyDataTask);
      instance.SetDestructor(&destruct_NicaCopyDataTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCopyDataTask*)
   {
      return GenerateInitInstanceLocal((::NicaCopyDataTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCopyDataTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaSmearAlgorithm(void *p);
   static void deleteArray_NicaSmearAlgorithm(void *p);
   static void destruct_NicaSmearAlgorithm(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaSmearAlgorithm*)
   {
      ::NicaSmearAlgorithm *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaSmearAlgorithm >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaSmearAlgorithm", ::NicaSmearAlgorithm::Class_Version(), "NicaSmearAlgorithm.h", 24,
                  typeid(::NicaSmearAlgorithm), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaSmearAlgorithm::Dictionary, isa_proxy, 4,
                  sizeof(::NicaSmearAlgorithm) );
      instance.SetDelete(&delete_NicaSmearAlgorithm);
      instance.SetDeleteArray(&deleteArray_NicaSmearAlgorithm);
      instance.SetDestructor(&destruct_NicaSmearAlgorithm);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaSmearAlgorithm*)
   {
      return GenerateInitInstanceLocal((::NicaSmearAlgorithm*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaSmearAlgorithm*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaEventSmear(void *p);
   static void deleteArray_NicaEventSmear(void *p);
   static void destruct_NicaEventSmear(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventSmear*)
   {
      ::NicaEventSmear *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventSmear >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventSmear", ::NicaEventSmear::Class_Version(), "NicaSmearAlgorithm.h", 51,
                  typeid(::NicaEventSmear), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventSmear::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventSmear) );
      instance.SetDelete(&delete_NicaEventSmear);
      instance.SetDeleteArray(&deleteArray_NicaEventSmear);
      instance.SetDestructor(&destruct_NicaEventSmear);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventSmear*)
   {
      return GenerateInitInstanceLocal((::NicaEventSmear*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventSmear*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaEventSmearVirtual(void *p = nullptr);
   static void *newArray_NicaEventSmearVirtual(Long_t size, void *p);
   static void delete_NicaEventSmearVirtual(void *p);
   static void deleteArray_NicaEventSmearVirtual(void *p);
   static void destruct_NicaEventSmearVirtual(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventSmearVirtual*)
   {
      ::NicaEventSmearVirtual *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventSmearVirtual >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventSmearVirtual", ::NicaEventSmearVirtual::Class_Version(), "NicaSmearAlgorithm.h", 67,
                  typeid(::NicaEventSmearVirtual), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventSmearVirtual::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventSmearVirtual) );
      instance.SetNew(&new_NicaEventSmearVirtual);
      instance.SetNewArray(&newArray_NicaEventSmearVirtual);
      instance.SetDelete(&delete_NicaEventSmearVirtual);
      instance.SetDeleteArray(&deleteArray_NicaEventSmearVirtual);
      instance.SetDestructor(&destruct_NicaEventSmearVirtual);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventSmearVirtual*)
   {
      return GenerateInitInstanceLocal((::NicaEventSmearVirtual*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventSmearVirtual*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaTrackSmear(void *p);
   static void deleteArray_NicaTrackSmear(void *p);
   static void destruct_NicaTrackSmear(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackSmear*)
   {
      ::NicaTrackSmear *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackSmear >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackSmear", ::NicaTrackSmear::Class_Version(), "NicaSmearAlgorithm.h", 81,
                  typeid(::NicaTrackSmear), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackSmear::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackSmear) );
      instance.SetDelete(&delete_NicaTrackSmear);
      instance.SetDeleteArray(&deleteArray_NicaTrackSmear);
      instance.SetDestructor(&destruct_NicaTrackSmear);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackSmear*)
   {
      return GenerateInitInstanceLocal((::NicaTrackSmear*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackSmear*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackSmearVirtual(void *p = nullptr);
   static void *newArray_NicaTrackSmearVirtual(Long_t size, void *p);
   static void delete_NicaTrackSmearVirtual(void *p);
   static void deleteArray_NicaTrackSmearVirtual(void *p);
   static void destruct_NicaTrackSmearVirtual(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackSmearVirtual*)
   {
      ::NicaTrackSmearVirtual *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackSmearVirtual >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackSmearVirtual", ::NicaTrackSmearVirtual::Class_Version(), "NicaSmearAlgorithm.h", 96,
                  typeid(::NicaTrackSmearVirtual), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackSmearVirtual::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackSmearVirtual) );
      instance.SetNew(&new_NicaTrackSmearVirtual);
      instance.SetNewArray(&newArray_NicaTrackSmearVirtual);
      instance.SetDelete(&delete_NicaTrackSmearVirtual);
      instance.SetDeleteArray(&deleteArray_NicaTrackSmearVirtual);
      instance.SetDestructor(&destruct_NicaTrackSmearVirtual);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackSmearVirtual*)
   {
      return GenerateInitInstanceLocal((::NicaTrackSmearVirtual*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackSmearVirtual*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaSmearTask(void *p = nullptr);
   static void *newArray_NicaSmearTask(Long_t size, void *p);
   static void delete_NicaSmearTask(void *p);
   static void deleteArray_NicaSmearTask(void *p);
   static void destruct_NicaSmearTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaSmearTask*)
   {
      ::NicaSmearTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaSmearTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaSmearTask", ::NicaSmearTask::Class_Version(), "NicaSmearTask.h", 24,
                  typeid(::NicaSmearTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaSmearTask::Dictionary, isa_proxy, 4,
                  sizeof(::NicaSmearTask) );
      instance.SetNew(&new_NicaSmearTask);
      instance.SetNewArray(&newArray_NicaSmearTask);
      instance.SetDelete(&delete_NicaSmearTask);
      instance.SetDeleteArray(&deleteArray_NicaSmearTask);
      instance.SetDestructor(&destruct_NicaSmearTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaSmearTask*)
   {
      return GenerateInitInstanceLocal((::NicaSmearTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaSmearTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackSmearMomentumPercent(void *p = nullptr);
   static void *newArray_NicaTrackSmearMomentumPercent(Long_t size, void *p);
   static void delete_NicaTrackSmearMomentumPercent(void *p);
   static void deleteArray_NicaTrackSmearMomentumPercent(void *p);
   static void destruct_NicaTrackSmearMomentumPercent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackSmearMomentumPercent*)
   {
      ::NicaTrackSmearMomentumPercent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackSmearMomentumPercent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackSmearMomentumPercent", ::NicaTrackSmearMomentumPercent::Class_Version(), "NicaTrackSmearMomentumPercent.h", 19,
                  typeid(::NicaTrackSmearMomentumPercent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackSmearMomentumPercent::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackSmearMomentumPercent) );
      instance.SetNew(&new_NicaTrackSmearMomentumPercent);
      instance.SetNewArray(&newArray_NicaTrackSmearMomentumPercent);
      instance.SetDelete(&delete_NicaTrackSmearMomentumPercent);
      instance.SetDeleteArray(&deleteArray_NicaTrackSmearMomentumPercent);
      instance.SetDestructor(&destruct_NicaTrackSmearMomentumPercent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackSmearMomentumPercent*)
   {
      return GenerateInitInstanceLocal((::NicaTrackSmearMomentumPercent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackSmearMomentumPercent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NicaEventAna::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventAna::Class_Name()
{
   return "NicaEventAna";
}

//______________________________________________________________________________
const char *NicaEventAna::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventAna*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventAna::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventAna*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventAna::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventAna*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventAna::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventAna*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventArray::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventArray::Class_Name()
{
   return "NicaEventArray";
}

//______________________________________________________________________________
const char *NicaEventArray::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventArray*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventArray::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventArray*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventArray::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventArray*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventArray::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventArray*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaMemoryMapManager::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaMemoryMapManager::Class_Name()
{
   return "NicaMemoryMapManager";
}

//______________________________________________________________________________
const char *NicaMemoryMapManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMemoryMapManager*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaMemoryMapManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMemoryMapManager*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaMemoryMapManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMemoryMapManager*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaMemoryMapManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMemoryMapManager*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackAna::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackAna::Class_Name()
{
   return "NicaTrackAna";
}

//______________________________________________________________________________
const char *NicaTrackAna::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackAna*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackAna::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackAna*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackAna::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackAna*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackAna::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackAna*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaMultiTrackAna::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaMultiTrackAna::Class_Name()
{
   return "NicaMultiTrackAna";
}

//______________________________________________________________________________
const char *NicaMultiTrackAna::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMultiTrackAna*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaMultiTrackAna::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMultiTrackAna*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaMultiTrackAna::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMultiTrackAna*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaMultiTrackAna::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMultiTrackAna*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackAna::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackAna::Class_Name()
{
   return "NicaTwoTrackAna";
}

//______________________________________________________________________________
const char *NicaTwoTrackAna::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackAna*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackAna::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackAna*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackAna::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackAna*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackAna::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackAna*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventAnaChain::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventAnaChain::Class_Name()
{
   return "NicaEventAnaChain";
}

//______________________________________________________________________________
const char *NicaEventAnaChain::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventAnaChain*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventAnaChain::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventAnaChain*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventAnaChain::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventAnaChain*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventAnaChain::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventAnaChain*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackAnaChain::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackAnaChain::Class_Name()
{
   return "NicaTrackAnaChain";
}

//______________________________________________________________________________
const char *NicaTrackAnaChain::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackAnaChain*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackAnaChain::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackAnaChain*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackAnaChain::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackAnaChain*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackAnaChain::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackAnaChain*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrackAnaChain::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrackAnaChain::Class_Name()
{
   return "NicaTwoTrackAnaChain";
}

//______________________________________________________________________________
const char *NicaTwoTrackAnaChain::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackAnaChain*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrackAnaChain::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackAnaChain*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrackAnaChain::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackAnaChain*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrackAnaChain::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrackAnaChain*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaSplitedTrackToStatusTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaSplitedTrackToStatusTask::Class_Name()
{
   return "NicaSplitedTrackToStatusTask";
}

//______________________________________________________________________________
const char *NicaSplitedTrackToStatusTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSplitedTrackToStatusTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaSplitedTrackToStatusTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSplitedTrackToStatusTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaSplitedTrackToStatusTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSplitedTrackToStatusTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaSplitedTrackToStatusTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSplitedTrackToStatusTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaBoostTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaBoostTask::Class_Name()
{
   return "NicaBoostTask";
}

//______________________________________________________________________________
const char *NicaBoostTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaBoostTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaBoostTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaBoostTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaBoostTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaBoostTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaBoostTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaBoostTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaMiniEventTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaMiniEventTask::Class_Name()
{
   return "NicaMiniEventTask";
}

//______________________________________________________________________________
const char *NicaMiniEventTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMiniEventTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaMiniEventTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMiniEventTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaMiniEventTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMiniEventTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaMiniEventTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMiniEventTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCopyDataTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCopyDataTask::Class_Name()
{
   return "NicaCopyDataTask";
}

//______________________________________________________________________________
const char *NicaCopyDataTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCopyDataTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCopyDataTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCopyDataTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCopyDataTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCopyDataTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCopyDataTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCopyDataTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaSmearAlgorithm::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaSmearAlgorithm::Class_Name()
{
   return "NicaSmearAlgorithm";
}

//______________________________________________________________________________
const char *NicaSmearAlgorithm::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearAlgorithm*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaSmearAlgorithm::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearAlgorithm*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaSmearAlgorithm::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearAlgorithm*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaSmearAlgorithm::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearAlgorithm*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventSmear::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventSmear::Class_Name()
{
   return "NicaEventSmear";
}

//______________________________________________________________________________
const char *NicaEventSmear::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventSmear*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventSmear::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventSmear*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventSmear::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventSmear*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventSmear::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventSmear*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventSmearVirtual::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventSmearVirtual::Class_Name()
{
   return "NicaEventSmearVirtual";
}

//______________________________________________________________________________
const char *NicaEventSmearVirtual::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventSmearVirtual*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventSmearVirtual::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventSmearVirtual*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventSmearVirtual::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventSmearVirtual*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventSmearVirtual::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventSmearVirtual*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackSmear::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackSmear::Class_Name()
{
   return "NicaTrackSmear";
}

//______________________________________________________________________________
const char *NicaTrackSmear::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackSmear*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackSmear::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackSmear*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackSmear::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackSmear*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackSmear::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackSmear*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackSmearVirtual::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackSmearVirtual::Class_Name()
{
   return "NicaTrackSmearVirtual";
}

//______________________________________________________________________________
const char *NicaTrackSmearVirtual::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackSmearVirtual*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackSmearVirtual::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackSmearVirtual*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackSmearVirtual::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackSmearVirtual*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackSmearVirtual::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackSmearVirtual*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaSmearTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaSmearTask::Class_Name()
{
   return "NicaSmearTask";
}

//______________________________________________________________________________
const char *NicaSmearTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaSmearTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaSmearTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaSmearTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackSmearMomentumPercent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackSmearMomentumPercent::Class_Name()
{
   return "NicaTrackSmearMomentumPercent";
}

//______________________________________________________________________________
const char *NicaTrackSmearMomentumPercent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackSmearMomentumPercent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackSmearMomentumPercent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackSmearMomentumPercent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackSmearMomentumPercent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackSmearMomentumPercent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackSmearMomentumPercent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackSmearMomentumPercent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void NicaEventAna::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventAna.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventAna::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventAna::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventAna(void *p) {
      return  p ? new(p) ::NicaEventAna : new ::NicaEventAna;
   }
   static void *newArray_NicaEventAna(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventAna[nElements] : new ::NicaEventAna[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventAna(void *p) {
      delete ((::NicaEventAna*)p);
   }
   static void deleteArray_NicaEventAna(void *p) {
      delete [] ((::NicaEventAna*)p);
   }
   static void destruct_NicaEventAna(void *p) {
      typedef ::NicaEventAna current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventAna

//______________________________________________________________________________
void NicaEventArray::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventArray.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventArray::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventArray::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaEventArray(void *p) {
      delete ((::NicaEventArray*)p);
   }
   static void deleteArray_NicaEventArray(void *p) {
      delete [] ((::NicaEventArray*)p);
   }
   static void destruct_NicaEventArray(void *p) {
      typedef ::NicaEventArray current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventArray

//______________________________________________________________________________
void NicaMemoryMapManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaMemoryMapManager.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaMemoryMapManager::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaMemoryMapManager::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaMemoryMapManager(void *p) {
      return  p ? new(p) ::NicaMemoryMapManager : new ::NicaMemoryMapManager;
   }
   static void *newArray_NicaMemoryMapManager(Long_t nElements, void *p) {
      return p ? new(p) ::NicaMemoryMapManager[nElements] : new ::NicaMemoryMapManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaMemoryMapManager(void *p) {
      delete ((::NicaMemoryMapManager*)p);
   }
   static void deleteArray_NicaMemoryMapManager(void *p) {
      delete [] ((::NicaMemoryMapManager*)p);
   }
   static void destruct_NicaMemoryMapManager(void *p) {
      typedef ::NicaMemoryMapManager current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaMemoryMapManager

//______________________________________________________________________________
void NicaTrackAna::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackAna.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackAna::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackAna::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackAna(void *p) {
      return  p ? new(p) ::NicaTrackAna : new ::NicaTrackAna;
   }
   static void *newArray_NicaTrackAna(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackAna[nElements] : new ::NicaTrackAna[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackAna(void *p) {
      delete ((::NicaTrackAna*)p);
   }
   static void deleteArray_NicaTrackAna(void *p) {
      delete [] ((::NicaTrackAna*)p);
   }
   static void destruct_NicaTrackAna(void *p) {
      typedef ::NicaTrackAna current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackAna

//______________________________________________________________________________
void NicaMultiTrackAna::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaMultiTrackAna.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaMultiTrackAna::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaMultiTrackAna::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaMultiTrackAna(void *p) {
      return  p ? new(p) ::NicaMultiTrackAna : new ::NicaMultiTrackAna;
   }
   static void *newArray_NicaMultiTrackAna(Long_t nElements, void *p) {
      return p ? new(p) ::NicaMultiTrackAna[nElements] : new ::NicaMultiTrackAna[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaMultiTrackAna(void *p) {
      delete ((::NicaMultiTrackAna*)p);
   }
   static void deleteArray_NicaMultiTrackAna(void *p) {
      delete [] ((::NicaMultiTrackAna*)p);
   }
   static void destruct_NicaMultiTrackAna(void *p) {
      typedef ::NicaMultiTrackAna current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaMultiTrackAna

//______________________________________________________________________________
void NicaTwoTrackAna::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackAna.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackAna::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackAna::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackAna(void *p) {
      return  p ? new(p) ::NicaTwoTrackAna : new ::NicaTwoTrackAna;
   }
   static void *newArray_NicaTwoTrackAna(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackAna[nElements] : new ::NicaTwoTrackAna[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackAna(void *p) {
      delete ((::NicaTwoTrackAna*)p);
   }
   static void deleteArray_NicaTwoTrackAna(void *p) {
      delete [] ((::NicaTwoTrackAna*)p);
   }
   static void destruct_NicaTwoTrackAna(void *p) {
      typedef ::NicaTwoTrackAna current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackAna

//______________________________________________________________________________
void NicaEventAnaChain::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventAnaChain.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventAnaChain::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventAnaChain::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventAnaChain(void *p) {
      return  p ? new(p) ::NicaEventAnaChain : new ::NicaEventAnaChain;
   }
   static void *newArray_NicaEventAnaChain(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventAnaChain[nElements] : new ::NicaEventAnaChain[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventAnaChain(void *p) {
      delete ((::NicaEventAnaChain*)p);
   }
   static void deleteArray_NicaEventAnaChain(void *p) {
      delete [] ((::NicaEventAnaChain*)p);
   }
   static void destruct_NicaEventAnaChain(void *p) {
      typedef ::NicaEventAnaChain current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventAnaChain

//______________________________________________________________________________
void NicaTrackAnaChain::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackAnaChain.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackAnaChain::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackAnaChain::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackAnaChain(void *p) {
      return  p ? new(p) ::NicaTrackAnaChain : new ::NicaTrackAnaChain;
   }
   static void *newArray_NicaTrackAnaChain(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackAnaChain[nElements] : new ::NicaTrackAnaChain[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackAnaChain(void *p) {
      delete ((::NicaTrackAnaChain*)p);
   }
   static void deleteArray_NicaTrackAnaChain(void *p) {
      delete [] ((::NicaTrackAnaChain*)p);
   }
   static void destruct_NicaTrackAnaChain(void *p) {
      typedef ::NicaTrackAnaChain current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackAnaChain

//______________________________________________________________________________
void NicaTwoTrackAnaChain::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrackAnaChain.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrackAnaChain::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrackAnaChain::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrackAnaChain(void *p) {
      return  p ? new(p) ::NicaTwoTrackAnaChain : new ::NicaTwoTrackAnaChain;
   }
   static void *newArray_NicaTwoTrackAnaChain(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrackAnaChain[nElements] : new ::NicaTwoTrackAnaChain[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrackAnaChain(void *p) {
      delete ((::NicaTwoTrackAnaChain*)p);
   }
   static void deleteArray_NicaTwoTrackAnaChain(void *p) {
      delete [] ((::NicaTwoTrackAnaChain*)p);
   }
   static void destruct_NicaTwoTrackAnaChain(void *p) {
      typedef ::NicaTwoTrackAnaChain current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrackAnaChain

//______________________________________________________________________________
void NicaSplitedTrackToStatusTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaSplitedTrackToStatusTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaSplitedTrackToStatusTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaSplitedTrackToStatusTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaSplitedTrackToStatusTask(void *p) {
      return  p ? new(p) ::NicaSplitedTrackToStatusTask : new ::NicaSplitedTrackToStatusTask;
   }
   static void *newArray_NicaSplitedTrackToStatusTask(Long_t nElements, void *p) {
      return p ? new(p) ::NicaSplitedTrackToStatusTask[nElements] : new ::NicaSplitedTrackToStatusTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaSplitedTrackToStatusTask(void *p) {
      delete ((::NicaSplitedTrackToStatusTask*)p);
   }
   static void deleteArray_NicaSplitedTrackToStatusTask(void *p) {
      delete [] ((::NicaSplitedTrackToStatusTask*)p);
   }
   static void destruct_NicaSplitedTrackToStatusTask(void *p) {
      typedef ::NicaSplitedTrackToStatusTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaSplitedTrackToStatusTask

//______________________________________________________________________________
void NicaBoostTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaBoostTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaBoostTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaBoostTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaBoostTask(void *p) {
      return  p ? new(p) ::NicaBoostTask : new ::NicaBoostTask;
   }
   static void *newArray_NicaBoostTask(Long_t nElements, void *p) {
      return p ? new(p) ::NicaBoostTask[nElements] : new ::NicaBoostTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaBoostTask(void *p) {
      delete ((::NicaBoostTask*)p);
   }
   static void deleteArray_NicaBoostTask(void *p) {
      delete [] ((::NicaBoostTask*)p);
   }
   static void destruct_NicaBoostTask(void *p) {
      typedef ::NicaBoostTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaBoostTask

//______________________________________________________________________________
void NicaMiniEventTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaMiniEventTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaMiniEventTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaMiniEventTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaMiniEventTask(void *p) {
      return  p ? new(p) ::NicaMiniEventTask : new ::NicaMiniEventTask;
   }
   static void *newArray_NicaMiniEventTask(Long_t nElements, void *p) {
      return p ? new(p) ::NicaMiniEventTask[nElements] : new ::NicaMiniEventTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaMiniEventTask(void *p) {
      delete ((::NicaMiniEventTask*)p);
   }
   static void deleteArray_NicaMiniEventTask(void *p) {
      delete [] ((::NicaMiniEventTask*)p);
   }
   static void destruct_NicaMiniEventTask(void *p) {
      typedef ::NicaMiniEventTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaMiniEventTask

//______________________________________________________________________________
void NicaCopyDataTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCopyDataTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCopyDataTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCopyDataTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCopyDataTask(void *p) {
      return  p ? new(p) ::NicaCopyDataTask : new ::NicaCopyDataTask;
   }
   static void *newArray_NicaCopyDataTask(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCopyDataTask[nElements] : new ::NicaCopyDataTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCopyDataTask(void *p) {
      delete ((::NicaCopyDataTask*)p);
   }
   static void deleteArray_NicaCopyDataTask(void *p) {
      delete [] ((::NicaCopyDataTask*)p);
   }
   static void destruct_NicaCopyDataTask(void *p) {
      typedef ::NicaCopyDataTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCopyDataTask

//______________________________________________________________________________
void NicaSmearAlgorithm::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaSmearAlgorithm.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaSmearAlgorithm::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaSmearAlgorithm::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaSmearAlgorithm(void *p) {
      delete ((::NicaSmearAlgorithm*)p);
   }
   static void deleteArray_NicaSmearAlgorithm(void *p) {
      delete [] ((::NicaSmearAlgorithm*)p);
   }
   static void destruct_NicaSmearAlgorithm(void *p) {
      typedef ::NicaSmearAlgorithm current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaSmearAlgorithm

//______________________________________________________________________________
void NicaEventSmear::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventSmear.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventSmear::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventSmear::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaEventSmear(void *p) {
      delete ((::NicaEventSmear*)p);
   }
   static void deleteArray_NicaEventSmear(void *p) {
      delete [] ((::NicaEventSmear*)p);
   }
   static void destruct_NicaEventSmear(void *p) {
      typedef ::NicaEventSmear current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventSmear

//______________________________________________________________________________
void NicaEventSmearVirtual::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventSmearVirtual.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventSmearVirtual::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventSmearVirtual::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaEventSmearVirtual(void *p) {
      return  p ? new(p) ::NicaEventSmearVirtual : new ::NicaEventSmearVirtual;
   }
   static void *newArray_NicaEventSmearVirtual(Long_t nElements, void *p) {
      return p ? new(p) ::NicaEventSmearVirtual[nElements] : new ::NicaEventSmearVirtual[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaEventSmearVirtual(void *p) {
      delete ((::NicaEventSmearVirtual*)p);
   }
   static void deleteArray_NicaEventSmearVirtual(void *p) {
      delete [] ((::NicaEventSmearVirtual*)p);
   }
   static void destruct_NicaEventSmearVirtual(void *p) {
      typedef ::NicaEventSmearVirtual current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventSmearVirtual

//______________________________________________________________________________
void NicaTrackSmear::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackSmear.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackSmear::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackSmear::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaTrackSmear(void *p) {
      delete ((::NicaTrackSmear*)p);
   }
   static void deleteArray_NicaTrackSmear(void *p) {
      delete [] ((::NicaTrackSmear*)p);
   }
   static void destruct_NicaTrackSmear(void *p) {
      typedef ::NicaTrackSmear current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackSmear

//______________________________________________________________________________
void NicaTrackSmearVirtual::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackSmearVirtual.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackSmearVirtual::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackSmearVirtual::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackSmearVirtual(void *p) {
      return  p ? new(p) ::NicaTrackSmearVirtual : new ::NicaTrackSmearVirtual;
   }
   static void *newArray_NicaTrackSmearVirtual(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackSmearVirtual[nElements] : new ::NicaTrackSmearVirtual[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackSmearVirtual(void *p) {
      delete ((::NicaTrackSmearVirtual*)p);
   }
   static void deleteArray_NicaTrackSmearVirtual(void *p) {
      delete [] ((::NicaTrackSmearVirtual*)p);
   }
   static void destruct_NicaTrackSmearVirtual(void *p) {
      typedef ::NicaTrackSmearVirtual current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackSmearVirtual

//______________________________________________________________________________
void NicaSmearTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaSmearTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaSmearTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaSmearTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaSmearTask(void *p) {
      return  p ? new(p) ::NicaSmearTask : new ::NicaSmearTask;
   }
   static void *newArray_NicaSmearTask(Long_t nElements, void *p) {
      return p ? new(p) ::NicaSmearTask[nElements] : new ::NicaSmearTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaSmearTask(void *p) {
      delete ((::NicaSmearTask*)p);
   }
   static void deleteArray_NicaSmearTask(void *p) {
      delete [] ((::NicaSmearTask*)p);
   }
   static void destruct_NicaSmearTask(void *p) {
      typedef ::NicaSmearTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaSmearTask

//______________________________________________________________________________
void NicaTrackSmearMomentumPercent::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackSmearMomentumPercent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackSmearMomentumPercent::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackSmearMomentumPercent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackSmearMomentumPercent(void *p) {
      return  p ? new(p) ::NicaTrackSmearMomentumPercent : new ::NicaTrackSmearMomentumPercent;
   }
   static void *newArray_NicaTrackSmearMomentumPercent(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackSmearMomentumPercent[nElements] : new ::NicaTrackSmearMomentumPercent[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackSmearMomentumPercent(void *p) {
      delete ((::NicaTrackSmearMomentumPercent*)p);
   }
   static void deleteArray_NicaTrackSmearMomentumPercent(void *p) {
      delete [] ((::NicaTrackSmearMomentumPercent*)p);
   }
   static void destruct_NicaTrackSmearMomentumPercent(void *p) {
      typedef ::NicaTrackSmearMomentumPercent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackSmearMomentumPercent

namespace {
  void TriggerDictionaryInitialization_G__NicaAnaDict_Impl() {
    static const char* headers[] = {
"NicaEventAna.h",
"NicaMemoryMapManager.h",
"NicaMultiTrackAna.h",
"NicaTrackAna.h",
"NicaTwoTrackAna.h",
"NicaEventAnaChain.h",
"NicaTrackAnaChain.h",
"NicaTwoTrackAnaChain.h",
"NicaSplitedTrackToStatusTask.h",
"NicaBoostTask.h",
"NicaMiniEventTask.h",
"NicaCopyDataTask.h",
"NicaSmearTask.h",
"NicaSmearAlgorithm.h",
"NicaTrackSmearMomentumPercent.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/chains",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly/smear_codes",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/hist",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io/html",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/std",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/minimizer",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/eventcuts",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/detector",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/mc",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/resolution",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/detector",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/mc",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/cutmonitors",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/detector",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/compound",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/hiddeninfo",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/analysis/base/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__NicaAnaDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$NicaEventAna.h")))  NicaEventAna;
class __attribute__((annotate("$clingAutoload$NicaMemoryMapManager.h")))  NicaEventArray;
class __attribute__((annotate("$clingAutoload$NicaMemoryMapManager.h")))  NicaMemoryMapManager;
class __attribute__((annotate("$clingAutoload$NicaTrackAna.h")))  __attribute__((annotate("$clingAutoload$NicaMultiTrackAna.h")))  NicaTrackAna;
class __attribute__((annotate("$clingAutoload$NicaMultiTrackAna.h")))  NicaMultiTrackAna;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackAna.h")))  NicaTwoTrackAna;
class __attribute__((annotate("$clingAutoload$NicaEventAnaChain.h")))  NicaEventAnaChain;
class __attribute__((annotate("$clingAutoload$NicaTrackAnaChain.h")))  NicaTrackAnaChain;
class __attribute__((annotate("$clingAutoload$NicaTwoTrackAnaChain.h")))  NicaTwoTrackAnaChain;
class __attribute__((annotate("$clingAutoload$NicaSplitedTrackToStatusTask.h")))  NicaSplitedTrackToStatusTask;
class __attribute__((annotate("$clingAutoload$NicaBoostTask.h")))  NicaBoostTask;
class __attribute__((annotate("$clingAutoload$NicaMiniEventTask.h")))  NicaMiniEventTask;
class __attribute__((annotate("$clingAutoload$NicaCopyDataTask.h")))  NicaCopyDataTask;
class __attribute__((annotate("$clingAutoload$NicaSmearAlgorithm.h")))  __attribute__((annotate("$clingAutoload$NicaSmearTask.h")))  NicaSmearAlgorithm;
class __attribute__((annotate("$clingAutoload$NicaSmearAlgorithm.h")))  __attribute__((annotate("$clingAutoload$NicaSmearTask.h")))  NicaEventSmear;
class __attribute__((annotate("$clingAutoload$NicaSmearAlgorithm.h")))  __attribute__((annotate("$clingAutoload$NicaSmearTask.h")))  NicaEventSmearVirtual;
class __attribute__((annotate("$clingAutoload$NicaSmearAlgorithm.h")))  __attribute__((annotate("$clingAutoload$NicaSmearTask.h")))  NicaTrackSmear;
class __attribute__((annotate("$clingAutoload$NicaSmearAlgorithm.h")))  __attribute__((annotate("$clingAutoload$NicaSmearTask.h")))  NicaTrackSmearVirtual;
class __attribute__((annotate("$clingAutoload$NicaSmearTask.h")))  NicaSmearTask;
class __attribute__((annotate("$clingAutoload$NicaTrackSmearMomentumPercent.h")))  NicaTrackSmearMomentumPercent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__NicaAnaDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "NicaEventAna.h"
#include "NicaMemoryMapManager.h"
#include "NicaMultiTrackAna.h"
#include "NicaTrackAna.h"
#include "NicaTwoTrackAna.h"
#include "NicaEventAnaChain.h"
#include "NicaTrackAnaChain.h"
#include "NicaTwoTrackAnaChain.h"
#include "NicaSplitedTrackToStatusTask.h"
#include "NicaBoostTask.h"
#include "NicaMiniEventTask.h"
#include "NicaCopyDataTask.h"
#include "NicaSmearTask.h"
#include "NicaSmearAlgorithm.h"
#include "NicaTrackSmearMomentumPercent.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"NicaBoostTask", payloadCode, "@",
"NicaCopyDataTask", payloadCode, "@",
"NicaEventAna", payloadCode, "@",
"NicaEventAnaChain", payloadCode, "@",
"NicaEventArray", payloadCode, "@",
"NicaEventSmear", payloadCode, "@",
"NicaEventSmearVirtual", payloadCode, "@",
"NicaMemoryMapManager", payloadCode, "@",
"NicaMiniEventTask", payloadCode, "@",
"NicaMultiTrackAna", payloadCode, "@",
"NicaSmearAlgorithm", payloadCode, "@",
"NicaSmearTask", payloadCode, "@",
"NicaSplitedTrackToStatusTask", payloadCode, "@",
"NicaTrackAna", payloadCode, "@",
"NicaTrackAnaChain", payloadCode, "@",
"NicaTrackSmear", payloadCode, "@",
"NicaTrackSmearMomentumPercent", payloadCode, "@",
"NicaTrackSmearVirtual", payloadCode, "@",
"NicaTwoTrackAna", payloadCode, "@",
"NicaTwoTrackAnaChain", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__NicaAnaDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__NicaAnaDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__NicaAnaDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__NicaAnaDict() {
  TriggerDictionaryInitialization_G__NicaAnaDict_Impl();
}
