// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIheinemanndIgpuunpackerdIcbmrootdIbuild2dIexternaldINICAFEMTOmIprefixdIsrcdINICAFEMTOmIbuilddIfeaturesdIG__NicaFeaturesDict
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
#include "NicaCout.h"
#include "NicaArray.h"
#include "NicaOptions.h"
#include "NicaParameter.h"
#include "NicaStd.h"
#include "NicaObjectMatrix.h"
#include "NicaHelix.h"
#include "NicaHelixBase.h"
#include "NicaLink.h"
#include "NicaConst.h"
#include "NicaXMLNode.h"
#include "NicaJobs.h"
#include "NicaPack.h"
#include "NicaPackage.h"
#include "NicaPackage2HTML.h"
#include "NicaAnaFile.h"
#include "NicaHtmlCore.h"
#include "NicaPackageSql.h"
#include "NicaHtmlDiv.h"
#include "NicaHtmlFile.h"
#include "NicaHtmlObject.h"
#include "NicaHtmlTable.h"
#include "NicaDividedHisto.h"
#include "NicaHistogramManager.h"
#include "NicaMultiGraph.h"
#include "NicaSplines.h"
#include "NicaChiSqMap2D.h"
#include "NicaStdHist.h"
#include "NicaStdString.h"
#include "NicaStdMath.h"
#include "NicaFitParam.h"
#include "NicaMinimizer.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace NicaStd {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *NicaStd_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("NicaStd", 0 /*version*/, "NicaStdHist.h", 21,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &NicaStd_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *NicaStd_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_NicaCout(void *p = nullptr);
   static void *newArray_NicaCout(Long_t size, void *p);
   static void delete_NicaCout(void *p);
   static void deleteArray_NicaCout(void *p);
   static void destruct_NicaCout(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaCout*)
   {
      ::NicaCout *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaCout >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaCout", ::NicaCout::Class_Version(), "NicaCout.h", 18,
                  typeid(::NicaCout), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaCout::Dictionary, isa_proxy, 4,
                  sizeof(::NicaCout) );
      instance.SetNew(&new_NicaCout);
      instance.SetNewArray(&newArray_NicaCout);
      instance.SetDelete(&delete_NicaCout);
      instance.SetDeleteArray(&deleteArray_NicaCout);
      instance.SetDestructor(&destruct_NicaCout);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaCout*)
   {
      return GenerateInitInstanceLocal((::NicaCout*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaCout*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_1lEboolgR_Dictionary();
   static void NicaArray_1lEboolgR_TClassManip(TClass*);
   static void *new_NicaArray_1lEboolgR(void *p = nullptr);
   static void *newArray_NicaArray_1lEboolgR(Long_t size, void *p);
   static void delete_NicaArray_1lEboolgR(void *p);
   static void deleteArray_NicaArray_1lEboolgR(void *p);
   static void destruct_NicaArray_1lEboolgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_1<bool>*)
   {
      ::NicaArray_1<bool> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_1<bool> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_1<bool>", ::NicaArray_1<bool>::Class_Version(), "NicaArray.h", 18,
                  typeid(::NicaArray_1<bool>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_1lEboolgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_1<bool>) );
      instance.SetNew(&new_NicaArray_1lEboolgR);
      instance.SetNewArray(&newArray_NicaArray_1lEboolgR);
      instance.SetDelete(&delete_NicaArray_1lEboolgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_1lEboolgR);
      instance.SetDestructor(&destruct_NicaArray_1lEboolgR);

      ::ROOT::AddClassAlternate("NicaArray_1<bool>","NicaArray_1<Bool_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_1<bool>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_1<bool>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_1<bool>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_1lEboolgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<bool>*)nullptr)->GetClass();
      NicaArray_1lEboolgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_1lEboolgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_1lEdoublegR_Dictionary();
   static void NicaArray_1lEdoublegR_TClassManip(TClass*);
   static void *new_NicaArray_1lEdoublegR(void *p = nullptr);
   static void *newArray_NicaArray_1lEdoublegR(Long_t size, void *p);
   static void delete_NicaArray_1lEdoublegR(void *p);
   static void deleteArray_NicaArray_1lEdoublegR(void *p);
   static void destruct_NicaArray_1lEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_1<double>*)
   {
      ::NicaArray_1<double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_1<double> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_1<double>", ::NicaArray_1<double>::Class_Version(), "NicaArray.h", 18,
                  typeid(::NicaArray_1<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_1lEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_1<double>) );
      instance.SetNew(&new_NicaArray_1lEdoublegR);
      instance.SetNewArray(&newArray_NicaArray_1lEdoublegR);
      instance.SetDelete(&delete_NicaArray_1lEdoublegR);
      instance.SetDeleteArray(&deleteArray_NicaArray_1lEdoublegR);
      instance.SetDestructor(&destruct_NicaArray_1lEdoublegR);

      ::ROOT::AddClassAlternate("NicaArray_1<double>","NicaArray_1<Double_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_1<double>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_1<double>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_1<double>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_1lEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<double>*)nullptr)->GetClass();
      NicaArray_1lEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_1lEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_1lEfloatgR_Dictionary();
   static void NicaArray_1lEfloatgR_TClassManip(TClass*);
   static void *new_NicaArray_1lEfloatgR(void *p = nullptr);
   static void *newArray_NicaArray_1lEfloatgR(Long_t size, void *p);
   static void delete_NicaArray_1lEfloatgR(void *p);
   static void deleteArray_NicaArray_1lEfloatgR(void *p);
   static void destruct_NicaArray_1lEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_1<float>*)
   {
      ::NicaArray_1<float> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_1<float> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_1<float>", ::NicaArray_1<float>::Class_Version(), "NicaArray.h", 18,
                  typeid(::NicaArray_1<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_1lEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_1<float>) );
      instance.SetNew(&new_NicaArray_1lEfloatgR);
      instance.SetNewArray(&newArray_NicaArray_1lEfloatgR);
      instance.SetDelete(&delete_NicaArray_1lEfloatgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_1lEfloatgR);
      instance.SetDestructor(&destruct_NicaArray_1lEfloatgR);

      ::ROOT::AddClassAlternate("NicaArray_1<float>","NicaArray_1<Float_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_1<float>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_1<float>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_1<float>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_1lEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<float>*)nullptr)->GetClass();
      NicaArray_1lEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_1lEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_1lEshortgR_Dictionary();
   static void NicaArray_1lEshortgR_TClassManip(TClass*);
   static void *new_NicaArray_1lEshortgR(void *p = nullptr);
   static void *newArray_NicaArray_1lEshortgR(Long_t size, void *p);
   static void delete_NicaArray_1lEshortgR(void *p);
   static void deleteArray_NicaArray_1lEshortgR(void *p);
   static void destruct_NicaArray_1lEshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_1<short>*)
   {
      ::NicaArray_1<short> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_1<short> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_1<short>", ::NicaArray_1<short>::Class_Version(), "NicaArray.h", 18,
                  typeid(::NicaArray_1<short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_1lEshortgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_1<short>) );
      instance.SetNew(&new_NicaArray_1lEshortgR);
      instance.SetNewArray(&newArray_NicaArray_1lEshortgR);
      instance.SetDelete(&delete_NicaArray_1lEshortgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_1lEshortgR);
      instance.SetDestructor(&destruct_NicaArray_1lEshortgR);

      ::ROOT::AddClassAlternate("NicaArray_1<short>","NicaArray_1<Short_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_1<short>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_1<short>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_1<short>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_1lEshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<short>*)nullptr)->GetClass();
      NicaArray_1lEshortgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_1lEshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_1lEintgR_Dictionary();
   static void NicaArray_1lEintgR_TClassManip(TClass*);
   static void *new_NicaArray_1lEintgR(void *p = nullptr);
   static void *newArray_NicaArray_1lEintgR(Long_t size, void *p);
   static void delete_NicaArray_1lEintgR(void *p);
   static void deleteArray_NicaArray_1lEintgR(void *p);
   static void destruct_NicaArray_1lEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_1<int>*)
   {
      ::NicaArray_1<int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_1<int> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_1<int>", ::NicaArray_1<int>::Class_Version(), "NicaArray.h", 18,
                  typeid(::NicaArray_1<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_1lEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_1<int>) );
      instance.SetNew(&new_NicaArray_1lEintgR);
      instance.SetNewArray(&newArray_NicaArray_1lEintgR);
      instance.SetDelete(&delete_NicaArray_1lEintgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_1lEintgR);
      instance.SetDestructor(&destruct_NicaArray_1lEintgR);

      ::ROOT::AddClassAlternate("NicaArray_1<int>","NicaArray_1<Int_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_1<int>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_1<int>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_1<int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_1lEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<int>*)nullptr)->GetClass();
      NicaArray_1lEintgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_1lEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_2lEdoublegR_Dictionary();
   static void NicaArray_2lEdoublegR_TClassManip(TClass*);
   static void *new_NicaArray_2lEdoublegR(void *p = nullptr);
   static void *newArray_NicaArray_2lEdoublegR(Long_t size, void *p);
   static void delete_NicaArray_2lEdoublegR(void *p);
   static void deleteArray_NicaArray_2lEdoublegR(void *p);
   static void destruct_NicaArray_2lEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_2<double>*)
   {
      ::NicaArray_2<double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_2<double> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_2<double>", ::NicaArray_2<double>::Class_Version(), "NicaArray.h", 102,
                  typeid(::NicaArray_2<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_2lEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_2<double>) );
      instance.SetNew(&new_NicaArray_2lEdoublegR);
      instance.SetNewArray(&newArray_NicaArray_2lEdoublegR);
      instance.SetDelete(&delete_NicaArray_2lEdoublegR);
      instance.SetDeleteArray(&deleteArray_NicaArray_2lEdoublegR);
      instance.SetDestructor(&destruct_NicaArray_2lEdoublegR);

      ::ROOT::AddClassAlternate("NicaArray_2<double>","NicaArray_2<Double_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_2<double>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_2<double>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_2<double>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_2lEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<double>*)nullptr)->GetClass();
      NicaArray_2lEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_2lEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_2lEfloatgR_Dictionary();
   static void NicaArray_2lEfloatgR_TClassManip(TClass*);
   static void *new_NicaArray_2lEfloatgR(void *p = nullptr);
   static void *newArray_NicaArray_2lEfloatgR(Long_t size, void *p);
   static void delete_NicaArray_2lEfloatgR(void *p);
   static void deleteArray_NicaArray_2lEfloatgR(void *p);
   static void destruct_NicaArray_2lEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_2<float>*)
   {
      ::NicaArray_2<float> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_2<float> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_2<float>", ::NicaArray_2<float>::Class_Version(), "NicaArray.h", 102,
                  typeid(::NicaArray_2<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_2lEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_2<float>) );
      instance.SetNew(&new_NicaArray_2lEfloatgR);
      instance.SetNewArray(&newArray_NicaArray_2lEfloatgR);
      instance.SetDelete(&delete_NicaArray_2lEfloatgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_2lEfloatgR);
      instance.SetDestructor(&destruct_NicaArray_2lEfloatgR);

      ::ROOT::AddClassAlternate("NicaArray_2<float>","NicaArray_2<Float_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_2<float>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_2<float>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_2<float>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_2lEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<float>*)nullptr)->GetClass();
      NicaArray_2lEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_2lEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_2lEshortgR_Dictionary();
   static void NicaArray_2lEshortgR_TClassManip(TClass*);
   static void *new_NicaArray_2lEshortgR(void *p = nullptr);
   static void *newArray_NicaArray_2lEshortgR(Long_t size, void *p);
   static void delete_NicaArray_2lEshortgR(void *p);
   static void deleteArray_NicaArray_2lEshortgR(void *p);
   static void destruct_NicaArray_2lEshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_2<short>*)
   {
      ::NicaArray_2<short> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_2<short> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_2<short>", ::NicaArray_2<short>::Class_Version(), "NicaArray.h", 102,
                  typeid(::NicaArray_2<short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_2lEshortgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_2<short>) );
      instance.SetNew(&new_NicaArray_2lEshortgR);
      instance.SetNewArray(&newArray_NicaArray_2lEshortgR);
      instance.SetDelete(&delete_NicaArray_2lEshortgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_2lEshortgR);
      instance.SetDestructor(&destruct_NicaArray_2lEshortgR);

      ::ROOT::AddClassAlternate("NicaArray_2<short>","NicaArray_2<Short_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_2<short>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_2<short>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_2<short>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_2lEshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<short>*)nullptr)->GetClass();
      NicaArray_2lEshortgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_2lEshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_2lEintgR_Dictionary();
   static void NicaArray_2lEintgR_TClassManip(TClass*);
   static void *new_NicaArray_2lEintgR(void *p = nullptr);
   static void *newArray_NicaArray_2lEintgR(Long_t size, void *p);
   static void delete_NicaArray_2lEintgR(void *p);
   static void deleteArray_NicaArray_2lEintgR(void *p);
   static void destruct_NicaArray_2lEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_2<int>*)
   {
      ::NicaArray_2<int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_2<int> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_2<int>", ::NicaArray_2<int>::Class_Version(), "NicaArray.h", 102,
                  typeid(::NicaArray_2<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_2lEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_2<int>) );
      instance.SetNew(&new_NicaArray_2lEintgR);
      instance.SetNewArray(&newArray_NicaArray_2lEintgR);
      instance.SetDelete(&delete_NicaArray_2lEintgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_2lEintgR);
      instance.SetDestructor(&destruct_NicaArray_2lEintgR);

      ::ROOT::AddClassAlternate("NicaArray_2<int>","NicaArray_2<Int_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_2<int>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_2<int>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_2<int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_2lEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<int>*)nullptr)->GetClass();
      NicaArray_2lEintgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_2lEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_2lEboolgR_Dictionary();
   static void NicaArray_2lEboolgR_TClassManip(TClass*);
   static void *new_NicaArray_2lEboolgR(void *p = nullptr);
   static void *newArray_NicaArray_2lEboolgR(Long_t size, void *p);
   static void delete_NicaArray_2lEboolgR(void *p);
   static void deleteArray_NicaArray_2lEboolgR(void *p);
   static void destruct_NicaArray_2lEboolgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_2<bool>*)
   {
      ::NicaArray_2<bool> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_2<bool> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_2<bool>", ::NicaArray_2<bool>::Class_Version(), "NicaArray.h", 102,
                  typeid(::NicaArray_2<bool>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_2lEboolgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_2<bool>) );
      instance.SetNew(&new_NicaArray_2lEboolgR);
      instance.SetNewArray(&newArray_NicaArray_2lEboolgR);
      instance.SetDelete(&delete_NicaArray_2lEboolgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_2lEboolgR);
      instance.SetDestructor(&destruct_NicaArray_2lEboolgR);

      ::ROOT::AddClassAlternate("NicaArray_2<bool>","NicaArray_2<Bool_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_2<bool>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_2<bool>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_2<bool>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_2lEboolgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<bool>*)nullptr)->GetClass();
      NicaArray_2lEboolgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_2lEboolgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_3lEdoublegR_Dictionary();
   static void NicaArray_3lEdoublegR_TClassManip(TClass*);
   static void *new_NicaArray_3lEdoublegR(void *p = nullptr);
   static void *newArray_NicaArray_3lEdoublegR(Long_t size, void *p);
   static void delete_NicaArray_3lEdoublegR(void *p);
   static void deleteArray_NicaArray_3lEdoublegR(void *p);
   static void destruct_NicaArray_3lEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_3<double>*)
   {
      ::NicaArray_3<double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_3<double> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_3<double>", ::NicaArray_3<double>::Class_Version(), "NicaArray.h", 172,
                  typeid(::NicaArray_3<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_3lEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_3<double>) );
      instance.SetNew(&new_NicaArray_3lEdoublegR);
      instance.SetNewArray(&newArray_NicaArray_3lEdoublegR);
      instance.SetDelete(&delete_NicaArray_3lEdoublegR);
      instance.SetDeleteArray(&deleteArray_NicaArray_3lEdoublegR);
      instance.SetDestructor(&destruct_NicaArray_3lEdoublegR);

      ::ROOT::AddClassAlternate("NicaArray_3<double>","NicaArray_3<Double_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_3<double>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_3<double>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_3<double>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_3lEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<double>*)nullptr)->GetClass();
      NicaArray_3lEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_3lEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_3lEfloatgR_Dictionary();
   static void NicaArray_3lEfloatgR_TClassManip(TClass*);
   static void *new_NicaArray_3lEfloatgR(void *p = nullptr);
   static void *newArray_NicaArray_3lEfloatgR(Long_t size, void *p);
   static void delete_NicaArray_3lEfloatgR(void *p);
   static void deleteArray_NicaArray_3lEfloatgR(void *p);
   static void destruct_NicaArray_3lEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_3<float>*)
   {
      ::NicaArray_3<float> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_3<float> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_3<float>", ::NicaArray_3<float>::Class_Version(), "NicaArray.h", 172,
                  typeid(::NicaArray_3<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_3lEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_3<float>) );
      instance.SetNew(&new_NicaArray_3lEfloatgR);
      instance.SetNewArray(&newArray_NicaArray_3lEfloatgR);
      instance.SetDelete(&delete_NicaArray_3lEfloatgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_3lEfloatgR);
      instance.SetDestructor(&destruct_NicaArray_3lEfloatgR);

      ::ROOT::AddClassAlternate("NicaArray_3<float>","NicaArray_3<Float_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_3<float>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_3<float>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_3<float>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_3lEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<float>*)nullptr)->GetClass();
      NicaArray_3lEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_3lEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_3lEshortgR_Dictionary();
   static void NicaArray_3lEshortgR_TClassManip(TClass*);
   static void *new_NicaArray_3lEshortgR(void *p = nullptr);
   static void *newArray_NicaArray_3lEshortgR(Long_t size, void *p);
   static void delete_NicaArray_3lEshortgR(void *p);
   static void deleteArray_NicaArray_3lEshortgR(void *p);
   static void destruct_NicaArray_3lEshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_3<short>*)
   {
      ::NicaArray_3<short> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_3<short> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_3<short>", ::NicaArray_3<short>::Class_Version(), "NicaArray.h", 172,
                  typeid(::NicaArray_3<short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_3lEshortgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_3<short>) );
      instance.SetNew(&new_NicaArray_3lEshortgR);
      instance.SetNewArray(&newArray_NicaArray_3lEshortgR);
      instance.SetDelete(&delete_NicaArray_3lEshortgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_3lEshortgR);
      instance.SetDestructor(&destruct_NicaArray_3lEshortgR);

      ::ROOT::AddClassAlternate("NicaArray_3<short>","NicaArray_3<Short_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_3<short>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_3<short>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_3<short>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_3lEshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<short>*)nullptr)->GetClass();
      NicaArray_3lEshortgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_3lEshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_3lEintgR_Dictionary();
   static void NicaArray_3lEintgR_TClassManip(TClass*);
   static void *new_NicaArray_3lEintgR(void *p = nullptr);
   static void *newArray_NicaArray_3lEintgR(Long_t size, void *p);
   static void delete_NicaArray_3lEintgR(void *p);
   static void deleteArray_NicaArray_3lEintgR(void *p);
   static void destruct_NicaArray_3lEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_3<int>*)
   {
      ::NicaArray_3<int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_3<int> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_3<int>", ::NicaArray_3<int>::Class_Version(), "NicaArray.h", 172,
                  typeid(::NicaArray_3<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_3lEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_3<int>) );
      instance.SetNew(&new_NicaArray_3lEintgR);
      instance.SetNewArray(&newArray_NicaArray_3lEintgR);
      instance.SetDelete(&delete_NicaArray_3lEintgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_3lEintgR);
      instance.SetDestructor(&destruct_NicaArray_3lEintgR);

      ::ROOT::AddClassAlternate("NicaArray_3<int>","NicaArray_3<Int_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_3<int>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_3<int>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_3<int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_3lEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<int>*)nullptr)->GetClass();
      NicaArray_3lEintgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_3lEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_3lEboolgR_Dictionary();
   static void NicaArray_3lEboolgR_TClassManip(TClass*);
   static void *new_NicaArray_3lEboolgR(void *p = nullptr);
   static void *newArray_NicaArray_3lEboolgR(Long_t size, void *p);
   static void delete_NicaArray_3lEboolgR(void *p);
   static void deleteArray_NicaArray_3lEboolgR(void *p);
   static void destruct_NicaArray_3lEboolgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_3<bool>*)
   {
      ::NicaArray_3<bool> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_3<bool> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_3<bool>", ::NicaArray_3<bool>::Class_Version(), "NicaArray.h", 172,
                  typeid(::NicaArray_3<bool>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_3lEboolgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_3<bool>) );
      instance.SetNew(&new_NicaArray_3lEboolgR);
      instance.SetNewArray(&newArray_NicaArray_3lEboolgR);
      instance.SetDelete(&delete_NicaArray_3lEboolgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_3lEboolgR);
      instance.SetDestructor(&destruct_NicaArray_3lEboolgR);

      ::ROOT::AddClassAlternate("NicaArray_3<bool>","NicaArray_3<Bool_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_3<bool>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_3<bool>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_3<bool>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_3lEboolgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<bool>*)nullptr)->GetClass();
      NicaArray_3lEboolgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_3lEboolgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_4lEdoublegR_Dictionary();
   static void NicaArray_4lEdoublegR_TClassManip(TClass*);
   static void *new_NicaArray_4lEdoublegR(void *p = nullptr);
   static void *newArray_NicaArray_4lEdoublegR(Long_t size, void *p);
   static void delete_NicaArray_4lEdoublegR(void *p);
   static void deleteArray_NicaArray_4lEdoublegR(void *p);
   static void destruct_NicaArray_4lEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_4<double>*)
   {
      ::NicaArray_4<double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_4<double> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_4<double>", ::NicaArray_4<double>::Class_Version(), "NicaArray.h", 247,
                  typeid(::NicaArray_4<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_4lEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_4<double>) );
      instance.SetNew(&new_NicaArray_4lEdoublegR);
      instance.SetNewArray(&newArray_NicaArray_4lEdoublegR);
      instance.SetDelete(&delete_NicaArray_4lEdoublegR);
      instance.SetDeleteArray(&deleteArray_NicaArray_4lEdoublegR);
      instance.SetDestructor(&destruct_NicaArray_4lEdoublegR);

      ::ROOT::AddClassAlternate("NicaArray_4<double>","NicaArray_4<Double_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_4<double>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_4<double>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_4<double>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_4lEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<double>*)nullptr)->GetClass();
      NicaArray_4lEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_4lEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_4lEfloatgR_Dictionary();
   static void NicaArray_4lEfloatgR_TClassManip(TClass*);
   static void *new_NicaArray_4lEfloatgR(void *p = nullptr);
   static void *newArray_NicaArray_4lEfloatgR(Long_t size, void *p);
   static void delete_NicaArray_4lEfloatgR(void *p);
   static void deleteArray_NicaArray_4lEfloatgR(void *p);
   static void destruct_NicaArray_4lEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_4<float>*)
   {
      ::NicaArray_4<float> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_4<float> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_4<float>", ::NicaArray_4<float>::Class_Version(), "NicaArray.h", 247,
                  typeid(::NicaArray_4<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_4lEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_4<float>) );
      instance.SetNew(&new_NicaArray_4lEfloatgR);
      instance.SetNewArray(&newArray_NicaArray_4lEfloatgR);
      instance.SetDelete(&delete_NicaArray_4lEfloatgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_4lEfloatgR);
      instance.SetDestructor(&destruct_NicaArray_4lEfloatgR);

      ::ROOT::AddClassAlternate("NicaArray_4<float>","NicaArray_4<Float_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_4<float>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_4<float>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_4<float>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_4lEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<float>*)nullptr)->GetClass();
      NicaArray_4lEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_4lEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_4lEshortgR_Dictionary();
   static void NicaArray_4lEshortgR_TClassManip(TClass*);
   static void *new_NicaArray_4lEshortgR(void *p = nullptr);
   static void *newArray_NicaArray_4lEshortgR(Long_t size, void *p);
   static void delete_NicaArray_4lEshortgR(void *p);
   static void deleteArray_NicaArray_4lEshortgR(void *p);
   static void destruct_NicaArray_4lEshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_4<short>*)
   {
      ::NicaArray_4<short> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_4<short> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_4<short>", ::NicaArray_4<short>::Class_Version(), "NicaArray.h", 247,
                  typeid(::NicaArray_4<short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_4lEshortgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_4<short>) );
      instance.SetNew(&new_NicaArray_4lEshortgR);
      instance.SetNewArray(&newArray_NicaArray_4lEshortgR);
      instance.SetDelete(&delete_NicaArray_4lEshortgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_4lEshortgR);
      instance.SetDestructor(&destruct_NicaArray_4lEshortgR);

      ::ROOT::AddClassAlternate("NicaArray_4<short>","NicaArray_4<Short_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_4<short>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_4<short>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_4<short>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_4lEshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<short>*)nullptr)->GetClass();
      NicaArray_4lEshortgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_4lEshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_4lEintgR_Dictionary();
   static void NicaArray_4lEintgR_TClassManip(TClass*);
   static void *new_NicaArray_4lEintgR(void *p = nullptr);
   static void *newArray_NicaArray_4lEintgR(Long_t size, void *p);
   static void delete_NicaArray_4lEintgR(void *p);
   static void deleteArray_NicaArray_4lEintgR(void *p);
   static void destruct_NicaArray_4lEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_4<int>*)
   {
      ::NicaArray_4<int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_4<int> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_4<int>", ::NicaArray_4<int>::Class_Version(), "NicaArray.h", 247,
                  typeid(::NicaArray_4<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_4lEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_4<int>) );
      instance.SetNew(&new_NicaArray_4lEintgR);
      instance.SetNewArray(&newArray_NicaArray_4lEintgR);
      instance.SetDelete(&delete_NicaArray_4lEintgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_4lEintgR);
      instance.SetDestructor(&destruct_NicaArray_4lEintgR);

      ::ROOT::AddClassAlternate("NicaArray_4<int>","NicaArray_4<Int_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_4<int>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_4<int>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_4<int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_4lEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<int>*)nullptr)->GetClass();
      NicaArray_4lEintgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_4lEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaArray_4lEboolgR_Dictionary();
   static void NicaArray_4lEboolgR_TClassManip(TClass*);
   static void *new_NicaArray_4lEboolgR(void *p = nullptr);
   static void *newArray_NicaArray_4lEboolgR(Long_t size, void *p);
   static void delete_NicaArray_4lEboolgR(void *p);
   static void deleteArray_NicaArray_4lEboolgR(void *p);
   static void destruct_NicaArray_4lEboolgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaArray_4<bool>*)
   {
      ::NicaArray_4<bool> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaArray_4<bool> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaArray_4<bool>", ::NicaArray_4<bool>::Class_Version(), "NicaArray.h", 247,
                  typeid(::NicaArray_4<bool>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaArray_4lEboolgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaArray_4<bool>) );
      instance.SetNew(&new_NicaArray_4lEboolgR);
      instance.SetNewArray(&newArray_NicaArray_4lEboolgR);
      instance.SetDelete(&delete_NicaArray_4lEboolgR);
      instance.SetDeleteArray(&deleteArray_NicaArray_4lEboolgR);
      instance.SetDestructor(&destruct_NicaArray_4lEboolgR);

      ::ROOT::AddClassAlternate("NicaArray_4<bool>","NicaArray_4<Bool_t>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaArray_4<bool>*)
   {
      return GenerateInitInstanceLocal((::NicaArray_4<bool>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaArray_4<bool>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaArray_4lEboolgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<bool>*)nullptr)->GetClass();
      NicaArray_4lEboolgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaArray_4lEboolgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaOptionArray(void *p = nullptr);
   static void *newArray_NicaOptionArray(Long_t size, void *p);
   static void delete_NicaOptionArray(void *p);
   static void deleteArray_NicaOptionArray(void *p);
   static void destruct_NicaOptionArray(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaOptionArray*)
   {
      ::NicaOptionArray *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaOptionArray >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaOptionArray", ::NicaOptionArray::Class_Version(), "NicaOptions.h", 18,
                  typeid(::NicaOptionArray), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaOptionArray::Dictionary, isa_proxy, 4,
                  sizeof(::NicaOptionArray) );
      instance.SetNew(&new_NicaOptionArray);
      instance.SetNewArray(&newArray_NicaOptionArray);
      instance.SetDelete(&delete_NicaOptionArray);
      instance.SetDeleteArray(&deleteArray_NicaOptionArray);
      instance.SetDestructor(&destruct_NicaOptionArray);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaOptionArray*)
   {
      return GenerateInitInstanceLocal((::NicaOptionArray*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaOptionArray*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaOptionConverter(void *p = nullptr);
   static void *newArray_NicaOptionConverter(Long_t size, void *p);
   static void delete_NicaOptionConverter(void *p);
   static void deleteArray_NicaOptionConverter(void *p);
   static void destruct_NicaOptionConverter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaOptionConverter*)
   {
      ::NicaOptionConverter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaOptionConverter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaOptionConverter", ::NicaOptionConverter::Class_Version(), "NicaOptions.h", 52,
                  typeid(::NicaOptionConverter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaOptionConverter::Dictionary, isa_proxy, 4,
                  sizeof(::NicaOptionConverter) );
      instance.SetNew(&new_NicaOptionConverter);
      instance.SetNewArray(&newArray_NicaOptionConverter);
      instance.SetDelete(&delete_NicaOptionConverter);
      instance.SetDeleteArray(&deleteArray_NicaOptionConverter);
      instance.SetDestructor(&destruct_NicaOptionConverter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaOptionConverter*)
   {
      return GenerateInitInstanceLocal((::NicaOptionConverter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaOptionConverter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaPack(void *p = nullptr);
   static void *newArray_NicaPack(Long_t size, void *p);
   static void delete_NicaPack(void *p);
   static void deleteArray_NicaPack(void *p);
   static void destruct_NicaPack(void *p);
   static Long64_t merge_NicaPack(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaPack*)
   {
      ::NicaPack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaPack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaPack", ::NicaPack::Class_Version(), "NicaPack.h", 23,
                  typeid(::NicaPack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaPack::Dictionary, isa_proxy, 4,
                  sizeof(::NicaPack) );
      instance.SetNew(&new_NicaPack);
      instance.SetNewArray(&newArray_NicaPack);
      instance.SetDelete(&delete_NicaPack);
      instance.SetDeleteArray(&deleteArray_NicaPack);
      instance.SetDestructor(&destruct_NicaPack);
      instance.SetMerge(&merge_NicaPack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaPack*)
   {
      return GenerateInitInstanceLocal((::NicaPack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaPack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaParameter(void *p = nullptr);
   static void *newArray_NicaParameter(Long_t size, void *p);
   static void delete_NicaParameter(void *p);
   static void deleteArray_NicaParameter(void *p);
   static void destruct_NicaParameter(void *p);
   static Long64_t merge_NicaParameter(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaParameter*)
   {
      ::NicaParameter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaParameter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaParameter", ::NicaParameter::Class_Version(), "NicaParameter.h", 20,
                  typeid(::NicaParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaParameter::Dictionary, isa_proxy, 4,
                  sizeof(::NicaParameter) );
      instance.SetNew(&new_NicaParameter);
      instance.SetNewArray(&newArray_NicaParameter);
      instance.SetDelete(&delete_NicaParameter);
      instance.SetDeleteArray(&deleteArray_NicaParameter);
      instance.SetDestructor(&destruct_NicaParameter);
      instance.SetMerge(&merge_NicaParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaParameter*)
   {
      return GenerateInitInstanceLocal((::NicaParameter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaParameter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaParameterInt(void *p = nullptr);
   static void *newArray_NicaParameterInt(Long_t size, void *p);
   static void delete_NicaParameterInt(void *p);
   static void deleteArray_NicaParameterInt(void *p);
   static void destruct_NicaParameterInt(void *p);
   static Long64_t merge_NicaParameterInt(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaParameterInt*)
   {
      ::NicaParameterInt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaParameterInt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaParameterInt", ::NicaParameterInt::Class_Version(), "NicaParameter.h", 92,
                  typeid(::NicaParameterInt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaParameterInt::Dictionary, isa_proxy, 4,
                  sizeof(::NicaParameterInt) );
      instance.SetNew(&new_NicaParameterInt);
      instance.SetNewArray(&newArray_NicaParameterInt);
      instance.SetDelete(&delete_NicaParameterInt);
      instance.SetDeleteArray(&deleteArray_NicaParameterInt);
      instance.SetDestructor(&destruct_NicaParameterInt);
      instance.SetMerge(&merge_NicaParameterInt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaParameterInt*)
   {
      return GenerateInitInstanceLocal((::NicaParameterInt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaParameterInt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaParameterDouble(void *p = nullptr);
   static void *newArray_NicaParameterDouble(Long_t size, void *p);
   static void delete_NicaParameterDouble(void *p);
   static void deleteArray_NicaParameterDouble(void *p);
   static void destruct_NicaParameterDouble(void *p);
   static Long64_t merge_NicaParameterDouble(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaParameterDouble*)
   {
      ::NicaParameterDouble *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaParameterDouble >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaParameterDouble", ::NicaParameterDouble::Class_Version(), "NicaParameter.h", 123,
                  typeid(::NicaParameterDouble), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaParameterDouble::Dictionary, isa_proxy, 4,
                  sizeof(::NicaParameterDouble) );
      instance.SetNew(&new_NicaParameterDouble);
      instance.SetNewArray(&newArray_NicaParameterDouble);
      instance.SetDelete(&delete_NicaParameterDouble);
      instance.SetDeleteArray(&deleteArray_NicaParameterDouble);
      instance.SetDestructor(&destruct_NicaParameterDouble);
      instance.SetMerge(&merge_NicaParameterDouble);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaParameterDouble*)
   {
      return GenerateInitInstanceLocal((::NicaParameterDouble*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaParameterDouble*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaParameterFloat(void *p = nullptr);
   static void *newArray_NicaParameterFloat(Long_t size, void *p);
   static void delete_NicaParameterFloat(void *p);
   static void deleteArray_NicaParameterFloat(void *p);
   static void destruct_NicaParameterFloat(void *p);
   static Long64_t merge_NicaParameterFloat(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaParameterFloat*)
   {
      ::NicaParameterFloat *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaParameterFloat >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaParameterFloat", ::NicaParameterFloat::Class_Version(), "NicaParameter.h", 154,
                  typeid(::NicaParameterFloat), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaParameterFloat::Dictionary, isa_proxy, 4,
                  sizeof(::NicaParameterFloat) );
      instance.SetNew(&new_NicaParameterFloat);
      instance.SetNewArray(&newArray_NicaParameterFloat);
      instance.SetDelete(&delete_NicaParameterFloat);
      instance.SetDeleteArray(&deleteArray_NicaParameterFloat);
      instance.SetDestructor(&destruct_NicaParameterFloat);
      instance.SetMerge(&merge_NicaParameterFloat);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaParameterFloat*)
   {
      return GenerateInitInstanceLocal((::NicaParameterFloat*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaParameterFloat*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaParameterString(void *p = nullptr);
   static void *newArray_NicaParameterString(Long_t size, void *p);
   static void delete_NicaParameterString(void *p);
   static void deleteArray_NicaParameterString(void *p);
   static void destruct_NicaParameterString(void *p);
   static Long64_t merge_NicaParameterString(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaParameterString*)
   {
      ::NicaParameterString *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaParameterString >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaParameterString", ::NicaParameterString::Class_Version(), "NicaParameter.h", 185,
                  typeid(::NicaParameterString), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaParameterString::Dictionary, isa_proxy, 4,
                  sizeof(::NicaParameterString) );
      instance.SetNew(&new_NicaParameterString);
      instance.SetNewArray(&newArray_NicaParameterString);
      instance.SetDelete(&delete_NicaParameterString);
      instance.SetDeleteArray(&deleteArray_NicaParameterString);
      instance.SetDestructor(&destruct_NicaParameterString);
      instance.SetMerge(&merge_NicaParameterString);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaParameterString*)
   {
      return GenerateInitInstanceLocal((::NicaParameterString*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaParameterString*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaParameterUInt(void *p = nullptr);
   static void *newArray_NicaParameterUInt(Long_t size, void *p);
   static void delete_NicaParameterUInt(void *p);
   static void deleteArray_NicaParameterUInt(void *p);
   static void destruct_NicaParameterUInt(void *p);
   static Long64_t merge_NicaParameterUInt(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaParameterUInt*)
   {
      ::NicaParameterUInt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaParameterUInt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaParameterUInt", ::NicaParameterUInt::Class_Version(), "NicaParameter.h", 216,
                  typeid(::NicaParameterUInt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaParameterUInt::Dictionary, isa_proxy, 4,
                  sizeof(::NicaParameterUInt) );
      instance.SetNew(&new_NicaParameterUInt);
      instance.SetNewArray(&newArray_NicaParameterUInt);
      instance.SetDelete(&delete_NicaParameterUInt);
      instance.SetDeleteArray(&deleteArray_NicaParameterUInt);
      instance.SetDestructor(&destruct_NicaParameterUInt);
      instance.SetMerge(&merge_NicaParameterUInt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaParameterUInt*)
   {
      return GenerateInitInstanceLocal((::NicaParameterUInt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaParameterUInt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaParameterULong(void *p = nullptr);
   static void *newArray_NicaParameterULong(Long_t size, void *p);
   static void delete_NicaParameterULong(void *p);
   static void deleteArray_NicaParameterULong(void *p);
   static void destruct_NicaParameterULong(void *p);
   static Long64_t merge_NicaParameterULong(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaParameterULong*)
   {
      ::NicaParameterULong *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaParameterULong >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaParameterULong", ::NicaParameterULong::Class_Version(), "NicaParameter.h", 247,
                  typeid(::NicaParameterULong), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaParameterULong::Dictionary, isa_proxy, 4,
                  sizeof(::NicaParameterULong) );
      instance.SetNew(&new_NicaParameterULong);
      instance.SetNewArray(&newArray_NicaParameterULong);
      instance.SetDelete(&delete_NicaParameterULong);
      instance.SetDeleteArray(&deleteArray_NicaParameterULong);
      instance.SetDestructor(&destruct_NicaParameterULong);
      instance.SetMerge(&merge_NicaParameterULong);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaParameterULong*)
   {
      return GenerateInitInstanceLocal((::NicaParameterULong*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaParameterULong*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaParameterULong64(void *p = nullptr);
   static void *newArray_NicaParameterULong64(Long_t size, void *p);
   static void delete_NicaParameterULong64(void *p);
   static void deleteArray_NicaParameterULong64(void *p);
   static void destruct_NicaParameterULong64(void *p);
   static Long64_t merge_NicaParameterULong64(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaParameterULong64*)
   {
      ::NicaParameterULong64 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaParameterULong64 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaParameterULong64", ::NicaParameterULong64::Class_Version(), "NicaParameter.h", 278,
                  typeid(::NicaParameterULong64), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaParameterULong64::Dictionary, isa_proxy, 4,
                  sizeof(::NicaParameterULong64) );
      instance.SetNew(&new_NicaParameterULong64);
      instance.SetNewArray(&newArray_NicaParameterULong64);
      instance.SetDelete(&delete_NicaParameterULong64);
      instance.SetDeleteArray(&deleteArray_NicaParameterULong64);
      instance.SetDestructor(&destruct_NicaParameterULong64);
      instance.SetMerge(&merge_NicaParameterULong64);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaParameterULong64*)
   {
      return GenerateInitInstanceLocal((::NicaParameterULong64*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaParameterULong64*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaParameterBool(void *p = nullptr);
   static void *newArray_NicaParameterBool(Long_t size, void *p);
   static void delete_NicaParameterBool(void *p);
   static void deleteArray_NicaParameterBool(void *p);
   static void destruct_NicaParameterBool(void *p);
   static Long64_t merge_NicaParameterBool(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaParameterBool*)
   {
      ::NicaParameterBool *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaParameterBool >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaParameterBool", ::NicaParameterBool::Class_Version(), "NicaParameter.h", 309,
                  typeid(::NicaParameterBool), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaParameterBool::Dictionary, isa_proxy, 4,
                  sizeof(::NicaParameterBool) );
      instance.SetNew(&new_NicaParameterBool);
      instance.SetNewArray(&newArray_NicaParameterBool);
      instance.SetDelete(&delete_NicaParameterBool);
      instance.SetDeleteArray(&deleteArray_NicaParameterBool);
      instance.SetDestructor(&destruct_NicaParameterBool);
      instance.SetMerge(&merge_NicaParameterBool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaParameterBool*)
   {
      return GenerateInitInstanceLocal((::NicaParameterBool*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaParameterBool*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaObjectMatrix_1(void *p = nullptr);
   static void *newArray_NicaObjectMatrix_1(Long_t size, void *p);
   static void delete_NicaObjectMatrix_1(void *p);
   static void deleteArray_NicaObjectMatrix_1(void *p);
   static void destruct_NicaObjectMatrix_1(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaObjectMatrix_1*)
   {
      ::NicaObjectMatrix_1 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaObjectMatrix_1 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaObjectMatrix_1", ::NicaObjectMatrix_1::Class_Version(), "NicaObjectMatrix.h", 24,
                  typeid(::NicaObjectMatrix_1), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaObjectMatrix_1::Dictionary, isa_proxy, 4,
                  sizeof(::NicaObjectMatrix_1) );
      instance.SetNew(&new_NicaObjectMatrix_1);
      instance.SetNewArray(&newArray_NicaObjectMatrix_1);
      instance.SetDelete(&delete_NicaObjectMatrix_1);
      instance.SetDeleteArray(&deleteArray_NicaObjectMatrix_1);
      instance.SetDestructor(&destruct_NicaObjectMatrix_1);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaObjectMatrix_1*)
   {
      return GenerateInitInstanceLocal((::NicaObjectMatrix_1*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaObjectMatrix_1*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaObjectMatrix_2(void *p = nullptr);
   static void *newArray_NicaObjectMatrix_2(Long_t size, void *p);
   static void delete_NicaObjectMatrix_2(void *p);
   static void deleteArray_NicaObjectMatrix_2(void *p);
   static void destruct_NicaObjectMatrix_2(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaObjectMatrix_2*)
   {
      ::NicaObjectMatrix_2 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaObjectMatrix_2 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaObjectMatrix_2", ::NicaObjectMatrix_2::Class_Version(), "NicaObjectMatrix.h", 101,
                  typeid(::NicaObjectMatrix_2), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaObjectMatrix_2::Dictionary, isa_proxy, 4,
                  sizeof(::NicaObjectMatrix_2) );
      instance.SetNew(&new_NicaObjectMatrix_2);
      instance.SetNewArray(&newArray_NicaObjectMatrix_2);
      instance.SetDelete(&delete_NicaObjectMatrix_2);
      instance.SetDeleteArray(&deleteArray_NicaObjectMatrix_2);
      instance.SetDestructor(&destruct_NicaObjectMatrix_2);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaObjectMatrix_2*)
   {
      return GenerateInitInstanceLocal((::NicaObjectMatrix_2*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaObjectMatrix_2*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaObjectMatrix_3(void *p = nullptr);
   static void *newArray_NicaObjectMatrix_3(Long_t size, void *p);
   static void delete_NicaObjectMatrix_3(void *p);
   static void deleteArray_NicaObjectMatrix_3(void *p);
   static void destruct_NicaObjectMatrix_3(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaObjectMatrix_3*)
   {
      ::NicaObjectMatrix_3 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaObjectMatrix_3 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaObjectMatrix_3", ::NicaObjectMatrix_3::Class_Version(), "NicaObjectMatrix.h", 160,
                  typeid(::NicaObjectMatrix_3), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaObjectMatrix_3::Dictionary, isa_proxy, 4,
                  sizeof(::NicaObjectMatrix_3) );
      instance.SetNew(&new_NicaObjectMatrix_3);
      instance.SetNewArray(&newArray_NicaObjectMatrix_3);
      instance.SetDelete(&delete_NicaObjectMatrix_3);
      instance.SetDeleteArray(&deleteArray_NicaObjectMatrix_3);
      instance.SetDestructor(&destruct_NicaObjectMatrix_3);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaObjectMatrix_3*)
   {
      return GenerateInitInstanceLocal((::NicaObjectMatrix_3*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaObjectMatrix_3*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaObjectMatrix_4(void *p = nullptr);
   static void *newArray_NicaObjectMatrix_4(Long_t size, void *p);
   static void delete_NicaObjectMatrix_4(void *p);
   static void deleteArray_NicaObjectMatrix_4(void *p);
   static void destruct_NicaObjectMatrix_4(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaObjectMatrix_4*)
   {
      ::NicaObjectMatrix_4 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaObjectMatrix_4 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaObjectMatrix_4", ::NicaObjectMatrix_4::Class_Version(), "NicaObjectMatrix.h", 219,
                  typeid(::NicaObjectMatrix_4), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaObjectMatrix_4::Dictionary, isa_proxy, 4,
                  sizeof(::NicaObjectMatrix_4) );
      instance.SetNew(&new_NicaObjectMatrix_4);
      instance.SetNewArray(&newArray_NicaObjectMatrix_4);
      instance.SetDelete(&delete_NicaObjectMatrix_4);
      instance.SetDeleteArray(&deleteArray_NicaObjectMatrix_4);
      instance.SetDestructor(&destruct_NicaObjectMatrix_4);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaObjectMatrix_4*)
   {
      return GenerateInitInstanceLocal((::NicaObjectMatrix_4*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaObjectMatrix_4*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaHelixBase(void *p = nullptr);
   static void *newArray_NicaHelixBase(Long_t size, void *p);
   static void delete_NicaHelixBase(void *p);
   static void deleteArray_NicaHelixBase(void *p);
   static void destruct_NicaHelixBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHelixBase*)
   {
      ::NicaHelixBase *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHelixBase >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHelixBase", ::NicaHelixBase::Class_Version(), "NicaHelixBase.h", 18,
                  typeid(::NicaHelixBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaHelixBase::Dictionary, isa_proxy, 4,
                  sizeof(::NicaHelixBase) );
      instance.SetNew(&new_NicaHelixBase);
      instance.SetNewArray(&newArray_NicaHelixBase);
      instance.SetDelete(&delete_NicaHelixBase);
      instance.SetDeleteArray(&deleteArray_NicaHelixBase);
      instance.SetDestructor(&destruct_NicaHelixBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHelixBase*)
   {
      return GenerateInitInstanceLocal((::NicaHelixBase*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHelixBase*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaHelixZ(void *p = nullptr);
   static void *newArray_NicaHelixZ(Long_t size, void *p);
   static void delete_NicaHelixZ(void *p);
   static void deleteArray_NicaHelixZ(void *p);
   static void destruct_NicaHelixZ(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHelixZ*)
   {
      ::NicaHelixZ *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHelixZ >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHelixZ", ::NicaHelixZ::Class_Version(), "NicaHelix.h", 22,
                  typeid(::NicaHelixZ), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaHelixZ::Dictionary, isa_proxy, 4,
                  sizeof(::NicaHelixZ) );
      instance.SetNew(&new_NicaHelixZ);
      instance.SetNewArray(&newArray_NicaHelixZ);
      instance.SetDelete(&delete_NicaHelixZ);
      instance.SetDeleteArray(&deleteArray_NicaHelixZ);
      instance.SetDestructor(&destruct_NicaHelixZ);

      ::ROOT::AddClassAlternate("NicaHelixZ","NicaHelix");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHelixZ*)
   {
      return GenerateInitInstanceLocal((::NicaHelixZ*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHelixZ*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaHelixX(void *p = nullptr);
   static void *newArray_NicaHelixX(Long_t size, void *p);
   static void delete_NicaHelixX(void *p);
   static void deleteArray_NicaHelixX(void *p);
   static void destruct_NicaHelixX(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHelixX*)
   {
      ::NicaHelixX *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHelixX >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHelixX", ::NicaHelixX::Class_Version(), "NicaHelix.h", 172,
                  typeid(::NicaHelixX), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaHelixX::Dictionary, isa_proxy, 4,
                  sizeof(::NicaHelixX) );
      instance.SetNew(&new_NicaHelixX);
      instance.SetNewArray(&newArray_NicaHelixX);
      instance.SetDelete(&delete_NicaHelixX);
      instance.SetDeleteArray(&deleteArray_NicaHelixX);
      instance.SetDestructor(&destruct_NicaHelixX);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHelixX*)
   {
      return GenerateInitInstanceLocal((::NicaHelixX*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHelixX*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaHelixY(void *p = nullptr);
   static void *newArray_NicaHelixY(Long_t size, void *p);
   static void delete_NicaHelixY(void *p);
   static void deleteArray_NicaHelixY(void *p);
   static void destruct_NicaHelixY(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHelixY*)
   {
      ::NicaHelixY *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHelixY >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHelixY", ::NicaHelixY::Class_Version(), "NicaHelix.h", 336,
                  typeid(::NicaHelixY), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaHelixY::Dictionary, isa_proxy, 4,
                  sizeof(::NicaHelixY) );
      instance.SetNew(&new_NicaHelixY);
      instance.SetNewArray(&newArray_NicaHelixY);
      instance.SetDelete(&delete_NicaHelixY);
      instance.SetDeleteArray(&deleteArray_NicaHelixY);
      instance.SetDestructor(&destruct_NicaHelixY);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHelixY*)
   {
      return GenerateInitInstanceLocal((::NicaHelixY*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHelixY*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaLink(void *p = nullptr);
   static void *newArray_NicaLink(Long_t size, void *p);
   static void delete_NicaLink(void *p);
   static void deleteArray_NicaLink(void *p);
   static void destruct_NicaLink(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaLink*)
   {
      ::NicaLink *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaLink >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaLink", ::NicaLink::Class_Version(), "NicaLink.h", 19,
                  typeid(::NicaLink), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaLink::Dictionary, isa_proxy, 4,
                  sizeof(::NicaLink) );
      instance.SetNew(&new_NicaLink);
      instance.SetNewArray(&newArray_NicaLink);
      instance.SetDelete(&delete_NicaLink);
      instance.SetDeleteArray(&deleteArray_NicaLink);
      instance.SetDestructor(&destruct_NicaLink);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaLink*)
   {
      return GenerateInitInstanceLocal((::NicaLink*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaLink*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaConst(void *p = nullptr);
   static void *newArray_NicaConst(Long_t size, void *p);
   static void delete_NicaConst(void *p);
   static void deleteArray_NicaConst(void *p);
   static void destruct_NicaConst(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaConst*)
   {
      ::NicaConst *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaConst >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaConst", ::NicaConst::Class_Version(), "NicaConst.h", 16,
                  typeid(::NicaConst), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaConst::Dictionary, isa_proxy, 4,
                  sizeof(::NicaConst) );
      instance.SetNew(&new_NicaConst);
      instance.SetNewArray(&newArray_NicaConst);
      instance.SetDelete(&delete_NicaConst);
      instance.SetDeleteArray(&deleteArray_NicaConst);
      instance.SetDestructor(&destruct_NicaConst);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaConst*)
   {
      return GenerateInitInstanceLocal((::NicaConst*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaConst*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaXMLAttrib(void *p = nullptr);
   static void *newArray_NicaXMLAttrib(Long_t size, void *p);
   static void delete_NicaXMLAttrib(void *p);
   static void deleteArray_NicaXMLAttrib(void *p);
   static void destruct_NicaXMLAttrib(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaXMLAttrib*)
   {
      ::NicaXMLAttrib *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaXMLAttrib >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaXMLAttrib", ::NicaXMLAttrib::Class_Version(), "NicaXMLNode.h", 26,
                  typeid(::NicaXMLAttrib), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaXMLAttrib::Dictionary, isa_proxy, 4,
                  sizeof(::NicaXMLAttrib) );
      instance.SetNew(&new_NicaXMLAttrib);
      instance.SetNewArray(&newArray_NicaXMLAttrib);
      instance.SetDelete(&delete_NicaXMLAttrib);
      instance.SetDeleteArray(&deleteArray_NicaXMLAttrib);
      instance.SetDestructor(&destruct_NicaXMLAttrib);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaXMLAttrib*)
   {
      return GenerateInitInstanceLocal((::NicaXMLAttrib*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaXMLAttrib*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaXMLNode(void *p = nullptr);
   static void *newArray_NicaXMLNode(Long_t size, void *p);
   static void delete_NicaXMLNode(void *p);
   static void deleteArray_NicaXMLNode(void *p);
   static void destruct_NicaXMLNode(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaXMLNode*)
   {
      ::NicaXMLNode *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaXMLNode >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaXMLNode", ::NicaXMLNode::Class_Version(), "NicaXMLNode.h", 57,
                  typeid(::NicaXMLNode), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaXMLNode::Dictionary, isa_proxy, 4,
                  sizeof(::NicaXMLNode) );
      instance.SetNew(&new_NicaXMLNode);
      instance.SetNewArray(&newArray_NicaXMLNode);
      instance.SetDelete(&delete_NicaXMLNode);
      instance.SetDeleteArray(&deleteArray_NicaXMLNode);
      instance.SetDestructor(&destruct_NicaXMLNode);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaXMLNode*)
   {
      return GenerateInitInstanceLocal((::NicaXMLNode*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaXMLNode*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaXMLFile(void *p = nullptr);
   static void *newArray_NicaXMLFile(Long_t size, void *p);
   static void delete_NicaXMLFile(void *p);
   static void deleteArray_NicaXMLFile(void *p);
   static void destruct_NicaXMLFile(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaXMLFile*)
   {
      ::NicaXMLFile *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaXMLFile >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaXMLFile", ::NicaXMLFile::Class_Version(), "NicaXMLNode.h", 153,
                  typeid(::NicaXMLFile), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaXMLFile::Dictionary, isa_proxy, 4,
                  sizeof(::NicaXMLFile) );
      instance.SetNew(&new_NicaXMLFile);
      instance.SetNewArray(&newArray_NicaXMLFile);
      instance.SetDelete(&delete_NicaXMLFile);
      instance.SetDeleteArray(&deleteArray_NicaXMLFile);
      instance.SetDestructor(&destruct_NicaXMLFile);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaXMLFile*)
   {
      return GenerateInitInstanceLocal((::NicaXMLFile*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaXMLFile*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaJobs(void *p);
   static void deleteArray_NicaJobs(void *p);
   static void destruct_NicaJobs(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaJobs*)
   {
      ::NicaJobs *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaJobs >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaJobs", ::NicaJobs::Class_Version(), "NicaJobs.h", 17,
                  typeid(::NicaJobs), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaJobs::Dictionary, isa_proxy, 4,
                  sizeof(::NicaJobs) );
      instance.SetDelete(&delete_NicaJobs);
      instance.SetDeleteArray(&deleteArray_NicaJobs);
      instance.SetDestructor(&destruct_NicaJobs);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaJobs*)
   {
      return GenerateInitInstanceLocal((::NicaJobs*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaJobs*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaPackage(void *p = nullptr);
   static void *newArray_NicaPackage(Long_t size, void *p);
   static void delete_NicaPackage(void *p);
   static void deleteArray_NicaPackage(void *p);
   static void destruct_NicaPackage(void *p);
   static Long64_t merge_NicaPackage(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaPackage*)
   {
      ::NicaPackage *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaPackage >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaPackage", ::NicaPackage::Class_Version(), "NicaPackage.h", 24,
                  typeid(::NicaPackage), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaPackage::Dictionary, isa_proxy, 4,
                  sizeof(::NicaPackage) );
      instance.SetNew(&new_NicaPackage);
      instance.SetNewArray(&newArray_NicaPackage);
      instance.SetDelete(&delete_NicaPackage);
      instance.SetDeleteArray(&deleteArray_NicaPackage);
      instance.SetDestructor(&destruct_NicaPackage);
      instance.SetMerge(&merge_NicaPackage);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaPackage*)
   {
      return GenerateInitInstanceLocal((::NicaPackage*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaPackage*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaPackage2HTML(void *p);
   static void deleteArray_NicaPackage2HTML(void *p);
   static void destruct_NicaPackage2HTML(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaPackage2HTML*)
   {
      ::NicaPackage2HTML *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaPackage2HTML >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaPackage2HTML", ::NicaPackage2HTML::Class_Version(), "NicaPackage2HTML.h", 30,
                  typeid(::NicaPackage2HTML), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaPackage2HTML::Dictionary, isa_proxy, 4,
                  sizeof(::NicaPackage2HTML) );
      instance.SetDelete(&delete_NicaPackage2HTML);
      instance.SetDeleteArray(&deleteArray_NicaPackage2HTML);
      instance.SetDestructor(&destruct_NicaPackage2HTML);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaPackage2HTML*)
   {
      return GenerateInitInstanceLocal((::NicaPackage2HTML*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaPackage2HTML*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaAnaFile(void *p = nullptr);
   static void *newArray_NicaAnaFile(Long_t size, void *p);
   static void delete_NicaAnaFile(void *p);
   static void deleteArray_NicaAnaFile(void *p);
   static void destruct_NicaAnaFile(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaAnaFile*)
   {
      ::NicaAnaFile *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaAnaFile >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaAnaFile", ::NicaAnaFile::Class_Version(), "NicaAnaFile.h", 26,
                  typeid(::NicaAnaFile), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaAnaFile::Dictionary, isa_proxy, 4,
                  sizeof(::NicaAnaFile) );
      instance.SetNew(&new_NicaAnaFile);
      instance.SetNewArray(&newArray_NicaAnaFile);
      instance.SetDelete(&delete_NicaAnaFile);
      instance.SetDeleteArray(&deleteArray_NicaAnaFile);
      instance.SetDestructor(&destruct_NicaAnaFile);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaAnaFile*)
   {
      return GenerateInitInstanceLocal((::NicaAnaFile*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaAnaFile*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaHtmlCore(void *p = nullptr);
   static void *newArray_NicaHtmlCore(Long_t size, void *p);
   static void delete_NicaHtmlCore(void *p);
   static void deleteArray_NicaHtmlCore(void *p);
   static void destruct_NicaHtmlCore(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHtmlCore*)
   {
      ::NicaHtmlCore *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHtmlCore >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHtmlCore", ::NicaHtmlCore::Class_Version(), "NicaHtmlCore.h", 20,
                  typeid(::NicaHtmlCore), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaHtmlCore::Dictionary, isa_proxy, 4,
                  sizeof(::NicaHtmlCore) );
      instance.SetNew(&new_NicaHtmlCore);
      instance.SetNewArray(&newArray_NicaHtmlCore);
      instance.SetDelete(&delete_NicaHtmlCore);
      instance.SetDeleteArray(&deleteArray_NicaHtmlCore);
      instance.SetDestructor(&destruct_NicaHtmlCore);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHtmlCore*)
   {
      return GenerateInitInstanceLocal((::NicaHtmlCore*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHtmlCore*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaPackageSql(void *p);
   static void deleteArray_NicaPackageSql(void *p);
   static void destruct_NicaPackageSql(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaPackageSql*)
   {
      ::NicaPackageSql *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaPackageSql >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaPackageSql", ::NicaPackageSql::Class_Version(), "NicaPackageSql.h", 22,
                  typeid(::NicaPackageSql), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaPackageSql::Dictionary, isa_proxy, 4,
                  sizeof(::NicaPackageSql) );
      instance.SetDelete(&delete_NicaPackageSql);
      instance.SetDeleteArray(&deleteArray_NicaPackageSql);
      instance.SetDestructor(&destruct_NicaPackageSql);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaPackageSql*)
   {
      return GenerateInitInstanceLocal((::NicaPackageSql*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaPackageSql*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaHtmlObject(void *p);
   static void deleteArray_NicaHtmlObject(void *p);
   static void destruct_NicaHtmlObject(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHtmlObject*)
   {
      ::NicaHtmlObject *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHtmlObject >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHtmlObject", ::NicaHtmlObject::Class_Version(), "NicaHtmlObject.h", 14,
                  typeid(::NicaHtmlObject), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaHtmlObject::Dictionary, isa_proxy, 4,
                  sizeof(::NicaHtmlObject) );
      instance.SetDelete(&delete_NicaHtmlObject);
      instance.SetDeleteArray(&deleteArray_NicaHtmlObject);
      instance.SetDestructor(&destruct_NicaHtmlObject);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHtmlObject*)
   {
      return GenerateInitInstanceLocal((::NicaHtmlObject*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHtmlObject*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaHtmlDiv(void *p = nullptr);
   static void *newArray_NicaHtmlDiv(Long_t size, void *p);
   static void delete_NicaHtmlDiv(void *p);
   static void deleteArray_NicaHtmlDiv(void *p);
   static void destruct_NicaHtmlDiv(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHtmlDiv*)
   {
      ::NicaHtmlDiv *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHtmlDiv >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHtmlDiv", ::NicaHtmlDiv::Class_Version(), "NicaHtmlDiv.h", 13,
                  typeid(::NicaHtmlDiv), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaHtmlDiv::Dictionary, isa_proxy, 4,
                  sizeof(::NicaHtmlDiv) );
      instance.SetNew(&new_NicaHtmlDiv);
      instance.SetNewArray(&newArray_NicaHtmlDiv);
      instance.SetDelete(&delete_NicaHtmlDiv);
      instance.SetDeleteArray(&deleteArray_NicaHtmlDiv);
      instance.SetDestructor(&destruct_NicaHtmlDiv);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHtmlDiv*)
   {
      return GenerateInitInstanceLocal((::NicaHtmlDiv*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHtmlDiv*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaHtmlFile(void *p = nullptr);
   static void *newArray_NicaHtmlFile(Long_t size, void *p);
   static void delete_NicaHtmlFile(void *p);
   static void deleteArray_NicaHtmlFile(void *p);
   static void destruct_NicaHtmlFile(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHtmlFile*)
   {
      ::NicaHtmlFile *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHtmlFile >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHtmlFile", ::NicaHtmlFile::Class_Version(), "NicaHtmlFile.h", 18,
                  typeid(::NicaHtmlFile), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaHtmlFile::Dictionary, isa_proxy, 4,
                  sizeof(::NicaHtmlFile) );
      instance.SetNew(&new_NicaHtmlFile);
      instance.SetNewArray(&newArray_NicaHtmlFile);
      instance.SetDelete(&delete_NicaHtmlFile);
      instance.SetDeleteArray(&deleteArray_NicaHtmlFile);
      instance.SetDestructor(&destruct_NicaHtmlFile);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHtmlFile*)
   {
      return GenerateInitInstanceLocal((::NicaHtmlFile*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHtmlFile*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaHtmlTable(void *p = nullptr);
   static void *newArray_NicaHtmlTable(Long_t size, void *p);
   static void delete_NicaHtmlTable(void *p);
   static void deleteArray_NicaHtmlTable(void *p);
   static void destruct_NicaHtmlTable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHtmlTable*)
   {
      ::NicaHtmlTable *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHtmlTable >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHtmlTable", ::NicaHtmlTable::Class_Version(), "NicaHtmlTable.h", 13,
                  typeid(::NicaHtmlTable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaHtmlTable::Dictionary, isa_proxy, 4,
                  sizeof(::NicaHtmlTable) );
      instance.SetNew(&new_NicaHtmlTable);
      instance.SetNewArray(&newArray_NicaHtmlTable);
      instance.SetDelete(&delete_NicaHtmlTable);
      instance.SetDeleteArray(&deleteArray_NicaHtmlTable);
      instance.SetDestructor(&destruct_NicaHtmlTable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHtmlTable*)
   {
      return GenerateInitInstanceLocal((::NicaHtmlTable*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHtmlTable*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaHtmlRow(void *p = nullptr);
   static void *newArray_NicaHtmlRow(Long_t size, void *p);
   static void delete_NicaHtmlRow(void *p);
   static void deleteArray_NicaHtmlRow(void *p);
   static void destruct_NicaHtmlRow(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHtmlRow*)
   {
      ::NicaHtmlRow *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHtmlRow >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHtmlRow", ::NicaHtmlRow::Class_Version(), "NicaHtmlTable.h", 23,
                  typeid(::NicaHtmlRow), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaHtmlRow::Dictionary, isa_proxy, 4,
                  sizeof(::NicaHtmlRow) );
      instance.SetNew(&new_NicaHtmlRow);
      instance.SetNewArray(&newArray_NicaHtmlRow);
      instance.SetDelete(&delete_NicaHtmlRow);
      instance.SetDeleteArray(&deleteArray_NicaHtmlRow);
      instance.SetDestructor(&destruct_NicaHtmlRow);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHtmlRow*)
   {
      return GenerateInitInstanceLocal((::NicaHtmlRow*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHtmlRow*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaHtmlCell(void *p = nullptr);
   static void *newArray_NicaHtmlCell(Long_t size, void *p);
   static void delete_NicaHtmlCell(void *p);
   static void deleteArray_NicaHtmlCell(void *p);
   static void destruct_NicaHtmlCell(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHtmlCell*)
   {
      ::NicaHtmlCell *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHtmlCell >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHtmlCell", ::NicaHtmlCell::Class_Version(), "NicaHtmlTable.h", 33,
                  typeid(::NicaHtmlCell), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaHtmlCell::Dictionary, isa_proxy, 4,
                  sizeof(::NicaHtmlCell) );
      instance.SetNew(&new_NicaHtmlCell);
      instance.SetNewArray(&newArray_NicaHtmlCell);
      instance.SetDelete(&delete_NicaHtmlCell);
      instance.SetDeleteArray(&deleteArray_NicaHtmlCell);
      instance.SetDestructor(&destruct_NicaHtmlCell);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHtmlCell*)
   {
      return GenerateInitInstanceLocal((::NicaHtmlCell*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHtmlCell*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaHtmlCellCol(void *p = nullptr);
   static void *newArray_NicaHtmlCellCol(Long_t size, void *p);
   static void delete_NicaHtmlCellCol(void *p);
   static void deleteArray_NicaHtmlCellCol(void *p);
   static void destruct_NicaHtmlCellCol(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHtmlCellCol*)
   {
      ::NicaHtmlCellCol *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHtmlCellCol >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHtmlCellCol", ::NicaHtmlCellCol::Class_Version(), "NicaHtmlTable.h", 50,
                  typeid(::NicaHtmlCellCol), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaHtmlCellCol::Dictionary, isa_proxy, 4,
                  sizeof(::NicaHtmlCellCol) );
      instance.SetNew(&new_NicaHtmlCellCol);
      instance.SetNewArray(&newArray_NicaHtmlCellCol);
      instance.SetDelete(&delete_NicaHtmlCellCol);
      instance.SetDeleteArray(&deleteArray_NicaHtmlCellCol);
      instance.SetDestructor(&destruct_NicaHtmlCellCol);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHtmlCellCol*)
   {
      return GenerateInitInstanceLocal((::NicaHtmlCellCol*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHtmlCellCol*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaHtmlCellRow(void *p = nullptr);
   static void *newArray_NicaHtmlCellRow(Long_t size, void *p);
   static void delete_NicaHtmlCellRow(void *p);
   static void deleteArray_NicaHtmlCellRow(void *p);
   static void destruct_NicaHtmlCellRow(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHtmlCellRow*)
   {
      ::NicaHtmlCellRow *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHtmlCellRow >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHtmlCellRow", ::NicaHtmlCellRow::Class_Version(), "NicaHtmlTable.h", 58,
                  typeid(::NicaHtmlCellRow), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaHtmlCellRow::Dictionary, isa_proxy, 4,
                  sizeof(::NicaHtmlCellRow) );
      instance.SetNew(&new_NicaHtmlCellRow);
      instance.SetNewArray(&newArray_NicaHtmlCellRow);
      instance.SetDelete(&delete_NicaHtmlCellRow);
      instance.SetDeleteArray(&deleteArray_NicaHtmlCellRow);
      instance.SetDestructor(&destruct_NicaHtmlCellRow);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHtmlCellRow*)
   {
      return GenerateInitInstanceLocal((::NicaHtmlCellRow*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHtmlCellRow*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaDividedHisto1D(void *p = nullptr);
   static void *newArray_NicaDividedHisto1D(Long_t size, void *p);
   static void delete_NicaDividedHisto1D(void *p);
   static void deleteArray_NicaDividedHisto1D(void *p);
   static void destruct_NicaDividedHisto1D(void *p);
   static Long64_t merge_NicaDividedHisto1D(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaDividedHisto1D*)
   {
      ::NicaDividedHisto1D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaDividedHisto1D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaDividedHisto1D", ::NicaDividedHisto1D::Class_Version(), "NicaDividedHisto.h", 27,
                  typeid(::NicaDividedHisto1D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaDividedHisto1D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaDividedHisto1D) );
      instance.SetNew(&new_NicaDividedHisto1D);
      instance.SetNewArray(&newArray_NicaDividedHisto1D);
      instance.SetDelete(&delete_NicaDividedHisto1D);
      instance.SetDeleteArray(&deleteArray_NicaDividedHisto1D);
      instance.SetDestructor(&destruct_NicaDividedHisto1D);
      instance.SetMerge(&merge_NicaDividedHisto1D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaDividedHisto1D*)
   {
      return GenerateInitInstanceLocal((::NicaDividedHisto1D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaDividedHisto1D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaDividedHisto2D(void *p = nullptr);
   static void *newArray_NicaDividedHisto2D(Long_t size, void *p);
   static void delete_NicaDividedHisto2D(void *p);
   static void deleteArray_NicaDividedHisto2D(void *p);
   static void destruct_NicaDividedHisto2D(void *p);
   static Long64_t merge_NicaDividedHisto2D(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaDividedHisto2D*)
   {
      ::NicaDividedHisto2D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaDividedHisto2D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaDividedHisto2D", ::NicaDividedHisto2D::Class_Version(), "NicaDividedHisto.h", 383,
                  typeid(::NicaDividedHisto2D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaDividedHisto2D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaDividedHisto2D) );
      instance.SetNew(&new_NicaDividedHisto2D);
      instance.SetNewArray(&newArray_NicaDividedHisto2D);
      instance.SetDelete(&delete_NicaDividedHisto2D);
      instance.SetDeleteArray(&deleteArray_NicaDividedHisto2D);
      instance.SetDestructor(&destruct_NicaDividedHisto2D);
      instance.SetMerge(&merge_NicaDividedHisto2D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaDividedHisto2D*)
   {
      return GenerateInitInstanceLocal((::NicaDividedHisto2D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaDividedHisto2D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaDividedHisto3D(void *p = nullptr);
   static void *newArray_NicaDividedHisto3D(Long_t size, void *p);
   static void delete_NicaDividedHisto3D(void *p);
   static void deleteArray_NicaDividedHisto3D(void *p);
   static void destruct_NicaDividedHisto3D(void *p);
   static Long64_t merge_NicaDividedHisto3D(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaDividedHisto3D*)
   {
      ::NicaDividedHisto3D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaDividedHisto3D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaDividedHisto3D", ::NicaDividedHisto3D::Class_Version(), "NicaDividedHisto.h", 529,
                  typeid(::NicaDividedHisto3D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaDividedHisto3D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaDividedHisto3D) );
      instance.SetNew(&new_NicaDividedHisto3D);
      instance.SetNewArray(&newArray_NicaDividedHisto3D);
      instance.SetDelete(&delete_NicaDividedHisto3D);
      instance.SetDeleteArray(&deleteArray_NicaDividedHisto3D);
      instance.SetDestructor(&destruct_NicaDividedHisto3D);
      instance.SetMerge(&merge_NicaDividedHisto3D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaDividedHisto3D*)
   {
      return GenerateInitInstanceLocal((::NicaDividedHisto3D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaDividedHisto3D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaHistogramAxisConf(void *p = nullptr);
   static void *newArray_NicaHistogramAxisConf(Long_t size, void *p);
   static void delete_NicaHistogramAxisConf(void *p);
   static void deleteArray_NicaHistogramAxisConf(void *p);
   static void destruct_NicaHistogramAxisConf(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramAxisConf*)
   {
      ::NicaHistogramAxisConf *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramAxisConf >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramAxisConf", ::NicaHistogramAxisConf::Class_Version(), "NicaHistogramManager.h", 21,
                  typeid(::NicaHistogramAxisConf), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaHistogramAxisConf::Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramAxisConf) );
      instance.SetNew(&new_NicaHistogramAxisConf);
      instance.SetNewArray(&newArray_NicaHistogramAxisConf);
      instance.SetDelete(&delete_NicaHistogramAxisConf);
      instance.SetDeleteArray(&deleteArray_NicaHistogramAxisConf);
      instance.SetDestructor(&destruct_NicaHistogramAxisConf);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramAxisConf*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramAxisConf*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramAxisConf*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_1_1DlETH1IgR_Dictionary();
   static void NicaHistogramManager_1_1DlETH1IgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_1_1DlETH1IgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_1_1DlETH1IgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_1_1DlETH1IgR(void *p);
   static void deleteArray_NicaHistogramManager_1_1DlETH1IgR(void *p);
   static void destruct_NicaHistogramManager_1_1DlETH1IgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_1_1D<TH1I>*)
   {
      ::NicaHistogramManager_1_1D<TH1I> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_1_1D<TH1I> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_1_1D<TH1I>", ::NicaHistogramManager_1_1D<TH1I>::Class_Version(), "NicaHistogramManager.h", 70,
                  typeid(::NicaHistogramManager_1_1D<TH1I>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_1_1DlETH1IgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_1_1D<TH1I>) );
      instance.SetNew(&new_NicaHistogramManager_1_1DlETH1IgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_1_1DlETH1IgR);
      instance.SetDelete(&delete_NicaHistogramManager_1_1DlETH1IgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_1_1DlETH1IgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_1_1DlETH1IgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_1_1D<TH1I>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_1_1D<TH1I>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1I>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_1_1DlETH1IgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1I>*)nullptr)->GetClass();
      NicaHistogramManager_1_1DlETH1IgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_1_1DlETH1IgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_1_1DlETH1FgR_Dictionary();
   static void NicaHistogramManager_1_1DlETH1FgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_1_1DlETH1FgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_1_1DlETH1FgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_1_1DlETH1FgR(void *p);
   static void deleteArray_NicaHistogramManager_1_1DlETH1FgR(void *p);
   static void destruct_NicaHistogramManager_1_1DlETH1FgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_1_1D<TH1F>*)
   {
      ::NicaHistogramManager_1_1D<TH1F> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_1_1D<TH1F> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_1_1D<TH1F>", ::NicaHistogramManager_1_1D<TH1F>::Class_Version(), "NicaHistogramManager.h", 70,
                  typeid(::NicaHistogramManager_1_1D<TH1F>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_1_1DlETH1FgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_1_1D<TH1F>) );
      instance.SetNew(&new_NicaHistogramManager_1_1DlETH1FgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_1_1DlETH1FgR);
      instance.SetDelete(&delete_NicaHistogramManager_1_1DlETH1FgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_1_1DlETH1FgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_1_1DlETH1FgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_1_1D<TH1F>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_1_1D<TH1F>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1F>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_1_1DlETH1FgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1F>*)nullptr)->GetClass();
      NicaHistogramManager_1_1DlETH1FgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_1_1DlETH1FgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_1_1DlETH1DgR_Dictionary();
   static void NicaHistogramManager_1_1DlETH1DgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_1_1DlETH1DgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_1_1DlETH1DgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_1_1DlETH1DgR(void *p);
   static void deleteArray_NicaHistogramManager_1_1DlETH1DgR(void *p);
   static void destruct_NicaHistogramManager_1_1DlETH1DgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_1_1D<TH1D>*)
   {
      ::NicaHistogramManager_1_1D<TH1D> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_1_1D<TH1D> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_1_1D<TH1D>", ::NicaHistogramManager_1_1D<TH1D>::Class_Version(), "NicaHistogramManager.h", 70,
                  typeid(::NicaHistogramManager_1_1D<TH1D>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_1_1DlETH1DgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_1_1D<TH1D>) );
      instance.SetNew(&new_NicaHistogramManager_1_1DlETH1DgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_1_1DlETH1DgR);
      instance.SetDelete(&delete_NicaHistogramManager_1_1DlETH1DgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_1_1DlETH1DgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_1_1DlETH1DgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_1_1D<TH1D>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_1_1D<TH1D>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1D>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_1_1DlETH1DgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1D>*)nullptr)->GetClass();
      NicaHistogramManager_1_1DlETH1DgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_1_1DlETH1DgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_1_2DlETH2IgR_Dictionary();
   static void NicaHistogramManager_1_2DlETH2IgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_1_2DlETH2IgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_1_2DlETH2IgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_1_2DlETH2IgR(void *p);
   static void deleteArray_NicaHistogramManager_1_2DlETH2IgR(void *p);
   static void destruct_NicaHistogramManager_1_2DlETH2IgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_1_2D<TH2I>*)
   {
      ::NicaHistogramManager_1_2D<TH2I> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_1_2D<TH2I> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_1_2D<TH2I>", ::NicaHistogramManager_1_2D<TH2I>::Class_Version(), "NicaHistogramManager.h", 183,
                  typeid(::NicaHistogramManager_1_2D<TH2I>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_1_2DlETH2IgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_1_2D<TH2I>) );
      instance.SetNew(&new_NicaHistogramManager_1_2DlETH2IgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_1_2DlETH2IgR);
      instance.SetDelete(&delete_NicaHistogramManager_1_2DlETH2IgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_1_2DlETH2IgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_1_2DlETH2IgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_1_2D<TH2I>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_1_2D<TH2I>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2I>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_1_2DlETH2IgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2I>*)nullptr)->GetClass();
      NicaHistogramManager_1_2DlETH2IgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_1_2DlETH2IgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_1_2DlETH2FgR_Dictionary();
   static void NicaHistogramManager_1_2DlETH2FgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_1_2DlETH2FgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_1_2DlETH2FgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_1_2DlETH2FgR(void *p);
   static void deleteArray_NicaHistogramManager_1_2DlETH2FgR(void *p);
   static void destruct_NicaHistogramManager_1_2DlETH2FgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_1_2D<TH2F>*)
   {
      ::NicaHistogramManager_1_2D<TH2F> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_1_2D<TH2F> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_1_2D<TH2F>", ::NicaHistogramManager_1_2D<TH2F>::Class_Version(), "NicaHistogramManager.h", 183,
                  typeid(::NicaHistogramManager_1_2D<TH2F>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_1_2DlETH2FgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_1_2D<TH2F>) );
      instance.SetNew(&new_NicaHistogramManager_1_2DlETH2FgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_1_2DlETH2FgR);
      instance.SetDelete(&delete_NicaHistogramManager_1_2DlETH2FgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_1_2DlETH2FgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_1_2DlETH2FgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_1_2D<TH2F>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_1_2D<TH2F>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2F>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_1_2DlETH2FgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2F>*)nullptr)->GetClass();
      NicaHistogramManager_1_2DlETH2FgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_1_2DlETH2FgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_1_2DlETH2DgR_Dictionary();
   static void NicaHistogramManager_1_2DlETH2DgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_1_2DlETH2DgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_1_2DlETH2DgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_1_2DlETH2DgR(void *p);
   static void deleteArray_NicaHistogramManager_1_2DlETH2DgR(void *p);
   static void destruct_NicaHistogramManager_1_2DlETH2DgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_1_2D<TH2D>*)
   {
      ::NicaHistogramManager_1_2D<TH2D> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_1_2D<TH2D> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_1_2D<TH2D>", ::NicaHistogramManager_1_2D<TH2D>::Class_Version(), "NicaHistogramManager.h", 183,
                  typeid(::NicaHistogramManager_1_2D<TH2D>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_1_2DlETH2DgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_1_2D<TH2D>) );
      instance.SetNew(&new_NicaHistogramManager_1_2DlETH2DgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_1_2DlETH2DgR);
      instance.SetDelete(&delete_NicaHistogramManager_1_2DlETH2DgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_1_2DlETH2DgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_1_2DlETH2DgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_1_2D<TH2D>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_1_2D<TH2D>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2D>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_1_2DlETH2DgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2D>*)nullptr)->GetClass();
      NicaHistogramManager_1_2DlETH2DgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_1_2DlETH2DgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_1_3DlETH3IgR_Dictionary();
   static void NicaHistogramManager_1_3DlETH3IgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_1_3DlETH3IgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_1_3DlETH3IgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_1_3DlETH3IgR(void *p);
   static void deleteArray_NicaHistogramManager_1_3DlETH3IgR(void *p);
   static void destruct_NicaHistogramManager_1_3DlETH3IgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_1_3D<TH3I>*)
   {
      ::NicaHistogramManager_1_3D<TH3I> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_1_3D<TH3I> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_1_3D<TH3I>", ::NicaHistogramManager_1_3D<TH3I>::Class_Version(), "NicaHistogramManager.h", 304,
                  typeid(::NicaHistogramManager_1_3D<TH3I>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_1_3DlETH3IgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_1_3D<TH3I>) );
      instance.SetNew(&new_NicaHistogramManager_1_3DlETH3IgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_1_3DlETH3IgR);
      instance.SetDelete(&delete_NicaHistogramManager_1_3DlETH3IgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_1_3DlETH3IgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_1_3DlETH3IgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_1_3D<TH3I>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_1_3D<TH3I>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3I>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_1_3DlETH3IgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3I>*)nullptr)->GetClass();
      NicaHistogramManager_1_3DlETH3IgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_1_3DlETH3IgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_1_3DlETH3FgR_Dictionary();
   static void NicaHistogramManager_1_3DlETH3FgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_1_3DlETH3FgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_1_3DlETH3FgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_1_3DlETH3FgR(void *p);
   static void deleteArray_NicaHistogramManager_1_3DlETH3FgR(void *p);
   static void destruct_NicaHistogramManager_1_3DlETH3FgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_1_3D<TH3F>*)
   {
      ::NicaHistogramManager_1_3D<TH3F> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_1_3D<TH3F> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_1_3D<TH3F>", ::NicaHistogramManager_1_3D<TH3F>::Class_Version(), "NicaHistogramManager.h", 304,
                  typeid(::NicaHistogramManager_1_3D<TH3F>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_1_3DlETH3FgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_1_3D<TH3F>) );
      instance.SetNew(&new_NicaHistogramManager_1_3DlETH3FgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_1_3DlETH3FgR);
      instance.SetDelete(&delete_NicaHistogramManager_1_3DlETH3FgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_1_3DlETH3FgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_1_3DlETH3FgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_1_3D<TH3F>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_1_3D<TH3F>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3F>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_1_3DlETH3FgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3F>*)nullptr)->GetClass();
      NicaHistogramManager_1_3DlETH3FgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_1_3DlETH3FgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_1_3DlETH3DgR_Dictionary();
   static void NicaHistogramManager_1_3DlETH3DgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_1_3DlETH3DgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_1_3DlETH3DgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_1_3DlETH3DgR(void *p);
   static void deleteArray_NicaHistogramManager_1_3DlETH3DgR(void *p);
   static void destruct_NicaHistogramManager_1_3DlETH3DgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_1_3D<TH3D>*)
   {
      ::NicaHistogramManager_1_3D<TH3D> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_1_3D<TH3D> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_1_3D<TH3D>", ::NicaHistogramManager_1_3D<TH3D>::Class_Version(), "NicaHistogramManager.h", 304,
                  typeid(::NicaHistogramManager_1_3D<TH3D>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_1_3DlETH3DgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_1_3D<TH3D>) );
      instance.SetNew(&new_NicaHistogramManager_1_3DlETH3DgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_1_3DlETH3DgR);
      instance.SetDelete(&delete_NicaHistogramManager_1_3DlETH3DgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_1_3DlETH3DgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_1_3DlETH3DgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_1_3D<TH3D>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_1_3D<TH3D>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3D>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_1_3DlETH3DgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3D>*)nullptr)->GetClass();
      NicaHistogramManager_1_3DlETH3DgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_1_3DlETH3DgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_2_1DlETH1IgR_Dictionary();
   static void NicaHistogramManager_2_1DlETH1IgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_2_1DlETH1IgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_2_1DlETH1IgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_2_1DlETH1IgR(void *p);
   static void deleteArray_NicaHistogramManager_2_1DlETH1IgR(void *p);
   static void destruct_NicaHistogramManager_2_1DlETH1IgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_2_1D<TH1I>*)
   {
      ::NicaHistogramManager_2_1D<TH1I> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_2_1D<TH1I> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_2_1D<TH1I>", ::NicaHistogramManager_2_1D<TH1I>::Class_Version(), "NicaHistogramManager.h", 434,
                  typeid(::NicaHistogramManager_2_1D<TH1I>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_2_1DlETH1IgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_2_1D<TH1I>) );
      instance.SetNew(&new_NicaHistogramManager_2_1DlETH1IgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_2_1DlETH1IgR);
      instance.SetDelete(&delete_NicaHistogramManager_2_1DlETH1IgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_2_1DlETH1IgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_2_1DlETH1IgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_2_1D<TH1I>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_2_1D<TH1I>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1I>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_2_1DlETH1IgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1I>*)nullptr)->GetClass();
      NicaHistogramManager_2_1DlETH1IgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_2_1DlETH1IgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_2_1DlETH1FgR_Dictionary();
   static void NicaHistogramManager_2_1DlETH1FgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_2_1DlETH1FgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_2_1DlETH1FgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_2_1DlETH1FgR(void *p);
   static void deleteArray_NicaHistogramManager_2_1DlETH1FgR(void *p);
   static void destruct_NicaHistogramManager_2_1DlETH1FgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_2_1D<TH1F>*)
   {
      ::NicaHistogramManager_2_1D<TH1F> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_2_1D<TH1F> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_2_1D<TH1F>", ::NicaHistogramManager_2_1D<TH1F>::Class_Version(), "NicaHistogramManager.h", 434,
                  typeid(::NicaHistogramManager_2_1D<TH1F>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_2_1DlETH1FgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_2_1D<TH1F>) );
      instance.SetNew(&new_NicaHistogramManager_2_1DlETH1FgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_2_1DlETH1FgR);
      instance.SetDelete(&delete_NicaHistogramManager_2_1DlETH1FgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_2_1DlETH1FgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_2_1DlETH1FgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_2_1D<TH1F>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_2_1D<TH1F>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1F>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_2_1DlETH1FgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1F>*)nullptr)->GetClass();
      NicaHistogramManager_2_1DlETH1FgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_2_1DlETH1FgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_2_1DlETH1DgR_Dictionary();
   static void NicaHistogramManager_2_1DlETH1DgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_2_1DlETH1DgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_2_1DlETH1DgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_2_1DlETH1DgR(void *p);
   static void deleteArray_NicaHistogramManager_2_1DlETH1DgR(void *p);
   static void destruct_NicaHistogramManager_2_1DlETH1DgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_2_1D<TH1D>*)
   {
      ::NicaHistogramManager_2_1D<TH1D> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_2_1D<TH1D> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_2_1D<TH1D>", ::NicaHistogramManager_2_1D<TH1D>::Class_Version(), "NicaHistogramManager.h", 434,
                  typeid(::NicaHistogramManager_2_1D<TH1D>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_2_1DlETH1DgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_2_1D<TH1D>) );
      instance.SetNew(&new_NicaHistogramManager_2_1DlETH1DgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_2_1DlETH1DgR);
      instance.SetDelete(&delete_NicaHistogramManager_2_1DlETH1DgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_2_1DlETH1DgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_2_1DlETH1DgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_2_1D<TH1D>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_2_1D<TH1D>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1D>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_2_1DlETH1DgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1D>*)nullptr)->GetClass();
      NicaHistogramManager_2_1DlETH1DgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_2_1DlETH1DgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_2_2DlETH2IgR_Dictionary();
   static void NicaHistogramManager_2_2DlETH2IgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_2_2DlETH2IgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_2_2DlETH2IgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_2_2DlETH2IgR(void *p);
   static void deleteArray_NicaHistogramManager_2_2DlETH2IgR(void *p);
   static void destruct_NicaHistogramManager_2_2DlETH2IgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_2_2D<TH2I>*)
   {
      ::NicaHistogramManager_2_2D<TH2I> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_2_2D<TH2I> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_2_2D<TH2I>", ::NicaHistogramManager_2_2D<TH2I>::Class_Version(), "NicaHistogramManager.h", 530,
                  typeid(::NicaHistogramManager_2_2D<TH2I>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_2_2DlETH2IgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_2_2D<TH2I>) );
      instance.SetNew(&new_NicaHistogramManager_2_2DlETH2IgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_2_2DlETH2IgR);
      instance.SetDelete(&delete_NicaHistogramManager_2_2DlETH2IgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_2_2DlETH2IgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_2_2DlETH2IgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_2_2D<TH2I>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_2_2D<TH2I>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2I>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_2_2DlETH2IgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2I>*)nullptr)->GetClass();
      NicaHistogramManager_2_2DlETH2IgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_2_2DlETH2IgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_2_2DlETH2FgR_Dictionary();
   static void NicaHistogramManager_2_2DlETH2FgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_2_2DlETH2FgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_2_2DlETH2FgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_2_2DlETH2FgR(void *p);
   static void deleteArray_NicaHistogramManager_2_2DlETH2FgR(void *p);
   static void destruct_NicaHistogramManager_2_2DlETH2FgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_2_2D<TH2F>*)
   {
      ::NicaHistogramManager_2_2D<TH2F> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_2_2D<TH2F> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_2_2D<TH2F>", ::NicaHistogramManager_2_2D<TH2F>::Class_Version(), "NicaHistogramManager.h", 530,
                  typeid(::NicaHistogramManager_2_2D<TH2F>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_2_2DlETH2FgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_2_2D<TH2F>) );
      instance.SetNew(&new_NicaHistogramManager_2_2DlETH2FgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_2_2DlETH2FgR);
      instance.SetDelete(&delete_NicaHistogramManager_2_2DlETH2FgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_2_2DlETH2FgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_2_2DlETH2FgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_2_2D<TH2F>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_2_2D<TH2F>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2F>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_2_2DlETH2FgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2F>*)nullptr)->GetClass();
      NicaHistogramManager_2_2DlETH2FgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_2_2DlETH2FgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_2_2DlETH2DgR_Dictionary();
   static void NicaHistogramManager_2_2DlETH2DgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_2_2DlETH2DgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_2_2DlETH2DgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_2_2DlETH2DgR(void *p);
   static void deleteArray_NicaHistogramManager_2_2DlETH2DgR(void *p);
   static void destruct_NicaHistogramManager_2_2DlETH2DgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_2_2D<TH2D>*)
   {
      ::NicaHistogramManager_2_2D<TH2D> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_2_2D<TH2D> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_2_2D<TH2D>", ::NicaHistogramManager_2_2D<TH2D>::Class_Version(), "NicaHistogramManager.h", 530,
                  typeid(::NicaHistogramManager_2_2D<TH2D>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_2_2DlETH2DgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_2_2D<TH2D>) );
      instance.SetNew(&new_NicaHistogramManager_2_2DlETH2DgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_2_2DlETH2DgR);
      instance.SetDelete(&delete_NicaHistogramManager_2_2DlETH2DgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_2_2DlETH2DgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_2_2DlETH2DgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_2_2D<TH2D>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_2_2D<TH2D>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2D>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_2_2DlETH2DgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2D>*)nullptr)->GetClass();
      NicaHistogramManager_2_2DlETH2DgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_2_2DlETH2DgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_2_3DlETH3IgR_Dictionary();
   static void NicaHistogramManager_2_3DlETH3IgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_2_3DlETH3IgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_2_3DlETH3IgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_2_3DlETH3IgR(void *p);
   static void deleteArray_NicaHistogramManager_2_3DlETH3IgR(void *p);
   static void destruct_NicaHistogramManager_2_3DlETH3IgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_2_3D<TH3I>*)
   {
      ::NicaHistogramManager_2_3D<TH3I> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_2_3D<TH3I> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_2_3D<TH3I>", ::NicaHistogramManager_2_3D<TH3I>::Class_Version(), "NicaHistogramManager.h", 623,
                  typeid(::NicaHistogramManager_2_3D<TH3I>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_2_3DlETH3IgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_2_3D<TH3I>) );
      instance.SetNew(&new_NicaHistogramManager_2_3DlETH3IgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_2_3DlETH3IgR);
      instance.SetDelete(&delete_NicaHistogramManager_2_3DlETH3IgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_2_3DlETH3IgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_2_3DlETH3IgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_2_3D<TH3I>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_2_3D<TH3I>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3I>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_2_3DlETH3IgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3I>*)nullptr)->GetClass();
      NicaHistogramManager_2_3DlETH3IgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_2_3DlETH3IgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_2_3DlETH3FgR_Dictionary();
   static void NicaHistogramManager_2_3DlETH3FgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_2_3DlETH3FgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_2_3DlETH3FgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_2_3DlETH3FgR(void *p);
   static void deleteArray_NicaHistogramManager_2_3DlETH3FgR(void *p);
   static void destruct_NicaHistogramManager_2_3DlETH3FgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_2_3D<TH3F>*)
   {
      ::NicaHistogramManager_2_3D<TH3F> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_2_3D<TH3F> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_2_3D<TH3F>", ::NicaHistogramManager_2_3D<TH3F>::Class_Version(), "NicaHistogramManager.h", 623,
                  typeid(::NicaHistogramManager_2_3D<TH3F>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_2_3DlETH3FgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_2_3D<TH3F>) );
      instance.SetNew(&new_NicaHistogramManager_2_3DlETH3FgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_2_3DlETH3FgR);
      instance.SetDelete(&delete_NicaHistogramManager_2_3DlETH3FgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_2_3DlETH3FgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_2_3DlETH3FgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_2_3D<TH3F>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_2_3D<TH3F>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3F>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_2_3DlETH3FgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3F>*)nullptr)->GetClass();
      NicaHistogramManager_2_3DlETH3FgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_2_3DlETH3FgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_2_3DlETH3DgR_Dictionary();
   static void NicaHistogramManager_2_3DlETH3DgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_2_3DlETH3DgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_2_3DlETH3DgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_2_3DlETH3DgR(void *p);
   static void deleteArray_NicaHistogramManager_2_3DlETH3DgR(void *p);
   static void destruct_NicaHistogramManager_2_3DlETH3DgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_2_3D<TH3D>*)
   {
      ::NicaHistogramManager_2_3D<TH3D> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_2_3D<TH3D> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_2_3D<TH3D>", ::NicaHistogramManager_2_3D<TH3D>::Class_Version(), "NicaHistogramManager.h", 623,
                  typeid(::NicaHistogramManager_2_3D<TH3D>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_2_3DlETH3DgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_2_3D<TH3D>) );
      instance.SetNew(&new_NicaHistogramManager_2_3DlETH3DgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_2_3DlETH3DgR);
      instance.SetDelete(&delete_NicaHistogramManager_2_3DlETH3DgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_2_3DlETH3DgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_2_3DlETH3DgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_2_3D<TH3D>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_2_3D<TH3D>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3D>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_2_3DlETH3DgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3D>*)nullptr)->GetClass();
      NicaHistogramManager_2_3DlETH3DgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_2_3DlETH3DgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_3_1DlETH1IgR_Dictionary();
   static void NicaHistogramManager_3_1DlETH1IgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_3_1DlETH1IgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_3_1DlETH1IgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_3_1DlETH1IgR(void *p);
   static void deleteArray_NicaHistogramManager_3_1DlETH1IgR(void *p);
   static void destruct_NicaHistogramManager_3_1DlETH1IgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_3_1D<TH1I>*)
   {
      ::NicaHistogramManager_3_1D<TH1I> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_3_1D<TH1I> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_3_1D<TH1I>", ::NicaHistogramManager_3_1D<TH1I>::Class_Version(), "NicaHistogramManager.h", 726,
                  typeid(::NicaHistogramManager_3_1D<TH1I>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_3_1DlETH1IgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_3_1D<TH1I>) );
      instance.SetNew(&new_NicaHistogramManager_3_1DlETH1IgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_3_1DlETH1IgR);
      instance.SetDelete(&delete_NicaHistogramManager_3_1DlETH1IgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_3_1DlETH1IgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_3_1DlETH1IgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_3_1D<TH1I>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_3_1D<TH1I>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1I>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_3_1DlETH1IgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1I>*)nullptr)->GetClass();
      NicaHistogramManager_3_1DlETH1IgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_3_1DlETH1IgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_3_1DlETH1FgR_Dictionary();
   static void NicaHistogramManager_3_1DlETH1FgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_3_1DlETH1FgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_3_1DlETH1FgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_3_1DlETH1FgR(void *p);
   static void deleteArray_NicaHistogramManager_3_1DlETH1FgR(void *p);
   static void destruct_NicaHistogramManager_3_1DlETH1FgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_3_1D<TH1F>*)
   {
      ::NicaHistogramManager_3_1D<TH1F> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_3_1D<TH1F> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_3_1D<TH1F>", ::NicaHistogramManager_3_1D<TH1F>::Class_Version(), "NicaHistogramManager.h", 726,
                  typeid(::NicaHistogramManager_3_1D<TH1F>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_3_1DlETH1FgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_3_1D<TH1F>) );
      instance.SetNew(&new_NicaHistogramManager_3_1DlETH1FgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_3_1DlETH1FgR);
      instance.SetDelete(&delete_NicaHistogramManager_3_1DlETH1FgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_3_1DlETH1FgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_3_1DlETH1FgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_3_1D<TH1F>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_3_1D<TH1F>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1F>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_3_1DlETH1FgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1F>*)nullptr)->GetClass();
      NicaHistogramManager_3_1DlETH1FgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_3_1DlETH1FgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_3_1DlETH1DgR_Dictionary();
   static void NicaHistogramManager_3_1DlETH1DgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_3_1DlETH1DgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_3_1DlETH1DgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_3_1DlETH1DgR(void *p);
   static void deleteArray_NicaHistogramManager_3_1DlETH1DgR(void *p);
   static void destruct_NicaHistogramManager_3_1DlETH1DgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_3_1D<TH1D>*)
   {
      ::NicaHistogramManager_3_1D<TH1D> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_3_1D<TH1D> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_3_1D<TH1D>", ::NicaHistogramManager_3_1D<TH1D>::Class_Version(), "NicaHistogramManager.h", 726,
                  typeid(::NicaHistogramManager_3_1D<TH1D>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_3_1DlETH1DgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_3_1D<TH1D>) );
      instance.SetNew(&new_NicaHistogramManager_3_1DlETH1DgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_3_1DlETH1DgR);
      instance.SetDelete(&delete_NicaHistogramManager_3_1DlETH1DgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_3_1DlETH1DgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_3_1DlETH1DgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_3_1D<TH1D>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_3_1D<TH1D>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1D>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_3_1DlETH1DgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1D>*)nullptr)->GetClass();
      NicaHistogramManager_3_1DlETH1DgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_3_1DlETH1DgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_3_2DlETH2IgR_Dictionary();
   static void NicaHistogramManager_3_2DlETH2IgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_3_2DlETH2IgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_3_2DlETH2IgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_3_2DlETH2IgR(void *p);
   static void deleteArray_NicaHistogramManager_3_2DlETH2IgR(void *p);
   static void destruct_NicaHistogramManager_3_2DlETH2IgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_3_2D<TH2I>*)
   {
      ::NicaHistogramManager_3_2D<TH2I> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_3_2D<TH2I> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_3_2D<TH2I>", ::NicaHistogramManager_3_2D<TH2I>::Class_Version(), "NicaHistogramManager.h", 822,
                  typeid(::NicaHistogramManager_3_2D<TH2I>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_3_2DlETH2IgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_3_2D<TH2I>) );
      instance.SetNew(&new_NicaHistogramManager_3_2DlETH2IgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_3_2DlETH2IgR);
      instance.SetDelete(&delete_NicaHistogramManager_3_2DlETH2IgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_3_2DlETH2IgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_3_2DlETH2IgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_3_2D<TH2I>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_3_2D<TH2I>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2I>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_3_2DlETH2IgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2I>*)nullptr)->GetClass();
      NicaHistogramManager_3_2DlETH2IgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_3_2DlETH2IgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_3_2DlETH2FgR_Dictionary();
   static void NicaHistogramManager_3_2DlETH2FgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_3_2DlETH2FgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_3_2DlETH2FgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_3_2DlETH2FgR(void *p);
   static void deleteArray_NicaHistogramManager_3_2DlETH2FgR(void *p);
   static void destruct_NicaHistogramManager_3_2DlETH2FgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_3_2D<TH2F>*)
   {
      ::NicaHistogramManager_3_2D<TH2F> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_3_2D<TH2F> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_3_2D<TH2F>", ::NicaHistogramManager_3_2D<TH2F>::Class_Version(), "NicaHistogramManager.h", 822,
                  typeid(::NicaHistogramManager_3_2D<TH2F>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_3_2DlETH2FgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_3_2D<TH2F>) );
      instance.SetNew(&new_NicaHistogramManager_3_2DlETH2FgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_3_2DlETH2FgR);
      instance.SetDelete(&delete_NicaHistogramManager_3_2DlETH2FgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_3_2DlETH2FgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_3_2DlETH2FgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_3_2D<TH2F>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_3_2D<TH2F>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2F>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_3_2DlETH2FgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2F>*)nullptr)->GetClass();
      NicaHistogramManager_3_2DlETH2FgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_3_2DlETH2FgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_3_2DlETH2DgR_Dictionary();
   static void NicaHistogramManager_3_2DlETH2DgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_3_2DlETH2DgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_3_2DlETH2DgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_3_2DlETH2DgR(void *p);
   static void deleteArray_NicaHistogramManager_3_2DlETH2DgR(void *p);
   static void destruct_NicaHistogramManager_3_2DlETH2DgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_3_2D<TH2D>*)
   {
      ::NicaHistogramManager_3_2D<TH2D> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_3_2D<TH2D> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_3_2D<TH2D>", ::NicaHistogramManager_3_2D<TH2D>::Class_Version(), "NicaHistogramManager.h", 822,
                  typeid(::NicaHistogramManager_3_2D<TH2D>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_3_2DlETH2DgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_3_2D<TH2D>) );
      instance.SetNew(&new_NicaHistogramManager_3_2DlETH2DgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_3_2DlETH2DgR);
      instance.SetDelete(&delete_NicaHistogramManager_3_2DlETH2DgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_3_2DlETH2DgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_3_2DlETH2DgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_3_2D<TH2D>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_3_2D<TH2D>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2D>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_3_2DlETH2DgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2D>*)nullptr)->GetClass();
      NicaHistogramManager_3_2DlETH2DgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_3_2DlETH2DgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_3_3DlETH3IgR_Dictionary();
   static void NicaHistogramManager_3_3DlETH3IgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_3_3DlETH3IgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_3_3DlETH3IgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_3_3DlETH3IgR(void *p);
   static void deleteArray_NicaHistogramManager_3_3DlETH3IgR(void *p);
   static void destruct_NicaHistogramManager_3_3DlETH3IgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_3_3D<TH3I>*)
   {
      ::NicaHistogramManager_3_3D<TH3I> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_3_3D<TH3I> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_3_3D<TH3I>", ::NicaHistogramManager_3_3D<TH3I>::Class_Version(), "NicaHistogramManager.h", 925,
                  typeid(::NicaHistogramManager_3_3D<TH3I>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_3_3DlETH3IgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_3_3D<TH3I>) );
      instance.SetNew(&new_NicaHistogramManager_3_3DlETH3IgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_3_3DlETH3IgR);
      instance.SetDelete(&delete_NicaHistogramManager_3_3DlETH3IgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_3_3DlETH3IgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_3_3DlETH3IgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_3_3D<TH3I>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_3_3D<TH3I>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3I>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_3_3DlETH3IgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3I>*)nullptr)->GetClass();
      NicaHistogramManager_3_3DlETH3IgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_3_3DlETH3IgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_3_3DlETH3FgR_Dictionary();
   static void NicaHistogramManager_3_3DlETH3FgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_3_3DlETH3FgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_3_3DlETH3FgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_3_3DlETH3FgR(void *p);
   static void deleteArray_NicaHistogramManager_3_3DlETH3FgR(void *p);
   static void destruct_NicaHistogramManager_3_3DlETH3FgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_3_3D<TH3F>*)
   {
      ::NicaHistogramManager_3_3D<TH3F> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_3_3D<TH3F> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_3_3D<TH3F>", ::NicaHistogramManager_3_3D<TH3F>::Class_Version(), "NicaHistogramManager.h", 925,
                  typeid(::NicaHistogramManager_3_3D<TH3F>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_3_3DlETH3FgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_3_3D<TH3F>) );
      instance.SetNew(&new_NicaHistogramManager_3_3DlETH3FgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_3_3DlETH3FgR);
      instance.SetDelete(&delete_NicaHistogramManager_3_3DlETH3FgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_3_3DlETH3FgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_3_3DlETH3FgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_3_3D<TH3F>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_3_3D<TH3F>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3F>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_3_3DlETH3FgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3F>*)nullptr)->GetClass();
      NicaHistogramManager_3_3DlETH3FgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_3_3DlETH3FgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_3_3DlETH3DgR_Dictionary();
   static void NicaHistogramManager_3_3DlETH3DgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_3_3DlETH3DgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_3_3DlETH3DgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_3_3DlETH3DgR(void *p);
   static void deleteArray_NicaHistogramManager_3_3DlETH3DgR(void *p);
   static void destruct_NicaHistogramManager_3_3DlETH3DgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_3_3D<TH3D>*)
   {
      ::NicaHistogramManager_3_3D<TH3D> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_3_3D<TH3D> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_3_3D<TH3D>", ::NicaHistogramManager_3_3D<TH3D>::Class_Version(), "NicaHistogramManager.h", 925,
                  typeid(::NicaHistogramManager_3_3D<TH3D>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_3_3DlETH3DgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_3_3D<TH3D>) );
      instance.SetNew(&new_NicaHistogramManager_3_3DlETH3DgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_3_3DlETH3DgR);
      instance.SetDelete(&delete_NicaHistogramManager_3_3DlETH3DgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_3_3DlETH3DgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_3_3DlETH3DgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_3_3D<TH3D>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_3_3D<TH3D>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3D>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_3_3DlETH3DgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3D>*)nullptr)->GetClass();
      NicaHistogramManager_3_3DlETH3DgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_3_3DlETH3DgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_4_1DlETH1IgR_Dictionary();
   static void NicaHistogramManager_4_1DlETH1IgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_4_1DlETH1IgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_4_1DlETH1IgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_4_1DlETH1IgR(void *p);
   static void deleteArray_NicaHistogramManager_4_1DlETH1IgR(void *p);
   static void destruct_NicaHistogramManager_4_1DlETH1IgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_4_1D<TH1I>*)
   {
      ::NicaHistogramManager_4_1D<TH1I> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_4_1D<TH1I> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_4_1D<TH1I>", ::NicaHistogramManager_4_1D<TH1I>::Class_Version(), "NicaHistogramManager.h", 1036,
                  typeid(::NicaHistogramManager_4_1D<TH1I>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_4_1DlETH1IgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_4_1D<TH1I>) );
      instance.SetNew(&new_NicaHistogramManager_4_1DlETH1IgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_4_1DlETH1IgR);
      instance.SetDelete(&delete_NicaHistogramManager_4_1DlETH1IgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_4_1DlETH1IgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_4_1DlETH1IgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_4_1D<TH1I>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_4_1D<TH1I>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1I>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_4_1DlETH1IgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1I>*)nullptr)->GetClass();
      NicaHistogramManager_4_1DlETH1IgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_4_1DlETH1IgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_4_1DlETH1FgR_Dictionary();
   static void NicaHistogramManager_4_1DlETH1FgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_4_1DlETH1FgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_4_1DlETH1FgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_4_1DlETH1FgR(void *p);
   static void deleteArray_NicaHistogramManager_4_1DlETH1FgR(void *p);
   static void destruct_NicaHistogramManager_4_1DlETH1FgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_4_1D<TH1F>*)
   {
      ::NicaHistogramManager_4_1D<TH1F> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_4_1D<TH1F> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_4_1D<TH1F>", ::NicaHistogramManager_4_1D<TH1F>::Class_Version(), "NicaHistogramManager.h", 1036,
                  typeid(::NicaHistogramManager_4_1D<TH1F>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_4_1DlETH1FgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_4_1D<TH1F>) );
      instance.SetNew(&new_NicaHistogramManager_4_1DlETH1FgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_4_1DlETH1FgR);
      instance.SetDelete(&delete_NicaHistogramManager_4_1DlETH1FgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_4_1DlETH1FgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_4_1DlETH1FgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_4_1D<TH1F>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_4_1D<TH1F>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1F>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_4_1DlETH1FgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1F>*)nullptr)->GetClass();
      NicaHistogramManager_4_1DlETH1FgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_4_1DlETH1FgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_4_1DlETH1DgR_Dictionary();
   static void NicaHistogramManager_4_1DlETH1DgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_4_1DlETH1DgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_4_1DlETH1DgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_4_1DlETH1DgR(void *p);
   static void deleteArray_NicaHistogramManager_4_1DlETH1DgR(void *p);
   static void destruct_NicaHistogramManager_4_1DlETH1DgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_4_1D<TH1D>*)
   {
      ::NicaHistogramManager_4_1D<TH1D> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_4_1D<TH1D> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_4_1D<TH1D>", ::NicaHistogramManager_4_1D<TH1D>::Class_Version(), "NicaHistogramManager.h", 1036,
                  typeid(::NicaHistogramManager_4_1D<TH1D>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_4_1DlETH1DgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_4_1D<TH1D>) );
      instance.SetNew(&new_NicaHistogramManager_4_1DlETH1DgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_4_1DlETH1DgR);
      instance.SetDelete(&delete_NicaHistogramManager_4_1DlETH1DgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_4_1DlETH1DgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_4_1DlETH1DgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_4_1D<TH1D>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_4_1D<TH1D>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1D>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_4_1DlETH1DgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1D>*)nullptr)->GetClass();
      NicaHistogramManager_4_1DlETH1DgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_4_1DlETH1DgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_4_2DlETH2IgR_Dictionary();
   static void NicaHistogramManager_4_2DlETH2IgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_4_2DlETH2IgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_4_2DlETH2IgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_4_2DlETH2IgR(void *p);
   static void deleteArray_NicaHistogramManager_4_2DlETH2IgR(void *p);
   static void destruct_NicaHistogramManager_4_2DlETH2IgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_4_2D<TH2I>*)
   {
      ::NicaHistogramManager_4_2D<TH2I> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_4_2D<TH2I> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_4_2D<TH2I>", ::NicaHistogramManager_4_2D<TH2I>::Class_Version(), "NicaHistogramManager.h", 1138,
                  typeid(::NicaHistogramManager_4_2D<TH2I>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_4_2DlETH2IgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_4_2D<TH2I>) );
      instance.SetNew(&new_NicaHistogramManager_4_2DlETH2IgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_4_2DlETH2IgR);
      instance.SetDelete(&delete_NicaHistogramManager_4_2DlETH2IgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_4_2DlETH2IgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_4_2DlETH2IgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_4_2D<TH2I>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_4_2D<TH2I>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2I>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_4_2DlETH2IgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2I>*)nullptr)->GetClass();
      NicaHistogramManager_4_2DlETH2IgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_4_2DlETH2IgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_4_2DlETH2FgR_Dictionary();
   static void NicaHistogramManager_4_2DlETH2FgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_4_2DlETH2FgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_4_2DlETH2FgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_4_2DlETH2FgR(void *p);
   static void deleteArray_NicaHistogramManager_4_2DlETH2FgR(void *p);
   static void destruct_NicaHistogramManager_4_2DlETH2FgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_4_2D<TH2F>*)
   {
      ::NicaHistogramManager_4_2D<TH2F> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_4_2D<TH2F> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_4_2D<TH2F>", ::NicaHistogramManager_4_2D<TH2F>::Class_Version(), "NicaHistogramManager.h", 1138,
                  typeid(::NicaHistogramManager_4_2D<TH2F>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_4_2DlETH2FgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_4_2D<TH2F>) );
      instance.SetNew(&new_NicaHistogramManager_4_2DlETH2FgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_4_2DlETH2FgR);
      instance.SetDelete(&delete_NicaHistogramManager_4_2DlETH2FgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_4_2DlETH2FgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_4_2DlETH2FgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_4_2D<TH2F>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_4_2D<TH2F>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2F>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_4_2DlETH2FgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2F>*)nullptr)->GetClass();
      NicaHistogramManager_4_2DlETH2FgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_4_2DlETH2FgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_4_2DlETH2DgR_Dictionary();
   static void NicaHistogramManager_4_2DlETH2DgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_4_2DlETH2DgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_4_2DlETH2DgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_4_2DlETH2DgR(void *p);
   static void deleteArray_NicaHistogramManager_4_2DlETH2DgR(void *p);
   static void destruct_NicaHistogramManager_4_2DlETH2DgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_4_2D<TH2D>*)
   {
      ::NicaHistogramManager_4_2D<TH2D> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_4_2D<TH2D> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_4_2D<TH2D>", ::NicaHistogramManager_4_2D<TH2D>::Class_Version(), "NicaHistogramManager.h", 1138,
                  typeid(::NicaHistogramManager_4_2D<TH2D>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_4_2DlETH2DgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_4_2D<TH2D>) );
      instance.SetNew(&new_NicaHistogramManager_4_2DlETH2DgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_4_2DlETH2DgR);
      instance.SetDelete(&delete_NicaHistogramManager_4_2DlETH2DgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_4_2DlETH2DgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_4_2DlETH2DgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_4_2D<TH2D>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_4_2D<TH2D>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2D>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_4_2DlETH2DgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2D>*)nullptr)->GetClass();
      NicaHistogramManager_4_2DlETH2DgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_4_2DlETH2DgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_4_3DlETH3IgR_Dictionary();
   static void NicaHistogramManager_4_3DlETH3IgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_4_3DlETH3IgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_4_3DlETH3IgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_4_3DlETH3IgR(void *p);
   static void deleteArray_NicaHistogramManager_4_3DlETH3IgR(void *p);
   static void destruct_NicaHistogramManager_4_3DlETH3IgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_4_3D<TH3I>*)
   {
      ::NicaHistogramManager_4_3D<TH3I> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_4_3D<TH3I> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_4_3D<TH3I>", ::NicaHistogramManager_4_3D<TH3I>::Class_Version(), "NicaHistogramManager.h", 1247,
                  typeid(::NicaHistogramManager_4_3D<TH3I>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_4_3DlETH3IgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_4_3D<TH3I>) );
      instance.SetNew(&new_NicaHistogramManager_4_3DlETH3IgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_4_3DlETH3IgR);
      instance.SetDelete(&delete_NicaHistogramManager_4_3DlETH3IgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_4_3DlETH3IgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_4_3DlETH3IgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_4_3D<TH3I>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_4_3D<TH3I>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3I>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_4_3DlETH3IgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3I>*)nullptr)->GetClass();
      NicaHistogramManager_4_3DlETH3IgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_4_3DlETH3IgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_4_3DlETH3FgR_Dictionary();
   static void NicaHistogramManager_4_3DlETH3FgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_4_3DlETH3FgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_4_3DlETH3FgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_4_3DlETH3FgR(void *p);
   static void deleteArray_NicaHistogramManager_4_3DlETH3FgR(void *p);
   static void destruct_NicaHistogramManager_4_3DlETH3FgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_4_3D<TH3F>*)
   {
      ::NicaHistogramManager_4_3D<TH3F> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_4_3D<TH3F> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_4_3D<TH3F>", ::NicaHistogramManager_4_3D<TH3F>::Class_Version(), "NicaHistogramManager.h", 1247,
                  typeid(::NicaHistogramManager_4_3D<TH3F>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_4_3DlETH3FgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_4_3D<TH3F>) );
      instance.SetNew(&new_NicaHistogramManager_4_3DlETH3FgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_4_3DlETH3FgR);
      instance.SetDelete(&delete_NicaHistogramManager_4_3DlETH3FgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_4_3DlETH3FgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_4_3DlETH3FgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_4_3D<TH3F>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_4_3D<TH3F>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3F>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_4_3DlETH3FgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3F>*)nullptr)->GetClass();
      NicaHistogramManager_4_3DlETH3FgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_4_3DlETH3FgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NicaHistogramManager_4_3DlETH3DgR_Dictionary();
   static void NicaHistogramManager_4_3DlETH3DgR_TClassManip(TClass*);
   static void *new_NicaHistogramManager_4_3DlETH3DgR(void *p = nullptr);
   static void *newArray_NicaHistogramManager_4_3DlETH3DgR(Long_t size, void *p);
   static void delete_NicaHistogramManager_4_3DlETH3DgR(void *p);
   static void deleteArray_NicaHistogramManager_4_3DlETH3DgR(void *p);
   static void destruct_NicaHistogramManager_4_3DlETH3DgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaHistogramManager_4_3D<TH3D>*)
   {
      ::NicaHistogramManager_4_3D<TH3D> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaHistogramManager_4_3D<TH3D> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaHistogramManager_4_3D<TH3D>", ::NicaHistogramManager_4_3D<TH3D>::Class_Version(), "NicaHistogramManager.h", 1247,
                  typeid(::NicaHistogramManager_4_3D<TH3D>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &NicaHistogramManager_4_3DlETH3DgR_Dictionary, isa_proxy, 4,
                  sizeof(::NicaHistogramManager_4_3D<TH3D>) );
      instance.SetNew(&new_NicaHistogramManager_4_3DlETH3DgR);
      instance.SetNewArray(&newArray_NicaHistogramManager_4_3DlETH3DgR);
      instance.SetDelete(&delete_NicaHistogramManager_4_3DlETH3DgR);
      instance.SetDeleteArray(&deleteArray_NicaHistogramManager_4_3DlETH3DgR);
      instance.SetDestructor(&destruct_NicaHistogramManager_4_3DlETH3DgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaHistogramManager_4_3D<TH3D>*)
   {
      return GenerateInitInstanceLocal((::NicaHistogramManager_4_3D<TH3D>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3D>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NicaHistogramManager_4_3DlETH3DgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3D>*)nullptr)->GetClass();
      NicaHistogramManager_4_3DlETH3DgR_TClassManip(theClass);
   return theClass;
   }

   static void NicaHistogramManager_4_3DlETH3DgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaMultiGraph(void *p = nullptr);
   static void *newArray_NicaMultiGraph(Long_t size, void *p);
   static void delete_NicaMultiGraph(void *p);
   static void deleteArray_NicaMultiGraph(void *p);
   static void destruct_NicaMultiGraph(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaMultiGraph*)
   {
      ::NicaMultiGraph *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaMultiGraph >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaMultiGraph", ::NicaMultiGraph::Class_Version(), "NicaMultiGraph.h", 21,
                  typeid(::NicaMultiGraph), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaMultiGraph::Dictionary, isa_proxy, 4,
                  sizeof(::NicaMultiGraph) );
      instance.SetNew(&new_NicaMultiGraph);
      instance.SetNewArray(&newArray_NicaMultiGraph);
      instance.SetDelete(&delete_NicaMultiGraph);
      instance.SetDeleteArray(&deleteArray_NicaMultiGraph);
      instance.SetDestructor(&destruct_NicaMultiGraph);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaMultiGraph*)
   {
      return GenerateInitInstanceLocal((::NicaMultiGraph*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaMultiGraph*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaSpline1D(void *p = nullptr);
   static void *newArray_NicaSpline1D(Long_t size, void *p);
   static void delete_NicaSpline1D(void *p);
   static void deleteArray_NicaSpline1D(void *p);
   static void destruct_NicaSpline1D(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaSpline1D*)
   {
      ::NicaSpline1D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaSpline1D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaSpline1D", ::NicaSpline1D::Class_Version(), "NicaSplines.h", 30,
                  typeid(::NicaSpline1D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaSpline1D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaSpline1D) );
      instance.SetNew(&new_NicaSpline1D);
      instance.SetNewArray(&newArray_NicaSpline1D);
      instance.SetDelete(&delete_NicaSpline1D);
      instance.SetDeleteArray(&deleteArray_NicaSpline1D);
      instance.SetDestructor(&destruct_NicaSpline1D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaSpline1D*)
   {
      return GenerateInitInstanceLocal((::NicaSpline1D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaSpline1D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaSpline2D(void *p = nullptr);
   static void *newArray_NicaSpline2D(Long_t size, void *p);
   static void delete_NicaSpline2D(void *p);
   static void deleteArray_NicaSpline2D(void *p);
   static void destruct_NicaSpline2D(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaSpline2D*)
   {
      ::NicaSpline2D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaSpline2D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaSpline2D", ::NicaSpline2D::Class_Version(), "NicaSplines.h", 105,
                  typeid(::NicaSpline2D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaSpline2D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaSpline2D) );
      instance.SetNew(&new_NicaSpline2D);
      instance.SetNewArray(&newArray_NicaSpline2D);
      instance.SetDelete(&delete_NicaSpline2D);
      instance.SetDeleteArray(&deleteArray_NicaSpline2D);
      instance.SetDestructor(&destruct_NicaSpline2D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaSpline2D*)
   {
      return GenerateInitInstanceLocal((::NicaSpline2D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaSpline2D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaSpline3D(void *p = nullptr);
   static void *newArray_NicaSpline3D(Long_t size, void *p);
   static void delete_NicaSpline3D(void *p);
   static void deleteArray_NicaSpline3D(void *p);
   static void destruct_NicaSpline3D(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaSpline3D*)
   {
      ::NicaSpline3D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaSpline3D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaSpline3D", ::NicaSpline3D::Class_Version(), "NicaSplines.h", 217,
                  typeid(::NicaSpline3D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaSpline3D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaSpline3D) );
      instance.SetNew(&new_NicaSpline3D);
      instance.SetNewArray(&newArray_NicaSpline3D);
      instance.SetDelete(&delete_NicaSpline3D);
      instance.SetDeleteArray(&deleteArray_NicaSpline3D);
      instance.SetDestructor(&destruct_NicaSpline3D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaSpline3D*)
   {
      return GenerateInitInstanceLocal((::NicaSpline3D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaSpline3D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaChiSqMap2D(void *p);
   static void deleteArray_NicaChiSqMap2D(void *p);
   static void destruct_NicaChiSqMap2D(void *p);
   static Long64_t merge_NicaChiSqMap2D(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaChiSqMap2D*)
   {
      ::NicaChiSqMap2D *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaChiSqMap2D >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaChiSqMap2D", ::NicaChiSqMap2D::Class_Version(), "NicaChiSqMap2D.h", 19,
                  typeid(::NicaChiSqMap2D), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaChiSqMap2D::Dictionary, isa_proxy, 4,
                  sizeof(::NicaChiSqMap2D) );
      instance.SetDelete(&delete_NicaChiSqMap2D);
      instance.SetDeleteArray(&deleteArray_NicaChiSqMap2D);
      instance.SetDestructor(&destruct_NicaChiSqMap2D);
      instance.SetMerge(&merge_NicaChiSqMap2D);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaChiSqMap2D*)
   {
      return GenerateInitInstanceLocal((::NicaChiSqMap2D*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaChiSqMap2D*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaFitParam(void *p = nullptr);
   static void *newArray_NicaFitParam(Long_t size, void *p);
   static void delete_NicaFitParam(void *p);
   static void deleteArray_NicaFitParam(void *p);
   static void destruct_NicaFitParam(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaFitParam*)
   {
      ::NicaFitParam *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaFitParam >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaFitParam", ::NicaFitParam::Class_Version(), "NicaFitParam.h", 16,
                  typeid(::NicaFitParam), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaFitParam::Dictionary, isa_proxy, 4,
                  sizeof(::NicaFitParam) );
      instance.SetNew(&new_NicaFitParam);
      instance.SetNewArray(&newArray_NicaFitParam);
      instance.SetDelete(&delete_NicaFitParam);
      instance.SetDeleteArray(&deleteArray_NicaFitParam);
      instance.SetDestructor(&destruct_NicaFitParam);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaFitParam*)
   {
      return GenerateInitInstanceLocal((::NicaFitParam*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaFitParam*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaMinimizer(void *p = nullptr);
   static void *newArray_NicaMinimizer(Long_t size, void *p);
   static void delete_NicaMinimizer(void *p);
   static void deleteArray_NicaMinimizer(void *p);
   static void destruct_NicaMinimizer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaMinimizer*)
   {
      ::NicaMinimizer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaMinimizer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaMinimizer", ::NicaMinimizer::Class_Version(), "NicaMinimizer.h", 53,
                  typeid(::NicaMinimizer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaMinimizer::Dictionary, isa_proxy, 4,
                  sizeof(::NicaMinimizer) );
      instance.SetNew(&new_NicaMinimizer);
      instance.SetNewArray(&newArray_NicaMinimizer);
      instance.SetDelete(&delete_NicaMinimizer);
      instance.SetDeleteArray(&deleteArray_NicaMinimizer);
      instance.SetDestructor(&destruct_NicaMinimizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaMinimizer*)
   {
      return GenerateInitInstanceLocal((::NicaMinimizer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaMinimizer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NicaCout::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaCout::Class_Name()
{
   return "NicaCout";
}

//______________________________________________________________________________
const char *NicaCout::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCout*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaCout::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaCout*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaCout::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCout*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaCout::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaCout*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_1<bool>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_1<bool>::Class_Name()
{
   return "NicaArray_1<bool>";
}

//______________________________________________________________________________
template <> const char *NicaArray_1<bool>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<bool>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_1<bool>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<bool>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_1<bool>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<bool>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_1<bool>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<bool>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_1<double>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_1<double>::Class_Name()
{
   return "NicaArray_1<double>";
}

//______________________________________________________________________________
template <> const char *NicaArray_1<double>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<double>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_1<double>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<double>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_1<double>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<double>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_1<double>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<double>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_1<float>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_1<float>::Class_Name()
{
   return "NicaArray_1<float>";
}

//______________________________________________________________________________
template <> const char *NicaArray_1<float>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<float>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_1<float>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<float>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_1<float>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<float>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_1<float>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<float>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_1<short>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_1<short>::Class_Name()
{
   return "NicaArray_1<short>";
}

//______________________________________________________________________________
template <> const char *NicaArray_1<short>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<short>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_1<short>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<short>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_1<short>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<short>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_1<short>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<short>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_1<int>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_1<int>::Class_Name()
{
   return "NicaArray_1<int>";
}

//______________________________________________________________________________
template <> const char *NicaArray_1<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<int>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_1<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<int>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_1<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<int>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_1<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_1<int>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_2<double>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_2<double>::Class_Name()
{
   return "NicaArray_2<double>";
}

//______________________________________________________________________________
template <> const char *NicaArray_2<double>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<double>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_2<double>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<double>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_2<double>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<double>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_2<double>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<double>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_2<float>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_2<float>::Class_Name()
{
   return "NicaArray_2<float>";
}

//______________________________________________________________________________
template <> const char *NicaArray_2<float>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<float>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_2<float>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<float>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_2<float>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<float>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_2<float>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<float>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_2<short>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_2<short>::Class_Name()
{
   return "NicaArray_2<short>";
}

//______________________________________________________________________________
template <> const char *NicaArray_2<short>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<short>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_2<short>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<short>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_2<short>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<short>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_2<short>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<short>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_2<int>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_2<int>::Class_Name()
{
   return "NicaArray_2<int>";
}

//______________________________________________________________________________
template <> const char *NicaArray_2<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<int>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_2<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<int>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_2<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<int>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_2<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<int>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_2<bool>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_2<bool>::Class_Name()
{
   return "NicaArray_2<bool>";
}

//______________________________________________________________________________
template <> const char *NicaArray_2<bool>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<bool>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_2<bool>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<bool>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_2<bool>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<bool>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_2<bool>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_2<bool>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_3<double>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_3<double>::Class_Name()
{
   return "NicaArray_3<double>";
}

//______________________________________________________________________________
template <> const char *NicaArray_3<double>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<double>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_3<double>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<double>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_3<double>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<double>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_3<double>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<double>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_3<float>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_3<float>::Class_Name()
{
   return "NicaArray_3<float>";
}

//______________________________________________________________________________
template <> const char *NicaArray_3<float>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<float>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_3<float>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<float>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_3<float>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<float>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_3<float>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<float>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_3<short>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_3<short>::Class_Name()
{
   return "NicaArray_3<short>";
}

//______________________________________________________________________________
template <> const char *NicaArray_3<short>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<short>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_3<short>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<short>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_3<short>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<short>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_3<short>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<short>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_3<int>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_3<int>::Class_Name()
{
   return "NicaArray_3<int>";
}

//______________________________________________________________________________
template <> const char *NicaArray_3<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<int>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_3<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<int>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_3<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<int>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_3<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<int>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_3<bool>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_3<bool>::Class_Name()
{
   return "NicaArray_3<bool>";
}

//______________________________________________________________________________
template <> const char *NicaArray_3<bool>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<bool>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_3<bool>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<bool>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_3<bool>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<bool>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_3<bool>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_3<bool>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_4<double>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_4<double>::Class_Name()
{
   return "NicaArray_4<double>";
}

//______________________________________________________________________________
template <> const char *NicaArray_4<double>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<double>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_4<double>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<double>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_4<double>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<double>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_4<double>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<double>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_4<float>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_4<float>::Class_Name()
{
   return "NicaArray_4<float>";
}

//______________________________________________________________________________
template <> const char *NicaArray_4<float>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<float>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_4<float>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<float>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_4<float>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<float>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_4<float>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<float>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_4<short>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_4<short>::Class_Name()
{
   return "NicaArray_4<short>";
}

//______________________________________________________________________________
template <> const char *NicaArray_4<short>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<short>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_4<short>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<short>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_4<short>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<short>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_4<short>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<short>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_4<int>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_4<int>::Class_Name()
{
   return "NicaArray_4<int>";
}

//______________________________________________________________________________
template <> const char *NicaArray_4<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<int>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_4<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<int>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_4<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<int>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_4<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<int>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaArray_4<bool>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaArray_4<bool>::Class_Name()
{
   return "NicaArray_4<bool>";
}

//______________________________________________________________________________
template <> const char *NicaArray_4<bool>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<bool>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaArray_4<bool>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<bool>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaArray_4<bool>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<bool>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaArray_4<bool>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaArray_4<bool>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaOptionArray::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaOptionArray::Class_Name()
{
   return "NicaOptionArray";
}

//______________________________________________________________________________
const char *NicaOptionArray::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaOptionArray*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaOptionArray::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaOptionArray*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaOptionArray::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaOptionArray*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaOptionArray::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaOptionArray*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaOptionConverter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaOptionConverter::Class_Name()
{
   return "NicaOptionConverter";
}

//______________________________________________________________________________
const char *NicaOptionConverter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaOptionConverter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaOptionConverter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaOptionConverter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaOptionConverter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaOptionConverter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaOptionConverter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaOptionConverter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaPack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaPack::Class_Name()
{
   return "NicaPack";
}

//______________________________________________________________________________
const char *NicaPack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaPack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaPack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaPack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaParameter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaParameter::Class_Name()
{
   return "NicaParameter";
}

//______________________________________________________________________________
const char *NicaParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaParameterInt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaParameterInt::Class_Name()
{
   return "NicaParameterInt";
}

//______________________________________________________________________________
const char *NicaParameterInt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterInt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaParameterInt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterInt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaParameterInt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterInt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaParameterInt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterInt*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaParameterDouble::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaParameterDouble::Class_Name()
{
   return "NicaParameterDouble";
}

//______________________________________________________________________________
const char *NicaParameterDouble::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterDouble*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaParameterDouble::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterDouble*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaParameterDouble::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterDouble*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaParameterDouble::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterDouble*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaParameterFloat::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaParameterFloat::Class_Name()
{
   return "NicaParameterFloat";
}

//______________________________________________________________________________
const char *NicaParameterFloat::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterFloat*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaParameterFloat::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterFloat*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaParameterFloat::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterFloat*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaParameterFloat::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterFloat*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaParameterString::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaParameterString::Class_Name()
{
   return "NicaParameterString";
}

//______________________________________________________________________________
const char *NicaParameterString::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterString*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaParameterString::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterString*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaParameterString::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterString*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaParameterString::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterString*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaParameterUInt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaParameterUInt::Class_Name()
{
   return "NicaParameterUInt";
}

//______________________________________________________________________________
const char *NicaParameterUInt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterUInt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaParameterUInt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterUInt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaParameterUInt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterUInt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaParameterUInt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterUInt*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaParameterULong::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaParameterULong::Class_Name()
{
   return "NicaParameterULong";
}

//______________________________________________________________________________
const char *NicaParameterULong::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterULong*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaParameterULong::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterULong*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaParameterULong::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterULong*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaParameterULong::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterULong*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaParameterULong64::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaParameterULong64::Class_Name()
{
   return "NicaParameterULong64";
}

//______________________________________________________________________________
const char *NicaParameterULong64::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterULong64*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaParameterULong64::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterULong64*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaParameterULong64::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterULong64*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaParameterULong64::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterULong64*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaParameterBool::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaParameterBool::Class_Name()
{
   return "NicaParameterBool";
}

//______________________________________________________________________________
const char *NicaParameterBool::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterBool*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaParameterBool::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterBool*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaParameterBool::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterBool*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaParameterBool::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaParameterBool*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaObjectMatrix_1::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaObjectMatrix_1::Class_Name()
{
   return "NicaObjectMatrix_1";
}

//______________________________________________________________________________
const char *NicaObjectMatrix_1::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_1*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaObjectMatrix_1::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_1*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaObjectMatrix_1::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_1*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaObjectMatrix_1::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_1*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaObjectMatrix_2::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaObjectMatrix_2::Class_Name()
{
   return "NicaObjectMatrix_2";
}

//______________________________________________________________________________
const char *NicaObjectMatrix_2::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_2*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaObjectMatrix_2::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_2*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaObjectMatrix_2::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_2*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaObjectMatrix_2::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_2*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaObjectMatrix_3::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaObjectMatrix_3::Class_Name()
{
   return "NicaObjectMatrix_3";
}

//______________________________________________________________________________
const char *NicaObjectMatrix_3::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_3*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaObjectMatrix_3::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_3*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaObjectMatrix_3::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_3*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaObjectMatrix_3::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_3*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaObjectMatrix_4::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaObjectMatrix_4::Class_Name()
{
   return "NicaObjectMatrix_4";
}

//______________________________________________________________________________
const char *NicaObjectMatrix_4::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_4*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaObjectMatrix_4::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_4*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaObjectMatrix_4::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_4*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaObjectMatrix_4::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaObjectMatrix_4*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaHelixBase::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaHelixBase::Class_Name()
{
   return "NicaHelixBase";
}

//______________________________________________________________________________
const char *NicaHelixBase::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixBase*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaHelixBase::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixBase*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaHelixBase::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixBase*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaHelixBase::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixBase*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaHelixZ::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaHelixZ::Class_Name()
{
   return "NicaHelixZ";
}

//______________________________________________________________________________
const char *NicaHelixZ::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixZ*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaHelixZ::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixZ*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaHelixZ::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixZ*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaHelixZ::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixZ*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaHelixX::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaHelixX::Class_Name()
{
   return "NicaHelixX";
}

//______________________________________________________________________________
const char *NicaHelixX::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixX*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaHelixX::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixX*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaHelixX::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixX*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaHelixX::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixX*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaHelixY::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaHelixY::Class_Name()
{
   return "NicaHelixY";
}

//______________________________________________________________________________
const char *NicaHelixY::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixY*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaHelixY::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixY*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaHelixY::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixY*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaHelixY::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHelixY*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaLink::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaLink::Class_Name()
{
   return "NicaLink";
}

//______________________________________________________________________________
const char *NicaLink::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaLink*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaLink::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaLink*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaLink::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaLink*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaLink::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaLink*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaConst::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaConst::Class_Name()
{
   return "NicaConst";
}

//______________________________________________________________________________
const char *NicaConst::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaConst*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaConst::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaConst*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaConst::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaConst*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaConst::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaConst*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaXMLAttrib::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaXMLAttrib::Class_Name()
{
   return "NicaXMLAttrib";
}

//______________________________________________________________________________
const char *NicaXMLAttrib::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaXMLAttrib*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaXMLAttrib::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaXMLAttrib*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaXMLAttrib::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaXMLAttrib*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaXMLAttrib::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaXMLAttrib*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaXMLNode::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaXMLNode::Class_Name()
{
   return "NicaXMLNode";
}

//______________________________________________________________________________
const char *NicaXMLNode::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaXMLNode*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaXMLNode::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaXMLNode*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaXMLNode::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaXMLNode*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaXMLNode::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaXMLNode*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaXMLFile::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaXMLFile::Class_Name()
{
   return "NicaXMLFile";
}

//______________________________________________________________________________
const char *NicaXMLFile::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaXMLFile*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaXMLFile::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaXMLFile*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaXMLFile::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaXMLFile*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaXMLFile::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaXMLFile*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaJobs::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaJobs::Class_Name()
{
   return "NicaJobs";
}

//______________________________________________________________________________
const char *NicaJobs::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaJobs*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaJobs::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaJobs*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaJobs::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaJobs*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaJobs::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaJobs*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaPackage::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaPackage::Class_Name()
{
   return "NicaPackage";
}

//______________________________________________________________________________
const char *NicaPackage::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPackage*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaPackage::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPackage*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaPackage::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPackage*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaPackage::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPackage*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaPackage2HTML::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaPackage2HTML::Class_Name()
{
   return "NicaPackage2HTML";
}

//______________________________________________________________________________
const char *NicaPackage2HTML::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPackage2HTML*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaPackage2HTML::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPackage2HTML*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaPackage2HTML::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPackage2HTML*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaPackage2HTML::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPackage2HTML*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaAnaFile::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaAnaFile::Class_Name()
{
   return "NicaAnaFile";
}

//______________________________________________________________________________
const char *NicaAnaFile::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaAnaFile*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaAnaFile::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaAnaFile*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaAnaFile::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaAnaFile*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaAnaFile::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaAnaFile*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaHtmlCore::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaHtmlCore::Class_Name()
{
   return "NicaHtmlCore";
}

//______________________________________________________________________________
const char *NicaHtmlCore::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCore*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaHtmlCore::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCore*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaHtmlCore::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCore*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaHtmlCore::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCore*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaPackageSql::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaPackageSql::Class_Name()
{
   return "NicaPackageSql";
}

//______________________________________________________________________________
const char *NicaPackageSql::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPackageSql*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaPackageSql::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaPackageSql*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaPackageSql::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPackageSql*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaPackageSql::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaPackageSql*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaHtmlObject::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaHtmlObject::Class_Name()
{
   return "NicaHtmlObject";
}

//______________________________________________________________________________
const char *NicaHtmlObject::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlObject*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaHtmlObject::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlObject*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaHtmlObject::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlObject*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaHtmlObject::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlObject*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaHtmlDiv::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaHtmlDiv::Class_Name()
{
   return "NicaHtmlDiv";
}

//______________________________________________________________________________
const char *NicaHtmlDiv::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlDiv*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaHtmlDiv::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlDiv*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaHtmlDiv::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlDiv*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaHtmlDiv::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlDiv*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaHtmlFile::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaHtmlFile::Class_Name()
{
   return "NicaHtmlFile";
}

//______________________________________________________________________________
const char *NicaHtmlFile::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlFile*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaHtmlFile::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlFile*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaHtmlFile::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlFile*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaHtmlFile::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlFile*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaHtmlTable::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaHtmlTable::Class_Name()
{
   return "NicaHtmlTable";
}

//______________________________________________________________________________
const char *NicaHtmlTable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlTable*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaHtmlTable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlTable*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaHtmlTable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlTable*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaHtmlTable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlTable*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaHtmlRow::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaHtmlRow::Class_Name()
{
   return "NicaHtmlRow";
}

//______________________________________________________________________________
const char *NicaHtmlRow::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlRow*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaHtmlRow::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlRow*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaHtmlRow::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlRow*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaHtmlRow::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlRow*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaHtmlCell::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaHtmlCell::Class_Name()
{
   return "NicaHtmlCell";
}

//______________________________________________________________________________
const char *NicaHtmlCell::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCell*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaHtmlCell::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCell*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaHtmlCell::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCell*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaHtmlCell::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCell*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaHtmlCellCol::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaHtmlCellCol::Class_Name()
{
   return "NicaHtmlCellCol";
}

//______________________________________________________________________________
const char *NicaHtmlCellCol::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCellCol*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaHtmlCellCol::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCellCol*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaHtmlCellCol::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCellCol*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaHtmlCellCol::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCellCol*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaHtmlCellRow::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaHtmlCellRow::Class_Name()
{
   return "NicaHtmlCellRow";
}

//______________________________________________________________________________
const char *NicaHtmlCellRow::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCellRow*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaHtmlCellRow::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCellRow*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaHtmlCellRow::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCellRow*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaHtmlCellRow::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHtmlCellRow*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaDividedHisto1D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaDividedHisto1D::Class_Name()
{
   return "NicaDividedHisto1D";
}

//______________________________________________________________________________
const char *NicaDividedHisto1D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaDividedHisto1D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaDividedHisto1D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaDividedHisto1D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaDividedHisto1D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaDividedHisto1D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaDividedHisto1D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaDividedHisto1D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaDividedHisto2D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaDividedHisto2D::Class_Name()
{
   return "NicaDividedHisto2D";
}

//______________________________________________________________________________
const char *NicaDividedHisto2D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaDividedHisto2D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaDividedHisto2D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaDividedHisto2D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaDividedHisto2D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaDividedHisto2D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaDividedHisto2D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaDividedHisto2D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaDividedHisto3D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaDividedHisto3D::Class_Name()
{
   return "NicaDividedHisto3D";
}

//______________________________________________________________________________
const char *NicaDividedHisto3D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaDividedHisto3D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaDividedHisto3D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaDividedHisto3D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaDividedHisto3D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaDividedHisto3D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaDividedHisto3D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaDividedHisto3D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaHistogramAxisConf::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaHistogramAxisConf::Class_Name()
{
   return "NicaHistogramAxisConf";
}

//______________________________________________________________________________
const char *NicaHistogramAxisConf::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramAxisConf*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaHistogramAxisConf::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramAxisConf*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaHistogramAxisConf::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramAxisConf*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaHistogramAxisConf::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramAxisConf*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_1_1D<TH1I>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_1D<TH1I>::Class_Name()
{
   return "NicaHistogramManager_1_1D<TH1I>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_1D<TH1I>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1I>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_1_1D<TH1I>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1I>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_1D<TH1I>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1I>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_1D<TH1I>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1I>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_1_1D<TH1F>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_1D<TH1F>::Class_Name()
{
   return "NicaHistogramManager_1_1D<TH1F>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_1D<TH1F>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1F>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_1_1D<TH1F>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1F>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_1D<TH1F>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1F>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_1D<TH1F>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1F>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_1_1D<TH1D>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_1D<TH1D>::Class_Name()
{
   return "NicaHistogramManager_1_1D<TH1D>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_1D<TH1D>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1D>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_1_1D<TH1D>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1D>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_1D<TH1D>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1D>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_1D<TH1D>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_1D<TH1D>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_1_2D<TH2I>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_2D<TH2I>::Class_Name()
{
   return "NicaHistogramManager_1_2D<TH2I>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_2D<TH2I>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2I>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_1_2D<TH2I>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2I>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_2D<TH2I>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2I>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_2D<TH2I>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2I>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_1_2D<TH2F>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_2D<TH2F>::Class_Name()
{
   return "NicaHistogramManager_1_2D<TH2F>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_2D<TH2F>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2F>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_1_2D<TH2F>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2F>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_2D<TH2F>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2F>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_2D<TH2F>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2F>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_1_2D<TH2D>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_2D<TH2D>::Class_Name()
{
   return "NicaHistogramManager_1_2D<TH2D>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_2D<TH2D>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2D>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_1_2D<TH2D>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2D>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_2D<TH2D>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2D>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_2D<TH2D>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_2D<TH2D>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_1_3D<TH3I>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_3D<TH3I>::Class_Name()
{
   return "NicaHistogramManager_1_3D<TH3I>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_3D<TH3I>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3I>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_1_3D<TH3I>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3I>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_3D<TH3I>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3I>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_3D<TH3I>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3I>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_1_3D<TH3F>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_3D<TH3F>::Class_Name()
{
   return "NicaHistogramManager_1_3D<TH3F>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_3D<TH3F>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3F>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_1_3D<TH3F>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3F>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_3D<TH3F>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3F>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_3D<TH3F>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3F>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_1_3D<TH3D>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_3D<TH3D>::Class_Name()
{
   return "NicaHistogramManager_1_3D<TH3D>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_1_3D<TH3D>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3D>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_1_3D<TH3D>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3D>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_3D<TH3D>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3D>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_1_3D<TH3D>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_1_3D<TH3D>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_2_1D<TH1I>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_1D<TH1I>::Class_Name()
{
   return "NicaHistogramManager_2_1D<TH1I>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_1D<TH1I>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1I>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_2_1D<TH1I>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1I>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_1D<TH1I>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1I>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_1D<TH1I>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1I>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_2_1D<TH1F>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_1D<TH1F>::Class_Name()
{
   return "NicaHistogramManager_2_1D<TH1F>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_1D<TH1F>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1F>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_2_1D<TH1F>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1F>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_1D<TH1F>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1F>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_1D<TH1F>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1F>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_2_1D<TH1D>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_1D<TH1D>::Class_Name()
{
   return "NicaHistogramManager_2_1D<TH1D>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_1D<TH1D>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1D>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_2_1D<TH1D>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1D>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_1D<TH1D>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1D>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_1D<TH1D>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_1D<TH1D>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_2_2D<TH2I>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_2D<TH2I>::Class_Name()
{
   return "NicaHistogramManager_2_2D<TH2I>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_2D<TH2I>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2I>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_2_2D<TH2I>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2I>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_2D<TH2I>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2I>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_2D<TH2I>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2I>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_2_2D<TH2F>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_2D<TH2F>::Class_Name()
{
   return "NicaHistogramManager_2_2D<TH2F>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_2D<TH2F>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2F>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_2_2D<TH2F>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2F>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_2D<TH2F>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2F>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_2D<TH2F>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2F>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_2_2D<TH2D>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_2D<TH2D>::Class_Name()
{
   return "NicaHistogramManager_2_2D<TH2D>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_2D<TH2D>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2D>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_2_2D<TH2D>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2D>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_2D<TH2D>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2D>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_2D<TH2D>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_2D<TH2D>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_2_3D<TH3I>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_3D<TH3I>::Class_Name()
{
   return "NicaHistogramManager_2_3D<TH3I>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_3D<TH3I>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3I>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_2_3D<TH3I>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3I>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_3D<TH3I>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3I>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_3D<TH3I>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3I>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_2_3D<TH3F>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_3D<TH3F>::Class_Name()
{
   return "NicaHistogramManager_2_3D<TH3F>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_3D<TH3F>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3F>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_2_3D<TH3F>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3F>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_3D<TH3F>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3F>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_3D<TH3F>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3F>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_2_3D<TH3D>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_3D<TH3D>::Class_Name()
{
   return "NicaHistogramManager_2_3D<TH3D>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_2_3D<TH3D>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3D>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_2_3D<TH3D>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3D>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_3D<TH3D>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3D>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_2_3D<TH3D>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_2_3D<TH3D>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_3_1D<TH1I>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_1D<TH1I>::Class_Name()
{
   return "NicaHistogramManager_3_1D<TH1I>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_1D<TH1I>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1I>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_3_1D<TH1I>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1I>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_1D<TH1I>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1I>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_1D<TH1I>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1I>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_3_1D<TH1F>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_1D<TH1F>::Class_Name()
{
   return "NicaHistogramManager_3_1D<TH1F>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_1D<TH1F>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1F>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_3_1D<TH1F>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1F>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_1D<TH1F>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1F>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_1D<TH1F>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1F>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_3_1D<TH1D>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_1D<TH1D>::Class_Name()
{
   return "NicaHistogramManager_3_1D<TH1D>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_1D<TH1D>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1D>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_3_1D<TH1D>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1D>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_1D<TH1D>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1D>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_1D<TH1D>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_1D<TH1D>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_3_2D<TH2I>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_2D<TH2I>::Class_Name()
{
   return "NicaHistogramManager_3_2D<TH2I>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_2D<TH2I>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2I>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_3_2D<TH2I>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2I>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_2D<TH2I>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2I>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_2D<TH2I>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2I>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_3_2D<TH2F>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_2D<TH2F>::Class_Name()
{
   return "NicaHistogramManager_3_2D<TH2F>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_2D<TH2F>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2F>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_3_2D<TH2F>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2F>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_2D<TH2F>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2F>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_2D<TH2F>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2F>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_3_2D<TH2D>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_2D<TH2D>::Class_Name()
{
   return "NicaHistogramManager_3_2D<TH2D>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_2D<TH2D>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2D>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_3_2D<TH2D>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2D>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_2D<TH2D>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2D>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_2D<TH2D>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_2D<TH2D>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_3_3D<TH3I>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_3D<TH3I>::Class_Name()
{
   return "NicaHistogramManager_3_3D<TH3I>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_3D<TH3I>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3I>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_3_3D<TH3I>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3I>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_3D<TH3I>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3I>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_3D<TH3I>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3I>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_3_3D<TH3F>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_3D<TH3F>::Class_Name()
{
   return "NicaHistogramManager_3_3D<TH3F>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_3D<TH3F>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3F>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_3_3D<TH3F>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3F>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_3D<TH3F>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3F>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_3D<TH3F>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3F>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_3_3D<TH3D>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_3D<TH3D>::Class_Name()
{
   return "NicaHistogramManager_3_3D<TH3D>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_3_3D<TH3D>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3D>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_3_3D<TH3D>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3D>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_3D<TH3D>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3D>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_3_3D<TH3D>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_3_3D<TH3D>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_4_1D<TH1I>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_1D<TH1I>::Class_Name()
{
   return "NicaHistogramManager_4_1D<TH1I>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_1D<TH1I>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1I>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_4_1D<TH1I>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1I>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_1D<TH1I>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1I>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_1D<TH1I>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1I>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_4_1D<TH1F>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_1D<TH1F>::Class_Name()
{
   return "NicaHistogramManager_4_1D<TH1F>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_1D<TH1F>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1F>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_4_1D<TH1F>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1F>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_1D<TH1F>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1F>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_1D<TH1F>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1F>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_4_1D<TH1D>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_1D<TH1D>::Class_Name()
{
   return "NicaHistogramManager_4_1D<TH1D>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_1D<TH1D>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1D>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_4_1D<TH1D>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1D>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_1D<TH1D>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1D>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_1D<TH1D>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_1D<TH1D>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_4_2D<TH2I>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_2D<TH2I>::Class_Name()
{
   return "NicaHistogramManager_4_2D<TH2I>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_2D<TH2I>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2I>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_4_2D<TH2I>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2I>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_2D<TH2I>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2I>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_2D<TH2I>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2I>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_4_2D<TH2F>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_2D<TH2F>::Class_Name()
{
   return "NicaHistogramManager_4_2D<TH2F>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_2D<TH2F>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2F>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_4_2D<TH2F>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2F>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_2D<TH2F>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2F>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_2D<TH2F>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2F>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_4_2D<TH2D>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_2D<TH2D>::Class_Name()
{
   return "NicaHistogramManager_4_2D<TH2D>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_2D<TH2D>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2D>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_4_2D<TH2D>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2D>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_2D<TH2D>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2D>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_2D<TH2D>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_2D<TH2D>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_4_3D<TH3I>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_3D<TH3I>::Class_Name()
{
   return "NicaHistogramManager_4_3D<TH3I>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_3D<TH3I>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3I>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_4_3D<TH3I>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3I>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_3D<TH3I>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3I>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_3D<TH3I>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3I>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_4_3D<TH3F>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_3D<TH3F>::Class_Name()
{
   return "NicaHistogramManager_4_3D<TH3F>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_3D<TH3F>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3F>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_4_3D<TH3F>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3F>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_3D<TH3F>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3F>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_3D<TH3F>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3F>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr NicaHistogramManager_4_3D<TH3D>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_3D<TH3D>::Class_Name()
{
   return "NicaHistogramManager_4_3D<TH3D>";
}

//______________________________________________________________________________
template <> const char *NicaHistogramManager_4_3D<TH3D>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3D>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int NicaHistogramManager_4_3D<TH3D>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3D>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_3D<TH3D>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3D>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *NicaHistogramManager_4_3D<TH3D>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaHistogramManager_4_3D<TH3D>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaMultiGraph::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaMultiGraph::Class_Name()
{
   return "NicaMultiGraph";
}

//______________________________________________________________________________
const char *NicaMultiGraph::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMultiGraph*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaMultiGraph::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMultiGraph*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaMultiGraph::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMultiGraph*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaMultiGraph::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMultiGraph*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaSpline1D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaSpline1D::Class_Name()
{
   return "NicaSpline1D";
}

//______________________________________________________________________________
const char *NicaSpline1D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSpline1D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaSpline1D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSpline1D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaSpline1D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSpline1D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaSpline1D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSpline1D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaSpline2D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaSpline2D::Class_Name()
{
   return "NicaSpline2D";
}

//______________________________________________________________________________
const char *NicaSpline2D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSpline2D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaSpline2D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSpline2D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaSpline2D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSpline2D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaSpline2D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSpline2D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaSpline3D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaSpline3D::Class_Name()
{
   return "NicaSpline3D";
}

//______________________________________________________________________________
const char *NicaSpline3D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSpline3D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaSpline3D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaSpline3D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaSpline3D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSpline3D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaSpline3D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaSpline3D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaChiSqMap2D::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaChiSqMap2D::Class_Name()
{
   return "NicaChiSqMap2D";
}

//______________________________________________________________________________
const char *NicaChiSqMap2D::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaChiSqMap2D*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaChiSqMap2D::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaChiSqMap2D*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaChiSqMap2D::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaChiSqMap2D*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaChiSqMap2D::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaChiSqMap2D*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaFitParam::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaFitParam::Class_Name()
{
   return "NicaFitParam";
}

//______________________________________________________________________________
const char *NicaFitParam::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFitParam*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaFitParam::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaFitParam*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaFitParam::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFitParam*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaFitParam::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaFitParam*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaMinimizer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaMinimizer::Class_Name()
{
   return "NicaMinimizer";
}

//______________________________________________________________________________
const char *NicaMinimizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMinimizer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaMinimizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaMinimizer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaMinimizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMinimizer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaMinimizer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaMinimizer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void NicaCout::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaCout.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaCout::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaCout::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaCout(void *p) {
      return  p ? new(p) ::NicaCout : new ::NicaCout;
   }
   static void *newArray_NicaCout(Long_t nElements, void *p) {
      return p ? new(p) ::NicaCout[nElements] : new ::NicaCout[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaCout(void *p) {
      delete ((::NicaCout*)p);
   }
   static void deleteArray_NicaCout(void *p) {
      delete [] ((::NicaCout*)p);
   }
   static void destruct_NicaCout(void *p) {
      typedef ::NicaCout current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaCout

//______________________________________________________________________________
template <> void NicaArray_1<bool>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_1<bool>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_1<bool>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_1<bool>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_1lEboolgR(void *p) {
      return  p ? new(p) ::NicaArray_1<bool> : new ::NicaArray_1<bool>;
   }
   static void *newArray_NicaArray_1lEboolgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_1<bool>[nElements] : new ::NicaArray_1<bool>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_1lEboolgR(void *p) {
      delete ((::NicaArray_1<bool>*)p);
   }
   static void deleteArray_NicaArray_1lEboolgR(void *p) {
      delete [] ((::NicaArray_1<bool>*)p);
   }
   static void destruct_NicaArray_1lEboolgR(void *p) {
      typedef ::NicaArray_1<bool> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_1<bool>

//______________________________________________________________________________
template <> void NicaArray_1<double>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_1<double>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_1<double>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_1<double>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_1lEdoublegR(void *p) {
      return  p ? new(p) ::NicaArray_1<double> : new ::NicaArray_1<double>;
   }
   static void *newArray_NicaArray_1lEdoublegR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_1<double>[nElements] : new ::NicaArray_1<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_1lEdoublegR(void *p) {
      delete ((::NicaArray_1<double>*)p);
   }
   static void deleteArray_NicaArray_1lEdoublegR(void *p) {
      delete [] ((::NicaArray_1<double>*)p);
   }
   static void destruct_NicaArray_1lEdoublegR(void *p) {
      typedef ::NicaArray_1<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_1<double>

//______________________________________________________________________________
template <> void NicaArray_1<float>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_1<float>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_1<float>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_1<float>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_1lEfloatgR(void *p) {
      return  p ? new(p) ::NicaArray_1<float> : new ::NicaArray_1<float>;
   }
   static void *newArray_NicaArray_1lEfloatgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_1<float>[nElements] : new ::NicaArray_1<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_1lEfloatgR(void *p) {
      delete ((::NicaArray_1<float>*)p);
   }
   static void deleteArray_NicaArray_1lEfloatgR(void *p) {
      delete [] ((::NicaArray_1<float>*)p);
   }
   static void destruct_NicaArray_1lEfloatgR(void *p) {
      typedef ::NicaArray_1<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_1<float>

//______________________________________________________________________________
template <> void NicaArray_1<short>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_1<short>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_1<short>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_1<short>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_1lEshortgR(void *p) {
      return  p ? new(p) ::NicaArray_1<short> : new ::NicaArray_1<short>;
   }
   static void *newArray_NicaArray_1lEshortgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_1<short>[nElements] : new ::NicaArray_1<short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_1lEshortgR(void *p) {
      delete ((::NicaArray_1<short>*)p);
   }
   static void deleteArray_NicaArray_1lEshortgR(void *p) {
      delete [] ((::NicaArray_1<short>*)p);
   }
   static void destruct_NicaArray_1lEshortgR(void *p) {
      typedef ::NicaArray_1<short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_1<short>

//______________________________________________________________________________
template <> void NicaArray_1<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_1<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_1<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_1<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_1lEintgR(void *p) {
      return  p ? new(p) ::NicaArray_1<int> : new ::NicaArray_1<int>;
   }
   static void *newArray_NicaArray_1lEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_1<int>[nElements] : new ::NicaArray_1<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_1lEintgR(void *p) {
      delete ((::NicaArray_1<int>*)p);
   }
   static void deleteArray_NicaArray_1lEintgR(void *p) {
      delete [] ((::NicaArray_1<int>*)p);
   }
   static void destruct_NicaArray_1lEintgR(void *p) {
      typedef ::NicaArray_1<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_1<int>

//______________________________________________________________________________
template <> void NicaArray_2<double>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_2<double>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_2<double>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_2<double>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_2lEdoublegR(void *p) {
      return  p ? new(p) ::NicaArray_2<double> : new ::NicaArray_2<double>;
   }
   static void *newArray_NicaArray_2lEdoublegR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_2<double>[nElements] : new ::NicaArray_2<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_2lEdoublegR(void *p) {
      delete ((::NicaArray_2<double>*)p);
   }
   static void deleteArray_NicaArray_2lEdoublegR(void *p) {
      delete [] ((::NicaArray_2<double>*)p);
   }
   static void destruct_NicaArray_2lEdoublegR(void *p) {
      typedef ::NicaArray_2<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_2<double>

//______________________________________________________________________________
template <> void NicaArray_2<float>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_2<float>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_2<float>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_2<float>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_2lEfloatgR(void *p) {
      return  p ? new(p) ::NicaArray_2<float> : new ::NicaArray_2<float>;
   }
   static void *newArray_NicaArray_2lEfloatgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_2<float>[nElements] : new ::NicaArray_2<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_2lEfloatgR(void *p) {
      delete ((::NicaArray_2<float>*)p);
   }
   static void deleteArray_NicaArray_2lEfloatgR(void *p) {
      delete [] ((::NicaArray_2<float>*)p);
   }
   static void destruct_NicaArray_2lEfloatgR(void *p) {
      typedef ::NicaArray_2<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_2<float>

//______________________________________________________________________________
template <> void NicaArray_2<short>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_2<short>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_2<short>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_2<short>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_2lEshortgR(void *p) {
      return  p ? new(p) ::NicaArray_2<short> : new ::NicaArray_2<short>;
   }
   static void *newArray_NicaArray_2lEshortgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_2<short>[nElements] : new ::NicaArray_2<short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_2lEshortgR(void *p) {
      delete ((::NicaArray_2<short>*)p);
   }
   static void deleteArray_NicaArray_2lEshortgR(void *p) {
      delete [] ((::NicaArray_2<short>*)p);
   }
   static void destruct_NicaArray_2lEshortgR(void *p) {
      typedef ::NicaArray_2<short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_2<short>

//______________________________________________________________________________
template <> void NicaArray_2<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_2<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_2<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_2<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_2lEintgR(void *p) {
      return  p ? new(p) ::NicaArray_2<int> : new ::NicaArray_2<int>;
   }
   static void *newArray_NicaArray_2lEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_2<int>[nElements] : new ::NicaArray_2<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_2lEintgR(void *p) {
      delete ((::NicaArray_2<int>*)p);
   }
   static void deleteArray_NicaArray_2lEintgR(void *p) {
      delete [] ((::NicaArray_2<int>*)p);
   }
   static void destruct_NicaArray_2lEintgR(void *p) {
      typedef ::NicaArray_2<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_2<int>

//______________________________________________________________________________
template <> void NicaArray_2<bool>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_2<bool>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_2<bool>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_2<bool>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_2lEboolgR(void *p) {
      return  p ? new(p) ::NicaArray_2<bool> : new ::NicaArray_2<bool>;
   }
   static void *newArray_NicaArray_2lEboolgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_2<bool>[nElements] : new ::NicaArray_2<bool>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_2lEboolgR(void *p) {
      delete ((::NicaArray_2<bool>*)p);
   }
   static void deleteArray_NicaArray_2lEboolgR(void *p) {
      delete [] ((::NicaArray_2<bool>*)p);
   }
   static void destruct_NicaArray_2lEboolgR(void *p) {
      typedef ::NicaArray_2<bool> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_2<bool>

//______________________________________________________________________________
template <> void NicaArray_3<double>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_3<double>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_3<double>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_3<double>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_3lEdoublegR(void *p) {
      return  p ? new(p) ::NicaArray_3<double> : new ::NicaArray_3<double>;
   }
   static void *newArray_NicaArray_3lEdoublegR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_3<double>[nElements] : new ::NicaArray_3<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_3lEdoublegR(void *p) {
      delete ((::NicaArray_3<double>*)p);
   }
   static void deleteArray_NicaArray_3lEdoublegR(void *p) {
      delete [] ((::NicaArray_3<double>*)p);
   }
   static void destruct_NicaArray_3lEdoublegR(void *p) {
      typedef ::NicaArray_3<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_3<double>

//______________________________________________________________________________
template <> void NicaArray_3<float>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_3<float>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_3<float>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_3<float>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_3lEfloatgR(void *p) {
      return  p ? new(p) ::NicaArray_3<float> : new ::NicaArray_3<float>;
   }
   static void *newArray_NicaArray_3lEfloatgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_3<float>[nElements] : new ::NicaArray_3<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_3lEfloatgR(void *p) {
      delete ((::NicaArray_3<float>*)p);
   }
   static void deleteArray_NicaArray_3lEfloatgR(void *p) {
      delete [] ((::NicaArray_3<float>*)p);
   }
   static void destruct_NicaArray_3lEfloatgR(void *p) {
      typedef ::NicaArray_3<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_3<float>

//______________________________________________________________________________
template <> void NicaArray_3<short>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_3<short>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_3<short>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_3<short>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_3lEshortgR(void *p) {
      return  p ? new(p) ::NicaArray_3<short> : new ::NicaArray_3<short>;
   }
   static void *newArray_NicaArray_3lEshortgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_3<short>[nElements] : new ::NicaArray_3<short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_3lEshortgR(void *p) {
      delete ((::NicaArray_3<short>*)p);
   }
   static void deleteArray_NicaArray_3lEshortgR(void *p) {
      delete [] ((::NicaArray_3<short>*)p);
   }
   static void destruct_NicaArray_3lEshortgR(void *p) {
      typedef ::NicaArray_3<short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_3<short>

//______________________________________________________________________________
template <> void NicaArray_3<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_3<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_3<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_3<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_3lEintgR(void *p) {
      return  p ? new(p) ::NicaArray_3<int> : new ::NicaArray_3<int>;
   }
   static void *newArray_NicaArray_3lEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_3<int>[nElements] : new ::NicaArray_3<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_3lEintgR(void *p) {
      delete ((::NicaArray_3<int>*)p);
   }
   static void deleteArray_NicaArray_3lEintgR(void *p) {
      delete [] ((::NicaArray_3<int>*)p);
   }
   static void destruct_NicaArray_3lEintgR(void *p) {
      typedef ::NicaArray_3<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_3<int>

//______________________________________________________________________________
template <> void NicaArray_3<bool>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_3<bool>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_3<bool>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_3<bool>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_3lEboolgR(void *p) {
      return  p ? new(p) ::NicaArray_3<bool> : new ::NicaArray_3<bool>;
   }
   static void *newArray_NicaArray_3lEboolgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_3<bool>[nElements] : new ::NicaArray_3<bool>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_3lEboolgR(void *p) {
      delete ((::NicaArray_3<bool>*)p);
   }
   static void deleteArray_NicaArray_3lEboolgR(void *p) {
      delete [] ((::NicaArray_3<bool>*)p);
   }
   static void destruct_NicaArray_3lEboolgR(void *p) {
      typedef ::NicaArray_3<bool> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_3<bool>

//______________________________________________________________________________
template <> void NicaArray_4<double>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_4<double>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_4<double>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_4<double>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_4lEdoublegR(void *p) {
      return  p ? new(p) ::NicaArray_4<double> : new ::NicaArray_4<double>;
   }
   static void *newArray_NicaArray_4lEdoublegR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_4<double>[nElements] : new ::NicaArray_4<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_4lEdoublegR(void *p) {
      delete ((::NicaArray_4<double>*)p);
   }
   static void deleteArray_NicaArray_4lEdoublegR(void *p) {
      delete [] ((::NicaArray_4<double>*)p);
   }
   static void destruct_NicaArray_4lEdoublegR(void *p) {
      typedef ::NicaArray_4<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_4<double>

//______________________________________________________________________________
template <> void NicaArray_4<float>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_4<float>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_4<float>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_4<float>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_4lEfloatgR(void *p) {
      return  p ? new(p) ::NicaArray_4<float> : new ::NicaArray_4<float>;
   }
   static void *newArray_NicaArray_4lEfloatgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_4<float>[nElements] : new ::NicaArray_4<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_4lEfloatgR(void *p) {
      delete ((::NicaArray_4<float>*)p);
   }
   static void deleteArray_NicaArray_4lEfloatgR(void *p) {
      delete [] ((::NicaArray_4<float>*)p);
   }
   static void destruct_NicaArray_4lEfloatgR(void *p) {
      typedef ::NicaArray_4<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_4<float>

//______________________________________________________________________________
template <> void NicaArray_4<short>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_4<short>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_4<short>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_4<short>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_4lEshortgR(void *p) {
      return  p ? new(p) ::NicaArray_4<short> : new ::NicaArray_4<short>;
   }
   static void *newArray_NicaArray_4lEshortgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_4<short>[nElements] : new ::NicaArray_4<short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_4lEshortgR(void *p) {
      delete ((::NicaArray_4<short>*)p);
   }
   static void deleteArray_NicaArray_4lEshortgR(void *p) {
      delete [] ((::NicaArray_4<short>*)p);
   }
   static void destruct_NicaArray_4lEshortgR(void *p) {
      typedef ::NicaArray_4<short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_4<short>

//______________________________________________________________________________
template <> void NicaArray_4<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_4<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_4<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_4<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_4lEintgR(void *p) {
      return  p ? new(p) ::NicaArray_4<int> : new ::NicaArray_4<int>;
   }
   static void *newArray_NicaArray_4lEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_4<int>[nElements] : new ::NicaArray_4<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_4lEintgR(void *p) {
      delete ((::NicaArray_4<int>*)p);
   }
   static void deleteArray_NicaArray_4lEintgR(void *p) {
      delete [] ((::NicaArray_4<int>*)p);
   }
   static void destruct_NicaArray_4lEintgR(void *p) {
      typedef ::NicaArray_4<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_4<int>

//______________________________________________________________________________
template <> void NicaArray_4<bool>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaArray_4<bool>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaArray_4<bool>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaArray_4<bool>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaArray_4lEboolgR(void *p) {
      return  p ? new(p) ::NicaArray_4<bool> : new ::NicaArray_4<bool>;
   }
   static void *newArray_NicaArray_4lEboolgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaArray_4<bool>[nElements] : new ::NicaArray_4<bool>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaArray_4lEboolgR(void *p) {
      delete ((::NicaArray_4<bool>*)p);
   }
   static void deleteArray_NicaArray_4lEboolgR(void *p) {
      delete [] ((::NicaArray_4<bool>*)p);
   }
   static void destruct_NicaArray_4lEboolgR(void *p) {
      typedef ::NicaArray_4<bool> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaArray_4<bool>

//______________________________________________________________________________
void NicaOptionArray::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaOptionArray.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaOptionArray::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaOptionArray::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaOptionArray(void *p) {
      return  p ? new(p) ::NicaOptionArray : new ::NicaOptionArray;
   }
   static void *newArray_NicaOptionArray(Long_t nElements, void *p) {
      return p ? new(p) ::NicaOptionArray[nElements] : new ::NicaOptionArray[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaOptionArray(void *p) {
      delete ((::NicaOptionArray*)p);
   }
   static void deleteArray_NicaOptionArray(void *p) {
      delete [] ((::NicaOptionArray*)p);
   }
   static void destruct_NicaOptionArray(void *p) {
      typedef ::NicaOptionArray current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaOptionArray

//______________________________________________________________________________
void NicaOptionConverter::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaOptionConverter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaOptionConverter::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaOptionConverter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaOptionConverter(void *p) {
      return  p ? new(p) ::NicaOptionConverter : new ::NicaOptionConverter;
   }
   static void *newArray_NicaOptionConverter(Long_t nElements, void *p) {
      return p ? new(p) ::NicaOptionConverter[nElements] : new ::NicaOptionConverter[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaOptionConverter(void *p) {
      delete ((::NicaOptionConverter*)p);
   }
   static void deleteArray_NicaOptionConverter(void *p) {
      delete [] ((::NicaOptionConverter*)p);
   }
   static void destruct_NicaOptionConverter(void *p) {
      typedef ::NicaOptionConverter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaOptionConverter

//______________________________________________________________________________
void NicaPack::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaPack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaPack::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaPack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaPack(void *p) {
      return  p ? new(p) ::NicaPack : new ::NicaPack;
   }
   static void *newArray_NicaPack(Long_t nElements, void *p) {
      return p ? new(p) ::NicaPack[nElements] : new ::NicaPack[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaPack(void *p) {
      delete ((::NicaPack*)p);
   }
   static void deleteArray_NicaPack(void *p) {
      delete [] ((::NicaPack*)p);
   }
   static void destruct_NicaPack(void *p) {
      typedef ::NicaPack current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaPack(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaPack*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaPack

//______________________________________________________________________________
void NicaParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaParameter(void *p) {
      return  p ? new(p) ::NicaParameter : new ::NicaParameter;
   }
   static void *newArray_NicaParameter(Long_t nElements, void *p) {
      return p ? new(p) ::NicaParameter[nElements] : new ::NicaParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaParameter(void *p) {
      delete ((::NicaParameter*)p);
   }
   static void deleteArray_NicaParameter(void *p) {
      delete [] ((::NicaParameter*)p);
   }
   static void destruct_NicaParameter(void *p) {
      typedef ::NicaParameter current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaParameter(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaParameter*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaParameter

//______________________________________________________________________________
void NicaParameterInt::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaParameterInt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaParameterInt::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaParameterInt::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaParameterInt(void *p) {
      return  p ? new(p) ::NicaParameterInt : new ::NicaParameterInt;
   }
   static void *newArray_NicaParameterInt(Long_t nElements, void *p) {
      return p ? new(p) ::NicaParameterInt[nElements] : new ::NicaParameterInt[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaParameterInt(void *p) {
      delete ((::NicaParameterInt*)p);
   }
   static void deleteArray_NicaParameterInt(void *p) {
      delete [] ((::NicaParameterInt*)p);
   }
   static void destruct_NicaParameterInt(void *p) {
      typedef ::NicaParameterInt current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaParameterInt(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaParameterInt*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaParameterInt

//______________________________________________________________________________
void NicaParameterDouble::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaParameterDouble.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaParameterDouble::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaParameterDouble::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaParameterDouble(void *p) {
      return  p ? new(p) ::NicaParameterDouble : new ::NicaParameterDouble;
   }
   static void *newArray_NicaParameterDouble(Long_t nElements, void *p) {
      return p ? new(p) ::NicaParameterDouble[nElements] : new ::NicaParameterDouble[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaParameterDouble(void *p) {
      delete ((::NicaParameterDouble*)p);
   }
   static void deleteArray_NicaParameterDouble(void *p) {
      delete [] ((::NicaParameterDouble*)p);
   }
   static void destruct_NicaParameterDouble(void *p) {
      typedef ::NicaParameterDouble current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaParameterDouble(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaParameterDouble*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaParameterDouble

//______________________________________________________________________________
void NicaParameterFloat::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaParameterFloat.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaParameterFloat::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaParameterFloat::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaParameterFloat(void *p) {
      return  p ? new(p) ::NicaParameterFloat : new ::NicaParameterFloat;
   }
   static void *newArray_NicaParameterFloat(Long_t nElements, void *p) {
      return p ? new(p) ::NicaParameterFloat[nElements] : new ::NicaParameterFloat[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaParameterFloat(void *p) {
      delete ((::NicaParameterFloat*)p);
   }
   static void deleteArray_NicaParameterFloat(void *p) {
      delete [] ((::NicaParameterFloat*)p);
   }
   static void destruct_NicaParameterFloat(void *p) {
      typedef ::NicaParameterFloat current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaParameterFloat(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaParameterFloat*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaParameterFloat

//______________________________________________________________________________
void NicaParameterString::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaParameterString.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaParameterString::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaParameterString::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaParameterString(void *p) {
      return  p ? new(p) ::NicaParameterString : new ::NicaParameterString;
   }
   static void *newArray_NicaParameterString(Long_t nElements, void *p) {
      return p ? new(p) ::NicaParameterString[nElements] : new ::NicaParameterString[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaParameterString(void *p) {
      delete ((::NicaParameterString*)p);
   }
   static void deleteArray_NicaParameterString(void *p) {
      delete [] ((::NicaParameterString*)p);
   }
   static void destruct_NicaParameterString(void *p) {
      typedef ::NicaParameterString current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaParameterString(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaParameterString*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaParameterString

//______________________________________________________________________________
void NicaParameterUInt::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaParameterUInt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaParameterUInt::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaParameterUInt::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaParameterUInt(void *p) {
      return  p ? new(p) ::NicaParameterUInt : new ::NicaParameterUInt;
   }
   static void *newArray_NicaParameterUInt(Long_t nElements, void *p) {
      return p ? new(p) ::NicaParameterUInt[nElements] : new ::NicaParameterUInt[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaParameterUInt(void *p) {
      delete ((::NicaParameterUInt*)p);
   }
   static void deleteArray_NicaParameterUInt(void *p) {
      delete [] ((::NicaParameterUInt*)p);
   }
   static void destruct_NicaParameterUInt(void *p) {
      typedef ::NicaParameterUInt current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaParameterUInt(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaParameterUInt*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaParameterUInt

//______________________________________________________________________________
void NicaParameterULong::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaParameterULong.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaParameterULong::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaParameterULong::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaParameterULong(void *p) {
      return  p ? new(p) ::NicaParameterULong : new ::NicaParameterULong;
   }
   static void *newArray_NicaParameterULong(Long_t nElements, void *p) {
      return p ? new(p) ::NicaParameterULong[nElements] : new ::NicaParameterULong[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaParameterULong(void *p) {
      delete ((::NicaParameterULong*)p);
   }
   static void deleteArray_NicaParameterULong(void *p) {
      delete [] ((::NicaParameterULong*)p);
   }
   static void destruct_NicaParameterULong(void *p) {
      typedef ::NicaParameterULong current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaParameterULong(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaParameterULong*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaParameterULong

//______________________________________________________________________________
void NicaParameterULong64::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaParameterULong64.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaParameterULong64::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaParameterULong64::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaParameterULong64(void *p) {
      return  p ? new(p) ::NicaParameterULong64 : new ::NicaParameterULong64;
   }
   static void *newArray_NicaParameterULong64(Long_t nElements, void *p) {
      return p ? new(p) ::NicaParameterULong64[nElements] : new ::NicaParameterULong64[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaParameterULong64(void *p) {
      delete ((::NicaParameterULong64*)p);
   }
   static void deleteArray_NicaParameterULong64(void *p) {
      delete [] ((::NicaParameterULong64*)p);
   }
   static void destruct_NicaParameterULong64(void *p) {
      typedef ::NicaParameterULong64 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaParameterULong64(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaParameterULong64*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaParameterULong64

//______________________________________________________________________________
void NicaParameterBool::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaParameterBool.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaParameterBool::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaParameterBool::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaParameterBool(void *p) {
      return  p ? new(p) ::NicaParameterBool : new ::NicaParameterBool;
   }
   static void *newArray_NicaParameterBool(Long_t nElements, void *p) {
      return p ? new(p) ::NicaParameterBool[nElements] : new ::NicaParameterBool[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaParameterBool(void *p) {
      delete ((::NicaParameterBool*)p);
   }
   static void deleteArray_NicaParameterBool(void *p) {
      delete [] ((::NicaParameterBool*)p);
   }
   static void destruct_NicaParameterBool(void *p) {
      typedef ::NicaParameterBool current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaParameterBool(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaParameterBool*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaParameterBool

//______________________________________________________________________________
void NicaObjectMatrix_1::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaObjectMatrix_1.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaObjectMatrix_1::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaObjectMatrix_1::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaObjectMatrix_1(void *p) {
      return  p ? new(p) ::NicaObjectMatrix_1 : new ::NicaObjectMatrix_1;
   }
   static void *newArray_NicaObjectMatrix_1(Long_t nElements, void *p) {
      return p ? new(p) ::NicaObjectMatrix_1[nElements] : new ::NicaObjectMatrix_1[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaObjectMatrix_1(void *p) {
      delete ((::NicaObjectMatrix_1*)p);
   }
   static void deleteArray_NicaObjectMatrix_1(void *p) {
      delete [] ((::NicaObjectMatrix_1*)p);
   }
   static void destruct_NicaObjectMatrix_1(void *p) {
      typedef ::NicaObjectMatrix_1 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaObjectMatrix_1

//______________________________________________________________________________
void NicaObjectMatrix_2::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaObjectMatrix_2.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaObjectMatrix_2::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaObjectMatrix_2::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaObjectMatrix_2(void *p) {
      return  p ? new(p) ::NicaObjectMatrix_2 : new ::NicaObjectMatrix_2;
   }
   static void *newArray_NicaObjectMatrix_2(Long_t nElements, void *p) {
      return p ? new(p) ::NicaObjectMatrix_2[nElements] : new ::NicaObjectMatrix_2[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaObjectMatrix_2(void *p) {
      delete ((::NicaObjectMatrix_2*)p);
   }
   static void deleteArray_NicaObjectMatrix_2(void *p) {
      delete [] ((::NicaObjectMatrix_2*)p);
   }
   static void destruct_NicaObjectMatrix_2(void *p) {
      typedef ::NicaObjectMatrix_2 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaObjectMatrix_2

//______________________________________________________________________________
void NicaObjectMatrix_3::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaObjectMatrix_3.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaObjectMatrix_3::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaObjectMatrix_3::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaObjectMatrix_3(void *p) {
      return  p ? new(p) ::NicaObjectMatrix_3 : new ::NicaObjectMatrix_3;
   }
   static void *newArray_NicaObjectMatrix_3(Long_t nElements, void *p) {
      return p ? new(p) ::NicaObjectMatrix_3[nElements] : new ::NicaObjectMatrix_3[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaObjectMatrix_3(void *p) {
      delete ((::NicaObjectMatrix_3*)p);
   }
   static void deleteArray_NicaObjectMatrix_3(void *p) {
      delete [] ((::NicaObjectMatrix_3*)p);
   }
   static void destruct_NicaObjectMatrix_3(void *p) {
      typedef ::NicaObjectMatrix_3 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaObjectMatrix_3

//______________________________________________________________________________
void NicaObjectMatrix_4::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaObjectMatrix_4.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaObjectMatrix_4::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaObjectMatrix_4::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaObjectMatrix_4(void *p) {
      return  p ? new(p) ::NicaObjectMatrix_4 : new ::NicaObjectMatrix_4;
   }
   static void *newArray_NicaObjectMatrix_4(Long_t nElements, void *p) {
      return p ? new(p) ::NicaObjectMatrix_4[nElements] : new ::NicaObjectMatrix_4[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaObjectMatrix_4(void *p) {
      delete ((::NicaObjectMatrix_4*)p);
   }
   static void deleteArray_NicaObjectMatrix_4(void *p) {
      delete [] ((::NicaObjectMatrix_4*)p);
   }
   static void destruct_NicaObjectMatrix_4(void *p) {
      typedef ::NicaObjectMatrix_4 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaObjectMatrix_4

//______________________________________________________________________________
void NicaHelixBase::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHelixBase.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHelixBase::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHelixBase::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHelixBase(void *p) {
      return  p ? new(p) ::NicaHelixBase : new ::NicaHelixBase;
   }
   static void *newArray_NicaHelixBase(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHelixBase[nElements] : new ::NicaHelixBase[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHelixBase(void *p) {
      delete ((::NicaHelixBase*)p);
   }
   static void deleteArray_NicaHelixBase(void *p) {
      delete [] ((::NicaHelixBase*)p);
   }
   static void destruct_NicaHelixBase(void *p) {
      typedef ::NicaHelixBase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHelixBase

//______________________________________________________________________________
void NicaHelixZ::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHelixZ.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHelixZ::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHelixZ::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHelixZ(void *p) {
      return  p ? new(p) ::NicaHelixZ : new ::NicaHelixZ;
   }
   static void *newArray_NicaHelixZ(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHelixZ[nElements] : new ::NicaHelixZ[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHelixZ(void *p) {
      delete ((::NicaHelixZ*)p);
   }
   static void deleteArray_NicaHelixZ(void *p) {
      delete [] ((::NicaHelixZ*)p);
   }
   static void destruct_NicaHelixZ(void *p) {
      typedef ::NicaHelixZ current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHelixZ

//______________________________________________________________________________
void NicaHelixX::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHelixX.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHelixX::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHelixX::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHelixX(void *p) {
      return  p ? new(p) ::NicaHelixX : new ::NicaHelixX;
   }
   static void *newArray_NicaHelixX(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHelixX[nElements] : new ::NicaHelixX[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHelixX(void *p) {
      delete ((::NicaHelixX*)p);
   }
   static void deleteArray_NicaHelixX(void *p) {
      delete [] ((::NicaHelixX*)p);
   }
   static void destruct_NicaHelixX(void *p) {
      typedef ::NicaHelixX current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHelixX

//______________________________________________________________________________
void NicaHelixY::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHelixY.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHelixY::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHelixY::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHelixY(void *p) {
      return  p ? new(p) ::NicaHelixY : new ::NicaHelixY;
   }
   static void *newArray_NicaHelixY(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHelixY[nElements] : new ::NicaHelixY[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHelixY(void *p) {
      delete ((::NicaHelixY*)p);
   }
   static void deleteArray_NicaHelixY(void *p) {
      delete [] ((::NicaHelixY*)p);
   }
   static void destruct_NicaHelixY(void *p) {
      typedef ::NicaHelixY current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHelixY

//______________________________________________________________________________
void NicaLink::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaLink.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaLink::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaLink::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaLink(void *p) {
      return  p ? new(p) ::NicaLink : new ::NicaLink;
   }
   static void *newArray_NicaLink(Long_t nElements, void *p) {
      return p ? new(p) ::NicaLink[nElements] : new ::NicaLink[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaLink(void *p) {
      delete ((::NicaLink*)p);
   }
   static void deleteArray_NicaLink(void *p) {
      delete [] ((::NicaLink*)p);
   }
   static void destruct_NicaLink(void *p) {
      typedef ::NicaLink current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaLink

//______________________________________________________________________________
void NicaConst::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaConst.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaConst::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaConst::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaConst(void *p) {
      return  p ? new(p) ::NicaConst : new ::NicaConst;
   }
   static void *newArray_NicaConst(Long_t nElements, void *p) {
      return p ? new(p) ::NicaConst[nElements] : new ::NicaConst[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaConst(void *p) {
      delete ((::NicaConst*)p);
   }
   static void deleteArray_NicaConst(void *p) {
      delete [] ((::NicaConst*)p);
   }
   static void destruct_NicaConst(void *p) {
      typedef ::NicaConst current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaConst

//______________________________________________________________________________
void NicaXMLAttrib::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaXMLAttrib.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaXMLAttrib::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaXMLAttrib::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaXMLAttrib(void *p) {
      return  p ? new(p) ::NicaXMLAttrib : new ::NicaXMLAttrib;
   }
   static void *newArray_NicaXMLAttrib(Long_t nElements, void *p) {
      return p ? new(p) ::NicaXMLAttrib[nElements] : new ::NicaXMLAttrib[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaXMLAttrib(void *p) {
      delete ((::NicaXMLAttrib*)p);
   }
   static void deleteArray_NicaXMLAttrib(void *p) {
      delete [] ((::NicaXMLAttrib*)p);
   }
   static void destruct_NicaXMLAttrib(void *p) {
      typedef ::NicaXMLAttrib current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaXMLAttrib

//______________________________________________________________________________
void NicaXMLNode::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaXMLNode.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaXMLNode::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaXMLNode::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaXMLNode(void *p) {
      return  p ? new(p) ::NicaXMLNode : new ::NicaXMLNode;
   }
   static void *newArray_NicaXMLNode(Long_t nElements, void *p) {
      return p ? new(p) ::NicaXMLNode[nElements] : new ::NicaXMLNode[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaXMLNode(void *p) {
      delete ((::NicaXMLNode*)p);
   }
   static void deleteArray_NicaXMLNode(void *p) {
      delete [] ((::NicaXMLNode*)p);
   }
   static void destruct_NicaXMLNode(void *p) {
      typedef ::NicaXMLNode current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaXMLNode

//______________________________________________________________________________
void NicaXMLFile::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaXMLFile.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaXMLFile::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaXMLFile::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaXMLFile(void *p) {
      return  p ? new(p) ::NicaXMLFile : new ::NicaXMLFile;
   }
   static void *newArray_NicaXMLFile(Long_t nElements, void *p) {
      return p ? new(p) ::NicaXMLFile[nElements] : new ::NicaXMLFile[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaXMLFile(void *p) {
      delete ((::NicaXMLFile*)p);
   }
   static void deleteArray_NicaXMLFile(void *p) {
      delete [] ((::NicaXMLFile*)p);
   }
   static void destruct_NicaXMLFile(void *p) {
      typedef ::NicaXMLFile current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaXMLFile

//______________________________________________________________________________
void NicaJobs::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaJobs.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaJobs::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaJobs::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaJobs(void *p) {
      delete ((::NicaJobs*)p);
   }
   static void deleteArray_NicaJobs(void *p) {
      delete [] ((::NicaJobs*)p);
   }
   static void destruct_NicaJobs(void *p) {
      typedef ::NicaJobs current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaJobs

//______________________________________________________________________________
void NicaPackage::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaPackage.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaPackage::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaPackage::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaPackage(void *p) {
      return  p ? new(p) ::NicaPackage : new ::NicaPackage;
   }
   static void *newArray_NicaPackage(Long_t nElements, void *p) {
      return p ? new(p) ::NicaPackage[nElements] : new ::NicaPackage[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaPackage(void *p) {
      delete ((::NicaPackage*)p);
   }
   static void deleteArray_NicaPackage(void *p) {
      delete [] ((::NicaPackage*)p);
   }
   static void destruct_NicaPackage(void *p) {
      typedef ::NicaPackage current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaPackage(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaPackage*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaPackage

//______________________________________________________________________________
void NicaPackage2HTML::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaPackage2HTML.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaPackage2HTML::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaPackage2HTML::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaPackage2HTML(void *p) {
      delete ((::NicaPackage2HTML*)p);
   }
   static void deleteArray_NicaPackage2HTML(void *p) {
      delete [] ((::NicaPackage2HTML*)p);
   }
   static void destruct_NicaPackage2HTML(void *p) {
      typedef ::NicaPackage2HTML current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaPackage2HTML

//______________________________________________________________________________
void NicaAnaFile::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaAnaFile.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaAnaFile::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaAnaFile::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaAnaFile(void *p) {
      return  p ? new(p) ::NicaAnaFile : new ::NicaAnaFile;
   }
   static void *newArray_NicaAnaFile(Long_t nElements, void *p) {
      return p ? new(p) ::NicaAnaFile[nElements] : new ::NicaAnaFile[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaAnaFile(void *p) {
      delete ((::NicaAnaFile*)p);
   }
   static void deleteArray_NicaAnaFile(void *p) {
      delete [] ((::NicaAnaFile*)p);
   }
   static void destruct_NicaAnaFile(void *p) {
      typedef ::NicaAnaFile current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaAnaFile

//______________________________________________________________________________
void NicaHtmlCore::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHtmlCore.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHtmlCore::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHtmlCore::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHtmlCore(void *p) {
      return  p ? new(p) ::NicaHtmlCore : new ::NicaHtmlCore;
   }
   static void *newArray_NicaHtmlCore(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHtmlCore[nElements] : new ::NicaHtmlCore[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHtmlCore(void *p) {
      delete ((::NicaHtmlCore*)p);
   }
   static void deleteArray_NicaHtmlCore(void *p) {
      delete [] ((::NicaHtmlCore*)p);
   }
   static void destruct_NicaHtmlCore(void *p) {
      typedef ::NicaHtmlCore current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHtmlCore

//______________________________________________________________________________
void NicaPackageSql::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaPackageSql.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaPackageSql::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaPackageSql::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaPackageSql(void *p) {
      delete ((::NicaPackageSql*)p);
   }
   static void deleteArray_NicaPackageSql(void *p) {
      delete [] ((::NicaPackageSql*)p);
   }
   static void destruct_NicaPackageSql(void *p) {
      typedef ::NicaPackageSql current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaPackageSql

//______________________________________________________________________________
void NicaHtmlObject::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHtmlObject.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHtmlObject::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHtmlObject::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaHtmlObject(void *p) {
      delete ((::NicaHtmlObject*)p);
   }
   static void deleteArray_NicaHtmlObject(void *p) {
      delete [] ((::NicaHtmlObject*)p);
   }
   static void destruct_NicaHtmlObject(void *p) {
      typedef ::NicaHtmlObject current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHtmlObject

//______________________________________________________________________________
void NicaHtmlDiv::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHtmlDiv.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHtmlDiv::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHtmlDiv::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHtmlDiv(void *p) {
      return  p ? new(p) ::NicaHtmlDiv : new ::NicaHtmlDiv;
   }
   static void *newArray_NicaHtmlDiv(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHtmlDiv[nElements] : new ::NicaHtmlDiv[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHtmlDiv(void *p) {
      delete ((::NicaHtmlDiv*)p);
   }
   static void deleteArray_NicaHtmlDiv(void *p) {
      delete [] ((::NicaHtmlDiv*)p);
   }
   static void destruct_NicaHtmlDiv(void *p) {
      typedef ::NicaHtmlDiv current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHtmlDiv

//______________________________________________________________________________
void NicaHtmlFile::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHtmlFile.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHtmlFile::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHtmlFile::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHtmlFile(void *p) {
      return  p ? new(p) ::NicaHtmlFile : new ::NicaHtmlFile;
   }
   static void *newArray_NicaHtmlFile(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHtmlFile[nElements] : new ::NicaHtmlFile[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHtmlFile(void *p) {
      delete ((::NicaHtmlFile*)p);
   }
   static void deleteArray_NicaHtmlFile(void *p) {
      delete [] ((::NicaHtmlFile*)p);
   }
   static void destruct_NicaHtmlFile(void *p) {
      typedef ::NicaHtmlFile current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHtmlFile

//______________________________________________________________________________
void NicaHtmlTable::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHtmlTable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHtmlTable::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHtmlTable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHtmlTable(void *p) {
      return  p ? new(p) ::NicaHtmlTable : new ::NicaHtmlTable;
   }
   static void *newArray_NicaHtmlTable(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHtmlTable[nElements] : new ::NicaHtmlTable[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHtmlTable(void *p) {
      delete ((::NicaHtmlTable*)p);
   }
   static void deleteArray_NicaHtmlTable(void *p) {
      delete [] ((::NicaHtmlTable*)p);
   }
   static void destruct_NicaHtmlTable(void *p) {
      typedef ::NicaHtmlTable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHtmlTable

//______________________________________________________________________________
void NicaHtmlRow::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHtmlRow.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHtmlRow::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHtmlRow::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHtmlRow(void *p) {
      return  p ? new(p) ::NicaHtmlRow : new ::NicaHtmlRow;
   }
   static void *newArray_NicaHtmlRow(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHtmlRow[nElements] : new ::NicaHtmlRow[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHtmlRow(void *p) {
      delete ((::NicaHtmlRow*)p);
   }
   static void deleteArray_NicaHtmlRow(void *p) {
      delete [] ((::NicaHtmlRow*)p);
   }
   static void destruct_NicaHtmlRow(void *p) {
      typedef ::NicaHtmlRow current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHtmlRow

//______________________________________________________________________________
void NicaHtmlCell::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHtmlCell.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHtmlCell::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHtmlCell::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHtmlCell(void *p) {
      return  p ? new(p) ::NicaHtmlCell : new ::NicaHtmlCell;
   }
   static void *newArray_NicaHtmlCell(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHtmlCell[nElements] : new ::NicaHtmlCell[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHtmlCell(void *p) {
      delete ((::NicaHtmlCell*)p);
   }
   static void deleteArray_NicaHtmlCell(void *p) {
      delete [] ((::NicaHtmlCell*)p);
   }
   static void destruct_NicaHtmlCell(void *p) {
      typedef ::NicaHtmlCell current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHtmlCell

//______________________________________________________________________________
void NicaHtmlCellCol::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHtmlCellCol.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHtmlCellCol::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHtmlCellCol::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHtmlCellCol(void *p) {
      return  p ? new(p) ::NicaHtmlCellCol : new ::NicaHtmlCellCol;
   }
   static void *newArray_NicaHtmlCellCol(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHtmlCellCol[nElements] : new ::NicaHtmlCellCol[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHtmlCellCol(void *p) {
      delete ((::NicaHtmlCellCol*)p);
   }
   static void deleteArray_NicaHtmlCellCol(void *p) {
      delete [] ((::NicaHtmlCellCol*)p);
   }
   static void destruct_NicaHtmlCellCol(void *p) {
      typedef ::NicaHtmlCellCol current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHtmlCellCol

//______________________________________________________________________________
void NicaHtmlCellRow::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHtmlCellRow.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHtmlCellRow::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHtmlCellRow::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHtmlCellRow(void *p) {
      return  p ? new(p) ::NicaHtmlCellRow : new ::NicaHtmlCellRow;
   }
   static void *newArray_NicaHtmlCellRow(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHtmlCellRow[nElements] : new ::NicaHtmlCellRow[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHtmlCellRow(void *p) {
      delete ((::NicaHtmlCellRow*)p);
   }
   static void deleteArray_NicaHtmlCellRow(void *p) {
      delete [] ((::NicaHtmlCellRow*)p);
   }
   static void destruct_NicaHtmlCellRow(void *p) {
      typedef ::NicaHtmlCellRow current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHtmlCellRow

//______________________________________________________________________________
void NicaDividedHisto1D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaDividedHisto1D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaDividedHisto1D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaDividedHisto1D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaDividedHisto1D(void *p) {
      return  p ? new(p) ::NicaDividedHisto1D : new ::NicaDividedHisto1D;
   }
   static void *newArray_NicaDividedHisto1D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaDividedHisto1D[nElements] : new ::NicaDividedHisto1D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaDividedHisto1D(void *p) {
      delete ((::NicaDividedHisto1D*)p);
   }
   static void deleteArray_NicaDividedHisto1D(void *p) {
      delete [] ((::NicaDividedHisto1D*)p);
   }
   static void destruct_NicaDividedHisto1D(void *p) {
      typedef ::NicaDividedHisto1D current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaDividedHisto1D(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaDividedHisto1D*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaDividedHisto1D

//______________________________________________________________________________
void NicaDividedHisto2D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaDividedHisto2D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaDividedHisto2D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaDividedHisto2D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaDividedHisto2D(void *p) {
      return  p ? new(p) ::NicaDividedHisto2D : new ::NicaDividedHisto2D;
   }
   static void *newArray_NicaDividedHisto2D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaDividedHisto2D[nElements] : new ::NicaDividedHisto2D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaDividedHisto2D(void *p) {
      delete ((::NicaDividedHisto2D*)p);
   }
   static void deleteArray_NicaDividedHisto2D(void *p) {
      delete [] ((::NicaDividedHisto2D*)p);
   }
   static void destruct_NicaDividedHisto2D(void *p) {
      typedef ::NicaDividedHisto2D current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaDividedHisto2D(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaDividedHisto2D*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaDividedHisto2D

//______________________________________________________________________________
void NicaDividedHisto3D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaDividedHisto3D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaDividedHisto3D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaDividedHisto3D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaDividedHisto3D(void *p) {
      return  p ? new(p) ::NicaDividedHisto3D : new ::NicaDividedHisto3D;
   }
   static void *newArray_NicaDividedHisto3D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaDividedHisto3D[nElements] : new ::NicaDividedHisto3D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaDividedHisto3D(void *p) {
      delete ((::NicaDividedHisto3D*)p);
   }
   static void deleteArray_NicaDividedHisto3D(void *p) {
      delete [] ((::NicaDividedHisto3D*)p);
   }
   static void destruct_NicaDividedHisto3D(void *p) {
      typedef ::NicaDividedHisto3D current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaDividedHisto3D(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaDividedHisto3D*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaDividedHisto3D

//______________________________________________________________________________
void NicaHistogramAxisConf::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramAxisConf.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramAxisConf::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramAxisConf::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramAxisConf(void *p) {
      return  p ? new(p) ::NicaHistogramAxisConf : new ::NicaHistogramAxisConf;
   }
   static void *newArray_NicaHistogramAxisConf(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramAxisConf[nElements] : new ::NicaHistogramAxisConf[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramAxisConf(void *p) {
      delete ((::NicaHistogramAxisConf*)p);
   }
   static void deleteArray_NicaHistogramAxisConf(void *p) {
      delete [] ((::NicaHistogramAxisConf*)p);
   }
   static void destruct_NicaHistogramAxisConf(void *p) {
      typedef ::NicaHistogramAxisConf current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramAxisConf

//______________________________________________________________________________
template <> void NicaHistogramManager_1_1D<TH1I>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_1_1D<TH1I>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_1_1D<TH1I>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_1_1D<TH1I>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_1_1DlETH1IgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_1_1D<TH1I> : new ::NicaHistogramManager_1_1D<TH1I>;
   }
   static void *newArray_NicaHistogramManager_1_1DlETH1IgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_1_1D<TH1I>[nElements] : new ::NicaHistogramManager_1_1D<TH1I>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_1_1DlETH1IgR(void *p) {
      delete ((::NicaHistogramManager_1_1D<TH1I>*)p);
   }
   static void deleteArray_NicaHistogramManager_1_1DlETH1IgR(void *p) {
      delete [] ((::NicaHistogramManager_1_1D<TH1I>*)p);
   }
   static void destruct_NicaHistogramManager_1_1DlETH1IgR(void *p) {
      typedef ::NicaHistogramManager_1_1D<TH1I> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_1_1D<TH1I>

//______________________________________________________________________________
template <> void NicaHistogramManager_1_1D<TH1F>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_1_1D<TH1F>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_1_1D<TH1F>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_1_1D<TH1F>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_1_1DlETH1FgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_1_1D<TH1F> : new ::NicaHistogramManager_1_1D<TH1F>;
   }
   static void *newArray_NicaHistogramManager_1_1DlETH1FgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_1_1D<TH1F>[nElements] : new ::NicaHistogramManager_1_1D<TH1F>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_1_1DlETH1FgR(void *p) {
      delete ((::NicaHistogramManager_1_1D<TH1F>*)p);
   }
   static void deleteArray_NicaHistogramManager_1_1DlETH1FgR(void *p) {
      delete [] ((::NicaHistogramManager_1_1D<TH1F>*)p);
   }
   static void destruct_NicaHistogramManager_1_1DlETH1FgR(void *p) {
      typedef ::NicaHistogramManager_1_1D<TH1F> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_1_1D<TH1F>

//______________________________________________________________________________
template <> void NicaHistogramManager_1_1D<TH1D>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_1_1D<TH1D>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_1_1D<TH1D>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_1_1D<TH1D>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_1_1DlETH1DgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_1_1D<TH1D> : new ::NicaHistogramManager_1_1D<TH1D>;
   }
   static void *newArray_NicaHistogramManager_1_1DlETH1DgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_1_1D<TH1D>[nElements] : new ::NicaHistogramManager_1_1D<TH1D>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_1_1DlETH1DgR(void *p) {
      delete ((::NicaHistogramManager_1_1D<TH1D>*)p);
   }
   static void deleteArray_NicaHistogramManager_1_1DlETH1DgR(void *p) {
      delete [] ((::NicaHistogramManager_1_1D<TH1D>*)p);
   }
   static void destruct_NicaHistogramManager_1_1DlETH1DgR(void *p) {
      typedef ::NicaHistogramManager_1_1D<TH1D> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_1_1D<TH1D>

//______________________________________________________________________________
template <> void NicaHistogramManager_1_2D<TH2I>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_1_2D<TH2I>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_1_2D<TH2I>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_1_2D<TH2I>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_1_2DlETH2IgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_1_2D<TH2I> : new ::NicaHistogramManager_1_2D<TH2I>;
   }
   static void *newArray_NicaHistogramManager_1_2DlETH2IgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_1_2D<TH2I>[nElements] : new ::NicaHistogramManager_1_2D<TH2I>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_1_2DlETH2IgR(void *p) {
      delete ((::NicaHistogramManager_1_2D<TH2I>*)p);
   }
   static void deleteArray_NicaHistogramManager_1_2DlETH2IgR(void *p) {
      delete [] ((::NicaHistogramManager_1_2D<TH2I>*)p);
   }
   static void destruct_NicaHistogramManager_1_2DlETH2IgR(void *p) {
      typedef ::NicaHistogramManager_1_2D<TH2I> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_1_2D<TH2I>

//______________________________________________________________________________
template <> void NicaHistogramManager_1_2D<TH2F>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_1_2D<TH2F>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_1_2D<TH2F>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_1_2D<TH2F>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_1_2DlETH2FgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_1_2D<TH2F> : new ::NicaHistogramManager_1_2D<TH2F>;
   }
   static void *newArray_NicaHistogramManager_1_2DlETH2FgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_1_2D<TH2F>[nElements] : new ::NicaHistogramManager_1_2D<TH2F>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_1_2DlETH2FgR(void *p) {
      delete ((::NicaHistogramManager_1_2D<TH2F>*)p);
   }
   static void deleteArray_NicaHistogramManager_1_2DlETH2FgR(void *p) {
      delete [] ((::NicaHistogramManager_1_2D<TH2F>*)p);
   }
   static void destruct_NicaHistogramManager_1_2DlETH2FgR(void *p) {
      typedef ::NicaHistogramManager_1_2D<TH2F> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_1_2D<TH2F>

//______________________________________________________________________________
template <> void NicaHistogramManager_1_2D<TH2D>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_1_2D<TH2D>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_1_2D<TH2D>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_1_2D<TH2D>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_1_2DlETH2DgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_1_2D<TH2D> : new ::NicaHistogramManager_1_2D<TH2D>;
   }
   static void *newArray_NicaHistogramManager_1_2DlETH2DgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_1_2D<TH2D>[nElements] : new ::NicaHistogramManager_1_2D<TH2D>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_1_2DlETH2DgR(void *p) {
      delete ((::NicaHistogramManager_1_2D<TH2D>*)p);
   }
   static void deleteArray_NicaHistogramManager_1_2DlETH2DgR(void *p) {
      delete [] ((::NicaHistogramManager_1_2D<TH2D>*)p);
   }
   static void destruct_NicaHistogramManager_1_2DlETH2DgR(void *p) {
      typedef ::NicaHistogramManager_1_2D<TH2D> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_1_2D<TH2D>

//______________________________________________________________________________
template <> void NicaHistogramManager_1_3D<TH3I>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_1_3D<TH3I>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_1_3D<TH3I>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_1_3D<TH3I>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_1_3DlETH3IgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_1_3D<TH3I> : new ::NicaHistogramManager_1_3D<TH3I>;
   }
   static void *newArray_NicaHistogramManager_1_3DlETH3IgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_1_3D<TH3I>[nElements] : new ::NicaHistogramManager_1_3D<TH3I>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_1_3DlETH3IgR(void *p) {
      delete ((::NicaHistogramManager_1_3D<TH3I>*)p);
   }
   static void deleteArray_NicaHistogramManager_1_3DlETH3IgR(void *p) {
      delete [] ((::NicaHistogramManager_1_3D<TH3I>*)p);
   }
   static void destruct_NicaHistogramManager_1_3DlETH3IgR(void *p) {
      typedef ::NicaHistogramManager_1_3D<TH3I> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_1_3D<TH3I>

//______________________________________________________________________________
template <> void NicaHistogramManager_1_3D<TH3F>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_1_3D<TH3F>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_1_3D<TH3F>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_1_3D<TH3F>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_1_3DlETH3FgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_1_3D<TH3F> : new ::NicaHistogramManager_1_3D<TH3F>;
   }
   static void *newArray_NicaHistogramManager_1_3DlETH3FgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_1_3D<TH3F>[nElements] : new ::NicaHistogramManager_1_3D<TH3F>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_1_3DlETH3FgR(void *p) {
      delete ((::NicaHistogramManager_1_3D<TH3F>*)p);
   }
   static void deleteArray_NicaHistogramManager_1_3DlETH3FgR(void *p) {
      delete [] ((::NicaHistogramManager_1_3D<TH3F>*)p);
   }
   static void destruct_NicaHistogramManager_1_3DlETH3FgR(void *p) {
      typedef ::NicaHistogramManager_1_3D<TH3F> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_1_3D<TH3F>

//______________________________________________________________________________
template <> void NicaHistogramManager_1_3D<TH3D>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_1_3D<TH3D>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_1_3D<TH3D>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_1_3D<TH3D>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_1_3DlETH3DgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_1_3D<TH3D> : new ::NicaHistogramManager_1_3D<TH3D>;
   }
   static void *newArray_NicaHistogramManager_1_3DlETH3DgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_1_3D<TH3D>[nElements] : new ::NicaHistogramManager_1_3D<TH3D>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_1_3DlETH3DgR(void *p) {
      delete ((::NicaHistogramManager_1_3D<TH3D>*)p);
   }
   static void deleteArray_NicaHistogramManager_1_3DlETH3DgR(void *p) {
      delete [] ((::NicaHistogramManager_1_3D<TH3D>*)p);
   }
   static void destruct_NicaHistogramManager_1_3DlETH3DgR(void *p) {
      typedef ::NicaHistogramManager_1_3D<TH3D> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_1_3D<TH3D>

//______________________________________________________________________________
template <> void NicaHistogramManager_2_1D<TH1I>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_2_1D<TH1I>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_2_1D<TH1I>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_2_1D<TH1I>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_2_1DlETH1IgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_2_1D<TH1I> : new ::NicaHistogramManager_2_1D<TH1I>;
   }
   static void *newArray_NicaHistogramManager_2_1DlETH1IgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_2_1D<TH1I>[nElements] : new ::NicaHistogramManager_2_1D<TH1I>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_2_1DlETH1IgR(void *p) {
      delete ((::NicaHistogramManager_2_1D<TH1I>*)p);
   }
   static void deleteArray_NicaHistogramManager_2_1DlETH1IgR(void *p) {
      delete [] ((::NicaHistogramManager_2_1D<TH1I>*)p);
   }
   static void destruct_NicaHistogramManager_2_1DlETH1IgR(void *p) {
      typedef ::NicaHistogramManager_2_1D<TH1I> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_2_1D<TH1I>

//______________________________________________________________________________
template <> void NicaHistogramManager_2_1D<TH1F>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_2_1D<TH1F>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_2_1D<TH1F>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_2_1D<TH1F>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_2_1DlETH1FgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_2_1D<TH1F> : new ::NicaHistogramManager_2_1D<TH1F>;
   }
   static void *newArray_NicaHistogramManager_2_1DlETH1FgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_2_1D<TH1F>[nElements] : new ::NicaHistogramManager_2_1D<TH1F>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_2_1DlETH1FgR(void *p) {
      delete ((::NicaHistogramManager_2_1D<TH1F>*)p);
   }
   static void deleteArray_NicaHistogramManager_2_1DlETH1FgR(void *p) {
      delete [] ((::NicaHistogramManager_2_1D<TH1F>*)p);
   }
   static void destruct_NicaHistogramManager_2_1DlETH1FgR(void *p) {
      typedef ::NicaHistogramManager_2_1D<TH1F> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_2_1D<TH1F>

//______________________________________________________________________________
template <> void NicaHistogramManager_2_1D<TH1D>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_2_1D<TH1D>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_2_1D<TH1D>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_2_1D<TH1D>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_2_1DlETH1DgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_2_1D<TH1D> : new ::NicaHistogramManager_2_1D<TH1D>;
   }
   static void *newArray_NicaHistogramManager_2_1DlETH1DgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_2_1D<TH1D>[nElements] : new ::NicaHistogramManager_2_1D<TH1D>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_2_1DlETH1DgR(void *p) {
      delete ((::NicaHistogramManager_2_1D<TH1D>*)p);
   }
   static void deleteArray_NicaHistogramManager_2_1DlETH1DgR(void *p) {
      delete [] ((::NicaHistogramManager_2_1D<TH1D>*)p);
   }
   static void destruct_NicaHistogramManager_2_1DlETH1DgR(void *p) {
      typedef ::NicaHistogramManager_2_1D<TH1D> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_2_1D<TH1D>

//______________________________________________________________________________
template <> void NicaHistogramManager_2_2D<TH2I>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_2_2D<TH2I>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_2_2D<TH2I>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_2_2D<TH2I>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_2_2DlETH2IgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_2_2D<TH2I> : new ::NicaHistogramManager_2_2D<TH2I>;
   }
   static void *newArray_NicaHistogramManager_2_2DlETH2IgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_2_2D<TH2I>[nElements] : new ::NicaHistogramManager_2_2D<TH2I>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_2_2DlETH2IgR(void *p) {
      delete ((::NicaHistogramManager_2_2D<TH2I>*)p);
   }
   static void deleteArray_NicaHistogramManager_2_2DlETH2IgR(void *p) {
      delete [] ((::NicaHistogramManager_2_2D<TH2I>*)p);
   }
   static void destruct_NicaHistogramManager_2_2DlETH2IgR(void *p) {
      typedef ::NicaHistogramManager_2_2D<TH2I> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_2_2D<TH2I>

//______________________________________________________________________________
template <> void NicaHistogramManager_2_2D<TH2F>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_2_2D<TH2F>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_2_2D<TH2F>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_2_2D<TH2F>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_2_2DlETH2FgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_2_2D<TH2F> : new ::NicaHistogramManager_2_2D<TH2F>;
   }
   static void *newArray_NicaHistogramManager_2_2DlETH2FgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_2_2D<TH2F>[nElements] : new ::NicaHistogramManager_2_2D<TH2F>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_2_2DlETH2FgR(void *p) {
      delete ((::NicaHistogramManager_2_2D<TH2F>*)p);
   }
   static void deleteArray_NicaHistogramManager_2_2DlETH2FgR(void *p) {
      delete [] ((::NicaHistogramManager_2_2D<TH2F>*)p);
   }
   static void destruct_NicaHistogramManager_2_2DlETH2FgR(void *p) {
      typedef ::NicaHistogramManager_2_2D<TH2F> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_2_2D<TH2F>

//______________________________________________________________________________
template <> void NicaHistogramManager_2_2D<TH2D>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_2_2D<TH2D>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_2_2D<TH2D>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_2_2D<TH2D>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_2_2DlETH2DgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_2_2D<TH2D> : new ::NicaHistogramManager_2_2D<TH2D>;
   }
   static void *newArray_NicaHistogramManager_2_2DlETH2DgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_2_2D<TH2D>[nElements] : new ::NicaHistogramManager_2_2D<TH2D>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_2_2DlETH2DgR(void *p) {
      delete ((::NicaHistogramManager_2_2D<TH2D>*)p);
   }
   static void deleteArray_NicaHistogramManager_2_2DlETH2DgR(void *p) {
      delete [] ((::NicaHistogramManager_2_2D<TH2D>*)p);
   }
   static void destruct_NicaHistogramManager_2_2DlETH2DgR(void *p) {
      typedef ::NicaHistogramManager_2_2D<TH2D> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_2_2D<TH2D>

//______________________________________________________________________________
template <> void NicaHistogramManager_2_3D<TH3I>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_2_3D<TH3I>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_2_3D<TH3I>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_2_3D<TH3I>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_2_3DlETH3IgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_2_3D<TH3I> : new ::NicaHistogramManager_2_3D<TH3I>;
   }
   static void *newArray_NicaHistogramManager_2_3DlETH3IgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_2_3D<TH3I>[nElements] : new ::NicaHistogramManager_2_3D<TH3I>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_2_3DlETH3IgR(void *p) {
      delete ((::NicaHistogramManager_2_3D<TH3I>*)p);
   }
   static void deleteArray_NicaHistogramManager_2_3DlETH3IgR(void *p) {
      delete [] ((::NicaHistogramManager_2_3D<TH3I>*)p);
   }
   static void destruct_NicaHistogramManager_2_3DlETH3IgR(void *p) {
      typedef ::NicaHistogramManager_2_3D<TH3I> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_2_3D<TH3I>

//______________________________________________________________________________
template <> void NicaHistogramManager_2_3D<TH3F>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_2_3D<TH3F>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_2_3D<TH3F>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_2_3D<TH3F>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_2_3DlETH3FgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_2_3D<TH3F> : new ::NicaHistogramManager_2_3D<TH3F>;
   }
   static void *newArray_NicaHistogramManager_2_3DlETH3FgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_2_3D<TH3F>[nElements] : new ::NicaHistogramManager_2_3D<TH3F>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_2_3DlETH3FgR(void *p) {
      delete ((::NicaHistogramManager_2_3D<TH3F>*)p);
   }
   static void deleteArray_NicaHistogramManager_2_3DlETH3FgR(void *p) {
      delete [] ((::NicaHistogramManager_2_3D<TH3F>*)p);
   }
   static void destruct_NicaHistogramManager_2_3DlETH3FgR(void *p) {
      typedef ::NicaHistogramManager_2_3D<TH3F> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_2_3D<TH3F>

//______________________________________________________________________________
template <> void NicaHistogramManager_2_3D<TH3D>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_2_3D<TH3D>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_2_3D<TH3D>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_2_3D<TH3D>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_2_3DlETH3DgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_2_3D<TH3D> : new ::NicaHistogramManager_2_3D<TH3D>;
   }
   static void *newArray_NicaHistogramManager_2_3DlETH3DgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_2_3D<TH3D>[nElements] : new ::NicaHistogramManager_2_3D<TH3D>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_2_3DlETH3DgR(void *p) {
      delete ((::NicaHistogramManager_2_3D<TH3D>*)p);
   }
   static void deleteArray_NicaHistogramManager_2_3DlETH3DgR(void *p) {
      delete [] ((::NicaHistogramManager_2_3D<TH3D>*)p);
   }
   static void destruct_NicaHistogramManager_2_3DlETH3DgR(void *p) {
      typedef ::NicaHistogramManager_2_3D<TH3D> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_2_3D<TH3D>

//______________________________________________________________________________
template <> void NicaHistogramManager_3_1D<TH1I>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_3_1D<TH1I>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_3_1D<TH1I>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_3_1D<TH1I>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_3_1DlETH1IgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_3_1D<TH1I> : new ::NicaHistogramManager_3_1D<TH1I>;
   }
   static void *newArray_NicaHistogramManager_3_1DlETH1IgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_3_1D<TH1I>[nElements] : new ::NicaHistogramManager_3_1D<TH1I>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_3_1DlETH1IgR(void *p) {
      delete ((::NicaHistogramManager_3_1D<TH1I>*)p);
   }
   static void deleteArray_NicaHistogramManager_3_1DlETH1IgR(void *p) {
      delete [] ((::NicaHistogramManager_3_1D<TH1I>*)p);
   }
   static void destruct_NicaHistogramManager_3_1DlETH1IgR(void *p) {
      typedef ::NicaHistogramManager_3_1D<TH1I> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_3_1D<TH1I>

//______________________________________________________________________________
template <> void NicaHistogramManager_3_1D<TH1F>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_3_1D<TH1F>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_3_1D<TH1F>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_3_1D<TH1F>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_3_1DlETH1FgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_3_1D<TH1F> : new ::NicaHistogramManager_3_1D<TH1F>;
   }
   static void *newArray_NicaHistogramManager_3_1DlETH1FgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_3_1D<TH1F>[nElements] : new ::NicaHistogramManager_3_1D<TH1F>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_3_1DlETH1FgR(void *p) {
      delete ((::NicaHistogramManager_3_1D<TH1F>*)p);
   }
   static void deleteArray_NicaHistogramManager_3_1DlETH1FgR(void *p) {
      delete [] ((::NicaHistogramManager_3_1D<TH1F>*)p);
   }
   static void destruct_NicaHistogramManager_3_1DlETH1FgR(void *p) {
      typedef ::NicaHistogramManager_3_1D<TH1F> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_3_1D<TH1F>

//______________________________________________________________________________
template <> void NicaHistogramManager_3_1D<TH1D>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_3_1D<TH1D>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_3_1D<TH1D>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_3_1D<TH1D>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_3_1DlETH1DgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_3_1D<TH1D> : new ::NicaHistogramManager_3_1D<TH1D>;
   }
   static void *newArray_NicaHistogramManager_3_1DlETH1DgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_3_1D<TH1D>[nElements] : new ::NicaHistogramManager_3_1D<TH1D>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_3_1DlETH1DgR(void *p) {
      delete ((::NicaHistogramManager_3_1D<TH1D>*)p);
   }
   static void deleteArray_NicaHistogramManager_3_1DlETH1DgR(void *p) {
      delete [] ((::NicaHistogramManager_3_1D<TH1D>*)p);
   }
   static void destruct_NicaHistogramManager_3_1DlETH1DgR(void *p) {
      typedef ::NicaHistogramManager_3_1D<TH1D> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_3_1D<TH1D>

//______________________________________________________________________________
template <> void NicaHistogramManager_3_2D<TH2I>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_3_2D<TH2I>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_3_2D<TH2I>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_3_2D<TH2I>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_3_2DlETH2IgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_3_2D<TH2I> : new ::NicaHistogramManager_3_2D<TH2I>;
   }
   static void *newArray_NicaHistogramManager_3_2DlETH2IgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_3_2D<TH2I>[nElements] : new ::NicaHistogramManager_3_2D<TH2I>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_3_2DlETH2IgR(void *p) {
      delete ((::NicaHistogramManager_3_2D<TH2I>*)p);
   }
   static void deleteArray_NicaHistogramManager_3_2DlETH2IgR(void *p) {
      delete [] ((::NicaHistogramManager_3_2D<TH2I>*)p);
   }
   static void destruct_NicaHistogramManager_3_2DlETH2IgR(void *p) {
      typedef ::NicaHistogramManager_3_2D<TH2I> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_3_2D<TH2I>

//______________________________________________________________________________
template <> void NicaHistogramManager_3_2D<TH2F>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_3_2D<TH2F>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_3_2D<TH2F>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_3_2D<TH2F>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_3_2DlETH2FgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_3_2D<TH2F> : new ::NicaHistogramManager_3_2D<TH2F>;
   }
   static void *newArray_NicaHistogramManager_3_2DlETH2FgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_3_2D<TH2F>[nElements] : new ::NicaHistogramManager_3_2D<TH2F>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_3_2DlETH2FgR(void *p) {
      delete ((::NicaHistogramManager_3_2D<TH2F>*)p);
   }
   static void deleteArray_NicaHistogramManager_3_2DlETH2FgR(void *p) {
      delete [] ((::NicaHistogramManager_3_2D<TH2F>*)p);
   }
   static void destruct_NicaHistogramManager_3_2DlETH2FgR(void *p) {
      typedef ::NicaHistogramManager_3_2D<TH2F> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_3_2D<TH2F>

//______________________________________________________________________________
template <> void NicaHistogramManager_3_2D<TH2D>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_3_2D<TH2D>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_3_2D<TH2D>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_3_2D<TH2D>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_3_2DlETH2DgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_3_2D<TH2D> : new ::NicaHistogramManager_3_2D<TH2D>;
   }
   static void *newArray_NicaHistogramManager_3_2DlETH2DgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_3_2D<TH2D>[nElements] : new ::NicaHistogramManager_3_2D<TH2D>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_3_2DlETH2DgR(void *p) {
      delete ((::NicaHistogramManager_3_2D<TH2D>*)p);
   }
   static void deleteArray_NicaHistogramManager_3_2DlETH2DgR(void *p) {
      delete [] ((::NicaHistogramManager_3_2D<TH2D>*)p);
   }
   static void destruct_NicaHistogramManager_3_2DlETH2DgR(void *p) {
      typedef ::NicaHistogramManager_3_2D<TH2D> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_3_2D<TH2D>

//______________________________________________________________________________
template <> void NicaHistogramManager_3_3D<TH3I>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_3_3D<TH3I>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_3_3D<TH3I>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_3_3D<TH3I>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_3_3DlETH3IgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_3_3D<TH3I> : new ::NicaHistogramManager_3_3D<TH3I>;
   }
   static void *newArray_NicaHistogramManager_3_3DlETH3IgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_3_3D<TH3I>[nElements] : new ::NicaHistogramManager_3_3D<TH3I>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_3_3DlETH3IgR(void *p) {
      delete ((::NicaHistogramManager_3_3D<TH3I>*)p);
   }
   static void deleteArray_NicaHistogramManager_3_3DlETH3IgR(void *p) {
      delete [] ((::NicaHistogramManager_3_3D<TH3I>*)p);
   }
   static void destruct_NicaHistogramManager_3_3DlETH3IgR(void *p) {
      typedef ::NicaHistogramManager_3_3D<TH3I> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_3_3D<TH3I>

//______________________________________________________________________________
template <> void NicaHistogramManager_3_3D<TH3F>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_3_3D<TH3F>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_3_3D<TH3F>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_3_3D<TH3F>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_3_3DlETH3FgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_3_3D<TH3F> : new ::NicaHistogramManager_3_3D<TH3F>;
   }
   static void *newArray_NicaHistogramManager_3_3DlETH3FgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_3_3D<TH3F>[nElements] : new ::NicaHistogramManager_3_3D<TH3F>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_3_3DlETH3FgR(void *p) {
      delete ((::NicaHistogramManager_3_3D<TH3F>*)p);
   }
   static void deleteArray_NicaHistogramManager_3_3DlETH3FgR(void *p) {
      delete [] ((::NicaHistogramManager_3_3D<TH3F>*)p);
   }
   static void destruct_NicaHistogramManager_3_3DlETH3FgR(void *p) {
      typedef ::NicaHistogramManager_3_3D<TH3F> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_3_3D<TH3F>

//______________________________________________________________________________
template <> void NicaHistogramManager_3_3D<TH3D>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_3_3D<TH3D>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_3_3D<TH3D>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_3_3D<TH3D>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_3_3DlETH3DgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_3_3D<TH3D> : new ::NicaHistogramManager_3_3D<TH3D>;
   }
   static void *newArray_NicaHistogramManager_3_3DlETH3DgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_3_3D<TH3D>[nElements] : new ::NicaHistogramManager_3_3D<TH3D>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_3_3DlETH3DgR(void *p) {
      delete ((::NicaHistogramManager_3_3D<TH3D>*)p);
   }
   static void deleteArray_NicaHistogramManager_3_3DlETH3DgR(void *p) {
      delete [] ((::NicaHistogramManager_3_3D<TH3D>*)p);
   }
   static void destruct_NicaHistogramManager_3_3DlETH3DgR(void *p) {
      typedef ::NicaHistogramManager_3_3D<TH3D> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_3_3D<TH3D>

//______________________________________________________________________________
template <> void NicaHistogramManager_4_1D<TH1I>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_4_1D<TH1I>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_4_1D<TH1I>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_4_1D<TH1I>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_4_1DlETH1IgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_4_1D<TH1I> : new ::NicaHistogramManager_4_1D<TH1I>;
   }
   static void *newArray_NicaHistogramManager_4_1DlETH1IgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_4_1D<TH1I>[nElements] : new ::NicaHistogramManager_4_1D<TH1I>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_4_1DlETH1IgR(void *p) {
      delete ((::NicaHistogramManager_4_1D<TH1I>*)p);
   }
   static void deleteArray_NicaHistogramManager_4_1DlETH1IgR(void *p) {
      delete [] ((::NicaHistogramManager_4_1D<TH1I>*)p);
   }
   static void destruct_NicaHistogramManager_4_1DlETH1IgR(void *p) {
      typedef ::NicaHistogramManager_4_1D<TH1I> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_4_1D<TH1I>

//______________________________________________________________________________
template <> void NicaHistogramManager_4_1D<TH1F>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_4_1D<TH1F>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_4_1D<TH1F>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_4_1D<TH1F>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_4_1DlETH1FgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_4_1D<TH1F> : new ::NicaHistogramManager_4_1D<TH1F>;
   }
   static void *newArray_NicaHistogramManager_4_1DlETH1FgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_4_1D<TH1F>[nElements] : new ::NicaHistogramManager_4_1D<TH1F>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_4_1DlETH1FgR(void *p) {
      delete ((::NicaHistogramManager_4_1D<TH1F>*)p);
   }
   static void deleteArray_NicaHistogramManager_4_1DlETH1FgR(void *p) {
      delete [] ((::NicaHistogramManager_4_1D<TH1F>*)p);
   }
   static void destruct_NicaHistogramManager_4_1DlETH1FgR(void *p) {
      typedef ::NicaHistogramManager_4_1D<TH1F> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_4_1D<TH1F>

//______________________________________________________________________________
template <> void NicaHistogramManager_4_1D<TH1D>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_4_1D<TH1D>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_4_1D<TH1D>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_4_1D<TH1D>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_4_1DlETH1DgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_4_1D<TH1D> : new ::NicaHistogramManager_4_1D<TH1D>;
   }
   static void *newArray_NicaHistogramManager_4_1DlETH1DgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_4_1D<TH1D>[nElements] : new ::NicaHistogramManager_4_1D<TH1D>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_4_1DlETH1DgR(void *p) {
      delete ((::NicaHistogramManager_4_1D<TH1D>*)p);
   }
   static void deleteArray_NicaHistogramManager_4_1DlETH1DgR(void *p) {
      delete [] ((::NicaHistogramManager_4_1D<TH1D>*)p);
   }
   static void destruct_NicaHistogramManager_4_1DlETH1DgR(void *p) {
      typedef ::NicaHistogramManager_4_1D<TH1D> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_4_1D<TH1D>

//______________________________________________________________________________
template <> void NicaHistogramManager_4_2D<TH2I>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_4_2D<TH2I>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_4_2D<TH2I>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_4_2D<TH2I>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_4_2DlETH2IgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_4_2D<TH2I> : new ::NicaHistogramManager_4_2D<TH2I>;
   }
   static void *newArray_NicaHistogramManager_4_2DlETH2IgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_4_2D<TH2I>[nElements] : new ::NicaHistogramManager_4_2D<TH2I>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_4_2DlETH2IgR(void *p) {
      delete ((::NicaHistogramManager_4_2D<TH2I>*)p);
   }
   static void deleteArray_NicaHistogramManager_4_2DlETH2IgR(void *p) {
      delete [] ((::NicaHistogramManager_4_2D<TH2I>*)p);
   }
   static void destruct_NicaHistogramManager_4_2DlETH2IgR(void *p) {
      typedef ::NicaHistogramManager_4_2D<TH2I> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_4_2D<TH2I>

//______________________________________________________________________________
template <> void NicaHistogramManager_4_2D<TH2F>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_4_2D<TH2F>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_4_2D<TH2F>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_4_2D<TH2F>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_4_2DlETH2FgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_4_2D<TH2F> : new ::NicaHistogramManager_4_2D<TH2F>;
   }
   static void *newArray_NicaHistogramManager_4_2DlETH2FgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_4_2D<TH2F>[nElements] : new ::NicaHistogramManager_4_2D<TH2F>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_4_2DlETH2FgR(void *p) {
      delete ((::NicaHistogramManager_4_2D<TH2F>*)p);
   }
   static void deleteArray_NicaHistogramManager_4_2DlETH2FgR(void *p) {
      delete [] ((::NicaHistogramManager_4_2D<TH2F>*)p);
   }
   static void destruct_NicaHistogramManager_4_2DlETH2FgR(void *p) {
      typedef ::NicaHistogramManager_4_2D<TH2F> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_4_2D<TH2F>

//______________________________________________________________________________
template <> void NicaHistogramManager_4_2D<TH2D>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_4_2D<TH2D>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_4_2D<TH2D>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_4_2D<TH2D>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_4_2DlETH2DgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_4_2D<TH2D> : new ::NicaHistogramManager_4_2D<TH2D>;
   }
   static void *newArray_NicaHistogramManager_4_2DlETH2DgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_4_2D<TH2D>[nElements] : new ::NicaHistogramManager_4_2D<TH2D>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_4_2DlETH2DgR(void *p) {
      delete ((::NicaHistogramManager_4_2D<TH2D>*)p);
   }
   static void deleteArray_NicaHistogramManager_4_2DlETH2DgR(void *p) {
      delete [] ((::NicaHistogramManager_4_2D<TH2D>*)p);
   }
   static void destruct_NicaHistogramManager_4_2DlETH2DgR(void *p) {
      typedef ::NicaHistogramManager_4_2D<TH2D> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_4_2D<TH2D>

//______________________________________________________________________________
template <> void NicaHistogramManager_4_3D<TH3I>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_4_3D<TH3I>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_4_3D<TH3I>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_4_3D<TH3I>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_4_3DlETH3IgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_4_3D<TH3I> : new ::NicaHistogramManager_4_3D<TH3I>;
   }
   static void *newArray_NicaHistogramManager_4_3DlETH3IgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_4_3D<TH3I>[nElements] : new ::NicaHistogramManager_4_3D<TH3I>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_4_3DlETH3IgR(void *p) {
      delete ((::NicaHistogramManager_4_3D<TH3I>*)p);
   }
   static void deleteArray_NicaHistogramManager_4_3DlETH3IgR(void *p) {
      delete [] ((::NicaHistogramManager_4_3D<TH3I>*)p);
   }
   static void destruct_NicaHistogramManager_4_3DlETH3IgR(void *p) {
      typedef ::NicaHistogramManager_4_3D<TH3I> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_4_3D<TH3I>

//______________________________________________________________________________
template <> void NicaHistogramManager_4_3D<TH3F>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_4_3D<TH3F>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_4_3D<TH3F>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_4_3D<TH3F>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_4_3DlETH3FgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_4_3D<TH3F> : new ::NicaHistogramManager_4_3D<TH3F>;
   }
   static void *newArray_NicaHistogramManager_4_3DlETH3FgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_4_3D<TH3F>[nElements] : new ::NicaHistogramManager_4_3D<TH3F>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_4_3DlETH3FgR(void *p) {
      delete ((::NicaHistogramManager_4_3D<TH3F>*)p);
   }
   static void deleteArray_NicaHistogramManager_4_3DlETH3FgR(void *p) {
      delete [] ((::NicaHistogramManager_4_3D<TH3F>*)p);
   }
   static void destruct_NicaHistogramManager_4_3DlETH3FgR(void *p) {
      typedef ::NicaHistogramManager_4_3D<TH3F> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_4_3D<TH3F>

//______________________________________________________________________________
template <> void NicaHistogramManager_4_3D<TH3D>::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaHistogramManager_4_3D<TH3D>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaHistogramManager_4_3D<TH3D>::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaHistogramManager_4_3D<TH3D>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaHistogramManager_4_3DlETH3DgR(void *p) {
      return  p ? new(p) ::NicaHistogramManager_4_3D<TH3D> : new ::NicaHistogramManager_4_3D<TH3D>;
   }
   static void *newArray_NicaHistogramManager_4_3DlETH3DgR(Long_t nElements, void *p) {
      return p ? new(p) ::NicaHistogramManager_4_3D<TH3D>[nElements] : new ::NicaHistogramManager_4_3D<TH3D>[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaHistogramManager_4_3DlETH3DgR(void *p) {
      delete ((::NicaHistogramManager_4_3D<TH3D>*)p);
   }
   static void deleteArray_NicaHistogramManager_4_3DlETH3DgR(void *p) {
      delete [] ((::NicaHistogramManager_4_3D<TH3D>*)p);
   }
   static void destruct_NicaHistogramManager_4_3DlETH3DgR(void *p) {
      typedef ::NicaHistogramManager_4_3D<TH3D> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaHistogramManager_4_3D<TH3D>

//______________________________________________________________________________
void NicaMultiGraph::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaMultiGraph.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaMultiGraph::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaMultiGraph::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaMultiGraph(void *p) {
      return  p ? new(p) ::NicaMultiGraph : new ::NicaMultiGraph;
   }
   static void *newArray_NicaMultiGraph(Long_t nElements, void *p) {
      return p ? new(p) ::NicaMultiGraph[nElements] : new ::NicaMultiGraph[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaMultiGraph(void *p) {
      delete ((::NicaMultiGraph*)p);
   }
   static void deleteArray_NicaMultiGraph(void *p) {
      delete [] ((::NicaMultiGraph*)p);
   }
   static void destruct_NicaMultiGraph(void *p) {
      typedef ::NicaMultiGraph current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaMultiGraph

//______________________________________________________________________________
void NicaSpline1D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaSpline1D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaSpline1D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaSpline1D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaSpline1D(void *p) {
      return  p ? new(p) ::NicaSpline1D : new ::NicaSpline1D;
   }
   static void *newArray_NicaSpline1D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaSpline1D[nElements] : new ::NicaSpline1D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaSpline1D(void *p) {
      delete ((::NicaSpline1D*)p);
   }
   static void deleteArray_NicaSpline1D(void *p) {
      delete [] ((::NicaSpline1D*)p);
   }
   static void destruct_NicaSpline1D(void *p) {
      typedef ::NicaSpline1D current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaSpline1D

//______________________________________________________________________________
void NicaSpline2D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaSpline2D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaSpline2D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaSpline2D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaSpline2D(void *p) {
      return  p ? new(p) ::NicaSpline2D : new ::NicaSpline2D;
   }
   static void *newArray_NicaSpline2D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaSpline2D[nElements] : new ::NicaSpline2D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaSpline2D(void *p) {
      delete ((::NicaSpline2D*)p);
   }
   static void deleteArray_NicaSpline2D(void *p) {
      delete [] ((::NicaSpline2D*)p);
   }
   static void destruct_NicaSpline2D(void *p) {
      typedef ::NicaSpline2D current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaSpline2D

//______________________________________________________________________________
void NicaSpline3D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaSpline3D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaSpline3D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaSpline3D::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaSpline3D(void *p) {
      return  p ? new(p) ::NicaSpline3D : new ::NicaSpline3D;
   }
   static void *newArray_NicaSpline3D(Long_t nElements, void *p) {
      return p ? new(p) ::NicaSpline3D[nElements] : new ::NicaSpline3D[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaSpline3D(void *p) {
      delete ((::NicaSpline3D*)p);
   }
   static void deleteArray_NicaSpline3D(void *p) {
      delete [] ((::NicaSpline3D*)p);
   }
   static void destruct_NicaSpline3D(void *p) {
      typedef ::NicaSpline3D current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaSpline3D

//______________________________________________________________________________
void NicaChiSqMap2D::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaChiSqMap2D.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaChiSqMap2D::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaChiSqMap2D::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaChiSqMap2D(void *p) {
      delete ((::NicaChiSqMap2D*)p);
   }
   static void deleteArray_NicaChiSqMap2D(void *p) {
      delete [] ((::NicaChiSqMap2D*)p);
   }
   static void destruct_NicaChiSqMap2D(void *p) {
      typedef ::NicaChiSqMap2D current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaChiSqMap2D(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaChiSqMap2D*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaChiSqMap2D

//______________________________________________________________________________
void NicaFitParam::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaFitParam.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaFitParam::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaFitParam::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaFitParam(void *p) {
      return  p ? new(p) ::NicaFitParam : new ::NicaFitParam;
   }
   static void *newArray_NicaFitParam(Long_t nElements, void *p) {
      return p ? new(p) ::NicaFitParam[nElements] : new ::NicaFitParam[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaFitParam(void *p) {
      delete ((::NicaFitParam*)p);
   }
   static void deleteArray_NicaFitParam(void *p) {
      delete [] ((::NicaFitParam*)p);
   }
   static void destruct_NicaFitParam(void *p) {
      typedef ::NicaFitParam current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaFitParam

//______________________________________________________________________________
void NicaMinimizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaMinimizer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaMinimizer::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaMinimizer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaMinimizer(void *p) {
      return  p ? new(p) ::NicaMinimizer : new ::NicaMinimizer;
   }
   static void *newArray_NicaMinimizer(Long_t nElements, void *p) {
      return p ? new(p) ::NicaMinimizer[nElements] : new ::NicaMinimizer[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaMinimizer(void *p) {
      delete ((::NicaMinimizer*)p);
   }
   static void deleteArray_NicaMinimizer(void *p) {
      delete [] ((::NicaMinimizer*)p);
   }
   static void destruct_NicaMinimizer(void *p) {
      typedef ::NicaMinimizer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaMinimizer

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
                  &vectorlETStringgR_Dictionary, isa_proxy, 4,
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
                  &vectorlENicaFitParamgR_Dictionary, isa_proxy, 4,
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
  void TriggerDictionaryInitialization_G__NicaFeaturesDict_Impl() {
    static const char* headers[] = {
"NicaCout.h",
"NicaArray.h",
"NicaOptions.h",
"NicaParameter.h",
"NicaStd.h",
"NicaObjectMatrix.h",
"NicaHelix.h",
"NicaHelixBase.h",
"NicaLink.h",
"NicaConst.h",
"NicaXMLNode.h",
"NicaJobs.h",
"NicaPack.h",
"NicaPackage.h",
"NicaPackage2HTML.h",
"NicaAnaFile.h",
"NicaHtmlCore.h",
"NicaPackageSql.h",
"NicaHtmlDiv.h",
"NicaHtmlFile.h",
"NicaHtmlObject.h",
"NicaHtmlTable.h",
"NicaDividedHisto.h",
"NicaHistogramManager.h",
"NicaMultiGraph.h",
"NicaSplines.h",
"NicaChiSqMap2D.h",
"NicaStdHist.h",
"NicaStdString.h",
"NicaStdMath.h",
"NicaFitParam.h",
"NicaMinimizer.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/hist",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io/html",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/std",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/minimizer",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/minimizer",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/features/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__NicaFeaturesDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$TString.h")))  __attribute__((annotate("$clingAutoload$NicaCout.h")))  TString;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$NicaFitParam.h")))  NicaFitParam;
class __attribute__((annotate("$clingAutoload$NicaCout.h")))  NicaCout;
template <typename T> class __attribute__((annotate("$clingAutoload$NicaArray.h")))  NicaArray_1;

template <typename T> class __attribute__((annotate("$clingAutoload$NicaArray.h")))  NicaArray_2;

template <typename T> class __attribute__((annotate("$clingAutoload$NicaArray.h")))  NicaArray_3;

template <typename T> class __attribute__((annotate("$clingAutoload$NicaArray.h")))  NicaArray_4;

class __attribute__((annotate("$clingAutoload$NicaOptions.h")))  NicaOptionArray;
class __attribute__((annotate("$clingAutoload$NicaOptions.h")))  NicaOptionConverter;
class __attribute__((annotate("$clingAutoload$NicaPack.h")))  __attribute__((annotate("$clingAutoload$NicaParameter.h")))  NicaPack;
class __attribute__((annotate("$clingAutoload$NicaParameter.h")))  NicaParameter;
class __attribute__((annotate("$clingAutoload$NicaParameter.h")))  NicaParameterInt;
class __attribute__((annotate("$clingAutoload$NicaParameter.h")))  NicaParameterDouble;
class __attribute__((annotate("$clingAutoload$NicaParameter.h")))  NicaParameterFloat;
class __attribute__((annotate("$clingAutoload$NicaParameter.h")))  NicaParameterString;
class __attribute__((annotate("$clingAutoload$NicaParameter.h")))  NicaParameterUInt;
class __attribute__((annotate("$clingAutoload$NicaParameter.h")))  NicaParameterULong;
class __attribute__((annotate("$clingAutoload$NicaParameter.h")))  NicaParameterULong64;
class __attribute__((annotate("$clingAutoload$NicaParameter.h")))  NicaParameterBool;
class __attribute__((annotate("$clingAutoload$NicaObjectMatrix.h")))  NicaObjectMatrix_1;
class __attribute__((annotate("$clingAutoload$NicaObjectMatrix.h")))  NicaObjectMatrix_2;
class __attribute__((annotate("$clingAutoload$NicaObjectMatrix.h")))  NicaObjectMatrix_3;
class __attribute__((annotate("$clingAutoload$NicaObjectMatrix.h")))  NicaObjectMatrix_4;
class __attribute__((annotate("$clingAutoload$NicaHelixBase.h")))  __attribute__((annotate("$clingAutoload$NicaHelix.h")))  NicaHelixBase;
class __attribute__((annotate("$clingAutoload$NicaHelix.h")))  NicaHelixZ;
class __attribute__((annotate("$clingAutoload$NicaHelix.h")))  NicaHelixX;
class __attribute__((annotate("$clingAutoload$NicaHelix.h")))  NicaHelixY;
class __attribute__((annotate("$clingAutoload$NicaLink.h")))  NicaLink;
class __attribute__((annotate("$clingAutoload$NicaConst.h")))  NicaConst;
class __attribute__((annotate("$clingAutoload$NicaXMLNode.h")))  NicaXMLAttrib;
class __attribute__((annotate("$clingAutoload$NicaXMLNode.h")))  NicaXMLNode;
class __attribute__((annotate("$clingAutoload$NicaXMLNode.h")))  NicaXMLFile;
class __attribute__((annotate("$clingAutoload$NicaJobs.h")))  NicaJobs;
class __attribute__((annotate("$clingAutoload$NicaPackage.h")))  NicaPackage;
class __attribute__((annotate("$clingAutoload$NicaPackage2HTML.h")))  NicaPackage2HTML;
class __attribute__((annotate("$clingAutoload$NicaAnaFile.h")))  NicaAnaFile;
class __attribute__((annotate("$clingAutoload$NicaHtmlCore.h")))  NicaHtmlCore;
class __attribute__((annotate("$clingAutoload$NicaPackageSql.h")))  NicaPackageSql;
class __attribute__((annotate("$clingAutoload$NicaHtmlObject.h")))  __attribute__((annotate("$clingAutoload$NicaHtmlDiv.h")))  NicaHtmlObject;
class __attribute__((annotate("$clingAutoload$NicaHtmlDiv.h")))  NicaHtmlDiv;
class __attribute__((annotate("$clingAutoload$NicaHtmlFile.h")))  NicaHtmlFile;
class __attribute__((annotate("$clingAutoload$NicaHtmlTable.h")))  NicaHtmlTable;
class __attribute__((annotate("$clingAutoload$NicaHtmlTable.h")))  NicaHtmlRow;
class __attribute__((annotate("$clingAutoload$NicaHtmlTable.h")))  NicaHtmlCell;
class __attribute__((annotate("$clingAutoload$NicaHtmlTable.h")))  NicaHtmlCellCol;
class __attribute__((annotate("$clingAutoload$NicaHtmlTable.h")))  NicaHtmlCellRow;
class __attribute__((annotate("$clingAutoload$NicaDividedHisto.h")))  NicaDividedHisto1D;
class __attribute__((annotate("$clingAutoload$NicaDividedHisto.h")))  NicaDividedHisto2D;
class __attribute__((annotate("$clingAutoload$NicaDividedHisto.h")))  NicaDividedHisto3D;
class __attribute__((annotate("$clingAutoload$NicaHistogramManager.h")))  NicaHistogramAxisConf;
class __attribute__((annotate("$clingAutoload$TH1.h")))  __attribute__((annotate("$clingAutoload$NicaDividedHisto.h")))  TH1I;
template <class T> class __attribute__((annotate("$clingAutoload$NicaHistogramManager.h")))  NicaHistogramManager_1_1D;

class __attribute__((annotate("$clingAutoload$TH1.h")))  __attribute__((annotate("$clingAutoload$NicaDividedHisto.h")))  TH1F;
class __attribute__((annotate("$clingAutoload$TH1.h")))  __attribute__((annotate("$clingAutoload$NicaDividedHisto.h")))  TH1D;
class __attribute__((annotate("$clingAutoload$TH2.h")))  __attribute__((annotate("$clingAutoload$NicaDividedHisto.h")))  TH2I;
template <class T> class __attribute__((annotate("$clingAutoload$NicaHistogramManager.h")))  NicaHistogramManager_1_2D;

class __attribute__((annotate("$clingAutoload$TH2.h")))  __attribute__((annotate("$clingAutoload$NicaDividedHisto.h")))  TH2F;
class __attribute__((annotate("$clingAutoload$TH2.h")))  __attribute__((annotate("$clingAutoload$NicaDividedHisto.h")))  TH2D;
class __attribute__((annotate("$clingAutoload$TH3.h")))  __attribute__((annotate("$clingAutoload$NicaDividedHisto.h")))  TH3I;
template <class T> class __attribute__((annotate("$clingAutoload$NicaHistogramManager.h")))  NicaHistogramManager_1_3D;

class __attribute__((annotate("$clingAutoload$TH3.h")))  __attribute__((annotate("$clingAutoload$NicaDividedHisto.h")))  TH3F;
class __attribute__((annotate("$clingAutoload$TH3.h")))  __attribute__((annotate("$clingAutoload$NicaDividedHisto.h")))  TH3D;
template <class T> class __attribute__((annotate("$clingAutoload$NicaHistogramManager.h")))  NicaHistogramManager_2_1D;

template <class T> class __attribute__((annotate("$clingAutoload$NicaHistogramManager.h")))  NicaHistogramManager_2_2D;

template <class T> class __attribute__((annotate("$clingAutoload$NicaHistogramManager.h")))  NicaHistogramManager_2_3D;

template <class T> class __attribute__((annotate("$clingAutoload$NicaHistogramManager.h")))  NicaHistogramManager_3_1D;

template <class T> class __attribute__((annotate("$clingAutoload$NicaHistogramManager.h")))  NicaHistogramManager_3_2D;

template <class T> class __attribute__((annotate("$clingAutoload$NicaHistogramManager.h")))  NicaHistogramManager_3_3D;

template <class T> class __attribute__((annotate("$clingAutoload$NicaHistogramManager.h")))  NicaHistogramManager_4_1D;

template <class T> class __attribute__((annotate("$clingAutoload$NicaHistogramManager.h")))  NicaHistogramManager_4_2D;

template <class T> class __attribute__((annotate("$clingAutoload$NicaHistogramManager.h")))  NicaHistogramManager_4_3D;

class __attribute__((annotate("$clingAutoload$NicaMultiGraph.h")))  NicaMultiGraph;
class __attribute__((annotate("$clingAutoload$NicaSplines.h")))  NicaSpline1D;
class __attribute__((annotate("$clingAutoload$NicaSplines.h")))  NicaSpline2D;
class __attribute__((annotate("$clingAutoload$NicaSplines.h")))  NicaSpline3D;
class __attribute__((annotate("$clingAutoload$NicaChiSqMap2D.h")))  NicaChiSqMap2D;
class __attribute__((annotate("$clingAutoload$NicaMinimizer.h")))  NicaMinimizer;
typedef NicaHelixZ NicaHelix __attribute__((annotate("$clingAutoload$NicaHelix.h"))) ;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__NicaFeaturesDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "NicaCout.h"
#include "NicaArray.h"
#include "NicaOptions.h"
#include "NicaParameter.h"
#include "NicaStd.h"
#include "NicaObjectMatrix.h"
#include "NicaHelix.h"
#include "NicaHelixBase.h"
#include "NicaLink.h"
#include "NicaConst.h"
#include "NicaXMLNode.h"
#include "NicaJobs.h"
#include "NicaPack.h"
#include "NicaPackage.h"
#include "NicaPackage2HTML.h"
#include "NicaAnaFile.h"
#include "NicaHtmlCore.h"
#include "NicaPackageSql.h"
#include "NicaHtmlDiv.h"
#include "NicaHtmlFile.h"
#include "NicaHtmlObject.h"
#include "NicaHtmlTable.h"
#include "NicaDividedHisto.h"
#include "NicaHistogramManager.h"
#include "NicaMultiGraph.h"
#include "NicaSplines.h"
#include "NicaChiSqMap2D.h"
#include "NicaStdHist.h"
#include "NicaStdString.h"
#include "NicaStdMath.h"
#include "NicaFitParam.h"
#include "NicaMinimizer.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"NicaAnaFile", payloadCode, "@",
"NicaArray_1<Bool_t>", payloadCode, "@",
"NicaArray_1<Double_t>", payloadCode, "@",
"NicaArray_1<Float_t>", payloadCode, "@",
"NicaArray_1<Int_t>", payloadCode, "@",
"NicaArray_1<Short_t>", payloadCode, "@",
"NicaArray_1<bool>", payloadCode, "@",
"NicaArray_1<double>", payloadCode, "@",
"NicaArray_1<float>", payloadCode, "@",
"NicaArray_1<int>", payloadCode, "@",
"NicaArray_1<short>", payloadCode, "@",
"NicaArray_2<Bool_t>", payloadCode, "@",
"NicaArray_2<Double_t>", payloadCode, "@",
"NicaArray_2<Float_t>", payloadCode, "@",
"NicaArray_2<Int_t>", payloadCode, "@",
"NicaArray_2<Short_t>", payloadCode, "@",
"NicaArray_2<bool>", payloadCode, "@",
"NicaArray_2<double>", payloadCode, "@",
"NicaArray_2<float>", payloadCode, "@",
"NicaArray_2<int>", payloadCode, "@",
"NicaArray_2<short>", payloadCode, "@",
"NicaArray_3<Bool_t>", payloadCode, "@",
"NicaArray_3<Double_t>", payloadCode, "@",
"NicaArray_3<Float_t>", payloadCode, "@",
"NicaArray_3<Int_t>", payloadCode, "@",
"NicaArray_3<Short_t>", payloadCode, "@",
"NicaArray_3<bool>", payloadCode, "@",
"NicaArray_3<double>", payloadCode, "@",
"NicaArray_3<float>", payloadCode, "@",
"NicaArray_3<int>", payloadCode, "@",
"NicaArray_3<short>", payloadCode, "@",
"NicaArray_4<Bool_t>", payloadCode, "@",
"NicaArray_4<Double_t>", payloadCode, "@",
"NicaArray_4<Float_t>", payloadCode, "@",
"NicaArray_4<Int_t>", payloadCode, "@",
"NicaArray_4<Short_t>", payloadCode, "@",
"NicaArray_4<bool>", payloadCode, "@",
"NicaArray_4<double>", payloadCode, "@",
"NicaArray_4<float>", payloadCode, "@",
"NicaArray_4<int>", payloadCode, "@",
"NicaArray_4<short>", payloadCode, "@",
"NicaChiSqMap2D", payloadCode, "@",
"NicaConst", payloadCode, "@",
"NicaCout", payloadCode, "@",
"NicaDividedHisto1D", payloadCode, "@",
"NicaDividedHisto2D", payloadCode, "@",
"NicaDividedHisto3D", payloadCode, "@",
"NicaFitParam", payloadCode, "@",
"NicaHelix", payloadCode, "@",
"NicaHelixBase", payloadCode, "@",
"NicaHelixX", payloadCode, "@",
"NicaHelixY", payloadCode, "@",
"NicaHelixZ", payloadCode, "@",
"NicaHistogramAxisConf", payloadCode, "@",
"NicaHistogramManager_1_1D<TH1D>", payloadCode, "@",
"NicaHistogramManager_1_1D<TH1F>", payloadCode, "@",
"NicaHistogramManager_1_1D<TH1I>", payloadCode, "@",
"NicaHistogramManager_1_2D<TH2D>", payloadCode, "@",
"NicaHistogramManager_1_2D<TH2F>", payloadCode, "@",
"NicaHistogramManager_1_2D<TH2I>", payloadCode, "@",
"NicaHistogramManager_1_3D<TH3D>", payloadCode, "@",
"NicaHistogramManager_1_3D<TH3F>", payloadCode, "@",
"NicaHistogramManager_1_3D<TH3I>", payloadCode, "@",
"NicaHistogramManager_2_1D<TH1D>", payloadCode, "@",
"NicaHistogramManager_2_1D<TH1F>", payloadCode, "@",
"NicaHistogramManager_2_1D<TH1I>", payloadCode, "@",
"NicaHistogramManager_2_2D<TH2D>", payloadCode, "@",
"NicaHistogramManager_2_2D<TH2F>", payloadCode, "@",
"NicaHistogramManager_2_2D<TH2I>", payloadCode, "@",
"NicaHistogramManager_2_3D<TH3D>", payloadCode, "@",
"NicaHistogramManager_2_3D<TH3F>", payloadCode, "@",
"NicaHistogramManager_2_3D<TH3I>", payloadCode, "@",
"NicaHistogramManager_3_1D<TH1D>", payloadCode, "@",
"NicaHistogramManager_3_1D<TH1F>", payloadCode, "@",
"NicaHistogramManager_3_1D<TH1I>", payloadCode, "@",
"NicaHistogramManager_3_2D<TH2D>", payloadCode, "@",
"NicaHistogramManager_3_2D<TH2F>", payloadCode, "@",
"NicaHistogramManager_3_2D<TH2I>", payloadCode, "@",
"NicaHistogramManager_3_3D<TH3D>", payloadCode, "@",
"NicaHistogramManager_3_3D<TH3F>", payloadCode, "@",
"NicaHistogramManager_3_3D<TH3I>", payloadCode, "@",
"NicaHistogramManager_4_1D<TH1D>", payloadCode, "@",
"NicaHistogramManager_4_1D<TH1F>", payloadCode, "@",
"NicaHistogramManager_4_1D<TH1I>", payloadCode, "@",
"NicaHistogramManager_4_2D<TH2D>", payloadCode, "@",
"NicaHistogramManager_4_2D<TH2F>", payloadCode, "@",
"NicaHistogramManager_4_2D<TH2I>", payloadCode, "@",
"NicaHistogramManager_4_3D<TH3D>", payloadCode, "@",
"NicaHistogramManager_4_3D<TH3F>", payloadCode, "@",
"NicaHistogramManager_4_3D<TH3I>", payloadCode, "@",
"NicaHtmlCell", payloadCode, "@",
"NicaHtmlCellCol", payloadCode, "@",
"NicaHtmlCellRow", payloadCode, "@",
"NicaHtmlCore", payloadCode, "@",
"NicaHtmlDiv", payloadCode, "@",
"NicaHtmlFile", payloadCode, "@",
"NicaHtmlObject", payloadCode, "@",
"NicaHtmlRow", payloadCode, "@",
"NicaHtmlTable", payloadCode, "@",
"NicaJobs", payloadCode, "@",
"NicaLink", payloadCode, "@",
"NicaMinimizer", payloadCode, "@",
"NicaMultiGraph", payloadCode, "@",
"NicaObjectMatrix_1", payloadCode, "@",
"NicaObjectMatrix_2", payloadCode, "@",
"NicaObjectMatrix_3", payloadCode, "@",
"NicaObjectMatrix_4", payloadCode, "@",
"NicaOptionArray", payloadCode, "@",
"NicaOptionConverter", payloadCode, "@",
"NicaPack", payloadCode, "@",
"NicaPackage", payloadCode, "@",
"NicaPackage2HTML", payloadCode, "@",
"NicaPackageSql", payloadCode, "@",
"NicaParameter", payloadCode, "@",
"NicaParameterBool", payloadCode, "@",
"NicaParameterDouble", payloadCode, "@",
"NicaParameterFloat", payloadCode, "@",
"NicaParameterInt", payloadCode, "@",
"NicaParameterString", payloadCode, "@",
"NicaParameterUInt", payloadCode, "@",
"NicaParameterULong", payloadCode, "@",
"NicaParameterULong64", payloadCode, "@",
"NicaSpline1D", payloadCode, "@",
"NicaSpline2D", payloadCode, "@",
"NicaSpline3D", payloadCode, "@",
"NicaXMLAttrib", payloadCode, "@",
"NicaXMLFile", payloadCode, "@",
"NicaXMLNode", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__NicaFeaturesDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__NicaFeaturesDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__NicaFeaturesDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__NicaFeaturesDict() {
  TriggerDictionaryInitialization_G__NicaFeaturesDict_Impl();
}
