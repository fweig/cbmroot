// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__NicaCbmFormat
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
#include "NicaCbmFormatTypes.h"
#include "NicaCbmFullEvent.h"
#include "CbmHbtFullEvent.h"
#include "NicaGeneratorReadUnigen.h"
#include "NicaGeneratorWriteUnigen.h"
#include "NicaUnigenSource.h"
#include "NicaUnigenEvent.h"
#include "NicaUnigenEventInterface.h"
#include "NicaUnigenTrack.h"
#include "NicaUnigenTrackInterface.h"
#include "NicaCbmMCEvent.h"
#include "NicaCbmMCEventInterface.h"
#include "NicaCbmMCTrack.h"
#include "NicaCbmMCTrackInterface.h"
#include "NicaCbmEvent.h"
#include "NicaCbmEventInterface.h"
#include "NicaCbmTrack.h"
#include "NicaCbmTrackInterface.h"
#include "CbmHbtTrack.h"
#include "CbmHbtEvent.h"
#include "CbmAnaTreeContainer.h"
#include "CbmAnaTreeSource.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace NicaCbmFormats {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *NicaCbmFormats_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("NicaCbmFormats", 0 /*version*/, "NicaCbmFormatTypes.h", 18,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &NicaCbmFormats_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *NicaCbmFormats_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_NicaCbmFullEvent(void *p = nullptr);
   static void *newArray_NicaCbmFullEvent(Long_t size, void *p);
   static void delete_NicaCbmFullEvent(void *p);
   static void deleteArray_NicaCbmFullEvent(void *p);
   static void destruct_NicaCbmFullEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCbmFullEvent*)
   {
      ::NicaCbmFullEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCbmFullEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCbmFullEvent", ::NicaCbmFullEvent::Class_Version(), "NicaCbmFullEvent.h", 21,
                  typeid(::NicaCbmFullEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCbmFullEvent::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCbmFullEvent) );
      instance.SetNew(&new_NicaCbmFullEvent);
      instance.SetNewArray(&newArray_NicaCbmFullEvent);
      instance.SetDelete(&delete_NicaCbmFullEvent);
      instance.SetDeleteArray(&deleteArray_NicaCbmFullEvent);
      instance.SetDestructor(&destruct_NicaCbmFullEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCbmFullEvent*)
   {
      return GenerateInitInstanceLocal((::NicaCbmFullEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCbmFullEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmHbtFullEvent(void *p = nullptr);
   static void *newArray_CbmHbtFullEvent(Long_t size, void *p);
   static void delete_CbmHbtFullEvent(void *p);
   static void deleteArray_CbmHbtFullEvent(void *p);
   static void destruct_CbmHbtFullEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmHbtFullEvent*)
   {
      ::CbmHbtFullEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmHbtFullEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmHbtFullEvent", ::CbmHbtFullEvent::Class_Version(), "CbmHbtFullEvent.h", 22,
                  typeid(::CbmHbtFullEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmHbtFullEvent::Dictionary, isa_proxy, 4,
                  sizeof(::CbmHbtFullEvent) );
      instance.SetNew(&new_CbmHbtFullEvent);
      instance.SetNewArray(&newArray_CbmHbtFullEvent);
      instance.SetDelete(&delete_CbmHbtFullEvent);
      instance.SetDeleteArray(&deleteArray_CbmHbtFullEvent);
      instance.SetDestructor(&destruct_CbmHbtFullEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmHbtFullEvent*)
   {
      return GenerateInitInstanceLocal((::CbmHbtFullEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmHbtFullEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaGeneratorReadUnigen(void *p);
   static void deleteArray_NicaGeneratorReadUnigen(void *p);
   static void destruct_NicaGeneratorReadUnigen(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaGeneratorReadUnigen*)
   {
      ::NicaGeneratorReadUnigen *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaGeneratorReadUnigen >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaGeneratorReadUnigen", ::NicaGeneratorReadUnigen::Class_Version(), "NicaGeneratorReadUnigen.h", 27,
                  typeid(::NicaGeneratorReadUnigen), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaGeneratorReadUnigen::Dictionary, isa_proxy, 4,
                  sizeof(::NicaGeneratorReadUnigen) );
      instance.SetDelete(&delete_NicaGeneratorReadUnigen);
      instance.SetDeleteArray(&deleteArray_NicaGeneratorReadUnigen);
      instance.SetDestructor(&destruct_NicaGeneratorReadUnigen);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaGeneratorReadUnigen*)
   {
      return GenerateInitInstanceLocal((::NicaGeneratorReadUnigen*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaGeneratorReadUnigen*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaGeneratorWriteUnigen(void *p = nullptr);
   static void *newArray_NicaGeneratorWriteUnigen(Long_t size, void *p);
   static void delete_NicaGeneratorWriteUnigen(void *p);
   static void deleteArray_NicaGeneratorWriteUnigen(void *p);
   static void destruct_NicaGeneratorWriteUnigen(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaGeneratorWriteUnigen*)
   {
      ::NicaGeneratorWriteUnigen *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaGeneratorWriteUnigen >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaGeneratorWriteUnigen", ::NicaGeneratorWriteUnigen::Class_Version(), "NicaGeneratorWriteUnigen.h", 22,
                  typeid(::NicaGeneratorWriteUnigen), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaGeneratorWriteUnigen::Dictionary, isa_proxy, 4,
                  sizeof(::NicaGeneratorWriteUnigen) );
      instance.SetNew(&new_NicaGeneratorWriteUnigen);
      instance.SetNewArray(&newArray_NicaGeneratorWriteUnigen);
      instance.SetDelete(&delete_NicaGeneratorWriteUnigen);
      instance.SetDeleteArray(&deleteArray_NicaGeneratorWriteUnigen);
      instance.SetDestructor(&destruct_NicaGeneratorWriteUnigen);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaGeneratorWriteUnigen*)
   {
      return GenerateInitInstanceLocal((::NicaGeneratorWriteUnigen*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaGeneratorWriteUnigen*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaUnigenSource(void *p);
   static void deleteArray_NicaUnigenSource(void *p);
   static void destruct_NicaUnigenSource(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaUnigenSource*)
   {
      ::NicaUnigenSource *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaUnigenSource >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaUnigenSource", ::NicaUnigenSource::Class_Version(), "NicaUnigenSource.h", 26,
                  typeid(::NicaUnigenSource), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaUnigenSource::Dictionary, isa_proxy, 4,
                  sizeof(::NicaUnigenSource) );
      instance.SetDelete(&delete_NicaUnigenSource);
      instance.SetDeleteArray(&deleteArray_NicaUnigenSource);
      instance.SetDestructor(&destruct_NicaUnigenSource);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaUnigenSource*)
   {
      return GenerateInitInstanceLocal((::NicaUnigenSource*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaUnigenSource*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaUnigenTrack(void *p = nullptr);
   static void *newArray_NicaUnigenTrack(Long_t size, void *p);
   static void delete_NicaUnigenTrack(void *p);
   static void deleteArray_NicaUnigenTrack(void *p);
   static void destruct_NicaUnigenTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaUnigenTrack*)
   {
      ::NicaUnigenTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaUnigenTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaUnigenTrack", ::NicaUnigenTrack::Class_Version(), "NicaUnigenTrack.h", 25,
                  typeid(::NicaUnigenTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaUnigenTrack::Dictionary, isa_proxy, 4,
                  sizeof(::NicaUnigenTrack) );
      instance.SetNew(&new_NicaUnigenTrack);
      instance.SetNewArray(&newArray_NicaUnigenTrack);
      instance.SetDelete(&delete_NicaUnigenTrack);
      instance.SetDeleteArray(&deleteArray_NicaUnigenTrack);
      instance.SetDestructor(&destruct_NicaUnigenTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaUnigenTrack*)
   {
      return GenerateInitInstanceLocal((::NicaUnigenTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaUnigenTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaUnigenEvent(void *p = nullptr);
   static void *newArray_NicaUnigenEvent(Long_t size, void *p);
   static void delete_NicaUnigenEvent(void *p);
   static void deleteArray_NicaUnigenEvent(void *p);
   static void destruct_NicaUnigenEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaUnigenEvent*)
   {
      ::NicaUnigenEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaUnigenEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaUnigenEvent", ::NicaUnigenEvent::Class_Version(), "NicaUnigenEvent.h", 24,
                  typeid(::NicaUnigenEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaUnigenEvent::Dictionary, isa_proxy, 4,
                  sizeof(::NicaUnigenEvent) );
      instance.SetNew(&new_NicaUnigenEvent);
      instance.SetNewArray(&newArray_NicaUnigenEvent);
      instance.SetDelete(&delete_NicaUnigenEvent);
      instance.SetDeleteArray(&deleteArray_NicaUnigenEvent);
      instance.SetDestructor(&destruct_NicaUnigenEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaUnigenEvent*)
   {
      return GenerateInitInstanceLocal((::NicaUnigenEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaUnigenEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaUnigenEventInterface(void *p = nullptr);
   static void *newArray_NicaUnigenEventInterface(Long_t size, void *p);
   static void delete_NicaUnigenEventInterface(void *p);
   static void deleteArray_NicaUnigenEventInterface(void *p);
   static void destruct_NicaUnigenEventInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaUnigenEventInterface*)
   {
      ::NicaUnigenEventInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaUnigenEventInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaUnigenEventInterface", ::NicaUnigenEventInterface::Class_Version(), "NicaUnigenEventInterface.h", 25,
                  typeid(::NicaUnigenEventInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaUnigenEventInterface::Dictionary, isa_proxy, 4,
                  sizeof(::NicaUnigenEventInterface) );
      instance.SetNew(&new_NicaUnigenEventInterface);
      instance.SetNewArray(&newArray_NicaUnigenEventInterface);
      instance.SetDelete(&delete_NicaUnigenEventInterface);
      instance.SetDeleteArray(&deleteArray_NicaUnigenEventInterface);
      instance.SetDestructor(&destruct_NicaUnigenEventInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaUnigenEventInterface*)
   {
      return GenerateInitInstanceLocal((::NicaUnigenEventInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaUnigenEventInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaUnigenTrackInterface(void *p = nullptr);
   static void *newArray_NicaUnigenTrackInterface(Long_t size, void *p);
   static void delete_NicaUnigenTrackInterface(void *p);
   static void deleteArray_NicaUnigenTrackInterface(void *p);
   static void destruct_NicaUnigenTrackInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaUnigenTrackInterface*)
   {
      ::NicaUnigenTrackInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaUnigenTrackInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaUnigenTrackInterface", ::NicaUnigenTrackInterface::Class_Version(), "NicaUnigenTrackInterface.h", 21,
                  typeid(::NicaUnigenTrackInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaUnigenTrackInterface::Dictionary, isa_proxy, 4,
                  sizeof(::NicaUnigenTrackInterface) );
      instance.SetNew(&new_NicaUnigenTrackInterface);
      instance.SetNewArray(&newArray_NicaUnigenTrackInterface);
      instance.SetDelete(&delete_NicaUnigenTrackInterface);
      instance.SetDeleteArray(&deleteArray_NicaUnigenTrackInterface);
      instance.SetDestructor(&destruct_NicaUnigenTrackInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaUnigenTrackInterface*)
   {
      return GenerateInitInstanceLocal((::NicaUnigenTrackInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaUnigenTrackInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCbmMCEvent(void *p = nullptr);
   static void *newArray_NicaCbmMCEvent(Long_t size, void *p);
   static void delete_NicaCbmMCEvent(void *p);
   static void deleteArray_NicaCbmMCEvent(void *p);
   static void destruct_NicaCbmMCEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCbmMCEvent*)
   {
      ::NicaCbmMCEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCbmMCEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCbmMCEvent", ::NicaCbmMCEvent::Class_Version(), "NicaCbmMCEvent.h", 18,
                  typeid(::NicaCbmMCEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCbmMCEvent::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCbmMCEvent) );
      instance.SetNew(&new_NicaCbmMCEvent);
      instance.SetNewArray(&newArray_NicaCbmMCEvent);
      instance.SetDelete(&delete_NicaCbmMCEvent);
      instance.SetDeleteArray(&deleteArray_NicaCbmMCEvent);
      instance.SetDestructor(&destruct_NicaCbmMCEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCbmMCEvent*)
   {
      return GenerateInitInstanceLocal((::NicaCbmMCEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCbmMCEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaTreeRecoSourceContainer(void *p = nullptr);
   static void *newArray_CbmAnaTreeRecoSourceContainer(Long_t size, void *p);
   static void delete_CbmAnaTreeRecoSourceContainer(void *p);
   static void deleteArray_CbmAnaTreeRecoSourceContainer(void *p);
   static void destruct_CbmAnaTreeRecoSourceContainer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaTreeRecoSourceContainer*)
   {
      ::CbmAnaTreeRecoSourceContainer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaTreeRecoSourceContainer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaTreeRecoSourceContainer", ::CbmAnaTreeRecoSourceContainer::Class_Version(), "CbmAnaTreeContainer.h", 33,
                  typeid(::CbmAnaTreeRecoSourceContainer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaTreeRecoSourceContainer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaTreeRecoSourceContainer) );
      instance.SetNew(&new_CbmAnaTreeRecoSourceContainer);
      instance.SetNewArray(&newArray_CbmAnaTreeRecoSourceContainer);
      instance.SetDelete(&delete_CbmAnaTreeRecoSourceContainer);
      instance.SetDeleteArray(&deleteArray_CbmAnaTreeRecoSourceContainer);
      instance.SetDestructor(&destruct_CbmAnaTreeRecoSourceContainer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaTreeRecoSourceContainer*)
   {
      return GenerateInitInstanceLocal((::CbmAnaTreeRecoSourceContainer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaTreeRecoSourceContainer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaTreeMcSourceContainer(void *p = nullptr);
   static void *newArray_CbmAnaTreeMcSourceContainer(Long_t size, void *p);
   static void delete_CbmAnaTreeMcSourceContainer(void *p);
   static void deleteArray_CbmAnaTreeMcSourceContainer(void *p);
   static void destruct_CbmAnaTreeMcSourceContainer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaTreeMcSourceContainer*)
   {
      ::CbmAnaTreeMcSourceContainer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaTreeMcSourceContainer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaTreeMcSourceContainer", ::CbmAnaTreeMcSourceContainer::Class_Version(), "CbmAnaTreeContainer.h", 76,
                  typeid(::CbmAnaTreeMcSourceContainer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaTreeMcSourceContainer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaTreeMcSourceContainer) );
      instance.SetNew(&new_CbmAnaTreeMcSourceContainer);
      instance.SetNewArray(&newArray_CbmAnaTreeMcSourceContainer);
      instance.SetDelete(&delete_CbmAnaTreeMcSourceContainer);
      instance.SetDeleteArray(&deleteArray_CbmAnaTreeMcSourceContainer);
      instance.SetDestructor(&destruct_CbmAnaTreeMcSourceContainer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaTreeMcSourceContainer*)
   {
      return GenerateInitInstanceLocal((::CbmAnaTreeMcSourceContainer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaTreeMcSourceContainer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCbmMCTrackInterface(void *p = nullptr);
   static void *newArray_NicaCbmMCTrackInterface(Long_t size, void *p);
   static void delete_NicaCbmMCTrackInterface(void *p);
   static void deleteArray_NicaCbmMCTrackInterface(void *p);
   static void destruct_NicaCbmMCTrackInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCbmMCTrackInterface*)
   {
      ::NicaCbmMCTrackInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCbmMCTrackInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCbmMCTrackInterface", ::NicaCbmMCTrackInterface::Class_Version(), "NicaCbmMCTrackInterface.h", 18,
                  typeid(::NicaCbmMCTrackInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCbmMCTrackInterface::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCbmMCTrackInterface) );
      instance.SetNew(&new_NicaCbmMCTrackInterface);
      instance.SetNewArray(&newArray_NicaCbmMCTrackInterface);
      instance.SetDelete(&delete_NicaCbmMCTrackInterface);
      instance.SetDeleteArray(&deleteArray_NicaCbmMCTrackInterface);
      instance.SetDestructor(&destruct_NicaCbmMCTrackInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCbmMCTrackInterface*)
   {
      return GenerateInitInstanceLocal((::NicaCbmMCTrackInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCbmMCTrackInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCbmMCEventInterface(void *p = nullptr);
   static void *newArray_NicaCbmMCEventInterface(Long_t size, void *p);
   static void delete_NicaCbmMCEventInterface(void *p);
   static void deleteArray_NicaCbmMCEventInterface(void *p);
   static void destruct_NicaCbmMCEventInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCbmMCEventInterface*)
   {
      ::NicaCbmMCEventInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCbmMCEventInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCbmMCEventInterface", ::NicaCbmMCEventInterface::Class_Version(), "NicaCbmMCEventInterface.h", 24,
                  typeid(::NicaCbmMCEventInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCbmMCEventInterface::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCbmMCEventInterface) );
      instance.SetNew(&new_NicaCbmMCEventInterface);
      instance.SetNewArray(&newArray_NicaCbmMCEventInterface);
      instance.SetDelete(&delete_NicaCbmMCEventInterface);
      instance.SetDeleteArray(&deleteArray_NicaCbmMCEventInterface);
      instance.SetDestructor(&destruct_NicaCbmMCEventInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCbmMCEventInterface*)
   {
      return GenerateInitInstanceLocal((::NicaCbmMCEventInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCbmMCEventInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCbmMCTrack(void *p = nullptr);
   static void *newArray_NicaCbmMCTrack(Long_t size, void *p);
   static void delete_NicaCbmMCTrack(void *p);
   static void deleteArray_NicaCbmMCTrack(void *p);
   static void destruct_NicaCbmMCTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCbmMCTrack*)
   {
      ::NicaCbmMCTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCbmMCTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCbmMCTrack", ::NicaCbmMCTrack::Class_Version(), "NicaCbmMCTrack.h", 17,
                  typeid(::NicaCbmMCTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCbmMCTrack::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCbmMCTrack) );
      instance.SetNew(&new_NicaCbmMCTrack);
      instance.SetNewArray(&newArray_NicaCbmMCTrack);
      instance.SetDelete(&delete_NicaCbmMCTrack);
      instance.SetDeleteArray(&deleteArray_NicaCbmMCTrack);
      instance.SetDestructor(&destruct_NicaCbmMCTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCbmMCTrack*)
   {
      return GenerateInitInstanceLocal((::NicaCbmMCTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCbmMCTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCbmTrackInterface(void *p = nullptr);
   static void *newArray_NicaCbmTrackInterface(Long_t size, void *p);
   static void delete_NicaCbmTrackInterface(void *p);
   static void deleteArray_NicaCbmTrackInterface(void *p);
   static void destruct_NicaCbmTrackInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCbmTrackInterface*)
   {
      ::NicaCbmTrackInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCbmTrackInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCbmTrackInterface", ::NicaCbmTrackInterface::Class_Version(), "NicaCbmTrackInterface.h", 19,
                  typeid(::NicaCbmTrackInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCbmTrackInterface::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCbmTrackInterface) );
      instance.SetNew(&new_NicaCbmTrackInterface);
      instance.SetNewArray(&newArray_NicaCbmTrackInterface);
      instance.SetDelete(&delete_NicaCbmTrackInterface);
      instance.SetDeleteArray(&deleteArray_NicaCbmTrackInterface);
      instance.SetDestructor(&destruct_NicaCbmTrackInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCbmTrackInterface*)
   {
      return GenerateInitInstanceLocal((::NicaCbmTrackInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCbmTrackInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCbmEventInterface(void *p = nullptr);
   static void *newArray_NicaCbmEventInterface(Long_t size, void *p);
   static void delete_NicaCbmEventInterface(void *p);
   static void deleteArray_NicaCbmEventInterface(void *p);
   static void destruct_NicaCbmEventInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCbmEventInterface*)
   {
      ::NicaCbmEventInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCbmEventInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCbmEventInterface", ::NicaCbmEventInterface::Class_Version(), "NicaCbmEventInterface.h", 25,
                  typeid(::NicaCbmEventInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCbmEventInterface::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCbmEventInterface) );
      instance.SetNew(&new_NicaCbmEventInterface);
      instance.SetNewArray(&newArray_NicaCbmEventInterface);
      instance.SetDelete(&delete_NicaCbmEventInterface);
      instance.SetDeleteArray(&deleteArray_NicaCbmEventInterface);
      instance.SetDestructor(&destruct_NicaCbmEventInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCbmEventInterface*)
   {
      return GenerateInitInstanceLocal((::NicaCbmEventInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCbmEventInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCbmTrack(void *p = nullptr);
   static void *newArray_NicaCbmTrack(Long_t size, void *p);
   static void delete_NicaCbmTrack(void *p);
   static void deleteArray_NicaCbmTrack(void *p);
   static void destruct_NicaCbmTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCbmTrack*)
   {
      ::NicaCbmTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCbmTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCbmTrack", ::NicaCbmTrack::Class_Version(), "NicaCbmTrack.h", 27,
                  typeid(::NicaCbmTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCbmTrack::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCbmTrack) );
      instance.SetNew(&new_NicaCbmTrack);
      instance.SetNewArray(&newArray_NicaCbmTrack);
      instance.SetDelete(&delete_NicaCbmTrack);
      instance.SetDeleteArray(&deleteArray_NicaCbmTrack);
      instance.SetDestructor(&destruct_NicaCbmTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCbmTrack*)
   {
      return GenerateInitInstanceLocal((::NicaCbmTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCbmTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCbmEvent(void *p = nullptr);
   static void *newArray_NicaCbmEvent(Long_t size, void *p);
   static void delete_NicaCbmEvent(void *p);
   static void deleteArray_NicaCbmEvent(void *p);
   static void destruct_NicaCbmEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCbmEvent*)
   {
      ::NicaCbmEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCbmEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCbmEvent", ::NicaCbmEvent::Class_Version(), "NicaCbmEvent.h", 19,
                  typeid(::NicaCbmEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCbmEvent::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCbmEvent) );
      instance.SetNew(&new_NicaCbmEvent);
      instance.SetNewArray(&newArray_NicaCbmEvent);
      instance.SetDelete(&delete_NicaCbmEvent);
      instance.SetDeleteArray(&deleteArray_NicaCbmEvent);
      instance.SetDestructor(&destruct_NicaCbmEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCbmEvent*)
   {
      return GenerateInitInstanceLocal((::NicaCbmEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCbmEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmHbtTrack(void *p = nullptr);
   static void *newArray_CbmHbtTrack(Long_t size, void *p);
   static void delete_CbmHbtTrack(void *p);
   static void deleteArray_CbmHbtTrack(void *p);
   static void destruct_CbmHbtTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmHbtTrack*)
   {
      ::CbmHbtTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmHbtTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmHbtTrack", ::CbmHbtTrack::Class_Version(), "CbmHbtTrack.h", 23,
                  typeid(::CbmHbtTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmHbtTrack::Dictionary, isa_proxy, 4,
                  sizeof(::CbmHbtTrack) );
      instance.SetNew(&new_CbmHbtTrack);
      instance.SetNewArray(&newArray_CbmHbtTrack);
      instance.SetDelete(&delete_CbmHbtTrack);
      instance.SetDeleteArray(&deleteArray_CbmHbtTrack);
      instance.SetDestructor(&destruct_CbmHbtTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmHbtTrack*)
   {
      return GenerateInitInstanceLocal((::CbmHbtTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmHbtTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmHbtEvent(void *p = nullptr);
   static void *newArray_CbmHbtEvent(Long_t size, void *p);
   static void delete_CbmHbtEvent(void *p);
   static void deleteArray_CbmHbtEvent(void *p);
   static void destruct_CbmHbtEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmHbtEvent*)
   {
      ::CbmHbtEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmHbtEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmHbtEvent", ::CbmHbtEvent::Class_Version(), "CbmHbtEvent.h", 21,
                  typeid(::CbmHbtEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmHbtEvent::Dictionary, isa_proxy, 4,
                  sizeof(::CbmHbtEvent) );
      instance.SetNew(&new_CbmHbtEvent);
      instance.SetNewArray(&newArray_CbmHbtEvent);
      instance.SetDelete(&delete_CbmHbtEvent);
      instance.SetDeleteArray(&deleteArray_CbmHbtEvent);
      instance.SetDestructor(&destruct_CbmHbtEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmHbtEvent*)
   {
      return GenerateInitInstanceLocal((::CbmHbtEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmHbtEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmAnaTreeSource(void *p = nullptr);
   static void *newArray_CbmAnaTreeSource(Long_t size, void *p);
   static void delete_CbmAnaTreeSource(void *p);
   static void deleteArray_CbmAnaTreeSource(void *p);
   static void destruct_CbmAnaTreeSource(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAnaTreeSource*)
   {
      ::CbmAnaTreeSource *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmAnaTreeSource >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmAnaTreeSource", ::CbmAnaTreeSource::Class_Version(), "CbmAnaTreeSource.h", 28,
                  typeid(::CbmAnaTreeSource), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmAnaTreeSource::Dictionary, isa_proxy, 4,
                  sizeof(::CbmAnaTreeSource) );
      instance.SetNew(&new_CbmAnaTreeSource);
      instance.SetNewArray(&newArray_CbmAnaTreeSource);
      instance.SetDelete(&delete_CbmAnaTreeSource);
      instance.SetDeleteArray(&deleteArray_CbmAnaTreeSource);
      instance.SetDestructor(&destruct_CbmAnaTreeSource);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAnaTreeSource*)
   {
      return GenerateInitInstanceLocal((::CbmAnaTreeSource*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAnaTreeSource*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NicaCbmFullEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCbmFullEvent::Class_Name()
{
   return "NicaCbmFullEvent";
}

//______________________________________________________________________________
const char *NicaCbmFullEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmFullEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCbmFullEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmFullEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCbmFullEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmFullEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCbmFullEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmFullEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmHbtFullEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmHbtFullEvent::Class_Name()
{
   return "CbmHbtFullEvent";
}

//______________________________________________________________________________
const char *CbmHbtFullEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHbtFullEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmHbtFullEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHbtFullEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmHbtFullEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHbtFullEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmHbtFullEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHbtFullEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaGeneratorReadUnigen::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaGeneratorReadUnigen::Class_Name()
{
   return "NicaGeneratorReadUnigen";
}

//______________________________________________________________________________
const char *NicaGeneratorReadUnigen::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadUnigen*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaGeneratorReadUnigen::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadUnigen*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaGeneratorReadUnigen::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadUnigen*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaGeneratorReadUnigen::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadUnigen*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaGeneratorWriteUnigen::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaGeneratorWriteUnigen::Class_Name()
{
   return "NicaGeneratorWriteUnigen";
}

//______________________________________________________________________________
const char *NicaGeneratorWriteUnigen::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorWriteUnigen*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaGeneratorWriteUnigen::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorWriteUnigen*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaGeneratorWriteUnigen::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorWriteUnigen*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaGeneratorWriteUnigen::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorWriteUnigen*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaUnigenSource::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaUnigenSource::Class_Name()
{
   return "NicaUnigenSource";
}

//______________________________________________________________________________
const char *NicaUnigenSource::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenSource*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaUnigenSource::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenSource*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaUnigenSource::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenSource*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaUnigenSource::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenSource*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaUnigenTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaUnigenTrack::Class_Name()
{
   return "NicaUnigenTrack";
}

//______________________________________________________________________________
const char *NicaUnigenTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaUnigenTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaUnigenTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaUnigenTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaUnigenEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaUnigenEvent::Class_Name()
{
   return "NicaUnigenEvent";
}

//______________________________________________________________________________
const char *NicaUnigenEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaUnigenEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaUnigenEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaUnigenEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaUnigenEventInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaUnigenEventInterface::Class_Name()
{
   return "NicaUnigenEventInterface";
}

//______________________________________________________________________________
const char *NicaUnigenEventInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenEventInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaUnigenEventInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenEventInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaUnigenEventInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenEventInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaUnigenEventInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenEventInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaUnigenTrackInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaUnigenTrackInterface::Class_Name()
{
   return "NicaUnigenTrackInterface";
}

//______________________________________________________________________________
const char *NicaUnigenTrackInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenTrackInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaUnigenTrackInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenTrackInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaUnigenTrackInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenTrackInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaUnigenTrackInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaUnigenTrackInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCbmMCEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCbmMCEvent::Class_Name()
{
   return "NicaCbmMCEvent";
}

//______________________________________________________________________________
const char *NicaCbmMCEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCbmMCEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCbmMCEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCbmMCEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaTreeRecoSourceContainer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaTreeRecoSourceContainer::Class_Name()
{
   return "CbmAnaTreeRecoSourceContainer";
}

//______________________________________________________________________________
const char *CbmAnaTreeRecoSourceContainer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaTreeRecoSourceContainer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaTreeRecoSourceContainer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaTreeRecoSourceContainer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaTreeRecoSourceContainer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaTreeRecoSourceContainer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaTreeRecoSourceContainer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaTreeRecoSourceContainer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaTreeMcSourceContainer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaTreeMcSourceContainer::Class_Name()
{
   return "CbmAnaTreeMcSourceContainer";
}

//______________________________________________________________________________
const char *CbmAnaTreeMcSourceContainer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaTreeMcSourceContainer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaTreeMcSourceContainer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaTreeMcSourceContainer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaTreeMcSourceContainer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaTreeMcSourceContainer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaTreeMcSourceContainer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaTreeMcSourceContainer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCbmMCTrackInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCbmMCTrackInterface::Class_Name()
{
   return "NicaCbmMCTrackInterface";
}

//______________________________________________________________________________
const char *NicaCbmMCTrackInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCTrackInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCbmMCTrackInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCTrackInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCbmMCTrackInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCTrackInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCbmMCTrackInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCTrackInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCbmMCEventInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCbmMCEventInterface::Class_Name()
{
   return "NicaCbmMCEventInterface";
}

//______________________________________________________________________________
const char *NicaCbmMCEventInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCEventInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCbmMCEventInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCEventInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCbmMCEventInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCEventInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCbmMCEventInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCEventInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCbmMCTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCbmMCTrack::Class_Name()
{
   return "NicaCbmMCTrack";
}

//______________________________________________________________________________
const char *NicaCbmMCTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCbmMCTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCbmMCTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCbmMCTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmMCTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCbmTrackInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCbmTrackInterface::Class_Name()
{
   return "NicaCbmTrackInterface";
}

//______________________________________________________________________________
const char *NicaCbmTrackInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmTrackInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCbmTrackInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmTrackInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCbmTrackInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmTrackInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCbmTrackInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmTrackInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCbmEventInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCbmEventInterface::Class_Name()
{
   return "NicaCbmEventInterface";
}

//______________________________________________________________________________
const char *NicaCbmEventInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmEventInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCbmEventInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmEventInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCbmEventInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmEventInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCbmEventInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmEventInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCbmTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCbmTrack::Class_Name()
{
   return "NicaCbmTrack";
}

//______________________________________________________________________________
const char *NicaCbmTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCbmTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCbmTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCbmTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCbmEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCbmEvent::Class_Name()
{
   return "NicaCbmEvent";
}

//______________________________________________________________________________
const char *NicaCbmEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCbmEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCbmEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCbmEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCbmEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmHbtTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmHbtTrack::Class_Name()
{
   return "CbmHbtTrack";
}

//______________________________________________________________________________
const char *CbmHbtTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHbtTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmHbtTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHbtTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmHbtTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHbtTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmHbtTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHbtTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmHbtEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmHbtEvent::Class_Name()
{
   return "CbmHbtEvent";
}

//______________________________________________________________________________
const char *CbmHbtEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHbtEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmHbtEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHbtEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmHbtEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHbtEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmHbtEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHbtEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmAnaTreeSource::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmAnaTreeSource::Class_Name()
{
   return "CbmAnaTreeSource";
}

//______________________________________________________________________________
const char *CbmAnaTreeSource::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaTreeSource*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmAnaTreeSource::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaTreeSource*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmAnaTreeSource::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaTreeSource*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmAnaTreeSource::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmAnaTreeSource*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void NicaCbmFullEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCbmFullEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCbmFullEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCbmFullEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCbmFullEvent(void *p) {
      return  p ? new(p) ::NicaCbmFullEvent : new ::NicaCbmFullEvent;
   }
   static void *newArray_NicaCbmFullEvent(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCbmFullEvent[nElements] : new ::NicaCbmFullEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCbmFullEvent(void *p) {
      delete ((::NicaCbmFullEvent*)p);
   }
   static void deleteArray_NicaCbmFullEvent(void *p) {
      delete [] ((::NicaCbmFullEvent*)p);
   }
   static void destruct_NicaCbmFullEvent(void *p) {
      typedef ::NicaCbmFullEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCbmFullEvent

//______________________________________________________________________________
void CbmHbtFullEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmHbtFullEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmHbtFullEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmHbtFullEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmHbtFullEvent(void *p) {
      return  p ? new(p) ::CbmHbtFullEvent : new ::CbmHbtFullEvent;
   }
   static void *newArray_CbmHbtFullEvent(Long_t nElements, void *p) {
      return p ? new(p) ::CbmHbtFullEvent[nElements] : new ::CbmHbtFullEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmHbtFullEvent(void *p) {
      delete ((::CbmHbtFullEvent*)p);
   }
   static void deleteArray_CbmHbtFullEvent(void *p) {
      delete [] ((::CbmHbtFullEvent*)p);
   }
   static void destruct_CbmHbtFullEvent(void *p) {
      typedef ::CbmHbtFullEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmHbtFullEvent

//______________________________________________________________________________
void NicaGeneratorReadUnigen::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaGeneratorReadUnigen.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaGeneratorReadUnigen::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaGeneratorReadUnigen::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaGeneratorReadUnigen(void *p) {
      delete ((::NicaGeneratorReadUnigen*)p);
   }
   static void deleteArray_NicaGeneratorReadUnigen(void *p) {
      delete [] ((::NicaGeneratorReadUnigen*)p);
   }
   static void destruct_NicaGeneratorReadUnigen(void *p) {
      typedef ::NicaGeneratorReadUnigen current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaGeneratorReadUnigen

//______________________________________________________________________________
void NicaGeneratorWriteUnigen::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaGeneratorWriteUnigen.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaGeneratorWriteUnigen::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaGeneratorWriteUnigen::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaGeneratorWriteUnigen(void *p) {
      return  p ? new(p) ::NicaGeneratorWriteUnigen : new ::NicaGeneratorWriteUnigen;
   }
   static void *newArray_NicaGeneratorWriteUnigen(Long_t nElements, void *p) {
      return p ? new(p) ::NicaGeneratorWriteUnigen[nElements] : new ::NicaGeneratorWriteUnigen[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaGeneratorWriteUnigen(void *p) {
      delete ((::NicaGeneratorWriteUnigen*)p);
   }
   static void deleteArray_NicaGeneratorWriteUnigen(void *p) {
      delete [] ((::NicaGeneratorWriteUnigen*)p);
   }
   static void destruct_NicaGeneratorWriteUnigen(void *p) {
      typedef ::NicaGeneratorWriteUnigen current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaGeneratorWriteUnigen

//______________________________________________________________________________
void NicaUnigenSource::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaUnigenSource.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaUnigenSource::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaUnigenSource::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaUnigenSource(void *p) {
      delete ((::NicaUnigenSource*)p);
   }
   static void deleteArray_NicaUnigenSource(void *p) {
      delete [] ((::NicaUnigenSource*)p);
   }
   static void destruct_NicaUnigenSource(void *p) {
      typedef ::NicaUnigenSource current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaUnigenSource

//______________________________________________________________________________
void NicaUnigenTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaUnigenTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaUnigenTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaUnigenTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaUnigenTrack(void *p) {
      return  p ? new(p) ::NicaUnigenTrack : new ::NicaUnigenTrack;
   }
   static void *newArray_NicaUnigenTrack(Long_t nElements, void *p) {
      return p ? new(p) ::NicaUnigenTrack[nElements] : new ::NicaUnigenTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaUnigenTrack(void *p) {
      delete ((::NicaUnigenTrack*)p);
   }
   static void deleteArray_NicaUnigenTrack(void *p) {
      delete [] ((::NicaUnigenTrack*)p);
   }
   static void destruct_NicaUnigenTrack(void *p) {
      typedef ::NicaUnigenTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaUnigenTrack

//______________________________________________________________________________
void NicaUnigenEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaUnigenEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaUnigenEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaUnigenEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaUnigenEvent(void *p) {
      return  p ? new(p) ::NicaUnigenEvent : new ::NicaUnigenEvent;
   }
   static void *newArray_NicaUnigenEvent(Long_t nElements, void *p) {
      return p ? new(p) ::NicaUnigenEvent[nElements] : new ::NicaUnigenEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaUnigenEvent(void *p) {
      delete ((::NicaUnigenEvent*)p);
   }
   static void deleteArray_NicaUnigenEvent(void *p) {
      delete [] ((::NicaUnigenEvent*)p);
   }
   static void destruct_NicaUnigenEvent(void *p) {
      typedef ::NicaUnigenEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaUnigenEvent

//______________________________________________________________________________
void NicaUnigenEventInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaUnigenEventInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaUnigenEventInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaUnigenEventInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaUnigenEventInterface(void *p) {
      return  p ? new(p) ::NicaUnigenEventInterface : new ::NicaUnigenEventInterface;
   }
   static void *newArray_NicaUnigenEventInterface(Long_t nElements, void *p) {
      return p ? new(p) ::NicaUnigenEventInterface[nElements] : new ::NicaUnigenEventInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaUnigenEventInterface(void *p) {
      delete ((::NicaUnigenEventInterface*)p);
   }
   static void deleteArray_NicaUnigenEventInterface(void *p) {
      delete [] ((::NicaUnigenEventInterface*)p);
   }
   static void destruct_NicaUnigenEventInterface(void *p) {
      typedef ::NicaUnigenEventInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaUnigenEventInterface

//______________________________________________________________________________
void NicaUnigenTrackInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaUnigenTrackInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaUnigenTrackInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaUnigenTrackInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaUnigenTrackInterface(void *p) {
      return  p ? new(p) ::NicaUnigenTrackInterface : new ::NicaUnigenTrackInterface;
   }
   static void *newArray_NicaUnigenTrackInterface(Long_t nElements, void *p) {
      return p ? new(p) ::NicaUnigenTrackInterface[nElements] : new ::NicaUnigenTrackInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaUnigenTrackInterface(void *p) {
      delete ((::NicaUnigenTrackInterface*)p);
   }
   static void deleteArray_NicaUnigenTrackInterface(void *p) {
      delete [] ((::NicaUnigenTrackInterface*)p);
   }
   static void destruct_NicaUnigenTrackInterface(void *p) {
      typedef ::NicaUnigenTrackInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaUnigenTrackInterface

//______________________________________________________________________________
void NicaCbmMCEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCbmMCEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCbmMCEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCbmMCEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCbmMCEvent(void *p) {
      return  p ? new(p) ::NicaCbmMCEvent : new ::NicaCbmMCEvent;
   }
   static void *newArray_NicaCbmMCEvent(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCbmMCEvent[nElements] : new ::NicaCbmMCEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCbmMCEvent(void *p) {
      delete ((::NicaCbmMCEvent*)p);
   }
   static void deleteArray_NicaCbmMCEvent(void *p) {
      delete [] ((::NicaCbmMCEvent*)p);
   }
   static void destruct_NicaCbmMCEvent(void *p) {
      typedef ::NicaCbmMCEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCbmMCEvent

//______________________________________________________________________________
void CbmAnaTreeRecoSourceContainer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaTreeRecoSourceContainer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaTreeRecoSourceContainer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaTreeRecoSourceContainer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaTreeRecoSourceContainer(void *p) {
      return  p ? new(p) ::CbmAnaTreeRecoSourceContainer : new ::CbmAnaTreeRecoSourceContainer;
   }
   static void *newArray_CbmAnaTreeRecoSourceContainer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaTreeRecoSourceContainer[nElements] : new ::CbmAnaTreeRecoSourceContainer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaTreeRecoSourceContainer(void *p) {
      delete ((::CbmAnaTreeRecoSourceContainer*)p);
   }
   static void deleteArray_CbmAnaTreeRecoSourceContainer(void *p) {
      delete [] ((::CbmAnaTreeRecoSourceContainer*)p);
   }
   static void destruct_CbmAnaTreeRecoSourceContainer(void *p) {
      typedef ::CbmAnaTreeRecoSourceContainer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaTreeRecoSourceContainer

//______________________________________________________________________________
void CbmAnaTreeMcSourceContainer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaTreeMcSourceContainer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaTreeMcSourceContainer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaTreeMcSourceContainer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaTreeMcSourceContainer(void *p) {
      return  p ? new(p) ::CbmAnaTreeMcSourceContainer : new ::CbmAnaTreeMcSourceContainer;
   }
   static void *newArray_CbmAnaTreeMcSourceContainer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaTreeMcSourceContainer[nElements] : new ::CbmAnaTreeMcSourceContainer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaTreeMcSourceContainer(void *p) {
      delete ((::CbmAnaTreeMcSourceContainer*)p);
   }
   static void deleteArray_CbmAnaTreeMcSourceContainer(void *p) {
      delete [] ((::CbmAnaTreeMcSourceContainer*)p);
   }
   static void destruct_CbmAnaTreeMcSourceContainer(void *p) {
      typedef ::CbmAnaTreeMcSourceContainer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaTreeMcSourceContainer

//______________________________________________________________________________
void NicaCbmMCTrackInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCbmMCTrackInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCbmMCTrackInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCbmMCTrackInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCbmMCTrackInterface(void *p) {
      return  p ? new(p) ::NicaCbmMCTrackInterface : new ::NicaCbmMCTrackInterface;
   }
   static void *newArray_NicaCbmMCTrackInterface(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCbmMCTrackInterface[nElements] : new ::NicaCbmMCTrackInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCbmMCTrackInterface(void *p) {
      delete ((::NicaCbmMCTrackInterface*)p);
   }
   static void deleteArray_NicaCbmMCTrackInterface(void *p) {
      delete [] ((::NicaCbmMCTrackInterface*)p);
   }
   static void destruct_NicaCbmMCTrackInterface(void *p) {
      typedef ::NicaCbmMCTrackInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCbmMCTrackInterface

//______________________________________________________________________________
void NicaCbmMCEventInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCbmMCEventInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCbmMCEventInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCbmMCEventInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCbmMCEventInterface(void *p) {
      return  p ? new(p) ::NicaCbmMCEventInterface : new ::NicaCbmMCEventInterface;
   }
   static void *newArray_NicaCbmMCEventInterface(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCbmMCEventInterface[nElements] : new ::NicaCbmMCEventInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCbmMCEventInterface(void *p) {
      delete ((::NicaCbmMCEventInterface*)p);
   }
   static void deleteArray_NicaCbmMCEventInterface(void *p) {
      delete [] ((::NicaCbmMCEventInterface*)p);
   }
   static void destruct_NicaCbmMCEventInterface(void *p) {
      typedef ::NicaCbmMCEventInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCbmMCEventInterface

//______________________________________________________________________________
void NicaCbmMCTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCbmMCTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCbmMCTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCbmMCTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCbmMCTrack(void *p) {
      return  p ? new(p) ::NicaCbmMCTrack : new ::NicaCbmMCTrack;
   }
   static void *newArray_NicaCbmMCTrack(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCbmMCTrack[nElements] : new ::NicaCbmMCTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCbmMCTrack(void *p) {
      delete ((::NicaCbmMCTrack*)p);
   }
   static void deleteArray_NicaCbmMCTrack(void *p) {
      delete [] ((::NicaCbmMCTrack*)p);
   }
   static void destruct_NicaCbmMCTrack(void *p) {
      typedef ::NicaCbmMCTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCbmMCTrack

//______________________________________________________________________________
void NicaCbmTrackInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCbmTrackInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCbmTrackInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCbmTrackInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCbmTrackInterface(void *p) {
      return  p ? new(p) ::NicaCbmTrackInterface : new ::NicaCbmTrackInterface;
   }
   static void *newArray_NicaCbmTrackInterface(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCbmTrackInterface[nElements] : new ::NicaCbmTrackInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCbmTrackInterface(void *p) {
      delete ((::NicaCbmTrackInterface*)p);
   }
   static void deleteArray_NicaCbmTrackInterface(void *p) {
      delete [] ((::NicaCbmTrackInterface*)p);
   }
   static void destruct_NicaCbmTrackInterface(void *p) {
      typedef ::NicaCbmTrackInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCbmTrackInterface

//______________________________________________________________________________
void NicaCbmEventInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCbmEventInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCbmEventInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCbmEventInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCbmEventInterface(void *p) {
      return  p ? new(p) ::NicaCbmEventInterface : new ::NicaCbmEventInterface;
   }
   static void *newArray_NicaCbmEventInterface(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCbmEventInterface[nElements] : new ::NicaCbmEventInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCbmEventInterface(void *p) {
      delete ((::NicaCbmEventInterface*)p);
   }
   static void deleteArray_NicaCbmEventInterface(void *p) {
      delete [] ((::NicaCbmEventInterface*)p);
   }
   static void destruct_NicaCbmEventInterface(void *p) {
      typedef ::NicaCbmEventInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCbmEventInterface

//______________________________________________________________________________
void NicaCbmTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCbmTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCbmTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCbmTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCbmTrack(void *p) {
      return  p ? new(p) ::NicaCbmTrack : new ::NicaCbmTrack;
   }
   static void *newArray_NicaCbmTrack(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCbmTrack[nElements] : new ::NicaCbmTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCbmTrack(void *p) {
      delete ((::NicaCbmTrack*)p);
   }
   static void deleteArray_NicaCbmTrack(void *p) {
      delete [] ((::NicaCbmTrack*)p);
   }
   static void destruct_NicaCbmTrack(void *p) {
      typedef ::NicaCbmTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCbmTrack

//______________________________________________________________________________
void NicaCbmEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCbmEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCbmEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCbmEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCbmEvent(void *p) {
      return  p ? new(p) ::NicaCbmEvent : new ::NicaCbmEvent;
   }
   static void *newArray_NicaCbmEvent(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCbmEvent[nElements] : new ::NicaCbmEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCbmEvent(void *p) {
      delete ((::NicaCbmEvent*)p);
   }
   static void deleteArray_NicaCbmEvent(void *p) {
      delete [] ((::NicaCbmEvent*)p);
   }
   static void destruct_NicaCbmEvent(void *p) {
      typedef ::NicaCbmEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCbmEvent

//______________________________________________________________________________
void CbmHbtTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmHbtTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmHbtTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmHbtTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmHbtTrack(void *p) {
      return  p ? new(p) ::CbmHbtTrack : new ::CbmHbtTrack;
   }
   static void *newArray_CbmHbtTrack(Long_t nElements, void *p) {
      return p ? new(p) ::CbmHbtTrack[nElements] : new ::CbmHbtTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmHbtTrack(void *p) {
      delete ((::CbmHbtTrack*)p);
   }
   static void deleteArray_CbmHbtTrack(void *p) {
      delete [] ((::CbmHbtTrack*)p);
   }
   static void destruct_CbmHbtTrack(void *p) {
      typedef ::CbmHbtTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmHbtTrack

//______________________________________________________________________________
void CbmHbtEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmHbtEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmHbtEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmHbtEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmHbtEvent(void *p) {
      return  p ? new(p) ::CbmHbtEvent : new ::CbmHbtEvent;
   }
   static void *newArray_CbmHbtEvent(Long_t nElements, void *p) {
      return p ? new(p) ::CbmHbtEvent[nElements] : new ::CbmHbtEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmHbtEvent(void *p) {
      delete ((::CbmHbtEvent*)p);
   }
   static void deleteArray_CbmHbtEvent(void *p) {
      delete [] ((::CbmHbtEvent*)p);
   }
   static void destruct_CbmHbtEvent(void *p) {
      typedef ::CbmHbtEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmHbtEvent

//______________________________________________________________________________
void CbmAnaTreeSource::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmAnaTreeSource.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmAnaTreeSource::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmAnaTreeSource::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAnaTreeSource(void *p) {
      return  p ? new(p) ::CbmAnaTreeSource : new ::CbmAnaTreeSource;
   }
   static void *newArray_CbmAnaTreeSource(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAnaTreeSource[nElements] : new ::CbmAnaTreeSource[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAnaTreeSource(void *p) {
      delete ((::CbmAnaTreeSource*)p);
   }
   static void deleteArray_CbmAnaTreeSource(void *p) {
      delete [] ((::CbmAnaTreeSource*)p);
   }
   static void destruct_CbmAnaTreeSource(void *p) {
      typedef ::CbmAnaTreeSource current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAnaTreeSource

namespace ROOT {
   static TClass *vectorlETStringgR_Dictionary();
   static void vectorlETStringgR_TClassManip(TClass*);
   static void *new_vectorlETStringgR(void *p = nullptr);
   static void *newArray_vectorlETStringgR(Long_t size, void *p);
   static void delete_vectorlETStringgR(void *p);
   static void deleteArray_vectorlETStringgR(void *p);
   static void destruct_vectorlETStringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TString>*)
   {
      vector<TString> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TString>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TString>", -2, "vector", 389,
                  typeid(vector<TString>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETStringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TString>) );
      instance.SetNew(&new_vectorlETStringgR);
      instance.SetNewArray(&newArray_vectorlETStringgR);
      instance.SetDelete(&delete_vectorlETStringgR);
      instance.SetDeleteArray(&deleteArray_vectorlETStringgR);
      instance.SetDestructor(&destruct_vectorlETStringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TString> >()));

      ::ROOT::AddClassAlternate("vector<TString>","std::vector<TString, std::allocator<TString> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TString>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETStringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TString>*)nullptr)->GetClass();
      vectorlETStringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETStringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETStringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TString> : new vector<TString>;
   }
   static void *newArray_vectorlETStringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TString>[nElements] : new vector<TString>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETStringgR(void *p) {
      delete ((vector<TString>*)p);
   }
   static void deleteArray_vectorlETStringgR(void *p) {
      delete [] ((vector<TString>*)p);
   }
   static void destruct_vectorlETStringgR(void *p) {
      typedef vector<TString> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TString>

namespace {
  void TriggerDictionaryInitialization_libNicaCbmFormat_Impl() {
    static const char* headers[] = {
"NicaCbmFormatTypes.h",
"NicaCbmFullEvent.h",
"CbmHbtFullEvent.h",
"NicaGeneratorReadUnigen.h",
"NicaGeneratorWriteUnigen.h",
"NicaUnigenSource.h",
"NicaUnigenEvent.h",
"NicaUnigenEventInterface.h",
"NicaUnigenTrack.h",
"NicaUnigenTrackInterface.h",
"NicaCbmMCEvent.h",
"NicaCbmMCEventInterface.h",
"NicaCbmMCTrack.h",
"NicaCbmMCTrackInterface.h",
"NicaCbmEvent.h",
"NicaCbmEventInterface.h",
"NicaCbmTrack.h",
"NicaCbmTrackInterface.h",
"CbmHbtTrack.h",
"CbmHbtEvent.h",
"CbmAnaTreeContainer.h",
"CbmAnaTreeSource.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/combined",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/unigen",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/mc",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/reco",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/femto",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/combined",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/unigen",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/mc",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/reco",
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/femto",
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
"/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/helpers",
"/home/heinemann/gpuunpacker/cbmroot/build2/include",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators/unigen",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/generators/pluto",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/analysis/PWGC2F/femtoscopy/nicafemto/format/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libNicaCbmFormat dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$NicaCbmFullEvent.h")))  NicaCbmFullEvent;
class __attribute__((annotate("$clingAutoload$CbmHbtFullEvent.h")))  CbmHbtFullEvent;
class __attribute__((annotate("$clingAutoload$NicaGeneratorReadUnigen.h")))  NicaGeneratorReadUnigen;
class __attribute__((annotate("$clingAutoload$NicaGeneratorWriteUnigen.h")))  NicaGeneratorWriteUnigen;
class __attribute__((annotate("$clingAutoload$NicaUnigenSource.h")))  NicaUnigenSource;
class __attribute__((annotate("$clingAutoload$NicaUnigenTrack.h")))  __attribute__((annotate("$clingAutoload$NicaUnigenEvent.h")))  NicaUnigenTrack;
class __attribute__((annotate("$clingAutoload$NicaUnigenEvent.h")))  NicaUnigenEvent;
class __attribute__((annotate("$clingAutoload$NicaUnigenEventInterface.h")))  NicaUnigenEventInterface;
class __attribute__((annotate("$clingAutoload$NicaUnigenTrackInterface.h")))  NicaUnigenTrackInterface;
class __attribute__((annotate("$clingAutoload$NicaCbmMCEvent.h")))  NicaCbmMCEvent;
class __attribute__((annotate("$clingAutoload$CbmAnaTreeContainer.h")))  __attribute__((annotate("$clingAutoload$NicaCbmMCEventInterface.h")))  CbmAnaTreeRecoSourceContainer;
class __attribute__((annotate("$clingAutoload$CbmAnaTreeContainer.h")))  __attribute__((annotate("$clingAutoload$NicaCbmMCEventInterface.h")))  CbmAnaTreeMcSourceContainer;
class __attribute__((annotate("$clingAutoload$NicaCbmMCTrackInterface.h")))  __attribute__((annotate("$clingAutoload$NicaCbmMCEventInterface.h")))  NicaCbmMCTrackInterface;
class __attribute__((annotate("$clingAutoload$NicaCbmMCEventInterface.h")))  NicaCbmMCEventInterface;
class __attribute__((annotate("$clingAutoload$NicaCbmMCTrack.h")))  NicaCbmMCTrack;
class __attribute__((annotate("$clingAutoload$NicaCbmTrackInterface.h")))  __attribute__((annotate("$clingAutoload$NicaCbmEvent.h")))  NicaCbmTrackInterface;
class __attribute__((annotate("$clingAutoload$NicaCbmEventInterface.h")))  __attribute__((annotate("$clingAutoload$NicaCbmEvent.h")))  NicaCbmEventInterface;
class __attribute__((annotate("$clingAutoload$NicaCbmTrack.h")))  __attribute__((annotate("$clingAutoload$NicaCbmEvent.h")))  NicaCbmTrack;
class __attribute__((annotate("$clingAutoload$NicaCbmEvent.h")))  NicaCbmEvent;
class __attribute__((annotate("$clingAutoload$CbmHbtTrack.h")))  CbmHbtTrack;
class __attribute__((annotate("$clingAutoload$CbmHbtEvent.h")))  CbmHbtEvent;
class __attribute__((annotate("$clingAutoload$CbmAnaTreeSource.h")))  CbmAnaTreeSource;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libNicaCbmFormat dictionary payload"

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
#include "NicaCbmFormatTypes.h"
#include "NicaCbmFullEvent.h"
#include "CbmHbtFullEvent.h"
#include "NicaGeneratorReadUnigen.h"
#include "NicaGeneratorWriteUnigen.h"
#include "NicaUnigenSource.h"
#include "NicaUnigenEvent.h"
#include "NicaUnigenEventInterface.h"
#include "NicaUnigenTrack.h"
#include "NicaUnigenTrackInterface.h"
#include "NicaCbmMCEvent.h"
#include "NicaCbmMCEventInterface.h"
#include "NicaCbmMCTrack.h"
#include "NicaCbmMCTrackInterface.h"
#include "NicaCbmEvent.h"
#include "NicaCbmEventInterface.h"
#include "NicaCbmTrack.h"
#include "NicaCbmTrackInterface.h"
#include "CbmHbtTrack.h"
#include "CbmHbtEvent.h"
#include "CbmAnaTreeContainer.h"
#include "CbmAnaTreeSource.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmAnaTreeMcSourceContainer", payloadCode, "@",
"CbmAnaTreeRecoSourceContainer", payloadCode, "@",
"CbmAnaTreeSource", payloadCode, "@",
"CbmHbtEvent", payloadCode, "@",
"CbmHbtFullEvent", payloadCode, "@",
"CbmHbtTrack", payloadCode, "@",
"NicaCbmEvent", payloadCode, "@",
"NicaCbmEventInterface", payloadCode, "@",
"NicaCbmFullEvent", payloadCode, "@",
"NicaCbmMCEvent", payloadCode, "@",
"NicaCbmMCEventInterface", payloadCode, "@",
"NicaCbmMCTrack", payloadCode, "@",
"NicaCbmMCTrackInterface", payloadCode, "@",
"NicaCbmTrack", payloadCode, "@",
"NicaCbmTrackInterface", payloadCode, "@",
"NicaGeneratorReadUnigen", payloadCode, "@",
"NicaGeneratorWriteUnigen", payloadCode, "@",
"NicaUnigenEvent", payloadCode, "@",
"NicaUnigenEventInterface", payloadCode, "@",
"NicaUnigenSource", payloadCode, "@",
"NicaUnigenTrack", payloadCode, "@",
"NicaUnigenTrackInterface", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libNicaCbmFormat",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libNicaCbmFormat_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libNicaCbmFormat_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libNicaCbmFormat() {
  TriggerDictionaryInitialization_libNicaCbmFormat_Impl();
}
