// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmMuchBase
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
#include "CbmGeoMuchPar.h"
#include "CbmMcbm2018MuchPar.h"
#include "CbmMuchUnpackPar.h"
#include "CbmMuchContFact.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchLayer.h"
#include "CbmMuchLayerSide.h"
#include "CbmMuchModule.h"
#include "CbmMuchModuleGem.h"
#include "CbmMuchModuleGemRadial.h"
#include "CbmMuchModuleGemRectangular.h"
#include "CbmMuchPad.h"
#include "CbmMuchPadRadial.h"
#include "CbmMuchPadRectangular.h"
#include "CbmMuchPointInfo.h"
#include "CbmMuchSector.h"
#include "CbmMuchSectorRadial.h"
#include "CbmMuchSectorRectangular.h"
#include "CbmMuchStation.h"
#include "CbmMuchSegmentAuto.h"
#include "CbmMuchSegmentManual.h"
#include "CbmMuchSegmentSector.h"
#include "CbmMuchTrackingInterface.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmGeoMuchPar(void *p = nullptr);
   static void *newArray_CbmGeoMuchPar(Long_t size, void *p);
   static void delete_CbmGeoMuchPar(void *p);
   static void deleteArray_CbmGeoMuchPar(void *p);
   static void destruct_CbmGeoMuchPar(void *p);
   static void streamer_CbmGeoMuchPar(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGeoMuchPar*)
   {
      ::CbmGeoMuchPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGeoMuchPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGeoMuchPar", ::CbmGeoMuchPar::Class_Version(), "CbmGeoMuchPar.h", 29,
                  typeid(::CbmGeoMuchPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGeoMuchPar::Dictionary, isa_proxy, 16,
                  sizeof(::CbmGeoMuchPar) );
      instance.SetNew(&new_CbmGeoMuchPar);
      instance.SetNewArray(&newArray_CbmGeoMuchPar);
      instance.SetDelete(&delete_CbmGeoMuchPar);
      instance.SetDeleteArray(&deleteArray_CbmGeoMuchPar);
      instance.SetDestructor(&destruct_CbmGeoMuchPar);
      instance.SetStreamerFunc(&streamer_CbmGeoMuchPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGeoMuchPar*)
   {
      return GenerateInitInstanceLocal((::CbmGeoMuchPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGeoMuchPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMcbm2018MuchPar(void *p = nullptr);
   static void *newArray_CbmMcbm2018MuchPar(Long_t size, void *p);
   static void delete_CbmMcbm2018MuchPar(void *p);
   static void deleteArray_CbmMcbm2018MuchPar(void *p);
   static void destruct_CbmMcbm2018MuchPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMcbm2018MuchPar*)
   {
      ::CbmMcbm2018MuchPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMcbm2018MuchPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMcbm2018MuchPar", ::CbmMcbm2018MuchPar::Class_Version(), "CbmMcbm2018MuchPar.h", 25,
                  typeid(::CbmMcbm2018MuchPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMcbm2018MuchPar::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMcbm2018MuchPar) );
      instance.SetNew(&new_CbmMcbm2018MuchPar);
      instance.SetNewArray(&newArray_CbmMcbm2018MuchPar);
      instance.SetDelete(&delete_CbmMcbm2018MuchPar);
      instance.SetDeleteArray(&deleteArray_CbmMcbm2018MuchPar);
      instance.SetDestructor(&destruct_CbmMcbm2018MuchPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMcbm2018MuchPar*)
   {
      return GenerateInitInstanceLocal((::CbmMcbm2018MuchPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMcbm2018MuchPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchUnpackPar(void *p = nullptr);
   static void *newArray_CbmMuchUnpackPar(Long_t size, void *p);
   static void delete_CbmMuchUnpackPar(void *p);
   static void deleteArray_CbmMuchUnpackPar(void *p);
   static void destruct_CbmMuchUnpackPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchUnpackPar*)
   {
      ::CbmMuchUnpackPar *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchUnpackPar >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchUnpackPar", ::CbmMuchUnpackPar::Class_Version(), "CbmMuchUnpackPar.h", 23,
                  typeid(::CbmMuchUnpackPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchUnpackPar::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchUnpackPar) );
      instance.SetNew(&new_CbmMuchUnpackPar);
      instance.SetNewArray(&newArray_CbmMuchUnpackPar);
      instance.SetDelete(&delete_CbmMuchUnpackPar);
      instance.SetDeleteArray(&deleteArray_CbmMuchUnpackPar);
      instance.SetDestructor(&destruct_CbmMuchUnpackPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchUnpackPar*)
   {
      return GenerateInitInstanceLocal((::CbmMuchUnpackPar*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchUnpackPar*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchContFact(void *p = nullptr);
   static void *newArray_CbmMuchContFact(Long_t size, void *p);
   static void delete_CbmMuchContFact(void *p);
   static void deleteArray_CbmMuchContFact(void *p);
   static void destruct_CbmMuchContFact(void *p);
   static void streamer_CbmMuchContFact(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchContFact*)
   {
      ::CbmMuchContFact *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchContFact >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchContFact", ::CbmMuchContFact::Class_Version(), "CbmMuchContFact.h", 26,
                  typeid(::CbmMuchContFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchContFact::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMuchContFact) );
      instance.SetNew(&new_CbmMuchContFact);
      instance.SetNewArray(&newArray_CbmMuchContFact);
      instance.SetDelete(&delete_CbmMuchContFact);
      instance.SetDeleteArray(&deleteArray_CbmMuchContFact);
      instance.SetDestructor(&destruct_CbmMuchContFact);
      instance.SetStreamerFunc(&streamer_CbmMuchContFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchContFact*)
   {
      return GenerateInitInstanceLocal((::CbmMuchContFact*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchContFact*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmMuchGeoScheme(void *p);
   static void deleteArray_CbmMuchGeoScheme(void *p);
   static void destruct_CbmMuchGeoScheme(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchGeoScheme*)
   {
      ::CbmMuchGeoScheme *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchGeoScheme >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchGeoScheme", ::CbmMuchGeoScheme::Class_Version(), "CbmMuchGeoScheme.h", 47,
                  typeid(::CbmMuchGeoScheme), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchGeoScheme::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchGeoScheme) );
      instance.SetDelete(&delete_CbmMuchGeoScheme);
      instance.SetDeleteArray(&deleteArray_CbmMuchGeoScheme);
      instance.SetDestructor(&destruct_CbmMuchGeoScheme);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchGeoScheme*)
   {
      return GenerateInitInstanceLocal((::CbmMuchGeoScheme*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchGeoScheme*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchLayerSide(void *p = nullptr);
   static void *newArray_CbmMuchLayerSide(Long_t size, void *p);
   static void delete_CbmMuchLayerSide(void *p);
   static void deleteArray_CbmMuchLayerSide(void *p);
   static void destruct_CbmMuchLayerSide(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchLayerSide*)
   {
      ::CbmMuchLayerSide *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchLayerSide >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchLayerSide", ::CbmMuchLayerSide::Class_Version(), "CbmMuchLayerSide.h", 26,
                  typeid(::CbmMuchLayerSide), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchLayerSide::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchLayerSide) );
      instance.SetNew(&new_CbmMuchLayerSide);
      instance.SetNewArray(&newArray_CbmMuchLayerSide);
      instance.SetDelete(&delete_CbmMuchLayerSide);
      instance.SetDeleteArray(&deleteArray_CbmMuchLayerSide);
      instance.SetDestructor(&destruct_CbmMuchLayerSide);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchLayerSide*)
   {
      return GenerateInitInstanceLocal((::CbmMuchLayerSide*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchLayerSide*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchLayer(void *p = nullptr);
   static void *newArray_CbmMuchLayer(Long_t size, void *p);
   static void delete_CbmMuchLayer(void *p);
   static void deleteArray_CbmMuchLayer(void *p);
   static void destruct_CbmMuchLayer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchLayer*)
   {
      ::CbmMuchLayer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchLayer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchLayer", ::CbmMuchLayer::Class_Version(), "CbmMuchLayer.h", 25,
                  typeid(::CbmMuchLayer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchLayer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchLayer) );
      instance.SetNew(&new_CbmMuchLayer);
      instance.SetNewArray(&newArray_CbmMuchLayer);
      instance.SetDelete(&delete_CbmMuchLayer);
      instance.SetDeleteArray(&deleteArray_CbmMuchLayer);
      instance.SetDestructor(&destruct_CbmMuchLayer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchLayer*)
   {
      return GenerateInitInstanceLocal((::CbmMuchLayer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchLayer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchModule(void *p = nullptr);
   static void *newArray_CbmMuchModule(Long_t size, void *p);
   static void delete_CbmMuchModule(void *p);
   static void deleteArray_CbmMuchModule(void *p);
   static void destruct_CbmMuchModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchModule*)
   {
      ::CbmMuchModule *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchModule >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchModule", ::CbmMuchModule::Class_Version(), "CbmMuchModule.h", 28,
                  typeid(::CbmMuchModule), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchModule::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchModule) );
      instance.SetNew(&new_CbmMuchModule);
      instance.SetNewArray(&newArray_CbmMuchModule);
      instance.SetDelete(&delete_CbmMuchModule);
      instance.SetDeleteArray(&deleteArray_CbmMuchModule);
      instance.SetDestructor(&destruct_CbmMuchModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchModule*)
   {
      return GenerateInitInstanceLocal((::CbmMuchModule*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchModule*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchModuleGem(void *p = nullptr);
   static void *newArray_CbmMuchModuleGem(Long_t size, void *p);
   static void delete_CbmMuchModuleGem(void *p);
   static void deleteArray_CbmMuchModuleGem(void *p);
   static void destruct_CbmMuchModuleGem(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchModuleGem*)
   {
      ::CbmMuchModuleGem *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchModuleGem >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchModuleGem", ::CbmMuchModuleGem::Class_Version(), "CbmMuchModuleGem.h", 28,
                  typeid(::CbmMuchModuleGem), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchModuleGem::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchModuleGem) );
      instance.SetNew(&new_CbmMuchModuleGem);
      instance.SetNewArray(&newArray_CbmMuchModuleGem);
      instance.SetDelete(&delete_CbmMuchModuleGem);
      instance.SetDeleteArray(&deleteArray_CbmMuchModuleGem);
      instance.SetDestructor(&destruct_CbmMuchModuleGem);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchModuleGem*)
   {
      return GenerateInitInstanceLocal((::CbmMuchModuleGem*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchModuleGem*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchModuleGemRadial(void *p = nullptr);
   static void *newArray_CbmMuchModuleGemRadial(Long_t size, void *p);
   static void delete_CbmMuchModuleGemRadial(void *p);
   static void deleteArray_CbmMuchModuleGemRadial(void *p);
   static void destruct_CbmMuchModuleGemRadial(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchModuleGemRadial*)
   {
      ::CbmMuchModuleGemRadial *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchModuleGemRadial >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchModuleGemRadial", ::CbmMuchModuleGemRadial::Class_Version(), "CbmMuchModuleGemRadial.h", 25,
                  typeid(::CbmMuchModuleGemRadial), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchModuleGemRadial::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchModuleGemRadial) );
      instance.SetNew(&new_CbmMuchModuleGemRadial);
      instance.SetNewArray(&newArray_CbmMuchModuleGemRadial);
      instance.SetDelete(&delete_CbmMuchModuleGemRadial);
      instance.SetDeleteArray(&deleteArray_CbmMuchModuleGemRadial);
      instance.SetDestructor(&destruct_CbmMuchModuleGemRadial);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchModuleGemRadial*)
   {
      return GenerateInitInstanceLocal((::CbmMuchModuleGemRadial*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchModuleGemRadial*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchModuleGemRectangular(void *p = nullptr);
   static void *newArray_CbmMuchModuleGemRectangular(Long_t size, void *p);
   static void delete_CbmMuchModuleGemRectangular(void *p);
   static void deleteArray_CbmMuchModuleGemRectangular(void *p);
   static void destruct_CbmMuchModuleGemRectangular(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchModuleGemRectangular*)
   {
      ::CbmMuchModuleGemRectangular *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchModuleGemRectangular >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchModuleGemRectangular", ::CbmMuchModuleGemRectangular::Class_Version(), "CbmMuchModuleGemRectangular.h", 24,
                  typeid(::CbmMuchModuleGemRectangular), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchModuleGemRectangular::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchModuleGemRectangular) );
      instance.SetNew(&new_CbmMuchModuleGemRectangular);
      instance.SetNewArray(&newArray_CbmMuchModuleGemRectangular);
      instance.SetDelete(&delete_CbmMuchModuleGemRectangular);
      instance.SetDeleteArray(&deleteArray_CbmMuchModuleGemRectangular);
      instance.SetDestructor(&destruct_CbmMuchModuleGemRectangular);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchModuleGemRectangular*)
   {
      return GenerateInitInstanceLocal((::CbmMuchModuleGemRectangular*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchModuleGemRectangular*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchPadRadial(void *p = nullptr);
   static void *newArray_CbmMuchPadRadial(Long_t size, void *p);
   static void delete_CbmMuchPadRadial(void *p);
   static void deleteArray_CbmMuchPadRadial(void *p);
   static void destruct_CbmMuchPadRadial(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchPadRadial*)
   {
      ::CbmMuchPadRadial *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchPadRadial >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchPadRadial", ::CbmMuchPadRadial::Class_Version(), "CbmMuchPadRadial.h", 21,
                  typeid(::CbmMuchPadRadial), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchPadRadial::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchPadRadial) );
      instance.SetNew(&new_CbmMuchPadRadial);
      instance.SetNewArray(&newArray_CbmMuchPadRadial);
      instance.SetDelete(&delete_CbmMuchPadRadial);
      instance.SetDeleteArray(&deleteArray_CbmMuchPadRadial);
      instance.SetDestructor(&destruct_CbmMuchPadRadial);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchPadRadial*)
   {
      return GenerateInitInstanceLocal((::CbmMuchPadRadial*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchPadRadial*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchPadRectangular(void *p = nullptr);
   static void *newArray_CbmMuchPadRectangular(Long_t size, void *p);
   static void delete_CbmMuchPadRectangular(void *p);
   static void deleteArray_CbmMuchPadRectangular(void *p);
   static void destruct_CbmMuchPadRectangular(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchPadRectangular*)
   {
      ::CbmMuchPadRectangular *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchPadRectangular >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchPadRectangular", ::CbmMuchPadRectangular::Class_Version(), "CbmMuchPadRectangular.h", 25,
                  typeid(::CbmMuchPadRectangular), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchPadRectangular::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchPadRectangular) );
      instance.SetNew(&new_CbmMuchPadRectangular);
      instance.SetNewArray(&newArray_CbmMuchPadRectangular);
      instance.SetDelete(&delete_CbmMuchPadRectangular);
      instance.SetDeleteArray(&deleteArray_CbmMuchPadRectangular);
      instance.SetDestructor(&destruct_CbmMuchPadRectangular);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchPadRectangular*)
   {
      return GenerateInitInstanceLocal((::CbmMuchPadRectangular*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchPadRectangular*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchPointInfo(void *p = nullptr);
   static void *newArray_CbmMuchPointInfo(Long_t size, void *p);
   static void delete_CbmMuchPointInfo(void *p);
   static void deleteArray_CbmMuchPointInfo(void *p);
   static void destruct_CbmMuchPointInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchPointInfo*)
   {
      ::CbmMuchPointInfo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchPointInfo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchPointInfo", ::CbmMuchPointInfo::Class_Version(), "CbmMuchPointInfo.h", 15,
                  typeid(::CbmMuchPointInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchPointInfo::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchPointInfo) );
      instance.SetNew(&new_CbmMuchPointInfo);
      instance.SetNewArray(&newArray_CbmMuchPointInfo);
      instance.SetDelete(&delete_CbmMuchPointInfo);
      instance.SetDeleteArray(&deleteArray_CbmMuchPointInfo);
      instance.SetDestructor(&destruct_CbmMuchPointInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchPointInfo*)
   {
      return GenerateInitInstanceLocal((::CbmMuchPointInfo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchPointInfo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchSector(void *p = nullptr);
   static void *newArray_CbmMuchSector(Long_t size, void *p);
   static void delete_CbmMuchSector(void *p);
   static void deleteArray_CbmMuchSector(void *p);
   static void destruct_CbmMuchSector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchSector*)
   {
      ::CbmMuchSector *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchSector >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchSector", ::CbmMuchSector::Class_Version(), "CbmMuchSector.h", 26,
                  typeid(::CbmMuchSector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchSector::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchSector) );
      instance.SetNew(&new_CbmMuchSector);
      instance.SetNewArray(&newArray_CbmMuchSector);
      instance.SetDelete(&delete_CbmMuchSector);
      instance.SetDeleteArray(&deleteArray_CbmMuchSector);
      instance.SetDestructor(&destruct_CbmMuchSector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchSector*)
   {
      return GenerateInitInstanceLocal((::CbmMuchSector*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchSector*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchSectorRadial(void *p = nullptr);
   static void *newArray_CbmMuchSectorRadial(Long_t size, void *p);
   static void delete_CbmMuchSectorRadial(void *p);
   static void deleteArray_CbmMuchSectorRadial(void *p);
   static void destruct_CbmMuchSectorRadial(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchSectorRadial*)
   {
      ::CbmMuchSectorRadial *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchSectorRadial >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchSectorRadial", ::CbmMuchSectorRadial::Class_Version(), "CbmMuchSectorRadial.h", 23,
                  typeid(::CbmMuchSectorRadial), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchSectorRadial::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchSectorRadial) );
      instance.SetNew(&new_CbmMuchSectorRadial);
      instance.SetNewArray(&newArray_CbmMuchSectorRadial);
      instance.SetDelete(&delete_CbmMuchSectorRadial);
      instance.SetDeleteArray(&deleteArray_CbmMuchSectorRadial);
      instance.SetDestructor(&destruct_CbmMuchSectorRadial);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchSectorRadial*)
   {
      return GenerateInitInstanceLocal((::CbmMuchSectorRadial*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchSectorRadial*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchSectorRectangular(void *p = nullptr);
   static void *newArray_CbmMuchSectorRectangular(Long_t size, void *p);
   static void delete_CbmMuchSectorRectangular(void *p);
   static void deleteArray_CbmMuchSectorRectangular(void *p);
   static void destruct_CbmMuchSectorRectangular(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchSectorRectangular*)
   {
      ::CbmMuchSectorRectangular *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchSectorRectangular >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchSectorRectangular", ::CbmMuchSectorRectangular::Class_Version(), "CbmMuchSectorRectangular.h", 28,
                  typeid(::CbmMuchSectorRectangular), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchSectorRectangular::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchSectorRectangular) );
      instance.SetNew(&new_CbmMuchSectorRectangular);
      instance.SetNewArray(&newArray_CbmMuchSectorRectangular);
      instance.SetDelete(&delete_CbmMuchSectorRectangular);
      instance.SetDeleteArray(&deleteArray_CbmMuchSectorRectangular);
      instance.SetDestructor(&destruct_CbmMuchSectorRectangular);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchSectorRectangular*)
   {
      return GenerateInitInstanceLocal((::CbmMuchSectorRectangular*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchSectorRectangular*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchStation(void *p = nullptr);
   static void *newArray_CbmMuchStation(Long_t size, void *p);
   static void delete_CbmMuchStation(void *p);
   static void deleteArray_CbmMuchStation(void *p);
   static void destruct_CbmMuchStation(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchStation*)
   {
      ::CbmMuchStation *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchStation >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchStation", ::CbmMuchStation::Class_Version(), "CbmMuchStation.h", 26,
                  typeid(::CbmMuchStation), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchStation::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchStation) );
      instance.SetNew(&new_CbmMuchStation);
      instance.SetNewArray(&newArray_CbmMuchStation);
      instance.SetDelete(&delete_CbmMuchStation);
      instance.SetDeleteArray(&deleteArray_CbmMuchStation);
      instance.SetDestructor(&destruct_CbmMuchStation);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchStation*)
   {
      return GenerateInitInstanceLocal((::CbmMuchStation*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchStation*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchSegmentAuto(void *p = nullptr);
   static void *newArray_CbmMuchSegmentAuto(Long_t size, void *p);
   static void delete_CbmMuchSegmentAuto(void *p);
   static void deleteArray_CbmMuchSegmentAuto(void *p);
   static void destruct_CbmMuchSegmentAuto(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchSegmentAuto*)
   {
      ::CbmMuchSegmentAuto *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchSegmentAuto >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchSegmentAuto", ::CbmMuchSegmentAuto::Class_Version(), "CbmMuchSegmentAuto.h", 33,
                  typeid(::CbmMuchSegmentAuto), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchSegmentAuto::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchSegmentAuto) );
      instance.SetNew(&new_CbmMuchSegmentAuto);
      instance.SetNewArray(&newArray_CbmMuchSegmentAuto);
      instance.SetDelete(&delete_CbmMuchSegmentAuto);
      instance.SetDeleteArray(&deleteArray_CbmMuchSegmentAuto);
      instance.SetDestructor(&destruct_CbmMuchSegmentAuto);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchSegmentAuto*)
   {
      return GenerateInitInstanceLocal((::CbmMuchSegmentAuto*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchSegmentAuto*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchSegmentManual(void *p = nullptr);
   static void *newArray_CbmMuchSegmentManual(Long_t size, void *p);
   static void delete_CbmMuchSegmentManual(void *p);
   static void deleteArray_CbmMuchSegmentManual(void *p);
   static void destruct_CbmMuchSegmentManual(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchSegmentManual*)
   {
      ::CbmMuchSegmentManual *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchSegmentManual >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchSegmentManual", ::CbmMuchSegmentManual::Class_Version(), "CbmMuchSegmentManual.h", 35,
                  typeid(::CbmMuchSegmentManual), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchSegmentManual::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchSegmentManual) );
      instance.SetNew(&new_CbmMuchSegmentManual);
      instance.SetNewArray(&newArray_CbmMuchSegmentManual);
      instance.SetDelete(&delete_CbmMuchSegmentManual);
      instance.SetDeleteArray(&deleteArray_CbmMuchSegmentManual);
      instance.SetDestructor(&destruct_CbmMuchSegmentManual);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchSegmentManual*)
   {
      return GenerateInitInstanceLocal((::CbmMuchSegmentManual*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchSegmentManual*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchSegmentSector(void *p = nullptr);
   static void *newArray_CbmMuchSegmentSector(Long_t size, void *p);
   static void delete_CbmMuchSegmentSector(void *p);
   static void deleteArray_CbmMuchSegmentSector(void *p);
   static void destruct_CbmMuchSegmentSector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchSegmentSector*)
   {
      ::CbmMuchSegmentSector *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchSegmentSector >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchSegmentSector", ::CbmMuchSegmentSector::Class_Version(), "CbmMuchSegmentSector.h", 37,
                  typeid(::CbmMuchSegmentSector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchSegmentSector::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchSegmentSector) );
      instance.SetNew(&new_CbmMuchSegmentSector);
      instance.SetNewArray(&newArray_CbmMuchSegmentSector);
      instance.SetDelete(&delete_CbmMuchSegmentSector);
      instance.SetDeleteArray(&deleteArray_CbmMuchSegmentSector);
      instance.SetDestructor(&destruct_CbmMuchSegmentSector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchSegmentSector*)
   {
      return GenerateInitInstanceLocal((::CbmMuchSegmentSector*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchSegmentSector*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchTrackingInterface(void *p = nullptr);
   static void *newArray_CbmMuchTrackingInterface(Long_t size, void *p);
   static void delete_CbmMuchTrackingInterface(void *p);
   static void deleteArray_CbmMuchTrackingInterface(void *p);
   static void destruct_CbmMuchTrackingInterface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchTrackingInterface*)
   {
      ::CbmMuchTrackingInterface *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchTrackingInterface >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchTrackingInterface", ::CbmMuchTrackingInterface::Class_Version(), "CbmMuchTrackingInterface.h", 37,
                  typeid(::CbmMuchTrackingInterface), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchTrackingInterface::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchTrackingInterface) );
      instance.SetNew(&new_CbmMuchTrackingInterface);
      instance.SetNewArray(&newArray_CbmMuchTrackingInterface);
      instance.SetDelete(&delete_CbmMuchTrackingInterface);
      instance.SetDeleteArray(&deleteArray_CbmMuchTrackingInterface);
      instance.SetDestructor(&destruct_CbmMuchTrackingInterface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchTrackingInterface*)
   {
      return GenerateInitInstanceLocal((::CbmMuchTrackingInterface*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchTrackingInterface*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmGeoMuchPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGeoMuchPar::Class_Name()
{
   return "CbmGeoMuchPar";
}

//______________________________________________________________________________
const char *CbmGeoMuchPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoMuchPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGeoMuchPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoMuchPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGeoMuchPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoMuchPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGeoMuchPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGeoMuchPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMcbm2018MuchPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMcbm2018MuchPar::Class_Name()
{
   return "CbmMcbm2018MuchPar";
}

//______________________________________________________________________________
const char *CbmMcbm2018MuchPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MuchPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMcbm2018MuchPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MuchPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMcbm2018MuchPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MuchPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMcbm2018MuchPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMcbm2018MuchPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchUnpackPar::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchUnpackPar::Class_Name()
{
   return "CbmMuchUnpackPar";
}

//______________________________________________________________________________
const char *CbmMuchUnpackPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackPar*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchUnpackPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackPar*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchUnpackPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackPar*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchUnpackPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchUnpackPar*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchContFact::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchContFact::Class_Name()
{
   return "CbmMuchContFact";
}

//______________________________________________________________________________
const char *CbmMuchContFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchContFact*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchContFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchContFact*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchContFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchContFact*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchContFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchContFact*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchGeoScheme::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchGeoScheme::Class_Name()
{
   return "CbmMuchGeoScheme";
}

//______________________________________________________________________________
const char *CbmMuchGeoScheme::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchGeoScheme*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchGeoScheme::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchGeoScheme*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchGeoScheme::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchGeoScheme*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchGeoScheme::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchGeoScheme*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchLayerSide::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchLayerSide::Class_Name()
{
   return "CbmMuchLayerSide";
}

//______________________________________________________________________________
const char *CbmMuchLayerSide::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchLayerSide*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchLayerSide::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchLayerSide*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchLayerSide::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchLayerSide*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchLayerSide::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchLayerSide*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchLayer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchLayer::Class_Name()
{
   return "CbmMuchLayer";
}

//______________________________________________________________________________
const char *CbmMuchLayer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchLayer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchLayer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchLayer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchLayer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchLayer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchLayer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchLayer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchModule::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchModule::Class_Name()
{
   return "CbmMuchModule";
}

//______________________________________________________________________________
const char *CbmMuchModule::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModule*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchModule::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModule*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchModule::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModule*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchModule::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModule*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchModuleGem::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchModuleGem::Class_Name()
{
   return "CbmMuchModuleGem";
}

//______________________________________________________________________________
const char *CbmMuchModuleGem::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModuleGem*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchModuleGem::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModuleGem*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchModuleGem::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModuleGem*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchModuleGem::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModuleGem*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchModuleGemRadial::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchModuleGemRadial::Class_Name()
{
   return "CbmMuchModuleGemRadial";
}

//______________________________________________________________________________
const char *CbmMuchModuleGemRadial::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModuleGemRadial*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchModuleGemRadial::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModuleGemRadial*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchModuleGemRadial::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModuleGemRadial*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchModuleGemRadial::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModuleGemRadial*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchModuleGemRectangular::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchModuleGemRectangular::Class_Name()
{
   return "CbmMuchModuleGemRectangular";
}

//______________________________________________________________________________
const char *CbmMuchModuleGemRectangular::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModuleGemRectangular*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchModuleGemRectangular::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModuleGemRectangular*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchModuleGemRectangular::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModuleGemRectangular*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchModuleGemRectangular::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchModuleGemRectangular*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchPadRadial::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchPadRadial::Class_Name()
{
   return "CbmMuchPadRadial";
}

//______________________________________________________________________________
const char *CbmMuchPadRadial::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPadRadial*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchPadRadial::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPadRadial*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchPadRadial::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPadRadial*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchPadRadial::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPadRadial*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchPadRectangular::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchPadRectangular::Class_Name()
{
   return "CbmMuchPadRectangular";
}

//______________________________________________________________________________
const char *CbmMuchPadRectangular::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPadRectangular*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchPadRectangular::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPadRectangular*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchPadRectangular::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPadRectangular*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchPadRectangular::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPadRectangular*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchPointInfo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchPointInfo::Class_Name()
{
   return "CbmMuchPointInfo";
}

//______________________________________________________________________________
const char *CbmMuchPointInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPointInfo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchPointInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPointInfo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchPointInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPointInfo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchPointInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPointInfo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchSector::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchSector::Class_Name()
{
   return "CbmMuchSector";
}

//______________________________________________________________________________
const char *CbmMuchSector::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSector*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchSector::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSector*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchSector::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSector*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchSector::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSector*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchSectorRadial::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchSectorRadial::Class_Name()
{
   return "CbmMuchSectorRadial";
}

//______________________________________________________________________________
const char *CbmMuchSectorRadial::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSectorRadial*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchSectorRadial::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSectorRadial*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchSectorRadial::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSectorRadial*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchSectorRadial::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSectorRadial*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchSectorRectangular::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchSectorRectangular::Class_Name()
{
   return "CbmMuchSectorRectangular";
}

//______________________________________________________________________________
const char *CbmMuchSectorRectangular::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSectorRectangular*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchSectorRectangular::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSectorRectangular*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchSectorRectangular::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSectorRectangular*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchSectorRectangular::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSectorRectangular*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchStation::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchStation::Class_Name()
{
   return "CbmMuchStation";
}

//______________________________________________________________________________
const char *CbmMuchStation::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchStation*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchStation::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchStation*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchStation::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchStation*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchStation::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchStation*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchSegmentAuto::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchSegmentAuto::Class_Name()
{
   return "CbmMuchSegmentAuto";
}

//______________________________________________________________________________
const char *CbmMuchSegmentAuto::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSegmentAuto*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchSegmentAuto::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSegmentAuto*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchSegmentAuto::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSegmentAuto*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchSegmentAuto::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSegmentAuto*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchSegmentManual::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchSegmentManual::Class_Name()
{
   return "CbmMuchSegmentManual";
}

//______________________________________________________________________________
const char *CbmMuchSegmentManual::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSegmentManual*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchSegmentManual::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSegmentManual*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchSegmentManual::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSegmentManual*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchSegmentManual::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSegmentManual*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchSegmentSector::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchSegmentSector::Class_Name()
{
   return "CbmMuchSegmentSector";
}

//______________________________________________________________________________
const char *CbmMuchSegmentSector::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSegmentSector*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchSegmentSector::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSegmentSector*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchSegmentSector::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSegmentSector*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchSegmentSector::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchSegmentSector*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchTrackingInterface::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchTrackingInterface::Class_Name()
{
   return "CbmMuchTrackingInterface";
}

//______________________________________________________________________________
const char *CbmMuchTrackingInterface::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrackingInterface*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchTrackingInterface::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrackingInterface*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchTrackingInterface::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrackingInterface*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchTrackingInterface::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrackingInterface*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmGeoMuchPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGeoMuchPar.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairParGenericSet::Streamer(R__b);
      R__b >> fGeoSensNodes;
      R__b >> fGeoPassNodes;
      R__b >> fStations;
      R__b.CheckByteCount(R__s, R__c, CbmGeoMuchPar::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmGeoMuchPar::IsA(), kTRUE);
      FairParGenericSet::Streamer(R__b);
      R__b << fGeoSensNodes;
      R__b << fGeoPassNodes;
      R__b << fStations;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGeoMuchPar(void *p) {
      return  p ? new(p) ::CbmGeoMuchPar : new ::CbmGeoMuchPar;
   }
   static void *newArray_CbmGeoMuchPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGeoMuchPar[nElements] : new ::CbmGeoMuchPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGeoMuchPar(void *p) {
      delete ((::CbmGeoMuchPar*)p);
   }
   static void deleteArray_CbmGeoMuchPar(void *p) {
      delete [] ((::CbmGeoMuchPar*)p);
   }
   static void destruct_CbmGeoMuchPar(void *p) {
      typedef ::CbmGeoMuchPar current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmGeoMuchPar(TBuffer &buf, void *obj) {
      ((::CbmGeoMuchPar*)obj)->::CbmGeoMuchPar::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmGeoMuchPar

//______________________________________________________________________________
void CbmMcbm2018MuchPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMcbm2018MuchPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMcbm2018MuchPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMcbm2018MuchPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMcbm2018MuchPar(void *p) {
      return  p ? new(p) ::CbmMcbm2018MuchPar : new ::CbmMcbm2018MuchPar;
   }
   static void *newArray_CbmMcbm2018MuchPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMcbm2018MuchPar[nElements] : new ::CbmMcbm2018MuchPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMcbm2018MuchPar(void *p) {
      delete ((::CbmMcbm2018MuchPar*)p);
   }
   static void deleteArray_CbmMcbm2018MuchPar(void *p) {
      delete [] ((::CbmMcbm2018MuchPar*)p);
   }
   static void destruct_CbmMcbm2018MuchPar(void *p) {
      typedef ::CbmMcbm2018MuchPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMcbm2018MuchPar

//______________________________________________________________________________
void CbmMuchUnpackPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchUnpackPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchUnpackPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchUnpackPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchUnpackPar(void *p) {
      return  p ? new(p) ::CbmMuchUnpackPar : new ::CbmMuchUnpackPar;
   }
   static void *newArray_CbmMuchUnpackPar(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchUnpackPar[nElements] : new ::CbmMuchUnpackPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchUnpackPar(void *p) {
      delete ((::CbmMuchUnpackPar*)p);
   }
   static void deleteArray_CbmMuchUnpackPar(void *p) {
      delete [] ((::CbmMuchUnpackPar*)p);
   }
   static void destruct_CbmMuchUnpackPar(void *p) {
      typedef ::CbmMuchUnpackPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchUnpackPar

//______________________________________________________________________________
void CbmMuchContFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchContFact.

   FairContFact::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchContFact(void *p) {
      return  p ? new(p) ::CbmMuchContFact : new ::CbmMuchContFact;
   }
   static void *newArray_CbmMuchContFact(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchContFact[nElements] : new ::CbmMuchContFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchContFact(void *p) {
      delete ((::CbmMuchContFact*)p);
   }
   static void deleteArray_CbmMuchContFact(void *p) {
      delete [] ((::CbmMuchContFact*)p);
   }
   static void destruct_CbmMuchContFact(void *p) {
      typedef ::CbmMuchContFact current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMuchContFact(TBuffer &buf, void *obj) {
      ((::CbmMuchContFact*)obj)->::CbmMuchContFact::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMuchContFact

//______________________________________________________________________________
void CbmMuchGeoScheme::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchGeoScheme.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchGeoScheme::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchGeoScheme::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmMuchGeoScheme(void *p) {
      delete ((::CbmMuchGeoScheme*)p);
   }
   static void deleteArray_CbmMuchGeoScheme(void *p) {
      delete [] ((::CbmMuchGeoScheme*)p);
   }
   static void destruct_CbmMuchGeoScheme(void *p) {
      typedef ::CbmMuchGeoScheme current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchGeoScheme

//______________________________________________________________________________
void CbmMuchLayerSide::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchLayerSide.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchLayerSide::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchLayerSide::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchLayerSide(void *p) {
      return  p ? new(p) ::CbmMuchLayerSide : new ::CbmMuchLayerSide;
   }
   static void *newArray_CbmMuchLayerSide(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchLayerSide[nElements] : new ::CbmMuchLayerSide[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchLayerSide(void *p) {
      delete ((::CbmMuchLayerSide*)p);
   }
   static void deleteArray_CbmMuchLayerSide(void *p) {
      delete [] ((::CbmMuchLayerSide*)p);
   }
   static void destruct_CbmMuchLayerSide(void *p) {
      typedef ::CbmMuchLayerSide current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchLayerSide

//______________________________________________________________________________
void CbmMuchLayer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchLayer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchLayer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchLayer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchLayer(void *p) {
      return  p ? new(p) ::CbmMuchLayer : new ::CbmMuchLayer;
   }
   static void *newArray_CbmMuchLayer(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchLayer[nElements] : new ::CbmMuchLayer[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchLayer(void *p) {
      delete ((::CbmMuchLayer*)p);
   }
   static void deleteArray_CbmMuchLayer(void *p) {
      delete [] ((::CbmMuchLayer*)p);
   }
   static void destruct_CbmMuchLayer(void *p) {
      typedef ::CbmMuchLayer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchLayer

//______________________________________________________________________________
void CbmMuchModule::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchModule.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchModule::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchModule::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchModule(void *p) {
      return  p ? new(p) ::CbmMuchModule : new ::CbmMuchModule;
   }
   static void *newArray_CbmMuchModule(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchModule[nElements] : new ::CbmMuchModule[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchModule(void *p) {
      delete ((::CbmMuchModule*)p);
   }
   static void deleteArray_CbmMuchModule(void *p) {
      delete [] ((::CbmMuchModule*)p);
   }
   static void destruct_CbmMuchModule(void *p) {
      typedef ::CbmMuchModule current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchModule

//______________________________________________________________________________
void CbmMuchModuleGem::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchModuleGem.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchModuleGem::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchModuleGem::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchModuleGem(void *p) {
      return  p ? new(p) ::CbmMuchModuleGem : new ::CbmMuchModuleGem;
   }
   static void *newArray_CbmMuchModuleGem(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchModuleGem[nElements] : new ::CbmMuchModuleGem[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchModuleGem(void *p) {
      delete ((::CbmMuchModuleGem*)p);
   }
   static void deleteArray_CbmMuchModuleGem(void *p) {
      delete [] ((::CbmMuchModuleGem*)p);
   }
   static void destruct_CbmMuchModuleGem(void *p) {
      typedef ::CbmMuchModuleGem current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchModuleGem

//______________________________________________________________________________
void CbmMuchModuleGemRadial::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchModuleGemRadial.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchModuleGemRadial::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchModuleGemRadial::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchModuleGemRadial(void *p) {
      return  p ? new(p) ::CbmMuchModuleGemRadial : new ::CbmMuchModuleGemRadial;
   }
   static void *newArray_CbmMuchModuleGemRadial(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchModuleGemRadial[nElements] : new ::CbmMuchModuleGemRadial[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchModuleGemRadial(void *p) {
      delete ((::CbmMuchModuleGemRadial*)p);
   }
   static void deleteArray_CbmMuchModuleGemRadial(void *p) {
      delete [] ((::CbmMuchModuleGemRadial*)p);
   }
   static void destruct_CbmMuchModuleGemRadial(void *p) {
      typedef ::CbmMuchModuleGemRadial current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchModuleGemRadial

//______________________________________________________________________________
void CbmMuchModuleGemRectangular::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchModuleGemRectangular.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchModuleGemRectangular::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchModuleGemRectangular::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchModuleGemRectangular(void *p) {
      return  p ? new(p) ::CbmMuchModuleGemRectangular : new ::CbmMuchModuleGemRectangular;
   }
   static void *newArray_CbmMuchModuleGemRectangular(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchModuleGemRectangular[nElements] : new ::CbmMuchModuleGemRectangular[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchModuleGemRectangular(void *p) {
      delete ((::CbmMuchModuleGemRectangular*)p);
   }
   static void deleteArray_CbmMuchModuleGemRectangular(void *p) {
      delete [] ((::CbmMuchModuleGemRectangular*)p);
   }
   static void destruct_CbmMuchModuleGemRectangular(void *p) {
      typedef ::CbmMuchModuleGemRectangular current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchModuleGemRectangular

//______________________________________________________________________________
void CbmMuchPadRadial::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchPadRadial.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchPadRadial::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchPadRadial::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchPadRadial(void *p) {
      return  p ? new(p) ::CbmMuchPadRadial : new ::CbmMuchPadRadial;
   }
   static void *newArray_CbmMuchPadRadial(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchPadRadial[nElements] : new ::CbmMuchPadRadial[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchPadRadial(void *p) {
      delete ((::CbmMuchPadRadial*)p);
   }
   static void deleteArray_CbmMuchPadRadial(void *p) {
      delete [] ((::CbmMuchPadRadial*)p);
   }
   static void destruct_CbmMuchPadRadial(void *p) {
      typedef ::CbmMuchPadRadial current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchPadRadial

//______________________________________________________________________________
void CbmMuchPadRectangular::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchPadRectangular.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchPadRectangular::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchPadRectangular::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchPadRectangular(void *p) {
      return  p ? new(p) ::CbmMuchPadRectangular : new ::CbmMuchPadRectangular;
   }
   static void *newArray_CbmMuchPadRectangular(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchPadRectangular[nElements] : new ::CbmMuchPadRectangular[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchPadRectangular(void *p) {
      delete ((::CbmMuchPadRectangular*)p);
   }
   static void deleteArray_CbmMuchPadRectangular(void *p) {
      delete [] ((::CbmMuchPadRectangular*)p);
   }
   static void destruct_CbmMuchPadRectangular(void *p) {
      typedef ::CbmMuchPadRectangular current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchPadRectangular

//______________________________________________________________________________
void CbmMuchPointInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchPointInfo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchPointInfo::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchPointInfo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchPointInfo(void *p) {
      return  p ? new(p) ::CbmMuchPointInfo : new ::CbmMuchPointInfo;
   }
   static void *newArray_CbmMuchPointInfo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchPointInfo[nElements] : new ::CbmMuchPointInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchPointInfo(void *p) {
      delete ((::CbmMuchPointInfo*)p);
   }
   static void deleteArray_CbmMuchPointInfo(void *p) {
      delete [] ((::CbmMuchPointInfo*)p);
   }
   static void destruct_CbmMuchPointInfo(void *p) {
      typedef ::CbmMuchPointInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchPointInfo

//______________________________________________________________________________
void CbmMuchSector::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchSector.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchSector::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchSector::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchSector(void *p) {
      return  p ? new(p) ::CbmMuchSector : new ::CbmMuchSector;
   }
   static void *newArray_CbmMuchSector(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchSector[nElements] : new ::CbmMuchSector[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchSector(void *p) {
      delete ((::CbmMuchSector*)p);
   }
   static void deleteArray_CbmMuchSector(void *p) {
      delete [] ((::CbmMuchSector*)p);
   }
   static void destruct_CbmMuchSector(void *p) {
      typedef ::CbmMuchSector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchSector

//______________________________________________________________________________
void CbmMuchSectorRadial::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchSectorRadial.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchSectorRadial::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchSectorRadial::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchSectorRadial(void *p) {
      return  p ? new(p) ::CbmMuchSectorRadial : new ::CbmMuchSectorRadial;
   }
   static void *newArray_CbmMuchSectorRadial(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchSectorRadial[nElements] : new ::CbmMuchSectorRadial[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchSectorRadial(void *p) {
      delete ((::CbmMuchSectorRadial*)p);
   }
   static void deleteArray_CbmMuchSectorRadial(void *p) {
      delete [] ((::CbmMuchSectorRadial*)p);
   }
   static void destruct_CbmMuchSectorRadial(void *p) {
      typedef ::CbmMuchSectorRadial current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchSectorRadial

//______________________________________________________________________________
void CbmMuchSectorRectangular::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchSectorRectangular.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchSectorRectangular::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchSectorRectangular::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchSectorRectangular(void *p) {
      return  p ? new(p) ::CbmMuchSectorRectangular : new ::CbmMuchSectorRectangular;
   }
   static void *newArray_CbmMuchSectorRectangular(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchSectorRectangular[nElements] : new ::CbmMuchSectorRectangular[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchSectorRectangular(void *p) {
      delete ((::CbmMuchSectorRectangular*)p);
   }
   static void deleteArray_CbmMuchSectorRectangular(void *p) {
      delete [] ((::CbmMuchSectorRectangular*)p);
   }
   static void destruct_CbmMuchSectorRectangular(void *p) {
      typedef ::CbmMuchSectorRectangular current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchSectorRectangular

//______________________________________________________________________________
void CbmMuchStation::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchStation.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchStation::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchStation::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchStation(void *p) {
      return  p ? new(p) ::CbmMuchStation : new ::CbmMuchStation;
   }
   static void *newArray_CbmMuchStation(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchStation[nElements] : new ::CbmMuchStation[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchStation(void *p) {
      delete ((::CbmMuchStation*)p);
   }
   static void deleteArray_CbmMuchStation(void *p) {
      delete [] ((::CbmMuchStation*)p);
   }
   static void destruct_CbmMuchStation(void *p) {
      typedef ::CbmMuchStation current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchStation

//______________________________________________________________________________
void CbmMuchSegmentAuto::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchSegmentAuto.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchSegmentAuto::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchSegmentAuto::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchSegmentAuto(void *p) {
      return  p ? new(p) ::CbmMuchSegmentAuto : new ::CbmMuchSegmentAuto;
   }
   static void *newArray_CbmMuchSegmentAuto(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchSegmentAuto[nElements] : new ::CbmMuchSegmentAuto[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchSegmentAuto(void *p) {
      delete ((::CbmMuchSegmentAuto*)p);
   }
   static void deleteArray_CbmMuchSegmentAuto(void *p) {
      delete [] ((::CbmMuchSegmentAuto*)p);
   }
   static void destruct_CbmMuchSegmentAuto(void *p) {
      typedef ::CbmMuchSegmentAuto current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchSegmentAuto

//______________________________________________________________________________
void CbmMuchSegmentManual::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchSegmentManual.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchSegmentManual::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchSegmentManual::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchSegmentManual(void *p) {
      return  p ? new(p) ::CbmMuchSegmentManual : new ::CbmMuchSegmentManual;
   }
   static void *newArray_CbmMuchSegmentManual(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchSegmentManual[nElements] : new ::CbmMuchSegmentManual[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchSegmentManual(void *p) {
      delete ((::CbmMuchSegmentManual*)p);
   }
   static void deleteArray_CbmMuchSegmentManual(void *p) {
      delete [] ((::CbmMuchSegmentManual*)p);
   }
   static void destruct_CbmMuchSegmentManual(void *p) {
      typedef ::CbmMuchSegmentManual current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchSegmentManual

//______________________________________________________________________________
void CbmMuchSegmentSector::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchSegmentSector.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchSegmentSector::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchSegmentSector::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchSegmentSector(void *p) {
      return  p ? new(p) ::CbmMuchSegmentSector : new ::CbmMuchSegmentSector;
   }
   static void *newArray_CbmMuchSegmentSector(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchSegmentSector[nElements] : new ::CbmMuchSegmentSector[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchSegmentSector(void *p) {
      delete ((::CbmMuchSegmentSector*)p);
   }
   static void deleteArray_CbmMuchSegmentSector(void *p) {
      delete [] ((::CbmMuchSegmentSector*)p);
   }
   static void destruct_CbmMuchSegmentSector(void *p) {
      typedef ::CbmMuchSegmentSector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchSegmentSector

//______________________________________________________________________________
void CbmMuchTrackingInterface::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchTrackingInterface.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchTrackingInterface::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchTrackingInterface::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchTrackingInterface(void *p) {
      return  p ? new(p) ::CbmMuchTrackingInterface : new ::CbmMuchTrackingInterface;
   }
   static void *newArray_CbmMuchTrackingInterface(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchTrackingInterface[nElements] : new ::CbmMuchTrackingInterface[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchTrackingInterface(void *p) {
      delete ((::CbmMuchTrackingInterface*)p);
   }
   static void deleteArray_CbmMuchTrackingInterface(void *p) {
      delete [] ((::CbmMuchTrackingInterface*)p);
   }
   static void destruct_CbmMuchTrackingInterface(void *p) {
      typedef ::CbmMuchTrackingInterface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchTrackingInterface

namespace ROOT {
   static TClass *vectorlEvectorlEintgRsPgR_Dictionary();
   static void vectorlEvectorlEintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEintgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEintgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEintgRsPgR(void *p);
   static void destruct_vectorlEvectorlEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<int> >*)
   {
      vector<vector<int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<int> >", -2, "vector", 389,
                  typeid(vector<vector<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<int> >) );
      instance.SetNew(&new_vectorlEvectorlEintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEintgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<int> > >()));

      ::ROOT::AddClassAlternate("vector<vector<int> >","std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<int> >*)nullptr)->GetClass();
      vectorlEvectorlEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<int> > : new vector<vector<int> >;
   }
   static void *newArray_vectorlEvectorlEintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<int> >[nElements] : new vector<vector<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEintgRsPgR(void *p) {
      delete ((vector<vector<int> >*)p);
   }
   static void deleteArray_vectorlEvectorlEintgRsPgR(void *p) {
      delete [] ((vector<vector<int> >*)p);
   }
   static void destruct_vectorlEvectorlEintgRsPgR(void *p) {
      typedef vector<vector<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<int> >

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
   static TClass *vectorlECbmMuchSectormUgR_Dictionary();
   static void vectorlECbmMuchSectormUgR_TClassManip(TClass*);
   static void *new_vectorlECbmMuchSectormUgR(void *p = nullptr);
   static void *newArray_vectorlECbmMuchSectormUgR(Long_t size, void *p);
   static void delete_vectorlECbmMuchSectormUgR(void *p);
   static void deleteArray_vectorlECbmMuchSectormUgR(void *p);
   static void destruct_vectorlECbmMuchSectormUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMuchSector*>*)
   {
      vector<CbmMuchSector*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMuchSector*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMuchSector*>", -2, "vector", 389,
                  typeid(vector<CbmMuchSector*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMuchSectormUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmMuchSector*>) );
      instance.SetNew(&new_vectorlECbmMuchSectormUgR);
      instance.SetNewArray(&newArray_vectorlECbmMuchSectormUgR);
      instance.SetDelete(&delete_vectorlECbmMuchSectormUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMuchSectormUgR);
      instance.SetDestructor(&destruct_vectorlECbmMuchSectormUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMuchSector*> >()));

      ::ROOT::AddClassAlternate("vector<CbmMuchSector*>","std::vector<CbmMuchSector*, std::allocator<CbmMuchSector*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMuchSector*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMuchSectormUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMuchSector*>*)nullptr)->GetClass();
      vectorlECbmMuchSectormUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMuchSectormUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMuchSectormUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMuchSector*> : new vector<CbmMuchSector*>;
   }
   static void *newArray_vectorlECbmMuchSectormUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMuchSector*>[nElements] : new vector<CbmMuchSector*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMuchSectormUgR(void *p) {
      delete ((vector<CbmMuchSector*>*)p);
   }
   static void deleteArray_vectorlECbmMuchSectormUgR(void *p) {
      delete [] ((vector<CbmMuchSector*>*)p);
   }
   static void destruct_vectorlECbmMuchSectormUgR(void *p) {
      typedef vector<CbmMuchSector*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMuchSector*>

namespace ROOT {
   static TClass *maplEintcOvectorlEintgRsPgR_Dictionary();
   static void maplEintcOvectorlEintgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOvectorlEintgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOvectorlEintgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOvectorlEintgRsPgR(void *p);
   static void deleteArray_maplEintcOvectorlEintgRsPgR(void *p);
   static void destruct_maplEintcOvectorlEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,vector<int> >*)
   {
      map<int,vector<int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,vector<int> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,vector<int> >", -2, "map", 100,
                  typeid(map<int,vector<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOvectorlEintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,vector<int> >) );
      instance.SetNew(&new_maplEintcOvectorlEintgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOvectorlEintgRsPgR);
      instance.SetDelete(&delete_maplEintcOvectorlEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOvectorlEintgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOvectorlEintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,vector<int> > >()));

      ::ROOT::AddClassAlternate("map<int,vector<int> >","std::map<int, std::vector<int, std::allocator<int> >, std::less<int>, std::allocator<std::pair<int const, std::vector<int, std::allocator<int> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,vector<int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOvectorlEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,vector<int> >*)nullptr)->GetClass();
      maplEintcOvectorlEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOvectorlEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOvectorlEintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<int> > : new map<int,vector<int> >;
   }
   static void *newArray_maplEintcOvectorlEintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<int> >[nElements] : new map<int,vector<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOvectorlEintgRsPgR(void *p) {
      delete ((map<int,vector<int> >*)p);
   }
   static void deleteArray_maplEintcOvectorlEintgRsPgR(void *p) {
      delete [] ((map<int,vector<int> >*)p);
   }
   static void destruct_maplEintcOvectorlEintgRsPgR(void *p) {
      typedef map<int,vector<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,vector<int> >

namespace ROOT {
   static TClass *maplEintcOvectorlEdoublegRsPgR_Dictionary();
   static void maplEintcOvectorlEdoublegRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOvectorlEdoublegRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOvectorlEdoublegRsPgR(Long_t size, void *p);
   static void delete_maplEintcOvectorlEdoublegRsPgR(void *p);
   static void deleteArray_maplEintcOvectorlEdoublegRsPgR(void *p);
   static void destruct_maplEintcOvectorlEdoublegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,vector<double> >*)
   {
      map<int,vector<double> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,vector<double> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,vector<double> >", -2, "map", 100,
                  typeid(map<int,vector<double> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOvectorlEdoublegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,vector<double> >) );
      instance.SetNew(&new_maplEintcOvectorlEdoublegRsPgR);
      instance.SetNewArray(&newArray_maplEintcOvectorlEdoublegRsPgR);
      instance.SetDelete(&delete_maplEintcOvectorlEdoublegRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOvectorlEdoublegRsPgR);
      instance.SetDestructor(&destruct_maplEintcOvectorlEdoublegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,vector<double> > >()));

      ::ROOT::AddClassAlternate("map<int,vector<double> >","std::map<int, std::vector<double, std::allocator<double> >, std::less<int>, std::allocator<std::pair<int const, std::vector<double, std::allocator<double> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,vector<double> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOvectorlEdoublegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,vector<double> >*)nullptr)->GetClass();
      maplEintcOvectorlEdoublegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOvectorlEdoublegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOvectorlEdoublegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<double> > : new map<int,vector<double> >;
   }
   static void *newArray_maplEintcOvectorlEdoublegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<double> >[nElements] : new map<int,vector<double> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOvectorlEdoublegRsPgR(void *p) {
      delete ((map<int,vector<double> >*)p);
   }
   static void deleteArray_maplEintcOvectorlEdoublegRsPgR(void *p) {
      delete [] ((map<int,vector<double> >*)p);
   }
   static void destruct_maplEintcOvectorlEdoublegRsPgR(void *p) {
      typedef map<int,vector<double> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,vector<double> >

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
  void TriggerDictionaryInitialization_libCbmMuchBase_Impl() {
    static const char* headers[] = {
"CbmGeoMuchPar.h",
"CbmMcbm2018MuchPar.h",
"CbmMuchUnpackPar.h",
"CbmMuchContFact.h",
"CbmMuchGeoScheme.h",
"CbmMuchLayer.h",
"CbmMuchLayerSide.h",
"CbmMuchModule.h",
"CbmMuchModuleGem.h",
"CbmMuchModuleGemRadial.h",
"CbmMuchModuleGemRectangular.h",
"CbmMuchPad.h",
"CbmMuchPadRadial.h",
"CbmMuchPadRectangular.h",
"CbmMuchPointInfo.h",
"CbmMuchSector.h",
"CbmMuchSectorRadial.h",
"CbmMuchSectorRectangular.h",
"CbmMuchStation.h",
"CbmMuchSegmentAuto.h",
"CbmMuchSegmentManual.h",
"CbmMuchSegmentSector.h",
"CbmMuchTrackingInterface.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/detectors/much",
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
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/core/detectors/much/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmMuchBase dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmGeoMuchPar.h")))  CbmGeoMuchPar;
class __attribute__((annotate("$clingAutoload$CbmMcbm2018MuchPar.h")))  CbmMcbm2018MuchPar;
class __attribute__((annotate("$clingAutoload$CbmMuchUnpackPar.h")))  CbmMuchUnpackPar;
class __attribute__((annotate(R"ATTRDUMP(Factory for all Much parameter containers)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmMuchContFact.h")))  CbmMuchContFact;
class __attribute__((annotate("$clingAutoload$CbmMuchGeoScheme.h")))  CbmMuchGeoScheme;
class __attribute__((annotate("$clingAutoload$CbmMuchLayerSide.h")))  __attribute__((annotate("$clingAutoload$CbmMuchLayer.h")))  CbmMuchLayerSide;
class __attribute__((annotate("$clingAutoload$CbmMuchLayer.h")))  CbmMuchLayer;
class __attribute__((annotate("$clingAutoload$CbmMuchModule.h")))  CbmMuchModule;
class __attribute__((annotate("$clingAutoload$CbmMuchModuleGem.h")))  CbmMuchModuleGem;
class __attribute__((annotate("$clingAutoload$CbmMuchModuleGemRadial.h")))  CbmMuchModuleGemRadial;
class __attribute__((annotate("$clingAutoload$CbmMuchModuleGemRectangular.h")))  CbmMuchModuleGemRectangular;
class __attribute__((annotate("$clingAutoload$CbmMuchPadRadial.h")))  CbmMuchPadRadial;
class __attribute__((annotate("$clingAutoload$CbmMuchPadRectangular.h")))  CbmMuchPadRectangular;
class __attribute__((annotate("$clingAutoload$CbmMuchPointInfo.h")))  CbmMuchPointInfo;
class __attribute__((annotate("$clingAutoload$CbmMuchSector.h")))  CbmMuchSector;
class __attribute__((annotate("$clingAutoload$CbmMuchSectorRadial.h")))  CbmMuchSectorRadial;
class __attribute__((annotate("$clingAutoload$CbmMuchSectorRectangular.h")))  CbmMuchSectorRectangular;
class __attribute__((annotate("$clingAutoload$CbmMuchStation.h")))  CbmMuchStation;
class __attribute__((annotate("$clingAutoload$CbmMuchSegmentAuto.h")))  CbmMuchSegmentAuto;
class __attribute__((annotate("$clingAutoload$CbmMuchSegmentManual.h")))  CbmMuchSegmentManual;
class __attribute__((annotate("$clingAutoload$CbmMuchSegmentSector.h")))  CbmMuchSegmentSector;
class __attribute__((annotate("$clingAutoload$CbmMuchTrackingInterface.h")))  CbmMuchTrackingInterface;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmMuchBase dictionary payload"

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
#include "CbmGeoMuchPar.h"
#include "CbmMcbm2018MuchPar.h"
#include "CbmMuchUnpackPar.h"
#include "CbmMuchContFact.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchLayer.h"
#include "CbmMuchLayerSide.h"
#include "CbmMuchModule.h"
#include "CbmMuchModuleGem.h"
#include "CbmMuchModuleGemRadial.h"
#include "CbmMuchModuleGemRectangular.h"
#include "CbmMuchPad.h"
#include "CbmMuchPadRadial.h"
#include "CbmMuchPadRectangular.h"
#include "CbmMuchPointInfo.h"
#include "CbmMuchSector.h"
#include "CbmMuchSectorRadial.h"
#include "CbmMuchSectorRectangular.h"
#include "CbmMuchStation.h"
#include "CbmMuchSegmentAuto.h"
#include "CbmMuchSegmentManual.h"
#include "CbmMuchSegmentSector.h"
#include "CbmMuchTrackingInterface.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmGeoMuchPar", payloadCode, "@",
"CbmMcbm2018MuchPar", payloadCode, "@",
"CbmMuchContFact", payloadCode, "@",
"CbmMuchGeoScheme", payloadCode, "@",
"CbmMuchLayer", payloadCode, "@",
"CbmMuchLayerSide", payloadCode, "@",
"CbmMuchModule", payloadCode, "@",
"CbmMuchModuleGem", payloadCode, "@",
"CbmMuchModuleGemRadial", payloadCode, "@",
"CbmMuchModuleGemRectangular", payloadCode, "@",
"CbmMuchPadRadial", payloadCode, "@",
"CbmMuchPadRectangular", payloadCode, "@",
"CbmMuchPointInfo", payloadCode, "@",
"CbmMuchSector", payloadCode, "@",
"CbmMuchSectorRadial", payloadCode, "@",
"CbmMuchSectorRectangular", payloadCode, "@",
"CbmMuchSegmentAuto", payloadCode, "@",
"CbmMuchSegmentManual", payloadCode, "@",
"CbmMuchSegmentSector", payloadCode, "@",
"CbmMuchStation", payloadCode, "@",
"CbmMuchTrackingInterface", payloadCode, "@",
"CbmMuchUnpackPar", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmMuchBase",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmMuchBase_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmMuchBase_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmMuchBase() {
  TriggerDictionaryInitialization_libCbmMuchBase_Impl();
}
