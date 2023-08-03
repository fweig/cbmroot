// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmQaBase
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
#include "CbmQaConstants.h"
#include "CbmQaCanvas.h"
#include "CbmQaEff.h"
#include "CbmQaPie.h"
#include "CbmQaHist.h"
#include "CbmQaTable.h"
#include "CbmQaTask.h"
#include "CbmQaIO.h"
#include "CbmQaCheckerCore.h"
#include "CbmQaCheckerFileHandler.h"
#include "CbmQaCheckerHist1DHandler.h"
#include "CbmQaCheckerHist2DHandler.h"
#include "CbmQaCheckerObjectHandler.h"
#include "CbmQaCheckerProfile1DHandler.h"
#include "CbmQaCheckerObjectDB.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CbmQaCanvas(void *p = nullptr);
   static void *newArray_CbmQaCanvas(Long_t size, void *p);
   static void delete_CbmQaCanvas(void *p);
   static void deleteArray_CbmQaCanvas(void *p);
   static void destruct_CbmQaCanvas(void *p);
   static void streamer_CbmQaCanvas(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmQaCanvas*)
   {
      ::CbmQaCanvas *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmQaCanvas >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmQaCanvas", ::CbmQaCanvas::Class_Version(), "CbmQaCanvas.h", 31,
                  typeid(::CbmQaCanvas), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmQaCanvas::Dictionary, isa_proxy, 17,
                  sizeof(::CbmQaCanvas) );
      instance.SetNew(&new_CbmQaCanvas);
      instance.SetNewArray(&newArray_CbmQaCanvas);
      instance.SetDelete(&delete_CbmQaCanvas);
      instance.SetDeleteArray(&deleteArray_CbmQaCanvas);
      instance.SetDestructor(&destruct_CbmQaCanvas);
      instance.SetStreamerFunc(&streamer_CbmQaCanvas);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmQaCanvas*)
   {
      return GenerateInitInstanceLocal((::CbmQaCanvas*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmQaCanvas*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmQaEff(void *p = nullptr);
   static void *newArray_CbmQaEff(Long_t size, void *p);
   static void delete_CbmQaEff(void *p);
   static void deleteArray_CbmQaEff(void *p);
   static void destruct_CbmQaEff(void *p);
   static Long64_t merge_CbmQaEff(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmQaEff*)
   {
      ::CbmQaEff *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmQaEff >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmQaEff", ::CbmQaEff::Class_Version(), "CbmQaEff.h", 31,
                  typeid(::CbmQaEff), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmQaEff::Dictionary, isa_proxy, 4,
                  sizeof(::CbmQaEff) );
      instance.SetNew(&new_CbmQaEff);
      instance.SetNewArray(&newArray_CbmQaEff);
      instance.SetDelete(&delete_CbmQaEff);
      instance.SetDeleteArray(&deleteArray_CbmQaEff);
      instance.SetDestructor(&destruct_CbmQaEff);
      instance.SetMerge(&merge_CbmQaEff);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmQaEff*)
   {
      return GenerateInitInstanceLocal((::CbmQaEff*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmQaEff*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmQaPieSlice(void *p = nullptr);
   static void *newArray_CbmQaPieSlice(Long_t size, void *p);
   static void delete_CbmQaPieSlice(void *p);
   static void deleteArray_CbmQaPieSlice(void *p);
   static void destruct_CbmQaPieSlice(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmQaPieSlice*)
   {
      ::CbmQaPieSlice *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmQaPieSlice >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmQaPieSlice", ::CbmQaPieSlice::Class_Version(), "CbmQaPie.h", 23,
                  typeid(::CbmQaPieSlice), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmQaPieSlice::Dictionary, isa_proxy, 4,
                  sizeof(::CbmQaPieSlice) );
      instance.SetNew(&new_CbmQaPieSlice);
      instance.SetNewArray(&newArray_CbmQaPieSlice);
      instance.SetDelete(&delete_CbmQaPieSlice);
      instance.SetDeleteArray(&deleteArray_CbmQaPieSlice);
      instance.SetDestructor(&destruct_CbmQaPieSlice);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmQaPieSlice*)
   {
      return GenerateInitInstanceLocal((::CbmQaPieSlice*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmQaPieSlice*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmQaPie(void *p = nullptr);
   static void *newArray_CbmQaPie(Long_t size, void *p);
   static void delete_CbmQaPie(void *p);
   static void deleteArray_CbmQaPie(void *p);
   static void destruct_CbmQaPie(void *p);
   static void streamer_CbmQaPie(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmQaPie*)
   {
      ::CbmQaPie *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmQaPie >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmQaPie", ::CbmQaPie::Class_Version(), "CbmQaPie.h", 41,
                  typeid(::CbmQaPie), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmQaPie::Dictionary, isa_proxy, 17,
                  sizeof(::CbmQaPie) );
      instance.SetNew(&new_CbmQaPie);
      instance.SetNewArray(&newArray_CbmQaPie);
      instance.SetDelete(&delete_CbmQaPie);
      instance.SetDeleteArray(&deleteArray_CbmQaPie);
      instance.SetDestructor(&destruct_CbmQaPie);
      instance.SetStreamerFunc(&streamer_CbmQaPie);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmQaPie*)
   {
      return GenerateInitInstanceLocal((::CbmQaPie*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmQaPie*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmQaHistlETProfile2DgR_Dictionary();
   static void CbmQaHistlETProfile2DgR_TClassManip(TClass*);
   static void *new_CbmQaHistlETProfile2DgR(void *p = nullptr);
   static void *newArray_CbmQaHistlETProfile2DgR(Long_t size, void *p);
   static void delete_CbmQaHistlETProfile2DgR(void *p);
   static void deleteArray_CbmQaHistlETProfile2DgR(void *p);
   static void destruct_CbmQaHistlETProfile2DgR(void *p);
   static void directoryAutoAdd_CbmQaHistlETProfile2DgR(void *obj, TDirectory *dir);
   static Long64_t merge_CbmQaHistlETProfile2DgR(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmQaHist<TProfile2D>*)
   {
      ::CbmQaHist<TProfile2D> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmQaHist<TProfile2D> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmQaHist<TProfile2D>", ::CbmQaHist<TProfile2D>::Class_Version(), "CbmQaHist.h", 37,
                  typeid(::CbmQaHist<TProfile2D>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmQaHistlETProfile2DgR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmQaHist<TProfile2D>) );
      instance.SetNew(&new_CbmQaHistlETProfile2DgR);
      instance.SetNewArray(&newArray_CbmQaHistlETProfile2DgR);
      instance.SetDelete(&delete_CbmQaHistlETProfile2DgR);
      instance.SetDeleteArray(&deleteArray_CbmQaHistlETProfile2DgR);
      instance.SetDestructor(&destruct_CbmQaHistlETProfile2DgR);
      instance.SetDirectoryAutoAdd(&directoryAutoAdd_CbmQaHistlETProfile2DgR);
      instance.SetMerge(&merge_CbmQaHistlETProfile2DgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmQaHist<TProfile2D>*)
   {
      return GenerateInitInstanceLocal((::CbmQaHist<TProfile2D>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmQaHist<TProfile2D>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmQaHistlETProfile2DgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TProfile2D>*)nullptr)->GetClass();
      CbmQaHistlETProfile2DgR_TClassManip(theClass);
   return theClass;
   }

   static void CbmQaHistlETProfile2DgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmQaHistlETProfilegR_Dictionary();
   static void CbmQaHistlETProfilegR_TClassManip(TClass*);
   static void *new_CbmQaHistlETProfilegR(void *p = nullptr);
   static void *newArray_CbmQaHistlETProfilegR(Long_t size, void *p);
   static void delete_CbmQaHistlETProfilegR(void *p);
   static void deleteArray_CbmQaHistlETProfilegR(void *p);
   static void destruct_CbmQaHistlETProfilegR(void *p);
   static void directoryAutoAdd_CbmQaHistlETProfilegR(void *obj, TDirectory *dir);
   static Long64_t merge_CbmQaHistlETProfilegR(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmQaHist<TProfile>*)
   {
      ::CbmQaHist<TProfile> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmQaHist<TProfile> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmQaHist<TProfile>", ::CbmQaHist<TProfile>::Class_Version(), "CbmQaHist.h", 37,
                  typeid(::CbmQaHist<TProfile>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmQaHistlETProfilegR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmQaHist<TProfile>) );
      instance.SetNew(&new_CbmQaHistlETProfilegR);
      instance.SetNewArray(&newArray_CbmQaHistlETProfilegR);
      instance.SetDelete(&delete_CbmQaHistlETProfilegR);
      instance.SetDeleteArray(&deleteArray_CbmQaHistlETProfilegR);
      instance.SetDestructor(&destruct_CbmQaHistlETProfilegR);
      instance.SetDirectoryAutoAdd(&directoryAutoAdd_CbmQaHistlETProfilegR);
      instance.SetMerge(&merge_CbmQaHistlETProfilegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmQaHist<TProfile>*)
   {
      return GenerateInitInstanceLocal((::CbmQaHist<TProfile>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmQaHist<TProfile>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmQaHistlETProfilegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TProfile>*)nullptr)->GetClass();
      CbmQaHistlETProfilegR_TClassManip(theClass);
   return theClass;
   }

   static void CbmQaHistlETProfilegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmQaHistlETH1IgR_Dictionary();
   static void CbmQaHistlETH1IgR_TClassManip(TClass*);
   static void *new_CbmQaHistlETH1IgR(void *p = nullptr);
   static void *newArray_CbmQaHistlETH1IgR(Long_t size, void *p);
   static void delete_CbmQaHistlETH1IgR(void *p);
   static void deleteArray_CbmQaHistlETH1IgR(void *p);
   static void destruct_CbmQaHistlETH1IgR(void *p);
   static void directoryAutoAdd_CbmQaHistlETH1IgR(void *obj, TDirectory *dir);
   static Long64_t merge_CbmQaHistlETH1IgR(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmQaHist<TH1I>*)
   {
      ::CbmQaHist<TH1I> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmQaHist<TH1I> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmQaHist<TH1I>", ::CbmQaHist<TH1I>::Class_Version(), "CbmQaHist.h", 37,
                  typeid(::CbmQaHist<TH1I>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmQaHistlETH1IgR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmQaHist<TH1I>) );
      instance.SetNew(&new_CbmQaHistlETH1IgR);
      instance.SetNewArray(&newArray_CbmQaHistlETH1IgR);
      instance.SetDelete(&delete_CbmQaHistlETH1IgR);
      instance.SetDeleteArray(&deleteArray_CbmQaHistlETH1IgR);
      instance.SetDestructor(&destruct_CbmQaHistlETH1IgR);
      instance.SetDirectoryAutoAdd(&directoryAutoAdd_CbmQaHistlETH1IgR);
      instance.SetMerge(&merge_CbmQaHistlETH1IgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmQaHist<TH1I>*)
   {
      return GenerateInitInstanceLocal((::CbmQaHist<TH1I>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmQaHist<TH1I>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmQaHistlETH1IgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1I>*)nullptr)->GetClass();
      CbmQaHistlETH1IgR_TClassManip(theClass);
   return theClass;
   }

   static void CbmQaHistlETH1IgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmQaHistlETH1DgR_Dictionary();
   static void CbmQaHistlETH1DgR_TClassManip(TClass*);
   static void *new_CbmQaHistlETH1DgR(void *p = nullptr);
   static void *newArray_CbmQaHistlETH1DgR(Long_t size, void *p);
   static void delete_CbmQaHistlETH1DgR(void *p);
   static void deleteArray_CbmQaHistlETH1DgR(void *p);
   static void destruct_CbmQaHistlETH1DgR(void *p);
   static void directoryAutoAdd_CbmQaHistlETH1DgR(void *obj, TDirectory *dir);
   static Long64_t merge_CbmQaHistlETH1DgR(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmQaHist<TH1D>*)
   {
      ::CbmQaHist<TH1D> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmQaHist<TH1D> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmQaHist<TH1D>", ::CbmQaHist<TH1D>::Class_Version(), "CbmQaHist.h", 37,
                  typeid(::CbmQaHist<TH1D>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmQaHistlETH1DgR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmQaHist<TH1D>) );
      instance.SetNew(&new_CbmQaHistlETH1DgR);
      instance.SetNewArray(&newArray_CbmQaHistlETH1DgR);
      instance.SetDelete(&delete_CbmQaHistlETH1DgR);
      instance.SetDeleteArray(&deleteArray_CbmQaHistlETH1DgR);
      instance.SetDestructor(&destruct_CbmQaHistlETH1DgR);
      instance.SetDirectoryAutoAdd(&directoryAutoAdd_CbmQaHistlETH1DgR);
      instance.SetMerge(&merge_CbmQaHistlETH1DgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmQaHist<TH1D>*)
   {
      return GenerateInitInstanceLocal((::CbmQaHist<TH1D>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmQaHist<TH1D>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmQaHistlETH1DgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1D>*)nullptr)->GetClass();
      CbmQaHistlETH1DgR_TClassManip(theClass);
   return theClass;
   }

   static void CbmQaHistlETH1DgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmQaHistlETH1FgR_Dictionary();
   static void CbmQaHistlETH1FgR_TClassManip(TClass*);
   static void *new_CbmQaHistlETH1FgR(void *p = nullptr);
   static void *newArray_CbmQaHistlETH1FgR(Long_t size, void *p);
   static void delete_CbmQaHistlETH1FgR(void *p);
   static void deleteArray_CbmQaHistlETH1FgR(void *p);
   static void destruct_CbmQaHistlETH1FgR(void *p);
   static void directoryAutoAdd_CbmQaHistlETH1FgR(void *obj, TDirectory *dir);
   static Long64_t merge_CbmQaHistlETH1FgR(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmQaHist<TH1F>*)
   {
      ::CbmQaHist<TH1F> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmQaHist<TH1F> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmQaHist<TH1F>", ::CbmQaHist<TH1F>::Class_Version(), "CbmQaHist.h", 37,
                  typeid(::CbmQaHist<TH1F>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmQaHistlETH1FgR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmQaHist<TH1F>) );
      instance.SetNew(&new_CbmQaHistlETH1FgR);
      instance.SetNewArray(&newArray_CbmQaHistlETH1FgR);
      instance.SetDelete(&delete_CbmQaHistlETH1FgR);
      instance.SetDeleteArray(&deleteArray_CbmQaHistlETH1FgR);
      instance.SetDestructor(&destruct_CbmQaHistlETH1FgR);
      instance.SetDirectoryAutoAdd(&directoryAutoAdd_CbmQaHistlETH1FgR);
      instance.SetMerge(&merge_CbmQaHistlETH1FgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmQaHist<TH1F>*)
   {
      return GenerateInitInstanceLocal((::CbmQaHist<TH1F>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmQaHist<TH1F>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmQaHistlETH1FgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1F>*)nullptr)->GetClass();
      CbmQaHistlETH1FgR_TClassManip(theClass);
   return theClass;
   }

   static void CbmQaHistlETH1FgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmQaTable(void *p = nullptr);
   static void *newArray_CbmQaTable(Long_t size, void *p);
   static void delete_CbmQaTable(void *p);
   static void deleteArray_CbmQaTable(void *p);
   static void destruct_CbmQaTable(void *p);
   static void directoryAutoAdd_CbmQaTable(void *obj, TDirectory *dir);
   static Long64_t merge_CbmQaTable(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmQaTable*)
   {
      ::CbmQaTable *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmQaTable >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmQaTable", ::CbmQaTable::Class_Version(), "CbmQaTable.h", 24,
                  typeid(::CbmQaTable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmQaTable::Dictionary, isa_proxy, 4,
                  sizeof(::CbmQaTable) );
      instance.SetNew(&new_CbmQaTable);
      instance.SetNewArray(&newArray_CbmQaTable);
      instance.SetDelete(&delete_CbmQaTable);
      instance.SetDeleteArray(&deleteArray_CbmQaTable);
      instance.SetDestructor(&destruct_CbmQaTable);
      instance.SetDirectoryAutoAdd(&directoryAutoAdd_CbmQaTable);
      instance.SetMerge(&merge_CbmQaTable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmQaTable*)
   {
      return GenerateInitInstanceLocal((::CbmQaTable*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmQaTable*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmQaTask(void *p);
   static void deleteArray_CbmQaTask(void *p);
   static void destruct_CbmQaTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmQaTask*)
   {
      ::CbmQaTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmQaTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmQaTask", ::CbmQaTask::Class_Version(), "CbmQaTask.h", 40,
                  typeid(::CbmQaTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmQaTask::Dictionary, isa_proxy, 4,
                  sizeof(::CbmQaTask) );
      instance.SetDelete(&delete_CbmQaTask);
      instance.SetDeleteArray(&deleteArray_CbmQaTask);
      instance.SetDestructor(&destruct_CbmQaTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmQaTask*)
   {
      return GenerateInitInstanceLocal((::CbmQaTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmQaTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *cbmcLcLqacLcLcheckercLcLObjectDB_Dictionary();
   static void cbmcLcLqacLcLcheckercLcLObjectDB_TClassManip(TClass*);
   static void *new_cbmcLcLqacLcLcheckercLcLObjectDB(void *p = nullptr);
   static void *newArray_cbmcLcLqacLcLcheckercLcLObjectDB(Long_t size, void *p);
   static void delete_cbmcLcLqacLcLcheckercLcLObjectDB(void *p);
   static void deleteArray_cbmcLcLqacLcLcheckercLcLObjectDB(void *p);
   static void destruct_cbmcLcLqacLcLcheckercLcLObjectDB(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cbm::qa::checker::ObjectDB*)
   {
      ::cbm::qa::checker::ObjectDB *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::cbm::qa::checker::ObjectDB));
      static ::ROOT::TGenericClassInfo 
         instance("cbm::qa::checker::ObjectDB", "CbmQaCheckerObjectDB.h", 25,
                  typeid(::cbm::qa::checker::ObjectDB), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &cbmcLcLqacLcLcheckercLcLObjectDB_Dictionary, isa_proxy, 4,
                  sizeof(::cbm::qa::checker::ObjectDB) );
      instance.SetNew(&new_cbmcLcLqacLcLcheckercLcLObjectDB);
      instance.SetNewArray(&newArray_cbmcLcLqacLcLcheckercLcLObjectDB);
      instance.SetDelete(&delete_cbmcLcLqacLcLcheckercLcLObjectDB);
      instance.SetDeleteArray(&deleteArray_cbmcLcLqacLcLcheckercLcLObjectDB);
      instance.SetDestructor(&destruct_cbmcLcLqacLcLcheckercLcLObjectDB);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cbm::qa::checker::ObjectDB*)
   {
      return GenerateInitInstanceLocal((::cbm::qa::checker::ObjectDB*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cbm::qa::checker::ObjectDB*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cbmcLcLqacLcLcheckercLcLObjectDB_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cbm::qa::checker::ObjectDB*)nullptr)->GetClass();
      cbmcLcLqacLcLcheckercLcLObjectDB_TClassManip(theClass);
   return theClass;
   }

   static void cbmcLcLqacLcLcheckercLcLObjectDB_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *cbmcLcLqacLcLcheckercLcLCore_Dictionary();
   static void cbmcLcLqacLcLcheckercLcLCore_TClassManip(TClass*);
   static void *new_cbmcLcLqacLcLcheckercLcLCore(void *p = nullptr);
   static void *newArray_cbmcLcLqacLcLcheckercLcLCore(Long_t size, void *p);
   static void delete_cbmcLcLqacLcLcheckercLcLCore(void *p);
   static void deleteArray_cbmcLcLqacLcLcheckercLcLCore(void *p);
   static void destruct_cbmcLcLqacLcLcheckercLcLCore(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cbm::qa::checker::Core*)
   {
      ::cbm::qa::checker::Core *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::cbm::qa::checker::Core));
      static ::ROOT::TGenericClassInfo 
         instance("cbm::qa::checker::Core", "CbmQaCheckerCore.h", 31,
                  typeid(::cbm::qa::checker::Core), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &cbmcLcLqacLcLcheckercLcLCore_Dictionary, isa_proxy, 4,
                  sizeof(::cbm::qa::checker::Core) );
      instance.SetNew(&new_cbmcLcLqacLcLcheckercLcLCore);
      instance.SetNewArray(&newArray_cbmcLcLqacLcLcheckercLcLCore);
      instance.SetDelete(&delete_cbmcLcLqacLcLcheckercLcLCore);
      instance.SetDeleteArray(&deleteArray_cbmcLcLqacLcLcheckercLcLCore);
      instance.SetDestructor(&destruct_cbmcLcLqacLcLcheckercLcLCore);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cbm::qa::checker::Core*)
   {
      return GenerateInitInstanceLocal((::cbm::qa::checker::Core*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cbm::qa::checker::Core*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cbmcLcLqacLcLcheckercLcLCore_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cbm::qa::checker::Core*)nullptr)->GetClass();
      cbmcLcLqacLcLcheckercLcLCore_TClassManip(theClass);
   return theClass;
   }

   static void cbmcLcLqacLcLcheckercLcLCore_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *cbmcLcLqacLcLcheckercLcLFileHandler_Dictionary();
   static void cbmcLcLqacLcLcheckercLcLFileHandler_TClassManip(TClass*);
   static void delete_cbmcLcLqacLcLcheckercLcLFileHandler(void *p);
   static void deleteArray_cbmcLcLqacLcLcheckercLcLFileHandler(void *p);
   static void destruct_cbmcLcLqacLcLcheckercLcLFileHandler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cbm::qa::checker::FileHandler*)
   {
      ::cbm::qa::checker::FileHandler *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::cbm::qa::checker::FileHandler));
      static ::ROOT::TGenericClassInfo 
         instance("cbm::qa::checker::FileHandler", "CbmQaCheckerFileHandler.h", 29,
                  typeid(::cbm::qa::checker::FileHandler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &cbmcLcLqacLcLcheckercLcLFileHandler_Dictionary, isa_proxy, 4,
                  sizeof(::cbm::qa::checker::FileHandler) );
      instance.SetDelete(&delete_cbmcLcLqacLcLcheckercLcLFileHandler);
      instance.SetDeleteArray(&deleteArray_cbmcLcLqacLcLcheckercLcLFileHandler);
      instance.SetDestructor(&destruct_cbmcLcLqacLcLcheckercLcLFileHandler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cbm::qa::checker::FileHandler*)
   {
      return GenerateInitInstanceLocal((::cbm::qa::checker::FileHandler*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cbm::qa::checker::FileHandler*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cbmcLcLqacLcLcheckercLcLFileHandler_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cbm::qa::checker::FileHandler*)nullptr)->GetClass();
      cbmcLcLqacLcLcheckercLcLFileHandler_TClassManip(theClass);
   return theClass;
   }

   static void cbmcLcLqacLcLcheckercLcLFileHandler_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *cbmcLcLqacLcLcheckercLcLObjectHandler_Dictionary();
   static void cbmcLcLqacLcLcheckercLcLObjectHandler_TClassManip(TClass*);
   static void delete_cbmcLcLqacLcLcheckercLcLObjectHandler(void *p);
   static void deleteArray_cbmcLcLqacLcLcheckercLcLObjectHandler(void *p);
   static void destruct_cbmcLcLqacLcLcheckercLcLObjectHandler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cbm::qa::checker::ObjectHandler*)
   {
      ::cbm::qa::checker::ObjectHandler *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::cbm::qa::checker::ObjectHandler));
      static ::ROOT::TGenericClassInfo 
         instance("cbm::qa::checker::ObjectHandler", "CbmQaCheckerObjectHandler.h", 34,
                  typeid(::cbm::qa::checker::ObjectHandler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &cbmcLcLqacLcLcheckercLcLObjectHandler_Dictionary, isa_proxy, 4,
                  sizeof(::cbm::qa::checker::ObjectHandler) );
      instance.SetDelete(&delete_cbmcLcLqacLcLcheckercLcLObjectHandler);
      instance.SetDeleteArray(&deleteArray_cbmcLcLqacLcLcheckercLcLObjectHandler);
      instance.SetDestructor(&destruct_cbmcLcLqacLcLcheckercLcLObjectHandler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cbm::qa::checker::ObjectHandler*)
   {
      return GenerateInitInstanceLocal((::cbm::qa::checker::ObjectHandler*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cbm::qa::checker::ObjectHandler*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cbmcLcLqacLcLcheckercLcLObjectHandler_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cbm::qa::checker::ObjectHandler*)nullptr)->GetClass();
      cbmcLcLqacLcLcheckercLcLObjectHandler_TClassManip(theClass);
   return theClass;
   }

   static void cbmcLcLqacLcLcheckercLcLObjectHandler_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *cbmcLcLqacLcLcheckercLcLHist1DHandler_Dictionary();
   static void cbmcLcLqacLcLcheckercLcLHist1DHandler_TClassManip(TClass*);
   static void delete_cbmcLcLqacLcLcheckercLcLHist1DHandler(void *p);
   static void deleteArray_cbmcLcLqacLcLcheckercLcLHist1DHandler(void *p);
   static void destruct_cbmcLcLqacLcLcheckercLcLHist1DHandler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cbm::qa::checker::Hist1DHandler*)
   {
      ::cbm::qa::checker::Hist1DHandler *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::cbm::qa::checker::Hist1DHandler));
      static ::ROOT::TGenericClassInfo 
         instance("cbm::qa::checker::Hist1DHandler", "CbmQaCheckerHist1DHandler.h", 29,
                  typeid(::cbm::qa::checker::Hist1DHandler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &cbmcLcLqacLcLcheckercLcLHist1DHandler_Dictionary, isa_proxy, 4,
                  sizeof(::cbm::qa::checker::Hist1DHandler) );
      instance.SetDelete(&delete_cbmcLcLqacLcLcheckercLcLHist1DHandler);
      instance.SetDeleteArray(&deleteArray_cbmcLcLqacLcLcheckercLcLHist1DHandler);
      instance.SetDestructor(&destruct_cbmcLcLqacLcLcheckercLcLHist1DHandler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cbm::qa::checker::Hist1DHandler*)
   {
      return GenerateInitInstanceLocal((::cbm::qa::checker::Hist1DHandler*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cbm::qa::checker::Hist1DHandler*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cbmcLcLqacLcLcheckercLcLHist1DHandler_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cbm::qa::checker::Hist1DHandler*)nullptr)->GetClass();
      cbmcLcLqacLcLcheckercLcLHist1DHandler_TClassManip(theClass);
   return theClass;
   }

   static void cbmcLcLqacLcLcheckercLcLHist1DHandler_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *cbmcLcLqacLcLcheckercLcLHist2DHandler_Dictionary();
   static void cbmcLcLqacLcLcheckercLcLHist2DHandler_TClassManip(TClass*);
   static void delete_cbmcLcLqacLcLcheckercLcLHist2DHandler(void *p);
   static void deleteArray_cbmcLcLqacLcLcheckercLcLHist2DHandler(void *p);
   static void destruct_cbmcLcLqacLcLcheckercLcLHist2DHandler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cbm::qa::checker::Hist2DHandler*)
   {
      ::cbm::qa::checker::Hist2DHandler *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::cbm::qa::checker::Hist2DHandler));
      static ::ROOT::TGenericClassInfo 
         instance("cbm::qa::checker::Hist2DHandler", "CbmQaCheckerHist2DHandler.h", 19,
                  typeid(::cbm::qa::checker::Hist2DHandler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &cbmcLcLqacLcLcheckercLcLHist2DHandler_Dictionary, isa_proxy, 4,
                  sizeof(::cbm::qa::checker::Hist2DHandler) );
      instance.SetDelete(&delete_cbmcLcLqacLcLcheckercLcLHist2DHandler);
      instance.SetDeleteArray(&deleteArray_cbmcLcLqacLcLcheckercLcLHist2DHandler);
      instance.SetDestructor(&destruct_cbmcLcLqacLcLcheckercLcLHist2DHandler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cbm::qa::checker::Hist2DHandler*)
   {
      return GenerateInitInstanceLocal((::cbm::qa::checker::Hist2DHandler*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cbm::qa::checker::Hist2DHandler*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cbmcLcLqacLcLcheckercLcLHist2DHandler_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cbm::qa::checker::Hist2DHandler*)nullptr)->GetClass();
      cbmcLcLqacLcLcheckercLcLHist2DHandler_TClassManip(theClass);
   return theClass;
   }

   static void cbmcLcLqacLcLcheckercLcLHist2DHandler_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *cbmcLcLqacLcLcheckercLcLProfile1DHandler_Dictionary();
   static void cbmcLcLqacLcLcheckercLcLProfile1DHandler_TClassManip(TClass*);
   static void delete_cbmcLcLqacLcLcheckercLcLProfile1DHandler(void *p);
   static void deleteArray_cbmcLcLqacLcLcheckercLcLProfile1DHandler(void *p);
   static void destruct_cbmcLcLqacLcLcheckercLcLProfile1DHandler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cbm::qa::checker::Profile1DHandler*)
   {
      ::cbm::qa::checker::Profile1DHandler *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::cbm::qa::checker::Profile1DHandler));
      static ::ROOT::TGenericClassInfo 
         instance("cbm::qa::checker::Profile1DHandler", "CbmQaCheckerProfile1DHandler.h", 19,
                  typeid(::cbm::qa::checker::Profile1DHandler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &cbmcLcLqacLcLcheckercLcLProfile1DHandler_Dictionary, isa_proxy, 4,
                  sizeof(::cbm::qa::checker::Profile1DHandler) );
      instance.SetDelete(&delete_cbmcLcLqacLcLcheckercLcLProfile1DHandler);
      instance.SetDeleteArray(&deleteArray_cbmcLcLqacLcLcheckercLcLProfile1DHandler);
      instance.SetDestructor(&destruct_cbmcLcLqacLcLcheckercLcLProfile1DHandler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cbm::qa::checker::Profile1DHandler*)
   {
      return GenerateInitInstanceLocal((::cbm::qa::checker::Profile1DHandler*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cbm::qa::checker::Profile1DHandler*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cbmcLcLqacLcLcheckercLcLProfile1DHandler_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cbm::qa::checker::Profile1DHandler*)nullptr)->GetClass();
      cbmcLcLqacLcLcheckercLcLProfile1DHandler_TClassManip(theClass);
   return theClass;
   }

   static void cbmcLcLqacLcLcheckercLcLProfile1DHandler_TClassManip(TClass* ){
   }

} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmQaCanvas::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmQaCanvas::Class_Name()
{
   return "CbmQaCanvas";
}

//______________________________________________________________________________
const char *CbmQaCanvas::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaCanvas*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmQaCanvas::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaCanvas*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmQaCanvas::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaCanvas*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmQaCanvas::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaCanvas*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmQaEff::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmQaEff::Class_Name()
{
   return "CbmQaEff";
}

//______________________________________________________________________________
const char *CbmQaEff::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaEff*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmQaEff::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaEff*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmQaEff::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaEff*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmQaEff::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaEff*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmQaPieSlice::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmQaPieSlice::Class_Name()
{
   return "CbmQaPieSlice";
}

//______________________________________________________________________________
const char *CbmQaPieSlice::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaPieSlice*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmQaPieSlice::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaPieSlice*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmQaPieSlice::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaPieSlice*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmQaPieSlice::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaPieSlice*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmQaPie::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmQaPie::Class_Name()
{
   return "CbmQaPie";
}

//______________________________________________________________________________
const char *CbmQaPie::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaPie*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmQaPie::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaPie*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmQaPie::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaPie*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmQaPie::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaPie*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmQaHist<TProfile2D>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmQaHist<TProfile2D>::Class_Name()
{
   return "CbmQaHist<TProfile2D>";
}

//______________________________________________________________________________
template <> const char *CbmQaHist<TProfile2D>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TProfile2D>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmQaHist<TProfile2D>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TProfile2D>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmQaHist<TProfile2D>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TProfile2D>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmQaHist<TProfile2D>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TProfile2D>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmQaHist<TProfile>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmQaHist<TProfile>::Class_Name()
{
   return "CbmQaHist<TProfile>";
}

//______________________________________________________________________________
template <> const char *CbmQaHist<TProfile>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TProfile>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmQaHist<TProfile>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TProfile>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmQaHist<TProfile>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TProfile>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmQaHist<TProfile>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TProfile>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmQaHist<TH1I>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmQaHist<TH1I>::Class_Name()
{
   return "CbmQaHist<TH1I>";
}

//______________________________________________________________________________
template <> const char *CbmQaHist<TH1I>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1I>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmQaHist<TH1I>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1I>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmQaHist<TH1I>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1I>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmQaHist<TH1I>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1I>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmQaHist<TH1D>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmQaHist<TH1D>::Class_Name()
{
   return "CbmQaHist<TH1D>";
}

//______________________________________________________________________________
template <> const char *CbmQaHist<TH1D>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1D>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmQaHist<TH1D>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1D>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmQaHist<TH1D>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1D>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmQaHist<TH1D>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1D>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmQaHist<TH1F>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmQaHist<TH1F>::Class_Name()
{
   return "CbmQaHist<TH1F>";
}

//______________________________________________________________________________
template <> const char *CbmQaHist<TH1F>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1F>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmQaHist<TH1F>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1F>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmQaHist<TH1F>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1F>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmQaHist<TH1F>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaHist<TH1F>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmQaTable::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmQaTable::Class_Name()
{
   return "CbmQaTable";
}

//______________________________________________________________________________
const char *CbmQaTable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaTable*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmQaTable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaTable*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmQaTable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaTable*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmQaTable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaTable*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmQaTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmQaTask::Class_Name()
{
   return "CbmQaTask";
}

//______________________________________________________________________________
const char *CbmQaTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmQaTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmQaTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmQaTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmQaTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmQaTask*)nullptr)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmQaCanvas(void *p) {
      return  p ? new(p) ::CbmQaCanvas : new ::CbmQaCanvas;
   }
   static void *newArray_CbmQaCanvas(Long_t nElements, void *p) {
      return p ? new(p) ::CbmQaCanvas[nElements] : new ::CbmQaCanvas[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmQaCanvas(void *p) {
      delete ((::CbmQaCanvas*)p);
   }
   static void deleteArray_CbmQaCanvas(void *p) {
      delete [] ((::CbmQaCanvas*)p);
   }
   static void destruct_CbmQaCanvas(void *p) {
      typedef ::CbmQaCanvas current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmQaCanvas(TBuffer &buf, void *obj) {
      ((::CbmQaCanvas*)obj)->::CbmQaCanvas::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmQaCanvas

//______________________________________________________________________________
void CbmQaEff::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmQaEff.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmQaEff::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmQaEff::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmQaEff(void *p) {
      return  p ? new(p) ::CbmQaEff : new ::CbmQaEff;
   }
   static void *newArray_CbmQaEff(Long_t nElements, void *p) {
      return p ? new(p) ::CbmQaEff[nElements] : new ::CbmQaEff[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmQaEff(void *p) {
      delete ((::CbmQaEff*)p);
   }
   static void deleteArray_CbmQaEff(void *p) {
      delete [] ((::CbmQaEff*)p);
   }
   static void destruct_CbmQaEff(void *p) {
      typedef ::CbmQaEff current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_CbmQaEff(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::CbmQaEff*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::CbmQaEff

//______________________________________________________________________________
void CbmQaPieSlice::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmQaPieSlice.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmQaPieSlice::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmQaPieSlice::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmQaPieSlice(void *p) {
      return  p ? new(p) ::CbmQaPieSlice : new ::CbmQaPieSlice;
   }
   static void *newArray_CbmQaPieSlice(Long_t nElements, void *p) {
      return p ? new(p) ::CbmQaPieSlice[nElements] : new ::CbmQaPieSlice[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmQaPieSlice(void *p) {
      delete ((::CbmQaPieSlice*)p);
   }
   static void deleteArray_CbmQaPieSlice(void *p) {
      delete [] ((::CbmQaPieSlice*)p);
   }
   static void destruct_CbmQaPieSlice(void *p) {
      typedef ::CbmQaPieSlice current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmQaPieSlice

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmQaPie(void *p) {
      return  p ? new(p) ::CbmQaPie : new ::CbmQaPie;
   }
   static void *newArray_CbmQaPie(Long_t nElements, void *p) {
      return p ? new(p) ::CbmQaPie[nElements] : new ::CbmQaPie[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmQaPie(void *p) {
      delete ((::CbmQaPie*)p);
   }
   static void deleteArray_CbmQaPie(void *p) {
      delete [] ((::CbmQaPie*)p);
   }
   static void destruct_CbmQaPie(void *p) {
      typedef ::CbmQaPie current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmQaPie(TBuffer &buf, void *obj) {
      ((::CbmQaPie*)obj)->::CbmQaPie::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmQaPie

//______________________________________________________________________________
template <> void CbmQaHist<TProfile2D>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmQaHist<TProfile2D>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmQaHist<TProfile2D>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmQaHist<TProfile2D>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmQaHistlETProfile2DgR(void *p) {
      return  p ? new(p) ::CbmQaHist<TProfile2D> : new ::CbmQaHist<TProfile2D>;
   }
   static void *newArray_CbmQaHistlETProfile2DgR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmQaHist<TProfile2D>[nElements] : new ::CbmQaHist<TProfile2D>[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmQaHistlETProfile2DgR(void *p) {
      delete ((::CbmQaHist<TProfile2D>*)p);
   }
   static void deleteArray_CbmQaHistlETProfile2DgR(void *p) {
      delete [] ((::CbmQaHist<TProfile2D>*)p);
   }
   static void destruct_CbmQaHistlETProfile2DgR(void *p) {
      typedef ::CbmQaHist<TProfile2D> current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the directory auto add.
   static void directoryAutoAdd_CbmQaHistlETProfile2DgR(void *p, TDirectory *dir) {
      ((::CbmQaHist<TProfile2D>*)p)->DirectoryAutoAdd(dir);
   }
   // Wrapper around the merge function.
   static Long64_t  merge_CbmQaHistlETProfile2DgR(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::CbmQaHist<TProfile2D>*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::CbmQaHist<TProfile2D>

//______________________________________________________________________________
template <> void CbmQaHist<TProfile>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmQaHist<TProfile>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmQaHist<TProfile>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmQaHist<TProfile>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmQaHistlETProfilegR(void *p) {
      return  p ? new(p) ::CbmQaHist<TProfile> : new ::CbmQaHist<TProfile>;
   }
   static void *newArray_CbmQaHistlETProfilegR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmQaHist<TProfile>[nElements] : new ::CbmQaHist<TProfile>[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmQaHistlETProfilegR(void *p) {
      delete ((::CbmQaHist<TProfile>*)p);
   }
   static void deleteArray_CbmQaHistlETProfilegR(void *p) {
      delete [] ((::CbmQaHist<TProfile>*)p);
   }
   static void destruct_CbmQaHistlETProfilegR(void *p) {
      typedef ::CbmQaHist<TProfile> current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the directory auto add.
   static void directoryAutoAdd_CbmQaHistlETProfilegR(void *p, TDirectory *dir) {
      ((::CbmQaHist<TProfile>*)p)->DirectoryAutoAdd(dir);
   }
   // Wrapper around the merge function.
   static Long64_t  merge_CbmQaHistlETProfilegR(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::CbmQaHist<TProfile>*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::CbmQaHist<TProfile>

//______________________________________________________________________________
template <> void CbmQaHist<TH1I>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmQaHist<TH1I>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmQaHist<TH1I>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmQaHist<TH1I>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmQaHistlETH1IgR(void *p) {
      return  p ? new(p) ::CbmQaHist<TH1I> : new ::CbmQaHist<TH1I>;
   }
   static void *newArray_CbmQaHistlETH1IgR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmQaHist<TH1I>[nElements] : new ::CbmQaHist<TH1I>[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmQaHistlETH1IgR(void *p) {
      delete ((::CbmQaHist<TH1I>*)p);
   }
   static void deleteArray_CbmQaHistlETH1IgR(void *p) {
      delete [] ((::CbmQaHist<TH1I>*)p);
   }
   static void destruct_CbmQaHistlETH1IgR(void *p) {
      typedef ::CbmQaHist<TH1I> current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the directory auto add.
   static void directoryAutoAdd_CbmQaHistlETH1IgR(void *p, TDirectory *dir) {
      ((::CbmQaHist<TH1I>*)p)->DirectoryAutoAdd(dir);
   }
   // Wrapper around the merge function.
   static Long64_t  merge_CbmQaHistlETH1IgR(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::CbmQaHist<TH1I>*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::CbmQaHist<TH1I>

//______________________________________________________________________________
template <> void CbmQaHist<TH1D>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmQaHist<TH1D>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmQaHist<TH1D>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmQaHist<TH1D>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmQaHistlETH1DgR(void *p) {
      return  p ? new(p) ::CbmQaHist<TH1D> : new ::CbmQaHist<TH1D>;
   }
   static void *newArray_CbmQaHistlETH1DgR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmQaHist<TH1D>[nElements] : new ::CbmQaHist<TH1D>[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmQaHistlETH1DgR(void *p) {
      delete ((::CbmQaHist<TH1D>*)p);
   }
   static void deleteArray_CbmQaHistlETH1DgR(void *p) {
      delete [] ((::CbmQaHist<TH1D>*)p);
   }
   static void destruct_CbmQaHistlETH1DgR(void *p) {
      typedef ::CbmQaHist<TH1D> current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the directory auto add.
   static void directoryAutoAdd_CbmQaHistlETH1DgR(void *p, TDirectory *dir) {
      ((::CbmQaHist<TH1D>*)p)->DirectoryAutoAdd(dir);
   }
   // Wrapper around the merge function.
   static Long64_t  merge_CbmQaHistlETH1DgR(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::CbmQaHist<TH1D>*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::CbmQaHist<TH1D>

//______________________________________________________________________________
template <> void CbmQaHist<TH1F>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmQaHist<TH1F>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmQaHist<TH1F>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmQaHist<TH1F>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmQaHistlETH1FgR(void *p) {
      return  p ? new(p) ::CbmQaHist<TH1F> : new ::CbmQaHist<TH1F>;
   }
   static void *newArray_CbmQaHistlETH1FgR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmQaHist<TH1F>[nElements] : new ::CbmQaHist<TH1F>[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmQaHistlETH1FgR(void *p) {
      delete ((::CbmQaHist<TH1F>*)p);
   }
   static void deleteArray_CbmQaHistlETH1FgR(void *p) {
      delete [] ((::CbmQaHist<TH1F>*)p);
   }
   static void destruct_CbmQaHistlETH1FgR(void *p) {
      typedef ::CbmQaHist<TH1F> current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the directory auto add.
   static void directoryAutoAdd_CbmQaHistlETH1FgR(void *p, TDirectory *dir) {
      ((::CbmQaHist<TH1F>*)p)->DirectoryAutoAdd(dir);
   }
   // Wrapper around the merge function.
   static Long64_t  merge_CbmQaHistlETH1FgR(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::CbmQaHist<TH1F>*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::CbmQaHist<TH1F>

//______________________________________________________________________________
void CbmQaTable::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmQaTable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmQaTable::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmQaTable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmQaTable(void *p) {
      return  p ? new(p) ::CbmQaTable : new ::CbmQaTable;
   }
   static void *newArray_CbmQaTable(Long_t nElements, void *p) {
      return p ? new(p) ::CbmQaTable[nElements] : new ::CbmQaTable[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmQaTable(void *p) {
      delete ((::CbmQaTable*)p);
   }
   static void deleteArray_CbmQaTable(void *p) {
      delete [] ((::CbmQaTable*)p);
   }
   static void destruct_CbmQaTable(void *p) {
      typedef ::CbmQaTable current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the directory auto add.
   static void directoryAutoAdd_CbmQaTable(void *p, TDirectory *dir) {
      ((::CbmQaTable*)p)->DirectoryAutoAdd(dir);
   }
   // Wrapper around the merge function.
   static Long64_t  merge_CbmQaTable(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::CbmQaTable*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::CbmQaTable

//______________________________________________________________________________
void CbmQaTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmQaTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmQaTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmQaTask::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmQaTask(void *p) {
      delete ((::CbmQaTask*)p);
   }
   static void deleteArray_CbmQaTask(void *p) {
      delete [] ((::CbmQaTask*)p);
   }
   static void destruct_CbmQaTask(void *p) {
      typedef ::CbmQaTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmQaTask

namespace ROOT {
   // Wrappers around operator new
   static void *new_cbmcLcLqacLcLcheckercLcLObjectDB(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::cbm::qa::checker::ObjectDB : new ::cbm::qa::checker::ObjectDB;
   }
   static void *newArray_cbmcLcLqacLcLcheckercLcLObjectDB(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::cbm::qa::checker::ObjectDB[nElements] : new ::cbm::qa::checker::ObjectDB[nElements];
   }
   // Wrapper around operator delete
   static void delete_cbmcLcLqacLcLcheckercLcLObjectDB(void *p) {
      delete ((::cbm::qa::checker::ObjectDB*)p);
   }
   static void deleteArray_cbmcLcLqacLcLcheckercLcLObjectDB(void *p) {
      delete [] ((::cbm::qa::checker::ObjectDB*)p);
   }
   static void destruct_cbmcLcLqacLcLcheckercLcLObjectDB(void *p) {
      typedef ::cbm::qa::checker::ObjectDB current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cbm::qa::checker::ObjectDB

namespace ROOT {
   // Wrappers around operator new
   static void *new_cbmcLcLqacLcLcheckercLcLCore(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::cbm::qa::checker::Core : new ::cbm::qa::checker::Core;
   }
   static void *newArray_cbmcLcLqacLcLcheckercLcLCore(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::cbm::qa::checker::Core[nElements] : new ::cbm::qa::checker::Core[nElements];
   }
   // Wrapper around operator delete
   static void delete_cbmcLcLqacLcLcheckercLcLCore(void *p) {
      delete ((::cbm::qa::checker::Core*)p);
   }
   static void deleteArray_cbmcLcLqacLcLcheckercLcLCore(void *p) {
      delete [] ((::cbm::qa::checker::Core*)p);
   }
   static void destruct_cbmcLcLqacLcLcheckercLcLCore(void *p) {
      typedef ::cbm::qa::checker::Core current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cbm::qa::checker::Core

namespace ROOT {
   // Wrapper around operator delete
   static void delete_cbmcLcLqacLcLcheckercLcLFileHandler(void *p) {
      delete ((::cbm::qa::checker::FileHandler*)p);
   }
   static void deleteArray_cbmcLcLqacLcLcheckercLcLFileHandler(void *p) {
      delete [] ((::cbm::qa::checker::FileHandler*)p);
   }
   static void destruct_cbmcLcLqacLcLcheckercLcLFileHandler(void *p) {
      typedef ::cbm::qa::checker::FileHandler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cbm::qa::checker::FileHandler

namespace ROOT {
   // Wrapper around operator delete
   static void delete_cbmcLcLqacLcLcheckercLcLObjectHandler(void *p) {
      delete ((::cbm::qa::checker::ObjectHandler*)p);
   }
   static void deleteArray_cbmcLcLqacLcLcheckercLcLObjectHandler(void *p) {
      delete [] ((::cbm::qa::checker::ObjectHandler*)p);
   }
   static void destruct_cbmcLcLqacLcLcheckercLcLObjectHandler(void *p) {
      typedef ::cbm::qa::checker::ObjectHandler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cbm::qa::checker::ObjectHandler

namespace ROOT {
   // Wrapper around operator delete
   static void delete_cbmcLcLqacLcLcheckercLcLHist1DHandler(void *p) {
      delete ((::cbm::qa::checker::Hist1DHandler*)p);
   }
   static void deleteArray_cbmcLcLqacLcLcheckercLcLHist1DHandler(void *p) {
      delete [] ((::cbm::qa::checker::Hist1DHandler*)p);
   }
   static void destruct_cbmcLcLqacLcLcheckercLcLHist1DHandler(void *p) {
      typedef ::cbm::qa::checker::Hist1DHandler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cbm::qa::checker::Hist1DHandler

namespace ROOT {
   // Wrapper around operator delete
   static void delete_cbmcLcLqacLcLcheckercLcLHist2DHandler(void *p) {
      delete ((::cbm::qa::checker::Hist2DHandler*)p);
   }
   static void deleteArray_cbmcLcLqacLcLcheckercLcLHist2DHandler(void *p) {
      delete [] ((::cbm::qa::checker::Hist2DHandler*)p);
   }
   static void destruct_cbmcLcLqacLcLcheckercLcLHist2DHandler(void *p) {
      typedef ::cbm::qa::checker::Hist2DHandler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cbm::qa::checker::Hist2DHandler

namespace ROOT {
   // Wrapper around operator delete
   static void delete_cbmcLcLqacLcLcheckercLcLProfile1DHandler(void *p) {
      delete ((::cbm::qa::checker::Profile1DHandler*)p);
   }
   static void deleteArray_cbmcLcLqacLcLcheckercLcLProfile1DHandler(void *p) {
      delete [] ((::cbm::qa::checker::Profile1DHandler*)p);
   }
   static void destruct_cbmcLcLqacLcLcheckercLcLProfile1DHandler(void *p) {
      typedef ::cbm::qa::checker::Profile1DHandler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cbm::qa::checker::Profile1DHandler

namespace ROOT {
   static TClass *vectorlEunsignedsPshortgR_Dictionary();
   static void vectorlEunsignedsPshortgR_TClassManip(TClass*);
   static void *new_vectorlEunsignedsPshortgR(void *p = nullptr);
   static void *newArray_vectorlEunsignedsPshortgR(Long_t size, void *p);
   static void delete_vectorlEunsignedsPshortgR(void *p);
   static void deleteArray_vectorlEunsignedsPshortgR(void *p);
   static void destruct_vectorlEunsignedsPshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned short>*)
   {
      vector<unsigned short> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned short>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<unsigned short>", -2, "vector", 389,
                  typeid(vector<unsigned short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEunsignedsPshortgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<unsigned short>) );
      instance.SetNew(&new_vectorlEunsignedsPshortgR);
      instance.SetNewArray(&newArray_vectorlEunsignedsPshortgR);
      instance.SetDelete(&delete_vectorlEunsignedsPshortgR);
      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPshortgR);
      instance.SetDestructor(&destruct_vectorlEunsignedsPshortgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned short> >()));

      ::ROOT::AddClassAlternate("vector<unsigned short>","std::vector<unsigned short, std::allocator<unsigned short> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<unsigned short>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEunsignedsPshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<unsigned short>*)nullptr)->GetClass();
      vectorlEunsignedsPshortgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEunsignedsPshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEunsignedsPshortgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned short> : new vector<unsigned short>;
   }
   static void *newArray_vectorlEunsignedsPshortgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned short>[nElements] : new vector<unsigned short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEunsignedsPshortgR(void *p) {
      delete ((vector<unsigned short>*)p);
   }
   static void deleteArray_vectorlEunsignedsPshortgR(void *p) {
      delete [] ((vector<unsigned short>*)p);
   }
   static void destruct_vectorlEunsignedsPshortgR(void *p) {
      typedef vector<unsigned short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<unsigned short>

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
   static TClass *vectorlETNamedmUgR_Dictionary();
   static void vectorlETNamedmUgR_TClassManip(TClass*);
   static void *new_vectorlETNamedmUgR(void *p = nullptr);
   static void *newArray_vectorlETNamedmUgR(Long_t size, void *p);
   static void delete_vectorlETNamedmUgR(void *p);
   static void deleteArray_vectorlETNamedmUgR(void *p);
   static void destruct_vectorlETNamedmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TNamed*>*)
   {
      vector<TNamed*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TNamed*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TNamed*>", -2, "vector", 389,
                  typeid(vector<TNamed*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETNamedmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TNamed*>) );
      instance.SetNew(&new_vectorlETNamedmUgR);
      instance.SetNewArray(&newArray_vectorlETNamedmUgR);
      instance.SetDelete(&delete_vectorlETNamedmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETNamedmUgR);
      instance.SetDestructor(&destruct_vectorlETNamedmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TNamed*> >()));

      ::ROOT::AddClassAlternate("vector<TNamed*>","std::vector<TNamed*, std::allocator<TNamed*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TNamed*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETNamedmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TNamed*>*)nullptr)->GetClass();
      vectorlETNamedmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETNamedmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETNamedmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TNamed*> : new vector<TNamed*>;
   }
   static void *newArray_vectorlETNamedmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TNamed*>[nElements] : new vector<TNamed*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETNamedmUgR(void *p) {
      delete ((vector<TNamed*>*)p);
   }
   static void deleteArray_vectorlETNamedmUgR(void *p) {
      delete [] ((vector<TNamed*>*)p);
   }
   static void destruct_vectorlETNamedmUgR(void *p) {
      typedef vector<TNamed*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TNamed*>

namespace ROOT {
   static TClass *vectorlECbmQaPieSlicegR_Dictionary();
   static void vectorlECbmQaPieSlicegR_TClassManip(TClass*);
   static void *new_vectorlECbmQaPieSlicegR(void *p = nullptr);
   static void *newArray_vectorlECbmQaPieSlicegR(Long_t size, void *p);
   static void delete_vectorlECbmQaPieSlicegR(void *p);
   static void deleteArray_vectorlECbmQaPieSlicegR(void *p);
   static void destruct_vectorlECbmQaPieSlicegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmQaPieSlice>*)
   {
      vector<CbmQaPieSlice> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmQaPieSlice>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmQaPieSlice>", -2, "vector", 389,
                  typeid(vector<CbmQaPieSlice>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmQaPieSlicegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmQaPieSlice>) );
      instance.SetNew(&new_vectorlECbmQaPieSlicegR);
      instance.SetNewArray(&newArray_vectorlECbmQaPieSlicegR);
      instance.SetDelete(&delete_vectorlECbmQaPieSlicegR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmQaPieSlicegR);
      instance.SetDestructor(&destruct_vectorlECbmQaPieSlicegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmQaPieSlice> >()));

      ::ROOT::AddClassAlternate("vector<CbmQaPieSlice>","std::vector<CbmQaPieSlice, std::allocator<CbmQaPieSlice> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmQaPieSlice>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmQaPieSlicegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmQaPieSlice>*)nullptr)->GetClass();
      vectorlECbmQaPieSlicegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmQaPieSlicegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmQaPieSlicegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmQaPieSlice> : new vector<CbmQaPieSlice>;
   }
   static void *newArray_vectorlECbmQaPieSlicegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmQaPieSlice>[nElements] : new vector<CbmQaPieSlice>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmQaPieSlicegR(void *p) {
      delete ((vector<CbmQaPieSlice>*)p);
   }
   static void deleteArray_vectorlECbmQaPieSlicegR(void *p) {
      delete [] ((vector<CbmQaPieSlice>*)p);
   }
   static void destruct_vectorlECbmQaPieSlicegR(void *p) {
      typedef vector<CbmQaPieSlice> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmQaPieSlice>

namespace ROOT {
   static TClass *bitsetlE1gR_Dictionary();
   static void bitsetlE1gR_TClassManip(TClass*);
   static void *new_bitsetlE1gR(void *p = nullptr);
   static void *newArray_bitsetlE1gR(Long_t size, void *p);
   static void delete_bitsetlE1gR(void *p);
   static void deleteArray_bitsetlE1gR(void *p);
   static void destruct_bitsetlE1gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const bitset<1>*)
   {
      bitset<1> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(bitset<1>));
      static ::ROOT::TGenericClassInfo 
         instance("bitset<1>", 2, "bitset", 751,
                  typeid(bitset<1>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &bitsetlE1gR_Dictionary, isa_proxy, 0,
                  sizeof(bitset<1>) );
      instance.SetNew(&new_bitsetlE1gR);
      instance.SetNewArray(&newArray_bitsetlE1gR);
      instance.SetDelete(&delete_bitsetlE1gR);
      instance.SetDeleteArray(&deleteArray_bitsetlE1gR);
      instance.SetDestructor(&destruct_bitsetlE1gR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback<Internal::TStdBitsetHelper< bitset<1> > >()));

      ::ROOT::AddClassAlternate("bitset<1>","std::bitset<1ul>");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const bitset<1>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *bitsetlE1gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const bitset<1>*)nullptr)->GetClass();
      bitsetlE1gR_TClassManip(theClass);
   return theClass;
   }

   static void bitsetlE1gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_bitsetlE1gR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) bitset<1> : new bitset<1>;
   }
   static void *newArray_bitsetlE1gR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) bitset<1>[nElements] : new bitset<1>[nElements];
   }
   // Wrapper around operator delete
   static void delete_bitsetlE1gR(void *p) {
      delete ((bitset<1>*)p);
   }
   static void deleteArray_bitsetlE1gR(void *p) {
      delete [] ((bitset<1>*)p);
   }
   static void destruct_bitsetlE1gR(void *p) {
      typedef bitset<1> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class bitset<1>

namespace ROOT {
   static TClass *bitsetlE16gR_Dictionary();
   static void bitsetlE16gR_TClassManip(TClass*);
   static void *new_bitsetlE16gR(void *p = nullptr);
   static void *newArray_bitsetlE16gR(Long_t size, void *p);
   static void delete_bitsetlE16gR(void *p);
   static void deleteArray_bitsetlE16gR(void *p);
   static void destruct_bitsetlE16gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const bitset<16>*)
   {
      bitset<16> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(bitset<16>));
      static ::ROOT::TGenericClassInfo 
         instance("bitset<16>", 2, "bitset", 751,
                  typeid(bitset<16>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &bitsetlE16gR_Dictionary, isa_proxy, 0,
                  sizeof(bitset<16>) );
      instance.SetNew(&new_bitsetlE16gR);
      instance.SetNewArray(&newArray_bitsetlE16gR);
      instance.SetDelete(&delete_bitsetlE16gR);
      instance.SetDeleteArray(&deleteArray_bitsetlE16gR);
      instance.SetDestructor(&destruct_bitsetlE16gR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback<Internal::TStdBitsetHelper< bitset<16> > >()));

      ::ROOT::AddClassAlternate("bitset<16>","std::bitset<16ul>");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const bitset<16>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *bitsetlE16gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const bitset<16>*)nullptr)->GetClass();
      bitsetlE16gR_TClassManip(theClass);
   return theClass;
   }

   static void bitsetlE16gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_bitsetlE16gR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) bitset<16> : new bitset<16>;
   }
   static void *newArray_bitsetlE16gR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) bitset<16>[nElements] : new bitset<16>[nElements];
   }
   // Wrapper around operator delete
   static void delete_bitsetlE16gR(void *p) {
      delete ((bitset<16>*)p);
   }
   static void deleteArray_bitsetlE16gR(void *p) {
      delete [] ((bitset<16>*)p);
   }
   static void destruct_bitsetlE16gR(void *p) {
      typedef bitset<16> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class bitset<16>

namespace {
  void TriggerDictionaryInitialization_libCbmQaBase_Impl() {
    static const char* headers[] = {
"CbmQaConstants.h",
"CbmQaCanvas.h",
"CbmQaEff.h",
"CbmQaPie.h",
"CbmQaHist.h",
"CbmQaTable.h",
"CbmQaTask.h",
"CbmQaIO.h",
"CbmQaCheckerCore.h",
"CbmQaCheckerFileHandler.h",
"CbmQaCheckerHist1DHandler.h",
"CbmQaCheckerHist2DHandler.h",
"CbmQaCheckerObjectHandler.h",
"CbmQaCheckerProfile1DHandler.h",
"CbmQaCheckerObjectDB.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/qa",
"/home/heinemann/gpuunpacker/cbmroot/core/qa/checker",
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
"/home/heinemann/gpuunpacker/cbmroot/core/qa/checker",
"/home/heinemann/gpuunpacker/cbmroot/core/qa",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/heinemann/gpuunpacker/cbmroot/external/yaml-cpp/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/core/qa/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmQaBase dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmQaCanvas.h")))  CbmQaCanvas;
class __attribute__((annotate("$clingAutoload$CbmQaEff.h")))  CbmQaEff;
class __attribute__((annotate("$clingAutoload$CbmQaPie.h")))  CbmQaPieSlice;
class __attribute__((annotate("$clingAutoload$CbmQaPie.h")))  CbmQaPie;
class __attribute__((annotate("$clingAutoload$TProfile2D.h")))  __attribute__((annotate("$clingAutoload$CbmQaHist.h")))  TProfile2D;
template <class HistTypeT> class __attribute__((annotate("$clingAutoload$CbmQaHist.h")))  CbmQaHist;

class __attribute__((annotate("$clingAutoload$TProfile.h")))  __attribute__((annotate("$clingAutoload$CbmQaHist.h")))  TProfile;
class __attribute__((annotate("$clingAutoload$TH1.h")))  __attribute__((annotate("$clingAutoload$CbmQaEff.h")))  TH1I;
class __attribute__((annotate("$clingAutoload$TH1.h")))  __attribute__((annotate("$clingAutoload$CbmQaEff.h")))  TH1D;
class __attribute__((annotate("$clingAutoload$TH1.h")))  __attribute__((annotate("$clingAutoload$CbmQaEff.h")))  TH1F;
class __attribute__((annotate("$clingAutoload$CbmQaTable.h")))  CbmQaTable;
class __attribute__((annotate("$clingAutoload$CbmQaTask.h")))  CbmQaTask;
namespace cbm{namespace qa{namespace checker{class __attribute__((annotate("$clingAutoload$CbmQaCheckerObjectDB.h")))  __attribute__((annotate("$clingAutoload$CbmQaCheckerCore.h")))  ObjectDB;}}}
namespace cbm{namespace qa{namespace checker{class __attribute__((annotate("$clingAutoload$CbmQaCheckerCore.h")))  Core;}}}
namespace cbm{namespace qa{namespace checker{class __attribute__((annotate("$clingAutoload$CbmQaCheckerFileHandler.h")))  FileHandler;}}}
namespace cbm{namespace qa{namespace checker{class __attribute__((annotate("$clingAutoload$CbmQaCheckerObjectHandler.h")))  __attribute__((annotate("$clingAutoload$CbmQaCheckerHist1DHandler.h")))  ObjectHandler;}}}
namespace cbm{namespace qa{namespace checker{class __attribute__((annotate("$clingAutoload$CbmQaCheckerHist1DHandler.h")))  Hist1DHandler;}}}
namespace cbm{namespace qa{namespace checker{class __attribute__((annotate("$clingAutoload$CbmQaCheckerHist2DHandler.h")))  Hist2DHandler;}}}
namespace cbm{namespace qa{namespace checker{class __attribute__((annotate("$clingAutoload$CbmQaCheckerProfile1DHandler.h")))  Profile1DHandler;}}}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmQaBase dictionary payload"

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

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "CbmQaConstants.h"
#include "CbmQaCanvas.h"
#include "CbmQaEff.h"
#include "CbmQaPie.h"
#include "CbmQaHist.h"
#include "CbmQaTable.h"
#include "CbmQaTask.h"
#include "CbmQaIO.h"
#include "CbmQaCheckerCore.h"
#include "CbmQaCheckerFileHandler.h"
#include "CbmQaCheckerHist1DHandler.h"
#include "CbmQaCheckerHist2DHandler.h"
#include "CbmQaCheckerObjectHandler.h"
#include "CbmQaCheckerProfile1DHandler.h"
#include "CbmQaCheckerObjectDB.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CbmQaCanvas", payloadCode, "@",
"CbmQaEff", payloadCode, "@",
"CbmQaHist<TH1D>", payloadCode, "@",
"CbmQaHist<TH1F>", payloadCode, "@",
"CbmQaHist<TH1I>", payloadCode, "@",
"CbmQaHist<TProfile2D>", payloadCode, "@",
"CbmQaHist<TProfile>", payloadCode, "@",
"CbmQaPie", payloadCode, "@",
"CbmQaPieSlice", payloadCode, "@",
"CbmQaTable", payloadCode, "@",
"CbmQaTask", payloadCode, "@",
"cbm::qa::checker::Core", payloadCode, "@",
"cbm::qa::checker::FileHandler", payloadCode, "@",
"cbm::qa::checker::Hist1DHandler", payloadCode, "@",
"cbm::qa::checker::Hist2DHandler", payloadCode, "@",
"cbm::qa::checker::ObjectDB", payloadCode, "@",
"cbm::qa::checker::ObjectHandler", payloadCode, "@",
"cbm::qa::checker::Profile1DHandler", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmQaBase",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmQaBase_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmQaBase_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmQaBase() {
  TriggerDictionaryInitialization_libCbmQaBase_Impl();
}
