// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIheinemanndIgpuunpackerdIcbmrootdIbuild2dIexternaldINICAFEMTOmIprefixdIsrcdINICAFEMTOmIbuilddIanalysisdIqadIG__NicaQADict
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
#include "NicaQAPlot.h"
#include "NicaQAPlotReport.h"
#include "NicaQAEventTask.h"
#include "NicaQATrackTask.h"
#include "NicaQAHistoSettings.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_NicaQAHistoSettings(void *p = nullptr);
   static void *newArray_NicaQAHistoSettings(Long_t size, void *p);
   static void delete_NicaQAHistoSettings(void *p);
   static void deleteArray_NicaQAHistoSettings(void *p);
   static void destruct_NicaQAHistoSettings(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaQAHistoSettings*)
   {
      ::NicaQAHistoSettings *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaQAHistoSettings >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaQAHistoSettings", ::NicaQAHistoSettings::Class_Version(), "NicaQAHistoSettings.h", 19,
                  typeid(::NicaQAHistoSettings), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaQAHistoSettings::Dictionary, isa_proxy, 4,
                  sizeof(::NicaQAHistoSettings) );
      instance.SetNew(&new_NicaQAHistoSettings);
      instance.SetNewArray(&newArray_NicaQAHistoSettings);
      instance.SetDelete(&delete_NicaQAHistoSettings);
      instance.SetDeleteArray(&deleteArray_NicaQAHistoSettings);
      instance.SetDestructor(&destruct_NicaQAHistoSettings);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaQAHistoSettings*)
   {
      return GenerateInitInstanceLocal((::NicaQAHistoSettings*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaQAHistoSettings*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaQAPlotReport(void *p = nullptr);
   static void *newArray_NicaQAPlotReport(Long_t size, void *p);
   static void delete_NicaQAPlotReport(void *p);
   static void deleteArray_NicaQAPlotReport(void *p);
   static void destruct_NicaQAPlotReport(void *p);
   static Long64_t merge_NicaQAPlotReport(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaQAPlotReport*)
   {
      ::NicaQAPlotReport *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaQAPlotReport >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaQAPlotReport", ::NicaQAPlotReport::Class_Version(), "NicaQAPlotReport.h", 32,
                  typeid(::NicaQAPlotReport), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaQAPlotReport::Dictionary, isa_proxy, 4,
                  sizeof(::NicaQAPlotReport) );
      instance.SetNew(&new_NicaQAPlotReport);
      instance.SetNewArray(&newArray_NicaQAPlotReport);
      instance.SetDelete(&delete_NicaQAPlotReport);
      instance.SetDeleteArray(&deleteArray_NicaQAPlotReport);
      instance.SetDestructor(&destruct_NicaQAPlotReport);
      instance.SetMerge(&merge_NicaQAPlotReport);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaQAPlotReport*)
   {
      return GenerateInitInstanceLocal((::NicaQAPlotReport*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaQAPlotReport*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaQAPlot(void *p = nullptr);
   static void *newArray_NicaQAPlot(Long_t size, void *p);
   static void delete_NicaQAPlot(void *p);
   static void deleteArray_NicaQAPlot(void *p);
   static void destruct_NicaQAPlot(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaQAPlot*)
   {
      ::NicaQAPlot *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaQAPlot >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaQAPlot", ::NicaQAPlot::Class_Version(), "NicaQAPlot.h", 30,
                  typeid(::NicaQAPlot), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaQAPlot::Dictionary, isa_proxy, 4,
                  sizeof(::NicaQAPlot) );
      instance.SetNew(&new_NicaQAPlot);
      instance.SetNewArray(&newArray_NicaQAPlot);
      instance.SetDelete(&delete_NicaQAPlot);
      instance.SetDeleteArray(&deleteArray_NicaQAPlot);
      instance.SetDestructor(&destruct_NicaQAPlot);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaQAPlot*)
   {
      return GenerateInitInstanceLocal((::NicaQAPlot*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaQAPlot*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaQAEventTask(void *p = nullptr);
   static void *newArray_NicaQAEventTask(Long_t size, void *p);
   static void delete_NicaQAEventTask(void *p);
   static void deleteArray_NicaQAEventTask(void *p);
   static void destruct_NicaQAEventTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaQAEventTask*)
   {
      ::NicaQAEventTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaQAEventTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaQAEventTask", ::NicaQAEventTask::Class_Version(), "NicaQAEventTask.h", 23,
                  typeid(::NicaQAEventTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaQAEventTask::Dictionary, isa_proxy, 4,
                  sizeof(::NicaQAEventTask) );
      instance.SetNew(&new_NicaQAEventTask);
      instance.SetNewArray(&newArray_NicaQAEventTask);
      instance.SetDelete(&delete_NicaQAEventTask);
      instance.SetDeleteArray(&deleteArray_NicaQAEventTask);
      instance.SetDestructor(&destruct_NicaQAEventTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaQAEventTask*)
   {
      return GenerateInitInstanceLocal((::NicaQAEventTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaQAEventTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaQATrackTask(void *p = nullptr);
   static void *newArray_NicaQATrackTask(Long_t size, void *p);
   static void delete_NicaQATrackTask(void *p);
   static void deleteArray_NicaQATrackTask(void *p);
   static void destruct_NicaQATrackTask(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaQATrackTask*)
   {
      ::NicaQATrackTask *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaQATrackTask >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaQATrackTask", ::NicaQATrackTask::Class_Version(), "NicaQATrackTask.h", 21,
                  typeid(::NicaQATrackTask), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaQATrackTask::Dictionary, isa_proxy, 4,
                  sizeof(::NicaQATrackTask) );
      instance.SetNew(&new_NicaQATrackTask);
      instance.SetNewArray(&newArray_NicaQATrackTask);
      instance.SetDelete(&delete_NicaQATrackTask);
      instance.SetDeleteArray(&deleteArray_NicaQATrackTask);
      instance.SetDestructor(&destruct_NicaQATrackTask);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaQATrackTask*)
   {
      return GenerateInitInstanceLocal((::NicaQATrackTask*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaQATrackTask*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NicaQAHistoSettings::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaQAHistoSettings::Class_Name()
{
   return "NicaQAHistoSettings";
}

//______________________________________________________________________________
const char *NicaQAHistoSettings::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaQAHistoSettings*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaQAHistoSettings::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaQAHistoSettings*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaQAHistoSettings::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaQAHistoSettings*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaQAHistoSettings::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaQAHistoSettings*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaQAPlotReport::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaQAPlotReport::Class_Name()
{
   return "NicaQAPlotReport";
}

//______________________________________________________________________________
const char *NicaQAPlotReport::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaQAPlotReport*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaQAPlotReport::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaQAPlotReport*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaQAPlotReport::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaQAPlotReport*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaQAPlotReport::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaQAPlotReport*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaQAPlot::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaQAPlot::Class_Name()
{
   return "NicaQAPlot";
}

//______________________________________________________________________________
const char *NicaQAPlot::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaQAPlot*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaQAPlot::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaQAPlot*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaQAPlot::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaQAPlot*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaQAPlot::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaQAPlot*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaQAEventTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaQAEventTask::Class_Name()
{
   return "NicaQAEventTask";
}

//______________________________________________________________________________
const char *NicaQAEventTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaQAEventTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaQAEventTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaQAEventTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaQAEventTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaQAEventTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaQAEventTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaQAEventTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaQATrackTask::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaQATrackTask::Class_Name()
{
   return "NicaQATrackTask";
}

//______________________________________________________________________________
const char *NicaQATrackTask::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaQATrackTask*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaQATrackTask::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaQATrackTask*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaQATrackTask::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaQATrackTask*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaQATrackTask::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaQATrackTask*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void NicaQAHistoSettings::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaQAHistoSettings.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaQAHistoSettings::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaQAHistoSettings::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaQAHistoSettings(void *p) {
      return  p ? new(p) ::NicaQAHistoSettings : new ::NicaQAHistoSettings;
   }
   static void *newArray_NicaQAHistoSettings(Long_t nElements, void *p) {
      return p ? new(p) ::NicaQAHistoSettings[nElements] : new ::NicaQAHistoSettings[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaQAHistoSettings(void *p) {
      delete ((::NicaQAHistoSettings*)p);
   }
   static void deleteArray_NicaQAHistoSettings(void *p) {
      delete [] ((::NicaQAHistoSettings*)p);
   }
   static void destruct_NicaQAHistoSettings(void *p) {
      typedef ::NicaQAHistoSettings current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaQAHistoSettings

//______________________________________________________________________________
void NicaQAPlotReport::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaQAPlotReport.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaQAPlotReport::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaQAPlotReport::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaQAPlotReport(void *p) {
      return  p ? new(p) ::NicaQAPlotReport : new ::NicaQAPlotReport;
   }
   static void *newArray_NicaQAPlotReport(Long_t nElements, void *p) {
      return p ? new(p) ::NicaQAPlotReport[nElements] : new ::NicaQAPlotReport[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaQAPlotReport(void *p) {
      delete ((::NicaQAPlotReport*)p);
   }
   static void deleteArray_NicaQAPlotReport(void *p) {
      delete [] ((::NicaQAPlotReport*)p);
   }
   static void destruct_NicaQAPlotReport(void *p) {
      typedef ::NicaQAPlotReport current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the merge function.
   static Long64_t  merge_NicaQAPlotReport(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::NicaQAPlotReport*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::NicaQAPlotReport

//______________________________________________________________________________
void NicaQAPlot::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaQAPlot.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaQAPlot::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaQAPlot::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaQAPlot(void *p) {
      return  p ? new(p) ::NicaQAPlot : new ::NicaQAPlot;
   }
   static void *newArray_NicaQAPlot(Long_t nElements, void *p) {
      return p ? new(p) ::NicaQAPlot[nElements] : new ::NicaQAPlot[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaQAPlot(void *p) {
      delete ((::NicaQAPlot*)p);
   }
   static void deleteArray_NicaQAPlot(void *p) {
      delete [] ((::NicaQAPlot*)p);
   }
   static void destruct_NicaQAPlot(void *p) {
      typedef ::NicaQAPlot current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaQAPlot

//______________________________________________________________________________
void NicaQAEventTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaQAEventTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaQAEventTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaQAEventTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaQAEventTask(void *p) {
      return  p ? new(p) ::NicaQAEventTask : new ::NicaQAEventTask;
   }
   static void *newArray_NicaQAEventTask(Long_t nElements, void *p) {
      return p ? new(p) ::NicaQAEventTask[nElements] : new ::NicaQAEventTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaQAEventTask(void *p) {
      delete ((::NicaQAEventTask*)p);
   }
   static void deleteArray_NicaQAEventTask(void *p) {
      delete [] ((::NicaQAEventTask*)p);
   }
   static void destruct_NicaQAEventTask(void *p) {
      typedef ::NicaQAEventTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaQAEventTask

//______________________________________________________________________________
void NicaQATrackTask::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaQATrackTask.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaQATrackTask::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaQATrackTask::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaQATrackTask(void *p) {
      return  p ? new(p) ::NicaQATrackTask : new ::NicaQATrackTask;
   }
   static void *newArray_NicaQATrackTask(Long_t nElements, void *p) {
      return p ? new(p) ::NicaQATrackTask[nElements] : new ::NicaQATrackTask[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaQATrackTask(void *p) {
      delete ((::NicaQATrackTask*)p);
   }
   static void deleteArray_NicaQATrackTask(void *p) {
      delete [] ((::NicaQATrackTask*)p);
   }
   static void destruct_NicaQATrackTask(void *p) {
      typedef ::NicaQATrackTask current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaQATrackTask

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

namespace ROOT {
   static TClass *vectorlENicaQAHistoSettingsgR_Dictionary();
   static void vectorlENicaQAHistoSettingsgR_TClassManip(TClass*);
   static void *new_vectorlENicaQAHistoSettingsgR(void *p = nullptr);
   static void *newArray_vectorlENicaQAHistoSettingsgR(Long_t size, void *p);
   static void delete_vectorlENicaQAHistoSettingsgR(void *p);
   static void deleteArray_vectorlENicaQAHistoSettingsgR(void *p);
   static void destruct_vectorlENicaQAHistoSettingsgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<NicaQAHistoSettings>*)
   {
      vector<NicaQAHistoSettings> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<NicaQAHistoSettings>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<NicaQAHistoSettings>", -2, "vector", 389,
                  typeid(vector<NicaQAHistoSettings>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlENicaQAHistoSettingsgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<NicaQAHistoSettings>) );
      instance.SetNew(&new_vectorlENicaQAHistoSettingsgR);
      instance.SetNewArray(&newArray_vectorlENicaQAHistoSettingsgR);
      instance.SetDelete(&delete_vectorlENicaQAHistoSettingsgR);
      instance.SetDeleteArray(&deleteArray_vectorlENicaQAHistoSettingsgR);
      instance.SetDestructor(&destruct_vectorlENicaQAHistoSettingsgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<NicaQAHistoSettings> >()));

      ::ROOT::AddClassAlternate("vector<NicaQAHistoSettings>","std::vector<NicaQAHistoSettings, std::allocator<NicaQAHistoSettings> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<NicaQAHistoSettings>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlENicaQAHistoSettingsgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<NicaQAHistoSettings>*)nullptr)->GetClass();
      vectorlENicaQAHistoSettingsgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlENicaQAHistoSettingsgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlENicaQAHistoSettingsgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<NicaQAHistoSettings> : new vector<NicaQAHistoSettings>;
   }
   static void *newArray_vectorlENicaQAHistoSettingsgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<NicaQAHistoSettings>[nElements] : new vector<NicaQAHistoSettings>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlENicaQAHistoSettingsgR(void *p) {
      delete ((vector<NicaQAHistoSettings>*)p);
   }
   static void deleteArray_vectorlENicaQAHistoSettingsgR(void *p) {
      delete [] ((vector<NicaQAHistoSettings>*)p);
   }
   static void destruct_vectorlENicaQAHistoSettingsgR(void *p) {
      typedef vector<NicaQAHistoSettings> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<NicaQAHistoSettings>

namespace {
  void TriggerDictionaryInitialization_G__NicaQADict_Impl() {
    static const char* headers[] = {
"NicaQAPlot.h",
"NicaQAPlotReport.h",
"NicaQAEventTask.h",
"NicaQATrackTask.h",
"NicaQAHistoSettings.h",
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
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/qa",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/analysis/qa/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__NicaQADict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$NicaQAHistoSettings.h")))  __attribute__((annotate("$clingAutoload$NicaQAPlot.h")))  NicaQAHistoSettings;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$NicaQAPlotReport.h")))  __attribute__((annotate("$clingAutoload$NicaQAPlot.h")))  NicaQAPlotReport;
class __attribute__((annotate("$clingAutoload$NicaQAPlot.h")))  NicaQAPlot;
class __attribute__((annotate("$clingAutoload$NicaQAEventTask.h")))  NicaQAEventTask;
class __attribute__((annotate("$clingAutoload$NicaQATrackTask.h")))  NicaQATrackTask;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__NicaQADict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "NicaQAPlot.h"
#include "NicaQAPlotReport.h"
#include "NicaQAEventTask.h"
#include "NicaQATrackTask.h"
#include "NicaQAHistoSettings.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"NicaQAEventTask", payloadCode, "@",
"NicaQAHistoSettings", payloadCode, "@",
"NicaQAPlot", payloadCode, "@",
"NicaQAPlotReport", payloadCode, "@",
"NicaQATrackTask", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__NicaQADict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__NicaQADict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__NicaQADict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__NicaQADict() {
  TriggerDictionaryInitialization_G__NicaQADict_Impl();
}
