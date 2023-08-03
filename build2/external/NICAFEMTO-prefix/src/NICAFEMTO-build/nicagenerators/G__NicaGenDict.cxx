// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIheinemanndIgpuunpackerdIcbmrootdIbuild2dIexternaldINICAFEMTOmIprefixdIsrcdINICAFEMTOmIbuilddInicageneratorsdIG__NicaGenDict
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
#include "NicaGeneratorReadAscii.h"
#include "NicaGeneratorReadCF.h"
#include "NicaGeneratorReadTxt.h"
#include "NicaGeneratorReadUrQMD.h"
#include "NicaGeneratorReadTree.h"
#include "NicaGeneratorReadTherminator2.h"
#include "NicaGeneratorParametersArray.h"
#include "NicaGeneratorRead.h"
#include "NicaGeneratorWrite.h"
#include "NicaRunSim.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_NicaGeneratorWrite(void *p);
   static void deleteArray_NicaGeneratorWrite(void *p);
   static void destruct_NicaGeneratorWrite(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaGeneratorWrite*)
   {
      ::NicaGeneratorWrite *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaGeneratorWrite >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaGeneratorWrite", ::NicaGeneratorWrite::Class_Version(), "NicaGeneratorWrite.h", 23,
                  typeid(::NicaGeneratorWrite), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaGeneratorWrite::Dictionary, isa_proxy, 4,
                  sizeof(::NicaGeneratorWrite) );
      instance.SetDelete(&delete_NicaGeneratorWrite);
      instance.SetDeleteArray(&deleteArray_NicaGeneratorWrite);
      instance.SetDestructor(&destruct_NicaGeneratorWrite);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaGeneratorWrite*)
   {
      return GenerateInitInstanceLocal((::NicaGeneratorWrite*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaGeneratorWrite*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaGeneratorRead(void *p = nullptr);
   static void *newArray_NicaGeneratorRead(Long_t size, void *p);
   static void delete_NicaGeneratorRead(void *p);
   static void deleteArray_NicaGeneratorRead(void *p);
   static void destruct_NicaGeneratorRead(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaGeneratorRead*)
   {
      ::NicaGeneratorRead *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaGeneratorRead >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaGeneratorRead", ::NicaGeneratorRead::Class_Version(), "NicaGeneratorRead.h", 21,
                  typeid(::NicaGeneratorRead), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaGeneratorRead::Dictionary, isa_proxy, 4,
                  sizeof(::NicaGeneratorRead) );
      instance.SetNew(&new_NicaGeneratorRead);
      instance.SetNewArray(&newArray_NicaGeneratorRead);
      instance.SetDelete(&delete_NicaGeneratorRead);
      instance.SetDeleteArray(&deleteArray_NicaGeneratorRead);
      instance.SetDestructor(&destruct_NicaGeneratorRead);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaGeneratorRead*)
   {
      return GenerateInitInstanceLocal((::NicaGeneratorRead*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaGeneratorRead*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaGeneratorReadTxt(void *p);
   static void deleteArray_NicaGeneratorReadTxt(void *p);
   static void destruct_NicaGeneratorReadTxt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaGeneratorReadTxt*)
   {
      ::NicaGeneratorReadTxt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaGeneratorReadTxt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaGeneratorReadTxt", ::NicaGeneratorReadTxt::Class_Version(), "NicaGeneratorReadTxt.h", 21,
                  typeid(::NicaGeneratorReadTxt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaGeneratorReadTxt::Dictionary, isa_proxy, 4,
                  sizeof(::NicaGeneratorReadTxt) );
      instance.SetDelete(&delete_NicaGeneratorReadTxt);
      instance.SetDeleteArray(&deleteArray_NicaGeneratorReadTxt);
      instance.SetDestructor(&destruct_NicaGeneratorReadTxt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaGeneratorReadTxt*)
   {
      return GenerateInitInstanceLocal((::NicaGeneratorReadTxt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaGeneratorReadTxt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaGeneratorReadAscii(void *p);
   static void deleteArray_NicaGeneratorReadAscii(void *p);
   static void destruct_NicaGeneratorReadAscii(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaGeneratorReadAscii*)
   {
      ::NicaGeneratorReadAscii *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaGeneratorReadAscii >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaGeneratorReadAscii", ::NicaGeneratorReadAscii::Class_Version(), "NicaGeneratorReadAscii.h", 16,
                  typeid(::NicaGeneratorReadAscii), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaGeneratorReadAscii::Dictionary, isa_proxy, 4,
                  sizeof(::NicaGeneratorReadAscii) );
      instance.SetDelete(&delete_NicaGeneratorReadAscii);
      instance.SetDeleteArray(&deleteArray_NicaGeneratorReadAscii);
      instance.SetDestructor(&destruct_NicaGeneratorReadAscii);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaGeneratorReadAscii*)
   {
      return GenerateInitInstanceLocal((::NicaGeneratorReadAscii*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaGeneratorReadAscii*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaGeneratorReadTree(void *p);
   static void deleteArray_NicaGeneratorReadTree(void *p);
   static void destruct_NicaGeneratorReadTree(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaGeneratorReadTree*)
   {
      ::NicaGeneratorReadTree *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaGeneratorReadTree >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaGeneratorReadTree", ::NicaGeneratorReadTree::Class_Version(), "NicaGeneratorReadTree.h", 22,
                  typeid(::NicaGeneratorReadTree), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaGeneratorReadTree::Dictionary, isa_proxy, 4,
                  sizeof(::NicaGeneratorReadTree) );
      instance.SetDelete(&delete_NicaGeneratorReadTree);
      instance.SetDeleteArray(&deleteArray_NicaGeneratorReadTree);
      instance.SetDestructor(&destruct_NicaGeneratorReadTree);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaGeneratorReadTree*)
   {
      return GenerateInitInstanceLocal((::NicaGeneratorReadTree*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaGeneratorReadTree*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaGeneratorReadCF(void *p);
   static void deleteArray_NicaGeneratorReadCF(void *p);
   static void destruct_NicaGeneratorReadCF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaGeneratorReadCF*)
   {
      ::NicaGeneratorReadCF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaGeneratorReadCF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaGeneratorReadCF", ::NicaGeneratorReadCF::Class_Version(), "NicaGeneratorReadCF.h", 15,
                  typeid(::NicaGeneratorReadCF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaGeneratorReadCF::Dictionary, isa_proxy, 4,
                  sizeof(::NicaGeneratorReadCF) );
      instance.SetDelete(&delete_NicaGeneratorReadCF);
      instance.SetDeleteArray(&deleteArray_NicaGeneratorReadCF);
      instance.SetDestructor(&destruct_NicaGeneratorReadCF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaGeneratorReadCF*)
   {
      return GenerateInitInstanceLocal((::NicaGeneratorReadCF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaGeneratorReadCF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaGeneratorReadUrQMD(void *p);
   static void deleteArray_NicaGeneratorReadUrQMD(void *p);
   static void destruct_NicaGeneratorReadUrQMD(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaGeneratorReadUrQMD*)
   {
      ::NicaGeneratorReadUrQMD *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaGeneratorReadUrQMD >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaGeneratorReadUrQMD", ::NicaGeneratorReadUrQMD::Class_Version(), "NicaGeneratorReadUrQMD.h", 16,
                  typeid(::NicaGeneratorReadUrQMD), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaGeneratorReadUrQMD::Dictionary, isa_proxy, 4,
                  sizeof(::NicaGeneratorReadUrQMD) );
      instance.SetDelete(&delete_NicaGeneratorReadUrQMD);
      instance.SetDeleteArray(&deleteArray_NicaGeneratorReadUrQMD);
      instance.SetDestructor(&destruct_NicaGeneratorReadUrQMD);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaGeneratorReadUrQMD*)
   {
      return GenerateInitInstanceLocal((::NicaGeneratorReadUrQMD*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaGeneratorReadUrQMD*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NicaGeneratorReadTherminator2(void *p);
   static void deleteArray_NicaGeneratorReadTherminator2(void *p);
   static void destruct_NicaGeneratorReadTherminator2(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaGeneratorReadTherminator2*)
   {
      ::NicaGeneratorReadTherminator2 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaGeneratorReadTherminator2 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaGeneratorReadTherminator2", ::NicaGeneratorReadTherminator2::Class_Version(), "NicaGeneratorReadTherminator2.h", 17,
                  typeid(::NicaGeneratorReadTherminator2), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaGeneratorReadTherminator2::Dictionary, isa_proxy, 4,
                  sizeof(::NicaGeneratorReadTherminator2) );
      instance.SetDelete(&delete_NicaGeneratorReadTherminator2);
      instance.SetDeleteArray(&deleteArray_NicaGeneratorReadTherminator2);
      instance.SetDestructor(&destruct_NicaGeneratorReadTherminator2);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaGeneratorReadTherminator2*)
   {
      return GenerateInitInstanceLocal((::NicaGeneratorReadTherminator2*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaGeneratorReadTherminator2*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaGeneratorParametersArray(void *p = nullptr);
   static void *newArray_NicaGeneratorParametersArray(Long_t size, void *p);
   static void delete_NicaGeneratorParametersArray(void *p);
   static void deleteArray_NicaGeneratorParametersArray(void *p);
   static void destruct_NicaGeneratorParametersArray(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaGeneratorParametersArray*)
   {
      ::NicaGeneratorParametersArray *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaGeneratorParametersArray >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaGeneratorParametersArray", ::NicaGeneratorParametersArray::Class_Version(), "NicaGeneratorParametersArray.h", 20,
                  typeid(::NicaGeneratorParametersArray), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaGeneratorParametersArray::Dictionary, isa_proxy, 4,
                  sizeof(::NicaGeneratorParametersArray) );
      instance.SetNew(&new_NicaGeneratorParametersArray);
      instance.SetNewArray(&newArray_NicaGeneratorParametersArray);
      instance.SetDelete(&delete_NicaGeneratorParametersArray);
      instance.SetDeleteArray(&deleteArray_NicaGeneratorParametersArray);
      instance.SetDestructor(&destruct_NicaGeneratorParametersArray);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaGeneratorParametersArray*)
   {
      return GenerateInitInstanceLocal((::NicaGeneratorParametersArray*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaGeneratorParametersArray*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NicaRunSim(void *p = nullptr);
   static void *newArray_NicaRunSim(Long_t size, void *p);
   static void delete_NicaRunSim(void *p);
   static void deleteArray_NicaRunSim(void *p);
   static void destruct_NicaRunSim(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NicaRunSim*)
   {
      ::NicaRunSim *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NicaRunSim >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("NicaRunSim", ::NicaRunSim::Class_Version(), "NicaRunSim.h", 25,
                  typeid(::NicaRunSim), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NicaRunSim::Dictionary, isa_proxy, 4,
                  sizeof(::NicaRunSim) );
      instance.SetNew(&new_NicaRunSim);
      instance.SetNewArray(&newArray_NicaRunSim);
      instance.SetDelete(&delete_NicaRunSim);
      instance.SetDeleteArray(&deleteArray_NicaRunSim);
      instance.SetDestructor(&destruct_NicaRunSim);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NicaRunSim*)
   {
      return GenerateInitInstanceLocal((::NicaRunSim*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NicaRunSim*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NicaGeneratorWrite::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaGeneratorWrite::Class_Name()
{
   return "NicaGeneratorWrite";
}

//______________________________________________________________________________
const char *NicaGeneratorWrite::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorWrite*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaGeneratorWrite::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorWrite*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaGeneratorWrite::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorWrite*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaGeneratorWrite::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorWrite*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaGeneratorRead::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaGeneratorRead::Class_Name()
{
   return "NicaGeneratorRead";
}

//______________________________________________________________________________
const char *NicaGeneratorRead::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorRead*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaGeneratorRead::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorRead*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaGeneratorRead::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorRead*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaGeneratorRead::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorRead*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaGeneratorReadTxt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaGeneratorReadTxt::Class_Name()
{
   return "NicaGeneratorReadTxt";
}

//______________________________________________________________________________
const char *NicaGeneratorReadTxt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadTxt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaGeneratorReadTxt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadTxt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaGeneratorReadTxt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadTxt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaGeneratorReadTxt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadTxt*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaGeneratorReadAscii::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaGeneratorReadAscii::Class_Name()
{
   return "NicaGeneratorReadAscii";
}

//______________________________________________________________________________
const char *NicaGeneratorReadAscii::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadAscii*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaGeneratorReadAscii::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadAscii*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaGeneratorReadAscii::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadAscii*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaGeneratorReadAscii::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadAscii*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaGeneratorReadTree::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaGeneratorReadTree::Class_Name()
{
   return "NicaGeneratorReadTree";
}

//______________________________________________________________________________
const char *NicaGeneratorReadTree::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadTree*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaGeneratorReadTree::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadTree*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaGeneratorReadTree::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadTree*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaGeneratorReadTree::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadTree*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaGeneratorReadCF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaGeneratorReadCF::Class_Name()
{
   return "NicaGeneratorReadCF";
}

//______________________________________________________________________________
const char *NicaGeneratorReadCF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadCF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaGeneratorReadCF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadCF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaGeneratorReadCF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadCF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaGeneratorReadCF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadCF*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaGeneratorReadUrQMD::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaGeneratorReadUrQMD::Class_Name()
{
   return "NicaGeneratorReadUrQMD";
}

//______________________________________________________________________________
const char *NicaGeneratorReadUrQMD::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadUrQMD*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaGeneratorReadUrQMD::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadUrQMD*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaGeneratorReadUrQMD::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadUrQMD*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaGeneratorReadUrQMD::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadUrQMD*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaGeneratorReadTherminator2::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaGeneratorReadTherminator2::Class_Name()
{
   return "NicaGeneratorReadTherminator2";
}

//______________________________________________________________________________
const char *NicaGeneratorReadTherminator2::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadTherminator2*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaGeneratorReadTherminator2::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadTherminator2*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaGeneratorReadTherminator2::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadTherminator2*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaGeneratorReadTherminator2::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorReadTherminator2*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaGeneratorParametersArray::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaGeneratorParametersArray::Class_Name()
{
   return "NicaGeneratorParametersArray";
}

//______________________________________________________________________________
const char *NicaGeneratorParametersArray::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorParametersArray*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaGeneratorParametersArray::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorParametersArray*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaGeneratorParametersArray::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorParametersArray*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaGeneratorParametersArray::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaGeneratorParametersArray*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NicaRunSim::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *NicaRunSim::Class_Name()
{
   return "NicaRunSim";
}

//______________________________________________________________________________
const char *NicaRunSim::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaRunSim*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int NicaRunSim::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NicaRunSim*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NicaRunSim::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaRunSim*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NicaRunSim::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NicaRunSim*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void NicaGeneratorWrite::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaGeneratorWrite.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaGeneratorWrite::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaGeneratorWrite::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaGeneratorWrite(void *p) {
      delete ((::NicaGeneratorWrite*)p);
   }
   static void deleteArray_NicaGeneratorWrite(void *p) {
      delete [] ((::NicaGeneratorWrite*)p);
   }
   static void destruct_NicaGeneratorWrite(void *p) {
      typedef ::NicaGeneratorWrite current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaGeneratorWrite

//______________________________________________________________________________
void NicaGeneratorRead::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaGeneratorRead.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaGeneratorRead::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaGeneratorRead::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaGeneratorRead(void *p) {
      return  p ? new(p) ::NicaGeneratorRead : new ::NicaGeneratorRead;
   }
   static void *newArray_NicaGeneratorRead(Long_t nElements, void *p) {
      return p ? new(p) ::NicaGeneratorRead[nElements] : new ::NicaGeneratorRead[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaGeneratorRead(void *p) {
      delete ((::NicaGeneratorRead*)p);
   }
   static void deleteArray_NicaGeneratorRead(void *p) {
      delete [] ((::NicaGeneratorRead*)p);
   }
   static void destruct_NicaGeneratorRead(void *p) {
      typedef ::NicaGeneratorRead current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaGeneratorRead

//______________________________________________________________________________
void NicaGeneratorReadTxt::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaGeneratorReadTxt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaGeneratorReadTxt::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaGeneratorReadTxt::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaGeneratorReadTxt(void *p) {
      delete ((::NicaGeneratorReadTxt*)p);
   }
   static void deleteArray_NicaGeneratorReadTxt(void *p) {
      delete [] ((::NicaGeneratorReadTxt*)p);
   }
   static void destruct_NicaGeneratorReadTxt(void *p) {
      typedef ::NicaGeneratorReadTxt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaGeneratorReadTxt

//______________________________________________________________________________
void NicaGeneratorReadAscii::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaGeneratorReadAscii.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaGeneratorReadAscii::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaGeneratorReadAscii::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaGeneratorReadAscii(void *p) {
      delete ((::NicaGeneratorReadAscii*)p);
   }
   static void deleteArray_NicaGeneratorReadAscii(void *p) {
      delete [] ((::NicaGeneratorReadAscii*)p);
   }
   static void destruct_NicaGeneratorReadAscii(void *p) {
      typedef ::NicaGeneratorReadAscii current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaGeneratorReadAscii

//______________________________________________________________________________
void NicaGeneratorReadTree::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaGeneratorReadTree.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaGeneratorReadTree::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaGeneratorReadTree::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaGeneratorReadTree(void *p) {
      delete ((::NicaGeneratorReadTree*)p);
   }
   static void deleteArray_NicaGeneratorReadTree(void *p) {
      delete [] ((::NicaGeneratorReadTree*)p);
   }
   static void destruct_NicaGeneratorReadTree(void *p) {
      typedef ::NicaGeneratorReadTree current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaGeneratorReadTree

//______________________________________________________________________________
void NicaGeneratorReadCF::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaGeneratorReadCF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaGeneratorReadCF::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaGeneratorReadCF::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaGeneratorReadCF(void *p) {
      delete ((::NicaGeneratorReadCF*)p);
   }
   static void deleteArray_NicaGeneratorReadCF(void *p) {
      delete [] ((::NicaGeneratorReadCF*)p);
   }
   static void destruct_NicaGeneratorReadCF(void *p) {
      typedef ::NicaGeneratorReadCF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaGeneratorReadCF

//______________________________________________________________________________
void NicaGeneratorReadUrQMD::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaGeneratorReadUrQMD.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaGeneratorReadUrQMD::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaGeneratorReadUrQMD::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaGeneratorReadUrQMD(void *p) {
      delete ((::NicaGeneratorReadUrQMD*)p);
   }
   static void deleteArray_NicaGeneratorReadUrQMD(void *p) {
      delete [] ((::NicaGeneratorReadUrQMD*)p);
   }
   static void destruct_NicaGeneratorReadUrQMD(void *p) {
      typedef ::NicaGeneratorReadUrQMD current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaGeneratorReadUrQMD

//______________________________________________________________________________
void NicaGeneratorReadTherminator2::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaGeneratorReadTherminator2.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaGeneratorReadTherminator2::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaGeneratorReadTherminator2::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NicaGeneratorReadTherminator2(void *p) {
      delete ((::NicaGeneratorReadTherminator2*)p);
   }
   static void deleteArray_NicaGeneratorReadTherminator2(void *p) {
      delete [] ((::NicaGeneratorReadTherminator2*)p);
   }
   static void destruct_NicaGeneratorReadTherminator2(void *p) {
      typedef ::NicaGeneratorReadTherminator2 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaGeneratorReadTherminator2

//______________________________________________________________________________
void NicaGeneratorParametersArray::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaGeneratorParametersArray.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaGeneratorParametersArray::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaGeneratorParametersArray::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaGeneratorParametersArray(void *p) {
      return  p ? new(p) ::NicaGeneratorParametersArray : new ::NicaGeneratorParametersArray;
   }
   static void *newArray_NicaGeneratorParametersArray(Long_t nElements, void *p) {
      return p ? new(p) ::NicaGeneratorParametersArray[nElements] : new ::NicaGeneratorParametersArray[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaGeneratorParametersArray(void *p) {
      delete ((::NicaGeneratorParametersArray*)p);
   }
   static void deleteArray_NicaGeneratorParametersArray(void *p) {
      delete [] ((::NicaGeneratorParametersArray*)p);
   }
   static void destruct_NicaGeneratorParametersArray(void *p) {
      typedef ::NicaGeneratorParametersArray current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaGeneratorParametersArray

//______________________________________________________________________________
void NicaRunSim::Streamer(TBuffer &R__b)
{
   // Stream an object of class NicaRunSim.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NicaRunSim::Class(),this);
   } else {
      R__b.WriteClassBuffer(NicaRunSim::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NicaRunSim(void *p) {
      return  p ? new(p) ::NicaRunSim : new ::NicaRunSim;
   }
   static void *newArray_NicaRunSim(Long_t nElements, void *p) {
      return p ? new(p) ::NicaRunSim[nElements] : new ::NicaRunSim[nElements];
   }
   // Wrapper around operator delete
   static void delete_NicaRunSim(void *p) {
      delete ((::NicaRunSim*)p);
   }
   static void deleteArray_NicaRunSim(void *p) {
      delete [] ((::NicaRunSim*)p);
   }
   static void destruct_NicaRunSim(void *p) {
      typedef ::NicaRunSim current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NicaRunSim

namespace {
  void TriggerDictionaryInitialization_G__NicaGenDict_Impl() {
    static const char* headers[] = {
"NicaGeneratorReadAscii.h",
"NicaGeneratorReadCF.h",
"NicaGeneratorReadTxt.h",
"NicaGeneratorReadUrQMD.h",
"NicaGeneratorReadTree.h",
"NicaGeneratorReadTherminator2.h",
"NicaGeneratorParametersArray.h",
"NicaGeneratorRead.h",
"NicaGeneratorWrite.h",
"NicaRunSim.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/detector",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/compound",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/hiddeninfo",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/nicagenerators",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/nicagenerators/readers",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/hist",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io/html",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/std",
"/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/minimizer",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairroot_v18.6.9/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/cbmdata/_deps/fairsoft_apr22/install/include",
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/nicagenerators/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__NicaGenDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$NicaGeneratorWrite.h")))  __attribute__((annotate("$clingAutoload$NicaGeneratorReadAscii.h")))  NicaGeneratorWrite;
class __attribute__((annotate("$clingAutoload$NicaGeneratorRead.h")))  __attribute__((annotate("$clingAutoload$NicaGeneratorReadAscii.h")))  NicaGeneratorRead;
class __attribute__((annotate("$clingAutoload$NicaGeneratorReadTxt.h")))  __attribute__((annotate("$clingAutoload$NicaGeneratorReadAscii.h")))  NicaGeneratorReadTxt;
class __attribute__((annotate("$clingAutoload$NicaGeneratorReadAscii.h")))  NicaGeneratorReadAscii;
class __attribute__((annotate("$clingAutoload$NicaGeneratorReadTree.h")))  __attribute__((annotate("$clingAutoload$NicaGeneratorReadCF.h")))  NicaGeneratorReadTree;
class __attribute__((annotate("$clingAutoload$NicaGeneratorReadCF.h")))  NicaGeneratorReadCF;
class __attribute__((annotate("$clingAutoload$NicaGeneratorReadUrQMD.h")))  NicaGeneratorReadUrQMD;
class __attribute__((annotate("$clingAutoload$NicaGeneratorReadTherminator2.h")))  NicaGeneratorReadTherminator2;
class __attribute__((annotate("$clingAutoload$NicaGeneratorParametersArray.h")))  NicaGeneratorParametersArray;
class __attribute__((annotate("$clingAutoload$NicaRunSim.h")))  NicaRunSim;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__NicaGenDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "NicaGeneratorReadAscii.h"
#include "NicaGeneratorReadCF.h"
#include "NicaGeneratorReadTxt.h"
#include "NicaGeneratorReadUrQMD.h"
#include "NicaGeneratorReadTree.h"
#include "NicaGeneratorReadTherminator2.h"
#include "NicaGeneratorParametersArray.h"
#include "NicaGeneratorRead.h"
#include "NicaGeneratorWrite.h"
#include "NicaRunSim.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"NicaGeneratorParametersArray", payloadCode, "@",
"NicaGeneratorRead", payloadCode, "@",
"NicaGeneratorReadAscii", payloadCode, "@",
"NicaGeneratorReadCF", payloadCode, "@",
"NicaGeneratorReadTherminator2", payloadCode, "@",
"NicaGeneratorReadTree", payloadCode, "@",
"NicaGeneratorReadTxt", payloadCode, "@",
"NicaGeneratorReadUrQMD", payloadCode, "@",
"NicaGeneratorWrite", payloadCode, "@",
"NicaRunSim", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__NicaGenDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__NicaGenDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__NicaGenDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__NicaGenDict() {
  TriggerDictionaryInitialization_G__NicaGenDict_Impl();
}
