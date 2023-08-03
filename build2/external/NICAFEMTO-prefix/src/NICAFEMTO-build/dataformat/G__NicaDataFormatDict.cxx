// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIheinemanndIgpuunpackerdIcbmrootdIbuild2dIexternaldINICAFEMTOmIprefixdIsrcdINICAFEMTOmIbuilddIdataformatdIG__NicaDataFormatDict
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
#include "NicaDataFormatManager.h"
#include "NicaEvent.h"
#include "NicaTrack.h"
#include "NicaTwoTrack.h"
#include "NicaMCEvent.h"
#include "NicaMCTrack.h"
#include "NicaEventInterface.h"
#include "NicaMCEventInterface.h"
#include "NicaTrackInterface.h"
#include "NicaMCTrackInterface.h"
#include "NicaVirtualTrack.h"
#include "NicaVirtualEvent.h"
#include "NicaTrackClones.h"
#include "NicaDataFormat.h"
#include "NicaComplexEvent.h"
#include "NicaComplexTrack.h"
#include "NicaSmearedEvent.h"
#include "NicaSmearedTrack.h"
#include "NicaToFTrack.h"
#include "NicaTpcTrack.h"
#include "NicaExpTrack.h"
#include "NicaExpEvent.h"
#include "NicaExpEventInterface.h"
#include "NicaDetectorTrack.h"
#include "NicaDetectorEvent.h"
#include "NicaV0Track.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace NicaDataFieldID {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *NicaDataFieldID_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("NicaDataFieldID", 0 /*version*/, "NicaDataFormat.h", 14,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &NicaDataFieldID_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *NicaDataFieldID_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace NicaDetectorID {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *NicaDetectorID_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("NicaDetectorID", 0 /*version*/, "NicaDataFormat.h", 96,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &NicaDetectorID_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *NicaDetectorID_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_NicaV0Track(void *p = nullptr);
   static void *newArray_NicaV0Track(Long_t size, void *p);
   static void delete_NicaV0Track(void *p);
   static void deleteArray_NicaV0Track(void *p);
   static void destruct_NicaV0Track(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaV0Track*)
   {
      ::NicaV0Track *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaV0Track >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaV0Track", ::NicaV0Track::Class_Version(), "NicaV0Track.h", 19,
                  typeid(::NicaV0Track), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaV0Track::Dictionary, isa_proxy, 4,
                  sizeof(::NicaV0Track) );
      instance.SetNew(&new_NicaV0Track);
      instance.SetNewArray(&newArray_NicaV0Track);
      instance.SetDelete(&delete_NicaV0Track);
      instance.SetDeleteArray(&deleteArray_NicaV0Track);
      instance.SetDestructor(&destruct_NicaV0Track);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaV0Track*)
   {
      return GenerateInitInstanceLocal((::NicaV0Track*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaV0Track*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrack(void *p = nullptr);
   static void *newArray_NicaTrack(Long_t size, void *p);
   static void delete_NicaTrack(void *p);
   static void deleteArray_NicaTrack(void *p);
   static void destruct_NicaTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrack*)
   {
      ::NicaTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrack", ::NicaTrack::Class_Version(), "NicaTrack.h", 25,
                  typeid(::NicaTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrack::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrack) );
      instance.SetNew(&new_NicaTrack);
      instance.SetNewArray(&newArray_NicaTrack);
      instance.SetDelete(&delete_NicaTrack);
      instance.SetDeleteArray(&deleteArray_NicaTrack);
      instance.SetDestructor(&destruct_NicaTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrack*)
   {
      return GenerateInitInstanceLocal((::NicaTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaDataFormatManager(void *p = nullptr);
   static void *newArray_NicaDataFormatManager(Long_t size, void *p);
   static void delete_NicaDataFormatManager(void *p);
   static void deleteArray_NicaDataFormatManager(void *p);
   static void destruct_NicaDataFormatManager(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaDataFormatManager*)
   {
      ::NicaDataFormatManager *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaDataFormatManager >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaDataFormatManager", ::NicaDataFormatManager::Class_Version(), "NicaDataFormatManager.h", 25,
                  typeid(::NicaDataFormatManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaDataFormatManager::Dictionary, isa_proxy, 4,
                  sizeof(::NicaDataFormatManager) );
      instance.SetNew(&new_NicaDataFormatManager);
      instance.SetNewArray(&newArray_NicaDataFormatManager);
      instance.SetDelete(&delete_NicaDataFormatManager);
      instance.SetDeleteArray(&deleteArray_NicaDataFormatManager);
      instance.SetDestructor(&destruct_NicaDataFormatManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaDataFormatManager*)
   {
      return GenerateInitInstanceLocal((::NicaDataFormatManager*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaDataFormatManager*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackInterface(void *p = nullptr);
   static void *newArray_NicaTrackInterface(Long_t size, void *p);
   static void delete_NicaTrackInterface(void *p);
   static void deleteArray_NicaTrackInterface(void *p);
   static void destruct_NicaTrackInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackInterface*)
   {
      ::NicaTrackInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackInterface", ::NicaTrackInterface::Class_Version(), "NicaTrackInterface.h", 16,
                  typeid(::NicaTrackInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackInterface::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackInterface) );
      instance.SetNew(&new_NicaTrackInterface);
      instance.SetNewArray(&newArray_NicaTrackInterface);
      instance.SetDelete(&delete_NicaTrackInterface);
      instance.SetDeleteArray(&deleteArray_NicaTrackInterface);
      instance.SetDestructor(&destruct_NicaTrackInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackInterface*)
   {
      return GenerateInitInstanceLocal((::NicaTrackInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaEventInterface(void *p);
   static void deleteArray_NicaEventInterface(void *p);
   static void destruct_NicaEventInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEventInterface*)
   {
      ::NicaEventInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEventInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEventInterface", ::NicaEventInterface::Class_Version(), "NicaEventInterface.h", 28,
                  typeid(::NicaEventInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEventInterface::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEventInterface) );
      instance.SetDelete(&delete_NicaEventInterface);
      instance.SetDeleteArray(&deleteArray_NicaEventInterface);
      instance.SetDestructor(&destruct_NicaEventInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEventInterface*)
   {
      return GenerateInitInstanceLocal((::NicaEventInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEventInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaEvent(void *p);
   static void deleteArray_NicaEvent(void *p);
   static void destruct_NicaEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaEvent*)
   {
      ::NicaEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaEvent", ::NicaEvent::Class_Version(), "NicaEvent.h", 47,
                  typeid(::NicaEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaEvent::Dictionary, isa_proxy, 4,
                  sizeof(::NicaEvent) );
      instance.SetDelete(&delete_NicaEvent);
      instance.SetDeleteArray(&deleteArray_NicaEvent);
      instance.SetDestructor(&destruct_NicaEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaEvent*)
   {
      return GenerateInitInstanceLocal((::NicaEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTwoTrack(void *p = nullptr);
   static void *newArray_NicaTwoTrack(Long_t size, void *p);
   static void delete_NicaTwoTrack(void *p);
   static void deleteArray_NicaTwoTrack(void *p);
   static void destruct_NicaTwoTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTwoTrack*)
   {
      ::NicaTwoTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTwoTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTwoTrack", ::NicaTwoTrack::Class_Version(), "NicaTwoTrack.h", 20,
                  typeid(::NicaTwoTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTwoTrack::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTwoTrack) );
      instance.SetNew(&new_NicaTwoTrack);
      instance.SetNewArray(&newArray_NicaTwoTrack);
      instance.SetDelete(&delete_NicaTwoTrack);
      instance.SetDeleteArray(&deleteArray_NicaTwoTrack);
      instance.SetDestructor(&destruct_NicaTwoTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTwoTrack*)
   {
      return GenerateInitInstanceLocal((::NicaTwoTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTwoTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaMCEvent(void *p);
   static void deleteArray_NicaMCEvent(void *p);
   static void destruct_NicaMCEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaMCEvent*)
   {
      ::NicaMCEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaMCEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaMCEvent", ::NicaMCEvent::Class_Version(), "NicaMCEvent.h", 16,
                  typeid(::NicaMCEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaMCEvent::Dictionary, isa_proxy, 4,
                  sizeof(::NicaMCEvent) );
      instance.SetDelete(&delete_NicaMCEvent);
      instance.SetDeleteArray(&deleteArray_NicaMCEvent);
      instance.SetDestructor(&destruct_NicaMCEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaMCEvent*)
   {
      return GenerateInitInstanceLocal((::NicaMCEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaMCEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaMCTrack(void *p = nullptr);
   static void *newArray_NicaMCTrack(Long_t size, void *p);
   static void delete_NicaMCTrack(void *p);
   static void deleteArray_NicaMCTrack(void *p);
   static void destruct_NicaMCTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaMCTrack*)
   {
      ::NicaMCTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaMCTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaMCTrack", ::NicaMCTrack::Class_Version(), "NicaMCTrack.h", 16,
                  typeid(::NicaMCTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaMCTrack::Dictionary, isa_proxy, 4,
                  sizeof(::NicaMCTrack) );
      instance.SetNew(&new_NicaMCTrack);
      instance.SetNewArray(&newArray_NicaMCTrack);
      instance.SetDelete(&delete_NicaMCTrack);
      instance.SetDeleteArray(&deleteArray_NicaMCTrack);
      instance.SetDestructor(&destruct_NicaMCTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaMCTrack*)
   {
      return GenerateInitInstanceLocal((::NicaMCTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaMCTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaMCEventInterface_Dictionary();
   static void NicaMCEventInterface_TClassManip(TClass*);
   static void delete_NicaMCEventInterface(void *p);
   static void deleteArray_NicaMCEventInterface(void *p);
   static void destruct_NicaMCEventInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaMCEventInterface*)
   {
      ::NicaMCEventInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::NicaMCEventInterface));
      static ::ROOT::TGenericClassInfo 
         instance("NicaMCEventInterface", "NicaMCEventInterface.h", 14,
                  typeid(::NicaMCEventInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaMCEventInterface_Dictionary, isa_proxy, 4,
                  sizeof(::NicaMCEventInterface) );
      instance.SetDelete(&delete_NicaMCEventInterface);
      instance.SetDeleteArray(&deleteArray_NicaMCEventInterface);
      instance.SetDestructor(&destruct_NicaMCEventInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaMCEventInterface*)
   {
      return GenerateInitInstanceLocal((::NicaMCEventInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaMCEventInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaMCEventInterface_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaMCEventInterface*)nullptr)->GetClass();
      NicaMCEventInterface_TClassManip(theClass);
   return theClass;
   }

   static void NicaMCEventInterface_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaMCTrackInterface(void *p = nullptr);
   static void *newArray_NicaMCTrackInterface(Long_t size, void *p);
   static void delete_NicaMCTrackInterface(void *p);
   static void deleteArray_NicaMCTrackInterface(void *p);
   static void destruct_NicaMCTrackInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaMCTrackInterface*)
   {
      ::NicaMCTrackInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaMCTrackInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaMCTrackInterface", ::NicaMCTrackInterface::Class_Version(), "NicaMCTrackInterface.h", 14,
                  typeid(::NicaMCTrackInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaMCTrackInterface::Dictionary, isa_proxy, 4,
                  sizeof(::NicaMCTrackInterface) );
      instance.SetNew(&new_NicaMCTrackInterface);
      instance.SetNewArray(&newArray_NicaMCTrackInterface);
      instance.SetDelete(&delete_NicaMCTrackInterface);
      instance.SetDeleteArray(&deleteArray_NicaMCTrackInterface);
      instance.SetDestructor(&destruct_NicaMCTrackInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaMCTrackInterface*)
   {
      return GenerateInitInstanceLocal((::NicaMCTrackInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaMCTrackInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaVirtualTrack(void *p = nullptr);
   static void *newArray_NicaVirtualTrack(Long_t size, void *p);
   static void delete_NicaVirtualTrack(void *p);
   static void deleteArray_NicaVirtualTrack(void *p);
   static void destruct_NicaVirtualTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaVirtualTrack*)
   {
      ::NicaVirtualTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaVirtualTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaVirtualTrack", ::NicaVirtualTrack::Class_Version(), "NicaVirtualTrack.h", 12,
                  typeid(::NicaVirtualTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaVirtualTrack::Dictionary, isa_proxy, 4,
                  sizeof(::NicaVirtualTrack) );
      instance.SetNew(&new_NicaVirtualTrack);
      instance.SetNewArray(&newArray_NicaVirtualTrack);
      instance.SetDelete(&delete_NicaVirtualTrack);
      instance.SetDeleteArray(&deleteArray_NicaVirtualTrack);
      instance.SetDestructor(&destruct_NicaVirtualTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaVirtualTrack*)
   {
      return GenerateInitInstanceLocal((::NicaVirtualTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaVirtualTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaVirtualEvent(void *p = nullptr);
   static void *newArray_NicaVirtualEvent(Long_t size, void *p);
   static void delete_NicaVirtualEvent(void *p);
   static void deleteArray_NicaVirtualEvent(void *p);
   static void destruct_NicaVirtualEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaVirtualEvent*)
   {
      ::NicaVirtualEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaVirtualEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaVirtualEvent", ::NicaVirtualEvent::Class_Version(), "NicaVirtualEvent.h", 18,
                  typeid(::NicaVirtualEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaVirtualEvent::Dictionary, isa_proxy, 4,
                  sizeof(::NicaVirtualEvent) );
      instance.SetNew(&new_NicaVirtualEvent);
      instance.SetNewArray(&newArray_NicaVirtualEvent);
      instance.SetDelete(&delete_NicaVirtualEvent);
      instance.SetDeleteArray(&deleteArray_NicaVirtualEvent);
      instance.SetDestructor(&destruct_NicaVirtualEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaVirtualEvent*)
   {
      return GenerateInitInstanceLocal((::NicaVirtualEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaVirtualEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTrackClones(void *p = nullptr);
   static void *newArray_NicaTrackClones(Long_t size, void *p);
   static void delete_NicaTrackClones(void *p);
   static void deleteArray_NicaTrackClones(void *p);
   static void destruct_NicaTrackClones(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTrackClones*)
   {
      ::NicaTrackClones *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTrackClones >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTrackClones", ::NicaTrackClones::Class_Version(), "NicaTrackClones.h", 16,
                  typeid(::NicaTrackClones), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTrackClones::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTrackClones) );
      instance.SetNew(&new_NicaTrackClones);
      instance.SetNewArray(&newArray_NicaTrackClones);
      instance.SetDelete(&delete_NicaTrackClones);
      instance.SetDeleteArray(&deleteArray_NicaTrackClones);
      instance.SetDestructor(&destruct_NicaTrackClones);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTrackClones*)
   {
      return GenerateInitInstanceLocal((::NicaTrackClones*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTrackClones*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaComplexEvent(void *p = nullptr);
   static void *newArray_NicaComplexEvent(Long_t size, void *p);
   static void delete_NicaComplexEvent(void *p);
   static void deleteArray_NicaComplexEvent(void *p);
   static void destruct_NicaComplexEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaComplexEvent*)
   {
      ::NicaComplexEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaComplexEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaComplexEvent", ::NicaComplexEvent::Class_Version(), "NicaComplexEvent.h", 22,
                  typeid(::NicaComplexEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaComplexEvent::Dictionary, isa_proxy, 4,
                  sizeof(::NicaComplexEvent) );
      instance.SetNew(&new_NicaComplexEvent);
      instance.SetNewArray(&newArray_NicaComplexEvent);
      instance.SetDelete(&delete_NicaComplexEvent);
      instance.SetDeleteArray(&deleteArray_NicaComplexEvent);
      instance.SetDestructor(&destruct_NicaComplexEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaComplexEvent*)
   {
      return GenerateInitInstanceLocal((::NicaComplexEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaComplexEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaComplexTrack(void *p = nullptr);
   static void *newArray_NicaComplexTrack(Long_t size, void *p);
   static void delete_NicaComplexTrack(void *p);
   static void deleteArray_NicaComplexTrack(void *p);
   static void destruct_NicaComplexTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaComplexTrack*)
   {
      ::NicaComplexTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaComplexTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaComplexTrack", ::NicaComplexTrack::Class_Version(), "NicaComplexTrack.h", 18,
                  typeid(::NicaComplexTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaComplexTrack::Dictionary, isa_proxy, 4,
                  sizeof(::NicaComplexTrack) );
      instance.SetNew(&new_NicaComplexTrack);
      instance.SetNewArray(&newArray_NicaComplexTrack);
      instance.SetDelete(&delete_NicaComplexTrack);
      instance.SetDeleteArray(&deleteArray_NicaComplexTrack);
      instance.SetDestructor(&destruct_NicaComplexTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaComplexTrack*)
   {
      return GenerateInitInstanceLocal((::NicaComplexTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaComplexTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaSmearedEvent(void *p = nullptr);
   static void *newArray_NicaSmearedEvent(Long_t size, void *p);
   static void delete_NicaSmearedEvent(void *p);
   static void deleteArray_NicaSmearedEvent(void *p);
   static void destruct_NicaSmearedEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaSmearedEvent*)
   {
      ::NicaSmearedEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaSmearedEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaSmearedEvent", ::NicaSmearedEvent::Class_Version(), "NicaSmearedEvent.h", 17,
                  typeid(::NicaSmearedEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaSmearedEvent::Dictionary, isa_proxy, 4,
                  sizeof(::NicaSmearedEvent) );
      instance.SetNew(&new_NicaSmearedEvent);
      instance.SetNewArray(&newArray_NicaSmearedEvent);
      instance.SetDelete(&delete_NicaSmearedEvent);
      instance.SetDeleteArray(&deleteArray_NicaSmearedEvent);
      instance.SetDestructor(&destruct_NicaSmearedEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaSmearedEvent*)
   {
      return GenerateInitInstanceLocal((::NicaSmearedEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaSmearedEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaSmearedTrack(void *p = nullptr);
   static void *newArray_NicaSmearedTrack(Long_t size, void *p);
   static void delete_NicaSmearedTrack(void *p);
   static void deleteArray_NicaSmearedTrack(void *p);
   static void destruct_NicaSmearedTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaSmearedTrack*)
   {
      ::NicaSmearedTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaSmearedTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaSmearedTrack", ::NicaSmearedTrack::Class_Version(), "NicaSmearedTrack.h", 19,
                  typeid(::NicaSmearedTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaSmearedTrack::Dictionary, isa_proxy, 4,
                  sizeof(::NicaSmearedTrack) );
      instance.SetNew(&new_NicaSmearedTrack);
      instance.SetNewArray(&newArray_NicaSmearedTrack);
      instance.SetDelete(&delete_NicaSmearedTrack);
      instance.SetDeleteArray(&deleteArray_NicaSmearedTrack);
      instance.SetDestructor(&destruct_NicaSmearedTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaSmearedTrack*)
   {
      return GenerateInitInstanceLocal((::NicaSmearedTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaSmearedTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaDetectorTrack(void *p = nullptr);
   static void *newArray_NicaDetectorTrack(Long_t size, void *p);
   static void delete_NicaDetectorTrack(void *p);
   static void deleteArray_NicaDetectorTrack(void *p);
   static void destruct_NicaDetectorTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaDetectorTrack*)
   {
      ::NicaDetectorTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaDetectorTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaDetectorTrack", ::NicaDetectorTrack::Class_Version(), "NicaDetectorTrack.h", 19,
                  typeid(::NicaDetectorTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaDetectorTrack::Dictionary, isa_proxy, 4,
                  sizeof(::NicaDetectorTrack) );
      instance.SetNew(&new_NicaDetectorTrack);
      instance.SetNewArray(&newArray_NicaDetectorTrack);
      instance.SetDelete(&delete_NicaDetectorTrack);
      instance.SetDeleteArray(&deleteArray_NicaDetectorTrack);
      instance.SetDestructor(&destruct_NicaDetectorTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaDetectorTrack*)
   {
      return GenerateInitInstanceLocal((::NicaDetectorTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaDetectorTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaToFTrack(void *p = nullptr);
   static void *newArray_NicaToFTrack(Long_t size, void *p);
   static void delete_NicaToFTrack(void *p);
   static void deleteArray_NicaToFTrack(void *p);
   static void destruct_NicaToFTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaToFTrack*)
   {
      ::NicaToFTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaToFTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaToFTrack", ::NicaToFTrack::Class_Version(), "NicaToFTrack.h", 16,
                  typeid(::NicaToFTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaToFTrack::Dictionary, isa_proxy, 4,
                  sizeof(::NicaToFTrack) );
      instance.SetNew(&new_NicaToFTrack);
      instance.SetNewArray(&newArray_NicaToFTrack);
      instance.SetDelete(&delete_NicaToFTrack);
      instance.SetDeleteArray(&deleteArray_NicaToFTrack);
      instance.SetDestructor(&destruct_NicaToFTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaToFTrack*)
   {
      return GenerateInitInstanceLocal((::NicaToFTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaToFTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaTpcTrack(void *p = nullptr);
   static void *newArray_NicaTpcTrack(Long_t size, void *p);
   static void delete_NicaTpcTrack(void *p);
   static void deleteArray_NicaTpcTrack(void *p);
   static void destruct_NicaTpcTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaTpcTrack*)
   {
      ::NicaTpcTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaTpcTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaTpcTrack", ::NicaTpcTrack::Class_Version(), "NicaTpcTrack.h", 16,
                  typeid(::NicaTpcTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaTpcTrack::Dictionary, isa_proxy, 4,
                  sizeof(::NicaTpcTrack) );
      instance.SetNew(&new_NicaTpcTrack);
      instance.SetNewArray(&newArray_NicaTpcTrack);
      instance.SetDelete(&delete_NicaTpcTrack);
      instance.SetDeleteArray(&deleteArray_NicaTpcTrack);
      instance.SetDestructor(&destruct_NicaTpcTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaTpcTrack*)
   {
      return GenerateInitInstanceLocal((::NicaTpcTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaTpcTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaExpTrack(void *p = nullptr);
   static void *newArray_NicaExpTrack(Long_t size, void *p);
   static void delete_NicaExpTrack(void *p);
   static void deleteArray_NicaExpTrack(void *p);
   static void destruct_NicaExpTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaExpTrack*)
   {
      ::NicaExpTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaExpTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaExpTrack", ::NicaExpTrack::Class_Version(), "NicaExpTrack.h", 22,
                  typeid(::NicaExpTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaExpTrack::Dictionary, isa_proxy, 4,
                  sizeof(::NicaExpTrack) );
      instance.SetNew(&new_NicaExpTrack);
      instance.SetNewArray(&newArray_NicaExpTrack);
      instance.SetDelete(&delete_NicaExpTrack);
      instance.SetDeleteArray(&deleteArray_NicaExpTrack);
      instance.SetDestructor(&destruct_NicaExpTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaExpTrack*)
   {
      return GenerateInitInstanceLocal((::NicaExpTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaExpTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaExpTrackHelix(void *p = nullptr);
   static void *newArray_NicaExpTrackHelix(Long_t size, void *p);
   static void delete_NicaExpTrackHelix(void *p);
   static void deleteArray_NicaExpTrackHelix(void *p);
   static void destruct_NicaExpTrackHelix(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaExpTrackHelix*)
   {
      ::NicaExpTrackHelix *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaExpTrackHelix >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaExpTrackHelix", ::NicaExpTrackHelix::Class_Version(), "NicaExpTrack.h", 100,
                  typeid(::NicaExpTrackHelix), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaExpTrackHelix::Dictionary, isa_proxy, 4,
                  sizeof(::NicaExpTrackHelix) );
      instance.SetNew(&new_NicaExpTrackHelix);
      instance.SetNewArray(&newArray_NicaExpTrackHelix);
      instance.SetDelete(&delete_NicaExpTrackHelix);
      instance.SetDeleteArray(&deleteArray_NicaExpTrackHelix);
      instance.SetDestructor(&destruct_NicaExpTrackHelix);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaExpTrackHelix*)
   {
      return GenerateInitInstanceLocal((::NicaExpTrackHelix*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaExpTrackHelix*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaExpEvent(void *p);
   static void deleteArray_NicaExpEvent(void *p);
   static void destruct_NicaExpEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaExpEvent*)
   {
      ::NicaExpEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaExpEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaExpEvent", ::NicaExpEvent::Class_Version(), "NicaExpEvent.h", 20,
                  typeid(::NicaExpEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaExpEvent::Dictionary, isa_proxy, 4,
                  sizeof(::NicaExpEvent) );
      instance.SetDelete(&delete_NicaExpEvent);
      instance.SetDeleteArray(&deleteArray_NicaExpEvent);
      instance.SetDestructor(&destruct_NicaExpEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaExpEvent*)
   {
      return GenerateInitInstanceLocal((::NicaExpEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaExpEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaExpEventHelix_Dictionary();
   static void NicaExpEventHelix_TClassManip(TClass*);
   static void delete_NicaExpEventHelix(void *p);
   static void deleteArray_NicaExpEventHelix(void *p);
   static void destruct_NicaExpEventHelix(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaExpEventHelix*)
   {
      ::NicaExpEventHelix *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::NicaExpEventHelix));
      static ::ROOT::TGenericClassInfo 
         instance("NicaExpEventHelix", "NicaExpEvent.h", 43,
                  typeid(::NicaExpEventHelix), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaExpEventHelix_Dictionary, isa_proxy, 4,
                  sizeof(::NicaExpEventHelix) );
      instance.SetDelete(&delete_NicaExpEventHelix);
      instance.SetDeleteArray(&deleteArray_NicaExpEventHelix);
      instance.SetDestructor(&destruct_NicaExpEventHelix);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaExpEventHelix*)
   {
      return GenerateInitInstanceLocal((::NicaExpEventHelix*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaExpEventHelix*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaExpEventHelix_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaExpEventHelix*)nullptr)->GetClass();
      NicaExpEventHelix_TClassManip(theClass);
   return theClass;
   }

   static void NicaExpEventHelix_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaExpEventInterface(void *p);
   static void deleteArray_NicaExpEventInterface(void *p);
   static void destruct_NicaExpEventInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaExpEventInterface*)
   {
      ::NicaExpEventInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaExpEventInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaExpEventInterface", ::NicaExpEventInterface::Class_Version(), "NicaExpEventInterface.h", 16,
                  typeid(::NicaExpEventInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaExpEventInterface::Dictionary, isa_proxy, 4,
                  sizeof(::NicaExpEventInterface) );
      instance.SetDelete(&delete_NicaExpEventInterface);
      instance.SetDeleteArray(&deleteArray_NicaExpEventInterface);
      instance.SetDestructor(&destruct_NicaExpEventInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaExpEventInterface*)
   {
      return GenerateInitInstanceLocal((::NicaExpEventInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaExpEventInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaDetectorEvent(void *p = nullptr);
   static void *newArray_NicaDetectorEvent(Long_t size, void *p);
   static void delete_NicaDetectorEvent(void *p);
   static void deleteArray_NicaDetectorEvent(void *p);
   static void destruct_NicaDetectorEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaDetectorEvent*)
   {
      ::NicaDetectorEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaDetectorEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaDetectorEvent", ::NicaDetectorEvent::Class_Version(), "NicaDetectorEvent.h", 19,
                  typeid(::NicaDetectorEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaDetectorEvent::Dictionary, isa_proxy, 4,
                  sizeof(::NicaDetectorEvent) );
      instance.SetNew(&new_NicaDetectorEvent);
      instance.SetNewArray(&newArray_NicaDetectorEvent);
      instance.SetDelete(&delete_NicaDetectorEvent);
      instance.SetDeleteArray(&deleteArray_NicaDetectorEvent);
      instance.SetDestructor(&destruct_NicaDetectorEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaDetectorEvent*)
   {
      return GenerateInitInstanceLocal((::NicaDetectorEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaDetectorEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NicaV0Track::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaV0Track::Class_Name()
{
   return "NicaV0Track";
}

//______________________________________________________________________________
const char *NicaV0Track::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaV0Track*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaV0Track::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaV0Track*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaV0Track::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaV0Track*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaV0Track::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaV0Track*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrack::Class_Name()
{
   return "NicaTrack";
}

//______________________________________________________________________________
const char *NicaTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaDataFormatManager::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaDataFormatManager::Class_Name()
{
   return "NicaDataFormatManager";
}

//______________________________________________________________________________
const char *NicaDataFormatManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaDataFormatManager*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaDataFormatManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaDataFormatManager*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaDataFormatManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaDataFormatManager*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaDataFormatManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaDataFormatManager*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackInterface::Class_Name()
{
   return "NicaTrackInterface";
}

//______________________________________________________________________________
const char *NicaTrackInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEventInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEventInterface::Class_Name()
{
   return "NicaEventInterface";
}

//______________________________________________________________________________
const char *NicaEventInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEventInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEventInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEventInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEventInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEventInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaEvent::Class_Name()
{
   return "NicaEvent";
}

//______________________________________________________________________________
const char *NicaEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTwoTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTwoTrack::Class_Name()
{
   return "NicaTwoTrack";
}

//______________________________________________________________________________
const char *NicaTwoTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTwoTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTwoTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTwoTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTwoTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaMCEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaMCEvent::Class_Name()
{
   return "NicaMCEvent";
}

//______________________________________________________________________________
const char *NicaMCEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMCEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaMCEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMCEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaMCEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMCEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaMCEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMCEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaMCTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaMCTrack::Class_Name()
{
   return "NicaMCTrack";
}

//______________________________________________________________________________
const char *NicaMCTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMCTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaMCTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMCTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaMCTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMCTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaMCTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMCTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaMCTrackInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaMCTrackInterface::Class_Name()
{
   return "NicaMCTrackInterface";
}

//______________________________________________________________________________
const char *NicaMCTrackInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMCTrackInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaMCTrackInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMCTrackInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaMCTrackInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMCTrackInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaMCTrackInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMCTrackInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaVirtualTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaVirtualTrack::Class_Name()
{
   return "NicaVirtualTrack";
}

//______________________________________________________________________________
const char *NicaVirtualTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaVirtualTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaVirtualTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaVirtualTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaVirtualTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaVirtualTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaVirtualTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaVirtualTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaVirtualEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaVirtualEvent::Class_Name()
{
   return "NicaVirtualEvent";
}

//______________________________________________________________________________
const char *NicaVirtualEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaVirtualEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaVirtualEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaVirtualEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaVirtualEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaVirtualEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaVirtualEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaVirtualEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTrackClones::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTrackClones::Class_Name()
{
   return "NicaTrackClones";
}

//______________________________________________________________________________
const char *NicaTrackClones::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackClones*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTrackClones::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackClones*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTrackClones::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackClones*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTrackClones::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTrackClones*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaComplexEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaComplexEvent::Class_Name()
{
   return "NicaComplexEvent";
}

//______________________________________________________________________________
const char *NicaComplexEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaComplexEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaComplexEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaComplexEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaComplexEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaComplexEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaComplexEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaComplexEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaComplexTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaComplexTrack::Class_Name()
{
   return "NicaComplexTrack";
}

//______________________________________________________________________________
const char *NicaComplexTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaComplexTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaComplexTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaComplexTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaComplexTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaComplexTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaComplexTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaComplexTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaSmearedEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaSmearedEvent::Class_Name()
{
   return "NicaSmearedEvent";
}

//______________________________________________________________________________
const char *NicaSmearedEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearedEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaSmearedEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearedEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaSmearedEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearedEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaSmearedEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearedEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaSmearedTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaSmearedTrack::Class_Name()
{
   return "NicaSmearedTrack";
}

//______________________________________________________________________________
const char *NicaSmearedTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearedTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaSmearedTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearedTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaSmearedTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearedTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaSmearedTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSmearedTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaDetectorTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaDetectorTrack::Class_Name()
{
   return "NicaDetectorTrack";
}

//______________________________________________________________________________
const char *NicaDetectorTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaDetectorTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaDetectorTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaDetectorTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaDetectorTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaDetectorTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaDetectorTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaDetectorTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaToFTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaToFTrack::Class_Name()
{
   return "NicaToFTrack";
}

//______________________________________________________________________________
const char *NicaToFTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaToFTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaToFTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaToFTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaToFTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaToFTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaToFTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaToFTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaTpcTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaTpcTrack::Class_Name()
{
   return "NicaTpcTrack";
}

//______________________________________________________________________________
const char *NicaTpcTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTpcTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaTpcTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaTpcTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaTpcTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTpcTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaTpcTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaTpcTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaExpTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaExpTrack::Class_Name()
{
   return "NicaExpTrack";
}

//______________________________________________________________________________
const char *NicaExpTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaExpTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaExpTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaExpTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaExpTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaExpTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaExpTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaExpTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaExpTrackHelix::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaExpTrackHelix::Class_Name()
{
   return "NicaExpTrackHelix";
}

//______________________________________________________________________________
const char *NicaExpTrackHelix::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaExpTrackHelix*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaExpTrackHelix::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaExpTrackHelix*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaExpTrackHelix::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaExpTrackHelix*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaExpTrackHelix::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaExpTrackHelix*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaExpEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaExpEvent::Class_Name()
{
   return "NicaExpEvent";
}

//______________________________________________________________________________
const char *NicaExpEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaExpEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaExpEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaExpEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaExpEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaExpEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaExpEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaExpEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaExpEventInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaExpEventInterface::Class_Name()
{
   return "NicaExpEventInterface";
}

//______________________________________________________________________________
const char *NicaExpEventInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaExpEventInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaExpEventInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaExpEventInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaExpEventInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaExpEventInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaExpEventInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaExpEventInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaDetectorEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaDetectorEvent::Class_Name()
{
   return "NicaDetectorEvent";
}

//______________________________________________________________________________
const char *NicaDetectorEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaDetectorEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaDetectorEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaDetectorEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaDetectorEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaDetectorEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaDetectorEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaDetectorEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void NicaV0Track::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaV0Track.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaV0Track::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaV0Track::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaV0Track(void *p) {
      return  p ? new(p) ::NicaV0Track : new ::NicaV0Track;
   }
   static void *newArray_NicaV0Track(Long_t nElements, void *p) {
      return p ? new(p) ::NicaV0Track[nElements] : new ::NicaV0Track[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaV0Track(void *p) {
      delete ((::NicaV0Track*)p);
   }
   static void deleteArray_NicaV0Track(void *p) {
      delete [] ((::NicaV0Track*)p);
   }
   static void destruct_NicaV0Track(void *p) {
      typedef ::NicaV0Track current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaV0Track

//______________________________________________________________________________
void NicaTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrack(void *p) {
      return  p ? new(p) ::NicaTrack : new ::NicaTrack;
   }
   static void *newArray_NicaTrack(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrack[nElements] : new ::NicaTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrack(void *p) {
      delete ((::NicaTrack*)p);
   }
   static void deleteArray_NicaTrack(void *p) {
      delete [] ((::NicaTrack*)p);
   }
   static void destruct_NicaTrack(void *p) {
      typedef ::NicaTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrack

//______________________________________________________________________________
void NicaDataFormatManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaDataFormatManager.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaDataFormatManager::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaDataFormatManager::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaDataFormatManager(void *p) {
      return  p ? new(p) ::NicaDataFormatManager : new ::NicaDataFormatManager;
   }
   static void *newArray_NicaDataFormatManager(Long_t nElements, void *p) {
      return p ? new(p) ::NicaDataFormatManager[nElements] : new ::NicaDataFormatManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaDataFormatManager(void *p) {
      delete ((::NicaDataFormatManager*)p);
   }
   static void deleteArray_NicaDataFormatManager(void *p) {
      delete [] ((::NicaDataFormatManager*)p);
   }
   static void destruct_NicaDataFormatManager(void *p) {
      typedef ::NicaDataFormatManager current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaDataFormatManager

//______________________________________________________________________________
void NicaTrackInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackInterface(void *p) {
      return  p ? new(p) ::NicaTrackInterface : new ::NicaTrackInterface;
   }
   static void *newArray_NicaTrackInterface(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackInterface[nElements] : new ::NicaTrackInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackInterface(void *p) {
      delete ((::NicaTrackInterface*)p);
   }
   static void deleteArray_NicaTrackInterface(void *p) {
      delete [] ((::NicaTrackInterface*)p);
   }
   static void destruct_NicaTrackInterface(void *p) {
      typedef ::NicaTrackInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackInterface

//______________________________________________________________________________
void NicaEventInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEventInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEventInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEventInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaEventInterface(void *p) {
      delete ((::NicaEventInterface*)p);
   }
   static void deleteArray_NicaEventInterface(void *p) {
      delete [] ((::NicaEventInterface*)p);
   }
   static void destruct_NicaEventInterface(void *p) {
      typedef ::NicaEventInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEventInterface

//______________________________________________________________________________
void NicaEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaEvent(void *p) {
      delete ((::NicaEvent*)p);
   }
   static void deleteArray_NicaEvent(void *p) {
      delete [] ((::NicaEvent*)p);
   }
   static void destruct_NicaEvent(void *p) {
      typedef ::NicaEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaEvent

//______________________________________________________________________________
void NicaTwoTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTwoTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTwoTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTwoTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTwoTrack(void *p) {
      return  p ? new(p) ::NicaTwoTrack : new ::NicaTwoTrack;
   }
   static void *newArray_NicaTwoTrack(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTwoTrack[nElements] : new ::NicaTwoTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTwoTrack(void *p) {
      delete ((::NicaTwoTrack*)p);
   }
   static void deleteArray_NicaTwoTrack(void *p) {
      delete [] ((::NicaTwoTrack*)p);
   }
   static void destruct_NicaTwoTrack(void *p) {
      typedef ::NicaTwoTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTwoTrack

//______________________________________________________________________________
void NicaMCEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaMCEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaMCEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaMCEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaMCEvent(void *p) {
      delete ((::NicaMCEvent*)p);
   }
   static void deleteArray_NicaMCEvent(void *p) {
      delete [] ((::NicaMCEvent*)p);
   }
   static void destruct_NicaMCEvent(void *p) {
      typedef ::NicaMCEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaMCEvent

//______________________________________________________________________________
void NicaMCTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaMCTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaMCTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaMCTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaMCTrack(void *p) {
      return  p ? new(p) ::NicaMCTrack : new ::NicaMCTrack;
   }
   static void *newArray_NicaMCTrack(Long_t nElements, void *p) {
      return p ? new(p) ::NicaMCTrack[nElements] : new ::NicaMCTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaMCTrack(void *p) {
      delete ((::NicaMCTrack*)p);
   }
   static void deleteArray_NicaMCTrack(void *p) {
      delete [] ((::NicaMCTrack*)p);
   }
   static void destruct_NicaMCTrack(void *p) {
      typedef ::NicaMCTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaMCTrack

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaMCEventInterface(void *p) {
      delete ((::NicaMCEventInterface*)p);
   }
   static void deleteArray_NicaMCEventInterface(void *p) {
      delete [] ((::NicaMCEventInterface*)p);
   }
   static void destruct_NicaMCEventInterface(void *p) {
      typedef ::NicaMCEventInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaMCEventInterface

//______________________________________________________________________________
void NicaMCTrackInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaMCTrackInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaMCTrackInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaMCTrackInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaMCTrackInterface(void *p) {
      return  p ? new(p) ::NicaMCTrackInterface : new ::NicaMCTrackInterface;
   }
   static void *newArray_NicaMCTrackInterface(Long_t nElements, void *p) {
      return p ? new(p) ::NicaMCTrackInterface[nElements] : new ::NicaMCTrackInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaMCTrackInterface(void *p) {
      delete ((::NicaMCTrackInterface*)p);
   }
   static void deleteArray_NicaMCTrackInterface(void *p) {
      delete [] ((::NicaMCTrackInterface*)p);
   }
   static void destruct_NicaMCTrackInterface(void *p) {
      typedef ::NicaMCTrackInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaMCTrackInterface

//______________________________________________________________________________
void NicaVirtualTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaVirtualTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaVirtualTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaVirtualTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaVirtualTrack(void *p) {
      return  p ? new(p) ::NicaVirtualTrack : new ::NicaVirtualTrack;
   }
   static void *newArray_NicaVirtualTrack(Long_t nElements, void *p) {
      return p ? new(p) ::NicaVirtualTrack[nElements] : new ::NicaVirtualTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaVirtualTrack(void *p) {
      delete ((::NicaVirtualTrack*)p);
   }
   static void deleteArray_NicaVirtualTrack(void *p) {
      delete [] ((::NicaVirtualTrack*)p);
   }
   static void destruct_NicaVirtualTrack(void *p) {
      typedef ::NicaVirtualTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaVirtualTrack

//______________________________________________________________________________
void NicaVirtualEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaVirtualEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaVirtualEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaVirtualEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaVirtualEvent(void *p) {
      return  p ? new(p) ::NicaVirtualEvent : new ::NicaVirtualEvent;
   }
   static void *newArray_NicaVirtualEvent(Long_t nElements, void *p) {
      return p ? new(p) ::NicaVirtualEvent[nElements] : new ::NicaVirtualEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaVirtualEvent(void *p) {
      delete ((::NicaVirtualEvent*)p);
   }
   static void deleteArray_NicaVirtualEvent(void *p) {
      delete [] ((::NicaVirtualEvent*)p);
   }
   static void destruct_NicaVirtualEvent(void *p) {
      typedef ::NicaVirtualEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaVirtualEvent

//______________________________________________________________________________
void NicaTrackClones::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTrackClones.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTrackClones::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTrackClones::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTrackClones(void *p) {
      return  p ? new(p) ::NicaTrackClones : new ::NicaTrackClones;
   }
   static void *newArray_NicaTrackClones(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTrackClones[nElements] : new ::NicaTrackClones[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTrackClones(void *p) {
      delete ((::NicaTrackClones*)p);
   }
   static void deleteArray_NicaTrackClones(void *p) {
      delete [] ((::NicaTrackClones*)p);
   }
   static void destruct_NicaTrackClones(void *p) {
      typedef ::NicaTrackClones current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTrackClones

//______________________________________________________________________________
void NicaComplexEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaComplexEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaComplexEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaComplexEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaComplexEvent(void *p) {
      return  p ? new(p) ::NicaComplexEvent : new ::NicaComplexEvent;
   }
   static void *newArray_NicaComplexEvent(Long_t nElements, void *p) {
      return p ? new(p) ::NicaComplexEvent[nElements] : new ::NicaComplexEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaComplexEvent(void *p) {
      delete ((::NicaComplexEvent*)p);
   }
   static void deleteArray_NicaComplexEvent(void *p) {
      delete [] ((::NicaComplexEvent*)p);
   }
   static void destruct_NicaComplexEvent(void *p) {
      typedef ::NicaComplexEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaComplexEvent

//______________________________________________________________________________
void NicaComplexTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaComplexTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaComplexTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaComplexTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaComplexTrack(void *p) {
      return  p ? new(p) ::NicaComplexTrack : new ::NicaComplexTrack;
   }
   static void *newArray_NicaComplexTrack(Long_t nElements, void *p) {
      return p ? new(p) ::NicaComplexTrack[nElements] : new ::NicaComplexTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaComplexTrack(void *p) {
      delete ((::NicaComplexTrack*)p);
   }
   static void deleteArray_NicaComplexTrack(void *p) {
      delete [] ((::NicaComplexTrack*)p);
   }
   static void destruct_NicaComplexTrack(void *p) {
      typedef ::NicaComplexTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaComplexTrack

//______________________________________________________________________________
void NicaSmearedEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaSmearedEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaSmearedEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaSmearedEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaSmearedEvent(void *p) {
      return  p ? new(p) ::NicaSmearedEvent : new ::NicaSmearedEvent;
   }
   static void *newArray_NicaSmearedEvent(Long_t nElements, void *p) {
      return p ? new(p) ::NicaSmearedEvent[nElements] : new ::NicaSmearedEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaSmearedEvent(void *p) {
      delete ((::NicaSmearedEvent*)p);
   }
   static void deleteArray_NicaSmearedEvent(void *p) {
      delete [] ((::NicaSmearedEvent*)p);
   }
   static void destruct_NicaSmearedEvent(void *p) {
      typedef ::NicaSmearedEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaSmearedEvent

//______________________________________________________________________________
void NicaSmearedTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaSmearedTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaSmearedTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaSmearedTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaSmearedTrack(void *p) {
      return  p ? new(p) ::NicaSmearedTrack : new ::NicaSmearedTrack;
   }
   static void *newArray_NicaSmearedTrack(Long_t nElements, void *p) {
      return p ? new(p) ::NicaSmearedTrack[nElements] : new ::NicaSmearedTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaSmearedTrack(void *p) {
      delete ((::NicaSmearedTrack*)p);
   }
   static void deleteArray_NicaSmearedTrack(void *p) {
      delete [] ((::NicaSmearedTrack*)p);
   }
   static void destruct_NicaSmearedTrack(void *p) {
      typedef ::NicaSmearedTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaSmearedTrack

//______________________________________________________________________________
void NicaDetectorTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaDetectorTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaDetectorTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaDetectorTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaDetectorTrack(void *p) {
      return  p ? new(p) ::NicaDetectorTrack : new ::NicaDetectorTrack;
   }
   static void *newArray_NicaDetectorTrack(Long_t nElements, void *p) {
      return p ? new(p) ::NicaDetectorTrack[nElements] : new ::NicaDetectorTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaDetectorTrack(void *p) {
      delete ((::NicaDetectorTrack*)p);
   }
   static void deleteArray_NicaDetectorTrack(void *p) {
      delete [] ((::NicaDetectorTrack*)p);
   }
   static void destruct_NicaDetectorTrack(void *p) {
      typedef ::NicaDetectorTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaDetectorTrack

//______________________________________________________________________________
void NicaToFTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaToFTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaToFTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaToFTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaToFTrack(void *p) {
      return  p ? new(p) ::NicaToFTrack : new ::NicaToFTrack;
   }
   static void *newArray_NicaToFTrack(Long_t nElements, void *p) {
      return p ? new(p) ::NicaToFTrack[nElements] : new ::NicaToFTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaToFTrack(void *p) {
      delete ((::NicaToFTrack*)p);
   }
   static void deleteArray_NicaToFTrack(void *p) {
      delete [] ((::NicaToFTrack*)p);
   }
   static void destruct_NicaToFTrack(void *p) {
      typedef ::NicaToFTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaToFTrack

//______________________________________________________________________________
void NicaTpcTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaTpcTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaTpcTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaTpcTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaTpcTrack(void *p) {
      return  p ? new(p) ::NicaTpcTrack : new ::NicaTpcTrack;
   }
   static void *newArray_NicaTpcTrack(Long_t nElements, void *p) {
      return p ? new(p) ::NicaTpcTrack[nElements] : new ::NicaTpcTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaTpcTrack(void *p) {
      delete ((::NicaTpcTrack*)p);
   }
   static void deleteArray_NicaTpcTrack(void *p) {
      delete [] ((::NicaTpcTrack*)p);
   }
   static void destruct_NicaTpcTrack(void *p) {
      typedef ::NicaTpcTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaTpcTrack

//______________________________________________________________________________
void NicaExpTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaExpTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaExpTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaExpTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaExpTrack(void *p) {
      return  p ? new(p) ::NicaExpTrack : new ::NicaExpTrack;
   }
   static void *newArray_NicaExpTrack(Long_t nElements, void *p) {
      return p ? new(p) ::NicaExpTrack[nElements] : new ::NicaExpTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaExpTrack(void *p) {
      delete ((::NicaExpTrack*)p);
   }
   static void deleteArray_NicaExpTrack(void *p) {
      delete [] ((::NicaExpTrack*)p);
   }
   static void destruct_NicaExpTrack(void *p) {
      typedef ::NicaExpTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaExpTrack

//______________________________________________________________________________
void NicaExpTrackHelix::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaExpTrackHelix.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaExpTrackHelix::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaExpTrackHelix::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaExpTrackHelix(void *p) {
      return  p ? new(p) ::NicaExpTrackHelix : new ::NicaExpTrackHelix;
   }
   static void *newArray_NicaExpTrackHelix(Long_t nElements, void *p) {
      return p ? new(p) ::NicaExpTrackHelix[nElements] : new ::NicaExpTrackHelix[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaExpTrackHelix(void *p) {
      delete ((::NicaExpTrackHelix*)p);
   }
   static void deleteArray_NicaExpTrackHelix(void *p) {
      delete [] ((::NicaExpTrackHelix*)p);
   }
   static void destruct_NicaExpTrackHelix(void *p) {
      typedef ::NicaExpTrackHelix current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaExpTrackHelix

//______________________________________________________________________________
void NicaExpEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaExpEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaExpEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaExpEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaExpEvent(void *p) {
      delete ((::NicaExpEvent*)p);
   }
   static void deleteArray_NicaExpEvent(void *p) {
      delete [] ((::NicaExpEvent*)p);
   }
   static void destruct_NicaExpEvent(void *p) {
      typedef ::NicaExpEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaExpEvent

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaExpEventHelix(void *p) {
      delete ((::NicaExpEventHelix*)p);
   }
   static void deleteArray_NicaExpEventHelix(void *p) {
      delete [] ((::NicaExpEventHelix*)p);
   }
   static void destruct_NicaExpEventHelix(void *p) {
      typedef ::NicaExpEventHelix current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaExpEventHelix

//______________________________________________________________________________
void NicaExpEventInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaExpEventInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaExpEventInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaExpEventInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaExpEventInterface(void *p) {
      delete ((::NicaExpEventInterface*)p);
   }
   static void deleteArray_NicaExpEventInterface(void *p) {
      delete [] ((::NicaExpEventInterface*)p);
   }
   static void destruct_NicaExpEventInterface(void *p) {
      typedef ::NicaExpEventInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaExpEventInterface

//______________________________________________________________________________
void NicaDetectorEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaDetectorEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaDetectorEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaDetectorEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaDetectorEvent(void *p) {
      return  p ? new(p) ::NicaDetectorEvent : new ::NicaDetectorEvent;
   }
   static void *newArray_NicaDetectorEvent(Long_t nElements, void *p) {
      return p ? new(p) ::NicaDetectorEvent[nElements] : new ::NicaDetectorEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaDetectorEvent(void *p) {
      delete ((::NicaDetectorEvent*)p);
   }
   static void deleteArray_NicaDetectorEvent(void *p) {
      delete [] ((::NicaDetectorEvent*)p);
   }
   static void destruct_NicaDetectorEvent(void *p) {
      typedef ::NicaDetectorEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaDetectorEvent

namespace {
  void TriggerDictionaryInitialization_G__NicaDataFormatDict_Impl() {
    static const char* headers[] = {
"NicaDataFormatManager.h",
"NicaEvent.h",
"NicaTrack.h",
"NicaTwoTrack.h",
"NicaMCEvent.h",
"NicaMCTrack.h",
"NicaEventInterface.h",
"NicaMCEventInterface.h",
"NicaTrackInterface.h",
"NicaMCTrackInterface.h",
"NicaVirtualTrack.h",
"NicaVirtualEvent.h",
"NicaTrackClones.h",
"NicaDataFormat.h",
"NicaComplexEvent.h",
"NicaComplexTrack.h",
"NicaSmearedEvent.h",
"NicaSmearedTrack.h",
"NicaToFTrack.h",
"NicaTpcTrack.h",
"NicaExpTrack.h",
"NicaExpEvent.h",
"NicaExpEventInterface.h",
"NicaDetectorTrack.h",
"NicaDetectorEvent.h",
"NicaV0Track.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/hist",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io/html",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/std",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/minimizer",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/detector",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/compound",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/hiddeninfo",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/dataformat/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__NicaDataFormatDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$NicaV0Track.h")))  __attribute__((annotate("$clingAutoload$NicaDataFormatManager.h")))  NicaV0Track;
class __attribute__((annotate("$clingAutoload$NicaTrack.h")))  __attribute__((annotate("$clingAutoload$NicaDataFormatManager.h")))  NicaTrack;
class __attribute__((annotate("$clingAutoload$NicaDataFormatManager.h")))  NicaDataFormatManager;
class __attribute__((annotate("$clingAutoload$NicaTrackInterface.h")))  __attribute__((annotate("$clingAutoload$NicaEvent.h")))  NicaTrackInterface;
class __attribute__((annotate("$clingAutoload$NicaEventInterface.h")))  __attribute__((annotate("$clingAutoload$NicaEvent.h")))  NicaEventInterface;
class __attribute__((annotate("$clingAutoload$NicaEvent.h")))  NicaEvent;
class __attribute__((annotate("$clingAutoload$NicaTwoTrack.h")))  NicaTwoTrack;
class __attribute__((annotate("$clingAutoload$NicaMCEvent.h")))  NicaMCEvent;
class __attribute__((annotate("$clingAutoload$NicaMCTrack.h")))  NicaMCTrack;
class __attribute__((annotate("$clingAutoload$NicaMCEventInterface.h")))  NicaMCEventInterface;
class __attribute__((annotate("$clingAutoload$NicaMCTrackInterface.h")))  NicaMCTrackInterface;
class __attribute__((annotate("$clingAutoload$NicaVirtualTrack.h")))  NicaVirtualTrack;
class __attribute__((annotate("$clingAutoload$NicaVirtualEvent.h")))  NicaVirtualEvent;
class __attribute__((annotate("$clingAutoload$NicaTrackClones.h")))  NicaTrackClones;
class __attribute__((annotate("$clingAutoload$NicaComplexEvent.h")))  NicaComplexEvent;
class __attribute__((annotate("$clingAutoload$NicaComplexTrack.h")))  NicaComplexTrack;
class __attribute__((annotate("$clingAutoload$NicaSmearedEvent.h")))  NicaSmearedEvent;
class __attribute__((annotate("$clingAutoload$NicaSmearedTrack.h")))  NicaSmearedTrack;
class __attribute__((annotate("$clingAutoload$NicaDetectorTrack.h")))  __attribute__((annotate("$clingAutoload$NicaToFTrack.h")))  NicaDetectorTrack;
class __attribute__((annotate("$clingAutoload$NicaToFTrack.h")))  NicaToFTrack;
class __attribute__((annotate("$clingAutoload$NicaTpcTrack.h")))  NicaTpcTrack;
class __attribute__((annotate("$clingAutoload$NicaExpTrack.h")))  NicaExpTrack;
class __attribute__((annotate("$clingAutoload$NicaExpTrack.h")))  NicaExpTrackHelix;
class __attribute__((annotate("$clingAutoload$NicaExpEvent.h")))  NicaExpEvent;
class __attribute__((annotate("$clingAutoload$NicaExpEvent.h")))  NicaExpEventHelix;
class __attribute__((annotate("$clingAutoload$NicaExpEventInterface.h")))  NicaExpEventInterface;
class __attribute__((annotate("$clingAutoload$NicaDetectorEvent.h")))  NicaDetectorEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__NicaDataFormatDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "NicaDataFormatManager.h"
#include "NicaEvent.h"
#include "NicaTrack.h"
#include "NicaTwoTrack.h"
#include "NicaMCEvent.h"
#include "NicaMCTrack.h"
#include "NicaEventInterface.h"
#include "NicaMCEventInterface.h"
#include "NicaTrackInterface.h"
#include "NicaMCTrackInterface.h"
#include "NicaVirtualTrack.h"
#include "NicaVirtualEvent.h"
#include "NicaTrackClones.h"
#include "NicaDataFormat.h"
#include "NicaComplexEvent.h"
#include "NicaComplexTrack.h"
#include "NicaSmearedEvent.h"
#include "NicaSmearedTrack.h"
#include "NicaToFTrack.h"
#include "NicaTpcTrack.h"
#include "NicaExpTrack.h"
#include "NicaExpEvent.h"
#include "NicaExpEventInterface.h"
#include "NicaDetectorTrack.h"
#include "NicaDetectorEvent.h"
#include "NicaV0Track.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"ENicaCutUpdate", payloadCode, "@",
"NicaComplexEvent", payloadCode, "@",
"NicaComplexTrack", payloadCode, "@",
"NicaDataFieldID::EComplexTrack", payloadCode, "@",
"NicaDataFieldID::EEvent", payloadCode, "@",
"NicaDataFieldID::EExpTrack", payloadCode, "@",
"NicaDataFieldID::EMcEvent", payloadCode, "@",
"NicaDataFieldID::EMcTrack", payloadCode, "@",
"NicaDataFieldID::ETrack", payloadCode, "@",
"NicaDataFormatManager", payloadCode, "@",
"NicaDetectorEvent", payloadCode, "@",
"NicaDetectorTrack", payloadCode, "@",
"NicaEvent", payloadCode, "@",
"NicaEventInterface", payloadCode, "@",
"NicaExpEvent", payloadCode, "@",
"NicaExpEventHelix", payloadCode, "@",
"NicaExpEventInterface", payloadCode, "@",
"NicaExpTrack", payloadCode, "@",
"NicaExpTrackHelix", payloadCode, "@",
"NicaMCEvent", payloadCode, "@",
"NicaMCEventInterface", payloadCode, "@",
"NicaMCTrack", payloadCode, "@",
"NicaMCTrackInterface", payloadCode, "@",
"NicaSmearedEvent", payloadCode, "@",
"NicaSmearedTrack", payloadCode, "@",
"NicaToFTrack", payloadCode, "@",
"NicaTpcTrack", payloadCode, "@",
"NicaTrack", payloadCode, "@",
"NicaTrackClones", payloadCode, "@",
"NicaTrackInterface", payloadCode, "@",
"NicaTwoTrack", payloadCode, "@",
"NicaV0Track", payloadCode, "@",
"NicaVirtualEvent", payloadCode, "@",
"NicaVirtualTrack", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__NicaDataFormatDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__NicaDataFormatDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__NicaDataFormatDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__NicaDataFormatDict() {
  TriggerDictionaryInitialization_G__NicaDataFormatDict_Impl();
}
