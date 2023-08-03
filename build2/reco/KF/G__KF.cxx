// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__KF
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
#include "CbmKF.h"
#include "CbmKFFieldMath.h"
#include "CbmKFHit.h"
#include "CbmKFMaterial.h"
#include "CbmKFMath.h"
#include "CbmKFPixelMeasurement.h"
#include "CbmKFPrimaryVertexFinder.h"
#include "CbmKFSecondaryVertexFinder.h"
#include "CbmKFTrackInterface.h"
#include "CbmKFUMeasurement.h"
#include "CbmKFVertexInterface.h"
#include "CbmTrackingDetectorInterfaceInit.h"
#include "CbmKFStsHit.h"
#include "CbmKFTrack.h"
#include "CbmKFTrdHit.h"
#include "CbmKFTofHit.h"
#include "CbmPVFinderKF.h"
#include "CbmStsFitPerformanceTask.h"
#include "CbmStsKFTrackFitter.h"
#include "CbmStsKFSecondaryVertexFinder.h"
#include "CbmTrdTrackFitterKF.h"
#include "CbmTofTrackFitterKF.h"
#include "CbmGlobalTrackFitterKF.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmKFTrackInterface(void *p = nullptr);
   static void *newArray_CbmKFTrackInterface(Long_t size, void *p);
   static void delete_CbmKFTrackInterface(void *p);
   static void deleteArray_CbmKFTrackInterface(void *p);
   static void destruct_CbmKFTrackInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFTrackInterface*)
   {
      ::CbmKFTrackInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFTrackInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFTrackInterface", ::CbmKFTrackInterface::Class_Version(), "CbmKFTrackInterface.h", 29,
                  typeid(::CbmKFTrackInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFTrackInterface::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFTrackInterface) );
      instance.SetNew(&new_CbmKFTrackInterface);
      instance.SetNewArray(&newArray_CbmKFTrackInterface);
      instance.SetDelete(&delete_CbmKFTrackInterface);
      instance.SetDeleteArray(&deleteArray_CbmKFTrackInterface);
      instance.SetDestructor(&destruct_CbmKFTrackInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFTrackInterface*)
   {
      return GenerateInitInstanceLocal((::CbmKFTrackInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFTrackInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFMaterial(void *p = nullptr);
   static void *newArray_CbmKFMaterial(Long_t size, void *p);
   static void delete_CbmKFMaterial(void *p);
   static void deleteArray_CbmKFMaterial(void *p);
   static void destruct_CbmKFMaterial(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFMaterial*)
   {
      ::CbmKFMaterial *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFMaterial >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFMaterial", ::CbmKFMaterial::Class_Version(), "CbmKFMaterial.h", 21,
                  typeid(::CbmKFMaterial), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFMaterial::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFMaterial) );
      instance.SetNew(&new_CbmKFMaterial);
      instance.SetNewArray(&newArray_CbmKFMaterial);
      instance.SetDelete(&delete_CbmKFMaterial);
      instance.SetDeleteArray(&deleteArray_CbmKFMaterial);
      instance.SetDestructor(&destruct_CbmKFMaterial);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFMaterial*)
   {
      return GenerateInitInstanceLocal((::CbmKFMaterial*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFMaterial*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFWall(void *p = nullptr);
   static void *newArray_CbmKFWall(Long_t size, void *p);
   static void delete_CbmKFWall(void *p);
   static void deleteArray_CbmKFWall(void *p);
   static void destruct_CbmKFWall(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFWall*)
   {
      ::CbmKFWall *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFWall >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFWall", ::CbmKFWall::Class_Version(), "CbmKFMaterial.h", 58,
                  typeid(::CbmKFWall), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFWall::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFWall) );
      instance.SetNew(&new_CbmKFWall);
      instance.SetNewArray(&newArray_CbmKFWall);
      instance.SetDelete(&delete_CbmKFWall);
      instance.SetDeleteArray(&deleteArray_CbmKFWall);
      instance.SetDestructor(&destruct_CbmKFWall);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFWall*)
   {
      return GenerateInitInstanceLocal((::CbmKFWall*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFWall*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFTube(void *p = nullptr);
   static void *newArray_CbmKFTube(Long_t size, void *p);
   static void delete_CbmKFTube(void *p);
   static void deleteArray_CbmKFTube(void *p);
   static void destruct_CbmKFTube(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFTube*)
   {
      ::CbmKFTube *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFTube >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFTube", ::CbmKFTube::Class_Version(), "CbmKFMaterial.h", 66,
                  typeid(::CbmKFTube), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFTube::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFTube) );
      instance.SetNew(&new_CbmKFTube);
      instance.SetNewArray(&newArray_CbmKFTube);
      instance.SetDelete(&delete_CbmKFTube);
      instance.SetDeleteArray(&deleteArray_CbmKFTube);
      instance.SetDestructor(&destruct_CbmKFTube);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFTube*)
   {
      return GenerateInitInstanceLocal((::CbmKFTube*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFTube*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFBox(void *p = nullptr);
   static void *newArray_CbmKFBox(Long_t size, void *p);
   static void delete_CbmKFBox(void *p);
   static void deleteArray_CbmKFBox(void *p);
   static void destruct_CbmKFBox(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFBox*)
   {
      ::CbmKFBox *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFBox >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFBox", ::CbmKFBox::Class_Version(), "CbmKFMaterial.h", 87,
                  typeid(::CbmKFBox), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFBox::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFBox) );
      instance.SetNew(&new_CbmKFBox);
      instance.SetNewArray(&newArray_CbmKFBox);
      instance.SetDelete(&delete_CbmKFBox);
      instance.SetDeleteArray(&deleteArray_CbmKFBox);
      instance.SetDestructor(&destruct_CbmKFBox);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFBox*)
   {
      return GenerateInitInstanceLocal((::CbmKFBox*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFBox*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFCone(void *p = nullptr);
   static void *newArray_CbmKFCone(Long_t size, void *p);
   static void delete_CbmKFCone(void *p);
   static void deleteArray_CbmKFCone(void *p);
   static void destruct_CbmKFCone(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFCone*)
   {
      ::CbmKFCone *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFCone >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFCone", ::CbmKFCone::Class_Version(), "CbmKFMaterial.h", 102,
                  typeid(::CbmKFCone), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFCone::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFCone) );
      instance.SetNew(&new_CbmKFCone);
      instance.SetNewArray(&newArray_CbmKFCone);
      instance.SetDelete(&delete_CbmKFCone);
      instance.SetDeleteArray(&deleteArray_CbmKFCone);
      instance.SetDestructor(&destruct_CbmKFCone);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFCone*)
   {
      return GenerateInitInstanceLocal((::CbmKFCone*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFCone*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKF(void *p = nullptr);
   static void *newArray_CbmKF(Long_t size, void *p);
   static void delete_CbmKF(void *p);
   static void deleteArray_CbmKF(void *p);
   static void destruct_CbmKF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKF*)
   {
      ::CbmKF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKF", ::CbmKF::Class_Version(), "CbmKF.h", 36,
                  typeid(::CbmKF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKF::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKF) );
      instance.SetNew(&new_CbmKF);
      instance.SetNewArray(&newArray_CbmKF);
      instance.SetDelete(&delete_CbmKF);
      instance.SetDeleteArray(&deleteArray_CbmKF);
      instance.SetDestructor(&destruct_CbmKF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKF*)
   {
      return GenerateInitInstanceLocal((::CbmKF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFFieldMath(void *p = nullptr);
   static void *newArray_CbmKFFieldMath(Long_t size, void *p);
   static void delete_CbmKFFieldMath(void *p);
   static void deleteArray_CbmKFFieldMath(void *p);
   static void destruct_CbmKFFieldMath(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFFieldMath*)
   {
      ::CbmKFFieldMath *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFFieldMath >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFFieldMath", ::CbmKFFieldMath::Class_Version(), "CbmKFFieldMath.h", 23,
                  typeid(::CbmKFFieldMath), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFFieldMath::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFFieldMath) );
      instance.SetNew(&new_CbmKFFieldMath);
      instance.SetNewArray(&newArray_CbmKFFieldMath);
      instance.SetDelete(&delete_CbmKFFieldMath);
      instance.SetDeleteArray(&deleteArray_CbmKFFieldMath);
      instance.SetDestructor(&destruct_CbmKFFieldMath);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFFieldMath*)
   {
      return GenerateInitInstanceLocal((::CbmKFFieldMath*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFFieldMath*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmKFHit(void *p);
   static void deleteArray_CbmKFHit(void *p);
   static void destruct_CbmKFHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFHit*)
   {
      ::CbmKFHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFHit", ::CbmKFHit::Class_Version(), "CbmKFHit.h", 20,
                  typeid(::CbmKFHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFHit::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFHit) );
      instance.SetDelete(&delete_CbmKFHit);
      instance.SetDeleteArray(&deleteArray_CbmKFHit);
      instance.SetDestructor(&destruct_CbmKFHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFHit*)
   {
      return GenerateInitInstanceLocal((::CbmKFHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFMath(void *p = nullptr);
   static void *newArray_CbmKFMath(Long_t size, void *p);
   static void delete_CbmKFMath(void *p);
   static void deleteArray_CbmKFMath(void *p);
   static void destruct_CbmKFMath(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFMath*)
   {
      ::CbmKFMath *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFMath >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFMath", ::CbmKFMath::Class_Version(), "CbmKFMath.h", 23,
                  typeid(::CbmKFMath), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFMath::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFMath) );
      instance.SetNew(&new_CbmKFMath);
      instance.SetNewArray(&newArray_CbmKFMath);
      instance.SetDelete(&delete_CbmKFMath);
      instance.SetDeleteArray(&deleteArray_CbmKFMath);
      instance.SetDestructor(&destruct_CbmKFMath);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFMath*)
   {
      return GenerateInitInstanceLocal((::CbmKFMath*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFMath*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFPixelMeasurement(void *p = nullptr);
   static void *newArray_CbmKFPixelMeasurement(Long_t size, void *p);
   static void delete_CbmKFPixelMeasurement(void *p);
   static void deleteArray_CbmKFPixelMeasurement(void *p);
   static void destruct_CbmKFPixelMeasurement(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFPixelMeasurement*)
   {
      ::CbmKFPixelMeasurement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFPixelMeasurement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFPixelMeasurement", ::CbmKFPixelMeasurement::Class_Version(), "CbmKFPixelMeasurement.h", 22,
                  typeid(::CbmKFPixelMeasurement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFPixelMeasurement::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFPixelMeasurement) );
      instance.SetNew(&new_CbmKFPixelMeasurement);
      instance.SetNewArray(&newArray_CbmKFPixelMeasurement);
      instance.SetDelete(&delete_CbmKFPixelMeasurement);
      instance.SetDeleteArray(&deleteArray_CbmKFPixelMeasurement);
      instance.SetDestructor(&destruct_CbmKFPixelMeasurement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFPixelMeasurement*)
   {
      return GenerateInitInstanceLocal((::CbmKFPixelMeasurement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFPixelMeasurement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFVertexInterface(void *p = nullptr);
   static void *newArray_CbmKFVertexInterface(Long_t size, void *p);
   static void delete_CbmKFVertexInterface(void *p);
   static void deleteArray_CbmKFVertexInterface(void *p);
   static void destruct_CbmKFVertexInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFVertexInterface*)
   {
      ::CbmKFVertexInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFVertexInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFVertexInterface", ::CbmKFVertexInterface::Class_Version(), "CbmKFVertexInterface.h", 27,
                  typeid(::CbmKFVertexInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFVertexInterface::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFVertexInterface) );
      instance.SetNew(&new_CbmKFVertexInterface);
      instance.SetNewArray(&newArray_CbmKFVertexInterface);
      instance.SetDelete(&delete_CbmKFVertexInterface);
      instance.SetDeleteArray(&deleteArray_CbmKFVertexInterface);
      instance.SetDestructor(&destruct_CbmKFVertexInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFVertexInterface*)
   {
      return GenerateInitInstanceLocal((::CbmKFVertexInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFVertexInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFPrimaryVertexFinder(void *p = nullptr);
   static void *newArray_CbmKFPrimaryVertexFinder(Long_t size, void *p);
   static void delete_CbmKFPrimaryVertexFinder(void *p);
   static void deleteArray_CbmKFPrimaryVertexFinder(void *p);
   static void destruct_CbmKFPrimaryVertexFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFPrimaryVertexFinder*)
   {
      ::CbmKFPrimaryVertexFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFPrimaryVertexFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFPrimaryVertexFinder", ::CbmKFPrimaryVertexFinder::Class_Version(), "CbmKFPrimaryVertexFinder.h", 22,
                  typeid(::CbmKFPrimaryVertexFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFPrimaryVertexFinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFPrimaryVertexFinder) );
      instance.SetNew(&new_CbmKFPrimaryVertexFinder);
      instance.SetNewArray(&newArray_CbmKFPrimaryVertexFinder);
      instance.SetDelete(&delete_CbmKFPrimaryVertexFinder);
      instance.SetDeleteArray(&deleteArray_CbmKFPrimaryVertexFinder);
      instance.SetDestructor(&destruct_CbmKFPrimaryVertexFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFPrimaryVertexFinder*)
   {
      return GenerateInitInstanceLocal((::CbmKFPrimaryVertexFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFPrimaryVertexFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFSecondaryVertexFinder(void *p = nullptr);
   static void *newArray_CbmKFSecondaryVertexFinder(Long_t size, void *p);
   static void delete_CbmKFSecondaryVertexFinder(void *p);
   static void deleteArray_CbmKFSecondaryVertexFinder(void *p);
   static void destruct_CbmKFSecondaryVertexFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFSecondaryVertexFinder*)
   {
      ::CbmKFSecondaryVertexFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFSecondaryVertexFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFSecondaryVertexFinder", ::CbmKFSecondaryVertexFinder::Class_Version(), "CbmKFSecondaryVertexFinder.h", 22,
                  typeid(::CbmKFSecondaryVertexFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFSecondaryVertexFinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFSecondaryVertexFinder) );
      instance.SetNew(&new_CbmKFSecondaryVertexFinder);
      instance.SetNewArray(&newArray_CbmKFSecondaryVertexFinder);
      instance.SetDelete(&delete_CbmKFSecondaryVertexFinder);
      instance.SetDeleteArray(&deleteArray_CbmKFSecondaryVertexFinder);
      instance.SetDestructor(&destruct_CbmKFSecondaryVertexFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFSecondaryVertexFinder*)
   {
      return GenerateInitInstanceLocal((::CbmKFSecondaryVertexFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFSecondaryVertexFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFUMeasurement(void *p = nullptr);
   static void *newArray_CbmKFUMeasurement(Long_t size, void *p);
   static void delete_CbmKFUMeasurement(void *p);
   static void deleteArray_CbmKFUMeasurement(void *p);
   static void destruct_CbmKFUMeasurement(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFUMeasurement*)
   {
      ::CbmKFUMeasurement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFUMeasurement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFUMeasurement", ::CbmKFUMeasurement::Class_Version(), "CbmKFUMeasurement.h", 20,
                  typeid(::CbmKFUMeasurement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFUMeasurement::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFUMeasurement) );
      instance.SetNew(&new_CbmKFUMeasurement);
      instance.SetNewArray(&newArray_CbmKFUMeasurement);
      instance.SetDelete(&delete_CbmKFUMeasurement);
      instance.SetDeleteArray(&deleteArray_CbmKFUMeasurement);
      instance.SetDestructor(&destruct_CbmKFUMeasurement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFUMeasurement*)
   {
      return GenerateInitInstanceLocal((::CbmKFUMeasurement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFUMeasurement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrackingDetectorInterfaceInit(void *p = nullptr);
   static void *newArray_CbmTrackingDetectorInterfaceInit(Long_t size, void *p);
   static void delete_CbmTrackingDetectorInterfaceInit(void *p);
   static void deleteArray_CbmTrackingDetectorInterfaceInit(void *p);
   static void destruct_CbmTrackingDetectorInterfaceInit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrackingDetectorInterfaceInit*)
   {
      ::CbmTrackingDetectorInterfaceInit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrackingDetectorInterfaceInit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrackingDetectorInterfaceInit", ::CbmTrackingDetectorInterfaceInit::Class_Version(), "CbmTrackingDetectorInterfaceInit.h", 27,
                  typeid(::CbmTrackingDetectorInterfaceInit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrackingDetectorInterfaceInit::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrackingDetectorInterfaceInit) );
      instance.SetNew(&new_CbmTrackingDetectorInterfaceInit);
      instance.SetNewArray(&newArray_CbmTrackingDetectorInterfaceInit);
      instance.SetDelete(&delete_CbmTrackingDetectorInterfaceInit);
      instance.SetDeleteArray(&deleteArray_CbmTrackingDetectorInterfaceInit);
      instance.SetDestructor(&destruct_CbmTrackingDetectorInterfaceInit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrackingDetectorInterfaceInit*)
   {
      return GenerateInitInstanceLocal((::CbmTrackingDetectorInterfaceInit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrackingDetectorInterfaceInit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFStsHit(void *p = nullptr);
   static void *newArray_CbmKFStsHit(Long_t size, void *p);
   static void delete_CbmKFStsHit(void *p);
   static void deleteArray_CbmKFStsHit(void *p);
   static void destruct_CbmKFStsHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFStsHit*)
   {
      ::CbmKFStsHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFStsHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFStsHit", ::CbmKFStsHit::Class_Version(), "CbmKFStsHit.h", 15,
                  typeid(::CbmKFStsHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFStsHit::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFStsHit) );
      instance.SetNew(&new_CbmKFStsHit);
      instance.SetNewArray(&newArray_CbmKFStsHit);
      instance.SetDelete(&delete_CbmKFStsHit);
      instance.SetDeleteArray(&deleteArray_CbmKFStsHit);
      instance.SetDestructor(&destruct_CbmKFStsHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFStsHit*)
   {
      return GenerateInitInstanceLocal((::CbmKFStsHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFStsHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFTrack(void *p = nullptr);
   static void *newArray_CbmKFTrack(Long_t size, void *p);
   static void delete_CbmKFTrack(void *p);
   static void deleteArray_CbmKFTrack(void *p);
   static void destruct_CbmKFTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFTrack*)
   {
      ::CbmKFTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFTrack", ::CbmKFTrack::Class_Version(), "CbmKFTrack.h", 25,
                  typeid(::CbmKFTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFTrack::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFTrack) );
      instance.SetNew(&new_CbmKFTrack);
      instance.SetNewArray(&newArray_CbmKFTrack);
      instance.SetDelete(&delete_CbmKFTrack);
      instance.SetDeleteArray(&deleteArray_CbmKFTrack);
      instance.SetDestructor(&destruct_CbmKFTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFTrack*)
   {
      return GenerateInitInstanceLocal((::CbmKFTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFTrdHit(void *p = nullptr);
   static void *newArray_CbmKFTrdHit(Long_t size, void *p);
   static void delete_CbmKFTrdHit(void *p);
   static void deleteArray_CbmKFTrdHit(void *p);
   static void destruct_CbmKFTrdHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFTrdHit*)
   {
      ::CbmKFTrdHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFTrdHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFTrdHit", ::CbmKFTrdHit::Class_Version(), "CbmKFTrdHit.h", 26,
                  typeid(::CbmKFTrdHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFTrdHit::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFTrdHit) );
      instance.SetNew(&new_CbmKFTrdHit);
      instance.SetNewArray(&newArray_CbmKFTrdHit);
      instance.SetDelete(&delete_CbmKFTrdHit);
      instance.SetDeleteArray(&deleteArray_CbmKFTrdHit);
      instance.SetDestructor(&destruct_CbmKFTrdHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFTrdHit*)
   {
      return GenerateInitInstanceLocal((::CbmKFTrdHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFTrdHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFTofHit(void *p = nullptr);
   static void *newArray_CbmKFTofHit(Long_t size, void *p);
   static void delete_CbmKFTofHit(void *p);
   static void deleteArray_CbmKFTofHit(void *p);
   static void destruct_CbmKFTofHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFTofHit*)
   {
      ::CbmKFTofHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFTofHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFTofHit", ::CbmKFTofHit::Class_Version(), "CbmKFTofHit.h", 26,
                  typeid(::CbmKFTofHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFTofHit::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFTofHit) );
      instance.SetNew(&new_CbmKFTofHit);
      instance.SetNewArray(&newArray_CbmKFTofHit);
      instance.SetDelete(&delete_CbmKFTofHit);
      instance.SetDeleteArray(&deleteArray_CbmKFTofHit);
      instance.SetDestructor(&destruct_CbmKFTofHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFTofHit*)
   {
      return GenerateInitInstanceLocal((::CbmKFTofHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFTofHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPVFinderKF(void *p = nullptr);
   static void *newArray_CbmPVFinderKF(Long_t size, void *p);
   static void delete_CbmPVFinderKF(void *p);
   static void deleteArray_CbmPVFinderKF(void *p);
   static void destruct_CbmPVFinderKF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPVFinderKF*)
   {
      ::CbmPVFinderKF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPVFinderKF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPVFinderKF", ::CbmPVFinderKF::Class_Version(), "CbmPVFinderKF.h", 18,
                  typeid(::CbmPVFinderKF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPVFinderKF::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPVFinderKF) );
      instance.SetNew(&new_CbmPVFinderKF);
      instance.SetNewArray(&newArray_CbmPVFinderKF);
      instance.SetDelete(&delete_CbmPVFinderKF);
      instance.SetDeleteArray(&deleteArray_CbmPVFinderKF);
      instance.SetDestructor(&destruct_CbmPVFinderKF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPVFinderKF*)
   {
      return GenerateInitInstanceLocal((::CbmPVFinderKF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPVFinderKF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsKFTrackFitter(void *p = nullptr);
   static void *newArray_CbmStsKFTrackFitter(Long_t size, void *p);
   static void delete_CbmStsKFTrackFitter(void *p);
   static void deleteArray_CbmStsKFTrackFitter(void *p);
   static void destruct_CbmStsKFTrackFitter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsKFTrackFitter*)
   {
      ::CbmStsKFTrackFitter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsKFTrackFitter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsKFTrackFitter", ::CbmStsKFTrackFitter::Class_Version(), "CbmStsKFTrackFitter.h", 17,
                  typeid(::CbmStsKFTrackFitter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsKFTrackFitter::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsKFTrackFitter) );
      instance.SetNew(&new_CbmStsKFTrackFitter);
      instance.SetNewArray(&newArray_CbmStsKFTrackFitter);
      instance.SetDelete(&delete_CbmStsKFTrackFitter);
      instance.SetDeleteArray(&deleteArray_CbmStsKFTrackFitter);
      instance.SetDestructor(&destruct_CbmStsKFTrackFitter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsKFTrackFitter*)
   {
      return GenerateInitInstanceLocal((::CbmStsKFTrackFitter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsKFTrackFitter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsFitPerformanceTask(void *p = nullptr);
   static void *newArray_CbmStsFitPerformanceTask(Long_t size, void *p);
   static void delete_CbmStsFitPerformanceTask(void *p);
   static void deleteArray_CbmStsFitPerformanceTask(void *p);
   static void destruct_CbmStsFitPerformanceTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsFitPerformanceTask*)
   {
      ::CbmStsFitPerformanceTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsFitPerformanceTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsFitPerformanceTask", ::CbmStsFitPerformanceTask::Class_Version(), "CbmStsFitPerformanceTask.h", 36,
                  typeid(::CbmStsFitPerformanceTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsFitPerformanceTask::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsFitPerformanceTask) );
      instance.SetNew(&new_CbmStsFitPerformanceTask);
      instance.SetNewArray(&newArray_CbmStsFitPerformanceTask);
      instance.SetDelete(&delete_CbmStsFitPerformanceTask);
      instance.SetDeleteArray(&deleteArray_CbmStsFitPerformanceTask);
      instance.SetDestructor(&destruct_CbmStsFitPerformanceTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsFitPerformanceTask*)
   {
      return GenerateInitInstanceLocal((::CbmStsFitPerformanceTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsFitPerformanceTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmKFVertex(void *p = nullptr);
   static void *newArray_CbmKFVertex(Long_t size, void *p);
   static void delete_CbmKFVertex(void *p);
   static void deleteArray_CbmKFVertex(void *p);
   static void destruct_CbmKFVertex(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmKFVertex*)
   {
      ::CbmKFVertex *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmKFVertex >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmKFVertex", ::CbmKFVertex::Class_Version(), "CbmKFVertex.h", 10,
                  typeid(::CbmKFVertex), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmKFVertex::Dictionary, isa_proxy, 4,
                  sizeof(::CbmKFVertex) );
      instance.SetNew(&new_CbmKFVertex);
      instance.SetNewArray(&newArray_CbmKFVertex);
      instance.SetDelete(&delete_CbmKFVertex);
      instance.SetDeleteArray(&deleteArray_CbmKFVertex);
      instance.SetDestructor(&destruct_CbmKFVertex);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmKFVertex*)
   {
      return GenerateInitInstanceLocal((::CbmKFVertex*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmKFVertex*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsKFSecondaryVertexFinder(void *p = nullptr);
   static void *newArray_CbmStsKFSecondaryVertexFinder(Long_t size, void *p);
   static void delete_CbmStsKFSecondaryVertexFinder(void *p);
   static void deleteArray_CbmStsKFSecondaryVertexFinder(void *p);
   static void destruct_CbmStsKFSecondaryVertexFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsKFSecondaryVertexFinder*)
   {
      ::CbmStsKFSecondaryVertexFinder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsKFSecondaryVertexFinder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsKFSecondaryVertexFinder", ::CbmStsKFSecondaryVertexFinder::Class_Version(), "CbmStsKFSecondaryVertexFinder.h", 24,
                  typeid(::CbmStsKFSecondaryVertexFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsKFSecondaryVertexFinder::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsKFSecondaryVertexFinder) );
      instance.SetNew(&new_CbmStsKFSecondaryVertexFinder);
      instance.SetNewArray(&newArray_CbmStsKFSecondaryVertexFinder);
      instance.SetDelete(&delete_CbmStsKFSecondaryVertexFinder);
      instance.SetDeleteArray(&deleteArray_CbmStsKFSecondaryVertexFinder);
      instance.SetDestructor(&destruct_CbmStsKFSecondaryVertexFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsKFSecondaryVertexFinder*)
   {
      return GenerateInitInstanceLocal((::CbmStsKFSecondaryVertexFinder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsKFSecondaryVertexFinder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdTrackFitterKF(void *p = nullptr);
   static void *newArray_CbmTrdTrackFitterKF(Long_t size, void *p);
   static void delete_CbmTrdTrackFitterKF(void *p);
   static void deleteArray_CbmTrdTrackFitterKF(void *p);
   static void destruct_CbmTrdTrackFitterKF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdTrackFitterKF*)
   {
      ::CbmTrdTrackFitterKF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdTrackFitterKF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdTrackFitterKF", ::CbmTrdTrackFitterKF::Class_Version(), "CbmTrdTrackFitterKF.h", 19,
                  typeid(::CbmTrdTrackFitterKF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdTrackFitterKF::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdTrackFitterKF) );
      instance.SetNew(&new_CbmTrdTrackFitterKF);
      instance.SetNewArray(&newArray_CbmTrdTrackFitterKF);
      instance.SetDelete(&delete_CbmTrdTrackFitterKF);
      instance.SetDeleteArray(&deleteArray_CbmTrdTrackFitterKF);
      instance.SetDestructor(&destruct_CbmTrdTrackFitterKF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdTrackFitterKF*)
   {
      return GenerateInitInstanceLocal((::CbmTrdTrackFitterKF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdTrackFitterKF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofTrackFitterKF(void *p = nullptr);
   static void *newArray_CbmTofTrackFitterKF(Long_t size, void *p);
   static void delete_CbmTofTrackFitterKF(void *p);
   static void deleteArray_CbmTofTrackFitterKF(void *p);
   static void destruct_CbmTofTrackFitterKF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofTrackFitterKF*)
   {
      ::CbmTofTrackFitterKF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofTrackFitterKF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofTrackFitterKF", ::CbmTofTrackFitterKF::Class_Version(), "CbmTofTrackFitterKF.h", 20,
                  typeid(::CbmTofTrackFitterKF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofTrackFitterKF::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofTrackFitterKF) );
      instance.SetNew(&new_CbmTofTrackFitterKF);
      instance.SetNewArray(&newArray_CbmTofTrackFitterKF);
      instance.SetDelete(&delete_CbmTofTrackFitterKF);
      instance.SetDeleteArray(&deleteArray_CbmTofTrackFitterKF);
      instance.SetDestructor(&destruct_CbmTofTrackFitterKF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofTrackFitterKF*)
   {
      return GenerateInitInstanceLocal((::CbmTofTrackFitterKF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofTrackFitterKF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmGlobalTrackFitterKF(void *p = nullptr);
   static void *newArray_CbmGlobalTrackFitterKF(Long_t size, void *p);
   static void delete_CbmGlobalTrackFitterKF(void *p);
   static void deleteArray_CbmGlobalTrackFitterKF(void *p);
   static void destruct_CbmGlobalTrackFitterKF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGlobalTrackFitterKF*)
   {
      ::CbmGlobalTrackFitterKF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGlobalTrackFitterKF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGlobalTrackFitterKF", ::CbmGlobalTrackFitterKF::Class_Version(), "CbmGlobalTrackFitterKF.h", 19,
                  typeid(::CbmGlobalTrackFitterKF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGlobalTrackFitterKF::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGlobalTrackFitterKF) );
      instance.SetNew(&new_CbmGlobalTrackFitterKF);
      instance.SetNewArray(&newArray_CbmGlobalTrackFitterKF);
      instance.SetDelete(&delete_CbmGlobalTrackFitterKF);
      instance.SetDeleteArray(&deleteArray_CbmGlobalTrackFitterKF);
      instance.SetDestructor(&destruct_CbmGlobalTrackFitterKF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGlobalTrackFitterKF*)
   {
      return GenerateInitInstanceLocal((::CbmGlobalTrackFitterKF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGlobalTrackFitterKF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmKFTrackInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFTrackInterface::Class_Name()
{
   return "CbmKFTrackInterface";
}

//______________________________________________________________________________
const char *CbmKFTrackInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrackInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFTrackInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrackInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFTrackInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrackInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFTrackInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrackInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFMaterial::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFMaterial::Class_Name()
{
   return "CbmKFMaterial";
}

//______________________________________________________________________________
const char *CbmKFMaterial::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFMaterial*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFMaterial::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFMaterial*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFMaterial::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFMaterial*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFMaterial::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFMaterial*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFWall::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFWall::Class_Name()
{
   return "CbmKFWall";
}

//______________________________________________________________________________
const char *CbmKFWall::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFWall*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFWall::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFWall*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFWall::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFWall*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFWall::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFWall*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFTube::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFTube::Class_Name()
{
   return "CbmKFTube";
}

//______________________________________________________________________________
const char *CbmKFTube::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTube*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFTube::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTube*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFTube::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTube*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFTube::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTube*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFBox::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFBox::Class_Name()
{
   return "CbmKFBox";
}

//______________________________________________________________________________
const char *CbmKFBox::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFBox*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFBox::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFBox*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFBox::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFBox*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFBox::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFBox*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFCone::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFCone::Class_Name()
{
   return "CbmKFCone";
}

//______________________________________________________________________________
const char *CbmKFCone::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFCone*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFCone::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFCone*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFCone::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFCone*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFCone::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFCone*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKF::Class_Name()
{
   return "CbmKF";
}

//______________________________________________________________________________
const char *CbmKF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFFieldMath::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFFieldMath::Class_Name()
{
   return "CbmKFFieldMath";
}

//______________________________________________________________________________
const char *CbmKFFieldMath::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFFieldMath*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFFieldMath::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFFieldMath*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFFieldMath::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFFieldMath*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFFieldMath::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFFieldMath*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFHit::Class_Name()
{
   return "CbmKFHit";
}

//______________________________________________________________________________
const char *CbmKFHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFMath::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFMath::Class_Name()
{
   return "CbmKFMath";
}

//______________________________________________________________________________
const char *CbmKFMath::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFMath*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFMath::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFMath*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFMath::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFMath*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFMath::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFMath*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFPixelMeasurement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFPixelMeasurement::Class_Name()
{
   return "CbmKFPixelMeasurement";
}

//______________________________________________________________________________
const char *CbmKFPixelMeasurement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFPixelMeasurement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFPixelMeasurement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFPixelMeasurement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFPixelMeasurement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFPixelMeasurement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFPixelMeasurement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFPixelMeasurement*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFVertexInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFVertexInterface::Class_Name()
{
   return "CbmKFVertexInterface";
}

//______________________________________________________________________________
const char *CbmKFVertexInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFVertexInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFVertexInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFVertexInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFVertexInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFVertexInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFVertexInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFVertexInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFPrimaryVertexFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFPrimaryVertexFinder::Class_Name()
{
   return "CbmKFPrimaryVertexFinder";
}

//______________________________________________________________________________
const char *CbmKFPrimaryVertexFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFPrimaryVertexFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFPrimaryVertexFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFPrimaryVertexFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFPrimaryVertexFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFPrimaryVertexFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFPrimaryVertexFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFPrimaryVertexFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFSecondaryVertexFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFSecondaryVertexFinder::Class_Name()
{
   return "CbmKFSecondaryVertexFinder";
}

//______________________________________________________________________________
const char *CbmKFSecondaryVertexFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFSecondaryVertexFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFSecondaryVertexFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFSecondaryVertexFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFSecondaryVertexFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFSecondaryVertexFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFSecondaryVertexFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFSecondaryVertexFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFUMeasurement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFUMeasurement::Class_Name()
{
   return "CbmKFUMeasurement";
}

//______________________________________________________________________________
const char *CbmKFUMeasurement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFUMeasurement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFUMeasurement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFUMeasurement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFUMeasurement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFUMeasurement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFUMeasurement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFUMeasurement*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrackingDetectorInterfaceInit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrackingDetectorInterfaceInit::Class_Name()
{
   return "CbmTrackingDetectorInterfaceInit";
}

//______________________________________________________________________________
const char *CbmTrackingDetectorInterfaceInit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackingDetectorInterfaceInit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrackingDetectorInterfaceInit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackingDetectorInterfaceInit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrackingDetectorInterfaceInit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackingDetectorInterfaceInit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrackingDetectorInterfaceInit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackingDetectorInterfaceInit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFStsHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFStsHit::Class_Name()
{
   return "CbmKFStsHit";
}

//______________________________________________________________________________
const char *CbmKFStsHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFStsHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFStsHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFStsHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFStsHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFStsHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFStsHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFStsHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFTrack::Class_Name()
{
   return "CbmKFTrack";
}

//______________________________________________________________________________
const char *CbmKFTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFTrdHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFTrdHit::Class_Name()
{
   return "CbmKFTrdHit";
}

//______________________________________________________________________________
const char *CbmKFTrdHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrdHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFTrdHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrdHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFTrdHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrdHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFTrdHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTrdHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFTofHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFTofHit::Class_Name()
{
   return "CbmKFTofHit";
}

//______________________________________________________________________________
const char *CbmKFTofHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTofHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFTofHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTofHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFTofHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTofHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFTofHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFTofHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPVFinderKF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPVFinderKF::Class_Name()
{
   return "CbmPVFinderKF";
}

//______________________________________________________________________________
const char *CbmPVFinderKF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPVFinderKF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPVFinderKF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPVFinderKF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPVFinderKF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPVFinderKF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPVFinderKF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPVFinderKF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsKFTrackFitter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsKFTrackFitter::Class_Name()
{
   return "CbmStsKFTrackFitter";
}

//______________________________________________________________________________
const char *CbmStsKFTrackFitter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsKFTrackFitter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsKFTrackFitter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsKFTrackFitter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsKFTrackFitter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsKFTrackFitter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsKFTrackFitter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsKFTrackFitter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsFitPerformanceTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsFitPerformanceTask::Class_Name()
{
   return "CbmStsFitPerformanceTask";
}

//______________________________________________________________________________
const char *CbmStsFitPerformanceTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFitPerformanceTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsFitPerformanceTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFitPerformanceTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsFitPerformanceTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFitPerformanceTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsFitPerformanceTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsFitPerformanceTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmKFVertex::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmKFVertex::Class_Name()
{
   return "CbmKFVertex";
}

//______________________________________________________________________________
const char *CbmKFVertex::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFVertex*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmKFVertex::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmKFVertex*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmKFVertex::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFVertex*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmKFVertex::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmKFVertex*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsKFSecondaryVertexFinder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsKFSecondaryVertexFinder::Class_Name()
{
   return "CbmStsKFSecondaryVertexFinder";
}

//______________________________________________________________________________
const char *CbmStsKFSecondaryVertexFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsKFSecondaryVertexFinder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsKFSecondaryVertexFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsKFSecondaryVertexFinder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsKFSecondaryVertexFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsKFSecondaryVertexFinder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsKFSecondaryVertexFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsKFSecondaryVertexFinder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdTrackFitterKF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdTrackFitterKF::Class_Name()
{
   return "CbmTrdTrackFitterKF";
}

//______________________________________________________________________________
const char *CbmTrdTrackFitterKF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFitterKF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdTrackFitterKF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFitterKF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdTrackFitterKF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFitterKF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdTrackFitterKF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrackFitterKF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofTrackFitterKF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofTrackFitterKF::Class_Name()
{
   return "CbmTofTrackFitterKF";
}

//______________________________________________________________________________
const char *CbmTofTrackFitterKF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFitterKF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofTrackFitterKF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFitterKF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofTrackFitterKF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFitterKF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofTrackFitterKF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackFitterKF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGlobalTrackFitterKF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGlobalTrackFitterKF::Class_Name()
{
   return "CbmGlobalTrackFitterKF";
}

//______________________________________________________________________________
const char *CbmGlobalTrackFitterKF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackFitterKF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGlobalTrackFitterKF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackFitterKF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGlobalTrackFitterKF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackFitterKF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGlobalTrackFitterKF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrackFitterKF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmKFTrackInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFTrackInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFTrackInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFTrackInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFTrackInterface(void *p) {
      return  p ? new(p) ::CbmKFTrackInterface : new ::CbmKFTrackInterface;
   }
   static void *newArray_CbmKFTrackInterface(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFTrackInterface[nElements] : new ::CbmKFTrackInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFTrackInterface(void *p) {
      delete ((::CbmKFTrackInterface*)p);
   }
   static void deleteArray_CbmKFTrackInterface(void *p) {
      delete [] ((::CbmKFTrackInterface*)p);
   }
   static void destruct_CbmKFTrackInterface(void *p) {
      typedef ::CbmKFTrackInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFTrackInterface

//______________________________________________________________________________
void CbmKFMaterial::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFMaterial.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFMaterial::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFMaterial::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFMaterial(void *p) {
      return  p ? new(p) ::CbmKFMaterial : new ::CbmKFMaterial;
   }
   static void *newArray_CbmKFMaterial(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFMaterial[nElements] : new ::CbmKFMaterial[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFMaterial(void *p) {
      delete ((::CbmKFMaterial*)p);
   }
   static void deleteArray_CbmKFMaterial(void *p) {
      delete [] ((::CbmKFMaterial*)p);
   }
   static void destruct_CbmKFMaterial(void *p) {
      typedef ::CbmKFMaterial current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFMaterial

//______________________________________________________________________________
void CbmKFWall::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFWall.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFWall::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFWall::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFWall(void *p) {
      return  p ? new(p) ::CbmKFWall : new ::CbmKFWall;
   }
   static void *newArray_CbmKFWall(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFWall[nElements] : new ::CbmKFWall[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFWall(void *p) {
      delete ((::CbmKFWall*)p);
   }
   static void deleteArray_CbmKFWall(void *p) {
      delete [] ((::CbmKFWall*)p);
   }
   static void destruct_CbmKFWall(void *p) {
      typedef ::CbmKFWall current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFWall

//______________________________________________________________________________
void CbmKFTube::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFTube.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFTube::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFTube::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFTube(void *p) {
      return  p ? new(p) ::CbmKFTube : new ::CbmKFTube;
   }
   static void *newArray_CbmKFTube(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFTube[nElements] : new ::CbmKFTube[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFTube(void *p) {
      delete ((::CbmKFTube*)p);
   }
   static void deleteArray_CbmKFTube(void *p) {
      delete [] ((::CbmKFTube*)p);
   }
   static void destruct_CbmKFTube(void *p) {
      typedef ::CbmKFTube current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFTube

//______________________________________________________________________________
void CbmKFBox::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFBox.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFBox::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFBox::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFBox(void *p) {
      return  p ? new(p) ::CbmKFBox : new ::CbmKFBox;
   }
   static void *newArray_CbmKFBox(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFBox[nElements] : new ::CbmKFBox[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFBox(void *p) {
      delete ((::CbmKFBox*)p);
   }
   static void deleteArray_CbmKFBox(void *p) {
      delete [] ((::CbmKFBox*)p);
   }
   static void destruct_CbmKFBox(void *p) {
      typedef ::CbmKFBox current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFBox

//______________________________________________________________________________
void CbmKFCone::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFCone.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFCone::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFCone::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFCone(void *p) {
      return  p ? new(p) ::CbmKFCone : new ::CbmKFCone;
   }
   static void *newArray_CbmKFCone(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFCone[nElements] : new ::CbmKFCone[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFCone(void *p) {
      delete ((::CbmKFCone*)p);
   }
   static void deleteArray_CbmKFCone(void *p) {
      delete [] ((::CbmKFCone*)p);
   }
   static void destruct_CbmKFCone(void *p) {
      typedef ::CbmKFCone current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFCone

//______________________________________________________________________________
void CbmKF::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKF::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKF(void *p) {
      return  p ? new(p) ::CbmKF : new ::CbmKF;
   }
   static void *newArray_CbmKF(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKF[nElements] : new ::CbmKF[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKF(void *p) {
      delete ((::CbmKF*)p);
   }
   static void deleteArray_CbmKF(void *p) {
      delete [] ((::CbmKF*)p);
   }
   static void destruct_CbmKF(void *p) {
      typedef ::CbmKF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKF

//______________________________________________________________________________
void CbmKFFieldMath::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFFieldMath.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFFieldMath::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFFieldMath::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFFieldMath(void *p) {
      return  p ? new(p) ::CbmKFFieldMath : new ::CbmKFFieldMath;
   }
   static void *newArray_CbmKFFieldMath(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFFieldMath[nElements] : new ::CbmKFFieldMath[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFFieldMath(void *p) {
      delete ((::CbmKFFieldMath*)p);
   }
   static void deleteArray_CbmKFFieldMath(void *p) {
      delete [] ((::CbmKFFieldMath*)p);
   }
   static void destruct_CbmKFFieldMath(void *p) {
      typedef ::CbmKFFieldMath current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFFieldMath

//______________________________________________________________________________
void CbmKFHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFHit::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmKFHit(void *p) {
      delete ((::CbmKFHit*)p);
   }
   static void deleteArray_CbmKFHit(void *p) {
      delete [] ((::CbmKFHit*)p);
   }
   static void destruct_CbmKFHit(void *p) {
      typedef ::CbmKFHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFHit

//______________________________________________________________________________
void CbmKFMath::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFMath.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFMath::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFMath::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFMath(void *p) {
      return  p ? new(p) ::CbmKFMath : new ::CbmKFMath;
   }
   static void *newArray_CbmKFMath(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFMath[nElements] : new ::CbmKFMath[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFMath(void *p) {
      delete ((::CbmKFMath*)p);
   }
   static void deleteArray_CbmKFMath(void *p) {
      delete [] ((::CbmKFMath*)p);
   }
   static void destruct_CbmKFMath(void *p) {
      typedef ::CbmKFMath current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFMath

//______________________________________________________________________________
void CbmKFPixelMeasurement::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFPixelMeasurement.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFPixelMeasurement::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFPixelMeasurement::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFPixelMeasurement(void *p) {
      return  p ? new(p) ::CbmKFPixelMeasurement : new ::CbmKFPixelMeasurement;
   }
   static void *newArray_CbmKFPixelMeasurement(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFPixelMeasurement[nElements] : new ::CbmKFPixelMeasurement[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFPixelMeasurement(void *p) {
      delete ((::CbmKFPixelMeasurement*)p);
   }
   static void deleteArray_CbmKFPixelMeasurement(void *p) {
      delete [] ((::CbmKFPixelMeasurement*)p);
   }
   static void destruct_CbmKFPixelMeasurement(void *p) {
      typedef ::CbmKFPixelMeasurement current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFPixelMeasurement

//______________________________________________________________________________
void CbmKFVertexInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFVertexInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFVertexInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFVertexInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFVertexInterface(void *p) {
      return  p ? new(p) ::CbmKFVertexInterface : new ::CbmKFVertexInterface;
   }
   static void *newArray_CbmKFVertexInterface(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFVertexInterface[nElements] : new ::CbmKFVertexInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFVertexInterface(void *p) {
      delete ((::CbmKFVertexInterface*)p);
   }
   static void deleteArray_CbmKFVertexInterface(void *p) {
      delete [] ((::CbmKFVertexInterface*)p);
   }
   static void destruct_CbmKFVertexInterface(void *p) {
      typedef ::CbmKFVertexInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFVertexInterface

//______________________________________________________________________________
void CbmKFPrimaryVertexFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFPrimaryVertexFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFPrimaryVertexFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFPrimaryVertexFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFPrimaryVertexFinder(void *p) {
      return  p ? new(p) ::CbmKFPrimaryVertexFinder : new ::CbmKFPrimaryVertexFinder;
   }
   static void *newArray_CbmKFPrimaryVertexFinder(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFPrimaryVertexFinder[nElements] : new ::CbmKFPrimaryVertexFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFPrimaryVertexFinder(void *p) {
      delete ((::CbmKFPrimaryVertexFinder*)p);
   }
   static void deleteArray_CbmKFPrimaryVertexFinder(void *p) {
      delete [] ((::CbmKFPrimaryVertexFinder*)p);
   }
   static void destruct_CbmKFPrimaryVertexFinder(void *p) {
      typedef ::CbmKFPrimaryVertexFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFPrimaryVertexFinder

//______________________________________________________________________________
void CbmKFSecondaryVertexFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFSecondaryVertexFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFSecondaryVertexFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFSecondaryVertexFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFSecondaryVertexFinder(void *p) {
      return  p ? new(p) ::CbmKFSecondaryVertexFinder : new ::CbmKFSecondaryVertexFinder;
   }
   static void *newArray_CbmKFSecondaryVertexFinder(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFSecondaryVertexFinder[nElements] : new ::CbmKFSecondaryVertexFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFSecondaryVertexFinder(void *p) {
      delete ((::CbmKFSecondaryVertexFinder*)p);
   }
   static void deleteArray_CbmKFSecondaryVertexFinder(void *p) {
      delete [] ((::CbmKFSecondaryVertexFinder*)p);
   }
   static void destruct_CbmKFSecondaryVertexFinder(void *p) {
      typedef ::CbmKFSecondaryVertexFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFSecondaryVertexFinder

//______________________________________________________________________________
void CbmKFUMeasurement::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFUMeasurement.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFUMeasurement::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFUMeasurement::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFUMeasurement(void *p) {
      return  p ? new(p) ::CbmKFUMeasurement : new ::CbmKFUMeasurement;
   }
   static void *newArray_CbmKFUMeasurement(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFUMeasurement[nElements] : new ::CbmKFUMeasurement[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFUMeasurement(void *p) {
      delete ((::CbmKFUMeasurement*)p);
   }
   static void deleteArray_CbmKFUMeasurement(void *p) {
      delete [] ((::CbmKFUMeasurement*)p);
   }
   static void destruct_CbmKFUMeasurement(void *p) {
      typedef ::CbmKFUMeasurement current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFUMeasurement

//______________________________________________________________________________
void CbmTrackingDetectorInterfaceInit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrackingDetectorInterfaceInit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrackingDetectorInterfaceInit::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrackingDetectorInterfaceInit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrackingDetectorInterfaceInit(void *p) {
      return  p ? new(p) ::CbmTrackingDetectorInterfaceInit : new ::CbmTrackingDetectorInterfaceInit;
   }
   static void *newArray_CbmTrackingDetectorInterfaceInit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrackingDetectorInterfaceInit[nElements] : new ::CbmTrackingDetectorInterfaceInit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrackingDetectorInterfaceInit(void *p) {
      delete ((::CbmTrackingDetectorInterfaceInit*)p);
   }
   static void deleteArray_CbmTrackingDetectorInterfaceInit(void *p) {
      delete [] ((::CbmTrackingDetectorInterfaceInit*)p);
   }
   static void destruct_CbmTrackingDetectorInterfaceInit(void *p) {
      typedef ::CbmTrackingDetectorInterfaceInit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrackingDetectorInterfaceInit

//______________________________________________________________________________
void CbmKFStsHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFStsHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFStsHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFStsHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFStsHit(void *p) {
      return  p ? new(p) ::CbmKFStsHit : new ::CbmKFStsHit;
   }
   static void *newArray_CbmKFStsHit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFStsHit[nElements] : new ::CbmKFStsHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFStsHit(void *p) {
      delete ((::CbmKFStsHit*)p);
   }
   static void deleteArray_CbmKFStsHit(void *p) {
      delete [] ((::CbmKFStsHit*)p);
   }
   static void destruct_CbmKFStsHit(void *p) {
      typedef ::CbmKFStsHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFStsHit

//______________________________________________________________________________
void CbmKFTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFTrack(void *p) {
      return  p ? new(p) ::CbmKFTrack : new ::CbmKFTrack;
   }
   static void *newArray_CbmKFTrack(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFTrack[nElements] : new ::CbmKFTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFTrack(void *p) {
      delete ((::CbmKFTrack*)p);
   }
   static void deleteArray_CbmKFTrack(void *p) {
      delete [] ((::CbmKFTrack*)p);
   }
   static void destruct_CbmKFTrack(void *p) {
      typedef ::CbmKFTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFTrack

//______________________________________________________________________________
void CbmKFTrdHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFTrdHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFTrdHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFTrdHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFTrdHit(void *p) {
      return  p ? new(p) ::CbmKFTrdHit : new ::CbmKFTrdHit;
   }
   static void *newArray_CbmKFTrdHit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFTrdHit[nElements] : new ::CbmKFTrdHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFTrdHit(void *p) {
      delete ((::CbmKFTrdHit*)p);
   }
   static void deleteArray_CbmKFTrdHit(void *p) {
      delete [] ((::CbmKFTrdHit*)p);
   }
   static void destruct_CbmKFTrdHit(void *p) {
      typedef ::CbmKFTrdHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFTrdHit

//______________________________________________________________________________
void CbmKFTofHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFTofHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFTofHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFTofHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFTofHit(void *p) {
      return  p ? new(p) ::CbmKFTofHit : new ::CbmKFTofHit;
   }
   static void *newArray_CbmKFTofHit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFTofHit[nElements] : new ::CbmKFTofHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFTofHit(void *p) {
      delete ((::CbmKFTofHit*)p);
   }
   static void deleteArray_CbmKFTofHit(void *p) {
      delete [] ((::CbmKFTofHit*)p);
   }
   static void destruct_CbmKFTofHit(void *p) {
      typedef ::CbmKFTofHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFTofHit

//______________________________________________________________________________
void CbmPVFinderKF::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPVFinderKF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPVFinderKF::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPVFinderKF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPVFinderKF(void *p) {
      return  p ? new(p) ::CbmPVFinderKF : new ::CbmPVFinderKF;
   }
   static void *newArray_CbmPVFinderKF(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPVFinderKF[nElements] : new ::CbmPVFinderKF[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPVFinderKF(void *p) {
      delete ((::CbmPVFinderKF*)p);
   }
   static void deleteArray_CbmPVFinderKF(void *p) {
      delete [] ((::CbmPVFinderKF*)p);
   }
   static void destruct_CbmPVFinderKF(void *p) {
      typedef ::CbmPVFinderKF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPVFinderKF

//______________________________________________________________________________
void CbmStsKFTrackFitter::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsKFTrackFitter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsKFTrackFitter::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsKFTrackFitter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsKFTrackFitter(void *p) {
      return  p ? new(p) ::CbmStsKFTrackFitter : new ::CbmStsKFTrackFitter;
   }
   static void *newArray_CbmStsKFTrackFitter(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsKFTrackFitter[nElements] : new ::CbmStsKFTrackFitter[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsKFTrackFitter(void *p) {
      delete ((::CbmStsKFTrackFitter*)p);
   }
   static void deleteArray_CbmStsKFTrackFitter(void *p) {
      delete [] ((::CbmStsKFTrackFitter*)p);
   }
   static void destruct_CbmStsKFTrackFitter(void *p) {
      typedef ::CbmStsKFTrackFitter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsKFTrackFitter

//______________________________________________________________________________
void CbmStsFitPerformanceTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsFitPerformanceTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsFitPerformanceTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsFitPerformanceTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsFitPerformanceTask(void *p) {
      return  p ? new(p) ::CbmStsFitPerformanceTask : new ::CbmStsFitPerformanceTask;
   }
   static void *newArray_CbmStsFitPerformanceTask(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsFitPerformanceTask[nElements] : new ::CbmStsFitPerformanceTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsFitPerformanceTask(void *p) {
      delete ((::CbmStsFitPerformanceTask*)p);
   }
   static void deleteArray_CbmStsFitPerformanceTask(void *p) {
      delete [] ((::CbmStsFitPerformanceTask*)p);
   }
   static void destruct_CbmStsFitPerformanceTask(void *p) {
      typedef ::CbmStsFitPerformanceTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsFitPerformanceTask

//______________________________________________________________________________
void CbmKFVertex::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmKFVertex.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmKFVertex::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmKFVertex::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmKFVertex(void *p) {
      return  p ? new(p) ::CbmKFVertex : new ::CbmKFVertex;
   }
   static void *newArray_CbmKFVertex(Long_t nElements, void *p) {
      return p ? new(p) ::CbmKFVertex[nElements] : new ::CbmKFVertex[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmKFVertex(void *p) {
      delete ((::CbmKFVertex*)p);
   }
   static void deleteArray_CbmKFVertex(void *p) {
      delete [] ((::CbmKFVertex*)p);
   }
   static void destruct_CbmKFVertex(void *p) {
      typedef ::CbmKFVertex current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmKFVertex

//______________________________________________________________________________
void CbmStsKFSecondaryVertexFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsKFSecondaryVertexFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsKFSecondaryVertexFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsKFSecondaryVertexFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsKFSecondaryVertexFinder(void *p) {
      return  p ? new(p) ::CbmStsKFSecondaryVertexFinder : new ::CbmStsKFSecondaryVertexFinder;
   }
   static void *newArray_CbmStsKFSecondaryVertexFinder(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsKFSecondaryVertexFinder[nElements] : new ::CbmStsKFSecondaryVertexFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsKFSecondaryVertexFinder(void *p) {
      delete ((::CbmStsKFSecondaryVertexFinder*)p);
   }
   static void deleteArray_CbmStsKFSecondaryVertexFinder(void *p) {
      delete [] ((::CbmStsKFSecondaryVertexFinder*)p);
   }
   static void destruct_CbmStsKFSecondaryVertexFinder(void *p) {
      typedef ::CbmStsKFSecondaryVertexFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsKFSecondaryVertexFinder

//______________________________________________________________________________
void CbmTrdTrackFitterKF::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdTrackFitterKF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdTrackFitterKF::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdTrackFitterKF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdTrackFitterKF(void *p) {
      return  p ? new(p) ::CbmTrdTrackFitterKF : new ::CbmTrdTrackFitterKF;
   }
   static void *newArray_CbmTrdTrackFitterKF(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdTrackFitterKF[nElements] : new ::CbmTrdTrackFitterKF[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdTrackFitterKF(void *p) {
      delete ((::CbmTrdTrackFitterKF*)p);
   }
   static void deleteArray_CbmTrdTrackFitterKF(void *p) {
      delete [] ((::CbmTrdTrackFitterKF*)p);
   }
   static void destruct_CbmTrdTrackFitterKF(void *p) {
      typedef ::CbmTrdTrackFitterKF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdTrackFitterKF

//______________________________________________________________________________
void CbmTofTrackFitterKF::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofTrackFitterKF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofTrackFitterKF::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofTrackFitterKF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofTrackFitterKF(void *p) {
      return  p ? new(p) ::CbmTofTrackFitterKF : new ::CbmTofTrackFitterKF;
   }
   static void *newArray_CbmTofTrackFitterKF(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofTrackFitterKF[nElements] : new ::CbmTofTrackFitterKF[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofTrackFitterKF(void *p) {
      delete ((::CbmTofTrackFitterKF*)p);
   }
   static void deleteArray_CbmTofTrackFitterKF(void *p) {
      delete [] ((::CbmTofTrackFitterKF*)p);
   }
   static void destruct_CbmTofTrackFitterKF(void *p) {
      typedef ::CbmTofTrackFitterKF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofTrackFitterKF

//______________________________________________________________________________
void CbmGlobalTrackFitterKF::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGlobalTrackFitterKF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGlobalTrackFitterKF::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGlobalTrackFitterKF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGlobalTrackFitterKF(void *p) {
      return  p ? new(p) ::CbmGlobalTrackFitterKF : new ::CbmGlobalTrackFitterKF;
   }
   static void *newArray_CbmGlobalTrackFitterKF(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGlobalTrackFitterKF[nElements] : new ::CbmGlobalTrackFitterKF[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGlobalTrackFitterKF(void *p) {
      delete ((::CbmGlobalTrackFitterKF*)p);
   }
   static void deleteArray_CbmGlobalTrackFitterKF(void *p) {
      delete [] ((::CbmGlobalTrackFitterKF*)p);
   }
   static void destruct_CbmGlobalTrackFitterKF(void *p) {
      typedef ::CbmGlobalTrackFitterKF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGlobalTrackFitterKF

namespace ROOT {
   static TClass *vectorlECbmStsTrackmUgR_Dictionary();
   static void vectorlECbmStsTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmStsTrackmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmStsTrackmUgR(Long_t size, void *p);
   static void delete_vectorlECbmStsTrackmUgR(void *p);
   static void deleteArray_vectorlECbmStsTrackmUgR(void *p);
   static void destruct_vectorlECbmStsTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsTrack*>*)
   {
      vector<CbmStsTrack*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsTrack*>", -2, "vector", 389,
                  typeid(vector<CbmStsTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsTrackmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmStsTrack*>) );
      instance.SetNew(&new_vectorlECbmStsTrackmUgR);
      instance.SetNewArray(&newArray_vectorlECbmStsTrackmUgR);
      instance.SetDelete(&delete_vectorlECbmStsTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsTrackmUgR);
      instance.SetDestructor(&destruct_vectorlECbmStsTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsTrack*> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsTrack*>","std::vector<CbmStsTrack*, std::allocator<CbmStsTrack*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsTrack*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsTrack*>*)nullptr)->GetClass();
      vectorlECbmStsTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsTrack*> : new vector<CbmStsTrack*>;
   }
   static void *newArray_vectorlECbmStsTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsTrack*>[nElements] : new vector<CbmStsTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsTrackmUgR(void *p) {
      delete ((vector<CbmStsTrack*>*)p);
   }
   static void deleteArray_vectorlECbmStsTrackmUgR(void *p) {
      delete [] ((vector<CbmStsTrack*>*)p);
   }
   static void destruct_vectorlECbmStsTrackmUgR(void *p) {
      typedef vector<CbmStsTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsTrack*>

namespace ROOT {
   static TClass *vectorlECbmKFWallgR_Dictionary();
   static void vectorlECbmKFWallgR_TClassManip(TClass*);
   static void *new_vectorlECbmKFWallgR(void *p = nullptr);
   static void *newArray_vectorlECbmKFWallgR(Long_t size, void *p);
   static void delete_vectorlECbmKFWallgR(void *p);
   static void deleteArray_vectorlECbmKFWallgR(void *p);
   static void destruct_vectorlECbmKFWallgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmKFWall>*)
   {
      vector<CbmKFWall> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmKFWall>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmKFWall>", -2, "vector", 389,
                  typeid(vector<CbmKFWall>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmKFWallgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmKFWall>) );
      instance.SetNew(&new_vectorlECbmKFWallgR);
      instance.SetNewArray(&newArray_vectorlECbmKFWallgR);
      instance.SetDelete(&delete_vectorlECbmKFWallgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmKFWallgR);
      instance.SetDestructor(&destruct_vectorlECbmKFWallgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmKFWall> >()));

      ::ROOT::AddClassAlternate("vector<CbmKFWall>","std::vector<CbmKFWall, std::allocator<CbmKFWall> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmKFWall>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmKFWallgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmKFWall>*)nullptr)->GetClass();
      vectorlECbmKFWallgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmKFWallgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmKFWallgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFWall> : new vector<CbmKFWall>;
   }
   static void *newArray_vectorlECbmKFWallgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFWall>[nElements] : new vector<CbmKFWall>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmKFWallgR(void *p) {
      delete ((vector<CbmKFWall>*)p);
   }
   static void deleteArray_vectorlECbmKFWallgR(void *p) {
      delete [] ((vector<CbmKFWall>*)p);
   }
   static void destruct_vectorlECbmKFWallgR(void *p) {
      typedef vector<CbmKFWall> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmKFWall>

namespace ROOT {
   static TClass *vectorlECbmKFTubegR_Dictionary();
   static void vectorlECbmKFTubegR_TClassManip(TClass*);
   static void *new_vectorlECbmKFTubegR(void *p = nullptr);
   static void *newArray_vectorlECbmKFTubegR(Long_t size, void *p);
   static void delete_vectorlECbmKFTubegR(void *p);
   static void deleteArray_vectorlECbmKFTubegR(void *p);
   static void destruct_vectorlECbmKFTubegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmKFTube>*)
   {
      vector<CbmKFTube> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmKFTube>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmKFTube>", -2, "vector", 389,
                  typeid(vector<CbmKFTube>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmKFTubegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmKFTube>) );
      instance.SetNew(&new_vectorlECbmKFTubegR);
      instance.SetNewArray(&newArray_vectorlECbmKFTubegR);
      instance.SetDelete(&delete_vectorlECbmKFTubegR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmKFTubegR);
      instance.SetDestructor(&destruct_vectorlECbmKFTubegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmKFTube> >()));

      ::ROOT::AddClassAlternate("vector<CbmKFTube>","std::vector<CbmKFTube, std::allocator<CbmKFTube> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmKFTube>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmKFTubegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmKFTube>*)nullptr)->GetClass();
      vectorlECbmKFTubegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmKFTubegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmKFTubegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFTube> : new vector<CbmKFTube>;
   }
   static void *newArray_vectorlECbmKFTubegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFTube>[nElements] : new vector<CbmKFTube>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmKFTubegR(void *p) {
      delete ((vector<CbmKFTube>*)p);
   }
   static void deleteArray_vectorlECbmKFTubegR(void *p) {
      delete [] ((vector<CbmKFTube>*)p);
   }
   static void destruct_vectorlECbmKFTubegR(void *p) {
      typedef vector<CbmKFTube> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmKFTube>

namespace ROOT {
   static TClass *vectorlECbmKFTrackInterfacemUgR_Dictionary();
   static void vectorlECbmKFTrackInterfacemUgR_TClassManip(TClass*);
   static void *new_vectorlECbmKFTrackInterfacemUgR(void *p = nullptr);
   static void *newArray_vectorlECbmKFTrackInterfacemUgR(Long_t size, void *p);
   static void delete_vectorlECbmKFTrackInterfacemUgR(void *p);
   static void deleteArray_vectorlECbmKFTrackInterfacemUgR(void *p);
   static void destruct_vectorlECbmKFTrackInterfacemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmKFTrackInterface*>*)
   {
      vector<CbmKFTrackInterface*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmKFTrackInterface*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmKFTrackInterface*>", -2, "vector", 389,
                  typeid(vector<CbmKFTrackInterface*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmKFTrackInterfacemUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmKFTrackInterface*>) );
      instance.SetNew(&new_vectorlECbmKFTrackInterfacemUgR);
      instance.SetNewArray(&newArray_vectorlECbmKFTrackInterfacemUgR);
      instance.SetDelete(&delete_vectorlECbmKFTrackInterfacemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmKFTrackInterfacemUgR);
      instance.SetDestructor(&destruct_vectorlECbmKFTrackInterfacemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmKFTrackInterface*> >()));

      ::ROOT::AddClassAlternate("vector<CbmKFTrackInterface*>","std::vector<CbmKFTrackInterface*, std::allocator<CbmKFTrackInterface*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmKFTrackInterface*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmKFTrackInterfacemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmKFTrackInterface*>*)nullptr)->GetClass();
      vectorlECbmKFTrackInterfacemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmKFTrackInterfacemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmKFTrackInterfacemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFTrackInterface*> : new vector<CbmKFTrackInterface*>;
   }
   static void *newArray_vectorlECbmKFTrackInterfacemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFTrackInterface*>[nElements] : new vector<CbmKFTrackInterface*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmKFTrackInterfacemUgR(void *p) {
      delete ((vector<CbmKFTrackInterface*>*)p);
   }
   static void deleteArray_vectorlECbmKFTrackInterfacemUgR(void *p) {
      delete [] ((vector<CbmKFTrackInterface*>*)p);
   }
   static void destruct_vectorlECbmKFTrackInterfacemUgR(void *p) {
      typedef vector<CbmKFTrackInterface*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmKFTrackInterface*>

namespace ROOT {
   static TClass *vectorlECbmKFTrackgR_Dictionary();
   static void vectorlECbmKFTrackgR_TClassManip(TClass*);
   static void *new_vectorlECbmKFTrackgR(void *p = nullptr);
   static void *newArray_vectorlECbmKFTrackgR(Long_t size, void *p);
   static void delete_vectorlECbmKFTrackgR(void *p);
   static void deleteArray_vectorlECbmKFTrackgR(void *p);
   static void destruct_vectorlECbmKFTrackgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmKFTrack>*)
   {
      vector<CbmKFTrack> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmKFTrack>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmKFTrack>", -2, "vector", 389,
                  typeid(vector<CbmKFTrack>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmKFTrackgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmKFTrack>) );
      instance.SetNew(&new_vectorlECbmKFTrackgR);
      instance.SetNewArray(&newArray_vectorlECbmKFTrackgR);
      instance.SetDelete(&delete_vectorlECbmKFTrackgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmKFTrackgR);
      instance.SetDestructor(&destruct_vectorlECbmKFTrackgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmKFTrack> >()));

      ::ROOT::AddClassAlternate("vector<CbmKFTrack>","std::vector<CbmKFTrack, std::allocator<CbmKFTrack> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmKFTrack>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmKFTrackgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmKFTrack>*)nullptr)->GetClass();
      vectorlECbmKFTrackgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmKFTrackgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmKFTrackgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFTrack> : new vector<CbmKFTrack>;
   }
   static void *newArray_vectorlECbmKFTrackgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFTrack>[nElements] : new vector<CbmKFTrack>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmKFTrackgR(void *p) {
      delete ((vector<CbmKFTrack>*)p);
   }
   static void deleteArray_vectorlECbmKFTrackgR(void *p) {
      delete [] ((vector<CbmKFTrack>*)p);
   }
   static void destruct_vectorlECbmKFTrackgR(void *p) {
      typedef vector<CbmKFTrack> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmKFTrack>

namespace ROOT {
   static TClass *vectorlECbmKFStsHitgR_Dictionary();
   static void vectorlECbmKFStsHitgR_TClassManip(TClass*);
   static void *new_vectorlECbmKFStsHitgR(void *p = nullptr);
   static void *newArray_vectorlECbmKFStsHitgR(Long_t size, void *p);
   static void delete_vectorlECbmKFStsHitgR(void *p);
   static void deleteArray_vectorlECbmKFStsHitgR(void *p);
   static void destruct_vectorlECbmKFStsHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmKFStsHit>*)
   {
      vector<CbmKFStsHit> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmKFStsHit>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmKFStsHit>", -2, "vector", 389,
                  typeid(vector<CbmKFStsHit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmKFStsHitgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmKFStsHit>) );
      instance.SetNew(&new_vectorlECbmKFStsHitgR);
      instance.SetNewArray(&newArray_vectorlECbmKFStsHitgR);
      instance.SetDelete(&delete_vectorlECbmKFStsHitgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmKFStsHitgR);
      instance.SetDestructor(&destruct_vectorlECbmKFStsHitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmKFStsHit> >()));

      ::ROOT::AddClassAlternate("vector<CbmKFStsHit>","std::vector<CbmKFStsHit, std::allocator<CbmKFStsHit> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmKFStsHit>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmKFStsHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmKFStsHit>*)nullptr)->GetClass();
      vectorlECbmKFStsHitgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmKFStsHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmKFStsHitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFStsHit> : new vector<CbmKFStsHit>;
   }
   static void *newArray_vectorlECbmKFStsHitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFStsHit>[nElements] : new vector<CbmKFStsHit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmKFStsHitgR(void *p) {
      delete ((vector<CbmKFStsHit>*)p);
   }
   static void deleteArray_vectorlECbmKFStsHitgR(void *p) {
      delete [] ((vector<CbmKFStsHit>*)p);
   }
   static void destruct_vectorlECbmKFStsHitgR(void *p) {
      typedef vector<CbmKFStsHit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmKFStsHit>

namespace ROOT {
   static TClass *vectorlECbmKFMaterialmUgR_Dictionary();
   static void vectorlECbmKFMaterialmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmKFMaterialmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmKFMaterialmUgR(Long_t size, void *p);
   static void delete_vectorlECbmKFMaterialmUgR(void *p);
   static void deleteArray_vectorlECbmKFMaterialmUgR(void *p);
   static void destruct_vectorlECbmKFMaterialmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmKFMaterial*>*)
   {
      vector<CbmKFMaterial*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmKFMaterial*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmKFMaterial*>", -2, "vector", 389,
                  typeid(vector<CbmKFMaterial*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmKFMaterialmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmKFMaterial*>) );
      instance.SetNew(&new_vectorlECbmKFMaterialmUgR);
      instance.SetNewArray(&newArray_vectorlECbmKFMaterialmUgR);
      instance.SetDelete(&delete_vectorlECbmKFMaterialmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmKFMaterialmUgR);
      instance.SetDestructor(&destruct_vectorlECbmKFMaterialmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmKFMaterial*> >()));

      ::ROOT::AddClassAlternate("vector<CbmKFMaterial*>","std::vector<CbmKFMaterial*, std::allocator<CbmKFMaterial*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmKFMaterial*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmKFMaterialmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmKFMaterial*>*)nullptr)->GetClass();
      vectorlECbmKFMaterialmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmKFMaterialmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmKFMaterialmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFMaterial*> : new vector<CbmKFMaterial*>;
   }
   static void *newArray_vectorlECbmKFMaterialmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFMaterial*>[nElements] : new vector<CbmKFMaterial*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmKFMaterialmUgR(void *p) {
      delete ((vector<CbmKFMaterial*>*)p);
   }
   static void deleteArray_vectorlECbmKFMaterialmUgR(void *p) {
      delete [] ((vector<CbmKFMaterial*>*)p);
   }
   static void destruct_vectorlECbmKFMaterialmUgR(void *p) {
      typedef vector<CbmKFMaterial*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmKFMaterial*>

namespace ROOT {
   static TClass *vectorlECbmKFHitmUgR_Dictionary();
   static void vectorlECbmKFHitmUgR_TClassManip(TClass*);
   static void *new_vectorlECbmKFHitmUgR(void *p = nullptr);
   static void *newArray_vectorlECbmKFHitmUgR(Long_t size, void *p);
   static void delete_vectorlECbmKFHitmUgR(void *p);
   static void deleteArray_vectorlECbmKFHitmUgR(void *p);
   static void destruct_vectorlECbmKFHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmKFHit*>*)
   {
      vector<CbmKFHit*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmKFHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmKFHit*>", -2, "vector", 389,
                  typeid(vector<CbmKFHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmKFHitmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmKFHit*>) );
      instance.SetNew(&new_vectorlECbmKFHitmUgR);
      instance.SetNewArray(&newArray_vectorlECbmKFHitmUgR);
      instance.SetDelete(&delete_vectorlECbmKFHitmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmKFHitmUgR);
      instance.SetDestructor(&destruct_vectorlECbmKFHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmKFHit*> >()));

      ::ROOT::AddClassAlternate("vector<CbmKFHit*>","std::vector<CbmKFHit*, std::allocator<CbmKFHit*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmKFHit*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmKFHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmKFHit*>*)nullptr)->GetClass();
      vectorlECbmKFHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmKFHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmKFHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFHit*> : new vector<CbmKFHit*>;
   }
   static void *newArray_vectorlECbmKFHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFHit*>[nElements] : new vector<CbmKFHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmKFHitmUgR(void *p) {
      delete ((vector<CbmKFHit*>*)p);
   }
   static void deleteArray_vectorlECbmKFHitmUgR(void *p) {
      delete [] ((vector<CbmKFHit*>*)p);
   }
   static void destruct_vectorlECbmKFHitmUgR(void *p) {
      typedef vector<CbmKFHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmKFHit*>

namespace ROOT {
   static TClass *vectorlECbmKFConegR_Dictionary();
   static void vectorlECbmKFConegR_TClassManip(TClass*);
   static void *new_vectorlECbmKFConegR(void *p = nullptr);
   static void *newArray_vectorlECbmKFConegR(Long_t size, void *p);
   static void delete_vectorlECbmKFConegR(void *p);
   static void deleteArray_vectorlECbmKFConegR(void *p);
   static void destruct_vectorlECbmKFConegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmKFCone>*)
   {
      vector<CbmKFCone> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmKFCone>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmKFCone>", -2, "vector", 389,
                  typeid(vector<CbmKFCone>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmKFConegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmKFCone>) );
      instance.SetNew(&new_vectorlECbmKFConegR);
      instance.SetNewArray(&newArray_vectorlECbmKFConegR);
      instance.SetDelete(&delete_vectorlECbmKFConegR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmKFConegR);
      instance.SetDestructor(&destruct_vectorlECbmKFConegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmKFCone> >()));

      ::ROOT::AddClassAlternate("vector<CbmKFCone>","std::vector<CbmKFCone, std::allocator<CbmKFCone> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmKFCone>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmKFConegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmKFCone>*)nullptr)->GetClass();
      vectorlECbmKFConegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmKFConegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmKFConegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFCone> : new vector<CbmKFCone>;
   }
   static void *newArray_vectorlECbmKFConegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFCone>[nElements] : new vector<CbmKFCone>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmKFConegR(void *p) {
      delete ((vector<CbmKFCone>*)p);
   }
   static void deleteArray_vectorlECbmKFConegR(void *p) {
      delete [] ((vector<CbmKFCone>*)p);
   }
   static void destruct_vectorlECbmKFConegR(void *p) {
      typedef vector<CbmKFCone> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmKFCone>

namespace ROOT {
   static TClass *vectorlECbmKFBoxgR_Dictionary();
   static void vectorlECbmKFBoxgR_TClassManip(TClass*);
   static void *new_vectorlECbmKFBoxgR(void *p = nullptr);
   static void *newArray_vectorlECbmKFBoxgR(Long_t size, void *p);
   static void delete_vectorlECbmKFBoxgR(void *p);
   static void deleteArray_vectorlECbmKFBoxgR(void *p);
   static void destruct_vectorlECbmKFBoxgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmKFBox>*)
   {
      vector<CbmKFBox> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmKFBox>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmKFBox>", -2, "vector", 389,
                  typeid(vector<CbmKFBox>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmKFBoxgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmKFBox>) );
      instance.SetNew(&new_vectorlECbmKFBoxgR);
      instance.SetNewArray(&newArray_vectorlECbmKFBoxgR);
      instance.SetDelete(&delete_vectorlECbmKFBoxgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmKFBoxgR);
      instance.SetDestructor(&destruct_vectorlECbmKFBoxgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmKFBox> >()));

      ::ROOT::AddClassAlternate("vector<CbmKFBox>","std::vector<CbmKFBox, std::allocator<CbmKFBox> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmKFBox>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmKFBoxgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmKFBox>*)nullptr)->GetClass();
      vectorlECbmKFBoxgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmKFBoxgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmKFBoxgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFBox> : new vector<CbmKFBox>;
   }
   static void *newArray_vectorlECbmKFBoxgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmKFBox>[nElements] : new vector<CbmKFBox>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmKFBoxgR(void *p) {
      delete ((vector<CbmKFBox>*)p);
   }
   static void deleteArray_vectorlECbmKFBoxgR(void *p) {
      delete [] ((vector<CbmKFBox>*)p);
   }
   static void destruct_vectorlECbmKFBoxgR(void *p) {
      typedef vector<CbmKFBox> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmKFBox>

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
  void TriggerDictionaryInitialization_libKF_Impl() {
    static const char* headers[] = {
"CbmKF.h",
"CbmKFFieldMath.h",
"CbmKFHit.h",
"CbmKFMaterial.h",
"CbmKFMath.h",
"CbmKFPixelMeasurement.h",
"CbmKFPrimaryVertexFinder.h",
"CbmKFSecondaryVertexFinder.h",
"CbmKFTrackInterface.h",
"CbmKFUMeasurement.h",
"CbmKFVertexInterface.h",
"CbmTrackingDetectorInterfaceInit.h",
"CbmKFStsHit.h",
"CbmKFTrack.h",
"CbmKFTrdHit.h",
"CbmKFTofHit.h",
"CbmPVFinderKF.h",
"CbmStsFitPerformanceTask.h",
"CbmStsKFTrackFitter.h",
"CbmStsKFSecondaryVertexFinder.h",
"CbmTrdTrackFitterKF.h",
"CbmTofTrackFitterKF.h",
"CbmGlobalTrackFitterKF.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF",
"/home/heinemann/gpuunpacker/cbmroot/reco/KF/Interface",
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
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc",
"/home/heinemann/gpuunpacker/cbmroot/external/xpu/src",
"/home/heinemann/gpuunpacker/cbmroot/external/GSL/include",
"/home/heinemann/gpuunpacker/cbmroot/reco/base",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/steer",
"/home/heinemann/gpuunpacker/cbmroot/core/config",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/base",
"/home/heinemann/gpuunpacker/cbmroot/sim/transport/geosetup",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/tools",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/SensorDataSheets",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/tasks",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/mvd/plugins/buffers",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/sts",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/tof",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/KF/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libKF dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmKFTrackInterface.h")))  __attribute__((annotate("$clingAutoload$CbmKF.h")))  CbmKFTrackInterface;
class __attribute__((annotate("$clingAutoload$CbmKFMaterial.h")))  __attribute__((annotate("$clingAutoload$CbmKF.h")))  CbmKFMaterial;
class __attribute__((annotate("$clingAutoload$CbmKFMaterial.h")))  __attribute__((annotate("$clingAutoload$CbmKF.h")))  CbmKFWall;
class __attribute__((annotate("$clingAutoload$CbmKFMaterial.h")))  __attribute__((annotate("$clingAutoload$CbmKF.h")))  CbmKFTube;
class __attribute__((annotate("$clingAutoload$CbmKFMaterial.h")))  __attribute__((annotate("$clingAutoload$CbmKF.h")))  CbmKFBox;
class __attribute__((annotate("$clingAutoload$CbmKFMaterial.h")))  __attribute__((annotate("$clingAutoload$CbmKF.h")))  CbmKFCone;
class __attribute__((annotate("$clingAutoload$CbmKF.h")))  CbmKF;
class __attribute__((annotate("$clingAutoload$CbmKFFieldMath.h")))  CbmKFFieldMath;
class __attribute__((annotate("$clingAutoload$CbmKFHit.h")))  CbmKFHit;
class __attribute__((annotate("$clingAutoload$CbmKFMath.h")))  CbmKFMath;
class __attribute__((annotate("$clingAutoload$CbmKFPixelMeasurement.h")))  CbmKFPixelMeasurement;
class __attribute__((annotate("$clingAutoload$CbmKFVertexInterface.h")))  __attribute__((annotate("$clingAutoload$CbmKFPrimaryVertexFinder.h")))  CbmKFVertexInterface;
class __attribute__((annotate("$clingAutoload$CbmKFPrimaryVertexFinder.h")))  CbmKFPrimaryVertexFinder;
class __attribute__((annotate("$clingAutoload$CbmKFSecondaryVertexFinder.h")))  CbmKFSecondaryVertexFinder;
class __attribute__((annotate("$clingAutoload$CbmKFUMeasurement.h")))  CbmKFUMeasurement;
class __attribute__((annotate("$clingAutoload$CbmTrackingDetectorInterfaceInit.h")))  CbmTrackingDetectorInterfaceInit;
class __attribute__((annotate("$clingAutoload$CbmKFStsHit.h")))  CbmKFStsHit;
class __attribute__((annotate("$clingAutoload$CbmKFTrack.h")))  CbmKFTrack;
class __attribute__((annotate("$clingAutoload$CbmKFTrdHit.h")))  CbmKFTrdHit;
class __attribute__((annotate("$clingAutoload$CbmKFTofHit.h")))  CbmKFTofHit;
class __attribute__((annotate("$clingAutoload$CbmPVFinderKF.h")))  CbmPVFinderKF;
class __attribute__((annotate("$clingAutoload$CbmStsKFTrackFitter.h")))  __attribute__((annotate("$clingAutoload$CbmStsFitPerformanceTask.h")))  CbmStsKFTrackFitter;
class __attribute__((annotate("$clingAutoload$CbmStsFitPerformanceTask.h")))  CbmStsFitPerformanceTask;
class __attribute__((annotate("$clingAutoload$CbmKFVertex.h")))  __attribute__((annotate("$clingAutoload$CbmStsKFSecondaryVertexFinder.h")))  CbmKFVertex;
class __attribute__((annotate("$clingAutoload$CbmStsKFSecondaryVertexFinder.h")))  CbmStsKFSecondaryVertexFinder;
class __attribute__((annotate("$clingAutoload$CbmTrdTrackFitterKF.h")))  CbmTrdTrackFitterKF;
class __attribute__((annotate("$clingAutoload$CbmTofTrackFitterKF.h")))  CbmTofTrackFitterKF;
class __attribute__((annotate("$clingAutoload$CbmGlobalTrackFitterKF.h")))  CbmGlobalTrackFitterKF;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libKF dictionary payload"

#ifndef DO_TPCCATRACKER_EFF_PERFORMANCE
  #define DO_TPCCATRACKER_EFF_PERFORMANCE 1
#endif
#ifndef NonhomogeneousField
  #define NonhomogeneousField 1
#endif
#ifndef CBM
  #define CBM 1
#endif
#ifndef USE_TIMERS
  #define USE_TIMERS 1
#endif
#ifndef HAVE_SSE
  #define HAVE_SSE 1
#endif
#ifndef DO_TPCCATRACKER_EFF_PERFORMANCE
  #define DO_TPCCATRACKER_EFF_PERFORMANCE 1
#endif
#ifndef NonhomogeneousField
  #define NonhomogeneousField 1
#endif
#ifndef CBM
  #define CBM 1
#endif
#ifndef USE_TIMERS
  #define USE_TIMERS 1
#endif
#ifndef HAVE_SSE
  #define HAVE_SSE 1
#endif
#ifndef DO_TPCCATRACKER_EFF_PERFORMANCE
  #define DO_TPCCATRACKER_EFF_PERFORMANCE 1
#endif
#ifndef NonhomogeneousField
  #define NonhomogeneousField 1
#endif
#ifndef CBM
  #define CBM 1
#endif
#ifndef USE_TIMERS
  #define USE_TIMERS 1
#endif
#ifndef HAVE_SSE
  #define HAVE_SSE 1
#endif
#ifndef DO_TPCCATRACKER_EFF_PERFORMANCE
  #define DO_TPCCATRACKER_EFF_PERFORMANCE 1
#endif
#ifndef NonhomogeneousField
  #define NonhomogeneousField 1
#endif
#ifndef CBM
  #define CBM 1
#endif
#ifndef USE_TIMERS
  #define USE_TIMERS 1
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
#include "CbmKF.h"
#include "CbmKFFieldMath.h"
#include "CbmKFHit.h"
#include "CbmKFMaterial.h"
#include "CbmKFMath.h"
#include "CbmKFPixelMeasurement.h"
#include "CbmKFPrimaryVertexFinder.h"
#include "CbmKFSecondaryVertexFinder.h"
#include "CbmKFTrackInterface.h"
#include "CbmKFUMeasurement.h"
#include "CbmKFVertexInterface.h"
#include "CbmTrackingDetectorInterfaceInit.h"
#include "CbmKFStsHit.h"
#include "CbmKFTrack.h"
#include "CbmKFTrdHit.h"
#include "CbmKFTofHit.h"
#include "CbmPVFinderKF.h"
#include "CbmStsFitPerformanceTask.h"
#include "CbmStsKFTrackFitter.h"
#include "CbmStsKFSecondaryVertexFinder.h"
#include "CbmTrdTrackFitterKF.h"
#include "CbmTofTrackFitterKF.h"
#include "CbmGlobalTrackFitterKF.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmGlobalTrackFitterKF", payloadCode, "@",
"CbmKF", payloadCode, "@",
"CbmKFBox", payloadCode, "@",
"CbmKFCone", payloadCode, "@",
"CbmKFFieldMath", payloadCode, "@",
"CbmKFHit", payloadCode, "@",
"CbmKFMaterial", payloadCode, "@",
"CbmKFMath", payloadCode, "@",
"CbmKFPixelMeasurement", payloadCode, "@",
"CbmKFPrimaryVertexFinder", payloadCode, "@",
"CbmKFSecondaryVertexFinder", payloadCode, "@",
"CbmKFStsHit", payloadCode, "@",
"CbmKFTofHit", payloadCode, "@",
"CbmKFTrack", payloadCode, "@",
"CbmKFTrackInterface", payloadCode, "@",
"CbmKFTrdHit", payloadCode, "@",
"CbmKFTube", payloadCode, "@",
"CbmKFUMeasurement", payloadCode, "@",
"CbmKFVertex", payloadCode, "@",
"CbmKFVertexInterface", payloadCode, "@",
"CbmKFWall", payloadCode, "@",
"CbmPVFinderKF", payloadCode, "@",
"CbmStsFitPerformanceTask", payloadCode, "@",
"CbmStsKFSecondaryVertexFinder", payloadCode, "@",
"CbmStsKFTrackFitter", payloadCode, "@",
"CbmTofTrackFitterKF", payloadCode, "@",
"CbmTrackingDetectorInterfaceInit", payloadCode, "@",
"CbmTrdTrackFitterKF", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libKF",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libKF_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libKF_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libKF() {
  TriggerDictionaryInitialization_libKF_Impl();
}
