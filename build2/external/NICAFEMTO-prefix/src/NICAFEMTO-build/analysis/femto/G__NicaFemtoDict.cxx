// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIheinemanndIgpuunpackerdIcbmrootdIbuild2dIexternaldINICAFEMTOmIprefixdIsrcdINICAFEMTOmIbuilddIanalysisdIfemtodIG__NicaFemtoDict
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
#include "NicaFemtoBasicAna.h"
#include "NicaFemtoFreezoutsAna.h"
#include "NicaFemtoDumpPairAna.h"
#include "NicaFemtoEventBinsAna.h"
#include "NicaFemtoDPhiDEta.h"
#include "NicaFemto1DCF.h"
#include "NicaFemto3DCF.h"
#include "NicaFemto3DCFQinv.h"
#include "NicaFemtoFastCut.h"
#include "NicaFemtoFreezoutGenerator.h"
#include "NicaFemtoPairKinematics.h"
#include "NicaFemtoPair.h"
#include "NicaFemtoMiniPair.h"
#include "NicaFemtoSourceModel.h"
#include "NicaFemtoConst.h"
#include "NicaFemtoSourceModelExp.h"
#include "NicaFemtoSourceModelGauss.h"
#include "NicaFemtoSourceModelGausMt.h"
#include "NicaCorrFit.h"
#include "NicaCorrFitFunc.h"
#include "NicaCorrFitMath1DCF.h"
#include "NicaCorrFitMath3DCF.h"
#include "NicaCorrFit1DCF.h"
#include "NicaCorrFit3DCF.h"
#include "NicaCorrFitKisiel.h"
#include "NicaCorrFitWielanek.h"
#include "NicaCorrFitGUI.h"
#include "NicaCorrFitHDFunc.h"
#include "NicaFemto1DCFAnaMap.h"
#include "NicaFemto1DCFAnaMapMC.h"
#include "NicaWielanekMapGenerator.h"
#include "NicaFemto1DCFAnaMapPairsDumped.h"
#include "NicaCorrFit1DCFSquare.h"
#include "NicaCorrFit1DCFLorentz.h"
#include "NicaCorrFit3DCFSquare.h"
#include "NicaCorrFit3DCFLorentz.h"
#include "NicaCorrFit1DCFExp.h"
#include "NicaCorrFit1DCFGauss.h"
#include "NicaCorrFit1DCFPoly.h"
#include "NicaCorrFit3DCFGauss.h"
#include "NicaCorrFit3DCFGauss2.h"
#include "NicaCorrFit3DCFGaussCross.h"
#include "NicaCorrFit1DCFCumac.h"
#include "NicaCorrFit3DCFBowlerSinyukov.h"
#include "NicaCorrFit3DCFBowlerSinyukovExtra.h"
#include "NicaFemtoFsiParsed.h"
#include "NicaFemtoWeightGeneratorLednicky2.h"
#include "NicaFemtoWeightGenerator.h"
#include "NicaFemtoWeightGeneratorBasic.h"
#include "NicaFemtoWeightGeneratorLednicky.h"
#include "NicaFemtoWeightGeneratorLednickyResidual.h"
#include "NicaFemtoWeightGeneratorSupressed.h"
#include "NicaFemtoCorrFunc.h"
#include "NicaFemtoCorrFuncPt.h"
#include "NicaFemtoCorrFuncKt.h"
#include "NicaFemtoCorrFuncAzimuthalKt.h"
#include "NicaFemtoCorrFuncSimple.h"
#include "NicaFemtoImaging.h"
#include "NicaFemtoImaging1D.h"
#include "NicaFemtoImaging3D.h"
#include "NicaFemtoSHCF.h"
#include "NicaFemtoYlm.h"
#include "NicaCorrFitSHCF.h"
#include "NicaCorrFitSmear1DCF.h"
#include "NicaCorrFitDatabase.h"
#include "NicaCorrFitDumpedPairAna.h"
#include "NicaCorrFitInfo.h"
#include "NicaCorrFit3DCFMultiDim.h"
#include "NicaCorrFitFunctor.h"
#include "NicaCorrFitPairGeneratorYPt.h"
#include "NicaCorrFitMapRebin.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace NicaFemto {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *NicaFemto_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("NicaFemto", 0 /*version*/, "NicaFemtoConst.h", 57,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &NicaFemto_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *NicaFemto_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void delete_NicaFemtoCorrFunc(void *p);
   static void deleteArray_NicaFemtoCorrFunc(void *p);
   static void destruct_NicaFemtoCorrFunc(void *p);
   static Long64_t merge_NicaFemtoCorrFunc(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoCorrFunc*)
   {
      ::NicaFemtoCorrFunc *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoCorrFunc >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoCorrFunc", ::NicaFemtoCorrFunc::Class_Version(), "NicaFemtoCorrFunc.h", 30,
                  typeid(::NicaFemtoCorrFunc), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoCorrFunc::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoCorrFunc) );
      instance.SetDelete(&delete_NicaFemtoCorrFunc);
      instance.SetDeleteArray(&deleteArray_NicaFemtoCorrFunc);
      instance.SetDestructor(&destruct_NicaFemtoCorrFunc);
      instance.SetMerge(&merge_NicaFemtoCorrFunc);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoCorrFunc*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoCorrFunc*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaFemtoCorrFunc1D(void *p);
   static void deleteArray_NicaFemtoCorrFunc1D(void *p);
   static void destruct_NicaFemtoCorrFunc1D(void *p);
   static Long64_t merge_NicaFemtoCorrFunc1D(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoCorrFunc1D*)
   {
      ::NicaFemtoCorrFunc1D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoCorrFunc1D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoCorrFunc1D", ::NicaFemtoCorrFunc1D::Class_Version(), "NicaFemtoCorrFunc.h", 106,
                  typeid(::NicaFemtoCorrFunc1D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoCorrFunc1D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoCorrFunc1D) );
      instance.SetDelete(&delete_NicaFemtoCorrFunc1D);
      instance.SetDeleteArray(&deleteArray_NicaFemtoCorrFunc1D);
      instance.SetDestructor(&destruct_NicaFemtoCorrFunc1D);
      instance.SetMerge(&merge_NicaFemtoCorrFunc1D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoCorrFunc1D*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoCorrFunc1D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc1D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaFemtoCorrFunc2D(void *p);
   static void deleteArray_NicaFemtoCorrFunc2D(void *p);
   static void destruct_NicaFemtoCorrFunc2D(void *p);
   static Long64_t merge_NicaFemtoCorrFunc2D(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoCorrFunc2D*)
   {
      ::NicaFemtoCorrFunc2D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoCorrFunc2D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoCorrFunc2D", ::NicaFemtoCorrFunc2D::Class_Version(), "NicaFemtoCorrFunc.h", 133,
                  typeid(::NicaFemtoCorrFunc2D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoCorrFunc2D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoCorrFunc2D) );
      instance.SetDelete(&delete_NicaFemtoCorrFunc2D);
      instance.SetDeleteArray(&deleteArray_NicaFemtoCorrFunc2D);
      instance.SetDestructor(&destruct_NicaFemtoCorrFunc2D);
      instance.SetMerge(&merge_NicaFemtoCorrFunc2D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoCorrFunc2D*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoCorrFunc2D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc2D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaFemtoSourceModel(void *p);
   static void deleteArray_NicaFemtoSourceModel(void *p);
   static void destruct_NicaFemtoSourceModel(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoSourceModel*)
   {
      ::NicaFemtoSourceModel *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoSourceModel >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoSourceModel", ::NicaFemtoSourceModel::Class_Version(), "NicaFemtoSourceModel.h", 23,
                  typeid(::NicaFemtoSourceModel), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoSourceModel::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoSourceModel) );
      instance.SetDelete(&delete_NicaFemtoSourceModel);
      instance.SetDeleteArray(&deleteArray_NicaFemtoSourceModel);
      instance.SetDestructor(&destruct_NicaFemtoSourceModel);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoSourceModel*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoSourceModel*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoSourceModel*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaFemtoSourceModel1D(void *p);
   static void deleteArray_NicaFemtoSourceModel1D(void *p);
   static void destruct_NicaFemtoSourceModel1D(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoSourceModel1D*)
   {
      ::NicaFemtoSourceModel1D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoSourceModel1D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoSourceModel1D", ::NicaFemtoSourceModel1D::Class_Version(), "NicaFemtoSourceModel.h", 143,
                  typeid(::NicaFemtoSourceModel1D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoSourceModel1D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoSourceModel1D) );
      instance.SetDelete(&delete_NicaFemtoSourceModel1D);
      instance.SetDeleteArray(&deleteArray_NicaFemtoSourceModel1D);
      instance.SetDestructor(&destruct_NicaFemtoSourceModel1D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoSourceModel1D*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoSourceModel1D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoSourceModel1D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaFemtoSourceModel3D(void *p);
   static void deleteArray_NicaFemtoSourceModel3D(void *p);
   static void destruct_NicaFemtoSourceModel3D(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoSourceModel3D*)
   {
      ::NicaFemtoSourceModel3D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoSourceModel3D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoSourceModel3D", ::NicaFemtoSourceModel3D::Class_Version(), "NicaFemtoSourceModel.h", 166,
                  typeid(::NicaFemtoSourceModel3D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoSourceModel3D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoSourceModel3D) );
      instance.SetDelete(&delete_NicaFemtoSourceModel3D);
      instance.SetDeleteArray(&deleteArray_NicaFemtoSourceModel3D);
      instance.SetDestructor(&destruct_NicaFemtoSourceModel3D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoSourceModel3D*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoSourceModel3D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoSourceModel3D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaFemtoFreezoutGenerator(void *p);
   static void deleteArray_NicaFemtoFreezoutGenerator(void *p);
   static void destruct_NicaFemtoFreezoutGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoFreezoutGenerator*)
   {
      ::NicaFemtoFreezoutGenerator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoFreezoutGenerator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoFreezoutGenerator", ::NicaFemtoFreezoutGenerator::Class_Version(), "NicaFemtoFreezoutGenerator.h", 18,
                  typeid(::NicaFemtoFreezoutGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoFreezoutGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoFreezoutGenerator) );
      instance.SetDelete(&delete_NicaFemtoFreezoutGenerator);
      instance.SetDeleteArray(&deleteArray_NicaFemtoFreezoutGenerator);
      instance.SetDestructor(&destruct_NicaFemtoFreezoutGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoFreezoutGenerator*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoFreezoutGenerator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGenerator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoFreezoutGeneratorLCMS(void *p = nullptr);
   static void *newArray_NicaFemtoFreezoutGeneratorLCMS(Long_t size, void *p);
   static void delete_NicaFemtoFreezoutGeneratorLCMS(void *p);
   static void deleteArray_NicaFemtoFreezoutGeneratorLCMS(void *p);
   static void destruct_NicaFemtoFreezoutGeneratorLCMS(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoFreezoutGeneratorLCMS*)
   {
      ::NicaFemtoFreezoutGeneratorLCMS *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoFreezoutGeneratorLCMS >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoFreezoutGeneratorLCMS", ::NicaFemtoFreezoutGeneratorLCMS::Class_Version(), "NicaFemtoFreezoutGenerator.h", 74,
                  typeid(::NicaFemtoFreezoutGeneratorLCMS), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoFreezoutGeneratorLCMS::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoFreezoutGeneratorLCMS) );
      instance.SetNew(&new_NicaFemtoFreezoutGeneratorLCMS);
      instance.SetNewArray(&newArray_NicaFemtoFreezoutGeneratorLCMS);
      instance.SetDelete(&delete_NicaFemtoFreezoutGeneratorLCMS);
      instance.SetDeleteArray(&deleteArray_NicaFemtoFreezoutGeneratorLCMS);
      instance.SetDestructor(&destruct_NicaFemtoFreezoutGeneratorLCMS);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoFreezoutGeneratorLCMS*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoFreezoutGeneratorLCMS*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGeneratorLCMS*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoFreezoutGeneratorPRF(void *p = nullptr);
   static void *newArray_NicaFemtoFreezoutGeneratorPRF(Long_t size, void *p);
   static void delete_NicaFemtoFreezoutGeneratorPRF(void *p);
   static void deleteArray_NicaFemtoFreezoutGeneratorPRF(void *p);
   static void destruct_NicaFemtoFreezoutGeneratorPRF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoFreezoutGeneratorPRF*)
   {
      ::NicaFemtoFreezoutGeneratorPRF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoFreezoutGeneratorPRF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoFreezoutGeneratorPRF", ::NicaFemtoFreezoutGeneratorPRF::Class_Version(), "NicaFemtoFreezoutGenerator.h", 95,
                  typeid(::NicaFemtoFreezoutGeneratorPRF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoFreezoutGeneratorPRF::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoFreezoutGeneratorPRF) );
      instance.SetNew(&new_NicaFemtoFreezoutGeneratorPRF);
      instance.SetNewArray(&newArray_NicaFemtoFreezoutGeneratorPRF);
      instance.SetDelete(&delete_NicaFemtoFreezoutGeneratorPRF);
      instance.SetDeleteArray(&deleteArray_NicaFemtoFreezoutGeneratorPRF);
      instance.SetDestructor(&destruct_NicaFemtoFreezoutGeneratorPRF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoFreezoutGeneratorPRF*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoFreezoutGeneratorPRF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGeneratorPRF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoWeightGenerator(void *p = nullptr);
   static void *newArray_NicaFemtoWeightGenerator(Long_t size, void *p);
   static void delete_NicaFemtoWeightGenerator(void *p);
   static void deleteArray_NicaFemtoWeightGenerator(void *p);
   static void destruct_NicaFemtoWeightGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoWeightGenerator*)
   {
      ::NicaFemtoWeightGenerator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoWeightGenerator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoWeightGenerator", ::NicaFemtoWeightGenerator::Class_Version(), "NicaFemtoWeightGenerator.h", 23,
                  typeid(::NicaFemtoWeightGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoWeightGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoWeightGenerator) );
      instance.SetNew(&new_NicaFemtoWeightGenerator);
      instance.SetNewArray(&newArray_NicaFemtoWeightGenerator);
      instance.SetDelete(&delete_NicaFemtoWeightGenerator);
      instance.SetDeleteArray(&deleteArray_NicaFemtoWeightGenerator);
      instance.SetDestructor(&destruct_NicaFemtoWeightGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoWeightGenerator*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoWeightGenerator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoWeightGenerator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoBasicAna(void *p = nullptr);
   static void *newArray_NicaFemtoBasicAna(Long_t size, void *p);
   static void delete_NicaFemtoBasicAna(void *p);
   static void deleteArray_NicaFemtoBasicAna(void *p);
   static void destruct_NicaFemtoBasicAna(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoBasicAna*)
   {
      ::NicaFemtoBasicAna *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoBasicAna >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoBasicAna", ::NicaFemtoBasicAna::Class_Version(), "NicaFemtoBasicAna.h", 28,
                  typeid(::NicaFemtoBasicAna), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoBasicAna::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoBasicAna) );
      instance.SetNew(&new_NicaFemtoBasicAna);
      instance.SetNewArray(&newArray_NicaFemtoBasicAna);
      instance.SetDelete(&delete_NicaFemtoBasicAna);
      instance.SetDeleteArray(&deleteArray_NicaFemtoBasicAna);
      instance.SetDestructor(&destruct_NicaFemtoBasicAna);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoBasicAna*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoBasicAna*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoBasicAna*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaFemtoFastCut(void *p);
   static void deleteArray_NicaFemtoFastCut(void *p);
   static void destruct_NicaFemtoFastCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoFastCut*)
   {
      ::NicaFemtoFastCut *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoFastCut >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoFastCut", ::NicaFemtoFastCut::Class_Version(), "NicaFemtoFastCut.h", 21,
                  typeid(::NicaFemtoFastCut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoFastCut::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoFastCut) );
      instance.SetDelete(&delete_NicaFemtoFastCut);
      instance.SetDeleteArray(&deleteArray_NicaFemtoFastCut);
      instance.SetDestructor(&destruct_NicaFemtoFastCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoFastCut*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoFastCut*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoFastCut*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoFastCutVirtual(void *p = nullptr);
   static void *newArray_NicaFemtoFastCutVirtual(Long_t size, void *p);
   static void delete_NicaFemtoFastCutVirtual(void *p);
   static void deleteArray_NicaFemtoFastCutVirtual(void *p);
   static void destruct_NicaFemtoFastCutVirtual(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoFastCutVirtual*)
   {
      ::NicaFemtoFastCutVirtual *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoFastCutVirtual >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoFastCutVirtual", ::NicaFemtoFastCutVirtual::Class_Version(), "NicaFemtoFastCut.h", 138,
                  typeid(::NicaFemtoFastCutVirtual), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoFastCutVirtual::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoFastCutVirtual) );
      instance.SetNew(&new_NicaFemtoFastCutVirtual);
      instance.SetNewArray(&newArray_NicaFemtoFastCutVirtual);
      instance.SetDelete(&delete_NicaFemtoFastCutVirtual);
      instance.SetDeleteArray(&deleteArray_NicaFemtoFastCutVirtual);
      instance.SetDestructor(&destruct_NicaFemtoFastCutVirtual);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoFastCutVirtual*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoFastCutVirtual*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoFastCutVirtual*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoFastCutKt(void *p = nullptr);
   static void *newArray_NicaFemtoFastCutKt(Long_t size, void *p);
   static void delete_NicaFemtoFastCutKt(void *p);
   static void deleteArray_NicaFemtoFastCutKt(void *p);
   static void destruct_NicaFemtoFastCutKt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoFastCutKt*)
   {
      ::NicaFemtoFastCutKt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoFastCutKt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoFastCutKt", ::NicaFemtoFastCutKt::Class_Version(), "NicaFemtoFastCut.h", 158,
                  typeid(::NicaFemtoFastCutKt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoFastCutKt::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoFastCutKt) );
      instance.SetNew(&new_NicaFemtoFastCutKt);
      instance.SetNewArray(&newArray_NicaFemtoFastCutKt);
      instance.SetDelete(&delete_NicaFemtoFastCutKt);
      instance.SetDeleteArray(&deleteArray_NicaFemtoFastCutKt);
      instance.SetDestructor(&destruct_NicaFemtoFastCutKt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoFastCutKt*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoFastCutKt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoFastCutKt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoFastCutPtSum(void *p = nullptr);
   static void *newArray_NicaFemtoFastCutPtSum(Long_t size, void *p);
   static void delete_NicaFemtoFastCutPtSum(void *p);
   static void deleteArray_NicaFemtoFastCutPtSum(void *p);
   static void destruct_NicaFemtoFastCutPtSum(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoFastCutPtSum*)
   {
      ::NicaFemtoFastCutPtSum *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoFastCutPtSum >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoFastCutPtSum", ::NicaFemtoFastCutPtSum::Class_Version(), "NicaFemtoFastCut.h", 178,
                  typeid(::NicaFemtoFastCutPtSum), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoFastCutPtSum::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoFastCutPtSum) );
      instance.SetNew(&new_NicaFemtoFastCutPtSum);
      instance.SetNewArray(&newArray_NicaFemtoFastCutPtSum);
      instance.SetDelete(&delete_NicaFemtoFastCutPtSum);
      instance.SetDeleteArray(&deleteArray_NicaFemtoFastCutPtSum);
      instance.SetDestructor(&destruct_NicaFemtoFastCutPtSum);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoFastCutPtSum*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoFastCutPtSum*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoFastCutPtSum*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoFastCutPhi(void *p = nullptr);
   static void *newArray_NicaFemtoFastCutPhi(Long_t size, void *p);
   static void delete_NicaFemtoFastCutPhi(void *p);
   static void deleteArray_NicaFemtoFastCutPhi(void *p);
   static void destruct_NicaFemtoFastCutPhi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoFastCutPhi*)
   {
      ::NicaFemtoFastCutPhi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoFastCutPhi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoFastCutPhi", ::NicaFemtoFastCutPhi::Class_Version(), "NicaFemtoFastCut.h", 198,
                  typeid(::NicaFemtoFastCutPhi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoFastCutPhi::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoFastCutPhi) );
      instance.SetNew(&new_NicaFemtoFastCutPhi);
      instance.SetNewArray(&newArray_NicaFemtoFastCutPhi);
      instance.SetDelete(&delete_NicaFemtoFastCutPhi);
      instance.SetDeleteArray(&deleteArray_NicaFemtoFastCutPhi);
      instance.SetDestructor(&destruct_NicaFemtoFastCutPhi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoFastCutPhi*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoFastCutPhi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoFastCutPhi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoFastCutEta(void *p = nullptr);
   static void *newArray_NicaFemtoFastCutEta(Long_t size, void *p);
   static void delete_NicaFemtoFastCutEta(void *p);
   static void deleteArray_NicaFemtoFastCutEta(void *p);
   static void destruct_NicaFemtoFastCutEta(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoFastCutEta*)
   {
      ::NicaFemtoFastCutEta *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoFastCutEta >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoFastCutEta", ::NicaFemtoFastCutEta::Class_Version(), "NicaFemtoFastCut.h", 232,
                  typeid(::NicaFemtoFastCutEta), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoFastCutEta::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoFastCutEta) );
      instance.SetNew(&new_NicaFemtoFastCutEta);
      instance.SetNewArray(&newArray_NicaFemtoFastCutEta);
      instance.SetDelete(&delete_NicaFemtoFastCutEta);
      instance.SetDeleteArray(&deleteArray_NicaFemtoFastCutEta);
      instance.SetDestructor(&destruct_NicaFemtoFastCutEta);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoFastCutEta*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoFastCutEta*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoFastCutEta*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoFreezoutsAna(void *p = nullptr);
   static void *newArray_NicaFemtoFreezoutsAna(Long_t size, void *p);
   static void delete_NicaFemtoFreezoutsAna(void *p);
   static void deleteArray_NicaFemtoFreezoutsAna(void *p);
   static void destruct_NicaFemtoFreezoutsAna(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoFreezoutsAna*)
   {
      ::NicaFemtoFreezoutsAna *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoFreezoutsAna >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoFreezoutsAna", ::NicaFemtoFreezoutsAna::Class_Version(), "NicaFemtoFreezoutsAna.h", 30,
                  typeid(::NicaFemtoFreezoutsAna), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoFreezoutsAna::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoFreezoutsAna) );
      instance.SetNew(&new_NicaFemtoFreezoutsAna);
      instance.SetNewArray(&newArray_NicaFemtoFreezoutsAna);
      instance.SetDelete(&delete_NicaFemtoFreezoutsAna);
      instance.SetDeleteArray(&deleteArray_NicaFemtoFreezoutsAna);
      instance.SetDestructor(&destruct_NicaFemtoFreezoutsAna);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoFreezoutsAna*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoFreezoutsAna*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoFreezoutsAna*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaFemtoPair(void *p);
   static void deleteArray_NicaFemtoPair(void *p);
   static void destruct_NicaFemtoPair(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoPair*)
   {
      ::NicaFemtoPair *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoPair >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoPair", ::NicaFemtoPair::Class_Version(), "NicaFemtoPair.h", 26,
                  typeid(::NicaFemtoPair), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoPair::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoPair) );
      instance.SetDelete(&delete_NicaFemtoPair);
      instance.SetDeleteArray(&deleteArray_NicaFemtoPair);
      instance.SetDestructor(&destruct_NicaFemtoPair);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoPair*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoPair*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoPair*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoPicoPair(void *p = nullptr);
   static void *newArray_NicaFemtoPicoPair(Long_t size, void *p);
   static void delete_NicaFemtoPicoPair(void *p);
   static void deleteArray_NicaFemtoPicoPair(void *p);
   static void destruct_NicaFemtoPicoPair(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoPicoPair*)
   {
      ::NicaFemtoPicoPair *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoPicoPair >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoPicoPair", ::NicaFemtoPicoPair::Class_Version(), "NicaFemtoMiniPair.h", 16,
                  typeid(::NicaFemtoPicoPair), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoPicoPair::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoPicoPair) );
      instance.SetNew(&new_NicaFemtoPicoPair);
      instance.SetNewArray(&newArray_NicaFemtoPicoPair);
      instance.SetDelete(&delete_NicaFemtoPicoPair);
      instance.SetDeleteArray(&deleteArray_NicaFemtoPicoPair);
      instance.SetDestructor(&destruct_NicaFemtoPicoPair);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoPicoPair*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoPicoPair*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoPicoPair*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoMicroPair(void *p = nullptr);
   static void *newArray_NicaFemtoMicroPair(Long_t size, void *p);
   static void delete_NicaFemtoMicroPair(void *p);
   static void deleteArray_NicaFemtoMicroPair(void *p);
   static void destruct_NicaFemtoMicroPair(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoMicroPair*)
   {
      ::NicaFemtoMicroPair *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoMicroPair >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoMicroPair", ::NicaFemtoMicroPair::Class_Version(), "NicaFemtoMiniPair.h", 95,
                  typeid(::NicaFemtoMicroPair), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoMicroPair::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoMicroPair) );
      instance.SetNew(&new_NicaFemtoMicroPair);
      instance.SetNewArray(&newArray_NicaFemtoMicroPair);
      instance.SetDelete(&delete_NicaFemtoMicroPair);
      instance.SetDeleteArray(&deleteArray_NicaFemtoMicroPair);
      instance.SetDestructor(&destruct_NicaFemtoMicroPair);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoMicroPair*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoMicroPair*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoMicroPair*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoMiniPair(void *p = nullptr);
   static void *newArray_NicaFemtoMiniPair(Long_t size, void *p);
   static void delete_NicaFemtoMiniPair(void *p);
   static void deleteArray_NicaFemtoMiniPair(void *p);
   static void destruct_NicaFemtoMiniPair(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoMiniPair*)
   {
      ::NicaFemtoMiniPair *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoMiniPair >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoMiniPair", ::NicaFemtoMiniPair::Class_Version(), "NicaFemtoMiniPair.h", 223,
                  typeid(::NicaFemtoMiniPair), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoMiniPair::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoMiniPair) );
      instance.SetNew(&new_NicaFemtoMiniPair);
      instance.SetNewArray(&newArray_NicaFemtoMiniPair);
      instance.SetDelete(&delete_NicaFemtoMiniPair);
      instance.SetDeleteArray(&deleteArray_NicaFemtoMiniPair);
      instance.SetDestructor(&destruct_NicaFemtoMiniPair);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoMiniPair*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoMiniPair*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoMiniPair*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoDumpPairAna(void *p = nullptr);
   static void *newArray_NicaFemtoDumpPairAna(Long_t size, void *p);
   static void delete_NicaFemtoDumpPairAna(void *p);
   static void deleteArray_NicaFemtoDumpPairAna(void *p);
   static void destruct_NicaFemtoDumpPairAna(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoDumpPairAna*)
   {
      ::NicaFemtoDumpPairAna *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoDumpPairAna >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoDumpPairAna", ::NicaFemtoDumpPairAna::Class_Version(), "NicaFemtoDumpPairAna.h", 17,
                  typeid(::NicaFemtoDumpPairAna), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoDumpPairAna::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoDumpPairAna) );
      instance.SetNew(&new_NicaFemtoDumpPairAna);
      instance.SetNewArray(&newArray_NicaFemtoDumpPairAna);
      instance.SetDelete(&delete_NicaFemtoDumpPairAna);
      instance.SetDeleteArray(&deleteArray_NicaFemtoDumpPairAna);
      instance.SetDestructor(&destruct_NicaFemtoDumpPairAna);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoDumpPairAna*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoDumpPairAna*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoDumpPairAna*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoEventBinsAna(void *p = nullptr);
   static void *newArray_NicaFemtoEventBinsAna(Long_t size, void *p);
   static void delete_NicaFemtoEventBinsAna(void *p);
   static void deleteArray_NicaFemtoEventBinsAna(void *p);
   static void destruct_NicaFemtoEventBinsAna(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoEventBinsAna*)
   {
      ::NicaFemtoEventBinsAna *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoEventBinsAna >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoEventBinsAna", ::NicaFemtoEventBinsAna::Class_Version(), "NicaFemtoEventBinsAna.h", 21,
                  typeid(::NicaFemtoEventBinsAna), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoEventBinsAna::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoEventBinsAna) );
      instance.SetNew(&new_NicaFemtoEventBinsAna);
      instance.SetNewArray(&newArray_NicaFemtoEventBinsAna);
      instance.SetDelete(&delete_NicaFemtoEventBinsAna);
      instance.SetDeleteArray(&deleteArray_NicaFemtoEventBinsAna);
      instance.SetDestructor(&destruct_NicaFemtoEventBinsAna);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoEventBinsAna*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoEventBinsAna*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoEventBinsAna*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoDPhiDEta(void *p = nullptr);
   static void *newArray_NicaFemtoDPhiDEta(Long_t size, void *p);
   static void delete_NicaFemtoDPhiDEta(void *p);
   static void deleteArray_NicaFemtoDPhiDEta(void *p);
   static void destruct_NicaFemtoDPhiDEta(void *p);
   static Long64_t merge_NicaFemtoDPhiDEta(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoDPhiDEta*)
   {
      ::NicaFemtoDPhiDEta *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoDPhiDEta >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoDPhiDEta", ::NicaFemtoDPhiDEta::Class_Version(), "NicaFemtoDPhiDEta.h", 16,
                  typeid(::NicaFemtoDPhiDEta), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoDPhiDEta::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoDPhiDEta) );
      instance.SetNew(&new_NicaFemtoDPhiDEta);
      instance.SetNewArray(&newArray_NicaFemtoDPhiDEta);
      instance.SetDelete(&delete_NicaFemtoDPhiDEta);
      instance.SetDeleteArray(&deleteArray_NicaFemtoDPhiDEta);
      instance.SetDestructor(&destruct_NicaFemtoDPhiDEta);
      instance.SetMerge(&merge_NicaFemtoDPhiDEta);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoDPhiDEta*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoDPhiDEta*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoDPhiDEta*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemto1DCF(void *p = nullptr);
   static void *newArray_NicaFemto1DCF(Long_t size, void *p);
   static void delete_NicaFemto1DCF(void *p);
   static void deleteArray_NicaFemto1DCF(void *p);
   static void destruct_NicaFemto1DCF(void *p);
   static Long64_t merge_NicaFemto1DCF(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemto1DCF*)
   {
      ::NicaFemto1DCF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemto1DCF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemto1DCF", ::NicaFemto1DCF::Class_Version(), "NicaFemto1DCF.h", 24,
                  typeid(::NicaFemto1DCF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemto1DCF::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemto1DCF) );
      instance.SetNew(&new_NicaFemto1DCF);
      instance.SetNewArray(&newArray_NicaFemto1DCF);
      instance.SetDelete(&delete_NicaFemto1DCF);
      instance.SetDeleteArray(&deleteArray_NicaFemto1DCF);
      instance.SetDestructor(&destruct_NicaFemto1DCF);
      instance.SetMerge(&merge_NicaFemto1DCF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemto1DCF*)
   {
      return GenerateInitInstanceLocal((::NicaFemto1DCF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemto1DCF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemto3DCF(void *p = nullptr);
   static void *newArray_NicaFemto3DCF(Long_t size, void *p);
   static void delete_NicaFemto3DCF(void *p);
   static void deleteArray_NicaFemto3DCF(void *p);
   static void destruct_NicaFemto3DCF(void *p);
   static Long64_t merge_NicaFemto3DCF(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemto3DCF*)
   {
      ::NicaFemto3DCF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemto3DCF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemto3DCF", ::NicaFemto3DCF::Class_Version(), "NicaFemto3DCF.h", 23,
                  typeid(::NicaFemto3DCF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemto3DCF::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemto3DCF) );
      instance.SetNew(&new_NicaFemto3DCF);
      instance.SetNewArray(&newArray_NicaFemto3DCF);
      instance.SetDelete(&delete_NicaFemto3DCF);
      instance.SetDeleteArray(&deleteArray_NicaFemto3DCF);
      instance.SetDestructor(&destruct_NicaFemto3DCF);
      instance.SetMerge(&merge_NicaFemto3DCF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemto3DCF*)
   {
      return GenerateInitInstanceLocal((::NicaFemto3DCF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemto3DCF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemto3DCFQinv(void *p = nullptr);
   static void *newArray_NicaFemto3DCFQinv(Long_t size, void *p);
   static void delete_NicaFemto3DCFQinv(void *p);
   static void deleteArray_NicaFemto3DCFQinv(void *p);
   static void destruct_NicaFemto3DCFQinv(void *p);
   static Long64_t merge_NicaFemto3DCFQinv(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemto3DCFQinv*)
   {
      ::NicaFemto3DCFQinv *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemto3DCFQinv >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemto3DCFQinv", ::NicaFemto3DCFQinv::Class_Version(), "NicaFemto3DCFQinv.h", 16,
                  typeid(::NicaFemto3DCFQinv), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemto3DCFQinv::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemto3DCFQinv) );
      instance.SetNew(&new_NicaFemto3DCFQinv);
      instance.SetNewArray(&newArray_NicaFemto3DCFQinv);
      instance.SetDelete(&delete_NicaFemto3DCFQinv);
      instance.SetDeleteArray(&deleteArray_NicaFemto3DCFQinv);
      instance.SetDestructor(&destruct_NicaFemto3DCFQinv);
      instance.SetMerge(&merge_NicaFemto3DCFQinv);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemto3DCFQinv*)
   {
      return GenerateInitInstanceLocal((::NicaFemto3DCFQinv*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemto3DCFQinv*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoPairPRF(void *p = nullptr);
   static void *newArray_NicaFemtoPairPRF(Long_t size, void *p);
   static void delete_NicaFemtoPairPRF(void *p);
   static void deleteArray_NicaFemtoPairPRF(void *p);
   static void destruct_NicaFemtoPairPRF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoPairPRF*)
   {
      ::NicaFemtoPairPRF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoPairPRF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoPairPRF", ::NicaFemtoPairPRF::Class_Version(), "NicaFemtoPairKinematics.h", 22,
                  typeid(::NicaFemtoPairPRF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoPairPRF::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoPairPRF) );
      instance.SetNew(&new_NicaFemtoPairPRF);
      instance.SetNewArray(&newArray_NicaFemtoPairPRF);
      instance.SetDelete(&delete_NicaFemtoPairPRF);
      instance.SetDeleteArray(&deleteArray_NicaFemtoPairPRF);
      instance.SetDestructor(&destruct_NicaFemtoPairPRF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoPairPRF*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoPairPRF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoPairPRF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaFemtoPair_SH(void *p);
   static void deleteArray_NicaFemtoPair_SH(void *p);
   static void destruct_NicaFemtoPair_SH(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoPair_SH*)
   {
      ::NicaFemtoPair_SH *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoPair_SH >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoPair_SH", ::NicaFemtoPair_SH::Class_Version(), "NicaFemtoPairKinematics.h", 37,
                  typeid(::NicaFemtoPair_SH), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoPair_SH::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoPair_SH) );
      instance.SetDelete(&delete_NicaFemtoPair_SH);
      instance.SetDeleteArray(&deleteArray_NicaFemtoPair_SH);
      instance.SetDestructor(&destruct_NicaFemtoPair_SH);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoPair_SH*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoPair_SH*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoPair_SH*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoPairLCMS(void *p = nullptr);
   static void *newArray_NicaFemtoPairLCMS(Long_t size, void *p);
   static void delete_NicaFemtoPairLCMS(void *p);
   static void deleteArray_NicaFemtoPairLCMS(void *p);
   static void destruct_NicaFemtoPairLCMS(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoPairLCMS*)
   {
      ::NicaFemtoPairLCMS *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoPairLCMS >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoPairLCMS", ::NicaFemtoPairLCMS::Class_Version(), "NicaFemtoPairKinematics.h", 53,
                  typeid(::NicaFemtoPairLCMS), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoPairLCMS::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoPairLCMS) );
      instance.SetNew(&new_NicaFemtoPairLCMS);
      instance.SetNewArray(&newArray_NicaFemtoPairLCMS);
      instance.SetDelete(&delete_NicaFemtoPairLCMS);
      instance.SetDeleteArray(&deleteArray_NicaFemtoPairLCMS);
      instance.SetDestructor(&destruct_NicaFemtoPairLCMS);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoPairLCMS*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoPairLCMS*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoPairLCMS*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoPairPRF_SH(void *p = nullptr);
   static void *newArray_NicaFemtoPairPRF_SH(Long_t size, void *p);
   static void delete_NicaFemtoPairPRF_SH(void *p);
   static void deleteArray_NicaFemtoPairPRF_SH(void *p);
   static void destruct_NicaFemtoPairPRF_SH(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoPairPRF_SH*)
   {
      ::NicaFemtoPairPRF_SH *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoPairPRF_SH >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoPairPRF_SH", ::NicaFemtoPairPRF_SH::Class_Version(), "NicaFemtoPairKinematics.h", 69,
                  typeid(::NicaFemtoPairPRF_SH), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoPairPRF_SH::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoPairPRF_SH) );
      instance.SetNew(&new_NicaFemtoPairPRF_SH);
      instance.SetNewArray(&newArray_NicaFemtoPairPRF_SH);
      instance.SetDelete(&delete_NicaFemtoPairPRF_SH);
      instance.SetDeleteArray(&deleteArray_NicaFemtoPairPRF_SH);
      instance.SetDestructor(&destruct_NicaFemtoPairPRF_SH);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoPairPRF_SH*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoPairPRF_SH*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoPairPRF_SH*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoPairLCMS_SH(void *p = nullptr);
   static void *newArray_NicaFemtoPairLCMS_SH(Long_t size, void *p);
   static void delete_NicaFemtoPairLCMS_SH(void *p);
   static void deleteArray_NicaFemtoPairLCMS_SH(void *p);
   static void destruct_NicaFemtoPairLCMS_SH(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoPairLCMS_SH*)
   {
      ::NicaFemtoPairLCMS_SH *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoPairLCMS_SH >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoPairLCMS_SH", ::NicaFemtoPairLCMS_SH::Class_Version(), "NicaFemtoPairKinematics.h", 85,
                  typeid(::NicaFemtoPairLCMS_SH), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoPairLCMS_SH::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoPairLCMS_SH) );
      instance.SetNew(&new_NicaFemtoPairLCMS_SH);
      instance.SetNewArray(&newArray_NicaFemtoPairLCMS_SH);
      instance.SetDelete(&delete_NicaFemtoPairLCMS_SH);
      instance.SetDeleteArray(&deleteArray_NicaFemtoPairLCMS_SH);
      instance.SetDestructor(&destruct_NicaFemtoPairLCMS_SH);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoPairLCMS_SH*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoPairLCMS_SH*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoPairLCMS_SH*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoPairDPhiDEta(void *p = nullptr);
   static void *newArray_NicaFemtoPairDPhiDEta(Long_t size, void *p);
   static void delete_NicaFemtoPairDPhiDEta(void *p);
   static void deleteArray_NicaFemtoPairDPhiDEta(void *p);
   static void destruct_NicaFemtoPairDPhiDEta(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoPairDPhiDEta*)
   {
      ::NicaFemtoPairDPhiDEta *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoPairDPhiDEta >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoPairDPhiDEta", ::NicaFemtoPairDPhiDEta::Class_Version(), "NicaFemtoPairKinematics.h", 100,
                  typeid(::NicaFemtoPairDPhiDEta), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoPairDPhiDEta::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoPairDPhiDEta) );
      instance.SetNew(&new_NicaFemtoPairDPhiDEta);
      instance.SetNewArray(&newArray_NicaFemtoPairDPhiDEta);
      instance.SetDelete(&delete_NicaFemtoPairDPhiDEta);
      instance.SetDeleteArray(&deleteArray_NicaFemtoPairDPhiDEta);
      instance.SetDestructor(&destruct_NicaFemtoPairDPhiDEta);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoPairDPhiDEta*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoPairDPhiDEta*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoPairDPhiDEta*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoPairPRFL(void *p = nullptr);
   static void *newArray_NicaFemtoPairPRFL(Long_t size, void *p);
   static void delete_NicaFemtoPairPRFL(void *p);
   static void deleteArray_NicaFemtoPairPRFL(void *p);
   static void destruct_NicaFemtoPairPRFL(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoPairPRFL*)
   {
      ::NicaFemtoPairPRFL *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoPairPRFL >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoPairPRFL", ::NicaFemtoPairPRFL::Class_Version(), "NicaFemtoPairKinematics.h", 124,
                  typeid(::NicaFemtoPairPRFL), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoPairPRFL::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoPairPRFL) );
      instance.SetNew(&new_NicaFemtoPairPRFL);
      instance.SetNewArray(&newArray_NicaFemtoPairPRFL);
      instance.SetDelete(&delete_NicaFemtoPairPRFL);
      instance.SetDeleteArray(&deleteArray_NicaFemtoPairPRFL);
      instance.SetDestructor(&destruct_NicaFemtoPairPRFL);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoPairPRFL*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoPairPRFL*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoPairPRFL*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoSourceModelExp1D(void *p = nullptr);
   static void *newArray_NicaFemtoSourceModelExp1D(Long_t size, void *p);
   static void delete_NicaFemtoSourceModelExp1D(void *p);
   static void deleteArray_NicaFemtoSourceModelExp1D(void *p);
   static void destruct_NicaFemtoSourceModelExp1D(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoSourceModelExp1D*)
   {
      ::NicaFemtoSourceModelExp1D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoSourceModelExp1D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoSourceModelExp1D", ::NicaFemtoSourceModelExp1D::Class_Version(), "NicaFemtoSourceModelExp.h", 19,
                  typeid(::NicaFemtoSourceModelExp1D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoSourceModelExp1D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoSourceModelExp1D) );
      instance.SetNew(&new_NicaFemtoSourceModelExp1D);
      instance.SetNewArray(&newArray_NicaFemtoSourceModelExp1D);
      instance.SetDelete(&delete_NicaFemtoSourceModelExp1D);
      instance.SetDeleteArray(&deleteArray_NicaFemtoSourceModelExp1D);
      instance.SetDestructor(&destruct_NicaFemtoSourceModelExp1D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoSourceModelExp1D*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoSourceModelExp1D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoSourceModelExp1D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoSourceModelGauss1D(void *p = nullptr);
   static void *newArray_NicaFemtoSourceModelGauss1D(Long_t size, void *p);
   static void delete_NicaFemtoSourceModelGauss1D(void *p);
   static void deleteArray_NicaFemtoSourceModelGauss1D(void *p);
   static void destruct_NicaFemtoSourceModelGauss1D(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoSourceModelGauss1D*)
   {
      ::NicaFemtoSourceModelGauss1D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoSourceModelGauss1D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoSourceModelGauss1D", ::NicaFemtoSourceModelGauss1D::Class_Version(), "NicaFemtoSourceModelGauss.h", 18,
                  typeid(::NicaFemtoSourceModelGauss1D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoSourceModelGauss1D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoSourceModelGauss1D) );
      instance.SetNew(&new_NicaFemtoSourceModelGauss1D);
      instance.SetNewArray(&newArray_NicaFemtoSourceModelGauss1D);
      instance.SetDelete(&delete_NicaFemtoSourceModelGauss1D);
      instance.SetDeleteArray(&deleteArray_NicaFemtoSourceModelGauss1D);
      instance.SetDestructor(&destruct_NicaFemtoSourceModelGauss1D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoSourceModelGauss1D*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoSourceModelGauss1D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGauss1D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoSourceModelGauss3D(void *p = nullptr);
   static void *newArray_NicaFemtoSourceModelGauss3D(Long_t size, void *p);
   static void delete_NicaFemtoSourceModelGauss3D(void *p);
   static void deleteArray_NicaFemtoSourceModelGauss3D(void *p);
   static void destruct_NicaFemtoSourceModelGauss3D(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoSourceModelGauss3D*)
   {
      ::NicaFemtoSourceModelGauss3D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoSourceModelGauss3D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoSourceModelGauss3D", ::NicaFemtoSourceModelGauss3D::Class_Version(), "NicaFemtoSourceModelGauss.h", 35,
                  typeid(::NicaFemtoSourceModelGauss3D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoSourceModelGauss3D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoSourceModelGauss3D) );
      instance.SetNew(&new_NicaFemtoSourceModelGauss3D);
      instance.SetNewArray(&newArray_NicaFemtoSourceModelGauss3D);
      instance.SetDelete(&delete_NicaFemtoSourceModelGauss3D);
      instance.SetDeleteArray(&deleteArray_NicaFemtoSourceModelGauss3D);
      instance.SetDestructor(&destruct_NicaFemtoSourceModelGauss3D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoSourceModelGauss3D*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoSourceModelGauss3D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGauss3D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoSourceModelGausMt(void *p = nullptr);
   static void *newArray_NicaFemtoSourceModelGausMt(Long_t size, void *p);
   static void delete_NicaFemtoSourceModelGausMt(void *p);
   static void deleteArray_NicaFemtoSourceModelGausMt(void *p);
   static void destruct_NicaFemtoSourceModelGausMt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoSourceModelGausMt*)
   {
      ::NicaFemtoSourceModelGausMt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoSourceModelGausMt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoSourceModelGausMt", ::NicaFemtoSourceModelGausMt::Class_Version(), "NicaFemtoSourceModelGausMt.h", 15,
                  typeid(::NicaFemtoSourceModelGausMt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoSourceModelGausMt::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoSourceModelGausMt) );
      instance.SetNew(&new_NicaFemtoSourceModelGausMt);
      instance.SetNewArray(&newArray_NicaFemtoSourceModelGausMt);
      instance.SetDelete(&delete_NicaFemtoSourceModelGausMt);
      instance.SetDeleteArray(&deleteArray_NicaFemtoSourceModelGausMt);
      instance.SetDestructor(&destruct_NicaFemtoSourceModelGausMt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoSourceModelGausMt*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoSourceModelGausMt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGausMt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaCorrFit(void *p);
   static void deleteArray_NicaCorrFit(void *p);
   static void destruct_NicaCorrFit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit*)
   {
      ::NicaCorrFit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit", ::NicaCorrFit::Class_Version(), "NicaCorrFit.h", 30,
                  typeid(::NicaCorrFit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit) );
      instance.SetDelete(&delete_NicaCorrFit);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit);
      instance.SetDestructor(&destruct_NicaCorrFit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaCorrFitFunc(void *p);
   static void deleteArray_NicaCorrFitFunc(void *p);
   static void destruct_NicaCorrFitFunc(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitFunc*)
   {
      ::NicaCorrFitFunc *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitFunc >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitFunc", ::NicaCorrFitFunc::Class_Version(), "NicaCorrFitFunc.h", 30,
                  typeid(::NicaCorrFitFunc), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitFunc::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitFunc) );
      instance.SetDelete(&delete_NicaCorrFitFunc);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitFunc);
      instance.SetDestructor(&destruct_NicaCorrFitFunc);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitFunc*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitFunc*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitFunc*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit1DCF(void *p = nullptr);
   static void *newArray_NicaCorrFit1DCF(Long_t size, void *p);
   static void delete_NicaCorrFit1DCF(void *p);
   static void deleteArray_NicaCorrFit1DCF(void *p);
   static void destruct_NicaCorrFit1DCF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit1DCF*)
   {
      ::NicaCorrFit1DCF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit1DCF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit1DCF", ::NicaCorrFit1DCF::Class_Version(), "NicaCorrFit1DCF.h", 24,
                  typeid(::NicaCorrFit1DCF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit1DCF::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit1DCF) );
      instance.SetNew(&new_NicaCorrFit1DCF);
      instance.SetNewArray(&newArray_NicaCorrFit1DCF);
      instance.SetDelete(&delete_NicaCorrFit1DCF);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit1DCF);
      instance.SetDestructor(&destruct_NicaCorrFit1DCF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit1DCF*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit1DCF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit1DCF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitMath1DCF(void *p = nullptr);
   static void *newArray_NicaCorrFitMath1DCF(Long_t size, void *p);
   static void delete_NicaCorrFitMath1DCF(void *p);
   static void deleteArray_NicaCorrFitMath1DCF(void *p);
   static void destruct_NicaCorrFitMath1DCF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitMath1DCF*)
   {
      ::NicaCorrFitMath1DCF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitMath1DCF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitMath1DCF", ::NicaCorrFitMath1DCF::Class_Version(), "NicaCorrFitMath1DCF.h", 14,
                  typeid(::NicaCorrFitMath1DCF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitMath1DCF::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitMath1DCF) );
      instance.SetNew(&new_NicaCorrFitMath1DCF);
      instance.SetNewArray(&newArray_NicaCorrFitMath1DCF);
      instance.SetDelete(&delete_NicaCorrFitMath1DCF);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitMath1DCF);
      instance.SetDestructor(&destruct_NicaCorrFitMath1DCF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitMath1DCF*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitMath1DCF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitMath1DCF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit1DCF_Sum(void *p = nullptr);
   static void *newArray_NicaCorrFit1DCF_Sum(Long_t size, void *p);
   static void delete_NicaCorrFit1DCF_Sum(void *p);
   static void deleteArray_NicaCorrFit1DCF_Sum(void *p);
   static void destruct_NicaCorrFit1DCF_Sum(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit1DCF_Sum*)
   {
      ::NicaCorrFit1DCF_Sum *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit1DCF_Sum >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit1DCF_Sum", ::NicaCorrFit1DCF_Sum::Class_Version(), "NicaCorrFitMath1DCF.h", 58,
                  typeid(::NicaCorrFit1DCF_Sum), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit1DCF_Sum::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit1DCF_Sum) );
      instance.SetNew(&new_NicaCorrFit1DCF_Sum);
      instance.SetNewArray(&newArray_NicaCorrFit1DCF_Sum);
      instance.SetDelete(&delete_NicaCorrFit1DCF_Sum);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit1DCF_Sum);
      instance.SetDestructor(&destruct_NicaCorrFit1DCF_Sum);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit1DCF_Sum*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit1DCF_Sum*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Sum*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit1DCF_SumRsame(void *p = nullptr);
   static void *newArray_NicaCorrFit1DCF_SumRsame(Long_t size, void *p);
   static void delete_NicaCorrFit1DCF_SumRsame(void *p);
   static void deleteArray_NicaCorrFit1DCF_SumRsame(void *p);
   static void destruct_NicaCorrFit1DCF_SumRsame(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit1DCF_SumRsame*)
   {
      ::NicaCorrFit1DCF_SumRsame *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit1DCF_SumRsame >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit1DCF_SumRsame", ::NicaCorrFit1DCF_SumRsame::Class_Version(), "NicaCorrFitMath1DCF.h", 77,
                  typeid(::NicaCorrFit1DCF_SumRsame), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit1DCF_SumRsame::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit1DCF_SumRsame) );
      instance.SetNew(&new_NicaCorrFit1DCF_SumRsame);
      instance.SetNewArray(&newArray_NicaCorrFit1DCF_SumRsame);
      instance.SetDelete(&delete_NicaCorrFit1DCF_SumRsame);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit1DCF_SumRsame);
      instance.SetDestructor(&destruct_NicaCorrFit1DCF_SumRsame);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit1DCF_SumRsame*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit1DCF_SumRsame*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_SumRsame*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaCorrFit1DCF_Multi(void *p);
   static void deleteArray_NicaCorrFit1DCF_Multi(void *p);
   static void destruct_NicaCorrFit1DCF_Multi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit1DCF_Multi*)
   {
      ::NicaCorrFit1DCF_Multi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit1DCF_Multi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit1DCF_Multi", ::NicaCorrFit1DCF_Multi::Class_Version(), "NicaCorrFitMath1DCF.h", 105,
                  typeid(::NicaCorrFit1DCF_Multi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit1DCF_Multi::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit1DCF_Multi) );
      instance.SetDelete(&delete_NicaCorrFit1DCF_Multi);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit1DCF_Multi);
      instance.SetDestructor(&destruct_NicaCorrFit1DCF_Multi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit1DCF_Multi*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit1DCF_Multi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Multi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaCorrFit3DCF(void *p);
   static void deleteArray_NicaCorrFit3DCF(void *p);
   static void destruct_NicaCorrFit3DCF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit3DCF*)
   {
      ::NicaCorrFit3DCF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit3DCF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit3DCF", ::NicaCorrFit3DCF::Class_Version(), "NicaCorrFit3DCF.h", 22,
                  typeid(::NicaCorrFit3DCF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit3DCF::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit3DCF) );
      instance.SetDelete(&delete_NicaCorrFit3DCF);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit3DCF);
      instance.SetDestructor(&destruct_NicaCorrFit3DCF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit3DCF*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit3DCF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit3DCF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaCorrFitMath3DCF(void *p);
   static void deleteArray_NicaCorrFitMath3DCF(void *p);
   static void destruct_NicaCorrFitMath3DCF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitMath3DCF*)
   {
      ::NicaCorrFitMath3DCF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitMath3DCF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitMath3DCF", ::NicaCorrFitMath3DCF::Class_Version(), "NicaCorrFitMath3DCF.h", 14,
                  typeid(::NicaCorrFitMath3DCF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitMath3DCF::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitMath3DCF) );
      instance.SetDelete(&delete_NicaCorrFitMath3DCF);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitMath3DCF);
      instance.SetDestructor(&destruct_NicaCorrFitMath3DCF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitMath3DCF*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitMath3DCF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitMath3DCF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaCorrFit3DCF_Sum(void *p);
   static void deleteArray_NicaCorrFit3DCF_Sum(void *p);
   static void destruct_NicaCorrFit3DCF_Sum(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit3DCF_Sum*)
   {
      ::NicaCorrFit3DCF_Sum *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit3DCF_Sum >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit3DCF_Sum", ::NicaCorrFit3DCF_Sum::Class_Version(), "NicaCorrFitMath3DCF.h", 48,
                  typeid(::NicaCorrFit3DCF_Sum), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit3DCF_Sum::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit3DCF_Sum) );
      instance.SetDelete(&delete_NicaCorrFit3DCF_Sum);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit3DCF_Sum);
      instance.SetDestructor(&destruct_NicaCorrFit3DCF_Sum);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit3DCF_Sum*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit3DCF_Sum*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Sum*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitKisiel(void *p = nullptr);
   static void *newArray_NicaCorrFitKisiel(Long_t size, void *p);
   static void delete_NicaCorrFitKisiel(void *p);
   static void deleteArray_NicaCorrFitKisiel(void *p);
   static void destruct_NicaCorrFitKisiel(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitKisiel*)
   {
      ::NicaCorrFitKisiel *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitKisiel >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitKisiel", ::NicaCorrFitKisiel::Class_Version(), "NicaCorrFitKisiel.h", 27,
                  typeid(::NicaCorrFitKisiel), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitKisiel::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitKisiel) );
      instance.SetNew(&new_NicaCorrFitKisiel);
      instance.SetNewArray(&newArray_NicaCorrFitKisiel);
      instance.SetDelete(&delete_NicaCorrFitKisiel);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitKisiel);
      instance.SetDestructor(&destruct_NicaCorrFitKisiel);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitKisiel*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitKisiel*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitKisiel*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitWielanek(void *p = nullptr);
   static void *newArray_NicaCorrFitWielanek(Long_t size, void *p);
   static void delete_NicaCorrFitWielanek(void *p);
   static void deleteArray_NicaCorrFitWielanek(void *p);
   static void destruct_NicaCorrFitWielanek(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitWielanek*)
   {
      ::NicaCorrFitWielanek *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitWielanek >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitWielanek", ::NicaCorrFitWielanek::Class_Version(), "NicaCorrFitWielanek.h", 21,
                  typeid(::NicaCorrFitWielanek), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitWielanek::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitWielanek) );
      instance.SetNew(&new_NicaCorrFitWielanek);
      instance.SetNewArray(&newArray_NicaCorrFitWielanek);
      instance.SetDelete(&delete_NicaCorrFitWielanek);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitWielanek);
      instance.SetDestructor(&destruct_NicaCorrFitWielanek);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitWielanek*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitWielanek*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitWielanek*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitParButton(void *p = nullptr);
   static void *newArray_NicaCorrFitParButton(Long_t size, void *p);
   static void delete_NicaCorrFitParButton(void *p);
   static void deleteArray_NicaCorrFitParButton(void *p);
   static void destruct_NicaCorrFitParButton(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitParButton*)
   {
      ::NicaCorrFitParButton *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitParButton >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitParButton", ::NicaCorrFitParButton::Class_Version(), "NicaCorrFitGUI.h", 27,
                  typeid(::NicaCorrFitParButton), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitParButton::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitParButton) );
      instance.SetNew(&new_NicaCorrFitParButton);
      instance.SetNewArray(&newArray_NicaCorrFitParButton);
      instance.SetDelete(&delete_NicaCorrFitParButton);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitParButton);
      instance.SetDestructor(&destruct_NicaCorrFitParButton);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitParButton*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitParButton*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitParButton*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitChiSelector(void *p = nullptr);
   static void *newArray_NicaCorrFitChiSelector(Long_t size, void *p);
   static void delete_NicaCorrFitChiSelector(void *p);
   static void deleteArray_NicaCorrFitChiSelector(void *p);
   static void destruct_NicaCorrFitChiSelector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitChiSelector*)
   {
      ::NicaCorrFitChiSelector *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitChiSelector >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitChiSelector", ::NicaCorrFitChiSelector::Class_Version(), "NicaCorrFitGUI.h", 45,
                  typeid(::NicaCorrFitChiSelector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitChiSelector::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitChiSelector) );
      instance.SetNew(&new_NicaCorrFitChiSelector);
      instance.SetNewArray(&newArray_NicaCorrFitChiSelector);
      instance.SetDelete(&delete_NicaCorrFitChiSelector);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitChiSelector);
      instance.SetDestructor(&destruct_NicaCorrFitChiSelector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitChiSelector*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitChiSelector*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitChiSelector*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaCorrFitGUI(void *p);
   static void deleteArray_NicaCorrFitGUI(void *p);
   static void destruct_NicaCorrFitGUI(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitGUI*)
   {
      ::NicaCorrFitGUI *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitGUI >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitGUI", ::NicaCorrFitGUI::Class_Version(), "NicaCorrFitGUI.h", 64,
                  typeid(::NicaCorrFitGUI), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitGUI::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitGUI) );
      instance.SetDelete(&delete_NicaCorrFitGUI);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitGUI);
      instance.SetDestructor(&destruct_NicaCorrFitGUI);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitGUI*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitGUI*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitGUI*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaCorrFitHDFunc(void *p);
   static void deleteArray_NicaCorrFitHDFunc(void *p);
   static void destruct_NicaCorrFitHDFunc(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitHDFunc*)
   {
      ::NicaCorrFitHDFunc *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitHDFunc >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitHDFunc", ::NicaCorrFitHDFunc::Class_Version(), "NicaCorrFitHDFunc.h", 21,
                  typeid(::NicaCorrFitHDFunc), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitHDFunc::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitHDFunc) );
      instance.SetDelete(&delete_NicaCorrFitHDFunc);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitHDFunc);
      instance.SetDestructor(&destruct_NicaCorrFitHDFunc);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitHDFunc*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitHDFunc*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitHDFunc1D(void *p = nullptr);
   static void *newArray_NicaCorrFitHDFunc1D(Long_t size, void *p);
   static void delete_NicaCorrFitHDFunc1D(void *p);
   static void deleteArray_NicaCorrFitHDFunc1D(void *p);
   static void destruct_NicaCorrFitHDFunc1D(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitHDFunc1D*)
   {
      ::NicaCorrFitHDFunc1D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitHDFunc1D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitHDFunc1D", ::NicaCorrFitHDFunc1D::Class_Version(), "NicaCorrFitHDFunc.h", 87,
                  typeid(::NicaCorrFitHDFunc1D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitHDFunc1D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitHDFunc1D) );
      instance.SetNew(&new_NicaCorrFitHDFunc1D);
      instance.SetNewArray(&newArray_NicaCorrFitHDFunc1D);
      instance.SetDelete(&delete_NicaCorrFitHDFunc1D);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitHDFunc1D);
      instance.SetDestructor(&destruct_NicaCorrFitHDFunc1D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitHDFunc1D*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitHDFunc1D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc1D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitHDFunc3D(void *p = nullptr);
   static void *newArray_NicaCorrFitHDFunc3D(Long_t size, void *p);
   static void delete_NicaCorrFitHDFunc3D(void *p);
   static void deleteArray_NicaCorrFitHDFunc3D(void *p);
   static void destruct_NicaCorrFitHDFunc3D(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitHDFunc3D*)
   {
      ::NicaCorrFitHDFunc3D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitHDFunc3D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitHDFunc3D", ::NicaCorrFitHDFunc3D::Class_Version(), "NicaCorrFitHDFunc.h", 137,
                  typeid(::NicaCorrFitHDFunc3D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitHDFunc3D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitHDFunc3D) );
      instance.SetNew(&new_NicaCorrFitHDFunc3D);
      instance.SetNewArray(&newArray_NicaCorrFitHDFunc3D);
      instance.SetDelete(&delete_NicaCorrFitHDFunc3D);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitHDFunc3D);
      instance.SetDestructor(&destruct_NicaCorrFitHDFunc3D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitHDFunc3D*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitHDFunc3D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc3D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoCorrFuncKtMap1D(void *p = nullptr);
   static void *newArray_NicaFemtoCorrFuncKtMap1D(Long_t size, void *p);
   static void delete_NicaFemtoCorrFuncKtMap1D(void *p);
   static void deleteArray_NicaFemtoCorrFuncKtMap1D(void *p);
   static void destruct_NicaFemtoCorrFuncKtMap1D(void *p);
   static Long64_t merge_NicaFemtoCorrFuncKtMap1D(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoCorrFuncKtMap1D*)
   {
      ::NicaFemtoCorrFuncKtMap1D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoCorrFuncKtMap1D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoCorrFuncKtMap1D", ::NicaFemtoCorrFuncKtMap1D::Class_Version(), "NicaFemto1DCFAnaMap.h", 24,
                  typeid(::NicaFemtoCorrFuncKtMap1D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoCorrFuncKtMap1D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoCorrFuncKtMap1D) );
      instance.SetNew(&new_NicaFemtoCorrFuncKtMap1D);
      instance.SetNewArray(&newArray_NicaFemtoCorrFuncKtMap1D);
      instance.SetDelete(&delete_NicaFemtoCorrFuncKtMap1D);
      instance.SetDeleteArray(&deleteArray_NicaFemtoCorrFuncKtMap1D);
      instance.SetDestructor(&destruct_NicaFemtoCorrFuncKtMap1D);
      instance.SetMerge(&merge_NicaFemtoCorrFuncKtMap1D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoCorrFuncKtMap1D*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoCorrFuncKtMap1D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncKtMap1D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemto1DCFAnaMap(void *p = nullptr);
   static void *newArray_NicaFemto1DCFAnaMap(Long_t size, void *p);
   static void delete_NicaFemto1DCFAnaMap(void *p);
   static void deleteArray_NicaFemto1DCFAnaMap(void *p);
   static void destruct_NicaFemto1DCFAnaMap(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemto1DCFAnaMap*)
   {
      ::NicaFemto1DCFAnaMap *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemto1DCFAnaMap >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemto1DCFAnaMap", ::NicaFemto1DCFAnaMap::Class_Version(), "NicaFemto1DCFAnaMap.h", 43,
                  typeid(::NicaFemto1DCFAnaMap), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemto1DCFAnaMap::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemto1DCFAnaMap) );
      instance.SetNew(&new_NicaFemto1DCFAnaMap);
      instance.SetNewArray(&newArray_NicaFemto1DCFAnaMap);
      instance.SetDelete(&delete_NicaFemto1DCFAnaMap);
      instance.SetDeleteArray(&deleteArray_NicaFemto1DCFAnaMap);
      instance.SetDestructor(&destruct_NicaFemto1DCFAnaMap);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemto1DCFAnaMap*)
   {
      return GenerateInitInstanceLocal((::NicaFemto1DCFAnaMap*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMap*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemto1DCFAnaMapMC(void *p = nullptr);
   static void *newArray_NicaFemto1DCFAnaMapMC(Long_t size, void *p);
   static void delete_NicaFemto1DCFAnaMapMC(void *p);
   static void deleteArray_NicaFemto1DCFAnaMapMC(void *p);
   static void destruct_NicaFemto1DCFAnaMapMC(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemto1DCFAnaMapMC*)
   {
      ::NicaFemto1DCFAnaMapMC *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemto1DCFAnaMapMC >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemto1DCFAnaMapMC", ::NicaFemto1DCFAnaMapMC::Class_Version(), "NicaFemto1DCFAnaMapMC.h", 29,
                  typeid(::NicaFemto1DCFAnaMapMC), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemto1DCFAnaMapMC::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemto1DCFAnaMapMC) );
      instance.SetNew(&new_NicaFemto1DCFAnaMapMC);
      instance.SetNewArray(&newArray_NicaFemto1DCFAnaMapMC);
      instance.SetDelete(&delete_NicaFemto1DCFAnaMapMC);
      instance.SetDeleteArray(&deleteArray_NicaFemto1DCFAnaMapMC);
      instance.SetDestructor(&destruct_NicaFemto1DCFAnaMapMC);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemto1DCFAnaMapMC*)
   {
      return GenerateInitInstanceLocal((::NicaFemto1DCFAnaMapMC*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMapMC*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaWielanekMapGenerator(void *p = nullptr);
   static void *newArray_NicaWielanekMapGenerator(Long_t size, void *p);
   static void delete_NicaWielanekMapGenerator(void *p);
   static void deleteArray_NicaWielanekMapGenerator(void *p);
   static void destruct_NicaWielanekMapGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaWielanekMapGenerator*)
   {
      ::NicaWielanekMapGenerator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaWielanekMapGenerator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaWielanekMapGenerator", ::NicaWielanekMapGenerator::Class_Version(), "NicaWielanekMapGenerator.h", 21,
                  typeid(::NicaWielanekMapGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaWielanekMapGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::NicaWielanekMapGenerator) );
      instance.SetNew(&new_NicaWielanekMapGenerator);
      instance.SetNewArray(&newArray_NicaWielanekMapGenerator);
      instance.SetDelete(&delete_NicaWielanekMapGenerator);
      instance.SetDeleteArray(&deleteArray_NicaWielanekMapGenerator);
      instance.SetDestructor(&destruct_NicaWielanekMapGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaWielanekMapGenerator*)
   {
      return GenerateInitInstanceLocal((::NicaWielanekMapGenerator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaWielanekMapGenerator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemto1DCFAnaMapPairsDumped(void *p = nullptr);
   static void *newArray_NicaFemto1DCFAnaMapPairsDumped(Long_t size, void *p);
   static void delete_NicaFemto1DCFAnaMapPairsDumped(void *p);
   static void deleteArray_NicaFemto1DCFAnaMapPairsDumped(void *p);
   static void destruct_NicaFemto1DCFAnaMapPairsDumped(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemto1DCFAnaMapPairsDumped*)
   {
      ::NicaFemto1DCFAnaMapPairsDumped *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemto1DCFAnaMapPairsDumped >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemto1DCFAnaMapPairsDumped", ::NicaFemto1DCFAnaMapPairsDumped::Class_Version(), "NicaFemto1DCFAnaMapPairsDumped.h", 23,
                  typeid(::NicaFemto1DCFAnaMapPairsDumped), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemto1DCFAnaMapPairsDumped::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemto1DCFAnaMapPairsDumped) );
      instance.SetNew(&new_NicaFemto1DCFAnaMapPairsDumped);
      instance.SetNewArray(&newArray_NicaFemto1DCFAnaMapPairsDumped);
      instance.SetDelete(&delete_NicaFemto1DCFAnaMapPairsDumped);
      instance.SetDeleteArray(&deleteArray_NicaFemto1DCFAnaMapPairsDumped);
      instance.SetDestructor(&destruct_NicaFemto1DCFAnaMapPairsDumped);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemto1DCFAnaMapPairsDumped*)
   {
      return GenerateInitInstanceLocal((::NicaFemto1DCFAnaMapPairsDumped*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMapPairsDumped*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit1DCF_Square(void *p = nullptr);
   static void *newArray_NicaCorrFit1DCF_Square(Long_t size, void *p);
   static void delete_NicaCorrFit1DCF_Square(void *p);
   static void deleteArray_NicaCorrFit1DCF_Square(void *p);
   static void destruct_NicaCorrFit1DCF_Square(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit1DCF_Square*)
   {
      ::NicaCorrFit1DCF_Square *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit1DCF_Square >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit1DCF_Square", ::NicaCorrFit1DCF_Square::Class_Version(), "NicaCorrFit1DCFSquare.h", 18,
                  typeid(::NicaCorrFit1DCF_Square), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit1DCF_Square::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit1DCF_Square) );
      instance.SetNew(&new_NicaCorrFit1DCF_Square);
      instance.SetNewArray(&newArray_NicaCorrFit1DCF_Square);
      instance.SetDelete(&delete_NicaCorrFit1DCF_Square);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit1DCF_Square);
      instance.SetDestructor(&destruct_NicaCorrFit1DCF_Square);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit1DCF_Square*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit1DCF_Square*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Square*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit1DCF_Lorentz(void *p = nullptr);
   static void *newArray_NicaCorrFit1DCF_Lorentz(Long_t size, void *p);
   static void delete_NicaCorrFit1DCF_Lorentz(void *p);
   static void deleteArray_NicaCorrFit1DCF_Lorentz(void *p);
   static void destruct_NicaCorrFit1DCF_Lorentz(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit1DCF_Lorentz*)
   {
      ::NicaCorrFit1DCF_Lorentz *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit1DCF_Lorentz >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit1DCF_Lorentz", ::NicaCorrFit1DCF_Lorentz::Class_Version(), "NicaCorrFit1DCFLorentz.h", 14,
                  typeid(::NicaCorrFit1DCF_Lorentz), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit1DCF_Lorentz::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit1DCF_Lorentz) );
      instance.SetNew(&new_NicaCorrFit1DCF_Lorentz);
      instance.SetNewArray(&newArray_NicaCorrFit1DCF_Lorentz);
      instance.SetDelete(&delete_NicaCorrFit1DCF_Lorentz);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit1DCF_Lorentz);
      instance.SetDestructor(&destruct_NicaCorrFit1DCF_Lorentz);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit1DCF_Lorentz*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit1DCF_Lorentz*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Lorentz*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit3DCF_Square(void *p = nullptr);
   static void *newArray_NicaCorrFit3DCF_Square(Long_t size, void *p);
   static void delete_NicaCorrFit3DCF_Square(void *p);
   static void deleteArray_NicaCorrFit3DCF_Square(void *p);
   static void destruct_NicaCorrFit3DCF_Square(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit3DCF_Square*)
   {
      ::NicaCorrFit3DCF_Square *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit3DCF_Square >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit3DCF_Square", ::NicaCorrFit3DCF_Square::Class_Version(), "NicaCorrFit3DCFSquare.h", 17,
                  typeid(::NicaCorrFit3DCF_Square), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit3DCF_Square::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit3DCF_Square) );
      instance.SetNew(&new_NicaCorrFit3DCF_Square);
      instance.SetNewArray(&newArray_NicaCorrFit3DCF_Square);
      instance.SetDelete(&delete_NicaCorrFit3DCF_Square);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit3DCF_Square);
      instance.SetDestructor(&destruct_NicaCorrFit3DCF_Square);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit3DCF_Square*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit3DCF_Square*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Square*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit3DCF_Lorentz(void *p = nullptr);
   static void *newArray_NicaCorrFit3DCF_Lorentz(Long_t size, void *p);
   static void delete_NicaCorrFit3DCF_Lorentz(void *p);
   static void deleteArray_NicaCorrFit3DCF_Lorentz(void *p);
   static void destruct_NicaCorrFit3DCF_Lorentz(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit3DCF_Lorentz*)
   {
      ::NicaCorrFit3DCF_Lorentz *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit3DCF_Lorentz >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit3DCF_Lorentz", ::NicaCorrFit3DCF_Lorentz::Class_Version(), "NicaCorrFit3DCFLorentz.h", 14,
                  typeid(::NicaCorrFit3DCF_Lorentz), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit3DCF_Lorentz::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit3DCF_Lorentz) );
      instance.SetNew(&new_NicaCorrFit3DCF_Lorentz);
      instance.SetNewArray(&newArray_NicaCorrFit3DCF_Lorentz);
      instance.SetDelete(&delete_NicaCorrFit3DCF_Lorentz);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit3DCF_Lorentz);
      instance.SetDestructor(&destruct_NicaCorrFit3DCF_Lorentz);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit3DCF_Lorentz*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit3DCF_Lorentz*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Lorentz*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit1DCF_Exp(void *p = nullptr);
   static void *newArray_NicaCorrFit1DCF_Exp(Long_t size, void *p);
   static void delete_NicaCorrFit1DCF_Exp(void *p);
   static void deleteArray_NicaCorrFit1DCF_Exp(void *p);
   static void destruct_NicaCorrFit1DCF_Exp(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit1DCF_Exp*)
   {
      ::NicaCorrFit1DCF_Exp *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit1DCF_Exp >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit1DCF_Exp", ::NicaCorrFit1DCF_Exp::Class_Version(), "NicaCorrFit1DCFExp.h", 18,
                  typeid(::NicaCorrFit1DCF_Exp), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit1DCF_Exp::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit1DCF_Exp) );
      instance.SetNew(&new_NicaCorrFit1DCF_Exp);
      instance.SetNewArray(&newArray_NicaCorrFit1DCF_Exp);
      instance.SetDelete(&delete_NicaCorrFit1DCF_Exp);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit1DCF_Exp);
      instance.SetDestructor(&destruct_NicaCorrFit1DCF_Exp);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit1DCF_Exp*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit1DCF_Exp*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Exp*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit1DCF_Gauss(void *p = nullptr);
   static void *newArray_NicaCorrFit1DCF_Gauss(Long_t size, void *p);
   static void delete_NicaCorrFit1DCF_Gauss(void *p);
   static void deleteArray_NicaCorrFit1DCF_Gauss(void *p);
   static void destruct_NicaCorrFit1DCF_Gauss(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit1DCF_Gauss*)
   {
      ::NicaCorrFit1DCF_Gauss *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit1DCF_Gauss >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit1DCF_Gauss", ::NicaCorrFit1DCF_Gauss::Class_Version(), "NicaCorrFit1DCFGauss.h", 17,
                  typeid(::NicaCorrFit1DCF_Gauss), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit1DCF_Gauss::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit1DCF_Gauss) );
      instance.SetNew(&new_NicaCorrFit1DCF_Gauss);
      instance.SetNewArray(&newArray_NicaCorrFit1DCF_Gauss);
      instance.SetDelete(&delete_NicaCorrFit1DCF_Gauss);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit1DCF_Gauss);
      instance.SetDestructor(&destruct_NicaCorrFit1DCF_Gauss);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit1DCF_Gauss*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit1DCF_Gauss*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Gauss*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit1DCF_Poly(void *p = nullptr);
   static void *newArray_NicaCorrFit1DCF_Poly(Long_t size, void *p);
   static void delete_NicaCorrFit1DCF_Poly(void *p);
   static void deleteArray_NicaCorrFit1DCF_Poly(void *p);
   static void destruct_NicaCorrFit1DCF_Poly(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit1DCF_Poly*)
   {
      ::NicaCorrFit1DCF_Poly *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit1DCF_Poly >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit1DCF_Poly", ::NicaCorrFit1DCF_Poly::Class_Version(), "NicaCorrFit1DCFPoly.h", 17,
                  typeid(::NicaCorrFit1DCF_Poly), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit1DCF_Poly::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit1DCF_Poly) );
      instance.SetNew(&new_NicaCorrFit1DCF_Poly);
      instance.SetNewArray(&newArray_NicaCorrFit1DCF_Poly);
      instance.SetDelete(&delete_NicaCorrFit1DCF_Poly);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit1DCF_Poly);
      instance.SetDestructor(&destruct_NicaCorrFit1DCF_Poly);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit1DCF_Poly*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit1DCF_Poly*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Poly*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaCorrFit1DCF_Poly2(void *p);
   static void deleteArray_NicaCorrFit1DCF_Poly2(void *p);
   static void destruct_NicaCorrFit1DCF_Poly2(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit1DCF_Poly2*)
   {
      ::NicaCorrFit1DCF_Poly2 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit1DCF_Poly2 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit1DCF_Poly2", ::NicaCorrFit1DCF_Poly2::Class_Version(), "NicaCorrFit1DCFPoly.h", 34,
                  typeid(::NicaCorrFit1DCF_Poly2), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit1DCF_Poly2::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit1DCF_Poly2) );
      instance.SetDelete(&delete_NicaCorrFit1DCF_Poly2);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit1DCF_Poly2);
      instance.SetDestructor(&destruct_NicaCorrFit1DCF_Poly2);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit1DCF_Poly2*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit1DCF_Poly2*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Poly2*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit3DCF_Gauss(void *p = nullptr);
   static void *newArray_NicaCorrFit3DCF_Gauss(Long_t size, void *p);
   static void delete_NicaCorrFit3DCF_Gauss(void *p);
   static void deleteArray_NicaCorrFit3DCF_Gauss(void *p);
   static void destruct_NicaCorrFit3DCF_Gauss(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit3DCF_Gauss*)
   {
      ::NicaCorrFit3DCF_Gauss *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit3DCF_Gauss >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit3DCF_Gauss", ::NicaCorrFit3DCF_Gauss::Class_Version(), "NicaCorrFit3DCFGauss.h", 17,
                  typeid(::NicaCorrFit3DCF_Gauss), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit3DCF_Gauss::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit3DCF_Gauss) );
      instance.SetNew(&new_NicaCorrFit3DCF_Gauss);
      instance.SetNewArray(&newArray_NicaCorrFit3DCF_Gauss);
      instance.SetDelete(&delete_NicaCorrFit3DCF_Gauss);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit3DCF_Gauss);
      instance.SetDestructor(&destruct_NicaCorrFit3DCF_Gauss);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit3DCF_Gauss*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit3DCF_Gauss*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Gauss*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit3DCF_Gauss2(void *p = nullptr);
   static void *newArray_NicaCorrFit3DCF_Gauss2(Long_t size, void *p);
   static void delete_NicaCorrFit3DCF_Gauss2(void *p);
   static void deleteArray_NicaCorrFit3DCF_Gauss2(void *p);
   static void destruct_NicaCorrFit3DCF_Gauss2(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit3DCF_Gauss2*)
   {
      ::NicaCorrFit3DCF_Gauss2 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit3DCF_Gauss2 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit3DCF_Gauss2", ::NicaCorrFit3DCF_Gauss2::Class_Version(), "NicaCorrFit3DCFGauss2.h", 17,
                  typeid(::NicaCorrFit3DCF_Gauss2), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit3DCF_Gauss2::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit3DCF_Gauss2) );
      instance.SetNew(&new_NicaCorrFit3DCF_Gauss2);
      instance.SetNewArray(&newArray_NicaCorrFit3DCF_Gauss2);
      instance.SetDelete(&delete_NicaCorrFit3DCF_Gauss2);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit3DCF_Gauss2);
      instance.SetDestructor(&destruct_NicaCorrFit3DCF_Gauss2);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit3DCF_Gauss2*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit3DCF_Gauss2*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Gauss2*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit3DCF_GaussCross(void *p = nullptr);
   static void *newArray_NicaCorrFit3DCF_GaussCross(Long_t size, void *p);
   static void delete_NicaCorrFit3DCF_GaussCross(void *p);
   static void deleteArray_NicaCorrFit3DCF_GaussCross(void *p);
   static void destruct_NicaCorrFit3DCF_GaussCross(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit3DCF_GaussCross*)
   {
      ::NicaCorrFit3DCF_GaussCross *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit3DCF_GaussCross >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit3DCF_GaussCross", ::NicaCorrFit3DCF_GaussCross::Class_Version(), "NicaCorrFit3DCFGaussCross.h", 17,
                  typeid(::NicaCorrFit3DCF_GaussCross), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit3DCF_GaussCross::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit3DCF_GaussCross) );
      instance.SetNew(&new_NicaCorrFit3DCF_GaussCross);
      instance.SetNewArray(&newArray_NicaCorrFit3DCF_GaussCross);
      instance.SetDelete(&delete_NicaCorrFit3DCF_GaussCross);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit3DCF_GaussCross);
      instance.SetDestructor(&destruct_NicaCorrFit3DCF_GaussCross);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit3DCF_GaussCross*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit3DCF_GaussCross*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_GaussCross*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit1DCFCumac(void *p = nullptr);
   static void *newArray_NicaCorrFit1DCFCumac(Long_t size, void *p);
   static void delete_NicaCorrFit1DCFCumac(void *p);
   static void deleteArray_NicaCorrFit1DCFCumac(void *p);
   static void destruct_NicaCorrFit1DCFCumac(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit1DCFCumac*)
   {
      ::NicaCorrFit1DCFCumac *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit1DCFCumac >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit1DCFCumac", ::NicaCorrFit1DCFCumac::Class_Version(), "NicaCorrFit1DCFCumac.h", 14,
                  typeid(::NicaCorrFit1DCFCumac), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit1DCFCumac::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit1DCFCumac) );
      instance.SetNew(&new_NicaCorrFit1DCFCumac);
      instance.SetNewArray(&newArray_NicaCorrFit1DCFCumac);
      instance.SetDelete(&delete_NicaCorrFit1DCFCumac);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit1DCFCumac);
      instance.SetDestructor(&destruct_NicaCorrFit1DCFCumac);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit1DCFCumac*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit1DCFCumac*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumac*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFIt1DCFCumacLamLam(void *p = nullptr);
   static void *newArray_NicaCorrFIt1DCFCumacLamLam(Long_t size, void *p);
   static void delete_NicaCorrFIt1DCFCumacLamLam(void *p);
   static void deleteArray_NicaCorrFIt1DCFCumacLamLam(void *p);
   static void destruct_NicaCorrFIt1DCFCumacLamLam(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFIt1DCFCumacLamLam*)
   {
      ::NicaCorrFIt1DCFCumacLamLam *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFIt1DCFCumacLamLam >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFIt1DCFCumacLamLam", ::NicaCorrFIt1DCFCumacLamLam::Class_Version(), "NicaCorrFit1DCFCumac.h", 51,
                  typeid(::NicaCorrFIt1DCFCumacLamLam), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFIt1DCFCumacLamLam::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFIt1DCFCumacLamLam) );
      instance.SetNew(&new_NicaCorrFIt1DCFCumacLamLam);
      instance.SetNewArray(&newArray_NicaCorrFIt1DCFCumacLamLam);
      instance.SetDelete(&delete_NicaCorrFIt1DCFCumacLamLam);
      instance.SetDeleteArray(&deleteArray_NicaCorrFIt1DCFCumacLamLam);
      instance.SetDestructor(&destruct_NicaCorrFIt1DCFCumacLamLam);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFIt1DCFCumacLamLam*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFIt1DCFCumacLamLam*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFIt1DCFCumacLamLam*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit1DCFCumacPLam(void *p = nullptr);
   static void *newArray_NicaCorrFit1DCFCumacPLam(Long_t size, void *p);
   static void delete_NicaCorrFit1DCFCumacPLam(void *p);
   static void deleteArray_NicaCorrFit1DCFCumacPLam(void *p);
   static void destruct_NicaCorrFit1DCFCumacPLam(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit1DCFCumacPLam*)
   {
      ::NicaCorrFit1DCFCumacPLam *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit1DCFCumacPLam >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit1DCFCumacPLam", ::NicaCorrFit1DCFCumacPLam::Class_Version(), "NicaCorrFit1DCFCumac.h", 70,
                  typeid(::NicaCorrFit1DCFCumacPLam), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit1DCFCumacPLam::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit1DCFCumacPLam) );
      instance.SetNew(&new_NicaCorrFit1DCFCumacPLam);
      instance.SetNewArray(&newArray_NicaCorrFit1DCFCumacPLam);
      instance.SetDelete(&delete_NicaCorrFit1DCFCumacPLam);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit1DCFCumacPLam);
      instance.SetDestructor(&destruct_NicaCorrFit1DCFCumacPLam);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit1DCFCumacPLam*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit1DCFCumacPLam*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumacPLam*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit1DCFCumacK0K0(void *p = nullptr);
   static void *newArray_NicaCorrFit1DCFCumacK0K0(Long_t size, void *p);
   static void delete_NicaCorrFit1DCFCumacK0K0(void *p);
   static void deleteArray_NicaCorrFit1DCFCumacK0K0(void *p);
   static void destruct_NicaCorrFit1DCFCumacK0K0(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit1DCFCumacK0K0*)
   {
      ::NicaCorrFit1DCFCumacK0K0 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit1DCFCumacK0K0 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit1DCFCumacK0K0", ::NicaCorrFit1DCFCumacK0K0::Class_Version(), "NicaCorrFit1DCFCumac.h", 86,
                  typeid(::NicaCorrFit1DCFCumacK0K0), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit1DCFCumacK0K0::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit1DCFCumacK0K0) );
      instance.SetNew(&new_NicaCorrFit1DCFCumacK0K0);
      instance.SetNewArray(&newArray_NicaCorrFit1DCFCumacK0K0);
      instance.SetDelete(&delete_NicaCorrFit1DCFCumacK0K0);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit1DCFCumacK0K0);
      instance.SetDestructor(&destruct_NicaCorrFit1DCFCumacK0K0);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit1DCFCumacK0K0*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit1DCFCumacK0K0*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumacK0K0*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit3DCFBowlerSinyukov(void *p = nullptr);
   static void *newArray_NicaCorrFit3DCFBowlerSinyukov(Long_t size, void *p);
   static void delete_NicaCorrFit3DCFBowlerSinyukov(void *p);
   static void deleteArray_NicaCorrFit3DCFBowlerSinyukov(void *p);
   static void destruct_NicaCorrFit3DCFBowlerSinyukov(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit3DCFBowlerSinyukov*)
   {
      ::NicaCorrFit3DCFBowlerSinyukov *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit3DCFBowlerSinyukov >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit3DCFBowlerSinyukov", ::NicaCorrFit3DCFBowlerSinyukov::Class_Version(), "NicaCorrFit3DCFBowlerSinyukov.h", 19,
                  typeid(::NicaCorrFit3DCFBowlerSinyukov), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit3DCFBowlerSinyukov::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit3DCFBowlerSinyukov) );
      instance.SetNew(&new_NicaCorrFit3DCFBowlerSinyukov);
      instance.SetNewArray(&newArray_NicaCorrFit3DCFBowlerSinyukov);
      instance.SetDelete(&delete_NicaCorrFit3DCFBowlerSinyukov);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit3DCFBowlerSinyukov);
      instance.SetDestructor(&destruct_NicaCorrFit3DCFBowlerSinyukov);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit3DCFBowlerSinyukov*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit3DCFBowlerSinyukov*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukov*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit3DCFBowlerSinyukovEllipse(void *p = nullptr);
   static void *newArray_NicaCorrFit3DCFBowlerSinyukovEllipse(Long_t size, void *p);
   static void delete_NicaCorrFit3DCFBowlerSinyukovEllipse(void *p);
   static void deleteArray_NicaCorrFit3DCFBowlerSinyukovEllipse(void *p);
   static void destruct_NicaCorrFit3DCFBowlerSinyukovEllipse(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit3DCFBowlerSinyukovEllipse*)
   {
      ::NicaCorrFit3DCFBowlerSinyukovEllipse *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit3DCFBowlerSinyukovEllipse >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit3DCFBowlerSinyukovEllipse", ::NicaCorrFit3DCFBowlerSinyukovEllipse::Class_Version(), "NicaCorrFit3DCFBowlerSinyukovExtra.h", 18,
                  typeid(::NicaCorrFit3DCFBowlerSinyukovEllipse), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit3DCFBowlerSinyukovEllipse::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit3DCFBowlerSinyukovEllipse) );
      instance.SetNew(&new_NicaCorrFit3DCFBowlerSinyukovEllipse);
      instance.SetNewArray(&newArray_NicaCorrFit3DCFBowlerSinyukovEllipse);
      instance.SetDelete(&delete_NicaCorrFit3DCFBowlerSinyukovEllipse);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit3DCFBowlerSinyukovEllipse);
      instance.SetDestructor(&destruct_NicaCorrFit3DCFBowlerSinyukovEllipse);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit3DCFBowlerSinyukovEllipse*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit3DCFBowlerSinyukovEllipse*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukovEllipse*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit3DCFBowlerSinyukovClassic(void *p = nullptr);
   static void *newArray_NicaCorrFit3DCFBowlerSinyukovClassic(Long_t size, void *p);
   static void delete_NicaCorrFit3DCFBowlerSinyukovClassic(void *p);
   static void deleteArray_NicaCorrFit3DCFBowlerSinyukovClassic(void *p);
   static void destruct_NicaCorrFit3DCFBowlerSinyukovClassic(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit3DCFBowlerSinyukovClassic*)
   {
      ::NicaCorrFit3DCFBowlerSinyukovClassic *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit3DCFBowlerSinyukovClassic >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit3DCFBowlerSinyukovClassic", ::NicaCorrFit3DCFBowlerSinyukovClassic::Class_Version(), "NicaCorrFit3DCFBowlerSinyukovExtra.h", 41,
                  typeid(::NicaCorrFit3DCFBowlerSinyukovClassic), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit3DCFBowlerSinyukovClassic::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit3DCFBowlerSinyukovClassic) );
      instance.SetNew(&new_NicaCorrFit3DCFBowlerSinyukovClassic);
      instance.SetNewArray(&newArray_NicaCorrFit3DCFBowlerSinyukovClassic);
      instance.SetDelete(&delete_NicaCorrFit3DCFBowlerSinyukovClassic);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit3DCFBowlerSinyukovClassic);
      instance.SetDestructor(&destruct_NicaCorrFit3DCFBowlerSinyukovClassic);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit3DCFBowlerSinyukovClassic*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit3DCFBowlerSinyukovClassic*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukovClassic*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoFsiParsed(void *p = nullptr);
   static void *newArray_NicaFemtoFsiParsed(Long_t size, void *p);
   static void delete_NicaFemtoFsiParsed(void *p);
   static void deleteArray_NicaFemtoFsiParsed(void *p);
   static void destruct_NicaFemtoFsiParsed(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoFsiParsed*)
   {
      ::NicaFemtoFsiParsed *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoFsiParsed >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoFsiParsed", ::NicaFemtoFsiParsed::Class_Version(), "NicaFemtoFsiParsed.h", 15,
                  typeid(::NicaFemtoFsiParsed), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoFsiParsed::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoFsiParsed) );
      instance.SetNew(&new_NicaFemtoFsiParsed);
      instance.SetNewArray(&newArray_NicaFemtoFsiParsed);
      instance.SetDelete(&delete_NicaFemtoFsiParsed);
      instance.SetDeleteArray(&deleteArray_NicaFemtoFsiParsed);
      instance.SetDestructor(&destruct_NicaFemtoFsiParsed);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoFsiParsed*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoFsiParsed*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoFsiParsed*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoWeightGeneratorLednicky2(void *p = nullptr);
   static void *newArray_NicaFemtoWeightGeneratorLednicky2(Long_t size, void *p);
   static void delete_NicaFemtoWeightGeneratorLednicky2(void *p);
   static void deleteArray_NicaFemtoWeightGeneratorLednicky2(void *p);
   static void destruct_NicaFemtoWeightGeneratorLednicky2(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoWeightGeneratorLednicky2*)
   {
      ::NicaFemtoWeightGeneratorLednicky2 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoWeightGeneratorLednicky2 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoWeightGeneratorLednicky2", ::NicaFemtoWeightGeneratorLednicky2::Class_Version(), "NicaFemtoWeightGeneratorLednicky2.h", 18,
                  typeid(::NicaFemtoWeightGeneratorLednicky2), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoWeightGeneratorLednicky2::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoWeightGeneratorLednicky2) );
      instance.SetNew(&new_NicaFemtoWeightGeneratorLednicky2);
      instance.SetNewArray(&newArray_NicaFemtoWeightGeneratorLednicky2);
      instance.SetDelete(&delete_NicaFemtoWeightGeneratorLednicky2);
      instance.SetDeleteArray(&deleteArray_NicaFemtoWeightGeneratorLednicky2);
      instance.SetDestructor(&destruct_NicaFemtoWeightGeneratorLednicky2);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoWeightGeneratorLednicky2*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoWeightGeneratorLednicky2*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednicky2*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoWeightGeneratorBasic(void *p = nullptr);
   static void *newArray_NicaFemtoWeightGeneratorBasic(Long_t size, void *p);
   static void delete_NicaFemtoWeightGeneratorBasic(void *p);
   static void deleteArray_NicaFemtoWeightGeneratorBasic(void *p);
   static void destruct_NicaFemtoWeightGeneratorBasic(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoWeightGeneratorBasic*)
   {
      ::NicaFemtoWeightGeneratorBasic *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoWeightGeneratorBasic >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoWeightGeneratorBasic", ::NicaFemtoWeightGeneratorBasic::Class_Version(), "NicaFemtoWeightGeneratorBasic.h", 18,
                  typeid(::NicaFemtoWeightGeneratorBasic), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoWeightGeneratorBasic::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoWeightGeneratorBasic) );
      instance.SetNew(&new_NicaFemtoWeightGeneratorBasic);
      instance.SetNewArray(&newArray_NicaFemtoWeightGeneratorBasic);
      instance.SetDelete(&delete_NicaFemtoWeightGeneratorBasic);
      instance.SetDeleteArray(&deleteArray_NicaFemtoWeightGeneratorBasic);
      instance.SetDestructor(&destruct_NicaFemtoWeightGeneratorBasic);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoWeightGeneratorBasic*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoWeightGeneratorBasic*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorBasic*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoWeightGeneratorLednicky(void *p = nullptr);
   static void *newArray_NicaFemtoWeightGeneratorLednicky(Long_t size, void *p);
   static void delete_NicaFemtoWeightGeneratorLednicky(void *p);
   static void deleteArray_NicaFemtoWeightGeneratorLednicky(void *p);
   static void destruct_NicaFemtoWeightGeneratorLednicky(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoWeightGeneratorLednicky*)
   {
      ::NicaFemtoWeightGeneratorLednicky *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoWeightGeneratorLednicky >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoWeightGeneratorLednicky", ::NicaFemtoWeightGeneratorLednicky::Class_Version(), "NicaFemtoWeightGeneratorLednicky.h", 19,
                  typeid(::NicaFemtoWeightGeneratorLednicky), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoWeightGeneratorLednicky::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoWeightGeneratorLednicky) );
      instance.SetNew(&new_NicaFemtoWeightGeneratorLednicky);
      instance.SetNewArray(&newArray_NicaFemtoWeightGeneratorLednicky);
      instance.SetDelete(&delete_NicaFemtoWeightGeneratorLednicky);
      instance.SetDeleteArray(&deleteArray_NicaFemtoWeightGeneratorLednicky);
      instance.SetDestructor(&destruct_NicaFemtoWeightGeneratorLednicky);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoWeightGeneratorLednicky*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoWeightGeneratorLednicky*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednicky*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoWeightGeneratorLednickyResidual(void *p = nullptr);
   static void *newArray_NicaFemtoWeightGeneratorLednickyResidual(Long_t size, void *p);
   static void delete_NicaFemtoWeightGeneratorLednickyResidual(void *p);
   static void deleteArray_NicaFemtoWeightGeneratorLednickyResidual(void *p);
   static void destruct_NicaFemtoWeightGeneratorLednickyResidual(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoWeightGeneratorLednickyResidual*)
   {
      ::NicaFemtoWeightGeneratorLednickyResidual *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoWeightGeneratorLednickyResidual >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoWeightGeneratorLednickyResidual", ::NicaFemtoWeightGeneratorLednickyResidual::Class_Version(), "NicaFemtoWeightGeneratorLednickyResidual.h", 20,
                  typeid(::NicaFemtoWeightGeneratorLednickyResidual), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoWeightGeneratorLednickyResidual::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoWeightGeneratorLednickyResidual) );
      instance.SetNew(&new_NicaFemtoWeightGeneratorLednickyResidual);
      instance.SetNewArray(&newArray_NicaFemtoWeightGeneratorLednickyResidual);
      instance.SetDelete(&delete_NicaFemtoWeightGeneratorLednickyResidual);
      instance.SetDeleteArray(&deleteArray_NicaFemtoWeightGeneratorLednickyResidual);
      instance.SetDestructor(&destruct_NicaFemtoWeightGeneratorLednickyResidual);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoWeightGeneratorLednickyResidual*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoWeightGeneratorLednickyResidual*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednickyResidual*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoWeightGeneratorSupressed(void *p = nullptr);
   static void *newArray_NicaFemtoWeightGeneratorSupressed(Long_t size, void *p);
   static void delete_NicaFemtoWeightGeneratorSupressed(void *p);
   static void deleteArray_NicaFemtoWeightGeneratorSupressed(void *p);
   static void destruct_NicaFemtoWeightGeneratorSupressed(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoWeightGeneratorSupressed*)
   {
      ::NicaFemtoWeightGeneratorSupressed *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoWeightGeneratorSupressed >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoWeightGeneratorSupressed", ::NicaFemtoWeightGeneratorSupressed::Class_Version(), "NicaFemtoWeightGeneratorSupressed.h", 17,
                  typeid(::NicaFemtoWeightGeneratorSupressed), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoWeightGeneratorSupressed::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoWeightGeneratorSupressed) );
      instance.SetNew(&new_NicaFemtoWeightGeneratorSupressed);
      instance.SetNewArray(&newArray_NicaFemtoWeightGeneratorSupressed);
      instance.SetDelete(&delete_NicaFemtoWeightGeneratorSupressed);
      instance.SetDeleteArray(&deleteArray_NicaFemtoWeightGeneratorSupressed);
      instance.SetDestructor(&destruct_NicaFemtoWeightGeneratorSupressed);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoWeightGeneratorSupressed*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoWeightGeneratorSupressed*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorSupressed*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaFemtoCorrFuncPt(void *p);
   static void deleteArray_NicaFemtoCorrFuncPt(void *p);
   static void destruct_NicaFemtoCorrFuncPt(void *p);
   static Long64_t merge_NicaFemtoCorrFuncPt(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoCorrFuncPt*)
   {
      ::NicaFemtoCorrFuncPt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoCorrFuncPt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoCorrFuncPt", ::NicaFemtoCorrFuncPt::Class_Version(), "NicaFemtoCorrFuncPt.h", 15,
                  typeid(::NicaFemtoCorrFuncPt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoCorrFuncPt::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoCorrFuncPt) );
      instance.SetDelete(&delete_NicaFemtoCorrFuncPt);
      instance.SetDeleteArray(&deleteArray_NicaFemtoCorrFuncPt);
      instance.SetDestructor(&destruct_NicaFemtoCorrFuncPt);
      instance.SetMerge(&merge_NicaFemtoCorrFuncPt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoCorrFuncPt*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoCorrFuncPt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncPt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoCorrFuncKt(void *p = nullptr);
   static void *newArray_NicaFemtoCorrFuncKt(Long_t size, void *p);
   static void delete_NicaFemtoCorrFuncKt(void *p);
   static void deleteArray_NicaFemtoCorrFuncKt(void *p);
   static void destruct_NicaFemtoCorrFuncKt(void *p);
   static Long64_t merge_NicaFemtoCorrFuncKt(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoCorrFuncKt*)
   {
      ::NicaFemtoCorrFuncKt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoCorrFuncKt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoCorrFuncKt", ::NicaFemtoCorrFuncKt::Class_Version(), "NicaFemtoCorrFuncKt.h", 17,
                  typeid(::NicaFemtoCorrFuncKt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoCorrFuncKt::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoCorrFuncKt) );
      instance.SetNew(&new_NicaFemtoCorrFuncKt);
      instance.SetNewArray(&newArray_NicaFemtoCorrFuncKt);
      instance.SetDelete(&delete_NicaFemtoCorrFuncKt);
      instance.SetDeleteArray(&deleteArray_NicaFemtoCorrFuncKt);
      instance.SetDestructor(&destruct_NicaFemtoCorrFuncKt);
      instance.SetMerge(&merge_NicaFemtoCorrFuncKt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoCorrFuncKt*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoCorrFuncKt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncKt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaFemtoCorrFuncAzimuthalKt(void *p);
   static void deleteArray_NicaFemtoCorrFuncAzimuthalKt(void *p);
   static void destruct_NicaFemtoCorrFuncAzimuthalKt(void *p);
   static Long64_t merge_NicaFemtoCorrFuncAzimuthalKt(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoCorrFuncAzimuthalKt*)
   {
      ::NicaFemtoCorrFuncAzimuthalKt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoCorrFuncAzimuthalKt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoCorrFuncAzimuthalKt", ::NicaFemtoCorrFuncAzimuthalKt::Class_Version(), "NicaFemtoCorrFuncAzimuthalKt.h", 15,
                  typeid(::NicaFemtoCorrFuncAzimuthalKt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoCorrFuncAzimuthalKt::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoCorrFuncAzimuthalKt) );
      instance.SetDelete(&delete_NicaFemtoCorrFuncAzimuthalKt);
      instance.SetDeleteArray(&deleteArray_NicaFemtoCorrFuncAzimuthalKt);
      instance.SetDestructor(&destruct_NicaFemtoCorrFuncAzimuthalKt);
      instance.SetMerge(&merge_NicaFemtoCorrFuncAzimuthalKt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoCorrFuncAzimuthalKt*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoCorrFuncAzimuthalKt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncAzimuthalKt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoCorrFuncSimple(void *p = nullptr);
   static void *newArray_NicaFemtoCorrFuncSimple(Long_t size, void *p);
   static void delete_NicaFemtoCorrFuncSimple(void *p);
   static void deleteArray_NicaFemtoCorrFuncSimple(void *p);
   static void destruct_NicaFemtoCorrFuncSimple(void *p);
   static Long64_t merge_NicaFemtoCorrFuncSimple(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoCorrFuncSimple*)
   {
      ::NicaFemtoCorrFuncSimple *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoCorrFuncSimple >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoCorrFuncSimple", ::NicaFemtoCorrFuncSimple::Class_Version(), "NicaFemtoCorrFuncSimple.h", 14,
                  typeid(::NicaFemtoCorrFuncSimple), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoCorrFuncSimple::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoCorrFuncSimple) );
      instance.SetNew(&new_NicaFemtoCorrFuncSimple);
      instance.SetNewArray(&newArray_NicaFemtoCorrFuncSimple);
      instance.SetDelete(&delete_NicaFemtoCorrFuncSimple);
      instance.SetDeleteArray(&deleteArray_NicaFemtoCorrFuncSimple);
      instance.SetDestructor(&destruct_NicaFemtoCorrFuncSimple);
      instance.SetMerge(&merge_NicaFemtoCorrFuncSimple);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoCorrFuncSimple*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoCorrFuncSimple*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncSimple*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoImaging(void *p = nullptr);
   static void *newArray_NicaFemtoImaging(Long_t size, void *p);
   static void delete_NicaFemtoImaging(void *p);
   static void deleteArray_NicaFemtoImaging(void *p);
   static void destruct_NicaFemtoImaging(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoImaging*)
   {
      ::NicaFemtoImaging *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoImaging >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoImaging", ::NicaFemtoImaging::Class_Version(), "NicaFemtoImaging.h", 15,
                  typeid(::NicaFemtoImaging), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoImaging::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoImaging) );
      instance.SetNew(&new_NicaFemtoImaging);
      instance.SetNewArray(&newArray_NicaFemtoImaging);
      instance.SetDelete(&delete_NicaFemtoImaging);
      instance.SetDeleteArray(&deleteArray_NicaFemtoImaging);
      instance.SetDestructor(&destruct_NicaFemtoImaging);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoImaging*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoImaging*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoImaging*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoImaging1D(void *p = nullptr);
   static void *newArray_NicaFemtoImaging1D(Long_t size, void *p);
   static void delete_NicaFemtoImaging1D(void *p);
   static void deleteArray_NicaFemtoImaging1D(void *p);
   static void destruct_NicaFemtoImaging1D(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoImaging1D*)
   {
      ::NicaFemtoImaging1D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoImaging1D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoImaging1D", ::NicaFemtoImaging1D::Class_Version(), "NicaFemtoImaging1D.h", 18,
                  typeid(::NicaFemtoImaging1D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoImaging1D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoImaging1D) );
      instance.SetNew(&new_NicaFemtoImaging1D);
      instance.SetNewArray(&newArray_NicaFemtoImaging1D);
      instance.SetDelete(&delete_NicaFemtoImaging1D);
      instance.SetDeleteArray(&deleteArray_NicaFemtoImaging1D);
      instance.SetDestructor(&destruct_NicaFemtoImaging1D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoImaging1D*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoImaging1D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoImaging1D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaFemtoImaging3D_Dictionary();
   static void NicaFemtoImaging3D_TClassManip(TClass*);
   static void *new_NicaFemtoImaging3D(void *p = nullptr);
   static void *newArray_NicaFemtoImaging3D(Long_t size, void *p);
   static void delete_NicaFemtoImaging3D(void *p);
   static void deleteArray_NicaFemtoImaging3D(void *p);
   static void destruct_NicaFemtoImaging3D(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoImaging3D*)
   {
      ::NicaFemtoImaging3D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::NicaFemtoImaging3D));
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoImaging3D", "NicaFemtoImaging3D.h", 15,
                  typeid(::NicaFemtoImaging3D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaFemtoImaging3D_Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoImaging3D) );
      instance.SetNew(&new_NicaFemtoImaging3D);
      instance.SetNewArray(&newArray_NicaFemtoImaging3D);
      instance.SetDelete(&delete_NicaFemtoImaging3D);
      instance.SetDeleteArray(&deleteArray_NicaFemtoImaging3D);
      instance.SetDestructor(&destruct_NicaFemtoImaging3D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoImaging3D*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoImaging3D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoImaging3D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaFemtoImaging3D_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoImaging3D*)nullptr)->GetClass();
      NicaFemtoImaging3D_TClassManip(theClass);
   return theClass;
   }

   static void NicaFemtoImaging3D_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoSHCF(void *p = nullptr);
   static void *newArray_NicaFemtoSHCF(Long_t size, void *p);
   static void delete_NicaFemtoSHCF(void *p);
   static void deleteArray_NicaFemtoSHCF(void *p);
   static void destruct_NicaFemtoSHCF(void *p);
   static Long64_t merge_NicaFemtoSHCF(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoSHCF*)
   {
      ::NicaFemtoSHCF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoSHCF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoSHCF", ::NicaFemtoSHCF::Class_Version(), "NicaFemtoSHCF.h", 25,
                  typeid(::NicaFemtoSHCF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoSHCF::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoSHCF) );
      instance.SetNew(&new_NicaFemtoSHCF);
      instance.SetNewArray(&newArray_NicaFemtoSHCF);
      instance.SetDelete(&delete_NicaFemtoSHCF);
      instance.SetDeleteArray(&deleteArray_NicaFemtoSHCF);
      instance.SetDestructor(&destruct_NicaFemtoSHCF);
      instance.SetMerge(&merge_NicaFemtoSHCF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoSHCF*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoSHCF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoSHCF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFemtoYlm(void *p = nullptr);
   static void *newArray_NicaFemtoYlm(Long_t size, void *p);
   static void delete_NicaFemtoYlm(void *p);
   static void deleteArray_NicaFemtoYlm(void *p);
   static void destruct_NicaFemtoYlm(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFemtoYlm*)
   {
      ::NicaFemtoYlm *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFemtoYlm >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFemtoYlm", ::NicaFemtoYlm::Class_Version(), "NicaFemtoYlm.h", 17,
                  typeid(::NicaFemtoYlm), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFemtoYlm::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFemtoYlm) );
      instance.SetNew(&new_NicaFemtoYlm);
      instance.SetNewArray(&newArray_NicaFemtoYlm);
      instance.SetDelete(&delete_NicaFemtoYlm);
      instance.SetDeleteArray(&deleteArray_NicaFemtoYlm);
      instance.SetDestructor(&destruct_NicaFemtoYlm);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFemtoYlm*)
   {
      return GenerateInitInstanceLocal((::NicaFemtoYlm*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFemtoYlm*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitSHCF(void *p = nullptr);
   static void *newArray_NicaCorrFitSHCF(Long_t size, void *p);
   static void delete_NicaCorrFitSHCF(void *p);
   static void deleteArray_NicaCorrFitSHCF(void *p);
   static void destruct_NicaCorrFitSHCF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitSHCF*)
   {
      ::NicaCorrFitSHCF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitSHCF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitSHCF", ::NicaCorrFitSHCF::Class_Version(), "NicaCorrFitSHCF.h", 20,
                  typeid(::NicaCorrFitSHCF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitSHCF::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitSHCF) );
      instance.SetNew(&new_NicaCorrFitSHCF);
      instance.SetNewArray(&newArray_NicaCorrFitSHCF);
      instance.SetDelete(&delete_NicaCorrFitSHCF);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitSHCF);
      instance.SetDestructor(&destruct_NicaCorrFitSHCF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitSHCF*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitSHCF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitSHCF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitSmear1DCF(void *p = nullptr);
   static void *newArray_NicaCorrFitSmear1DCF(Long_t size, void *p);
   static void delete_NicaCorrFitSmear1DCF(void *p);
   static void deleteArray_NicaCorrFitSmear1DCF(void *p);
   static void destruct_NicaCorrFitSmear1DCF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitSmear1DCF*)
   {
      ::NicaCorrFitSmear1DCF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitSmear1DCF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitSmear1DCF", ::NicaCorrFitSmear1DCF::Class_Version(), "NicaCorrFitSmear1DCF.h", 15,
                  typeid(::NicaCorrFitSmear1DCF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitSmear1DCF::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitSmear1DCF) );
      instance.SetNew(&new_NicaCorrFitSmear1DCF);
      instance.SetNewArray(&newArray_NicaCorrFitSmear1DCF);
      instance.SetDelete(&delete_NicaCorrFitSmear1DCF);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitSmear1DCF);
      instance.SetDestructor(&destruct_NicaCorrFitSmear1DCF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitSmear1DCF*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitSmear1DCF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitSmear1DCF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitDbParams(void *p = nullptr);
   static void *newArray_NicaCorrFitDbParams(Long_t size, void *p);
   static void delete_NicaCorrFitDbParams(void *p);
   static void deleteArray_NicaCorrFitDbParams(void *p);
   static void destruct_NicaCorrFitDbParams(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitDbParams*)
   {
      ::NicaCorrFitDbParams *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitDbParams >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitDbParams", ::NicaCorrFitDbParams::Class_Version(), "NicaCorrFitDatabase.h", 24,
                  typeid(::NicaCorrFitDbParams), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitDbParams::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitDbParams) );
      instance.SetNew(&new_NicaCorrFitDbParams);
      instance.SetNewArray(&newArray_NicaCorrFitDbParams);
      instance.SetDelete(&delete_NicaCorrFitDbParams);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitDbParams);
      instance.SetDestructor(&destruct_NicaCorrFitDbParams);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitDbParams*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitDbParams*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitDbParams*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitDbParamsSetup(void *p = nullptr);
   static void *newArray_NicaCorrFitDbParamsSetup(Long_t size, void *p);
   static void delete_NicaCorrFitDbParamsSetup(void *p);
   static void deleteArray_NicaCorrFitDbParamsSetup(void *p);
   static void destruct_NicaCorrFitDbParamsSetup(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitDbParamsSetup*)
   {
      ::NicaCorrFitDbParamsSetup *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitDbParamsSetup >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitDbParamsSetup", ::NicaCorrFitDbParamsSetup::Class_Version(), "NicaCorrFitDatabase.h", 47,
                  typeid(::NicaCorrFitDbParamsSetup), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitDbParamsSetup::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitDbParamsSetup) );
      instance.SetNew(&new_NicaCorrFitDbParamsSetup);
      instance.SetNewArray(&newArray_NicaCorrFitDbParamsSetup);
      instance.SetDelete(&delete_NicaCorrFitDbParamsSetup);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitDbParamsSetup);
      instance.SetDestructor(&destruct_NicaCorrFitDbParamsSetup);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitDbParamsSetup*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitDbParamsSetup*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitDbParamsSetup*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitDatabase(void *p = nullptr);
   static void *newArray_NicaCorrFitDatabase(Long_t size, void *p);
   static void delete_NicaCorrFitDatabase(void *p);
   static void deleteArray_NicaCorrFitDatabase(void *p);
   static void destruct_NicaCorrFitDatabase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitDatabase*)
   {
      ::NicaCorrFitDatabase *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitDatabase >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitDatabase", ::NicaCorrFitDatabase::Class_Version(), "NicaCorrFitDatabase.h", 74,
                  typeid(::NicaCorrFitDatabase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitDatabase::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitDatabase) );
      instance.SetNew(&new_NicaCorrFitDatabase);
      instance.SetNewArray(&newArray_NicaCorrFitDatabase);
      instance.SetDelete(&delete_NicaCorrFitDatabase);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitDatabase);
      instance.SetDestructor(&destruct_NicaCorrFitDatabase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitDatabase*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitDatabase*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitDatabase*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitDumpedPairAna(void *p = nullptr);
   static void *newArray_NicaCorrFitDumpedPairAna(Long_t size, void *p);
   static void delete_NicaCorrFitDumpedPairAna(void *p);
   static void deleteArray_NicaCorrFitDumpedPairAna(void *p);
   static void destruct_NicaCorrFitDumpedPairAna(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitDumpedPairAna*)
   {
      ::NicaCorrFitDumpedPairAna *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitDumpedPairAna >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitDumpedPairAna", ::NicaCorrFitDumpedPairAna::Class_Version(), "NicaCorrFitDumpedPairAna.h", 37,
                  typeid(::NicaCorrFitDumpedPairAna), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitDumpedPairAna::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitDumpedPairAna) );
      instance.SetNew(&new_NicaCorrFitDumpedPairAna);
      instance.SetNewArray(&newArray_NicaCorrFitDumpedPairAna);
      instance.SetDelete(&delete_NicaCorrFitDumpedPairAna);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitDumpedPairAna);
      instance.SetDestructor(&destruct_NicaCorrFitDumpedPairAna);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitDumpedPairAna*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitDumpedPairAna*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitDumpedPairAna*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitInfo(void *p = nullptr);
   static void *newArray_NicaCorrFitInfo(Long_t size, void *p);
   static void delete_NicaCorrFitInfo(void *p);
   static void deleteArray_NicaCorrFitInfo(void *p);
   static void destruct_NicaCorrFitInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitInfo*)
   {
      ::NicaCorrFitInfo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitInfo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitInfo", ::NicaCorrFitInfo::Class_Version(), "NicaCorrFitInfo.h", 20,
                  typeid(::NicaCorrFitInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitInfo::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitInfo) );
      instance.SetNew(&new_NicaCorrFitInfo);
      instance.SetNewArray(&newArray_NicaCorrFitInfo);
      instance.SetDelete(&delete_NicaCorrFitInfo);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitInfo);
      instance.SetDestructor(&destruct_NicaCorrFitInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitInfo*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitInfo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitInfo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFit3DCFMultiDim(void *p = nullptr);
   static void *newArray_NicaCorrFit3DCFMultiDim(Long_t size, void *p);
   static void delete_NicaCorrFit3DCFMultiDim(void *p);
   static void deleteArray_NicaCorrFit3DCFMultiDim(void *p);
   static void destruct_NicaCorrFit3DCFMultiDim(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFit3DCFMultiDim*)
   {
      ::NicaCorrFit3DCFMultiDim *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFit3DCFMultiDim >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFit3DCFMultiDim", ::NicaCorrFit3DCFMultiDim::Class_Version(), "NicaCorrFit3DCFMultiDim.h", 18,
                  typeid(::NicaCorrFit3DCFMultiDim), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFit3DCFMultiDim::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFit3DCFMultiDim) );
      instance.SetNew(&new_NicaCorrFit3DCFMultiDim);
      instance.SetNewArray(&newArray_NicaCorrFit3DCFMultiDim);
      instance.SetDelete(&delete_NicaCorrFit3DCFMultiDim);
      instance.SetDeleteArray(&deleteArray_NicaCorrFit3DCFMultiDim);
      instance.SetDestructor(&destruct_NicaCorrFit3DCFMultiDim);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFit3DCFMultiDim*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFit3DCFMultiDim*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFit3DCFMultiDim*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitFunctor(void *p = nullptr);
   static void *newArray_NicaCorrFitFunctor(Long_t size, void *p);
   static void delete_NicaCorrFitFunctor(void *p);
   static void deleteArray_NicaCorrFitFunctor(void *p);
   static void destruct_NicaCorrFitFunctor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitFunctor*)
   {
      ::NicaCorrFitFunctor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitFunctor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitFunctor", ::NicaCorrFitFunctor::Class_Version(), "NicaCorrFitFunctor.h", 24,
                  typeid(::NicaCorrFitFunctor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitFunctor::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitFunctor) );
      instance.SetNew(&new_NicaCorrFitFunctor);
      instance.SetNewArray(&newArray_NicaCorrFitFunctor);
      instance.SetDelete(&delete_NicaCorrFitFunctor);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitFunctor);
      instance.SetDestructor(&destruct_NicaCorrFitFunctor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitFunctor*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitFunctor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitFunctor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitPairGeneratorYPt(void *p = nullptr);
   static void *newArray_NicaCorrFitPairGeneratorYPt(Long_t size, void *p);
   static void delete_NicaCorrFitPairGeneratorYPt(void *p);
   static void deleteArray_NicaCorrFitPairGeneratorYPt(void *p);
   static void destruct_NicaCorrFitPairGeneratorYPt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitPairGeneratorYPt*)
   {
      ::NicaCorrFitPairGeneratorYPt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitPairGeneratorYPt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitPairGeneratorYPt", ::NicaCorrFitPairGeneratorYPt::Class_Version(), "NicaCorrFitPairGeneratorYPt.h", 25,
                  typeid(::NicaCorrFitPairGeneratorYPt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitPairGeneratorYPt::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitPairGeneratorYPt) );
      instance.SetNew(&new_NicaCorrFitPairGeneratorYPt);
      instance.SetNewArray(&newArray_NicaCorrFitPairGeneratorYPt);
      instance.SetDelete(&delete_NicaCorrFitPairGeneratorYPt);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitPairGeneratorYPt);
      instance.SetDestructor(&destruct_NicaCorrFitPairGeneratorYPt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitPairGeneratorYPt*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitPairGeneratorYPt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitPairGeneratorYPt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaCorrFitMapRebin(void *p = nullptr);
   static void *newArray_NicaCorrFitMapRebin(Long_t size, void *p);
   static void delete_NicaCorrFitMapRebin(void *p);
   static void deleteArray_NicaCorrFitMapRebin(void *p);
   static void destruct_NicaCorrFitMapRebin(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCorrFitMapRebin*)
   {
      ::NicaCorrFitMapRebin *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCorrFitMapRebin >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCorrFitMapRebin", ::NicaCorrFitMapRebin::Class_Version(), "NicaCorrFitMapRebin.h", 25,
                  typeid(::NicaCorrFitMapRebin), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCorrFitMapRebin::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCorrFitMapRebin) );
      instance.SetNew(&new_NicaCorrFitMapRebin);
      instance.SetNewArray(&newArray_NicaCorrFitMapRebin);
      instance.SetDelete(&delete_NicaCorrFitMapRebin);
      instance.SetDeleteArray(&deleteArray_NicaCorrFitMapRebin);
      instance.SetDestructor(&destruct_NicaCorrFitMapRebin);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCorrFitMapRebin*)
   {
      return GenerateInitInstanceLocal((::NicaCorrFitMapRebin*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCorrFitMapRebin*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoCorrFunc::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoCorrFunc::Class_Name()
{
   return "NicaFemtoCorrFunc";
}

//______________________________________________________________________________
const char *NicaFemtoCorrFunc::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoCorrFunc::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFunc::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFunc::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoCorrFunc1D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoCorrFunc1D::Class_Name()
{
   return "NicaFemtoCorrFunc1D";
}

//______________________________________________________________________________
const char *NicaFemtoCorrFunc1D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc1D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoCorrFunc1D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc1D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFunc1D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc1D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFunc1D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc1D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoCorrFunc2D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoCorrFunc2D::Class_Name()
{
   return "NicaFemtoCorrFunc2D";
}

//______________________________________________________________________________
const char *NicaFemtoCorrFunc2D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc2D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoCorrFunc2D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc2D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFunc2D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc2D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFunc2D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFunc2D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoSourceModel::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoSourceModel::Class_Name()
{
   return "NicaFemtoSourceModel";
}

//______________________________________________________________________________
const char *NicaFemtoSourceModel::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModel*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoSourceModel::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModel*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoSourceModel::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModel*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoSourceModel::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModel*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoSourceModel1D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoSourceModel1D::Class_Name()
{
   return "NicaFemtoSourceModel1D";
}

//______________________________________________________________________________
const char *NicaFemtoSourceModel1D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModel1D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoSourceModel1D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModel1D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoSourceModel1D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModel1D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoSourceModel1D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModel1D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoSourceModel3D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoSourceModel3D::Class_Name()
{
   return "NicaFemtoSourceModel3D";
}

//______________________________________________________________________________
const char *NicaFemtoSourceModel3D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModel3D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoSourceModel3D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModel3D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoSourceModel3D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModel3D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoSourceModel3D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModel3D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoFreezoutGenerator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoFreezoutGenerator::Class_Name()
{
   return "NicaFemtoFreezoutGenerator";
}

//______________________________________________________________________________
const char *NicaFemtoFreezoutGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGenerator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoFreezoutGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGenerator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoFreezoutGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGenerator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoFreezoutGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGenerator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoFreezoutGeneratorLCMS::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoFreezoutGeneratorLCMS::Class_Name()
{
   return "NicaFemtoFreezoutGeneratorLCMS";
}

//______________________________________________________________________________
const char *NicaFemtoFreezoutGeneratorLCMS::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGeneratorLCMS*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoFreezoutGeneratorLCMS::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGeneratorLCMS*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoFreezoutGeneratorLCMS::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGeneratorLCMS*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoFreezoutGeneratorLCMS::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGeneratorLCMS*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoFreezoutGeneratorPRF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoFreezoutGeneratorPRF::Class_Name()
{
   return "NicaFemtoFreezoutGeneratorPRF";
}

//______________________________________________________________________________
const char *NicaFemtoFreezoutGeneratorPRF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGeneratorPRF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoFreezoutGeneratorPRF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGeneratorPRF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoFreezoutGeneratorPRF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGeneratorPRF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoFreezoutGeneratorPRF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutGeneratorPRF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoWeightGenerator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoWeightGenerator::Class_Name()
{
   return "NicaFemtoWeightGenerator";
}

//______________________________________________________________________________
const char *NicaFemtoWeightGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGenerator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoWeightGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGenerator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoWeightGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGenerator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoWeightGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGenerator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoBasicAna::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoBasicAna::Class_Name()
{
   return "NicaFemtoBasicAna";
}

//______________________________________________________________________________
const char *NicaFemtoBasicAna::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoBasicAna*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoBasicAna::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoBasicAna*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoBasicAna::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoBasicAna*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoBasicAna::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoBasicAna*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoFastCut::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoFastCut::Class_Name()
{
   return "NicaFemtoFastCut";
}

//______________________________________________________________________________
const char *NicaFemtoFastCut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCut*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoFastCut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCut*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoFastCut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCut*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoFastCut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCut*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoFastCutVirtual::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoFastCutVirtual::Class_Name()
{
   return "NicaFemtoFastCutVirtual";
}

//______________________________________________________________________________
const char *NicaFemtoFastCutVirtual::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutVirtual*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoFastCutVirtual::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutVirtual*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoFastCutVirtual::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutVirtual*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoFastCutVirtual::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutVirtual*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoFastCutKt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoFastCutKt::Class_Name()
{
   return "NicaFemtoFastCutKt";
}

//______________________________________________________________________________
const char *NicaFemtoFastCutKt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutKt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoFastCutKt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutKt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoFastCutKt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutKt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoFastCutKt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutKt*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoFastCutPtSum::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoFastCutPtSum::Class_Name()
{
   return "NicaFemtoFastCutPtSum";
}

//______________________________________________________________________________
const char *NicaFemtoFastCutPtSum::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutPtSum*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoFastCutPtSum::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutPtSum*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoFastCutPtSum::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutPtSum*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoFastCutPtSum::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutPtSum*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoFastCutPhi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoFastCutPhi::Class_Name()
{
   return "NicaFemtoFastCutPhi";
}

//______________________________________________________________________________
const char *NicaFemtoFastCutPhi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutPhi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoFastCutPhi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutPhi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoFastCutPhi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutPhi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoFastCutPhi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutPhi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoFastCutEta::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoFastCutEta::Class_Name()
{
   return "NicaFemtoFastCutEta";
}

//______________________________________________________________________________
const char *NicaFemtoFastCutEta::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutEta*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoFastCutEta::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutEta*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoFastCutEta::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutEta*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoFastCutEta::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFastCutEta*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoFreezoutsAna::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoFreezoutsAna::Class_Name()
{
   return "NicaFemtoFreezoutsAna";
}

//______________________________________________________________________________
const char *NicaFemtoFreezoutsAna::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutsAna*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoFreezoutsAna::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutsAna*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoFreezoutsAna::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutsAna*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoFreezoutsAna::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFreezoutsAna*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoPair::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoPair::Class_Name()
{
   return "NicaFemtoPair";
}

//______________________________________________________________________________
const char *NicaFemtoPair::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPair*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoPair::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPair*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoPair::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPair*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoPair::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPair*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoPicoPair::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoPicoPair::Class_Name()
{
   return "NicaFemtoPicoPair";
}

//______________________________________________________________________________
const char *NicaFemtoPicoPair::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPicoPair*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoPicoPair::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPicoPair*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoPicoPair::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPicoPair*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoPicoPair::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPicoPair*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoMicroPair::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoMicroPair::Class_Name()
{
   return "NicaFemtoMicroPair";
}

//______________________________________________________________________________
const char *NicaFemtoMicroPair::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoMicroPair*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoMicroPair::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoMicroPair*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoMicroPair::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoMicroPair*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoMicroPair::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoMicroPair*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoMiniPair::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoMiniPair::Class_Name()
{
   return "NicaFemtoMiniPair";
}

//______________________________________________________________________________
const char *NicaFemtoMiniPair::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoMiniPair*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoMiniPair::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoMiniPair*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoMiniPair::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoMiniPair*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoMiniPair::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoMiniPair*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoDumpPairAna::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoDumpPairAna::Class_Name()
{
   return "NicaFemtoDumpPairAna";
}

//______________________________________________________________________________
const char *NicaFemtoDumpPairAna::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoDumpPairAna*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoDumpPairAna::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoDumpPairAna*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoDumpPairAna::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoDumpPairAna*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoDumpPairAna::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoDumpPairAna*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoEventBinsAna::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoEventBinsAna::Class_Name()
{
   return "NicaFemtoEventBinsAna";
}

//______________________________________________________________________________
const char *NicaFemtoEventBinsAna::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoEventBinsAna*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoEventBinsAna::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoEventBinsAna*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoEventBinsAna::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoEventBinsAna*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoEventBinsAna::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoEventBinsAna*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoDPhiDEta::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoDPhiDEta::Class_Name()
{
   return "NicaFemtoDPhiDEta";
}

//______________________________________________________________________________
const char *NicaFemtoDPhiDEta::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoDPhiDEta*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoDPhiDEta::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoDPhiDEta*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoDPhiDEta::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoDPhiDEta*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoDPhiDEta::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoDPhiDEta*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemto1DCF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemto1DCF::Class_Name()
{
   return "NicaFemto1DCF";
}

//______________________________________________________________________________
const char *NicaFemto1DCF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemto1DCF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemto1DCF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemto1DCF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemto3DCF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemto3DCF::Class_Name()
{
   return "NicaFemto3DCF";
}

//______________________________________________________________________________
const char *NicaFemto3DCF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto3DCF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemto3DCF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto3DCF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemto3DCF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto3DCF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemto3DCF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto3DCF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemto3DCFQinv::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemto3DCFQinv::Class_Name()
{
   return "NicaFemto3DCFQinv";
}

//______________________________________________________________________________
const char *NicaFemto3DCFQinv::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto3DCFQinv*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemto3DCFQinv::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto3DCFQinv*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemto3DCFQinv::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto3DCFQinv*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemto3DCFQinv::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto3DCFQinv*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoPairPRF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoPairPRF::Class_Name()
{
   return "NicaFemtoPairPRF";
}

//______________________________________________________________________________
const char *NicaFemtoPairPRF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairPRF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoPairPRF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairPRF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoPairPRF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairPRF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoPairPRF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairPRF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoPair_SH::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoPair_SH::Class_Name()
{
   return "NicaFemtoPair_SH";
}

//______________________________________________________________________________
const char *NicaFemtoPair_SH::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPair_SH*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoPair_SH::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPair_SH*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoPair_SH::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPair_SH*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoPair_SH::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPair_SH*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoPairLCMS::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoPairLCMS::Class_Name()
{
   return "NicaFemtoPairLCMS";
}

//______________________________________________________________________________
const char *NicaFemtoPairLCMS::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairLCMS*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoPairLCMS::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairLCMS*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoPairLCMS::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairLCMS*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoPairLCMS::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairLCMS*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoPairPRF_SH::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoPairPRF_SH::Class_Name()
{
   return "NicaFemtoPairPRF_SH";
}

//______________________________________________________________________________
const char *NicaFemtoPairPRF_SH::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairPRF_SH*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoPairPRF_SH::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairPRF_SH*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoPairPRF_SH::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairPRF_SH*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoPairPRF_SH::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairPRF_SH*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoPairLCMS_SH::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoPairLCMS_SH::Class_Name()
{
   return "NicaFemtoPairLCMS_SH";
}

//______________________________________________________________________________
const char *NicaFemtoPairLCMS_SH::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairLCMS_SH*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoPairLCMS_SH::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairLCMS_SH*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoPairLCMS_SH::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairLCMS_SH*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoPairLCMS_SH::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairLCMS_SH*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoPairDPhiDEta::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoPairDPhiDEta::Class_Name()
{
   return "NicaFemtoPairDPhiDEta";
}

//______________________________________________________________________________
const char *NicaFemtoPairDPhiDEta::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairDPhiDEta*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoPairDPhiDEta::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairDPhiDEta*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoPairDPhiDEta::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairDPhiDEta*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoPairDPhiDEta::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairDPhiDEta*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoPairPRFL::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoPairPRFL::Class_Name()
{
   return "NicaFemtoPairPRFL";
}

//______________________________________________________________________________
const char *NicaFemtoPairPRFL::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairPRFL*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoPairPRFL::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairPRFL*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoPairPRFL::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairPRFL*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoPairPRFL::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoPairPRFL*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoSourceModelExp1D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoSourceModelExp1D::Class_Name()
{
   return "NicaFemtoSourceModelExp1D";
}

//______________________________________________________________________________
const char *NicaFemtoSourceModelExp1D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelExp1D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoSourceModelExp1D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelExp1D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoSourceModelExp1D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelExp1D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoSourceModelExp1D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelExp1D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoSourceModelGauss1D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoSourceModelGauss1D::Class_Name()
{
   return "NicaFemtoSourceModelGauss1D";
}

//______________________________________________________________________________
const char *NicaFemtoSourceModelGauss1D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGauss1D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoSourceModelGauss1D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGauss1D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoSourceModelGauss1D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGauss1D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoSourceModelGauss1D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGauss1D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoSourceModelGauss3D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoSourceModelGauss3D::Class_Name()
{
   return "NicaFemtoSourceModelGauss3D";
}

//______________________________________________________________________________
const char *NicaFemtoSourceModelGauss3D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGauss3D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoSourceModelGauss3D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGauss3D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoSourceModelGauss3D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGauss3D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoSourceModelGauss3D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGauss3D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoSourceModelGausMt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoSourceModelGausMt::Class_Name()
{
   return "NicaFemtoSourceModelGausMt";
}

//______________________________________________________________________________
const char *NicaFemtoSourceModelGausMt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGausMt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoSourceModelGausMt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGausMt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoSourceModelGausMt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGausMt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoSourceModelGausMt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSourceModelGausMt*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit::Class_Name()
{
   return "NicaCorrFit";
}

//______________________________________________________________________________
const char *NicaCorrFit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitFunc::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitFunc::Class_Name()
{
   return "NicaCorrFitFunc";
}

//______________________________________________________________________________
const char *NicaCorrFitFunc::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitFunc*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitFunc::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitFunc*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitFunc::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitFunc*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitFunc::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitFunc*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit1DCF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit1DCF::Class_Name()
{
   return "NicaCorrFit1DCF";
}

//______________________________________________________________________________
const char *NicaCorrFit1DCF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit1DCF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitMath1DCF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitMath1DCF::Class_Name()
{
   return "NicaCorrFitMath1DCF";
}

//______________________________________________________________________________
const char *NicaCorrFitMath1DCF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitMath1DCF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitMath1DCF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitMath1DCF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitMath1DCF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitMath1DCF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitMath1DCF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitMath1DCF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit1DCF_Sum::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Sum::Class_Name()
{
   return "NicaCorrFit1DCF_Sum";
}

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Sum::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Sum*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit1DCF_Sum::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Sum*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Sum::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Sum*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Sum::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Sum*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit1DCF_SumRsame::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit1DCF_SumRsame::Class_Name()
{
   return "NicaCorrFit1DCF_SumRsame";
}

//______________________________________________________________________________
const char *NicaCorrFit1DCF_SumRsame::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_SumRsame*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit1DCF_SumRsame::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_SumRsame*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_SumRsame::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_SumRsame*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_SumRsame::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_SumRsame*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit1DCF_Multi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Multi::Class_Name()
{
   return "NicaCorrFit1DCF_Multi";
}

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Multi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Multi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit1DCF_Multi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Multi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Multi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Multi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Multi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Multi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit3DCF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit3DCF::Class_Name()
{
   return "NicaCorrFit3DCF";
}

//______________________________________________________________________________
const char *NicaCorrFit3DCF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit3DCF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitMath3DCF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitMath3DCF::Class_Name()
{
   return "NicaCorrFitMath3DCF";
}

//______________________________________________________________________________
const char *NicaCorrFitMath3DCF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitMath3DCF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitMath3DCF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitMath3DCF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitMath3DCF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitMath3DCF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitMath3DCF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitMath3DCF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit3DCF_Sum::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit3DCF_Sum::Class_Name()
{
   return "NicaCorrFit3DCF_Sum";
}

//______________________________________________________________________________
const char *NicaCorrFit3DCF_Sum::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Sum*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit3DCF_Sum::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Sum*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCF_Sum::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Sum*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCF_Sum::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Sum*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitKisiel::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitKisiel::Class_Name()
{
   return "NicaCorrFitKisiel";
}

//______________________________________________________________________________
const char *NicaCorrFitKisiel::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitKisiel*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitKisiel::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitKisiel*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitKisiel::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitKisiel*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitKisiel::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitKisiel*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitWielanek::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitWielanek::Class_Name()
{
   return "NicaCorrFitWielanek";
}

//______________________________________________________________________________
const char *NicaCorrFitWielanek::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitWielanek*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitWielanek::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitWielanek*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitWielanek::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitWielanek*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitWielanek::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitWielanek*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitParButton::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitParButton::Class_Name()
{
   return "NicaCorrFitParButton";
}

//______________________________________________________________________________
const char *NicaCorrFitParButton::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitParButton*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitParButton::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitParButton*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitParButton::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitParButton*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitParButton::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitParButton*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitChiSelector::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitChiSelector::Class_Name()
{
   return "NicaCorrFitChiSelector";
}

//______________________________________________________________________________
const char *NicaCorrFitChiSelector::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitChiSelector*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitChiSelector::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitChiSelector*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitChiSelector::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitChiSelector*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitChiSelector::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitChiSelector*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitGUI::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitGUI::Class_Name()
{
   return "NicaCorrFitGUI";
}

//______________________________________________________________________________
const char *NicaCorrFitGUI::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitGUI*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitGUI::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitGUI*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitGUI::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitGUI*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitGUI::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitGUI*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitHDFunc::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitHDFunc::Class_Name()
{
   return "NicaCorrFitHDFunc";
}

//______________________________________________________________________________
const char *NicaCorrFitHDFunc::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitHDFunc::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitHDFunc::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitHDFunc::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitHDFunc1D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitHDFunc1D::Class_Name()
{
   return "NicaCorrFitHDFunc1D";
}

//______________________________________________________________________________
const char *NicaCorrFitHDFunc1D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc1D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitHDFunc1D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc1D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitHDFunc1D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc1D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitHDFunc1D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc1D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitHDFunc3D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitHDFunc3D::Class_Name()
{
   return "NicaCorrFitHDFunc3D";
}

//______________________________________________________________________________
const char *NicaCorrFitHDFunc3D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc3D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitHDFunc3D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc3D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitHDFunc3D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc3D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitHDFunc3D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitHDFunc3D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoCorrFuncKtMap1D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoCorrFuncKtMap1D::Class_Name()
{
   return "NicaFemtoCorrFuncKtMap1D";
}

//______________________________________________________________________________
const char *NicaFemtoCorrFuncKtMap1D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncKtMap1D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoCorrFuncKtMap1D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncKtMap1D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFuncKtMap1D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncKtMap1D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFuncKtMap1D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncKtMap1D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemto1DCFAnaMap::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemto1DCFAnaMap::Class_Name()
{
   return "NicaFemto1DCFAnaMap";
}

//______________________________________________________________________________
const char *NicaFemto1DCFAnaMap::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMap*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemto1DCFAnaMap::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMap*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemto1DCFAnaMap::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMap*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemto1DCFAnaMap::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMap*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemto1DCFAnaMapMC::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemto1DCFAnaMapMC::Class_Name()
{
   return "NicaFemto1DCFAnaMapMC";
}

//______________________________________________________________________________
const char *NicaFemto1DCFAnaMapMC::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMapMC*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemto1DCFAnaMapMC::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMapMC*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemto1DCFAnaMapMC::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMapMC*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemto1DCFAnaMapMC::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMapMC*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaWielanekMapGenerator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaWielanekMapGenerator::Class_Name()
{
   return "NicaWielanekMapGenerator";
}

//______________________________________________________________________________
const char *NicaWielanekMapGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaWielanekMapGenerator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaWielanekMapGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaWielanekMapGenerator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaWielanekMapGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaWielanekMapGenerator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaWielanekMapGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaWielanekMapGenerator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemto1DCFAnaMapPairsDumped::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemto1DCFAnaMapPairsDumped::Class_Name()
{
   return "NicaFemto1DCFAnaMapPairsDumped";
}

//______________________________________________________________________________
const char *NicaFemto1DCFAnaMapPairsDumped::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMapPairsDumped*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemto1DCFAnaMapPairsDumped::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMapPairsDumped*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemto1DCFAnaMapPairsDumped::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMapPairsDumped*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemto1DCFAnaMapPairsDumped::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemto1DCFAnaMapPairsDumped*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit1DCF_Square::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Square::Class_Name()
{
   return "NicaCorrFit1DCF_Square";
}

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Square::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Square*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit1DCF_Square::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Square*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Square::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Square*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Square::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Square*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit1DCF_Lorentz::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Lorentz::Class_Name()
{
   return "NicaCorrFit1DCF_Lorentz";
}

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Lorentz::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Lorentz*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit1DCF_Lorentz::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Lorentz*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Lorentz::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Lorentz*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Lorentz::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Lorentz*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit3DCF_Square::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit3DCF_Square::Class_Name()
{
   return "NicaCorrFit3DCF_Square";
}

//______________________________________________________________________________
const char *NicaCorrFit3DCF_Square::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Square*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit3DCF_Square::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Square*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCF_Square::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Square*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCF_Square::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Square*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit3DCF_Lorentz::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit3DCF_Lorentz::Class_Name()
{
   return "NicaCorrFit3DCF_Lorentz";
}

//______________________________________________________________________________
const char *NicaCorrFit3DCF_Lorentz::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Lorentz*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit3DCF_Lorentz::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Lorentz*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCF_Lorentz::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Lorentz*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCF_Lorentz::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Lorentz*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit1DCF_Exp::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Exp::Class_Name()
{
   return "NicaCorrFit1DCF_Exp";
}

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Exp::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Exp*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit1DCF_Exp::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Exp*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Exp::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Exp*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Exp::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Exp*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit1DCF_Gauss::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Gauss::Class_Name()
{
   return "NicaCorrFit1DCF_Gauss";
}

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Gauss::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Gauss*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit1DCF_Gauss::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Gauss*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Gauss::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Gauss*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Gauss::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Gauss*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit1DCF_Poly::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Poly::Class_Name()
{
   return "NicaCorrFit1DCF_Poly";
}

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Poly::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Poly*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit1DCF_Poly::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Poly*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Poly::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Poly*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Poly::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Poly*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit1DCF_Poly2::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Poly2::Class_Name()
{
   return "NicaCorrFit1DCF_Poly2";
}

//______________________________________________________________________________
const char *NicaCorrFit1DCF_Poly2::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Poly2*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit1DCF_Poly2::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Poly2*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Poly2::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Poly2*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCF_Poly2::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCF_Poly2*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit3DCF_Gauss::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit3DCF_Gauss::Class_Name()
{
   return "NicaCorrFit3DCF_Gauss";
}

//______________________________________________________________________________
const char *NicaCorrFit3DCF_Gauss::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Gauss*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit3DCF_Gauss::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Gauss*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCF_Gauss::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Gauss*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCF_Gauss::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Gauss*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit3DCF_Gauss2::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit3DCF_Gauss2::Class_Name()
{
   return "NicaCorrFit3DCF_Gauss2";
}

//______________________________________________________________________________
const char *NicaCorrFit3DCF_Gauss2::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Gauss2*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit3DCF_Gauss2::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Gauss2*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCF_Gauss2::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Gauss2*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCF_Gauss2::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_Gauss2*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit3DCF_GaussCross::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit3DCF_GaussCross::Class_Name()
{
   return "NicaCorrFit3DCF_GaussCross";
}

//______________________________________________________________________________
const char *NicaCorrFit3DCF_GaussCross::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_GaussCross*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit3DCF_GaussCross::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_GaussCross*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCF_GaussCross::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_GaussCross*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCF_GaussCross::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCF_GaussCross*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit1DCFCumac::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit1DCFCumac::Class_Name()
{
   return "NicaCorrFit1DCFCumac";
}

//______________________________________________________________________________
const char *NicaCorrFit1DCFCumac::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumac*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit1DCFCumac::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumac*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCFCumac::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumac*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCFCumac::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumac*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFIt1DCFCumacLamLam::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFIt1DCFCumacLamLam::Class_Name()
{
   return "NicaCorrFIt1DCFCumacLamLam";
}

//______________________________________________________________________________
const char *NicaCorrFIt1DCFCumacLamLam::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFIt1DCFCumacLamLam*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFIt1DCFCumacLamLam::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFIt1DCFCumacLamLam*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFIt1DCFCumacLamLam::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFIt1DCFCumacLamLam*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFIt1DCFCumacLamLam::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFIt1DCFCumacLamLam*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit1DCFCumacPLam::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit1DCFCumacPLam::Class_Name()
{
   return "NicaCorrFit1DCFCumacPLam";
}

//______________________________________________________________________________
const char *NicaCorrFit1DCFCumacPLam::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumacPLam*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit1DCFCumacPLam::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumacPLam*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCFCumacPLam::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumacPLam*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCFCumacPLam::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumacPLam*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit1DCFCumacK0K0::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit1DCFCumacK0K0::Class_Name()
{
   return "NicaCorrFit1DCFCumacK0K0";
}

//______________________________________________________________________________
const char *NicaCorrFit1DCFCumacK0K0::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumacK0K0*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit1DCFCumacK0K0::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumacK0K0*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCFCumacK0K0::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumacK0K0*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit1DCFCumacK0K0::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit1DCFCumacK0K0*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit3DCFBowlerSinyukov::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit3DCFBowlerSinyukov::Class_Name()
{
   return "NicaCorrFit3DCFBowlerSinyukov";
}

//______________________________________________________________________________
const char *NicaCorrFit3DCFBowlerSinyukov::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukov*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit3DCFBowlerSinyukov::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukov*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCFBowlerSinyukov::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukov*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCFBowlerSinyukov::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukov*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit3DCFBowlerSinyukovEllipse::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit3DCFBowlerSinyukovEllipse::Class_Name()
{
   return "NicaCorrFit3DCFBowlerSinyukovEllipse";
}

//______________________________________________________________________________
const char *NicaCorrFit3DCFBowlerSinyukovEllipse::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukovEllipse*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit3DCFBowlerSinyukovEllipse::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukovEllipse*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCFBowlerSinyukovEllipse::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukovEllipse*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCFBowlerSinyukovEllipse::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukovEllipse*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit3DCFBowlerSinyukovClassic::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit3DCFBowlerSinyukovClassic::Class_Name()
{
   return "NicaCorrFit3DCFBowlerSinyukovClassic";
}

//______________________________________________________________________________
const char *NicaCorrFit3DCFBowlerSinyukovClassic::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukovClassic*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit3DCFBowlerSinyukovClassic::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukovClassic*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCFBowlerSinyukovClassic::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukovClassic*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCFBowlerSinyukovClassic::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFBowlerSinyukovClassic*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoFsiParsed::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoFsiParsed::Class_Name()
{
   return "NicaFemtoFsiParsed";
}

//______________________________________________________________________________
const char *NicaFemtoFsiParsed::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFsiParsed*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoFsiParsed::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFsiParsed*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoFsiParsed::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFsiParsed*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoFsiParsed::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoFsiParsed*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoWeightGeneratorLednicky2::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoWeightGeneratorLednicky2::Class_Name()
{
   return "NicaFemtoWeightGeneratorLednicky2";
}

//______________________________________________________________________________
const char *NicaFemtoWeightGeneratorLednicky2::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednicky2*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoWeightGeneratorLednicky2::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednicky2*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoWeightGeneratorLednicky2::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednicky2*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoWeightGeneratorLednicky2::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednicky2*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoWeightGeneratorBasic::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoWeightGeneratorBasic::Class_Name()
{
   return "NicaFemtoWeightGeneratorBasic";
}

//______________________________________________________________________________
const char *NicaFemtoWeightGeneratorBasic::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorBasic*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoWeightGeneratorBasic::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorBasic*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoWeightGeneratorBasic::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorBasic*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoWeightGeneratorBasic::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorBasic*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoWeightGeneratorLednicky::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoWeightGeneratorLednicky::Class_Name()
{
   return "NicaFemtoWeightGeneratorLednicky";
}

//______________________________________________________________________________
const char *NicaFemtoWeightGeneratorLednicky::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednicky*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoWeightGeneratorLednicky::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednicky*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoWeightGeneratorLednicky::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednicky*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoWeightGeneratorLednicky::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednicky*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoWeightGeneratorLednickyResidual::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoWeightGeneratorLednickyResidual::Class_Name()
{
   return "NicaFemtoWeightGeneratorLednickyResidual";
}

//______________________________________________________________________________
const char *NicaFemtoWeightGeneratorLednickyResidual::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednickyResidual*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoWeightGeneratorLednickyResidual::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednickyResidual*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoWeightGeneratorLednickyResidual::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednickyResidual*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoWeightGeneratorLednickyResidual::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorLednickyResidual*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoWeightGeneratorSupressed::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoWeightGeneratorSupressed::Class_Name()
{
   return "NicaFemtoWeightGeneratorSupressed";
}

//______________________________________________________________________________
const char *NicaFemtoWeightGeneratorSupressed::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorSupressed*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoWeightGeneratorSupressed::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorSupressed*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoWeightGeneratorSupressed::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorSupressed*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoWeightGeneratorSupressed::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoWeightGeneratorSupressed*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoCorrFuncPt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoCorrFuncPt::Class_Name()
{
   return "NicaFemtoCorrFuncPt";
}

//______________________________________________________________________________
const char *NicaFemtoCorrFuncPt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncPt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoCorrFuncPt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncPt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFuncPt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncPt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFuncPt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncPt*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoCorrFuncKt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoCorrFuncKt::Class_Name()
{
   return "NicaFemtoCorrFuncKt";
}

//______________________________________________________________________________
const char *NicaFemtoCorrFuncKt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncKt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoCorrFuncKt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncKt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFuncKt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncKt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFuncKt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncKt*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoCorrFuncAzimuthalKt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoCorrFuncAzimuthalKt::Class_Name()
{
   return "NicaFemtoCorrFuncAzimuthalKt";
}

//______________________________________________________________________________
const char *NicaFemtoCorrFuncAzimuthalKt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncAzimuthalKt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoCorrFuncAzimuthalKt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncAzimuthalKt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFuncAzimuthalKt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncAzimuthalKt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFuncAzimuthalKt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncAzimuthalKt*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoCorrFuncSimple::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoCorrFuncSimple::Class_Name()
{
   return "NicaFemtoCorrFuncSimple";
}

//______________________________________________________________________________
const char *NicaFemtoCorrFuncSimple::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncSimple*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoCorrFuncSimple::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncSimple*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFuncSimple::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncSimple*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoCorrFuncSimple::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoCorrFuncSimple*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoImaging::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoImaging::Class_Name()
{
   return "NicaFemtoImaging";
}

//______________________________________________________________________________
const char *NicaFemtoImaging::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoImaging*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoImaging::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoImaging*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoImaging::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoImaging*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoImaging::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoImaging*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoImaging1D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoImaging1D::Class_Name()
{
   return "NicaFemtoImaging1D";
}

//______________________________________________________________________________
const char *NicaFemtoImaging1D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoImaging1D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoImaging1D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoImaging1D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoImaging1D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoImaging1D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoImaging1D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoImaging1D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoSHCF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoSHCF::Class_Name()
{
   return "NicaFemtoSHCF";
}

//______________________________________________________________________________
const char *NicaFemtoSHCF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSHCF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoSHCF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSHCF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoSHCF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSHCF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoSHCF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoSHCF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFemtoYlm::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFemtoYlm::Class_Name()
{
   return "NicaFemtoYlm";
}

//______________________________________________________________________________
const char *NicaFemtoYlm::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoYlm*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFemtoYlm::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoYlm*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFemtoYlm::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoYlm*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFemtoYlm::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFemtoYlm*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitSHCF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitSHCF::Class_Name()
{
   return "NicaCorrFitSHCF";
}

//______________________________________________________________________________
const char *NicaCorrFitSHCF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitSHCF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitSHCF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitSHCF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitSHCF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitSHCF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitSHCF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitSHCF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitSmear1DCF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitSmear1DCF::Class_Name()
{
   return "NicaCorrFitSmear1DCF";
}

//______________________________________________________________________________
const char *NicaCorrFitSmear1DCF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitSmear1DCF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitSmear1DCF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitSmear1DCF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitSmear1DCF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitSmear1DCF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitSmear1DCF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitSmear1DCF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitDbParams::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitDbParams::Class_Name()
{
   return "NicaCorrFitDbParams";
}

//______________________________________________________________________________
const char *NicaCorrFitDbParams::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDbParams*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitDbParams::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDbParams*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitDbParams::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDbParams*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitDbParams::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDbParams*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitDbParamsSetup::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitDbParamsSetup::Class_Name()
{
   return "NicaCorrFitDbParamsSetup";
}

//______________________________________________________________________________
const char *NicaCorrFitDbParamsSetup::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDbParamsSetup*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitDbParamsSetup::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDbParamsSetup*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitDbParamsSetup::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDbParamsSetup*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitDbParamsSetup::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDbParamsSetup*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitDatabase::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitDatabase::Class_Name()
{
   return "NicaCorrFitDatabase";
}

//______________________________________________________________________________
const char *NicaCorrFitDatabase::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDatabase*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitDatabase::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDatabase*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitDatabase::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDatabase*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitDatabase::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDatabase*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitDumpedPairAna::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitDumpedPairAna::Class_Name()
{
   return "NicaCorrFitDumpedPairAna";
}

//______________________________________________________________________________
const char *NicaCorrFitDumpedPairAna::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDumpedPairAna*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitDumpedPairAna::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDumpedPairAna*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitDumpedPairAna::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDumpedPairAna*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitDumpedPairAna::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitDumpedPairAna*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitInfo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitInfo::Class_Name()
{
   return "NicaCorrFitInfo";
}

//______________________________________________________________________________
const char *NicaCorrFitInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitInfo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitInfo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitInfo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitInfo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFit3DCFMultiDim::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFit3DCFMultiDim::Class_Name()
{
   return "NicaCorrFit3DCFMultiDim";
}

//______________________________________________________________________________
const char *NicaCorrFit3DCFMultiDim::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFMultiDim*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFit3DCFMultiDim::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFMultiDim*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCFMultiDim::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFMultiDim*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFit3DCFMultiDim::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFit3DCFMultiDim*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitFunctor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitFunctor::Class_Name()
{
   return "NicaCorrFitFunctor";
}

//______________________________________________________________________________
const char *NicaCorrFitFunctor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitFunctor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitFunctor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitFunctor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitFunctor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitFunctor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitFunctor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitFunctor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitPairGeneratorYPt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitPairGeneratorYPt::Class_Name()
{
   return "NicaCorrFitPairGeneratorYPt";
}

//______________________________________________________________________________
const char *NicaCorrFitPairGeneratorYPt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitPairGeneratorYPt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitPairGeneratorYPt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitPairGeneratorYPt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitPairGeneratorYPt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitPairGeneratorYPt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitPairGeneratorYPt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitPairGeneratorYPt*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaCorrFitMapRebin::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCorrFitMapRebin::Class_Name()
{
   return "NicaCorrFitMapRebin";
}

//______________________________________________________________________________
const char *NicaCorrFitMapRebin::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitMapRebin*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCorrFitMapRebin::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitMapRebin*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCorrFitMapRebin::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitMapRebin*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCorrFitMapRebin::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCorrFitMapRebin*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void NicaFemtoCorrFunc::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoCorrFunc.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoCorrFunc::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoCorrFunc::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaFemtoCorrFunc(void *p) {
      delete ((::NicaFemtoCorrFunc*)p);
   }
   static void deleteArray_NicaFemtoCorrFunc(void *p) {
      delete [] ((::NicaFemtoCorrFunc*)p);
   }
   static void destruct_NicaFemtoCorrFunc(void *p) {
      typedef ::NicaFemtoCorrFunc current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaFemtoCorrFunc(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaFemtoCorrFunc*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaFemtoCorrFunc

//______________________________________________________________________________
void NicaFemtoCorrFunc1D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoCorrFunc1D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoCorrFunc1D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoCorrFunc1D::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaFemtoCorrFunc1D(void *p) {
      delete ((::NicaFemtoCorrFunc1D*)p);
   }
   static void deleteArray_NicaFemtoCorrFunc1D(void *p) {
      delete [] ((::NicaFemtoCorrFunc1D*)p);
   }
   static void destruct_NicaFemtoCorrFunc1D(void *p) {
      typedef ::NicaFemtoCorrFunc1D current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaFemtoCorrFunc1D(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaFemtoCorrFunc1D*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaFemtoCorrFunc1D

//______________________________________________________________________________
void NicaFemtoCorrFunc2D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoCorrFunc2D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoCorrFunc2D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoCorrFunc2D::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaFemtoCorrFunc2D(void *p) {
      delete ((::NicaFemtoCorrFunc2D*)p);
   }
   static void deleteArray_NicaFemtoCorrFunc2D(void *p) {
      delete [] ((::NicaFemtoCorrFunc2D*)p);
   }
   static void destruct_NicaFemtoCorrFunc2D(void *p) {
      typedef ::NicaFemtoCorrFunc2D current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaFemtoCorrFunc2D(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaFemtoCorrFunc2D*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaFemtoCorrFunc2D

//______________________________________________________________________________
void NicaFemtoSourceModel::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoSourceModel.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoSourceModel::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoSourceModel::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaFemtoSourceModel(void *p) {
      delete ((::NicaFemtoSourceModel*)p);
   }
   static void deleteArray_NicaFemtoSourceModel(void *p) {
      delete [] ((::NicaFemtoSourceModel*)p);
   }
   static void destruct_NicaFemtoSourceModel(void *p) {
      typedef ::NicaFemtoSourceModel current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoSourceModel

//______________________________________________________________________________
void NicaFemtoSourceModel1D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoSourceModel1D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoSourceModel1D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoSourceModel1D::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaFemtoSourceModel1D(void *p) {
      delete ((::NicaFemtoSourceModel1D*)p);
   }
   static void deleteArray_NicaFemtoSourceModel1D(void *p) {
      delete [] ((::NicaFemtoSourceModel1D*)p);
   }
   static void destruct_NicaFemtoSourceModel1D(void *p) {
      typedef ::NicaFemtoSourceModel1D current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoSourceModel1D

//______________________________________________________________________________
void NicaFemtoSourceModel3D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoSourceModel3D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoSourceModel3D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoSourceModel3D::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaFemtoSourceModel3D(void *p) {
      delete ((::NicaFemtoSourceModel3D*)p);
   }
   static void deleteArray_NicaFemtoSourceModel3D(void *p) {
      delete [] ((::NicaFemtoSourceModel3D*)p);
   }
   static void destruct_NicaFemtoSourceModel3D(void *p) {
      typedef ::NicaFemtoSourceModel3D current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoSourceModel3D

//______________________________________________________________________________
void NicaFemtoFreezoutGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoFreezoutGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoFreezoutGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoFreezoutGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaFemtoFreezoutGenerator(void *p) {
      delete ((::NicaFemtoFreezoutGenerator*)p);
   }
   static void deleteArray_NicaFemtoFreezoutGenerator(void *p) {
      delete [] ((::NicaFemtoFreezoutGenerator*)p);
   }
   static void destruct_NicaFemtoFreezoutGenerator(void *p) {
      typedef ::NicaFemtoFreezoutGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoFreezoutGenerator

//______________________________________________________________________________
void NicaFemtoFreezoutGeneratorLCMS::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoFreezoutGeneratorLCMS.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoFreezoutGeneratorLCMS::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoFreezoutGeneratorLCMS::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoFreezoutGeneratorLCMS(void *p) {
      return  p ? new(p) ::NicaFemtoFreezoutGeneratorLCMS : new ::NicaFemtoFreezoutGeneratorLCMS;
   }
   static void *newArray_NicaFemtoFreezoutGeneratorLCMS(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoFreezoutGeneratorLCMS[nElements] : new ::NicaFemtoFreezoutGeneratorLCMS[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoFreezoutGeneratorLCMS(void *p) {
      delete ((::NicaFemtoFreezoutGeneratorLCMS*)p);
   }
   static void deleteArray_NicaFemtoFreezoutGeneratorLCMS(void *p) {
      delete [] ((::NicaFemtoFreezoutGeneratorLCMS*)p);
   }
   static void destruct_NicaFemtoFreezoutGeneratorLCMS(void *p) {
      typedef ::NicaFemtoFreezoutGeneratorLCMS current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoFreezoutGeneratorLCMS

//______________________________________________________________________________
void NicaFemtoFreezoutGeneratorPRF::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoFreezoutGeneratorPRF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoFreezoutGeneratorPRF::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoFreezoutGeneratorPRF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoFreezoutGeneratorPRF(void *p) {
      return  p ? new(p) ::NicaFemtoFreezoutGeneratorPRF : new ::NicaFemtoFreezoutGeneratorPRF;
   }
   static void *newArray_NicaFemtoFreezoutGeneratorPRF(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoFreezoutGeneratorPRF[nElements] : new ::NicaFemtoFreezoutGeneratorPRF[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoFreezoutGeneratorPRF(void *p) {
      delete ((::NicaFemtoFreezoutGeneratorPRF*)p);
   }
   static void deleteArray_NicaFemtoFreezoutGeneratorPRF(void *p) {
      delete [] ((::NicaFemtoFreezoutGeneratorPRF*)p);
   }
   static void destruct_NicaFemtoFreezoutGeneratorPRF(void *p) {
      typedef ::NicaFemtoFreezoutGeneratorPRF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoFreezoutGeneratorPRF

//______________________________________________________________________________
void NicaFemtoWeightGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoWeightGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoWeightGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoWeightGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoWeightGenerator(void *p) {
      return  p ? new(p) ::NicaFemtoWeightGenerator : new ::NicaFemtoWeightGenerator;
   }
   static void *newArray_NicaFemtoWeightGenerator(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoWeightGenerator[nElements] : new ::NicaFemtoWeightGenerator[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoWeightGenerator(void *p) {
      delete ((::NicaFemtoWeightGenerator*)p);
   }
   static void deleteArray_NicaFemtoWeightGenerator(void *p) {
      delete [] ((::NicaFemtoWeightGenerator*)p);
   }
   static void destruct_NicaFemtoWeightGenerator(void *p) {
      typedef ::NicaFemtoWeightGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoWeightGenerator

//______________________________________________________________________________
void NicaFemtoBasicAna::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoBasicAna.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoBasicAna::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoBasicAna::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoBasicAna(void *p) {
      return  p ? new(p) ::NicaFemtoBasicAna : new ::NicaFemtoBasicAna;
   }
   static void *newArray_NicaFemtoBasicAna(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoBasicAna[nElements] : new ::NicaFemtoBasicAna[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoBasicAna(void *p) {
      delete ((::NicaFemtoBasicAna*)p);
   }
   static void deleteArray_NicaFemtoBasicAna(void *p) {
      delete [] ((::NicaFemtoBasicAna*)p);
   }
   static void destruct_NicaFemtoBasicAna(void *p) {
      typedef ::NicaFemtoBasicAna current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoBasicAna

//______________________________________________________________________________
void NicaFemtoFastCut::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoFastCut.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoFastCut::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoFastCut::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaFemtoFastCut(void *p) {
      delete ((::NicaFemtoFastCut*)p);
   }
   static void deleteArray_NicaFemtoFastCut(void *p) {
      delete [] ((::NicaFemtoFastCut*)p);
   }
   static void destruct_NicaFemtoFastCut(void *p) {
      typedef ::NicaFemtoFastCut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoFastCut

//______________________________________________________________________________
void NicaFemtoFastCutVirtual::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoFastCutVirtual.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoFastCutVirtual::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoFastCutVirtual::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoFastCutVirtual(void *p) {
      return  p ? new(p) ::NicaFemtoFastCutVirtual : new ::NicaFemtoFastCutVirtual;
   }
   static void *newArray_NicaFemtoFastCutVirtual(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoFastCutVirtual[nElements] : new ::NicaFemtoFastCutVirtual[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoFastCutVirtual(void *p) {
      delete ((::NicaFemtoFastCutVirtual*)p);
   }
   static void deleteArray_NicaFemtoFastCutVirtual(void *p) {
      delete [] ((::NicaFemtoFastCutVirtual*)p);
   }
   static void destruct_NicaFemtoFastCutVirtual(void *p) {
      typedef ::NicaFemtoFastCutVirtual current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoFastCutVirtual

//______________________________________________________________________________
void NicaFemtoFastCutKt::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoFastCutKt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoFastCutKt::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoFastCutKt::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoFastCutKt(void *p) {
      return  p ? new(p) ::NicaFemtoFastCutKt : new ::NicaFemtoFastCutKt;
   }
   static void *newArray_NicaFemtoFastCutKt(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoFastCutKt[nElements] : new ::NicaFemtoFastCutKt[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoFastCutKt(void *p) {
      delete ((::NicaFemtoFastCutKt*)p);
   }
   static void deleteArray_NicaFemtoFastCutKt(void *p) {
      delete [] ((::NicaFemtoFastCutKt*)p);
   }
   static void destruct_NicaFemtoFastCutKt(void *p) {
      typedef ::NicaFemtoFastCutKt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoFastCutKt

//______________________________________________________________________________
void NicaFemtoFastCutPtSum::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoFastCutPtSum.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoFastCutPtSum::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoFastCutPtSum::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoFastCutPtSum(void *p) {
      return  p ? new(p) ::NicaFemtoFastCutPtSum : new ::NicaFemtoFastCutPtSum;
   }
   static void *newArray_NicaFemtoFastCutPtSum(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoFastCutPtSum[nElements] : new ::NicaFemtoFastCutPtSum[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoFastCutPtSum(void *p) {
      delete ((::NicaFemtoFastCutPtSum*)p);
   }
   static void deleteArray_NicaFemtoFastCutPtSum(void *p) {
      delete [] ((::NicaFemtoFastCutPtSum*)p);
   }
   static void destruct_NicaFemtoFastCutPtSum(void *p) {
      typedef ::NicaFemtoFastCutPtSum current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoFastCutPtSum

//______________________________________________________________________________
void NicaFemtoFastCutPhi::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoFastCutPhi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoFastCutPhi::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoFastCutPhi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoFastCutPhi(void *p) {
      return  p ? new(p) ::NicaFemtoFastCutPhi : new ::NicaFemtoFastCutPhi;
   }
   static void *newArray_NicaFemtoFastCutPhi(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoFastCutPhi[nElements] : new ::NicaFemtoFastCutPhi[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoFastCutPhi(void *p) {
      delete ((::NicaFemtoFastCutPhi*)p);
   }
   static void deleteArray_NicaFemtoFastCutPhi(void *p) {
      delete [] ((::NicaFemtoFastCutPhi*)p);
   }
   static void destruct_NicaFemtoFastCutPhi(void *p) {
      typedef ::NicaFemtoFastCutPhi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoFastCutPhi

//______________________________________________________________________________
void NicaFemtoFastCutEta::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoFastCutEta.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoFastCutEta::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoFastCutEta::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoFastCutEta(void *p) {
      return  p ? new(p) ::NicaFemtoFastCutEta : new ::NicaFemtoFastCutEta;
   }
   static void *newArray_NicaFemtoFastCutEta(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoFastCutEta[nElements] : new ::NicaFemtoFastCutEta[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoFastCutEta(void *p) {
      delete ((::NicaFemtoFastCutEta*)p);
   }
   static void deleteArray_NicaFemtoFastCutEta(void *p) {
      delete [] ((::NicaFemtoFastCutEta*)p);
   }
   static void destruct_NicaFemtoFastCutEta(void *p) {
      typedef ::NicaFemtoFastCutEta current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoFastCutEta

//______________________________________________________________________________
void NicaFemtoFreezoutsAna::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoFreezoutsAna.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoFreezoutsAna::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoFreezoutsAna::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoFreezoutsAna(void *p) {
      return  p ? new(p) ::NicaFemtoFreezoutsAna : new ::NicaFemtoFreezoutsAna;
   }
   static void *newArray_NicaFemtoFreezoutsAna(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoFreezoutsAna[nElements] : new ::NicaFemtoFreezoutsAna[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoFreezoutsAna(void *p) {
      delete ((::NicaFemtoFreezoutsAna*)p);
   }
   static void deleteArray_NicaFemtoFreezoutsAna(void *p) {
      delete [] ((::NicaFemtoFreezoutsAna*)p);
   }
   static void destruct_NicaFemtoFreezoutsAna(void *p) {
      typedef ::NicaFemtoFreezoutsAna current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoFreezoutsAna

//______________________________________________________________________________
void NicaFemtoPair::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoPair.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoPair::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoPair::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaFemtoPair(void *p) {
      delete ((::NicaFemtoPair*)p);
   }
   static void deleteArray_NicaFemtoPair(void *p) {
      delete [] ((::NicaFemtoPair*)p);
   }
   static void destruct_NicaFemtoPair(void *p) {
      typedef ::NicaFemtoPair current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoPair

//______________________________________________________________________________
void NicaFemtoPicoPair::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoPicoPair.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoPicoPair::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoPicoPair::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoPicoPair(void *p) {
      return  p ? new(p) ::NicaFemtoPicoPair : new ::NicaFemtoPicoPair;
   }
   static void *newArray_NicaFemtoPicoPair(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoPicoPair[nElements] : new ::NicaFemtoPicoPair[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoPicoPair(void *p) {
      delete ((::NicaFemtoPicoPair*)p);
   }
   static void deleteArray_NicaFemtoPicoPair(void *p) {
      delete [] ((::NicaFemtoPicoPair*)p);
   }
   static void destruct_NicaFemtoPicoPair(void *p) {
      typedef ::NicaFemtoPicoPair current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoPicoPair

//______________________________________________________________________________
void NicaFemtoMicroPair::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoMicroPair.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoMicroPair::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoMicroPair::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoMicroPair(void *p) {
      return  p ? new(p) ::NicaFemtoMicroPair : new ::NicaFemtoMicroPair;
   }
   static void *newArray_NicaFemtoMicroPair(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoMicroPair[nElements] : new ::NicaFemtoMicroPair[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoMicroPair(void *p) {
      delete ((::NicaFemtoMicroPair*)p);
   }
   static void deleteArray_NicaFemtoMicroPair(void *p) {
      delete [] ((::NicaFemtoMicroPair*)p);
   }
   static void destruct_NicaFemtoMicroPair(void *p) {
      typedef ::NicaFemtoMicroPair current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoMicroPair

//______________________________________________________________________________
void NicaFemtoMiniPair::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoMiniPair.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoMiniPair::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoMiniPair::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoMiniPair(void *p) {
      return  p ? new(p) ::NicaFemtoMiniPair : new ::NicaFemtoMiniPair;
   }
   static void *newArray_NicaFemtoMiniPair(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoMiniPair[nElements] : new ::NicaFemtoMiniPair[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoMiniPair(void *p) {
      delete ((::NicaFemtoMiniPair*)p);
   }
   static void deleteArray_NicaFemtoMiniPair(void *p) {
      delete [] ((::NicaFemtoMiniPair*)p);
   }
   static void destruct_NicaFemtoMiniPair(void *p) {
      typedef ::NicaFemtoMiniPair current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoMiniPair

//______________________________________________________________________________
void NicaFemtoDumpPairAna::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoDumpPairAna.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoDumpPairAna::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoDumpPairAna::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoDumpPairAna(void *p) {
      return  p ? new(p) ::NicaFemtoDumpPairAna : new ::NicaFemtoDumpPairAna;
   }
   static void *newArray_NicaFemtoDumpPairAna(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoDumpPairAna[nElements] : new ::NicaFemtoDumpPairAna[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoDumpPairAna(void *p) {
      delete ((::NicaFemtoDumpPairAna*)p);
   }
   static void deleteArray_NicaFemtoDumpPairAna(void *p) {
      delete [] ((::NicaFemtoDumpPairAna*)p);
   }
   static void destruct_NicaFemtoDumpPairAna(void *p) {
      typedef ::NicaFemtoDumpPairAna current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoDumpPairAna

//______________________________________________________________________________
void NicaFemtoEventBinsAna::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoEventBinsAna.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoEventBinsAna::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoEventBinsAna::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoEventBinsAna(void *p) {
      return  p ? new(p) ::NicaFemtoEventBinsAna : new ::NicaFemtoEventBinsAna;
   }
   static void *newArray_NicaFemtoEventBinsAna(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoEventBinsAna[nElements] : new ::NicaFemtoEventBinsAna[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoEventBinsAna(void *p) {
      delete ((::NicaFemtoEventBinsAna*)p);
   }
   static void deleteArray_NicaFemtoEventBinsAna(void *p) {
      delete [] ((::NicaFemtoEventBinsAna*)p);
   }
   static void destruct_NicaFemtoEventBinsAna(void *p) {
      typedef ::NicaFemtoEventBinsAna current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoEventBinsAna

//______________________________________________________________________________
void NicaFemtoDPhiDEta::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoDPhiDEta.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoDPhiDEta::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoDPhiDEta::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoDPhiDEta(void *p) {
      return  p ? new(p) ::NicaFemtoDPhiDEta : new ::NicaFemtoDPhiDEta;
   }
   static void *newArray_NicaFemtoDPhiDEta(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoDPhiDEta[nElements] : new ::NicaFemtoDPhiDEta[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoDPhiDEta(void *p) {
      delete ((::NicaFemtoDPhiDEta*)p);
   }
   static void deleteArray_NicaFemtoDPhiDEta(void *p) {
      delete [] ((::NicaFemtoDPhiDEta*)p);
   }
   static void destruct_NicaFemtoDPhiDEta(void *p) {
      typedef ::NicaFemtoDPhiDEta current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaFemtoDPhiDEta(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaFemtoDPhiDEta*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaFemtoDPhiDEta

//______________________________________________________________________________
void NicaFemto1DCF::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemto1DCF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemto1DCF::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemto1DCF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemto1DCF(void *p) {
      return  p ? new(p) ::NicaFemto1DCF : new ::NicaFemto1DCF;
   }
   static void *newArray_NicaFemto1DCF(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemto1DCF[nElements] : new ::NicaFemto1DCF[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemto1DCF(void *p) {
      delete ((::NicaFemto1DCF*)p);
   }
   static void deleteArray_NicaFemto1DCF(void *p) {
      delete [] ((::NicaFemto1DCF*)p);
   }
   static void destruct_NicaFemto1DCF(void *p) {
      typedef ::NicaFemto1DCF current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaFemto1DCF(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaFemto1DCF*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaFemto1DCF

//______________________________________________________________________________
void NicaFemto3DCF::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemto3DCF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemto3DCF::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemto3DCF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemto3DCF(void *p) {
      return  p ? new(p) ::NicaFemto3DCF : new ::NicaFemto3DCF;
   }
   static void *newArray_NicaFemto3DCF(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemto3DCF[nElements] : new ::NicaFemto3DCF[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemto3DCF(void *p) {
      delete ((::NicaFemto3DCF*)p);
   }
   static void deleteArray_NicaFemto3DCF(void *p) {
      delete [] ((::NicaFemto3DCF*)p);
   }
   static void destruct_NicaFemto3DCF(void *p) {
      typedef ::NicaFemto3DCF current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaFemto3DCF(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaFemto3DCF*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaFemto3DCF

//______________________________________________________________________________
void NicaFemto3DCFQinv::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemto3DCFQinv.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemto3DCFQinv::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemto3DCFQinv::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemto3DCFQinv(void *p) {
      return  p ? new(p) ::NicaFemto3DCFQinv : new ::NicaFemto3DCFQinv;
   }
   static void *newArray_NicaFemto3DCFQinv(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemto3DCFQinv[nElements] : new ::NicaFemto3DCFQinv[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemto3DCFQinv(void *p) {
      delete ((::NicaFemto3DCFQinv*)p);
   }
   static void deleteArray_NicaFemto3DCFQinv(void *p) {
      delete [] ((::NicaFemto3DCFQinv*)p);
   }
   static void destruct_NicaFemto3DCFQinv(void *p) {
      typedef ::NicaFemto3DCFQinv current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaFemto3DCFQinv(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaFemto3DCFQinv*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaFemto3DCFQinv

//______________________________________________________________________________
void NicaFemtoPairPRF::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoPairPRF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoPairPRF::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoPairPRF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoPairPRF(void *p) {
      return  p ? new(p) ::NicaFemtoPairPRF : new ::NicaFemtoPairPRF;
   }
   static void *newArray_NicaFemtoPairPRF(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoPairPRF[nElements] : new ::NicaFemtoPairPRF[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoPairPRF(void *p) {
      delete ((::NicaFemtoPairPRF*)p);
   }
   static void deleteArray_NicaFemtoPairPRF(void *p) {
      delete [] ((::NicaFemtoPairPRF*)p);
   }
   static void destruct_NicaFemtoPairPRF(void *p) {
      typedef ::NicaFemtoPairPRF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoPairPRF

//______________________________________________________________________________
void NicaFemtoPair_SH::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoPair_SH.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoPair_SH::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoPair_SH::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaFemtoPair_SH(void *p) {
      delete ((::NicaFemtoPair_SH*)p);
   }
   static void deleteArray_NicaFemtoPair_SH(void *p) {
      delete [] ((::NicaFemtoPair_SH*)p);
   }
   static void destruct_NicaFemtoPair_SH(void *p) {
      typedef ::NicaFemtoPair_SH current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoPair_SH

//______________________________________________________________________________
void NicaFemtoPairLCMS::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoPairLCMS.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoPairLCMS::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoPairLCMS::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoPairLCMS(void *p) {
      return  p ? new(p) ::NicaFemtoPairLCMS : new ::NicaFemtoPairLCMS;
   }
   static void *newArray_NicaFemtoPairLCMS(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoPairLCMS[nElements] : new ::NicaFemtoPairLCMS[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoPairLCMS(void *p) {
      delete ((::NicaFemtoPairLCMS*)p);
   }
   static void deleteArray_NicaFemtoPairLCMS(void *p) {
      delete [] ((::NicaFemtoPairLCMS*)p);
   }
   static void destruct_NicaFemtoPairLCMS(void *p) {
      typedef ::NicaFemtoPairLCMS current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoPairLCMS

//______________________________________________________________________________
void NicaFemtoPairPRF_SH::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoPairPRF_SH.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoPairPRF_SH::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoPairPRF_SH::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoPairPRF_SH(void *p) {
      return  p ? new(p) ::NicaFemtoPairPRF_SH : new ::NicaFemtoPairPRF_SH;
   }
   static void *newArray_NicaFemtoPairPRF_SH(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoPairPRF_SH[nElements] : new ::NicaFemtoPairPRF_SH[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoPairPRF_SH(void *p) {
      delete ((::NicaFemtoPairPRF_SH*)p);
   }
   static void deleteArray_NicaFemtoPairPRF_SH(void *p) {
      delete [] ((::NicaFemtoPairPRF_SH*)p);
   }
   static void destruct_NicaFemtoPairPRF_SH(void *p) {
      typedef ::NicaFemtoPairPRF_SH current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoPairPRF_SH

//______________________________________________________________________________
void NicaFemtoPairLCMS_SH::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoPairLCMS_SH.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoPairLCMS_SH::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoPairLCMS_SH::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoPairLCMS_SH(void *p) {
      return  p ? new(p) ::NicaFemtoPairLCMS_SH : new ::NicaFemtoPairLCMS_SH;
   }
   static void *newArray_NicaFemtoPairLCMS_SH(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoPairLCMS_SH[nElements] : new ::NicaFemtoPairLCMS_SH[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoPairLCMS_SH(void *p) {
      delete ((::NicaFemtoPairLCMS_SH*)p);
   }
   static void deleteArray_NicaFemtoPairLCMS_SH(void *p) {
      delete [] ((::NicaFemtoPairLCMS_SH*)p);
   }
   static void destruct_NicaFemtoPairLCMS_SH(void *p) {
      typedef ::NicaFemtoPairLCMS_SH current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoPairLCMS_SH

//______________________________________________________________________________
void NicaFemtoPairDPhiDEta::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoPairDPhiDEta.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoPairDPhiDEta::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoPairDPhiDEta::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoPairDPhiDEta(void *p) {
      return  p ? new(p) ::NicaFemtoPairDPhiDEta : new ::NicaFemtoPairDPhiDEta;
   }
   static void *newArray_NicaFemtoPairDPhiDEta(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoPairDPhiDEta[nElements] : new ::NicaFemtoPairDPhiDEta[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoPairDPhiDEta(void *p) {
      delete ((::NicaFemtoPairDPhiDEta*)p);
   }
   static void deleteArray_NicaFemtoPairDPhiDEta(void *p) {
      delete [] ((::NicaFemtoPairDPhiDEta*)p);
   }
   static void destruct_NicaFemtoPairDPhiDEta(void *p) {
      typedef ::NicaFemtoPairDPhiDEta current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoPairDPhiDEta

//______________________________________________________________________________
void NicaFemtoPairPRFL::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoPairPRFL.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoPairPRFL::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoPairPRFL::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoPairPRFL(void *p) {
      return  p ? new(p) ::NicaFemtoPairPRFL : new ::NicaFemtoPairPRFL;
   }
   static void *newArray_NicaFemtoPairPRFL(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoPairPRFL[nElements] : new ::NicaFemtoPairPRFL[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoPairPRFL(void *p) {
      delete ((::NicaFemtoPairPRFL*)p);
   }
   static void deleteArray_NicaFemtoPairPRFL(void *p) {
      delete [] ((::NicaFemtoPairPRFL*)p);
   }
   static void destruct_NicaFemtoPairPRFL(void *p) {
      typedef ::NicaFemtoPairPRFL current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoPairPRFL

//______________________________________________________________________________
void NicaFemtoSourceModelExp1D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoSourceModelExp1D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoSourceModelExp1D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoSourceModelExp1D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoSourceModelExp1D(void *p) {
      return  p ? new(p) ::NicaFemtoSourceModelExp1D : new ::NicaFemtoSourceModelExp1D;
   }
   static void *newArray_NicaFemtoSourceModelExp1D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoSourceModelExp1D[nElements] : new ::NicaFemtoSourceModelExp1D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoSourceModelExp1D(void *p) {
      delete ((::NicaFemtoSourceModelExp1D*)p);
   }
   static void deleteArray_NicaFemtoSourceModelExp1D(void *p) {
      delete [] ((::NicaFemtoSourceModelExp1D*)p);
   }
   static void destruct_NicaFemtoSourceModelExp1D(void *p) {
      typedef ::NicaFemtoSourceModelExp1D current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoSourceModelExp1D

//______________________________________________________________________________
void NicaFemtoSourceModelGauss1D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoSourceModelGauss1D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoSourceModelGauss1D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoSourceModelGauss1D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoSourceModelGauss1D(void *p) {
      return  p ? new(p) ::NicaFemtoSourceModelGauss1D : new ::NicaFemtoSourceModelGauss1D;
   }
   static void *newArray_NicaFemtoSourceModelGauss1D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoSourceModelGauss1D[nElements] : new ::NicaFemtoSourceModelGauss1D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoSourceModelGauss1D(void *p) {
      delete ((::NicaFemtoSourceModelGauss1D*)p);
   }
   static void deleteArray_NicaFemtoSourceModelGauss1D(void *p) {
      delete [] ((::NicaFemtoSourceModelGauss1D*)p);
   }
   static void destruct_NicaFemtoSourceModelGauss1D(void *p) {
      typedef ::NicaFemtoSourceModelGauss1D current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoSourceModelGauss1D

//______________________________________________________________________________
void NicaFemtoSourceModelGauss3D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoSourceModelGauss3D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoSourceModelGauss3D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoSourceModelGauss3D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoSourceModelGauss3D(void *p) {
      return  p ? new(p) ::NicaFemtoSourceModelGauss3D : new ::NicaFemtoSourceModelGauss3D;
   }
   static void *newArray_NicaFemtoSourceModelGauss3D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoSourceModelGauss3D[nElements] : new ::NicaFemtoSourceModelGauss3D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoSourceModelGauss3D(void *p) {
      delete ((::NicaFemtoSourceModelGauss3D*)p);
   }
   static void deleteArray_NicaFemtoSourceModelGauss3D(void *p) {
      delete [] ((::NicaFemtoSourceModelGauss3D*)p);
   }
   static void destruct_NicaFemtoSourceModelGauss3D(void *p) {
      typedef ::NicaFemtoSourceModelGauss3D current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoSourceModelGauss3D

//______________________________________________________________________________
void NicaFemtoSourceModelGausMt::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoSourceModelGausMt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoSourceModelGausMt::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoSourceModelGausMt::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoSourceModelGausMt(void *p) {
      return  p ? new(p) ::NicaFemtoSourceModelGausMt : new ::NicaFemtoSourceModelGausMt;
   }
   static void *newArray_NicaFemtoSourceModelGausMt(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoSourceModelGausMt[nElements] : new ::NicaFemtoSourceModelGausMt[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoSourceModelGausMt(void *p) {
      delete ((::NicaFemtoSourceModelGausMt*)p);
   }
   static void deleteArray_NicaFemtoSourceModelGausMt(void *p) {
      delete [] ((::NicaFemtoSourceModelGausMt*)p);
   }
   static void destruct_NicaFemtoSourceModelGausMt(void *p) {
      typedef ::NicaFemtoSourceModelGausMt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoSourceModelGausMt

//______________________________________________________________________________
void NicaCorrFit::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaCorrFit(void *p) {
      delete ((::NicaCorrFit*)p);
   }
   static void deleteArray_NicaCorrFit(void *p) {
      delete [] ((::NicaCorrFit*)p);
   }
   static void destruct_NicaCorrFit(void *p) {
      typedef ::NicaCorrFit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit

//______________________________________________________________________________
void NicaCorrFitFunc::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitFunc.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitFunc::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitFunc::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaCorrFitFunc(void *p) {
      delete ((::NicaCorrFitFunc*)p);
   }
   static void deleteArray_NicaCorrFitFunc(void *p) {
      delete [] ((::NicaCorrFitFunc*)p);
   }
   static void destruct_NicaCorrFitFunc(void *p) {
      typedef ::NicaCorrFitFunc current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitFunc

//______________________________________________________________________________
void NicaCorrFit1DCF::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit1DCF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit1DCF::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit1DCF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit1DCF(void *p) {
      return  p ? new(p) ::NicaCorrFit1DCF : new ::NicaCorrFit1DCF;
   }
   static void *newArray_NicaCorrFit1DCF(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit1DCF[nElements] : new ::NicaCorrFit1DCF[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit1DCF(void *p) {
      delete ((::NicaCorrFit1DCF*)p);
   }
   static void deleteArray_NicaCorrFit1DCF(void *p) {
      delete [] ((::NicaCorrFit1DCF*)p);
   }
   static void destruct_NicaCorrFit1DCF(void *p) {
      typedef ::NicaCorrFit1DCF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit1DCF

//______________________________________________________________________________
void NicaCorrFitMath1DCF::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitMath1DCF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitMath1DCF::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitMath1DCF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitMath1DCF(void *p) {
      return  p ? new(p) ::NicaCorrFitMath1DCF : new ::NicaCorrFitMath1DCF;
   }
   static void *newArray_NicaCorrFitMath1DCF(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitMath1DCF[nElements] : new ::NicaCorrFitMath1DCF[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitMath1DCF(void *p) {
      delete ((::NicaCorrFitMath1DCF*)p);
   }
   static void deleteArray_NicaCorrFitMath1DCF(void *p) {
      delete [] ((::NicaCorrFitMath1DCF*)p);
   }
   static void destruct_NicaCorrFitMath1DCF(void *p) {
      typedef ::NicaCorrFitMath1DCF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitMath1DCF

//______________________________________________________________________________
void NicaCorrFit1DCF_Sum::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit1DCF_Sum.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit1DCF_Sum::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit1DCF_Sum::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit1DCF_Sum(void *p) {
      return  p ? new(p) ::NicaCorrFit1DCF_Sum : new ::NicaCorrFit1DCF_Sum;
   }
   static void *newArray_NicaCorrFit1DCF_Sum(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit1DCF_Sum[nElements] : new ::NicaCorrFit1DCF_Sum[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit1DCF_Sum(void *p) {
      delete ((::NicaCorrFit1DCF_Sum*)p);
   }
   static void deleteArray_NicaCorrFit1DCF_Sum(void *p) {
      delete [] ((::NicaCorrFit1DCF_Sum*)p);
   }
   static void destruct_NicaCorrFit1DCF_Sum(void *p) {
      typedef ::NicaCorrFit1DCF_Sum current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit1DCF_Sum

//______________________________________________________________________________
void NicaCorrFit1DCF_SumRsame::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit1DCF_SumRsame.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit1DCF_SumRsame::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit1DCF_SumRsame::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit1DCF_SumRsame(void *p) {
      return  p ? new(p) ::NicaCorrFit1DCF_SumRsame : new ::NicaCorrFit1DCF_SumRsame;
   }
   static void *newArray_NicaCorrFit1DCF_SumRsame(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit1DCF_SumRsame[nElements] : new ::NicaCorrFit1DCF_SumRsame[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit1DCF_SumRsame(void *p) {
      delete ((::NicaCorrFit1DCF_SumRsame*)p);
   }
   static void deleteArray_NicaCorrFit1DCF_SumRsame(void *p) {
      delete [] ((::NicaCorrFit1DCF_SumRsame*)p);
   }
   static void destruct_NicaCorrFit1DCF_SumRsame(void *p) {
      typedef ::NicaCorrFit1DCF_SumRsame current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit1DCF_SumRsame

//______________________________________________________________________________
void NicaCorrFit1DCF_Multi::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit1DCF_Multi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit1DCF_Multi::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit1DCF_Multi::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaCorrFit1DCF_Multi(void *p) {
      delete ((::NicaCorrFit1DCF_Multi*)p);
   }
   static void deleteArray_NicaCorrFit1DCF_Multi(void *p) {
      delete [] ((::NicaCorrFit1DCF_Multi*)p);
   }
   static void destruct_NicaCorrFit1DCF_Multi(void *p) {
      typedef ::NicaCorrFit1DCF_Multi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit1DCF_Multi

//______________________________________________________________________________
void NicaCorrFit3DCF::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit3DCF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit3DCF::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit3DCF::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaCorrFit3DCF(void *p) {
      delete ((::NicaCorrFit3DCF*)p);
   }
   static void deleteArray_NicaCorrFit3DCF(void *p) {
      delete [] ((::NicaCorrFit3DCF*)p);
   }
   static void destruct_NicaCorrFit3DCF(void *p) {
      typedef ::NicaCorrFit3DCF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit3DCF

//______________________________________________________________________________
void NicaCorrFitMath3DCF::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitMath3DCF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitMath3DCF::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitMath3DCF::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaCorrFitMath3DCF(void *p) {
      delete ((::NicaCorrFitMath3DCF*)p);
   }
   static void deleteArray_NicaCorrFitMath3DCF(void *p) {
      delete [] ((::NicaCorrFitMath3DCF*)p);
   }
   static void destruct_NicaCorrFitMath3DCF(void *p) {
      typedef ::NicaCorrFitMath3DCF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitMath3DCF

//______________________________________________________________________________
void NicaCorrFit3DCF_Sum::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit3DCF_Sum.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit3DCF_Sum::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit3DCF_Sum::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaCorrFit3DCF_Sum(void *p) {
      delete ((::NicaCorrFit3DCF_Sum*)p);
   }
   static void deleteArray_NicaCorrFit3DCF_Sum(void *p) {
      delete [] ((::NicaCorrFit3DCF_Sum*)p);
   }
   static void destruct_NicaCorrFit3DCF_Sum(void *p) {
      typedef ::NicaCorrFit3DCF_Sum current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit3DCF_Sum

//______________________________________________________________________________
void NicaCorrFitKisiel::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitKisiel.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitKisiel::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitKisiel::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitKisiel(void *p) {
      return  p ? new(p) ::NicaCorrFitKisiel : new ::NicaCorrFitKisiel;
   }
   static void *newArray_NicaCorrFitKisiel(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitKisiel[nElements] : new ::NicaCorrFitKisiel[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitKisiel(void *p) {
      delete ((::NicaCorrFitKisiel*)p);
   }
   static void deleteArray_NicaCorrFitKisiel(void *p) {
      delete [] ((::NicaCorrFitKisiel*)p);
   }
   static void destruct_NicaCorrFitKisiel(void *p) {
      typedef ::NicaCorrFitKisiel current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitKisiel

//______________________________________________________________________________
void NicaCorrFitWielanek::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitWielanek.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitWielanek::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitWielanek::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitWielanek(void *p) {
      return  p ? new(p) ::NicaCorrFitWielanek : new ::NicaCorrFitWielanek;
   }
   static void *newArray_NicaCorrFitWielanek(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitWielanek[nElements] : new ::NicaCorrFitWielanek[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitWielanek(void *p) {
      delete ((::NicaCorrFitWielanek*)p);
   }
   static void deleteArray_NicaCorrFitWielanek(void *p) {
      delete [] ((::NicaCorrFitWielanek*)p);
   }
   static void destruct_NicaCorrFitWielanek(void *p) {
      typedef ::NicaCorrFitWielanek current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitWielanek

//______________________________________________________________________________
void NicaCorrFitParButton::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitParButton.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitParButton::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitParButton::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitParButton(void *p) {
      return  p ? new(p) ::NicaCorrFitParButton : new ::NicaCorrFitParButton;
   }
   static void *newArray_NicaCorrFitParButton(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitParButton[nElements] : new ::NicaCorrFitParButton[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitParButton(void *p) {
      delete ((::NicaCorrFitParButton*)p);
   }
   static void deleteArray_NicaCorrFitParButton(void *p) {
      delete [] ((::NicaCorrFitParButton*)p);
   }
   static void destruct_NicaCorrFitParButton(void *p) {
      typedef ::NicaCorrFitParButton current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitParButton

//______________________________________________________________________________
void NicaCorrFitChiSelector::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitChiSelector.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitChiSelector::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitChiSelector::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitChiSelector(void *p) {
      return  p ? new(p) ::NicaCorrFitChiSelector : new ::NicaCorrFitChiSelector;
   }
   static void *newArray_NicaCorrFitChiSelector(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitChiSelector[nElements] : new ::NicaCorrFitChiSelector[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitChiSelector(void *p) {
      delete ((::NicaCorrFitChiSelector*)p);
   }
   static void deleteArray_NicaCorrFitChiSelector(void *p) {
      delete [] ((::NicaCorrFitChiSelector*)p);
   }
   static void destruct_NicaCorrFitChiSelector(void *p) {
      typedef ::NicaCorrFitChiSelector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitChiSelector

//______________________________________________________________________________
void NicaCorrFitGUI::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitGUI.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitGUI::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitGUI::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaCorrFitGUI(void *p) {
      delete ((::NicaCorrFitGUI*)p);
   }
   static void deleteArray_NicaCorrFitGUI(void *p) {
      delete [] ((::NicaCorrFitGUI*)p);
   }
   static void destruct_NicaCorrFitGUI(void *p) {
      typedef ::NicaCorrFitGUI current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitGUI

//______________________________________________________________________________
void NicaCorrFitHDFunc::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitHDFunc.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitHDFunc::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitHDFunc::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaCorrFitHDFunc(void *p) {
      delete ((::NicaCorrFitHDFunc*)p);
   }
   static void deleteArray_NicaCorrFitHDFunc(void *p) {
      delete [] ((::NicaCorrFitHDFunc*)p);
   }
   static void destruct_NicaCorrFitHDFunc(void *p) {
      typedef ::NicaCorrFitHDFunc current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitHDFunc

//______________________________________________________________________________
void NicaCorrFitHDFunc1D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitHDFunc1D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitHDFunc1D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitHDFunc1D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitHDFunc1D(void *p) {
      return  p ? new(p) ::NicaCorrFitHDFunc1D : new ::NicaCorrFitHDFunc1D;
   }
   static void *newArray_NicaCorrFitHDFunc1D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitHDFunc1D[nElements] : new ::NicaCorrFitHDFunc1D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitHDFunc1D(void *p) {
      delete ((::NicaCorrFitHDFunc1D*)p);
   }
   static void deleteArray_NicaCorrFitHDFunc1D(void *p) {
      delete [] ((::NicaCorrFitHDFunc1D*)p);
   }
   static void destruct_NicaCorrFitHDFunc1D(void *p) {
      typedef ::NicaCorrFitHDFunc1D current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitHDFunc1D

//______________________________________________________________________________
void NicaCorrFitHDFunc3D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitHDFunc3D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitHDFunc3D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitHDFunc3D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitHDFunc3D(void *p) {
      return  p ? new(p) ::NicaCorrFitHDFunc3D : new ::NicaCorrFitHDFunc3D;
   }
   static void *newArray_NicaCorrFitHDFunc3D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitHDFunc3D[nElements] : new ::NicaCorrFitHDFunc3D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitHDFunc3D(void *p) {
      delete ((::NicaCorrFitHDFunc3D*)p);
   }
   static void deleteArray_NicaCorrFitHDFunc3D(void *p) {
      delete [] ((::NicaCorrFitHDFunc3D*)p);
   }
   static void destruct_NicaCorrFitHDFunc3D(void *p) {
      typedef ::NicaCorrFitHDFunc3D current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitHDFunc3D

//______________________________________________________________________________
void NicaFemtoCorrFuncKtMap1D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoCorrFuncKtMap1D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoCorrFuncKtMap1D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoCorrFuncKtMap1D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoCorrFuncKtMap1D(void *p) {
      return  p ? new(p) ::NicaFemtoCorrFuncKtMap1D : new ::NicaFemtoCorrFuncKtMap1D;
   }
   static void *newArray_NicaFemtoCorrFuncKtMap1D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoCorrFuncKtMap1D[nElements] : new ::NicaFemtoCorrFuncKtMap1D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoCorrFuncKtMap1D(void *p) {
      delete ((::NicaFemtoCorrFuncKtMap1D*)p);
   }
   static void deleteArray_NicaFemtoCorrFuncKtMap1D(void *p) {
      delete [] ((::NicaFemtoCorrFuncKtMap1D*)p);
   }
   static void destruct_NicaFemtoCorrFuncKtMap1D(void *p) {
      typedef ::NicaFemtoCorrFuncKtMap1D current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaFemtoCorrFuncKtMap1D(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaFemtoCorrFuncKtMap1D*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaFemtoCorrFuncKtMap1D

//______________________________________________________________________________
void NicaFemto1DCFAnaMap::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemto1DCFAnaMap.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemto1DCFAnaMap::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemto1DCFAnaMap::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemto1DCFAnaMap(void *p) {
      return  p ? new(p) ::NicaFemto1DCFAnaMap : new ::NicaFemto1DCFAnaMap;
   }
   static void *newArray_NicaFemto1DCFAnaMap(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemto1DCFAnaMap[nElements] : new ::NicaFemto1DCFAnaMap[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemto1DCFAnaMap(void *p) {
      delete ((::NicaFemto1DCFAnaMap*)p);
   }
   static void deleteArray_NicaFemto1DCFAnaMap(void *p) {
      delete [] ((::NicaFemto1DCFAnaMap*)p);
   }
   static void destruct_NicaFemto1DCFAnaMap(void *p) {
      typedef ::NicaFemto1DCFAnaMap current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemto1DCFAnaMap

//______________________________________________________________________________
void NicaFemto1DCFAnaMapMC::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemto1DCFAnaMapMC.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemto1DCFAnaMapMC::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemto1DCFAnaMapMC::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemto1DCFAnaMapMC(void *p) {
      return  p ? new(p) ::NicaFemto1DCFAnaMapMC : new ::NicaFemto1DCFAnaMapMC;
   }
   static void *newArray_NicaFemto1DCFAnaMapMC(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemto1DCFAnaMapMC[nElements] : new ::NicaFemto1DCFAnaMapMC[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemto1DCFAnaMapMC(void *p) {
      delete ((::NicaFemto1DCFAnaMapMC*)p);
   }
   static void deleteArray_NicaFemto1DCFAnaMapMC(void *p) {
      delete [] ((::NicaFemto1DCFAnaMapMC*)p);
   }
   static void destruct_NicaFemto1DCFAnaMapMC(void *p) {
      typedef ::NicaFemto1DCFAnaMapMC current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemto1DCFAnaMapMC

//______________________________________________________________________________
void NicaWielanekMapGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaWielanekMapGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaWielanekMapGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaWielanekMapGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaWielanekMapGenerator(void *p) {
      return  p ? new(p) ::NicaWielanekMapGenerator : new ::NicaWielanekMapGenerator;
   }
   static void *newArray_NicaWielanekMapGenerator(Long_t nElements, void *p) {
      return p ? new(p) ::NicaWielanekMapGenerator[nElements] : new ::NicaWielanekMapGenerator[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaWielanekMapGenerator(void *p) {
      delete ((::NicaWielanekMapGenerator*)p);
   }
   static void deleteArray_NicaWielanekMapGenerator(void *p) {
      delete [] ((::NicaWielanekMapGenerator*)p);
   }
   static void destruct_NicaWielanekMapGenerator(void *p) {
      typedef ::NicaWielanekMapGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaWielanekMapGenerator

//______________________________________________________________________________
void NicaFemto1DCFAnaMapPairsDumped::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemto1DCFAnaMapPairsDumped.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemto1DCFAnaMapPairsDumped::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemto1DCFAnaMapPairsDumped::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemto1DCFAnaMapPairsDumped(void *p) {
      return  p ? new(p) ::NicaFemto1DCFAnaMapPairsDumped : new ::NicaFemto1DCFAnaMapPairsDumped;
   }
   static void *newArray_NicaFemto1DCFAnaMapPairsDumped(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemto1DCFAnaMapPairsDumped[nElements] : new ::NicaFemto1DCFAnaMapPairsDumped[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemto1DCFAnaMapPairsDumped(void *p) {
      delete ((::NicaFemto1DCFAnaMapPairsDumped*)p);
   }
   static void deleteArray_NicaFemto1DCFAnaMapPairsDumped(void *p) {
      delete [] ((::NicaFemto1DCFAnaMapPairsDumped*)p);
   }
   static void destruct_NicaFemto1DCFAnaMapPairsDumped(void *p) {
      typedef ::NicaFemto1DCFAnaMapPairsDumped current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemto1DCFAnaMapPairsDumped

//______________________________________________________________________________
void NicaCorrFit1DCF_Square::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit1DCF_Square.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit1DCF_Square::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit1DCF_Square::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit1DCF_Square(void *p) {
      return  p ? new(p) ::NicaCorrFit1DCF_Square : new ::NicaCorrFit1DCF_Square;
   }
   static void *newArray_NicaCorrFit1DCF_Square(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit1DCF_Square[nElements] : new ::NicaCorrFit1DCF_Square[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit1DCF_Square(void *p) {
      delete ((::NicaCorrFit1DCF_Square*)p);
   }
   static void deleteArray_NicaCorrFit1DCF_Square(void *p) {
      delete [] ((::NicaCorrFit1DCF_Square*)p);
   }
   static void destruct_NicaCorrFit1DCF_Square(void *p) {
      typedef ::NicaCorrFit1DCF_Square current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit1DCF_Square

//______________________________________________________________________________
void NicaCorrFit1DCF_Lorentz::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit1DCF_Lorentz.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit1DCF_Lorentz::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit1DCF_Lorentz::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit1DCF_Lorentz(void *p) {
      return  p ? new(p) ::NicaCorrFit1DCF_Lorentz : new ::NicaCorrFit1DCF_Lorentz;
   }
   static void *newArray_NicaCorrFit1DCF_Lorentz(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit1DCF_Lorentz[nElements] : new ::NicaCorrFit1DCF_Lorentz[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit1DCF_Lorentz(void *p) {
      delete ((::NicaCorrFit1DCF_Lorentz*)p);
   }
   static void deleteArray_NicaCorrFit1DCF_Lorentz(void *p) {
      delete [] ((::NicaCorrFit1DCF_Lorentz*)p);
   }
   static void destruct_NicaCorrFit1DCF_Lorentz(void *p) {
      typedef ::NicaCorrFit1DCF_Lorentz current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit1DCF_Lorentz

//______________________________________________________________________________
void NicaCorrFit3DCF_Square::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit3DCF_Square.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit3DCF_Square::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit3DCF_Square::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit3DCF_Square(void *p) {
      return  p ? new(p) ::NicaCorrFit3DCF_Square : new ::NicaCorrFit3DCF_Square;
   }
   static void *newArray_NicaCorrFit3DCF_Square(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit3DCF_Square[nElements] : new ::NicaCorrFit3DCF_Square[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit3DCF_Square(void *p) {
      delete ((::NicaCorrFit3DCF_Square*)p);
   }
   static void deleteArray_NicaCorrFit3DCF_Square(void *p) {
      delete [] ((::NicaCorrFit3DCF_Square*)p);
   }
   static void destruct_NicaCorrFit3DCF_Square(void *p) {
      typedef ::NicaCorrFit3DCF_Square current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit3DCF_Square

//______________________________________________________________________________
void NicaCorrFit3DCF_Lorentz::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit3DCF_Lorentz.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit3DCF_Lorentz::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit3DCF_Lorentz::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit3DCF_Lorentz(void *p) {
      return  p ? new(p) ::NicaCorrFit3DCF_Lorentz : new ::NicaCorrFit3DCF_Lorentz;
   }
   static void *newArray_NicaCorrFit3DCF_Lorentz(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit3DCF_Lorentz[nElements] : new ::NicaCorrFit3DCF_Lorentz[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit3DCF_Lorentz(void *p) {
      delete ((::NicaCorrFit3DCF_Lorentz*)p);
   }
   static void deleteArray_NicaCorrFit3DCF_Lorentz(void *p) {
      delete [] ((::NicaCorrFit3DCF_Lorentz*)p);
   }
   static void destruct_NicaCorrFit3DCF_Lorentz(void *p) {
      typedef ::NicaCorrFit3DCF_Lorentz current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit3DCF_Lorentz

//______________________________________________________________________________
void NicaCorrFit1DCF_Exp::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit1DCF_Exp.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit1DCF_Exp::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit1DCF_Exp::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit1DCF_Exp(void *p) {
      return  p ? new(p) ::NicaCorrFit1DCF_Exp : new ::NicaCorrFit1DCF_Exp;
   }
   static void *newArray_NicaCorrFit1DCF_Exp(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit1DCF_Exp[nElements] : new ::NicaCorrFit1DCF_Exp[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit1DCF_Exp(void *p) {
      delete ((::NicaCorrFit1DCF_Exp*)p);
   }
   static void deleteArray_NicaCorrFit1DCF_Exp(void *p) {
      delete [] ((::NicaCorrFit1DCF_Exp*)p);
   }
   static void destruct_NicaCorrFit1DCF_Exp(void *p) {
      typedef ::NicaCorrFit1DCF_Exp current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit1DCF_Exp

//______________________________________________________________________________
void NicaCorrFit1DCF_Gauss::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit1DCF_Gauss.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit1DCF_Gauss::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit1DCF_Gauss::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit1DCF_Gauss(void *p) {
      return  p ? new(p) ::NicaCorrFit1DCF_Gauss : new ::NicaCorrFit1DCF_Gauss;
   }
   static void *newArray_NicaCorrFit1DCF_Gauss(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit1DCF_Gauss[nElements] : new ::NicaCorrFit1DCF_Gauss[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit1DCF_Gauss(void *p) {
      delete ((::NicaCorrFit1DCF_Gauss*)p);
   }
   static void deleteArray_NicaCorrFit1DCF_Gauss(void *p) {
      delete [] ((::NicaCorrFit1DCF_Gauss*)p);
   }
   static void destruct_NicaCorrFit1DCF_Gauss(void *p) {
      typedef ::NicaCorrFit1DCF_Gauss current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit1DCF_Gauss

//______________________________________________________________________________
void NicaCorrFit1DCF_Poly::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit1DCF_Poly.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit1DCF_Poly::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit1DCF_Poly::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit1DCF_Poly(void *p) {
      return  p ? new(p) ::NicaCorrFit1DCF_Poly : new ::NicaCorrFit1DCF_Poly;
   }
   static void *newArray_NicaCorrFit1DCF_Poly(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit1DCF_Poly[nElements] : new ::NicaCorrFit1DCF_Poly[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit1DCF_Poly(void *p) {
      delete ((::NicaCorrFit1DCF_Poly*)p);
   }
   static void deleteArray_NicaCorrFit1DCF_Poly(void *p) {
      delete [] ((::NicaCorrFit1DCF_Poly*)p);
   }
   static void destruct_NicaCorrFit1DCF_Poly(void *p) {
      typedef ::NicaCorrFit1DCF_Poly current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit1DCF_Poly

//______________________________________________________________________________
void NicaCorrFit1DCF_Poly2::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit1DCF_Poly2.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit1DCF_Poly2::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit1DCF_Poly2::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaCorrFit1DCF_Poly2(void *p) {
      delete ((::NicaCorrFit1DCF_Poly2*)p);
   }
   static void deleteArray_NicaCorrFit1DCF_Poly2(void *p) {
      delete [] ((::NicaCorrFit1DCF_Poly2*)p);
   }
   static void destruct_NicaCorrFit1DCF_Poly2(void *p) {
      typedef ::NicaCorrFit1DCF_Poly2 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit1DCF_Poly2

//______________________________________________________________________________
void NicaCorrFit3DCF_Gauss::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit3DCF_Gauss.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit3DCF_Gauss::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit3DCF_Gauss::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit3DCF_Gauss(void *p) {
      return  p ? new(p) ::NicaCorrFit3DCF_Gauss : new ::NicaCorrFit3DCF_Gauss;
   }
   static void *newArray_NicaCorrFit3DCF_Gauss(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit3DCF_Gauss[nElements] : new ::NicaCorrFit3DCF_Gauss[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit3DCF_Gauss(void *p) {
      delete ((::NicaCorrFit3DCF_Gauss*)p);
   }
   static void deleteArray_NicaCorrFit3DCF_Gauss(void *p) {
      delete [] ((::NicaCorrFit3DCF_Gauss*)p);
   }
   static void destruct_NicaCorrFit3DCF_Gauss(void *p) {
      typedef ::NicaCorrFit3DCF_Gauss current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit3DCF_Gauss

//______________________________________________________________________________
void NicaCorrFit3DCF_Gauss2::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit3DCF_Gauss2.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit3DCF_Gauss2::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit3DCF_Gauss2::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit3DCF_Gauss2(void *p) {
      return  p ? new(p) ::NicaCorrFit3DCF_Gauss2 : new ::NicaCorrFit3DCF_Gauss2;
   }
   static void *newArray_NicaCorrFit3DCF_Gauss2(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit3DCF_Gauss2[nElements] : new ::NicaCorrFit3DCF_Gauss2[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit3DCF_Gauss2(void *p) {
      delete ((::NicaCorrFit3DCF_Gauss2*)p);
   }
   static void deleteArray_NicaCorrFit3DCF_Gauss2(void *p) {
      delete [] ((::NicaCorrFit3DCF_Gauss2*)p);
   }
   static void destruct_NicaCorrFit3DCF_Gauss2(void *p) {
      typedef ::NicaCorrFit3DCF_Gauss2 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit3DCF_Gauss2

//______________________________________________________________________________
void NicaCorrFit3DCF_GaussCross::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit3DCF_GaussCross.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit3DCF_GaussCross::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit3DCF_GaussCross::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit3DCF_GaussCross(void *p) {
      return  p ? new(p) ::NicaCorrFit3DCF_GaussCross : new ::NicaCorrFit3DCF_GaussCross;
   }
   static void *newArray_NicaCorrFit3DCF_GaussCross(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit3DCF_GaussCross[nElements] : new ::NicaCorrFit3DCF_GaussCross[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit3DCF_GaussCross(void *p) {
      delete ((::NicaCorrFit3DCF_GaussCross*)p);
   }
   static void deleteArray_NicaCorrFit3DCF_GaussCross(void *p) {
      delete [] ((::NicaCorrFit3DCF_GaussCross*)p);
   }
   static void destruct_NicaCorrFit3DCF_GaussCross(void *p) {
      typedef ::NicaCorrFit3DCF_GaussCross current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit3DCF_GaussCross

//______________________________________________________________________________
void NicaCorrFit1DCFCumac::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit1DCFCumac.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit1DCFCumac::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit1DCFCumac::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit1DCFCumac(void *p) {
      return  p ? new(p) ::NicaCorrFit1DCFCumac : new ::NicaCorrFit1DCFCumac;
   }
   static void *newArray_NicaCorrFit1DCFCumac(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit1DCFCumac[nElements] : new ::NicaCorrFit1DCFCumac[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit1DCFCumac(void *p) {
      delete ((::NicaCorrFit1DCFCumac*)p);
   }
   static void deleteArray_NicaCorrFit1DCFCumac(void *p) {
      delete [] ((::NicaCorrFit1DCFCumac*)p);
   }
   static void destruct_NicaCorrFit1DCFCumac(void *p) {
      typedef ::NicaCorrFit1DCFCumac current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit1DCFCumac

//______________________________________________________________________________
void NicaCorrFIt1DCFCumacLamLam::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFIt1DCFCumacLamLam.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFIt1DCFCumacLamLam::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFIt1DCFCumacLamLam::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFIt1DCFCumacLamLam(void *p) {
      return  p ? new(p) ::NicaCorrFIt1DCFCumacLamLam : new ::NicaCorrFIt1DCFCumacLamLam;
   }
   static void *newArray_NicaCorrFIt1DCFCumacLamLam(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFIt1DCFCumacLamLam[nElements] : new ::NicaCorrFIt1DCFCumacLamLam[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFIt1DCFCumacLamLam(void *p) {
      delete ((::NicaCorrFIt1DCFCumacLamLam*)p);
   }
   static void deleteArray_NicaCorrFIt1DCFCumacLamLam(void *p) {
      delete [] ((::NicaCorrFIt1DCFCumacLamLam*)p);
   }
   static void destruct_NicaCorrFIt1DCFCumacLamLam(void *p) {
      typedef ::NicaCorrFIt1DCFCumacLamLam current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFIt1DCFCumacLamLam

//______________________________________________________________________________
void NicaCorrFit1DCFCumacPLam::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit1DCFCumacPLam.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit1DCFCumacPLam::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit1DCFCumacPLam::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit1DCFCumacPLam(void *p) {
      return  p ? new(p) ::NicaCorrFit1DCFCumacPLam : new ::NicaCorrFit1DCFCumacPLam;
   }
   static void *newArray_NicaCorrFit1DCFCumacPLam(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit1DCFCumacPLam[nElements] : new ::NicaCorrFit1DCFCumacPLam[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit1DCFCumacPLam(void *p) {
      delete ((::NicaCorrFit1DCFCumacPLam*)p);
   }
   static void deleteArray_NicaCorrFit1DCFCumacPLam(void *p) {
      delete [] ((::NicaCorrFit1DCFCumacPLam*)p);
   }
   static void destruct_NicaCorrFit1DCFCumacPLam(void *p) {
      typedef ::NicaCorrFit1DCFCumacPLam current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit1DCFCumacPLam

//______________________________________________________________________________
void NicaCorrFit1DCFCumacK0K0::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit1DCFCumacK0K0.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit1DCFCumacK0K0::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit1DCFCumacK0K0::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit1DCFCumacK0K0(void *p) {
      return  p ? new(p) ::NicaCorrFit1DCFCumacK0K0 : new ::NicaCorrFit1DCFCumacK0K0;
   }
   static void *newArray_NicaCorrFit1DCFCumacK0K0(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit1DCFCumacK0K0[nElements] : new ::NicaCorrFit1DCFCumacK0K0[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit1DCFCumacK0K0(void *p) {
      delete ((::NicaCorrFit1DCFCumacK0K0*)p);
   }
   static void deleteArray_NicaCorrFit1DCFCumacK0K0(void *p) {
      delete [] ((::NicaCorrFit1DCFCumacK0K0*)p);
   }
   static void destruct_NicaCorrFit1DCFCumacK0K0(void *p) {
      typedef ::NicaCorrFit1DCFCumacK0K0 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit1DCFCumacK0K0

//______________________________________________________________________________
void NicaCorrFit3DCFBowlerSinyukov::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit3DCFBowlerSinyukov.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit3DCFBowlerSinyukov::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit3DCFBowlerSinyukov::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit3DCFBowlerSinyukov(void *p) {
      return  p ? new(p) ::NicaCorrFit3DCFBowlerSinyukov : new ::NicaCorrFit3DCFBowlerSinyukov;
   }
   static void *newArray_NicaCorrFit3DCFBowlerSinyukov(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit3DCFBowlerSinyukov[nElements] : new ::NicaCorrFit3DCFBowlerSinyukov[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit3DCFBowlerSinyukov(void *p) {
      delete ((::NicaCorrFit3DCFBowlerSinyukov*)p);
   }
   static void deleteArray_NicaCorrFit3DCFBowlerSinyukov(void *p) {
      delete [] ((::NicaCorrFit3DCFBowlerSinyukov*)p);
   }
   static void destruct_NicaCorrFit3DCFBowlerSinyukov(void *p) {
      typedef ::NicaCorrFit3DCFBowlerSinyukov current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit3DCFBowlerSinyukov

//______________________________________________________________________________
void NicaCorrFit3DCFBowlerSinyukovEllipse::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit3DCFBowlerSinyukovEllipse.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit3DCFBowlerSinyukovEllipse::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit3DCFBowlerSinyukovEllipse::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit3DCFBowlerSinyukovEllipse(void *p) {
      return  p ? new(p) ::NicaCorrFit3DCFBowlerSinyukovEllipse : new ::NicaCorrFit3DCFBowlerSinyukovEllipse;
   }
   static void *newArray_NicaCorrFit3DCFBowlerSinyukovEllipse(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit3DCFBowlerSinyukovEllipse[nElements] : new ::NicaCorrFit3DCFBowlerSinyukovEllipse[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit3DCFBowlerSinyukovEllipse(void *p) {
      delete ((::NicaCorrFit3DCFBowlerSinyukovEllipse*)p);
   }
   static void deleteArray_NicaCorrFit3DCFBowlerSinyukovEllipse(void *p) {
      delete [] ((::NicaCorrFit3DCFBowlerSinyukovEllipse*)p);
   }
   static void destruct_NicaCorrFit3DCFBowlerSinyukovEllipse(void *p) {
      typedef ::NicaCorrFit3DCFBowlerSinyukovEllipse current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit3DCFBowlerSinyukovEllipse

//______________________________________________________________________________
void NicaCorrFit3DCFBowlerSinyukovClassic::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit3DCFBowlerSinyukovClassic.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit3DCFBowlerSinyukovClassic::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit3DCFBowlerSinyukovClassic::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit3DCFBowlerSinyukovClassic(void *p) {
      return  p ? new(p) ::NicaCorrFit3DCFBowlerSinyukovClassic : new ::NicaCorrFit3DCFBowlerSinyukovClassic;
   }
   static void *newArray_NicaCorrFit3DCFBowlerSinyukovClassic(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit3DCFBowlerSinyukovClassic[nElements] : new ::NicaCorrFit3DCFBowlerSinyukovClassic[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit3DCFBowlerSinyukovClassic(void *p) {
      delete ((::NicaCorrFit3DCFBowlerSinyukovClassic*)p);
   }
   static void deleteArray_NicaCorrFit3DCFBowlerSinyukovClassic(void *p) {
      delete [] ((::NicaCorrFit3DCFBowlerSinyukovClassic*)p);
   }
   static void destruct_NicaCorrFit3DCFBowlerSinyukovClassic(void *p) {
      typedef ::NicaCorrFit3DCFBowlerSinyukovClassic current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit3DCFBowlerSinyukovClassic

//______________________________________________________________________________
void NicaFemtoFsiParsed::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoFsiParsed.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoFsiParsed::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoFsiParsed::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoFsiParsed(void *p) {
      return  p ? new(p) ::NicaFemtoFsiParsed : new ::NicaFemtoFsiParsed;
   }
   static void *newArray_NicaFemtoFsiParsed(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoFsiParsed[nElements] : new ::NicaFemtoFsiParsed[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoFsiParsed(void *p) {
      delete ((::NicaFemtoFsiParsed*)p);
   }
   static void deleteArray_NicaFemtoFsiParsed(void *p) {
      delete [] ((::NicaFemtoFsiParsed*)p);
   }
   static void destruct_NicaFemtoFsiParsed(void *p) {
      typedef ::NicaFemtoFsiParsed current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoFsiParsed

//______________________________________________________________________________
void NicaFemtoWeightGeneratorLednicky2::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoWeightGeneratorLednicky2.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoWeightGeneratorLednicky2::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoWeightGeneratorLednicky2::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoWeightGeneratorLednicky2(void *p) {
      return  p ? new(p) ::NicaFemtoWeightGeneratorLednicky2 : new ::NicaFemtoWeightGeneratorLednicky2;
   }
   static void *newArray_NicaFemtoWeightGeneratorLednicky2(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoWeightGeneratorLednicky2[nElements] : new ::NicaFemtoWeightGeneratorLednicky2[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoWeightGeneratorLednicky2(void *p) {
      delete ((::NicaFemtoWeightGeneratorLednicky2*)p);
   }
   static void deleteArray_NicaFemtoWeightGeneratorLednicky2(void *p) {
      delete [] ((::NicaFemtoWeightGeneratorLednicky2*)p);
   }
   static void destruct_NicaFemtoWeightGeneratorLednicky2(void *p) {
      typedef ::NicaFemtoWeightGeneratorLednicky2 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoWeightGeneratorLednicky2

//______________________________________________________________________________
void NicaFemtoWeightGeneratorBasic::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoWeightGeneratorBasic.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoWeightGeneratorBasic::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoWeightGeneratorBasic::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoWeightGeneratorBasic(void *p) {
      return  p ? new(p) ::NicaFemtoWeightGeneratorBasic : new ::NicaFemtoWeightGeneratorBasic;
   }
   static void *newArray_NicaFemtoWeightGeneratorBasic(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoWeightGeneratorBasic[nElements] : new ::NicaFemtoWeightGeneratorBasic[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoWeightGeneratorBasic(void *p) {
      delete ((::NicaFemtoWeightGeneratorBasic*)p);
   }
   static void deleteArray_NicaFemtoWeightGeneratorBasic(void *p) {
      delete [] ((::NicaFemtoWeightGeneratorBasic*)p);
   }
   static void destruct_NicaFemtoWeightGeneratorBasic(void *p) {
      typedef ::NicaFemtoWeightGeneratorBasic current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoWeightGeneratorBasic

//______________________________________________________________________________
void NicaFemtoWeightGeneratorLednicky::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoWeightGeneratorLednicky.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoWeightGeneratorLednicky::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoWeightGeneratorLednicky::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoWeightGeneratorLednicky(void *p) {
      return  p ? new(p) ::NicaFemtoWeightGeneratorLednicky : new ::NicaFemtoWeightGeneratorLednicky;
   }
   static void *newArray_NicaFemtoWeightGeneratorLednicky(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoWeightGeneratorLednicky[nElements] : new ::NicaFemtoWeightGeneratorLednicky[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoWeightGeneratorLednicky(void *p) {
      delete ((::NicaFemtoWeightGeneratorLednicky*)p);
   }
   static void deleteArray_NicaFemtoWeightGeneratorLednicky(void *p) {
      delete [] ((::NicaFemtoWeightGeneratorLednicky*)p);
   }
   static void destruct_NicaFemtoWeightGeneratorLednicky(void *p) {
      typedef ::NicaFemtoWeightGeneratorLednicky current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoWeightGeneratorLednicky

//______________________________________________________________________________
void NicaFemtoWeightGeneratorLednickyResidual::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoWeightGeneratorLednickyResidual.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoWeightGeneratorLednickyResidual::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoWeightGeneratorLednickyResidual::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoWeightGeneratorLednickyResidual(void *p) {
      return  p ? new(p) ::NicaFemtoWeightGeneratorLednickyResidual : new ::NicaFemtoWeightGeneratorLednickyResidual;
   }
   static void *newArray_NicaFemtoWeightGeneratorLednickyResidual(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoWeightGeneratorLednickyResidual[nElements] : new ::NicaFemtoWeightGeneratorLednickyResidual[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoWeightGeneratorLednickyResidual(void *p) {
      delete ((::NicaFemtoWeightGeneratorLednickyResidual*)p);
   }
   static void deleteArray_NicaFemtoWeightGeneratorLednickyResidual(void *p) {
      delete [] ((::NicaFemtoWeightGeneratorLednickyResidual*)p);
   }
   static void destruct_NicaFemtoWeightGeneratorLednickyResidual(void *p) {
      typedef ::NicaFemtoWeightGeneratorLednickyResidual current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoWeightGeneratorLednickyResidual

//______________________________________________________________________________
void NicaFemtoWeightGeneratorSupressed::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoWeightGeneratorSupressed.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoWeightGeneratorSupressed::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoWeightGeneratorSupressed::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoWeightGeneratorSupressed(void *p) {
      return  p ? new(p) ::NicaFemtoWeightGeneratorSupressed : new ::NicaFemtoWeightGeneratorSupressed;
   }
   static void *newArray_NicaFemtoWeightGeneratorSupressed(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoWeightGeneratorSupressed[nElements] : new ::NicaFemtoWeightGeneratorSupressed[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoWeightGeneratorSupressed(void *p) {
      delete ((::NicaFemtoWeightGeneratorSupressed*)p);
   }
   static void deleteArray_NicaFemtoWeightGeneratorSupressed(void *p) {
      delete [] ((::NicaFemtoWeightGeneratorSupressed*)p);
   }
   static void destruct_NicaFemtoWeightGeneratorSupressed(void *p) {
      typedef ::NicaFemtoWeightGeneratorSupressed current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoWeightGeneratorSupressed

//______________________________________________________________________________
void NicaFemtoCorrFuncPt::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoCorrFuncPt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoCorrFuncPt::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoCorrFuncPt::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaFemtoCorrFuncPt(void *p) {
      delete ((::NicaFemtoCorrFuncPt*)p);
   }
   static void deleteArray_NicaFemtoCorrFuncPt(void *p) {
      delete [] ((::NicaFemtoCorrFuncPt*)p);
   }
   static void destruct_NicaFemtoCorrFuncPt(void *p) {
      typedef ::NicaFemtoCorrFuncPt current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaFemtoCorrFuncPt(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaFemtoCorrFuncPt*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaFemtoCorrFuncPt

//______________________________________________________________________________
void NicaFemtoCorrFuncKt::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoCorrFuncKt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoCorrFuncKt::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoCorrFuncKt::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoCorrFuncKt(void *p) {
      return  p ? new(p) ::NicaFemtoCorrFuncKt : new ::NicaFemtoCorrFuncKt;
   }
   static void *newArray_NicaFemtoCorrFuncKt(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoCorrFuncKt[nElements] : new ::NicaFemtoCorrFuncKt[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoCorrFuncKt(void *p) {
      delete ((::NicaFemtoCorrFuncKt*)p);
   }
   static void deleteArray_NicaFemtoCorrFuncKt(void *p) {
      delete [] ((::NicaFemtoCorrFuncKt*)p);
   }
   static void destruct_NicaFemtoCorrFuncKt(void *p) {
      typedef ::NicaFemtoCorrFuncKt current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaFemtoCorrFuncKt(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaFemtoCorrFuncKt*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaFemtoCorrFuncKt

//______________________________________________________________________________
void NicaFemtoCorrFuncAzimuthalKt::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoCorrFuncAzimuthalKt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoCorrFuncAzimuthalKt::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoCorrFuncAzimuthalKt::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaFemtoCorrFuncAzimuthalKt(void *p) {
      delete ((::NicaFemtoCorrFuncAzimuthalKt*)p);
   }
   static void deleteArray_NicaFemtoCorrFuncAzimuthalKt(void *p) {
      delete [] ((::NicaFemtoCorrFuncAzimuthalKt*)p);
   }
   static void destruct_NicaFemtoCorrFuncAzimuthalKt(void *p) {
      typedef ::NicaFemtoCorrFuncAzimuthalKt current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaFemtoCorrFuncAzimuthalKt(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaFemtoCorrFuncAzimuthalKt*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaFemtoCorrFuncAzimuthalKt

//______________________________________________________________________________
void NicaFemtoCorrFuncSimple::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoCorrFuncSimple.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoCorrFuncSimple::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoCorrFuncSimple::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoCorrFuncSimple(void *p) {
      return  p ? new(p) ::NicaFemtoCorrFuncSimple : new ::NicaFemtoCorrFuncSimple;
   }
   static void *newArray_NicaFemtoCorrFuncSimple(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoCorrFuncSimple[nElements] : new ::NicaFemtoCorrFuncSimple[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoCorrFuncSimple(void *p) {
      delete ((::NicaFemtoCorrFuncSimple*)p);
   }
   static void deleteArray_NicaFemtoCorrFuncSimple(void *p) {
      delete [] ((::NicaFemtoCorrFuncSimple*)p);
   }
   static void destruct_NicaFemtoCorrFuncSimple(void *p) {
      typedef ::NicaFemtoCorrFuncSimple current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaFemtoCorrFuncSimple(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaFemtoCorrFuncSimple*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaFemtoCorrFuncSimple

//______________________________________________________________________________
void NicaFemtoImaging::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoImaging.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoImaging::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoImaging::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoImaging(void *p) {
      return  p ? new(p) ::NicaFemtoImaging : new ::NicaFemtoImaging;
   }
   static void *newArray_NicaFemtoImaging(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoImaging[nElements] : new ::NicaFemtoImaging[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoImaging(void *p) {
      delete ((::NicaFemtoImaging*)p);
   }
   static void deleteArray_NicaFemtoImaging(void *p) {
      delete [] ((::NicaFemtoImaging*)p);
   }
   static void destruct_NicaFemtoImaging(void *p) {
      typedef ::NicaFemtoImaging current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoImaging

//______________________________________________________________________________
void NicaFemtoImaging1D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoImaging1D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoImaging1D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoImaging1D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoImaging1D(void *p) {
      return  p ? new(p) ::NicaFemtoImaging1D : new ::NicaFemtoImaging1D;
   }
   static void *newArray_NicaFemtoImaging1D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoImaging1D[nElements] : new ::NicaFemtoImaging1D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoImaging1D(void *p) {
      delete ((::NicaFemtoImaging1D*)p);
   }
   static void deleteArray_NicaFemtoImaging1D(void *p) {
      delete [] ((::NicaFemtoImaging1D*)p);
   }
   static void destruct_NicaFemtoImaging1D(void *p) {
      typedef ::NicaFemtoImaging1D current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoImaging1D

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoImaging3D(void *p) {
      return  p ? new(p) ::NicaFemtoImaging3D : new ::NicaFemtoImaging3D;
   }
   static void *newArray_NicaFemtoImaging3D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoImaging3D[nElements] : new ::NicaFemtoImaging3D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoImaging3D(void *p) {
      delete ((::NicaFemtoImaging3D*)p);
   }
   static void deleteArray_NicaFemtoImaging3D(void *p) {
      delete [] ((::NicaFemtoImaging3D*)p);
   }
   static void destruct_NicaFemtoImaging3D(void *p) {
      typedef ::NicaFemtoImaging3D current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoImaging3D

//______________________________________________________________________________
void NicaFemtoSHCF::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoSHCF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoSHCF::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoSHCF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoSHCF(void *p) {
      return  p ? new(p) ::NicaFemtoSHCF : new ::NicaFemtoSHCF;
   }
   static void *newArray_NicaFemtoSHCF(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoSHCF[nElements] : new ::NicaFemtoSHCF[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoSHCF(void *p) {
      delete ((::NicaFemtoSHCF*)p);
   }
   static void deleteArray_NicaFemtoSHCF(void *p) {
      delete [] ((::NicaFemtoSHCF*)p);
   }
   static void destruct_NicaFemtoSHCF(void *p) {
      typedef ::NicaFemtoSHCF current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaFemtoSHCF(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaFemtoSHCF*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaFemtoSHCF

//______________________________________________________________________________
void NicaFemtoYlm::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFemtoYlm.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFemtoYlm::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFemtoYlm::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFemtoYlm(void *p) {
      return  p ? new(p) ::NicaFemtoYlm : new ::NicaFemtoYlm;
   }
   static void *newArray_NicaFemtoYlm(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFemtoYlm[nElements] : new ::NicaFemtoYlm[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFemtoYlm(void *p) {
      delete ((::NicaFemtoYlm*)p);
   }
   static void deleteArray_NicaFemtoYlm(void *p) {
      delete [] ((::NicaFemtoYlm*)p);
   }
   static void destruct_NicaFemtoYlm(void *p) {
      typedef ::NicaFemtoYlm current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFemtoYlm

//______________________________________________________________________________
void NicaCorrFitSHCF::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitSHCF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitSHCF::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitSHCF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitSHCF(void *p) {
      return  p ? new(p) ::NicaCorrFitSHCF : new ::NicaCorrFitSHCF;
   }
   static void *newArray_NicaCorrFitSHCF(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitSHCF[nElements] : new ::NicaCorrFitSHCF[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitSHCF(void *p) {
      delete ((::NicaCorrFitSHCF*)p);
   }
   static void deleteArray_NicaCorrFitSHCF(void *p) {
      delete [] ((::NicaCorrFitSHCF*)p);
   }
   static void destruct_NicaCorrFitSHCF(void *p) {
      typedef ::NicaCorrFitSHCF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitSHCF

//______________________________________________________________________________
void NicaCorrFitSmear1DCF::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitSmear1DCF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitSmear1DCF::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitSmear1DCF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitSmear1DCF(void *p) {
      return  p ? new(p) ::NicaCorrFitSmear1DCF : new ::NicaCorrFitSmear1DCF;
   }
   static void *newArray_NicaCorrFitSmear1DCF(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitSmear1DCF[nElements] : new ::NicaCorrFitSmear1DCF[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitSmear1DCF(void *p) {
      delete ((::NicaCorrFitSmear1DCF*)p);
   }
   static void deleteArray_NicaCorrFitSmear1DCF(void *p) {
      delete [] ((::NicaCorrFitSmear1DCF*)p);
   }
   static void destruct_NicaCorrFitSmear1DCF(void *p) {
      typedef ::NicaCorrFitSmear1DCF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitSmear1DCF

//______________________________________________________________________________
void NicaCorrFitDbParams::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitDbParams.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitDbParams::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitDbParams::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitDbParams(void *p) {
      return  p ? new(p) ::NicaCorrFitDbParams : new ::NicaCorrFitDbParams;
   }
   static void *newArray_NicaCorrFitDbParams(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitDbParams[nElements] : new ::NicaCorrFitDbParams[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitDbParams(void *p) {
      delete ((::NicaCorrFitDbParams*)p);
   }
   static void deleteArray_NicaCorrFitDbParams(void *p) {
      delete [] ((::NicaCorrFitDbParams*)p);
   }
   static void destruct_NicaCorrFitDbParams(void *p) {
      typedef ::NicaCorrFitDbParams current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitDbParams

//______________________________________________________________________________
void NicaCorrFitDbParamsSetup::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitDbParamsSetup.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitDbParamsSetup::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitDbParamsSetup::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitDbParamsSetup(void *p) {
      return  p ? new(p) ::NicaCorrFitDbParamsSetup : new ::NicaCorrFitDbParamsSetup;
   }
   static void *newArray_NicaCorrFitDbParamsSetup(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitDbParamsSetup[nElements] : new ::NicaCorrFitDbParamsSetup[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitDbParamsSetup(void *p) {
      delete ((::NicaCorrFitDbParamsSetup*)p);
   }
   static void deleteArray_NicaCorrFitDbParamsSetup(void *p) {
      delete [] ((::NicaCorrFitDbParamsSetup*)p);
   }
   static void destruct_NicaCorrFitDbParamsSetup(void *p) {
      typedef ::NicaCorrFitDbParamsSetup current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitDbParamsSetup

//______________________________________________________________________________
void NicaCorrFitDatabase::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitDatabase.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitDatabase::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitDatabase::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitDatabase(void *p) {
      return  p ? new(p) ::NicaCorrFitDatabase : new ::NicaCorrFitDatabase;
   }
   static void *newArray_NicaCorrFitDatabase(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitDatabase[nElements] : new ::NicaCorrFitDatabase[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitDatabase(void *p) {
      delete ((::NicaCorrFitDatabase*)p);
   }
   static void deleteArray_NicaCorrFitDatabase(void *p) {
      delete [] ((::NicaCorrFitDatabase*)p);
   }
   static void destruct_NicaCorrFitDatabase(void *p) {
      typedef ::NicaCorrFitDatabase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitDatabase

//______________________________________________________________________________
void NicaCorrFitDumpedPairAna::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitDumpedPairAna.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitDumpedPairAna::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitDumpedPairAna::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitDumpedPairAna(void *p) {
      return  p ? new(p) ::NicaCorrFitDumpedPairAna : new ::NicaCorrFitDumpedPairAna;
   }
   static void *newArray_NicaCorrFitDumpedPairAna(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitDumpedPairAna[nElements] : new ::NicaCorrFitDumpedPairAna[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitDumpedPairAna(void *p) {
      delete ((::NicaCorrFitDumpedPairAna*)p);
   }
   static void deleteArray_NicaCorrFitDumpedPairAna(void *p) {
      delete [] ((::NicaCorrFitDumpedPairAna*)p);
   }
   static void destruct_NicaCorrFitDumpedPairAna(void *p) {
      typedef ::NicaCorrFitDumpedPairAna current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitDumpedPairAna

//______________________________________________________________________________
void NicaCorrFitInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitInfo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitInfo::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitInfo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitInfo(void *p) {
      return  p ? new(p) ::NicaCorrFitInfo : new ::NicaCorrFitInfo;
   }
   static void *newArray_NicaCorrFitInfo(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitInfo[nElements] : new ::NicaCorrFitInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitInfo(void *p) {
      delete ((::NicaCorrFitInfo*)p);
   }
   static void deleteArray_NicaCorrFitInfo(void *p) {
      delete [] ((::NicaCorrFitInfo*)p);
   }
   static void destruct_NicaCorrFitInfo(void *p) {
      typedef ::NicaCorrFitInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitInfo

//______________________________________________________________________________
void NicaCorrFit3DCFMultiDim::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFit3DCFMultiDim.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFit3DCFMultiDim::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFit3DCFMultiDim::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFit3DCFMultiDim(void *p) {
      return  p ? new(p) ::NicaCorrFit3DCFMultiDim : new ::NicaCorrFit3DCFMultiDim;
   }
   static void *newArray_NicaCorrFit3DCFMultiDim(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFit3DCFMultiDim[nElements] : new ::NicaCorrFit3DCFMultiDim[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFit3DCFMultiDim(void *p) {
      delete ((::NicaCorrFit3DCFMultiDim*)p);
   }
   static void deleteArray_NicaCorrFit3DCFMultiDim(void *p) {
      delete [] ((::NicaCorrFit3DCFMultiDim*)p);
   }
   static void destruct_NicaCorrFit3DCFMultiDim(void *p) {
      typedef ::NicaCorrFit3DCFMultiDim current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFit3DCFMultiDim

//______________________________________________________________________________
void NicaCorrFitFunctor::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitFunctor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitFunctor::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitFunctor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitFunctor(void *p) {
      return  p ? new(p) ::NicaCorrFitFunctor : new ::NicaCorrFitFunctor;
   }
   static void *newArray_NicaCorrFitFunctor(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitFunctor[nElements] : new ::NicaCorrFitFunctor[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitFunctor(void *p) {
      delete ((::NicaCorrFitFunctor*)p);
   }
   static void deleteArray_NicaCorrFitFunctor(void *p) {
      delete [] ((::NicaCorrFitFunctor*)p);
   }
   static void destruct_NicaCorrFitFunctor(void *p) {
      typedef ::NicaCorrFitFunctor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitFunctor

//______________________________________________________________________________
void NicaCorrFitPairGeneratorYPt::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitPairGeneratorYPt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitPairGeneratorYPt::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitPairGeneratorYPt::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitPairGeneratorYPt(void *p) {
      return  p ? new(p) ::NicaCorrFitPairGeneratorYPt : new ::NicaCorrFitPairGeneratorYPt;
   }
   static void *newArray_NicaCorrFitPairGeneratorYPt(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitPairGeneratorYPt[nElements] : new ::NicaCorrFitPairGeneratorYPt[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitPairGeneratorYPt(void *p) {
      delete ((::NicaCorrFitPairGeneratorYPt*)p);
   }
   static void deleteArray_NicaCorrFitPairGeneratorYPt(void *p) {
      delete [] ((::NicaCorrFitPairGeneratorYPt*)p);
   }
   static void destruct_NicaCorrFitPairGeneratorYPt(void *p) {
      typedef ::NicaCorrFitPairGeneratorYPt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitPairGeneratorYPt

//______________________________________________________________________________
void NicaCorrFitMapRebin::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCorrFitMapRebin.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCorrFitMapRebin::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCorrFitMapRebin::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCorrFitMapRebin(void *p) {
      return  p ? new(p) ::NicaCorrFitMapRebin : new ::NicaCorrFitMapRebin;
   }
   static void *newArray_NicaCorrFitMapRebin(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCorrFitMapRebin[nElements] : new ::NicaCorrFitMapRebin[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCorrFitMapRebin(void *p) {
      delete ((::NicaCorrFitMapRebin*)p);
   }
   static void deleteArray_NicaCorrFitMapRebin(void *p) {
      delete [] ((::NicaCorrFitMapRebin*)p);
   }
   static void destruct_NicaCorrFitMapRebin(void *p) {
      typedef ::NicaCorrFitMapRebin current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCorrFitMapRebin

namespace ROOT {
   static TClass *vectorlENicaFitParamgR_Dictionary();
   static void vectorlENicaFitParamgR_TClassManip(TClass*);
   static void *new_vectorlENicaFitParamgR(void *p = nullptr);
   static void *newArray_vectorlENicaFitParamgR(Long_t size, void *p);
   static void delete_vectorlENicaFitParamgR(void *p);
   static void deleteArray_vectorlENicaFitParamgR(void *p);
   static void destruct_vectorlENicaFitParamgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<NicaFitParam>*)
   {
      vector<NicaFitParam> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<NicaFitParam>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<NicaFitParam>", -2, "vector", 389,
                  typeid(vector<NicaFitParam>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlENicaFitParamgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<NicaFitParam>) );
      instance.SetNew(&new_vectorlENicaFitParamgR);
      instance.SetNewArray(&newArray_vectorlENicaFitParamgR);
      instance.SetDelete(&delete_vectorlENicaFitParamgR);
      instance.SetDeleteArray(&deleteArray_vectorlENicaFitParamgR);
      instance.SetDestructor(&destruct_vectorlENicaFitParamgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<NicaFitParam> >()));

      ::ROOT::AddClassAlternate("vector<NicaFitParam>","std::vector<NicaFitParam, std::allocator<NicaFitParam> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<NicaFitParam>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlENicaFitParamgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<NicaFitParam>*)nullptr)->GetClass();
      vectorlENicaFitParamgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlENicaFitParamgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlENicaFitParamgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<NicaFitParam> : new vector<NicaFitParam>;
   }
   static void *newArray_vectorlENicaFitParamgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<NicaFitParam>[nElements] : new vector<NicaFitParam>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlENicaFitParamgR(void *p) {
      delete ((vector<NicaFitParam>*)p);
   }
   static void deleteArray_vectorlENicaFitParamgR(void *p) {
      delete [] ((vector<NicaFitParam>*)p);
   }
   static void destruct_vectorlENicaFitParamgR(void *p) {
      typedef vector<NicaFitParam> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<NicaFitParam>

namespace {
  void TriggerDictionaryInitialization_G__NicaFemtoDict_Impl() {
    static const char* headers[] = {
"NicaFemtoBasicAna.h",
"NicaFemtoFreezoutsAna.h",
"NicaFemtoDumpPairAna.h",
"NicaFemtoEventBinsAna.h",
"NicaFemtoDPhiDEta.h",
"NicaFemto1DCF.h",
"NicaFemto3DCF.h",
"NicaFemto3DCFQinv.h",
"NicaFemtoFastCut.h",
"NicaFemtoFreezoutGenerator.h",
"NicaFemtoPairKinematics.h",
"NicaFemtoPair.h",
"NicaFemtoMiniPair.h",
"NicaFemtoSourceModel.h",
"NicaFemtoConst.h",
"NicaFemtoSourceModelExp.h",
"NicaFemtoSourceModelGauss.h",
"NicaFemtoSourceModelGausMt.h",
"NicaCorrFit.h",
"NicaCorrFitFunc.h",
"NicaCorrFitMath1DCF.h",
"NicaCorrFitMath3DCF.h",
"NicaCorrFit1DCF.h",
"NicaCorrFit3DCF.h",
"NicaCorrFitKisiel.h",
"NicaCorrFitWielanek.h",
"NicaCorrFitGUI.h",
"NicaCorrFitHDFunc.h",
"NicaFemto1DCFAnaMap.h",
"NicaFemto1DCFAnaMapMC.h",
"NicaWielanekMapGenerator.h",
"NicaFemto1DCFAnaMapPairsDumped.h",
"NicaCorrFit1DCFSquare.h",
"NicaCorrFit1DCFLorentz.h",
"NicaCorrFit3DCFSquare.h",
"NicaCorrFit3DCFLorentz.h",
"NicaCorrFit1DCFExp.h",
"NicaCorrFit1DCFGauss.h",
"NicaCorrFit1DCFPoly.h",
"NicaCorrFit3DCFGauss.h",
"NicaCorrFit3DCFGauss2.h",
"NicaCorrFit3DCFGaussCross.h",
"NicaCorrFit1DCFCumac.h",
"NicaCorrFit3DCFBowlerSinyukov.h",
"NicaCorrFit3DCFBowlerSinyukovExtra.h",
"NicaFemtoFsiParsed.h",
"NicaFemtoWeightGeneratorLednicky2.h",
"NicaFemtoWeightGenerator.h",
"NicaFemtoWeightGeneratorBasic.h",
"NicaFemtoWeightGeneratorLednicky.h",
"NicaFemtoWeightGeneratorLednickyResidual.h",
"NicaFemtoWeightGeneratorSupressed.h",
"NicaFemtoCorrFunc.h",
"NicaFemtoCorrFuncPt.h",
"NicaFemtoCorrFuncKt.h",
"NicaFemtoCorrFuncAzimuthalKt.h",
"NicaFemtoCorrFuncSimple.h",
"NicaFemtoImaging.h",
"NicaFemtoImaging1D.h",
"NicaFemtoImaging3D.h",
"NicaFemtoSHCF.h",
"NicaFemtoYlm.h",
"NicaCorrFitSHCF.h",
"NicaCorrFitSmear1DCF.h",
"NicaCorrFitDatabase.h",
"NicaCorrFitDumpedPairAna.h",
"NicaCorrFitInfo.h",
"NicaCorrFit3DCFMultiDim.h",
"NicaCorrFitFunctor.h",
"NicaCorrFitPairGeneratorYPt.h",
"NicaCorrFitMapRebin.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/detector",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/compound",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/hiddeninfo",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/hist",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io/html",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/std",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/minimizer",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/chains",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly/smear_codes",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/ana",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/weights",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerators",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerator_multidim",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/imaging",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/source_models",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/corrfunc",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto",
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
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/analysis/femto/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__NicaFemtoDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$NicaFemtoCorrFunc.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoBasicAna.h")))  NicaFemtoCorrFunc;
class __attribute__((annotate("$clingAutoload$NicaFemtoCorrFunc.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoBasicAna.h")))  NicaFemtoCorrFunc1D;
class __attribute__((annotate("$clingAutoload$NicaFemtoCorrFunc.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoBasicAna.h")))  NicaFemtoCorrFunc2D;
class __attribute__((annotate("$clingAutoload$NicaFemtoSourceModel.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoBasicAna.h")))  NicaFemtoSourceModel;
class __attribute__((annotate("$clingAutoload$NicaFemtoSourceModel.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoBasicAna.h")))  NicaFemtoSourceModel1D;
class __attribute__((annotate("$clingAutoload$NicaFemtoSourceModel.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoBasicAna.h")))  NicaFemtoSourceModel3D;
class __attribute__((annotate("$clingAutoload$NicaFemtoFreezoutGenerator.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoBasicAna.h")))  NicaFemtoFreezoutGenerator;
class __attribute__((annotate("$clingAutoload$NicaFemtoFreezoutGenerator.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoBasicAna.h")))  NicaFemtoFreezoutGeneratorLCMS;
class __attribute__((annotate("$clingAutoload$NicaFemtoFreezoutGenerator.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoBasicAna.h")))  NicaFemtoFreezoutGeneratorPRF;
class __attribute__((annotate("$clingAutoload$NicaFemtoWeightGenerator.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoBasicAna.h")))  NicaFemtoWeightGenerator;
class __attribute__((annotate("$clingAutoload$NicaFemtoBasicAna.h")))  NicaFemtoBasicAna;
class __attribute__((annotate("$clingAutoload$NicaFemtoFastCut.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoFreezoutsAna.h")))  NicaFemtoFastCut;
class __attribute__((annotate("$clingAutoload$NicaFemtoFastCut.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoFreezoutsAna.h")))  NicaFemtoFastCutVirtual;
class __attribute__((annotate("$clingAutoload$NicaFemtoFastCut.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoFreezoutsAna.h")))  NicaFemtoFastCutKt;
class __attribute__((annotate("$clingAutoload$NicaFemtoFastCut.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoFreezoutsAna.h")))  NicaFemtoFastCutPtSum;
class __attribute__((annotate("$clingAutoload$NicaFemtoFastCut.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoFreezoutsAna.h")))  NicaFemtoFastCutPhi;
class __attribute__((annotate("$clingAutoload$NicaFemtoFastCut.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoFreezoutsAna.h")))  NicaFemtoFastCutEta;
class __attribute__((annotate("$clingAutoload$NicaFemtoFreezoutsAna.h")))  NicaFemtoFreezoutsAna;
class __attribute__((annotate("$clingAutoload$NicaFemtoPair.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoDumpPairAna.h")))  NicaFemtoPair;
class __attribute__((annotate("$clingAutoload$NicaFemtoMiniPair.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoDumpPairAna.h")))  NicaFemtoPicoPair;
class __attribute__((annotate("$clingAutoload$NicaFemtoMiniPair.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoDumpPairAna.h")))  NicaFemtoMicroPair;
class __attribute__((annotate("$clingAutoload$NicaFemtoMiniPair.h")))  __attribute__((annotate("$clingAutoload$NicaFemtoDumpPairAna.h")))  NicaFemtoMiniPair;
class __attribute__((annotate("$clingAutoload$NicaFemtoDumpPairAna.h")))  NicaFemtoDumpPairAna;
class __attribute__((annotate("$clingAutoload$NicaFemtoEventBinsAna.h")))  NicaFemtoEventBinsAna;
class __attribute__((annotate("$clingAutoload$NicaFemtoDPhiDEta.h")))  NicaFemtoDPhiDEta;
class __attribute__((annotate("$clingAutoload$NicaFemto1DCF.h")))  NicaFemto1DCF;
class __attribute__((annotate("$clingAutoload$NicaFemto3DCF.h")))  NicaFemto3DCF;
class __attribute__((annotate("$clingAutoload$NicaFemto3DCFQinv.h")))  NicaFemto3DCFQinv;
class __attribute__((annotate("$clingAutoload$NicaFemtoPairKinematics.h")))  NicaFemtoPairPRF;
class __attribute__((annotate("$clingAutoload$NicaFemtoPairKinematics.h")))  NicaFemtoPair_SH;
class __attribute__((annotate("$clingAutoload$NicaFemtoPairKinematics.h")))  NicaFemtoPairLCMS;
class __attribute__((annotate("$clingAutoload$NicaFemtoPairKinematics.h")))  NicaFemtoPairPRF_SH;
class __attribute__((annotate("$clingAutoload$NicaFemtoPairKinematics.h")))  NicaFemtoPairLCMS_SH;
class __attribute__((annotate("$clingAutoload$NicaFemtoPairKinematics.h")))  NicaFemtoPairDPhiDEta;
class __attribute__((annotate("$clingAutoload$NicaFemtoPairKinematics.h")))  NicaFemtoPairPRFL;
class __attribute__((annotate("$clingAutoload$NicaFemtoSourceModelExp.h")))  NicaFemtoSourceModelExp1D;
class __attribute__((annotate("$clingAutoload$NicaFemtoSourceModelGauss.h")))  NicaFemtoSourceModelGauss1D;
class __attribute__((annotate("$clingAutoload$NicaFemtoSourceModelGauss.h")))  NicaFemtoSourceModelGauss3D;
class __attribute__((annotate("$clingAutoload$NicaFemtoSourceModelGausMt.h")))  NicaFemtoSourceModelGausMt;
class __attribute__((annotate("$clingAutoload$NicaCorrFit.h")))  NicaCorrFit;
class __attribute__((annotate("$clingAutoload$NicaCorrFitFunc.h")))  NicaCorrFitFunc;
class __attribute__((annotate("$clingAutoload$NicaCorrFit1DCF.h")))  __attribute__((annotate("$clingAutoload$NicaCorrFitMath1DCF.h")))  NicaCorrFit1DCF;
class __attribute__((annotate("$clingAutoload$NicaCorrFitMath1DCF.h")))  NicaCorrFitMath1DCF;
class __attribute__((annotate("$clingAutoload$NicaCorrFitMath1DCF.h")))  NicaCorrFit1DCF_Sum;
class __attribute__((annotate("$clingAutoload$NicaCorrFitMath1DCF.h")))  NicaCorrFit1DCF_SumRsame;
class __attribute__((annotate("$clingAutoload$NicaCorrFitMath1DCF.h")))  NicaCorrFit1DCF_Multi;
class __attribute__((annotate("$clingAutoload$NicaCorrFit3DCF.h")))  __attribute__((annotate("$clingAutoload$NicaCorrFitMath3DCF.h")))  NicaCorrFit3DCF;
class __attribute__((annotate("$clingAutoload$NicaCorrFitMath3DCF.h")))  NicaCorrFitMath3DCF;
class __attribute__((annotate("$clingAutoload$NicaCorrFitMath3DCF.h")))  NicaCorrFit3DCF_Sum;
class __attribute__((annotate("$clingAutoload$NicaCorrFitKisiel.h")))  NicaCorrFitKisiel;
class __attribute__((annotate("$clingAutoload$NicaCorrFitWielanek.h")))  NicaCorrFitWielanek;
class __attribute__((annotate("$clingAutoload$NicaCorrFitGUI.h")))  NicaCorrFitParButton;
class __attribute__((annotate("$clingAutoload$NicaCorrFitGUI.h")))  NicaCorrFitChiSelector;
class __attribute__((annotate("$clingAutoload$NicaCorrFitGUI.h")))  NicaCorrFitGUI;
class __attribute__((annotate("$clingAutoload$NicaCorrFitHDFunc.h")))  NicaCorrFitHDFunc;
class __attribute__((annotate("$clingAutoload$NicaCorrFitHDFunc.h")))  NicaCorrFitHDFunc1D;
class __attribute__((annotate("$clingAutoload$NicaCorrFitHDFunc.h")))  NicaCorrFitHDFunc3D;
class __attribute__((annotate("$clingAutoload$NicaFemto1DCFAnaMap.h")))  NicaFemtoCorrFuncKtMap1D;
class __attribute__((annotate("$clingAutoload$NicaFemto1DCFAnaMap.h")))  NicaFemto1DCFAnaMap;
class __attribute__((annotate("$clingAutoload$NicaFemto1DCFAnaMapMC.h")))  NicaFemto1DCFAnaMapMC;
class __attribute__((annotate("$clingAutoload$NicaWielanekMapGenerator.h")))  NicaWielanekMapGenerator;
class __attribute__((annotate("$clingAutoload$NicaFemto1DCFAnaMapPairsDumped.h")))  NicaFemto1DCFAnaMapPairsDumped;
class __attribute__((annotate("$clingAutoload$NicaCorrFit1DCFSquare.h")))  NicaCorrFit1DCF_Square;
class __attribute__((annotate("$clingAutoload$NicaCorrFit1DCFLorentz.h")))  NicaCorrFit1DCF_Lorentz;
class __attribute__((annotate("$clingAutoload$NicaCorrFit3DCFSquare.h")))  NicaCorrFit3DCF_Square;
class __attribute__((annotate("$clingAutoload$NicaCorrFit3DCFLorentz.h")))  NicaCorrFit3DCF_Lorentz;
class __attribute__((annotate("$clingAutoload$NicaCorrFit1DCFExp.h")))  NicaCorrFit1DCF_Exp;
class __attribute__((annotate("$clingAutoload$NicaCorrFit1DCFGauss.h")))  NicaCorrFit1DCF_Gauss;
class __attribute__((annotate("$clingAutoload$NicaCorrFit1DCFPoly.h")))  NicaCorrFit1DCF_Poly;
class __attribute__((annotate("$clingAutoload$NicaCorrFit1DCFPoly.h")))  NicaCorrFit1DCF_Poly2;
class __attribute__((annotate("$clingAutoload$NicaCorrFit3DCFGauss.h")))  NicaCorrFit3DCF_Gauss;
class __attribute__((annotate("$clingAutoload$NicaCorrFit3DCFGauss2.h")))  NicaCorrFit3DCF_Gauss2;
class __attribute__((annotate("$clingAutoload$NicaCorrFit3DCFGaussCross.h")))  NicaCorrFit3DCF_GaussCross;
class __attribute__((annotate("$clingAutoload$NicaCorrFit1DCFCumac.h")))  NicaCorrFit1DCFCumac;
class __attribute__((annotate("$clingAutoload$NicaCorrFit1DCFCumac.h")))  NicaCorrFIt1DCFCumacLamLam;
class __attribute__((annotate("$clingAutoload$NicaCorrFit1DCFCumac.h")))  NicaCorrFit1DCFCumacPLam;
class __attribute__((annotate("$clingAutoload$NicaCorrFit1DCFCumac.h")))  NicaCorrFit1DCFCumacK0K0;
class __attribute__((annotate("$clingAutoload$NicaCorrFit3DCFBowlerSinyukov.h")))  NicaCorrFit3DCFBowlerSinyukov;
class __attribute__((annotate("$clingAutoload$NicaCorrFit3DCFBowlerSinyukovExtra.h")))  NicaCorrFit3DCFBowlerSinyukovEllipse;
class __attribute__((annotate("$clingAutoload$NicaCorrFit3DCFBowlerSinyukovExtra.h")))  NicaCorrFit3DCFBowlerSinyukovClassic;
class __attribute__((annotate("$clingAutoload$NicaFemtoFsiParsed.h")))  NicaFemtoFsiParsed;
class __attribute__((annotate("$clingAutoload$NicaFemtoWeightGeneratorLednicky2.h")))  NicaFemtoWeightGeneratorLednicky2;
class __attribute__((annotate("$clingAutoload$NicaFemtoWeightGeneratorBasic.h")))  NicaFemtoWeightGeneratorBasic;
class __attribute__((annotate("$clingAutoload$NicaFemtoWeightGeneratorLednicky.h")))  NicaFemtoWeightGeneratorLednicky;
class __attribute__((annotate("$clingAutoload$NicaFemtoWeightGeneratorLednickyResidual.h")))  NicaFemtoWeightGeneratorLednickyResidual;
class __attribute__((annotate("$clingAutoload$NicaFemtoWeightGeneratorSupressed.h")))  NicaFemtoWeightGeneratorSupressed;
class __attribute__((annotate("$clingAutoload$NicaFemtoCorrFuncPt.h")))  NicaFemtoCorrFuncPt;
class __attribute__((annotate("$clingAutoload$NicaFemtoCorrFuncKt.h")))  NicaFemtoCorrFuncKt;
class __attribute__((annotate("$clingAutoload$NicaFemtoCorrFuncAzimuthalKt.h")))  NicaFemtoCorrFuncAzimuthalKt;
class __attribute__((annotate("$clingAutoload$NicaFemtoCorrFuncSimple.h")))  NicaFemtoCorrFuncSimple;
class __attribute__((annotate("$clingAutoload$NicaFemtoImaging.h")))  NicaFemtoImaging;
class __attribute__((annotate("$clingAutoload$NicaFemtoImaging1D.h")))  NicaFemtoImaging1D;
class __attribute__((annotate("$clingAutoload$NicaFemtoImaging3D.h")))  NicaFemtoImaging3D;
class __attribute__((annotate("$clingAutoload$NicaFemtoSHCF.h")))  NicaFemtoSHCF;
class __attribute__((annotate("$clingAutoload$NicaFemtoYlm.h")))  NicaFemtoYlm;
class __attribute__((annotate("$clingAutoload$NicaCorrFitSHCF.h")))  NicaCorrFitSHCF;
class __attribute__((annotate("$clingAutoload$NicaCorrFitSmear1DCF.h")))  NicaCorrFitSmear1DCF;
class __attribute__((annotate("$clingAutoload$NicaCorrFitDatabase.h")))  NicaCorrFitDbParams;
class __attribute__((annotate("$clingAutoload$NicaCorrFitDatabase.h")))  NicaCorrFitDbParamsSetup;
class __attribute__((annotate("$clingAutoload$NicaCorrFitDatabase.h")))  NicaCorrFitDatabase;
class __attribute__((annotate("$clingAutoload$NicaCorrFitDumpedPairAna.h")))  NicaCorrFitDumpedPairAna;
class __attribute__((annotate("$clingAutoload$NicaCorrFitInfo.h")))  NicaCorrFitInfo;
class __attribute__((annotate("$clingAutoload$NicaCorrFit3DCFMultiDim.h")))  NicaCorrFit3DCFMultiDim;
class __attribute__((annotate("$clingAutoload$NicaCorrFitFunctor.h")))  NicaCorrFitFunctor;
class __attribute__((annotate("$clingAutoload$NicaCorrFitPairGeneratorYPt.h")))  NicaCorrFitPairGeneratorYPt;
class __attribute__((annotate("$clingAutoload$NicaCorrFitMapRebin.h")))  NicaCorrFitMapRebin;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__NicaFemtoDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "NicaFemtoBasicAna.h"
#include "NicaFemtoFreezoutsAna.h"
#include "NicaFemtoDumpPairAna.h"
#include "NicaFemtoEventBinsAna.h"
#include "NicaFemtoDPhiDEta.h"
#include "NicaFemto1DCF.h"
#include "NicaFemto3DCF.h"
#include "NicaFemto3DCFQinv.h"
#include "NicaFemtoFastCut.h"
#include "NicaFemtoFreezoutGenerator.h"
#include "NicaFemtoPairKinematics.h"
#include "NicaFemtoPair.h"
#include "NicaFemtoMiniPair.h"
#include "NicaFemtoSourceModel.h"
#include "NicaFemtoConst.h"
#include "NicaFemtoSourceModelExp.h"
#include "NicaFemtoSourceModelGauss.h"
#include "NicaFemtoSourceModelGausMt.h"
#include "NicaCorrFit.h"
#include "NicaCorrFitFunc.h"
#include "NicaCorrFitMath1DCF.h"
#include "NicaCorrFitMath3DCF.h"
#include "NicaCorrFit1DCF.h"
#include "NicaCorrFit3DCF.h"
#include "NicaCorrFitKisiel.h"
#include "NicaCorrFitWielanek.h"
#include "NicaCorrFitGUI.h"
#include "NicaCorrFitHDFunc.h"
#include "NicaFemto1DCFAnaMap.h"
#include "NicaFemto1DCFAnaMapMC.h"
#include "NicaWielanekMapGenerator.h"
#include "NicaFemto1DCFAnaMapPairsDumped.h"
#include "NicaCorrFit1DCFSquare.h"
#include "NicaCorrFit1DCFLorentz.h"
#include "NicaCorrFit3DCFSquare.h"
#include "NicaCorrFit3DCFLorentz.h"
#include "NicaCorrFit1DCFExp.h"
#include "NicaCorrFit1DCFGauss.h"
#include "NicaCorrFit1DCFPoly.h"
#include "NicaCorrFit3DCFGauss.h"
#include "NicaCorrFit3DCFGauss2.h"
#include "NicaCorrFit3DCFGaussCross.h"
#include "NicaCorrFit1DCFCumac.h"
#include "NicaCorrFit3DCFBowlerSinyukov.h"
#include "NicaCorrFit3DCFBowlerSinyukovExtra.h"
#include "NicaFemtoFsiParsed.h"
#include "NicaFemtoWeightGeneratorLednicky2.h"
#include "NicaFemtoWeightGenerator.h"
#include "NicaFemtoWeightGeneratorBasic.h"
#include "NicaFemtoWeightGeneratorLednicky.h"
#include "NicaFemtoWeightGeneratorLednickyResidual.h"
#include "NicaFemtoWeightGeneratorSupressed.h"
#include "NicaFemtoCorrFunc.h"
#include "NicaFemtoCorrFuncPt.h"
#include "NicaFemtoCorrFuncKt.h"
#include "NicaFemtoCorrFuncAzimuthalKt.h"
#include "NicaFemtoCorrFuncSimple.h"
#include "NicaFemtoImaging.h"
#include "NicaFemtoImaging1D.h"
#include "NicaFemtoImaging3D.h"
#include "NicaFemtoSHCF.h"
#include "NicaFemtoYlm.h"
#include "NicaCorrFitSHCF.h"
#include "NicaCorrFitSmear1DCF.h"
#include "NicaCorrFitDatabase.h"
#include "NicaCorrFitDumpedPairAna.h"
#include "NicaCorrFitInfo.h"
#include "NicaCorrFit3DCFMultiDim.h"
#include "NicaCorrFitFunctor.h"
#include "NicaCorrFitPairGeneratorYPt.h"
#include "NicaCorrFitMapRebin.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"ENicaFemtoKinematics", payloadCode, "@",
"ENicaFemtoPairType", payloadCode, "@",
"NicaCorrFIt1DCFCumacLamLam", payloadCode, "@",
"NicaCorrFit", payloadCode, "@",
"NicaCorrFit1DCF", payloadCode, "@",
"NicaCorrFit1DCFCumac", payloadCode, "@",
"NicaCorrFit1DCFCumacK0K0", payloadCode, "@",
"NicaCorrFit1DCFCumacPLam", payloadCode, "@",
"NicaCorrFit1DCF_Exp", payloadCode, "@",
"NicaCorrFit1DCF_Gauss", payloadCode, "@",
"NicaCorrFit1DCF_Lorentz", payloadCode, "@",
"NicaCorrFit1DCF_Multi", payloadCode, "@",
"NicaCorrFit1DCF_Poly", payloadCode, "@",
"NicaCorrFit1DCF_Poly2", payloadCode, "@",
"NicaCorrFit1DCF_Square", payloadCode, "@",
"NicaCorrFit1DCF_Sum", payloadCode, "@",
"NicaCorrFit1DCF_SumRsame", payloadCode, "@",
"NicaCorrFit3DCF", payloadCode, "@",
"NicaCorrFit3DCFBowlerSinyukov", payloadCode, "@",
"NicaCorrFit3DCFBowlerSinyukovClassic", payloadCode, "@",
"NicaCorrFit3DCFBowlerSinyukovEllipse", payloadCode, "@",
"NicaCorrFit3DCFMultiDim", payloadCode, "@",
"NicaCorrFit3DCF_Gauss", payloadCode, "@",
"NicaCorrFit3DCF_Gauss2", payloadCode, "@",
"NicaCorrFit3DCF_GaussCross", payloadCode, "@",
"NicaCorrFit3DCF_Lorentz", payloadCode, "@",
"NicaCorrFit3DCF_Square", payloadCode, "@",
"NicaCorrFit3DCF_Sum", payloadCode, "@",
"NicaCorrFitChiSelector", payloadCode, "@",
"NicaCorrFitDatabase", payloadCode, "@",
"NicaCorrFitDbParams", payloadCode, "@",
"NicaCorrFitDbParamsSetup", payloadCode, "@",
"NicaCorrFitDumpedPairAna", payloadCode, "@",
"NicaCorrFitFunc", payloadCode, "@",
"NicaCorrFitFunctor", payloadCode, "@",
"NicaCorrFitGUI", payloadCode, "@",
"NicaCorrFitHDFunc", payloadCode, "@",
"NicaCorrFitHDFunc1D", payloadCode, "@",
"NicaCorrFitHDFunc3D", payloadCode, "@",
"NicaCorrFitInfo", payloadCode, "@",
"NicaCorrFitKisiel", payloadCode, "@",
"NicaCorrFitMapRebin", payloadCode, "@",
"NicaCorrFitMath1DCF", payloadCode, "@",
"NicaCorrFitMath3DCF", payloadCode, "@",
"NicaCorrFitPairGeneratorYPt", payloadCode, "@",
"NicaCorrFitParButton", payloadCode, "@",
"NicaCorrFitSHCF", payloadCode, "@",
"NicaCorrFitSmear1DCF", payloadCode, "@",
"NicaCorrFitWielanek", payloadCode, "@",
"NicaFemto1DCF", payloadCode, "@",
"NicaFemto1DCFAnaMap", payloadCode, "@",
"NicaFemto1DCFAnaMapMC", payloadCode, "@",
"NicaFemto1DCFAnaMapPairsDumped", payloadCode, "@",
"NicaFemto3DCF", payloadCode, "@",
"NicaFemto3DCFQinv", payloadCode, "@",
"NicaFemtoBasicAna", payloadCode, "@",
"NicaFemtoCorrFunc", payloadCode, "@",
"NicaFemtoCorrFunc1D", payloadCode, "@",
"NicaFemtoCorrFunc2D", payloadCode, "@",
"NicaFemtoCorrFuncAzimuthalKt", payloadCode, "@",
"NicaFemtoCorrFuncKt", payloadCode, "@",
"NicaFemtoCorrFuncKtMap1D", payloadCode, "@",
"NicaFemtoCorrFuncPt", payloadCode, "@",
"NicaFemtoCorrFuncSimple", payloadCode, "@",
"NicaFemtoDPhiDEta", payloadCode, "@",
"NicaFemtoDumpPairAna", payloadCode, "@",
"NicaFemtoEventBinsAna", payloadCode, "@",
"NicaFemtoFastCut", payloadCode, "@",
"NicaFemtoFastCutEta", payloadCode, "@",
"NicaFemtoFastCutKt", payloadCode, "@",
"NicaFemtoFastCutPhi", payloadCode, "@",
"NicaFemtoFastCutPtSum", payloadCode, "@",
"NicaFemtoFastCutVirtual", payloadCode, "@",
"NicaFemtoFreezoutGenerator", payloadCode, "@",
"NicaFemtoFreezoutGeneratorLCMS", payloadCode, "@",
"NicaFemtoFreezoutGeneratorPRF", payloadCode, "@",
"NicaFemtoFreezoutsAna", payloadCode, "@",
"NicaFemtoFsiParsed", payloadCode, "@",
"NicaFemtoImaging", payloadCode, "@",
"NicaFemtoImaging1D", payloadCode, "@",
"NicaFemtoImaging3D", payloadCode, "@",
"NicaFemtoMicroPair", payloadCode, "@",
"NicaFemtoMiniPair", payloadCode, "@",
"NicaFemtoPair", payloadCode, "@",
"NicaFemtoPairDPhiDEta", payloadCode, "@",
"NicaFemtoPairLCMS", payloadCode, "@",
"NicaFemtoPairLCMS_SH", payloadCode, "@",
"NicaFemtoPairPRF", payloadCode, "@",
"NicaFemtoPairPRFL", payloadCode, "@",
"NicaFemtoPairPRF_SH", payloadCode, "@",
"NicaFemtoPair_SH", payloadCode, "@",
"NicaFemtoPicoPair", payloadCode, "@",
"NicaFemtoSHCF", payloadCode, "@",
"NicaFemtoSourceModel", payloadCode, "@",
"NicaFemtoSourceModel1D", payloadCode, "@",
"NicaFemtoSourceModel3D", payloadCode, "@",
"NicaFemtoSourceModelExp1D", payloadCode, "@",
"NicaFemtoSourceModelGausMt", payloadCode, "@",
"NicaFemtoSourceModelGauss1D", payloadCode, "@",
"NicaFemtoSourceModelGauss3D", payloadCode, "@",
"NicaFemtoWeightGenerator", payloadCode, "@",
"NicaFemtoWeightGeneratorBasic", payloadCode, "@",
"NicaFemtoWeightGeneratorLednicky", payloadCode, "@",
"NicaFemtoWeightGeneratorLednicky2", payloadCode, "@",
"NicaFemtoWeightGeneratorLednickyResidual", payloadCode, "@",
"NicaFemtoWeightGeneratorSupressed", payloadCode, "@",
"NicaFemtoYlm", payloadCode, "@",
"NicaWielanekMapGenerator", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__NicaFemtoDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__NicaFemtoDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__NicaFemtoDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__NicaFemtoDict() {
  TriggerDictionaryInitialization_G__NicaFemtoDict_Impl();
}
